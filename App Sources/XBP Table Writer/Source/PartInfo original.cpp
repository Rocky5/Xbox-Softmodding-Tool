// PartInfo.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "partition.h"

#include <xfont.h> //used for directX textout
#include <io.h>
#include <stdio.h> //used for swprintf
#include <stdlib.h>
#include <Xtl.h>
#include <winbase.h>
#include "Winioctl.h"
#include "Undocumented.h"
#include "arial18normal.h"
#include "lucon18bold.h"
#include "arial32bold.h"

XboxPartitionTable PartTbl, SavedPartTbl, *KernelPartTblAdd;
XboxPartitionTable BackupPartTbl =
{
	{ '*', '*', '*', '*', 'P', 'A', 'R', 'T', 'I', 'N', 'F', 'O', '*', '*', '*', '*' },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{
		{ { 'X', 'B', 'O', 'X', ' ', 'S', 'H', 'E', 'L', 'L', ' ', ' ', ' ', ' ', ' ', ' '}, PE_PARTFLAGS_IN_USE, XBOX_MUSICPART_LBA_START, XBOX_MUSICPART_LBA_SIZE, 0 },
		{ { 'X', 'B', 'O', 'X', ' ', 'D', 'A', 'T', 'A', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, PE_PARTFLAGS_IN_USE, XBOX_SYSPART_LBA_START, XBOX_SYSPART_LBA_SIZE, 0 },
		{ { 'X', 'B', 'O', 'X', ' ', 'G', 'A', 'M', 'E', ' ', 'S', 'W', 'A', 'P', ' ', '1'}, PE_PARTFLAGS_IN_USE, XBOX_SWAPPART1_LBA_START, XBOX_SWAPPART_LBA_SIZE, 0 },
		{ { 'X', 'B', 'O', 'X', ' ', 'G', 'A', 'M', 'E', ' ', 'S', 'W', 'A', 'P', ' ', '2'}, PE_PARTFLAGS_IN_USE, XBOX_SWAPPART2_LBA_START, XBOX_SWAPPART_LBA_SIZE, 0 },
		{ { 'X', 'B', 'O', 'X', ' ', 'G', 'A', 'M', 'E', ' ', 'S', 'W', 'A', 'P', ' ', '3'}, PE_PARTFLAGS_IN_USE, XBOX_SWAPPART3_LBA_START, XBOX_SWAPPART_LBA_SIZE, 0 },
		{ { 'X', 'B', 'O', 'X', ' ', 'F', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, 0, XBOX_PART6_LBA_START, 0, 0 },
		{ { 'X', 'B', 'O', 'X', ' ', 'G', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, 0, XBOX_PART6_LBA_START + 0, 0, 0 },
		{ { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, 0, 0, 0, 0 },
		{ { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, 0, 0, 0, 0 },
		{ { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, 0, 0, 0, 0 },
		{ { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, 0, 0, 0, 0 },
		{ { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, 0, 0, 0, 0 },
		{ { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, 0, 0, 0, 0 },
		{ { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, 0, 0, 0, 0 },
	}
};

ANSI_STRING partition_str[14];
char partStr[][32] = {
	"\\Device\\Harddisk0\\Partition1",
	"\\Device\\Harddisk0\\Partition2",
	"\\Device\\Harddisk0\\Partition3",
	"\\Device\\Harddisk0\\Partition4",
	"\\Device\\Harddisk0\\Partition5",
	"\\Device\\Harddisk0\\Partition6",
	"\\Device\\Harddisk0\\Partition7",
	"\\Device\\Harddisk0\\Partition8",
	"\\Device\\Harddisk0\\Partition9",
	"\\Device\\Harddisk0\\Partition10",
	"\\Device\\Harddisk0\\Partition11",
	"\\Device\\Harddisk0\\Partition12",
	"\\Device\\Harddisk0\\Partition13",
	"\\Device\\Harddisk0\\Partition14"
};

DWORD g_dwUserSectors;
DWORD g_dwGotInfo = 1;
DWORD lba48_version;

int g_iCurrentPart;
int g_iMovePart = 0;
int g_iClusterSize = 16;
int g_iFunction = 0;

wchar_t buf1[200], buf2[200], buf3[200], buf4[200],buf5[200];

typedef struct _IDE_DISK_EXTENSION {
    PDEVICE_OBJECT DeviceObject;
    PARTITION_INFORMATION PartitionInformation;
} IDE_DISK_EXTENSION, *PIDE_DISK_EXTENSION;

typedef struct _UNICODE_STRING {unsigned short Length; unsigned short MaximumLength; PSTR Buffer;} UNICODE_STRING,*PUNICODE_STRING;

extern "C"
{
	XBOXAPI LONG WINAPI HalReadSMBusValue(UCHAR devddress, UCHAR offset, UCHAR readdw, LPBYTE pdata);
	BOOL WINAPI XapiFormatFATVolumeEx( PANSI_STRING VolumePath, ULONG BytesPerCluster );
	BOOL WINAPI GetDiskFreeSpace( PANSI_STRING lpRootPathName, DWORD lpSectorsPerCluster,DWORD lpBytesPerSector,DWORD lpNumberOfFreeClusters,DWORD lpTotalNumberOfClusters);

	extern unsigned int WINAPI NtFsControlFile(HANDLE FileHandle, HANDLE Event OPTIONAL, PIO_APC_ROUTINE ApcRoutine OPTIONAL, PVOID ApcContext OPTIONAL, PIO_STATUS_BLOCK IoStatusBlock, ULONG FsControlCode, PVOID InputBuffer OPTIONAL, ULONG InputBufferLength, PVOID OutputBuffer OPTIONAL, ULONG OutputBufferLength);
	extern unsigned int oCreateDevice( PDRIVER_OBJECT DriverObject, ULONG DeviceExtensionSize, PANSI_STRING DeviceName, DEVICE_TYPE DeviceType, BOOLEAN Exclusive, PDEVICE_OBJECT *DeviceObject );
}

float SCREEN_SIZE_X = 640;
float SCREEN_SIZE_Y = 480;

LPDIRECT3D8 g_pD3D = NULL;                      // DirectX Object
LPDIRECT3DDEVICE8 g_pD3DDevice = NULL;          // Screen Object
LPDIRECT3DTEXTURE8	g_pBackTexture = NULL;	//Background Texture

struct TRANSFORMED_VERTEX
{
    float	x,y,z, rhw;
    float	tu, tv;
};

#define D3DFVF_CUSTOM_TRANSFORMED_VERTEX (D3DFVF_XYZRHW|D3DFVF_TEX1)


struct SCREENVERTEX
{
    D3DXVECTOR4 pos;   // The transformed position for the vertex point.
    DWORD       color; // The vertex color.
};
#define D3DFVF_SCREENVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)


TRANSFORMED_VERTEX back_verts[] ={
	{ 0.0f,				SCREEN_SIZE_Y,	1.0f,	1.0f,	0.0f,	1.0f },
	{ 0.0f,				0.0f,			1.0f,	1.0f,	0.0f,	0.0f },
	{ SCREEN_SIZE_X,	SCREEN_SIZE_Y,	1.0f,	1.0f,	1.0f,	1.0f },
	{ SCREEN_SIZE_X,	0.0f,			1.0f,	1.0f,	1.0f,	0.0f }
};

XFONT*      g_pLucon18BitmapFont;    // Pointer to the Arial18Normal Bitmap font
XFONT*      g_pArial18BitmapFont;    // Pointer to the Arial18Normal Bitmap font
XFONT*      g_pArial32BitmapFont;    // Pointer to the Arial18Normal Bitmap font

HANDLE		g_Controller[4];
int			aButton, aButtonVal, tButton;
short		aLThumbY;

int			g_ypos = 1;

void GetBackdrop(unsigned char **Backdrop, unsigned int *BackdropSize);
int XInputGetStateAny(int *analog, int *analog_val, int *buttons, short *l_thumb_y_val, int wait = 2);

void InitialiseD3D()
{
	unsigned char *	Backdrop;
	UINT			BackdropSize;

	//First of all, create the main D3D object. If it is created successfully we
	//should get a pointer to an IDirect3D8 interface.
	g_pD3D = Direct3DCreate8(D3D_SDK_VERSION);

	//Create a structure to hold the settings for our device
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	//Fill the structure.
	// Set fullscreen 640x480x32 mode

	d3dpp.BackBufferWidth = (UINT) SCREEN_SIZE_X;
	d3dpp.BackBufferHeight = (UINT) SCREEN_SIZE_Y;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;

	// Create one backbuffer and a zbuffer
	d3dpp.BackBufferCount = 1;

	// Set up how the backbuffer is "presented" to the frontbuffer each time
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	//Create a Direct3D device.
	g_pD3D->CreateDevice(0, D3DDEVTYPE_HAL, NULL,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &g_pD3DDevice);

	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	DWORD videoFlags = XGetVideoFlags();

	//Turn on z-buffering
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);

//	XFONT_OpenBitmapFontFromMemory(L"d:\\lucon.bmf", 32768, &g_pLucon18BitmapFont);
	XFONT_OpenBitmapFontFromMemory(lucon18bold, sizeof(lucon18bold), &g_pLucon18BitmapFont);
	XFONT_OpenBitmapFontFromMemory(arial18normal, sizeof(arial18normal), &g_pArial18BitmapFont);
	XFONT_OpenBitmapFontFromMemory(arial32bold, sizeof(arial32bold), &g_pArial32BitmapFont);

	GetBackdrop(&Backdrop, &BackdropSize);
//	D3DXCreateTextureFromFile( g_pD3DDevice, "d:\\media\\gfx\\backdrop.bmp", &g_pBackTexture);
//	D3DXCreateTextureFromFileEx(g_pD3DDevice, "d:\\media\\gfx\\backdrop.bmp", D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &g_pBackTexture);
	D3DXCreateTextureFromFileInMemory(g_pD3DDevice, Backdrop, BackdropSize, &g_pBackTexture);
}

void CleanUp()
{
	g_pLucon18BitmapFont->Release();
	g_pArial18BitmapFont->Release();
	g_pArial32BitmapFont->Release();
    g_pD3DDevice->Release();
    g_pD3D->Release();
}

//-----------------------------------------------------------------------------
// Name: DrawRect()
// Desc:
//-----------------------------------------------------------------------------
VOID DrawRect( FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2,
                            DWORD dwStartColor, DWORD dwEndColor )
{
	SCREENVERTEX v[4];
    v[0].pos = D3DXVECTOR4( x1-0.5f, y1-0.5f, 1.0f, 1.0f );  v[0].color = dwStartColor;
    v[1].pos = D3DXVECTOR4( x2-0.5f, y1-0.5f, 1.0f, 1.0f );  v[1].color = dwStartColor;
    v[2].pos = D3DXVECTOR4( x1-0.5f, y2-0.5f, 1.0f, 1.0f );  v[2].color = dwEndColor;
    v[3].pos = D3DXVECTOR4( x2-0.5f, y2-0.5f, 1.0f, 1.0f );  v[3].color = dwEndColor;

    // Render the rectangle
    g_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_DISABLE );
    g_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   TRUE );
    g_pD3DDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
    g_pD3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

//	g_pD3DDevice->SetVertexShader(D3DFVF_CUSTOM_TRANSFORMED_VERTEX);
    g_pD3DDevice->SetVertexShader( D3DFVF_SCREENVERTEX );
    g_pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, v, sizeof(SCREENVERTEX) );
}

void DisplayTableText(LPCWSTR text_string, ULONG xpos, ULONG ypos, int color = 0, int just = XFONT_LEFT)
{
	//Create some DirectX text buffers
	LPDIRECT3DSURFACE8 g_pFrontBuffer;

	//InitialiseFonts
	g_pD3DDevice->GetBackBuffer(0,D3DBACKBUFFER_TYPE_MONO,&g_pFrontBuffer);

	//Display our text.
	switch (color)
	{
	case 1:
		g_pLucon18BitmapFont->SetBkMode(XFONT_TRANSPARENT);
		g_pLucon18BitmapFont->SetTextColor(D3DCOLOR_XRGB(255,40,40));
		break;
	case 2:
		g_pLucon18BitmapFont->SetBkMode(XFONT_TRANSPARENT);
		g_pLucon18BitmapFont->SetTextColor(D3DCOLOR_XRGB(20,255,20));
		break;
	case 3:
		g_pLucon18BitmapFont->SetBkMode(XFONT_TRANSPARENT);
		g_pLucon18BitmapFont->SetTextColor(D3DCOLOR_XRGB(100,100,255));
		break;
	case 4:
		g_pLucon18BitmapFont->SetBkMode(XFONT_TRANSPARENT);
		g_pLucon18BitmapFont->SetTextColor(D3DCOLOR_XRGB(224,224,20));
		break;
	case 5:
		g_pLucon18BitmapFont->SetBkMode(XFONT_OPAQUE);
		g_pLucon18BitmapFont->SetBkColor(D3DCOLOR_XRGB(40,40,40));
		g_pLucon18BitmapFont->SetTextColor(D3DCOLOR_XRGB(255,255,255));
		break;
	case 6:
		g_pLucon18BitmapFont->SetBkMode(XFONT_OPAQUE);
		g_pLucon18BitmapFont->SetBkColor(D3DCOLOR_XRGB(40,40,40));
		g_pLucon18BitmapFont->SetTextColor(D3DCOLOR_XRGB(255,20,20));
		break;
	case 7:
		g_pLucon18BitmapFont->SetBkMode(XFONT_TRANSPARENT);
		g_pLucon18BitmapFont->SetTextColor(D3DCOLOR_XRGB(140,140,140));
		break;
	case 8:
		g_pLucon18BitmapFont->SetBkMode(XFONT_TRANSPARENT);
		g_pLucon18BitmapFont->SetTextColor(D3DCOLOR_XRGB(160,160,0));
		break;
	default:
		g_pLucon18BitmapFont->SetBkMode(XFONT_TRANSPARENT);
		g_pLucon18BitmapFont->SetTextColor(D3DCOLOR_XRGB(240,240,240));
		break;
	}

	g_pLucon18BitmapFont->SetTextAlignment( XFONT_TOP | just );
	g_pLucon18BitmapFont->TextOut( g_pFrontBuffer, text_string, -1, (long)xpos, (long)ypos );

	//Release our TextBuffers
//	g_pArial18BitmapFont->Release();
	g_pFrontBuffer->Release();

}

void DisplayText(LPCWSTR text_string, ULONG xpos, ULONG ypos, int color = 0, int just = XFONT_LEFT)
{
	//Create some DirectX text buffers
	LPDIRECT3DSURFACE8 g_pFrontBuffer;

	//InitialiseFonts
	g_pD3DDevice->GetBackBuffer(0,D3DBACKBUFFER_TYPE_MONO,&g_pFrontBuffer);

	//Display our text.
	switch (color)
	{
	case 1:
		g_pArial18BitmapFont->SetBkMode(XFONT_TRANSPARENT);
		g_pArial18BitmapFont->SetTextColor(D3DCOLOR_XRGB(255,40,40));
		break;
	case 2:
		g_pArial18BitmapFont->SetBkMode(XFONT_TRANSPARENT);
		g_pArial18BitmapFont->SetTextColor(D3DCOLOR_XRGB(20,255,20));
		break;
	case 3:
		g_pArial18BitmapFont->SetBkMode(XFONT_TRANSPARENT);
		g_pArial18BitmapFont->SetTextColor(D3DCOLOR_XRGB(100,100,255));
		break;
	case 4:
		g_pArial18BitmapFont->SetBkMode(XFONT_TRANSPARENT);
		g_pArial18BitmapFont->SetTextColor(D3DCOLOR_XRGB(224,224,20));
		break;
	case 5:
		g_pArial18BitmapFont->SetBkMode(XFONT_OPAQUE);
		g_pArial18BitmapFont->SetBkColor(D3DCOLOR_XRGB(40,40,40));
		g_pArial18BitmapFont->SetTextColor(D3DCOLOR_XRGB(255,255,255));
		break;
	case 6:
		g_pArial18BitmapFont->SetBkMode(XFONT_OPAQUE);
		g_pArial18BitmapFont->SetBkColor(D3DCOLOR_XRGB(40,40,40));
		g_pArial18BitmapFont->SetTextColor(D3DCOLOR_XRGB(255,20,20));
		break;
	case 7:
		g_pArial18BitmapFont->SetBkMode(XFONT_TRANSPARENT);
		g_pArial18BitmapFont->SetTextColor(D3DCOLOR_XRGB(140,140,140));
		break;
	case 8:
		g_pArial18BitmapFont->SetBkMode(XFONT_TRANSPARENT);
		g_pArial18BitmapFont->SetTextColor(D3DCOLOR_XRGB(160,160,0));
		break;
	default:
		g_pArial18BitmapFont->SetBkMode(XFONT_TRANSPARENT);
		g_pArial18BitmapFont->SetTextColor(D3DCOLOR_XRGB(240,240,240));
		break;
	}

	g_pArial18BitmapFont->SetTextAlignment( XFONT_TOP | just );
	g_pArial18BitmapFont->TextOut( g_pFrontBuffer, text_string, -1, (long)xpos, (long)ypos );

	//Release our TextBuffers
//	g_pArial18BitmapFont->Release();
	g_pFrontBuffer->Release();

}

void DisplayWarning()
{
	//Create some DirectX text buffers
	LPDIRECT3DSURFACE8 g_pFrontBuffer;

	//InitialiseFonts
	g_pD3DDevice->GetBackBuffer(0,D3DBACKBUFFER_TYPE_MONO,&g_pFrontBuffer);

	//Display our text.
	g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	g_pArial32BitmapFont->SetTextAlignment(XFONT_CENTER | XFONT_TOP);
	g_pArial32BitmapFont->SetTextColor(D3DCOLOR_XRGB(255,30,20));
	g_pArial32BitmapFont->TextOut( g_pFrontBuffer, L"!!!!!!!! WARNING !!!!!!!!", -1, 320, 80 );

	g_pArial18BitmapFont->SetTextColor(D3DCOLOR_XRGB(240,240,240));
	g_pArial18BitmapFont->SetTextAlignment(XFONT_LEFT | XFONT_TOP);

	if (!g_iFunction)
	{
		g_pArial18BitmapFont->TextOut( g_pFrontBuffer, L"Changed partitions will be formatted!", -1, 60, 200 );
	}
	else
	{
		swprintf(buf1, L"Partition %d will be formatted!", g_iCurrentPart+1);
		g_pArial18BitmapFont->TextOut( g_pFrontBuffer, buf1, -1, 60, 200 );
	}

	g_pArial18BitmapFont->TextOut( g_pFrontBuffer, L"Press", -1, 60, 240 );
	g_pArial18BitmapFont->SetTextColor(D3DCOLOR_XRGB(240,240,0));
	g_pArial18BitmapFont->TextOut( g_pFrontBuffer, L"Y", -1, 110, 240 );
	g_pArial18BitmapFont->SetTextColor(D3DCOLOR_XRGB(240,240,240));
	g_pArial18BitmapFont->TextOut( g_pFrontBuffer, L"to continue or any other button to abort.", -1, 128, 240 );

	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);

	//Release our TextBuffers
	g_pFrontBuffer->Release();

}

bool DisplayNoInfo()
{
	LD_LAUNCH_DASHBOARD LaunchDashData = { XLD_LAUNCH_DASHBOARD_MAIN_MENU };

	//Create some DirectX text buffers
	LPDIRECT3DSURFACE8 g_pFrontBuffer;

	//InitialiseFonts
	g_pD3DDevice->GetBackBuffer(0,D3DBACKBUFFER_TYPE_MONO,&g_pFrontBuffer);

	//Display our text.
	g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
	g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	g_pArial32BitmapFont->SetTextAlignment(XFONT_CENTER | XFONT_TOP);
	g_pArial32BitmapFont->SetTextColor(D3DCOLOR_XRGB(255,30,20));
	g_pArial32BitmapFont->TextOut( g_pFrontBuffer, L"!!!!!!!! WARNING !!!!!!!!", -1, 320, 80 );


	g_pArial18BitmapFont->SetTextColor(D3DCOLOR_XRGB(240,240,240));
	g_pArial18BitmapFont->SetTextAlignment(XFONT_LEFT | XFONT_TOP);

	g_pArial18BitmapFont->TextOut( g_pFrontBuffer, L"LBA48 info not found! Custom Partitions will not work with this BIOS!", -1, 50, 200 );
//	g_pArial18BitmapFont->TextOut( g_pFrontBuffer, L"Only single partition format and table writing enabled.", -1, 50, 220 );
	g_pArial18BitmapFont->TextOut( g_pFrontBuffer, L"Update your BIOS to one which supports at least LBA48 version 2.", -1, 50, 240 );
	g_pArial18BitmapFont->TextOut( g_pFrontBuffer, L"Press", -1, 50, 280 );
	g_pArial18BitmapFont->SetTextColor(D3DCOLOR_XRGB(240,240,0));
	g_pArial18BitmapFont->TextOut( g_pFrontBuffer, L"Y", -1, 100, 280 );
	g_pArial18BitmapFont->SetTextColor(D3DCOLOR_XRGB(240,240,240));
	g_pArial18BitmapFont->TextOut( g_pFrontBuffer, L"to continue or any other button to abort.", -1, 118, 280 );


	//Release our TextBuffers
	g_pFrontBuffer->Release();

	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);

	if (-1 != XInputGetStateAny(&aButton, &aButtonVal, &tButton, &aLThumbY, 0))
		XInputGetStateAny(&aButton, &aButtonVal, &tButton, &aLThumbY);

	XInputGetStateAny(&aButton, &aButtonVal, &tButton, &aLThumbY);

	if (aButton == 3)
		return 1;
	return 0;

	//DisplayText(L"Quitting...", 60, 20 * g_ypos++, 2);
	//CleanUp();
	//XLaunchNewImage( NULL, (LAUNCH_DATA*)&LaunchDashData );
}

unsigned int
read_lba48_info(XboxPartitionTable *p_table, XboxPartitionTable **p_table_add, DWORD *total_sectors, DWORD *version)
{
	ANSI_STRING a_file;
	OBJECT_ATTRIBUTES obj_attr;
	IO_STATUS_BLOCK io_stat_block;
	HANDLE handle;
	unsigned int stat;
	unsigned int lba48_ioctl_cmd_in_buf[100];
	unsigned int lba48_ioctl_cmd_out_buf[100];
	unsigned int geom_ioctl_cmd_in_buf[100];
	unsigned int partition_table_addr;
	DISK_GEOMETRY DiskGeometry;

	memset(p_table, 0, sizeof(XboxPartitionTable));

	RtlInitAnsiString(&a_file, "\\Device\\Harddisk0\\partition0");
	obj_attr.RootDirectory = 0;
	obj_attr.ObjectName = &a_file;
	obj_attr.Attributes = OBJ_CASE_INSENSITIVE;

	stat = NtOpenFile(&handle, (GENERIC_READ|0x00100000),
		&obj_attr, &io_stat_block, (FILE_SHARE_READ|FILE_SHARE_WRITE), 0x10);

	if (stat != STATUS_SUCCESS) {
		return stat;
	}

//////////////////// geom ////////////////////
		memset(geom_ioctl_cmd_in_buf, 0, sizeof(geom_ioctl_cmd_in_buf));
		geom_ioctl_cmd_in_buf[0] = IOCTL_SUBCMD_GET_INFO;

		stat = NtDeviceIoControlFile(handle, 0, 0, 0, &io_stat_block,
			IOCTL_DISK_GET_DRIVE_GEOMETRY,
			geom_ioctl_cmd_in_buf, sizeof(geom_ioctl_cmd_in_buf),
			&DiskGeometry, sizeof(DiskGeometry));
		if (stat == STATUS_SUCCESS)
		{
			*total_sectors = DiskGeometry.Cylinders.LowPart;
		}
		else
			*total_sectors = 0;

//////////////////// lba48 ////////////////////
		if (stat == STATUS_SUCCESS)
		{
			memset(lba48_ioctl_cmd_in_buf, 0, sizeof(lba48_ioctl_cmd_in_buf));
			lba48_ioctl_cmd_in_buf[0] = IOCTL_SUBCMD_GET_INFO;

			memset(lba48_ioctl_cmd_out_buf, 0, sizeof(lba48_ioctl_cmd_out_buf));

			stat = NtDeviceIoControlFile(handle, 0, 0, 0, &io_stat_block,
				IOCTL_CMD_LBA48_ACCESS,
				lba48_ioctl_cmd_in_buf, sizeof(lba48_ioctl_cmd_in_buf),
				lba48_ioctl_cmd_out_buf, sizeof(lba48_ioctl_cmd_out_buf));
		}
//////////////////////////////////////////////

	NtClose(handle);
	if (stat != STATUS_SUCCESS) {
		return stat;
	}

	if ((lba48_ioctl_cmd_out_buf[LBA48_GET_INFO_MAGIC1_IDX] != LBA48_GET_INFO_MAGIC1_VAL) ||
		(lba48_ioctl_cmd_out_buf[LBA48_GET_INFO_MAGIC2_IDX] != LBA48_GET_INFO_MAGIC2_VAL)) {

			return STATUS_UNSUCCESSFUL;
		}

		// this way is compatible with Paul's lba48 patched kernels
		partition_table_addr = lba48_ioctl_cmd_out_buf[LBA48_GET_INFO_LOWCODE_BASE_IDX];
		partition_table_addr += lba48_ioctl_cmd_out_buf[LBA48_GET_INFO_PART_TABLE_OFS_IDX];

		*version = lba48_ioctl_cmd_out_buf[LBA48_GET_INFO_PATCHCODE_VERSION_IDX];

		if (*version == 2)
			partition_table_addr += 0x200;

		*p_table_add = (XboxPartitionTable *) partition_table_addr;


		memcpy(p_table, (void *) partition_table_addr, sizeof(XboxPartitionTable));

		return STATUS_SUCCESS;
}
unsigned int
get_clustersize(ANSI_STRING part)
{
//	OBJECT_ATTRIBUTES ObjectAttributes;
//	IO_STATUS_BLOCK IoStatusBlock;
//	HANDLE VolumeHandle;
//	unsigned int status;
//	unsigned int geom_ioctl_cmd_in_buf[100];
//	DISK_GEOMETRY DiskGeometry;
//	DWORD cluster_size;
//
////	RtlInitAnsiString(&VolumePath, part);
////	ObjectAttributes.RootDirectory = 0;
////	ObjectAttributes.ObjectName = &VolumePath;
////	ObjectAttributes.Attributes = OBJ_CASE_INSENSITIVE;
//
//	InitializeObjectAttributes(&ObjectAttributes, part,
//        OBJ_CASE_INSENSITIVE, NULL);
//
//	status = NtOpenFile(&VolumeHandle, SYNCHRONIZE | FILE_READ_DATA |
//		FILE_WRITE_DATA, &ObjectAttributes, &IoStatusBlock, 0,
//		FILE_SYNCHRONOUS_IO_ALERT | FILE_NO_INTERMEDIATE_BUFFERING);
//
////	status = NtOpenFile(&VolumeHandle, (GENERIC_READ|0x00100000),
////		&ObjectAttributes, &IoStatusBlock, (FILE_SHARE_READ|FILE_SHARE_WRITE), 0x10);
//
//	if (status != STATUS_SUCCESS) {
//		return 0;
//	}
//
////////////////////// geom ////////////////////
//		memset(geom_ioctl_cmd_in_buf, 0, sizeof(geom_ioctl_cmd_in_buf));
//		geom_ioctl_cmd_in_buf[0] = IOCTL_SUBCMD_GET_INFO;
//
//		status = NtDeviceIoControlFile(VolumeHandle, 0, 0, 0, &IoStatusBlock,
//			IOCTL_DISK_GET_DRIVE_GEOMETRY,
//			geom_ioctl_cmd_in_buf, sizeof(geom_ioctl_cmd_in_buf),
//			&DiskGeometry, sizeof(DiskGeometry));
//		if (status == STATUS_SUCCESS)
//		{
//			cluster_size = DiskGeometry.BytesPerSector;
//		}
//		else
//			cluster_size = 0;
//
//	NtClose(VolumeHandle);
//	if (status != STATUS_SUCCESS) {
//		return 0;
//	}
//
//	return cluster_size;

	HANDLE hDisk;
	NTSTATUS status;
//	ANSI_STRING a_path;
	OBJECT_ATTRIBUTES ObjectAttributes;
	IO_STATUS_BLOCK IoStatusBlock;
	BYTE PartSector[XBOX_HD_SECTOR_SIZE*8];
	LARGE_INTEGER RWOffset;
	FATX_SUPERBLOCK fsb;

	//RtlInitAnsiString(&a_path, part);
	InitializeObjectAttributes(&ObjectAttributes, &part, OBJ_CASE_INSENSITIVE, NULL);

	RWOffset.QuadPart = 0;
	memset(PartSector, 0, sizeof(PartSector));

	status = NtOpenFile(&hDisk, SYNCHRONIZE | GENERIC_ALL, &ObjectAttributes, &IoStatusBlock, 0, FILE_SYNCHRONOUS_IO_ALERT);
	if (NT_SUCCESS(status))
		status = NtReadFile(hDisk, NULL, NULL, NULL, &IoStatusBlock, (PVOID) PartSector, sizeof(PartSector), &RWOffset);
	else
		return 0;
	NtClose(hDisk);
	memcpy (&fsb,PartSector,sizeof(PartSector));
	return fsb.ClusterSize/2;

}

int XInputGetStateAny(int *analog, int *analog_val, int *buttons, short *l_thumb_y_val, int wait)
{
	XINPUT_STATE ControllerState[4];
	int i, j, c = -1;
	*analog = -1;
	*buttons = 0;
	*l_thumb_y_val = 0;

	do
	{
		for (i = 0; i < 4; i++)
		{
			if (g_Controller[i])
			{
				XInputGetState(g_Controller[i], &ControllerState[i]);
				*buttons = ControllerState[i].Gamepad.wButtons;

				if (*buttons)
				{
					c = i;
					break;
				}

				if (!wait && (ControllerState[i].Gamepad.sThumbLY < -28672 || ControllerState[i].Gamepad.sThumbLY > 28671))
				{
					*l_thumb_y_val = ControllerState[i].Gamepad.sThumbLY;
					c = i;
					break;
				}

				for (j = 0; j < 8; j++)
				{
					if (ControllerState[i].Gamepad.bAnalogButtons[j])
					{
						*analog_val = ControllerState[i].Gamepad.bAnalogButtons[j];
						*analog = j;
						c = i;
						break;
					}
				}
			}
		}
		if (!wait)
			return c;
	} while (c == -1);

	for (;wait == 2;)
	{
		XInputGetState(g_Controller[c], &ControllerState[c]);

		if (!ControllerState[c].Gamepad.wButtons && !ControllerState[c].Gamepad.bAnalogButtons[*analog])
			break;
	}

	return c;
}

void DisplayHelp()
{

	//Create some DirectX text buffers
	LPDIRECT3DSURFACE8 m_pFrontBuffer, m_pBackBuffer;
	g_pD3DDevice->GetBackBuffer(-1,D3DBACKBUFFER_TYPE_MONO,&m_pFrontBuffer);
	g_pD3DDevice->GetBackBuffer(0,D3DBACKBUFFER_TYPE_MONO,&m_pBackBuffer);

	LD_LAUNCH_DASHBOARD LaunchDashData = { XLD_LAUNCH_DASHBOARD_MAIN_MENU };

	g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	g_ypos = 1;

	DrawRect(0,0,639,479,0xff000040,0xff000020);

	DrawRect(0,0,639,42,0xff000050,0xff000020);

	DrawRect(0,42,639,62,0xff000000,0xff000040);

	DrawRect(0,338,639,358,0xff000026,0xff000000);

	DrawRect(0,358,639,430,0xff000040,0xff000020);

	DrawRect(0,430,639,450,0xff000000,0xff000020);

	DisplayText(L"XBpartitioner Help", 319, 20 * g_ypos++, 0, XFONT_CENTER);
	g_ypos++;
	DisplayText(L"A...................................", 50, 20 * g_ypos, 2);
	DisplayText(L"cycle through standard setup schemes", 240, 20 * g_ypos++);
	DisplayText(L"B...................................", 50, 20 * g_ypos, 1);
	DisplayText(L"distribute space among extended partitions", 240, 20 * g_ypos++);
	DisplayText(L"X...................................", 50, 20 * g_ypos, 3);
	DisplayText(L"toggle change size \\ change start sector", 240, 20 * g_ypos++);
	DisplayText(L"L Trigger.......................", 50, 20 * g_ypos, 4);
	DisplayText(L"increase partition size", 240, 20 * g_ypos++);
	DisplayText(L"R Trigger.......................", 50, 20 * g_ypos, 2);
	DisplayText(L"decrease partition size", 240, 20 * g_ypos++);
	DisplayText(L"D-Pad-Left....................", 50, 20 * g_ypos, 1);
	DisplayText(L"increase partition size by 1GB", 240, 20 * g_ypos++);
	DisplayText(L"D-Pad-Right..................", 50, 20 * g_ypos, 3);
	DisplayText(L"decrease partition size by 1GB", 240, 20 * g_ypos++);
	DisplayText(L"L Thumb Stick...............", 50, 20 * g_ypos, 4);
	DisplayText(L"partition size fine control", 240, 20 * g_ypos++);
	DisplayText(L"R Thumb Button............", 50, 20 * g_ypos, 2);
	DisplayText(L"enable \\ disable current partition", 240, 20 * g_ypos++);
	DisplayText(L"L+R Thumb Buttons......", 50, 20 * g_ypos, 1);
	DisplayText(L"undo all changes since last write", 240, 20 * g_ypos++);
	DisplayText(L"Black.............................", 50, 20 * g_ypos, 3);
	DisplayText(L"erase the partition table from the disk", 240, 20 * g_ypos++);
	DisplayText(L"White.............................", 50, 20 * g_ypos, 4);
	DisplayText(L"select Start Button action", 240, 20 * g_ypos++);
	DisplayText(L"Start..............................", 50, 20 * g_ypos, 2);
	DisplayText(L"write & format changed \\ write only \\ format", 240, 20 * g_ypos++);
	g_ypos++;
	DisplayText(L"Actions which write to disk require confirmation.", 50, 4 + 20 * g_ypos++, 4);
	DisplayText(L"Press Start now to begin editing the partition table.", 50, 4 + 20 * g_ypos++, 4);
	DisplayText(L"Press any other button to quit.", 50, 4 + 20 * g_ypos++, 4);



	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);

	XInputGetStateAny(&aButton, &aButtonVal, &tButton, &aLThumbY);
	XInputGetStateAny(&aButton, &aButtonVal, &tButton, &aLThumbY);

	if (tButton & XINPUT_GAMEPAD_START)
	{
//		g_pD3DDevice->Clear(0, 0, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
//		g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
		return;
	}


	for (ULONG rect_color = 0x00000000; rect_color != 0xfe000000; rect_color += 0x02000000)
	{
		g_pD3DDevice->CopyRects(m_pFrontBuffer, 0, 0, m_pBackBuffer, 0);
		DrawRect(0, 0, 639, 479, rect_color, rect_color);
		g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
	}


	DisplayText(L"Quitting...", 50, 20 * g_ypos++, 2);
	CleanUp();
	XLaunchNewImage( NULL, (LAUNCH_DATA*)&LaunchDashData );
}

void DrawTable(XboxPartitionTable *PartTbl, ULONG TotalSectors, int g_iCurrentPart)
{
	ULONG FreeSectors = part_get_free_sectors(PartTbl, TotalSectors);
	int i, offset = 8;
	unsigned int cluster_size;

	LARGE_INTEGER sz, free_sz;
	sz.QuadPart = (ULONGLONG) TotalSectors * 512UL;
	sz.QuadPart /= 1048576;
	free_sz.QuadPart = (ULONGLONG) FreeSectors * 512UL;
	free_sz.QuadPart /= 1048576;
//	swprintf(buf1, L"Disk Size: %I64uMB - %I64uMB Free", sz, free_sz);//((ULONGLONG) TotalSectors * 512UL)) / 1000000000000UL;
//	DisplayText(buf1, 50, 20 * g_ypos++, 2);

	g_ypos = 4;
	for (i = 0; i < 5; i++)
	{
		swprintf(buf1, L"%d", i+1);
		swprintf(buf2, L"start %.08x", PartTbl->TableEntries[i].LBAStart);
		swprintf(buf3, L" size %.08x", PartTbl->TableEntries[i].LBASize);
		swprintf(buf4, L"%.2f MB", (double) ((ULONGLONG) PartTbl->TableEntries[i].LBASize * 512) / 1048576);


		if (g_iCurrentPart == i /*&& PartTbl->TableEntries[i].Flags*/)
		{
			DrawRect(48, (float) (20 * g_ypos + offset - 4), 596, (float) (20 * g_ypos + offset + 20), 0xa0404040, 0xa0000000);
			DisplayTableText(buf1, 50, 20 * g_ypos + offset, (PartTbl->TableEntries[i].Flags & PE_PARTFLAGS_IN_USE) ? 4 : 8);
			DisplayTableText(buf2, 84, 20 * g_ypos + offset, (PartTbl->TableEntries[i].Flags & PE_PARTFLAGS_IN_USE) ? (g_iMovePart ? 0 : 4) : (g_iMovePart ? 0 : 8));
			DisplayTableText(buf3, 258, 20 * g_ypos + offset, (PartTbl->TableEntries[i].Flags & PE_PARTFLAGS_IN_USE) ? (g_iMovePart ? 4 : 0) : (g_iMovePart ? 8 : 0));
			DisplayTableText(buf4, 592, 20 * g_ypos++ + offset, (PartTbl->TableEntries[i].Flags & PE_PARTFLAGS_IN_USE) ? 4 : 8, XFONT_RIGHT);
		}
		else
		{
			DisplayTableText(buf1, 50, 20 * g_ypos + offset, (PartTbl->TableEntries[i].Flags & PE_PARTFLAGS_IN_USE) ? 2 : 7);
			DisplayTableText(buf2, 84, 20 * g_ypos + offset, (PartTbl->TableEntries[i].Flags & PE_PARTFLAGS_IN_USE) ? 2 : 7);
			DisplayTableText(buf3, 258, 20 * g_ypos + offset, (PartTbl->TableEntries[i].Flags & PE_PARTFLAGS_IN_USE) ? 2 : 7);
			DisplayTableText(buf4, 592, 20 * g_ypos++ + offset, (PartTbl->TableEntries[i].Flags & PE_PARTFLAGS_IN_USE) ? 2 : 7, XFONT_RIGHT);
		}
	}

	g_ypos += 2;

	offset = 13;


	for (; i < 14; i++)
	{
		swprintf(buf1, L"%d", i+1);
		swprintf(buf2, L"start %.08x", PartTbl->TableEntries[i].LBAStart);
		swprintf(buf3, L" size %.08x", PartTbl->TableEntries[i].LBASize);
		swprintf(buf4, L"%.2f GB", (double) ((ULONGLONG) PartTbl->TableEntries[i].LBASize * 512) / 1073741824);
		cluster_size = get_clustersize(partition_str[i]);
						if (cluster_size >0)
						{
							if ((PartTbl->TableEntries[i].LBASize>=LBASIZE_512GB && cluster_size<64)||
								(PartTbl->TableEntries[i].LBASize>=LBASIZE_256GB && cluster_size<32))
									swprintf(buf5, L"ER", cluster_size);
							else
									swprintf(buf5, L"%d", cluster_size);
						}
						else
						{
							swprintf(buf5, L"");
						}

		if (g_iCurrentPart == i /*&& PartTbl->TableEntries[i].Flags*/)
		{
			DrawRect(48, (float) (20 * g_ypos + offset - 4), 596, (float) (20 * g_ypos + offset + 20), 0xa0404040, 0xa0000000);
			DisplayTableText(buf1, 50, 20 * g_ypos + offset, (PartTbl->TableEntries[i].Flags & PE_PARTFLAGS_IN_USE) ? 4 : 8);
			DisplayTableText(buf2, 84, 20 * g_ypos + offset, (PartTbl->TableEntries[i].Flags & PE_PARTFLAGS_IN_USE) ? (g_iMovePart ? 0 : 4) : (g_iMovePart ? 0 : 8));
			DisplayTableText(buf3, 258, 20 * g_ypos + offset, (PartTbl->TableEntries[i].Flags & PE_PARTFLAGS_IN_USE) ? (g_iMovePart ? 4 : 0) : (g_iMovePart ? 8 : 0));
			DisplayTableText(buf4, 592, 20 * g_ypos++ + offset, (PartTbl->TableEntries[i].Flags & PE_PARTFLAGS_IN_USE) ? 4 : 8, XFONT_RIGHT);
		}
		else
		{
			DisplayTableText(buf1, 50, 20 * g_ypos + offset, (PartTbl->TableEntries[i].Flags & PE_PARTFLAGS_IN_USE) ? 2 : 7);
			DisplayTableText(buf2, 84, 20 * g_ypos + offset, (PartTbl->TableEntries[i].Flags & PE_PARTFLAGS_IN_USE) ? 2 : 7);
			DisplayTableText(buf3, 258, 20 * g_ypos + offset, (PartTbl->TableEntries[i].Flags & PE_PARTFLAGS_IN_USE) ? 2 : 7);
		     if(cluster_size!=0 && PartTbl->TableEntries[i].LBASize>0)
				DisplayTableText(buf5, 438, 20 * g_ypos + offset, (PartTbl->TableEntries[i].Flags & PE_PARTFLAGS_IN_USE) ? 2 : 7);
			DisplayTableText(buf4, 592, 20 * g_ypos++ + offset, (PartTbl->TableEntries[i].Flags & PE_PARTFLAGS_IN_USE) ? 2 : 7, XFONT_RIGHT);
		}
	}
}


//void DrawTable(XboxPartitionTable *PartTbl, ULONG TotalSectors, int g_iCurrentPart)
//{
//	ULONG FreeSectors = part_get_free_sectors(PartTbl, TotalSectors);
//	int i, offset = 8;
//	unsigned int cluster_size;
//
//	LARGE_INTEGER sz, free_sz;
//	sz.QuadPart = (ULONGLONG) TotalSectors * 512UL;
//	sz.QuadPart /= 1048576;
//	free_sz.QuadPart = (ULONGLONG) FreeSectors * 512UL;
//	free_sz.QuadPart /= 1048576;
////	swprintf(buf1, L"Disk Size: %I64uMB - %I64uMB Free", sz, free_sz);//((ULONGLONG) TotalSectors * 512UL)) / 1000000000000UL;
////	DisplayText(buf1, 50, 20 * g_ypos++, 2);
//
//	g_ypos = 4;
//	for (i = 0; i < 5; i++)
//	{
//		swprintf(buf1, L"%d", i+1);
//		swprintf(buf2, L"start %.08x", PartTbl->TableEntries[i].LBAStart);
//		swprintf(buf3, L" size %.08x", PartTbl->TableEntries[i].LBASize);
//		swprintf(buf4, L"%.2f MB", (double) ((ULONGLONG) PartTbl->TableEntries[i].LBASize * 512) / 1048576);
//
//
//		if (g_iCurrentPart == i /*&& PartTbl->TableEntries[i].Flags*/)
//		{
//			DrawRect(48, (float) (20 * g_ypos + offset - 4), 596, (float) (20 * g_ypos + offset + 20), 0xa0404040, 0xa0000000);
//			DisplayTableText(buf1, 50, 20 * g_ypos + offset, (PartTbl->TableEntries[i].Flags & PE_PARTFLAGS_IN_USE) ? 4 : 8);
//			DisplayTableText(buf2, 84, 20 * g_ypos + offset, (PartTbl->TableEntries[i].Flags & PE_PARTFLAGS_IN_USE) ? (g_iMovePart ? 0 : 4) : (g_iMovePart ? 0 : 8));
//			DisplayTableText(buf3, 258, 20 * g_ypos + offset, (PartTbl->TableEntries[i].Flags & PE_PARTFLAGS_IN_USE) ? (g_iMovePart ? 4 : 0) : (g_iMovePart ? 8 : 0));
//			DisplayTableText(buf4, 592, 20 * g_ypos++ + offset, (PartTbl->TableEntries[i].Flags & PE_PARTFLAGS_IN_USE) ? 4 : 8, XFONT_RIGHT);
//		}
//		else
//		{
//			DisplayTableText(buf1, 50, 20 * g_ypos + offset, (PartTbl->TableEntries[i].Flags & PE_PARTFLAGS_IN_USE) ? 2 : 7);
//			DisplayTableText(buf2, 84, 20 * g_ypos + offset, (PartTbl->TableEntries[i].Flags & PE_PARTFLAGS_IN_USE) ? 2 : 7);
//			DisplayTableText(buf3, 258, 20 * g_ypos + offset, (PartTbl->TableEntries[i].Flags & PE_PARTFLAGS_IN_USE) ? 2 : 7);
//			DisplayTableText(buf4, 592, 20 * g_ypos++ + offset, (PartTbl->TableEntries[i].Flags & PE_PARTFLAGS_IN_USE) ? 2 : 7, XFONT_RIGHT);
//		}
//	}
//
//	g_ypos += 2;
//
//	offset = 13;
//
//
//	for (; i < 14; i++)
//	{
//		swprintf(buf1, L"%d", i+1);
//		swprintf(buf2, L"start %.08x", PartTbl->TableEntries[i].LBAStart);
//		swprintf(buf3, L" size %.08x", PartTbl->TableEntries[i].LBASize);
//		swprintf(buf4, L"%.2f GB", (double) ((ULONGLONG) PartTbl->TableEntries[i].LBASize * 512) / 1073741824);
//		cluster_size = get_clustersize(partition_str[i]);
//						if (cluster_size >0)
//						{
//							if ((PartTbl->TableEntries[i].LBASize>=LBASIZE_512GB && cluster_size<64)||
//								(PartTbl->TableEntries[i].LBASize>=LBASIZE_256GB && cluster_size<32))
//									swprintf(buf5, L"ERR", cluster_size);
//							else
//									swprintf(buf5, L"%dK", cluster_size);
//						}
//						else
//						{
//							swprintf(buf5, L"");
//						}
//
//		if (g_iCurrentPart == i /*&& PartTbl->TableEntries[i].Flags*/)
//		{
//			DrawRect(48, (float) (20 * g_ypos + offset - 4), 596, (float) (20 * g_ypos + offset + 20), 0xa0404040, 0xa0000000);
//			DisplayTableText(buf1, 50, 20 * g_ypos + offset, (PartTbl->TableEntries[i].Flags & PE_PARTFLAGS_IN_USE) ? 4 : 8);
//			DisplayTableText(buf2, 84, 20 * g_ypos + offset, (PartTbl->TableEntries[i].Flags & PE_PARTFLAGS_IN_USE) ? (g_iMovePart ? 0 : 4) : (g_iMovePart ? 0 : 8));
//			DisplayTableText(buf3, 258, 20 * g_ypos + offset, (PartTbl->TableEntries[i].Flags & PE_PARTFLAGS_IN_USE) ? (g_iMovePart ? 4 : 0) : (g_iMovePart ? 8 : 0));
//			DisplayTableText(buf4, 592, 20 * g_ypos++ + offset, (PartTbl->TableEntries[i].Flags & PE_PARTFLAGS_IN_USE) ? 4 : 8, XFONT_RIGHT);
//		}
//		else
//		{
//			DisplayTableText(buf1, 50, 20 * g_ypos + offset, (PartTbl->TableEntries[i].Flags & PE_PARTFLAGS_IN_USE) ? 2 : 7);
//			DisplayTableText(buf2, 84, 20 * g_ypos + offset, (PartTbl->TableEntries[i].Flags & PE_PARTFLAGS_IN_USE) ? 2 : 7);
//			DisplayTableText(buf3, 258, 20 * g_ypos + offset, (PartTbl->TableEntries[i].Flags & PE_PARTFLAGS_IN_USE) ? 2 : 7);
//		     if(cluster_size!=0 && PartTbl->TableEntries[i].LBASize>0)
//				DisplayTableText(buf5, 438, 20 * g_ypos + offset, (PartTbl->TableEntries[i].Flags & PE_PARTFLAGS_IN_USE) ? 2 : 7);
//			DisplayTableText(buf4, 592, 20 * g_ypos++ + offset, (PartTbl->TableEntries[i].Flags & PE_PARTFLAGS_IN_USE) ? 2 : 7, XFONT_RIGHT);
//		}
//	}
//}

void DrawTitle()
{
	switch (g_iFunction)
	{
	case 2:
		swprintf(buf1, L"Format (%dk clusters)", g_iClusterSize);
		DisplayText(buf1, 418, 20, 0, XFONT_LEFT);
		break;
	case 1:
		DisplayText(L"Write Table", 418, 20, 0, XFONT_LEFT);
		break;
	default:
		DisplayText(L"Write Table & Format", 418, 20, 0, XFONT_LEFT);
		break;
	}
}

void DrawInfo()
{
	switch (g_iFunction)
	{
	case 2:
		DisplayText(L"Press Start to format current partition. Press Back for help.", 60,  448, 0);
		break;
	case 1:
		DisplayText(L"Press Start to write partition table. Press Back for help.", 60,  448, 0);
		break;
	default:
		DisplayText(L"Press Start write table and format. Press Back for help.", 60,  448, 0);
		break;
	}
}

void DrawScreen(int g_iCurrentPart)
{
	g_ypos = 1;


		g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
		g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

	    g_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
		g_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
		g_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_TFACTOR);
		g_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE);
		g_pD3DDevice->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255) );

		g_pD3DDevice->SetTexture(0, (D3DBaseTexture *) g_pBackTexture);
		g_pD3DDevice->SetVertexShader(D3DFVF_CUSTOM_TRANSFORMED_VERTEX);
		g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, back_verts, sizeof(TRANSFORMED_VERTEX));


	swprintf(buf1, L"XBpartitioner 1.3 - LBA48 patch v%d", lba48_version);
	DisplayText(buf1, 52, 20, 0, XFONT_LEFT);

	DrawTitle();
	DrawTable(&PartTbl, g_dwUserSectors, g_iCurrentPart);
}

