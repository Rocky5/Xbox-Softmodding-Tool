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

#include "XKBaseApp.h"

#include "XKUtils.h"
#include "XKGeneral.h"
#include "XKEEPROM.h"
#include "XKHDD.h"
#include "XKCRC.h"

#include "XKControl.h"
#include "XKControl_Panel.h"
#include "XKControl_TextBox.h"
#include "XKControl_Menu.h"
#include "XKControl_Keyboard.h"
#include "wmvplayer.h"


#define MainConfigFileName "D:\\ConfigMagic.ini"
#define SkinConfigFileName "D:\\media\\Skin.ini"

class ConfigMagicApp : public XKBaseApp  
{
private:

	enum CONFIG_SECTIONS
	{
		
		STARTUP_SCREEN = 0x00,
		WARNING_SCREEN,
		XBOX_CONFIG_SCREEN,


		LOAD_XBOX_EEPROM_SCREEN = 0xF0,
		WRITE_XBOX_EEPROM_SCREEN = 0xF1,
		LOAD_BIN_EEPROM_SCREEN = 0xF2,
		EDIT_EEPROM_SCREEN = 0xF3,
		LOAD_CFG_EEPROM_SCREEN = 0xF4,
		CREATE_BACKUP_SCREEN = 0xF5,
		LOCK_HDD_SCREEN = 0xFD,
		UNLOCK_HDD_SCREEN = 0xFE,
		EXIT_SCREEN = 0xFF
	};

	void	SectionHandler();
	void	StartupScreen();
	void	ExitScreen();

	void	WarningScreen();
	void	CreateEEPROMBackup(LPCSTR BackupFilePrefix);
	void	XBOXConfigScreen();
	void	Unlock_HDD_Screen();
	void	Lock_HDD_Screen();
	void	Load_XBOX_EEPROM_Screen();
	void	Write_XBOX_EEPROM_Screen();
	void	Load_CFG_EEPROM_Screen();
	void	Load_BIN_EEPROM_Screen();
	void	Create_Backup_Files_Screen();
	void	Edit_EEPROM_Screen();
	void	Build_EEPROM_From_Screen();

	void	LoadMainMenuItems();
	void	InitStatusScreen();
	void	InitConfigScreen();
	void	InitWarningScreen();
	
	void	ResetConfigScreen();
	void	ResetLocations(LONG xoffset, LONG yoffset);
	void	ProcessMenuSelect(LPCSTR MenuItemSelected);

	void	LoadSettingsFromINI();
	HRESULT	PlayVideo(CHAR* strFilename);
	void	WriteTXTInfoFile(LPCSTR strFilename);

	UCHAR				m_EEPROM_BIN_FileName[FILENAME_MAX];
	UCHAR				m_EEPROM_CFG_FileName[FILENAME_MAX];
	UCHAR				m_EEPROM_Backup_Path[FILENAME_MAX];
	
	BOOL				m_EnryptedRegionValid;
	BOOL				m_XBOX_EEPROM_Current;

	long				XConfSections;
	XKEEPROM*			m_pXKEEPROM;
	XBOX_VERSION		m_XBOX_Version;
	
	ULONG				m_MenuXOffset;
	ULONG				m_MenuYOffset;
	LPXKControl_Menu	m_pMainMenu;
	BOOL				ShowMenu;

	LONG				m_KeyboardHeight;
	LONG				m_KeyboardWidth;


	LPXKControl_Panel	m_ActiveForm;
	LPXKControl_Panel	m_pFrmInfo;
	LPXKControl_Panel	m_pFrmStatus;
	LPXKControl_Panel	m_pFrmWarning;

	CXBFont				m_XBFont1;
	CXBFont				m_XBFont2;
	CXBFont				m_XBFont3;
	CXBFont				m_XBFont4;

	D3DCOLOR m_clrCurrentHeadingColor;
	D3DCOLOR m_clrBinFileHeadingColor;
	D3DCOLOR m_clrCfgFileHeadingColor;
	D3DCOLOR m_clrOnTheFlyHeadingColor;
	D3DCOLOR m_clrMessageTextColor;
	D3DCOLOR m_clrStatusTextColor;
	D3DCOLOR m_clrLabelsTextColor;
	D3DCOLOR m_clrValuesTextColor;
	D3DCOLOR m_clrHDDStatusTextColor;
	D3DCOLOR m_clrVersionTextColor;
	D3DCOLOR m_clrMenuItemTextColor;
	D3DCOLOR m_clrMenuSelectTextColor;
	D3DCOLOR m_clrMenuHeadingColor;

	LPXKControl_TextBox	txtStatus;
	LPXKControl_TextBox	txtStatusMsg;
    LPXKControl_TextBox txtStatusMsg2;

	LPXKControl_TextBox	txtTitle;

	LPXKControl_TextBox	txtSerial;
	LPXKControl_TextBox	txtMAC;
	LPXKControl_TextBox	txtVideoFormat;
	LPXKControl_TextBox txtOnlineKey;
	LPXKControl_TextBox txtDVDZone;
	LPXKControl_TextBox txtDVDModel;
	LPXKControl_TextBox txtHDDKey;
	LPXKControl_TextBox txtHDDModel;
	LPXKControl_TextBox txtHDDSerial;
	LPXKControl_TextBox txtHDDStatus;
	LPXKControl_TextBox txtHDDPwd;

	LPXKControl_TextBox	lblSerial;
	LPXKControl_TextBox	lblMAC;
	LPXKControl_TextBox	lblVideoFormat;
	LPXKControl_TextBox lblOnlineKey;
	LPXKControl_TextBox lblDVDZone;
	LPXKControl_TextBox lblDVDModel;
	LPXKControl_TextBox lblHDDKey;
	LPXKControl_TextBox lblHDDModel;
	LPXKControl_TextBox lblHDDSerial;
	LPXKControl_TextBox lblHDDPwd;
	LPXKControl_TextBox lblXBVersion;
	LPXKControl_TextBox lblCMVersion;
	

	LPXKControl_Keyboard m_kbdEditBox;

	LPD3DXSPRITE		m_bgSprite;

	BOOL				m_bBlind;


    LPDIRECTSOUND8      m_pDSound;			// DirectSound object
    CWMVPlayer		m_XKVideoPlayer;    // Video Player object
    LPDIRECT3DTEXTURE8  m_pOverlay[2];		// Overlay textures
    LPDIRECT3DSURFACE8  m_pSurface[2];		// Overlay Surfaces
    DWORD               m_dwCurrent;		// Current surface

public:
	virtual HRESULT		Initialize();
	virtual HRESULT		Render();
	ConfigMagicApp();
	virtual ~ConfigMagicApp();




};
