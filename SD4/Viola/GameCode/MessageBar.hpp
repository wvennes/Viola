//---------------------------------------------------------------------------
// MessageBar.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_MESSAGEBAR_
#define _INCLUDED_MESSAGEBAR_

#include <deque>
#include <string>
#include "Engine/Math/Colors.hpp"

class TextRenderer;


/////////////////////////////////////////////////////////////////////////////
struct MessageData
{
	std::string		m_message;
	Rgba			m_color;

	MessageData( const std::string& message, const Rgba& color ) : m_message( message ), m_color( color ) { }
};
typedef std::deque< MessageData > MessageList;


/////////////////////////////////////////////////////////////////////////////
class MessageBar
{
public:
	MessageBar();
	~MessageBar();

	void				Render( const float cellHeight );

	void				LogPrintf( const char* message, ... );
	void				LogPrintf( const Rgba& color, const char* message, ... );
	void				AddMessageToLog( const std::string& message );
	void				ReadMessage();
	void				ClearMessages();
	bool				HasSeenAllMessages();
	const MessageList*	GetMessageList();
	void				SetHistorySize( const unsigned int historySize );

private:
	MessageList			m_messageList;

private:
	void				LogPrintf( const char* message, va_list args );
	void				LogPrintf( const Rgba& color, const char* message, va_list args );

};

extern TextRenderer* g_theTextRenderer;
extern unsigned int g_currentShaderProgramID;

#endif