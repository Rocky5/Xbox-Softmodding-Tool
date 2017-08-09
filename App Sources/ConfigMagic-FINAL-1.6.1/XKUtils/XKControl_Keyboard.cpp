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

#include "XKControl_Keyboard.h"

XKControl_Keyboard::XKControl_Keyboard(string Name, LPD3DXSPRITE d3dSprite, CXBFont* Font)
: XKControl(Name, d3dSprite)
{
	m_pXBFont = Font;

	Initialize();
}

XKControl_Keyboard::XKControl_Keyboard(string Name, LPD3DXSPRITE d3dSprite ,CXBFont* Font, int XOffset, int YOffset, int Width, int Height)
: XKControl(Name, d3dSprite, XOffset, YOffset, Width, Height)
{
	m_pXBFont = Font;
	m_pEditTextFont = Font;

	Initialize();
}

XKControl_Keyboard::~XKControl_Keyboard()
{
	delete m_EditBox;

	//Delete All existing Keys in Keyboard..
	for (XKKeyItemMap::iterator ci = m_KeyboardItemsMap.begin(); ci != m_KeyboardItemsMap.end(); ++ci)
	{
		//get second map field, the OBJECT...
		KeyItem tmpKey = (KeyItem) ci->second;
		delete tmpKey.ItemObject;
	}
	m_KeyboardItemsMap.clear();
}


void XKControl_Keyboard::Initialize()
{
	m_KeyboardLayout = TYPE_FULL;

	m_Shift = FALSE;

	m_EditTextOffset.x = 5;
	m_EditTextOffset.y = 25;

	m_KeysPerRow = 10;
	m_KeysPerCol = 6;
	m_ControlKeys = 7;
	m_MaxCharacters = 32;

	m_EditTextColor = XKControl_TextBox::YELLOW;
	m_ItemsTextColor = XKControl_TextBox::WHITE;
	m_ItemSelectedColor = XKControl_TextBox::YELLOW;
	
	m_EditBox = new XKControl_TextBox("txtEditBox", m_BGSprite);
	m_EditBox->SetTextColor(m_EditTextColor);
	m_EditBox->SetBitmapFont(m_pXBFont);
	m_EditBox->SetTextCentered(TRUE);
	m_EditBox->SetTextShadow(TRUE);

	float tmpW=0, tmpH=0;
	m_pEditTextFont->GetTextExtent(L"X", &tmpW, &tmpH);
	m_EditBox->SetSize(m_Width, (INT) tmpH + 5);

	m_EditBox->SetPosition((INT)(m_BGLocation.x + m_EditTextOffset.x), (INT)(m_BGLocation.y + m_EditTextOffset.y));
	m_EditBox->SetCursorBlinkRate(250);

	m_ActiveControlIndex = 0;

	SetStandardKeyboardLayout(TYPE_FULL);

}

void XKControl_Keyboard::SetPosition(int XOffset, int YOffset)
{
	//set coordinates..
	m_BGLocation.x = (float) XOffset;
	m_BGLocation.y = (float) YOffset;

	m_EditBox->SetPosition((INT)(m_BGLocation.x + m_EditTextOffset.x), (INT)(m_BGLocation.y + m_EditTextOffset.y));

	SetStandardKeyboardLayout(TYPE_FULL);
}

void XKControl_Keyboard::SetSize(int Width, int Height)
{
	if (!m_imageLoaded)
	{
		//Set Scaling 1 to 1
		m_BGScaling.x = (float) 1;
		m_BGScaling.y = (float) 1;
	}
	else
	{
		D3DSURFACE_DESC desc;
		m_BGTexture->GetLevelDesc(0, &desc);
		m_BGScaling.x = (float) Width / desc.Width;
		m_BGScaling.y = (float) Height / desc.Height;
	}
		
	m_Width = Width;
	m_Height = Height;

	float tmpW=0, tmpH=0;
	m_pEditTextFont->GetTextExtent(L"X", &tmpW, &tmpH);
	m_EditBox->SetSize(m_Width, (INT) tmpH + 5);

	SetStandardKeyboardLayout(TYPE_FULL);
}

