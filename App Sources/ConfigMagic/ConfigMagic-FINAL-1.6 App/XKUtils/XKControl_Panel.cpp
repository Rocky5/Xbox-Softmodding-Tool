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

#include "XKControl_Panel.h"

XKControl_Panel::XKControl_Panel(string Name, LPD3DXSPRITE d3dSprite)
: XKControl(Name, d3dSprite)
{
	Initialize();
}

XKControl_Panel::XKControl_Panel(string Name, LPD3DXSPRITE d3dSprite ,int XOffset, int YOffset, int Width, int Height)
: XKControl(Name, d3dSprite, XOffset, YOffset, Width, Height)
{
	Initialize();
}

XKControl_Panel::~XKControl_Panel()
{

}

void XKControl_Panel::Initialize()
{
	m_ActiveControlIndex = 0;
	m_Controls_Locked = TRUE;
}

/* Create a control on a container.. X & Y positions is relative to container... */
void XKControl_Panel::AddControl(LPXKControl ControlObject, int XOffset, int YOffset, int Width, int Height)
{
	//calculate absolute position on screen...
	ControlObject->SetSize(Width, Height);
	ControlObject->SetPosition((int)m_BGLocation.x + XOffset, (int)m_BGLocation.y + YOffset);
	m_ControlMap[ControlObject->GetName()] = ControlObject;
}

/* remove a control from a container.. */
void XKControl_Panel::RemoveControl(LPXKControl ControlObject)
{
	m_ControlMap.erase(ControlObject->GetName());
}

LPXKControl	XKControl_Panel::GetControl(string ControlName)
{
	return (LPXKControl) m_ControlMap[ControlName];
}

void XKControl_Panel::SetPosition(int XOffset, int YOffset)
{
	//Now reset locations for all the Controls in the Map...
	for (XKControlMap::iterator ci = m_ControlMap.begin(); ci != m_ControlMap.end(); ++ci)
	{
		int cx = 0, cy = 0;

		//get second map field, the OBJECT...
		LPXKControl control = (LPXKControl) ci->second;
		control->GetPosition(&cx, &cy);
		
		//calculate control current relative position to container...
		cx -= (int)m_BGLocation.x; 
		cy -= (int)m_BGLocation.y; 
	
		//set controls new absolute postion to screen
		control->SetPosition(XOffset + cx, YOffset + cy);
	}

	//set container new position..
	m_BGLocation.x = (float) XOffset;
	m_BGLocation.y = (float) YOffset;
}

void XKControl_Panel::FocusNextControl()
{

	//Dont Adjust focus if controls are locked..
	if (m_Controls_Locked)
		return;

	INT HigestTabIndex = 0;
	INT nextTabIndex = 0xFFFF;

	//Find Next Highest TabStop, then set focus to all controls with that tab stop..
	for (XKControlMap::iterator ci = m_ControlMap.begin(); ci != m_ControlMap.end(); ++ci)
	{
		//get second map field, the OBJECT...
		LPXKControl control = (LPXKControl) ci->second;
		control->SetFocus(FALSE);
		
		//Get the Control with next tab index
		if ((control->GetTabIndex() > m_ActiveControlIndex) && (control->GetTabIndex() < nextTabIndex))
			nextTabIndex = control->GetTabIndex();

		if (control->GetTabStop())
			HigestTabIndex = (HigestTabIndex < control->GetTabIndex())?control->GetTabIndex():HigestTabIndex;
	}


	//Make sure we dont step past the highest index number..
	m_ActiveControlIndex = (nextTabIndex > HigestTabIndex)?HigestTabIndex:nextTabIndex;


	//Set Focus to all controls with this TabIndex..
	for (XKControlMap::iterator ci = m_ControlMap.begin(); ci != m_ControlMap.end(); ++ci)
	{
		//get second map field, the OBJECT...
		LPXKControl control = (LPXKControl) ci->second;
		
		if ((control->GetTabIndex() == m_ActiveControlIndex) && (control->GetTabStop()))
			control->SetFocus(TRUE);
	}
}

