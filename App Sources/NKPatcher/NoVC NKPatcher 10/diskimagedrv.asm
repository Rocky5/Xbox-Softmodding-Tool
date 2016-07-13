;;;
;;; diskimagedrv.asm
;;;
;;; Copyright 2004 rmenhal
;;;
;;; Licensed under GNU General Public License version 2. See the file COPYING
;;; for details.
;;;

%include "diskcdromdef.inc"


%macro	diskimagedrv 4
%ifdef VIRTUAL_C_DRIVE
	mov	eax,%1
	mov	ebx,%2
	mov	ecx,%3
	mov	edx,%4
	call	installdiskimagedrv
%endif
%endmacro


	
%ifdef VIRTUAL_C_DRIVE
	
	CODE_SECTION
	
installdiskimagedrv:	
	mov	edi,[expd_space]
	test	edi,edi
	jz	near .exit

	mov	esi,[memdiff]

	mov	byte [eax+esi],0E9h
	lea	ebp,[edi+create_device_patch-disk_image_patch-5]
	sub	ebp,eax
	mov	[eax+esi+1],ebp

	add	eax,byte 5
	setcalljmptarget eax, disk_image_patch, create_device_patch.var_jback_cont, edi
	mov	[create_device_patch.devtab+4],ebx
	setcalljmptarget ecx, disk_image_patch, create_device_patch.var_jback_success, edi
	mov	[create_device_patch.rsize+1],edx
	mov	[disk_image_device_control.rsize+1],edx
	

	mov	ebp,[memdiff]
	
	mov	ebx,[kexports]
	add	ebx,ebp
	mov	ecx,disk_image_patch
	mov	esi,.exp_tab
	call	tune_exp_refs

	mov	eax,[ebx + KERNEL_EXPORTS.IoInvalidDeviceRequest]
	add	eax,80010000h
	push	edi
	mov	edi,disk_image_driver_object.MajorFunction
	STOS_CONST 14*4
	pop	edi
		
	lea	eax,[edi+disk_image_return_success-disk_image_patch]
	mov	[disk_image_driver_object.MajorFunction + IRP_MJ_CREATE*4],eax
	mov	[disk_image_driver_object.MajorFunction + IRP_MJ_CLOSE*4],eax

	lea	eax,[edi+disk_image_read_write-disk_image_patch]
	mov	[disk_image_driver_object.MajorFunction + IRP_MJ_READ*4],eax
	mov	[disk_image_driver_object.MajorFunction + IRP_MJ_WRITE*4],eax

	lea	eax,[edi+disk_image_flush_buffers-disk_image_patch]
	mov	[disk_image_driver_object.MajorFunction+IRP_MJ_FLUSH_BUFFERS*4],eax

	lea	eax,[edi+disk_image_device_control-disk_image_patch]
	mov	[disk_image_driver_object.MajorFunction+IRP_MJ_DEVICE_CONTROL*4],eax


	add	[create_device_patch.drvob+1],edi
	add	[disk_image_read_write.croua+1],edi
	add	[disk_image_flush_buffers.croua+1],edi
	add	[create_device_patch.scfa+1],edi
	add	[create_device_patch.virtc_file+4],edi

	mov	esi,disk_image_patch
	mov	ecx,DISK_IMAGE_PATCH_SIZE/4
	add	edi,ebp
	rep	movsd
	sub	edi,ebp

	mov	[expd_space],edi
.exit:	
	ret


