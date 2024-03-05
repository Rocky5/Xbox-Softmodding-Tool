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

#include "XKControl_TextBox.h"

XKControl_TextBox::XKControl_TextBox(string Name, LPD3DXSPRITE d3dSprite)
: XKControl(Name, d3dSprite)
{
	m_Width = 200;
	m_Height = 30;

	Initialize();
}

XKControl_TextBox::XKControl_TextBox(string Name, LPD3DXSPRITE d3dSprite ,int XOffset, int YOffset, int Width, int Height)
: XKControl(Name, d3dSprite, XOffset, YOffset, Width, Height)
{
	Initialize();
}


XKControl_TextBox::~XKControl_TextBox()
{
}

void XKControl_TextBox::Initialize()
{
	m_pXBFont = NULL;
	m_TextLocation.x = 0;
	m_TextLocation.y = 0;
	m_TextCentered = FALSE;
	m_ShadowEnabled = FALSE;

	m_BGDevice->GetBackBuffer(0,D3DBACKBUFFER_TYPE_MONO ,&m_pSurface);

	SetSize(m_Width,m_Height);
	SetTextOffset(5,1);

	SetTextColor(WHITE);
	SetFocusTextColor(YELLOW);
	SetText("XKTextBox");

	m_CursorPosition = GetText().length();
	m_CursorBlinkRate = 0; //Dont blink....
	m_CursorVisible = FALSE;

}

void XKControl_TextBox::SetPosition(int XOffset, int YOffset)
{

	float relativeposx = m_TextLocation.x - m_BGLocation.x;
	float relativeposy = m_TextLocation.y - m_BGLocation.y;

	//set coordinates..
	m_BGLocation.x = (float) XOffset;
	m_BGLocation.y = (float) YOffset;

	SetTextOffset((int)relativeposx, (int)relativeposy);
}

void XKControl_TextBox::SetCursorPosition(INT CursorPosition)
{
	if (CursorPosition < 0)
		m_CursorPosition = 0;
	else if ((UINT)CursorPosition > GetText().length())
		m_CursorPosition = GetText().length();
	else
		m_CursorPosition = CursorPosition;
}

INT	XKControl_TextBox::GetCursorPosition()
{
	return m_CursorPosition;
}

void XKControl_TextBox::SetCursorBlinkRate(UINT BlinkRate)
{
	m_CursorBlinkRate = BlinkRate;
}

void XKControl_TextBox::SetCursorVisible(BOOL Visible)
{
	m_CursorVisible = Visible;
}


string XKControl_TextBox::GetText()
{
	return m_Text;
}

void XKControl_TextBox::SetText(string Text)
{
	m_Text = Text;
	m_CursorPosition = m_Text.length();
	
//	LPWSTR tempstr = new WCHAR[strlen(m_Text.c_str())+1];
//	ZeroMemory(tempstr, sizeof(WCHAR) * (strlen(m_Text.c_str())+1));

//    MultiByteToWideChar(CP_ACP,0,m_Text.c_str(),(int)strlen(m_Text.c_str()),tempstr,(int)strlen(m_Text.c_str()) * sizeof(WCHAR));
}

void XKControl_TextBox::SetTextColor(D3DCOLOR color)
{
	m_TextColor = color;
}

void XKControl_TextBox::SetFocusTextColor(D3DCOLOR color)
{
	m_FocusTextColor = color;
}

void XKControl_TextBox::SetTextCentered(BOOL TextCentered)
{
	m_TextCentered = TextCentered;
}

void XKControl_TextBox::SetTextShadow(BOOL ShadowEnabled)
{
	m_ShadowEnabled = ShadowEnabled;
}


void XKControl_TextBox::SetTextOffset(INT XOffset, INT YOffset)
{
	m_TextLocation.x = m_BGLocation.x + XOffset;
	m_TextLocation.y = m_BGLocation.y + YOffset;

	m_TextClipRect.x1 = (LONG)m_TextLocation.x;
	m_TextClipRect.x2 = (LONG)m_TextLocation.x + m_Width - (XOffset*2);
	m_TextClipRect.y1 = (LONG)m_TextLocation.y;
	m_TextClipRect.y2 = (LONG)m_TextLocation.y + m_Height - (YOffset*2);

}

