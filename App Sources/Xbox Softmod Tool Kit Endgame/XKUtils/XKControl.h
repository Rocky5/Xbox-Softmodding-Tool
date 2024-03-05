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
#include <stdlib.h>
#include <string>
#include <map>

using namespace std;

class XKControl  
{
public:

	enum CONFIG_COLORS
	{
		BLACK = D3DCOLOR_ARGB(0xFF, 0x00, 0x00, 0x00),
		BLUE = D3DCOLOR_ARGB(0xFF, 0x20, 0x20, 0xFF),
		WHITE = D3DCOLOR_ARGB(0xFF, 0xff, 0xff, 0xff),
		ORANGE = D3DCOLOR_ARGB(0xFF, 0xDC, 0x82, 0x00),
		RED = D3DCOLOR_ARGB(0xFF, 0xD0, 0x10, 0x10),
		GREEN = D3DCOLOR_ARGB(0xFF, 0x5F, 0xAF, 0x00),
		CYAN = D3DCOLOR_ARGB(0xFF, 0x00, 0xBE, 0xFA),
		YELLOW = D3DCOLOR_ARGB(0xFF, 0xE6, 0xE6, 0x00)
	};


	XKControl(string Name, LPD3DXSPRITE d3dSprite);
	XKControl(string Name, LPD3DXSPRITE d3dSprite ,int XOffset, int YOffset, int Width, int Height);
	virtual ~XKControl();

	virtual void	Initialize();
	virtual void	Render();
	virtual	void	SetVisible(BOOL isVisible);
	virtual	BOOL	IsVisible();
	virtual	void	SetFocus(BOOL isVisible);
	virtual	BOOL	HasFocus();

	virtual BOOL	LoadImage(LPCSTR ImageFileName);
	virtual BOOL	LoadTransparentImage(LPCSTR ImageFileName, D3DCOLOR TransparentColor);

	//CREATE XKIMAGE CLASS TO LOAD IMAGES FROM DISK AND CREATE TEXTURES.....
	//	virtual void	SetImage(XKImage* ImageObj);
	virtual BOOL	IsImageLoaded();
	virtual	void	GetSize(LPINT pWidth, LPINT pHeight);
	virtual void	SetSize(int Width, int Height);
	virtual void	SetPosition(int XOffset, int YOffset);
	virtual void	GetPosition(LPINT pXOffset, LPINT pYOffset);

	//Dummy Blank Set & Get Text..
	virtual	void	SetText(string Text);
	virtual	string	GetText();

	virtual	void	SetTabIndex(INT TabIndex);
	virtual	INT		GetTabIndex();
	virtual	void	SetTabStop(BOOL tabStop);
	virtual	BOOL	GetTabStop();



	virtual void	SetAlphaChannel(UCHAR AlphaChannel);
	virtual UCHAR	GetAlphaChannel();
	virtual string	GetName();

	virtual void	Begin();
	virtual void	End();

protected:
	virtual	void	Construct(string Name, LPD3DXSPRITE d3dSprite ,int XOffset, int YOffset, int Width, int Height);
	
	LPDIRECT3DDEVICE8		m_BGDevice;
	LPD3DXSPRITE			m_BGSprite;
	D3DXVECTOR2				m_BGLocation;
	D3DXVECTOR2				m_BGScaling;
	LPDIRECT3DTEXTURE8		m_BGTexture;

	INT						m_Width;
	INT						m_Height;

	BOOL					m_hasFocus;
	BOOL					m_isVisible;
	BOOL					m_imageLoaded;

	UCHAR					m_AlpaChannel;

	string					m_Name;

	BOOL					m_TabStop;
	INT						m_TabIndex;

} ;

//the std vector map container fir controls..
typedef XKControl* LPXKControl;
typedef map<string, LPXKControl> XKControlMap;
