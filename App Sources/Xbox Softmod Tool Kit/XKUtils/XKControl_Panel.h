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
#include <stdlib.h>
#include <string>
#include <map>

#include "xkcontrol.h"

using namespace std;

class XKControl_Panel : public XKControl
{
public:
	XKControl_Panel(string Name, LPD3DXSPRITE d3dSprite);
	XKControl_Panel(string Name, LPD3DXSPRITE d3dSprite ,int XOffset, int YOffset, int Width, int Height);
	virtual ~XKControl_Panel();

	virtual void		Initialize();

	virtual	void		AddControl(LPXKControl ControlObject, int XOffset, int YOffset, int Width, int Height);
	virtual	void		RemoveControl(LPXKControl ControlObject);
	virtual	LPXKControl	GetControl(string ControlName);

	virtual	void		Render();
	virtual	void		SetPosition(int XOffset, int YOffset);

	virtual	void		FocusNextControl();
	virtual	void		FocusPreviousControl();
	virtual	void		LockControls(BOOL lockControls);
	virtual BOOL		IsControlsLocked();
	virtual	void		SetFocusControlText(string Text);
	virtual LPXKControl GetFocusControl();

protected:

	XKControlMap	m_ControlMap;

	INT				m_ActiveControlIndex;
	BOOL			m_Controls_Locked;

};

typedef XKControl_Panel* LPXKControl_Panel;