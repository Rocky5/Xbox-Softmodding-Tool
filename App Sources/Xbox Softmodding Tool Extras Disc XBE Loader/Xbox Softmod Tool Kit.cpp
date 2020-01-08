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
#include "ShadowC Header.h"
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
#define Retail_Bios_Hash_File				"D:\\Media\\RetailMD5Hashes.ini"
#define Retail_Bios_Save_Path				"E:\\Backups\\BIOS\\bios.bin"
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
void ConfigMagicApp::CheckBios()
{
	if(g_Gamepads[0].hDevice && g_Gamepads[0].bPressedAnalogButtons[XINPUT_GAMEPAD_BLACK])
	{
		if (file_exist("d:\\Softmod\\Applications\\Enigmah-X\\default.xbe"))
		{
			((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Loading Enigmah-X");
			Render();
			Sleep(3000);
			XKUtils::LaunchXBE("d:\\Softmod\\Applications\\Enigmah-X\\default.xbe");
		}
	}
	XKUtils::MountDiskS();
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
	// ((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("CHECKING BIOS");
	// Render();
	// Sleep(1000);
	// Detect a 1024 KB Bios MD5
	MD5Buffer (flash_copy,0,1024);
	strcpy(BIOS_Name,CheckMD5(Listone, MD5_Sign));
	strBiosName = BIOS_Name;
	if ( strcmp(BIOS_Name, "Unknown") == 0)
	{ 
		LED_Flash_Red;
		((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("UNOFFICIAL/HACKED BIOS MODE");
		((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Loading Hardmod menu");
		Render();
		Sleep(3000);
		XKUtils::LaunchXBE("d:\\Hardmod\\default.xbe");
	}
	else
	{
		((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText(strBiosName+" BIOS DETECTED");
		if (file_exist("S:\\nkpatcher\\rescuedash\\resoftmod files.zip"))
		{
			if (g_Gamepads[0].hDevice && g_Gamepads[0].wPressedButtons & XINPUT_GAMEPAD_START)
			{
				LED_Flash_Red;
				((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Loading Xbox Softmodding Tools TSOP Flasher Menu");
				Render();
				Sleep(3000);
				XKUtils::LaunchXBE("d:\\FlashSM\\default.xbe");
			}
			else if(g_Gamepads[0].hDevice && g_Gamepads[0].bPressedAnalogButtons[XINPUT_GAMEPAD_WHITE])
			{
				((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Loading XBST menu with ShadowC disabled");
				Render();
				Sleep(3000);
				XKUtils::LaunchXBE("d:\\Softmod\\shadowc.xbe");
			}
			else
			{
				LED_Flash_Orange;
				((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Loading Xbox Softmodding Tools Menu");
				Render();
				Sleep(3000);
				XKUtils::LaunchXBE("d:\\Softmod\\default.xbe");
			}
		}
		else
		{
			LED_Flash_Green;
			((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Loading Other Softmod Menu");
			Render();	
			Sleep(3000);
			XKUtils::LaunchXBE("d:\\OtherSM\\default.xbe");
		}
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
	XKUtils::MountDiskC();
	XKUtils::MountDiskE();
	XKUtils::MountDiskS();
	XKUtils::MountDiskX();
	HRESULT retVal = S_OK;
	m_pXKEEPROM = new XKEEPROM();
	m_EnryptedRegionValid = FALSE;
	m_XBOX_EEPROM_Current = FALSE;
	//incase path is on these drives..
	// Check for font files
	retVal = m_XBFont1.Create(m_pd3dDevice, "D:\\media\\Font_Message.xpr");
	retVal = m_XBFont2.Create(m_pd3dDevice, "D:\\media\\Font_Status.xpr");
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
	((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("INITIALISING");
	((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Please Wait");
	Render();
	Sleep(1000);
	XBInput_GetInput();
	CheckBios();
	return retVal;
}
void ConfigMagicApp::InitScreen()
{
	m_pFrmStatus = new XKControl_Panel("Status", m_bgSprite, XOffset, YOffset, ScreenWidth,ScreenHeight);
	// Check for image file
	m_pFrmStatus->LoadImage("D:\\media\\background.png");
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