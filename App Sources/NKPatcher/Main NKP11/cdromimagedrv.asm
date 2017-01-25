;;;
;;; cdromimagedrv.asm
;;;
;;; Copyright 2004 rmenhal
;;;
;;; Licensed under GNU General Public License version 2. See the file COPYING
;;; for details.
;;;
%include "diskcdromdef.inc"
%macro	cdromimagedrv 4
%ifdef VIRTUAL_CDROM_DRIVE
	mov	eax,%1
	mov	ebx,%2
	mov	ecx,%3
	mov	edx,%4
	call	installcdromimagedrv
%endif
%endmacro
%ifdef VIRTUAL_CDROM_DRIVE
	CODE_SECTION
installcdromimagedrv:
	mov	edi,[expd_space]
	test	edi,edi
	jz	near .exit
	mov	[cdrom_image_attach_and_link.decd+1],ebx
	mov	[cdrom_image_attach_and_link.decd2+1],ebx
	mov	[cdrom_image_attach_and_link.symcd+1],ecx
	mov	[cdrom_image_detach_and_unlink.decd+1],ebx
	mov	[cdrom_image_detach_and_unlink.symcd+1],ecx
	mov	[cdrom_image_attach.symcd+1],ecx
	mov	[cdrom_image_create_virtual.partd+2],edx
	mov	[cdrom_image_attach.partd+1],edx
	mov	esi,[memdiff]
	mov	edx,[eax+esi]
	mov	[cdrom_image_create_real],edx
	mov	edx,[eax+esi+4]
	mov	[cdrom_image_create_real+4],edx
	mov	byte [eax+esi],0E9h
	lea	ebp,[edi+cdrom_image_create_patch-cdrom_image_patch-5]
	sub	ebp,eax
	mov	[eax+esi+1],ebp
	add	eax,byte 8
	setcalljmptarget eax, cdrom_image_patch, cdrom_image_create_real.var_jback_real, edi
	mov	ebp,[memdiff]
	mov	ebx,[kexports]
	add	ebx,ebp
	mov	ecx,cdrom_image_patch
	mov	esi,.exp_tab
	call	tune_exp_refs
	mov	eax,[ebx + KERNEL_EXPORTS.HalReadSMCTrayState]
	add	eax,80010000h
	setcalljmptarget eax, cdrom_image_patch, cdrom_image_read_tray_state.var_jback_real, edi
	mov	esi,[kexports]
	add	esi,ebp
	lea	eax,[edi+cdrom_image_read_tray_state-cdrom_image_patch-80010000h]
	mov	[esi + KERNEL_EXPORTS.HalReadSMCTrayState],eax
	mov	eax,[ebx + KERNEL_EXPORTS.IoInvalidDeviceRequest]
	add	eax,80010000h
	push	edi
	mov	edi,cdrom_image_driver_object.MajorFunction
	STOS_CONST 14*4
	pop	edi
	lea	eax,[edi+cdrom_image_return_success-cdrom_image_patch]
	mov	[cdrom_image_driver_object.MajorFunction + IRP_MJ_CREATE*4],eax
	mov	[cdrom_image_driver_object.MajorFunction + IRP_MJ_CLOSE*4],eax
	lea	eax,[edi+cdrom_image_read-cdrom_image_patch]
	mov	[cdrom_image_driver_object.MajorFunction + IRP_MJ_READ*4],eax
	lea	eax,[edi+cdrom_image_device_control-cdrom_image_patch]
	mov	[cdrom_image_driver_object.MajorFunction+IRP_MJ_DEVICE_CONTROL*4],eax
	add	[cdrom_image_create_virtual.drvob+1],edi
	add	[cdrom_image_create_virtual.devn+1],edi
	add	[cdrom_image_create_virtual.devn2+1],edi
	add	[cdrom_image_attach_and_link.devn+1],edi
	add	[cdrom_image_create_virtual.dosn+1],edi
	add	[cdrom_image_read.croua+1],edi
	add	[process_user_attach.sfbuf+1],edi
	add	[attach_virtual_disc.sfbuf+1],edi
	add	[detach_virtual_disc.sfbuf+1],edi
	add	[virtual_cdrom_dev_objstr+4],edi
	add	[virtual_cdrom_dos_objstr+4],edi
	add	[cdrom_image_read_tray_state.state+1],edi
	add	[cdrom_image_read_tray_state.chnge+1],edi
	add	[cdrom_image_reset_tray_state.state+2],edi
	add	[cdrom_image_reset_tray_state.chnge+2],edi
	add	[cdrom_image_disc_load.state+2],edi
	add	[cdrom_image_disc_load.chnge+2],edi
	add	[cdrom_image_read_toc.ptoc+1],edi
	mov	esi,cdrom_image_patch
	mov	ecx,CDROM_IMAGE_PATCH_SIZE/4
	add	edi,ebp
	rep	movsd
	sub	edi,ebp
	mov	[expd_space],edi
.exit:	
	ret