void XKControl_Keyboard::SetEditTextColor(D3DCOLOR EditTextColor)
{
	m_EditTextColor = EditTextColor;
	m_EditBox->SetTextColor(m_EditTextColor);
}

void XKControl_Keyboard::SetItemsTextColor(D3DCOLOR ItemColor)
{
	m_ItemsTextColor = ItemColor;
}

void XKControl_Keyboard::SetMaxCharacters(INT MaxChars)
{
	m_MaxCharacters = MaxChars;
}

void XKControl_Keyboard::SetItemsSelectedColor(D3DCOLOR ItemSelectedColor)
{
	m_ItemSelectedColor = ItemSelectedColor;
}

void XKControl_Keyboard::SetText(string Text)
{
	if (Text.length() > (UINT)m_MaxCharacters)
	{
		LPSTR tmpStr = new CHAR[m_MaxCharacters+1];
		ZeroMemory(tmpStr, m_MaxCharacters+1);
		strncpy(tmpStr, Text.c_str(), m_MaxCharacters);
		m_EditBox->SetText(tmpStr);
		delete[] tmpStr;
	}
	else
		m_EditBox->SetText(Text);

//	m_CursorLocation = Text.length();
}

string XKControl_Keyboard::GetText()
{
	return 	m_EditBox->GetText();
}

string XKControl_Keyboard::GetSelectedKeyText()
{
	string retVal = "";

	KeyItem tmpKey = (KeyItem) m_KeyboardItemsMap[m_ActiveControlIndex];
	retVal = tmpKey.ItemObject->GetText();

	return retVal;
}

XKControl_Keyboard::KEY_ITEM_TYPE XKControl_Keyboard::GetSelectedKeyType()
{
	KEY_ITEM_TYPE retVal;

	KeyItem tmpKey = (KeyItem) m_KeyboardItemsMap[m_ActiveControlIndex];
	retVal = tmpKey.ItemType;

	return retVal;
}

void XKControl_Keyboard::SelectNextKeyUp()
{
	INT iCurrent =  m_ActiveControlIndex;
	KeyItem tmpKey = (KeyItem) m_KeyboardItemsMap[m_ActiveControlIndex];
	tmpKey.ItemObject->SetFocus(FALSE);

	while(!tmpKey.ItemObject->HasFocus())
	{
		if ((m_ActiveControlIndex / m_KeysPerRow)+1 <=  m_KeysPerCol-2)
			m_ActiveControlIndex -= m_KeysPerRow;
		else
			m_ActiveControlIndex -=  m_ControlKeys;



		if (m_ActiveControlIndex >= 0)
		{
			
			tmpKey = (KeyItem) m_KeyboardItemsMap[m_ActiveControlIndex];
			if (tmpKey.ItemObject->GetTabStop())
				tmpKey.ItemObject->SetFocus(TRUE);
		}
		else
		{
			m_ActiveControlIndex = iCurrent;
			tmpKey = (KeyItem) m_KeyboardItemsMap[m_ActiveControlIndex];
			tmpKey.ItemObject->SetFocus(TRUE);
		};
	}

}

void XKControl_Keyboard::SelectNextKeyDown()
{

	INT itemsCount = m_KeyboardItemsMap.size();
	INT iCurrent =  m_ActiveControlIndex;

	KeyItem tmpKey = (KeyItem) m_KeyboardItemsMap[m_ActiveControlIndex];
	tmpKey.ItemObject->SetFocus(FALSE);

	while(!tmpKey.ItemObject->HasFocus())
	{
		if ((m_ActiveControlIndex / m_KeysPerRow) + 1 <=  m_KeysPerCol-3)
			m_ActiveControlIndex += m_KeysPerRow;
		else
		{
			INT tmpAdjust = 0;

			if (((m_ActiveControlIndex+m_ControlKeys) / m_KeysPerRow) + 1 <=  m_KeysPerCol-2)
			{
				tmpAdjust = m_KeysPerRow - m_ControlKeys;
				tmpAdjust = tmpAdjust - (m_ActiveControlIndex - (m_KeysPerRow * (UINT)(m_ActiveControlIndex / m_KeysPerRow)));
			}

			m_ActiveControlIndex +=  m_ControlKeys + tmpAdjust;
		}



		if (m_ActiveControlIndex < itemsCount)
		{
			
			tmpKey = (KeyItem) m_KeyboardItemsMap[m_ActiveControlIndex];
			if (tmpKey.ItemObject->GetTabStop())
				tmpKey.ItemObject->SetFocus(TRUE);
		}
		else
		{
			m_ActiveControlIndex = iCurrent;
			tmpKey = (KeyItem) m_KeyboardItemsMap[m_ActiveControlIndex];
			tmpKey.ItemObject->SetFocus(TRUE);
		};
	}

}

