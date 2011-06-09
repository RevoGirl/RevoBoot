/*
 * Copyright (c) 2009 by Master Chief.
 *
 * Refactored (static and dynamic CPU data gathering) by DHP in 2010.
 * Expanded (requestMaxTurbo added) by DHP in May 2011.
 * Simplified by DHP in Juni 2011 (thanks to MC and flAked for the idea).
 */

#ifndef __LIBSAIO_CPU_STATIC_CPU_DATA_H
#define __LIBSAIO_CPU_STATIC_CPU_DATA_H

//==============================================================================

void initCPUStruct(void)
{
	gPlatform.CPU.Type			= STATIC_CPU_Type;			// Used in SMBIOS/dynamic_data.h, 'About This Mac' and System Profiler.

	gPlatform.CPU.NumCores		= STATIC_CPU_NumCores;		// machdep.cpu.cores_per_package - used in: ACPI/ssdt_pm_generator.h
	gPlatform.CPU.NumThreads	= STATIC_CPU_NumThreads;	// machdep.cpu.logical_per_package - used in: ACPI/ssdt_pm_generator.h

	gPlatform.CPU.FSBFrequency	= STATIC_CPU_FSBFrequency;	// hw.busfrequency - very important to get this right!

	gPlatform.CPU.QPISpeed		= STATIC_CPU_QPISpeed;		// QuickPath Interconnect - used in: libsaio/SMBIOS/dynamic_data.h

	requestMaxTurbo(0);
}

#endif /* !__LIBSAIO_CPU_STATIC_CPU_DATA_H */