NTSTATUS write_partition_table(XboxPartitionTable *p_table)
{
	HANDLE hDisk;
	NTSTATUS status;
	ANSI_STRING a_path;
	OBJECT_ATTRIBUTES ObjectAttributes;
	IO_STATUS_BLOCK IoStatusBlock;
	BYTE PartSector[XBOX_HD_SECTOR_SIZE];
	LARGE_INTEGER RWOffset;


	RtlInitAnsiString(&a_path, "\\Device\\Harddisk0\\partition0");
	InitializeObjectAttributes(&ObjectAttributes, &a_path, OBJ_CASE_INSENSITIVE, NULL);

	RWOffset.QuadPart = 0;
	memset(PartSector, 0, sizeof(PartSector));
	if(p_table)
		memcpy(PartSector, p_table, sizeof(XboxPartitionTable));

	status = NtOpenFile(&hDisk, SYNCHRONIZE | GENERIC_ALL, &ObjectAttributes, &IoStatusBlock, 0, FILE_SYNCHRONOUS_IO_ALERT);
	if (NT_SUCCESS(status))
		status = NtWriteFile(hDisk, NULL, NULL, NULL, &IoStatusBlock, (PVOID) PartSector, 512, &RWOffset);
	NtClose(hDisk);
	return status;

}


