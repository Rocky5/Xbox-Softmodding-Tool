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
#include "shortcutxbe.h"
#define dashloader_Files_path	"E:\\UDATA\\21585554\\000000000000\\nkpatcher settings\\dashloader\\"
static FILE* logfile = NULL;
void initlog()
{
	/* mount up a drive to use for debug logging */
	XUnmount("E:");
	XMount("E:", "\\Device\\Harddisk0\\Partition1");
	logfile = fopen(dashloader_Files_path"Dashloader.log", "w+t");
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
void ErrorHandler(char *xbepath)
{
	debuglog("\n------------------------------------------------");
	debuglog("Prep Dashboard");
	debuglog("------------------------------------------------");
	debuglog("Loading Prep Dashboard - E:\\Prep\\Default.xbe");
	XLaunchXBE("E:\\Prep\\Default.xbe");
	debuglog("Dashboard doesn't Exist");
	/**/
	debuglog("\n------------------------------------------------");
	debuglog("Softmod Quick Update/Upgrade Dashboard");
	debuglog("------------------------------------------------");
	debuglog("Loading Quick Update Dashboard - E:\\Quick Update\\Default.xbe");
	XLaunchXBE("E:\\Quick Update\\Default.xbe");
	debuglog("Loading Quick Upgrade Dashboard - E:\\Quick Upgrade\\Default.xbe");
	XLaunchXBE("E:\\Quick Upgrade\\Default.xbe");
	debuglog("Dashboards don't Exist");
	/**/
	debuglog("\n------------------------------------------------");
	debuglog("ShadowC Partition Integrity Check");
	debuglog("------------------------------------------------");
	debuglog("Checking ShadowC image integrity");
	XLaunchXBE("C:\\nkpatcher\\rescuedash\\loader.xbe");
	debuglog("ShadowC image is loaded");
	/**/
	debuglog("\n------------------------------------------------");
	debuglog("Dashboard Locations");
	debuglog("------------------------------------------------");
	/**/
	debuglog("Loading C:\\XBMC-Emustation\\Default.xbe");
	XLaunchXBE("C:\\XBMC-Emustation\\Default.xbe");
	debuglog("Dashboard doesn't Exist\n");
	/**/
	debuglog("Loading E:\\XBMC-Emustation\\Default.xbe");
	XLaunchXBE("E:\\XBMC-Emustation\\Default.xbe");
	debuglog("Dashboard doesn't Exist\n");
	/**/
	debuglog("Loading F:\\XBMC-Emustation\\Default.xbe");
	XLaunchXBE("F:\\XBMC-Emustation\\Default.xbe");
	debuglog("Dashboard doesn't Exist\n");
	/**/
	debuglog("Loading C:\\XBMC4Gamers\\Default.xbe");
	XLaunchXBE("C:\\XBMC4Gamers\\Default.xbe");
	debuglog("Dashboard doesn't Exist\n");
	/**/
	debuglog("Loading E:\\XBMC4Gamers\\Default.xbe");
	XLaunchXBE("E:\\XBMC4Gamers\\Default.xbe");
	debuglog("Dashboard doesn't Exist\n");
	/**/
	debuglog("Loading F:\\XBMC4Gamers\\Default.xbe");
	XLaunchXBE("F:\\XBMC4Gamers\\Default.xbe");
	debuglog("Dashboard doesn't Exist\n");
	/**/
	debuglog("Loading C:\\XBMC4Xbox\\Default.xbe");
	XLaunchXBE("C:\\XBMC4Xbox\\Default.xbe");
	debuglog("Dashboard doesn't Exist\n");
	/**/
	debuglog("Loading E:\\XBMC4Xbox\\Default.xbe");
	XLaunchXBE("E:\\XBMC4Xbox\\Default.xbe");
	debuglog("Dashboard doesn't Exist\n");
	/**/
	debuglog("Loading F:\\XBMC4Xbox\\Default.xbe");
	XLaunchXBE("F:\\XBMC4Xbox\\Default.xbe");
	debuglog("Dashboard doesn't Exist\n");
	/**/
	debuglog("Loading C:\\XBMC\\Default.xbe");
	XLaunchXBE("C:\\XBMC\\Default.xbe");
	debuglog("Dashboard doesn't Exist\n");
	/**/
	debuglog("Loading E:\\XBMC\\Default.xbe");
	XLaunchXBE("E:\\XBMC\\Default.xbe");
	debuglog("Dashboard doesn't Exist\n");
	/**/
	debuglog("Loading F:\\XBMC\\Default.xbe");
	XLaunchXBE("F:\\XBMC\\Default.xbe");
	debuglog("Dashboard doesn't Exist\n");
	/**/
	debuglog("Loading C:\\Dashboard\\Default.xbe");
	XLaunchXBE("C:\\Dashboard\\Default.xbe");
	debuglog("Dashboard doesn't Exist\n");
	/**/
	debuglog("Loading E:\\Dashboard\\Default.xbe");
	XLaunchXBE("E:\\Dashboard\\Default.xbe");
	debuglog("Dashboard doesn't Exist\n");
	/**/
	debuglog("Loading F:\\Dashboard\\Default.xbe");
	XLaunchXBE("F:\\Dashboard\\Default.xbe");
	debuglog("Dashboard doesn't Exist\n");
	/**/
	debuglog("Loading C:\\Dash\\Default.xbe");
	XLaunchXBE("C:\\Dash\\Default.xbe");
	debuglog("Dashboard doesn't Exist\n");
	/**/
	debuglog("Loading E:\\Dash\\Default.xbe");
	XLaunchXBE("E:\\Dash\\Default.xbe");
	debuglog("Dashboard doesn't Exist\n");
	/**/
	debuglog("Loading F:\\Dash\\Default.xbe");
	XLaunchXBE("F:\\Dash\\Default.xbe");
	debuglog("Dashboard doesn't Exist\n");
	/**/
	debuglog("Loading E:\\Default.xbe");
	XLaunchXBE("E:\\Default.xbe");
	debuglog("Dashboard doesn't Exist\n");
	/**/
	debuglog("Loading E:\\Dashboard.xbe");
	XLaunchXBE("E:\\Dashboard.xbe");
	debuglog("Dashboard doesn't Exist\n");
	/**/
	debuglog("Loading C:\\Evoxdash.xbe");
	XLaunchXBE("C:\\Evoxdash.xbe");
	debuglog("Dashboard doesn't Exist\n");
	/**/
	debuglog("Loading E:\\Evoxdash.xbe");
	XLaunchXBE("E:\\Evoxdash.xbe");
	debuglog("Dashboard doesn't Exist\n");
	/**/
	debuglog("Loading C:\\XBMC.xbe");
	XLaunchXBE("C:\\XBMC.xbe");
	debuglog("Dashboard doesn't Exist\n");
	/**/
	debuglog("Loading E:\\XBMC.xbe");
	XLaunchXBE("E:\\XBMC.xbe");
	debuglog("Dashboard doesn't Exist\n");
	/**/
	debuglog("\n------------------------------------------------");
	debuglog("Rescue Dashboard Locations");
	debuglog("------------------------------------------------");
	/**/
	debuglog("Loading Rescue Dashboard TDATA");
	XLaunchXBE("E:\\TDATA\\Rescuedash\\Default.xbe");
	debuglog("Rescue Dashboard doesn't Exist\n");
	/**/
	debuglog("Loading Rescue Dashboard UDATA");
	XLaunchXBE("E:\\UDATA\\Rescuedash\\Default.xbe");
	debuglog("RescueDashboard doesn't Exist\n");
	/**/
	debuglog("Loading Shadowc rescue Dashboard");
	XLaunchXBE("R:\\NKPatcher\\rescuedash\\loader.xbe");
	debuglog("Rescue Dashboard doesn't Exist\n");
	/**/
	debuglog("\n------------------------------------------------");
	debuglog("All failed :( - trying to reboot");
	debuglog("------------------------------------------------");
	XReboot();
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
	/* launching xbe */
	XLaunchXBE(target);
	/* if we get here something went wrong */
	return -1;
}
int LaunchRecovery(char* filename)
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
	/* launching xbe */
	XLaunchXBE(target);
	/* if we get here something went wrong */
	return -1;
}
/* initial starting point of program */
int main(int argc,char* argv[])
{
	char shortcut[MAX_PATH];
	initlog();
	/* move to xbepath buffer */	
	strcpy(shortcut, dashloader_Files_path"Custom_Dash.cfg");
	XUnmount("E:");
	XMount("E:", "\\Device\\Harddisk0\\Partition1");
	debuglog("Dashloader Build 1.1");
	XInitDevices( 0, NULL );
	if( FAILED( XBInput_CreateGamepads( &m_Gamepad ) ) )
	{
		debuglog("ERROR - Cant create gamepad");
	}
	int timer = 2;
	while(1)
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
		INT nThumbLX = 0;
		INT nThumbLY = 0;
		INT nThumbRX = 0;
		INT nThumbRY = 0;
		for( DWORD i=0; i<4; i++ )
		{
			if( m_Gamepad[i].hDevice )
			{
				// Only account for thumbstick info beyond the deadzone
				if( m_Gamepad[i].sThumbLX > XINPUT_DEADZONE ||
						m_Gamepad[i].sThumbLX < -XINPUT_DEADZONE )
				nThumbLX += m_Gamepad[i].sThumbLX;
				if( m_Gamepad[i].sThumbLY > XINPUT_DEADZONE ||
						m_Gamepad[i].sThumbLY < -XINPUT_DEADZONE )
				nThumbLY += m_Gamepad[i].sThumbLY;
				if( m_Gamepad[i].sThumbRX > XINPUT_DEADZONE ||
						m_Gamepad[i].sThumbRX < -XINPUT_DEADZONE )
				nThumbRX += m_Gamepad[i].sThumbRX;
				if( m_Gamepad[i].sThumbRY > XINPUT_DEADZONE ||
						m_Gamepad[i].sThumbRY < -XINPUT_DEADZONE )
				nThumbRY += m_Gamepad[i].sThumbRY;
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
		// Clamp summed thumbstick values to proper range
		m_DefaultGamepad.sThumbLX = SHORT( nThumbLX );
		m_DefaultGamepad.sThumbLY = SHORT( nThumbLY );
		m_DefaultGamepad.sThumbRX = SHORT( nThumbRX );
		m_DefaultGamepad.sThumbRY = SHORT( nThumbRY );
		if( m_DefaultGamepad.bPressedAnalogButtons[XINPUT_GAMEPAD_Y] && (m_DefaultGamepad.wPressedButtons & XINPUT_GAMEPAD_START) )
		{
			debuglog("\n------------------------------------------------");
			debuglog("Rescue Dashboard Locations");
			debuglog("------------------------------------------------");
			debuglog("Loading Custom Rescue Dashboard\n");
			LaunchRecovery(dashloader_Files_path"Custom_Recovery.cfg");
			debuglog("Custom Rescue Dashboard doesn't Exist\n");
			/**/
			debuglog("Loading Rescue Dashboard TDATA");
			XLaunchXBE("E:\\TDATA\\Rescuedash\\Default.xbe");
			debuglog("Rescue Dashboard doesn't Exist\n");
			/**/
			debuglog("Loading Rescue Dashboard UDATA");
			XLaunchXBE("E:\\UDATA\\Rescuedash\\Default.xbe");
			debuglog("RescueDashboard doesn't Exist\n");
			/**/
			debuglog("Loading Shadowc rescue Dashboard");
			XLaunchXBE("R:\\NKPatcher\\rescuedash\\loader.xbe");
			debuglog("Rescue Dashboard doesn't Exist\n");
		}
		else if( m_DefaultGamepad.bPressedAnalogButtons[XINPUT_GAMEPAD_A] )
		{
			strcpy(shortcut, dashloader_Files_path"A_Button_Dash.cfg");
		}
		else if( m_DefaultGamepad.bPressedAnalogButtons[XINPUT_GAMEPAD_B] )
		{
			strcpy(shortcut, dashloader_Files_path"B_Button_Dash.cfg");
		}
		else if( m_DefaultGamepad.bPressedAnalogButtons[XINPUT_GAMEPAD_X] )
		{
			strcpy(shortcut, dashloader_Files_path"X_Button_Dash.cfg");
		}
		else if( m_DefaultGamepad.bPressedAnalogButtons[XINPUT_GAMEPAD_Y] )
		{
			strcpy(shortcut, dashloader_Files_path"Y_Button_Dash.cfg");
		}
		else if( m_DefaultGamepad.wPressedButtons & XINPUT_GAMEPAD_START )
		{
			strcpy(shortcut, dashloader_Files_path"Start_Button_Dash.cfg");
		}
		else if( m_DefaultGamepad.wPressedButtons & XINPUT_GAMEPAD_BACK )
		{
			strcpy(shortcut, dashloader_Files_path"Back_Button_Dash.cfg");
		}
		else if( m_DefaultGamepad.bPressedAnalogButtons[XINPUT_GAMEPAD_BLACK] )
		{
			strcpy(shortcut, dashloader_Files_path"Black_Button_Dash.cfg");
		}
		else if( m_DefaultGamepad.bPressedAnalogButtons[XINPUT_GAMEPAD_WHITE] )
		{
			strcpy(shortcut, dashloader_Files_path"White_Button_Dash.cfg");
		}
		Sleep(500);
		timer -= 1;
		if(timer==0)
		break;
	}
	LaunchShortcut(shortcut);
	ErrorHandler(dashloader_Files_path"Dashloader.log");
}