.exp_tab:
	set_exp_entry TUNE_CALL, create_device_patch, IoCreateDevice, 1
	set_exp_entry TUNE_CALL, create_device_patch, IoDeleteDevice, 1
	set_exp_entry TUNE_CALL, create_device_patch, IoDeleteDevice, 2
	set_exp_entry TUNE_CALL, create_device_patch, NtCreateFile, 1
	set_exp_entry TUNE_CALL, create_device_patch, NtClose, 1
	set_exp_entry TUNE_CALL, create_device_patch, NtQueryInformationFile, 1
	set_exp_entry TUNE_CALL, create_device_patch, ObReferenceObjectByHandle, 1
	set_exp_entry TUNE_CALL, create_device_patch, ObfDereferenceObject, 1
	set_exp_entry TUNE_PUSH, create_device_patch, IoFileObjectType, 1
	set_exp_entry TUNE_CALL, disk_image_read_write, IoAllocateIrp, 1
	set_exp_entry TUNE_CALL, disk_image_read_write, IoFreeIrp, 1
	set_exp_entry TUNE_CALL, disk_image_read_write, IofCallDriver, 1
	set_exp_entry TUNE_CALL, disk_image_read_write, IofCompleteRequest, 1
	set_exp_entry TUNE_CALL, disk_image_read_write, IofCompleteRequest, 2
	set_exp_entry TUNE_CALL, disk_image_flush_buffers, IoAllocateIrp, 1
	set_exp_entry TUNE_CALL, disk_image_flush_buffers, IoFreeIrp, 1
	set_exp_entry TUNE_CALL, disk_image_flush_buffers, IofCallDriver, 1
	set_exp_entry TUNE_CALL, disk_image_flush_buffers, IofCompleteRequest, 1
	set_exp_entry TUNE_CALL, disk_image_flush_buffers, IofCompleteRequest, 2
	set_exp_entry TUNE_CALL, disk_image_device_control, IofCompleteRequest, 1
	set_exp_entry TUNE_CALL, disk_image_return_success, IofCompleteRequest, 1
	set_exp_entry_last



	DATA_SECTION

align 16

disk_image_patch:
	
create_device_patch:
.part_info	equ -28h

	cmp	ebx,byte VIRTUAL_C_DRIVE
	jne	.skip

	mov	[ebp + .part_info + PARTITION_INFORMATION.HiddenSectors],eax

	lea	eax,[ebp+8]
	push	eax
	push	byte 0
	push	byte FILE_DEVICE_DISK
	push	byte 0
	push	byte DISK_IMAGE_EXTENSION_size
.drvob:	push	dword (disk_image_driver_object-disk_image_patch)
	VCALL	IoCreateDevice_1	; call IoCreateDevice
	test	eax,eax
	jl	.error

	mov	esi,[ebp+8]
	mov	edi,[esi + DEVICE_OBJECT.DeviceExtension]
	mov	[edi + DISK_IMAGE_EXTENSION.dev_obj],esi


	push	eax
	push	eax
	mov	ecx,esp		; IO status
	
	push	byte 40h
.scfa:	push	dword (.virtc_file-disk_image_patch)
	push	byte 0
	mov	edx,esp		; Object attributes

	lea	eax,[edi + DISK_IMAGE_EXTENSION.image_handle]
	
	push	84Ah
	push	byte 1
	push	byte 1
	push	byte 0
	push	byte 0
	push	ecx
	push	edx
	push	20003h
	push	eax
	VCALL	NtCreateFile_1	; call NtCreateFile
	add	esp,byte 12+8
	test	eax,eax
	jge	.openok

	;; Disk image file open failed. Fall back to using the real disk.

.error_del_dev:	
	push	esi
	VCALL	IoDeleteDevice_1 ; call IoDeleteDevice

.error:
	push	byte 1
	pop	edi
	mov	eax,[ebp + .part_info + PARTITION_INFORMATION.HiddenSectors]
.skip:	
	mov	esi,512
	VJMP	jback_cont

.openok:


	sub	esp,byte FILE_NETWORK_OPEN_INFORMATION_size+8
	mov	eax,esp		; IO status
	lea	edx,[eax+8]	; FILE_NETWORK_OPEN_INFORMATION

	push	byte FileNetworkOpenInformation
	push	byte FILE_NETWORK_OPEN_INFORMATION_size
	push	edx
	push	eax
	push	dword [edi + DISK_IMAGE_EXTENSION.image_handle]
	VCALL	NtQueryInformationFile_1
	mov	ecx,[esp+8 + FILE_NETWORK_OPEN_INFORMATION.EndOfFile]
	mov	edx,[esp+8 + FILE_NETWORK_OPEN_INFORMATION.EndOfFile+4]
	add	esp,byte FILE_NETWORK_OPEN_INFORMATION_size+8
	test	eax,eax
	jl	.error_del_dev

	and	ecx,~0x1ff
	jnz	.lenok
	test	edx,edx
	jz	.error_del_dev
.lenok:	

	mov	[edi + DISK_IMAGE_EXTENSION.part_info + PARTITION_INFORMATION.PartitionLength],ecx
	mov	[edi + DISK_IMAGE_EXTENSION.part_info + PARTITION_INFORMATION.PartitionLength+4],edx

	;; Set the starting sector to the end of disk. If any software tries
	;; to actually use the virtual via Partition0 it will then just get
	;; back an error status.
	
