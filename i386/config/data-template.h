/*
 * Copyright (c) 2009 Master Chief.
 * Refactored by DHP in 2010-2011.
 */


#if PATCH_ACPI_TABLE_DATA && INCLUDE_ACPI_DATA
	#undef INCLUDE_ACPI_DATA
	#include "acpi/data.h"

#elif INJECT_EFI_DEVICE_PROPERTIES && INCLUDE_EFI_DATA
	#undef INCLUDE_EFI_DATA
	#include "efi/data.h"

#elif USE_STATIC_SMBIOS_DATA && INCLUDE_SMBIOS_DATA
	#undef INCLUDE_SMBIOS_DATA
	#include "smbios/data.h"

#endif
