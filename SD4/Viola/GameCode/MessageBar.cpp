//---------------------------------------------------------------------------
// MessageBar.cpp
//---------------------------------------------------------------------------

#include <stdarg.h>
#include "Engine/Rendering/TextRenderer.hpp"
#include "MessageBar.hpp"


//---------------------------------------------------------------------------
MessageBar::MessageBar()
	: m_messageList( MessageList() )
{

}


//---------------------------------------------------------------------------
MessageBar::~MessageBar()
{
	m_messageList.clear();
	//std::vector< MessageData >().swap( m_messageList );
}


//---------------------------------------------------------------------------
void MessageBar::LogPrintf( const char* message, ... )
{
	va_list args;
	va_start( args, message );
	{
		LogPrintf( message, args );
	}
	va_end( args );
}


//---------------------------------------------------------------------------
void MessageBar::LogPrintf( const Rgba& color, const char* message, ... )
{
	va_list args;
	va_start( args, message );
	{
		LogPrintf( color, message, args );
	}
	va_end( args );
}


//---------------------------------------------------------------------------
void MessageBar::LogPrintf( const char* message, va_list args )
{
	static const unsigned int BUFFER_SIZE = 2048;
	static char buffer[ BUFFER_SIZE ];
	vsnprintf_s( buffer, BUFFER_SIZE, BUFFER_SIZE-1, message, args );
	m_messageList.push_back( MessageData( buffer, Colors::WHITE ) );
}


//---------------------------------------------------------------------------
void MessageBar::LogPrintf( const Rgba& color, const char* message, va_list args )
{
	static const unsigned int BUFFER_SIZE = 2048;
	static char buffer[ BUFFER_SIZE ];
	vsnprintf_s( buffer, BUFFER_SIZE, BUFFER_SIZE-1, message, args );
	m_messageList.push_back( MessageData( buffer, color ) );
}


//---------------------------------------------------------------------------
void MessageBar::AddMessageToLog( const std::string& message )
{
	m_messageList.push_back( MessageData( message, Colors::WHITE ) );
}


//---------------------------------------------------------------------------
bool MessageBar::HasSeenAllMessages()
{
	return ( m_messageList.size() <= 1 );
}


//---------------------------------------------------------------------------
void MessageBar::ReadMessage()
{
	if ( !m_messageList.empty() ) m_messageList.pop_front();
}


//---------------------------------------------------------------------------
void MessageBar::ClearMessages()
{
// 	while ( !m_messageList.empty() )
// 	{
// 
// 	}
	m_messageList.clear();
}


//---------------------------------------------------------------------------
const MessageList* MessageBar::GetMessageList()
{
//	va_list args;
	return &m_messageList;
}


//---------------------------------------------------------------------------
void MessageBar::Render( const float cellHeight )
{
	if ( m_messageList.empty() ) return;

	if ( m_messageList.size() > 1 )
	{
		g_theTextRenderer->RenderText2D( ( m_messageList.front().m_message + " --MORE--" ).c_str(), cellHeight, Vec2f( 0.f, 0.f ), m_messageList.back().m_color, g_currentShaderProgramID );
	}
	else
	{
		g_theTextRenderer->RenderText2D( m_messageList.front().m_message.c_str(), cellHeight, Vec2f( 0.f, 0.f ), m_messageList.back().m_color, g_currentShaderProgramID );
	}
}