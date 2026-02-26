;-----------------------------------------
; SolOS
; Copyright (C) 2026 Luka Saarivirta
;
; Licensed under the GNU GPL v2.
; See LICENSE file in project root for full license text.
;-----------------------------------------


;=========================================
; boot/multiboot.asm
; Multiboot header for GRUB bootloader
;=========================================

section .multiboot
align 4

dd 0x1BADB002           ; Magic number identifying multiboot-compliant kernel
dd 0x00000000           ; Flags (currently none set; can request memory info, module alignment, etc.)
dd -(0x1BADB002 + 0x00000000) ; Checksum so sum of magic, flags, and checksum = 0