//unsigned int
//read_superblock(XboxPartitionTable *p_table, int num)
//{
//	HANDLE hDisk;
//	NTSTATUS status;
//	ANSI_STRING a_path;
//	OBJECT_ATTRIBUTES ObjectAttributes;
//	IO_STATUS_BLOCK IoStatusBlock;
//	BYTE PartSector[XBOX_HD_SECTOR_SIZE];
//	BYTE SuperBlockBuffer[XBOX_HD_SECTOR_SIZE];
//	LARGE_INTEGER RWOffset;
//
//
//	RtlInitAnsiString(&a_path, "\\Device\\Harddisk0\\partition0");
//	InitializeObjectAttributes(&ObjectAttributes, &a_path, OBJ_CASE_INSENSITIVE, NULL);
//
//	FATX_SUPERBLOCK SuperBlock;
//	//{
//	//	{ 'F', 'A', 'T', 'X' },
//	//	8,
//	//	32,
//	//	1,
//	//	0
//	//};
//
//	char part[] = "\\Device\\Harddisk0\\Partition0\\";
//
//	memset(&SuperBlock, 0, 4096);
//
//	RWOffset.QuadPart = 0x0fffffff; //p_table->TableEntries[num - 1].LBAStart;
//	RWOffset.QuadPart *= (ULONGLONG) 512;
////	RWOffset.QuadPart = 0x13CA7200 + 4096;
//
//	memset(PartSector, 0, sizeof(PartSector));
//
////	if(p_table)
////		memcpy(PartSector, p_table, sizeof(XboxPartitionTable));
//
//	status = XapiFormatFATVolumeEx(&partition_str[num - 1], 32768);
//	status = NtOpenFile(&hDisk, (GENERIC_READ|GENERIC_WRITE),
//		&ObjectAttributes, &IoStatusBlock, (FILE_SHARE_READ|FILE_SHARE_WRITE), FILE_SYNCHRONOUS_IO_ALERT);
//
//	if (NT_SUCCESS(status))
//	{
//		status = NtReadFile(hDisk, NULL, NULL, NULL, &IoStatusBlock, (PVOID) &SuperBlock, 4096, &RWOffset);
//		SuperBlock.ClusterSize = 0x40;
//		status = NtWriteFile(hDisk, NULL, NULL, NULL, &IoStatusBlock, (PVOID) &SuperBlock, 4096, &RWOffset);
//		status = NtReadFile(hDisk, NULL, NULL, NULL, &IoStatusBlock, (PVOID) &SuperBlock, 4096, &RWOffset);
//		NtClose(hDisk);
//	}
//
//	return status;
//}

