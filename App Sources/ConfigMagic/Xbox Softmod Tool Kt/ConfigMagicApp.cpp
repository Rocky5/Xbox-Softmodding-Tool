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
				UnleashX_XBE1.write("\x58\x42\x45\x48\x04\x5A\x64\x49\x69\x20\x13\x50\x01\x6E\x3D\x17\x0F\xD2\x96\x47\x97\xEE\xA6\xC7\x58\x21\x9D\x05\x6D\x2D\xB6\x3F\x50\xD9\x0A\xD6\x11\xE3\x79\xF6\x68\x9A\xA4\xF2\x1D\xB5\x99\x42\x78\xEA\x14\xC8\x64\xAB\x22\xBB\x47\xB6\x9A\x08\x29\x17\x56\xF2\x24\x46\x18\x77\x9B\xE8\x6F\xC2\xA1\xE3\xE9\x8F\xFB\xCF\x6D\x60\xA8\x35\xFE\x3D\x7A\x99\x18\x9B\xF7\xDE\x13\x99\x43\xE1\x7F\x57\x1A\x24\x9B\xAF\xA8\x21\xD4\xCB\x56\x21\x08\x34\x09\x84\x59\xF0\xF9\xE7\x56\x81\x6D\xB7\xD7\x91\x4F\x66\x25\x68\x6E\x78\x57\xF8\x04\xD2\x69\x5F\x1F\x6A\x03\x3F\x45\x91\x33\xFE\x88\x64\x40\xD5\x15\x5E\x38\x3B\x01\x22\x65\xA8\x0D\x9E\x2A\x1A\x83\x80\x59\x65\x49\xBB\xE4\x41\xA6\xB3\x92\x34\xB8\x9F\x50\x81\x69\x23\xA9\xED\xC3\x1E\x1C\xD0\xE6\x33\xB7\x99\x1A\x5A\x44\x53\x18\x30\x67\x94\xD3\x4C\x59\x13\x26\x10\x5A\x17\x98\x47\x2F\xCD\x0D\x5C\x65\x56\x0A\x13\xDC\xA7\x86\x2B\x58\xFE\x6A\x29\xC9\xE4\xE2\xF8\x36\x60\x46\x75\x55\x1E\xF5\xBB\x8A\x77\x9E\xE0\xA3\x8E\x33\x8E\xBE\x87\x42\x62\x43\x00\x23\x92\x95\xDB\x2E\xE6\x90\xFB\xBB\x25\x70\x42\x45\x7C\xCA\x54", 260);
				// Change XBE Title ( NKPatcher Settings )
				UnleashX_XBE1.seekp(400);
				UnleashX_XBE1.write("\x4E\x00\x4B\x00\x50\x00\x61\x00\x74\x00\x63\x00\x68\x00\x65\x00\x72\x00\x20\x00\x53\x00\x65\x00\x74\x00\x74\x00\x69\x00\x6E\x00\x67\x00\x73", 35);
				// XBE Hash
				UnleashX_XBE1.seekp(1532);
				UnleashX_XBE1.write("\xBA\xC6\x20\xBF\x8A\xC7\x5A\xEF\x79\xB7\x8A\xF3\xD2\x3E\x66\xD0\xC7\xB1\x80\x52", 20);
				// Remove recent.dat creation
				UnleashX_XBE1.seekp(2353004);
				UnleashX_XBE1.write("\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 10);
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
				UnleashX_XBE2.write("\x58\x42\x45\x48\x09\x51\xD4\xF7\xA3\xE7\x75\xE2\xA8\xDE\x37\x4E\xE5\x31\xBC\x14\xC7\xF7\x9A\x87\x66\x0A\x89\xF5\x65\xD5\x11\xE0\xCD\xE2\x6E\x89\x38\x72\x46\xD1\x04\x79\xCA\x17\x88\x76\x81\x7F\xD1\x32\x12\xE2\xC8\x23\x3A\x4A\xE8\x65\x04\x9A\x44\xC4\x9D\x8B\xF3\x07\xB3\x80\x32\x49\xC3\xAE\xDB\x60\xDE\x71\x50\x56\xDB\x46\xD4\xB2\xC5\xE9\x6A\xC2\x6F\xC0\x8D\x79\x0D\x01\xE0\xDE\xAB\x1F\x3A\x38\x12\x11\x1B\xCB\xD4\xEE\x15\x7C\x9E\x95\x23\x9B\x29\xA3\x13\x6C\x76\x54\xDC\xD2\xA7\x91\x2D\xBE\x4D\x54\xC0\xF9\xB8\x06\xC5\xFF\x15\x87\x68\x39\x4B\xBA\xCF\xAF\x60\x70\x9F\x82\x18\xE6\xE6\x07\xA4\x19\xD6\xC3\x34\xCF\x78\xB5\x56\x8D\x53\xC7\x8D\x1D\x2C\x93\xB9\xAE\xC5\xC5\x68\x0E\xB4\x50\x89\xD6\x2D\xAF\x65\xE1\x1D\x36\x0E\xC1\xCC\x40\x74\xAB\xBC\x11\x25\x1A\x15\xE0\x56\xBE\x75\xA1\x55\xE8\xC8\xB3\xEC\xD9\x74\x3C\x0F\xBB\xB3\x32\xD3\x96\xCB\x3F\xDC\x6F\x8A\x39\x83\x80\xD9\x53\xE2\xF2\xB8\x07\x04\xDB\xFF\x0F\xE8\x95\x56\xFB\xDB\x85\x4F\xDA\x9D\x01\x0D\x4D\x77\x75\x98\x54\x43\x02\x84\x09\xD9\x0D\x42\xCC\x15\x76\xFA\x78\xB1\x62\x0A\x3A\x91\x19", 260);
				// Change XBE Title ( Resoftmod Dash )
				UnleashX_XBE2.seekp(400);
				UnleashX_XBE2.write("\x52\x00\x65\x00\x53\x00\x6F\x00\x66\x00\x74\x00\x6D\x00\x6F\x00\x64\x00\x20\x00\x44\x00\x61\x00\x73\x00\x68", 27);
				// XBE Hash
				UnleashX_XBE2.seekp(1532);
				UnleashX_XBE2.write("\x19\x0B\x5F\xA8\xA6\x1D\xB7\x7E\xE7\xB0\xDF\x2C\x3C\x33\x74\xA7\x00\xCD\xB8\x5C", 20);
				// Remove recent.dat creation
				UnleashX_XBE2.seekp(2353004);
				UnleashX_XBE2.write("\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 10);
				UnleashX_XBE2.close();
				
				std::ofstream UnleashX_XBE3(CRescueDash, std::ios::binary | std::ios::in | std::ios::out);
				// Habibi Signature
				UnleashX_XBE3.seekp(0);
				UnleashX_XBE3.write("\x58\x42\x45\x48\x83\xCA\x2B\x3A\xBB\x46\xEF\xFA\xA1\xFE\x65\x69\xA1\x80\x4C\xF6\xD5\x6C\xF4\xB7\x01\x02\x8A\x01\xF9\xE6\xD6\xD9\x15\xDC\xAE\x15\xA0\x12\x02\xFC\xA6\xE5\x7A\x93\xC9\x8E\x74\xB9\x5A\xE4\x2D\x7A\x1E\xA0\xAC\xFC\x30\x23\x65\x6F\xB8\x35\x44\x61\xDE\x9D\x85\x59\xD4\x55\x82\x7C\xA9\x88\xAA\x1C\xC0\xDE\xD8\x47\x9E\xE5\x95\xA0\x5E\x6C\xD9\x42\x85\x22\xC1\x04\xED\x00\x6C\x75\x43\x34\x57\xC9\x4A\xDD\x3E\x16\x27\x57\x7D\xA2\x15\x4C\x1C\x5C\x63\x5E\xDE\xFF\x75\xFD\xFC\xC8\x4B\x32\x4D\xD8\x14\xD0\x0A\xAE\xB5\xB7\x1B\x11\xB1\x74\x12\x13\x4D\xF4\xDF\x2B\x4F\x9D\xA8\xD5\xD0\x21\x3E\x8C\xBB\x51\x8B\xDD\xE1\x6C\xC9\x56\x98\x83\x4D\x12\x64\x7D\xC3\x72\xC9\x92\x44\x43\x31\x5A\x09\xED\xA4\x2D\xAA\x0C\xA0\xCC\xF0\x1B\x30\x74\x00\xB1\x6A\x66\xFB\x17\xC4\xD0\xA6\x45\xB4\x6C\x8B\xF4\xCF\xD1\x94\xE6\xD6\xF7\x4A\x6E\xD9\x08\x23\x55\x6D\x6B\x3C\x36\x88\x53\x2F\x3E\x9A\x4D\x01\xE1\xF6\x92\xBE\xEA\xC8\xF0\xB9\xAB\x57\x77\x22\x51\x4A\xB9\xDB\x2A\x29\xAD\x28\x32\x3B\xB1\x29\xE4\xDB\x9E\xD6\x05\x3F\x25\x53\x44\x2C\x13\x9F\xDF\xAB\x58\xDC\x20", 260);
				// Change XBE Title ( C Recovery Dash )
				UnleashX_XBE3.seekp(400);
				UnleashX_XBE3.write("\x43\x00\x20\x00\x52\x00\x65\x00\x63\x00\x6F\x00\x76\x00\x65\x00\x72\x00\x79\x00\x20\x00\x44\x00\x61\x00\x73\x00\x68\x00", 29);
				// XBE Hash
				UnleashX_XBE3.seekp(1532);
				UnleashX_XBE3.write("\x19\x0B\x5F\xA8\xA6\x1D\xB7\x7E\xE7\xB0\xDF\x2C\x3C\x33\x74\xA7\x00\xCD\xB8\x5C", 20);
				// Remove recent.dat creation
				UnleashX_XBE3.seekp(2353004);
				UnleashX_XBE3.write("\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 10);
				UnleashX_XBE3.close();
				
				std::ofstream UnleashX_XBE4(ERescueDash1, std::ios::binary | std::ios::in | std::ios::out);
				// Habibi Signature
				UnleashX_XBE4.seekp(0);
				UnleashX_XBE4.write("\x58\x42\x45\x48\x06\x38\x5A\x0B\xC5\x16\x8D\x76\xE9\x42\x07\x7C\xFC\x1E\x0E\xD5\xE3\x57\xAA\x2F\xC2\x6F\x80\x87\xD1\x9F\x82\xDA\xB1\x75\xDA\x2C\x0E\x82\xA5\x54\x9A\x37\x0C\x86\x12\xA9\xD9\x45\x51\x3B\x24\xB6\x63\x7A\xCB\xE8\xAA\xAE\x26\x9B\xCA\xBC\x92\xB3\x3F\x18\xF2\x98\x43\x01\xA5\x3F\x41\xDE\xB2\x17\xB2\xDD\x97\xEB\x00\x9A\xED\x47\xA3\xC9\xDE\x70\x25\x4C\xB0\x35\xC6\x7B\xE5\xF1\x3D\xD5\xF9\x54\x75\x10\xC4\xAD\x58\x71\x61\xCC\x63\xA0\x41\x07\xF3\xC6\x83\xBB\x3F\x30\x70\x01\x4E\x11\x26\x55\xAE\x9A\xB3\x28\x43\x50\x23\x08\x1C\xAE\x63\x6C\x63\xC1\xC8\x7E\x78\xE3\x09\xF7\x83\xF5\x99\x5A\x61\x8E\xDF\xB2\x82\x57\xF7\x4E\xEF\x49\xA7\xAA\xE3\x45\x4F\x08\x8B\x32\xDB\x2A\xFA\x57\xA9\xE4\x1E\x40\x1B\xBB\x3F\x67\x19\xA0\x96\x1A\xA6\x41\x02\xBF\x85\xFB\x5E\x0B\x7A\xC2\x96\x61\x19\x17\x9D\x88\x89\x55\xD0\xFB\x75\x68\x19\xC5\x04\x35\x17\xCF\xEF\x87\xA5\x4D\x56\x4E\x38\x54\x40\x49\x1B\x72\xC6\xE3\x98\xB2\x3E\x19\x6D\x30\x1C\x8E\x2D\xDB\xB3\x6C\xD1\x84\x96\xBB\xF2\x05\xCF\xA0\x18\x25\x8C\xD9\x5E\x36\x96\xD4\x77\x22\x65\x44\x19\xB3\xB0\x4C", 260);
				// Change XBE Title ( E Recovery Dash )
				UnleashX_XBE4.seekp(400);
				UnleashX_XBE4.write("\x45\x00\x20\x00\x52\x00\x65\x00\x63\x00\x6F\x00\x76\x00\x65\x00\x72\x00\x79\x00\x20\x00\x44\x00\x61\x00\x73\x00\x68\x00", 29);
				// XBE Hash
				UnleashX_XBE4.seekp(1532);
				UnleashX_XBE4.write("\x19\x0B\x5F\xA8\xA6\x1D\xB7\x7E\xE7\xB0\xDF\x2C\x3C\x33\x74\xA7\x00\xCD\xB8\x5C", 20);
				// Remove recent.dat creation
				UnleashX_XBE4.seekp(2353004);
				UnleashX_XBE4.write("\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 10);
				UnleashX_XBE4.close();
				
				std::ofstream UnleashX_XBE5(ERescueDash2, std::ios::binary | std::ios::in | std::ios::out);
				// Habibi Signature
				UnleashX_XBE5.seekp(0);
				UnleashX_XBE5.write("\x58\x42\x45\x48\x06\x38\x5A\x0B\xC5\x16\x8D\x76\xE9\x42\x07\x7C\xFC\x1E\x0E\xD5\xE3\x57\xAA\x2F\xC2\x6F\x80\x87\xD1\x9F\x82\xDA\xB1\x75\xDA\x2C\x0E\x82\xA5\x54\x9A\x37\x0C\x86\x12\xA9\xD9\x45\x51\x3B\x24\xB6\x63\x7A\xCB\xE8\xAA\xAE\x26\x9B\xCA\xBC\x92\xB3\x3F\x18\xF2\x98\x43\x01\xA5\x3F\x41\xDE\xB2\x17\xB2\xDD\x97\xEB\x00\x9A\xED\x47\xA3\xC9\xDE\x70\x25\x4C\xB0\x35\xC6\x7B\xE5\xF1\x3D\xD5\xF9\x54\x75\x10\xC4\xAD\x58\x71\x61\xCC\x63\xA0\x41\x07\xF3\xC6\x83\xBB\x3F\x30\x70\x01\x4E\x11\x26\x55\xAE\x9A\xB3\x28\x43\x50\x23\x08\x1C\xAE\x63\x6C\x63\xC1\xC8\x7E\x78\xE3\x09\xF7\x83\xF5\x99\x5A\x61\x8E\xDF\xB2\x82\x57\xF7\x4E\xEF\x49\xA7\xAA\xE3\x45\x4F\x08\x8B\x32\xDB\x2A\xFA\x57\xA9\xE4\x1E\x40\x1B\xBB\x3F\x67\x19\xA0\x96\x1A\xA6\x41\x02\xBF\x85\xFB\x5E\x0B\x7A\xC2\x96\x61\x19\x17\x9D\x88\x89\x55\xD0\xFB\x75\x68\x19\xC5\x04\x35\x17\xCF\xEF\x87\xA5\x4D\x56\x4E\x38\x54\x40\x49\x1B\x72\xC6\xE3\x98\xB2\x3E\x19\x6D\x30\x1C\x8E\x2D\xDB\xB3\x6C\xD1\x84\x96\xBB\xF2\x05\xCF\xA0\x18\x25\x8C\xD9\x5E\x36\x96\xD4\x77\x22\x65\x44\x19\xB3\xB0\x4C", 260);
				// Change XBE Title ( E Recovery Dash )
				UnleashX_XBE5.seekp(400);
				UnleashX_XBE5.write("\x45\x00\x20\x00\x52\x00\x65\x00\x63\x00\x6F\x00\x76\x00\x65\x00\x72\x00\x79\x00\x20\x00\x44\x00\x61\x00\x73\x00\x68\x00", 29);
				// XBE Hash
				UnleashX_XBE5.seekp(1532);
				UnleashX_XBE5.write("\x19\x0B\x5F\xA8\xA6\x1D\xB7\x7E\xE7\xB0\xDF\x2C\x3C\x33\x74\xA7\x00\xCD\xB8\x5C", 20);
				// Remove recent.dat creation
				UnleashX_XBE5.seekp(2353004);
				UnleashX_XBE5.write("\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 10);
				UnleashX_XBE5.close();
				
				std::ofstream UnleashX_XBE6(DashSettings, std::ios::binary | std::ios::in | std::ios::out);
				// Habibi Signature
				UnleashX_XBE6.seekp(0);
				UnleashX_XBE6.write("\x58\x42\x45\x48\xCF\x77\x26\xD8\xD6\x28\x51\x26\xFF\x13\xDC\x34\x4F\x54\x6B\xAB\xFE\xD8\xB9\xB6\xB3\xC1\x09\xD8\x7A\xAF\xF2\xFE\x6E\x6E\x76\xAA\x53\x99\x49\xA5\xA7\xFF\x79\xC6\xEF\x11\x05\x32\xA6\x37\x5F\xDD\x0B\xFE\x48\x93\x4A\x28\x7C\x95\x86\xF3\xE4\x52\x56\xFF\x98\x92\x2B\xAE\xF8\x50\x3D\xF1\x40\x15\x53\xF3\x52\xA5\x01\xDF\x2A\xBB\x0D\xFE\x50\x8D\x1C\x19\xF2\x81\x54\x57\x82\x77\x91\xF4\xB6\x0F\x8A\xCC\xC4\x37\xF2\xFB\x80\x5A\x52\xC8\x69\xAE\x99\x00\x7A\x28\x73\x62\xD6\x53\xA7\x20\xBF\x10\x51\xE6\x1A\x86\x90\x2F\xF1\x0F\xD2\x6C\xBB\x1A\x14\x63\xA1\x36\xC9\xCA\xE7\x84\x98\x5F\x06\xE7\xC6\x5A\x42\x98\x6C\xAC\xB7\xD6\x44\x36\x1F\x40\x25\x76\x11\x14\x2F\x8C\x8C\x63\x95\x22\x4F\x51\x3F\x7E\x24\x5D\x4E\x2A\x17\x7D\x72\x8F\x46\xB4\x8E\x47\x45\xEC\x1F\xDD\x67\x5A\x1A\x41\xED\xDA\x4C\x01\x6C\x78\x1D\x78\xA3\x0A\xC8\xD2\x0E\x32\xFE\x56\x4A\x9A\xAF\xF2\x02\xB0\xA0\x18\x27\xCE\x1B\xAC\x99\x66\x98\x49\x44\xDF\x38\x12\x8D\x9A\x35\x94\x64\x91\x2C\x02\xA7\xAF\xB2\x35\xA4\x28\x75\xBE\x37\x3A\xED\x8D\x5F\x43\x55\x36\xF3\x29\x45\x2A\xD5\x86", 260);
				// Change XBE Title ( Backup, Restore or Move )
				UnleashX_XBE6.seekp(400);
				UnleashX_XBE6.write("\x42\x00\x61\x00\x63\x00\x6B\x00\x75\x00\x70\x00\x2C\x00\x20\x00\x52\x00\x65\x00\x73\x00\x74\x00\x6F\x00\x72\x00\x65\x00\x20\x00\x6F\x00\x72\x00\x20\x00\x4D\x00\x6F\x00\x76\x00\x65", 45);
				// XBE Hash
				UnleashX_XBE6.seekp(1532);
				UnleashX_XBE6.write("\x19\x0B\x5F\xA8\xA6\x1D\xB7\x7E\xE7\xB0\xDF\x2C\x3C\x33\x74\xA7\x00\xCD\xB8\x5C", 20);
				// Remove recent.dat creation
				UnleashX_XBE6.seekp(2353004);
				UnleashX_XBE6.write("\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 10);
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
				UnleashX_XBE1.write("\x58\x42\x45\x48\x04\x5A\x64\x49\x69\x20\x13\x50\x01\x6E\x3D\x17\x0F\xD2\x96\x47\x97\xEE\xA6\xC7\x58\x21\x9D\x05\x6D\x2D\xB6\x3F\x50\xD9\x0A\xD6\x11\xE3\x79\xF6\x68\x9A\xA4\xF2\x1D\xB5\x99\x42\x78\xEA\x14\xC8\x64\xAB\x22\xBB\x47\xB6\x9A\x08\x29\x17\x56\xF2\x24\x46\x18\x77\x9B\xE8\x6F\xC2\xA1\xE3\xE9\x8F\xFB\xCF\x6D\x60\xA8\x35\xFE\x3D\x7A\x99\x18\x9B\xF7\xDE\x13\x99\x43\xE1\x7F\x57\x1A\x24\x9B\xAF\xA8\x21\xD4\xCB\x56\x21\x08\x34\x09\x84\x59\xF0\xF9\xE7\x56\x81\x6D\xB7\xD7\x91\x4F\x66\x25\x68\x6E\x78\x57\xF8\x04\xD2\x69\x5F\x1F\x6A\x03\x3F\x45\x91\x33\xFE\x88\x64\x40\xD5\x15\x5E\x38\x3B\x01\x22\x65\xA8\x0D\x9E\x2A\x1A\x83\x80\x59\x65\x49\xBB\xE4\x41\xA6\xB3\x92\x34\xB8\x9F\x50\x81\x69\x23\xA9\xED\xC3\x1E\x1C\xD0\xE6\x33\xB7\x99\x1A\x5A\x44\x53\x18\x30\x67\x94\xD3\x4C\x59\x13\x26\x10\x5A\x17\x98\x47\x2F\xCD\x0D\x5C\x65\x56\x0A\x13\xDC\xA7\x86\x2B\x58\xFE\x6A\x29\xC9\xE4\xE2\xF8\x36\x60\x46\x75\x55\x1E\xF5\xBB\x8A\x77\x9E\xE0\xA3\x8E\x33\x8E\xBE\x87\x42\x62\x43\x00\x23\x92\x95\xDB\x2E\xE6\x90\xFB\xBB\x25\x70\x42\x45\x7C\xCA\x54", 260);
				// Change XBE Title ( NKPatcher Settings )
				UnleashX_XBE1.seekp(400);
				UnleashX_XBE1.write("\x4E\x00\x4B\x00\x50\x00\x61\x00\x74\x00\x63\x00\x68\x00\x65\x00\x72\x00\x20\x00\x53\x00\x65\x00\x74\x00\x74\x00\x69\x00\x6E\x00\x67\x00\x73", 35);
				// XBE Hash
				UnleashX_XBE1.seekp(1532);
				UnleashX_XBE1.write("\xBA\xC6\x20\xBF\x8A\xC7\x5A\xEF\x79\xB7\x8A\xF3\xD2\x3E\x66\xD0\xC7\xB1\x80\x52", 20);
				// Remove recent.dat creation
				UnleashX_XBE1.seekp(2353004);
				UnleashX_XBE1.write("\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 10);
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
				UnleashX_XBE2.write("\x58\x42\x45\x48\x09\x51\xD4\xF7\xA3\xE7\x75\xE2\xA8\xDE\x37\x4E\xE5\x31\xBC\x14\xC7\xF7\x9A\x87\x66\x0A\x89\xF5\x65\xD5\x11\xE0\xCD\xE2\x6E\x89\x38\x72\x46\xD1\x04\x79\xCA\x17\x88\x76\x81\x7F\xD1\x32\x12\xE2\xC8\x23\x3A\x4A\xE8\x65\x04\x9A\x44\xC4\x9D\x8B\xF3\x07\xB3\x80\x32\x49\xC3\xAE\xDB\x60\xDE\x71\x50\x56\xDB\x46\xD4\xB2\xC5\xE9\x6A\xC2\x6F\xC0\x8D\x79\x0D\x01\xE0\xDE\xAB\x1F\x3A\x38\x12\x11\x1B\xCB\xD4\xEE\x15\x7C\x9E\x95\x23\x9B\x29\xA3\x13\x6C\x76\x54\xDC\xD2\xA7\x91\x2D\xBE\x4D\x54\xC0\xF9\xB8\x06\xC5\xFF\x15\x87\x68\x39\x4B\xBA\xCF\xAF\x60\x70\x9F\x82\x18\xE6\xE6\x07\xA4\x19\xD6\xC3\x34\xCF\x78\xB5\x56\x8D\x53\xC7\x8D\x1D\x2C\x93\xB9\xAE\xC5\xC5\x68\x0E\xB4\x50\x89\xD6\x2D\xAF\x65\xE1\x1D\x36\x0E\xC1\xCC\x40\x74\xAB\xBC\x11\x25\x1A\x15\xE0\x56\xBE\x75\xA1\x55\xE8\xC8\xB3\xEC\xD9\x74\x3C\x0F\xBB\xB3\x32\xD3\x96\xCB\x3F\xDC\x6F\x8A\x39\x83\x80\xD9\x53\xE2\xF2\xB8\x07\x04\xDB\xFF\x0F\xE8\x95\x56\xFB\xDB\x85\x4F\xDA\x9D\x01\x0D\x4D\x77\x75\x98\x54\x43\x02\x84\x09\xD9\x0D\x42\xCC\x15\x76\xFA\x78\xB1\x62\x0A\x3A\x91\x19", 260);
				// Change XBE Title ( Resoftmod Dash )
				UnleashX_XBE2.seekp(400);
				UnleashX_XBE2.write("\x52\x00\x65\x00\x53\x00\x6F\x00\x66\x00\x74\x00\x6D\x00\x6F\x00\x64\x00\x20\x00\x44\x00\x61\x00\x73\x00\x68", 27);
				// XBE Hash
				UnleashX_XBE2.seekp(1532);
				UnleashX_XBE2.write("\x19\x0B\x5F\xA8\xA6\x1D\xB7\x7E\xE7\xB0\xDF\x2C\x3C\x33\x74\xA7\x00\xCD\xB8\x5C", 20);
				// Remove recent.dat creation
				UnleashX_XBE2.seekp(2353004);
				UnleashX_XBE2.write("\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 10);
				UnleashX_XBE2.close();
				
				std::ofstream UnleashX_XBE3(CRescueDash, std::ios::binary | std::ios::in | std::ios::out);
				// Habibi Signature
				UnleashX_XBE3.seekp(0);
				UnleashX_XBE3.write("\x58\x42\x45\x48\x83\xCA\x2B\x3A\xBB\x46\xEF\xFA\xA1\xFE\x65\x69\xA1\x80\x4C\xF6\xD5\x6C\xF4\xB7\x01\x02\x8A\x01\xF9\xE6\xD6\xD9\x15\xDC\xAE\x15\xA0\x12\x02\xFC\xA6\xE5\x7A\x93\xC9\x8E\x74\xB9\x5A\xE4\x2D\x7A\x1E\xA0\xAC\xFC\x30\x23\x65\x6F\xB8\x35\x44\x61\xDE\x9D\x85\x59\xD4\x55\x82\x7C\xA9\x88\xAA\x1C\xC0\xDE\xD8\x47\x9E\xE5\x95\xA0\x5E\x6C\xD9\x42\x85\x22\xC1\x04\xED\x00\x6C\x75\x43\x34\x57\xC9\x4A\xDD\x3E\x16\x27\x57\x7D\xA2\x15\x4C\x1C\x5C\x63\x5E\xDE\xFF\x75\xFD\xFC\xC8\x4B\x32\x4D\xD8\x14\xD0\x0A\xAE\xB5\xB7\x1B\x11\xB1\x74\x12\x13\x4D\xF4\xDF\x2B\x4F\x9D\xA8\xD5\xD0\x21\x3E\x8C\xBB\x51\x8B\xDD\xE1\x6C\xC9\x56\x98\x83\x4D\x12\x64\x7D\xC3\x72\xC9\x92\x44\x43\x31\x5A\x09\xED\xA4\x2D\xAA\x0C\xA0\xCC\xF0\x1B\x30\x74\x00\xB1\x6A\x66\xFB\x17\xC4\xD0\xA6\x45\xB4\x6C\x8B\xF4\xCF\xD1\x94\xE6\xD6\xF7\x4A\x6E\xD9\x08\x23\x55\x6D\x6B\x3C\x36\x88\x53\x2F\x3E\x9A\x4D\x01\xE1\xF6\x92\xBE\xEA\xC8\xF0\xB9\xAB\x57\x77\x22\x51\x4A\xB9\xDB\x2A\x29\xAD\x28\x32\x3B\xB1\x29\xE4\xDB\x9E\xD6\x05\x3F\x25\x53\x44\x2C\x13\x9F\xDF\xAB\x58\xDC\x20", 260);
				// Change XBE Title ( C Recovery Dash )
				UnleashX_XBE3.seekp(400);
				UnleashX_XBE3.write("\x43\x00\x20\x00\x52\x00\x65\x00\x63\x00\x6F\x00\x76\x00\x65\x00\x72\x00\x79\x00\x20\x00\x44\x00\x61\x00\x73\x00\x68\x00", 29);
				// XBE Hash
				UnleashX_XBE3.seekp(1532);
				UnleashX_XBE3.write("\x19\x0B\x5F\xA8\xA6\x1D\xB7\x7E\xE7\xB0\xDF\x2C\x3C\x33\x74\xA7\x00\xCD\xB8\x5C", 20);
				// Remove recent.dat creation
				UnleashX_XBE3.seekp(2353004);
				UnleashX_XBE3.write("\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 10);
				UnleashX_XBE3.close();
				
				std::ofstream UnleashX_XBE4(ERescueDash1, std::ios::binary | std::ios::in | std::ios::out);
				// Habibi Signature
				UnleashX_XBE4.seekp(0);
				UnleashX_XBE4.write("\x58\x42\x45\x48\x06\x38\x5A\x0B\xC5\x16\x8D\x76\xE9\x42\x07\x7C\xFC\x1E\x0E\xD5\xE3\x57\xAA\x2F\xC2\x6F\x80\x87\xD1\x9F\x82\xDA\xB1\x75\xDA\x2C\x0E\x82\xA5\x54\x9A\x37\x0C\x86\x12\xA9\xD9\x45\x51\x3B\x24\xB6\x63\x7A\xCB\xE8\xAA\xAE\x26\x9B\xCA\xBC\x92\xB3\x3F\x18\xF2\x98\x43\x01\xA5\x3F\x41\xDE\xB2\x17\xB2\xDD\x97\xEB\x00\x9A\xED\x47\xA3\xC9\xDE\x70\x25\x4C\xB0\x35\xC6\x7B\xE5\xF1\x3D\xD5\xF9\x54\x75\x10\xC4\xAD\x58\x71\x61\xCC\x63\xA0\x41\x07\xF3\xC6\x83\xBB\x3F\x30\x70\x01\x4E\x11\x26\x55\xAE\x9A\xB3\x28\x43\x50\x23\x08\x1C\xAE\x63\x6C\x63\xC1\xC8\x7E\x78\xE3\x09\xF7\x83\xF5\x99\x5A\x61\x8E\xDF\xB2\x82\x57\xF7\x4E\xEF\x49\xA7\xAA\xE3\x45\x4F\x08\x8B\x32\xDB\x2A\xFA\x57\xA9\xE4\x1E\x40\x1B\xBB\x3F\x67\x19\xA0\x96\x1A\xA6\x41\x02\xBF\x85\xFB\x5E\x0B\x7A\xC2\x96\x61\x19\x17\x9D\x88\x89\x55\xD0\xFB\x75\x68\x19\xC5\x04\x35\x17\xCF\xEF\x87\xA5\x4D\x56\x4E\x38\x54\x40\x49\x1B\x72\xC6\xE3\x98\xB2\x3E\x19\x6D\x30\x1C\x8E\x2D\xDB\xB3\x6C\xD1\x84\x96\xBB\xF2\x05\xCF\xA0\x18\x25\x8C\xD9\x5E\x36\x96\xD4\x77\x22\x65\x44\x19\xB3\xB0\x4C", 260);
				// Change XBE Title ( E Recovery Dash )
				UnleashX_XBE4.seekp(400);
				UnleashX_XBE4.write("\x45\x00\x20\x00\x52\x00\x65\x00\x63\x00\x6F\x00\x76\x00\x65\x00\x72\x00\x79\x00\x20\x00\x44\x00\x61\x00\x73\x00\x68\x00", 29);
				// XBE Hash
				UnleashX_XBE4.seekp(1532);
				UnleashX_XBE4.write("\x19\x0B\x5F\xA8\xA6\x1D\xB7\x7E\xE7\xB0\xDF\x2C\x3C\x33\x74\xA7\x00\xCD\xB8\x5C", 20);
				// Remove recent.dat creation
				UnleashX_XBE4.seekp(2353004);
				UnleashX_XBE4.write("\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 10);
				UnleashX_XBE4.close();
				
				std::ofstream UnleashX_XBE5(ERescueDash2, std::ios::binary | std::ios::in | std::ios::out);
				// Habibi Signature
				UnleashX_XBE5.seekp(0);
				UnleashX_XBE5.write("\x58\x42\x45\x48\x06\x38\x5A\x0B\xC5\x16\x8D\x76\xE9\x42\x07\x7C\xFC\x1E\x0E\xD5\xE3\x57\xAA\x2F\xC2\x6F\x80\x87\xD1\x9F\x82\xDA\xB1\x75\xDA\x2C\x0E\x82\xA5\x54\x9A\x37\x0C\x86\x12\xA9\xD9\x45\x51\x3B\x24\xB6\x63\x7A\xCB\xE8\xAA\xAE\x26\x9B\xCA\xBC\x92\xB3\x3F\x18\xF2\x98\x43\x01\xA5\x3F\x41\xDE\xB2\x17\xB2\xDD\x97\xEB\x00\x9A\xED\x47\xA3\xC9\xDE\x70\x25\x4C\xB0\x35\xC6\x7B\xE5\xF1\x3D\xD5\xF9\x54\x75\x10\xC4\xAD\x58\x71\x61\xCC\x63\xA0\x41\x07\xF3\xC6\x83\xBB\x3F\x30\x70\x01\x4E\x11\x26\x55\xAE\x9A\xB3\x28\x43\x50\x23\x08\x1C\xAE\x63\x6C\x63\xC1\xC8\x7E\x78\xE3\x09\xF7\x83\xF5\x99\x5A\x61\x8E\xDF\xB2\x82\x57\xF7\x4E\xEF\x49\xA7\xAA\xE3\x45\x4F\x08\x8B\x32\xDB\x2A\xFA\x57\xA9\xE4\x1E\x40\x1B\xBB\x3F\x67\x19\xA0\x96\x1A\xA6\x41\x02\xBF\x85\xFB\x5E\x0B\x7A\xC2\x96\x61\x19\x17\x9D\x88\x89\x55\xD0\xFB\x75\x68\x19\xC5\x04\x35\x17\xCF\xEF\x87\xA5\x4D\x56\x4E\x38\x54\x40\x49\x1B\x72\xC6\xE3\x98\xB2\x3E\x19\x6D\x30\x1C\x8E\x2D\xDB\xB3\x6C\xD1\x84\x96\xBB\xF2\x05\xCF\xA0\x18\x25\x8C\xD9\x5E\x36\x96\xD4\x77\x22\x65\x44\x19\xB3\xB0\x4C", 260);
				// Change XBE Title ( E Recovery Dash )
				UnleashX_XBE5.seekp(400);
				UnleashX_XBE5.write("\x45\x00\x20\x00\x52\x00\x65\x00\x63\x00\x6F\x00\x76\x00\x65\x00\x72\x00\x79\x00\x20\x00\x44\x00\x61\x00\x73\x00\x68\x00", 29);
				// XBE Hash
				UnleashX_XBE5.seekp(1532);
				UnleashX_XBE5.write("\x19\x0B\x5F\xA8\xA6\x1D\xB7\x7E\xE7\xB0\xDF\x2C\x3C\x33\x74\xA7\x00\xCD\xB8\x5C", 20);
				// Remove recent.dat creation
				UnleashX_XBE5.seekp(2353004);
				UnleashX_XBE5.write("\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 10);
				UnleashX_XBE5.close();
				
				std::ofstream UnleashX_XBE6(DashSettings, std::ios::binary | std::ios::in | std::ios::out);
				// Habibi Signature
				UnleashX_XBE6.seekp(0);
				UnleashX_XBE6.write("\x58\x42\x45\x48\xCF\x77\x26\xD8\xD6\x28\x51\x26\xFF\x13\xDC\x34\x4F\x54\x6B\xAB\xFE\xD8\xB9\xB6\xB3\xC1\x09\xD8\x7A\xAF\xF2\xFE\x6E\x6E\x76\xAA\x53\x99\x49\xA5\xA7\xFF\x79\xC6\xEF\x11\x05\x32\xA6\x37\x5F\xDD\x0B\xFE\x48\x93\x4A\x28\x7C\x95\x86\xF3\xE4\x52\x56\xFF\x98\x92\x2B\xAE\xF8\x50\x3D\xF1\x40\x15\x53\xF3\x52\xA5\x01\xDF\x2A\xBB\x0D\xFE\x50\x8D\x1C\x19\xF2\x81\x54\x57\x82\x77\x91\xF4\xB6\x0F\x8A\xCC\xC4\x37\xF2\xFB\x80\x5A\x52\xC8\x69\xAE\x99\x00\x7A\x28\x73\x62\xD6\x53\xA7\x20\xBF\x10\x51\xE6\x1A\x86\x90\x2F\xF1\x0F\xD2\x6C\xBB\x1A\x14\x63\xA1\x36\xC9\xCA\xE7\x84\x98\x5F\x06\xE7\xC6\x5A\x42\x98\x6C\xAC\xB7\xD6\x44\x36\x1F\x40\x25\x76\x11\x14\x2F\x8C\x8C\x63\x95\x22\x4F\x51\x3F\x7E\x24\x5D\x4E\x2A\x17\x7D\x72\x8F\x46\xB4\x8E\x47\x45\xEC\x1F\xDD\x67\x5A\x1A\x41\xED\xDA\x4C\x01\x6C\x78\x1D\x78\xA3\x0A\xC8\xD2\x0E\x32\xFE\x56\x4A\x9A\xAF\xF2\x02\xB0\xA0\x18\x27\xCE\x1B\xAC\x99\x66\x98\x49\x44\xDF\x38\x12\x8D\x9A\x35\x94\x64\x91\x2C\x02\xA7\xAF\xB2\x35\xA4\x28\x75\xBE\x37\x3A\xED\x8D\x5F\x43\x55\x36\xF3\x29\x45\x2A\xD5\x86", 260);
				// Change XBE Title ( Backup, Restore or Move )
				UnleashX_XBE6.seekp(400);
				UnleashX_XBE6.write("\x42\x00\x61\x00\x63\x00\x6B\x00\x75\x00\x70\x00\x2C\x00\x20\x00\x52\x00\x65\x00\x73\x00\x74\x00\x6F\x00\x72\x00\x65\x00\x20\x00\x6F\x00\x72\x00\x20\x00\x4D\x00\x6F\x00\x76\x00\x65", 45);
				// XBE Hash
				UnleashX_XBE6.seekp(1532);
				UnleashX_XBE6.write("\x19\x0B\x5F\xA8\xA6\x1D\xB7\x7E\xE7\xB0\xDF\x2C\x3C\x33\x74\xA7\x00\xCD\xB8\x5C", 20);
				// Remove recent.dat creation
				UnleashX_XBE6.seekp(2353004);
				UnleashX_XBE6.write("\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 10);
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