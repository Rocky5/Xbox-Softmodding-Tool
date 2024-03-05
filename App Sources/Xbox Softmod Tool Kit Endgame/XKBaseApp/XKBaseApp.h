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
#include "xbapp.h"
#include "xkconsole.h"


// You would typically use this class as a basis for you own Console
// Application.. and just override the functions the way you want...

class XKBaseApp : public CXBApplication
{
protected:

	//Use and Augment these enumns in your own app
	//to add more sections to your code..
	enum XSections
	{
		STARTUP_SCREEN = 0x00,
		EXIT_SCREEN = 0xFF
	};
	
	XKConsole* pxkconsole;
	LPCSTR XAppTitle;
	long XAppSection;
	BOOL WaitForInput;

	int XOffset;
	int YOffset;
	int XImgCorrection;
	int YImgCorrection;
	int ScreenWidth;
	int ScreenHeight;

	//Override this default section handler 
	//with your own..
	virtual void SectionHandler();

	//Override these default methods if you want..
	virtual void StartupScreen();
	virtual void ExitScreen();
	

public:
	XKBaseApp();
	virtual ~XKBaseApp();
	virtual HRESULT Initialize();
	virtual HRESULT FrameMove();
	virtual HRESULT Render();
	virtual HRESULT Cleanup();
	virtual void SetAppTitle(LPCSTR sAppTitle);
	


/*

    // Main objects used for creating and rendering the 3D scene
	//These variables are defined in base class..
	//Only present here for readability and reference..
	//DO NOT REMOVE !
	D3DPRESENT_PARAMETERS m_d3dpp;
    LPDIRECT3D8           m_pD3D;              // The D3D enumerator object
    LPDIRECT3DDEVICE8     m_pd3dDevice;        // The D3D rendering device
    LPDIRECT3DSURFACE8    m_pBackBuffer;       // The back buffer
    LPDIRECT3DSURFACE8    m_pDepthBuffer;      // The depth buffer
*/
};
