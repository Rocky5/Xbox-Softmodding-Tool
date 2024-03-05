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

#include "XKControl_Menu.h"


XKControl_Menu::XKControl_Menu(string Name, LPD3DXSPRITE d3dSprite)
: XKControl(Name, d3dSprite)
{
	Initialize();
}

XKControl_Menu::XKControl_Menu(string Name, LPD3DXSPRITE d3dSprite ,int XOffset, int YOffset, int Width, int Height)
: XKControl(Name, d3dSprite, XOffset, YOffset, Width, Height)
{
	Initialize();

}

XKControl_Menu::~XKControl_Menu()
{

	SAFE_RELEASE(m_pRenderSurface);
	SAFE_RELEASE(m_pRenderTexture);

}


void XKControl_Menu::Initialize()
{
	m_MenuItemFont = NULL;
	m_MenuTitleFont = NULL;
	m_pRenderTexture = NULL;
	m_pRenderSurface = NULL;
	
	//Get Device Surface
	m_BGDevice->GetBackBuffer(0,D3DBACKBUFFER_TYPE_MONO ,&m_pSurface);
	//Get new texture to draw on..
	D3DXCreateTexture(m_BGDevice, m_Width, m_Height, 1, 0L, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pRenderTexture);
	//Get new Texture's Surface
	m_pRenderTexture->GetSurfaceLevel(0, &m_pRenderSurface);

	m_MenuAnimateSpeed = 20;
	m_CurrentLocation.top = 0;
	m_CurrentLocation.bottom = m_Height;
	m_CurrentLocation.right = m_Width;
	m_CurrentLocation.left = m_Width;

	m_BGTextureLocation.x = 0;
	m_BGTextureLocation.y = 0;

	m_TitleTextOffset.x = 0;
	m_TitleTextOffset.y = 0;

	m_ItemsTextOffset.x = 0;
	m_ItemsTextOffset.y = 80; //Height of Menu Title..

	
	//Set Default Menu Title Clip Region..
	m_TitleTextClipRect.x1 = 0;
	m_TitleTextClipRect.x2 = m_Width;
	m_TitleTextClipRect.y1 = 0;
	m_TitleTextClipRect.y2 = (LONG)m_ItemsTextOffset.y;

	m_TextColor = D3DCOLOR_ARGB(0xFF, 0xFF, 0xFF, 0xFF);
	m_ItemsTextColor = D3DCOLOR_ARGB(0xFF, 0xFF, 0xFF, 0xFF);
	m_ItemSelectedColor = D3DCOLOR_ARGB(0xFF, 0x00, 0x00, 0x00);

	
	m_isVisible = FALSE;
	m_Animated = TRUE;


}

void XKControl_Menu::SetTitleTextFont(CXBFont* pFont)
{
	m_MenuTitleFont = pFont;
}

void XKControl_Menu::SetItemTextFont(CXBFont* pFont)
{
	m_MenuItemFont = pFont;
}

void XKControl_Menu::SetPosition(int XOffset, int YOffset)
{

	//set coordinates..
	m_BGLocation.x = (float) XOffset;
	m_BGLocation.y = (float) YOffset;
}

void XKControl_Menu::SetTitleOffset(int XOffset, int YOffset)
{

	//set coordinates..
	m_TitleTextOffset.x = (float) XOffset;
	m_TitleTextOffset.y = (float) YOffset;

	m_TitleTextClipRect.x1 = 0;
	m_TitleTextClipRect.x2 = m_Width-(XOffset * 2);
}

void XKControl_Menu::SetTitleTextColor(D3DCOLOR TitleColor)
{
	m_TextColor = TitleColor;
}

void XKControl_Menu::SetItemsTextColor(D3DCOLOR ItemColor)
{
	m_ItemsTextColor = ItemColor;
}

void XKControl_Menu::SetItemsSelectedColor(D3DCOLOR ItemSelectedColor)
{
	m_ItemSelectedColor = ItemSelectedColor;
}

void XKControl_Menu::SetItemsOffset(int XOffset, int YOffset)
{

	//set coordinates..
	m_ItemsTextOffset.x = (float) XOffset;
	m_ItemsTextOffset.y = (float) YOffset;


	//Set Default Menu Title Clip Region..
	m_TitleTextClipRect.y1 = 0;
	m_TitleTextClipRect.y2 = YOffset;

}

void XKControl_Menu::SetVisible(BOOL isVisible)
{
	m_Animated = FALSE;
	m_isVisible = isVisible;
}

