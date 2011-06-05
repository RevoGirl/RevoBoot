/*
 * Copyright (c) 2009 by Master Chief.
 *
 * Refactored (static and dynamic CPU data gathering) by DHP in 2010.
 */

#ifndef __LIBSAIO_CPU_STATIC_CPU_DATA_H
#define __LIBSAIO_CPU_STATIC_CPU_DATA_H

//==============================================================================

void initCPUStruct(void)
{
	gPlatform.CPU.Vendor			= STATIC_CPU_Vendor;		// machdep.cpu.vendor "GenuineIntel"
	gPlatform.CPU.Signature			= STATIC_CPU_Signature;		// machdep.cpu.signature
	gPlatform.CPU.Stepping			= STATIC_CPU_Stepping;		// machdep.cpu.stepping
	gPlatform.CPU.Model				= STATIC_CPU_Model;			// machdep.cpu.model (see cpu/essentials.h)
	gPlatform.CPU.Family			= STATIC_CPU_Family;		// machdep.cpu.family
	gPlatform.CPU.ExtModel			= STATIC_CPU_ExtModel;		// machdep.cpu.extmodel
	gPlatform.CPU.ExtFamily			= STATIC_CPU_ExtFamily;		// machdep.cpu.extfamily

	gPlatform.CPU.Type				= STATIC_CPU_Type;			// Used in SMBIOS patcher.

	gPlatform.CPU.NumCores			= STATIC_CPU_NumCores;		// machdep.cpu.cores_per_package
	gPlatform.CPU.NumThreads		= STATIC_CPU_NumThreads;	// machdep.cpu.logical_per_package

	gPlatform.CPU.Features			= STATIC_CPU_Features;		// 

	gPlatform.CPU.TSCFrequency		= STATIC_CPU_TSCFrequency;	// 
	gPlatform.CPU.FSBFrequency		= STATIC_CPU_FSBFrequency;	// hw.busfrequency aka ((2261000 / 17) * 1000). 
	gPlatform.CPU.CPUFrequency		= STATIC_CPU_CPUFrequency;	// hw.cpufrequency.

	gPlatform.CPU.QPISpeed			= STATIC_CPU_QPISpeed;		// QuckPath Interconnect Speed (used in SMBIOS patcher).

	gPlatform.CPU.CoreTurboRatio[0]	= STATIC_CPU_MaxTurboMultiplier;

	requestMaxTurbo(STATIC_CPU_MaxMultiplier || 0);
}

#endif /* !__LIBSAIO_CPU_STATIC_CPU_DATA_H */