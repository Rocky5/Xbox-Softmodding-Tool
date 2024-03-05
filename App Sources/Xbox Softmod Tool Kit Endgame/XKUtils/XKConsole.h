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

class XKConsole  
{
public:
	void SetTitleBarVisible(BOOL isVisible);
	XKConsole();
	XKConsole(LPDIRECT3DDEVICE8 pDD3DevicePtr, long iConsoleWidth, long iConsoleHeight, long iConsoleXOffset, long iConsoleYOffset);
	virtual ~XKConsole();

	void SetTitle(LPCSTR sTitle);
	void Render();
	void Clear();
	void RenderTitleBar();

	void SetConsoleTextColor(UCHAR r, UCHAR g, UCHAR b);
	void WriteLine(LPCSTR pString);

protected:
	BOOL TitleBarVisible;
	typedef LPWSTR* LPPWSTR;
	void Initialize();
	void InitBuffers();
	void RenderTitleText();

	LPDIRECT3DDEVICE8 m_pD3DDevicePtr;
	LPDIRECT3DSURFACE8 m_pD3DSurfacePtr;
	XFONT* m_pConsoleTextFontPtr;
	D3DCOLOR m_ConsoleTextColor;

	long m_ConsoleXOffset;
	long m_ConsoleYOffset;
	long m_ConsoleWidth;
	long m_ConsoleHeight;

	long m_FontWidth;
	long m_FontHeight;

	LPWSTR m_ConsoleTitle;

	LPPWSTR m_ppConsoleBuffer;
	int m_ConsoleMaxLines;
	int m_ConsoleLinePos;

};
