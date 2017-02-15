//-----------------------------------------------------------------------------
// File: XBApp.cpp
//
// Desc: Application class for the XBox samples.
//
// Hist: 11.01.00 - New for November XDK release
//       12.15.00 - Changes for December XDK release
//       12.19.01 - Changes for March XDK release
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------
#include "XBApp.h"
#include <D3D8Perf.h>




//-----------------------------------------------------------------------------
// Global access to common members
//-----------------------------------------------------------------------------
CXBApplication*    g_pXBApp     = NULL;
LPDIRECT3DDEVICE8  g_pd3dDevice = NULL;

// Deadzone for the gamepad inputs
const SHORT XINPUT_DEADZONE = (SHORT)( 0.24f * FLOAT(0x7FFF) );




//-----------------------------------------------------------------------------
// Name: CXBApplication()
// Desc: Constructor
//-----------------------------------------------------------------------------
CXBApplication::CXBApplication()
{
    // Initialize member variables
    g_pXBApp          = this;

    // Direct3D variables
    m_pD3D            = NULL;
    m_pd3dDevice      = NULL;
    m_pDepthBuffer    = NULL;
    m_pBackBuffer     = NULL;

    // Variables to perform app timing
    m_bPaused         = FALSE;
    m_fTime           = 0.0f;
    m_fElapsedTime    = 0.0f;
    m_fAppTime        = 0.0f;
    m_fElapsedAppTime = 0.0f;
    m_fFPS            = 0.0f;
    m_strFrameRate[0] = L'\0';

    // Set up the presentation parameters for a double-buffered, 640x480,
    // 32-bit display using depth-stencil. Override these parameters in
    // your derived class as your app requires.
    ZeroMemory( &m_d3dpp, sizeof(m_d3dpp) );
    m_d3dpp.BackBufferWidth        = 640;
    m_d3dpp.BackBufferHeight       = 480;
    m_d3dpp.BackBufferFormat       = D3DFMT_A8R8G8B8;
    m_d3dpp.BackBufferCount        = 1;
    m_d3dpp.EnableAutoDepthStencil = TRUE;
    m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
    m_d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;

    // Specify number and type of input devices this app will be using. By
    // default, you can use 0 and NULL, which triggers XInputDevices() to
    // pre-alloc the default number and types of devices. To use chat or
    // other devices, override these variables in your derived class.
    m_dwNumInputDeviceTypes = 0;
    m_InputDeviceTypes      = NULL;
}




