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
#include "xkhdd.h"
#include "xkeeprom.h"
#include <fstream>
#include <time.h>

//////////////////////////////////////////////////////////////////////
// Save path for the EEPROM files
//////////////////////////////////////////////////////////////////////
#define Backup_Path "E:\\Backups\\EEPROM\\"
#define Virtual_Path "C:\\NKPatcher\\Configs\\"
//#define Backup_Path "D:\\"

//////////////////////////////////////////////////////////////////////
// Config files, or if you will enabler files.
//////////////////////////////////////////////////////////////////////
#define Virtual_File "D:\\Virtual_EEPROM_Backup.enabled"
#define Reboot_File "D:\\Reboot.enabled"
#define Override_File "D:\\Standalone_Mode.enabled"
#define Backup "E:\\Prep\\done.xbe"
#define Update_Font "D:\\Update_Font.enabled"
#define Restore_Font "D:\\Restore_Font.enabled"

//////////////////////////////////////////////////////////////////////
// Create Directory paths.
// Cant seem to create a dir structure with one path, so had to use two.
//////////////////////////////////////////////////////////////////////
#define Backup_EEPROM_Save_Path1 "E:\\Backups"
#define Backup_EEPROM_Save_Path2 "E:\\Backups\\EEPROM"
#define Virtual_EEPROM_Save_Path1 "C:\\NKPatcher"
#define Virtual_EEPROM_Save_Path2 "C:\\NKPatcher\\Configs"

//////////////////////////////////////////////////////////////////////
// Setup Kernel selection for first run of softmod.
//////////////////////////////////////////////////////////////////////
#define FirstRunBin "E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\modules\\reload nkp\\firstrun.bin"
#define FirstRunXBE "E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\modules\\reload nkp\\firstrun.xbe"
#define UpdateRunBin "E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\modules\\reload nkp\\updaterun.bin"
#define NKPLoader "E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\modules\\reload nkp\\default.xbe"
#define SecondRun "E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\modules\\reload nkp\\secondrun.bin"
#define Font_Path "C:\\NKPatcher\\Configs\\fonts\\"
#define xbox_xtf_File_Path "C:\\xodash\\xbox.xtf"
#define Generic_Font_File "C:\\NKPatcher\\Configs\\Fonts\\generic.xtf"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

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

	std::ifstream dashprep(SecondRun);
	if (dashprep.good())
	{
		dashprep.close();
		remove(SecondRun);
		remove(FirstRunBin);
		remove(FirstRunXBE);
		remove("E:\\Prep\\default.xbe");
		RemoveDirectory("E:\\Prep");
		CreateDirectory("C:\\Dashboard", NULL);
		CreateDirectory("C:\\Dashboard\\Skins", NULL);
		CreateDirectory("C:\\Dashboard\\Skins\\Softmod", NULL);
		CopyFile("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\unleashx.xbe", "C:\\Dashboard\\default.xbe", NULL);
		CopyFile("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\configs\\dash.xml", "C:\\Dashboard\\config.xml", NULL);
		CopyFile("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\Skins\\Dashboard\\skin.xml", "C:\\Dashboard\\Skins\\Softmod\\skin.xml", NULL);
		CopyFile("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\Skins\\Dashboard\\UXSplash.jpg", "C:\\Dashboard\\Skins\\Softmod\\UXSplash.jpg", NULL);
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

		//Create E:\Backups\EEPROM directory
		CreateDirectory(Virtual_EEPROM_Save_Path1, NULL);
		CreateDirectory(Virtual_EEPROM_Save_Path2, NULL);
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
		strcat(tmpFileName, Font_Path);
		strcat(tmpFileName, tmpFileStr);
		ZeroMemory(tmpFileName2, FILENAME_MAX);
		strcat(tmpFileName2, "E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\");
		strcat(tmpFileName2, tmpFileStr);
		strcat(tmpFileName2, ".png");
		//WriteKERNELInfoFile(tmpFileName);
		
		std::ifstream stream(FirstRunBin);
		if (stream.good())
			{
				stream.close();
				remove(xbox_xtf_File_Path);
				remove(FirstRunBin);
				remove("C:\\NKPatcher\\Configs\\shadowc_off.bin");
				remove("E:\\Prep\\default.xbe");
				RemoveDirectory("E:\\Prep");
				remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\generic.png");
				remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.3944.1.png");
				remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.4034.1.png");
				remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.4627.1.png");
				remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.4817.1.png");
				remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.5101.1.png");
				remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.5530.1.png");
				remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.5713.1.png");
				remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.5838.1.png");
				CopyFile(Generic_Font_File, xbox_xtf_File_Path, 1);
				CopyFile("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\enabled.png", "E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\generic.png", NULL);
				CopyFile("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\enabled.png", SecondRun, NULL);
				XKUtils::LaunchXBE(FirstRunXBE);
			}
		
		std::ifstream updaterun(UpdateRunBin);
		if (updaterun.good())
			{
				updaterun.close();
				remove(xbox_xtf_File_Path);
				remove(UpdateRunBin);
				remove("C:\\NKPatcher\\Configs\\shadowc_off.bin");
				remove("E:\\Prep\\default.xbe");
				RemoveDirectory("E:\\Prep");
				remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\generic.png");
				remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.3944.1.png");
				remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.4034.1.png");
				remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.4627.1.png");
				remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.4817.1.png");
				remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.5101.1.png");
				remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.5530.1.png");
				remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.5713.1.png");
				remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.5838.1.png");
				CopyFile(Generic_Font_File, xbox_xtf_File_Path, 1);
				CopyFile("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\enabled.png", "E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\generic.png", NULL);
				XKUtils::LaunchXBE(NKPLoader);
			}

		std::ifstream updatefont(Update_Font);
		if (updatefont.good())
			{
				updatefont.close();
				remove(xbox_xtf_File_Path);
				remove(Update_Font);
				remove("C:\\NKPatcher\\Configs\\shadowc_off.bin");
				remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\generic.png");
				remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.3944.1.png");
				remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.4034.1.png");
				remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.4627.1.png");
				remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.4817.1.png");
				remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.5101.1.png");
				remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.5530.1.png");
				remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.5713.1.png");
				remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.5838.1.png");
				CopyFile(tmpFileName, xbox_xtf_File_Path, 1);
				CopyFile("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\enabled.png", tmpFileName2, 1);
				XKUtils::XBOXRebootToDash();
			}
			
		std::ifstream restorefont(Restore_Font);
		if (restorefont.good())
			{
				restorefont.close();
				remove(xbox_xtf_File_Path);
				remove(Restore_Font);
				remove("C:\\NKPatcher\\Configs\\shadowc_off.bin");
				remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\generic.png");
				remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.3944.1.png");
				remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.4034.1.png");
				remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.4627.1.png");
				remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.4817.1.png");
				remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.5101.1.png");
				remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.5530.1.png");
				remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.5713.1.png");
				remove("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\K.1.0.5838.1.png");
				CopyFile(Generic_Font_File, xbox_xtf_File_Path, 1);
				CopyFile("E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\enabled.png", "E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\toggles\\font\\generic.png", NULL);
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