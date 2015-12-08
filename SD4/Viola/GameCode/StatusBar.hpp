//---------------------------------------------------------------------------
// StatusBar.hpp
//---------------------------------------------------------------------------

#pragma once
#ifndef _INCLUDED_STATUSBAR_
#define _INCLUDED_STATUSBAR_

class Player;
class TextRenderer;

/////////////////////////////////////////////////////////////////////////////
class StatusBar
{
public:
	StatusBar();
	~StatusBar();

	void		SetPlayer( Player* player );
	void		Render( float cellHeight );

private:
	Player*		m_player;

};

extern TextRenderer* g_theTextRenderer;
extern unsigned int g_currentShaderProgramID;

#endif