.exp_tab:		
	set_exp_entry TUNE_CALL, cdrom_image_create_virtual, IoCreateDevice, 1
	set_exp_entry TUNE_CALL, cdrom_image_create_virtual, IoCreateSymbolicLink, 1
	set_exp_entry TUNE_CALL, cdrom_image_create_virtual, KeInitializeEvent, 1
	set_exp_entry TUNE_CALL, cdrom_image_create_virtual, KeInitializeEvent, 2
	set_exp_entry TUNE_PUSH, attach_virtual_disc, IoFileObjectType, 1
	set_exp_entry TUNE_CALL, attach_virtual_disc, ObReferenceObjectByHandle, 1
	set_exp_entry TUNE_CALL, attach_virtual_disc, NtCreateFile, 1
	set_exp_entry TUNE_CALL, attach_virtual_disc, NtQueryInformationFile, 1
	set_exp_entry TUNE_CALL, attach_virtual_disc, ObfDereferenceObject, 1
	set_exp_entry TUNE_CALL, attach_virtual_disc, NtClose, 1
	set_exp_entry TUNE_CALL, detach_virtual_disc, ObfDereferenceObject, 1
	set_exp_entry TUNE_CALL, detach_virtual_disc, NtClose, 1
	set_exp_entry TUNE_CALL, cdrom_image_read, IoAllocateIrp, 1
	set_exp_entry TUNE_CALL, cdrom_image_read, IoFreeIrp, 1
	set_exp_entry TUNE_CALL, cdrom_image_read, IoFreeIrp, 2
	set_exp_entry TUNE_CALL, cdrom_image_read, IofCallDriver, 1
	set_exp_entry TUNE_CALL, cdrom_image_read, IofCompleteRequest, 1
	set_exp_entry TUNE_CALL, cdrom_image_read, IofCompleteRequest, 2
	set_exp_entry TUNE_CALL, cdrom_image_device_control, IofCompleteRequest, 1
	set_exp_entry TUNE_CALL, cdrom_image_attach, IoDismountVolume, 1
	set_exp_entry TUNE_CALL, cdrom_image_attach, IoDeleteSymbolicLink, 1
	set_exp_entry TUNE_CALL, cdrom_image_attach, IoDeleteDevice, 1
	set_exp_entry TUNE_CALL, cdrom_image_attach_and_link, IoCreateSymbolicLink, 1
	set_exp_entry TUNE_CALL, cdrom_image_attach_and_link, IoCreateSymbolicLink, 2
	set_exp_entry TUNE_CALL, cdrom_image_detach_and_unlink, IoDeleteSymbolicLink, 1
	set_exp_entry TUNE_CALL, cdrom_image_detach_and_unlink, IoDeleteSymbolicLink, 2
	set_exp_entry TUNE_CALL, cdrom_image_return_success, IofCompleteRequest, 1
	set_exp_entry TUNE_CALL, cdrom_image_acquire_slice_lock, KeWaitForSingleObject, 1
	set_exp_entry TUNE_CALL, cdrom_image_release_slice_lock, KeSetEvent, 1
	set_exp_entry TUNE_CALL, cdrom_image_clear_read_users, KeWaitForSingleObject, 1
	set_exp_entry TUNE_CALL, cdrom_image_decrease_user_count, KeSetEvent, 1
	set_exp_entry_last
	DATA_SECTION
align 16
cdrom_image_patch:
virtual_cdrom_dev_name:	db '\Device\CdRom1',0
.end:	
virtual_cdrom_dos_name:	db '\??\CdRom1:',0
.end:
align 4
virtual_cdrom_dev_objstr	dw virtual_cdrom_dev_name.end - virtual_cdrom_dev_name - 1
			dw virtual_cdrom_dev_name.end - virtual_cdrom_dev_name
			dd virtual_cdrom_dev_name - cdrom_image_patch
virtual_cdrom_dos_objstr	dw virtual_cdrom_dos_name.end - virtual_cdrom_dos_name - 1
			dw virtual_cdrom_dos_name.end - virtual_cdrom_dos_name
			dd virtual_cdrom_dos_name - cdrom_image_patch
cdrom_image_create_real:	
	push	ebp
	mov	ebp,esp
	sub	esp,byte 20h
	push	ebx
	push	esi
	VJMP	jback_real
cdrom_image_create_patch:		
cdrom_image_create_virtual:
	push	esi
	push	eax
	push	esp
	push	byte 0
	push	byte FILE_DEVICE_CD_ROM
.devn:	push	dword (virtual_cdrom_dev_objstr - cdrom_image_patch)
	push	VIRTUAL_CDROM_EXTENSION_size
.drvob:	push	dword (cdrom_image_driver_object - cdrom_image_patch)
	VCALL	IoCreateDevice_1	; call IoCreateDevice
	pop	esi
	test	eax,eax
	jl	near .exit
.devn2:	push	dword (virtual_cdrom_dev_objstr - cdrom_image_patch)
.dosn:	push	dword (virtual_cdrom_dos_objstr - cdrom_image_patch)
	VCALL	IoCreateSymbolicLink_1
	;; Set flag for direct I/O only. We can do scatter/gather if there's only
	;; one slice, so we'll set it later in that case.
	or	dword [esi + DEVICE_OBJECT.Flags],byte 04h
	push	byte 1
	pop	dword [esi + DEVICE_OBJECT.AlignmentRequirement]
	mov	dword [esi + DEVICE_OBJECT.SectorSize],2048
	mov	edx,[esi + DEVICE_OBJECT.DeviceExtension]
	lea	eax,[edx + VIRTUAL_CDROM_EXTENSION.slice_lock_event]
	or	dword [edx + VIRTUAL_CDROM_EXTENSION.slice_lock_count],byte -1
	push	byte 1
	push	byte SynchronizationEvent
	push	eax
	VCALL	KeInitializeEvent_1
	mov	edx,[esi + DEVICE_OBJECT.DeviceExtension]
	lea	eax,[edx + VIRTUAL_CDROM_EXTENSION.users_clear_event]
	mov	dword [edx + VIRTUAL_CDROM_EXTENSION.read_user_count],80000000h
	push	byte 0
	push	byte NotificationEvent
	push	eax
	VCALL	KeInitializeEvent_2
	call	cdrom_image_reset_tray_state
	and	dword [esi + DEVICE_OBJECT.Flags],byte ~10h
.partd:	mov	[8003ae60h],esi
	mov	ecx,esi
	call	cdrom_image_attach_and_link
	xor	eax,eax
.exit:	
	pop	esi
	ret
cdrom_image_attach_and_link:
	push	ecx
	call	attach_virtual_disc
	test	eax,eax
	jge	.createlinks
	call	cdrom_image_create_real
	ret
.createlinks:	
.devn:	push	dword (virtual_cdrom_dev_objstr - cdrom_image_patch)
.decd:	push	800123b8h
	VCALL	IoCreateSymbolicLink_1
.decd2:	push	800123b8h
.symcd:	push	800123cch
	VCALL	IoCreateSymbolicLink_2
	ret
cdrom_image_detach_and_unlink:	
	push	ecx
	call	detach_virtual_disc
