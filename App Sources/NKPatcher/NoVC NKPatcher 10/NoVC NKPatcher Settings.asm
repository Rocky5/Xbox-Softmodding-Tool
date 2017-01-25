[ORG 0x10000]
[BITS 32]
; XBE HEADER PART ------------------------------------------------------------
db 	'XBEH'
times	0x100 db 0	; reserve 100 bytes for the xbe security signature
                        ; This only matters on signed xbe's which are
                        ; ms authorised :)
dd 	0x10000         ; Base address of image
dd	0x1000          ; Size of header
dd	EOF-$$		; Size of image
dd      0x178		; Size of image header
dd	0x3f16d3ce	; Time Date Stamp
dd	cert_header	; Certificate Address (plus offset)
dd	0x1		; Number of sections
dd	section_header	; Sectrion headers Address(plus offset)
dd	0x1		; Initialisation Flags
dd	(entry_point-$$+0x10000) ^ 0xA8FC57AB	
                        ; Entry point (XOR with 0xA8FC57AB)
                        ; ***** Very important - this will be our entry
                        ; point for our code - which is the base address of
                        ; our file added to the actual file offset.
dd	TLS		; Thread local storage directory address
dd	0x0		; Size of stack commit
dd	0x0		; Size of heap reserve
dd	0x0		; Size of heap commit
dd	0x0		; Original base address
dd	0x0		; Original size of image
dd	0x0		; Original checksum
dd	0x3f16d3ce	; Original time date stamp
dd	debugpath	; Debug path name address
dd	debugpath
dd	szname
dd	(kernel_thunk-$$+0x10000) ^ 0x5B6D40B6		
                        ; Kernel image thunk address
                        ; We XOR the original address with 0x5B6D40B6
dd	0x0		; Non-kernel import directory address
dd	0x0		; Number of library versions
dd	0x0		; Library versions area addresses
dd	0x0		; Kernel library version address
dd	0x0		; XAPI library version address
dd	0		; Logo bitmap address
dd	0	        ; Logo bitmap size
                        ; Not included a logo bitmap for this simple example
                        ; and will still run without it, but you could encode
                        ; a really cool image logo for you app maybe :)
; Note: - a lot of assemblers use "[ORG 0x354]" to align data within the assembled
;         file, but for nasm, you have to use: TIMES 0x178-($-$$), which implies
;         that everything from this line is from 0x178 in the file.
; XBE CERTIFICATION ----------------------------------------------------------
TIMES 0x178-($-$$) DB 0 ; I've aligned the certificate to exactly 0x178 in
                        ; the file, but you could exclude this if you want.
cert_header:
dd	0x1dc		; Size of Certification
dd	0x3f16d3ce	; Date Stamp
dd	0x0		; Title ID
title_xbe: 
      dw  'N','K','P','a','t','c','h','e','r',' ','S','e','t','t','i','n','g','s',
times	0x50-$+title_xbe db 0	; Title name null terminated string
times	0x40 db 0	; Alt Title
dd	0x800000FF	; Allowed Media
dd	0x80000007	; Game Region
dd	-1		; Game Rating
dd	0x0		; Disk Number
dd	0x0		; version
times	0x10 db 0	; Lan
times	0x10 db 0	; Signature Key
times	0x100 db 0	; Alt Sig
; SECTION HEADERS ------------------------------------------------------------
TIMES 0x354-($-$$) DB 0 ; Similar to the xbe cert header, I've aligned the
                        ; single section header to offset 0x354 exactly, but
                        ; you don't have to.
section_header:
dd	0x07			; Flags
dd	section_1_start		; Virtual Address (remember offset of 0x10000)
dd	EOF-section_1_start	; Virutal Size
dd	section_1_start-$$	; File pointer to raw data
dd	EOF-section_1_start	; Size of raw data
dd	section_name		; Address of the section name (Null terminated)
dd	0x0			;
dd	rc1			; head_count_address 
dd	rc2			; tail_count_address 
times	20 db 0			; section digest
debugpath:
	times 16 db 0
szname:
	dw 'x','b','d','e','v',0,0,0
rc1:
  dd 0
rc2:
  dd 0
section_name:
  db '.','t','e','x','t',0,0
; 'OUR CODE'
; THE SECTION WE DEFINED -----------------------------------------------------
; This is the start of our section.. the start of our simple
; code.
; Its offset in the file is at 0x1000
TIMES 0x1000-($-$$) DB 0
section_1_start:		; this should be 0x11000 in memory when loaded
entry_point:
	jmp	near start
	align 4