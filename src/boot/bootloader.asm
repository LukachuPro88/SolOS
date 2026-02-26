;-----------------------------------------
; SolOS
; Copyright (C) 2026 Luka Saarivirta
;
; Licensed under the GNU GPL v2.
; See LICENSE file in project root for full license text.
;-----------------------------------------


;=========================================
; boot/bootloader.asm
; Bootloader with Protected Mode Setup
; Assembled at 0x7C00 for BIOS
; Supports: Loading kernel, basic GDT, protected mode switch
;=========================================

[org 0x7C00]      ; BIOS loads bootloader here
[bits 16]         ; Real mode (16-bit)

;-----------------------------------------
; 1 | Disable interrupts before initialization
;-----------------------------------------
cli                ; Disable interrupts temporarily
jmp 0x0000:fix_cs  ; Jump to segment initialization routine

;-----------------------------------------
; 2 | Segment register setup & stack
;-----------------------------------------
fix_cs:
    xor ax, ax      ; Clear AX
    mov ds, ax      ; Data segment = 0
    mov es, ax      ; Extra segment = 0

    mov ax, 0x9000  ; Stack segment
    mov ss, ax
    mov sp, 0xFFFF  ; Stack pointer at top of segment

    mov [boot_drive], dl ; Save BIOS boot drive number
    sti                 ; Re-enable interrupts

;-----------------------------------------
; 3 | Simple bootloader debug output
;-----------------------------------------
; Print 'B' to confirm bootloader running
mov ah, 0x0E         ; BIOS teletype output
mov al, 'B'
int 0x10             ; Call BIOS video interrupt

; Set cursor shape for visual feedback
mov ah, 0x01
mov ch, 0x0D
mov cl, 0x0F
int 0x10

;-----------------------------------------
; 4 | Disk reset & kernel loading
;-----------------------------------------
mov ah, 0x00         ; Reset disk
mov dl, [boot_drive]
int 0x13

; Load kernel from disk into memory at 0x1000
mov ah, 0x02         ; Read sectors
mov al, 32           ; Number of sectors (example)
mov ch, 0            ; Cylinder
mov cl, 2            ; Sector
mov dh, 0            ; Head
mov dl, [boot_drive]
mov bx, 0x1000       ; Destination address
int 0x13
jc disk_error        ; Jump if carry flag set (error)

; Print 'L' to confirm load success
mov ah, 0x0E
mov al, 'L'
int 0x10

;-----------------------------------------
; 5 | Setup GDT (Global Descriptor Table)
;-----------------------------------------
lgdt [gdt_descriptor] ; Load GDT into GDTR

;-----------------------------------------
; 6 | Enable protected mode
;-----------------------------------------
cli                   ; Disable interrupts during mode switch
mov eax, cr0
or eax, 1             ; Set PE (Protected Mode Enable) bit
mov cr0, eax

jmp 0x08:protected_mode ; Far jump to code segment in protected mode

;-----------------------------------------
; Disk error handler
;-----------------------------------------
disk_error:
    mov ah, 0x0E
    mov al, 'E'       ; Print E for disk error
    int 0x10
    hlt               ; Stop CPU

;-----------------------------------------
; 7 | Protected mode setup (32-bit)
;-----------------------------------------
[bits 32]
protected_mode:
    ; Load data segments
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov esp, 0x90000       ; Stack in protected mode

    ; Example: write directly to video memory
    mov dword [0xB8000], 0x0F500F42

    jmp 0x1000             ; Jump to kernel start

;-----------------------------------------
; 8 | Boot drive storage
;-----------------------------------------
boot_drive: db 0

;-----------------------------------------
; 9 | GDT (Global Descriptor Table) setup
;-----------------------------------------
gdt_start:
    dq 0                   ; Null descriptor

gdt_code:
    dw 0xFFFF, 0x0000
    db 0x00, 0x9A, 0xCF, 0x00  ; Code segment: executable, readable

gdt_data:
    dw 0xFFFF, 0x0000
    db 0x00, 0x92, 0xCF, 0x00  ; Data segment: writable

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1  ; Limit
    dd gdt_start                ; Base

;-----------------------------------------
; 10 | Bootloader padding & signature
;-----------------------------------------
times 510-($-$$) db 0  ; Fill to 510 bytes
dw 0xAA55              ; BIOS boot signature