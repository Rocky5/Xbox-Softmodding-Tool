#include <stdio.h>
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


********************************************************************************************************
**	     XKUTILS.CPP - XBOX Utility Class' Implementation      
********************************************************************************************************
**
**	This Class encapsulates some XBOX utility functions and are mostly self explanatory
**	not a lot of commenting or documentation needed here...
**
********************************************************************************************************

********************************************************************************************************
**	CREDITS:
********************************************************************************************************
**	XBOX-LINUX TEAM:
**  ---------------
**		Wow, you guys are awsome !!  I bow down to your greatness !!  
**		REFERENCE URL:  http://xbox-linux.sourceforge.net
**
********************************************************************************************************

UPDATE LOG:
--------------------------------------------------------------------------------------------------------
Date: 11/28/2004
By: Yoshihiro
Reason: Updated for Xbox 1.6 and other Cool Stuff :)
--------------------------------------------------------------------------------------------------------
Date: 02/18/2003
By: UNDEAD [team-assembly]
Reason: Prepared 0.2 for Public Release
--------------------------------------------------------------------------------------------------------

*/
#pragma once
#if defined (_XBOX)
	//This complete file is only supported for XBOX..
	#pragma message ("Compiling for XBOX: " __FILE__)


#include "XKUtils.h"
#include "XKExports.h"


LONG XKUtils::MountDevice(LPSTR sSymbolicLinkName, LPSTR sDeviceName)
{

	UNICODE_STRING 	deviceName;
	deviceName.Buffer  = sDeviceName;
	deviceName.Length = (USHORT)strlen(sDeviceName);
	deviceName.MaximumLength = (USHORT)strlen(sDeviceName) + 1;

	UNICODE_STRING 	symbolicLinkName;
	symbolicLinkName.Buffer  = sSymbolicLinkName;
	symbolicLinkName.Length = (USHORT)strlen(sSymbolicLinkName);
	symbolicLinkName.MaximumLength = (USHORT)strlen(sSymbolicLinkName) + 1;

	return IoCreateSymbolicLink(&symbolicLinkName, &deviceName);

}

LONG XKUtils::UnMountDevice(LPSTR sSymbolicLinkName)
{
	UNICODE_STRING 	symbolicLinkName;
	symbolicLinkName.Buffer  = sSymbolicLinkName;
	symbolicLinkName.Length = (USHORT)strlen(sSymbolicLinkName);
	symbolicLinkName.MaximumLength = (USHORT)strlen(sSymbolicLinkName) + 1;

	return IoDeleteSymbolicLink(&symbolicLinkName);

}
LONG XKUtils::MountDiskA()
{
	char cTemp[MAX_PATH];
	int iInd;
	PANSI_STRING usString=(PANSI_STRING)XeImageFileName;

	strncpy(cTemp,(char *)usString->Buffer,usString->Length);
	cTemp[usString->Length]=0;
	for (iInd=strlen(cTemp)-1;iInd>=0;iInd--)
	{
		if (cTemp[iInd]=='\\')
		{
			cTemp[++iInd]=0;
			break;
		}
	}
	return MountDevice(DriveA,cTemp);
}
LONG XKUtils::MountDiskC()
{
	return MountDevice(DriveC, DeviceC);
}

LONG XKUtils::MountDiskD()
{
	return MountDevice(DriveD, CdRom);
}

LONG XKUtils::MountDiskE()
{
	return MountDevice(DriveE, DeviceE);
}

LONG XKUtils::MountDiskF()
{
	return MountDevice(DriveF, DeviceF);
}
LONG XKUtils::MountDiskG()
{
	return MountDevice(DriveG, DeviceG);
}
LONG XKUtils::MountDiskH()
{
	return MountDevice(DriveH, DeviceH);
}
LONG XKUtils::MountDiskX()
{
	return MountDevice(DriveX, DeviceX);
}
LONG XKUtils::MountDiskY()
{
	return MountDevice(DriveY, DeviceY);
}
LONG XKUtils::MountDiskZ()
{
	return MountDevice(DriveZ, DeviceZ);
}
LONG XKUtils::MountDiskS()
{
	return MountDevice(DriveS, DeviceS);
}

void XKUtils::MountAllDisks()
{
	UnMountAllDisks();
	MountDiskA();
	MountDiskC();
	MountDiskD();
	MountDiskE();
	MountDiskF();
	MountDiskG();
	MountDiskH();
	MountDiskX();
	MountDiskY();
	MountDiskZ();
	MountDiskS();
}
LONG XKUtils::UnMountDiskA()
{
	return UnMountDevice(DriveA);
}
LONG XKUtils::UnMountDiskC()
{
	return UnMountDevice(DriveC);
}

LONG XKUtils::UnMountDiskD()
{
	return UnMountDevice(DriveD);
}

LONG XKUtils::UnMountDiskE()
{
	return UnMountDevice(DriveE);
}

LONG XKUtils::UnMountDiskF()
{
	return UnMountDevice(DriveF);
}
LONG XKUtils::UnMountDiskG()
{
	return UnMountDevice(DriveG);
}
LONG XKUtils::UnMountDiskH()
{
	return UnMountDevice(DriveH);
}
LONG XKUtils::UnMountDiskX()
{
	return UnMountDevice(DriveX);
}

