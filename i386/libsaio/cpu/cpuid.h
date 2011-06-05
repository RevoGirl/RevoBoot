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
 *
 * Refactoring for Revolution done by DHP in 2010. Original copyright header 
 * also reinstated.
 *
 */

#ifndef __LIBSAIO_CPU_CPUID_H
#define __LIBSAIO_CPU_CPUID_H


#define getCachedCPUID(leaf, reg)	gPlatform.CPU.ID[leaf][reg]


//==============================================================================
// Copied from: xnu/osfmk/cpuid.h

typedef enum
{
	eax,
	ebx,
	ecx,
	edx
} cpuid_register_t;


//==============================================================================
// Copied from: xnu/osfmk/i386/cpuid.h

static inline void cpuid(uint32_t * data)
{
	asm volatile("cpuid" 
				 :	"=a" (data[eax]),
					"=b" (data[ebx]),
					"=c" (data[ecx]),
					"=d" (data[edx])
				 :	"a"  (data[eax]),
					"b"  (data[ebx]),
					"c"  (data[ecx]),
					"d"  (data[edx])
	);
}


//==============================================================================
// Copied from: xnu/osfmk/i386/cpuid.h

static inline void do_cpuid(uint32_t selector, uint32_t * data)
{
	asm volatile("cpuid"
				 :	"=a" (data[eax]),
					"=b" (data[ebx]),
					"=c" (data[ecx]),
					"=d" (data[edx])
				 :	"a"  (selector)
	);
}


//==============================================================================
// Copied from: chameleon/i386/libsaio/cpu.h 
// Copyright 2008 Islam Ahmed Zaid. All rights reserved.  <azismed@gmail.com> ?

static inline void do_cpuid2(uint32_t selector, uint32_t selector2, uint32_t * data)
{
	asm volatile("cpuid"
				 :	"=a" (data[eax]),
					"=b" (data[ebx]),
					"=c" (data[ecx]),
					"=d" (data[edx])
				 :	"a" (selector),
					"c" (selector2)
	);
}

#endif /* !__LIBSAIO_CPU_CPUID_H */
