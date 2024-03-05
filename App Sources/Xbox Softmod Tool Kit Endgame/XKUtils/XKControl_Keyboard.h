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
#include "XKControl_TextBox.h"

using namespace std;


#define REGULAR_KEYBOARD_ITEMS {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'}
#define SPECIAL_KEYBOARD_ITEMS {'-', ':', ',' ,'='}

class XKControl_Keyboard : public XKControl  
{
public:

	enum INPUT_ACTION
	{
		SHIFT_UP,
		SHIFT_DOWN,
		SHIFT_LEFT,
		SHIFT_RIGHT,
		PRESS_KEY
	};

	enum KEYBOARD_LAYOUT
	{
		TYPE_DECIMAL = 10,
		TYPE_HEX = 16,
		TYPE_FULL = 36
	};	


	enum KEY_ITEM_TYPE
	{
		NORMAL_KEY,
		SPECIAL_KEY,
		SPECIAL_SPACE_KEY,
		CTRL_BACKSPACE_KEY,
		CTRL_MOVE_LEFT_KEY,
		CTRL_MOVE_RIGHT_KEY,
		CTRL_SHIFT_KEY,
		CTRL_ENTER_KEY,
		CTRL_CANCEL_KEY
	};

	struct KeyItem
	{
		KEY_ITEM_TYPE		ItemType;
		string				NormalStringDisplay;
		string				ShiftStringDisplay;
		string				NormalStringValue;
		string				ShiftStringValue;
		LPXKControl_TextBox	ItemObject;
	};

	typedef KeyItem* LPKeyItem;
	//use the Map Key as the Control TabIndex also...
	typedef map<int, KeyItem> XKKeyItemMap;

	

	
	XKControl_Keyboard(string Name, LPD3DXSPRITE d3dSprite, CXBFont* Font);
	XKControl_Keyboard(string Name, LPD3DXSPRITE d3dSprite ,CXBFont* Font, int XOffset, int YOffset, int Width, int Height);
	virtual ~XKControl_Keyboard();

	virtual void			Initialize();
	virtual	void			Render();
	virtual	void			SetPosition(int XOffset, int YOffset);
	virtual void			SetSize(int Width, int Height);
	virtual void			SetMaxCharacters(INT MaxChars);
	
	virtual	void			SetEditTextColor(D3DCOLOR EditTextColor);
	virtual void			SetItemsTextColor(D3DCOLOR ItemColor);
	virtual void			SetItemsSelectedColor(D3DCOLOR ItemSelectedColor);
	virtual	void			SetEditTextFont(CXBFont* pFont);
	virtual	void			SetText(string Text);
	virtual	string			GetText();
	virtual	string			GetSelectedKeyText();
	virtual	KEY_ITEM_TYPE	GetSelectedKeyType();
	virtual	void			PressSelectedKey();

	virtual	void			SetStandardKeyboardLayout(XKControl_Keyboard::KEYBOARD_LAYOUT Layout);

	virtual	void			ProcessAction(INPUT_ACTION ActionType);
	
protected:
	KEYBOARD_LAYOUT			m_KeyboardLayout;
	XKKeyItemMap			m_KeyboardItemsMap;

	virtual	void			CreateKeyboard();
	virtual	void			SelectNextKeyUp();
	virtual	void			SelectNextKeyDown();
	virtual	void			SelectNextKeyLeft();
	virtual	void			SelectNextKeyRight();
	virtual	void			ValidateKeys();
	BOOL					m_Shift;

	INT						m_KeysPerRow;
	INT						m_KeysPerCol;
	INT						m_ControlKeys;
	
	INT						m_MaxCharacters;


	D3DXVECTOR2				m_EditTextOffset;

	LPXKControl_TextBox		m_EditBox;

	CXBFont*				m_pXBFont;
	CXBFont*				m_pEditTextFont;
	

	D3DCOLOR				m_EditTextColor;
	D3DCOLOR				m_ItemsTextColor;
	D3DCOLOR				m_ItemSelectedColor;

	INT						m_ActiveControlIndex;
};

typedef XKControl_Keyboard* LPXKControl_Keyboard;

