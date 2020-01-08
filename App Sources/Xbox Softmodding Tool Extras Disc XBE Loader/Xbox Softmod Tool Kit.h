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
#include "md5.h"

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

struct Bios
{
 char Name[50];
 char Signature[33];
};

class ConfigMagicApp : public XKBaseApp  
{

int file_exist(const char *name);

private:

	void	InitScreen();
	void	INIT_SCREEN();
	void	ResetLocations(LONG xoffset, LONG yoffset);
	void	ProcessMenuSelect(LPCSTR MenuItemSelected);
	void	LoadSettings();
	void	WriteTXTInfoFile(LPCSTR strFilename);
	void	Lock_New_HDD();
	void	UnLock_New_HDD();
	void	Write_XBOX_EEPROM();
	void	Force_Write_XBOX_EEPROM();
	void	CreateEEPROMBackup();
	
	/////////////////////////////////////////
	void	CheckVirtualEEPROM();
	void	VirtualEEPROMRemoval();
	void	EnableVirtualEEPROM();
	void	SetUpDashboard();
	void	Stage2OfInstallSoftmod();
	void	Stage2OfUpdateSoftmod();
	void	InstallKernelFont();
	void	UpdateKernelFont();
	void	RestoreKernelFont();
	void	ReloadNKPatcherSettings();
	void	NullHardDriveKey();
	void	LockHardDrive();
	void	ChangeVideoRegionToNTSC();
	void	ChangeVideoRegionToPAL();
	void	NULLEEPROMCheckSum3();
	void	NTSCResetToFactoryEEPROM();
	void	PALResetToFactoryEEPROM();
	void	PatchNKPatcherToSupportOtherPartitions();
	void	PatchNKPatcherToSupportFAndG();
	void	PatchNKPatcherToSupportDualBoot();
	void	DumpDecryptedEEPROM();
	void	EnablePersistentSoftmodState();
	void	DisablePersistentSoftmodState();
	void	SetLanguage();
	void	CheckBios();
	void	CreateBiosBackup();
	void	TSOPCleanup();
	void	XBST_Softmodded_System();
	void	Other_Softmodded_System();
	void	Hardmodded_System();
	
	/////////////////////////////////////////
	void	PatchXBEFiles();
	void	NULL_HDD_KEY();
	void	CreateDecryptedEEPROMBackup();
	void	V_Serial();
	void	ColdBootSoftmod();
	void	ChangeRegionNTSC();
	void	ChangeRegionPAL();
	void	NULLChecksum3();
	void	FactoryResetPAL();
	void	FactoryResetNTSC();
	void	EnableControlledError16State();
	void	DisableControlledError16State();
	void	KernelFontInstall();
	void	SetupRNKP();

	
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
	BOOL				bios_dumped;

	LPXKControl_Panel	m_ActiveForm;
	LPXKControl_Panel	m_pFrmInfo;
	LPXKControl_Panel	m_pFrmStatus;

	CXBFont				m_XBFont;
	CXBFont				m_XBFont1;
	CXBFont				m_XBFont2;

	D3DCOLOR m_clrMessageTextColor;
	D3DCOLOR m_clrStatusTextColor;
	LPXKControl_TextBox	txtStatus;
	LPXKControl_TextBox	txtStatusMsg;
    LPXKControl_TextBox txtStatusMsg2;
	LPXKControl_TextBox	txtTitle;	
	LPD3DXSPRITE		m_bgSprite;
    LPDIRECT3DTEXTURE8  m_pOverlay[2];		// Overlay textures
    LPDIRECT3DSURFACE8  m_pSurface[2];		// Overlay Surfaces
    DWORD               m_dwCurrent;		// Current surface
	
    char MD5_Sign[32 + 1];
    struct Bios * LoadBiosSigns();
    bool CheckBios(CStdString& strDetBiosNa);
    char* ReturnBiosName(char *buffer, char *str);
    char* ReturnBiosSign(char *buffer, char *str);
    char* CheckMD5 (struct Bios *Listone, char *Sign);
    char* MD5Buffer(char *filename,long PosizioneInizio,int KBytes);

public:
	virtual HRESULT		Initialize();
	virtual HRESULT		Render();
};