.rsize:	mov	eax,[8003B438h]
	mov	edx,512
	
	mov	[edi + DISK_IMAGE_EXTENSION.part_info + PARTITION_INFORMATION.HiddenSectors],eax
	mov	[edi + DISK_IMAGE_EXTENSION.part_info + PARTITION_INFORMATION.PartitionNumber],ebx

	or	dword [esi + DEVICE_OBJECT.Flags],byte 44h

	push	byte 1
	pop	dword [esi + DEVICE_OBJECT.AlignmentRequirement]

	mov	[esi + DEVICE_OBJECT.SectorSize],edx

	mul	edx
	mov	[edi + DISK_IMAGE_EXTENSION.part_info + PARTITION_INFORMATION.StartingOffset],eax
	mov	[edi + DISK_IMAGE_EXTENSION.part_info + PARTITION_INFORMATION.StartingOffset+4],edx
	
	
	lea	eax,[edi + DISK_IMAGE_EXTENSION.file_object]
	push	eax
	VPUSH	IoFileObjectType_1
	push	dword [edi + DISK_IMAGE_EXTENSION.image_handle]
	VCALL	ObReferenceObjectByHandle_1

	
	and	dword [esi + DEVICE_OBJECT.Flags],byte ~10h

	xor	eax,eax
.devtab:cmpxchg	[8003AFA0h+4*ebx],esi
	je	.insertok

	;; A device already occupying partition 2 slot. Clean up ours.

	push	dword [edi + DISK_IMAGE_EXTENSION.file_object]
	VCALL	ObfDereferenceObject_1

	push	dword [edi + DISK_IMAGE_EXTENSION.image_handle]
	VCALL	NtClose_1

	push	esi
	VCALL	IoDeleteDevice_2 ; call IoDeleteDevice
	
.insertok:
	;; success

	VJMP	jback_success	; jump back



.virtc_filestr	db VIRTUAL_C_PATH, 0
.virtc_file	dw $-.virtc_filestr-1, $-.virtc_filestr
		dd .virtc_filestr-disk_image_patch


	
disk_image_read_write:
	;; [esp+4] = address to our device object
	;; [esp+8] = address to IRP

	push	ebp
	mov	ebp,esp

	push	ebx
	push	esi
	push	edi

	mov	edi,[ebp+0Ch]
	mov	esi,[edi + IRP.Tail + IRP_TAIL_OVERLAY.CurrentStackLocation]
	;; EDI = address to parent IRP
	;; ESI = address to parent IRP's current stack location

	mov	ebx,[ebp+8]
	mov	ebx,[ebx + DEVICE_OBJECT.DeviceExtension]
	mov	ebx,[ebx + DISK_IMAGE_EXTENSION.file_object]

	mov	edx,[ebx + FILE_OBJECT.DeviceObject]
	mov	al,[edx + DEVICE_OBJECT.StackSize]
	push	eax
	VCALL	IoAllocateIrp_1
	test	eax,eax
	jnz	.irpok

	mov	esi,STATUS_INSUFFICIENT_RESOURCES
	mov	[edi + IRP.IoStatus + IO_STATUS_BLOCK.Status],esi

	mov	dl,0
	mov	ecx,edi
	VCALL	IofCompleteRequest_1

	mov	eax,esi
	jmp	short .exit

.irpok:
	mov	edx,eax
	;; EDX = our new child IRP
	
	mov	ecx,[edx + IRP.Tail + IRP_TAIL_OVERLAY.CurrentStackLocation]
	sub	ecx,byte IO_STACK_LOCATION_size
	;; ECX = next stack location of child IRP

	mov	al,[esi + IO_STACK_LOCATION.MajorFunction]
	mov	[ecx + IO_STACK_LOCATION.MajorFunction],al

	mov	[ecx + IO_STACK_LOCATION.FileObject],ebx

	or	dword [edx + IRP.Flags],byte IRP_NOCACHE


	mov	eax,[esi + IO_STACK_LOCATION.Parameters + IOSL_PARAMETERS_READ.offset]
	test	byte [edi + IRP.Flags+1],4
	jnz	.scattergather

	;; Check if the request is from cache (bit 7 set). In that case .offset is
	;; the buffer address.
	
	test	byte [esi + IO_STACK_LOCATION.Flags],80h
	jnz	.setub
	add	eax,[edi + IRP.UserBuffer]
