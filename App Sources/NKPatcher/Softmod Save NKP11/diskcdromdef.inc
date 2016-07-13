%ifndef DISKCDROMDEF_INC
%define DISKCDROMDEF_INC


struc			LIST_ENTRY
.Flink:			resd 1
.Blink:			resd 1
endstruc


struc			OBJECT_STRING
.Length:		resw 1
.MaximumLength:		resw 1
.Buffer:		resd 1
endstruc


struc			DISK_GEOMETRY
.Cylinders:		resq 1
.MediaType:		resd 1
.TracksPerCylinder:	resd 1
.SectorsPerTrack:	resd 1
.BytesPerSector:		resd 1
endstruc
	
	
struc			PARTITION_INFORMATION
.StartingOffset:		resq 1
.PartitionLength:	resq 1
.HiddenSectors:		resd 1
.PartitionNumber:	resd 1
.PartitionType:		resb 1
.BootIndicator:		resb 1
.RecognizedPartition:	resb 1
.RewritePartition:	resb 1
.unknown:		resd 1	; where does this go?
endstruc





struc			KDEVICE_QUEUE
.Type:			resw 1
.Size:			resb 1	; only byte on Xbox, not word
.Busy:			resb 1
.DeviceListHead:		resb LIST_ENTRY_size
endstruc



%define NotificationEvent	0
%define SynchronizationEvent	1


struc			DISPATCHER_HEADER
.Type:			resb 1
.Absolute:		resb 1
.Size:			resb 1
.Inserted:		resb 1
.SignalState:		resd 1
.WaitListHead:		resb LIST_ENTRY_size
endstruc


struc			KEVENT
.Header:		resb DISPATCHER_HEADER_size
endstruc

	
struc			DEVICE_OBJECT
.Type:			resw 1
.Size:			resw 1
.ReferenceCount:		resd 1
.DriverObject:		resd 1
.NextDevice:		resd 1
.CurrentIrp:		resd 1
.Flags:			resd 1
.DeviceExtension:	resd 1
.DeviceType:		resb 1	; this is only a byte (not dword) on Xbox
.unknown:		resb 1
.StackSize:		resb 1
.unknown2:		resb 1
.SectorSize:		resd 1	; dword on Xbox, not word
.AlignmentRequirement:	resd 1
.DeviceQueue:		resb KDEVICE_QUEUE_size
.DeviceLock:		resb KEVENT_size
.unknown3:		resd 1
endstruc


struc			IO_STATUS_BLOCK
.Status:		resd 1
.Pointer		equ .Status
.Information:		resd 1
endstruc


struc			IRP
.Type:			resw 1
.Size:			resw 1
.Flags:			resd 1
.ThreadListEntry:	resb LIST_ENTRY_size
.IoStatus:		resb IO_STATUS_BLOCK_size
.StackCount:		resb 1
.CurrentLocation:	resb 1
.PendingReturned:	resb 1
.Cancel:		resb 1
.UserIosb:		resd 1
.UserEvent:		resd 1
.unknown:		resd 1	; what's this?
.Overlay:		resb 8
.UserBuffer:		resd 1
.SegmentArray:		resd 1	; from an argument to NtReadFileScatter
				; or NtWriteFileGather
.buflen_locked:		resd 1	; length of locked buffer (for direct I/O).
.Tail:			resb 40
endstruc

	
struc			IRP_TAIL_OVERLAY
.DriverContext:		resd 5
.DeviceQueueEntry	equ .DriverContext
.Thread:		resd 1
.ListEntry:		resb LIST_ENTRY_size
.CurrentStackLocation:	resd 1
.PacketType		equ .CurrentStackLocation
.OriginalFileObject:	resd 1
endstruc	


struc			IO_STACK_LOCATION
.MajorFunction:		resb 1
.MinorFunction:		resb 1
.Flags:			resb 1
.Control:		resb 1
.Parameters:		resb 16
.DeviceObject:		resd 1
.FileObject:		resd 1
.CompletionRoutine:	resd 1
.Context:		resd 1
endstruc


%define SL_PENDING_RETURNED	0x01
%define SL_INVOKE_ON_SUCCESS	0x40
%define SL_INVOKE_ON_ERROR	0x80

%define IRP_MJ_READ		2
%define IRP_MJ_WRITE		3

%define IRP_NOCACHE		0x00000001
%define IRP_READ_OPERATION	0x00000010
%define IRP_WRITE_OPERATION	0x00000020
	
%define IRP_MJ_CREATE		0x00
%define IRP_MJ_CLOSE		0x01
%define IRP_MJ_READ		0x02
%define IRP_MJ_WRITE		0x03
%define IRP_MJ_FLUSH_BUFFERS	0x06
%define IRP_MJ_DEVICE_CONTROL	0x0a

	

