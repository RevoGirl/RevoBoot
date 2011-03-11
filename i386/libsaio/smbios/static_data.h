/*
 * Copyright 2009 by Master Chief.
 *
 * Updates:
 *			- Dynamic and static SMBIOS data gathering added by DHP in 2010.
 *
 * Credits:
 *			- blackosx, DB1, dgsga, FKA, humph, scrax and STLVNUB (testers).
 */

#ifndef __LIBSAIO_SMBIOS_STATIC_DATA_H
#define __LIBSAIO_SMBIOS_STATIC_DATA_H

#include "essentials.h"

#define INCLUDE_SMBIOS_DATA		1
#include "../../config/data.h"

static uint32_t SMBIOS_Table[] = 
{
	STATIC_SMBIOS_DATA	// Will get replaced with the data from: config/smbios/data.h
};

#endif /* !__LIBSAIO_SMBIOS_STATIC_DATA_H */