.setub:	mov	[edx + IRP.UserBuffer],eax

	;; Our buffer was already locked upstairs.
	
	or	byte [edx + IRP.Flags+1],2
	jmp	short .bufok

.scattergather:	
	or	byte [edx + IRP.Flags+1],4
	shr	eax,12-3
	and	eax,byte ~7
	add	eax,[edi + IRP.SegmentArray]
	mov	[edx + IRP.SegmentArray],eax
.bufok:	


	mov	eax,[esi+IO_STACK_LOCATION.Parameters+IOSL_PARAMETERS_READ.Length]
	mov	[ecx+IO_STACK_LOCATION.Parameters+IOSL_PARAMETERS_READ.Length],eax

	mov	[edx + IRP.buflen_locked],eax ; buffers already locked

	and	dword [ecx + IO_STACK_LOCATION.Parameters + IOSL_PARAMETERS_READ.offset],byte 0

	mov	eax,[esi + IO_STACK_LOCATION.Parameters + IOSL_PARAMETERS_READ.ByteOffset]
	mov	[ecx + IO_STACK_LOCATION.Parameters + IOSL_PARAMETERS_READ.ByteOffset],eax
	mov	eax,[esi + IO_STACK_LOCATION.Parameters + IOSL_PARAMETERS_READ.ByteOffset+4]
	mov	[ecx + IO_STACK_LOCATION.Parameters + IOSL_PARAMETERS_READ.ByteOffset+4],eax

.croua:	mov	eax,.completion_routine-disk_image_patch
	mov	[ecx + IO_STACK_LOCATION.CompletionRoutine],eax
	mov	[ecx + IO_STACK_LOCATION.Context],edi
	or	byte [ecx + IO_STACK_LOCATION.Control],SL_INVOKE_ON_SUCCESS | SL_INVOKE_ON_ERROR

	or	byte [esi + IO_STACK_LOCATION.Control],SL_PENDING_RETURNED
	
	mov	ecx,[ebx + FILE_OBJECT.DeviceObject]
	VCALL	IofCallDriver_1

	mov	eax,STATUS_PENDING

.exit:	
	pop	edi
	pop	esi
	pop	ebx
	pop	ebp
	ret	8




.completion_routine:
	;; [esp+4] = address of device object
	;; [esp+8] = address of our child IRP
	;; [esp+0Ch] = routine context (address of parent IRP)

	mov	edx,[esp+8]
	mov	ecx,[esp+0Ch]

	;; Copy child's IO status to parent
	
	mov	eax,[edx + IRP.IoStatus]
	mov	[ecx + IRP.IoStatus],eax
	mov	eax,[edx + IRP.IoStatus+4]
	mov	[ecx + IRP.IoStatus+4],eax
	push	ecx

	;; Get rid of the child

	push	edx
	VCALL	IoFreeIrp_1

	;; Complete the parent IRP
	
	mov	dl,1
	pop	ecx
	VCALL	IofCompleteRequest_2

	mov	eax,STATUS_MORE_PROCESSING_REQUIRED
	ret	12



disk_image_device_control:
	push	ebp
	mov	ebp,esp
	
	push	esi
	push	edi
	
	mov	edi,[ebp+0Ch]
	mov	esi,[edi + IRP.Tail + IRP_TAIL_OVERLAY.CurrentStackLocation]

	mov	eax,[esi + IO_STACK_LOCATION.Parameters + IOSL_PARAMETERS_DEVICEIOCONTROL.IoControlCode]
	cmp	eax,IOCTL_DISK_GET_DRIVE_GEOMETRY
	je	.getgeom
	cmp	eax,IOCTL_DISK_GET_PARTITION_INFO
	je	.getpart
	cmp	eax,IOCTL_DISK_VERIFY
	je	.diskverify
	mov	eax,STATUS_INVALID_DEVICE_REQUEST
	jmp	short .exit

.getgeom:
	cmp	dword [esi + IO_STACK_LOCATION.Parameters + IOSL_PARAMETERS_DEVICEIOCONTROL.OutputBufferLength],byte DISK_GEOMETRY_size
	mov	eax,STATUS_BUFFER_TOO_SMALL
	jb	.exit

	mov	edx,[edi + IRP.UserBuffer]

