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
#include "Xbox Softmod Tool Kit.h"
#include <d3d8.h>
#include "xkhdd.h"
#include "xkeeprom.h"
#include <time.h>
#include <cstdio>
#include <sys/stat.h>
extern "C" XPP_DEVICE_TYPE XDEVICE_TYPE_IR_REMOTE_TABLE;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Shared paths
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define Retail_Bios_Hash_File				"D:\\RetailMD5Hashes.ini"
#define Retail_Bios_Save_Path				"E:\\Backups\\BIOS\\"
#define PrepDir								"E:\\Prep\\"
#define PrepXBE			PrepDir				"default.xbe"
#define CleanXBE		PrepDir				"cleanup.xbe"
#define LXblastOS		PrepDir				"load_xblast_os.bin"
#define enableHD		PrepDir				"enableHD.bin"
#define NoVCBin			PrepDir				"novc.bin"
#define BootCFG			PrepDir				"boot.cfg"
#define BiosDir								"C:\\bios\\"
#define BiosBin			BiosDir				"bios.bin"
#define EvoxXBE								"S:\\evoxdash.xbe"
#define IndBios								"S:\\ind-bios.cfg"
#define SBiosDir							"S:\\bios\\"
#define SBiosBin		SBiosDir			"bios.bin"
#define RebootFile		PrepDir				"reboot.bin"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LED Colours
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define	LED_Orange							XKUtils::SetXBOXLEDStatus(XKUtils::LED_REGISTER_CYCLE0_GREEN|XKUtils::LED_REGISTER_CYCLE0_RED|XKUtils::LED_REGISTER_CYCLE1_GREEN|XKUtils::LED_REGISTER_CYCLE1_RED|XKUtils::LED_REGISTER_CYCLE2_GREEN|XKUtils::LED_REGISTER_CYCLE2_RED|XKUtils::LED_REGISTER_CYCLE3_GREEN|XKUtils::LED_REGISTER_CYCLE3_RED)
#define LED_Green							XKUtils::SetXBOXLEDStatus(XKUtils::LED_REGISTER_CYCLE0_GREEN|XKUtils::LED_REGISTER_CYCLE1_GREEN|XKUtils::LED_REGISTER_CYCLE2_GREEN|XKUtils::LED_REGISTER_CYCLE3_GREEN)
#define	LED_Red								XKUtils::SetXBOXLEDStatus(XKUtils::LED_REGISTER_CYCLE0_RED|XKUtils::LED_REGISTER_CYCLE0_RED|XKUtils::LED_REGISTER_CYCLE1_RED|XKUtils::LED_REGISTER_CYCLE1_RED|XKUtils::LED_REGISTER_CYCLE2_RED|XKUtils::LED_REGISTER_CYCLE2_RED|XKUtils::LED_REGISTER_CYCLE3_RED|XKUtils::LED_REGISTER_CYCLE3_RED)
#define LED_Flash_Orange					XKUtils::SetXBOXLEDStatus(XKUtils::LED_REGISTER_CYCLE0_GREEN|XKUtils::LED_REGISTER_CYCLE0_RED|XKUtils::LED_REGISTER_CYCLE3_GREEN|XKUtils::LED_REGISTER_CYCLE3_RED)
#define LED_Flash_Green						XKUtils::SetXBOXLEDStatus(XKUtils::LED_REGISTER_CYCLE0_GREEN|XKUtils::LED_REGISTER_CYCLE3_GREEN)
#define LED_Flash_Red						XKUtils::SetXBOXLEDStatus(XKUtils::LED_REGISTER_CYCLE0_RED|XKUtils::LED_REGISTER_CYCLE3_RED)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Main Code
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ConfigMagicApp::CreateBiosBackup()
{
	XKUtils::MountDiskE();
	((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("BACKING UP BIOS");
	((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Please wait...");
	Render();
	Sleep(2000);
	CreateDirectory("E:\\Backups", NULL);
	CreateDirectory("E:\\Backups\\BIOS", NULL);
	BYTE data;
	char *BIOS_Name;
	CStdString strBiosName;
	DWORD addr        = FLASH_BASE_ADDRESS;
	DWORD addr_kernel = KERNEL_BASE_ADDRESS;
	CXBoxFlash mbFlash;
	char * flash_copy;

	flash_copy = (char *) malloc(0x100000);

	BIOS_Name     = (char*) malloc(100);

	struct Bios *Listone = LoadBiosSigns();

	if( !Listone )
	{
		free(BIOS_Name);
	}

	for(int loop=0;loop<0x100000;loop++)
	{
		data = mbFlash.Read(addr++);
		flash_copy[loop] = data;
	}

	((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("CHECKING BIOS");
	Render();
	Sleep(1000);
	// Detect a 1024 KB Bios MD5
	MD5Buffer (flash_copy,0,1024);
	strcpy(BIOS_Name,CheckMD5(Listone, MD5_Sign));
	if ( strcmp(BIOS_Name, "Unknown") == 0)
	{ 
		bios_dumped = 0;
	}
	else
	{		
		bios_dumped = 1;
	}
	strBiosName = BIOS_Name;
	if ( (bios_dumped == 1) && (strBiosName == "Retail 3944") || (strBiosName == "Retail 4034") || (strBiosName == "Retail 4134") || (strBiosName == "Retail 4817") || (strBiosName == "Retail 5101") || (strBiosName == "Retail 5530") || (strBiosName == "Retail 5713") || (strBiosName == "Retail 5838") )
	{
		((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText(strBiosName+" BIOS DETECTED");
		((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Backing Up BIOS");
		Render();
		Sleep(2000);
		CreateDirectory("E:\\Backups", NULL);
		CreateDirectory("E:\\Backups\\BIOS", NULL);
		FILE *fp;
		DWORD addr        = FLASH_BASE_ADDRESS;
		DWORD addr_kernel = KERNEL_BASE_ADDRESS;
		char * flash_copy, data;
		CXBoxFlash mbFlash;

		flash_copy = (char *) malloc(0x100000);
		
		if((fp = fopen(Retail_Bios_Save_Path+strBiosName+" Bios.bin", "wb")) != NULL)
		{
			for(int loop=0;loop<0x100000;loop++)
			{
				data = mbFlash.Read(addr++);
				flash_copy[loop] = data;
			}
			if ( (MD5Buffer(flash_copy,0,256) == MD5Buffer(flash_copy,262144,256)) && (MD5Buffer(flash_copy,524288,256)== MD5Buffer(flash_copy,786432,256)) )
			{
				fwrite(flash_copy, 0x40000, 1, fp);
				fclose(fp);
			}
		}
		else
		{
			if ((MD5Buffer(flash_copy,0,512)) == (MD5Buffer(flash_copy,524288,512)))
			{
				fwrite(flash_copy, 0x80000, 1, fp);
				fclose(fp);
			}
			else
			{
				fwrite(flash_copy, 0x100000, 1, fp);
				fclose(fp);
			}
		}
		((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Complete");
		Render();
		Sleep(1500);
	}
	XKUtils::XBOXRebootToDash();
}

void ConfigMagicApp::CheckBios()
{
	XKUtils::MountDiskE();
	BYTE data;
	char *BIOS_Name;
	CStdString strBiosName;
	DWORD addr        = FLASH_BASE_ADDRESS;
	DWORD addr_kernel = KERNEL_BASE_ADDRESS;
	CXBoxFlash mbFlash;
	char * flash_copy;

	flash_copy = (char *) malloc(0x100000);

	BIOS_Name  = (char*) malloc(100);

	struct Bios *Listone = LoadBiosSigns();

	if( !Listone )
	{
		free(BIOS_Name);
	}

	for(int loop=0;loop<0x100000;loop++)
	{
		data = mbFlash.Read(addr++);
		flash_copy[loop] = data;
	}

	// Detect a 1024 KB Bios MD5
	MD5Buffer (flash_copy,0,1024);
	strcpy(BIOS_Name,CheckMD5(Listone, MD5_Sign));
	if ( strcmp(BIOS_Name, "Unknown") == 0)
	{ 
		bios_dumped = 0;
	}
	else
	{		
		bios_dumped = 1;
	}
	strBiosName = BIOS_Name;
	
	if (file_exist(RebootFile))
	{
		remove(RebootFile);
		DisableHDRes();
		XKUtils::XBOXPowerCycle();
	}
	else
	{
		if (file_exist(LXblastOS))
		{
			remove(LXblastOS);
			LED_Flash_Red;
			XKUtils::LaunchXBE("E:\\Prep\\xBlast OS.xbe");
		}
		else
		{
			if (file_exist(enableHD))
			{
				remove(enableHD);
				EnableHDRes();
				XKUtils::XBOXReset();
			}
		}
	}
	if (bios_dumped == 1)
	{
		if ( (strBiosName == "Retail 3944") || (strBiosName == "Retail 4034") || (strBiosName == "Retail 4134") || (strBiosName == "Retail 4817") || (strBiosName == "Retail 5101") || (strBiosName == "Retail 5530") || (strBiosName == "Retail 5713") || (strBiosName == "Retail 5838") )
		{
			LED_Flash_Orange;
			XKUtils::LaunchXBE("E:\\Prep\\softmod.xbe");
		}
	}
	else
	{
		LED_Flash_Red;
		XKUtils::LaunchXBE("E:\\Prep\\hardmod.xbe");
	}
}

void ConfigMagicApp::DisableHDRes()
{
	m_pXKEEPROM->ReadFromXBOX();
	m_pXKEEPROM->SetVideoFlagsString("00000000"); // Set Videoflags to (0A) 480p, 720p or (4A) PAL60, 480p and 720p
	Force_Write_XBOX_EEPROM();
}
void ConfigMagicApp::EnableHDRes()
{
	m_pXKEEPROM->ReadFromXBOX();
	m_pXKEEPROM->SetVideoFlagsString("00004A00"); // Set Videoflags to (0A) 480p, 720p or (4A) PAL60, 480p and 720p
	Force_Write_XBOX_EEPROM();
}

void ConfigMagicApp::Force_Write_XBOX_EEPROM()
{
	//Decrypting EEPROM, if it fails.. Display fail message!
	if (m_pXKEEPROM->Decrypt())
	{
		//Re-Encrypt with correct Version...
		m_EnryptedRegionValid = TRUE;
		m_XBOX_Version = m_pXKEEPROM->GetXBOXVersion();
		m_pXKEEPROM->EncryptAndCalculateCRC(m_XBOX_Version);
		if (m_pXKEEPROM->IsEncrypted())
		{
			m_pXKEEPROM->WriteToXBOX();
			LED_Flash_Green;
		}
		else
		{
			LED_Flash_Red;
		}
	}
	else
	{
		//Encrypted Region is Invalid.. DONT Allow EEPROM Update..
		m_EnryptedRegionValid = FALSE;
		LED_Flash_Red;
	}
}

struct Bios * ConfigMagicApp::LoadBiosSigns()
{
	FILE *infile;

	if ((infile = fopen(Retail_Bios_Hash_File,"r")) == NULL)
	{
		return NULL;
	}
	else
	{
		struct Bios * Listone = (struct Bios *)calloc(1000, sizeof(struct Bios));
		int cntBioses=0;
		char buffer[255];
		char stringone[255];
		do
		{
			fgets(stringone,255,infile);
			if  (stringone[0] != '#')
			{
				if (strstr(stringone,"=")!= NULL)
				{
					strcpy(Listone[cntBioses].Name,ReturnBiosName(buffer, stringone));
					strcpy(Listone[cntBioses].Signature,ReturnBiosSign(buffer, stringone));
					cntBioses++;
				}
			}
		} while( !feof( infile ) && cntBioses < 999 );
		fclose(infile);
		strcpy(Listone[cntBioses++].Name,"\0");
		strcpy(Listone[cntBioses++].Signature,"\0");
		return Listone;
	}
}

char* ConfigMagicApp::MD5Buffer(char *buffer, long PosizioneInizio,int KBytes)
{
	XBMC::XBMC_MD5 mdContext;
	CStdString md5sumstring;
	mdContext.append((unsigned char *)(buffer + PosizioneInizio), KBytes * 1024);
	mdContext.getDigest(md5sumstring);
	strcpy(MD5_Sign, md5sumstring.c_str());
	return MD5_Sign;
}

char* ConfigMagicApp::ReturnBiosName(char *buffer, char *str)
{
	int cnt1,cnt2,i;
	cnt1=cnt2=0;

	for (i=0;i<255;i++) buffer[i]='\0';

	while (str[cnt2] != '=')
	{
		buffer[cnt1]=str[cnt2];
		cnt1++;
		cnt2++;
	}
	buffer[cnt1++]='\0';
	return buffer;
}
char* ConfigMagicApp::ReturnBiosSign(char *buffer, char *str)
{
	int cnt1,cnt2,i;
	cnt1=cnt2=0;
	for (i=0;i<255;i++) buffer[i]='\0';
	while (str[cnt2] != '=') cnt2++;
	cnt2++;
	while (str[cnt2] != NULL)
	{
		if ( str[cnt2] != ' ' )
		{
			buffer[cnt1]=toupper(str[cnt2]);
			cnt1++;
			cnt2++;
		}
		else cnt2++;
	}
	buffer[cnt1++]='\0';
	return buffer;
}
char* ConfigMagicApp::CheckMD5 (struct Bios *Listone, char *Sign)
{
	int cntBioses;
	cntBioses=0;
	do
	{
		if  (strstr(Listone[cntBioses].Signature, Sign) != NULL)
		{ return (Listone[cntBioses].Name);   }
		cntBioses++;
	}
	while( strcmp(Listone[cntBioses].Name,"\0") != 0);
	return ("Unknown");
}

int ConfigMagicApp::file_exist(const char *name)
{
	struct stat   buffer;
	return (stat (name, &buffer) == 0);
}

HRESULT ConfigMagicApp::Initialize()
{
	XKUtils::MountDiskE();
	HRESULT retVal = S_OK;
	m_pXKEEPROM = new XKEEPROM();
	m_EnryptedRegionValid = FALSE;
	m_XBOX_EEPROM_Current = FALSE;
	//incase path is on these drives..
	// Check for font files
	retVal = m_XBFont1.Create(m_pd3dDevice, "D:\\Media\\Font_Message.xpr");
	retVal = m_XBFont2.Create(m_pd3dDevice, "D:\\Media\\Font_Status.xpr");
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
	// ((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("INITIALISING");
	// ((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Please Wait");
	//CreateBiosBackup();
	CheckBios();
	XKUtils::XBOXReset();
	return retVal;
}

void ConfigMagicApp::InitScreen()
{
	m_pFrmStatus = new XKControl_Panel("Status", m_bgSprite, XOffset, YOffset, ScreenWidth,ScreenHeight);
	// Check for image file
	m_pFrmStatus->LoadImage("D:\\Media\\background.png");
	txtStatusMsg = new XKControl_TextBox("txtStatusMsg", m_bgSprite);
	txtStatusMsg->SetBitmapFont(&m_XBFont1);
	txtStatusMsg->SetTextColor(m_clrMessageTextColor);
	txtStatusMsg->SetTextCentered(TRUE);
	txtStatusMsg->SetTextShadow(FALSE);
	txtStatus = new XKControl_TextBox("txtStatus", m_bgSprite);
	txtStatus->SetBitmapFont(&m_XBFont2);
	txtStatus->SetTextColor(m_clrStatusTextColor);
	txtStatus->SetTextCentered(TRUE);
	txtStatus->SetTextShadow(FALSE);
	m_pFrmStatus->AddControl(txtStatusMsg,  0, 180,  ScreenWidth, 30);
	m_pFrmStatus->AddControl(txtStatus,		0, 400,  ScreenWidth, 30);
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
	m_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE,  FALSE); //some reason rendergradientbg doesn't wrok if this is false...
	RenderGradientBackground(0, 0);
	//Render Background Image..
	//if (m_ActiveForm->IsImageLoaded())
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
	m_clrMessageTextColor		= 0xffaee239;
	m_clrStatusTextColor		= 0xff626473;
}

//Application start point
void main()
{
	LED_Orange;
	ConfigMagicApp xbapp;
	xbapp.Create();
	xbapp.SetAppTitle("Xbox Softmodding Tool Extras Disc");
	xbapp.Run();
}