void XKControl_Menu::SetSize(int Width, int Height)
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


	//Reset Menu Position to Left and Hidden..
	m_isVisible = FALSE;
	m_CurrentLocation.top = 0;
	m_CurrentLocation.bottom = m_Height;
	m_CurrentLocation.right = m_Width ;
	m_CurrentLocation.left = m_Width;
	
	m_TitleTextClipRect.x1 = 0;
	m_TitleTextClipRect.x2 = m_Width;

}

/* Create a Menu Item container.. X & Y positions is relative to container... */
void XKControl_Menu::AddMenuItem(string ItemName, string ItemText)
{
	XKMenuItem tmpItem;
	tmpItem.Name = ItemName;
	tmpItem.Text = ItemText;
	tmpItem.Selected = FALSE;

	m_MenuMap[ItemName] = tmpItem;

}

/* remove a Menu Item from a container.. */
void XKControl_Menu::RemoveMenuItem(string ItemName)
{
	m_MenuMap.erase(ItemName);
}

void XKControl_Menu::SelectPreviousMenuItem()
{
	int itemsCount = 0;
	//Run through all items and Select the correct Item..
	for (XKMenuItemMap::iterator mi = m_MenuMap.begin(); mi != m_MenuMap.end(); ++mi)
	{
		//get second map field, the MenuItem OBJECT...
		XKMenuItem tmpItem = (XKMenuItem) mi->second;
		
		if (tmpItem.Selected)
		{
			//Found Current Seleted Item, Now select Next one..
			if (itemsCount > 0)
				--mi;

			XKMenuItem tmpItem = (XKMenuItem) mi->second;
			SetSelectedMenuItem(tmpItem.Name);

			//Skip spacers
			if (tmpItem.Text.length() < 2)
				SelectPreviousMenuItem();

			break;
		}

		itemsCount++;
	}
}

void XKControl_Menu::SelectNextMenuItem()
{
	int itemsCount = 0;
	//Run through all items and Select the correct Item..
	for (XKMenuItemMap::iterator mi = m_MenuMap.begin(); mi != m_MenuMap.end(); ++mi)
	{
		//get second map field, the MenuItem OBJECT...
		XKMenuItem tmpItem = (XKMenuItem) mi->second;
		
		if (tmpItem.Selected)
		{
			//Found Current Seleted Item, Now select Next one..
			if (itemsCount < (int)m_MenuMap.size()-1)
				++mi;

			XKMenuItem tmpItem = (XKMenuItem) mi->second;
			SetSelectedMenuItem(tmpItem.Name);

			//Skip spacers
			if (tmpItem.Text.length() < 2)
				SelectNextMenuItem();

			break;
		}

		itemsCount++;
	}

	
}

void XKControl_Menu::SetSelectedMenuItem(string ItemName)
{
	XKMenuItem* pselectedItem = NULL;

	//Run through all items and De-Select them..
	for (XKMenuItemMap::iterator mi = m_MenuMap.begin(); mi != m_MenuMap.end(); ++mi)
	{
		//get second map field, the MenuItem OBJECT...
		pselectedItem = (XKMenuItem*) &mi->second;
		if (pselectedItem != NULL)
		pselectedItem->Selected = FALSE;
	}


	//Now Select the only Item Specified
	pselectedItem = &m_MenuMap[ItemName];
	if (pselectedItem != NULL)
		pselectedItem->Selected = TRUE;
}

string XKControl_Menu::GetSelectedMenuItem()
{
	string retVal = "";

	//Run through all items and De-Select them..
	for (XKMenuItemMap::iterator mi = m_MenuMap.begin(); mi != m_MenuMap.end(); ++mi)
	{
		//get second map field, the MenuItem OBJECT...
		XKMenuItem tmpItem = (XKMenuItem) mi->second;
		
		if (tmpItem.Selected)
		{
			retVal = tmpItem.Name;
			break;
		}
	}

	return retVal;
}


