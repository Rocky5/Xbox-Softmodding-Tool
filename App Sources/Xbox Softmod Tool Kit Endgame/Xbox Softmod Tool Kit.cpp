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
*
* Source modified by Rocky5.
* Used with the Endgame Softmod Installer.
*
******************************************************************************************************
*/
#include "Xbox Softmod Tool Kit.h"
#include <d3d8.h>
#include "xkhdd.h"
#include <fstream>
#include <iostream>

#include "assets_bios.bfm.h"
#include "assets_boot.cfg.h"
#include "assets_config.xml.h"
#include "assets_loader.xbe.h"
#include "assets_payload.zip.h"
#include "assets_skin.zip.h"
#include "assets_unleashx.xbe.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Shared paths
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define Cache_Path			"E:\\CACHE\\"
// #define UX_Save				"E:\\UDATA\\9e115330\\0064122817A8"
#define Softmod_Save		"E:\\UDATA\\21585554"

#define Bios_BFM			Cache_Path	"bios.bfm"
#define Boot_CFG			Cache_Path	"boot.cfg"
#define Config_XML			Cache_Path	"config.xml"
#define Loader_XBE			Cache_Path	"loader.xbe"
#define Payload_Zip			Cache_Path	"payload.zip"
#define Skin_ZIP			Cache_Path	"softmod\\softmod.zip"
#define UnleashX_XBE		Cache_Path	"unleashx.xbe"

#define Cache_Folder		Cache_Path
#define Skin_Folder			Cache_Path	"softmod"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LED Colours
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define	LED_Orange			XKUtils::SetXBOXLEDStatus(XKUtils::LED_REGISTER_CYCLE0_GREEN|XKUtils::LED_REGISTER_CYCLE0_RED|XKUtils::LED_REGISTER_CYCLE1_GREEN|XKUtils::LED_REGISTER_CYCLE1_RED|XKUtils::LED_REGISTER_CYCLE2_GREEN|XKUtils::LED_REGISTER_CYCLE2_RED|XKUtils::LED_REGISTER_CYCLE3_GREEN|XKUtils::LED_REGISTER_CYCLE3_RED)
#define LED_Green			XKUtils::SetXBOXLEDStatus(XKUtils::LED_REGISTER_CYCLE0_GREEN|XKUtils::LED_REGISTER_CYCLE1_GREEN|XKUtils::LED_REGISTER_CYCLE2_GREEN|XKUtils::LED_REGISTER_CYCLE3_GREEN)
#define	LED_Red				XKUtils::SetXBOXLEDStatus(XKUtils::LED_REGISTER_CYCLE0_RED|XKUtils::LED_REGISTER_CYCLE0_RED|XKUtils::LED_REGISTER_CYCLE1_RED|XKUtils::LED_REGISTER_CYCLE1_RED|XKUtils::LED_REGISTER_CYCLE2_RED|XKUtils::LED_REGISTER_CYCLE2_RED|XKUtils::LED_REGISTER_CYCLE3_RED|XKUtils::LED_REGISTER_CYCLE3_RED)
#define LED_Flash_Orange	XKUtils::SetXBOXLEDStatus(XKUtils::LED_REGISTER_CYCLE0_GREEN|XKUtils::LED_REGISTER_CYCLE0_RED|XKUtils::LED_REGISTER_CYCLE3_GREEN|XKUtils::LED_REGISTER_CYCLE3_RED)
#define LED_Flash_Green		XKUtils::SetXBOXLEDStatus(XKUtils::LED_REGISTER_CYCLE0_GREEN|XKUtils::LED_REGISTER_CYCLE3_GREEN)
#define LED_Flash_Red		XKUtils::SetXBOXLEDStatus(XKUtils::LED_REGISTER_CYCLE0_RED|XKUtils::LED_REGISTER_CYCLE3_RED)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Main Code
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ConfigMagicApp::SaveFiles()
{
	CreateDirectory(Cache_Folder, NULL);
	CreateDirectory(Skin_Folder, NULL);

	LED_Flash_Green;

	CreateFiles(Bios_BFM,		Bios_BFM_Data,		sizeof(Bios_BFM_Data));
	CreateFiles(Boot_CFG,		Boot_CFG_Data,		sizeof(Boot_CFG_Data));
	CreateFiles(Config_XML,		Config_XML_Data,	sizeof(Config_XML_Data));
	CreateFiles(Loader_XBE,		Loader_XBE_Data,	sizeof(Loader_XBE_Data));
	CreateFiles(Payload_Zip,	Payload_ZIP_Data,	sizeof(Payload_ZIP_Data));
	CreateFiles(Skin_ZIP,		Skin_ZIP_Data,		sizeof(Skin_ZIP_Data));
	CreateFiles(UnleashX_XBE,	UnleashX_XBE_Data,	sizeof(UnleashX_XBE_Data));

	LED_Green;
}