void XKControl_TextBox::SetSize(INT Width, INT Height)
{
	INT XOffset =  (m_TextLocation.x <= 0)?0:(INT)(m_TextLocation.x - m_BGLocation.x);
	INT YOffset =  (m_TextLocation.y <= 0)?0:(INT)(m_TextLocation.y - m_BGLocation.y);

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

	SetTextOffset(XOffset, YOffset);
}

void XKControl_TextBox::SetBitmapFont(CXBFont* pFont)
{
	m_pXBFont = pFont;
}

HRESULT XKControl_TextBox::CreateBitmapFont(LPCSTR FontFileName)
{
	HRESULT retVal = S_FALSE;

    // Create a font
    if( FAILED(m_pXBFont->Create(m_BGDevice, FontFileName)))
        retVal = S_FALSE;
	else
		retVal = S_OK;

	return retVal;
}


void XKControl_TextBox::Render()
{
	if (m_isVisible)
	{

		LPWSTR tempstr = new WCHAR[strlen(m_Text.c_str())+1];
		ZeroMemory(tempstr, sizeof(WCHAR) * (strlen(m_Text.c_str())+1));
		MultiByteToWideChar(CP_ACP,0,m_Text.c_str(),(int)strlen(m_Text.c_str()),tempstr,(int)strlen(m_Text.c_str()) * sizeof(WCHAR));

		float tmpWidth = 0 , tmpHeight = 0 ;
		long tmpBoxWidth =  m_TextClipRect.x2 - m_TextClipRect.x1;
		long chrPos = wcslen(tempstr);

		//Strip out all new lines...
		LPWSTR newline = NULL;
		do
		{
			newline = wcschr(tempstr, '\n');
			if (newline != NULL)
				memset(newline, ' ', 0x01);
		}while(newline != NULL);

		//Trim out Chars to fit text box..
		do
		{
			m_pXBFont->GetTextExtent(tempstr, &tmpWidth, &tmpHeight, TRUE);
			if (tmpBoxWidth < tmpWidth)
				memset(tempstr + chrPos, 0x00, 0x02);

			chrPos--;
		}while(tmpBoxWidth < tmpWidth);

		if (m_imageLoaded)
		{
			if (!m_hasFocus)
				m_BGSprite->Draw(m_BGTexture,NULL,&m_BGScaling,NULL,0.0f ,&m_BGLocation,D3DCOLOR_ARGB(m_AlpaChannel, 0xff, 0xff, 0xff));
			else
				m_BGSprite->Draw(m_BGTexture,NULL,&m_BGScaling,NULL,0.0f ,&m_BGLocation,D3DCOLOR_ARGB(0xFF, 0xff, 0xff, 0xff));

		}

		ULONG tmpX = (ULONG)m_TextLocation.x;
		if (m_TextCentered)
			tmpX += (ULONG)(tmpBoxWidth - tmpWidth) / 2;


		if (m_ShadowEnabled)
			m_pXBFont->DrawText((float)tmpX+2, m_TextLocation.y+2, D3DCOLOR_ARGB(0xFA, 0x00, 0x00, 0x00), tempstr);

			if (!m_hasFocus)
				m_pXBFont->DrawText((float)tmpX, m_TextLocation.y, m_TextColor, tempstr);
			else
				m_pXBFont->DrawText((float)tmpX, m_TextLocation.y, m_FocusTextColor, tempstr);

		//Draw the Cursor..
		if ((m_CursorBlinkRate > 0) && (m_BlinkerTimer.HasElapsedMilliSeconds(m_CursorBlinkRate)))
			m_CursorVisible = !m_CursorVisible;

		if (m_CursorVisible)
		{
			memset(tempstr +  m_CursorPosition, 0x00, 1);
			m_pXBFont->GetTextExtent(tempstr, &tmpWidth, &tmpHeight, TRUE);
			m_pXBFont->DrawText((float)tmpX+tmpWidth+1, m_TextLocation.y+1, BLACK, L"|");
			m_pXBFont->DrawText((float)tmpX+tmpWidth, m_TextLocation.y, WHITE, L"|");

		}

		delete[] tempstr;
	}
}