struc			IOSL_PARAMETERS_READ
.Length:		resd 1
.offset:		resd 1
.ByteOffset:		resq 1
endstruc

	
struc			IOSL_PARAMETERS_WRITE
.Length:		resd 1
.offset:		resd 1
.ByteOffset:		resq 1
endstruc

	
struc			IOSL_PARAMETERS_DEVICEIOCONTROL
.OutputBufferLength:	resd 1
.InputBuffer:		resd 1
.InputBufferLength:	resd 1
.IoControlCode:		resd 1
endstruc
	

NTSTATUS_size		equ 4

%define STATUS_INFO_LENGTH_MISMATCH	0xC0000004
%define STATUS_INVALID_PARAMETER		0xC000000D
%define STATUS_INVALID_DEVICE_REQUEST	0xC0000010
%define STATUS_NO_MEDIA_IN_DEVICE		0xC0000013
%define STATUS_MORE_PROCESSING_REQUIRED	0xC0000016
%define STATUS_BUFFER_TOO_SMALL		0xC0000023
%define STATUS_INSUFFICIENT_RESOURCES	0xC000009A
%define STATUS_IO_DEVICE_ERROR		0xC0000185
%define STATUS_VOLUME_DISMOUNTED		0xC000026E
%define STATUS_PENDING			0x00000103

	
struc			FILE_OBJECT
.Type:			resw 1
.access:		resb 1
.Flags:			resb 1
.DeviceObject:		resd 1
.FsContext:		resd 1
.FsContext2:		resd 1
.FinalStatus:		resb NTSTATUS_size
.CurrentByteOffset:	resq 1
.RelatedFileObject:	resd 1
.CompletionContext:	resd 1
.unknown:		resd 1
.Lock:			resb KEVENT_size
.Event:			resb KEVENT_size
endstruc


%define FILE_DEVICE_CD_ROM		2
%define FILE_DEVICE_DISK			7

%define IOCTL_CDROM_READ_TOC		0x24000
%define IOCTL_CDROM_GET_DRIVE_GEOMETRY	0x2404c
%define IOCTL_CDROM_CHECK_VERIFY		0x24800

%define IOCTL_VIRTUAL_CDROM_ID		0x1EE7CD00
%define IOCTL_VIRTUAL_CDROM_ATTACH	0x1EE7CD01
%define IOCTL_VIRTUAL_CDROM_DETACH	0x1EE7CD02
		
%define IOCTL_IDE_PASS_THROUGH		0x4d028
%define IOCTL_DISK_GET_DRIVE_GEOMETRY	0x70000
%define IOCTL_DISK_GET_PARTITION_INFO	0x74004
%define IOCTL_DISK_VERIFY		0x70014


struc			IDEREGS
.bFeaturesReg:		resb 1
.bSectorCountReg:	resb 1
.bSectorNumberReg:	resb 1
.bCylLowReg:		resb 1
.bCylHighReg:		resb 1
.bDriveHeadReg:		resb 1
.bCommandReg:		resb 1
.bReserved:		resb 1
endstruc


struc			ATA_PASS_THROUGH
.IdeReg:		resb IDEREGS_size
.DataBufferSize:		resd 1
.DataBuffer:		resd 1
endstruc



struc			ATTACH_USER_INPUT
.num_slices:		resd 1
.slice_files:			; resb MAX_IMAGE_SLICES*OBJECT_STRING_size
endstruc


struc			VERIFY_INFORMATION
.StartingOffset:		resq 1
.Length:		resd 1
.unknown:		resd 1
endstruc



%define FileNetworkOpenInformation	34


struc			FILE_NETWORK_OPEN_INFORMATION
.CreationTime:		resq 1
.LastAccessTime:		resq 1
.LastWriteTime:		resq 1
.ChangeTime:		resq 1
.AllocationSize:		resq 1
.EndOfFile:		resq 1
.FileAttributes:		resd 1
.unknown:		resd 1
endstruc



%define MAX_IMAGE_SLICES		8
%define SLICE_FILENAMES_SIZE	1024

	
struc			IMAGE_SLICE
.file_handle:		resd 1
.file_object:		resd 1
.size:			resq 1
endstruc

	
%define VIRTCD_ATTACHED		0x00000001


struc			VIRTUAL_CDROM_EXTENSION
.flags:			resd 1
.slice_lock_count:	resd 1
.slice_lock_event:	resb KEVENT_size
.read_user_count:	resd 1
.users_clear_event:	resb KEVENT_size
.num_slices:		resd 1
.total_size:		resq 1
.slices:		resb MAX_IMAGE_SLICES*IMAGE_SLICE_size
endstruc


struc			IMAGE_SLICE_FILES
.num_slices:		resd 1
.slice_files:			; resb MAX_IMAGE_SLICES*OBJECT_STRING_size
endstruc


struc			DISK_IMAGE_EXTENSION
.dev_obj:		resd 1
.part_info:		resb PARTITION_INFORMATION_size
.image_handle:		resd 1
.file_object:		resd 1
endstruc


%endif	; DISKCDROMDEF_INC