LONG XKUtils::UnMountDiskY()
{
	return UnMountDevice(DriveY);
}

LONG XKUtils::UnMountDiskZ()
{
	return UnMountDevice(DriveZ);
}

LONG XKUtils::UnMountDiskS()
{
	return UnMountDevice(DriveS);
}

void XKUtils::UnMountAllDisks()
{
	UnMountDiskA();
	UnMountDiskC();
	UnMountDiskD();
	UnMountDiskE();
	UnMountDiskF();
	UnMountDiskG();
	UnMountDiskH();
	UnMountDiskX();
	UnMountDiskY();
	UnMountDiskZ();
	UnMountDiskS();
}

void XKUtils::XBOXRebootToDash()
{
	XLaunchNewImage(NULL,NULL);
}

void XKUtils::XBOXReset()
{
	OUTPUT_DEBUG_STRING("XKUtils: Reset...\n");
	HalWriteSMBusValue(SMBDEV_PIC16L, PIC16L_CMD_POWER, 0, POWER_SUBCMD_RESET);
}

void XKUtils::SetXBOXLEDStatus(UCHAR LEDStatus)
{
	OUTPUT_DEBUG_STRING("XKUtils: Setting LED Status Registers...\n");
	HalWriteSMBusValue(SMBDEV_PIC16L, PIC16L_CMD_LED_REGISTER, 0, LEDStatus);
	Sleep(10);
	HalWriteSMBusValue(SMBDEV_PIC16L, PIC16L_CMD_LED_MODE, 0, 1);
}

void XKUtils::XBOXPowerOff()
{
	//Console Shutdown...
	OUTPUT_DEBUG_STRING("XKUtils: Shutdown...\n");
	HalWriteSMBusValue(SMBDEV_PIC16L, PIC16L_CMD_POWER, 0, POWER_SUBCMD_POWER_OFF);

}

void XKUtils::XBOXPowerCycle()
{
	//Console Shutdown...
	OUTPUT_DEBUG_STRING("XKUtils: Cycle...\n");
	HalWriteSMBusValue(SMBDEV_PIC16L, PIC16L_CMD_POWER, 0, POWER_SUBCMD_CYCLE);

}


/*  StartPos and EndPos are both ZERO offset based */
void XKUtils::WriteEEPROMToXBOX(LPBYTE EEPROMDATA, UCHAR STARTPOS, UCHAR ENDPOS)
{

	OUTPUT_DEBUG_STRING( "XKUtils: Writing EEPROM to XBOX...\n" );
 	for (UCHAR i=STARTPOS;i<ENDPOS;i++)
	{

		HalWriteSMBusValue(SMBDEV_EEPROM , i, 0, EEPROMDATA[i]);
		Sleep(5);
	}

}

/*  StartPos and EndPos are both ZERO offset based */
void XKUtils::ReadEEPROMFromXBOX(LPBYTE EEPROMDATA, UCHAR STARTPOS, UCHAR ENDPOS)
{
	ZeroMemory(EEPROMDATA, 256);
	
	OUTPUT_DEBUG_STRING( "XKUtils: Reading EEPROM from XBOX...\n" );
	for (UCHAR i=STARTPOS;i<ENDPOS;i++)
	{
		HalReadSMBusValue(SMBDEV_EEPROM, i, 0, EEPROMDATA+i);
		Sleep(1);

	}
	
}

void XKUtils::DVDDisableEjectReset()
{

	OUTPUT_DEBUG_STRING("XKUtils: Disable Reset on DVD Tray Eject...\n");
	HalWriteSMBusValue(SMBDEV_PIC16L, PIC16L_CMD_RESET_ON_EJECT, 0, RESET_ON_EJECT_SUBCMD_DISABLE);
	Sleep(1);
}

void XKUtils::DVDEnableEjectReset()
{

	OUTPUT_DEBUG_STRING("XKUtils: Enable Reset on DVD Tray Eject...\n");
	HalWriteSMBusValue(SMBDEV_PIC16L, PIC16L_CMD_RESET_ON_EJECT, 0, RESET_ON_EJECT_SUBCMD_ENABLE);
	Sleep(1);
}

void XKUtils::DVDEjectTray()
{
	OUTPUT_DEBUG_STRING("XKUtils: Ejecting DVD Tray...\n");
	HalWriteSMBusValue(SMBDEV_PIC16L, PIC16L_CMD_EJECT, 0, 0x00);
	Sleep(1);
}

void XKUtils::DVDLoadTray()
{
	OUTPUT_DEBUG_STRING("XKUtils: Loading DVD Tray...\n");
	HalWriteSMBusValue(SMBDEV_PIC16L, PIC16L_CMD_EJECT, 0, EJECT_SUBCMD_LOAD);
	Sleep(1);
}

