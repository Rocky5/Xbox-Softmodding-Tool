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

int lba48error;

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
	D3DXCreateTextureFromFile( g_pD3DDevice, "d:\\backdrop.png", &g_pBackTexture);
//	D3DXCreateTextureFromFileEx(g_pD3DDevice, "d:\\media\\gfx\\backdrop.bmp", D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &g_pBackTexture);
//	D3DXCreateTextureFromFileInMemory(g_pD3DDevice, Backdrop, BackdropSize, &g_pBackTexture);
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
	case 7: // green
		g_pLucon18BitmapFont->SetBkMode(XFONT_TRANSPARENT);
		g_pLucon18BitmapFont->SetTextColor(D3DCOLOR_XRGB(174,226,57));
		break;
	case 8: // grey
		g_pLucon18BitmapFont->SetBkMode(XFONT_TRANSPARENT);
		g_pLucon18BitmapFont->SetTextColor(D3DCOLOR_XRGB(89,90,101));
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
	case 7: // green
		g_pArial18BitmapFont->SetBkMode(XFONT_TRANSPARENT);
		g_pArial18BitmapFont->SetTextColor(D3DCOLOR_XRGB(174,226,57));
		break;
	case 8: // grey
		g_pArial18BitmapFont->SetBkMode(XFONT_TRANSPARENT);
		g_pArial18BitmapFont->SetTextColor(D3DCOLOR_XRGB(89,90,101));
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
	g_pArial18BitmapFont->TextOut( g_pFrontBuffer, L"Press any button to exit", -1, 50, 280 );


	//Release our TextBuffers
	g_pFrontBuffer->Release();

	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);

	XInputGetStateAny(&aButton, &aButtonVal, &tButton, &aLThumbY);
	lba48error = 1;
	return 1;
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

	DisplayText(L"Quitting...", 50, 20 * g_ypos++, 2);
	CleanUp();
	XLaunchNewImage( NULL, (LAUNCH_DATA*)&LaunchDashData );
}

void DrawTitle()
{
}

void DrawInfo()
{
	DisplayText(L"Press (Back) to exit.", 240,  190, 7);
	DisplayText(L"Press (A) to write - F takes all, partition tables.", 134,  210, 7);
	DisplayText(L"Press (B) to write - F & G, partition tables.", 152,  230, 7);
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


	swprintf(buf1, L"XBP Table Writer 0.1 - LBA48 patch v%d", lba48_version);
	DisplayText(buf1, 20, 445, 8, XFONT_LEFT);
	swprintf(buf2, L"Do not use this if you have custom partition:", lba48_version);
	DisplayText(buf2, 20, 30, 8, XFONT_LEFT);
	swprintf(buf3, L"ie, you have used XBPartitioner to format your drive.", lba48_version);
	DisplayText(buf3, 20, 50, 8, XFONT_LEFT);
	swprintf(buf3, L"Doing so will break those partitions.", lba48_version);
	DisplayText(buf3, 20, 70, 8, XFONT_LEFT);

	DrawTitle();
	//DrawTable(&PartTbl, g_dwUserSectors, g_iCurrentPart);
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

void SelectClusterSize()
{
}

void DrawSpace(XboxPartitionTable *PartTbl, DWORD TotalSectors)
{
}

void DoFormatAll()
{
}

void DoWriteTable()
{
	DisplayText(L"Press", 230, 275, 7);
	DisplayText(L"Y", 280, 275, 1);
	DisplayText(L"to write table.", 296, 275, 7);
	DisplayText(L"or any other button to abort.", 210, 290, 7);

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
			DisplayText(L"partition table written...", 230,  280, 2);
			g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
			Sleep(1500);
		}
		else
		{
			DisplayText(L"Partition table write failed!", 220,  275, 1);
			DisplayText(L"Press any button to continue.", 205,  290, 0);
			g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
			XInputGetStateAny(&aButton, &aButtonVal, &tButton, &aLThumbY);
			XInputGetStateAny(&aButton, &aButtonVal, &tButton, &aLThumbY);
		}
	}
}

void DoFormatOne()
{
}
void DoStart()
{
	switch (g_iFunction)
	{
	case 0:
		DoWriteTable();
		break;
	case 1:
		break;
	case 2:
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

	int m_iCurrentScheme = 1;
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
		g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
		
		if (m_iWaitRelease)
			XInputGetStateAny(&aButton, &aButtonVal, &tButton, &aLThumbY);
			XInputGetStateAny(&aButton, &aButtonVal, &tButton, &aLThumbY, 0);
			m_iWaitRelease = 0;

			if (aButton == 0)
			{
				m_iCurrentScheme = 0;
				part_setup_standard_partitions(&PartTbl, &BackupPartTbl, g_dwUserSectors, m_iCurrentScheme);
				DoWriteTable();
			}
			if (aButton == 1)
			{
				m_iCurrentScheme = 1;
				part_setup_standard_partitions(&PartTbl, &BackupPartTbl, g_dwUserSectors, m_iCurrentScheme);
				DoWriteTable();
			}
			if (tButton & XINPUT_GAMEPAD_BACK)
			{
				DisplayHelp();
			}
			if (lba48error == 1)
			{
				XInputGetStateAny(&aButton, &aButtonVal, &tButton, &aLThumbY);
				XInputGetStateAny(&aButton, &aButtonVal, &tButton, &aLThumbY, 0);
				DisplayHelp();
			}
	}
}
