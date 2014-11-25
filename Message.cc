/////////////////////////////////////////////////////////////////////
//
// Message.cc
//
/////////////////////////////////////////////////////////////////////

#include <iostream>

#include "Logger.h"
#include "Message.h"
#include "ConvertFromBinary.h"
#include "ConvertToBinary.h"
#include "ConvertBinary.h"
#include "Exception.h"

using namespace std;

/////////////////////////////////////////////////////////////////////

Message::Message()
{
	clear();
}

Message::Message( const Message & msg )
{
	clear();
	*this = msg;
}

Message::Message( Message && msg )
{
	clear();
	*this = msg;
}

Message & Message::operator = ( const Message & msg )
{
	m_type = msg.m_type;
	m_messageId = msg.m_messageId;
	m_sequenceNum = msg.m_sequenceNum;
	m_srcAddress = msg.m_srcAddress;
	m_dstAddress = msg.m_dstAddress;
	m_dataMap = msg.m_dataMap;
}

Message & Message::operator = ( Message && msg )
{
	m_type = std::move( msg.m_type );
	m_messageId = std::move( msg.m_messageId );
	m_sequenceNum = std::move( msg.m_sequenceNum );
	m_srcAddress = std::move( msg.m_srcAddress );
	m_dstAddress = std::move( msg.m_dstAddress );
	m_dataMap = std::move( msg.m_dataMap );
}

void Message::clear()
{
	m_type = 0;
	m_messageId = 0;
	m_sequenceNum = 0;
	m_srcAddress="";
	m_dstAddress="";
	m_dataMap.clear();
}

string Message::buildUrl( const std::string & address, const std::string & name )
{
	return( "msg://" + address + "/" + name );
}

std::string Message::getAdrPortFromUrl( const std::string & url )
{
	string adrPortStr;
	string pat = "msg://(\\S+)/\\S+";
	vector<string> m = Str::getMatch( url, pat );

	if ( m.size() > 1 ) {
		adrPortStr = m[1];
	} else {
		throw Primitives2Exception("Message::getAdrPortFromUrl() : Invalid URL format");
	}
	return( adrPortStr );
}

std::string Message::getNameFromUrl( const std::string & url )
{
	string name;
	string pat = "msg://\\S+/(\\S+)";
	vector<string> m = Str::getMatch( url, pat );

	if ( m.size() > 1 ) {
		name = m[1];
	} else {
		throw Primitives2Exception("Message::getNameFromUrl() : Invalid URL format");
	}
	return( name );
}

/////////////////////////////////////////////////////////////////////

uint64_t Message::getLength( const Message & msg )
{
	Buffer buf;
	return(toBinary(msg,buf,false));
}

uint64_t Message::toBinary( const Message & msg, Buffer & buf )
{
	return(toBinary(msg,buf,true));
}

uint64_t Message::toBinary( const Message & msg, Buffer & buf, bool write )
{
	uint64_t dataSize = ConvertToBinary::getLength(msg.m_dataMap);
	uint64_t srcSize = msg.m_srcAddress.size();
	uint64_t dstSize = msg.m_dstAddress.size();

	uint64_t headerSize = sizeof(uint64_t)*7 + srcSize + dstSize;
	uint64_t totalSize = headerSize + dataSize;

	if (!write) return(totalSize);

	if ( buf.size() < totalSize )
		throw Primitives2Exception("Buffer is too small");

	uint64_t * p = (uint64_t *)&buf[0];

	*p = totalSize;			p++;
	*p = msg.m_type;		p++;
	*p = msg.m_messageId;		p++;
	*p = msg.m_sequenceNum;		p++;
	*p = srcSize;			p++;

	if (srcSize != 0)
		Buffer((uint8_t *)p,srcSize) = msg.m_srcAddress;

	p = (uint64_t *)(((uint8_t *)p) + srcSize);

	*p = dstSize;			p++;

	if (dstSize != 0)
		Buffer((uint8_t *)p,dstSize) = msg.m_dstAddress;

	p = (uint64_t *)(((uint8_t *)p) + dstSize);

	*p = dataSize;

	if (dataSize != 0) {
		Buffer sbuf = buf.getSubBuffer(headerSize);
		ConvertToBinary::toBinary( msg.m_dataMap, sbuf );
	}

	return(totalSize);
}

/////////////////////////////////////////////////////////////////////

void Message::fromBinary( const Buffer & buf, Message & msg )
{
	if ( buf.size() < sizeof(uint64_t) )
		throw Primitives2Exception("Buffer is too small");

	uint64_t * p = (uint64_t *)&buf[0];

	uint64_t totalSize = *p;	p++;

	if ( buf.size() < totalSize )
		throw Primitives2Exception("Buffer is too small");

	msg.clear();
	msg.m_type = *p;		p++;
	msg.m_messageId = *p;		p++;
	msg.m_sequenceNum = *p;		p++;
	uint64_t length = *p;		p++;
	msg.m_srcAddress = string((const char *)p,length);
	p = (uint64_t *)(((uint8_t *)p) + length);
	length = *p;			p++;
	msg.m_dstAddress = string((const char *)p,length);
	uint8_t * pData = ((uint8_t *)p) + ( length + sizeof(uint64_t) );

	uint64_t index = pData - &buf[0];
	Buffer sbuf = buf.getSubBuffer(index);
	msg.m_dataMap = ConvertFromBinary::fromBinary(sbuf);
}

/////////////////////////////////////////////////////////////////////

