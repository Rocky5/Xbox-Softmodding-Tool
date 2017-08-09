unit xbeheader;
interface
uses sysutils,windows;
// This file was taken from xbox-linux project's imagebld
// XBE stuff
// Not used in any exported kernel calls, but still useful.

type
// XBE header information
// Certificate structure
T_XBE_CERTIFICATE = record
	// 000 Size of certificate
	Size : cardinal;
	// 004 Certificate timestamp (unknown format)
	Timestamp : cardinal;
	// 008 Title ID
	TitleId : cardinal;
	// 00C Name of the game (Unicode)
  TitleName : array [0..39] of widechar;
	// 05C Alternate title ID's (0-terminated)
	AlternateTitleIds : array [0..15] of cardinal;
	// 09C Allowed media types - 1 bit match between XBE and media = boots
	MediaTypes : cardinal;
	// 0A0 Allowed game regions - 1 bit match between this and XBOX = boots
	GameRegion : cardinal;
	// 0A4 Allowed game ratings - 1 bit match between this and XBOX = boots
	GameRating : cardinal;
	// 0A8 Disk number (?)
	DiskNumber : cardinal;
	// 0AC Version (?)
	Version : cardinal;
	// 0B0 LAN key for this game
	LanKey : array [0..15] of byte;
	// 0C0 Signature key for this game
	SignatureKey : array [0..15] of byte;
	// 0D0 Signature keys for the alternate title ID's
	AlternateSignatureKeys:array [0..15,0..15] of byte;
	// 1D0
end;
P_XBE_CERTIFICATE = ^T_XBE_CERTIFICATE;

// Section headers
T_XBE_SECTION  = record
	// 000 Flags
	Flags : cardinal;
	// 004 Virtual address (where this section loads in RAM)
	VirtualAddress : pointer;
	// 008 Virtual size (size of section in RAM; after FileSize it's 00'd)
	VirtualSize : cardinal;
	// 00C File address (where in the file from which this section comes)
	FileAddress : cardinal;
	// 010 File size (size of the section in the XBE file)
	FileSize : cardinal;
	// 014 Pointer to section name
	SectionName : pchar;
	// 018 Section reference count - when >= 1, section is loaded
	SectionReferenceCount : integer;
	// 01C Pointer to head shared page reference count
	HeadReferenceCount : PWORD;
	// 020 Pointer to tail shared page reference count
	TailReferenceCount : PWord;
	// 024 SHA hash.  Hash int containing FileSize, then hash section.
	ShaHash : array[0..4] of integer;
	// 038
end;
P_XBE_SECTION = ^T_XBE_SECTION;

// TLS directory information
T_XBE_TLS = record
        // 000 raw data start address
        RawStart : integer;
        // 004 raw data end address
        RawEnd : integer;
        // 008 TLS index address
        TlsIndex : integer;
        // 00C TLS callbacks address
        TlsCallbacks : integer;
        // 010 size of zero fill
        SizeZeroFill : integer;
        // 014 Characteristics
        Characteristics: array [0..7] of byte;
        // 01C
end;
P_XBE_TLS = ^T_XBE_TLS;

// Library version data
T_XBE_LIBRARY = record
    // 000 Library Name
    Name : array [0..7] of byte;
    // 008 Major Version
    MajorVersion : word;
    // 00A Middle Version
    MiddleVersion : word;
    // 00C Minor Version
    MinorVersion : word;
    // 00E Flags
    Flags : word;
    // 010
end;
P_XBE_LIBRARY = T_XBE_LIBRARY;

