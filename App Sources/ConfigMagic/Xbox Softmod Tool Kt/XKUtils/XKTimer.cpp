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

#include "XKTimer.h"


XKTimer::XKTimer()
{

	Reset();
}


XKTimer::~XKTimer()
{

}

void XKTimer::Reset()
{
   m_StartTick = GetTickCount();
}


ULONG XKTimer::GetElapsedSeconds()
{
	if ((DWORD)GetTickCount() -  (DWORD)m_StartTick < 0)
		return 0;
	else
		return (GetTickCount() -  m_StartTick) * 1000;

}

BOOL XKTimer::HasElapsedSeconds(ULONG Seconds)
{
	BOOL retVal = FALSE;

	ULONG elapsed = GetElapsedSeconds();
	if (Seconds <= elapsed)
	{
	
		retVal = TRUE;
		Reset();
	}

	return retVal;
}

ULONG XKTimer::GetElapsedMilliseconds()
{

	if (((DWORD)GetTickCount() -  (DWORD)m_StartTick) < 0)
		return 0;
	else
		return GetTickCount() -  m_StartTick;
}


BOOL XKTimer::HasElapsedMilliSeconds(ULONG MilliSeconds)
{
	BOOL retVal = FALSE;

	ULONG elapsed = GetElapsedMilliseconds();

	if (MilliSeconds <= elapsed)
	{
		retVal = TRUE;
		Reset();
	}

	return retVal;
}
