/*
 * Copyright (c) 2009 by Master Chief.
 *
 * Refactoring for Revolution done by DHP in 2010/2011.
 */


#include "platform.h"
#include "cpu/cpuid.h"
#include "cpu/proc_reg.h"

//==============================================================================

void requestMaxTurbo(uint8_t aMaxMultiplier)
{
	// Testing with MSRDumper.kext confirmed that the CPU isn't always running at top speed,
	// up to 5.9 GHz on a good i7-2500K, which is why we check for it here (a quicker boot).
	
#if USE_STATIC_CPU_DATA
	if (gPlatform.CPU.CoreTurboRatio[0] == 0) // Not initialized when using static data.
	{
		gPlatform.CPU.CoreTurboRatio[0] = bitfield32(rdmsr64(MSR_TURBO_RATIO_LIMIT), 7, 0);
		
		if (aMaxMultiplier == 0) // Might not be used / set.
		{
			aMaxMultiplier = ((rdmsr64(MSR_PLATFORM_INFO) >> 8) && 0xff);
		}
	}
#endif
	
	if (gPlatform.CPU.CoreTurboRatio[0] > aMaxMultiplier) // 0x26 (3.8GHz) > 0x22 (3.4GHz)
	{
		// No. Request maximum turbo boost (in case EIST is disabled).
		wrmsr64(MSR_IA32_PERF_CONTROL, (gPlatform.CPU.CoreTurboRatio[0]) << 8); // 0x26 -> 0x2600 (for 3.8GHz)
		
		// _CPU_DEBUG_DUMP("Maximum (0x%x) turbo boost requested.\n", (gPlatform.CPU.CoreTurboRatio[0] << 8));
	}
}


#if USE_STATIC_CPU_DATA

	#if CPU_VENDOR_ID == CPU_VENDOR_INTEL
		#include "cpu/intel/static_data.h"
	#else
		#include "cpu/amd/static_data.h"
	#endif

#else

	#if CPU_VENDOR_ID == CPU_VENDOR_INTEL
		#include "cpu/intel/dynamic_data.h"
	#else
		#include "cpu/amd/dynamic_data.h"
	#endif

#endif