void XKControl_Keyboard::SelectNextKeyLeft()
{

	KeyItem tmpKey = (KeyItem) m_KeyboardItemsMap[m_ActiveControlIndex];
	tmpKey.ItemObject->SetFocus(FALSE);

	while(!tmpKey.ItemObject->HasFocus())
	{
		if (m_ActiveControlIndex > 0)
		{
			m_ActiveControlIndex--;
			tmpKey = (KeyItem) m_KeyboardItemsMap[m_ActiveControlIndex];

			if (tmpKey.ItemObject->GetTabStop())
				tmpKey.ItemObject->SetFocus(TRUE);
		}
		else
		{
			tmpKey.ItemObject->SetFocus(TRUE);
		};
	}

}

void XKControl_Keyboard::SelectNextKeyRight()
{
	int itemsCount = m_KeyboardItemsMap.size();

	KeyItem tmpKey = (KeyItem) m_KeyboardItemsMap[m_ActiveControlIndex];
	tmpKey.ItemObject->SetFocus(FALSE);

	while(!tmpKey.ItemObject->HasFocus())
	{
		if (m_ActiveControlIndex < itemsCount-1)
		{
			m_ActiveControlIndex++;
			tmpKey = (KeyItem) m_KeyboardItemsMap[m_ActiveControlIndex];

			if (tmpKey.ItemObject->GetTabStop())
				tmpKey.ItemObject->SetFocus(TRUE);
		}
		else
		{
			tmpKey.ItemObject->SetFocus(TRUE);
		};
	}
}

void XKControl_Keyboard::PressSelectedKey()
{
	KeyItem			tmpKey = (KeyItem) m_KeyboardItemsMap[m_ActiveControlIndex];
	string			CurrentString = m_EditBox->GetText();
	string			AppString; 
	INT				CursorPos = m_EditBox->GetCursorPosition();
	KEY_ITEM_TYPE	KeyType = tmpKey.ItemType;

	if (m_Shift)
		AppString = tmpKey.ShiftStringValue;
	else
		AppString = tmpKey.NormalStringValue;


	switch (KeyType)
	{
		case (NORMAL_KEY):
		case (SPECIAL_KEY):
			CurrentString.insert(CursorPos, AppString);
			CursorPos++;
			break;
		case (CTRL_BACKSPACE_KEY):
			if (((UINT)CursorPos<CurrentString.length()) && (CurrentString.length() > 0))
				CurrentString.erase(CursorPos, 1);
			else if  (CurrentString.length() > 0)
				CurrentString.erase(CursorPos-1, 1);
			break;
		case (CTRL_MOVE_LEFT_KEY):
			CursorPos--;
			break;
		case (CTRL_MOVE_RIGHT_KEY):
			CursorPos++;
			break;
		case (CTRL_SHIFT_KEY):
			m_Shift = !m_Shift;
			ValidateKeys();
			break;
	}


	SetText(CurrentString);
	m_EditBox->SetCursorPosition(CursorPos);
}


void XKControl_Keyboard::ProcessAction(INPUT_ACTION ActionType)
{
	if (m_isVisible)
	{
		switch (ActionType)
		{
			case (SHIFT_UP):
				SelectNextKeyUp();
				break;
			case (SHIFT_DOWN):
				SelectNextKeyDown();
				break;
			case (SHIFT_LEFT):
				SelectNextKeyLeft();
				break;
			case (SHIFT_RIGHT):
				SelectNextKeyRight();
				break;
			case (PRESS_KEY):
				PressSelectedKey();
				break;
		}
	}
}

