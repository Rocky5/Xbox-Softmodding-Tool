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
* Source modified by Rocky5 to only backup the EEPROM data.
* Used with the 2014/16 Softmod kit.
*
******************************************************************************************************
*/

// ConfigMagicApp.cpp: implementation of the ConfigMagicApp class.
//
//////////////////////////////////////////////////////////////////////

#include "ConfigMagicApp.h"
#include "firstrunxbe.h"
#include "ShadowCHeader.h"
#include "xkhdd.h"
#include "xkeeprom.h"
#include <fstream>
#include <time.h>


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Shared paths
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define PrepDir												"E:\\Prep\\"
#define NKPSDir												"E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\"
#define NKPDir												"C:\\NKPatcher\\"
#define NKPConfDir											"C:\\NKPatcher\\Configs\\"
#define NKPDirAlt											"H:\\NKPatcher\\"
#define NKPConfDirAlt										"H:\\NKPatcher\\Configs\\"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Save path for the EEPROM files
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define Backup_Path											"E:\\Backups\\EEPROM\\"
#define Virtual_Path				NKPConfDirAlt
#define Virtual_Full_Path			NKPConfDirAlt			"EEPROM.bin"
//#define Backup_Path "D:\\"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Create Directory paths
// Cant seem to create a dir structure with one path, so had to use two
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define Backup_EEPROM_Save_Path1							"E:\\Backups"
#define Backup_EEPROM_Save_Path2							"E:\\Backups\\EEPROM"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Config files, or if you will enabler files
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define Virtual_File										"D:\\Virtual_EEPROM_Backup.enabled"
#define Reboot_File											"D:\\Reboot.enabled"
#define Override_File										"D:\\Standalone_Mode.enabled"
#define Backup												"E:\\Prep\\done.xbe"
#define Update_Font											"D:\\Update_Font.enabled"
#define Restore_Font										"D:\\Restore_Font.enabled"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Dashboard Files
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DashboardXBE				NKPSDir					"configs\\alt xbe's\\dashboard.xbe"
#define DashboardXML				NKPSDir					"configs\\dash.xml"
#define DashboardSkinXML			NKPSDir					"Skins\\Dashboard\\skin.xml"
#define DashboardSkinwideXML		NKPSDir					"Skins\\Dashboard\\skinwide.xml"
#define DashboardSkinSplash			NKPSDir					"Skins\\Dashboard\\UXSplash.jpg"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enable/Disable png files
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define EnabledPNG					NKPSDir					"toggles\\enabled.png"
#define IconPNG						NKPSDir					"modules\\2016 softmod tool kit\\icon.png"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Setup Kernel selection for first run of softmod
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define FirstRunXBE					PrepDir					"default.xbe"
#define SecondRun					PrepDir					"secondrun.bin"
#define FirstRunBin											"D:\\firstrun.bin"
#define UpdateRunBin										"D:\\updaterun.bin"
#define Font_Path					NKPConfDir				"fonts\\"
#define xbox_xtf_File_Path									"C:\\xodash\\xbox.xtf"
#define Generic_Font_File			NKPConfDir				"Fonts\\generic.xtf"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Setup Alt selection for first run of softmod
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define Font_Path_Alt 				NKPConfDirAlt			"fonts\\"
#define xbox_xtf_File_Alt									"H:\\xodash\\xbox.xtf"
#define Generic_Font_File_Alt		NKPConfDirAlt			"Fonts\\generic.xtf"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Setup ShadowC selection for first run of softmod
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define ShadowC_Location			NKPDir					"shadowc\\shadowc.img"
#define ShadowC_Size										485
#define ShadowCOFF					NKPConfDir				"shadowc_off.bin"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Setup UnleashX for showing the ShadowC partition
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define NKPatcherSettings			NKPSDir					"UnleashX.xbe"
#define ResoftmodDash				NKPSDir					"modules\\resoftmod dash\\default.xbe"
#define CRescueDash					NKPDir					"rescuedash\\unleashx.xbe"
#define DashSettings				NKPSDir					"modules\\dash settings\\default.xbe"
#define ERescueDash1										"E:\\TDATA\\Rescuedash\\default.xbe"
#define ERescueDash2										"E:\\UDATA\\Rescuedash\\default.xbe"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ConfigMagicApp::ConfigMagicApp()
{
}