void XKControl_Menu::RenderMenuItems()
{
	float tmpWidth = 0 , tmpHeight = 0 ;
	WCHAR tempstr[100];
	int	tmpStrLen = 0;
	float nextItemOffset = m_ItemsTextOffset.y  + 5; //Add 5 pix seperator space..
	float tmpX = 0;

	//Run through all items and render them..
	for (XKMenuItemMap::iterator mi = m_MenuMap.begin(); mi != m_MenuMap.end(); ++mi)
	{
		//get second map field, the MenuItem OBJECT...
		XKMenuItem tmpItem = (XKMenuItem) mi->second;
		
		//get the Unicode Equivalent.
		ZeroMemory(tempstr, sizeof(WCHAR) * 100);
		tmpStrLen = strlen(tmpItem.Text.c_str())+1;
		MultiByteToWideChar(CP_ACP, 0, tmpItem.Text.c_str(), tmpStrLen, tempstr, tmpStrLen * sizeof(WCHAR));
	
		//Get the Menu Item Text Width & Height..
		m_MenuItemFont->GetTextExtent(tempstr, &tmpWidth, &tmpHeight, TRUE);

		tmpX = ((m_Width - tmpWidth) / 2) - m_ItemsTextOffset.x;
		
		if (tmpItem.Selected)
		{
			//Draw the Selected Menu Item Text + Shadow..
			m_MenuItemFont->DrawText(tmpX+2, nextItemOffset+2, m_ItemsTextColor, tempstr);
			m_MenuItemFont->DrawText(tmpX, nextItemOffset, m_ItemSelectedColor, tempstr);
		}
		else
		{
			//Draw the Non Selected Menu Item Text + Shadow..
			m_MenuItemFont->DrawText(tmpX+2, nextItemOffset+2, m_ItemSelectedColor, tempstr);
			m_MenuItemFont->DrawText(tmpX, nextItemOffset, m_ItemsTextColor, tempstr);
		}
		nextItemOffset += tmpHeight;

	}
}


void XKControl_Menu::Render()
{
	LPWSTR tempstr = new WCHAR[strlen(m_Name.c_str())+1];
	ZeroMemory(tempstr, sizeof(WCHAR) * (strlen(m_Name.c_str())+1));
    MultiByteToWideChar(CP_ACP,0,m_Name.c_str(),(int)strlen(m_Name.c_str()),tempstr,(int)strlen(m_Name.c_str()) * sizeof(WCHAR));

	//Get the Menu Title Centered Position Relative Menu Texture
	float tmpWidth = 0 , tmpHeight = 0 ;
	m_MenuTitleFont->GetTextExtent(tempstr, &tmpWidth, &tmpHeight, TRUE);
	float tmpX = m_TitleTextOffset.x + ((m_TitleTextClipRect.x2 - m_TitleTextClipRect.x1 - tmpWidth) / 2);
	float tmpY = m_TitleTextOffset.y + ((m_TitleTextClipRect.y2 - m_TitleTextClipRect.y1 - tmpHeight) / 2);

	//Set Render Target = our off screen Menu texture, then  create menu..
	m_BGDevice->SetRenderTarget(m_pRenderSurface, NULL);
	m_BGDevice->Clear( 0L, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0x00, 0x00, 0x00, 0x00), 1.0f, 0L );
	
	if (m_imageLoaded) //Only Draw Background if available..
		m_BGSprite->Draw(m_BGTexture, NULL, &m_BGScaling,NULL, 0.0f, &m_BGTextureLocation,D3DCOLOR_ARGB(m_AlpaChannel, 0xff, 0xff, 0xff));
	
	//Draw the Menu Title Text + Black Shadow..
	m_MenuTitleFont->DrawText(tmpX+3, tmpY+3, D3DCOLOR_ARGB(0xFA, 0x00, 0x00, 0x00), tempstr);
	m_MenuTitleFont->DrawText(tmpX, tmpY, m_TextColor, tempstr);

	//Draw All the Menu Items..
	RenderMenuItems();

	//Set Render targat back to main surface and draw Menu..
	m_BGDevice->SetRenderTarget(m_pSurface, NULL);
	m_BGSprite->Draw(m_pRenderTexture, &m_CurrentLocation, NULL ,NULL,0.0f ,&m_BGLocation,D3DCOLOR_ARGB(m_AlpaChannel, 0xff, 0xff, 0xff));


	//Small little algorithm to animate menu from left to right..
	if ((m_CurrentLocation.left > 0) && m_isVisible && !m_Animated)
	{
		m_CurrentLocation.left -= m_MenuAnimateSpeed;
		if ((UINT)m_CurrentLocation.left < m_MenuAnimateSpeed)
		{
			m_CurrentLocation.left = 0;
			m_Animated = TRUE;
			m_MenuAvailable = TRUE;
		}
	}
	else if ((m_CurrentLocation.left < m_Width) && !m_isVisible && !m_Animated)
	{
		m_CurrentLocation.left += m_MenuAnimateSpeed;
		if ((UINT)(m_Width - m_CurrentLocation.left) < m_MenuAnimateSpeed)
		{
			m_CurrentLocation.left = m_Width;
			m_Animated = TRUE;
			m_isVisible = FALSE;
		}
	}	
	else if (!m_Animated)
	{
			m_CurrentLocation.left = m_Width;
			m_Animated = TRUE;
			m_isVisible = FALSE;
	}

	delete[] tempstr;
}

BOOL XKControl_Menu::IsBusy()
{
	return !m_Animated;
}