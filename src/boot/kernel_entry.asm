;-----------------------------------------
; SolOS
; Copyright (C) 2026 Luka Saarivirta
;
; Licensed under the GNU GPL v2.
; See LICENSE file in project root for full license text.
;-----------------------------------------


;=========================================
; boot/kernel_entry.asm
; Entry point for the kernel in protected mode
;=========================================

[bits 32]
[extern kernel_main]

global _start

_start:
    call kernel_main   ; Jump to kernel main
halt_loop:
    hlt                ; Halt CPU
    jmp halt_loop      ; Infinite loop if kernel_main returns