.rsize:	mov	eax,[8003B438h]
	mov	[edx + DISK_GEOMETRY.Cylinders],eax
	xor	eax,eax
	mov	[edx + DISK_GEOMETRY.Cylinders+4],eax
	inc	eax
	mov	dword [edx + DISK_GEOMETRY.MediaType],12
	mov	[edx + DISK_GEOMETRY.TracksPerCylinder],eax
	mov	[edx + DISK_GEOMETRY.SectorsPerTrack],eax
	mov	dword [edx + DISK_GEOMETRY.BytesPerSector],512

	push	byte DISK_GEOMETRY_size
	pop	dword [edi + IRP.IoStatus + IO_STATUS_BLOCK.Information]

	xor	eax,eax
	jmp	short .exit

.getpart:
	cmp	dword [esi + IO_STACK_LOCATION.Parameters + IOSL_PARAMETERS_DEVICEIOCONTROL.OutputBufferLength],byte PARTITION_INFORMATION_size
	mov	eax,STATUS_BUFFER_TOO_SMALL
	jb	.exit
	
	mov	esi,[ebp+8]
	mov	esi,[esi + DEVICE_OBJECT.DeviceExtension]
	add	esi,byte DISK_IMAGE_EXTENSION.part_info
	push	edi
	mov	edi,[edi + IRP.UserBuffer]
	MOVS_CONST PARTITION_INFORMATION_size
	pop	edi

	push	byte PARTITION_INFORMATION_size
	pop	dword [edi + IRP.IoStatus + IO_STATUS_BLOCK.Information]
	
	xor	eax,eax
	jmp	short .exit

.diskverify:	
	push	edi
	push	dword [ebp+8]
	call	disk_image_ioctl_verify

.exit:
	cmp	eax,STATUS_PENDING
	je	.done

	push	eax
	mov	[edi + IRP.IoStatus + IO_STATUS_BLOCK.Status],eax
	mov	dl,0
	mov	ecx,edi
	VCALL	IofCompleteRequest_1
	pop	eax
	
.done:	
	pop	edi
	pop	esi
	pop	ebp
	ret	8



disk_image_ioctl_verify:	
	push	esi
	push	edi
	
	mov	edi,[esp+8+8]
	mov	esi,[edi + IRP.Tail + IRP_TAIL_OVERLAY.CurrentStackLocation]
	
	cmp	dword [esi + IO_STACK_LOCATION.Parameters + IOSL_PARAMETERS_DEVICEIOCONTROL.InputBufferLength],byte VERIFY_INFORMATION_size
	mov	eax,STATUS_INFO_LENGTH_MISMATCH
	jb	.exit

	mov	ecx,[esi + IO_STACK_LOCATION.Parameters + IOSL_PARAMETERS_DEVICEIOCONTROL.InputBuffer]
	cmp	dword [ecx + VERIFY_INFORMATION.Length],20000h
	mov	eax,STATUS_INVALID_DEVICE_REQUEST
	ja	.exit

	and	dword [edi + IRP.IoStatus + IO_STATUS_BLOCK.Information],byte 0

	mov	esi,[esp+8+4]
	mov	esi,[esi + DEVICE_OBJECT.DeviceExtension]
	
	mov	eax,[ecx + VERIFY_INFORMATION.StartingOffset]
	mov	edx,[ecx + VERIFY_INFORMATION.StartingOffset+4]
	mov	ecx,[ecx + VERIFY_INFORMATION.Length]
	and	eax,~0x1ff
	and	ecx,~0x1ff
	
	cmp	edx,[esi + DISK_IMAGE_EXTENSION.part_info + PARTITION_INFORMATION.PartitionLength+4]
	ja	.fail
	jb	.start_ok
	cmp	eax,[esi + DISK_IMAGE_EXTENSION.part_info + PARTITION_INFORMATION.PartitionLength]
	ja	.fail
.start_ok:

	add	eax,ecx
	adc	edx,byte 0
	
	cmp	edx,[esi + DISK_IMAGE_EXTENSION.part_info + PARTITION_INFORMATION.PartitionLength+4]
	ja	.fail
	jb	.end_ok
	cmp	eax,[esi + DISK_IMAGE_EXTENSION.part_info + PARTITION_INFORMATION.PartitionLength]
	ja	.fail
.end_ok:
	xor	eax,eax
	jmp	short .exit
.fail:
	mov	eax,STATUS_IO_DEVICE_ERROR

.exit:	
	pop	edi
	pop	esi
	ret	8



disk_image_return_success:
	xor	edx,edx
	mov	ecx,[esp+8]
	mov	[ecx + IRP.IoStatus + IO_STATUS_BLOCK.Status],edx
	VCALL	IofCompleteRequest_1
	xor	eax,eax
	ret	8



