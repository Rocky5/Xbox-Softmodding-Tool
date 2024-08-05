#ifdef _MSC_VER
#include "msxdk.h"
#else
#include <hal/xbox.h>
#include <hal/fileio.h>
#include <openxdk/debug.h>
#include <xboxkrnl/xboxkrnl.h>
#include <xboxkrnl/types.h>
#define MAX_PATH 256
#include "xmount.h"
#include "msxdk.h"
#endif
#include <string.h>
#include <stdio.h>
#include <sys\stat.h>
#include <fstream>
#include <iostream>
#include "shortcutxbe.h"
#include "kernelpatcher.h"
#include "external.h"

#define ES_IGR "E:\\CACHE\\LocalCache20.bin"
#define Dashloader_Files_Path "C:\\Dashloader\\"
#define Ind_Checker_File "E:\\CACHE\\LocalCache30.bin"
#define Patcher_File "E:\\CACHE\\LocalCache40.bin"

#define Patched_M8 ((char*)0x8002691E)
#define Patched_Ind ((char*)0x8002B4B7)

#define UNPatched_M8_1 ((char*)0x8002691B)
#define UNPatched_M8_2 ((char*)0x8002691E)
#define UNPatched_M8_3 ((char*)0x8002690E)

#define UNPatched_Ind_1 ((char*)0x8002B4B5)
#define UNPatched_Ind_2 ((char*)0x8002B4B7)
#define UNPatched_Ind_3 ((char*)0x8002B4C7)

