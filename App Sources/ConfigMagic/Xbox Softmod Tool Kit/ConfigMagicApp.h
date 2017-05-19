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

#pragma once

#include <xtl.h>
#include "XKBaseApp.h"
#include "XKEEPROM.h"
#include "XKHDD.h"
#include "XKCRC.h"


//#define MainConfigFileName "D:\\Config.ini"
#define MainConfigFileName ""
#define SkinConfigFileName ""

class ConfigMagicApp : public XKBaseApp  
{
private:
	
	void	WriteTXTInfoFile(LPCSTR strFilename);
	void	XBEFiles();
	void	LoadSettingsFromINI();
	
	UCHAR				m_Reboot[FILENAME_MAX];
	
	BOOL				m_EnryptedRegionValid;
	BOOL				m_XBOX_EEPROM_Current;
	
	long				XConfSections;
	XKEEPROM*			m_pXKEEPROM;
	XBOX_VERSION		m_XBOX_Version;

public:
	virtual HRESULT		Initialize();
	ConfigMagicApp();
	virtual ~ConfigMagicApp();
};