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

// ConfigMagicApp.cpp: implementation of the ConfigMagicApp class.
//
//////////////////////////////////////////////////////////////////////

#include "ConfigMagicApp.h"
#include <d3d8.h>
#include "xkhdd.h"
#include "xkeeprom.h"
#include <fstream>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ConfigMagicApp::ConfigMagicApp()
{

}

ConfigMagicApp::~ConfigMagicApp()
{

	delete m_pXKEEPROM;
	delete pxkconsole;
	delete m_pMainMenu;
	delete m_kbdEditBox;
	delete m_pFrmWarning;
	delete m_pFrmStatus;
	delete m_pFrmInfo;

	delete txtStatusMsg;
	delete txtStatusMsg2;
	delete txtTitle;
	delete txtSerial;
	delete txtMAC;
	delete txtVideoFormat;
	delete txtOnlineKey;
	delete txtDVDZone;
	delete txtDVDModel;
	delete txtHDDKey;
	delete txtHDDModel;
	delete txtHDDSerial;
	delete txtHDDStatus;
	delete txtHDDPwd;
	delete lblSerial;
	delete lblMAC;
	delete lblVideoFormat;
	delete lblOnlineKey;
	delete lblDVDZone;
	delete lblDVDModel;
	delete lblHDDKey;
	delete lblHDDModel;
	delete lblHDDSerial;
	delete lblHDDPwd;
	delete lblXBVersion;
	delete txtStatus;
}


void ConfigMagicApp::SectionHandler()
{

	switch(XConfSections)
	{
		case(STARTUP_SCREEN):
				StartupScreen();
				break;
		case(WARNING_SCREEN):
				WarningScreen();
				break;
		case(LOAD_XBOX_EEPROM_SCREEN):
				Load_XBOX_EEPROM_Screen();
				break;
		case(WRITE_XBOX_EEPROM_SCREEN):
				Write_XBOX_EEPROM_Screen();
				Load_XBOX_EEPROM_Screen();
				break;
		case(LOAD_BIN_EEPROM_SCREEN):
				Load_BIN_EEPROM_Screen();
				break;
		case(LOAD_CFG_EEPROM_SCREEN):
				Load_CFG_EEPROM_Screen();
				break;
		case(EDIT_EEPROM_SCREEN):
				Edit_EEPROM_Screen();
				break;
		case(CREATE_BACKUP_SCREEN):
				Create_Backup_Files_Screen();
				break;
		case(LOCK_HDD_SCREEN):
				Lock_HDD_Screen();
				break;
		case(UNLOCK_HDD_SCREEN):
				Unlock_HDD_Screen();
				break;
		case(XBOX_CONFIG_SCREEN):
				XBOXConfigScreen();
				break;
		case(EXIT_SCREEN):
				ExitScreen();
				break;
	}
}

