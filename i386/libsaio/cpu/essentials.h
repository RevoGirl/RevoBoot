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

#ifndef __LIBSAIO_CPU_ESSENTIALS_H
#define __LIBSAIO_CPU_ESSENTIALS_H


/* Copied from xnu/osfmk/cpuid.c */
#define bit(n)				(1UL << (n))
#define bitmask32(h, l)		((bit(h) | (bit(h) - 1)) & ~ (bit(l) - 1))
#define bitfield32(x, h, l)	(((x) & bitmask32(h, l)) >> l)


// Added by DHP in 2010.
#define CPU_VENDOR_INTEL	0x756E6547
#define CPU_VENDOR_AMD		0x68747541


/* Copied from xnu/osfmk/cpuid.h */
#define CPU_STRING_UNKNOWN "Unknown CPU Typ"


 // Copied from xnu/osfmk/proc_reg.h
#define MSR_IA32_PLATFORM_ID	0x17
#define	MSR_CORE_THREAD_COUNT	0x35
#define	MSR_PLATFORM_INFO		0xCE
#define	MSR_IA32_PERF_STATUS	0x198	// MSR_IA32_PERF_STS in XNU
#define MSR_FLEX_RATIO			0x194
#define	MSR_TURBO_RATIO_LIMIT	0x1AD


// CPUID leaf index values (pointing to the right spot in CPUID/LEAF array).

#define LEAF_0				0			// DHP: Formely known as CPUID_n
#define LEAF_1				1
#define LEAF_2				2
#define LEAF_4				3
#define LEAF_B				4
#define LEAF_80				5
#define LEAF_81				6

#define MAX_CPUID_LEAVES	7			// DHP: Formely known as MAX_CPUID


/* Copied from: xnu/osfmk/cpuid.h */
#define CPU_MODEL_YONAH			0x0E
#define CPU_MODEL_MEROM			0x0F
#define CPU_MODEL_PENRYN		0x17
#define CPU_MODEL_NEHALEM		0x1A
#define CPU_MODEL_ATOM			0x1C
#define CPU_MODEL_FIELDS		0x1E	// Lynnfield, Clarksfield, Jasper
#define CPU_MODEL_DALES			0x1F	// Havendale, Auburndale
#define CPU_MODEL_DALES_32NM	0x25	// Clarkdale, Arrandale
#define CPU_MODEL_SB_CORE		0x2A	// Sandy Bridge Core Processors
#define CPU_MODEL_WESTMERE		0x2C	// Gulftown, Westmere-EP, Westmere-WS
#define CPU_MODEL_SB_XEON		0x2D	// Sandy Bridge Xeon Processors
#define CPU_MODEL_NEHALEM_EX	0x2E
#define CPU_MODEL_WESTMERE_EX	0x2F


#endif /* !__LIBSAIO_CPU_ESSENTIALS_H */
