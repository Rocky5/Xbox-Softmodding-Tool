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

#include "xkcontrol.h"

using namespace std;

class XKControl_Menu : public XKControl  
{
public:

	typedef struct XKMenuItem
	{
		ULONG		Index;
		string		Name;
		string		Text;
		BOOL		Selected;
		//Future add Function Pointer for callbacks..aka event driven menu..
	};
	typedef map<string, XKMenuItem> XKMenuItemMap;	


	XKControl_Menu(string Name, LPD3DXSPRITE d3dSprite);
	XKControl_Menu(string Name, LPD3DXSPRITE d3dSprite ,int XOffset, int YOffset, int Width, int Height);
	virtual ~XKControl_Menu();

	virtual		void	Initialize();
	virtual		void	SetPosition(int XOffset, int YOffset);
	virtual		void	SetTitleOffset(int XOffset, int YOffset);
	virtual		void	SetTitleTextColor(D3DCOLOR TitleColor);

	virtual		void	SetItemsOffset(int XOffset, int YOffset);
	virtual		void	SetItemsTextColor(D3DCOLOR TitleColor);
	virtual		void	SetItemsSelectedColor(D3DCOLOR TitleColor);
	virtual		void	AddMenuItem(string ItemName, string ItemText);
	virtual		void	RemoveMenuItem(string ItemName);
	virtual		void	SetSelectedMenuItem(string ItemName);
	virtual		string	GetSelectedMenuItem();
	virtual		void	SelectPreviousMenuItem();
	virtual		void	SelectNextMenuItem();

	virtual		void	SetSize(int Width, int Height);
	virtual		void	SetVisible(BOOL isVisible);

	virtual		void	Render();

	virtual		void	SetTitleTextFont(CXBFont* pFont);
	virtual		void	SetItemTextFont(CXBFont* pFont);
	virtual		BOOL	IsBusy();
	
protected:
	virtual		void	RenderMenuItems();

	D3DRECT				m_TitleTextClipRect;
	D3DXVECTOR2			m_TitleTextOffset;
	D3DXVECTOR2			m_ItemsTextOffset;
	D3DXVECTOR2			m_BGTextureLocation;


	LPDIRECT3DTEXTURE8	m_pRenderTexture;
	LPDIRECT3DSURFACE8  m_pRenderSurface;

	LPDIRECT3DSURFACE8	m_pSurface;

	CXBFont*			m_MenuTitleFont;
	CXBFont*			m_MenuItemFont;

	D3DCOLOR			m_TextColor;
	D3DCOLOR			m_ItemsTextColor;
	D3DCOLOR			m_ItemSelectedColor;

	XKMenuItemMap		m_MenuMap;

	UINT				m_MenuAnimateSpeed;
	RECT				m_CurrentLocation;
	BOOL				m_MenuAvailable;
	BOOL				m_Animated;

	ULONG				m_MenuItemWindowBegin;
	ULONG				m_MenuItemWindowSize;

};
typedef XKControl_Menu* LPXKControl_Menu;

