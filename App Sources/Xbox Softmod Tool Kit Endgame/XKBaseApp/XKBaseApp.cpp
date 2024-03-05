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
{
}

XKBaseApp::~XKBaseApp()
{
}

HRESULT XKBaseApp::Initialize()
{
	XOffset = 10;
	YOffset = 10;
	XImgCorrection = 10;
	YImgCorrection = 10;
	ScreenWidth = 640 - XImgCorrection;
	ScreenHeight = 480 - YImgCorrection;
	XAppTitle = "XKBaseApp";
	XAppSection = STARTUP_SCREEN;

	WaitForInput = FALSE;

	//User standard 640x480 and do minor offset adjustments...
	pxkconsole = new XKConsole(m_pd3dDevice, ScreenWidth, ScreenHeight, XOffset, YOffset);

	//Title..
	pxkconsole->SetTitle(XAppTitle);
	
	return S_OK;
}



HRESULT XKBaseApp::FrameMove()
{
	SectionHandler();

	return S_OK;
}

HRESULT XKBaseApp::Render()
{


	//Fill Background.. This our default color..
	DWORD TopColor = D3DCOLOR_XRGB(0x30,0x44,0x69); //Blue
	DWORD BottomColor = D3DCOLOR_XRGB(0xA2, 0xC1, 0xF8);//light blue...
	RenderGradientBackground(TopColor, BottomColor);

	//Render Console Title Bar..
	pxkconsole->RenderTitleBar();

	//Present Data
	m_pd3dDevice->Present (NULL, NULL, NULL, NULL);

	//Render Console Text....
	pxkconsole->Render();

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
	pxkconsole->SetTitle(XAppTitle);
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
		pxkconsole->Clear();
		pxkconsole->SetConsoleTextColor(255, 255, 255); //Use White Text
		pxkconsole->WriteLine("Welcome the Console Template Application...");
		pxkconsole->WriteLine("");
		pxkconsole->WriteLine("");
		pxkconsole->WriteLine("Press the 'X' Button on your controller to QUIT.");

		WaitForInput = TRUE;
	}
	else
	{
		//You can change or add additional button/Trigger Handlers here....

        if(m_DefaultGamepad.bPressedAnalogButtons[XINPUT_GAMEPAD_A])
		{
			// *******************    TODO !!  ******************************
			// Uncomment these 2 lines and use it advance one section at a time....
			// Also add some text on the screen to tell the use to press A button to continue..

			// XAppSection++;  
			// WaitForInput = FALSE;
		}

        if(m_DefaultGamepad.bPressedAnalogButtons[XINPUT_GAMEPAD_X])
		{
			XAppSection = EXIT_SCREEN;
			WaitForInput = FALSE;
		}

	}
}





void XKBaseApp::ExitScreen()
{
	if (!WaitForInput)
	{
		pxkconsole->Clear();
		pxkconsole->SetConsoleTextColor(255, 255, 255);
		pxkconsole->WriteLine("");
		pxkconsole->WriteLine("");
		pxkconsole->WriteLine("Thank you for using this Application");
		pxkconsole->WriteLine("");

		WaitForInput = TRUE;
	}
	else
	{
			// *******************    TODO !!  ******************************
			//You could put button/Trigger handlers and Reboot code here
	}

}