void XKControl_Keyboard::SetEditTextFont(CXBFont* pFont)
{
	m_pEditTextFont = pFont;

	m_EditBox->SetBitmapFont(m_pEditTextFont);
	m_EditBox->SetTextColor(m_EditTextColor);

	float tmpW=0, tmpH=0;
	m_pEditTextFont->GetTextExtent(L"X", &tmpW, &tmpH);
	m_EditBox->SetSize(m_Width, (INT) tmpH + 5);
}

void XKControl_Keyboard::SetStandardKeyboardLayout(XKControl_Keyboard::KEYBOARD_LAYOUT Layout)
{
	m_KeyboardLayout = Layout;
	CreateKeyboard();
}

void XKControl_Keyboard::CreateKeyboard()
{
	
	KEYBOARD_LAYOUT Layout = m_KeyboardLayout;

	//Delete Old Keys..
	//Now render all the Controls in the Map...
	for (XKKeyItemMap::iterator ci = m_KeyboardItemsMap.begin(); ci != m_KeyboardItemsMap.end(); ++ci)
	{
		//get second map field, the OBJECT...
		KeyItem tmpKey = (KeyItem) ci->second;
		delete tmpKey.ItemObject;
	}
	m_KeyboardItemsMap.clear();

	float tmpW=0, tmpH=0;
	m_pXBFont->GetTextExtent(L"X", &tmpW, &tmpH);

	INT KeyWidth = (m_Width - ((INT)m_EditTextOffset.x * 2)) / (m_KeysPerRow+1);
	INT KeyHeight = (m_Height - ((INT)m_EditTextOffset.y * 3) - (INT)(tmpH+5)) / (m_KeysPerCol);

	CHAR KeyboardNormalChars[36] = REGULAR_KEYBOARD_ITEMS;
	CHAR KeyboardSpecialChars[4] = SPECIAL_KEYBOARD_ITEMS;

	INT CurrentRow = 0, CurrentCol = 0;


	//Create New Keys..
	for (UINT i = 0; i < (UINT)TYPE_FULL; i++)
	{
		CHAR sKeyVal[2];
		ZeroMemory(sKeyVal,2);
		memcpy(sKeyVal, &KeyboardNormalChars[i], 1);
			
		KeyItem tmpITem;
		tmpITem.ItemType = NORMAL_KEY;
		strlwr(sKeyVal);
		tmpITem.NormalStringDisplay = sKeyVal;
		tmpITem.NormalStringValue = sKeyVal;
		strupr(sKeyVal);
		tmpITem.ShiftStringDisplay = sKeyVal;
		tmpITem.ShiftStringValue = sKeyVal;

		INT tmpX = (INT)m_BGLocation.x + (INT)m_EditTextOffset.x + KeyWidth/2 + (CurrentCol * KeyWidth);
		INT tmpY = (INT)m_BGLocation.y + (INT)m_EditTextOffset.y + (INT)(tmpH+10) + (CurrentRow * KeyHeight);

		tmpITem.ItemObject = new XKControl_TextBox(sKeyVal, m_BGSprite, tmpX, tmpY, KeyWidth, KeyHeight);
		if (!m_Shift)
			tmpITem.ItemObject->SetText(tmpITem.NormalStringDisplay);
		else
			tmpITem.ItemObject->SetText(tmpITem.ShiftStringDisplay);

		tmpITem.ItemObject->SetBitmapFont(m_pXBFont);
		tmpITem.ItemObject->SetTextCentered(TRUE);

		if (i < (UINT)Layout)
		{
			tmpITem.ItemObject->SetTextShadow(TRUE);
			tmpITem.ItemObject->SetTabIndex(i);
			tmpITem.ItemObject->SetTabStop(TRUE);
		}
		else
		{
			tmpITem.ItemObject->SetTextShadow(FALSE);
			tmpITem.ItemObject->SetTextColor(D3DCOLOR_ARGB(0x70, 0xFF, 0xFF, 0xFF));
			tmpITem.ItemObject->SetTabStop(FALSE);
		}

		m_KeyboardItemsMap[i] = tmpITem;

		if (CurrentCol == m_KeysPerRow-1)
		{
			CurrentCol = 0;
			CurrentRow++;
		}
		else
		{
			CurrentCol++;
		}

	}

	//Create New Special Keys 
	for (UINT i = TYPE_FULL; i < TYPE_FULL+sizeof(KeyboardSpecialChars); i++)
	{
		CHAR sKeyVal[2];
		ZeroMemory(sKeyVal,2);
		memcpy(sKeyVal, &KeyboardSpecialChars[i-TYPE_FULL], 1);
			
		KeyItem tmpITem;
		tmpITem.ItemType = SPECIAL_KEY;
		strlwr(sKeyVal);
		tmpITem.NormalStringDisplay = sKeyVal;
		tmpITem.NormalStringValue = sKeyVal;
		strupr(sKeyVal);
		tmpITem.ShiftStringDisplay = sKeyVal;
		tmpITem.ShiftStringValue = sKeyVal;

		INT tmpX = (INT)m_BGLocation.x + (INT)m_EditTextOffset.x + KeyWidth/2 + (CurrentCol * KeyWidth);
		INT tmpY = (INT)m_BGLocation.y + (INT)m_EditTextOffset.y + (INT)(tmpH+5) + (CurrentRow * KeyHeight);

		tmpITem.ItemObject = new XKControl_TextBox(sKeyVal, m_BGSprite, tmpX, tmpY, KeyWidth, KeyHeight);
		
		tmpITem.ItemObject->SetText(tmpITem.NormalStringDisplay);
		tmpITem.ItemObject->SetBitmapFont(m_pXBFont);
		tmpITem.ItemObject->SetTextShadow(TRUE);
		tmpITem.ItemObject->SetTextCentered(TRUE);
		tmpITem.ItemObject->SetTabIndex(i);
		tmpITem.ItemObject->SetTabStop(TRUE);

		m_KeyboardItemsMap[i] = tmpITem;

		if (CurrentCol == m_KeysPerRow-1)
		{
			CurrentCol = 0;
			CurrentRow++;
		}
		else
		{
			CurrentCol++;
		}

	}


	//Create the Bottom Control Keys.. 
	UINT I = TYPE_FULL + sizeof(KeyboardSpecialChars);
	INT tmpX = (INT)m_BGLocation.x + (INT)m_EditTextOffset.x +  KeyWidth/4 ;
	INT tmpY = (INT)m_BGLocation.y + (INT)m_EditTextOffset.y + (INT)(tmpH+5) + (CurrentRow * KeyHeight) + KeyHeight*2;
	KeyItem tmpITem;
	LPSTR tmpString = NULL;

	//Create New Control "Shift" Key
	tmpString = "Shift";	
	m_pXBFont->GetTextExtent(L"shft", &tmpW, &tmpH);

	tmpITem.ItemType = CTRL_SHIFT_KEY;
	tmpITem.NormalStringDisplay = tmpString;
	tmpITem.NormalStringValue = " ";
	tmpITem.ShiftStringDisplay = tmpString;
	tmpITem.ShiftStringValue = " ";

	tmpITem.ItemObject = new XKControl_TextBox(tmpString, m_BGSprite, tmpX, tmpY, KeyWidth*5, KeyHeight);
	tmpITem.ItemObject->SetText(tmpITem.NormalStringDisplay);
	tmpITem.ItemObject->SetBitmapFont(m_pXBFont);
	tmpITem.ItemObject->SetTextColor(CYAN);
	tmpITem.ItemObject->SetTextCentered(FALSE);
	tmpITem.ItemObject->SetTabIndex(I);
	if (Layout < TYPE_FULL)
	{
		tmpITem.ItemObject->SetTextColor(D3DCOLOR_ARGB(0x70, 0xFF, 0xFF, 0xFF));
		tmpITem.ItemObject->SetTextShadow(FALSE);
		tmpITem.ItemObject->SetTabStop(FALSE);
	}
	else
	{
		tmpITem.ItemObject->SetTextShadow(TRUE);
		tmpITem.ItemObject->SetTabStop(TRUE);
	}

	m_KeyboardItemsMap[I++] = tmpITem;

	tmpX += (INT)tmpW + KeyWidth/2;

	//Create New Normal "Space" Key
	tmpString = "Space";	
	m_pXBFont->GetTextExtent(L"space", &tmpW, &tmpH);

	tmpITem.ItemType = SPECIAL_KEY;
	tmpITem.NormalStringDisplay = tmpString;
	tmpITem.NormalStringValue = " ";
	tmpITem.ShiftStringDisplay = tmpString;
	tmpITem.ShiftStringValue = " ";

	tmpITem.ItemObject = new XKControl_TextBox(tmpString, m_BGSprite, tmpX, tmpY, KeyWidth*5, KeyHeight);
	tmpITem.ItemObject->SetText(tmpITem.NormalStringDisplay);
	tmpITem.ItemObject->SetBitmapFont(m_pXBFont);
	tmpITem.ItemObject->SetTextColor(CYAN);
	tmpITem.ItemObject->SetTextShadow(TRUE);
	tmpITem.ItemObject->SetTextCentered(FALSE);
	tmpITem.ItemObject->SetTabIndex(I);
	tmpITem.ItemObject->SetTabStop(TRUE);
	m_KeyboardItemsMap[I++] = tmpITem;

	tmpX += (INT)tmpW + KeyWidth/2;

	//Create New Control "BackSpace" Key
	tmpString = "Del";	
	m_pXBFont->GetTextExtent(L"del", &tmpW, &tmpH);

	tmpITem.ItemType = CTRL_BACKSPACE_KEY;
	tmpITem.NormalStringDisplay = tmpString;
	tmpITem.NormalStringValue = "";
	tmpITem.ShiftStringDisplay = tmpString;
	tmpITem.ShiftStringValue = "";

	tmpITem.ItemObject = new XKControl_TextBox(tmpString, m_BGSprite, tmpX, tmpY, KeyWidth*5, KeyHeight);
	tmpITem.ItemObject->SetText(tmpITem.NormalStringDisplay);
	tmpITem.ItemObject->SetBitmapFont(m_pXBFont);
	tmpITem.ItemObject->SetTextColor(CYAN);
	tmpITem.ItemObject->SetTextShadow(TRUE);
	tmpITem.ItemObject->SetTextCentered(FALSE);
	tmpITem.ItemObject->SetTabIndex(I);
	tmpITem.ItemObject->SetTabStop(TRUE);
	m_KeyboardItemsMap[I++] = tmpITem;

	tmpX += (INT)tmpW + KeyWidth/2;

	//Create New Control "Left" Key
	tmpString = "<";	
	m_pXBFont->GetTextExtent(L"<", &tmpW, &tmpH);

	tmpITem.ItemType = CTRL_MOVE_LEFT_KEY;
	tmpITem.NormalStringDisplay = tmpString;
	tmpITem.NormalStringValue = "";
	tmpITem.ShiftStringDisplay = tmpString;
	tmpITem.ShiftStringValue = "";

	tmpITem.ItemObject = new XKControl_TextBox(tmpString, m_BGSprite, tmpX, tmpY, KeyWidth*5, KeyHeight);
	tmpITem.ItemObject->SetText(tmpITem.NormalStringDisplay);
	tmpITem.ItemObject->SetBitmapFont(m_pXBFont);
	tmpITem.ItemObject->SetTextColor(CYAN);
	tmpITem.ItemObject->SetTextShadow(TRUE);
	tmpITem.ItemObject->SetTextCentered(FALSE);
	tmpITem.ItemObject->SetTabIndex(I);
	tmpITem.ItemObject->SetTabStop(TRUE);
	m_KeyboardItemsMap[I++] = tmpITem;

	tmpX += (INT)tmpW + KeyWidth/2;
	
	//Create New Control "Right" Key
	tmpString = ">";	
	m_pXBFont->GetTextExtent(L">", &tmpW, &tmpH);

	tmpITem.ItemType = CTRL_MOVE_RIGHT_KEY;
	tmpITem.NormalStringDisplay = tmpString;
	tmpITem.NormalStringValue = "";
	tmpITem.ShiftStringDisplay = tmpString;
	tmpITem.ShiftStringValue = "";

	tmpITem.ItemObject = new XKControl_TextBox(tmpString, m_BGSprite, tmpX, tmpY, KeyWidth*5, KeyHeight);
	tmpITem.ItemObject->SetText(tmpITem.NormalStringDisplay);
	tmpITem.ItemObject->SetBitmapFont(m_pXBFont);
	tmpITem.ItemObject->SetTextColor(CYAN);
	tmpITem.ItemObject->SetTextShadow(TRUE);
	tmpITem.ItemObject->SetTextCentered(FALSE);
	tmpITem.ItemObject->SetTabIndex(I);
	tmpITem.ItemObject->SetTabStop(TRUE);
	m_KeyboardItemsMap[I++] = tmpITem;

	tmpX += (INT)tmpW + KeyWidth/2;
	
	//Create New Control "Done" Key
	tmpString = "OK";	
	m_pXBFont->GetTextExtent(L"ok", &tmpW, &tmpH);

	tmpITem.ItemType = CTRL_ENTER_KEY;
	tmpITem.NormalStringDisplay = tmpString;
	tmpITem.NormalStringValue = "";
	tmpITem.ShiftStringDisplay = tmpString;
	tmpITem.ShiftStringValue = "";

	tmpITem.ItemObject = new XKControl_TextBox(tmpString, m_BGSprite, tmpX, tmpY, KeyWidth*5, KeyHeight);
	tmpITem.ItemObject->SetText(tmpITem.NormalStringDisplay);
	tmpITem.ItemObject->SetBitmapFont(m_pXBFont);
	tmpITem.ItemObject->SetTextColor(GREEN);
	tmpITem.ItemObject->SetTextShadow(TRUE);
	tmpITem.ItemObject->SetTextCentered(FALSE);
	tmpITem.ItemObject->SetTabIndex(I);
	tmpITem.ItemObject->SetTabStop(TRUE);
	m_KeyboardItemsMap[I++] = tmpITem;

	tmpX += (INT)tmpW + KeyWidth/2;
	
	//Create New Control "Cancel" Key
	tmpString = "Cancel";	
	m_pXBFont->GetTextExtent(L"cancel", &tmpW, &tmpH);

	tmpITem.ItemType = CTRL_CANCEL_KEY;
	tmpITem.NormalStringDisplay = tmpString;
	tmpITem.NormalStringValue = "";
	tmpITem.ShiftStringDisplay = tmpString;
	tmpITem.ShiftStringValue = "";

	tmpITem.ItemObject = new XKControl_TextBox(tmpString, m_BGSprite, tmpX, tmpY, KeyWidth*5, KeyHeight);
	tmpITem.ItemObject->SetText(tmpITem.NormalStringDisplay);
	tmpITem.ItemObject->SetBitmapFont(m_pXBFont);
	tmpITem.ItemObject->SetTextColor(RED);
	tmpITem.ItemObject->SetTextShadow(TRUE);
	tmpITem.ItemObject->SetTextCentered(FALSE);
	tmpITem.ItemObject->SetTabIndex(I);
	tmpITem.ItemObject->SetTabStop(TRUE);
	m_KeyboardItemsMap[I] = tmpITem;

	//Set Focus on Cancel
	m_ActiveControlIndex = I;
	tmpITem.ItemObject->SetFocus(TRUE);
}

