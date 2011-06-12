/*
 * Copyright (c) 2009 by Master Chief.
 *
 * Refactoring for Revolution done by DHP in 2010/2011.
 * Expanded (requestMaxTurbo added) by DHP in May 2011.
 * Simplified by DHP in Juni 2011 (thanks to MC and flAked for the idea).
 * Code copied from Intel/dynamic_data.h  by DHP in juni 2011.
 * New compiler directive (BOOT_TURBO_RATIO) added by Jeroen (June 2011).
 */


#include "platform.h"
#include "cpu/cpuid.h"
#include "cpu/proc_reg.h"


#if AUTOMATIC_SSDT_PR_CREATION || DEBUG_CPU_TDP
//==============================================================================
// Called from dynamic_data.h and i386/libsaio/ACPI/ssdt_pr_generator.h

uint8_t getTDP(void)
{
	uint8_t		powerUnit = bitfield32(rdmsr64(MSR_RAPL_POWER_UNIT), 3, 0);
	uint32_t	powerLimit = bitfield32(rdmsr64(MSR_PKG_RAPL_POWER_LIMIT), 14, 0);

	uint8_t		tdp = (powerLimit / (1 << powerUnit));

	if (tdp == 255)
	{
		powerLimit = bitfield32(rdmsr64(MSR_PKG_POWER_INFO), 14, 0);
		tdp = (powerLimit / (1 << powerUnit));
	}

#if DEBUG_CPU_TDP
	printf("RAPL Power Limit : %d\n", powerLimit);	// 0x7F8 / 2040
	printf("RAPL Power Unit  : %d\n", powerUnit);	// 3 (1/8 Watt)
	printf("RAPL Package TDP : %d\n", tdp);			// 255
	printf("RAPL Power Limit : %d\n", powerLimit);
	printf("CPU IA (Core) TDP: %d\n", tdp);			// 95
	sleep(5);
#endif

	return tdp;
}
#endif


//==============================================================================

void initTurboRatios()
{
	// Get turbo ratio(s).
	uint64_t msr = rdmsr64(MSR_TURBO_RATIO_LIMIT);
	
#if AUTOMATIC_SSDT_PR_CREATION || DEBUG_CPU_TURBO_RATIOS
	// All CPU's have at least two cores (think mobility CPU here).
	gPlatform.CPU.CoreTurboRatio[0] = bitfield32(msr, 7, 0);
	gPlatform.CPU.CoreTurboRatio[1] = bitfield32(msr, 15, 8);
	
	// Additionally for quad and six core CPU's.
	if (gPlatform.CPU.NumCores >= 4)
	{
		gPlatform.CPU.CoreTurboRatio[2] = bitfield32(msr, 23, 16);
		gPlatform.CPU.CoreTurboRatio[3] = bitfield32(msr, 31, 24);
		
		// For the lucky few with a six core Gulftown CPU.
		if (gPlatform.CPU.NumCores >= 6)
		{
			// bitfield32() supports 32 bit values only and thus we 
			// have to do it a little different here (bit shifting).
			gPlatform.CPU.CoreTurboRatio[4] = ((msr >> 32) & 0xff);
			gPlatform.CPU.CoreTurboRatio[5] = ((msr >> 40) & 0xff);
		}
	}
	
	// Are all core ratios set to the same value?
	//
	// DHP: This should follow the same route as what I did for ssdt_pr_generator.h 
	//		i.e. don't assume that all installations have 4 cores (think scalable).
	//
	if ((gPlatform.CPU.CoreTurboRatio[0] == gPlatform.CPU.CoreTurboRatio[1]) &&
		(gPlatform.CPU.CoreTurboRatio[1] == gPlatform.CPU.CoreTurboRatio[2]) &&
		(gPlatform.CPU.CoreTurboRatio[2] == gPlatform.CPU.CoreTurboRatio[3]))
	{
		// Yes. We only need one so wipe the rest (keeping the one for max cores).
		gPlatform.CPU.CoreTurboRatio[0] = gPlatform.CPU.CoreTurboRatio[1] = gPlatform.CPU.CoreTurboRatio[2] = 0;
	}
#else
	gPlatform.CPU.CoreTurboRatio[0] = bitfield32(msr, 7, 0);
#endif
}


//==============================================================================

void requestMaxTurbo(uint8_t aMaxMultiplier)
{
	// Testing with MSRDumper.kext confirmed that the CPU isn't always running at top speed,
	// up to 5.9 GHz on a good i7-2500K, which is why we check for it here (a quicker boot).
	
#if USE_STATIC_CPU_DATA
	// Turbo Disabled?
	if ((rdmsr64(IA32_MISC_ENABLES) >> 32) & 0x40)
	{
		return; // Yes.
	}

	aMaxMultiplier = ((rdmsr64(MSR_PLATFORM_INFO) >> 8) && 0xff);
#endif
	initTurboRatios();

	// Is the maximum turbo ratio reached already?
	if (gPlatform.CPU.CoreTurboRatio[0] > aMaxMultiplier) // 0x26 (3.8GHz) > 0x22 (3.4GHz)
	{
		// No. Request maximum turbo boost (in case EIST is disabled).
		wrmsr64(MSR_IA32_PERF_CONTROL, BOOT_TURBO_RATIO || (gPlatform.CPU.CoreTurboRatio[0] << 8)); // Example: 0x26 -> 0x2600 (for 3.8GHz)

		// Note: The above compiler directive was added, on request, to trigger the required 
		//		 boot turbo multiplier (SB 'iMac' running at 5.4+ GHz did not want to boot).

		_CPU_DEBUG_DUMP("Maximum (%d00) turbo boost requested.\n", gPlatform.CPU.CoreTurboRatio[0]);
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