ConfigMagicApp::~ConfigMagicApp()
{
	delete m_pXKEEPROM;
}

HRESULT ConfigMagicApp::Initialize()
{

	HRESULT retVal = S_OK;
	
	m_EnryptedRegionValid = FALSE;
	m_XBOX_EEPROM_Current = FALSE;
	
	m_pXKEEPROM = new XKEEPROM();
	
	//Load Settings from Config File.. Disabled for now
	
	//save current eeprom context..
	XKEEPROM::EEPROMDATA currentEEPROM;
	m_pXKEEPROM->GetEEPROMData(&currentEEPROM);
	BOOL EncryptedState = m_pXKEEPROM->IsEncrypted();
	
	//Read XBOX EEPROM and save to disk..
	m_pXKEEPROM->ReadFromXBOX();
	
	//incase path is on these drives..
	XKUtils::MountDiskC();
	XKUtils::MountDiskE();
	XKUtils::MountDiskH();

	std::ifstream dashprep(SecondRun);
	if (dashprep.good())
	{
		dashprep.close();
		remove(FirstRunXBE);
		remove(SecondRun);
		RemoveDirectory(PrepDir);
		CreateDirectory("C:\\Dashboard", NULL);
		CreateDirectory("C:\\Dashboard\\Skins", NULL);
		CreateDirectory("C:\\Dashboard\\Skins\\Softmod", NULL);
		CreateDirectory("C:\\Dashboard\\Skins\\Softmod Wide", NULL);
		CopyFile(DashboardXBE, "C:\\Dashboard\\default.xbe", NULL);
		CopyFile(DashboardXML, "C:\\Dashboard\\config.xml", NULL);
		CopyFile(DashboardSkinXML, "C:\\Dashboard\\Skins\\Softmod\\skin.xml", NULL);
		CopyFile(DashboardSkinwideXML, "C:\\Dashboard\\Skins\\Softmod Wide\\skin.xml", NULL);
		CopyFile(DashboardSkinSplash, "C:\\Dashboard\\Skins\\Softmod\\UXSplash.jpg", NULL);
		CopyFile(DashboardSkinSplash, "C:\\Dashboard\\Skins\\Softmod Wide\\UXSplash.jpg", NULL);
		XKUtils::XBOXPowerCycle();
	}
	else
	{
		dashprep.close();
	}
	
	std::ifstream VirtualEEPROM(Virtual_File);
	if (VirtualEEPROM.good())
	{
		VirtualEEPROM.close();
		remove(Virtual_Full_Path);
		remove("H:\\NKPatcher\\Configs\\EEPROM_off.bin");
		remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\veeprom\\disabled.png");
		CopyFile(EnabledPNG, "E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\veeprom\\enabled.png", NULL);
		//Create Full path for BIN File..
		CHAR tmpFileName[FILENAME_MAX];
		ZeroMemory(tmpFileName, FILENAME_MAX);
		strcat(tmpFileName, Virtual_Path);
		strcat(tmpFileName, "EEPROM.bin");
		m_pXKEEPROM->WriteToBINFile((LPCSTR)tmpFileName);
	}
		else
	{
		VirtualEEPROM.close();		
		//Create E:\Backups\EEPROM directory
		CreateDirectory(Backup_EEPROM_Save_Path1, NULL);
		CreateDirectory(Backup_EEPROM_Save_Path2, NULL);
/*
		CreateDirectory(Kernel_Version_Save_Path, NULL);
		Remove all kernel files. (clean them)
		remove(Kernel_Version_Save_Path"\\K.1.0.3944.1");
		remove(Kernel_Version_Save_Path"\\K.1.0.4034.1");
		remove(Kernel_Version_Save_Path"\\K.1.0.4627.1");
		remove(Kernel_Version_Save_Path"\\K.1.0.4817.1");
		remove(Kernel_Version_Save_Path"\\K.1.0.5101.1");
		remove(Kernel_Version_Save_Path"\\K.1.0.5530.1");
		remove(Kernel_Version_Save_Path"\\K.1.0.5713.1");
		remove(Kernel_Version_Save_Path"\\K.1.0.5838.1");
 */
		//Create Full path for BIN File..
		CHAR tmpFileName[FILENAME_MAX];
		ZeroMemory(tmpFileName, FILENAME_MAX);
		strcat(tmpFileName, Backup_Path);
		strcat(tmpFileName, "EEPROM.bin");
		m_pXKEEPROM->WriteToBINFile((LPCSTR)tmpFileName);
		
		//Write XBOX Information into .TXT file...
		ZeroMemory(tmpFileName, FILENAME_MAX);
		strcat(tmpFileName, Backup_Path);
		strcat(tmpFileName, "EEPROM.cfg");
		m_pXKEEPROM->WriteToCFGFile((LPCSTR)tmpFileName);
		
		ZeroMemory(tmpFileName, FILENAME_MAX);
		strcat(tmpFileName, Backup_Path);
		strcat(tmpFileName, "XBOX INFO.txt");
		WriteTXTInfoFile(tmpFileName);
		
		//Write XBOX Kernel Information file...
		CHAR tmpFileName2[FILENAME_MAX];
		CHAR tmpData[256];
		LPSTR tmpFileStr = new CHAR[2048];
		DWORD tmpSize = 256;
		ZeroMemory(tmpData, tmpSize);
		ZeroMemory(tmpFileStr, 2048);
		tmpSize = 256;ZeroMemory(tmpData, tmpSize);
		sprintf(tmpData, "K.%d.%d.%d.%d", *((USHORT*)XboxKrnlVersion),*((USHORT*)XboxKrnlVersion+1),*((USHORT*)XboxKrnlVersion+2),*((USHORT*)XboxKrnlVersion+3));
		strcat(tmpFileStr, tmpData);
		ZeroMemory(tmpFileName, FILENAME_MAX);
		std::ifstream FontPath(Update_Font);
			if (FontPath.good())
				{
					FontPath.close();
					strcat(tmpFileName, Font_Path_Alt);
				} else {
					FontPath.close();
					strcat(tmpFileName, Font_Path);
				}
		strcat(tmpFileName, tmpFileStr);
		ZeroMemory(tmpFileName2, FILENAME_MAX);
		strcat(tmpFileName2, "E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\");
		strcat(tmpFileName2, tmpFileStr);
		strcat(tmpFileName2, ".png");
		//WriteKERNELInfoFile(tmpFileName);
		/*
			First run code, this will patch UnleashX, install a clean ShadowC/img and also install the generic font, though the font install isn't needed as the generic font is used by default.
		*/
		std::ifstream stream(FirstRunBin);
		if (stream.good())
			{	
				stream.close();
				remove(ShadowCOFF);
				remove(xbox_xtf_File_Path);
				remove(FirstRunBin);
				remove(FirstRunXBE);
				RemoveDirectory(PrepDir);
				CopyFile(Generic_Font_File, xbox_xtf_File_Path, 1);
				CopyFile(EnabledPNG, "E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\generic.png", NULL);

				/* Patch UnleashX to show unprotected C partition for NKPatcher Settings */
				std::ofstream UnleashX_XBE1(NKPatcherSettings, std::ios::binary | std::ios::in | std::ios::out);
				UnleashX_XBE1.seekp(400);
				UnleashX_XBE1.write("\x4E\x00\x4B\x00\x50\x00\x61\x00\x74\x00\x63\x00\x68\x00\x65\x00\x72\x00\x20\x00\x53\x00\x65\x00\x74\x00\x74\x00\x69\x00\x6E\x00\x67\x00\x73", 35);
				UnleashX_XBE1.seekp(2399875);
				UnleashX_XBE1.write("14", 2);
				UnleashX_XBE1.close();
				
				std::ofstream UnleashX_XBE2(ResoftmodDash, std::ios::binary | std::ios::in | std::ios::out);
				UnleashX_XBE2.seekp(400);
				UnleashX_XBE2.write("\x52\x00\x65\x00\x53\x00\x6F\x00\x66\x00\x74\x00\x6D\x00\x6F\x00\x64\x00\x20\x00\x44\x00\x61\x00\x73\x00\x68", 27);
				UnleashX_XBE2.seekp(2399875);
				UnleashX_XBE2.write("14", 2);
				CopyFile(IconPNG, "E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\modules\\resoftmod dash\\icon.png", NULL);
				UnleashX_XBE2.close();
				
				std::ofstream UnleashX_XBE3(CRescueDash, std::ios::binary | std::ios::in | std::ios::out);
				UnleashX_XBE3.seekp(400);
				UnleashX_XBE3.write("\x43\x00\x20\x00\x52\x00\x65\x00\x63\x00\x6F\x00\x76\x00\x65\x00\x72\x00\x79\x00\x20\x00\x44\x00\x61\x00\x73\x00\x68\x00", 29);
				UnleashX_XBE3.close();
				
				std::ofstream UnleashX_XBE4(ERescueDash1, std::ios::binary | std::ios::in | std::ios::out);
				UnleashX_XBE4.seekp(400);
				UnleashX_XBE4.write("\x45\x00\x20\x00\x52\x00\x65\x00\x63\x00\x6F\x00\x76\x00\x65\x00\x72\x00\x79\x00\x20\x00\x44\x00\x61\x00\x73\x00\x68\x00", 29);
				UnleashX_XBE4.close();
				
				std::ofstream UnleashX_XBE5(ERescueDash2, std::ios::binary | std::ios::in | std::ios::out);
				UnleashX_XBE5.seekp(400);
				UnleashX_XBE5.write("\x45\x00\x20\x00\x52\x00\x65\x00\x63\x00\x6F\x00\x76\x00\x65\x00\x72\x00\x79\x00\x20\x00\x44\x00\x61\x00\x73\x00\x68\x00", 29);
				UnleashX_XBE5.close();
				
				std::ofstream UnleashX_XBE6(DashSettings, std::ios::binary | std::ios::in | std::ios::out);
				UnleashX_XBE6.seekp(400);
				UnleashX_XBE6.write("\x42\x00\x61\x00\x63\x00\x6B\x00\x75\x00\x70\x00\x2C\x00\x20\x00\x52\x00\x65\x00\x73\x00\x74\x00\x6F\x00\x72\x00\x65\x00\x20\x00\x6F\x00\x72\x00\x20\x00\x4D\x00\x6F\x00\x76\x00\x65", 45);
				CopyFile(IconPNG, "E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\modules\\dash settings\\icon.png", NULL);
				UnleashX_XBE6.close();
				/**/
				{
					/* Create firstrun.xbe */
					CreateDirectory("E:\\Prep", NULL);
					CopyFile(EnabledPNG, SecondRun, NULL);
					int i;
					std::ofstream FirstRunXBEFile(FirstRunXBE, std::ios::binary);
					for(i = 0; i < sizeof(firstrunxbeh); i++)
					{
						FirstRunXBEFile << firstrunxbeh[i];
					}
					FirstRunXBEFile.close();
				}
				{
					/* Create ShadowC.img */
					int i;
					std::ofstream FatxHeaderFile(ShadowC_Location, std::ios::binary);
					for(i = 0; i < sizeof(fatxheader); i++)
					{
						FatxHeaderFile << fatxheader[i];
					}
					FatxHeaderFile.close();
					std::ofstream ofs(ShadowC_Location, std::ios::binary | std::ios::out);
					ofs.seekp((ShadowC_Size<<20) - 1);
					ofs.write("", 1);
					ofs.close();
				}
				/**/
				XKUtils::LaunchXBE(FirstRunXBE);
			}
		/*
			Update run code, this will patch UnleashX and also install the generic font, though the font install isn't needed as the generic font is used by default.
		*/
		std::ifstream updaterun(UpdateRunBin);
		if (updaterun.good())
			{
				updaterun.close();
				remove(ShadowCOFF);
				remove(xbox_xtf_File_Path);
				remove(UpdateRunBin);
				remove(FirstRunXBE);
				RemoveDirectory(PrepDir);
				CopyFile(Generic_Font_File, xbox_xtf_File_Path, 1);
				CopyFile(EnabledPNG, "E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\generic.png", NULL);
				
				/* Patch UnleashX to show unprotected C partition for NKPatcher Settings */
				std::ofstream UnleashX_XBE1(NKPatcherSettings, std::ios::binary | std::ios::in | std::ios::out);
				UnleashX_XBE1.seekp(400);
				UnleashX_XBE1.write("\x4E\x00\x4B\x00\x50\x00\x61\x00\x74\x00\x63\x00\x68\x00\x65\x00\x72\x00\x20\x00\x53\x00\x65\x00\x74\x00\x74\x00\x69\x00\x6E\x00\x67\x00\x73", 35);
				UnleashX_XBE1.seekp(2399875);
				UnleashX_XBE1.write("14", 2);
				UnleashX_XBE1.close();
				
				std::ofstream UnleashX_XBE2(ResoftmodDash, std::ios::binary | std::ios::in | std::ios::out);
				UnleashX_XBE2.seekp(400);
				UnleashX_XBE2.write("\x52\x00\x65\x00\x53\x00\x6F\x00\x66\x00\x74\x00\x6D\x00\x6F\x00\x64\x00\x20\x00\x44\x00\x61\x00\x73\x00\x68", 27);
				UnleashX_XBE2.seekp(2399875);
				UnleashX_XBE2.write("14", 2);
				CopyFile(IconPNG, "E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\modules\\resoftmod dash\\icon.png", NULL);
				UnleashX_XBE2.close();
				
				std::ofstream UnleashX_XBE3(CRescueDash, std::ios::binary | std::ios::in | std::ios::out);
				UnleashX_XBE3.seekp(400);
				UnleashX_XBE3.write("\x43\x00\x20\x00\x52\x00\x65\x00\x63\x00\x6F\x00\x76\x00\x65\x00\x72\x00\x79\x00\x20\x00\x44\x00\x61\x00\x73\x00\x68\x00", 29);
				UnleashX_XBE3.close();
				
				std::ofstream UnleashX_XBE4(ERescueDash1, std::ios::binary | std::ios::in | std::ios::out);
				UnleashX_XBE4.seekp(400);
				UnleashX_XBE4.write("\x45\x00\x20\x00\x52\x00\x65\x00\x63\x00\x6F\x00\x76\x00\x65\x00\x72\x00\x79\x00\x20\x00\x44\x00\x61\x00\x73\x00\x68\x00", 29);
				UnleashX_XBE4.close();
				
				std::ofstream UnleashX_XBE5(ERescueDash2, std::ios::binary | std::ios::in | std::ios::out);
				UnleashX_XBE5.seekp(400);
				UnleashX_XBE5.write("\x45\x00\x20\x00\x52\x00\x65\x00\x63\x00\x6F\x00\x76\x00\x65\x00\x72\x00\x79\x00\x20\x00\x44\x00\x61\x00\x73\x00\x68\x00", 29);
				UnleashX_XBE5.close();
				
				std::ofstream UnleashX_XBE6(DashSettings, std::ios::binary | std::ios::in | std::ios::out);
				UnleashX_XBE6.seekp(400);
				UnleashX_XBE6.write("\x42\x00\x61\x00\x63\x00\x6B\x00\x75\x00\x70\x00\x2C\x00\x20\x00\x52\x00\x65\x00\x73\x00\x74\x00\x6F\x00\x72\x00\x65\x00\x20\x00\x6F\x00\x72\x00\x20\x00\x4D\x00\x6F\x00\x76\x00\x65", 45);
				CopyFile(IconPNG, "E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\modules\\dash settings\\icon.png", NULL);
				UnleashX_XBE6.close();
				/**/
				XKUtils::XBOXReset();
			}
		/*
			Update font code, this will install the kernel specific font.
		*/
		std::ifstream updatefont(Update_Font);
		if (updatefont.good())
			{
				updatefont.close();
				std::ifstream updatefontcheck(tmpFileName);
				if (updatefontcheck.good())
					{
						remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\generic.png");
						remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.3944.1.png");
						remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.4034.1.png");
						remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.4627.1.png");
						remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.4817.1.png");
						remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.5101.1.png");
						remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.5530.1.png");
						remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.5713.1.png");
						remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.5838.1.png");
						remove(xbox_xtf_File_Alt);
						remove(Update_Font);
						CopyFile(tmpFileName, xbox_xtf_File_Alt, 1);
						CopyFile(EnabledPNG, tmpFileName2, 1);
					}
				XKUtils::LaunchXBE(NKPatcherSettings);
				XKUtils::XBOXRebootToDash();
			}
		/*
			Restore font code, this will install the generic font.
		*/
		std::ifstream restorefont(Restore_Font);
		if (restorefont.good())
			{
				restorefont.close();
				std::ifstream restorefontcheck(Generic_Font_File_Alt);
				if (restorefontcheck.good())
					{
						remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\generic.png");
						remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.3944.1.png");
						remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.4034.1.png");
						remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.4627.1.png");
						remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.4817.1.png");
						remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.5101.1.png");
						remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.5530.1.png");
						remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.5713.1.png");
						remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.5838.1.png");
						remove(xbox_xtf_File_Alt);
						remove(Restore_Font);
						CopyFile(Generic_Font_File_Alt, xbox_xtf_File_Alt, 1);
						CopyFile(EnabledPNG, "E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\generic.png", NULL);
					}
				XKUtils::LaunchXBE(NKPatcherSettings);
				XKUtils::XBOXRebootToDash();
			}
	}

	//switch eeprom context Back to previous
	if (EncryptedState)
		m_pXKEEPROM->SetEncryptedEEPROMData(&currentEEPROM);
	else 
		m_pXKEEPROM->SetDecryptedEEPROMData(m_XBOX_Version, &currentEEPROM);

	// Keep config files if Override config exists
	std::ifstream overidefile(Override_File);
	if (overidefile.good())
	{
		// Reboot Xbox if Reboot.Xbox exist
		std::ifstream overidefile(Reboot_File);
		if (overidefile.good())
		{
			overidefile.close();
			XKUtils::XBOXReset();
		}
			else
		{
			overidefile.close();
			XKUtils::LaunchXBE(NKPatcherSettings);
			XKUtils::XBOXRebootToDash();
		}
	}
		else
	{
		// Reboot Xbox if Reboot.Xbox exist
		std::ifstream rebootfile(Reboot_File);
		if (rebootfile.good())
		{
			rebootfile.close();
			remove(Reboot_File);
			remove(Virtual_File);
			XKUtils::XBOXReset();
		}
			else
		{
			rebootfile.close();
			remove(Virtual_File);
			XKUtils::LaunchXBE(NKPatcherSettings);
			XKUtils::XBOXRebootToDash();
		}
	}
	
	return retVal;;
}

void ConfigMagicApp::WriteTXTInfoFile(LPCSTR strFilename)
{
	
	BOOL retVal = FALSE;
	DWORD dwBytesWrote = 0;
	CHAR tmpData[256];
	LPSTR tmpFileStr = new CHAR[2048];
	DWORD tmpSize = 256;
	ZeroMemory(tmpData, tmpSize);
	ZeroMemory(tmpFileStr, 2048);
	
	HANDLE hf = CreateFile(strFilename, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hf !=  INVALID_HANDLE_VALUE)
	{
		
		//Get Xbox version
		m_EnryptedRegionValid = TRUE;
		m_XBOX_EEPROM_Current = TRUE;
		m_XBOX_Version = m_pXKEEPROM->GetXBOXVersion();

		//Re-Encrypt with correct Version...
		m_pXKEEPROM->EncryptAndCalculateCRC(m_XBOX_Version);
		
		//Time code
		time_t rawtime;
		struct tm * timeinfo;
		time ( &rawtime );
		timeinfo = localtime ( &rawtime );
		
		//Write File Header..
		strcat(tmpFileStr, "*******  XBOX & EEPROM INFORMATION FILE\r\n");
		sprintf(tmpData, "*******  Backed up on: %s", asctime (timeinfo) );
		strcat(tmpFileStr, tmpData);

		if (m_XBOX_Version == XKEEPROM::V1_0)
			strcat(tmpFileStr, "\r\nXBOX Version:\t\t\tV1.0");
		if (m_XBOX_Version == XKEEPROM::V1_1)
			strcat(tmpFileStr, "\r\nXBOX Version:\t\t\tV1.1/2/3/4/5");
        if (m_XBOX_Version == XKEEPROM::V1_6) 
            strcat(tmpFileStr, "\r\nXBOX Version:\t\t\tV1.6");

		//Get Memory Status
		strcat(tmpFileStr, "\r\nXBOX RAM in MBytes:\t\t");
		tmpSize = 256;ZeroMemory(tmpData, tmpSize);
		MEMORYSTATUS stat;
		GlobalMemoryStatus( &stat );
		ltoa(stat.dwTotalPhys/1024/1024, tmpData, 10);
		strcat(tmpFileStr, tmpData);
		
		//Get Kernel Version
		tmpSize = 256;ZeroMemory(tmpData, tmpSize);
		sprintf(tmpData, "\r\nKernel Version:\t\t\t%d.%d.%d.%d", *((USHORT*)XboxKrnlVersion),*((USHORT*)XboxKrnlVersion+1),*((USHORT*)XboxKrnlVersion+2),*((USHORT*)XboxKrnlVersion+3));
		strcat(tmpFileStr, tmpData);

		//Write Serial Number..
		strcat(tmpFileStr, "\r\n\r\nXBOX Serial Number:\t\t");
		tmpSize = 256;ZeroMemory(tmpData, tmpSize);
		m_pXKEEPROM->GetSerialNumberString(tmpData, &tmpSize);
		strcat(tmpFileStr, tmpData);

		//Write MAC Address..
		strcat(tmpFileStr, "\r\nXBOX MAC Address:\t\t");
		tmpSize = 256;ZeroMemory(tmpData, tmpSize);
		m_pXKEEPROM->GetMACAddressString(tmpData, &tmpSize);
		strcat(tmpFileStr, tmpData);

		//Write Online Key ..
		strcat(tmpFileStr, "\r\nXBOX Online Key:\t\t");
		tmpSize = 256;ZeroMemory(tmpData, tmpSize);
		m_pXKEEPROM->GetOnlineKeyString(tmpData, &tmpSize);
		strcat(tmpFileStr, tmpData);

		//Write VideoMode ..
		strcat(tmpFileStr, "\r\nXBOX Video Mode:\t\t");
		VIDEO_STANDARD vdo = m_pXKEEPROM->GetVideoStandardVal();
		if (vdo == XKEEPROM::VIDEO_STANDARD::PAL_I)
			strcat(tmpFileStr, "PAL");
		else
			strcat(tmpFileStr, "NTSC");


		//Write XBE Region..
		strcat(tmpFileStr, "\r\nXBOX XBE Region:\t\t");
		tmpSize = 256;ZeroMemory(tmpData, tmpSize);
		m_pXKEEPROM->GetXBERegionString(tmpData, &tmpSize);
		strcat(tmpFileStr, tmpData);

		//Write HDDKey..
		strcat(tmpFileStr, "\r\nXBOX HDD Key:\t\t\t");
		tmpSize = 256;ZeroMemory(tmpData, tmpSize);
		m_pXKEEPROM->GetHDDKeyString(tmpData, &tmpSize);
		strcat(tmpFileStr, tmpData);

		//Write Confounder..
		strcat(tmpFileStr, "\r\nXBOX Confounder:\t\t");
		tmpSize = 256;ZeroMemory(tmpData, tmpSize);
		m_pXKEEPROM->GetConfounderString(tmpData, &tmpSize);
		strcat(tmpFileStr, tmpData);

		//GET HDD Info...
		//Query ATA IDENTIFY
		XKHDD::ATA_COMMAND_OBJ cmdObj;
		ZeroMemory(&cmdObj, sizeof(XKHDD::ATA_COMMAND_OBJ));
		cmdObj.IPReg.bCommandReg = IDE_ATA_IDENTIFY;
		cmdObj.IPReg.bDriveHeadReg = IDE_DEVICE_MASTER;
		XKHDD::SendATACommand(IDE_PRIMARY_PORT, &cmdObj, IDE_COMMAND_READ);
		
		//Write HDD Model
		strcat(tmpFileStr, "\r\n\r\nXBOX HDD Model:\t\t\t");
		tmpSize = 256;ZeroMemory(tmpData, tmpSize);
		XKHDD::GetIDEModel(cmdObj.DATA_BUFFER, (LPSTR)tmpData, &tmpSize);
		strcat(tmpFileStr, tmpData);

		//Write HDD Serial..
		strcat(tmpFileStr, "\r\nXBOX HDD Serial:\t\t");
		tmpSize = 256;ZeroMemory(tmpData, tmpSize);
		XKHDD::GetIDESerial(cmdObj.DATA_BUFFER, (LPSTR)tmpData, &tmpSize);
		strcat(tmpFileStr, tmpData);
		
		//Write HDD Password..
		tmpSize = 256;ZeroMemory(tmpData, tmpSize);
		strcat(tmpFileStr, "\r\n\r\nXBOX HDD Password:\t\t");

		//Need decrypted HDD Key to calculate password !!
		BOOL OldState = m_pXKEEPROM->IsEncrypted();
		if (OldState)
			m_pXKEEPROM->Decrypt();
		XKEEPROM::EEPROMDATA tmptEEP;
		m_pXKEEPROM->GetEEPROMData(&tmptEEP);
		tmpSize = 256;ZeroMemory(tmpData, tmpSize);
		BYTE HDDpwd[20];ZeroMemory(HDDpwd, 20);
		XKHDD::GenerateHDDPwd((UCHAR*)&tmptEEP.HDDKkey, cmdObj.DATA_BUFFER, (UCHAR*)&HDDpwd);
		XKGeneral::BytesToHexStr(HDDpwd, 20, tmpData);
		strcat(tmpFileStr, tmpData);
		if (OldState)
			m_pXKEEPROM->EncryptAndCalculateCRC(m_XBOX_Version);

		//Query ATAPI IDENTIFY
		ZeroMemory(&cmdObj, sizeof(XKHDD::ATA_COMMAND_OBJ));
		cmdObj.IPReg.bCommandReg = IDE_ATAPI_IDENTIFY;
		cmdObj.IPReg.bDriveHeadReg = IDE_DEVICE_SLAVE;
		XKHDD::SendATACommand(IDE_PRIMARY_PORT, &cmdObj, IDE_COMMAND_READ);
		
		//Write DVD Model
		strcat(tmpFileStr, "\r\n\r\nXBOX DVD Model:\t\t\t");
		tmpSize = 256;ZeroMemory(tmpData, tmpSize);
		XKHDD::GetIDEModel(cmdObj.DATA_BUFFER, (LPSTR)tmpData, &tmpSize);
		strcat(tmpFileStr, tmpData);

		strupr(tmpFileStr);
		
		WriteFile(hf, tmpFileStr, (DWORD)strlen(tmpFileStr), &dwBytesWrote, NULL);

	}

	delete[] tmpFileStr;
	CloseHandle(hf);

}

void ConfigMagicApp::WriteKERNELInfoFile(LPCSTR strFilename)
{
	
	BOOL retVal = FALSE;
	DWORD dwBytesWrote = 0;
	CHAR tmpData[256];
	LPSTR tmpFileStr = new CHAR[2048];
	DWORD tmpSize = 256;
	ZeroMemory(tmpData, tmpSize);
	ZeroMemory(tmpFileStr, 2048);
	
	HANDLE hf = CreateFile(strFilename, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hf !=  INVALID_HANDLE_VALUE)
	{
		//Get Kernel Version
		tmpSize = 256;ZeroMemory(tmpData, tmpSize);
		sprintf(tmpData, "");
		strcat(tmpFileStr, tmpData);
		strupr(tmpFileStr);
		
		WriteFile(hf, tmpFileStr, (DWORD)strlen(tmpFileStr), &dwBytesWrote, NULL);

	}

	delete[] tmpFileStr;
	CloseHandle(hf);

}

void ConfigMagicApp::LoadSettingsFromINI()
{
	//Read all the User Type Config Values from the INI file...
	
	UCHAR tmpINIItem[FILENAME_MAX];
	DWORD tmpLen = 256;
	ZeroMemory(m_Reboot, FILENAME_MAX);

	tmpLen = FILENAME_MAX;
	ZeroMemory(tmpINIItem, tmpLen);
	if (XKGeneral::ReadINIFileItem(MainConfigFileName, "SETTINGS", "REBOOT", (LPSTR)tmpINIItem, &tmpLen))
	{
		XKGeneral::StripQuotes((LPSTR)tmpINIItem, &tmpLen);	
		strncpy((LPSTR)m_Reboot, (LPCSTR)tmpINIItem, tmpLen);
	}
	else
		strcpy((LPSTR)m_Reboot, "FALSE");
}