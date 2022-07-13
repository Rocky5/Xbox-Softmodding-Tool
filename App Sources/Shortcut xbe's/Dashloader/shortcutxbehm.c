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
#include "dismountxbe.h"
#include "kernelpatcher.h"
#define ES_IGR	"E:\\CACHE\\LocalCache20.bin"
#define dashloader_Files_path	"C:\\Dashloader\\"
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
	/* launching xbe */
	XLaunchXBE(target);
	/* if we get here something went wrong */
	return -1;
}
/* initial starting point of program */
int main(int argc,char* argv[])
{
	XMount("C:", "\\Device\\Harddisk0\\Partition2"); XMount("E:", "\\Device\\Harddisk0\\Partition1");
	CreateDirectory(dashloader_Files_path, NULL);
	XInitDevices( 0, NULL );
	if( FAILED( XBInput_CreateGamepads( &m_Gamepad ) ) )
	{
		debuglog("ERROR - Cant create gamepad");
	}
	logfile = fopen(dashloader_Files_path"Dashloader.log", "w+t");
	debuglog("Dashloader Build 1.4.2\n");
	// Ind-Bios 5003/M8+ with virtual disc loader patch		
	if (!file_exist(dashloader_Files_path"Disable Virtual ISO Support.bin"))
	{
		int i;
		char *patched_M8 = (char *)0x8002691E; // M8+
		if (*patched_M8=='™')
		debuglog("M8+ Detected and XISO support added\n");
		char *patched_Ind = (char *)0x8002B4B7; // ind-biod 5003
		if (*patched_Ind=='™')
		{
			debuglog("Ind-Bios 5003 Detected and XISO support added\n");
			XMount("D:", "\\Device\\Cdrom0"); XMount("VD:", "\\Device\\Cdrom1");
			CreateDirectory("E:\\CACHE", NULL);
			if (file_exist("VD:\\default.xbe"))
			{
				if (!file_exist("E:\\CACHE\\LocalCache30.bin"))
				{
					debuglog("Creating De-mounter");
					std::ofstream DismountXBEFile("E:\\CACHE\\LocalCache30.bin", std::ios::binary);
					for(i = 0; i < sizeof(dismount_xbe); i++)
					{
						DismountXBEFile << dismount_xbe[i];
					}
					DismountXBEFile.close();
					XLaunchXBE("D:\\default.xbe");
				}
				if (!file_exist(dashloader_Files_path"Disabled Virtual-ISO Dismount.bin"))
				{
					debuglog("Unmounting Virtual Drive");
					XLaunchXBE("E:\\CACHE\\LocalCache30.bin");
				}
			}
			remove("E:\\CACHE\\LocalCache30.bin");
		}
		// Check for characters at these offsets so I know to patch it.
		char *unpatched_M8_1 = (char *)0x8002691B; char *unpatched_M8_2 = (char *)0x8002691E; char *unpatched_M8_3 = (char *)0x8002690E;
		char *unpatched_Ind_1 = (char *)0x8002B4B5; char *unpatched_Ind_2 = (char *)0x8002B4B7; char *unpatched_Ind_3 = (char *)0x8002B4C7;
		if (*unpatched_M8_1=='ì' && *unpatched_M8_2==' ' && *unpatched_M8_3=='ÿ' || *unpatched_Ind_1=='ƒ' && *unpatched_Ind_2==' ' && *unpatched_Ind_3=='€')
		{
			if (!file_exist("E:\\CACHE\\LocalCache40.bin"))
			{
				std::ofstream PatcherXBEFile("E:\\CACHE\\LocalCache40.bin", std::ios::binary);
				for(i = 0; i < sizeof(kernel_patcher); i++)
				{
					PatcherXBEFile << kernel_patcher[i];
				}
				PatcherXBEFile.close();
			}
			XLaunchXBE("E:\\CACHE\\LocalCache40.bin");
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
			LaunchShortcut(dashloader_Files_path"Custom_Recovery.cfg");

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
			LaunchShortcut(dashloader_Files_path"A_Button_Dash.cfg");
		}
		if( m_DefaultGamepad.bPressedAnalogButtons[XINPUT_GAMEPAD_B] )
		{
			debuglog("Loading: B Button dashboard");
			LaunchShortcut(dashloader_Files_path"B_Button_Dash.cfg");
		}
		if( m_DefaultGamepad.bPressedAnalogButtons[XINPUT_GAMEPAD_X] )
		{
			debuglog("Loading: X Button dashboard");
			LaunchShortcut(dashloader_Files_path"X_Button_Dash.cfg");
		}
		if( m_DefaultGamepad.bPressedAnalogButtons[XINPUT_GAMEPAD_Y] )
		{
			debuglog("Loading: Y Button dashboard");
			LaunchShortcut(dashloader_Files_path"Y_Button_Dash.cfg");
		}
		if( m_DefaultGamepad.wPressedButtons & XINPUT_GAMEPAD_START )
		{
			debuglog("Loading: Start Button dashboard");
			LaunchShortcut(dashloader_Files_path"Start_Button_Dash.cfg");
		}
		if( m_DefaultGamepad.wPressedButtons & XINPUT_GAMEPAD_BACK )
		{
			debuglog("Loading: Back Button dashboard");
			LaunchShortcut(dashloader_Files_path"Back_Button_Dash.cfg");
		}
		if( m_DefaultGamepad.bPressedAnalogButtons[XINPUT_GAMEPAD_BLACK] )
		{
			debuglog("Loading: Black Button dashboard");
			LaunchShortcut(dashloader_Files_path"Black_Button_Dash.cfg");
		}
		if( m_DefaultGamepad.bPressedAnalogButtons[XINPUT_GAMEPAD_WHITE] )
		{
			debuglog("Loading: White Button dashboard");
			LaunchShortcut(dashloader_Files_path"White_Button_Dash.cfg");
		}

		Sleep(1);
	}
	// Try preset dashboard paths
	debuglog("Checking: Custom Dashboard\n");
	debuglog("\t> Loading: Custom dashboard");
	LaunchShortcut(dashloader_Files_path"Custom_Dash.cfg");
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