void SelectClusterSize()
{
	g_iClusterSize = 16;
	ULONGLONG compare = 0x20000000;
	while (PartTbl.TableEntries[g_iCurrentPart].LBASize > compare)
	{
		compare = compare * 2;
		g_iClusterSize = g_iClusterSize * 2;
	}
}

void DrawSpace(XboxPartitionTable *PartTbl, DWORD TotalSectors)
{
	swprintf(buf1, L"Free space: %.0f of %.0f MB", part_get_free_sectors(PartTbl, TotalSectors) / 2048.000, floor(TotalSectors / 2048.000));
	DisplayText(buf1, 60,  428, 0);
}

void DoFormatAll()
{
	int i;
	int status;

	DisplayWarning();
	XInputGetStateAny(&aButton, &aButtonVal, &tButton, &aLThumbY);
	XInputGetStateAny(&aButton, &aButtonVal, &tButton, &aLThumbY);

	if (aButton == 3)
	{
		if (!g_dwGotInfo)
			if (!DisplayNoInfo())
				return;

		if (!write_partition_table(&PartTbl))
		{
			int ClusterSize;
			ULONGLONG compare;

			g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
			DrawScreen(g_iCurrentPart);
			DisplayText(L"partition table written...", 60,  428, 2);
			g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
			Sleep(1500);

			if (KernelPartTblAdd)
				memcpy(KernelPartTblAdd, &PartTbl, sizeof(XboxPartitionTable));
			if (!g_iFunction)
			{
				g_ypos = 14;
				for (i = 0; i < 14; i++)
				{
					if (PartTbl.TableEntries[i].Flags & PE_PARTFLAGS_IN_USE &&
						(PartTbl.TableEntries[i].LBASize != SavedPartTbl.TableEntries[i].LBASize ||
						PartTbl.TableEntries[i].LBAStart != SavedPartTbl.TableEntries[i].LBAStart))
					{
						g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

						ClusterSize = 16;
						compare = 0x20000000; //256GB in LBA size
						while (PartTbl.TableEntries[i].LBASize > compare)
						{
							compare = compare * 2;
							ClusterSize = ClusterSize * 2;
						}

						DrawScreen(g_iCurrentPart);
						swprintf(buf1, L"formatting partition %d with %dk clusters...", i+1, ClusterSize);
						DisplayText(buf1, 60,  428, 2);
						g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
						status = XapiFormatFATVolumeEx(&partition_str[i], ClusterSize << 10);


					}
				}
			}
			memcpy(&SavedPartTbl, &PartTbl, sizeof(XboxPartitionTable));
		}
	}
}

