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
#define IconPNG						NKPSDir					"modules\\2016 softmod tool kit\\icon.png"
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
				UnleashX_XBE1.write("\x58\x42\x45\x48\xEE\xA6\x63\xEB\xEB\x63\x86\xF8\xE6\x60\x74\xDB\xE1\x12\x99\xB7\xC3\x46\x7D\x05\x13\x59\x33\x82\x4E\x3B\xB6\xA7\xA5\x22\x2E\xF6\x90\x05\xF7\xDE\x1B\x33\xCA\xA2\x75\x8F\xA9\xC9\x74\x6F\x3C\x06\xDF\x58\xF6\x51\xCF\x8A\x5B\x86\x39\xD5\x00\xA0\x2D\x8C\xF6\xBF\x83\x4A\x0A\xFB\x86\x81\x2B\xE5\x90\x2F\x9E\x76\x52\x93\xF4\x1A\x35\x02\xDF\xF6\xDB\x4E\x54\xC3\x62\x43\xE4\x1A\xA5\x46\x21\x33\x19\x80\x79\x1D\x9D\x1F\x84\x04\xB8\x0D\x01\xDC\xD9\x5D\x42\x91\x24\x17\x54\x99\x35\xFB\xDC\x96\x32\x4A\x8B\xFF\xAD\x8B\xCE\x9B\x01\x5D\xC2\x63\x8C\x43\xE7\xFD\xDE\xD2\x11\x0D\x5D\x8E\x3C\x94\xC9\x9D\x8E\xFF\x55\x8A\x7F\x3C\x93\x65\x28\xFC\x33\xE5\x68\x0C\xDA\x2C\xB0\x12\xBF\xA5\xE6\xC1\x33\xF6\xFC\x61\xDF\x38\xA2\xD3\xA8\xD2\xAA\xF5\x2C\x7B\xD3\x42\x71\x7D\x3F\xEC\x66\x3A\x61\x56\x6B\xC6\xD0\x4D\x4E\x4A\xC9\xF4\xB6\xCC\x02\x74\x6A\x6B\xFA\xF1\xD2\xB6\x9F\xB8\x2F\xB7\xA2\x5A\x13\x09\x84\x19\x8C\x48\x3C\x6D\xF9\xC2\x19\x1D\xF1\x7A\xDD\xF2\x6F\xAE\x70\xD8\x18\x9E\xDD\x37\xB5\xC7\xB0\xDE\xD1\x56\x32\x51\xD6\xF2\x03\x17\xD6\x0A\xAF\x42", 260);
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
				UnleashX_XBE2.write("\x58\x42\x45\x48\x50\x83\x1C\xD9\x77\x0B\xA1\xDB\x5B\xFE\x6F\xED\xAB\xDF\xB8\xCB\x96\xFE\xD3\x78\x22\x37\xA4\x48\xAC\x21\x15\x2F\xE9\x45\x43\x45\xE9\x9E\xA8\x49\x84\xDA\x36\xDC\x53\x00\x6F\x38\xE5\xF7\x10\x6B\xDA\xBD\x73\xB7\x57\x34\xB0\x64\x73\xDB\xC6\xBB\x62\x7B\xBA\x3C\xEE\xAF\xC4\x53\x1A\x10\xC4\x6C\xEC\x8C\xFD\xC8\xBE\xF9\x9A\xF9\xF4\x0A\x22\x1B\xF0\x12\x68\x87\xE5\x1A\x73\x9B\x5C\x55\x2E\xD8\xC4\x06\xAF\xD9\xC6\xFC\x6D\xB3\xD1\x49\x6E\x7B\x75\x9E\xB6\xA4\xEE\xA8\x15\x58\x72\x9A\x64\xEE\x95\x96\x19\x0C\x26\x09\xF8\x88\x15\xB7\xF6\xA4\xE7\xE2\x16\xA2\xB2\x64\x0E\x9A\x5D\x7C\xCE\x42\xEB\x63\xC6\xFF\x3B\xCA\x93\xC2\x55\xA0\x97\xD9\xAF\x70\xE9\xB0\x2E\xF8\x1B\xDA\xC1\x0D\xF1\xD1\xAD\x3D\x3D\xAB\xEE\x00\xEC\x82\x66\x18\x95\x8C\xB2\x07\x7A\x78\x3E\x91\xF8\x74\xF4\x12\x81\xF2\xF6\xE1\xC2\x51\x9C\x83\xFA\xCE\x46\xE8\xB0\x5C\xD5\xE9\x00\xA3\x45\xEE\xFD\x12\x9F\xE2\xDD\xE3\x06\x1C\x68\x92\x6F\xE3\x20\x83\x43\xB5\x7E\xC3\xBC\x6C\xBD\x72\x08\x1F\xA5\xF9\xE0\xF0\x09\xA6\x05\x1F\xB7\x46\x53\xA2\x1C\xED\x1E\x91\x5D\x80\xFC\x82\x90\x13", 260);
				// Change XBE Title ( Resoftmod Dash )
				UnleashX_XBE2.seekp(400);
				UnleashX_XBE2.write("\x52\x00\x65\x00\x53\x00\x6F\x00\x66\x00\x74\x00\x6D\x00\x6F\x00\x64\x00\x20\x00\x44\x00\x61\x00\x73\x00\x68", 27);
				CopyFile(IconPNG, "E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\modules\\resoftmod dash\\icon.png", NULL);
				UnleashX_XBE2.close();
				
				std::ofstream UnleashX_XBE3(CRescueDash, std::ios::binary | std::ios::in | std::ios::out);
				// Habibi Signature
				UnleashX_XBE3.seekp(0);
				UnleashX_XBE3.write("\x58\x42\x45\x48\xAC\xEF\xBC\x72\xFA\xC0\xF0\x16\x01\xB8\x54\x15\x8F\x10\x65\xE6\x3E\x7F\x81\x34\xDD\xD5\x88\x55\x43\x16\x17\x76\x86\xD9\xEA\xC3\x49\x41\xB1\xC9\xED\xA2\x6D\xAA\xDF\xA4\xA4\x6F\xA6\x00\xD2\x52\xA9\x92\x3F\x3B\xFD\xB8\x64\x6C\xCE\x70\x4A\x4E\x76\x91\xC7\x5D\xE7\x91\xBF\x27\xAA\x69\x6D\xA1\x00\x9D\xF8\xBC\xC6\xFE\xC5\xDD\x12\x3A\xEF\xF8\xF3\xDD\x7B\x49\xC8\x30\x69\x4F\xF3\xB1\xE5\x70\xE0\x20\x7F\xC2\x4A\x86\xAB\x8B\xD1\x7C\x07\x0B\x4A\xAD\x94\x55\xA4\x36\x8C\x53\x86\x89\x0E\xE2\xD6\xA3\xC8\x93\x53\x32\x03\xB3\xF4\x31\xAE\x2E\xCE\xF9\xE9\x0C\x70\x1C\x4A\x34\x0D\x6E\x97\x66\x43\x91\xBC\xF5\x46\x89\xAF\x99\xFF\x7A\x79\xB8\x65\xA1\x47\x27\x5B\xC9\x6D\xF1\x20\x0D\xA8\xBA\x7E\x20\x1C\x8C\xEC\x87\x9B\x24\x2D\x81\x6A\xF3\xD9\xFE\xED\xA9\x65\xA4\x1C\xD9\x78\x17\x86\x77\xAF\x5E\x7B\x1F\x29\x5A\x45\x2F\x45\x2D\xF8\x18\xCE\x2C\x6F\x25\x2A\x9B\x0A\xCF\xBD\x4D\xD5\xBC\xB7\xDD\xF6\x3A\xE8\xEE\x89\x95\x26\x2C\x18\x56\x0E\x87\x85\x6B\xFF\x2F\xC1\x84\x02\x96\xD1\x7C\xDC\xC8\x39\x1A\x2F\xD3\xD8\xC4\x2E\x29\x8D\xD4\x5B\x92\x75\x66", 260);
				// Change XBE Title ( C Recovery Dash )
				UnleashX_XBE3.seekp(400);
				UnleashX_XBE3.write("\x43\x00\x20\x00\x52\x00\x65\x00\x63\x00\x6F\x00\x76\x00\x65\x00\x72\x00\x79\x00\x20\x00\x44\x00\x61\x00\x73\x00\x68\x00", 29);
				UnleashX_XBE3.close();
				
				std::ofstream UnleashX_XBE4(ERescueDash1, std::ios::binary | std::ios::in | std::ios::out);
				// Habibi Signature
				UnleashX_XBE4.seekp(0);
				UnleashX_XBE4.write("\x58\x42\x45\x48\x2B\x85\x34\x15\x7E\xC6\x0A\xBF\xA1\xD4\xC2\xFD\x36\xCF\xAD\xD9\xEB\xF1\x00\x76\x69\x58\x2C\xFD\x37\x02\xD5\x7A\x96\x4E\x5F\xBF\x97\x97\x73\xE4\xF0\x26\x79\x06\x2C\xCC\x29\x06\xEF\x58\xC0\xE4\x03\xDF\x4E\x71\xF8\x2B\x5F\x42\x91\x32\x22\x22\xE3\x30\x6F\x66\x51\xB4\x98\x4C\xB2\x44\x4F\x89\xDE\x79\x83\x1C\xB8\x44\x8B\x53\xDD\x1D\xC0\xB7\xC5\xB1\xB0\x2D\xFD\x01\xA3\x2D\x98\xD9\x37\x0E\x81\x02\x25\x20\x69\x3A\x13\x48\x99\x07\xF1\x82\x5D\x60\x25\xEA\x8E\x71\x5B\x35\xEB\x89\xD5\xCF\x71\x8D\x71\x1B\x05\x94\x88\x1B\x52\xF2\x37\x6F\x37\x2A\x1E\x14\x5F\x22\x77\xC8\xCB\xF4\x1D\x21\x88\xB0\xBE\xF2\x83\xA4\x17\x12\x76\x7C\xB4\xFB\xE5\xE3\x91\xB1\x3F\x1C\x75\xAF\x31\x02\x44\x5A\x18\xB5\xA8\x08\x7B\x0E\x11\xB0\x43\x60\xDC\x93\x48\x9A\xFA\xFE\xE1\x68\x6F\xBA\x79\x03\xED\x6C\xB2\xFE\x0A\xC1\x0A\xE8\x6E\x0B\xDC\xBB\x40\xB1\xA8\xBB\xD3\x89\x53\xD0\x71\x01\x3B\xF0\xA1\x2C\xC9\xC8\xF3\xE5\x61\xCB\xD8\x4D\x87\x04\x1D\x13\xF0\x1E\x8B\xA0\x38\xB5\xD5\xA2\x15\xD5\xF8\x72\x1E\xA7\x34\x74\xA7\x65\xAA\x5A\x7A\x6A\xA1\x29\xDE\x3A\xCA\x27", 260);
				// Change XBE Title ( E Recovery Dash )
				UnleashX_XBE4.seekp(400);
				UnleashX_XBE4.write("\x45\x00\x20\x00\x52\x00\x65\x00\x63\x00\x6F\x00\x76\x00\x65\x00\x72\x00\x79\x00\x20\x00\x44\x00\x61\x00\x73\x00\x68\x00", 29);
				UnleashX_XBE4.close();
				
				std::ofstream UnleashX_XBE5(ERescueDash2, std::ios::binary | std::ios::in | std::ios::out);
				// Habibi Signature
				UnleashX_XBE5.seekp(0);
				UnleashX_XBE5.write("\x58\x42\x45\x48\x2B\x85\x34\x15\x7E\xC6\x0A\xBF\xA1\xD4\xC2\xFD\x36\xCF\xAD\xD9\xEB\xF1\x00\x76\x69\x58\x2C\xFD\x37\x02\xD5\x7A\x96\x4E\x5F\xBF\x97\x97\x73\xE4\xF0\x26\x79\x06\x2C\xCC\x29\x06\xEF\x58\xC0\xE4\x03\xDF\x4E\x71\xF8\x2B\x5F\x42\x91\x32\x22\x22\xE3\x30\x6F\x66\x51\xB4\x98\x4C\xB2\x44\x4F\x89\xDE\x79\x83\x1C\xB8\x44\x8B\x53\xDD\x1D\xC0\xB7\xC5\xB1\xB0\x2D\xFD\x01\xA3\x2D\x98\xD9\x37\x0E\x81\x02\x25\x20\x69\x3A\x13\x48\x99\x07\xF1\x82\x5D\x60\x25\xEA\x8E\x71\x5B\x35\xEB\x89\xD5\xCF\x71\x8D\x71\x1B\x05\x94\x88\x1B\x52\xF2\x37\x6F\x37\x2A\x1E\x14\x5F\x22\x77\xC8\xCB\xF4\x1D\x21\x88\xB0\xBE\xF2\x83\xA4\x17\x12\x76\x7C\xB4\xFB\xE5\xE3\x91\xB1\x3F\x1C\x75\xAF\x31\x02\x44\x5A\x18\xB5\xA8\x08\x7B\x0E\x11\xB0\x43\x60\xDC\x93\x48\x9A\xFA\xFE\xE1\x68\x6F\xBA\x79\x03\xED\x6C\xB2\xFE\x0A\xC1\x0A\xE8\x6E\x0B\xDC\xBB\x40\xB1\xA8\xBB\xD3\x89\x53\xD0\x71\x01\x3B\xF0\xA1\x2C\xC9\xC8\xF3\xE5\x61\xCB\xD8\x4D\x87\x04\x1D\x13\xF0\x1E\x8B\xA0\x38\xB5\xD5\xA2\x15\xD5\xF8\x72\x1E\xA7\x34\x74\xA7\x65\xAA\x5A\x7A\x6A\xA1\x29\xDE\x3A\xCA\x27", 260);
				// Change XBE Title ( E Recovery Dash )
				UnleashX_XBE5.seekp(400);
				UnleashX_XBE5.write("\x45\x00\x20\x00\x52\x00\x65\x00\x63\x00\x6F\x00\x76\x00\x65\x00\x72\x00\x79\x00\x20\x00\x44\x00\x61\x00\x73\x00\x68\x00", 29);
				UnleashX_XBE5.close();
				
				std::ofstream UnleashX_XBE6(DashSettings, std::ios::binary | std::ios::in | std::ios::out);
				// Habibi Signature
				UnleashX_XBE6.seekp(0);
				UnleashX_XBE6.write("\x58\x42\x45\x48\x4F\x87\x99\x18\x93\xD0\x6F\xCD\x5C\xF9\xBA\xE5\xDD\x99\x02\x35\x7C\x61\xED\xC6\xC4\x76\x45\x1D\xA0\x58\xEC\x50\x85\xDB\x8B\xCB\xFB\xFB\x05\xB3\xF1\x25\x72\x00\xF2\x3C\xC7\xA3\x78\x2B\x1F\x2A\x7C\x65\xD4\xD7\x69\xE2\xC0\x0E\x61\xF6\x31\xE8\x3A\xBC\xEA\xEA\xFB\x67\x80\x41\x4A\x8D\x6A\xF1\x8C\xAA\x8D\x2B\xFF\x6D\x71\x24\x24\x34\x04\xD6\x08\xD4\xD2\x85\x1B\x7F\x99\x70\x22\x62\x4E\xDD\x08\x74\x21\x01\x5D\x92\x0B\xAA\x5C\x95\x7B\x4E\xF8\x5F\x16\x2E\x72\xAD\xA6\x38\x87\x6B\xE5\x6C\xDD\x65\xAD\xDB\x5E\x73\xB3\x38\xEA\x64\x17\xF5\x8A\x3F\x26\x11\x8D\x20\x47\xAF\x70\x27\xE5\x3E\xAB\x0E\x52\x0C\x93\x79\x14\x79\xA2\x2C\x6F\xF1\x5F\x0C\x43\x55\x31\xD8\x7A\x37\xED\xF4\xB0\x71\x3E\xEB\x22\xFC\x48\x11\x7A\x42\x69\xA4\x1F\x42\x18\x9E\x05\x5A\x33\x21\x1F\x82\x66\x97\xF6\x82\x01\x98\x07\x8D\xBA\x24\x9A\xD2\x4A\x46\xB8\xD9\x17\x57\x35\xFD\x16\xE5\xAE\x8A\x98\xC5\xF5\xF9\x0D\x6B\x8F\xF7\x46\xA5\xF9\x0F\x06\xD0\x32\x76\x5E\x12\x5D\x8B\x9F\xA9\x46\xB2\x8C\xF7\xAD\x8E\x9F\xDE\x46\xBC\x17\x7B\x11\x96\x82\x8A\x90\xC0\x90\xF5\x8C\x04", 260);
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
				UnleashX_XBE1.write("\x58\x42\x45\x48\xEE\xA6\x63\xEB\xEB\x63\x86\xF8\xE6\x60\x74\xDB\xE1\x12\x99\xB7\xC3\x46\x7D\x05\x13\x59\x33\x82\x4E\x3B\xB6\xA7\xA5\x22\x2E\xF6\x90\x05\xF7\xDE\x1B\x33\xCA\xA2\x75\x8F\xA9\xC9\x74\x6F\x3C\x06\xDF\x58\xF6\x51\xCF\x8A\x5B\x86\x39\xD5\x00\xA0\x2D\x8C\xF6\xBF\x83\x4A\x0A\xFB\x86\x81\x2B\xE5\x90\x2F\x9E\x76\x52\x93\xF4\x1A\x35\x02\xDF\xF6\xDB\x4E\x54\xC3\x62\x43\xE4\x1A\xA5\x46\x21\x33\x19\x80\x79\x1D\x9D\x1F\x84\x04\xB8\x0D\x01\xDC\xD9\x5D\x42\x91\x24\x17\x54\x99\x35\xFB\xDC\x96\x32\x4A\x8B\xFF\xAD\x8B\xCE\x9B\x01\x5D\xC2\x63\x8C\x43\xE7\xFD\xDE\xD2\x11\x0D\x5D\x8E\x3C\x94\xC9\x9D\x8E\xFF\x55\x8A\x7F\x3C\x93\x65\x28\xFC\x33\xE5\x68\x0C\xDA\x2C\xB0\x12\xBF\xA5\xE6\xC1\x33\xF6\xFC\x61\xDF\x38\xA2\xD3\xA8\xD2\xAA\xF5\x2C\x7B\xD3\x42\x71\x7D\x3F\xEC\x66\x3A\x61\x56\x6B\xC6\xD0\x4D\x4E\x4A\xC9\xF4\xB6\xCC\x02\x74\x6A\x6B\xFA\xF1\xD2\xB6\x9F\xB8\x2F\xB7\xA2\x5A\x13\x09\x84\x19\x8C\x48\x3C\x6D\xF9\xC2\x19\x1D\xF1\x7A\xDD\xF2\x6F\xAE\x70\xD8\x18\x9E\xDD\x37\xB5\xC7\xB0\xDE\xD1\x56\x32\x51\xD6\xF2\x03\x17\xD6\x0A\xAF\x42", 260);
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
				UnleashX_XBE2.write("\x58\x42\x45\x48\x50\x83\x1C\xD9\x77\x0B\xA1\xDB\x5B\xFE\x6F\xED\xAB\xDF\xB8\xCB\x96\xFE\xD3\x78\x22\x37\xA4\x48\xAC\x21\x15\x2F\xE9\x45\x43\x45\xE9\x9E\xA8\x49\x84\xDA\x36\xDC\x53\x00\x6F\x38\xE5\xF7\x10\x6B\xDA\xBD\x73\xB7\x57\x34\xB0\x64\x73\xDB\xC6\xBB\x62\x7B\xBA\x3C\xEE\xAF\xC4\x53\x1A\x10\xC4\x6C\xEC\x8C\xFD\xC8\xBE\xF9\x9A\xF9\xF4\x0A\x22\x1B\xF0\x12\x68\x87\xE5\x1A\x73\x9B\x5C\x55\x2E\xD8\xC4\x06\xAF\xD9\xC6\xFC\x6D\xB3\xD1\x49\x6E\x7B\x75\x9E\xB6\xA4\xEE\xA8\x15\x58\x72\x9A\x64\xEE\x95\x96\x19\x0C\x26\x09\xF8\x88\x15\xB7\xF6\xA4\xE7\xE2\x16\xA2\xB2\x64\x0E\x9A\x5D\x7C\xCE\x42\xEB\x63\xC6\xFF\x3B\xCA\x93\xC2\x55\xA0\x97\xD9\xAF\x70\xE9\xB0\x2E\xF8\x1B\xDA\xC1\x0D\xF1\xD1\xAD\x3D\x3D\xAB\xEE\x00\xEC\x82\x66\x18\x95\x8C\xB2\x07\x7A\x78\x3E\x91\xF8\x74\xF4\x12\x81\xF2\xF6\xE1\xC2\x51\x9C\x83\xFA\xCE\x46\xE8\xB0\x5C\xD5\xE9\x00\xA3\x45\xEE\xFD\x12\x9F\xE2\xDD\xE3\x06\x1C\x68\x92\x6F\xE3\x20\x83\x43\xB5\x7E\xC3\xBC\x6C\xBD\x72\x08\x1F\xA5\xF9\xE0\xF0\x09\xA6\x05\x1F\xB7\x46\x53\xA2\x1C\xED\x1E\x91\x5D\x80\xFC\x82\x90\x13", 260);
				// Change XBE Title ( Resoftmod Dash )
				UnleashX_XBE2.seekp(400);
				UnleashX_XBE2.write("\x52\x00\x65\x00\x53\x00\x6F\x00\x66\x00\x74\x00\x6D\x00\x6F\x00\x64\x00\x20\x00\x44\x00\x61\x00\x73\x00\x68", 27);
				CopyFile(IconPNG, "E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\modules\\resoftmod dash\\icon.png", NULL);
				UnleashX_XBE2.close();
				
				std::ofstream UnleashX_XBE3(CRescueDash, std::ios::binary | std::ios::in | std::ios::out);
				// Habibi Signature
				UnleashX_XBE3.seekp(0);
				UnleashX_XBE3.write("\x58\x42\x45\x48\xAC\xEF\xBC\x72\xFA\xC0\xF0\x16\x01\xB8\x54\x15\x8F\x10\x65\xE6\x3E\x7F\x81\x34\xDD\xD5\x88\x55\x43\x16\x17\x76\x86\xD9\xEA\xC3\x49\x41\xB1\xC9\xED\xA2\x6D\xAA\xDF\xA4\xA4\x6F\xA6\x00\xD2\x52\xA9\x92\x3F\x3B\xFD\xB8\x64\x6C\xCE\x70\x4A\x4E\x76\x91\xC7\x5D\xE7\x91\xBF\x27\xAA\x69\x6D\xA1\x00\x9D\xF8\xBC\xC6\xFE\xC5\xDD\x12\x3A\xEF\xF8\xF3\xDD\x7B\x49\xC8\x30\x69\x4F\xF3\xB1\xE5\x70\xE0\x20\x7F\xC2\x4A\x86\xAB\x8B\xD1\x7C\x07\x0B\x4A\xAD\x94\x55\xA4\x36\x8C\x53\x86\x89\x0E\xE2\xD6\xA3\xC8\x93\x53\x32\x03\xB3\xF4\x31\xAE\x2E\xCE\xF9\xE9\x0C\x70\x1C\x4A\x34\x0D\x6E\x97\x66\x43\x91\xBC\xF5\x46\x89\xAF\x99\xFF\x7A\x79\xB8\x65\xA1\x47\x27\x5B\xC9\x6D\xF1\x20\x0D\xA8\xBA\x7E\x20\x1C\x8C\xEC\x87\x9B\x24\x2D\x81\x6A\xF3\xD9\xFE\xED\xA9\x65\xA4\x1C\xD9\x78\x17\x86\x77\xAF\x5E\x7B\x1F\x29\x5A\x45\x2F\x45\x2D\xF8\x18\xCE\x2C\x6F\x25\x2A\x9B\x0A\xCF\xBD\x4D\xD5\xBC\xB7\xDD\xF6\x3A\xE8\xEE\x89\x95\x26\x2C\x18\x56\x0E\x87\x85\x6B\xFF\x2F\xC1\x84\x02\x96\xD1\x7C\xDC\xC8\x39\x1A\x2F\xD3\xD8\xC4\x2E\x29\x8D\xD4\x5B\x92\x75\x66", 260);
				// Change XBE Title ( C Recovery Dash )
				UnleashX_XBE3.seekp(400);
				UnleashX_XBE3.write("\x43\x00\x20\x00\x52\x00\x65\x00\x63\x00\x6F\x00\x76\x00\x65\x00\x72\x00\x79\x00\x20\x00\x44\x00\x61\x00\x73\x00\x68\x00", 29);
				UnleashX_XBE3.close();
				
				std::ofstream UnleashX_XBE4(ERescueDash1, std::ios::binary | std::ios::in | std::ios::out);
				// Habibi Signature
				UnleashX_XBE4.seekp(0);
				UnleashX_XBE4.write("\x58\x42\x45\x48\x2B\x85\x34\x15\x7E\xC6\x0A\xBF\xA1\xD4\xC2\xFD\x36\xCF\xAD\xD9\xEB\xF1\x00\x76\x69\x58\x2C\xFD\x37\x02\xD5\x7A\x96\x4E\x5F\xBF\x97\x97\x73\xE4\xF0\x26\x79\x06\x2C\xCC\x29\x06\xEF\x58\xC0\xE4\x03\xDF\x4E\x71\xF8\x2B\x5F\x42\x91\x32\x22\x22\xE3\x30\x6F\x66\x51\xB4\x98\x4C\xB2\x44\x4F\x89\xDE\x79\x83\x1C\xB8\x44\x8B\x53\xDD\x1D\xC0\xB7\xC5\xB1\xB0\x2D\xFD\x01\xA3\x2D\x98\xD9\x37\x0E\x81\x02\x25\x20\x69\x3A\x13\x48\x99\x07\xF1\x82\x5D\x60\x25\xEA\x8E\x71\x5B\x35\xEB\x89\xD5\xCF\x71\x8D\x71\x1B\x05\x94\x88\x1B\x52\xF2\x37\x6F\x37\x2A\x1E\x14\x5F\x22\x77\xC8\xCB\xF4\x1D\x21\x88\xB0\xBE\xF2\x83\xA4\x17\x12\x76\x7C\xB4\xFB\xE5\xE3\x91\xB1\x3F\x1C\x75\xAF\x31\x02\x44\x5A\x18\xB5\xA8\x08\x7B\x0E\x11\xB0\x43\x60\xDC\x93\x48\x9A\xFA\xFE\xE1\x68\x6F\xBA\x79\x03\xED\x6C\xB2\xFE\x0A\xC1\x0A\xE8\x6E\x0B\xDC\xBB\x40\xB1\xA8\xBB\xD3\x89\x53\xD0\x71\x01\x3B\xF0\xA1\x2C\xC9\xC8\xF3\xE5\x61\xCB\xD8\x4D\x87\x04\x1D\x13\xF0\x1E\x8B\xA0\x38\xB5\xD5\xA2\x15\xD5\xF8\x72\x1E\xA7\x34\x74\xA7\x65\xAA\x5A\x7A\x6A\xA1\x29\xDE\x3A\xCA\x27", 260);
				// Change XBE Title ( E Recovery Dash )
				UnleashX_XBE4.seekp(400);
				UnleashX_XBE4.write("\x45\x00\x20\x00\x52\x00\x65\x00\x63\x00\x6F\x00\x76\x00\x65\x00\x72\x00\x79\x00\x20\x00\x44\x00\x61\x00\x73\x00\x68\x00", 29);
				UnleashX_XBE4.close();
				
				std::ofstream UnleashX_XBE5(ERescueDash2, std::ios::binary | std::ios::in | std::ios::out);
				// Habibi Signature
				UnleashX_XBE5.seekp(0);
				UnleashX_XBE5.write("\x58\x42\x45\x48\x2B\x85\x34\x15\x7E\xC6\x0A\xBF\xA1\xD4\xC2\xFD\x36\xCF\xAD\xD9\xEB\xF1\x00\x76\x69\x58\x2C\xFD\x37\x02\xD5\x7A\x96\x4E\x5F\xBF\x97\x97\x73\xE4\xF0\x26\x79\x06\x2C\xCC\x29\x06\xEF\x58\xC0\xE4\x03\xDF\x4E\x71\xF8\x2B\x5F\x42\x91\x32\x22\x22\xE3\x30\x6F\x66\x51\xB4\x98\x4C\xB2\x44\x4F\x89\xDE\x79\x83\x1C\xB8\x44\x8B\x53\xDD\x1D\xC0\xB7\xC5\xB1\xB0\x2D\xFD\x01\xA3\x2D\x98\xD9\x37\x0E\x81\x02\x25\x20\x69\x3A\x13\x48\x99\x07\xF1\x82\x5D\x60\x25\xEA\x8E\x71\x5B\x35\xEB\x89\xD5\xCF\x71\x8D\x71\x1B\x05\x94\x88\x1B\x52\xF2\x37\x6F\x37\x2A\x1E\x14\x5F\x22\x77\xC8\xCB\xF4\x1D\x21\x88\xB0\xBE\xF2\x83\xA4\x17\x12\x76\x7C\xB4\xFB\xE5\xE3\x91\xB1\x3F\x1C\x75\xAF\x31\x02\x44\x5A\x18\xB5\xA8\x08\x7B\x0E\x11\xB0\x43\x60\xDC\x93\x48\x9A\xFA\xFE\xE1\x68\x6F\xBA\x79\x03\xED\x6C\xB2\xFE\x0A\xC1\x0A\xE8\x6E\x0B\xDC\xBB\x40\xB1\xA8\xBB\xD3\x89\x53\xD0\x71\x01\x3B\xF0\xA1\x2C\xC9\xC8\xF3\xE5\x61\xCB\xD8\x4D\x87\x04\x1D\x13\xF0\x1E\x8B\xA0\x38\xB5\xD5\xA2\x15\xD5\xF8\x72\x1E\xA7\x34\x74\xA7\x65\xAA\x5A\x7A\x6A\xA1\x29\xDE\x3A\xCA\x27", 260);
				// Change XBE Title ( E Recovery Dash )
				UnleashX_XBE5.seekp(400);
				UnleashX_XBE5.write("\x45\x00\x20\x00\x52\x00\x65\x00\x63\x00\x6F\x00\x76\x00\x65\x00\x72\x00\x79\x00\x20\x00\x44\x00\x61\x00\x73\x00\x68\x00", 29);
				UnleashX_XBE5.close();
				
				std::ofstream UnleashX_XBE6(DashSettings, std::ios::binary | std::ios::in | std::ios::out);
				// Habibi Signature
				UnleashX_XBE6.seekp(0);
				UnleashX_XBE6.write("\x58\x42\x45\x48\x4F\x87\x99\x18\x93\xD0\x6F\xCD\x5C\xF9\xBA\xE5\xDD\x99\x02\x35\x7C\x61\xED\xC6\xC4\x76\x45\x1D\xA0\x58\xEC\x50\x85\xDB\x8B\xCB\xFB\xFB\x05\xB3\xF1\x25\x72\x00\xF2\x3C\xC7\xA3\x78\x2B\x1F\x2A\x7C\x65\xD4\xD7\x69\xE2\xC0\x0E\x61\xF6\x31\xE8\x3A\xBC\xEA\xEA\xFB\x67\x80\x41\x4A\x8D\x6A\xF1\x8C\xAA\x8D\x2B\xFF\x6D\x71\x24\x24\x34\x04\xD6\x08\xD4\xD2\x85\x1B\x7F\x99\x70\x22\x62\x4E\xDD\x08\x74\x21\x01\x5D\x92\x0B\xAA\x5C\x95\x7B\x4E\xF8\x5F\x16\x2E\x72\xAD\xA6\x38\x87\x6B\xE5\x6C\xDD\x65\xAD\xDB\x5E\x73\xB3\x38\xEA\x64\x17\xF5\x8A\x3F\x26\x11\x8D\x20\x47\xAF\x70\x27\xE5\x3E\xAB\x0E\x52\x0C\x93\x79\x14\x79\xA2\x2C\x6F\xF1\x5F\x0C\x43\x55\x31\xD8\x7A\x37\xED\xF4\xB0\x71\x3E\xEB\x22\xFC\x48\x11\x7A\x42\x69\xA4\x1F\x42\x18\x9E\x05\x5A\x33\x21\x1F\x82\x66\x97\xF6\x82\x01\x98\x07\x8D\xBA\x24\x9A\xD2\x4A\x46\xB8\xD9\x17\x57\x35\xFD\x16\xE5\xAE\x8A\x98\xC5\xF5\xF9\x0D\x6B\x8F\xF7\x46\xA5\xF9\x0F\x06\xD0\x32\x76\x5E\x12\x5D\x8B\x9F\xA9\x46\xB2\x8C\xF7\xAD\x8E\x9F\xDE\x46\xBC\x17\x7B\x11\x96\x82\x8A\x90\xC0\x90\xF5\x8C\x04", 260);
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