/*
 * Copyright 2009 by Master Chief.
 *
 * Refactored (dynamic and static ACPI patching) by DHP in 2010/2011.
 */

#ifndef __LIBSAIO_ACPI_STATIC_DATA_H
#define __LIBSAIO_ACPI_STATIC_DATA_H

#define INCLUDE_ACPI_DATA	1

#include "../config/data.h"

// The STATIC_xxx_TABLE_DATA's here will get replaced with the data from private_data.h

// Static data overrides for essential tables.

static uint32_t APIC_Table[] = 
{
#if STATIC_APIC_TABLE_INJECTION
	STATIC_APIC_TABLE_DATA
#endif
};


static uint32_t ECDT_Table[] =
{
#if STATIC_ECDT_TABLE_INJECTION
	STATIC_ECDT_TABLE_DATA
#endif
};


static uint32_t HPET_Table[] =
{
#if STATIC_HPET_TABLE_INJECTION
	STATIC_HPET_TABLE_DATA
#endif
};


static uint32_t MCFG_Table[] = 
{
#if STATIC_MCFG_TABLE_INJECTION
	STATIC_MCFG_TABLE_DATA
#endif
};


static uint32_t SBST_Table[] = 
{
#if STATIC_SBST_TABLE_INJECTION
	STATIC_SBST_TABLE_DATA
#endif
};


static uint32_t SSDT_Table[] = 
{
#if STATIC_SSDT_TABLE_INJECTION
	STATIC_SSDT_TABLE_DATA
#endif
};


// Static data overrides for special-essential tables.

static uint32_t DSDT_Table[] =
{
#if STATIC_DSDT_TABLE_INJECTION
	STATIC_DSDT_TABLE_DATA
#endif
};


static uint32_t FACS_Table[] =
{
#if STATIC_FACS_TABLE_INJECTION
	STATIC_FACS_TABLE_DATA
#endif
};


// Static data overrides for optional tables.

static uint32_t APIC2_Table[] = 
{
#if STATIC_APIC2_TABLE_INJECTION
	STATIC_APIC2_TABLE_DATA
#endif
};



static uint32_t SSDT_GPU_Table[] =
{
#if STATIC_SSDT_GPU_TABLE_INJECTION
	STATIC_SSDT_GPU_TABLE_DATA
#endif
};


static uint32_t SSDT_PR_Table[] = 
{
#if STATIC_SSDT_PR_TABLE_INJECTION
	STATIC_SSDT_PR_TABLE_DATA
#endif
};


static uint32_t SSDT_SATA_Table[] = 
{
#if STATIC_SSDT_SATA_TABLE_INJECTION
	STATIC_SSDT_SATA_TABLE_DATA
#endif
};


static uint32_t SSDT_USB_Table[] = 
{
#if STATIC_SSDT_USB_TABLE_INJECTION
	STATIC_SSDT_USB_TABLE_DATA
#endif
};


#endif /* !__LIBSAIO_ACPI_STATIC_DATA_H */