//-----------------------------------------------------------------------------
// Name: Create()
// Desc: Create the app
//-----------------------------------------------------------------------------
HRESULT CXBApplication::Create()
{
    HRESULT hr;

    // Create the Direct3D object
//    OUTPUT_DEBUG_STRING( "XBApp: Creating Direct3D...\n" );
    if( NULL == ( m_pD3D = Direct3DCreate8(D3D_SDK_VERSION) ) )
    {
//        OUTPUT_DEBUG_STRING( "XBApp: Unable to create Direct3D!\n" );
        return E_FAIL;
    }

    // Create the device
  //  OUTPUT_DEBUG_STRING( "XBApp: Creating the D3D device...\n" );
    if( FAILED( hr = m_pD3D->CreateDevice( 0, D3DDEVTYPE_HAL, NULL, 
                                           D3DCREATE_HARDWARE_VERTEXPROCESSING, 
                                           &m_d3dpp, &m_pd3dDevice ) ) )
    {
//        OUTPUT_DEBUG_STRING( "XBApp: Could not create D3D device!\n" );
        return hr;
    }

    // Allow global access to the device
    g_pd3dDevice = m_pd3dDevice;

    // Store pointers to the depth and back buffers
    m_pd3dDevice->GetDepthStencilSurface( &m_pDepthBuffer );
    m_pd3dDevice->GetBackBuffer( 0, 0, &m_pBackBuffer );

    // Initialize core peripheral port support. Note: If these parameters
    // are 0 and NULL, respectively, then the default number and types of 
    // controllers will be initialized.
    XInitDevices( m_dwNumInputDeviceTypes, m_InputDeviceTypes );

    // Initialize the app's device-dependent objects
//    OUTPUT_DEBUG_STRING( "XBApp: Initializing the app...\n" );
    if( FAILED( hr = Initialize() ) )
    {
//        OUTPUT_DEBUG_STRING( "XBApp: Call to Initialize() failed!\n" );
        return hr;
    }

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: Destroy()
// Desc: Cleanup objects
//-----------------------------------------------------------------------------
VOID CXBApplication::Destroy()
{
    // Perform app-specific cleanup
    Cleanup();

    // Release display objects
    SAFE_RELEASE( m_pd3dDevice );
    SAFE_RELEASE( m_pD3D );
}




//-----------------------------------------------------------------------------
// Name: Run()
// Desc: 
//-----------------------------------------------------------------------------
INT CXBApplication::Run()
{
//    OUTPUT_DEBUG_STRING( "XBApp: Running the application...\n" );

    // Get the frequency of the timer
    LARGE_INTEGER qwTicksPerSec;
    QueryPerformanceFrequency( &qwTicksPerSec );
    FLOAT fSecsPerTick = 1.0f / (FLOAT)qwTicksPerSec.QuadPart;

    // Save the start time
    LARGE_INTEGER qwTime, qwLastTime, qwElapsedTime;
    QueryPerformanceCounter( &qwTime );
    qwLastTime.QuadPart = qwTime.QuadPart;

    LARGE_INTEGER qwAppTime, qwElapsedAppTime;
    qwAppTime.QuadPart        = 0;
    qwElapsedTime.QuadPart    = 0;
    qwElapsedAppTime.QuadPart = 0;

    // Run the game loop, animating and rendering frames
    for( ;; )
    {
        //-----------------------------------------
        // Animate and render a frame
        //-----------------------------------------

        // Frame move the scene
        FrameMove();

        // Render the scene
        Render();
    }
}




//-----------------------------------------------------------------------------
// Name: RenderGradientBackground()
// Desc: Draws a gradient filled background
//-----------------------------------------------------------------------------
HRESULT CXBApplication::RenderGradientBackground( DWORD dwTopColor, 
                                                  DWORD dwBottomColor )
{
    // First time around, allocate a vertex buffer
    static LPDIRECT3DVERTEXBUFFER8 g_pVB  = NULL;
    if( g_pVB == NULL )
    {
        m_pd3dDevice->CreateVertexBuffer( 4*5*sizeof(FLOAT), D3DUSAGE_WRITEONLY, 
                                          0L, D3DPOOL_DEFAULT, &g_pVB );
        struct BACKGROUNDVERTEX { D3DXVECTOR4 p; D3DCOLOR color; };
        BACKGROUNDVERTEX* v;
        g_pVB->Lock( 0, 0, (BYTE**)&v, 0L );
        v[0].p = D3DXVECTOR4(   0 - 0.5f,   0 - 0.5f, 1.0f, 1.0f );  v[0].color = dwTopColor;
        v[1].p = D3DXVECTOR4( 640 - 0.5f,   0 - 0.5f, 1.0f, 1.0f );  v[1].color = dwTopColor;
        v[2].p = D3DXVECTOR4(   0 - 0.5f, 480 - 0.5f, 1.0f, 1.0f );  v[2].color = dwBottomColor;
        v[3].p = D3DXVECTOR4( 640 - 0.5f, 480 - 0.5f, 1.0f, 1.0f );  v[3].color = dwBottomColor;
        g_pVB->Unlock();
    }

    // Set states
    m_pd3dDevice->SetTexture( 0, NULL );
    m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_DISABLE );
    m_pd3dDevice->SetRenderState( D3DRS_ZENABLE, FALSE ); 
    m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE ); 
    m_pd3dDevice->SetVertexShader( D3DFVF_XYZRHW|D3DFVF_DIFFUSE );
    m_pd3dDevice->SetStreamSource( 0, g_pVB, 5*sizeof(FLOAT) );

    m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

    // Clear the zbuffer
    m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL, 0x00000000, 1.0f, 0L );

    return S_OK;
}




