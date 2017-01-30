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
				UnleashX_XBE1.write("\x58\x42\x45\x48\x15\x4C\x88\x4E\x4D\x8B\xA8\xC6\xA7\x78\x63\x0C\x79\x4D\x7B\x34\x2C\x03\x26\x07\x6B\x69\x68\x15\x22\xB4\x9C\x0A\xF2\x8D\xBF\x11\x2D\x64\x40\x28\x75\xAC\x63\x12\x7B\x3F\xCA\x87\x91\xBA\x62\x86\x68\xD3\x98\xD5\x49\x75\x03\x7D\x25\x8E\x81\xB6\x1F\x96\x14\xE8\x03\xFF\xE0\x6E\x07\xFD\xFF\xD8\xA9\x93\x98\x1D\x08\xE3\xD3\xF7\x35\xC4\x80\x4D\x77\xB9\xD2\x88\xEF\x85\x18\x63\x66\x20\x82\x3C\x34\x77\x0E\x35\x46\x0E\x99\x72\xED\x6F\xBA\x99\xB9\xA6\x62\xF5\x38\x2B\x49\x92\x9E\x2A\xC8\x91\x82\xAE\x42\x9E\x99\x1E\xEC\x54\xC8\x3E\x41\x31\xDA\xA6\x96\xBB\x80\x5F\x32\xEC\x30\x52\x77\xF6\x01\xE7\xFB\x65\xB4\xF6\xF1\xDD\xBE\xB1\xBF\x1C\xD3\xB2\x96\xD6\xD7\x87\x7B\x42\xA2\x4E\xBD\xF2\x99\x5B\xE3\xF9\x62\xAA\x95\x0E\x3C\xEF\x62\xCB\x2E\x31\x1F\x9F\x64\x05\x08\x6A\x07\x35\x2A\x3D\x62\xEA\xFD\x42\xF7\x8F\x5D\xB4\x17\x33\xB5\x02\xB1\xEF\x63\x7A\x5E\x4A\x7D\xB3\xAA\xE2\x16\x93\x2E\x9C\x56\x1A\xE5\x19\xDB\xAD\x97\x86\x30\xB6\x62\x1C\x38\xCE\xC2\xE7\x14\x70\x60\x66\x27\xDC\xA4\xB7\xB2\x3D\xDD\xBA\xF1\x9A\xB0\x35\xFC\xE9\x64\x79\x51\x18", 260);
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
				UnleashX_XBE2.write("\x58\x42\x45\x48\x66\xE1\xFE\xE9\xA0\xE5\x9B\x1A\xF8\xDC\xAB\xD8\x27\x2E\x24\x54\x35\x36\xB0\xD6\x40\x8B\x92\xE6\xA6\x69\x16\x53\x1D\xF5\x86\xDA\xE0\x52\x7F\xFF\xF4\x4E\xEE\xD5\xA1\x76\x22\x9C\xF2\x37\x73\xC1\x0C\xD8\xBC\x14\x7D\xB9\x70\x98\x33\xDA\xF0\xF5\x65\x8C\x6C\x07\x4E\xB2\x06\xC1\x1E\x10\x7E\x17\x2C\x48\xDE\x43\x1C\x76\x24\x9A\xEC\xDB\x1A\xB0\xB8\x0E\xE2\x5F\x78\x2E\xEF\x3E\xB6\x51\xB7\x4B\xB8\x78\x8E\x0B\xD9\x63\xBE\x0E\x3D\x08\xA3\xC6\xA9\xEC\x51\x8E\x98\x9B\xF4\x8F\x02\x85\x00\x38\x54\x89\x3C\xDF\xB9\x3E\x0A\x0B\x6C\xD4\x09\xCC\xB8\x78\x53\x5E\xBA\x31\x80\x0D\xFB\x01\xB7\x89\x1A\xFC\x28\x15\xF0\x86\x77\x64\xC0\x23\x66\xB9\xA1\xA4\x05\x53\x85\xBE\x8C\x9A\x4D\xEF\xF0\xCB\x21\x22\x97\xBA\x5A\xE1\xC1\x69\xB5\x1C\xAC\x5F\x69\xF3\x2A\x9B\xE2\xF9\xFD\xD7\xA9\x62\x85\x5B\x7A\xA6\x9A\xF2\x89\x02\xA9\xB5\x61\x2B\x0F\x2C\x79\xFF\xF4\x15\xFC\xCC\x22\x92\x3F\x4A\x7D\xC4\xA2\xF5\xFA\xDD\xE5\xCD\x0D\x2A\x9D\x6D\x21\x9B\x4F\xF8\xAB\xA6\xCF\xAC\x0F\xD2\x6C\x24\xDF\x1B\xC3\x89\xBC\xA1\x72\xC1\xCA\x60\x46\x2B\x01\xD1\x95\x16\x89\x2A", 260);
				// Change XBE Title ( Resoftmod Dash )
				UnleashX_XBE2.seekp(400);
				UnleashX_XBE2.write("\x52\x00\x65\x00\x53\x00\x6F\x00\x66\x00\x74\x00\x6D\x00\x6F\x00\x64\x00\x20\x00\x44\x00\x61\x00\x73\x00\x68", 27);
				CopyFile(IconPNG, "E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\modules\\resoftmod dash\\icon.png", NULL);
				UnleashX_XBE2.close();
				
				std::ofstream UnleashX_XBE3(CRescueDash, std::ios::binary | std::ios::in | std::ios::out);
				// Change XBE Title ( C Recovery Dash )
				UnleashX_XBE3.seekp(400);
				UnleashX_XBE3.write("\x43\x00\x20\x00\x52\x00\x65\x00\x63\x00\x6F\x00\x76\x00\x65\x00\x72\x00\x79\x00\x20\x00\x44\x00\x61\x00\x73\x00\x68\x00", 29);
				UnleashX_XBE3.close();
				
				std::ofstream UnleashX_XBE4(ERescueDash1, std::ios::binary | std::ios::in | std::ios::out);
				// Change XBE Title ( E Recovery Dash )
				UnleashX_XBE4.seekp(400);
				UnleashX_XBE4.write("\x45\x00\x20\x00\x52\x00\x65\x00\x63\x00\x6F\x00\x76\x00\x65\x00\x72\x00\x79\x00\x20\x00\x44\x00\x61\x00\x73\x00\x68\x00", 29);
				UnleashX_XBE4.close();
				
				std::ofstream UnleashX_XBE5(ERescueDash2, std::ios::binary | std::ios::in | std::ios::out);
				// Change XBE Title ( E Recovery Dash )
				UnleashX_XBE5.seekp(400);
				UnleashX_XBE5.write("\x45\x00\x20\x00\x52\x00\x65\x00\x63\x00\x6F\x00\x76\x00\x65\x00\x72\x00\x79\x00\x20\x00\x44\x00\x61\x00\x73\x00\x68\x00", 29);
				UnleashX_XBE5.close();
				
				std::ofstream UnleashX_XBE6(DashSettings, std::ios::binary | std::ios::in | std::ios::out);
				// Habibi Signature
				UnleashX_XBE6.seekp(0);
				UnleashX_XBE6.write("\x58\x42\x45\x48\x70\xA5\x6C\x87\x2B\xF4\x70\x2D\x89\xA8\x3D\xC5\xF2\x19\x73\x93\xB5\x16\x8E\xF5\x86\xFD\x3B\x5D\x96\x00\x87\xC3\x68\x71\x0F\xAD\xFC\x6F\x92\x7E\x62\x42\x74\x76\x5C\xA1\xAC\x35\xA2\xF3\x74\xEA\x09\x48\xE4\x6C\x0A\x6E\xDE\xE0\xED\x83\x27\x39\xD7\x9E\x34\x30\x48\x50\x9A\x9D\x6B\x72\x65\xB8\x3B\x66\xAC\x5E\x48\xFD\xB2\xB4\x4D\xC7\xF8\x76\x9F\x11\x76\xCC\x13\x3E\x24\xBD\x6C\x4C\x3A\x81\xF2\xD3\xCE\xDC\x85\xF9\x98\xA7\x88\xCB\x84\xE6\xEF\x3C\x8A\x62\xD5\x9E\xD3\x4E\xED\xCB\xAD\x42\x31\x8A\x16\x7D\xAE\x3D\x0D\x2C\xC0\xC2\x48\xAB\x63\x05\x3E\x61\x3D\x00\xD1\x87\xEA\xD8\xF8\xF4\x19\x85\xD1\xCC\x69\x39\xC5\x04\x5D\x84\xB1\x46\x53\xF7\x61\xC9\xAE\xB1\x9B\xF2\x32\xA4\xF2\x40\x9E\x6E\x21\x6C\x88\x87\x25\x35\x3A\x1E\xFA\x6C\xE3\xEB\x12\x51\x82\xA2\x46\xC0\xCE\xE2\x5F\x43\xAE\xE9\x08\x26\x45\x7C\x8B\xCB\x4E\x96\xEC\x36\xD4\xC7\x4A\xA7\x2B\x7B\x0E\x5C\x87\x53\x82\x59\xDF\xD4\xE1\x49\x85\xEE\xC7\x66\x0B\x7E\x34\x55\xAC\x41\x83\x69\xA7\xC8\xEF\x1C\xA9\x46\xD3\x35\x3A\x27\x34\x55\x9E\xDE\x4F\x54\x6E\xBB\x0E\x8C\x0A\x77\x2E\x1C", 260);
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
				UnleashX_XBE1.write("\x58\x42\x45\x48\x15\x4C\x88\x4E\x4D\x8B\xA8\xC6\xA7\x78\x63\x0C\x79\x4D\x7B\x34\x2C\x03\x26\x07\x6B\x69\x68\x15\x22\xB4\x9C\x0A\xF2\x8D\xBF\x11\x2D\x64\x40\x28\x75\xAC\x63\x12\x7B\x3F\xCA\x87\x91\xBA\x62\x86\x68\xD3\x98\xD5\x49\x75\x03\x7D\x25\x8E\x81\xB6\x1F\x96\x14\xE8\x03\xFF\xE0\x6E\x07\xFD\xFF\xD8\xA9\x93\x98\x1D\x08\xE3\xD3\xF7\x35\xC4\x80\x4D\x77\xB9\xD2\x88\xEF\x85\x18\x63\x66\x20\x82\x3C\x34\x77\x0E\x35\x46\x0E\x99\x72\xED\x6F\xBA\x99\xB9\xA6\x62\xF5\x38\x2B\x49\x92\x9E\x2A\xC8\x91\x82\xAE\x42\x9E\x99\x1E\xEC\x54\xC8\x3E\x41\x31\xDA\xA6\x96\xBB\x80\x5F\x32\xEC\x30\x52\x77\xF6\x01\xE7\xFB\x65\xB4\xF6\xF1\xDD\xBE\xB1\xBF\x1C\xD3\xB2\x96\xD6\xD7\x87\x7B\x42\xA2\x4E\xBD\xF2\x99\x5B\xE3\xF9\x62\xAA\x95\x0E\x3C\xEF\x62\xCB\x2E\x31\x1F\x9F\x64\x05\x08\x6A\x07\x35\x2A\x3D\x62\xEA\xFD\x42\xF7\x8F\x5D\xB4\x17\x33\xB5\x02\xB1\xEF\x63\x7A\x5E\x4A\x7D\xB3\xAA\xE2\x16\x93\x2E\x9C\x56\x1A\xE5\x19\xDB\xAD\x97\x86\x30\xB6\x62\x1C\x38\xCE\xC2\xE7\x14\x70\x60\x66\x27\xDC\xA4\xB7\xB2\x3D\xDD\xBA\xF1\x9A\xB0\x35\xFC\xE9\x64\x79\x51\x18", 260);
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
				UnleashX_XBE2.write("\x58\x42\x45\x48\x66\xE1\xFE\xE9\xA0\xE5\x9B\x1A\xF8\xDC\xAB\xD8\x27\x2E\x24\x54\x35\x36\xB0\xD6\x40\x8B\x92\xE6\xA6\x69\x16\x53\x1D\xF5\x86\xDA\xE0\x52\x7F\xFF\xF4\x4E\xEE\xD5\xA1\x76\x22\x9C\xF2\x37\x73\xC1\x0C\xD8\xBC\x14\x7D\xB9\x70\x98\x33\xDA\xF0\xF5\x65\x8C\x6C\x07\x4E\xB2\x06\xC1\x1E\x10\x7E\x17\x2C\x48\xDE\x43\x1C\x76\x24\x9A\xEC\xDB\x1A\xB0\xB8\x0E\xE2\x5F\x78\x2E\xEF\x3E\xB6\x51\xB7\x4B\xB8\x78\x8E\x0B\xD9\x63\xBE\x0E\x3D\x08\xA3\xC6\xA9\xEC\x51\x8E\x98\x9B\xF4\x8F\x02\x85\x00\x38\x54\x89\x3C\xDF\xB9\x3E\x0A\x0B\x6C\xD4\x09\xCC\xB8\x78\x53\x5E\xBA\x31\x80\x0D\xFB\x01\xB7\x89\x1A\xFC\x28\x15\xF0\x86\x77\x64\xC0\x23\x66\xB9\xA1\xA4\x05\x53\x85\xBE\x8C\x9A\x4D\xEF\xF0\xCB\x21\x22\x97\xBA\x5A\xE1\xC1\x69\xB5\x1C\xAC\x5F\x69\xF3\x2A\x9B\xE2\xF9\xFD\xD7\xA9\x62\x85\x5B\x7A\xA6\x9A\xF2\x89\x02\xA9\xB5\x61\x2B\x0F\x2C\x79\xFF\xF4\x15\xFC\xCC\x22\x92\x3F\x4A\x7D\xC4\xA2\xF5\xFA\xDD\xE5\xCD\x0D\x2A\x9D\x6D\x21\x9B\x4F\xF8\xAB\xA6\xCF\xAC\x0F\xD2\x6C\x24\xDF\x1B\xC3\x89\xBC\xA1\x72\xC1\xCA\x60\x46\x2B\x01\xD1\x95\x16\x89\x2A", 260);
				// Change XBE Title ( Resoftmod Dash )
				UnleashX_XBE2.seekp(400);
				UnleashX_XBE2.write("\x52\x00\x65\x00\x53\x00\x6F\x00\x66\x00\x74\x00\x6D\x00\x6F\x00\x64\x00\x20\x00\x44\x00\x61\x00\x73\x00\x68", 27);
				CopyFile(IconPNG, "E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\modules\\resoftmod dash\\icon.png", NULL);
				UnleashX_XBE2.close();
				
				std::ofstream UnleashX_XBE3(CRescueDash, std::ios::binary | std::ios::in | std::ios::out);
				// Change XBE Title ( C Recovery Dash )
				UnleashX_XBE3.seekp(400);
				UnleashX_XBE3.write("\x43\x00\x20\x00\x52\x00\x65\x00\x63\x00\x6F\x00\x76\x00\x65\x00\x72\x00\x79\x00\x20\x00\x44\x00\x61\x00\x73\x00\x68\x00", 29);
				UnleashX_XBE3.close();
				
				std::ofstream UnleashX_XBE4(ERescueDash1, std::ios::binary | std::ios::in | std::ios::out);
				// Change XBE Title ( E Recovery Dash )
				UnleashX_XBE4.seekp(400);
				UnleashX_XBE4.write("\x45\x00\x20\x00\x52\x00\x65\x00\x63\x00\x6F\x00\x76\x00\x65\x00\x72\x00\x79\x00\x20\x00\x44\x00\x61\x00\x73\x00\x68\x00", 29);
				UnleashX_XBE4.close();
				
				std::ofstream UnleashX_XBE5(ERescueDash2, std::ios::binary | std::ios::in | std::ios::out);
				// Change XBE Title ( E Recovery Dash )
				UnleashX_XBE5.seekp(400);
				UnleashX_XBE5.write("\x45\x00\x20\x00\x52\x00\x65\x00\x63\x00\x6F\x00\x76\x00\x65\x00\x72\x00\x79\x00\x20\x00\x44\x00\x61\x00\x73\x00\x68\x00", 29);
				UnleashX_XBE5.close();
				
				std::ofstream UnleashX_XBE6(DashSettings, std::ios::binary | std::ios::in | std::ios::out);
				// Habibi Signature
				UnleashX_XBE6.seekp(0);
				UnleashX_XBE6.write("\x58\x42\x45\x48\x70\xA5\x6C\x87\x2B\xF4\x70\x2D\x89\xA8\x3D\xC5\xF2\x19\x73\x93\xB5\x16\x8E\xF5\x86\xFD\x3B\x5D\x96\x00\x87\xC3\x68\x71\x0F\xAD\xFC\x6F\x92\x7E\x62\x42\x74\x76\x5C\xA1\xAC\x35\xA2\xF3\x74\xEA\x09\x48\xE4\x6C\x0A\x6E\xDE\xE0\xED\x83\x27\x39\xD7\x9E\x34\x30\x48\x50\x9A\x9D\x6B\x72\x65\xB8\x3B\x66\xAC\x5E\x48\xFD\xB2\xB4\x4D\xC7\xF8\x76\x9F\x11\x76\xCC\x13\x3E\x24\xBD\x6C\x4C\x3A\x81\xF2\xD3\xCE\xDC\x85\xF9\x98\xA7\x88\xCB\x84\xE6\xEF\x3C\x8A\x62\xD5\x9E\xD3\x4E\xED\xCB\xAD\x42\x31\x8A\x16\x7D\xAE\x3D\x0D\x2C\xC0\xC2\x48\xAB\x63\x05\x3E\x61\x3D\x00\xD1\x87\xEA\xD8\xF8\xF4\x19\x85\xD1\xCC\x69\x39\xC5\x04\x5D\x84\xB1\x46\x53\xF7\x61\xC9\xAE\xB1\x9B\xF2\x32\xA4\xF2\x40\x9E\x6E\x21\x6C\x88\x87\x25\x35\x3A\x1E\xFA\x6C\xE3\xEB\x12\x51\x82\xA2\x46\xC0\xCE\xE2\x5F\x43\xAE\xE9\x08\x26\x45\x7C\x8B\xCB\x4E\x96\xEC\x36\xD4\xC7\x4A\xA7\x2B\x7B\x0E\x5C\x87\x53\x82\x59\xDF\xD4\xE1\x49\x85\xEE\xC7\x66\x0B\x7E\x34\x55\xAC\x41\x83\x69\xA7\xC8\xEF\x1C\xA9\x46\xD3\x35\x3A\x27\x34\x55\x9E\xDE\x4F\x54\x6E\xBB\x0E\x8C\x0A\x77\x2E\x1C", 260);
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