.symcd:	push	800123cch
	VCALL	IoDeleteSymbolicLink_1
.decd:	push	800123b8h
	VCALL	IoDeleteSymbolicLink_2
	call	cdrom_image_create_real
	ret
cdrom_image_read_tray_state:
.state:	mov	eax,[tray_state-cdrom_image_patch]
	cmp	eax,byte TRAY_STATE_NO_DISC
	je	.jreal
	mov	ecx,[esp+4]
	mov	[ecx],eax
	mov	ecx,[esp+8]
	jecxz	.noch
.chnge:	mov	eax,[tray_state_changes-cdrom_image_patch]
	mov	[ecx],eax
.noch:	
	xor	eax,eax
	ret	8
.jreal:
	VJMP	jback_real
cdrom_image_reset_tray_state:
	push	byte TRAY_STATE_NO_DISC
.state:	pop	dword [tray_state-cdrom_image_patch]
.chnge:	and	dword [tray_state_changes-cdrom_image_patch],byte 0
	ret
cdrom_image_disc_load:		
	push	byte TRAY_STATE_DISC_LOADED
.set:	
.state:	pop	dword [tray_state-cdrom_image_patch]
.chnge:	inc	dword [tray_state_changes-cdrom_image_patch]
	ret
cdrom_image_disc_unload:		
	push	byte TRAY_STATE_NO_DISC
	jmp	short cdrom_image_disc_load.set
cdrom_image_acquire_slice_lock:
	;; ecx = Address to our device object
	mov	ecx,[ecx + DEVICE_OBJECT.DeviceExtension]
	inc	dword [ecx + VIRTUAL_CDROM_EXTENSION.slice_lock_count]
	jnz	.zzz
	ret
.zzz:
	xor	eax,eax
	add	ecx,byte VIRTUAL_CDROM_EXTENSION.slice_lock_event
	push	eax
	push	eax
	push	eax
	push	eax
	push	ecx
	VCALL	KeWaitForSingleObject_1
	ret
cdrom_image_release_slice_lock:
	;; ecx = Address to our device object
	mov	ecx,[ecx + DEVICE_OBJECT.DeviceExtension]
	dec	dword [ecx + VIRTUAL_CDROM_EXTENSION.slice_lock_count]
	jge	.wake_up
	ret
.wake_up:	
	add	ecx,byte VIRTUAL_CDROM_EXTENSION.slice_lock_event
	push	byte 0
	push	byte 0
	push	ecx
	VCALL	KeSetEvent_1
	ret
cdrom_image_allow_read_users:
	;; ecx = Address to our device object
	mov	ecx,[ecx + DEVICE_OBJECT.DeviceExtension]
	mov	dword [ecx + VIRTUAL_CDROM_EXTENSION.read_user_count],0
	ret
cdrom_image_clear_read_users:
	;; ecx = Address to our device object
	mov	ecx,[ecx + DEVICE_OBJECT.DeviceExtension]
	mov	dword [ecx + VIRTUAL_CDROM_EXTENSION.users_clear_event + KEVENT.Header + DISPATCHER_HEADER.SignalState],0
.again:	
	mov	eax,[ecx + VIRTUAL_CDROM_EXTENSION.read_user_count]
	mov	edx,eax
	or	edx,80000000h
	cmpxchg	[ecx + VIRTUAL_CDROM_EXTENSION.read_user_count],edx
	jne	.again
	test	eax,7FFFFFFFh
	jnz	.zzz
	ret
.zzz:	
	xor	eax,eax
	add	ecx,byte VIRTUAL_CDROM_EXTENSION.users_clear_event
	push	eax
	push	eax
	push	eax
	push	eax
	push	ecx
	VCALL	KeWaitForSingleObject_1
	ret
cdrom_image_increase_user_count:	
	;; In:	ecx = Address to our device object
	;; Out:	ZF (zero flag) is set if detach pending, otherwise clear
	mov	ecx,[ecx + DEVICE_OBJECT.DeviceExtension]
.again:	
	mov	eax,[ecx + VIRTUAL_CDROM_EXTENSION.read_user_count]
	test	eax,80000000h
	jnz	.exit
	mov	edx,eax
	inc	edx
	cmpxchg	[ecx + VIRTUAL_CDROM_EXTENSION.read_user_count],edx
	jne	.again
.exit:
	ret
cdrom_image_decrease_user_count:		
	;; In:	ecx = Address to our device object
	mov	ecx,[ecx + DEVICE_OBJECT.DeviceExtension]
.again:	
	mov	eax,[ecx + VIRTUAL_CDROM_EXTENSION.read_user_count]
	test	eax,80000000h
	jnz	.notify_if_last
	mov	edx,eax
	dec	edx
	cmpxchg	[ecx + VIRTUAL_CDROM_EXTENSION.read_user_count],edx
	jne	.again
	ret
.notify_if_last:
.again2:	
	mov	eax,[ecx + VIRTUAL_CDROM_EXTENSION.read_user_count]
	mov	edx,eax
	dec	edx
	cmpxchg	[ecx + VIRTUAL_CDROM_EXTENSION.read_user_count],edx
	jne	.again2
	cmp	edx,80000000h
	je	.notify
	ret
.notify:
	add	ecx,byte VIRTUAL_CDROM_EXTENSION.users_clear_event
	push	byte 0
	push	byte 0
	push	ecx
	VCALL	KeSetEvent_1
	ret
