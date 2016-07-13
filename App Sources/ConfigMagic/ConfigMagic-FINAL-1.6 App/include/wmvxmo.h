/*
 *  wmvxmo.h - Xbox WMV decoder XMO interfaces
 *
 *  Copyright (C) 2000-2001 Microsoft Corporation.  All Rights Reserved.
 *
 */

#ifndef _WMVXMO_H
#define _WMVXMO_H

typedef struct IWMVDecoder IWMVDecoder;
typedef IWMVDecoder* LPWMVDECODER;

//
// WMV decoder callback function for reading input data
//
typedef DWORD
(CALLBACK *LPFNWMVXMODATACALLBACK)(
    LPVOID pvContext,
    DWORD dwOffset,
    DWORD dwByteCount,
    LPVOID* ppvData
    );

//
// Function for creating WMV decoder instance
//
STDAPI
WmvCreateDecoder(
    IN LPCSTR pszFileName,
    IN LPFNWMVXMODATACALLBACK pfnCallback,
    IN DWORD dwVideoOutputFormat,
    IN LPVOID pvContext,
    OUT LPWMVDECODER* ppWmvDecoder
    );

//
// Supported video output format
//
#define WMVVIDEOFORMAT_YUY2     1

typedef struct _WMVVIDEOINFO {
    DWORD dwWidth;
    DWORD dwHeight;
    DWORD dwOutputBitsPerPixel;
    FLOAT fFramesPerSecond;
    DWORD dwBitsPerSecond;
} WMVVIDEOINFO, *PWMVVIDEOINFO, *LPWMVVIDEOINFO;

#undef INTERFACE
#define INTERFACE IWMVDecoder

DECLARE_INTERFACE_(IWMVDecoder, XMediaObject)
{
    // IUnknown methods
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    // XMediaObject methods
    STDMETHOD(GetInfo)(THIS_ LPXMEDIAINFO pInfo) PURE;
    STDMETHOD(GetStatus)(THIS_ LPDWORD pdwStatus) PURE;
    STDMETHOD(Process)(THIS_ LPCXMEDIAPACKET pInputPacket, LPCXMEDIAPACKET pOutputPacket) PURE;
    STDMETHOD(Discontinuity)(THIS) PURE;
    STDMETHOD(Flush)(THIS) PURE;

    STDMETHOD(GetPlayDuration)(THIS_ PREFERENCE_TIME prtPlayDuration, PREFERENCE_TIME prtPreroll) PURE;
    STDMETHOD(GetVideoInfo)(THIS_ LPWMVVIDEOINFO pVideoInfo) PURE;
    STDMETHOD(GetAudioInfo)(THIS_ LPWAVEFORMATEX pAudioInfo) PURE;
    STDMETHOD(Seek)(THIS_ PREFERENCE_TIME prtSeekPosition) PURE;
    STDMETHOD(ProcessMultiple)(THIS_ LPCXMEDIAPACKET pVideoOutputPacket, LPCXMEDIAPACKET pAudioOutputPacket) PURE;
};

//
// WMV error codes
//
#define WMV_FACILITY_CODE 0x879
#define MAKE_WMVHRESULT(code) MAKE_HRESULT(SEVERITY_ERROR, WMV_FACILITY_CODE, code)
#define WMVERR_NO_AUDIO_STREAM  MAKE_WMVHRESULT(0x100)
#define WMVERR_NO_VIDEO_STREAM  MAKE_WMVHRESULT(0x101)

#endif // !_WMVXMO_H

