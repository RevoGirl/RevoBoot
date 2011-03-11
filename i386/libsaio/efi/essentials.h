/*
 * Copyright (c) 2005 Apple Computer, Inc. All rights reserved.
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
 * EFI implementation for Revolution Copyright (c) 2010 by DHP.
 * All rights reserved.
 *
 */

#ifndef __LIBSAIO_EFI_ESSENTIALS_H
#define __LIBSAIO_EFI_ESSENTIALS_H

// #include "../config/settings.h"
#include "../boot2/debug.h"

typedef void      VOID;

typedef uint32_t  EFI_STATUS;

typedef uint8_t   EFI_BOOLEAN;

typedef int8_t    EFI_INT8;
typedef int16_t   EFI_INT16;
typedef int32_t   EFI_INT32;
typedef int64_t   EFI_INT64;

typedef uint8_t   EFI_UINT8;
typedef uint16_t  EFI_UINT16;
typedef uint32_t  EFI_UINT32;
typedef uint64_t  EFI_UINT64;

typedef uint32_t  EFI_UINTN;					// Natural size for firmware, not the kernel.

typedef int8_t    EFI_CHAR8;
typedef int16_t   EFI_CHAR16;
typedef int32_t   EFI_CHAR32;
typedef int64_t   EFI_CHAR64;

typedef uint32_t  EFI_PTR32;
typedef uint64_t  EFI_PTR64;

typedef uint32_t  EFI_HANDLE32;
typedef uint64_t  EFI_HANDLE64;


typedef struct EFI_GUID
{
	EFI_UINT32	Data1;
	EFI_UINT16	Data2;
	EFI_UINT16	Data3;
	EFI_UINT8	Data4[8];
} EFI_GUID;


#if EFI_64_BIT
	typedef uint64_t		EFI_UINT;
	typedef uint64_t		EFI_PTR;
	typedef uint64_t		EFI_HANDLE;
#else
	typedef uint32_t		EFI_UINT;
	typedef uint32_t		EFI_PTR;
	typedef uint32_t		EFI_HANDLE;
#endif

#endif /* !__LIBSAIO_EFI_ESSENTIALS_H */