void ConfigMagicApp::StartupScreen()
{
	if (!WaitForInput)
	{
		
		m_ActiveForm = m_pFrmWarning;
		((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("");
		pxkconsole->Clear();
		m_ActiveForm->SetVisible(FALSE);
		Render();
		XConfSections = WARNING_SCREEN;
		WaitForInput = FALSE;

		std::ifstream splashwmv("d:\\media\\splash.wmv");
		if (splashwmv.good())
		{
			splashwmv.close();
			PlayVideo("d:\\media\\splash.wmv");
		}
		m_ActiveForm->SetVisible(TRUE);
	}
}

void ConfigMagicApp::WarningScreen()
{
	if (!WaitForInput)
	{

		//FAST Flash Orange to get into Blind Restore..
		XKUtils::SetXBOXLEDStatus(XKUtils::LED_REGISTER_CYCLE0_GREEN|XKUtils::LED_REGISTER_CYCLE0_RED|XKUtils::LED_REGISTER_CYCLE2_GREEN|XKUtils::LED_REGISTER_CYCLE2_RED);

		m_ActiveForm = m_pFrmWarning;
		((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Press 'START' to Continue or 'BACK' to Exit");
		pxkconsole->Clear();

#ifndef _DEBUGX
		WaitForInput = TRUE;
#else
		XConfSections = LOAD_XBOX_EEPROM_SCREEN;
		WaitForInput = FALSE;		
#endif

	}
	else
	{
        if (m_DefaultGamepad.wPressedButtons & XINPUT_GAMEPAD_START)
		{
			XKUtils::SetXBOXLEDStatus(XKUtils::LED_REGISTER_CYCLE0_GREEN|XKUtils::LED_REGISTER_CYCLE1_GREEN|XKUtils::LED_REGISTER_CYCLE2_GREEN|XKUtils::LED_REGISTER_CYCLE3_GREEN);
			XConfSections = LOAD_XBOX_EEPROM_SCREEN;
			WaitForInput = FALSE;

		}

        if (m_DefaultGamepad.wPressedButtons & XINPUT_GAMEPAD_BACK)
		{
			XKUtils::SetXBOXLEDStatus(XKUtils::LED_REGISTER_CYCLE0_GREEN|XKUtils::LED_REGISTER_CYCLE1_GREEN|XKUtils::LED_REGISTER_CYCLE2_GREEN|XKUtils::LED_REGISTER_CYCLE3_GREEN);
			XConfSections = EXIT_SCREEN;
			WaitForInput = FALSE;
		}


        if( m_DefaultGamepad.bAnalogButtons[XINPUT_GAMEPAD_LEFT_TRIGGER] > 0 )
        {
            if( m_DefaultGamepad.bAnalogButtons[XINPUT_GAMEPAD_RIGHT_TRIGGER] > 0 )
            {
                if(m_DefaultGamepad.bPressedAnalogButtons[XINPUT_GAMEPAD_WHITE])
                {
					m_bBlind = TRUE;
					XConfSections = LOAD_XBOX_EEPROM_SCREEN;
					WaitForInput = FALSE;
                }
            }
        }

	}
}


void ConfigMagicApp::XBOXConfigScreen()
{
	
	if (m_pMainMenu->IsBusy())
		return;
	
	if (!WaitForInput)
	{
		ResetConfigScreen();

		m_ActiveForm = m_pFrmInfo;
		((LPXKControl_TextBox) m_pFrmInfo->GetControl("txtStatus"))->SetText("Please Wait..");
		Render();
		//HDD Object & ATA Command Structure..
		XKHDD::ATA_COMMAND_OBJ hddcommand;
		UCHAR HddPass[32];
		CHAR TempString1[100];
		DWORD slen = 0;

		ZeroMemory(HddPass, 32);

		//Decrypting EEPROM, if it fails.. Display fail message!
		if (m_pXKEEPROM->Decrypt())
		{

			m_EnryptedRegionValid = TRUE;
			//Print Serial Number
			slen = 13;
			ZeroMemory(TempString1, 100);
			m_pXKEEPROM->GetSerialNumberString(TempString1, &slen);
			((LPXKControl_TextBox) m_pFrmInfo->GetControl("txtSerial"))->SetText(TempString1);

			//Print MAC Address..
			ZeroMemory(TempString1, 100);
			m_pXKEEPROM->GetMACAddressString(TempString1, &slen);
			((LPXKControl_TextBox) m_pFrmInfo->GetControl("txtMAC"))->SetText(TempString1);

			//Print Online Key..
			slen = 16;
			ZeroMemory(TempString1, 100);
			m_pXKEEPROM->GetOnlineKeyString(TempString1, &slen);
			((LPXKControl_TextBox) m_pFrmInfo->GetControl("txtOnlineKey"))->SetText(TempString1);

			//Print Video Standard & XBE Region...
			DWORD xberegion = m_pXKEEPROM->GetXBERegionVal();

			DWORD VideoStandard = m_pXKEEPROM->GetVideoStandardVal();
			switch (VideoStandard)
			{
			case (XKEEPROM::NTSC_M):
				{
					sprintf(TempString1, "NTSC, Region %d\0", xberegion);
					break;
				}
			case (XKEEPROM::PAL_I):
				{
					sprintf(TempString1, "PAL, Region %d\0", xberegion);
					break;
				}
			default:
				{
					sprintf(TempString1, "UNKNOWN, Region %d\0", xberegion);
					break;
				}

			}
			((LPXKControl_TextBox) m_pFrmInfo->GetControl("txtVideoFormat"))->SetText(TempString1);


			//Print DVD Zone ..
			ZeroMemory(TempString1, 100);
			DWORD RegionCode = m_pXKEEPROM->GetDVDRegionVal();
			sprintf(TempString1, "%d\0", RegionCode);
			((LPXKControl_TextBox) m_pFrmInfo->GetControl("txtDVDZone"))->SetText(TempString1);

			//Print HDD Key...
			slen = 16;
			m_pXKEEPROM->GetHDDKeyString((LPSTR)&TempString1, &slen);
			((LPXKControl_TextBox) m_pFrmInfo->GetControl("txtHDDKey"))->SetText((LPSTR)&TempString1);


			if (m_XBOX_EEPROM_Current) //Only Show Data if real eeprom data..
			{
				//Get IDE_ATAPI_IDENTIFY Data for DVD-ROM
				((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Querying DVD..");
				Render();

				ZeroMemory(&hddcommand, sizeof(XKHDD::ATA_COMMAND_OBJ));
				hddcommand.DATA_BUFFSIZE = 0;
				hddcommand.IPReg.bDriveHeadReg = IDE_DEVICE_SLAVE;
				hddcommand.IPReg.bCommandReg = IDE_ATAPI_IDENTIFY;
				XKHDD::SendATACommand(IDE_PRIMARY_PORT, &hddcommand, IDE_COMMAND_READ);
				
				//Print DVD Model...
				ZeroMemory(TempString1, 100);
				XKHDD::GetIDEModel(hddcommand.DATA_BUFFER, TempString1, &slen);
				((LPXKControl_TextBox) m_pFrmInfo->GetControl("txtDVDModel"))->SetText(TempString1);

				//Get IDE_ATA_IDENTIFY Data for HDD ..
				((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Querying HDD..");
				Render();

				ZeroMemory(&hddcommand, sizeof(XKHDD::ATA_COMMAND_OBJ));
				hddcommand.DATA_BUFFSIZE = 0;
				hddcommand.IPReg.bDriveHeadReg = IDE_DEVICE_MASTER;
				hddcommand.IPReg.bCommandReg = IDE_ATA_IDENTIFY;
				XKHDD::SendATACommand(IDE_PRIMARY_PORT, &hddcommand, IDE_COMMAND_READ);

				XKEEPROM::EEPROMDATA tmpData;
				m_pXKEEPROM->GetEEPROMData(&tmpData);
				XKHDD::GenerateHDDPwd(tmpData.HDDKkey, hddcommand.DATA_BUFFER, HddPass);
				
				//Print HDD Model...
				ZeroMemory(TempString1, 100);
				XKHDD::GetIDEModel(hddcommand.DATA_BUFFER, TempString1, &slen);
				((LPXKControl_TextBox) m_pFrmInfo->GetControl("txtHDDModel"))->SetText(TempString1);

				//Print HDD Serial...
				ZeroMemory(TempString1, 100);
				XKHDD::GetIDESerial(hddcommand.DATA_BUFFER, TempString1, &slen);
				((LPXKControl_TextBox) m_pFrmInfo->GetControl("txtHDDSerial"))->SetText(TempString1);

				//Print HDD Password...
				ZeroMemory(TempString1, 100);
				XKGeneral::BytesToHexStr(HddPass, 20, TempString1);
				strupr(TempString1);
				((LPXKControl_TextBox) m_pFrmInfo->GetControl("txtHDDPwd"))->SetText(TempString1);

				//Print ATA Locked State
				DWORD SecStatus = XKHDD::GetIDESecurityStatus(hddcommand.DATA_BUFFER);
				if (!(SecStatus & IDE_SECURITY_SUPPORTED))
					sprintf(TempString1, "%s", "LOCKING NOT SUPPORTED");
				if ((SecStatus & IDE_SECURITY_SUPPORTED) && !(SecStatus & IDE_SECURITY_ENABLED))
					sprintf(TempString1, "%s", "NOT LOCKED");
				if ((SecStatus & IDE_SECURITY_SUPPORTED) && (SecStatus & IDE_SECURITY_ENABLED))
					sprintf(TempString1, "%s", "LOCKED");
				if (SecStatus & IDE_SECURITY_FROZEN)
					sprintf(TempString1, "%s FROZEN", TempString1);
				if (SecStatus & IDE_SECURITY_COUNT_EXPIRED)
					sprintf(TempString1, "%s", "REQUIRES RESET");

				((LPXKControl_TextBox) m_pFrmInfo->GetControl("txtHDDStatus"))->SetText(TempString1);
			}

			
			//Re Encrypt EEPROM with Current XBOX Version
			m_pXKEEPROM->EncryptAndCalculateCRC(m_XBOX_Version);

		}
		else
		{

			//Encrypted Region is Invalid.. DONT Allow Locking/Unlocking..
			m_EnryptedRegionValid = FALSE;

			m_ActiveForm = m_pFrmStatus;
			((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("EEPROM ENCRYPTED REGION INVALID");

		}

		
		((LPXKControl_TextBox) m_pFrmInfo->GetControl("txtStatus"))->SetText("Press START to open the Menu");
		WaitForInput = TRUE;

	}
	else
	{
		//You can change or add additional button/Trigger Handlers here....

        if (m_DefaultGamepad.wPressedButtons & XINPUT_GAMEPAD_START)
		{
			if (!m_pMainMenu->IsVisible())
			{
			
				m_pMainMenu->SetVisible(TRUE);
				((LPXKControl_TextBox) m_pFrmInfo->GetControl("txtStatus"))->SetText("Press BACK to close the Menu");
			}
		}

		if (m_DefaultGamepad.wPressedButtons & XINPUT_GAMEPAD_BACK)
		{
			if (m_pMainMenu->IsVisible())
			{
				m_pMainMenu->SetVisible(FALSE);
				((LPXKControl_TextBox) m_pFrmInfo->GetControl("txtStatus"))->SetText("Press START to open the Menu");
			}
		}

		
        if (m_DefaultGamepad.bPressedAnalogButtons[XINPUT_GAMEPAD_A]) 
		{


			
			
			if(m_pMainMenu->IsVisible())
			{

				string strItem = m_pMainMenu->GetSelectedMenuItem();
				LPCSTR tmpSelected = strItem.c_str();

				ProcessMenuSelect(tmpSelected);

				m_pMainMenu->SetVisible(FALSE);
				((LPXKControl_TextBox) m_pFrmInfo->GetControl("txtStatus"))->SetText("Press START to open the Menu");

			}
			else if ((!m_pFrmInfo->IsControlsLocked()) && (!m_kbdEditBox->IsVisible()))
			{
				
				m_kbdEditBox->SetText(m_pFrmInfo->GetFocusControl()->GetText());
				m_kbdEditBox->SetVisible(TRUE);
			}
			else if (m_kbdEditBox->IsVisible())
			{
				XKControl_Keyboard::KEY_ITEM_TYPE KeyPressed = m_kbdEditBox->GetSelectedKeyType();

				switch (KeyPressed)
				{
					case (XKControl_Keyboard::CTRL_CANCEL_KEY):
						m_kbdEditBox->SetVisible(FALSE);
						break;
					case (XKControl_Keyboard::CTRL_ENTER_KEY):
						m_pFrmInfo->GetFocusControl()->SetText(m_kbdEditBox->GetText());
						Build_EEPROM_From_Screen();
						m_kbdEditBox->SetVisible(FALSE);
						//Rebuild the screen...
						WaitForInput = FALSE;
						break;
					default:
						m_kbdEditBox->PressSelectedKey();
				}
			}
				
		}

        if (m_DefaultGamepad.wPressedButtons & XINPUT_GAMEPAD_DPAD_UP)
		{
			if(m_pMainMenu->IsVisible())
				m_pMainMenu->SelectPreviousMenuItem();
			else if (m_kbdEditBox->IsVisible())
				m_kbdEditBox->ProcessAction(XKControl_Keyboard::SHIFT_UP);
			else
				m_pFrmInfo->FocusPreviousControl();

		}

        if (m_DefaultGamepad.wPressedButtons & XINPUT_GAMEPAD_DPAD_DOWN)
		{
			if(m_pMainMenu->IsVisible())
				m_pMainMenu->SelectNextMenuItem();
			else if (m_kbdEditBox->IsVisible())
				m_kbdEditBox->ProcessAction(XKControl_Keyboard::SHIFT_DOWN);
			else
				m_pFrmInfo->FocusNextControl();
		}
		
        if (m_DefaultGamepad.wPressedButtons & XINPUT_GAMEPAD_DPAD_LEFT)
		{
			if(m_pMainMenu->IsVisible())
				m_pMainMenu->SelectPreviousMenuItem();
			else if (m_kbdEditBox->IsVisible())
				m_kbdEditBox->ProcessAction(XKControl_Keyboard::SHIFT_LEFT);
			else
				m_pFrmInfo->FocusPreviousControl();
		}

        if (m_DefaultGamepad.wPressedButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
		{
			if(m_pMainMenu->IsVisible())
				m_pMainMenu->SelectNextMenuItem();
			else if (m_kbdEditBox->IsVisible())
				m_kbdEditBox->ProcessAction(XKControl_Keyboard::SHIFT_RIGHT);
			else
				m_pFrmInfo->FocusNextControl();
		}

	}

}


void ConfigMagicApp::Load_XBOX_EEPROM_Screen()
{
	
	if (m_pMainMenu->IsBusy())
		return;

	if (!WaitForInput)
	{

		m_ActiveForm = m_pFrmStatus;

		m_pFrmInfo->LockControls(TRUE);
		((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("READING XBOX EEPROM");
		((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Please Wait...");
		//Refresh screen....
		Render();

		m_pXKEEPROM->ReadFromXBOX();

		txtTitle->SetText("CURRENT XBOX CONFIGURATION");
		txtTitle->SetTextColor(m_clrCurrentHeadingColor);


		// READ FORCED XBOX VERSION 
		UCHAR tmpINIItem[FILENAME_MAX];
		DWORD tmpLen = 256;
		tmpLen = FILENAME_MAX;
		ZeroMemory(tmpINIItem, tmpLen);
		if (XKGeneral::ReadINIFileItem(MainConfigFileName, "MAIN", "XboxVersion", (LPSTR)tmpINIItem, &tmpLen))
		{
			if (strcmp((LPCSTR)tmpINIItem, "1.0") == 0) 
			{
				m_XBOX_Version = XKEEPROM::XBOX_VERSION::V1_0;
			}
			else if (strcmp((LPCSTR)tmpINIItem, "1.1") == 0)
			{
				m_XBOX_Version = XKEEPROM::XBOX_VERSION::V1_1;
			}
			else if (strcmp((LPCSTR)tmpINIItem, "1.6") == 0)
			{
				m_XBOX_Version = XKEEPROM::XBOX_VERSION::V1_6;
			}
			else
			{
				m_XBOX_Version = XKEEPROM::XBOX_VERSION::V_NONE;
			}


			if (m_XBOX_Version != XKEEPROM::XBOX_VERSION::V_NONE)
			{
				//Set Version Number to Forced Version..
				if (m_XBOX_Version == XKEEPROM::V1_0)
					((LPXKControl_TextBox) m_pFrmInfo->GetControl("lblXBVersion"))->SetText("FORCE V1.0");
				else if (m_XBOX_Version == XKEEPROM::V1_1)
					((LPXKControl_TextBox) m_pFrmInfo->GetControl("lblXBVersion"))->SetText("FORCE V1.1");
                else if (m_XBOX_Version == XKEEPROM::V1_6)
					((LPXKControl_TextBox) m_pFrmInfo->GetControl("lblXBVersion"))->SetText("FORCE V1.6"); 
				((LPXKControl_TextBox) m_pFrmInfo->GetControl("lblXBVersion"))->SetTextColor(m_clrVersionTextColor);


				if (m_pXKEEPROM->Decrypt())
				{

					if (m_XBOX_Version != m_pXKEEPROM->GetXBOXVersion())
					{
						//EEPROM Version and Specified Versions do not match..
						//Just display Warning, then continue and auto correct the issue..
						((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("INCORRECT EEPROM VERSION !");
						((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Press A to Continue");
						WaitForInput = TRUE;
					}
					else
					{
						//EEPROM Version and Specified Versions is a perfect Match, dont even confirm..
						//Continue directly to config screen..
						XConfSections = XBOX_CONFIG_SCREEN;
						WaitForInput = FALSE;
					}

					//XBOX Version found, EEPROM decrypted, now Re-Encrypt with correct Version...
					m_pXKEEPROM->EncryptAndCalculateCRC(m_XBOX_Version);

					m_EnryptedRegionValid = TRUE;
					m_XBOX_EEPROM_Current = TRUE;
				}
				else
				{
					//EEPROM Encrypted Region Invalid..continue and let info screen display error..
					m_EnryptedRegionValid = FALSE;
					m_XBOX_EEPROM_Current = FALSE;
					XConfSections = XBOX_CONFIG_SCREEN;
					WaitForInput = FALSE;
				}
			}
			else
			{

				//Version Specified in INI FILE is corrupt !!!
				((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("ERROR 3 - INI FILE CORRUPT!");
				((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Press X to Exit the Application");

				m_EnryptedRegionValid = FALSE;
				m_XBOX_EEPROM_Current = FALSE;
				m_XBOX_Version = XKEEPROM::XBOX_VERSION::V_NONE;

				XConfSections = EXIT_SCREEN;
				WaitForInput = TRUE;
			}

		}
		//Have to decrypt XBOX eeprom with local RC4 Key if not Forced...
		else if (m_pXKEEPROM->Decrypt((LPBYTE)XboxEEPROMKey))
		{	
			m_EnryptedRegionValid = TRUE;
			m_XBOX_EEPROM_Current = TRUE;
			m_XBOX_Version = m_pXKEEPROM->GetXBOXVersion();

			((LPXKControl_TextBox) m_pFrmInfo->GetControl("lblXBVersion"))->SetTextColor(m_clrVersionTextColor);

			if (m_XBOX_Version == XKEEPROM::V1_0)
				((LPXKControl_TextBox) m_pFrmInfo->GetControl("lblXBVersion"))->SetText("XBOX V1.0");
			else if (m_XBOX_Version == XKEEPROM::V1_1)
				((LPXKControl_TextBox) m_pFrmInfo->GetControl("lblXBVersion"))->SetText("XBOX V1.1");
			else if (m_XBOX_Version == XKEEPROM::V1_6) 
                 ((LPXKControl_TextBox) m_pFrmInfo->GetControl("lblXBVersion"))->SetText("XBOX V1.6"); 

			//Re-Encrypt with correct Version...
			m_pXKEEPROM->EncryptAndCalculateCRC(m_XBOX_Version);
			
			XConfSections = XBOX_CONFIG_SCREEN;
			WaitForInput = FALSE;
		}
		//Could not decrypt with RC4 KEY , now autodetect with installed EEPROM version..
		else if (m_pXKEEPROM->Decrypt())
		{	

			m_EnryptedRegionValid = TRUE;
			m_XBOX_EEPROM_Current = TRUE;
			m_XBOX_Version = m_pXKEEPROM->GetXBOXVersion();
			((LPXKControl_TextBox) m_pFrmInfo->GetControl("lblXBVersion"))->SetTextColor(m_clrVersionTextColor);

			if (m_XBOX_Version == XKEEPROM::V1_0)
				((LPXKControl_TextBox) m_pFrmInfo->GetControl("lblXBVersion"))->SetText("AUTO V1.0");
			else if (m_XBOX_Version == XKEEPROM::V1_1)
				((LPXKControl_TextBox) m_pFrmInfo->GetControl("lblXBVersion"))->SetText("AUTO V1.1");
            else if (m_XBOX_Version == XKEEPROM::V1_6)
				((LPXKControl_TextBox) m_pFrmInfo->GetControl("lblXBVersion"))->SetText("AUTO V1.6");
			//Re-Encrypt with correct Version...
			m_pXKEEPROM->EncryptAndCalculateCRC(m_XBOX_Version);

			XConfSections = XBOX_CONFIG_SCREEN;
			WaitForInput = FALSE;
		}
		else
		{
			((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("ERROR 4 - EEPROM CORRUPT!");
			((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Press X to Exit the Application");

			m_EnryptedRegionValid = FALSE;
			m_XBOX_EEPROM_Current = FALSE;
			m_XBOX_Version = XKEEPROM::XBOX_VERSION::V_NONE;

			XConfSections = EXIT_SCREEN;
			WaitForInput = TRUE;

		}
	}
	else
	{
		//Button/Trigger Handlers....
        if (m_DefaultGamepad.bPressedAnalogButtons[XINPUT_GAMEPAD_A])
		{
			XConfSections = XBOX_CONFIG_SCREEN;
			WaitForInput = FALSE;
		}

	}



	// BLIND RESTORE WILL AUTO PROCEED TO UPDATE EEPROM EVEN
	// IF EEPROM IS INVALID.. .THEN FORCE MODE MUST HAVE BEEN USED...
	if (m_bBlind)
	{
		if(m_XBOX_Version != XKEEPROM::XBOX_VERSION::V_NONE)
		{
		
			XConfSections = LOAD_BIN_EEPROM_SCREEN;
		}
		else
		{
			//STEADY RED Means Could NOT detect XBOX Version...
			XKUtils::SetXBOXLEDStatus(XKUtils::LED_REGISTER_CYCLE0_RED|XKUtils::LED_REGISTER_CYCLE1_RED|XKUtils::LED_REGISTER_CYCLE2_RED|XKUtils::LED_REGISTER_CYCLE3_RED);
			XConfSections = EXIT_SCREEN;
		}
		WaitForInput = FALSE;
	}

}



void ConfigMagicApp::Load_BIN_EEPROM_Screen()
{
	if (m_pMainMenu->IsBusy())
		return;
	
	if (!WaitForInput)
	{
		XKUtils::MountDiskC();
		XKUtils::MountDiskE();
		XKUtils::MountDiskF();

		m_pFrmInfo->LockControls(TRUE);
		if (m_pXKEEPROM->ReadFromBINFile((LPCSTR)m_EEPROM_BIN_FileName, TRUE))
		{
		
			txtTitle->SetText(".BIN FILE CONFIGURATION");
			txtTitle->SetTextColor(m_clrBinFileHeadingColor);

			//Have to decrypt XBOX eeprom with Auto Detect..
			if (m_pXKEEPROM->Decrypt())
			{	
				m_EnryptedRegionValid = TRUE;

				//Re-Encrypt With Correct XBOX Version...
				m_pXKEEPROM->EncryptAndCalculateCRC(m_XBOX_Version);
			}
			else
			{	
				m_EnryptedRegionValid = FALSE;
			}

			m_XBOX_EEPROM_Current = FALSE;

			XConfSections = XBOX_CONFIG_SCREEN;
			WaitForInput = FALSE;
		}
		else
		{
			m_ActiveForm = m_pFrmStatus;
			((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("ERROR READING BIN FILE !!");
			((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Press A Button to continue.");
			WaitForInput = TRUE;
			m_EnryptedRegionValid = FALSE;
		}

		XKUtils::UnMountDiskC();
		XKUtils::UnMountDiskE();
		XKUtils::UnMountDiskF();

	}
	else
	{
		//Button/Trigger Handlers....
        if (m_DefaultGamepad.bPressedAnalogButtons[XINPUT_GAMEPAD_A])
		{
			XConfSections = LOAD_XBOX_EEPROM_SCREEN;
			WaitForInput = FALSE;
		}

	}	


	// BLIND RESTORE WILL AUTO PROCEED TO UPDATE EEPROM..
	if (m_bBlind)
	{
		if(m_EnryptedRegionValid)
		{
		
			XConfSections = WRITE_XBOX_EEPROM_SCREEN;
		}
		else
		{
			//FAST RED Means Problem with EEPROM BIN File..
			if (m_bBlind)
				XKUtils::SetXBOXLEDStatus(XKUtils::LED_REGISTER_CYCLE0_RED|XKUtils::LED_REGISTER_CYCLE1_RED);
			XConfSections = EXIT_SCREEN;
		}

		WaitForInput = FALSE;
	}

}

void ConfigMagicApp::Load_CFG_EEPROM_Screen()
{

	if (m_pMainMenu->IsBusy())
		return;
	
	if (!WaitForInput)
	{

		XKUtils::MountDiskC();
		XKUtils::MountDiskE();
		XKUtils::MountDiskF();

		m_pFrmInfo->LockControls(TRUE);
		txtTitle->SetText(".CFG FILE CONFIGURATION");
		txtTitle->SetTextColor(m_clrCfgFileHeadingColor);
		m_XBOX_EEPROM_Current = FALSE;

		if (m_pXKEEPROM->ReadFromCFGFile((LPSTR)&m_EEPROM_CFG_FileName))
		{
			//Decrypt the Generated EEPROM and Re-Encrypt with correct version..
			m_pXKEEPROM->Decrypt();
			m_pXKEEPROM->EncryptAndCalculateCRC(m_XBOX_Version);
			
			XConfSections = XBOX_CONFIG_SCREEN;
			m_EnryptedRegionValid = TRUE;

			WaitForInput = FALSE;
		}
		else
		{
			m_ActiveForm = m_pFrmStatus;
			((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("ERROR READING CFG FILE !!");
			((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Press A Button to continue.");
			WaitForInput = TRUE;
		}

		XKUtils::UnMountDiskC();
		XKUtils::UnMountDiskE();
		XKUtils::UnMountDiskF();


	}
	else
	{
		//Button/Trigger Handlers....
        if (m_DefaultGamepad.bPressedAnalogButtons[XINPUT_GAMEPAD_A])
		{
			XConfSections = XBOX_CONFIG_SCREEN;
			WaitForInput = FALSE;
		}

	}	

}


void ConfigMagicApp::Edit_EEPROM_Screen()
{
	if (m_pMainMenu->IsBusy())
		return;
	
	if (!WaitForInput)
	{

		txtTitle->SetText("NEW ON-THE-FLY CONFIGURATION");
		txtTitle->SetTextColor(m_clrOnTheFlyHeadingColor);
		m_XBOX_EEPROM_Current = FALSE;
		m_pFrmInfo->LockControls(FALSE);

		XConfSections = XBOX_CONFIG_SCREEN;
		WaitForInput = FALSE;
	}
}

void ConfigMagicApp::Write_XBOX_EEPROM_Screen()
{
	if (m_pMainMenu->IsBusy())
		return;

	if (!WaitForInput)
	{

		//STEADY ORANGE means BUSY..
		if (m_bBlind)
			XKUtils::SetXBOXLEDStatus(XKUtils::LED_REGISTER_CYCLE0_GREEN|XKUtils::LED_REGISTER_CYCLE0_RED|XKUtils::LED_REGISTER_CYCLE1_GREEN|XKUtils::LED_REGISTER_CYCLE1_RED|XKUtils::LED_REGISTER_CYCLE2_GREEN|XKUtils::LED_REGISTER_CYCLE2_RED|XKUtils::LED_REGISTER_CYCLE3_GREEN|XKUtils::LED_REGISTER_CYCLE3_RED);

		m_ActiveForm = m_pFrmStatus;
		((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("CREATING EEPROM BACKUP..");
		((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Please Wait...");
		//Refresh screen....
		Render();

		CreateEEPROMBackup("EEPROMBackupBeforeUpdate");

		((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("UPDATING XBOX EEPROM");
		//Refresh screen....
		Render();

		//ATA Command Object to check if Disk is locked..
		XKHDD::ATA_COMMAND_OBJ hddcommand;


		//Decrypting EEPROM, if it fails.. Display fail message!
		if (m_pXKEEPROM->Decrypt())
		{

			//Re-Encrypt with correct Version...
			m_EnryptedRegionValid = TRUE;
			m_pXKEEPROM->EncryptAndCalculateCRC(m_XBOX_Version);

		
			((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Querying HDD..");
			Render();
			//Get IDE_ATA_IDENTIFY Data for HDD ..
			ZeroMemory(&hddcommand, sizeof(XKHDD::ATA_COMMAND_OBJ));
			hddcommand.DATA_BUFFSIZE = 0;
			hddcommand.IPReg.bDriveHeadReg = IDE_DEVICE_MASTER;
			hddcommand.IPReg.bCommandReg = IDE_ATA_IDENTIFY;
			XKHDD::SendATACommand(IDE_PRIMARY_PORT, &hddcommand, IDE_COMMAND_READ);

			//Get ATA Locked State
			DWORD SecStatus = XKHDD::GetIDESecurityStatus(hddcommand.DATA_BUFFER);
			//Check if Disk is locked..
			if (!(SecStatus & IDE_SECURITY_ENABLED))
			{
				((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Writing EEPROM Data to XBOX..");
				Render();
				if (m_pXKEEPROM->IsEncrypted())
				{
				
					m_pXKEEPROM->WriteToXBOX();
					((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("XBOX EEPROM SUCCESSFULLY UPDATED");
					//STEADY GREEN Means Success...
					if (m_bBlind)
						XKUtils::SetXBOXLEDStatus(XKUtils::LED_REGISTER_CYCLE0_GREEN|XKUtils::LED_REGISTER_CYCLE1_GREEN|XKUtils::LED_REGISTER_CYCLE2_GREEN);

				}
				else
				{
					//FAST RED Means Problem with EEPROM Encrypted Region..
					if (m_bBlind)
						XKUtils::SetXBOXLEDStatus(XKUtils::LED_REGISTER_CYCLE0_RED|XKUtils::LED_REGISTER_CYCLE1_RED);
					
					((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("ERROR RE-ENCRYPTING EEPROM");
				}
			}
			else
			{
				((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("UNLOCK HDD BEFORE UPDATING EEPROM");

				//Flashing RED and ORANGE means HDD is LOCKED..
				if (m_bBlind)
					XKUtils::SetXBOXLEDStatus(XKUtils::LED_REGISTER_CYCLE0_GREEN|XKUtils::LED_REGISTER_CYCLE0_RED|XKUtils::LED_REGISTER_CYCLE1_GREEN|XKUtils::LED_REGISTER_CYCLE1_RED|XKUtils::LED_REGISTER_CYCLE2_RED|XKUtils::LED_REGISTER_CYCLE3_RED);

			}
		}
		else
		{
			//Encrypted Region is Invalid.. DONT Allow EEPROM Update..
			m_EnryptedRegionValid = FALSE;

			m_ActiveForm = m_pFrmStatus;
			((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("EEPROM ENCRYPTED REGION INVALID !!");

			//FAST RED Means Problem with EEPROM Encrypted Region..
			if (m_bBlind)
				XKUtils::SetXBOXLEDStatus(XKUtils::LED_REGISTER_CYCLE0_RED|XKUtils::LED_REGISTER_CYCLE1_RED);

		}

		((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Press A Button to continue.");
		WaitForInput = TRUE;
	}
	else
	{
		//Button/Trigger Handlers....
        if (m_DefaultGamepad.bPressedAnalogButtons[XINPUT_GAMEPAD_A])
		{
			XConfSections = XBOX_CONFIG_SCREEN;
			WaitForInput = FALSE;
		}

	}	

	// BLIND RESTORE WILL AUTO PROCEED TO EXIT SCREEN..
	if (m_bBlind)
	{
			XConfSections = EXIT_SCREEN;
			WaitForInput = FALSE;
	}
}

void ConfigMagicApp::Create_Backup_Files_Screen()
{
	if (m_pMainMenu->IsBusy())
		return;

	if (!WaitForInput)
	{
		m_ActiveForm = m_pFrmStatus;
		((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("CREATING BACKUP FILES..");
		((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Please Wait...");
		//Refresh screen....
		Render();

		CreateEEPROMBackup("EEPROMBackup");

		XConfSections = XBOX_CONFIG_SCREEN;
		WaitForInput = FALSE;
	}

}

void ConfigMagicApp::Unlock_HDD_Screen()
{
	if (m_pMainMenu->IsBusy())
		return;

	if (!WaitForInput)
	{
		m_ActiveForm = m_pFrmStatus;
		((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("UNLOCKING XBOX HDD");
		((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Please Wait...");
		//Refresh screen....
		Render();


		//ATA Command Structure..
		XKHDD::ATA_COMMAND_OBJ hddcommand;
		UCHAR HddPass[32];

		//Dont Lock if the EEPROM data was NOT read from XBOX
		if(m_XBOX_EEPROM_Current)
		{
			//Decrypting EEPROM, if it fails.. Display fail message!
			if (m_pXKEEPROM->Decrypt())
			{

				((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Querying HDD..");
				Render();
				//Get IDE_ATA_IDENTIFY Data for HDD ..
				ZeroMemory(&hddcommand, sizeof(XKHDD::ATA_COMMAND_OBJ));
				hddcommand.DATA_BUFFSIZE = 0;
				hddcommand.IPReg.bDriveHeadReg = IDE_DEVICE_MASTER;
				hddcommand.IPReg.bCommandReg = IDE_ATA_IDENTIFY;
				XKHDD::SendATACommand(IDE_PRIMARY_PORT, &hddcommand, IDE_COMMAND_READ);

				XKEEPROM::EEPROMDATA tmpData;
				m_pXKEEPROM->GetEEPROMData(&tmpData);
				XKHDD::GenerateHDDPwd(tmpData.HDDKkey, hddcommand.DATA_BUFFER, HddPass);
				
				//Get ATA Locked State
				DWORD SecStatus = XKHDD::GetIDESecurityStatus(hddcommand.DATA_BUFFER);
				//Check if Disk is Reall locked..
				if ((SecStatus & IDE_SECURITY_SUPPORTED) && (SecStatus & IDE_SECURITY_ENABLED))
				{
					

					((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Unlocking HDD..");
					Render();
					//Execute HDD Unlock..
					ZeroMemory(&hddcommand, sizeof(XKHDD::ATA_COMMAND_OBJ));
					hddcommand.DATA_BUFFSIZE = 512;
					hddcommand.IPReg.bDriveHeadReg = IDE_DEVICE_MASTER;
					hddcommand.IPReg.bCommandReg = IDE_ATA_SECURITY_UNLOCK;
					LPBYTE HDDP = (LPBYTE)&hddcommand.DATA_BUFFER;
					memcpy(HDDP+2, HddPass, 20);
					XKHDD::SendATACommand(IDE_PRIMARY_PORT, &hddcommand, IDE_COMMAND_WRITE);

					((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Querying HDD..");
					Render();
					//Query HDD To see if we succeeded..
					ZeroMemory(&hddcommand, sizeof(XKHDD::ATA_COMMAND_OBJ));
					hddcommand.DATA_BUFFSIZE = 512;
					hddcommand.IPReg.bDriveHeadReg = IDE_DEVICE_MASTER;
					hddcommand.IPReg.bCommandReg = IDE_ATA_IDENTIFY;
					XKHDD::SendATACommand(IDE_PRIMARY_PORT, &hddcommand, IDE_COMMAND_READ);

					((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Disable HDD Password..");
					Render();
					//Execute HDD Lock Disable
					ZeroMemory(&hddcommand, sizeof(XKHDD::ATA_COMMAND_OBJ));
					hddcommand.DATA_BUFFSIZE = 512;
					hddcommand.IPReg.bDriveHeadReg = IDE_DEVICE_MASTER;
					hddcommand.IPReg.bCommandReg = IDE_ATA_SECURITY_DISABLE;
					memcpy(HDDP+2, HddPass, 20);
					XKHDD::SendATACommand(IDE_PRIMARY_PORT, &hddcommand, IDE_COMMAND_WRITE);

					((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Querying HDD..");
					Render();

					//We Found That sometimes that by the first query it hasnt unlocked yet.. so we query three times if needed..
					int retrycnt = 0;
					do
					{
					
						//Query HDD To see if we succeeded..
						ZeroMemory(&hddcommand, sizeof(XKHDD::ATA_COMMAND_OBJ));
						hddcommand.DATA_BUFFSIZE = 512;
						hddcommand.IPReg.bDriveHeadReg = IDE_DEVICE_MASTER;
						hddcommand.IPReg.bCommandReg = IDE_ATA_IDENTIFY;
						XKHDD::SendATACommand(IDE_PRIMARY_PORT, &hddcommand, IDE_COMMAND_READ);
						SecStatus = XKHDD::GetIDESecurityStatus(hddcommand.DATA_BUFFER);

						retrycnt++;

					} while (!((SecStatus & IDE_SECURITY_SUPPORTED) && !(SecStatus & IDE_SECURITY_ENABLED)) && (retrycnt < 3));

									

					if ((SecStatus & IDE_SECURITY_SUPPORTED) && !(SecStatus & IDE_SECURITY_ENABLED))
					{
						m_ActiveForm = m_pFrmStatus;
						((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("MASTER PASS: TEAMASSEMBLY");
						((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("XBOX HDD NOW UNLOCKED ! Press A Button to continue.");
					}
					else
					{
						m_ActiveForm = m_pFrmStatus;
						((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("FAILED TO UNLOCK XBOX HDD !!");
						((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Press A Button to continue.");
					}

					
				}
				else
				{
					m_ActiveForm = m_pFrmStatus;
					((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("XBOX HDD IS NOT LOCKED ??");
					((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Press A Button to continue.");

				}

				//Re Encrypt EEPROM with Current XBOX Version
				m_pXKEEPROM->EncryptAndCalculateCRC(m_XBOX_Version);

			}
			else
			{
				//Encrypted Region is Invalid.. DONT Allow Locking/Unlocking..
				m_EnryptedRegionValid = FALSE;

				m_ActiveForm = m_pFrmStatus;
				((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("EEPROM ENCRYPTED REGION INVALID");
				((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Press A Button to continue.");

			}
		}
		else
		{
			m_ActiveForm = m_pFrmStatus;
			((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("LOAD XBOX EEPROM TO UNLOCK !!");
			((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Press A Button to continue.");

		}
				
		WaitForInput = TRUE;
	}
	else
	{
		//Button/Trigger Handlers....
        if (m_DefaultGamepad.bPressedAnalogButtons[XINPUT_GAMEPAD_A])
		{
			LoadMainMenuItems();
			XConfSections = XBOX_CONFIG_SCREEN;
			WaitForInput = FALSE;
		}

	}
}

void ConfigMagicApp::Lock_HDD_Screen()
{
	if (m_pMainMenu->IsBusy())
		return;
	
	if (!WaitForInput)
	{
		m_ActiveForm = m_pFrmStatus;
		((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("LOCKING XBOX HDD");
		((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Creating EEPROM Backup..");
		//Refresh screen....
		Render();
		CreateEEPROMBackup("EEPROMBackupBeforeLock");


		//ATA Command Structure..
		XKHDD::ATA_COMMAND_OBJ hddcommand;
		UCHAR HddPass[32];
		UCHAR MasterPassword[13] = "TEAMASSEMBLY";
		
		//Dont Lock if the EEPROM data was NOT read from XBOX
		if(m_XBOX_EEPROM_Current)
		{
			//Decrypting EEPROM, if it fails.. Display fail message!
			if (m_pXKEEPROM->Decrypt())
			{

				((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Querying HDD..");
				Render();
				//Get IDE_ATA_IDENTIFY Data for HDD ..
				ZeroMemory(&hddcommand, sizeof(XKHDD::ATA_COMMAND_OBJ));
				hddcommand.DATA_BUFFSIZE = 0;
				hddcommand.IPReg.bDriveHeadReg = IDE_DEVICE_MASTER;
				hddcommand.IPReg.bCommandReg = IDE_ATA_IDENTIFY;
				XKHDD::SendATACommand(IDE_PRIMARY_PORT, &hddcommand, IDE_COMMAND_READ);

				XKEEPROM::EEPROMDATA tmpData;
				m_pXKEEPROM->GetEEPROMData(&tmpData);
				XKHDD::GenerateHDDPwd(tmpData.HDDKkey, hddcommand.DATA_BUFFER, HddPass);

				//Get ATA Locked State
				DWORD SecStatus = XKHDD::GetIDESecurityStatus(hddcommand.DATA_BUFFER);
				//Check if Disk is already locked..
				if ((SecStatus & IDE_SECURITY_SUPPORTED) && !(SecStatus & IDE_SECURITY_ENABLED))
				{

					((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Setting Master Pasword: TEAMASSEMBLY");
					Render();
					//Execute HDD Unlock..
					ZeroMemory(&hddcommand, sizeof(XKHDD::ATA_COMMAND_OBJ));
					hddcommand.DATA_BUFFSIZE = 512;
					hddcommand.IPReg.bDriveHeadReg = IDE_DEVICE_MASTER;
					hddcommand.IPReg.bCommandReg = IDE_ATA_SECURITY_SETPASSWORD;
					LPBYTE HDDP = (LPBYTE)&hddcommand.DATA_BUFFER;
					LPDWORD pMastr = (LPDWORD) HDDP;
					*pMastr = 0x0001; //Set Master Pwd..
					memcpy(HDDP+2, MasterPassword, 13);
					XKHDD::SendATACommand(IDE_PRIMARY_PORT, &hddcommand, IDE_COMMAND_WRITE);


					((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Querying HDD..");
					Render();
					//Query HDD To see if we succeeded..
					ZeroMemory(&hddcommand, sizeof(XKHDD::ATA_COMMAND_OBJ));
					hddcommand.DATA_BUFFSIZE = 512;
					hddcommand.IPReg.bDriveHeadReg = IDE_DEVICE_MASTER;
					hddcommand.IPReg.bCommandReg = IDE_ATA_IDENTIFY;
					XKHDD::SendATACommand(IDE_PRIMARY_PORT, &hddcommand, IDE_COMMAND_READ);
					SecStatus = XKHDD::GetIDESecurityStatus(hddcommand.DATA_BUFFER);
					

					((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Locking HDD..");
					Render();
					//Execute HDD Unlock..
					ZeroMemory(&hddcommand, sizeof(XKHDD::ATA_COMMAND_OBJ));
					hddcommand.DATA_BUFFSIZE = 512;
					hddcommand.IPReg.bDriveHeadReg = IDE_DEVICE_MASTER;
					hddcommand.IPReg.bCommandReg = IDE_ATA_SECURITY_SETPASSWORD;
					memcpy(HDDP+2, HddPass, 20);
					XKHDD::SendATACommand(IDE_PRIMARY_PORT, &hddcommand, IDE_COMMAND_WRITE);


					//We Found That sometimes that by the first query it hasnt locked yet.. so we query twice if needed..
					int retrycnt = 0;
					do
					{
					
						((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Querying HDD..");
						Render();
						//Query HDD To see if we succeeded..
						ZeroMemory(&hddcommand, sizeof(XKHDD::ATA_COMMAND_OBJ));
						hddcommand.DATA_BUFFSIZE = 512;
						hddcommand.IPReg.bDriveHeadReg = IDE_DEVICE_MASTER;
						hddcommand.IPReg.bCommandReg = IDE_ATA_IDENTIFY;
						XKHDD::SendATACommand(IDE_PRIMARY_PORT, &hddcommand, IDE_COMMAND_READ);
						SecStatus = XKHDD::GetIDESecurityStatus(hddcommand.DATA_BUFFER);

						retrycnt++;

					} while (!((SecStatus & IDE_SECURITY_SUPPORTED) && (SecStatus & IDE_SECURITY_ENABLED)) && (retrycnt < 3));

									

					if ((SecStatus & IDE_SECURITY_SUPPORTED) && (SecStatus & IDE_SECURITY_ENABLED))
					{
						m_ActiveForm = m_pFrmStatus;
						((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("MASTER PASS : TEAMASSEMBLY ");
						((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("LOCKED HDD! Press A Button to continue.");
					}
					else
					{
						m_ActiveForm = m_pFrmStatus;
						((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("FAILED TO LOCK XBOX HDD !!");
						((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Press A Button to continue.");
					}

					
				}
				else
				{
					m_ActiveForm = m_pFrmStatus;
					if (!(SecStatus & IDE_SECURITY_SUPPORTED))
						((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("HDD DOES NOT SUPPORT LOCKING !");
					else
						((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("XBOX HDD IS ALREADY LOCKED ??");

					((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Press A Button to continue.");

				}

				// Re-Encrypt EEPROM with current XBOX Version..
				m_pXKEEPROM->EncryptAndCalculateCRC(m_XBOX_Version);

			}
			else
			{
				//Encrypted Region is Invalid.. DONT Allow Locking/Unlocking..
				m_EnryptedRegionValid = FALSE;

				m_ActiveForm = m_pFrmStatus;
				((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("EEPROM ENCRYPTED REGION INVALID");
				((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Press A Button to continue.");

			}
		}
		else
		{
			m_ActiveForm = m_pFrmStatus;
			((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("LOAD XBOX EEPROM TO LOCK !!");
			((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Press A Button to continue.");
		}
			
		WaitForInput = TRUE;
	}
	else
	{
		//Button/Trigger Handlers....
        if (m_DefaultGamepad.bPressedAnalogButtons[XINPUT_GAMEPAD_A])
		{
			LoadMainMenuItems();
			XConfSections = XBOX_CONFIG_SCREEN;
			WaitForInput = FALSE;
		}

	}
}

void ConfigMagicApp::ExitScreen()
{
	if (m_pMainMenu->IsBusy())
		return;

	if (!WaitForInput)
	{
		Render();
		m_ActiveForm = m_pFrmStatus;
		m_ActiveForm->LoadTransparentImage("D:\\Media\\credits.png", D3DCOLOR_ARGB(0xFF, 0xFF, 0x00, 0xFF));

		((LPXKControl_TextBox) m_pFrmStatus->GetControl("txtStatusMsg"))->SetText("");
		((LPXKControl_TextBox) m_ActiveForm->GetControl("txtStatus"))->SetText("Press 'X' to Load Dashboard");


		WaitForInput = TRUE;

		if (!m_bBlind)
		{
			std::ifstream creditswmv("d:\\media\\credits.wmv");
			if (creditswmv.good())
			{
				creditswmv.close();
				PlayVideo("d:\\media\\credits.wmv");
			}
		}
	}
	else
	{
      if (m_DefaultGamepad.bPressedAnalogButtons[XINPUT_GAMEPAD_X])
		{
			XKUtils::SetXBOXLEDStatus(XKUtils::LED_REGISTER_CYCLE0_GREEN|XKUtils::LED_REGISTER_CYCLE1_GREEN|XKUtils::LED_REGISTER_CYCLE2_GREEN|XKUtils::LED_REGISTER_CYCLE3_GREEN);
			XKUtils::XBOXRebootToDash();
		}
	
	}

}

HRESULT ConfigMagicApp::Initialize()
{

	HRESULT retVal = S_OK;

	XKUtils::SetXBOXLEDStatus(XKUtils::LED_REGISTER_CYCLE0_GREEN|XKUtils::LED_REGISTER_CYCLE1_GREEN|XKUtils::LED_REGISTER_CYCLE2_GREEN|XKUtils::LED_REGISTER_CYCLE3_GREEN);
	m_bBlind = FALSE;

	m_pXKEEPROM = new XKEEPROM();

	XConfSections = STARTUP_SCREEN;
	XAppTitle = "";

	m_EnryptedRegionValid = FALSE;
	m_XBOX_EEPROM_Current = FALSE;
	
	retVal = m_XBFont1.Create(m_pd3dDevice, "D:\\Media\\Font_Info.xpr"); // 13
	retVal = m_XBFont2.Create(m_pd3dDevice, "D:\\Media\\Font_Status.xpr"); // 17
	retVal = m_XBFont3.Create(m_pd3dDevice, "D:\\Media\\Font_Message.xpr"); // 22
	retVal = m_XBFont4.Create(m_pd3dDevice, "D:\\Media\\Font_Menu.xpr"); // 24
	
    // Create DirectSound
	DirectSoundCreate( NULL, &m_pDSound, NULL );

    // Initialize the Video Player
    m_XKVideoPlayer.Initialize( m_pd3dDevice, m_pDSound );

	//Load Settings from Config File.. 
	LoadSettingsFromINI();

	m_KeyboardWidth = 560;
	m_KeyboardHeight = 290;

	m_MenuXOffset = 0;
	m_MenuYOffset = 24;

	ScreenWidth = 640 - XImgCorrection;
	ScreenHeight = 480 - YImgCorrection;

	D3DXCreateSprite(m_pd3dDevice, &m_bgSprite);

	WaitForInput = FALSE;


	//Use the Console to write error data to screen....
	pxkconsole = new XKConsole(m_pd3dDevice, ScreenWidth, ScreenHeight-20, XOffset+20, YOffset+10);
	pxkconsole->SetTitleBarVisible(FALSE);
	pxkconsole->SetTitle(XAppTitle);

	m_pMainMenu = new XKControl_Menu("Main Menu", m_bgSprite, m_MenuXOffset+XOffset, m_MenuYOffset+YOffset, 300, 380);
	m_pMainMenu->LoadTransparentImage("D:\\Media\\Menu.png", D3DCOLOR_ARGB(0xFF, 0xFF, 0x00, 0xFF));
	m_pMainMenu->SetTitleOffset(0, 5);
	m_pMainMenu->SetTitleTextFont(&m_XBFont3);
	m_pMainMenu->SetTitleTextColor(m_clrMenuHeadingColor);
	m_pMainMenu->SetItemsOffset(0, 65); //XOffset is always from Right to exclude border..
	m_pMainMenu->SetItemTextFont(&m_XBFont4);
	m_pMainMenu->SetItemsTextColor(m_clrMenuItemTextColor);
	m_pMainMenu->SetItemsSelectedColor(m_clrMenuSelectTextColor);
	
	m_kbdEditBox = new XKControl_Keyboard("kbdEditBox", m_bgSprite, &m_XBFont3, XOffset + ((ScreenWidth/2) - (m_KeyboardWidth/2)), YOffset + ((ScreenHeight/2) - (m_KeyboardHeight/2)), m_KeyboardWidth, m_KeyboardHeight);
	m_kbdEditBox->LoadTransparentImage("D:\\Media\\keyboard1.png", D3DCOLOR_ARGB(0xFF, 0xFF, 0x00, 0xFF));
	m_kbdEditBox->SetStandardKeyboardLayout(XKControl_Keyboard::TYPE_FULL);
	m_kbdEditBox->SetVisible(FALSE);
	m_kbdEditBox->SetEditTextFont(&m_XBFont2);
	m_kbdEditBox->SetMaxCharacters(32);

	LoadMainMenuItems();
	
	InitWarningScreen();

	InitConfigScreen();

	InitStatusScreen();


	return retVal;;
}

void ConfigMagicApp::LoadMainMenuItems()
{	
	m_pMainMenu->AddMenuItem("A_LOADX", "Load XBOX EEPROM");
	m_pMainMenu->AddMenuItem("B_LOADB", "Load EEPROM.BIN File");
	m_pMainMenu->AddMenuItem("C_LOADC", "Build EEPROM.CFG File");
	m_pMainMenu->AddMenuItem("D_EDIT", "Edit EEPROM");
	m_pMainMenu->AddMenuItem("E_UNLOCKHDD", "Unlock HDD");
	m_pMainMenu->AddMenuItem("F_LOCKHDD", "Lock HDD");
	m_pMainMenu->AddMenuItem("G_BACKUP", "Backup EEPROM Files");
	m_pMainMenu->AddMenuItem("H_WRITEX", "Update XBOX EEPROM");
	m_pMainMenu->AddMenuItem("I_SPACE", " ");
	//ATA Command Object to check if Disk is locked..
	XKHDD::ATA_COMMAND_OBJ hddcommand;
	//Get IDE_ATA_IDENTIFY Data for HDD ..
	ZeroMemory(&hddcommand, sizeof(XKHDD::ATA_COMMAND_OBJ));
	hddcommand.DATA_BUFFSIZE = 0;
	hddcommand.IPReg.bDriveHeadReg = IDE_DEVICE_MASTER;
	hddcommand.IPReg.bCommandReg = IDE_ATA_IDENTIFY;
	XKHDD::SendATACommand(IDE_PRIMARY_PORT, &hddcommand, IDE_COMMAND_READ);
	//Get ATA Locked State
	DWORD SecStatus = XKHDD::GetIDESecurityStatus(hddcommand.DATA_BUFFER);
	//Check if Disk is locked..
	if ((SecStatus & IDE_SECURITY_ENABLED))
	{
		m_pMainMenu->AddMenuItem("J_EXIT", "EXIT ConfigMagic");
	}
	else
	{
		m_pMainMenu->AddMenuItem("J_EXIT", "Lock Your Hard Drive");
	}
	m_pMainMenu->SetSelectedMenuItem("A_LOADX");
}

void ConfigMagicApp::InitWarningScreen()
{
	UCHAR alphaChannel = 0x90;
	LPSTR sTextBoxFile = "D:\\Media\\textbox.png";

	txtStatus = new XKControl_TextBox("txtStatus", m_bgSprite);
	txtStatus->SetBitmapFont(&m_XBFont2);
	txtStatus->SetTextColor(m_clrStatusTextColor);
	txtStatus->SetTextCentered(TRUE);
	txtStatus->SetTextShadow(FALSE);

	m_pFrmWarning = new XKControl_Panel("Warning", m_bgSprite, XOffset, YOffset, ScreenWidth,ScreenHeight);
	m_pFrmWarning->LoadImage("D:\\Media\\Warning.png");

	m_pFrmWarning->AddControl(txtStatus,  0, 405,  ScreenWidth, 30);

}

void ConfigMagicApp::InitStatusScreen()
{
	m_pFrmStatus = new XKControl_Panel("Status", m_bgSprite, XOffset, YOffset, ScreenWidth,ScreenHeight);
	m_pFrmStatus->LoadImage("D:\\Media\\Status.png");
	
	txtStatusMsg = new XKControl_TextBox("txtStatusMsg", m_bgSprite);
	txtStatusMsg->SetBitmapFont(&m_XBFont3);
	txtStatusMsg->SetTextColor(m_clrMessageTextColor);
	txtStatusMsg->SetTextCentered(TRUE);
	txtStatusMsg->SetTextShadow(FALSE);

	m_pFrmStatus->AddControl(txtStatusMsg,  0, 180,  ScreenWidth, 30);
	m_pFrmStatus->AddControl(txtStatus,		0, 405,  ScreenWidth, 30);

}

void ConfigMagicApp::InitConfigScreen()
{
	UCHAR alphaChannel = 0xFF;
	LPSTR sTextBoxFile = "D:\\Media\\textbox.png";
	INT itabindex = 0;

	m_pFrmInfo = new XKControl_Panel("FrmInfo", m_bgSprite, XOffset, YOffset, ScreenWidth,ScreenHeight);
	m_pFrmInfo->LoadImage("D:\\Media\\Background.png");


	txtTitle = new XKControl_TextBox("txtTitle", m_bgSprite);
	txtTitle->SetBitmapFont(&m_XBFont3);
	txtTitle->SetTextColor(m_clrCurrentHeadingColor);
	txtTitle->SetTextCentered(TRUE);
	txtTitle->SetTextShadow(FALSE);
	txtTitle->SetText("CURRENT XBOX CONFIGURATION");

	txtSerial = new XKControl_TextBox("txtSerial", m_bgSprite);
	txtSerial->SetBitmapFont(&m_XBFont1);
	txtSerial->LoadTransparentImage(sTextBoxFile, D3DCOLOR_ARGB(0xFF, 0x00, 0x00, 0x00));
	txtSerial->SetTextColor(m_clrValuesTextColor);
	txtSerial->SetAlphaChannel(alphaChannel);
	txtSerial->SetTabIndex(itabindex++);
	txtSerial->SetTabStop(TRUE);
	
	txtMAC = new XKControl_TextBox("txtMAC", m_bgSprite);
	txtMAC->SetBitmapFont(&m_XBFont1);
	txtMAC->LoadTransparentImage(sTextBoxFile, D3DCOLOR_ARGB(0xFF, 0x00, 0x00, 0x00));
	txtMAC->SetTextColor(m_clrValuesTextColor);
	txtMAC->SetAlphaChannel(alphaChannel);
	txtMAC->SetTabIndex(itabindex++);
	txtMAC->SetTabStop(TRUE);


	txtVideoFormat = new XKControl_TextBox("txtVideoFormat", m_bgSprite);
	txtVideoFormat->SetBitmapFont(&m_XBFont1);
	txtVideoFormat->LoadTransparentImage(sTextBoxFile, D3DCOLOR_ARGB(0xFF, 0x00, 0x00, 0x00));
	txtVideoFormat->SetTextColor(m_clrValuesTextColor);
	txtVideoFormat->SetAlphaChannel(alphaChannel);
	txtVideoFormat->SetTabIndex(itabindex++);
	txtVideoFormat->SetTabStop(TRUE);

	txtOnlineKey = new XKControl_TextBox("txtOnlineKey", m_bgSprite);
	txtOnlineKey->SetBitmapFont(&m_XBFont1);
	txtOnlineKey->LoadTransparentImage(sTextBoxFile, D3DCOLOR_ARGB(0xFF, 0x00, 0x00, 0x00));
	txtOnlineKey->SetTextColor(m_clrValuesTextColor);
	txtOnlineKey->SetAlphaChannel(alphaChannel);
	txtOnlineKey->SetTabIndex(itabindex++);
	txtOnlineKey->SetTabStop(TRUE);
	
	txtDVDZone = new XKControl_TextBox("txtDVDZone", m_bgSprite);
	txtDVDZone->SetBitmapFont(&m_XBFont1);
	txtDVDZone->LoadTransparentImage(sTextBoxFile, D3DCOLOR_ARGB(0xFF, 0x00, 0x00, 0x00));
	txtDVDZone->SetTextColor(m_clrValuesTextColor);
	txtDVDZone->SetAlphaChannel(alphaChannel);
	txtDVDZone->SetTabIndex(itabindex++);
	txtDVDZone->SetTabStop(TRUE);
	
	txtDVDModel = new XKControl_TextBox("txtDVDModel", m_bgSprite);
	txtDVDModel->SetBitmapFont(&m_XBFont1);
	txtDVDModel->LoadTransparentImage(sTextBoxFile, D3DCOLOR_ARGB(0xFF, 0x00, 0x00, 0x00));
	txtDVDModel->SetTextColor(m_clrValuesTextColor);
	txtDVDModel->SetAlphaChannel(alphaChannel);

	txtHDDKey = new XKControl_TextBox("txtHDDKey", m_bgSprite);
	txtHDDKey->SetBitmapFont(&m_XBFont1);
	txtHDDKey->LoadTransparentImage(sTextBoxFile, D3DCOLOR_ARGB(0xFF, 0x00, 0x00, 0x00));
	txtHDDKey->SetTextColor(m_clrValuesTextColor);
	txtHDDKey->SetAlphaChannel(alphaChannel);
	txtHDDKey->SetTabIndex(itabindex++);
	txtHDDKey->SetTabStop(TRUE);

	txtHDDModel = new XKControl_TextBox("txtHDDModel", m_bgSprite);
	txtHDDModel->SetBitmapFont(&m_XBFont1);
	txtHDDModel->LoadTransparentImage(sTextBoxFile, D3DCOLOR_ARGB(0xFF, 0x00, 0x00, 0x00));
	txtHDDModel->SetTextColor(m_clrValuesTextColor);
	txtHDDModel->SetAlphaChannel(alphaChannel);

	txtHDDSerial = new XKControl_TextBox("txtHDDSerial", m_bgSprite);
	txtHDDSerial->SetBitmapFont(&m_XBFont1);
	txtHDDSerial->LoadTransparentImage(sTextBoxFile, D3DCOLOR_ARGB(0xFF, 0x00, 0x00, 0x00));
	txtHDDSerial->SetTextColor(m_clrValuesTextColor);
	txtHDDSerial->SetAlphaChannel(alphaChannel);

	txtHDDStatus = new XKControl_TextBox("txtHDDStatus", m_bgSprite);
	txtHDDStatus->SetBitmapFont(&m_XBFont1);
	txtHDDStatus->SetTextColor(m_clrHDDStatusTextColor);
	txtHDDStatus->SetTextCentered(TRUE);
	txtHDDStatus->SetTextShadow(FALSE);


	txtHDDPwd = new XKControl_TextBox("txtHDDPwd", m_bgSprite);
	txtHDDPwd->SetBitmapFont(&m_XBFont1);
	txtHDDPwd->LoadTransparentImage(sTextBoxFile, D3DCOLOR_ARGB(0xFF, 0xff, 0x00, 0xff));
	txtHDDPwd->SetTextColor(m_clrValuesTextColor);
	txtHDDPwd->SetAlphaChannel(alphaChannel);
	txtHDDPwd->SetTextCentered(TRUE);

	lblSerial = new XKControl_TextBox("lblSerial", m_bgSprite);
	lblSerial->SetBitmapFont(&m_XBFont1);
	lblSerial->SetTextColor(m_clrLabelsTextColor);
	lblSerial->SetText("SERIAL NUMBER");
	lblSerial->SetTextShadow(FALSE);
	
	lblMAC = new XKControl_TextBox("lblMAC", m_bgSprite);
	lblMAC->SetBitmapFont(&m_XBFont1);
	lblMAC->SetTextColor(m_clrLabelsTextColor);
	lblMAC->SetText("MAC ADDRESS");
	lblMAC->SetTextShadow(FALSE);

	lblVideoFormat = new XKControl_TextBox("lblVideoFormat", m_bgSprite);
	lblVideoFormat->SetBitmapFont(&m_XBFont1);
	lblVideoFormat->SetTextColor(m_clrLabelsTextColor);
	lblVideoFormat->SetText("VIDEO & XBE REGION");
	lblVideoFormat->SetTextShadow(FALSE);

	lblOnlineKey = new XKControl_TextBox("lblOnlineKey", m_bgSprite);
	lblOnlineKey->SetBitmapFont(&m_XBFont1);
	lblOnlineKey->SetTextColor(m_clrLabelsTextColor);
	lblOnlineKey->SetText("ONLINE KEY");
	lblOnlineKey->SetTextShadow(FALSE);

	lblDVDZone = new XKControl_TextBox("lblDVDZone", m_bgSprite);
	lblDVDZone->SetBitmapFont(&m_XBFont1);
	lblDVDZone->SetTextColor(m_clrLabelsTextColor);
	lblDVDZone->SetText("DVD KIT ZONE");
	lblDVDZone->SetTextShadow(FALSE);

	lblDVDModel = new XKControl_TextBox("lblDVDModel", m_bgSprite);
	lblDVDModel->SetBitmapFont(&m_XBFont1);
	lblDVDModel->SetTextColor(m_clrLabelsTextColor);
	lblDVDModel->SetText("DVD DRIVE MODEL");
	lblDVDModel->SetTextShadow(FALSE);

	lblHDDKey = new XKControl_TextBox("lblHDDKey", m_bgSprite);
	lblHDDKey->SetBitmapFont(&m_XBFont1);
	lblHDDKey->SetTextColor(m_clrLabelsTextColor);
	lblHDDKey->SetText("UNIQUE HDD KEY");
	lblHDDKey->SetTextShadow(FALSE);

	lblHDDModel = new XKControl_TextBox("lblHDDModel", m_bgSprite);
	lblHDDModel->SetBitmapFont(&m_XBFont1);
	lblHDDModel->SetTextColor(m_clrLabelsTextColor);
	lblHDDModel->SetText("HDD MODEL NUMBER");
	lblHDDModel->SetTextShadow(FALSE);

	lblHDDSerial = new XKControl_TextBox("lblHDDSerial", m_bgSprite);
	lblHDDSerial->SetBitmapFont(&m_XBFont1);
	lblHDDSerial->SetTextColor(m_clrLabelsTextColor);
	lblHDDSerial->SetText("HDD SERIAL NUMBER");
	lblHDDSerial->SetTextShadow(FALSE);
	
	lblHDDPwd = new XKControl_TextBox("lblHDDPwd", m_bgSprite);
	lblHDDPwd->SetBitmapFont(&m_XBFont1);
	lblHDDPwd->SetTextColor(m_clrLabelsTextColor);
	lblHDDPwd->SetText("CURRENT HDD UNLOCK PASSWORD");
	lblHDDPwd->SetTextShadow(FALSE);

	lblXBVersion = new XKControl_TextBox("lblXBVersion", m_bgSprite);
	lblXBVersion->SetBitmapFont(&m_XBFont1);
	lblXBVersion->SetTextColor(m_clrVersionTextColor);
	lblXBVersion->SetText("DETECTED.....");
	lblXBVersion->SetTextShadow(FALSE);
	
	lblCMVersion = new XKControl_TextBox("lblCMVersion", m_bgSprite);
	lblCMVersion->SetBitmapFont(&m_XBFont1);
	lblCMVersion->SetTextColor(m_clrVersionTextColor);
	lblCMVersion->SetText("v1.6.1");
	lblCMVersion->SetTextShadow(FALSE);

	m_pFrmInfo->AddControl(txtTitle,  0, 30,  ScreenWidth, 25);
	// Left, Top, Width, Hight
	m_pFrmInfo->AddControl(txtSerial,		200, 80,  390, 25);
	m_pFrmInfo->AddControl(txtMAC,			200, 110, 390, 25);
	m_pFrmInfo->AddControl(txtVideoFormat,	200, 140, 390, 25);
	m_pFrmInfo->AddControl(txtOnlineKey,	200, 170, 390, 25);
	m_pFrmInfo->AddControl(txtDVDZone,		200, 200, 390, 25);
	m_pFrmInfo->AddControl(txtDVDModel,		200, 230, 390, 25);
	m_pFrmInfo->AddControl(txtHDDKey,		200, 260, 390, 25);
	m_pFrmInfo->AddControl(txtHDDModel,		200, 290, 390, 25);
	m_pFrmInfo->AddControl(txtHDDSerial,	200, 320, 390, 25);
	m_pFrmInfo->AddControl(txtHDDStatus,	340, 350, 250, 25);
	m_pFrmInfo->AddControl(txtHDDPwd,		5, 375, ScreenWidth-10, 25);

	m_pFrmInfo->AddControl(lblSerial,		2, 80,  250, 25);
	m_pFrmInfo->AddControl(lblMAC,			2, 110, 250, 25);
	m_pFrmInfo->AddControl(lblVideoFormat,	2, 140, 250, 25);
	m_pFrmInfo->AddControl(lblOnlineKey,	2, 170, 250, 25);
	m_pFrmInfo->AddControl(lblDVDZone,		2, 200, 250, 25);
	m_pFrmInfo->AddControl(lblDVDModel,		2, 230, 250, 25);
	m_pFrmInfo->AddControl(lblHDDKey,		2, 260, 250, 25);
	m_pFrmInfo->AddControl(lblHDDModel,		2, 290, 250, 25);
	m_pFrmInfo->AddControl(lblHDDSerial,	2, 320, 250, 25);
	m_pFrmInfo->AddControl(lblHDDPwd,		2, 350, 400, 25);
	m_pFrmInfo->AddControl(lblXBVersion,	2, 0,  150, 20);
	m_pFrmInfo->AddControl(lblCMVersion,	535, 0,  150, 20);
	

	m_pFrmInfo->AddControl(txtStatus,  0, 405,  ScreenWidth, 30);
	
}

void ConfigMagicApp::ResetConfigScreen()
{
	txtSerial->SetText("");
	txtMAC->SetText("");
	txtOnlineKey->SetText("");
	txtDVDZone->SetText("");
	txtDVDModel->SetText("");
	txtHDDKey->SetText("");
	txtHDDModel->SetText("");
	txtHDDSerial->SetText("");
	txtHDDStatus->SetText("");
	txtHDDPwd->SetText("");
}


void ConfigMagicApp::ResetLocations(LONG xoffset, LONG yoffset)
{
	XOffset = (xoffset<=0)?0:xoffset;
	YOffset = (yoffset<=0)?0:yoffset;

	m_pFrmInfo->SetPosition(XOffset, YOffset);
	m_pFrmStatus->SetPosition(XOffset, YOffset);
//	m_pxkbg->SetPosition(XOffset, YOffset);
	m_pMainMenu->SetPosition(m_MenuXOffset+XOffset, m_MenuYOffset+YOffset);
	txtStatus->SetPosition(XOffset, YOffset+405);

}

HRESULT ConfigMagicApp::Render()
{
	//Clear screen..
    m_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE,  FALSE); //some reason rendergradientbg doesnt wrk if this is false...
	RenderGradientBackground(0, 0);


	//Render Background Image..
	if (m_ActiveForm->IsImageLoaded())
		m_ActiveForm->Render();

	if(m_kbdEditBox->IsImageLoaded())
		m_kbdEditBox->Render();

	if (m_pMainMenu->IsImageLoaded())
		m_pMainMenu->Render();

	//Present Data
	m_pd3dDevice->Present(NULL, NULL, NULL, NULL);

	//Render Console Text....
	pxkconsole->Render();

	return S_OK;
}


void ConfigMagicApp::ProcessMenuSelect(LPCSTR MenuItemSelected)
{


	//Process the Menu Select Action..
	
	if ( _stricmp(MenuItemSelected, "A_LOADX") == 0)
	{
		XConfSections = LOAD_XBOX_EEPROM_SCREEN;
		WaitForInput = FALSE;
	}
	

	if ( _stricmp(MenuItemSelected, "B_LOADB") == 0)
	{
		XConfSections = LOAD_BIN_EEPROM_SCREEN;
		WaitForInput = FALSE;
	}

	if (_stricmp(MenuItemSelected, "C_LOADC") == 0)
	{
		XConfSections = LOAD_CFG_EEPROM_SCREEN;
		WaitForInput = FALSE;
	}

	if (_stricmp(MenuItemSelected, "D_EDIT") == 0)
	{
		XConfSections = EDIT_EEPROM_SCREEN;
		WaitForInput = FALSE;
	}

	if ( _stricmp(MenuItemSelected, "E_UNLOCKHDD") == 0)
	{
		XConfSections = UNLOCK_HDD_SCREEN;
		WaitForInput = FALSE;
	}

	if (_stricmp(MenuItemSelected, "F_LOCKHDD") == 0)
	{
		XConfSections = LOCK_HDD_SCREEN;
		WaitForInput = FALSE;
	}

	if ( _stricmp(MenuItemSelected, "G_BACKUP") == 0)
	{
		XConfSections = CREATE_BACKUP_SCREEN;
		WaitForInput = FALSE;
	}

	if ( _stricmp(MenuItemSelected, "H_WRITEX") == 0)
	{
		XConfSections = WRITE_XBOX_EEPROM_SCREEN;
		WaitForInput = FALSE;
	}
	//ATA Command Object to check if Disk is locked..
	XKHDD::ATA_COMMAND_OBJ hddcommand;
	//Get IDE_ATA_IDENTIFY Data for HDD ..
	ZeroMemory(&hddcommand, sizeof(XKHDD::ATA_COMMAND_OBJ));
	hddcommand.DATA_BUFFSIZE = 0;
	hddcommand.IPReg.bDriveHeadReg = IDE_DEVICE_MASTER;
	hddcommand.IPReg.bCommandReg = IDE_ATA_IDENTIFY;
	XKHDD::SendATACommand(IDE_PRIMARY_PORT, &hddcommand, IDE_COMMAND_READ);
	//Get ATA Locked State
	DWORD SecStatus = XKHDD::GetIDESecurityStatus(hddcommand.DATA_BUFFER);
	//Check if Disk is locked..
	if ((SecStatus & IDE_SECURITY_ENABLED))
	{
		if (_stricmp(MenuItemSelected, "J_EXIT") == 0)
		{
			XConfSections = EXIT_SCREEN;
			WaitForInput = FALSE;
		}
	}
}

void ConfigMagicApp::Build_EEPROM_From_Screen()
{
		m_XBOX_EEPROM_Current = FALSE;

		//** Use Confounder from current image..
		//*** Add Confounder to screen later..

		//Get HDDKey from screen
		m_pXKEEPROM->SetHDDKeyString(txtHDDKey->GetText().c_str());

		//**** Parse Video & XBE Region.. *****
		//Parse Video Region..
		LPSTR VideoRegion = NULL;
		//Set Default to NTSC
		m_pXKEEPROM->SetVideoStandardVal(XKEEPROM::NTSC_M);

		VideoRegion = strstr(txtVideoFormat->GetText().c_str(), "PAL");
		if (VideoRegion != NULL)
			m_pXKEEPROM->SetVideoStandardVal(XKEEPROM::PAL_I);

		VideoRegion = strstr(txtVideoFormat->GetText().c_str(), "NTSC");
		if (VideoRegion != NULL)
			m_pXKEEPROM->SetVideoStandardVal(XKEEPROM::NTSC_M);


		//Parse XBE Region..
		CHAR XBERegion[2];
		memset(XBERegion, '0', 2);
		strncpy(XBERegion+1, txtVideoFormat->GetText().c_str() + strlen(txtVideoFormat->GetText().c_str())-1, 1);

		switch (atoi(XBERegion))
		{
			case (XKEEPROM::NORTH_AMERICA):
				m_pXKEEPROM->SetXBERegionVal(XKEEPROM::NORTH_AMERICA);
				break;
			case (XKEEPROM::JAPAN):
				m_pXKEEPROM->SetXBERegionVal(XKEEPROM::JAPAN);
				break;
			case (XKEEPROM::EURO_AUSTRALIA):
				m_pXKEEPROM->SetXBERegionVal(XKEEPROM::EURO_AUSTRALIA);
				break;
			default:
				m_pXKEEPROM->SetXBERegionVal(XKEEPROM::NORTH_AMERICA);
				break;
		}


		//Get Online Key
		m_pXKEEPROM->SetOnlineKeyString(txtOnlineKey->GetText().c_str());


		//Get SerialNumber
		m_pXKEEPROM->SetSerialNumberString(txtSerial->GetText().c_str());

		//Get MAC Address
		m_pXKEEPROM->SetMACAddressString(txtMAC->GetText().c_str());

		//Get DVD Kit Region
		switch (atoi(txtDVDZone->GetText().c_str()))
		{
			case (XKEEPROM::ZONE1):
				m_pXKEEPROM->SetDVDRegionVal(XKEEPROM::ZONE1);
				break;
			case (XKEEPROM::ZONE2):
				m_pXKEEPROM->SetDVDRegionVal(XKEEPROM::ZONE2);
				break;
			case (XKEEPROM::ZONE3):
				m_pXKEEPROM->SetDVDRegionVal(XKEEPROM::ZONE3);
				break;
			case (XKEEPROM::ZONE4):
				m_pXKEEPROM->SetDVDRegionVal(XKEEPROM::ZONE4);
				break;
			case (XKEEPROM::ZONE5):
				m_pXKEEPROM->SetDVDRegionVal(XKEEPROM::ZONE5);
				break;
			case (XKEEPROM::ZONE6):
				m_pXKEEPROM->SetDVDRegionVal(XKEEPROM::ZONE6);
				break;
			default:
				m_pXKEEPROM->SetDVDRegionVal(XKEEPROM::ZONE_NONE);
				break;
		}

}


void ConfigMagicApp::CreateEEPROMBackup(LPCSTR BackupFilePrefix)
{
		//save current eeprom context..
		XKEEPROM::EEPROMDATA currentEEPROM;
		m_pXKEEPROM->GetEEPROMData(&currentEEPROM);
		BOOL EncryptedState = m_pXKEEPROM->IsEncrypted();

		//Read XBOX EEPROM and save to disk..
		m_pXKEEPROM->ReadFromXBOX();
		
		//incase path is on these drives..
		XKUtils::MountDiskC();
		XKUtils::MountDiskE();
		XKUtils::MountDiskF();
		CHAR tmpFileName[FILENAME_MAX];


		//Create Full path for BIN File..
		ZeroMemory(tmpFileName, FILENAME_MAX);
		strcat(tmpFileName, (LPCSTR)m_EEPROM_Backup_Path);
		strcat(tmpFileName, "\\");
		strcat(tmpFileName, BackupFilePrefix);
		strcat(tmpFileName, ".bin");

		m_pXKEEPROM->WriteToBINFile((LPCSTR)tmpFileName);

		//Create Full path for CFG File..
		ZeroMemory(tmpFileName, FILENAME_MAX);
		strcat(tmpFileName, (LPCSTR)m_EEPROM_Backup_Path);
		strcat(tmpFileName, "\\");
		strcat(tmpFileName, BackupFilePrefix);
		strcat(tmpFileName, ".cfg");

		m_pXKEEPROM->WriteToCFGFile((LPCSTR)tmpFileName);

		//Create Full path for TXT File..
		ZeroMemory(tmpFileName, FILENAME_MAX);
		strcat(tmpFileName, (LPCSTR)m_EEPROM_Backup_Path);
		strcat(tmpFileName, "\\");
		strcat(tmpFileName, BackupFilePrefix);
		strcat(tmpFileName, ".TXT");


		WriteTXTInfoFile(tmpFileName);


		//Write XBOX Information into .TXT file...

		XKUtils::UnMountDiskC();
		XKUtils::UnMountDiskE();
		XKUtils::UnMountDiskF();

		//switch eeprom context Back to previous
		if (EncryptedState)
			m_pXKEEPROM->SetEncryptedEEPROMData(&currentEEPROM);
		else 
			m_pXKEEPROM->SetDecryptedEEPROMData(m_XBOX_Version, &currentEEPROM);
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

		
		//Write File Header..
		strcat(tmpFileStr, "*******  CONFIGMAGIC INFORMATION FILE  *******\r\n");

		if (m_XBOX_Version == XKEEPROM::V1_0)
			strcat(tmpFileStr, "\r\nXBOX Version = \t\tV1.0");
		else if (m_XBOX_Version == XKEEPROM::V1_1)
			strcat(tmpFileStr, "\r\nXBOX Version = \t\tV1.1");
        else if (m_XBOX_Version == XKEEPROM::V1_6) 
                 strcat(tmpFileStr, "\r\nXBOX Version = \t\tV1.6");
		//Get Kernel Version
		tmpSize = 256;ZeroMemory(tmpData, tmpSize);
		sprintf(tmpData, "\r\nKernel Version: \t%d.%d.%d.%d", *((USHORT*)XboxKrnlVersion),*((USHORT*)XboxKrnlVersion+1),*((USHORT*)XboxKrnlVersion+2),*((USHORT*)XboxKrnlVersion+3));
		strcat(tmpFileStr, tmpData);

		
		//Get Memory Status
		strcat(tmpFileStr, "\r\nXBOX RAM = \t\t");
		tmpSize = 256;ZeroMemory(tmpData, tmpSize);
		MEMORYSTATUS stat;
		GlobalMemoryStatus( &stat );
		ltoa(stat.dwTotalPhys/1024/1024, tmpData, 10);
		strcat(tmpFileStr, tmpData);
		strcat(tmpFileStr, " MBytes");


		//Write Serial Number..
		strcat(tmpFileStr, "\r\n\r\nXBOX Serial Number = \t");
		tmpSize = 256;ZeroMemory(tmpData, tmpSize);
		m_pXKEEPROM->GetSerialNumberString(tmpData, &tmpSize);
		strcat(tmpFileStr, tmpData);

		//Write MAC Address..
		strcat(tmpFileStr, "\r\nXBOX MAC Address = \t");
		tmpSize = 256;ZeroMemory(tmpData, tmpSize);
		m_pXKEEPROM->GetMACAddressString(tmpData, &tmpSize);
		strcat(tmpFileStr, tmpData);

		//Write Online Key ..
		strcat(tmpFileStr, "\r\nXBOX Online Key = \t");
		tmpSize = 256;ZeroMemory(tmpData, tmpSize);
		m_pXKEEPROM->GetOnlineKeyString(tmpData, &tmpSize);
		strcat(tmpFileStr, tmpData);

		//Write VideoMode ..
		strcat(tmpFileStr, "\r\nXBOX Video Mode = \t");
		VIDEO_STANDARD vdo = m_pXKEEPROM->GetVideoStandardVal();
		if (vdo == XKEEPROM::VIDEO_STANDARD::PAL_I)
			strcat(tmpFileStr, "PAL");
		else
			strcat(tmpFileStr, "NTSC");


		//Write XBE Region..
		strcat(tmpFileStr, "\r\nXBOX XBE Region = \t");
		tmpSize = 256;ZeroMemory(tmpData, tmpSize);
		m_pXKEEPROM->GetXBERegionString(tmpData, &tmpSize);
		strcat(tmpFileStr, tmpData);

		//Write HDDKey..
		strcat(tmpFileStr, "\r\nXBOX HDD Key = \t\t");
		tmpSize = 256;ZeroMemory(tmpData, tmpSize);
		m_pXKEEPROM->GetHDDKeyString(tmpData, &tmpSize);
		strcat(tmpFileStr, tmpData);

		//Write Confounder..
		strcat(tmpFileStr, "\r\nXBOX Confounder = \t");
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
		strcat(tmpFileStr, "\r\n\r\nXBOX HDD Model = \t");
		tmpSize = 256;ZeroMemory(tmpData, tmpSize);
		XKHDD::GetIDEModel(cmdObj.DATA_BUFFER, (LPSTR)tmpData, &tmpSize);
		strcat(tmpFileStr, tmpData);

		//Write HDD Serial..
		strcat(tmpFileStr, "\r\nXBOX HDD Serial = \t");
		tmpSize = 256;ZeroMemory(tmpData, tmpSize);
		XKHDD::GetIDESerial(cmdObj.DATA_BUFFER, (LPSTR)tmpData, &tmpSize);
		strcat(tmpFileStr, tmpData);
		
		//Write HDD Password..
		tmpSize = 256;ZeroMemory(tmpData, tmpSize);
		strcat(tmpFileStr, "\r\n\r\nXBOX HDD Password = \t");

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
		strcat(tmpFileStr, "\r\n\r\nXBOX DVD Model = \t");
		tmpSize = 256;ZeroMemory(tmpData, tmpSize);
		XKHDD::GetIDEModel(cmdObj.DATA_BUFFER, (LPSTR)tmpData, &tmpSize);
		strcat(tmpFileStr, tmpData);



		strupr(tmpFileStr);
		
		WriteFile(hf, tmpFileStr, (DWORD)strlen(tmpFileStr), &dwBytesWrote, NULL);

	}

	delete[] tmpFileStr;
	CloseHandle(hf);

}


//-----------------------------------------------------------------------------
//	Plays the specified video file.  Pressing the A button will stop
//  playback
//-----------------------------------------------------------------------------
HRESULT ConfigMagicApp::PlayVideo(CHAR* strFilename )
{
    // Open the video file to be played
    if( FAILED( m_XKVideoPlayer.OpenFile( strFilename ) ) )
        return XBAPPERR_MEDIANOTFOUND;

    // Get information about the video format
    WMVVIDEOINFO videoInfo;
    m_XKVideoPlayer.GetVideoInfo( &videoInfo );

    // Create two textures based off the video format.  We need to
    // use two textures because the overlay surface reads directly
    // from the texture.  Updating a texture while it's being used
    // by the overlay will cause tearing
    for( int i = 0; i < 2; i++ )
    {
        m_pd3dDevice->CreateTexture( videoInfo.dwWidth,
                                     videoInfo.dwHeight,
                                     1,
                                     0,
                                     D3DFMT_YUY2,
                                     0,
                                     &m_pOverlay[i] );
        m_pOverlay[i]->GetSurfaceLevel( 0, &m_pSurface[i] );
    }
    m_dwCurrent = 0;

    // Set up source and destination rectangles
    // TODO: Adjust for different aspect ratios?
    RECT SrcRect;
    RECT DestRect;

    SrcRect.left    = 0;
    SrcRect.top     = 0;
    SrcRect.right   = videoInfo.dwWidth;
    SrcRect.bottom  = videoInfo.dwHeight;

    DestRect.left   = XOffset;
    DestRect.top    = YOffset;
    DestRect.right  = ScreenWidth+XOffset;
    DestRect.bottom = ScreenHeight+YOffset;

    // Timing statistics
    DWORD dwMaxCycles = 0;
    DWORD dwFrame = 0;
    DWORD dwMaxFrame = 0xFFFFFFFF;

    // Enable the Direct3D overlay surface
    m_pd3dDevice->EnableOverlay( TRUE );

    // Playback video.  This is done in its own loop to avoid wasting time
    // waiting for buffer swaps, etc, when we don't need to.
    for( ; ; )
    {
        // If we're ready for the next frame...
        if( m_XKVideoPlayer.IsReady() )
        {
            dwFrame++;

            // Get the next frame into our texture
            m_XKVideoPlayer.GetTexture( m_pOverlay[m_dwCurrent] );

            // Update the overlay surface to use this texture
            m_pd3dDevice->UpdateOverlay( m_pSurface[m_dwCurrent], &SrcRect, &DestRect, FALSE, 0 );

            // Decode the next frame
            if( S_FALSE == m_XKVideoPlayer.DecodeNext() )
            {
                break;
            }

            // We'll use the other texture next time
            m_dwCurrent ^= 1;
        }
        
        // Check for A button to stop playback here
        BOOL bInput = FALSE;
        XBInput_GetInput();
        for( int i = 0; i < 4; i++ )
        {
            if( g_Gamepads[i].hDevice &&
                g_Gamepads[i].bPressedAnalogButtons[ XINPUT_GAMEPAD_A ] )
                bInput = TRUE;
        }

        if( bInput )
            break;
    }

    // We're done!
    OUTPUT_DEBUG_STRING( "Done!\n" );

    // When disabling the overlay, you should wait until just after a 
    // vertical blank to avoid any flickering
    m_pd3dDevice->BlockUntilVerticalBlank();
    m_pd3dDevice->EnableOverlay( FALSE );

    for( int i = 0; i < 2; i++ )
    {
        m_pSurface[i]->Release();
        m_pOverlay[i]->Release();
    }
    m_XKVideoPlayer.CloseFile();

    return S_OK;

}

void ConfigMagicApp::LoadSettingsFromINI()
{
	//Read all the User Type Config Values from the INI file...
	
	UCHAR tmpINIItem[FILENAME_MAX];
	DWORD tmpLen = 256;

	//*************************** READ SCREEN SIZE & OFFSETS *****************************************
	XOffset = 25;
	tmpLen = FILENAME_MAX;
	ZeroMemory(tmpINIItem, tmpLen);
	if (XKGeneral::ReadINIFileItem(SkinConfigFileName, "SKIN", "ScreenXOffset", (LPSTR)tmpINIItem, &tmpLen))
	{
		XOffset = XKGeneral::HexStrToDWORD(tmpINIItem, &tmpLen, TRUE, TRUE);
	}
	
	YOffset = 20;
	tmpLen = FILENAME_MAX;
	ZeroMemory(tmpINIItem, tmpLen);
	if (XKGeneral::ReadINIFileItem(SkinConfigFileName, "SKIN", "ScreenYOffset", (LPSTR)tmpINIItem, &tmpLen))
	{
		YOffset = XKGeneral::HexStrToDWORD(tmpINIItem, &tmpLen, TRUE, TRUE);
	}
	


	XImgCorrection = 45;
	tmpLen = FILENAME_MAX;
	ZeroMemory(tmpINIItem, tmpLen);
	if (XKGeneral::ReadINIFileItem(SkinConfigFileName, "SKIN", "ImgCorrectionX", (LPSTR)tmpINIItem, &tmpLen))
	{
		XImgCorrection = XKGeneral::HexStrToDWORD(tmpINIItem, &tmpLen, TRUE, TRUE);
	}
	
	YImgCorrection = 40;
	tmpLen = FILENAME_MAX;
	ZeroMemory(tmpINIItem, tmpLen);
	if (XKGeneral::ReadINIFileItem(SkinConfigFileName, "SKIN", "ImgCorrectionY", (LPSTR)tmpINIItem, &tmpLen))
	{
		YImgCorrection = XKGeneral::HexStrToDWORD(tmpINIItem, &tmpLen, TRUE, TRUE);
	}
	

	//*************************** READ FILE LOCATIONS  *****************************************

	ZeroMemory(m_EEPROM_BIN_FileName, FILENAME_MAX);
	ZeroMemory(m_EEPROM_CFG_FileName, FILENAME_MAX);
	ZeroMemory(m_EEPROM_Backup_Path, FILENAME_MAX);

	ZeroMemory(tmpINIItem, tmpLen);
	if (XKGeneral::ReadINIFileItem(MainConfigFileName, "INPUT_FILES", "EEPROMBinFile", (LPSTR)tmpINIItem, &tmpLen))
	{
		XKGeneral::StripQuotes((LPSTR)tmpINIItem, &tmpLen);	
		strncpy((LPSTR)m_EEPROM_BIN_FileName, (LPCSTR)tmpINIItem, tmpLen);
	}
	else
		strcpy((LPSTR)m_EEPROM_BIN_FileName, "D:\\data\\eeprom.bin");

	tmpLen = FILENAME_MAX;
	ZeroMemory(tmpINIItem, tmpLen);
	if (XKGeneral::ReadINIFileItem(MainConfigFileName, "INPUT_FILES", "EEPROMCfgFile", (LPSTR)tmpINIItem, &tmpLen))
	{
		XKGeneral::StripQuotes((LPSTR)tmpINIItem, &tmpLen);	
		strncpy((LPSTR)m_EEPROM_CFG_FileName, (LPCSTR)tmpINIItem, tmpLen);
	}
	else
		strcpy((LPSTR)m_EEPROM_CFG_FileName, "D:\\data\\eeprom.cfg");

	tmpLen = FILENAME_MAX;
	ZeroMemory(tmpINIItem, tmpLen);
	if (XKGeneral::ReadINIFileItem(MainConfigFileName, "OUTPUT_FILES", "BackupFilesPath", (LPSTR)tmpINIItem, &tmpLen))
	{
		XKGeneral::StripQuotes((LPSTR)tmpINIItem, &tmpLen);	
		strncpy((LPSTR)m_EEPROM_Backup_Path, (LPCSTR)tmpINIItem, tmpLen);
	}
	else
		strcpy((LPSTR)m_EEPROM_Backup_Path, "C:\\backup");


	//*************************** READ SKINABLE SETTINGS  *****************************************

	m_clrCurrentHeadingColor	 = 0xFF626F7F;
	tmpLen = FILENAME_MAX;
	ZeroMemory(tmpINIItem, tmpLen);
	if (XKGeneral::ReadINIFileItem(SkinConfigFileName, "SKIN", "CurrentHeadingColor", (LPSTR)tmpINIItem, &tmpLen))
	{
		m_clrCurrentHeadingColor = XKGeneral::HexStrToDWORD(tmpINIItem, &tmpLen, TRUE, TRUE);
	}

	m_clrBinFileHeadingColor	= 0xFF626F7F;
	tmpLen = FILENAME_MAX;
	ZeroMemory(tmpINIItem, tmpLen);
	if (XKGeneral::ReadINIFileItem(SkinConfigFileName, "SKIN", "BinFileHeadingColor", (LPSTR)tmpINIItem, &tmpLen))
	{
		m_clrBinFileHeadingColor = XKGeneral::HexStrToDWORD(tmpINIItem, &tmpLen, TRUE, TRUE);
	}
	
	m_clrCfgFileHeadingColor	= 0xFF626F7F;
	tmpLen = FILENAME_MAX;
	ZeroMemory(tmpINIItem, tmpLen);
	if (XKGeneral::ReadINIFileItem(SkinConfigFileName, "SKIN", "CfgFileHeadingColor", (LPSTR)tmpINIItem, &tmpLen))
	{
		m_clrCfgFileHeadingColor = XKGeneral::HexStrToDWORD(tmpINIItem, &tmpLen, TRUE, TRUE);
	}

	m_clrOnTheFlyHeadingColor	= 0xFF626F7F;
	tmpLen = FILENAME_MAX;
	ZeroMemory(tmpINIItem, tmpLen);
	if (XKGeneral::ReadINIFileItem(SkinConfigFileName, "SKIN", "OnTheFlyHeadingColor", (LPSTR)tmpINIItem, &tmpLen))
	{
		m_clrOnTheFlyHeadingColor = XKGeneral::HexStrToDWORD(tmpINIItem, &tmpLen, TRUE, TRUE);
	}

	m_clrMessageTextColor		= 0xFF626F7F;
	tmpLen = FILENAME_MAX;
	ZeroMemory(tmpINIItem, tmpLen);
	if (XKGeneral::ReadINIFileItem(SkinConfigFileName, "SKIN", "MessageTextColor", (LPSTR)tmpINIItem, &tmpLen))
	{
		m_clrMessageTextColor = XKGeneral::HexStrToDWORD(tmpINIItem, &tmpLen, TRUE, TRUE);
	}

	m_clrStatusTextColor		= 0xFF626F7F;
	tmpLen = FILENAME_MAX;
	ZeroMemory(tmpINIItem, tmpLen);
	if (XKGeneral::ReadINIFileItem(SkinConfigFileName, "SKIN", "StatusTextColor", (LPSTR)tmpINIItem, &tmpLen))
	{
		m_clrStatusTextColor = XKGeneral::HexStrToDWORD(tmpINIItem, &tmpLen, TRUE, TRUE);
	}

	m_clrLabelsTextColor		= 0xFFFFFFFF;
	tmpLen = FILENAME_MAX;
	ZeroMemory(tmpINIItem, tmpLen);
	if (XKGeneral::ReadINIFileItem(SkinConfigFileName, "SKIN", "LabelsTextColor", (LPSTR)tmpINIItem, &tmpLen))
	{
		m_clrLabelsTextColor = XKGeneral::HexStrToDWORD(tmpINIItem, &tmpLen, TRUE, TRUE);
	}

	m_clrValuesTextColor		= 0xFFB9C6CC;
	tmpLen = FILENAME_MAX;
	ZeroMemory(tmpINIItem, tmpLen);
	if (XKGeneral::ReadINIFileItem(SkinConfigFileName, "SKIN", "ValuesTextColor", (LPSTR)tmpINIItem, &tmpLen))
	{
		m_clrValuesTextColor = XKGeneral::HexStrToDWORD(tmpINIItem, &tmpLen, TRUE, TRUE);
	}

	m_clrHDDStatusTextColor		= 0xFFDC8200;
	tmpLen = FILENAME_MAX;
	ZeroMemory(tmpINIItem, tmpLen);
	if (XKGeneral::ReadINIFileItem(SkinConfigFileName, "SKIN", "HDDStatusTextColor", (LPSTR)tmpINIItem, &tmpLen))
	{
		m_clrHDDStatusTextColor = XKGeneral::HexStrToDWORD(tmpINIItem, &tmpLen, TRUE, TRUE);
	}

	m_clrVersionTextColor		= 0xFFDC8200;
	tmpLen = FILENAME_MAX;
	ZeroMemory(tmpINIItem, tmpLen);
	if (XKGeneral::ReadINIFileItem(SkinConfigFileName, "SKIN", "VersionTextColor", (LPSTR)tmpINIItem, &tmpLen))
	{
		m_clrVersionTextColor = XKGeneral::HexStrToDWORD(tmpINIItem, &tmpLen, TRUE, TRUE);
	}

	m_clrMenuItemTextColor		= 0xFFFFFFFF;
	tmpLen = FILENAME_MAX;
	ZeroMemory(tmpINIItem, tmpLen);
	if (XKGeneral::ReadINIFileItem(SkinConfigFileName, "SKIN", "MenuItemTextColor", (LPSTR)tmpINIItem, &tmpLen))
	{
		m_clrMenuItemTextColor = XKGeneral::HexStrToDWORD(tmpINIItem, &tmpLen, TRUE, TRUE);
	}

	m_clrMenuSelectTextColor	= 0xFF000000;
	tmpLen = FILENAME_MAX;
	ZeroMemory(tmpINIItem, tmpLen);
	if (XKGeneral::ReadINIFileItem(SkinConfigFileName, "SKIN", "MenuSelectTextColor", (LPSTR)tmpINIItem, &tmpLen))
	{
		m_clrMenuSelectTextColor = XKGeneral::HexStrToDWORD(tmpINIItem, &tmpLen, TRUE, TRUE);
	}

	m_clrMenuHeadingColor	= 0xFF000000;
	tmpLen = FILENAME_MAX;
	ZeroMemory(tmpINIItem, tmpLen);
	if (XKGeneral::ReadINIFileItem(SkinConfigFileName, "SKIN", "MenuHeadingColor", (LPSTR)tmpINIItem, &tmpLen))
	{
		m_clrMenuHeadingColor = XKGeneral::HexStrToDWORD(tmpINIItem, &tmpLen, TRUE, TRUE);
	}


}