void DoWriteTable()
{
	g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	DrawScreen(g_iCurrentPart);
	DisplayText(L"Partition table will be written to hard drive!", 60, 428, 0);
	DisplayText(L"Press", 60, 448);
	DisplayText(L"Y", 110, 448, 4);
	DisplayText(L"to continue or any other button to abort.", 128, 448);

	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);

	XInputGetStateAny(&aButton, &aButtonVal, &tButton, &aLThumbY);
	XInputGetStateAny(&aButton, &aButtonVal, &tButton, &aLThumbY);

	if (aButton == 3)
	{
		if (!g_dwGotInfo)
			if (!DisplayNoInfo())
				return;

		if (!write_partition_table(&PartTbl))
		{
			g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
			DrawScreen(g_iCurrentPart);
			DisplayText(L"partition table written...", 60,  428, 2);
			g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
			Sleep(1500);
		}
		else
		{
			g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
			DrawScreen(g_iCurrentPart);
			DisplayText(L"Partition table write failed!", 60, 428, 1);
			DisplayText(L"Press any button to continue.", 60, 448, 0);
			g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
			XInputGetStateAny(&aButton, &aButtonVal, &tButton, &aLThumbY);
			XInputGetStateAny(&aButton, &aButtonVal, &tButton, &aLThumbY);
		}
	}
}

