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

#include "XKConsole.h"


XKConsole::XKConsole()
{
	Initialize();
	InitBuffers();
}

XKConsole::XKConsole(LPDIRECT3DDEVICE8 pDD3DevicePtr, long iConsoleWidth, long iConsoleHeight, long iConsoleXOffset, long iConsoleYOffset)
{
	//initialize values..
	Initialize();

	m_ConsoleHeight = iConsoleHeight;
	m_ConsoleWidth = iConsoleWidth;
	m_ConsoleXOffset = iConsoleXOffset;
	m_ConsoleYOffset = iConsoleYOffset;

	InitBuffers();
	//Get Device and Surface
	m_pD3DDevicePtr = pDD3DevicePtr;
    m_pD3DDevicePtr->GetBackBuffer(-1,D3DBACKBUFFER_TYPE_MONO,&m_pD3DSurfacePtr);
}

XKConsole::~XKConsole()
{
	//Release TextBuffers
	if (m_pConsoleTextFontPtr != NULL)
		m_pConsoleTextFontPtr->Release();

	if (m_pD3DSurfacePtr != NULL)
		m_pD3DSurfacePtr->Release();

	if (m_ppConsoleBuffer != NULL)
	{
		for (int i=0; i < m_ConsoleMaxLines; i++)
		{
			if (m_ppConsoleBuffer[i] != NULL)
				delete[] m_ppConsoleBuffer[i];

		}
		delete[] m_ppConsoleBuffer;
	}
}

void XKConsole::SetConsoleTextColor(UCHAR r, UCHAR g, UCHAR b)
{
	m_ConsoleTextColor = D3DCOLOR_XRGB(r,g,b);
}

void XKConsole::WriteLine(LPCSTR pString)
{
	if (m_ConsoleLinePos >= m_ConsoleMaxLines-1)
	{
		if (m_ppConsoleBuffer[0] != NULL)
			delete[] m_ppConsoleBuffer[0];

		for (int i=0; i < m_ConsoleMaxLines-1; i++)
		{
			m_ppConsoleBuffer[i] = m_ppConsoleBuffer[i+1];
		}
	}
	else 
		m_ConsoleLinePos++;


	if (m_ppConsoleBuffer[m_ConsoleLinePos-1] != NULL)
		delete[] m_ppConsoleBuffer[m_ConsoleLinePos-1];

	m_ppConsoleBuffer[m_ConsoleLinePos-1] = new WCHAR[strlen(pString)+1];
	ZeroMemory((LPVOID)m_ppConsoleBuffer[m_ConsoleLinePos-1],(int) strlen(pString)+1 * sizeof(WCHAR));
    MultiByteToWideChar(CP_ACP,0,pString,(int)strlen(pString),m_ppConsoleBuffer[m_ConsoleLinePos-1],(int) strlen(pString) * sizeof(WCHAR));


}

void XKConsole::Clear()
{
	ZeroMemory (m_ppConsoleBuffer, m_ConsoleMaxLines * sizeof(LPCWSTR));
	m_ConsoleLinePos = 0;
	
}

void XKConsole::Initialize()
{

	//Initialize Values
	TitleBarVisible = TRUE;
	m_pD3DDevicePtr = NULL;
	m_pD3DSurfacePtr = NULL;
	m_pConsoleTextFontPtr = NULL;
	m_ConsoleTextColor =  D3DCOLOR_XRGB(0, 0, 0);
	m_ConsoleTitle = NULL;

	m_ConsoleWidth = 640;
	m_ConsoleHeight = 480;

	m_FontWidth = 12;
	m_FontHeight = 22;
	m_ConsoleMaxLines = ((m_ConsoleHeight-m_FontHeight) / m_FontHeight)-1;
	m_ConsoleXOffset = m_FontWidth;
	m_ConsoleYOffset = m_FontHeight;

	SetTitle("XBOX Console App");


}

void XKConsole::InitBuffers()
{

	m_ConsoleMaxLines = ((m_ConsoleHeight-m_FontHeight) / m_FontHeight)-1;

	m_ppConsoleBuffer = new LPWSTR[m_ConsoleMaxLines+1]; //Add one for possible NO_Title..
	ZeroMemory (m_ppConsoleBuffer, m_ConsoleMaxLines+1 * sizeof(LPCWSTR));
	m_ConsoleLinePos = 0;
	
}

void XKConsole::Render()
{
	RenderTitleText();

	//Open Font and set color...
	XFONT_OpenDefaultFont(&m_pConsoleTextFontPtr);
	m_pConsoleTextFontPtr->SetTextColor(m_ConsoleTextColor);

	int titleAdj = (TitleBarVisible)?0:1;

	for (int i = 1; i <= m_ConsoleLinePos; i++)
	{
		m_pConsoleTextFontPtr-> TextOut(m_pD3DSurfacePtr, m_ppConsoleBuffer[i-1], -1, m_ConsoleXOffset, m_ConsoleYOffset + m_FontHeight + (i*m_FontHeight) - (titleAdj*m_FontHeight) );
	}
}