void XKControl_Panel::FocusPreviousControl()
{
	//Dont Adjust focus if controls are locked..
	if (m_Controls_Locked)
		return;

	INT nextTabIndex = -1;
	INT LowestTabIndex = 0xFFFF;

	//Find Next Highest TabStop, then set focus to all controls with that tab stop..
	for (XKControlMap::iterator ci = m_ControlMap.begin(); ci != m_ControlMap.end(); ++ci)
	{
		//get second map field, the OBJECT...
		LPXKControl control = (LPXKControl) ci->second;
		control->SetFocus(FALSE);

		//Get the Control with next tab index
		if ((control->GetTabIndex() < m_ActiveControlIndex) && (control->GetTabIndex() > nextTabIndex))
			nextTabIndex = control->GetTabIndex();

		if (control->GetTabStop())
			LowestTabIndex = (LowestTabIndex > control->GetTabIndex())?control->GetTabIndex():LowestTabIndex;
	}


	//Make sure we dont step past the lowest index number..
	m_ActiveControlIndex = (nextTabIndex < LowestTabIndex)?LowestTabIndex:nextTabIndex;


	//Set Focus to all controls with this TabIndex..
	for (XKControlMap::iterator ci = m_ControlMap.begin(); ci != m_ControlMap.end(); ++ci)
	{
		//get second map field, the OBJECT...
		LPXKControl control = (LPXKControl) ci->second;
		
		if ((control->GetTabIndex() == m_ActiveControlIndex) && (control->GetTabStop()))
			control->SetFocus(TRUE);
	}
}

BOOL XKControl_Panel::IsControlsLocked()
{
	return m_Controls_Locked;
}

void XKControl_Panel::LockControls(BOOL lockControls)
{
	m_Controls_Locked = lockControls;

	if (m_Controls_Locked)
	{
	
		//set Focus to FALSE for all the Controls in the Map...
		for (XKControlMap::iterator ci = m_ControlMap.begin(); ci != m_ControlMap.end(); ++ci)
		{
			//get second map field, the OBJECT...
			LPXKControl control = (LPXKControl) ci->second;
			control->SetFocus(FALSE);
		}
	}
	else
	{
		m_ActiveControlIndex = 0;
		FocusPreviousControl();
	}
}

void XKControl_Panel::SetFocusControlText(string Text)
{
	//Set Text for all Controls in the Map that has focus..
	for (XKControlMap::iterator ci = m_ControlMap.begin(); ci != m_ControlMap.end(); ++ci)
	{
		//get second map field, the OBJECT...
		LPXKControl control = (LPXKControl) ci->second;
		if (control->HasFocus())
			control->SetText(Text);
	}

}

LPXKControl XKControl_Panel::GetFocusControl()
{
	LPXKControl retVal  = NULL;

	//First Control That Has Focus' Text..
	for (XKControlMap::iterator ci = m_ControlMap.begin(); ci != m_ControlMap.end(); ++ci)
	{
		//get second map field, the OBJECT...
		LPXKControl control = (LPXKControl) ci->second;
		if (control->HasFocus())
		{
			retVal = control;
			break;
		}
	}

	return retVal;
}

void XKControl_Panel::Render()
{
	
	//Render the Container background image..
	if (m_isVisible)
	{
		if (m_imageLoaded)
		{
			m_BGSprite->Draw(m_BGTexture,NULL,&m_BGScaling,NULL,0.0f ,&m_BGLocation,D3DCOLOR_ARGB(m_AlpaChannel, 0xff, 0xff, 0xff));
		}

		//Now render all the Controls in the Map...
		for (XKControlMap::iterator ci = m_ControlMap.begin(); ci != m_ControlMap.end(); ++ci)
		{
			//get second map field, the OBJECT...
			LPXKControl control = (LPXKControl) ci->second;
			control->Render();
		}
	}
}