uint64_t MessageClientSocket::getMessage( Socket & s, Buffer & buf, bool & retval, bool allowResize )
{
	retval = true;

	if ( allowResize ) buf.resize(sizeof(uint64_t));

	if ( buf.size() < sizeof(uint64_t) )
		throw Primitives2Exception("Buffer is too small");

	Buffer sb0 = buf.getSubBuffer(0,sizeof(uint64_t));

	if (!s.readWait( sb0 )) {
		retval = false;
		return(0);
	}

	uint64_t length = *((uint64_t*)&sb0[0]);

	if ( allowResize ) buf.resize(length);

	if ( buf.size() < length )
		throw Primitives2Exception("Buffer is too small");

	Buffer sb1 = buf.getSubBuffer(sizeof(uint64_t),length-sizeof(uint64_t));

	if (!s.readWait( sb1 )) {
		retval = false;
		return(0);
	}

	return( length );
}

bool MessageClientSocket::getMessage( Socket & s, Message & msg )
{
	bool retval;
	Buffer buf;
	MessageClientSocket::getMessage( s, buf, retval, true );
	if (!retval) return(false);
	Message::fromBinary( buf, msg );
	return( true );
}

bool MessageClientSocket::getMessage( EventHandler & hlr, Socket & s, const string & srcAddress )
{
	MessageData data;
	data.m_msg.reset( new Message );

	bool ret = MessageClientSocket::getMessage( s, *data.m_msg );
	if ( ! ret ) return( false );

	string name = Message::getNameFromUrl( data.m_msg->m_srcAddress );

	data.m_msg->m_srcAddress = Message::buildUrl( srcAddress, name );

	hlr.event( data );
	return( true );
}

/////////////////////////////////////////////////////////////////////

MessageClientSocket::MessageClientSocket( EventHandler & hlr ) : m_hlr(hlr)
{
	m_terminated = false;
	m_stopped = false;
}

bool MessageClientSocket::start( const IpAddress & adr )
{
	if (m_thread)
		throw Primitives2Exception("Already started");

	m_s.open( adr.getType(), SocketBase::eStream );

	if (!m_s.connect(adr)) return( false );

	m_thread.reset( new thread(threadFunctionStatic,this) );

	return( true );
}

void MessageClientSocket::stop()
{
	m_stopped = true;
	m_s.stop();
}

void MessageClientSocket::join()
{
	if (m_thread) m_thread->join();
}

bool MessageClientSocket::isStopped()
{
	return( m_stopped || m_terminated );
}

bool MessageClientSocket::write( const Message & msg )
{
	if (isStopped()) return( false );

	Buffer buf( Message::getLength(msg) );
	Message::toBinary( msg, buf );
	return( m_s.write( buf ) );
}

void MessageClientSocket::threadFunctionStatic( MessageClientSocket* self )
{
	try {
		string srcAddress = self->m_s.getConnectedAddress().getAddressPortString();

		while ( true ) {
			bool ret = MessageClientSocket::getMessage( self->m_hlr, self->m_s, srcAddress );
			if ( ! ret ) break;
			if ( self->m_stopped ) break;
		}
	}
	catch (...) {
		cout << "MessageClientSocket::threadFunctionStatic() : caught in ...\n";
	}
	try {
		self->m_terminated = true;
	}
	catch (...) {
		cout << "MessageClientSocket::threadFunctionStatic() : caught in ...\n";
	}
}

/////////////////////////////////////////////////////////////////////

MessageServerSocket::MessageServerSocket( EventHandler & hlr ) :
		m_hlr(hlr),
		m_acceptConnection(*this,&MessageServerSocket::acceptConnection),
		m_serverSocketThread(m_acceptConnection)
{
}

void MessageServerSocket::start( const IpAddress & adr )
{
	m_serverSocketThread.start( adr, SocketBase::eStream );
}

void MessageServerSocket::stop()
{
	m_serverSocketThread.stop();
}

void MessageServerSocket::join()
{
	m_serverSocketThread.join();
}

void MessageServerSocket::lock()
{
	m_serverSocketThread.lock();
}

void MessageServerSocket::unlock()
{
	m_serverSocketThread.unlock();
}

bool MessageServerSocket::find_unlocked( const IpAddress & adr )
{
	return(m_serverSocketThread.find_unlocked(adr));
}

bool MessageServerSocket::write_unlocked( const IpAddress & adr, const Message & msg )
{
	Buffer buf( Message::getLength(msg) );
	Message::toBinary( msg, buf );
	return(m_serverSocketThread.write_unlocked( adr, buf ));
}

void MessageServerSocket::stop_unlocked( const IpAddress & adr )
{
	m_serverSocketThread.getSocket_unlocked(adr)->stop();
}

bool MessageServerSocket::acceptConnection( EventData & data )
{
	SocketEventData & sdata = dynamic_cast<SocketEventData &>(data);

	string srcAddress = sdata.m_s->getAcceptedAddress().getAddressPortString();

	while ( true ) {
		bool ret = MessageClientSocket::getMessage( m_hlr, *sdata.m_s, srcAddress );
		if ( ! ret ) break;
	}
	return( true );
}

/////////////////////////////////////////////////////////////////////

MessageProcessor::MessageProcessor( EventHandler & hlr ) : m_hlr(hlr)
{
}

void MessageProcessor::start()
{
	if (m_thread)
		throw Primitives2Exception("Already started");

	m_thread.reset( new thread(threadFunctionStatic,this) );
}

void MessageProcessor::stop()
{
	m_messageQueue.stop();
}

void MessageProcessor::join()
{
	if (m_thread) m_thread->join();
}

void MessageProcessor::addMessage( std::shared_ptr<Message> msg )
{
	m_messageQueue.add( msg );
}

void MessageProcessor::threadFunctionStatic( MessageProcessor * self )
{
	while ( true ) {
		MessageData data;
		bool ret = self->m_messageQueue.get( data.m_msg );
		if (!ret) break;
		self->m_hlr.event( data );
	}
}

/////////////////////////////////////////////////////////////////////

