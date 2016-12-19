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

#include "XKControl.h"


XKControl::XKControl(string Name, LPD3DXSPRITE d3dSprite)
{
	Construct(Name, d3dSprite, 0, 0, 640, 480);
}

XKControl::XKControl(string Name, LPD3DXSPRITE d3dSprite ,int XOffset, int YOffset, int Width, int Height)
{

	Construct(Name, d3dSprite, XOffset, YOffset, Width, Height);
}

XKControl::~XKControl()
{

}


void XKControl::Construct(string Name, LPD3DXSPRITE d3dSprite ,int XOffset, int YOffset, int Width, int Height)
{
	m_BGDevice = NULL;
	m_BGSprite = NULL;
	m_BGTexture = NULL;

	m_hasFocus = FALSE;
	m_isVisible = TRUE;

	m_imageLoaded = FALSE;

	m_AlpaChannel = 0xFF;



	m_TabStop = FALSE;
	m_TabIndex = 0;

	m_Name = Name;

	m_BGSprite = d3dSprite;

	d3dSprite->GetDevice(&m_BGDevice);


	SetSize(Width, Height);
	SetPosition(XOffset, YOffset);

	Initialize();
}

void XKControl::Initialize()
{

}

void XKControl::SetVisible(BOOL isVisible)
{
	m_isVisible = isVisible;
}

BOOL XKControl::IsVisible()
{
	return m_isVisible;
}

void XKControl::SetFocus(BOOL hasFocus)
{
	m_hasFocus = hasFocus;
}

BOOL XKControl::HasFocus()
{
	return m_hasFocus;
}


void XKControl::SetTabStop(BOOL TabStop)
{
	m_TabStop = TabStop;
}

BOOL XKControl::GetTabStop()
{
	return m_TabStop;
}

void XKControl::SetTabIndex(INT TabIndex)
{
	m_TabIndex = TabIndex;
}

INT XKControl::GetTabIndex()
{
	return m_TabIndex;
}

BOOL XKControl::IsImageLoaded()
{
	return m_imageLoaded;
}

BOOL XKControl::LoadImage(LPCSTR ImageFileName)
{
	BOOL retVal = m_imageLoaded = FALSE;
	WIN32_FIND_DATA wfd;

	HANDLE hf = FindFirstFile(ImageFileName, &wfd);
	if (hf != INVALID_HANDLE_VALUE)
	{
		//Load and create texture from image file..
		if (D3DXCreateTextureFromFile (m_BGDevice, ImageFileName, &m_BGTexture) == D3D_OK)
		{
			retVal = m_imageLoaded = TRUE;
			SetSize(m_Width, m_Height);

		}
		
	}
	return retVal;
}


BOOL XKControl::LoadTransparentImage(LPCSTR ImageFileName, D3DCOLOR AGRBTransparentColor)
{
	BOOL retVal = m_imageLoaded = FALSE;
	WIN32_FIND_DATA wfd;

	if (m_BGTexture != NULL)
	{
		m_BGTexture->Release();
	}


	HANDLE hf = FindFirstFile(ImageFileName, &wfd);
	if (hf != INVALID_HANDLE_VALUE)
	{
		//Load and create texture from image file..
		if (D3DXCreateTextureFromFileEx(m_BGDevice, ImageFileName,D3DX_DEFAULT,
										D3DX_DEFAULT,D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, 
										D3DPOOL_DEFAULT, D3DX_DEFAULT,D3DX_DEFAULT,
										AGRBTransparentColor, //Color to make transparent. Dont forget to place FF for the alpha channel
										NULL, NULL, &m_BGTexture) == D3D_OK)
		{
			retVal = m_imageLoaded = TRUE;
			SetSize(m_Width, m_Height);
		}
		
	}
	return retVal;
}

void XKControl::SetSize(int Width, int Height)
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
}

void XKControl::SetPosition(int XOffset, int YOffset)
{
	//set coordinates..
	m_BGLocation.x = (float) XOffset;
	m_BGLocation.y = (float) YOffset;
}

void XKControl::GetPosition(LPINT pXOffset, LPINT pYOffset)
{
	if (pXOffset != NULL)
		*pXOffset = (int)m_BGLocation.x;
	if (pYOffset != NULL)
		*pYOffset = (int)m_BGLocation.y;
}

void XKControl::GetSize(LPINT pWidth, LPINT pHeight)
{
	if (pWidth != NULL)
		*pWidth = m_Width;
	if (pHeight != NULL)
		*pHeight = m_Height;
}

void XKControl::Begin()
{
	if (m_imageLoaded)
	{
		m_BGSprite->Begin();
	}
}

void XKControl::End()
{
	if (m_imageLoaded)
	{
		m_BGSprite->End();
	}
}

void XKControl::SetAlphaChannel(UCHAR AlphaChannel)
{
	m_AlpaChannel = AlphaChannel;
}

UCHAR XKControl::GetAlphaChannel()
{
	return m_AlpaChannel;
}

string	XKControl::GetName()
{
	return m_Name;
}

void XKControl::SetText(string Text)
{

}

string XKControl::GetText()
{
	return m_Name;
}

void XKControl::Render()
{
	if (m_imageLoaded && m_isVisible)
	{
		m_BGSprite->Draw(m_BGTexture,NULL,&m_BGScaling,NULL,0.0f ,&m_BGLocation,D3DCOLOR_ARGB(m_AlpaChannel, 0xff, 0xff, 0xff));
	}
}