cdrom_image_read:
.irp_list		equ 0-4*MAX_IMAGE_SLICES
.irp_count		equ .irp_list-4
.byte_offset		equ .irp_count-8
.length_left		equ .byte_offset-4
.user_buffer		equ .length_left-4
.segment_array		equ .user_buffer-4
.local_var_size		equ -.segment_array
	;; [esp+4] = address to our device object
	;; [esp+8] = address to IRP
	push	ebp
	mov	ebp,esp
	sub	esp,byte .local_var_size
	push	ebx
	push	esi
	push	edi
	mov	edi,[ebp+0Ch]
	;; EDI = address to parent IRP
	mov	ebx,[ebp+8]
	mov	ecx,ebx
	call	cdrom_image_increase_user_count
	mov	esi,STATUS_NO_MEDIA_IN_DEVICE
	jnz	near .exit_complete_no_dec
	mov	esi,[edi + IRP.Tail + IRP_TAIL_OVERLAY.CurrentStackLocation]
	;; ESI = address to parent IRP's current stack location
	cmp	dword [esi+IO_STACK_LOCATION.Parameters+IOSL_PARAMETERS_READ.Length],byte 0
	je	.zerolen
	mov	ebx,[ebx + DEVICE_OBJECT.DeviceExtension]
	mov	ecx,[ebx + VIRTUAL_CDROM_EXTENSION.num_slices]
	add	ebx,byte VIRTUAL_CDROM_EXTENSION.slices
	mov	eax,[esi + IO_STACK_LOCATION.Parameters + IOSL_PARAMETERS_READ.ByteOffset]
	mov	edx,[esi + IO_STACK_LOCATION.Parameters + IOSL_PARAMETERS_READ.ByteOffset+4]
.find_first_slice:
	mov	[ebp+.byte_offset],eax
	mov	[ebp+.byte_offset+4],edx
	sub	eax,[ebx + IMAGE_SLICE.size]
	sbb	edx,[ebx + IMAGE_SLICE.size+4]
	jc	.first_slice_found
	add	ebx,byte IMAGE_SLICE_size
	dec	ecx
	jnz	.find_first_slice
	mov	esi,STATUS_INVALID_PARAMETER
	jmp	short .exit_complete
.first_slice_found:
	mov	eax,[ebp+.byte_offset]
	mov	edx,[ebp+.byte_offset+4]
	add	eax,[esi+IO_STACK_LOCATION.Parameters+IOSL_PARAMETERS_READ.Length]
	adc	edx,byte 0
	sub	eax,byte 1
	sbb	edx,byte 0
	and	dword [ebp+.irp_count],byte 0
	push	ebx	
.allocate_irps_loop:
	push	ecx
	push	edx
	push	eax
	mov	edx,[ebx + IMAGE_SLICE.file_object]
	mov	edx,[edx + FILE_OBJECT.DeviceObject]
	mov	al,[edx + DEVICE_OBJECT.StackSize]
	push	eax
	VCALL	IoAllocateIrp_1
	test	eax,eax
	jz	.irperr
	mov	edx,[ebp+.irp_count]
	inc	dword [ebp+.irp_count]
	mov	[ebp+4*edx+.irp_list],eax
	pop	eax
	pop	edx
	pop	ecx
	sub	eax,[ebx + IMAGE_SLICE.size]
	sbb	edx,[ebx + IMAGE_SLICE.size+4]
	jc	.irpok
	add	ebx,byte IMAGE_SLICE_size
	dec	ecx
	jnz	.allocate_irps_loop
	pop	ebx
	mov	esi,STATUS_INVALID_PARAMETER
	jmp	short .exit_free_irps
.zerolen:
	xor	esi,esi
	mov	[edi + IRP.IoStatus + IO_STATUS_BLOCK.Information],esi
	jmp	short .exit_complete
.irperr:	
	pop	eax
	pop	edx
	pop	ecx
	pop	ebx
	mov	esi,STATUS_INSUFFICIENT_RESOURCES
.exit_free_irps:	
	cmp	dword [ebp + .irp_count],byte 0
	je	.free_done
	lea	ebx,[ebp + .irp_list]
.free_irps:
	push	dword [ebx]
	VCALL	IoFreeIrp_1
	add	ebx,byte 4
	dec	dword [ebp + .irp_count]
	jnz	.free_irps
.free_done:
.exit_complete:
	mov	ecx,[ebp+8]
	call	cdrom_image_decrease_user_count
.exit_complete_no_dec:	
	mov	[edi + IRP.IoStatus + IO_STATUS_BLOCK.Status],esi
	mov	dl,0
	mov	ecx,edi
	VCALL	IofCompleteRequest_1
	mov	eax,esi
	jmp	.exit
.irpok:
	pop	ebx
	mov	eax,[esi + IO_STACK_LOCATION.Parameters + IOSL_PARAMETERS_READ.offset]
	test	byte [edi + IRP.Flags+1],4
	jnz	.setscattergather
.setlinear:	
	;; Check if the request is from cache (bit 7 set). In that case .offset is
	;; the buffer address.
	test	byte [esi + IO_STACK_LOCATION.Flags],80h
	jnz	.setub
	add	eax,[edi + IRP.UserBuffer]
.setub:	mov	[ebp + .user_buffer],eax
	jmp	short .setbufok
.setscattergather:
	mov	edx,[ebp+8]
	test	byte [edx + DEVICE_OBJECT.Flags],40h ; can do scatter/gather?
	jz	.setlinear
	shr	eax,12-3
	and	eax,byte ~7
	add	eax,[edi + IRP.SegmentArray]
	mov	[ebp + .segment_array],eax
.setbufok:	
	mov	eax,[esi+IO_STACK_LOCATION.Parameters+IOSL_PARAMETERS_READ.Length]
	mov	[ebp + .length_left],eax
	or	byte [esi + IO_STACK_LOCATION.Control],SL_PENDING_RETURNED
	mov	eax,[ebp + .irp_count]
	mov	[edi + IRP.Tail + IRP_TAIL_OVERLAY.DriverContext],eax
	;; Initialize IO status to success and zero Information
	and	dword [edi + IRP.IoStatus + IO_STATUS_BLOCK.Status],byte 0
	and	dword [edi + IRP.IoStatus + IO_STATUS_BLOCK.Information],byte 0
	lea	edx,[ebp+.irp_list]
