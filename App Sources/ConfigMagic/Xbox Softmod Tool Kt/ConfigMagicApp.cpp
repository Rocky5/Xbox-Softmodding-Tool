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
* Used with the 2014/16 Softmod kit.
*
******************************************************************************************************
*/

// ConfigMagicApp.cpp: implementation of the ConfigMagicApp class.
//
//////////////////////////////////////////////////////////////////////

#include "ConfigMagicApp.h"
//#include "firstrunxbe.h"
#include "ShadowCHeader.h"
#include "xkhdd.h"
#include "xkeeprom.h"
#include <fstream>
#include <time.h>
#include <cstdio>


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
#define RunDashboard										"C:\\Dashboard\\default.xbe"
#define DashboardXBE				NKPSDir					"configs\\alt xbe's\\dashboard.xbe"
#define DashboardXML				NKPSDir					"configs\\dash.xml"
#define DashboardSkinXML			NKPSDir					"Skins\\Dashboard\\skin.xml"
#define DashboardSkinwideXML		NKPSDir					"Skins\\Dashboard\\skinwide.xml"
#define DashboardSkinevoxXML		NKPSDir					"Skins\\Dashboard\\skinevox.xml"
#define DashboardSkinSplash			NKPSDir					"Skins\\Dashboard\\UXSplash.jpg"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enable/Disable png files
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define EnabledPNG					NKPSDir					"toggles\\enabled.png"
#define IconPNG						NKPSDir					"modules\\Xbox softmod tool kit\\icon.png"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Setup Kernel selection for first run of softmod
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define FirstRunXBE					NKPDir					"default.xbe"
#define FirstRunLOG					NKPDir					"default.log"
#define FirstRunCFG					NKPDir					"default.cfg"
#define FirstinstallCFG				NKPDir					"dashloader\\firstinstall.cfg"
#define PrepXBE						PrepDir					"default.xbe"
#define SecondRun					PrepDir					"secondrun.bin"
#define FirstRunBin											"D:\\firstrun.bin"
#define UpdateRunBin										"D:\\updaterun.bin"
#define NKP_XBE						NKPConfDir				"nkpatcher\\6.xbe"
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
		remove(PrepXBE);
		remove(SecondRun);
		RemoveDirectory(PrepDir);
		CreateDirectory("C:\\Dashboard", NULL);
		CreateDirectory("C:\\Dashboard\\Skins", NULL);
		CreateDirectory("C:\\Dashboard\\Skins\\Softmod", NULL);
		CreateDirectory("C:\\Dashboard\\Skins\\Softmod Wide", NULL);
		CreateDirectory("C:\\Dashboard\\Skins\\Softmod Evox", NULL);
		CopyFile(DashboardXBE, "C:\\Dashboard\\default.xbe", NULL);
		CopyFile(DashboardXML, "C:\\Dashboard\\config.xml", NULL);
		CopyFile(DashboardSkinXML, "C:\\Dashboard\\Skins\\Softmod\\skin.xml", NULL);
		CopyFile(DashboardSkinwideXML, "C:\\Dashboard\\Skins\\Softmod Wide\\skin.xml", NULL);
		CopyFile(DashboardSkinevoxXML, "C:\\Dashboard\\Skins\\Softmod Evox\\skin.xml", NULL);
		CopyFile(DashboardSkinSplash, "C:\\Dashboard\\Skins\\Softmod\\UXSplash.jpg", NULL);
		CopyFile(DashboardSkinSplash, "C:\\Dashboard\\Skins\\Softmod Wide\\UXSplash.jpg", NULL);
		CopyFile(DashboardSkinSplash, "C:\\Dashboard\\Skins\\Softmod Evox\\UXSplash.jpg", NULL);
		XKUtils::LaunchXBE(RunDashboard);
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
		/* Change the serial number to V-EEPROM ENB */
		std::ofstream V_EEPROM(Virtual_Full_Path, std::ios::binary | std::ios::in | std::ios::out);
		V_EEPROM.seekp(52);
		V_EEPROM.write("\x56\x2D\x45\x45\x50\x52\x4F\x4D\x20\x45\x4E\x42", 12);
		V_EEPROM.close();
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
			First run code, this will patch UnleashX, create a clean ShadowC.img and also install the generic font, though the font install isn't needed as the generic font is used by default.
		*/
		std::ifstream stream(FirstRunBin);
		if (stream.good())
			{	
				stream.close();
				remove(ShadowCOFF);
				remove(FirstRunBin);
					//remove(FirstRunXBE);
					//remove(FirstRunLOG);
					//remove(FirstRunCFG);
					//remove(FirstinstallCFG);
					//CopyFile(NKP_XBE, FirstRunXBE, NULL);
				//remove(xbox_xtf_File_Path);
				//CopyFile(Generic_Font_File, xbox_xtf_File_Path, 1);
				CopyFile(EnabledPNG, SecondRun, NULL);
				CopyFile(EnabledPNG, "E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\generic.png", NULL);

				// Patch UnleashX to show unprotected C partition for NKPatcher Settings
				std::ofstream UnleashX_XBE1(NKPatcherSettings, std::ios::binary | std::ios::in | std::ios::out);
				// Habibi Signature
				UnleashX_XBE1.seekp(0);
				UnleashX_XBE1.write("\x58\x42\x45\x48\x6D\x5B\x31\xDF\x32\xF3\xAF\x7F\xD7\xF9\xFB\xC1\xD7\x9C\xE1\x19\xEB\x41\x6A\x60\x7C\x50\xFC\xAB\x0E\x27\x0D\x67\xEA\xBD\xC9\x21\x5F\x83\x7F\x1D\xC1\x8B\x59\xD4\x9F\x3C\xF9\x5A\x31\x6C\x1C\x36\xA7\x11\xD3\xB2\xAD\x43\xDE\xA3\xDB\xDB\x83\xC9\x3D\x04\xE7\x09\xB5\x7C\x32\xF1\x1D\xD0\x86\x7C\x1B\xFE\xDD\x58\x96\xDC\x9A\xBC\xBB\x46\xDD\xCF\x49\xCE\xDA\x66\x19\x2D\xBA\xD9\x91\x40\x2A\x94\xD6\x6D\x29\x5E\x8F\xA7\xBC\x20\xBD\x3A\x3F\x66\x24\xEB\xB8\xB0\xD8\x8D\xDC\x1E\x24\xD1\x75\x2C\xA5\xAF\xDB\x68\x70\xA2\x1A\xE4\xED\x34\x43\x0F\xC6\x77\x49\xD5\xAD\xA7\xD3\x11\x2B\x22\xFA\x11\x1F\x3D\xC4\x33\x0C\x07\x00\xAC\xEF\xA8\xBB\x24\x03\xD1\x7C\x94\x41\xFA\xA2\xE7\x43\x9F\x82\x03\xAA\x77\xD8\x79\x83\x70\x45\xAE\x18\x14\x8E\x9B\x62\x9A\x73\x32\x4D\xC2\xF5\x55\xA1\x60\x13\x8C\x0E\xD4\x65\x34\x3C\x19\xA8\x04\x2F\xD0\x4A\xA9\x92\x03\xAF\xAA\xA1\x3D\x9B\x5B\x47\x84\x4E\x1A\xE2\x3F\x0D\xFF\x19\x73\x96\xCB\x04\x1D\x14\x4D\xA6\x61\x46\x6C\xDC\x34\x39\x06\x84\xFF\xFB\xFA\x38\x2D\x4B\x5C\xA8\x5A\x8D\x7C\x95\xED\xE4\x3C\x4C\xC7\x43\x22", 260);
				// Change XBE Title ( NKPatcher Settings )
				UnleashX_XBE1.seekp(400);
				UnleashX_XBE1.write("\x4E\x00\x4B\x00\x50\x00\x61\x00\x74\x00\x63\x00\x68\x00\x65\x00\x72\x00\x20\x00\x53\x00\x65\x00\x74\x00\x74\x00\x69\x00\x6E\x00\x67\x00\x73", 35);
				// XBE Hash
				UnleashX_XBE1.seekp(1532);
				UnleashX_XBE1.write("\x05\xC1\xB2\xD7\xFD\x5F\x54\xD3\xF7\x14\x9C\xCA\x8C\x5A\x31\xD6\x5C\x7D\xB1\xED", 20);
				// Remove XBE TitleImage.xbx parsing
				UnleashX_XBE1.seekp(2353334);
				UnleashX_XBE1.write("\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 14);
				// Remove Items.xml
				UnleashX_XBE1.seekp(2355128);
				UnleashX_XBE1.write("\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 12);
				// Change C partition number
				UnleashX_XBE1.seekp(2399875);
				UnleashX_XBE1.write("14", 2);
				UnleashX_XBE1.close();
				
				std::ofstream UnleashX_XBE2(ResoftmodDash, std::ios::binary | std::ios::in | std::ios::out);
				// Habibi Signature
				UnleashX_XBE2.seekp(0);
				UnleashX_XBE2.write("\x58\x42\x45\x48\xBB\x2F\xF7\xD2\x62\x60\xC7\x79\x99\x10\x4B\x5D\x30\xE2\xC7\x18\x58\x4C\x10\x70\xB1\xEE\x63\xCA\x3B\x6B\x49\x1F\x75\x59\x06\x91\xCF\x19\xCB\xAF\x75\xD3\xE1\x64\x0B\x49\xAD\x8F\x4A\x35\xFF\x97\x93\x6A\xD7\x58\x42\xEB\x57\xFE\xA0\xD7\xF8\x83\xE5\xAF\x8C\xCD\x0B\xAF\x4C\xD5\x57\x64\xF8\x4F\xD8\x2D\xE2\x86\x12\xE5\x7B\x16\x7F\x3E\x18\xE8\xAD\xE8\xED\x32\xC5\x40\x2C\x99\xB0\x11\x7F\x10\xF3\x96\x71\xAD\x2D\xCF\x43\x34\xCF\xF9\x18\x02\x7E\x4D\x2C\xAB\x20\x89\xE7\x7E\x61\x63\x57\xC1\x05\xB3\x8C\xC6\x8A\x95\xBB\xCD\x1A\x9E\xC6\xA6\xB1\x85\x95\xFE\xEE\x62\x7F\x25\x17\xF6\x26\xFB\xD6\x73\x3A\xAD\x42\xA1\xD0\x05\x9B\x7F\xA0\x54\xEB\x91\x48\xAA\x9E\xD9\x94\x33\x4B\xFE\x56\xB3\x18\x9A\x0A\x83\x59\x51\x84\xE6\xFE\x92\xB9\xCE\xCD\xDC\x4D\x82\x7F\x17\xE2\x61\x43\xC7\x7D\xAC\xB9\x0E\x29\x26\x57\xF9\x96\xC4\x38\xC5\x3D\xF2\xC1\xD2\x8E\x69\x62\xC7\x45\x2B\x63\x01\x27\x2F\xA4\x63\xA4\x82\x46\xDE\x1C\x4F\x4E\x6E\x0F\x35\x37\x7D\x72\xE9\x5E\x5B\x9B\xF8\x5A\x48\x5A\xBC\xF3\xC8\xEA\x4C\xC4\x49\xEE\x61\xC5\x37\xCD\xA6\x16\xCD\x02\x75", 260);
				// Change XBE Title ( Resoftmod Dash )
				UnleashX_XBE2.seekp(400);
				UnleashX_XBE2.write("\x52\x00\x65\x00\x53\x00\x6F\x00\x66\x00\x74\x00\x6D\x00\x6F\x00\x64\x00\x20\x00\x44\x00\x61\x00\x73\x00\x68", 27);
				CopyFile(IconPNG, "E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\modules\\resoftmod dash\\icon.png", NULL);
				UnleashX_XBE2.close();
				
				std::ofstream UnleashX_XBE3(CRescueDash, std::ios::binary | std::ios::in | std::ios::out);
				// Habibi Signature
				UnleashX_XBE3.seekp(0);
				UnleashX_XBE3.write("\x58\x42\x45\x48\x1A\xB1\x2E\x57\x6F\x08\x51\x64\x82\x12\xBA\x01\x5D\x55\x05\x40\x0F\x9E\x88\x91\x8F\x5F\xA4\xE3\xB9\x66\xB5\xFC\x35\x00\x2F\x2D\xCC\x8E\x1B\xB1\x68\xE5\x17\x18\x6D\x04\x5C\x3F\xDE\xFB\x3B\xBA\xCA\x0E\xE2\x4B\xB7\x75\xDD\x6E\xEC\xE5\xBC\x38\xBF\x07\x13\x4B\x99\x97\x02\x89\xA4\x7C\x1D\x95\x5B\xB0\xD9\x72\x01\xCC\xB8\xB5\x01\x96\xAB\x92\x0D\xE4\xCF\xDB\x80\x9F\x40\xE1\x3B\xCC\x42\x72\xCC\x08\xFC\x43\xA9\x38\x36\x42\x2E\x4C\x33\x4C\xB8\x4E\x99\xA5\x8C\x06\x3C\xD1\xE6\x22\x49\x22\x82\xFA\x2E\xD4\xBC\x4E\xBD\xF2\xBD\xF3\x2E\x87\xB7\xFE\x97\x3C\x22\x8B\x05\x7C\xA4\xFA\xE9\x24\x20\x24\x3C\x0D\xF5\xEF\x39\xA7\xEF\xF1\x61\x50\x06\xD0\x4B\x88\xA4\xDF\x7B\xAF\xAA\xD6\xD6\xCA\x59\x6F\xDA\xB6\x6A\xF3\x07\x21\x95\xEE\xD1\x0D\x7F\x3F\x55\xF1\x76\xB2\x11\x15\x05\xBA\xB4\x15\xEF\x6C\x4B\x48\x81\x21\xEA\xF5\xC9\xFD\x77\x0F\x85\x85\x23\xCB\x51\x95\x55\x5B\x11\x09\x76\x22\x1D\x0D\xB8\x1B\xDE\xE2\x70\x85\x14\x5C\x4E\xA2\x00\xDF\xAB\x31\x24\xB7\x88\x50\x34\x38\x84\xAD\x1E\x51\x43\x9A\x11\x9E\x62\x28\xE1\xEE\xBF\x26\x7F\xEF\x54\x30", 260);
				// Change XBE Title ( C Recovery Dash )
				UnleashX_XBE3.seekp(400);
				UnleashX_XBE3.write("\x43\x00\x20\x00\x52\x00\x65\x00\x63\x00\x6F\x00\x76\x00\x65\x00\x72\x00\x79\x00\x20\x00\x44\x00\x61\x00\x73\x00\x68\x00", 29);
				UnleashX_XBE3.close();
				
				std::ofstream UnleashX_XBE4(ERescueDash1, std::ios::binary | std::ios::in | std::ios::out);
				// Habibi Signature
				UnleashX_XBE4.seekp(0);
				UnleashX_XBE4.write("\x58\x42\x45\x48\xFC\x37\x2D\xD9\x07\xD0\x14\x6C\x6E\x05\x2A\xB6\x8D\x84\xF2\xAF\x13\xA7\x55\x0F\x0D\xEF\xF1\xEF\xBF\x7A\x41\x39\x74\x2E\x82\xDB\xAC\x1B\xC1\xCB\xEB\x2A\x30\x6F\x3C\x21\x65\x0B\x40\x78\x0B\x03\x7A\x87\xE2\x6D\x79\xDC\xDF\xB6\xAC\xC5\x62\x3F\x79\xA1\xD1\xD0\xBF\x76\x3C\xF8\x00\x9F\x34\xD9\x2F\x01\xD4\x3C\xD0\x06\x15\xF9\x10\xB9\xB9\xFB\x19\x85\x7A\x58\xFD\xC5\x68\x12\xD8\xEC\x67\x76\x67\xBD\x03\x9D\xF2\x5E\x5F\xFE\x4E\xC0\x4B\x51\x29\xB9\x91\x63\x03\xE4\x73\xE7\x80\x51\xAE\xBB\xE6\x0B\xB1\xF6\x68\xE3\x5F\xA0\x27\x4E\x85\xCE\x7A\x26\x67\xF4\x32\xE4\x55\xEF\xE3\x89\x00\xA8\x0A\x3A\xB0\xD6\x1C\xFF\x70\xC7\xF1\xC1\x09\x2B\xF8\x42\x9C\xA9\xC5\xB6\x74\x3C\xAB\x41\x8C\x53\xB8\x52\xFC\xAD\x40\xBA\x9C\x83\x16\x85\xE1\xB8\x29\xF2\x48\xEC\xFE\x4C\x28\x73\x06\xE1\x8F\x6A\x21\xD5\x09\xA6\x82\x7C\xB1\xE0\x90\xD4\xBA\x68\xDE\xA2\x57\xBD\x95\x74\xD5\x11\x9F\x7A\x87\x65\x86\x83\xB7\x4F\xF2\x63\xFE\x0F\xAD\xF0\x4E\x4F\x91\x33\x88\x72\x6D\x5F\x2A\xFA\x9B\x2A\x25\xE5\xDB\x3C\x38\xFE\x36\x04\xE4\x8B\xFD\x93\xA0\x65\x49\xE0\x7C\x5B", 260);
				// Change XBE Title ( E Recovery Dash )
				UnleashX_XBE4.seekp(400);
				UnleashX_XBE4.write("\x45\x00\x20\x00\x52\x00\x65\x00\x63\x00\x6F\x00\x76\x00\x65\x00\x72\x00\x79\x00\x20\x00\x44\x00\x61\x00\x73\x00\x68\x00", 29);
				UnleashX_XBE4.close();
				
				std::ofstream UnleashX_XBE5(ERescueDash2, std::ios::binary | std::ios::in | std::ios::out);
				// Habibi Signature
				UnleashX_XBE5.seekp(0);
				UnleashX_XBE5.write("\x58\x42\x45\x48\xFC\x37\x2D\xD9\x07\xD0\x14\x6C\x6E\x05\x2A\xB6\x8D\x84\xF2\xAF\x13\xA7\x55\x0F\x0D\xEF\xF1\xEF\xBF\x7A\x41\x39\x74\x2E\x82\xDB\xAC\x1B\xC1\xCB\xEB\x2A\x30\x6F\x3C\x21\x65\x0B\x40\x78\x0B\x03\x7A\x87\xE2\x6D\x79\xDC\xDF\xB6\xAC\xC5\x62\x3F\x79\xA1\xD1\xD0\xBF\x76\x3C\xF8\x00\x9F\x34\xD9\x2F\x01\xD4\x3C\xD0\x06\x15\xF9\x10\xB9\xB9\xFB\x19\x85\x7A\x58\xFD\xC5\x68\x12\xD8\xEC\x67\x76\x67\xBD\x03\x9D\xF2\x5E\x5F\xFE\x4E\xC0\x4B\x51\x29\xB9\x91\x63\x03\xE4\x73\xE7\x80\x51\xAE\xBB\xE6\x0B\xB1\xF6\x68\xE3\x5F\xA0\x27\x4E\x85\xCE\x7A\x26\x67\xF4\x32\xE4\x55\xEF\xE3\x89\x00\xA8\x0A\x3A\xB0\xD6\x1C\xFF\x70\xC7\xF1\xC1\x09\x2B\xF8\x42\x9C\xA9\xC5\xB6\x74\x3C\xAB\x41\x8C\x53\xB8\x52\xFC\xAD\x40\xBA\x9C\x83\x16\x85\xE1\xB8\x29\xF2\x48\xEC\xFE\x4C\x28\x73\x06\xE1\x8F\x6A\x21\xD5\x09\xA6\x82\x7C\xB1\xE0\x90\xD4\xBA\x68\xDE\xA2\x57\xBD\x95\x74\xD5\x11\x9F\x7A\x87\x65\x86\x83\xB7\x4F\xF2\x63\xFE\x0F\xAD\xF0\x4E\x4F\x91\x33\x88\x72\x6D\x5F\x2A\xFA\x9B\x2A\x25\xE5\xDB\x3C\x38\xFE\x36\x04\xE4\x8B\xFD\x93\xA0\x65\x49\xE0\x7C\x5B", 260);
				// Change XBE Title ( E Recovery Dash )
				UnleashX_XBE5.seekp(400);
				UnleashX_XBE5.write("\x45\x00\x20\x00\x52\x00\x65\x00\x63\x00\x6F\x00\x76\x00\x65\x00\x72\x00\x79\x00\x20\x00\x44\x00\x61\x00\x73\x00\x68\x00", 29);
				UnleashX_XBE5.close();
				
				std::ofstream UnleashX_XBE6(DashSettings, std::ios::binary | std::ios::in | std::ios::out);
				// Habibi Signature
				UnleashX_XBE6.seekp(0);
				UnleashX_XBE6.write("\x58\x42\x45\x48\xCB\x12\xE6\x4D\x75\x55\x87\x52\x7B\x90\x17\x4B\x53\xCC\xAD\x75\x91\x5B\xED\x95\xDA\x55\x3E\x0B\xF8\x3B\x5D\x3E\xC7\x99\x3D\x69\x3D\xD8\xBA\xA1\x4E\xE9\x9B\x7F\x24\x48\x24\xEB\x93\xA2\x7A\xD5\x6D\x22\xBF\xF1\x21\x88\x25\x3E\x04\xFA\x9A\x58\x39\x12\xDD\x1D\xD5\x06\xA6\x3A\xF6\x9A\xC2\xA3\x43\xDF\x0F\xC8\xA8\xF0\x44\xC7\xEA\x96\xB0\xD8\x81\x7D\x28\x82\xF7\x6A\xD6\x97\x31\x19\xC5\xC4\xBB\xED\x4A\xE0\xA8\xBE\x3D\xFC\x24\xC0\x77\x55\x19\x56\xCD\x27\x9A\x95\x43\x96\x23\x4C\x61\xC8\x86\x36\x17\xDD\x89\xE0\x8D\x95\x36\x85\xBB\x00\xC4\x1F\x7B\x31\x98\xCE\x00\xA8\xB2\x5A\x03\x3C\xA4\xF1\x9E\xA6\xF2\xAB\xE9\x50\x11\xF1\x69\x9B\x40\x64\x47\xCE\x7A\xF6\xCA\x68\xCB\x50\x3A\xA3\x66\x87\xB2\x8C\x7D\x2F\x76\x5F\xD5\x24\x3B\x64\x8C\xC4\x94\xF5\xA1\x0A\xFF\xD9\x7C\x5A\xDE\x05\xA6\x64\x96\xE6\x70\x8B\xCD\x44\x01\xB0\x18\x40\x88\xAA\xCB\xD2\x46\x77\xC4\xCB\x55\xBB\x4B\x40\x18\x7E\x90\x56\x36\x31\x87\xF6\xC5\x25\xAF\x01\x5E\x4F\x32\xAF\xB8\xBF\xC9\x49\xF8\xE0\x94\x06\xA1\x27\x96\x87\x6D\xD8\x95\x40\x04\xA1\xE8\x2C\xB2\xEB\x78\x79", 260);
				// Change XBE Title ( Backup, Restore or Move )
				UnleashX_XBE6.seekp(400);
				UnleashX_XBE6.write("\x42\x00\x61\x00\x63\x00\x6B\x00\x75\x00\x70\x00\x2C\x00\x20\x00\x52\x00\x65\x00\x73\x00\x74\x00\x6F\x00\x72\x00\x65\x00\x20\x00\x6F\x00\x72\x00\x20\x00\x4D\x00\x6F\x00\x76\x00\x65", 45);
				CopyFile(IconPNG, "E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\modules\\dash settings\\icon.png", NULL);
				UnleashX_XBE6.close();
				/**/
					//{
					//	/* Create firstrun.xbe */
					//	CreateDirectory("E:\\Prep", NULL);
					//	CopyFile(EnabledPNG, SecondRun, NULL);
					//	int i;
					//	std::ofstream FirstRunXBEFile(FirstRunXBE, std::ios::binary);
					//	for(i = 0; i < sizeof(firstrunxbeh); i++)
					//	{
					//		FirstRunXBEFile << firstrunxbeh[i];
					//	}
					//	FirstRunXBEFile.close();
					//}
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
				XKUtils::XBOXReset();
			}
		/*
			Update run code, this will patch UnleashX and also install the generic font, though the font install isn't needed as the generic font is used by default.
		*/
		std::ifstream updaterun(UpdateRunBin);
		if (updaterun.good())
			{
				updaterun.close();
				remove(ShadowCOFF);
				remove(UpdateRunBin);
				remove(PrepXBE);
				RemoveDirectory(PrepDir);
				//remove(xbox_xtf_File_Path);
				//CopyFile(Generic_Font_File, xbox_xtf_File_Path, 1);
				CopyFile(EnabledPNG, "E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\generic.png", NULL);

				// Patch UnleashX to show unprotected C partition for NKPatcher Settings
				std::ofstream UnleashX_XBE1(NKPatcherSettings, std::ios::binary | std::ios::in | std::ios::out);
				// Habibi Signature
				UnleashX_XBE1.seekp(0);
				UnleashX_XBE1.write("\x58\x42\x45\x48\x6D\x5B\x31\xDF\x32\xF3\xAF\x7F\xD7\xF9\xFB\xC1\xD7\x9C\xE1\x19\xEB\x41\x6A\x60\x7C\x50\xFC\xAB\x0E\x27\x0D\x67\xEA\xBD\xC9\x21\x5F\x83\x7F\x1D\xC1\x8B\x59\xD4\x9F\x3C\xF9\x5A\x31\x6C\x1C\x36\xA7\x11\xD3\xB2\xAD\x43\xDE\xA3\xDB\xDB\x83\xC9\x3D\x04\xE7\x09\xB5\x7C\x32\xF1\x1D\xD0\x86\x7C\x1B\xFE\xDD\x58\x96\xDC\x9A\xBC\xBB\x46\xDD\xCF\x49\xCE\xDA\x66\x19\x2D\xBA\xD9\x91\x40\x2A\x94\xD6\x6D\x29\x5E\x8F\xA7\xBC\x20\xBD\x3A\x3F\x66\x24\xEB\xB8\xB0\xD8\x8D\xDC\x1E\x24\xD1\x75\x2C\xA5\xAF\xDB\x68\x70\xA2\x1A\xE4\xED\x34\x43\x0F\xC6\x77\x49\xD5\xAD\xA7\xD3\x11\x2B\x22\xFA\x11\x1F\x3D\xC4\x33\x0C\x07\x00\xAC\xEF\xA8\xBB\x24\x03\xD1\x7C\x94\x41\xFA\xA2\xE7\x43\x9F\x82\x03\xAA\x77\xD8\x79\x83\x70\x45\xAE\x18\x14\x8E\x9B\x62\x9A\x73\x32\x4D\xC2\xF5\x55\xA1\x60\x13\x8C\x0E\xD4\x65\x34\x3C\x19\xA8\x04\x2F\xD0\x4A\xA9\x92\x03\xAF\xAA\xA1\x3D\x9B\x5B\x47\x84\x4E\x1A\xE2\x3F\x0D\xFF\x19\x73\x96\xCB\x04\x1D\x14\x4D\xA6\x61\x46\x6C\xDC\x34\x39\x06\x84\xFF\xFB\xFA\x38\x2D\x4B\x5C\xA8\x5A\x8D\x7C\x95\xED\xE4\x3C\x4C\xC7\x43\x22", 260);
				// Change XBE Title ( NKPatcher Settings )
				UnleashX_XBE1.seekp(400);
				UnleashX_XBE1.write("\x4E\x00\x4B\x00\x50\x00\x61\x00\x74\x00\x63\x00\x68\x00\x65\x00\x72\x00\x20\x00\x53\x00\x65\x00\x74\x00\x74\x00\x69\x00\x6E\x00\x67\x00\x73", 35);
				// XBE Hash
				UnleashX_XBE1.seekp(1532);
				UnleashX_XBE1.write("\x05\xC1\xB2\xD7\xFD\x5F\x54\xD3\xF7\x14\x9C\xCA\x8C\x5A\x31\xD6\x5C\x7D\xB1\xED", 20);
				// Remove XBE TitleImage.xbx parsing
				UnleashX_XBE1.seekp(2353334);
				UnleashX_XBE1.write("\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 14);
				// Remove Items.xml
				UnleashX_XBE1.seekp(2355128);
				UnleashX_XBE1.write("\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 12);
				// Change C partition number
				UnleashX_XBE1.seekp(2399875);
				UnleashX_XBE1.write("14", 2);
				UnleashX_XBE1.close();
				
				std::ofstream UnleashX_XBE2(ResoftmodDash, std::ios::binary | std::ios::in | std::ios::out);
				// Habibi Signature
				UnleashX_XBE2.seekp(0);
				UnleashX_XBE2.write("\x58\x42\x45\x48\xBB\x2F\xF7\xD2\x62\x60\xC7\x79\x99\x10\x4B\x5D\x30\xE2\xC7\x18\x58\x4C\x10\x70\xB1\xEE\x63\xCA\x3B\x6B\x49\x1F\x75\x59\x06\x91\xCF\x19\xCB\xAF\x75\xD3\xE1\x64\x0B\x49\xAD\x8F\x4A\x35\xFF\x97\x93\x6A\xD7\x58\x42\xEB\x57\xFE\xA0\xD7\xF8\x83\xE5\xAF\x8C\xCD\x0B\xAF\x4C\xD5\x57\x64\xF8\x4F\xD8\x2D\xE2\x86\x12\xE5\x7B\x16\x7F\x3E\x18\xE8\xAD\xE8\xED\x32\xC5\x40\x2C\x99\xB0\x11\x7F\x10\xF3\x96\x71\xAD\x2D\xCF\x43\x34\xCF\xF9\x18\x02\x7E\x4D\x2C\xAB\x20\x89\xE7\x7E\x61\x63\x57\xC1\x05\xB3\x8C\xC6\x8A\x95\xBB\xCD\x1A\x9E\xC6\xA6\xB1\x85\x95\xFE\xEE\x62\x7F\x25\x17\xF6\x26\xFB\xD6\x73\x3A\xAD\x42\xA1\xD0\x05\x9B\x7F\xA0\x54\xEB\x91\x48\xAA\x9E\xD9\x94\x33\x4B\xFE\x56\xB3\x18\x9A\x0A\x83\x59\x51\x84\xE6\xFE\x92\xB9\xCE\xCD\xDC\x4D\x82\x7F\x17\xE2\x61\x43\xC7\x7D\xAC\xB9\x0E\x29\x26\x57\xF9\x96\xC4\x38\xC5\x3D\xF2\xC1\xD2\x8E\x69\x62\xC7\x45\x2B\x63\x01\x27\x2F\xA4\x63\xA4\x82\x46\xDE\x1C\x4F\x4E\x6E\x0F\x35\x37\x7D\x72\xE9\x5E\x5B\x9B\xF8\x5A\x48\x5A\xBC\xF3\xC8\xEA\x4C\xC4\x49\xEE\x61\xC5\x37\xCD\xA6\x16\xCD\x02\x75", 260);
				// Change XBE Title ( Resoftmod Dash )
				UnleashX_XBE2.seekp(400);
				UnleashX_XBE2.write("\x52\x00\x65\x00\x53\x00\x6F\x00\x66\x00\x74\x00\x6D\x00\x6F\x00\x64\x00\x20\x00\x44\x00\x61\x00\x73\x00\x68", 27);
				CopyFile(IconPNG, "E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\modules\\resoftmod dash\\icon.png", NULL);
				UnleashX_XBE2.close();
				
				std::ofstream UnleashX_XBE3(CRescueDash, std::ios::binary | std::ios::in | std::ios::out);
				// Habibi Signature
				UnleashX_XBE3.seekp(0);
				UnleashX_XBE3.write("\x58\x42\x45\x48\x1A\xB1\x2E\x57\x6F\x08\x51\x64\x82\x12\xBA\x01\x5D\x55\x05\x40\x0F\x9E\x88\x91\x8F\x5F\xA4\xE3\xB9\x66\xB5\xFC\x35\x00\x2F\x2D\xCC\x8E\x1B\xB1\x68\xE5\x17\x18\x6D\x04\x5C\x3F\xDE\xFB\x3B\xBA\xCA\x0E\xE2\x4B\xB7\x75\xDD\x6E\xEC\xE5\xBC\x38\xBF\x07\x13\x4B\x99\x97\x02\x89\xA4\x7C\x1D\x95\x5B\xB0\xD9\x72\x01\xCC\xB8\xB5\x01\x96\xAB\x92\x0D\xE4\xCF\xDB\x80\x9F\x40\xE1\x3B\xCC\x42\x72\xCC\x08\xFC\x43\xA9\x38\x36\x42\x2E\x4C\x33\x4C\xB8\x4E\x99\xA5\x8C\x06\x3C\xD1\xE6\x22\x49\x22\x82\xFA\x2E\xD4\xBC\x4E\xBD\xF2\xBD\xF3\x2E\x87\xB7\xFE\x97\x3C\x22\x8B\x05\x7C\xA4\xFA\xE9\x24\x20\x24\x3C\x0D\xF5\xEF\x39\xA7\xEF\xF1\x61\x50\x06\xD0\x4B\x88\xA4\xDF\x7B\xAF\xAA\xD6\xD6\xCA\x59\x6F\xDA\xB6\x6A\xF3\x07\x21\x95\xEE\xD1\x0D\x7F\x3F\x55\xF1\x76\xB2\x11\x15\x05\xBA\xB4\x15\xEF\x6C\x4B\x48\x81\x21\xEA\xF5\xC9\xFD\x77\x0F\x85\x85\x23\xCB\x51\x95\x55\x5B\x11\x09\x76\x22\x1D\x0D\xB8\x1B\xDE\xE2\x70\x85\x14\x5C\x4E\xA2\x00\xDF\xAB\x31\x24\xB7\x88\x50\x34\x38\x84\xAD\x1E\x51\x43\x9A\x11\x9E\x62\x28\xE1\xEE\xBF\x26\x7F\xEF\x54\x30", 260);
				// Change XBE Title ( C Recovery Dash )
				UnleashX_XBE3.seekp(400);
				UnleashX_XBE3.write("\x43\x00\x20\x00\x52\x00\x65\x00\x63\x00\x6F\x00\x76\x00\x65\x00\x72\x00\x79\x00\x20\x00\x44\x00\x61\x00\x73\x00\x68\x00", 29);
				UnleashX_XBE3.close();
				
				std::ofstream UnleashX_XBE4(ERescueDash1, std::ios::binary | std::ios::in | std::ios::out);
				// Habibi Signature
				UnleashX_XBE4.seekp(0);
				UnleashX_XBE4.write("\x58\x42\x45\x48\xFC\x37\x2D\xD9\x07\xD0\x14\x6C\x6E\x05\x2A\xB6\x8D\x84\xF2\xAF\x13\xA7\x55\x0F\x0D\xEF\xF1\xEF\xBF\x7A\x41\x39\x74\x2E\x82\xDB\xAC\x1B\xC1\xCB\xEB\x2A\x30\x6F\x3C\x21\x65\x0B\x40\x78\x0B\x03\x7A\x87\xE2\x6D\x79\xDC\xDF\xB6\xAC\xC5\x62\x3F\x79\xA1\xD1\xD0\xBF\x76\x3C\xF8\x00\x9F\x34\xD9\x2F\x01\xD4\x3C\xD0\x06\x15\xF9\x10\xB9\xB9\xFB\x19\x85\x7A\x58\xFD\xC5\x68\x12\xD8\xEC\x67\x76\x67\xBD\x03\x9D\xF2\x5E\x5F\xFE\x4E\xC0\x4B\x51\x29\xB9\x91\x63\x03\xE4\x73\xE7\x80\x51\xAE\xBB\xE6\x0B\xB1\xF6\x68\xE3\x5F\xA0\x27\x4E\x85\xCE\x7A\x26\x67\xF4\x32\xE4\x55\xEF\xE3\x89\x00\xA8\x0A\x3A\xB0\xD6\x1C\xFF\x70\xC7\xF1\xC1\x09\x2B\xF8\x42\x9C\xA9\xC5\xB6\x74\x3C\xAB\x41\x8C\x53\xB8\x52\xFC\xAD\x40\xBA\x9C\x83\x16\x85\xE1\xB8\x29\xF2\x48\xEC\xFE\x4C\x28\x73\x06\xE1\x8F\x6A\x21\xD5\x09\xA6\x82\x7C\xB1\xE0\x90\xD4\xBA\x68\xDE\xA2\x57\xBD\x95\x74\xD5\x11\x9F\x7A\x87\x65\x86\x83\xB7\x4F\xF2\x63\xFE\x0F\xAD\xF0\x4E\x4F\x91\x33\x88\x72\x6D\x5F\x2A\xFA\x9B\x2A\x25\xE5\xDB\x3C\x38\xFE\x36\x04\xE4\x8B\xFD\x93\xA0\x65\x49\xE0\x7C\x5B", 260);
				// Change XBE Title ( E Recovery Dash )
				UnleashX_XBE4.seekp(400);
				UnleashX_XBE4.write("\x45\x00\x20\x00\x52\x00\x65\x00\x63\x00\x6F\x00\x76\x00\x65\x00\x72\x00\x79\x00\x20\x00\x44\x00\x61\x00\x73\x00\x68\x00", 29);
				UnleashX_XBE4.close();
				
				std::ofstream UnleashX_XBE5(ERescueDash2, std::ios::binary | std::ios::in | std::ios::out);
				// Habibi Signature
				UnleashX_XBE5.seekp(0);
				UnleashX_XBE5.write("\x58\x42\x45\x48\xFC\x37\x2D\xD9\x07\xD0\x14\x6C\x6E\x05\x2A\xB6\x8D\x84\xF2\xAF\x13\xA7\x55\x0F\x0D\xEF\xF1\xEF\xBF\x7A\x41\x39\x74\x2E\x82\xDB\xAC\x1B\xC1\xCB\xEB\x2A\x30\x6F\x3C\x21\x65\x0B\x40\x78\x0B\x03\x7A\x87\xE2\x6D\x79\xDC\xDF\xB6\xAC\xC5\x62\x3F\x79\xA1\xD1\xD0\xBF\x76\x3C\xF8\x00\x9F\x34\xD9\x2F\x01\xD4\x3C\xD0\x06\x15\xF9\x10\xB9\xB9\xFB\x19\x85\x7A\x58\xFD\xC5\x68\x12\xD8\xEC\x67\x76\x67\xBD\x03\x9D\xF2\x5E\x5F\xFE\x4E\xC0\x4B\x51\x29\xB9\x91\x63\x03\xE4\x73\xE7\x80\x51\xAE\xBB\xE6\x0B\xB1\xF6\x68\xE3\x5F\xA0\x27\x4E\x85\xCE\x7A\x26\x67\xF4\x32\xE4\x55\xEF\xE3\x89\x00\xA8\x0A\x3A\xB0\xD6\x1C\xFF\x70\xC7\xF1\xC1\x09\x2B\xF8\x42\x9C\xA9\xC5\xB6\x74\x3C\xAB\x41\x8C\x53\xB8\x52\xFC\xAD\x40\xBA\x9C\x83\x16\x85\xE1\xB8\x29\xF2\x48\xEC\xFE\x4C\x28\x73\x06\xE1\x8F\x6A\x21\xD5\x09\xA6\x82\x7C\xB1\xE0\x90\xD4\xBA\x68\xDE\xA2\x57\xBD\x95\x74\xD5\x11\x9F\x7A\x87\x65\x86\x83\xB7\x4F\xF2\x63\xFE\x0F\xAD\xF0\x4E\x4F\x91\x33\x88\x72\x6D\x5F\x2A\xFA\x9B\x2A\x25\xE5\xDB\x3C\x38\xFE\x36\x04\xE4\x8B\xFD\x93\xA0\x65\x49\xE0\x7C\x5B", 260);
				// Change XBE Title ( E Recovery Dash )
				UnleashX_XBE5.seekp(400);
				UnleashX_XBE5.write("\x45\x00\x20\x00\x52\x00\x65\x00\x63\x00\x6F\x00\x76\x00\x65\x00\x72\x00\x79\x00\x20\x00\x44\x00\x61\x00\x73\x00\x68\x00", 29);
				UnleashX_XBE5.close();
				
				std::ofstream UnleashX_XBE6(DashSettings, std::ios::binary | std::ios::in | std::ios::out);
				// Habibi Signature
				UnleashX_XBE6.seekp(0);
				UnleashX_XBE6.write("\x58\x42\x45\x48\xCB\x12\xE6\x4D\x75\x55\x87\x52\x7B\x90\x17\x4B\x53\xCC\xAD\x75\x91\x5B\xED\x95\xDA\x55\x3E\x0B\xF8\x3B\x5D\x3E\xC7\x99\x3D\x69\x3D\xD8\xBA\xA1\x4E\xE9\x9B\x7F\x24\x48\x24\xEB\x93\xA2\x7A\xD5\x6D\x22\xBF\xF1\x21\x88\x25\x3E\x04\xFA\x9A\x58\x39\x12\xDD\x1D\xD5\x06\xA6\x3A\xF6\x9A\xC2\xA3\x43\xDF\x0F\xC8\xA8\xF0\x44\xC7\xEA\x96\xB0\xD8\x81\x7D\x28\x82\xF7\x6A\xD6\x97\x31\x19\xC5\xC4\xBB\xED\x4A\xE0\xA8\xBE\x3D\xFC\x24\xC0\x77\x55\x19\x56\xCD\x27\x9A\x95\x43\x96\x23\x4C\x61\xC8\x86\x36\x17\xDD\x89\xE0\x8D\x95\x36\x85\xBB\x00\xC4\x1F\x7B\x31\x98\xCE\x00\xA8\xB2\x5A\x03\x3C\xA4\xF1\x9E\xA6\xF2\xAB\xE9\x50\x11\xF1\x69\x9B\x40\x64\x47\xCE\x7A\xF6\xCA\x68\xCB\x50\x3A\xA3\x66\x87\xB2\x8C\x7D\x2F\x76\x5F\xD5\x24\x3B\x64\x8C\xC4\x94\xF5\xA1\x0A\xFF\xD9\x7C\x5A\xDE\x05\xA6\x64\x96\xE6\x70\x8B\xCD\x44\x01\xB0\x18\x40\x88\xAA\xCB\xD2\x46\x77\xC4\xCB\x55\xBB\x4B\x40\x18\x7E\x90\x56\x36\x31\x87\xF6\xC5\x25\xAF\x01\x5E\x4F\x32\xAF\xB8\xBF\xC9\x49\xF8\xE0\x94\x06\xA1\x27\x96\x87\x6D\xD8\x95\x40\x04\xA1\xE8\x2C\xB2\xEB\x78\x79", 260);
				// Change XBE Title ( Backup, Restore or Move )
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