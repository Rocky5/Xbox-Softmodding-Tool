/*
**********************************
**********************************
**      BROUGHT TO YOU BY:		**
**********************************
**********************************
**								**
**		  [TEAM ASSEMBLY]		**
**								**
**		www.team-assembly.com	**
**								**
******************************************************************************************************
* This is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
******************************************************************************************************
*/

#pragma once

#include <xtl.h>
#include <xfont.h>
#include "xbfont.h"
#include <stdlib.h>
#include <string>
#include <map>

#include "xktimer.h"
#include "xkcontrol.h"

using namespace std;

class XKControl_TextBox : public XKControl  
{
public:
	XKControl_TextBox(string Name, LPD3DXSPRITE d3dSprite);
	XKControl_TextBox(string Name, LPD3DXSPRITE d3dSprite ,int XOffset, int YOffset, int Width, int Height);
	virtual ~XKControl_TextBox();

	virtual		void	Initialize();
	virtual		void	SetPosition(INT XOffset, INT YOffset);
	virtual		void	SetSize(INT Width, INT Height);
	
	virtual		void	SetCursorPosition(INT CursorPosition);
	virtual		INT		GetCursorPosition();
	virtual		void	SetCursorBlinkRate(UINT BlinkRate);
	virtual		void	SetCursorVisible(BOOL Visible);

	virtual		void	Render();
	
	virtual		string	GetText();
	virtual		void	SetText(string Text);
	virtual		void	SetTextColor(D3DCOLOR color);
	virtual		void	SetFocusTextColor(D3DCOLOR color);
	virtual		void	SetTextOffset(int XOffset, int YOffset);
	virtual		HRESULT	CreateBitmapFont(LPCSTR FontFileName);
	virtual		void	SetBitmapFont(CXBFont* pFont);

	virtual		void	SetTextCentered(BOOL TextCentered);
	virtual		void	SetTextShadow(BOOL ShadowEnabled);


protected:
	string				m_Text;
	D3DRECT				m_TextClipRect;
	D3DXVECTOR2			m_TextLocation;

	LPDIRECT3DSURFACE8	m_pSurface;
	CXBFont*			m_pXBFont;
	D3DCOLOR			m_TextColor;
	D3DCOLOR			m_FocusTextColor;

	BOOL				m_TextCentered;
	BOOL				m_ShadowEnabled;

	INT					m_CursorPosition;
	UINT				m_CursorBlinkRate;
	BOOL				m_CursorVisible;

	XKTimer				m_BlinkerTimer;
 
};

typedef XKControl_TextBox* LPXKControl_TextBox;