void XKUtils::XBOXCPUTemp(int *piTemp)
{
	long BTemp;

	HalReadSMBusValue(SMBDEV_PIC16L,0x09,0,(BYTE *)&BTemp);
	*piTemp=(int)BTemp;
//	Sleep(1);
}
void XKUtils::XBOXSYSTemp(int *piTemp)
{
	long BTemp;

	HalReadSMBusValue(SMBDEV_PIC16L,0x0a,0,(BYTE *)&BTemp);
	*piTemp=(int)BTemp;
//	Sleep(1);
}
int XKUtils::GetFanSpeed(void)
{
	int bySpeed,iReturn=0;

	// read it
	HalReadSMBusValue(SMBDEV_PIC16L, 0x10, 0, (LPBYTE)&bySpeed );
	iReturn=(int)bySpeed;
	return (iReturn*2);
}
void XKUtils::SetFanSpeed(int iFanSpeed)
{
	// enable user specified speed
	HalWriteSMBusValue(SMBDEV_PIC16L,0x05,0,1); 
	Sleep(10);
	// write it
	HalWriteSMBusValue(SMBDEV_PIC16L,0x06,0,(iFanSpeed/2));
	Sleep(10);
	// Lock it
	HalWriteSMBusValue(SMBDEV_PIC16L,0x05,0,0); 
	// enable user specified speed
	HalWriteSMBusValue(SMBDEV_PIC16L,0x05,0,1);
}
int XKUtils::GetVideoEncoderType(void)
{
	int iTemp;

	if (HalReadSMBusValue(SMBDEV_VIDEO_ENCODER_CONNEXANT,VIDEO_ENCODER_CMD_DETECT,0,(LPBYTE)&iTemp)==0) return VIDEO_ENCODER_CONNEXANT;
	if (HalReadSMBusValue(SMBDEV_VIDEO_ENCODER_FOCUS,VIDEO_ENCODER_CMD_DETECT,0,(LPBYTE)&iTemp)==0) return VIDEO_ENCODER_FOCUS;
	if (HalReadSMBusValue(SMBDEV_VIDEO_ENCODER_XCALIBUR,VIDEO_ENCODER_CMD_DETECT,0,(LPBYTE)&iTemp)==0) return VIDEO_ENCODER_XCALIBUR;
	
	return VIDEO_ENCODER_UNKNOWN;
}
bool XKUtils::GetCPUFrequence(double *pdCPUFreq)
{
	unsigned __int64 Fwin;
	unsigned __int64 Twin_avant, Twin_apres;
	double Tcpu_avant, Tcpu_apres;
	double Fcpu;

	// Lit la frequence du chronometre Windows
	if (!QueryPerformanceFrequency((LARGE_INTEGER*)&Fwin)) return false;

	// Avant
	Tcpu_avant=RDTSC();
	QueryPerformanceCounter((LARGE_INTEGER*)&Twin_avant);

	// Pause de 500 ms
	Sleep(500);

	// Apres
	Tcpu_apres=RDTSC();
	QueryPerformanceCounter((LARGE_INTEGER*)&Twin_apres);

	// Calcule la fréquence en MHz
	Fcpu=(Tcpu_apres-Tcpu_avant);
	Fcpu*=Fwin;
	Fcpu/=(double)(Twin_apres-Twin_avant);
	*pdCPUFreq=Fcpu;
	return true;
}
double XKUtils::RDTSC(void)
{
	unsigned long a, b;
	double x;

	__asm
	{
		RDTSC
		mov [a],eax
		mov [b],edx
	}
	x=b;
	x*=4294967296;
	x+=a;
	return x;
}

void XKUtils::LaunchXBE(char *psFullPath)
{
	
	char sFullPathRep[MAX_PATH],sXBEName[MAX_PATH],sDrive;
	int iInd;

	sDrive=tolower(psFullPath[0]);
	switch (sDrive)
	{
		case 'c':
			strcpy(sFullPathRep,DeviceC);
		break;
		case 'd':
			strcpy(sFullPathRep,CdRom);
		break;
		case 'e':
			strcpy(sFullPathRep,DeviceE);
		break;
		case 'f':
			strcpy(sFullPathRep,DeviceF);
		break;
		case 'g':
			strcpy(sFullPathRep,DeviceG);
		break;
		case 'h':
			strcpy(sFullPathRep,DeviceH);
		break;
		case 'x':
			strcpy(sFullPathRep,DeviceX);
		break;
		case 'y':
			strcpy(sFullPathRep,DeviceY);
		break;
		case 'z':
			strcpy(sFullPathRep,DeviceZ);
		break;
		case 's':
			strcpy(sFullPathRep,DeviceS);
		break;
		default:
			return;
		break;
	}

	lstrcat(sFullPathRep,&psFullPath[2]);

	iInd=lstrlen(sFullPathRep);
	while (sFullPathRep[iInd]!='\\'&&iInd>0) iInd--;
	sFullPathRep[iInd]=0;
	
	sprintf(sXBEName,"D:\\%s",&sFullPathRep[++iInd]);

	XKUtils::UnMountDiskD();
	XKUtils::MountDevice("\\??\\D:",sFullPathRep);

	XLaunchNewImage(sXBEName,NULL);

}
#endif