void ConfigMagicApp::CreateFiles(const char *output, unsigned char *input, unsigned int size)
{
	std::ofstream AssetFile(output, std::ios::binary);
	AssetFile.write((char*)&input[0], size);
	AssetFile.close();
}

BOOL DeleteDirectory(const TCHAR* sPath)  
{  
    HANDLE hFind; // file handle
    WIN32_FIND_DATA FindFileData;

    TCHAR DirPath[MAX_PATH];
    TCHAR FileName[MAX_PATH];

    _tcscpy(DirPath,sPath);
    _tcscat(DirPath,_T("\\"));
    _tcscpy(FileName,sPath);
    _tcscat(FileName,_T("\\*")); // searching all files
    int nRet = 0;
    hFind = FindFirstFile(FileName, &FindFileData); // find the first file
    if( hFind != INVALID_HANDLE_VALUE ) 
    {
        do
        {
            _tcscpy(FileName + _tcslen(DirPath), FindFileData.cFileName);
            if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) 
            {
                // we have found a directory, recurse
                if( !DeleteDirectory(FileName) )
                break;   // directory couldn't be deleted
            }
            else 
            {
                if( !DeleteFile(FileName) ) 
                break;  // file couldn't be deleted
            }
        }
		while( FindNextFile(hFind, &FindFileData) );
		nRet = FindClose(hFind); // closing file handle
    }
	Sleep(100);
    return RemoveDirectory(sPath); // remove the empty (maybe not) directory and returns zero when RemoveDirectory function fails
}

HRESULT ConfigMagicApp::Initialize()
{
	XKUtils::MountDiskE();
	HRESULT retVal = S_OK;
	//Load Settings from Config File..
	LoadSettings();
	//Initialise the display.
	m_MenuXOffset = 0;
	m_MenuYOffset = 24;
	ScreenWidth = 640 - XImgCorrection;
	ScreenHeight = 480 - YImgCorrection;
	D3DXCreateSprite(m_pd3dDevice, &m_bgSprite);
	InitScreen();
	m_ActiveForm = m_pFrmStatus;
	Render();
	
	DeleteDirectory(Softmod_Save);
	// DeleteDirectory(UX_Save); // disabled as UX write protects files and it doesn't remove them all.
	DeleteDirectory(Cache_Path);
	SaveFiles();
	XKUtils::LaunchXBE("E:\\CACHE\\loader.xbe");
	
	return retVal;
}
void ConfigMagicApp::InitScreen()
{
	m_pFrmStatus = new XKControl_Panel("Status", m_bgSprite, XOffset, YOffset, ScreenWidth,ScreenHeight);
}
void ConfigMagicApp::ResetLocations(LONG xoffset, LONG yoffset)
{
	XOffset = (xoffset<=0)?0:xoffset;
	YOffset = (yoffset<=0)?0:yoffset;
	m_pFrmInfo->SetPosition(XOffset, YOffset);
	m_pFrmStatus->SetPosition(XOffset, YOffset);
	txtStatus->SetPosition(XOffset, YOffset+405);
}
HRESULT ConfigMagicApp::Render()
{
	//Clear screen..
	m_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE); //some reason rendergradientbg doesn't work if this is false...
	RenderGradientBackground(0xff000000, 0xff000000);
	m_ActiveForm->Render();
	//Present Data
	m_pd3dDevice->Present(NULL, NULL, NULL, NULL);
	return S_OK;
}
void ConfigMagicApp::LoadSettings()
{
	//*************************** READ SCREEN SIZE, OFFSETS & COLOURS *****************************************
	XOffset = 25;
	YOffset = 20;
	XImgCorrection = 45;
	YImgCorrection = 40;
	m_clrMessageTextColor		= 0xffefcb49;
	m_clrStatusTextColor		= 0xff979797;
}
//Application start point
void main()
{
	// LED_Orange;
	ConfigMagicApp xbapp;
	xbapp.Create();
	xbapp.SetAppTitle("Xbox Softmodding Tool");
	xbapp.Run();
}