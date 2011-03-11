/*
 * Copyright (c) 2000-2006 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_START@
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. The rights granted to you under the License
 * may not be used to create, or enable the creation or redistribution of,
 * unlawful or unlicensed copies of an Apple operating system, or to
 * circumvent, violate, or enable the circumvention or violation of, any
 * terms of an Apple operating system software license agreement.
 * 
 * Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_OSREFERENCE_LICENSE_HEADER_END@
 */
/*
 * @OSF_COPYRIGHT@
 */
/* CMU_ENDHIST */
/* 
 * Mach Operating System
 * Copyright (c) 1991,1990 Carnegie Mellon University
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 * 
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS "AS IS"
 * CONDITION.  CARNEGIE MELLON DISCLAIMS ANY LIABILITY OF ANY KIND FOR
 * ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
 * 
 * Carnegie Mellon requests users of this software to return to
 * 
 *  Software Distribution Coordinator  or  Software.Distribution@CS.CMU.EDU
 *  School of Computer Science
 *  Carnegie Mellon University
 *  Pittsburgh PA 15213-3890
 * 
 * any improvements or extensions that they make and grant Carnegie Mellon
 * the rights to redistribute these changes.
 */

 /*
  * DFE: enable_PIT2 and disable_PIT2 come from older XNU.
  */

#ifndef __LIBSAIO_CPU_PROC_REG_H
#define __LIBSAIO_CPU_PROC_REG_H


//==============================================================================

static inline uint64_t rdtsc64(void)
{
	uint64_t ret;
	
	__asm__ volatile("rdtsc" : "=A" (ret));

	return ret;
}


//==============================================================================

static inline uint64_t rdmsr64(uint32_t msr)
{
	uint64_t ret;

	__asm__ volatile("rdmsr" : "=A" (ret) : "c" (msr));

	return ret;
}


//==============================================================================

static inline void wrmsr64(uint32_t msr, uint64_t val)
{
	__asm__ volatile("wrmsr" : : "c" (msr), "A" (val));
}


//==============================================================================
/*
 * Enable or disable timer 2.
 * Port 0x61 controls timer 2:
 *   bit 0 gates the clock,
 *   bit 1 gates output to speaker.
 */
static inline void enable_PIT2(void)
{
    // Enable gate, disable speaker.
	__asm__ volatile(" inb   $0x61,%%al      \n\t"
					 " and   $0xFC,%%al       \n\t"  /* & ~0x03 */
					 " or    $1,%%al         \n\t"
					 " outb  %%al,$0x61      \n\t"
					 : : : "%al" );
}


//==============================================================================

static inline void disable_PIT2(void)
{
    // Disable gate and output to speaker.
	__asm__ volatile(" inb   $0x61,%%al      \n\t"
					 " and   $0xFC,%%al      \n\t"	/* & ~0x03 */
					 " outb  %%al,$0x61      \n\t"
					 : : : "%al" );
}

//==============================================================================
// DFE: set_PIT2_mode0, poll_PIT2_gate, and measure_tsc_frequency are roughly 
// based on Linux code.

static inline void set_PIT2_mode0(uint16_t value)
{
	/* Set the 8254 channel 2 to mode 0 with the specified value.
	 * In mode 0, the counter will initially set its gate low when the
	 * timer expires.  For this to be useful, you ought to set it high
	 * before calling this function.  The enable_PIT2 function does this.
	 */

	__asm__ volatile(" movb  $0xB0,%%al      \n\t"
					 " outb	%%al,$0x43	\n\t"
					 " movb	%%dl,%%al	\n\t"
					 " outb	%%al,$0x42	\n\t"
					 " movb	%%dh,%%al	\n\t"
					 " outb	%%al,$0x42"
					 : : "d"(value) /*: no clobber */ );
}


//==============================================================================
// Returns the number of times the loop ran before the PIT2 signaled.
 
static inline unsigned long poll_PIT2_gate(void)
{
	unsigned long count = 0;
	unsigned char nmi_sc_val;

    do
	{
		++count;
		__asm__ volatile("inb	$0x61,%0": "=q"(nmi_sc_val) /*:*/ /* no input */ /*:*/ /* no clobber */);
	} while( (nmi_sc_val & 0x20) == 0);

	return count;
}

#endif /* !__LIBSAIO_CPU_PROC_REG_H */
