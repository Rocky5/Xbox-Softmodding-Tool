
;-----------------------------------------------------------------
;       SET FAN SPEED 
;-----------------------------------------------------------------
	push	word FAN_SPEED
 	push	byte 0
 	push	byte 6	   		; fan_register
 	push	byte 20h		; pic_address
 	call	dword [HalWriteSMBusValue]
 	push	byte 1
 	push	byte 0
 	push	byte 5 			; fan_mode
 	push	byte 20h		; pic_address
 	call	dword [HalWriteSMBusValue]