void XKConsole::SetTitle(LPCSTR sTitle)
{
	if (m_ConsoleTitle != NULL)
		delete m_ConsoleTitle;

	m_ConsoleTitle = new WCHAR[strlen(sTitle)+1];
	ZeroMemory((LPVOID)m_ConsoleTitle,(int) strlen(sTitle) * sizeof(WCHAR));
    MultiByteToWideChar(CP_ACP,0,sTitle,(int)strlen(sTitle),m_ConsoleTitle,(int) strlen(sTitle) * sizeof(WCHAR));

}

void XKConsole::RenderTitleBar()
{
	if (TitleBarVisible)
	{
		// First time around, allocate a vertex buffer
		static LPDIRECT3DVERTEXBUFFER8 g_pVB  = NULL;
		if( g_pVB == NULL )
		{
			m_pD3DDevicePtr->CreateVertexBuffer( 4*5*sizeof(FLOAT), D3DUSAGE_WRITEONLY, 
											  0L, D3DPOOL_DEFAULT, &g_pVB );
			struct BACKGROUNDVERTEX { D3DXVECTOR4 p; D3DCOLOR color; };
			BACKGROUNDVERTEX* v;
			g_pVB->Lock( 0, 0, (BYTE**)&v, 0L );
			v[0].p = D3DXVECTOR4( m_ConsoleXOffset - 5 - 0.5f,   m_ConsoleYOffset - 5 - 0.5f, 1.0f, 1.0f );  v[0].color = D3DCOLOR_XRGB(0x44, 0x4E, 0x61);
			v[1].p = D3DXVECTOR4( m_ConsoleWidth-m_ConsoleXOffset + 5 - 0.5f,   m_ConsoleYOffset - 5 - 0.5f, 1.0f, 1.0f );  v[1].color = D3DCOLOR_XRGB(0x8D, 0xA5, 0xD1);
			v[2].p = D3DXVECTOR4(   m_ConsoleXOffset - 5 - 0.5f, m_ConsoleYOffset+m_FontHeight + 5  - 0.5f, 1.0f, 1.0f );  v[2].color = D3DCOLOR_XRGB(0x8D, 0xA5, 0xD1);
			v[3].p = D3DXVECTOR4( m_ConsoleWidth-m_ConsoleXOffset + 5 - 0.5f, m_ConsoleYOffset+m_FontHeight + 5 - 0.5f, 1.0f, 1.0f );  v[3].color = D3DCOLOR_XRGB(0x44, 0x4E, 0x61);
			g_pVB->Unlock();
		}

		// Set states
		m_pD3DDevicePtr->SetTexture(0, NULL );
		m_pD3DDevicePtr->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_DISABLE );
		m_pD3DDevicePtr->SetRenderState( D3DRS_ZENABLE, FALSE ); 
		m_pD3DDevicePtr->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE ); 
		m_pD3DDevicePtr->SetVertexShader( D3DFVF_XYZRHW|D3DFVF_DIFFUSE );
		m_pD3DDevicePtr->SetStreamSource( 0, g_pVB, 5*sizeof(FLOAT) );

		m_pD3DDevicePtr->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2 );
	}

}


void XKConsole::RenderTitleText()
{
	if (TitleBarVisible)
	{

		//Render The Console Title Bar.
		XFONT_OpenDefaultFont(&m_pConsoleTextFontPtr);
		m_pConsoleTextFontPtr->SetTextColor(D3DCOLOR_XRGB(255, 255, 255));
		m_pConsoleTextFontPtr->TextOut(m_pD3DSurfacePtr, m_ConsoleTitle, -1, (long) (m_ConsoleWidth/2 - ((wcslen(m_ConsoleTitle)/2)*m_FontWidth)) + m_ConsoleXOffset, m_ConsoleYOffset);
	}
}

void XKConsole::SetTitleBarVisible(BOOL isVisible)
{

	if (isVisible && !TitleBarVisible)
	{
		if (m_ppConsoleBuffer[m_ConsoleMaxLines])
		{
			delete[] m_ppConsoleBuffer[m_ConsoleMaxLines];
			m_ppConsoleBuffer[m_ConsoleMaxLines] = NULL;
		}

		m_ConsoleMaxLines--;
	}

	if (!isVisible && TitleBarVisible)
		m_ConsoleMaxLines++;

	TitleBarVisible = isVisible;
}
