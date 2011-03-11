/*! @file       efi_tables.h
    @abstract   Utility functions for dealing with EFI tables
    Copyright 2007 David F. Elliott.  All rights reserved.
 */
#ifndef _LIBSA_EFI_TABLES_H__
#define _LIBSA_EFI_TABLES_H__

#include "efi/essentials.h"

uint32_t crc32(uint32_t crc, const void *buf, size_t size);

void efi_guid_unparse_upper(EFI_GUID const *pGuid, char *out);
bool efi_guid_is_null(EFI_GUID const *pGuid);
int efi_guid_compare(EFI_GUID const *pG1, EFI_GUID const *pG2);

#endif //ndef _LIBSA_EFI_TABLES_H__