T_XBE_HEADER = record
	// 000 "XBEH"
	Magic : array[0..3] of char;
	// 004 RSA digital signature of the entire header area
	HeaderSignature : array [0..255] of byte;
	// 104 Base address of XBE image (must be 0x00010000?)
	BaseAddress : pointer;
	// 108 Size of all headers combined - other headers must be within this
	HeaderSize : cardinal;
	// 10C Size of entire image
	ImageSize : cardinal;
	// 110 Size of this header (always 0x178?)
	XbeHeaderSize : cardinal;
	// 114 Image timestamp - unknown format
	Timestamp : cardinal;
	// 118 Pointer to certificate data (must be within HeaderSize)
	Certificate : P_XBE_CERTIFICATE;
	// 11C Number of sections
	NumSections : integer;
	// 120 Pointer to section headers (must be within HeaderSize)
	Sections : P_XBE_SECTION;
	// 124 Initialization flags
	InitFlags : cardinal;
	// 128 Entry point (XOR'd; see xboxhacker.net)
	EntryPoint : pointer;
	// 12C Pointer to TLS directory
	TlsDirectory : P_XBE_TLS;
	// 130 Stack commit size
	StackCommit : cardinal;
	// 134 Heap reserve size
	HeapReserve : cardinal;
	// 138 Heap commit size
	HeapCommit : cardinal;
	// 13C PE base address (?)
	PeBaseAddress : pointer;
	// 140 PE image size (?)
	PeImageSize : cardinal;
	// 144 PE checksum (?)
	PeChecksum : cardinal;
	// 148 PE timestamp (?)
	PeTimestamp : cardinal;
	// 14C PC path and filename to EXE file from which XBE is derived
	PcExePath : pchar;
	// 150 PC filename (last part of PcExePath) from which XBE is derived
	PcExeFilename : pchar;
	// 154 PC filename (Unicode version of PcExeFilename)
	PcExeFilenameUnicode : pointer;
	// 158 Pointer to kernel thunk table (XOR'd; EFB1F152 debug)
	KernelThunkTable  : pcardinal;
	// 15C Non-kernel import table (debug only)
	DebugImportTable : pointer;
	// 160 Number of library headers
	NumLibraries : cardinal;
	// 164 Pointer to library headers
	Libraries  : P_XBE_LIBRARY;
	// 168 Pointer to kernel library header
	KernelLibrary : P_XBE_LIBRARY;
	// 16C Pointer to XAPI library
	XapiLibrary : P_XBE_LIBRARY;
	// 170 Pointer to logo bitmap (NULL = use default of Microsoft)
	LogoBitmap : pointer;
	// 174 Size of logo bitmap
	LogoBitmapSize : cardinal;
	// 178
end;

P_XBE_HEADER = ^T_XBE_HEADER;

{// Initialization flags
#define XBE_INIT_MOUNT_UTILITY          0x00000001
#define XBE_INIT_FORMAT_UTILITY         0x00000002
#define XBE_INIT_64M_RAM_ONLY           0x00000004
#define XBE_INIT_DONT_SETUP_HDD         0x00000008

// Region codes
#define XBE_REGION_US_CANADA            0x00000001
#define XBE_REGION_JAPAN                0x00000002
#define XBE_REGION_ELSEWHERE            0x00000004
#define XBE_REGION_DEBUG                0x80000000

// Media types
#define XBE_MEDIA_HDD                   0x00000001
#define XBE_MEDIA_XBOX_DVD              0x00000002
#define XBE_MEDIA_ANY_CD_OR_DVD         0x00000004
#define XBE_MEDIA_CD                    0x00000008
#define XBE_MEDIA_1LAYER_DVDROM         0x00000010
#define XBE_MEDIA_2LAYER_DVDROM         0x00000020
#define XBE_MEDIA_1LAYER_DVDR           0x00000040
#define XBE_MEDIA_2LAYER_DVDR           0x00000080
#define XBE_MEDIA_USB                   0x00000100
#define XBE_MEDIA_ALLOW_UNLOCKED_HDD    0x40000000

// Section flags
#define XBE_SEC_WRITABLE                0x00000001
#define XBE_SEC_PRELOAD                 0x00000002
#define XBE_SEC_EXECUTABLE              0x00000004
#define XBE_SEC_INSERTED_FILE           0x00000008
#define XBE_SEC_RO_HEAD_PAGE            0x00000010
#define XBE_SEC_RO_TAIL_PAGE            0x00000020
 }

implementation

end.
