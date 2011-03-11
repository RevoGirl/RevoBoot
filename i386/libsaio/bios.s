/*
 * Copyright (c) 1999-2003 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * Portions Copyright (c) 1999-2003 Apple Computer, Inc.  All Rights
 * Reserved.  This file contains Original Code and/or Modifications of
 * Original Code as defined in and that are subject to the Apple Public
 * Source License Version 2.0 (the "License").  You may not use this file
 * except in compliance with the License.  Please obtain a copy of the
 * License at http://www.apple.com/publicsource and read it before using
 * this file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE OR NON- INFRINGEMENT.  Please see the
 * License for the specific language governing rights and limitations
 * under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */
/*
 * Copyright 1993 NeXT Computer, Inc.
 * All rights reserved.
 *
 * Harness for calling real-mode BIOS functions.
 */

/*  Copyright 2007 David Elliott
    2007-12-30 dfe
    - Enhanced code to use specified DS register when doing BIOS interrupt
    - Fixed movl %ax,new_es bug which assembler was interpreting as
      movl %eax,new_es which was overwriting the next word.
 */
#include <architecture/i386/asm_help.h>
#include "memory.h"

#define data32  .byte 0x66
#define addr32  .byte 0x67

#define O_INT   0
#define O_EAX   4
#define O_EBX   8
#define O_ECX   12
#define O_EDX   16
#define O_EDI   20
#define O_ESI   24
#define O_EBP   28
#define O_CS    32
#define O_DS    34
#define O_ES    36
#define O_FLG   38

    .section __INIT,__data	// turbo - Data that must be in the first segment

/*  Saved registers:
    These used to be (and in theory ought to be) located in __DATA,__bss.
    The problem is that the larger the binary grows, more of the BSS gets
    pushed into the next real-mode segment.  Doing it this way we waste 24
    bytes in the binary that our loader (e.g. boot1) must now load.  But the
    advantage is that we relocate this data to ensure it stays in the first
    real-mode segment.  Therefore, depending on link order, quite a lot of
    new data, and possibly a lot of new executable code can be added to the
    binary since with this change the BSS and most of the DATA is now only
    accessed from protected mode where real-mode segment limits don't apply.

    With this change, plus the earlier change to respect DS (e.g. use huge
    pointers), the binary can grow much larger, currently up to exactly 63.5k
    which is the maximum that the first-stage bootsectors can handle.  To get
    more than that more changes are needed.  In that case we would have to
    play with Mach-O segments to ensure real-mode code and data got stuffed
    well within the first 63.5k.  Furthermore, we'd have to adjust the boot
    sectors to allow them to span segments.

    Since this change alone only gains us about 4k more than where we're at
    now (which is not anything to scoff at) it won't be very long before we
    need to start using Mach-O segments to force the linker to locate certain
    bits of code and data within the first 63.5k and modify the loaders to
    be able to load more than 63.5k.
 */
    .align 2
save_eax:   .space 4
    .align 2
save_edx:   .space 4
    .align 1
save_es:    .space 2
    .align 1
save_flag:  .space 2
    .align 2
new_eax:    .space 4
    .align 2
new_edx:    .space 4
    .align 1
new_es:     .space 2
    .align 1
new_ds:     .space 2

    .section __INIT,__text      // turbo - This code must reside within the first segment


/*============================================================================
 * Call real-mode BIOS INT functions.
 *
 */
LABEL(_bios)
    enter   $0, $0
    pushal

    movl    8(%ebp), %edx       // address of save area
    movb    O_INT(%edx), %al    // save int number
    movb    %al, do_int+1

    movl    O_EBX(%edx), %ebx
    movl    O_ECX(%edx), %ecx
    movl    O_EDI(%edx), %edi
    movl    O_ESI(%edx), %esi
    movl    O_EBP(%edx), %ebp
    movl    %edx, save_edx
    movl    O_EAX(%edx), %eax
    movl    %eax, new_eax
    movl    O_EDX(%edx), %eax
    movl    %eax, new_edx
    movw    O_ES(%edx),  %ax
    movw    %ax, new_es
    movw    O_DS(%edx),  %ax
    movw    %ax, new_ds

    call    __prot_to_real

    data32
    addr32
    mov     OFFSET16(new_eax), %eax
    data32
    addr32
    mov     OFFSET16(new_edx), %edx
    data32
    addr32
    mov     OFFSET16(new_es), %es

    push    %ds     // Save DS
    // Replace DS. WARNING: Don't access data until it's restored!
    addr32
    data32
    mov     OFFSET16(new_ds), %ds

do_int:
    int     $0x00
    pop     %ds     // Restore DS before we do anything else

    pushf
    data32
    addr32
    movl    %eax, OFFSET16(save_eax)
    popl    %eax                         // actually pop %ax
    addr32
    movl    %eax, OFFSET16(save_flag)  // actually movw
    mov     %es, %ax
    addr32
    movl    %eax, OFFSET16(save_es)    // actually movw
    data32
    call    __real_to_prot

    movl    %edx, new_edx       // save new edx before clobbering
    movl    save_edx, %edx
    movl    new_edx, %eax       // now move it into buffer
    movl    %eax, O_EDX(%edx)
    movl    save_eax, %eax
    movl    %eax, O_EAX(%edx)
    movw    save_es, %ax
    movw    %ax, O_ES(%edx)
    movw    save_flag, %ax
    movw    %ax, O_FLG(%edx)
    movl    %ebx, O_EBX(%edx)
    movl    %ecx, O_ECX(%edx)
    movl    %edi, O_EDI(%edx)
    movl    %esi, O_ESI(%edx)
    movl    %ebp, O_EBP(%edx)

    popal
    leave

    ret