disk_image_flush_buffers:
	;; [esp+4] = address to our device object
	;; [esp+8] = address to IRP

	push	ebp
	mov	ebp,esp

	push	ebx
	push	esi
	push	edi

	mov	edi,[ebp+0Ch]
	mov	esi,[edi + IRP.Tail + IRP_TAIL_OVERLAY.CurrentStackLocation]
	;; EDI = address to parent IRP
	;; ESI = address to parent IRP's current stack location

	mov	ebx,[ebp+8]
	mov	ebx,[ebx + DEVICE_OBJECT.DeviceExtension]
	mov	ebx,[ebx + DISK_IMAGE_EXTENSION.file_object]

	mov	edx,[ebx + FILE_OBJECT.DeviceObject]
	mov	al,[edx + DEVICE_OBJECT.StackSize]
	push	eax
	VCALL	IoAllocateIrp_1
	test	eax,eax
	jnz	.irpok

	mov	esi,STATUS_INSUFFICIENT_RESOURCES
	mov	[edi + IRP.IoStatus + IO_STATUS_BLOCK.Status],esi

	mov	dl,0
	mov	ecx,edi
	VCALL	IofCompleteRequest_1

	mov	eax,esi
	jmp	short .exit

.irpok:
	mov	edx,eax
	;; EDX = our new child IRP
	
	mov	ecx,[edx + IRP.Tail + IRP_TAIL_OVERLAY.CurrentStackLocation]
	sub	ecx,byte IO_STACK_LOCATION_size
	;; ECX = next stack location of child IRP

	push	esi
	push	edi
	push	ecx
	mov	edi,ecx
	MOVS_CONST IO_STACK_LOCATION.DeviceObject
	pop	ecx
	pop	edi
	pop	esi

	mov	byte [ecx + IO_STACK_LOCATION.Control],0

.croua:	mov	eax,.completion_routine-disk_image_patch
	mov	[ecx + IO_STACK_LOCATION.CompletionRoutine],eax
	mov	[ecx + IO_STACK_LOCATION.Context],edi
	or	byte [ecx + IO_STACK_LOCATION.Control],SL_INVOKE_ON_SUCCESS | SL_INVOKE_ON_ERROR

	or	byte [esi + IO_STACK_LOCATION.Control],SL_PENDING_RETURNED
	
	mov	ecx,[ebx + FILE_OBJECT.DeviceObject]
	VCALL	IofCallDriver_1

	mov	eax,STATUS_PENDING

.exit:	
	pop	edi
	pop	esi
	pop	ebx
	pop	ebp
	ret	8




.completion_routine:
	;; [esp+4] = address of device object
	;; [esp+8] = address of our child IRP
	;; [esp+0Ch] = routine context (address of parent IRP)

	mov	edx,[esp+8]
	mov	ecx,[esp+0Ch]

	;; Copy child's IO status to parent
	
	mov	eax,[edx + IRP.IoStatus]
	mov	[ecx + IRP.IoStatus],eax
	mov	eax,[edx + IRP.IoStatus+4]
	mov	[ecx + IRP.IoStatus+4],eax
	push	ecx

	;; Get rid of the child

	push	edx
	VCALL	IoFreeIrp_1

	;; Complete the parent IRP
	
	mov	dl,1
	pop	ecx
	VCALL	IofCompleteRequest_2

	mov	eax,STATUS_MORE_PROCESSING_REQUIRED
	ret	12



align 4

disk_image_driver_object:
		dd 0
		dd 0
		dd 0
.MajorFunction:	
		dd 800265C9h	; IRP_MJ_CREATE
		dd 800265C9h	; IRP_MJ_CLOSE
		dd 800243D6h	; IRP_MJ_READ
		dd 800243D6h	; IRP_MJ_WRITE
		dd 80016677h
		dd 80016677h
		dd 800244F2h	; IRP_MJ_FLUSH_BUFFERS
		dd 80016677h
		dd 80016677h
		dd 80016677h
		dd 8002458Ah	; IRP_MJ_DEVICE_CONTROL
		dd 80016677h
		dd 80016677h
		dd 80016677h
.end:	



align 16

DISK_IMAGE_PATCH_SIZE	equ $-disk_image_patch

%else 	; !VIRTUAL_C_DRIVE

DISK_IMAGE_PATCH_SIZE	equ 0

%endif	; !VIRTUAL_C_DRIVE