void XKControl_Keyboard::ValidateKeys()
{
	//Shift All existing Keys in Keyboard..
	for (XKKeyItemMap::iterator ci = m_KeyboardItemsMap.begin(); ci != m_KeyboardItemsMap.end(); ++ci)
	{
		//get second map field, the OBJECT...
		KeyItem tmpKey = (KeyItem) ci->second;
		if (!m_Shift)
			tmpKey.ItemObject->SetText(tmpKey.NormalStringDisplay);
		else
			tmpKey.ItemObject->SetText(tmpKey.ShiftStringDisplay);
	}

}

void XKControl_Keyboard::Render()
{
	if (m_isVisible)
	{
		if (m_imageLoaded)
			m_BGSprite->Draw(m_BGTexture,NULL,&m_BGScaling,NULL,0.0f ,&m_BGLocation,D3DCOLOR_ARGB(m_AlpaChannel, 0xff, 0xff, 0xff));

		//Now render all the Controls in the Map...
		for (XKKeyItemMap::iterator ci = m_KeyboardItemsMap.begin(); ci != m_KeyboardItemsMap.end(); ++ci)
		{
			//get second map field, the OBJECT...
			KeyItem tmpKey = (KeyItem) ci->second;
			tmpKey.ItemObject->Render();
		}

		m_EditBox->Render();
	}
}
