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

#include "configmagicapp.h"
#include "xkhdd.h"
#include "xkutils.h"

#ifdef _DEBUG
    #define OUTPUT_DEBUG_STRING(s) OutputDebugStringA(s)
#else
    #define OUTPUT_DEBUG_STRING(s) (VOID)(s)
#endif


//Application entry point
void __cdecl main()
{
	OUTPUT_DEBUG_STRING("Application Started..\n");
    ConfigMagicApp xbapp;

    if( FAILED( xbapp.Create() ) )
        return;
	xbapp.SetAppTitle("XBOX Config Magic");
    xbapp.Run();



}