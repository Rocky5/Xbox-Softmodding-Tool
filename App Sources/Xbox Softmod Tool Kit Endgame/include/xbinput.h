//-----------------------------------------------------------------------------
// File: XBInput.h
//
// Desc: Input helper functions for the XBox samples
//
// Hist: 12.15.00 - Separated from XBUtil.h for December XDK release
//       01.03.00 - Made changes for real Xbox controller
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------
#ifndef XBINPUT_H
#define XBINPUT_H




//-----------------------------------------------------------------------------
// Name: struct XBGAMEPAD
// Desc: structure for holding Gamepad data
//-----------------------------------------------------------------------------
struct XBGAMEPAD : public XINPUT_GAMEPAD
{
    // Inherited members from XINPUT_GAMEPAD
	//
    // WORD    wButtons;
    // BYTE    bAnalogButtons[8];
    // SHORT   sThumbLX;
    // SHORT   sThumbLY;
    // SHORT   sThumbRX;
    // SHORT   sThumbRY;

    // Thumb stick values in range [-1,+1]
    FLOAT      fX1;
    FLOAT      fY1;
    FLOAT      fX2;
    FLOAT      fY2;
    
    // Buttons pressed since last poll
    WORD       wLastButtons;
    BOOL       bLastAnalogButtons[8];
    WORD       wPressedButtons;
    BOOL       bPressedAnalogButtons[8];

	// Rumble properties
	XINPUT_RUMBLE	Rumble;
	XINPUT_FEEDBACK	Feedback;

    // Device properties
    XINPUT_CAPABILITIES caps;
    HANDLE     hDevice;

    // Flags for whether gamepad was just inserted or removed
    BOOL       bInserted;
    BOOL       bRemoved;
};




//-----------------------------------------------------------------------------
// Global access to gamepad devices
//-----------------------------------------------------------------------------
extern XBGAMEPAD g_Gamepads[4];




//-----------------------------------------------------------------------------
// Name: XBInput_CreateGamepads()
// Desc: Creates the gamepad devices
//-----------------------------------------------------------------------------
HRESULT XBInput_CreateGamepads( XBGAMEPAD** ppGamepads = NULL );




//-----------------------------------------------------------------------------
// Name: XBInput_GetInput()
// Desc: Processes input from the gamepads
//-----------------------------------------------------------------------------
VOID XBInput_GetInput( XBGAMEPAD* pGamepads = NULL );




//-----------------------------------------------------------------------------
// Name: XBInput_GetPrimaryController()
// Desc: The first controller used by the player or the controller on the
//       lowest numbered slot. NULL if no controllers inserted.
//-----------------------------------------------------------------------------
const XBGAMEPAD* XBInput_GetPrimaryController();




//-----------------------------------------------------------------------------
// Name: XBInput_IsAnyButtonActive()
// Desc: TRUE if any button or thumbstick depressed on the given controller
//-----------------------------------------------------------------------------
BOOL XBInput_IsAnyButtonActive( const XBGAMEPAD* );




#endif // XBINPUT_H
