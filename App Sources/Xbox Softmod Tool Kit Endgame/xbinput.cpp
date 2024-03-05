//-----------------------------------------------------------------------------
// File: XBInput.cpp
//
// Desc: Input helper functions for the XBox samples
//
// Hist: 12.15.00 - Separated from XBUtil.cpp for December XDK release
//       01.03.00 - Made changes for real Xbox controller
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------
#include <xtl.h>
#include "XBInput.h"




//-----------------------------------------------------------------------------
// Globals
//-----------------------------------------------------------------------------

// Deadzone for thumbsticks
#define XBINPUT_DEADZONE 0.25

// Threshold for analog buttons
#define XBINPUT_BUTTONTHRESHOLD 1

// Global instance of gamepad devices
XBGAMEPAD g_Gamepads[4];




//-----------------------------------------------------------------------------
// Name: XBInput_CreateGamepads()
// Desc: Creates the gamepad devices
//-----------------------------------------------------------------------------
HRESULT XBInput_CreateGamepads( XBGAMEPAD** ppGamepads )
{
    // Get a mask of all currently available devices
    DWORD dwDeviceMask = XGetDevices( XDEVICE_TYPE_GAMEPAD );

    // Open the devices
    for( DWORD i=0; i < XGetPortCount(); i++ )
    {
        ZeroMemory( &g_Gamepads[i], sizeof(XBGAMEPAD) );
        if( dwDeviceMask & (1<<i) ) 
        {
            // Get a handle to the device
            g_Gamepads[i].hDevice = XInputOpen( XDEVICE_TYPE_GAMEPAD, i, 
                                                XDEVICE_NO_SLOT, NULL );

            // Store capabilites of the device
            XInputGetCapabilities( g_Gamepads[i].hDevice, &g_Gamepads[i].caps );
        }
    }

    // Created devices are kept global, but for those who prefer member
    // variables, they can get a pointer to the gamepads returned.
    if( ppGamepads )
        (*ppGamepads) = g_Gamepads;

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: XBInput_GetInput()
// Desc: Processes input from the gamepads
//-----------------------------------------------------------------------------
VOID XBInput_GetInput( XBGAMEPAD* pGamepads )
{
    if( NULL == pGamepads )
        pGamepads = g_Gamepads;

    // Get status about gamepad insertions and removals. Note that, in order to
    // not miss devices, we will check for removed device BEFORE checking for
    // insertions
    DWORD dwInsertions, dwRemovals;
    XGetDeviceChanges( XDEVICE_TYPE_GAMEPAD, &dwInsertions, &dwRemovals );

    // Loop through all gamepads
    for( DWORD i=0; i < XGetPortCount(); i++ )
    {
        // Handle removed devices.
        pGamepads[i].bRemoved = ( dwRemovals & (1<<i) ) ? TRUE : FALSE;
        if( pGamepads[i].bRemoved )
        {
            XInputClose( pGamepads[i].hDevice );
            pGamepads[i].hDevice = NULL;
        }

        // Handle inserted devices
        pGamepads[i].bInserted = ( dwInsertions & (1<<i) ) ? TRUE : FALSE;
        if( pGamepads[i].bInserted ) 
        {
            // TCR 1-14 Device Types
            pGamepads[i].hDevice = XInputOpen( XDEVICE_TYPE_GAMEPAD, i, 
                                               XDEVICE_NO_SLOT, NULL );
            XInputGetCapabilities( g_Gamepads[i].hDevice, &g_Gamepads[i].caps );
        }

        // If we have a valid device, poll it's state and track button changes
        if( pGamepads[i].hDevice )
        {
            // Read the input state
            XINPUT_STATE xiState;
            XInputGetState( pGamepads[i].hDevice, &xiState );

            // Copy gamepad to local structure
            pGamepads[i].wButtons          = xiState.Gamepad.wButtons;
            pGamepads[i].bAnalogButtons[0] = xiState.Gamepad.bAnalogButtons[0];
            pGamepads[i].bAnalogButtons[1] = xiState.Gamepad.bAnalogButtons[1];
            pGamepads[i].bAnalogButtons[2] = xiState.Gamepad.bAnalogButtons[2];
            pGamepads[i].bAnalogButtons[3] = xiState.Gamepad.bAnalogButtons[3];
            pGamepads[i].bAnalogButtons[4] = xiState.Gamepad.bAnalogButtons[4];
            pGamepads[i].bAnalogButtons[5] = xiState.Gamepad.bAnalogButtons[5];
            pGamepads[i].bAnalogButtons[6] = xiState.Gamepad.bAnalogButtons[6];
            pGamepads[i].bAnalogButtons[7] = xiState.Gamepad.bAnalogButtons[7];
            pGamepads[i].sThumbLX          = xiState.Gamepad.sThumbLX;
            pGamepads[i].sThumbLY          = xiState.Gamepad.sThumbLY;
            pGamepads[i].sThumbRX          = xiState.Gamepad.sThumbRX;
            pGamepads[i].sThumbRY          = xiState.Gamepad.sThumbRY;

            // Put Xbox device input for the gamepad into our custom format
            pGamepads[i].fX1 = (pGamepads[i].sThumbLX+0.5f)/32767.5f;
            if( fabsf(pGamepads[i].fX1) < XBINPUT_DEADZONE )
                pGamepads[i].fX1 = 0.0f;

            pGamepads[i].fY1 = (pGamepads[i].sThumbLY+0.5f)/32767.5f;
            if( fabsf(pGamepads[i].fY1) < XBINPUT_DEADZONE )
                pGamepads[i].fY1 = 0.0f;

            pGamepads[i].fX2 = (pGamepads[i].sThumbRX+0.5f)/32767.5f;
            if( fabsf(pGamepads[i].fX2) < XBINPUT_DEADZONE )
                pGamepads[i].fX2 = 0.0f;

            pGamepads[i].fY2 = (pGamepads[i].sThumbRY+0.5f)/32767.5f;
            if( fabsf(pGamepads[i].fY2) < XBINPUT_DEADZONE ) 
                pGamepads[i].fY2 = 0.0f;

            // Get the boolean buttons that have been pressed since the last
            // call. Each button is represented by one bit.
            pGamepads[i].wPressedButtons = ( pGamepads[i].wLastButtons ^ pGamepads[i].wButtons ) & pGamepads[i].wButtons;
            pGamepads[i].wLastButtons    = pGamepads[i].wButtons;

            // Get the analog buttons that have been pressed since the last
            // call. Here, we considered an analog button pressed (a boolean 
            // condition) if that value is >= the threshold.
            for( DWORD b=0; b<8; b++ )
            {
                // Turn the 8-bit polled value into a boolean value
                BOOL bPressed = ( pGamepads[i].bAnalogButtons[b] >= XBINPUT_BUTTONTHRESHOLD );

                if( bPressed )
                    pGamepads[i].bPressedAnalogButtons[b] = !pGamepads[i].bLastAnalogButtons[b];
                else
                    pGamepads[i].bPressedAnalogButtons[b] = FALSE;
                
                // Store the current state for the next time
                pGamepads[i].bLastAnalogButtons[b] = bPressed;
            }
        }
    }
}



//-----------------------------------------------------------------------------
// Name: XBInput_GetPrimaryController()
// Desc: The primary controller is the first controller used by a player.
//       If no controller has been used or the controller has been removed,
//       the primary controller is the controller inserted at the lowest 
//       port number. Function returns NULL if no controller is inserted.
//-----------------------------------------------------------------------------
const XBGAMEPAD* XBInput_GetPrimaryController()
{
    static INT nPrimaryController = -1;

    // If primary controller has been set and hasn't been removed, use it
    const XBGAMEPAD* pGamePad = NULL;
    if( nPrimaryController != -1 )
    {
        pGamePad = &g_Gamepads[ nPrimaryController ];
        if( pGamePad->hDevice != NULL )
            return pGamePad;
    }

    // Primary controller hasn't been set or has been removed...

    // Examine each inserted controller to see if any is being used
    INT nFirst = -1;
    for( DWORD i=0; i < XGetPortCount(); ++i )
    {
        pGamePad = &g_Gamepads[i];
        if( pGamePad->hDevice != NULL )
        {
            // Remember the lowest inserted controller ID
            if( nFirst == -1 )
                nFirst = i;

            // If any button is active, we found the primary controller
            if( XBInput_IsAnyButtonActive( pGamePad ) )
            {
                nPrimaryController = i;
                return pGamePad;
            }
        }
    }

    // No controllers are inserted
    if( nFirst == -1 )
        return NULL;

    // The primary controller hasn't been set and no controller has been
    // used yet, so return the controller on the lowest port number
    pGamePad = &g_Gamepads[ nFirst ];
    return pGamePad;
}




//-----------------------------------------------------------------------------
// Name: XBInput_IsAnyButtonActive()
// Desc: TRUE if any button depressed or any thumbstick offset on the given
//       controller.
//-----------------------------------------------------------------------------
BOOL XBInput_IsAnyButtonActive( const XBGAMEPAD* pGamePad )
{
    // Check digital buttons
    if( pGamePad->wButtons )
        return TRUE;

    // Check analog buttons
    for( DWORD i = 0; i < 8; ++i )
    {
        if( pGamePad->bAnalogButtons[ i ] )
            return TRUE;
    }

    // Check thumbsticks
    if( pGamePad->fX1 >  XBINPUT_DEADZONE ||
        pGamePad->fX1 < -XBINPUT_DEADZONE ||
        pGamePad->fY1 >  XBINPUT_DEADZONE ||
        pGamePad->fY1 < -XBINPUT_DEADZONE )
    {
        return TRUE;
    }

    if( pGamePad->fX2 >  XBINPUT_DEADZONE ||
        pGamePad->fX2 < -XBINPUT_DEADZONE ||
        pGamePad->fY2 >  XBINPUT_DEADZONE ||
        pGamePad->fY2 < -XBINPUT_DEADZONE )
    {
        return TRUE;
    }

    // Nothing active
    return FALSE;
}
