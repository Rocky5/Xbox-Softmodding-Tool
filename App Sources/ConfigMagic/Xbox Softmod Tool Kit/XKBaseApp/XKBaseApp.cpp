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

#include "xkbaseApp.h"


XKBaseApp::XKBaseApp()
			:CXBApplication() //Call base class construct
{

}

XKBaseApp::~XKBaseApp()
{
}


HRESULT XKBaseApp::Initialize()
{
	
	return S_OK;
}



HRESULT XKBaseApp::FrameMove()
{
	SectionHandler();

	return S_OK;
}

HRESULT XKBaseApp::Render()
{

	return S_OK;
}



HRESULT XKBaseApp::Cleanup()
{
	return S_OK;
}

void XKBaseApp::SetAppTitle(LPCSTR sAppTitle)
{
	XAppTitle = sAppTitle;

	//Title..
}


void XKBaseApp::SectionHandler()
{

	switch(XAppSection)
	{
	case(STARTUP_SCREEN):
			StartupScreen();
			break;
	case(EXIT_SCREEN):
			ExitScreen();
			break;
	}
}


void XKBaseApp::StartupScreen()
{
	if (!WaitForInput)
	{
		WaitForInput = TRUE;
	}
	else
	{
		//You can change or add additional button/Trigger Handlers here....

	}
}





void XKBaseApp::ExitScreen()
{
	if (!WaitForInput)
	{
		WaitForInput = TRUE;
	}
	else
	{
			// *******************    TODO !!  ******************************
			//You could put button/Trigger handlers and Reboot code here
	}

}