.send_irps_loop:
	push	edx
	mov	edx,[edx]
	;; EDX = our new child IRP
	mov	ecx,[edx + IRP.Tail + IRP_TAIL_OVERLAY.CurrentStackLocation]
	sub	ecx,byte IO_STACK_LOCATION_size
	;; ECX = next stack location of child IRP
	mov	al,[esi + IO_STACK_LOCATION.MajorFunction]
	mov	[ecx + IO_STACK_LOCATION.MajorFunction],al
	or	dword [edx + IRP.Flags],byte IRP_NOCACHE
	push	edx
	mov	eax,[ebx + IMAGE_SLICE.size]
	mov	edx,[ebx + IMAGE_SLICE.size+4]
	sub	eax,[ebp + .byte_offset]
	sbb	edx,[ebp + .byte_offset+4]
	cmp	edx,byte 0
	ja	.lenall
	cmp	eax,[ebp + .length_left]
	jbe	.lenok
.lenall:
	mov	eax,[ebp + .length_left]
.lenok:	
	pop	edx
	sub	[ebp + .length_left],eax
	mov	[ecx+IO_STACK_LOCATION.Parameters+IOSL_PARAMETERS_READ.Length],eax
	mov	[edx + IRP.buflen_locked],eax ; buffers already locked
	test	byte [edi + IRP.Flags+1],4
	jnz	.scattergather
.linearoper:
	mov	eax,[ebp + .user_buffer]
	mov	[edx + IRP.UserBuffer],eax
	mov	eax,[ecx+IO_STACK_LOCATION.Parameters+IOSL_PARAMETERS_READ.Length]
	add	[ebp + .user_buffer],eax
	;; Our buffer was already locked upstairs.
	or	byte [edx + IRP.Flags+1],2
	jmp	short .bufok
.scattergather:	
	mov	eax,[ebp+8]
	test	byte [eax + DEVICE_OBJECT.Flags],40h ; can do scatter/gather?
	jz	.linearoper
	or	byte [edx + IRP.Flags+1],4
	mov	eax,[ebp + .segment_array]
	mov	[edx + IRP.SegmentArray],eax
	mov	eax,[ecx+IO_STACK_LOCATION.Parameters+IOSL_PARAMETERS_READ.Length]
	shr	eax,12-3
	and	eax,byte ~7
	add	[ebp + .segment_array],eax
.bufok:	
	mov	eax,[ebp + .byte_offset]
	mov	[ecx + IO_STACK_LOCATION.Parameters + IOSL_PARAMETERS_READ.ByteOffset],eax
	mov	eax,[ebp + .byte_offset+4]
	mov	[ecx + IO_STACK_LOCATION.Parameters + IOSL_PARAMETERS_READ.ByteOffset+4],eax
	xor	eax,eax
	mov	[ebp + .byte_offset],eax
	mov	[ebp + .byte_offset+4],eax
	mov	[ecx + IO_STACK_LOCATION.Parameters + IOSL_PARAMETERS_READ.offset],eax
.croua:	mov	eax,.completion_routine-cdrom_image_patch
	mov	[ecx + IO_STACK_LOCATION.CompletionRoutine],eax
	mov	[ecx + IO_STACK_LOCATION.Context],edi
	or	byte [ecx + IO_STACK_LOCATION.Control],SL_INVOKE_ON_SUCCESS | SL_INVOKE_ON_ERROR
	mov	eax,[ebx + IMAGE_SLICE.file_object]
	mov	[ecx + IO_STACK_LOCATION.FileObject],eax
	mov	ecx,[eax + FILE_OBJECT.DeviceObject]
	VCALL	IofCallDriver_1
	pop	edx
	add	edx,byte 4
	add	ebx,byte IMAGE_SLICE_size
	dec	dword [ebp + .irp_count]
	jnz	.send_irps_loop
	mov	eax,STATUS_PENDING
.exit:	
	pop	edi
	pop	esi
	pop	ebx
	leave
	ret	8
.completion_routine:
	;; [esp+4] = address of device object
	;; [esp+8] = address of our child IRP
	;; [esp+0Ch] = routine context (address of parent IRP)
	mov	edx,[esp+8]
	mov	ecx,[esp+0Ch]
	;; Copy IO Status only if its an error
	mov	eax,[edx + IRP.IoStatus + IO_STATUS_BLOCK.Status]
	test	eax,eax
	jge	.iostatsuc
	mov	[ecx + IRP.IoStatus + IO_STATUS_BLOCK.Status],eax
.iostatsuc:
	;; Accumulate IO status Information
	mov	eax,[edx + IRP.IoStatus + IO_STATUS_BLOCK.Information]
	add	[ecx + IRP.IoStatus + IO_STATUS_BLOCK.Information],eax ; atomic
	;; Get rid of the child
	push	ecx
	push	edx
	VCALL	IoFreeIrp_2
	pop	ecx
	;; Check if this is the last child IRP, and if so complete the parent.
	;; The check must be atomic.
	dec	dword [ecx + IRP.Tail + IRP_TAIL_OVERLAY.DriverContext]
	jnz	.skip_completion
	push	ecx
	mov	ecx,[ecx + IRP.Tail + IRP_TAIL_OVERLAY.CurrentStackLocation]
	mov	ecx,[ecx + IO_STACK_LOCATION.DeviceObject]
	call	cdrom_image_decrease_user_count
	pop	ecx
	mov	dl,1
	VCALL	IofCompleteRequest_2
.skip_completion:	
	mov	eax,STATUS_MORE_PROCESSING_REQUIRED
	ret	12
cdrom_image_device_control:
	push	ebp
	mov	ebp,esp
	push	esi
	push	edi
	mov	ecx,[ebp+8]
	call	cdrom_image_acquire_slice_lock
	mov	edi,[ebp+0Ch]
	mov	esi,[edi + IRP.Tail + IRP_TAIL_OVERLAY.CurrentStackLocation]
	mov	eax,[esi + IO_STACK_LOCATION.Parameters + IOSL_PARAMETERS_DEVICEIOCONTROL.IoControlCode]
	cmp	eax,0x24080	; authentication
	je	.success
	cmp	eax,IOCTL_CDROM_GET_DRIVE_GEOMETRY
	je	.getgeom
	cmp	eax,IOCTL_CDROM_READ_TOC
	je	.readtoc
	cmp	eax,IOCTL_CDROM_CHECK_VERIFY
	je	.checkverify
	cmp	eax,IOCTL_VIRTUAL_CDROM_ATTACH
	je	.attach
	cmp	eax,IOCTL_VIRTUAL_CDROM_DETACH
	je	.detach
	mov	eax,STATUS_INVALID_DEVICE_REQUEST
	jmp	short .exit