void DoFormatOne()
{
	int status;
	SelectClusterSize();
	DisplayWarning();
	XInputGetStateAny(&aButton, &aButtonVal, &tButton, &aLThumbY);
	XInputGetStateAny(&aButton, &aButtonVal, &tButton, &aLThumbY);

	if (aButton == 3)
	{
		if (!g_dwGotInfo)
			if (!DisplayNoInfo())
				return;

		g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
		DrawScreen(g_iCurrentPart);
		swprintf(buf1, L"formatting partition %d with %dk clusters...", g_iCurrentPart + 1, g_iClusterSize);
		DisplayText(buf1, 60,  428, 2);
		g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
		status = XapiFormatFATVolumeEx(&partition_str[g_iCurrentPart], g_iClusterSize << 10);
	}

}
void DoStart()
{
	switch (g_iFunction)
	{
	case 0:
		DoFormatAll();
		break;
	case 1:
		DoWriteTable();
		break;
	case 2:
		if(PartTbl.TableEntries[g_iCurrentPart].Flags & PE_PARTFLAGS_IN_USE)
			DoFormatOne();
		break;
	}
}

//-----------------------------------------------------------------------------
// Name: main()
// Desc: The application's entry point
//-----------------------------------------------------------------------------

void __cdecl main()
{
	int i;

	int m_iCurrentScheme = 3;
	int m_iWaitRelease = 0;

	for (i =0; i < 14; i++)
		RtlInitAnsiString(&partition_str[i], partStr[i]);

	for (i =0; i < 14; i++)
		RtlInitAnsiString(&partition_str[i], partStr[i]);

	LD_LAUNCH_DASHBOARD LaunchDashData = { XLD_LAUNCH_DASHBOARD_MAIN_MENU };

	InitialiseD3D();

	XDEVICE_PREALLOC_TYPE deviceTypes[] =
	{
		{XDEVICE_TYPE_GAMEPAD, 4},
	};

	XInitDevices( sizeof(deviceTypes) / sizeof(XDEVICE_PREALLOC_TYPE), deviceTypes );

	Sleep(2000);

	g_Controller[0] = XInputOpen( XDEVICE_TYPE_GAMEPAD, XDEVICE_PORT0, XDEVICE_NO_SLOT, 0);
	g_Controller[1] = XInputOpen( XDEVICE_TYPE_GAMEPAD, XDEVICE_PORT1, XDEVICE_NO_SLOT, 0);
	g_Controller[2] = XInputOpen( XDEVICE_TYPE_GAMEPAD, XDEVICE_PORT2, XDEVICE_NO_SLOT, 0);
	g_Controller[3] = XInputOpen( XDEVICE_TYPE_GAMEPAD, XDEVICE_PORT3, XDEVICE_NO_SLOT, 0);

	if(read_lba48_info(&PartTbl, &KernelPartTblAdd, &g_dwUserSectors, &lba48_version))
	{
		g_dwGotInfo = 0;
		memcpy(&PartTbl, &BackupPartTbl, sizeof(XboxPartitionTable));
		DisplayNoInfo();
	}

	// save a copy so we can check if partitions changed
	memcpy(&SavedPartTbl, &PartTbl, sizeof(XboxPartitionTable));

	g_iCurrentPart = part_get_last_available(&PartTbl);

	for (i = 0x0ff; i > 0x06; i -= 0x06)
	{
		ULONG rect_color = i << 24;

		g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
		DrawScreen(g_iCurrentPart);
		DrawInfo();
		DrawSpace(&PartTbl, g_dwUserSectors);
		DrawRect(0, 0, 639, 479, rect_color, rect_color);
		g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
	}

	while (true)
	{
		g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
		DrawScreen(g_iCurrentPart);
		DrawInfo();
		DrawSpace(&PartTbl, g_dwUserSectors);
		SelectClusterSize();
		swprintf(buf1, L"Will use %d KB clusters", g_iClusterSize);
		DisplayText(buf1, 380,  428, 0);
		g_pD3DDevice->Present(NULL, NULL, NULL, NULL);

		if (m_iWaitRelease)
			XInputGetStateAny(&aButton, &aButtonVal, &tButton, &aLThumbY);

		XInputGetStateAny(&aButton, &aButtonVal, &tButton, &aLThumbY, 0);

		m_iWaitRelease = 0;

		if (!tButton && !aLThumbY)
		{
			switch(aButton)
			{
			case 0:
				if (m_iCurrentScheme == 3)
					m_iCurrentScheme = 0;
				else
					m_iCurrentScheme++;
				part_setup_standard_partitions(&PartTbl, &BackupPartTbl, g_dwUserSectors, m_iCurrentScheme);
				m_iWaitRelease = 1;
				break;
			case 1:
				part_even(&PartTbl, g_dwUserSectors);
				g_iCurrentPart = part_get_last_available(&PartTbl) - 1;
				break;
			case 2:
				g_iMovePart = !g_iMovePart;
				m_iWaitRelease = 1;
				break;
			case 4:
				g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
				DrawScreen(g_iCurrentPart);
				DisplayText(L"Partition table will be erased!", 60,  428, 0);
				DisplayText(L"Press", 60,  448, 0);
				DisplayText(L"Y", 110,  448, 4);
				DisplayText(L"to continue or any other key to abort!", 128,  448, 0);
				g_pD3DDevice->Present(NULL, NULL, NULL, NULL);

				XInputGetStateAny(&aButton, &aButtonVal, &tButton, &aLThumbY);
				XInputGetStateAny(&aButton, &aButtonVal, &tButton, &aLThumbY);
				if (aButton == 3)
				{
					if (write_partition_table(0))
					{
						g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
						DrawScreen(g_iCurrentPart);
						DisplayText(L"Partition table erasure failed!", 60, 428, 1);
						DisplayText(L"Press any button to continue.", 60, 448, 0);
						g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
						XInputGetStateAny(&aButton, &aButtonVal, &tButton, &aLThumbY);
					}
					else
					{
						g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
						DrawScreen(g_iCurrentPart);
						DisplayText(L"partition table erased", 60, 428, 2);
						g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
						Sleep(1500);
					}
				}
				break;
			case 5:
				if(g_iFunction < 2)
					g_iFunction++;
				else
					g_iFunction = 0;
				m_iWaitRelease = 1;
				break;
			case 6: // bigger
				if (PartTbl.TableEntries[g_iCurrentPart].Flags ^ PE_PARTFLAGS_IN_USE)
					part_enable(&PartTbl, g_dwUserSectors, g_iCurrentPart);
				else
					part_change_size(&PartTbl, g_iCurrentPart, g_dwUserSectors,
					aButtonVal, g_iMovePart, 1);
				break;
			case 7: // smaller
				if (PartTbl.TableEntries[g_iCurrentPart].Flags & PE_PARTFLAGS_IN_USE)
					part_change_size(&PartTbl, g_iCurrentPart, g_dwUserSectors,
					aButtonVal, g_iMovePart, -1);
				break;
			default:
				break;
			}
		}
		else
		{
			if (aLThumbY > 28671)
			{
				part_change_size(&PartTbl, g_iCurrentPart, g_dwUserSectors,
					aButtonVal, g_iMovePart, 1, true);
				continue;
			}
			if (aLThumbY < -28672)
			{
				part_change_size(&PartTbl, g_iCurrentPart, g_dwUserSectors,
					aButtonVal, g_iMovePart, -1, true);
				continue;
			}
			if (tButton & XINPUT_GAMEPAD_LEFT_THUMB && tButton & XINPUT_GAMEPAD_RIGHT_THUMB)
			{
				memcpy(&PartTbl, &SavedPartTbl, sizeof(XboxPartitionTable));
				g_iCurrentPart = part_get_last_available(&PartTbl);
				m_iWaitRelease = 1;
				continue;
			}
			else if (tButton & XINPUT_GAMEPAD_RIGHT_THUMB /*&& (g_iCurrentPart == 13 || PartTbl.TableEntries[g_iCurrentPart + 1].Flags ^ PE_PARTFLAGS_IN_USE)*/)
			{
				if (PartTbl.TableEntries[g_iCurrentPart].Flags & PE_PARTFLAGS_IN_USE)
				{
					part_disable(&PartTbl, g_iCurrentPart);
					if (g_iCurrentPart)
						g_iCurrentPart--;
				}
				else
					part_enable(&PartTbl, g_dwUserSectors, g_iCurrentPart);

				m_iWaitRelease = 1;
			}
			else if (tButton & XINPUT_GAMEPAD_DPAD_RIGHT &&
				PartTbl.TableEntries[g_iCurrentPart].Flags & PE_PARTFLAGS_IN_USE)
			{
				if (g_iMovePart)
					part_up(&PartTbl, g_iCurrentPart, g_dwUserSectors, 0x200000);
				else
					part_sub_size(&PartTbl, g_iCurrentPart, g_dwUserSectors, 0x200000);
				Sleep(250);
			}
			else if (tButton & XINPUT_GAMEPAD_DPAD_LEFT &&
				PartTbl.TableEntries[g_iCurrentPart].Flags & PE_PARTFLAGS_IN_USE)
			{
				if (g_iMovePart)
					part_down(&PartTbl, g_iCurrentPart, g_dwUserSectors, 0x200000);
				else
					part_add_size(&PartTbl, g_iCurrentPart, g_dwUserSectors, 0x200000);
				Sleep(250);
			}
			else if (tButton & XINPUT_GAMEPAD_DPAD_UP && g_iCurrentPart > 0)
			{
				if (!PartTbl.TableEntries[g_iCurrentPart].LBASize)
					PartTbl.TableEntries[g_iCurrentPart].Flags &= ~PE_PARTFLAGS_IN_USE;
				g_iCurrentPart--;
				Sleep(150);
			}
			else if (tButton & XINPUT_GAMEPAD_DPAD_DOWN && g_iCurrentPart < 13)
			{
				if (!PartTbl.TableEntries[g_iCurrentPart].LBASize)
					PartTbl.TableEntries[g_iCurrentPart].Flags &= ~PE_PARTFLAGS_IN_USE;
				g_iCurrentPart++;
				Sleep(150);
			}
			else if (tButton & XINPUT_GAMEPAD_BACK)
			{
				DisplayHelp();
			}
			else if (tButton & XINPUT_GAMEPAD_START)
			{
				DoStart();
			}
		}
	}
}
