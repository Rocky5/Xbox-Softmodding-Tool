//-----------------------------------------------------------------------------
// File: XBApp.h
//
// Desc: Application class for the XBox samples.
//
// Hist: 11.01.00 - New for November XDK release
//       12.15.00 - Changes for December XDK release
//       02.19.01 - Changes for March XDK release
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------
#ifndef SHORTCUTXBE_H
#define SHORTCUTXBE_H
int file_exist (char *name);
#include "xbinput.h"
// Members to init the XINPUT devices.
XDEVICE_PREALLOC_TYPE* m_InputDeviceTypes;
DWORD                  m_dwNumInputDeviceTypes;
XBGAMEPAD*             m_Gamepad;
XBGAMEPAD              m_DefaultGamepad;
DWORD GetInput( void );
// Deadzone for the gamepad inputs
const SHORT XINPUT_DEADZONE = (SHORT)( 0.24f * FLOAT(0x7FFF) );
#endif