.success:	
	xor	eax,eax
	jmp	short .exit
.getgeom:
	push	edi
	push	dword [ebp+8]
	call	cdrom_image_get_geometry
	jmp	short .exit
.readtoc:
	push	edi
	push	dword [ebp+8]
	call	cdrom_image_read_toc
	jmp	short .exit
.checkverify:
	and	dword [edi + IRP.IoStatus + IO_STATUS_BLOCK.Information],byte 0
	mov	ecx,[ebp+8]
	mov	ecx,[ecx + DEVICE_OBJECT.DeviceExtension]
	test	byte [ecx + VIRTUAL_CDROM_EXTENSION.flags],byte VIRTCD_ATTACHED
	jnz	.success
	mov	eax,STATUS_NO_MEDIA_IN_DEVICE
	jmp	short .exit
.attach:
	push	edi
	push	dword [ebp+8]
	call	cdrom_image_attach
	jmp	short .exit
.detach:
	push	edi
	push	dword [ebp+8]
	call	cdrom_image_detach
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
	push	eax
	mov	ecx,[ebp+8]
	call	cdrom_image_release_slice_lock
	pop	eax
	pop	edi
	pop	esi
	pop	ebp
	ret	8
cdrom_image_get_geometry:	
	push	esi
	push	edi
	mov	edi,[esp+8+8]
	mov	esi,[edi + IRP.Tail + IRP_TAIL_OVERLAY.CurrentStackLocation]
	cmp	dword [esi + IO_STACK_LOCATION.Parameters + IOSL_PARAMETERS_DEVICEIOCONTROL.OutputBufferLength],byte DISK_GEOMETRY_size
	mov	eax,STATUS_BUFFER_TOO_SMALL
	jb	.exit
	mov	ecx,[esp+8+4]
	mov	ecx,[ecx + DEVICE_OBJECT.DeviceExtension]
	test	byte [ecx + VIRTUAL_CDROM_EXTENSION.flags],byte VIRTCD_ATTACHED
	mov	eax,STATUS_NO_MEDIA_IN_DEVICE
	jz	.exit
	mov	edx,[edi + IRP.UserBuffer]
	mov	eax,[ecx + VIRTUAL_CDROM_EXTENSION.total_size]
	mov	ecx,[ecx + VIRTUAL_CDROM_EXTENSION.total_size+4]
	shrd	eax,ecx,11
	shr	ecx,11
	mov	[edx + DISK_GEOMETRY.Cylinders],eax
	mov	[edx + DISK_GEOMETRY.Cylinders+4],ecx
	xor	eax,eax
	inc	eax
	mov	dword [edx + DISK_GEOMETRY.MediaType],11
	mov	[edx + DISK_GEOMETRY.TracksPerCylinder],eax
	mov	[edx + DISK_GEOMETRY.SectorsPerTrack],eax
	mov	dword [edx + DISK_GEOMETRY.BytesPerSector],2048
	push	byte DISK_GEOMETRY_size
	pop	dword [edi + IRP.IoStatus + IO_STATUS_BLOCK.Information]
	xor	eax,eax
.exit:
	pop	edi
	pop	esi
	ret	8
cdrom_image_read_toc:		
	push	esi
	push	edi
	mov	edi,[esp+8+8]
	mov	esi,[edi + IRP.Tail + IRP_TAIL_OVERLAY.CurrentStackLocation]
	mov	ecx,[esp+8+4]
	mov	ecx,[ecx + DEVICE_OBJECT.DeviceExtension]
	test	byte [ecx + VIRTUAL_CDROM_EXTENSION.flags],byte VIRTCD_ATTACHED
	mov	eax,STATUS_NO_MEDIA_IN_DEVICE
	jz	.exit
	mov	ecx,[esi + IO_STACK_LOCATION.Parameters + IOSL_PARAMETERS_DEVICEIOCONTROL.OutputBufferLength]
	cmp	ecx,byte .FAKETOC_SIZE
	mov	eax,STATUS_BUFFER_TOO_SMALL
	jb	.exit
	cmp	ecx,804
	jbe	.buf_ok
	mov	ecx,804
.buf_ok:	
	mov	[edi + IRP.IoStatus + IO_STATUS_BLOCK.Information],ecx
	mov	edi,[edi + IRP.UserBuffer]
.ptoc:	mov	esi,.faketoc - cdrom_image_patch
	movsd
	movsd
	movsd
	xor	eax,eax
.exit:
	pop	edi
	pop	esi
	ret	8
.faketoc	db 0x00,0x12,0x01,0x01,0x00,0x14,0x01,0x00,0x00,0x00,0x02,0x00
.FAKETOC_SIZE	equ $-.faketoc
cdrom_image_attach:
	push	ebx
	push	esi
	mov	esi,[esp+8+4]
.partd:	mov	ebx,8003ae60h
	mov	ecx,[esi + DEVICE_OBJECT.DeviceExtension]
	test	byte [ecx + VIRTUAL_CDROM_EXTENSION.flags],byte VIRTCD_ATTACHED
	jz	.skipdetach
	push	esi
	call	detach_virtual_disc
	push	dword [esp+8+8]
	push	esi
	call	process_user_attach
	test	eax,eax
	jl	.errdet
	push	esi
	call	attach_virtual_disc
	test	eax,eax
	jge	.exit
.errdet:
	push	eax
	call	cdrom_image_detach_and_unlink
	pop	eax
	jmp	short .exit
.skipdetach:
	push	dword [esp+8+8]
	push	esi
	call	process_user_attach
	test	eax,eax
	jl	.exit
	push	dword [ebx]
	VCALL	IoDismountVolume_1
	cmp	eax,STATUS_VOLUME_DISMOUNTED
	je	.dismok2
	test	eax,eax
	jl	.exit