static FILE* logfile = NULL;
int file_exist(char *name)
{
	struct stat   buffer;   
	return (stat (name, &buffer) == 0);
}
void debuglog(const char* format, ...)
{
	char buffer[1024];
	va_list va;
	va_start(va, format);
	vsprintf(buffer, format, va);
	va_end(va);
	strcat(buffer, "\n");
	if( logfile )
	{
		fputs(buffer, logfile);
		fflush(logfile);
	}
#ifdef _MSC_VER
	OutputDebugString(buffer);
#endif
}
int LaunchShortcut(char* filename)
{
	FILE* file;
	char target[MAX_PATH];
	unsigned int length;
	file = fopen(filename, "r+t");
	if(file == NULL)
	{
		/* debuglog("ERROR - Unable to open cutfile"); */
		return -1;
	}
	/* read first line untill linebreak */
	length = 0;
	while(1)
	{
		target[length] = fgetc(file);
		if( target[length] == EOF || target[length] == '\n' || target[length] == '\r' )
		break;
		length++;
	}
	fclose(file);
	if(length <= 0)
	{
		return -1;
	}
	/* null terminate string, and chop of any trailing blanks */
	target[length] = '\0';
	/* remove ES_File */
	remove(ES_IGR);
	Sleep(300);
	/* launching xbe */
	XLaunchXBE(target);
	/* if we get here something went wrong */
	return -1;
}
/* initial starting point of program */
int main(int argc,char* argv[])
{
	XMount("C:", "\\Device\\Harddisk0\\Partition2");
	XMount("E:", "\\Device\\Harddisk0\\Partition1");
	CreateDirectory("E:\\CACHE", NULL);
	CreateDirectory(Dashloader_Files_Path, NULL);
	XInitDevices( 0, NULL );
	if( FAILED( XBInput_CreateGamepads( &m_Gamepad ) ) )
	{
		debuglog("ERROR - Cant create gamepad");
	}
	logfile = fopen(Dashloader_Files_Path"Dashloader.log", "w+t");
	debuglog("Dashloader Build 1.4.3\n");
	// Ind-Bios 5003/M8+ with virtual disc loader patch		
	if (!file_exist(Dashloader_Files_Path"Disable Virtual ISO Support.bin"))
	{
		// I check for these so I dont try and patch unsupported bios
		if (*UNPatched_M8_1 == 'ì' && *UNPatched_M8_2 == ' ' && *UNPatched_M8_3 == 'ÿ' ||
			*UNPatched_Ind_1 == 'ƒ' && *UNPatched_Ind_2 == ' ' && *UNPatched_Ind_3 == '€')
		{
			if (!file_exist(Patcher_File))
			{
				std::ofstream PatcherXBEFile(Patcher_File, std::ios::binary);
				PatcherXBEFile.write(reinterpret_cast<const char*>(kernel_patcher), sizeof(kernel_patcher));
			}
			XLaunchXBE(Patcher_File);
		}
		remove(Patcher_File);
		int patched = 0;
		int legacy = 0;
		if (*Patched_M8 == '™')
		{
			// M8+ doesnt work with quick IGR when a virtual disc is mounted
			debuglog("M8+ Detected and XISO support added\n");
			patched = 1;
		}
		if (*Patched_Ind == '™')
		{
			debuglog("Ind-Bios 5003 Detected and XISO support added\n");
			patched = 1;
			legacy = 1;
 			if (!file_exist(Ind_Checker_File))
			{
				// Creating blank file so I know not to keep reloading the game over and over. This wouldn't be required if I didn't need to manually load the disc for indbios
				std::ofstream blankFile(Ind_Checker_File, std::ios::binary);
				blankFile.close();
				
				// This is so we can exit the game when we IGR. If we don't it does what M8+ does.
				// If using cerbios new attacher xbe you will need to IGR twice.
				XLaunchXBE("D:\\default.xbe");
			}
			remove(Ind_Checker_File);
		}
		// Dismount virtual disc drive if present and if not disabled
		// loggin is disbaled for this as it will populate even if there is no disc and if I add a check for a disc it would populate every time you load a disc and IGR. (Cerbios only)
		if (!file_exist(Dashloader_Files_Path"Disabled Virtual-ISO Dismount.bin"))
		{
			// debuglog("Unmounting: Virtual Drive\n");
			HANDLE h;
			NTSTATUS status;
			ANSI_STRING dev_name;
			if (legacy)
			{
				RtlInitAnsiString(&dev_name, "\\Device\\CdRom1");
				// debuglog("\t> Found NKPatcher virtual drive");
			}
			else
			{
				RtlInitAnsiString(&dev_name, "\\Device\\Virtual0\\Image0");
				// debuglog("\t> Found Cerbios virtual drive");
			}
			unsigned long IOCTL_VIRTUAL_CDROM = legacy ? IOCTL_VIRTUAL_CDROM_DETACH : IOCTL_VIRTUAL_DETACH;
			OBJECT_ATTRIBUTES obj_attr;
			obj_attr.RootDirectory = NULL;
			obj_attr.ObjectName = &dev_name;
			obj_attr.Attributes = OBJ_CASE_INSENSITIVE;
			IO_STATUS_BLOCK io_status;
			status = NtOpenFile(&h, GENERIC_READ | SYNCHRONIZE, &obj_attr, &io_status, FILE_SHARE_READ, FILE_SYNCHRONOUS_IO_NONALERT);
			if (NT_SUCCESS(status)) {
				status = NtDeviceIoControlFile(h, NULL, NULL, NULL, &io_status, IOCTL_VIRTUAL_CDROM, NULL, 0, NULL, 0);
				// if (NT_SUCCESS(status))
					// debuglog("\t\t> Dismounted virtual drive\n");
				// else
					// debuglog("\t\t> Failed to dismount virtual drive\n");
			}
			NtClose(h);
		}
	}
	// Sleep fixes screen resetting on some xbox.
	Sleep(300);
	if (file_exist(ES_IGR))
	LaunchShortcut(ES_IGR);
	int timer = 0;
	while(timer++ <= 1000)
	{
		//-----------------------------------------
		// Handle input
		//-----------------------------------------
		// Read the input for all connected gampads
		XBInput_GetInput( m_Gamepad );
		// Lump inputs of all connected gamepads into one common structure.
		// This is done so apps that need only one gamepad can function with
		// any gamepad.
		ZeroMemory( &m_DefaultGamepad, sizeof(m_DefaultGamepad) );
		for( DWORD i=0; i<4; i++ )
		{
			if( m_Gamepad[i].hDevice )
			{
				// Only account for thumbstick info beyond the deadzone
				m_DefaultGamepad.fX1 += m_Gamepad[i].fX1;
				m_DefaultGamepad.fY1 += m_Gamepad[i].fY1;
				m_DefaultGamepad.fX2 += m_Gamepad[i].fX2;
				m_DefaultGamepad.fY2 += m_Gamepad[i].fY2;
				m_DefaultGamepad.wButtons        |= m_Gamepad[i].wButtons;
				m_DefaultGamepad.wPressedButtons |= m_Gamepad[i].wPressedButtons;
				m_DefaultGamepad.wLastButtons    |= m_Gamepad[i].wLastButtons;
				for( DWORD b=0; b<8; b++ )
				{
					m_DefaultGamepad.bAnalogButtons[b]        |= m_Gamepad[i].bAnalogButtons[b];
					m_DefaultGamepad.bPressedAnalogButtons[b] |= m_Gamepad[i].bPressedAnalogButtons[b];
					m_DefaultGamepad.bLastAnalogButtons[b]    |= m_Gamepad[i].bLastAnalogButtons[b];
				}
			}
		}
		if( m_DefaultGamepad.bPressedAnalogButtons[XINPUT_GAMEPAD_Y] && (m_DefaultGamepad.wPressedButtons & XINPUT_GAMEPAD_START) )
		{
			debuglog("Checking: Rescue dashboards\n");
			debuglog("\t> Loading: Custom Rescue dashboard");
			LaunchShortcut(Dashloader_Files_Path"Custom_Recovery.cfg");

			debuglog("\t\t> Custom Rescue dashboard not found\n\n\t> Loading: Rescue dashboard TDATA");
			XLaunchXBE("E:\\TDATA\\Rescuedash\\Default.xbe");

			debuglog("\t\t> Rescue dashboard not found\n\n\t> Loading: Rescue dashboard UDATA");
			XLaunchXBE("E:\\UDATA\\Rescuedash\\Default.xbe");
		}
		if( m_DefaultGamepad.bPressedAnalogButtons[XINPUT_GAMEPAD_X] && m_DefaultGamepad.bPressedAnalogButtons[XINPUT_GAMEPAD_Y] )
		break;
		
		if( m_DefaultGamepad.bPressedAnalogButtons[XINPUT_GAMEPAD_WHITE] && m_DefaultGamepad.bPressedAnalogButtons[XINPUT_GAMEPAD_Y] )
		break;

		if( m_DefaultGamepad.bPressedAnalogButtons[XINPUT_GAMEPAD_A] )
		{
			debuglog("Loading: A Button dashboard");
			LaunchShortcut(Dashloader_Files_Path"A_Button_Dash.cfg");
		}
		if( m_DefaultGamepad.bPressedAnalogButtons[XINPUT_GAMEPAD_B] )
		{
			debuglog("Loading: B Button dashboard");
			LaunchShortcut(Dashloader_Files_Path"B_Button_Dash.cfg");
		}
		if( m_DefaultGamepad.bPressedAnalogButtons[XINPUT_GAMEPAD_X] )
		{
			debuglog("Loading: X Button dashboard");
			LaunchShortcut(Dashloader_Files_Path"X_Button_Dash.cfg");
		}
		if( m_DefaultGamepad.bPressedAnalogButtons[XINPUT_GAMEPAD_Y] )
		{
			debuglog("Loading: Y Button dashboard");
			LaunchShortcut(Dashloader_Files_Path"Y_Button_Dash.cfg");
		}
		if( m_DefaultGamepad.wPressedButtons & XINPUT_GAMEPAD_START )
		{
			debuglog("Loading: Start Button dashboard");
			LaunchShortcut(Dashloader_Files_Path"Start_Button_Dash.cfg");
		}
		if( m_DefaultGamepad.wPressedButtons & XINPUT_GAMEPAD_BACK )
		{
			debuglog("Loading: Back Button dashboard");
			LaunchShortcut(Dashloader_Files_Path"Back_Button_Dash.cfg");
		}
		if( m_DefaultGamepad.bPressedAnalogButtons[XINPUT_GAMEPAD_BLACK] )
		{
			debuglog("Loading: Black Button dashboard");
			LaunchShortcut(Dashloader_Files_Path"Black_Button_Dash.cfg");
		}
		if( m_DefaultGamepad.bPressedAnalogButtons[XINPUT_GAMEPAD_WHITE] )
		{
			debuglog("Loading: White Button dashboard");
			LaunchShortcut(Dashloader_Files_Path"White_Button_Dash.cfg");
		}

		Sleep(1);
	}
	// Try preset dashboard paths

	debuglog("Checking: Prep dashboard\n");
	debuglog("\t> Loading: Prep dashboard - E:\\Prep\\Default.xbe");
	XLaunchXBE("E:\\Prep\\Default.xbe");
	debuglog("\t\t> Prep dashboard not found\n");

	debuglog("Checking: Custom Dashboard\n");
	debuglog("\t> Loading: Custom dashboard");
	LaunchShortcut(Dashloader_Files_Path"Custom_Dash.cfg");
	debuglog("\t\t> Custom dashboard not found\n");
	
	debuglog("Checking: Dashboard Locations\n");
	
	debuglog("\t> Loading: C:\\XBMC-Emustation\\Default.xbe");
	XLaunchXBE("C:\\XBMC-Emustation\\Default.xbe");
	
	debuglog("\t\t> Dashboard not found\n\n\t> Loading: E:\\XBMC-Emustation\\Default.xbe");
	XLaunchXBE("E:\\XBMC-Emustation\\Default.xbe");
	
	debuglog("\t\t> Dashboard not found\n\n\t> Loading: F:\\XBMC-Emustation\\Default.xbe");
	XLaunchXBE("F:\\XBMC-Emustation\\Default.xbe");
	
	debuglog("\t\t> Dashboard not found\n\n\t> Loading: C:\\XBMC4Gamers\\Default.xbe");
	XLaunchXBE("C:\\XBMC4Gamers\\Default.xbe");
	
	debuglog("\t\t> Dashboard not found\n\n\t> Loading: E:\\XBMC4Gamers\\Default.xbe");
	XLaunchXBE("E:\\XBMC4Gamers\\Default.xbe");
	
	debuglog("\t\t> Dashboard not found\n\n\t> Loading: F:\\XBMC4Gamers\\Default.xbe");
	XLaunchXBE("F:\\XBMC4Gamers\\Default.xbe");
	
	debuglog("\t\t> Dashboard not found\n\n\t> Loading: C:\\XBMC4Xbox\\Default.xbe");
	XLaunchXBE("C:\\XBMC4Xbox\\Default.xbe");
	
	debuglog("\t\t> Dashboard not found\n\n\t> Loading: E:\\XBMC4Xbox\\Default.xbe");
	XLaunchXBE("E:\\XBMC4Xbox\\Default.xbe");
	
	debuglog("\t\t> Dashboard not found\n\n\t> Loading: F:\\XBMC4Xbox\\Default.xbe");
	XLaunchXBE("F:\\XBMC4Xbox\\Default.xbe");
	
	debuglog("\t\t> Dashboard not found\n\n\t> Loading: C:\\XBMC\\Default.xbe");
	XLaunchXBE("C:\\XBMC\\Default.xbe");
	
	debuglog("\t\t> Dashboard not found\n\n\t> Loading: E:\\XBMC\\Default.xbe");
	XLaunchXBE("E:\\XBMC\\Default.xbe");
	
	debuglog("\t\t> Dashboard not found\n\n\t> Loading: F:\\XBMC\\Default.xbe");
	XLaunchXBE("F:\\XBMC\\Default.xbe");
	
	debuglog("\t\t> Dashboard not found\n\n\t> Loading: C:\\dashboard\\Default.xbe");
	XLaunchXBE("C:\\dashboard\\Default.xbe");
	
	debuglog("\t\t> Dashboard not found\n\n\t> Loading: E:\\dashboard\\Default.xbe");
	XLaunchXBE("E:\\dashboard\\Default.xbe");
	
	debuglog("\t\t> Dashboard not found\n\n\t> Loading: F:\\dashboard\\Default.xbe");
	XLaunchXBE("F:\\dashboard\\Default.xbe");
	
	debuglog("\t\t> Dashboard not found\n\n\t> Loading: C:\\Dash\\Default.xbe");
	XLaunchXBE("C:\\Dash\\Default.xbe");
	
	debuglog("\t\t> Dashboard not found\n\n\t> Loading: E:\\Dash\\Default.xbe");
	XLaunchXBE("E:\\Dash\\Default.xbe");
	
	debuglog("\t\t> Dashboard not found\n\n\t> Loading: F:\\Dash\\Default.xbe");
	XLaunchXBE("F:\\Dash\\Default.xbe");
	
	debuglog("\t\t> Dashboard not found\n\n\t> Loading: E:\\Default.xbe");
	XLaunchXBE("E:\\Default.xbe");
	
	debuglog("\t\t> Dashboard not found\n\n\t> Loading: E:\\dashboard.xbe");
	XLaunchXBE("E:\\dashboard.xbe");
	
	debuglog("\t\t> Dashboard not found\n\n\t> Loading: E:\\Evoxdash.xbe");
	XLaunchXBE("E:\\Evoxdash.xbe");
	
	debuglog("\t\t> Dashboard not found\n\n\t> Loading: C:\\XBMC.xbe");
	XLaunchXBE("C:\\XBMC.xbe");
	
	debuglog("\t\t> Dashboard not found\n\n\t> Loading: E:\\XBMC.xbe");
	XLaunchXBE("E:\\XBMC.xbe");
	debuglog("\t\t> Dashboard not found\n");

	debuglog("\nAll failed :( - insert a disc and load from there");
	XReboot();
}