.dismok2:	
.symcd:	push	800123cch
	VCALL	IoDeleteSymbolicLink_1
	push	dword [ebx]
	VCALL	IoDeleteDevice_1
	test	eax,eax
	jl	.exit
	mov	[ebx],esi
	mov	ecx,esi
	call	cdrom_image_attach_and_link
	xor	eax,eax
.exit:
	pop	esi
	pop	ebx
	ret	8
cdrom_image_detach:	
	mov	ecx,[esp+4]
	mov	ecx,[ecx + DEVICE_OBJECT.DeviceExtension]
	test	byte [ecx + VIRTUAL_CDROM_EXTENSION.flags],byte VIRTCD_ATTACHED
	jz	.exit_success
	mov	edx,[esp+8]
	and	dword [edx + IRP.IoStatus + IO_STATUS_BLOCK.Information],byte 0
	mov	ecx,[esp+4]
	call	cdrom_image_detach_and_unlink
.exit_success:	
	xor	eax,eax	
.exit:
	ret	8
process_user_attach:
	push	ebp
	mov	ebp,esp
	push	ebx	
	push	esi
	push	edi
	mov	edi,[ebp+0Ch]
	mov	esi,[edi + IRP.Tail + IRP_TAIL_OVERLAY.CurrentStackLocation]
	mov	ecx,[esi + IO_STACK_LOCATION.Parameters + IOSL_PARAMETERS_DEVICEIOCONTROL.InputBufferLength]
	cmp	ecx,byte 4
	mov	eax,STATUS_INVALID_PARAMETER
	jb	.exit
	mov	esi,[esi + IO_STACK_LOCATION.Parameters + IOSL_PARAMETERS_DEVICEIOCONTROL.InputBuffer]
	mov	edx,[esi + ATTACH_USER_INPUT.num_slices]
	test	edx,edx
	jz	.exit
	cmp	edx,byte MAX_IMAGE_SLICES
	ja	.exit
	lea	edx,[OBJECT_STRING_size*edx + ATTACH_USER_INPUT.slice_files]
	cmp	ecx,edx
	jb	.exit
	and	dword [edi + IRP.IoStatus + IO_STATUS_BLOCK.Information],byte 0
	mov	ecx,[esi + ATTACH_USER_INPUT.num_slices]
	lea	eax,[esi + ATTACH_USER_INPUT.slice_files]
.sfbuf:	mov	edi,slice_filenames-cdrom_image_patch
	push	ecx
	push	edi
	lea	edx,[edi + IMAGE_SLICE_FILES.slice_files]
	lea	ebx,[IMAGE_SLICE_FILES.slice_files + OBJECT_STRING_size*ecx]
	add	edi,ebx
.copy_slice_files:
	movzx	esi,word [eax + OBJECT_STRING.Length]
	add	ebx,esi
	cmp	ebx,SLICE_FILENAMES_SIZE
	ja	.outofmem
	push	ecx
	mov	ecx,esi
	mov	[edx + OBJECT_STRING.Length],cx
	mov	[edx + OBJECT_STRING.MaximumLength],cx
	mov	[edx + OBJECT_STRING.Buffer],edi
	mov	esi,[eax + OBJECT_STRING.Buffer]
	push	ecx
	shr	ecx,2
	rep	movsd
	pop	ecx
	and	ecx,byte 3
	rep	movsb
	pop	ecx
	add	eax,byte OBJECT_STRING_size
	add	edx,byte OBJECT_STRING_size
	dec	ecx
	jnz	.copy_slice_files
	pop	edi
	xor	eax,eax
	pop	dword [edi + IMAGE_SLICE_FILES.num_slices]
.exit:
	pop	edi
	pop	esi
	pop	ebx
	pop	ebp
	ret	8
.outofmem:
	pop	edi
	pop	ecx
	and	dword [edi + IMAGE_SLICE_FILES.num_slices],byte 0
	mov	eax,STATUS_INSUFFICIENT_RESOURCES
	jmp	short .exit
attach_virtual_disc:
.opened_count:		equ 0 - 4
.slice_count:		equ .opened_count - 4
.io_status		equ .slice_count - IO_STATUS_BLOCK_size
.open_info		equ .io_status - FILE_NETWORK_OPEN_INFORMATION_size
.local_var_size		equ -.open_info
	;; [esp+4] = Virtual CD-ROM device object address
	push	ebp
	mov	ebp,esp
	sub	esp,byte .local_var_size
	push	ebx
	push	esi
	push	edi
.sfbuf:	mov	esi,slice_filenames-cdrom_image_patch
	mov	edx,[esi + IMAGE_SLICE_FILES.num_slices]
	test	edx,edx
	mov	eax,STATUS_NO_MEDIA_IN_DEVICE
	jz	near .exit
	xor	eax,eax
	mov	[ebp + .opened_count],eax
	mov	[ebp + .slice_count],edx
	mov	edi,[ebp+8]
	mov	edi,[edi + DEVICE_OBJECT.DeviceExtension]
	lea	ebx,[edi + VIRTUAL_CDROM_EXTENSION.total_size]
	add	esi,byte IMAGE_SLICE_FILES.slice_files
	add	edi,byte VIRTUAL_CDROM_EXTENSION.slices
	mov	[ebx],eax
	mov	[ebx+4],eax
.open_loop:		
	push	byte 40h
	push	esi
	push	byte 0
	mov	edx,esp		; Object attributes
	push	848h
	push	byte 1
	push	byte 1
	push	byte 0
	push	byte 0
	lea	eax,[ebp + .io_status]
	push	eax
	push	edx
	push	20001h
	push	edi
	VCALL	NtCreateFile_1	; call NtCreateFile
	add	esp,byte 12
	test	eax,eax
	jl	near .err
	inc	dword [ebp + .opened_count]
	lea	eax,[edi + IMAGE_SLICE.file_object]
	push	eax
	VPUSH	IoFileObjectType_1
	push	dword [edi + IMAGE_SLICE.file_handle]
	VCALL	ObReferenceObjectByHandle_1
	test	eax,eax
	jge	.refok
	and	dword [edi + IMAGE_SLICE.file_object],byte 0
	jmp	short .err
.refok:
	push	byte FileNetworkOpenInformation
	push	byte FILE_NETWORK_OPEN_INFORMATION_size
	lea	eax,[ebp + .open_info]
	push	eax
	lea	eax,[ebp + .io_status]
	push	eax
	push	dword [edi + IMAGE_SLICE.file_handle]
	VCALL	NtQueryInformationFile_1
	test	eax,eax
	jl	.err
	mov	eax,[ebp + .open_info + FILE_NETWORK_OPEN_INFORMATION.EndOfFile]
	mov	edx,[ebp + .open_info + FILE_NETWORK_OPEN_INFORMATION.EndOfFile+4]
	and	eax,~0x7ff
	jnz	.lenok
	test	edx,edx
	jz	.err
.lenok:	
	mov	[edi + IMAGE_SLICE.size],eax
	mov	[edi + IMAGE_SLICE.size+4],edx
	add	[ebx],eax
	adc	[ebx+4],edx
	add	esi,byte OBJECT_STRING_size
	add	edi,byte IMAGE_SLICE_size
	dec	dword [ebp + .slice_count]
	jnz	.open_loop
	mov	edi,[ebp+8]
	mov	eax,[ebp + .opened_count]
	mov	ecx,edi
	mov	edi,[edi + DEVICE_OBJECT.DeviceExtension]
	mov	dword [edi + VIRTUAL_CDROM_EXTENSION.num_slices],eax
	;; Set scatter/gather flag in case we have only one slice. With several
	;; slices this is not possible since the sector size 2048 is too fine
	;; for 4096 byte pages. Actually it's possible with some added complexity
	;; when a page spans a slice boundary, but let's not bother with it.
	;; 
	;; The setting will propagate to the file system driver when it's mounted
	;; (any current file system will obviously need to be dismounted anyway.)
	and	byte [ecx + DEVICE_OBJECT.Flags],byte ~40h
	cmp	eax,byte 1
	ja	.noscattergather
	or	byte [ecx + DEVICE_OBJECT.Flags],byte 40h
.noscattergather:	
	or	dword [edi + VIRTUAL_CDROM_EXTENSION.flags],byte VIRTCD_ATTACHED
	;; ecx = our device object
	call	cdrom_image_allow_read_users
	call	cdrom_image_disc_load
.exit_success:	
	xor	eax,eax
.exit:	
	pop	edi
	pop	esi
	pop	ebx
	leave
	ret	4
.err:	
	mov	edi,[ebp+8]
	mov	edi,[edi + DEVICE_OBJECT.DeviceExtension]
	add	edi,byte VIRTUAL_CDROM_EXTENSION.slices
	mov	ebx,[ebp + .opened_count]
	test	ebx,ebx
	jz	.close_done
.close_loop:
	mov	ecx,[edi + IMAGE_SLICE.file_object]
	jecxz	.noderef
	VCALL	ObfDereferenceObject_1
.noderef:	
	push	dword [edi + IMAGE_SLICE.file_handle]
	VCALL	NtClose_1
	add	edi,byte IMAGE_SLICE_size
	dec	ebx
	jnz	.close_loop
.close_done:
	mov	eax,STATUS_INVALID_PARAMETER
	jmp	short .exit
detach_virtual_disc:
	push	ebp
	mov	ebp,esp
	push	ebx
	push	edi
	mov	edi,[ebp+8]
	mov	edi,[edi + DEVICE_OBJECT.DeviceExtension]
	test	byte [edi + VIRTUAL_CDROM_EXTENSION.flags],byte VIRTCD_ATTACHED
	jz	.exit_success
	call	cdrom_image_disc_unload
	mov	ecx,[ebp+8]
	call	cdrom_image_clear_read_users
	and	byte [edi + VIRTUAL_CDROM_EXTENSION.flags],byte ~VIRTCD_ATTACHED
	mov	ebx,[edi + VIRTUAL_CDROM_EXTENSION.num_slices]
	test	ebx,ebx
	jz	.close_done
	add	edi,byte VIRTUAL_CDROM_EXTENSION.slices
.close_loop:
	mov	ecx,[edi + IMAGE_SLICE.file_object]
	jecxz	.noderef
	VCALL	ObfDereferenceObject_1
.noderef:	
	push	dword [edi + IMAGE_SLICE.file_handle]
	VCALL	NtClose_1
	add	edi,byte IMAGE_SLICE_size
	dec	ebx
	jnz	.close_loop
.close_done:
.sfbuf:	mov	eax,slice_filenames-cdrom_image_patch
	and	dword [eax + IMAGE_SLICE_FILES.num_slices],byte 0
.exit_success:	
	xor	eax,eax
.exit:	
	pop	edi
	pop	ebx
	pop	ebp
	ret	4
cdrom_image_return_success:
	xor	edx,edx
	mov	ecx,[esp+8]
	mov	[ecx + IRP.IoStatus + IO_STATUS_BLOCK.Status],edx
	VCALL	IofCompleteRequest_1
	xor	eax,eax
	ret	8
align 4
cdrom_image_driver_object:
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
;;; --------------------------------------------------------------------------
;;; Read/write variables
;;; --------------------------------------------------------------------------
align 4
TRAY_STATE_NO_DISC	equ 0x40
TRAY_STATE_DISC_LOADED	equ 0x60
tray_state:		dd TRAY_STATE_NO_DISC
tray_state_changes:	dd 0
align 4
slice_filenames:		times SLICE_FILENAMES_SIZE db 0
align 16
CDROM_IMAGE_PATCH_SIZE	equ $-cdrom_image_patch
%else 	; !VIRTUAL_CDROM_DRIVE
CDROM_IMAGE_PATCH_SIZE	equ 0
%endif 	; !VIRTUAL_CDROM_DRIVE