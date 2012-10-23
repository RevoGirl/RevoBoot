/*
 * Copyright (c) 2009 Master Chief. All rights reserved.
 *
 * Note: This is an essential part of the build process for RevoBoot v1.5.00 and greater.
 *
 * Updates:
 *
 *			- Latest cleanups and additional directives added by DHP in 2011.
 *			- Static CPU data simplified by DHP in Juni 2011 (thanks to MC and flAked for the idea).
 *			- Automatic creation / injection of SSDT_PR.aml added by DHP in June 2011.
 *			- New compiler directive (BOOT_TURBO_BOOST_RATIO) added by Jeroen (June 2011).
 *			- SMBIOS data logic moved to preprocessor code (PikerAlpha, October 2012).
 *			- STATIC_MODEL_NAME moved to libsaio/i386/SMBIOS/model_data.h (PikerAlpha, October 2012).
 *			- STATIC_MAC_PRODUCT_NAME moved to libsaio/i386/SMBIOS/model_data.h (PikerAlpha, October 2012).
 *			- STATIC_SMBIOS_MODEL_ID rrenamed to TARGET_MODEL (PikerAlpha, October 2012).
 *			- OVERRIDE_DYNAMIC_PRODUCT_DETECTION removed/no longer supported (PikerAlpha, October 2012).
 *			- INTEL_CORE_TECHNOLOGY per default set to 1 (PikerAlpha, October 2012).
 *
 */


//--------------------------------------------------------------- ACPI.C -------------------------------------------------------------------


#define ACPI_10_SUPPORT						0	// Set to 0 by Default. Set to 1 for ACPI 1.0 compliant BIOS versions.
												//
												// Note:	Must go first (before acpi/essentials) since it is used there,
												//			and requires you to set PATCH_ACPI_TABLE_DATA to 1.


#define PATCH_ACPI_TABLE_DATA				1	// Set to 1 by default (enabling patching). Use 0 to keep the original
												// unmodified ACPI tables, but please note (very well) that this is
												// only supported by very few motherboards / BIOS'es. You may also
												// need a kext like OSXRestart.kext to be able to restart your system,
												// this due to a possibly broken FACP table in your BIOS!
												//
												// Note: Requires one of the following STATIC_* and/or LOAD_* settings:


#define USE_STATIC_ACPI_BASE_ADDRESS		0	// Set to 0 by default. Use 1 only after gathering the base address!
												// Note: Don't forget to set PATCH_ACPI_TABLE_DATA to 1.


#if USE_STATIC_ACPI_BASE_ADDRESS
	#define	STATIC_ACPI_BASE_ADDRESS		0x000f0450	// Set DEBUG_ACPI to 1 to get this address.
#endif


#define STATIC_APIC_TABLE_INJECTION			0	// Set to 0 by default. Use 1 when you want to inject a modified copy
												// with say stripped out unused CPU's or other required modifications.
												//
												// Note: Don't forget to set PATCH_ACPI_TABLE_DATA to 1.


#define STATIC_APIC2_TABLE_INJECTION		0	// Set to 0 by default. Use 1 when you want to inject a second APIC
												// (ACPI-1) table data for additional CPU core support.
												//
												// Note: Don't forget to set PATCH_ACPI_TABLE_DATA to 1.


#define STATIC_DSDT_TABLE_INJECTION			0	// Set to 0 by default. Use 1 when you want to inject static DSDT data.


#define STATIC_ECDT_TABLE_INJECTION			0	// Set to 0 by default. Use 1 when want to inject a custom ECDT table.
												//
												// Note: Don't forget to set PATCH_ACPI_TABLE_DATA to 1.


#define STATIC_FACS_TABLE_INJECTION			0	// Set to 0 by default. Use 1 when want to inject a custom FACS table.
												//
												// Note: Don't forget to set PATCH_ACPI_TABLE_DATA to 1.


#define STATIC_HPET_TABLE_INJECTION			0	// Set to 0 by default. Use 1 when you want to inject a static copy
												// of a custom HPET table.
												//
												// Note: Don't forget to set PATCH_ACPI_TABLE_DATA to 1.


#define STATIC_SSDT_TABLE_INJECTION			0	// Set to 0 by default. Use 1 when you want to inject modifications
												// that can / should be done from this ACPI table.
												//
												// Note: Don't forget to set PATCH_ACPI_TABLE_DATA to 1.


#define STATIC_SSDT_GPU_TABLE_INJECTION		0	// Set to 0 by default. Use 1 when you want to inject modifications
												// for your graphics card (like I do for my ATI PCI-E card).
												//
												// Note: Don't forget to set PATCH_ACPI_TABLE_DATA to 1.


#define STATIC_SSDT_PR_TABLE_INJECTION		0	// Set to 0 by default. Use 1 when you want to inject your Intel
												// SpeedStep related modifications (like I do).
												//
												// Note: Don't forget to set PATCH_ACPI_TABLE_DATA to 1.


#define STATIC_SSDT_SATA_TABLE_INJECTION	0	// Set to 0 by default. Use 1 when want to inject SATA related modifications.
												//
												// Note: Don't forget to set PATCH_ACPI_TABLE_DATA to 1.


#define STATIC_SSDT_USB_TABLE_INJECTION		0	// Set to 0 by default. Use 1 when want to inject USB related modifications.
												//
												// Note: Don't forget to set PATCH_ACPI_TABLE_DATA to 1.


#define LOAD_DSDT_TABLE_FROM_EXTRA_ACPI		1	// Set to 0 by default. Use 1 when your setup requires a modified DSDT table
												// and you want to load: /Extra/ACPI/dsdt.aml instead of injecting a static
												// DSDT table from: RevoBoot/i386/config/ACPI/data.h
												//
												// Note: Don't forget to set PATCH_ACPI_TABLE_DATA to 1.


#define LOAD_SSDT_TABLE_FROM_EXTRA_ACPI		0	// Set to 1 by default. Use 0 only after you've converted your SSDT into
												// STATIC_SSDT_TABLE_INJECTION in: RevoBoot/i386/config/ACPI/data.h
												// or when you don't want/need to load /Extra/ACPI/SSDT.aml
												//
												// Note: Don't forget to set PATCH_ACPI_TABLE_DATA to 1.


#define LOAD_EXTRA_ACPI_TABLES				(LOAD_DSDT_TABLE_FROM_EXTRA_ACPI || LOAD_SSDT_TABLE_FROM_EXTRA_ACPI)


#define AUTOMATIC_SSDT_PR_CREATION			1	// Set to 0 by default (support for Sandy Bridge only).
												//
												// This injects a custom SSDT (in configure mode) with:
												//
												// Use 1 to inject: P/C-State definition blocks.
												// Use 2 to inject: Processor (CPUn, 0x0n, 0x00000410, 0x06) {...} declaration blocks.
												// Use 3 to inject: Both of the above.
												// Use 4 to inject: Device (SBUS) {...} which is required for Power Management.
												// Use 5 to inject: P/C-State definition blocks plus the former (Device SBUS).
												// Use 7 to inject: All of the above.
												//
												// Notes:	Device SBUS can only be injected when it isn't part of other ACPI tables!
												//			This feature should only be used once, to extract the SSDT_PR from ioreg
												//			and use it as STATIC_SSDT_PR_TABLE_DATA in RevoBoot/i386/config/ACPI/data.h


#if AUTOMATIC_SSDT_PR_CREATION && STATIC_SSDT_PR_TABLE_INJECTION == 0
	#define MAX_NUMBER_OF_P_STATES			22	// The i5-2500K need 18 for the base-range (1600-3300) plus 4 for the Turbo modes.
												// The i7-2600K need 19 for the base-range (1600-3400) plus 4 for the Turbo modes.
												// The i7-2700K need 20 for the base-range (1600-3500) plus 4 for the Turbo modes.
												// The i7-3770K need 20 for the base-range (1600-3500) plus 4 for the Turbo modes.
												// Low power (mobility) processors might need an extended range!
												//
												// Note: AICPUPM wants a P-State for each 100 MHz bank or it will fail (see note below).

#define DROP_FACTORY_SSDT_TABLES			1	// Set to 1 by default (this setting is required).
												//
												// Note: Do not change this setting (must drop SSDT tables).

#define NUMBER_OF_TURBO_STATES				4	// Set to 4 by default.
												//
												// Note:	Make sure to add a full range, one P-State for each 100 MHz when OC'ing
												//			or AICPIPM will fail with: "P-State Stepper Error 18 at step N on CPU N"

#define OVERRIDE_ACPI_METHODS				0	// Set to 0 by default (do nothing).
												// Use 1 to override Method _PTS in a static SSDT or Extra/ACPI/SSDT.aml
												// Use 2 to override Method _WAK in a static SSDT or Extra/ACPI/SSDT.aml
												// Use 3 to override both _PTS and _WAK.
												//
												// Note:	This changes the underscore of _PTS and/or _WAK into a "Z" which
												//			allows you to inject a customized copy from /Extra/ACPI/SSDT.aml
#else
	#define OVERRIDE_ACPI_METHODS			0	// Set to 0 by default (do nothing).

	#define DROP_FACTORY_SSDT_TABLES		0	// Set to 0 by default. Use 1 with caution (might disable SpeedStep).
#endif

#define REPLACE_EXISTING_SSDT_TABLES		0	// Set to 0 by default. Use 1 with caution (might disable SpeedStep).
												//
												// Note: Don't forget to set PATCH_ACPI_TABLE_DATA to 1.

#define	APPLE_STYLE_ACPI					0	// Set to 0 by default. Use 1 to change the OEMID's to Mac likes.
												//
												// Note:	Don't forget to set PATCH_ACPI_TABLE_DATA to 1 and keep in mind that this can
												//			only change the headers of injected/replaced tables. Not the factory tables.


#define DEBUG_ACPI							0	// Set to 0 by default. Use 1 when things don't seem to work for you.


//--------------------------------------------------------------- BOOT.C -------------------------------------------------------------------


#define PRE_LINKED_KERNEL_SUPPORT		1	// Set to 1 by default. Change this to 0 to disable the use of pre-linked kernels.

#define MUST_ENABLE_A20					0	// Set to 0 by default. Change this to 1 when your hardware requires it.

#define SAFE_MALLOC						0	// Set to 0 by default. Change this to 1 when booting halts with a memory allocation error.

#define DEBUG_BOOT						0	// Set to 0 by default. Change this to 1 when things don't seem to work for you.


//---------------------------------------------------------------- CPU.C -------------------------------------------------------------------


#define USE_STATIC_CPU_DATA				1	// Set to 0 by default (dynamic data collection). Change this to 1 to use static data.

#define CPU_VENDOR_ID					CPU_VENDOR_INTEL // CPU_VENDOR_AMD is not supported.

#define INTEL_CORE_TECHNOLOGY			1	// Set to 1 by default. Use 0 for older non Intel Core CPU's (removes unused code).
											//
											// Warning: Do not use 0 on Core Technology CPU's or sysctl's machdep.tsc.frequency will be
											//			initialized with the wrong value (various things, like the spinner will go mad).

#define OC_BUSRATIO_CORRECTION			0	// Set to 0 by default. Change this to busratio-100 (OC'ed systems with a changed busratio).

#define BOOT_TURBO_RATIO				0	// Set to 0 by default. Change this to the desired (and supported) max turbo multiplier.
											//
											// Example:  0x2800 for 4.0 GHz on a i7-2600.

#define DEBUG_CPU						0	// Set to 0 by default. Change this to 1 when things don't seem to work for you.
											// Note: CPU info data will not be displayed when USE_STATIC_CPU_DATA is set to 1

#if DEBUG_CPU
#define DEBUG_CPU_TURBO_RATIOS			0	// Set to 0 by default. Change this to 1 when you want to check the core ratio.

#define DEBUG_CST_SUPPORT				0	// Set to 0 by default. Change this to 1 to check the in BIOS enabled C-States.

#define DEBUG_TSS_SUPPORT				0	// Set to 0 by default. Change this to 1 to check the T-State Clock Modulation.

#define DEBUG_CPU_TDP					0	// Set to 0 by default. Change this to 1 when you want to check the TDP.
#endif

//---------------------------------------------------------- CPU/STATIC_DATA.C -------------------------------------------------------------


#if USE_STATIC_CPU_DATA
	#define STATIC_CPU_Type				0x602			// kSMBTypeOemProcessorType - used in: libsaio/SMBIOS/dynamic_data.h

	#define STATIC_CPU_NumThreads		4				// Used in: i386/libsaio/ACPI/ssdt_pr_generator.h

	#define STATIC_CPU_FSBFrequency		100000000ULL	// 9 digits + ULL - used in: i386/libsaio/efi.c

	#define STATIC_CPU_QPISpeed			0				// kSMBTypeOemProcessorBusSpeed (0 for Sandy Bridge / Jaketown).
#endif

#define STATIC_CPU_NumCores				4				// Set to 4 by default. Must be set to the number of cores for your processor!
														//
														// Note: Used in i386/libsaio/ACPI/ssdt_pr_generator.h, cpu.c and platform.c
														//		 for both static and dynamic CPU data.

//--------------------------------------------------------------- DISK.C -------------------------------------------------------------------


#define EFI_SYSTEM_PARTITION_SUPPORT	0	// Set to 0 by default. Set this to 1 when your system boots from the hidden EFI partition.

#define LEGACY_BIOS_READ_SUPPORT		0	// Set to 0 by default. Change this to 1 for crappy old BIOSes.

#define LION_FILEVAULT_SUPPORT			0	// Set to 0 by default.  Setting this to 1 will make RevoBoot skip encrypted boot partitions
											// and boot from the Recovery HD partition instead (when available).

#define APPLE_RAID_SUPPORT				0	// Set to 0 by default. Change this to 1 for Apple Software RAID support.

#define DEBUG_DISK						0	// Set to 0 by default. Change it to 1 when things don't seem to work for you.


//------------------------------------------------------------- DRIVERS.C -------------------------------------------------------------------


#define DEBUG_DRIVERS					0	// Set to 0 by default. Change it to 1 when things don't seem to work for you.


//---------------------------------------------------------------- EFI.C -------------------------------------------------------------------


#define APPLE_STYLE_EFI					1	// Set to 1 by default. Change this to 1 to add additional 'Mac-like' properties.

#define INJECT_EFI_DEVICE_PROPERTIES	0	// Set to 0 by default. Change this to 1 when you need to inject 'device-properties'.
											//
											// Note: Required when not setting device-properties from your DSDT/SSDT.

#define EFI_64_BIT						1	// Set to 1 by default for EFI64 on 64-bit platforms. Supporting both
											// 32 and 64-bit boot modes (using arch=i386/x86_64 under Kernel Flags).
											//
											// Change this to 0 for 32-bit only platforms (think Intel Atom CPU)
											// or when you want to boot with EFI32 (for testing) on a 64-bit
											// platform, but then you must make a small change in platform.c (see comment in file).
											//
											// Note: Do not change this setting, unless you know what you are doing.

#define STATIC_SMSERIALNUMBER			"SOMESRLNUMBR" // Example only!

#define STATIC_SYSTEM_SERIAL_NUMBER		{ 'S', 'O', 'M', 'E', 'S', 'R', 'L', 'N', 'U', 'M', 'B', 'R' } // Example only!

#define STATIC_SYSTEM_ID				{ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F } // Example only!

#define DEBUG_EFI						0	// Set to 0 by default. Change this to 1 when things don't seem to work for you.

#define EFI_DEBUG_MODE					0	// Set to 0 by default (for OS X 10.7 LION only).


//------------------------------------------------------------- GRAPHICS.C -----------------------------------------------------------------

#define USE_STATIC_DISPLAY_RESOLUTION	1	// Set to 0 by default. Use 1 when you need to override the resolution detection features
											// in RevoBoot, which may not be supported by your BIOS and/or display (monitor).

#define STATIC_SCREEN_WIDTH				1900	// Used (in RevoBoot v1.0.35 and greater) when USE_STATIC_DISPLAY_RESOLUTION is 1 and when
												// USE_STATIC_DISPLAY_RESOLUTION is 0 but getResolutionFromEDID() isn't supported (failed).

#define STATIC_SCREEN_HEIGHT			1200	// Used (in RevoBoot v1.0.35 and greater) when USE_STATIC_DISPLAY_RESOLUTION is 1 and when
												// USE_STATIC_DISPLAY_RESOLUTION is 0 but getResolutionFromEDID() isn't supported (failed).

#define DEBUG_BOOT_GRAPHICS				0	// Set to 0 by default. Use 1 when to see debug info.


//------------------------------------------------------------ STRINGDATA.H ----------------------------------------------------------------

#define LION_FILEVAULT_SUPPORT			0	// Set to 0 by default.  Setting this to 1 will make RevoBoot skip encrypted boot partitions
											// and boot from the Recovery HD partition instead (when available).

#if LION_FILEVAULT_SUPPORT
	#define LION_RECOVERY_SUPPORT		1	// Make RevoBoot search for the Recovery HD partition and boot from it (when available).
#else
	#define LION_RECOVERY_SUPPORT		0	// Set to 0 by default. Setting this to 1 will make RevoBoot search for the Recovery HD and
											// try to boot from it, when it is properly setup and modified for RevoBoot.
#endif

#define LION_INSTALL_SUPPORT			0	// Set to 0 by default. Setting this to 1 will make RevoBoot search in specific directories
											// for com.apple.Boot.plist – required for Mac like Lion OS X installations.

//-------------------------------------------------------------- SMBIOS.C ------------------------------------------------------------------


#define USE_STATIC_SMBIOS_DATA				0	// Set to 0 by default (dynamic data collection). Change this to 1 to use static data.

#if (USE_STATIC_SMBIOS_DATA == 0 && USE_STATIC_CPU_DATA == 1)
	#undef USE_STATIC_CPU_DATA					// Prevent boot failures due to wrong settings (until I figured out what we are missing).
#endif

#define OVERRIDE_DYNAMIC_MEMORY_DETECTION	0	// Set to 0 by default. Change this to 0 only when your SMBIOS data (type 17) is correct, or when
												// you want/need to override some/all of the SMBIOS data.
												//
												// Note: Defaults to n MB 1066 DDR3 when set to 0 (to prevent errors in Profile Manager).


#define TARGET_MODEL						MACMINI	// Set to MACMINI by default. Supported models are:
													//
													// IMAC and IMAC_131, IMAC_122, IMAC_111, IMAC_121
													// MACBOOK and MACBOOK_41
													// MACBOOKAIR and MACBOOKAIR_42, MACBOOKAIR_41
													// MACBOOKPRO and MACBOOKPRO_101, MACBOOKPRO_91, MACBOOKPRO_83, MACBOOKPRO_82,
													// MACBOOKPRO_81, MACBOOKPRO_61
													// MACMINI and MACMINI_53, MACMINI_52, MACMINI_51
													// MACPRO and MACPRO_51, MACPRO_41, MACPRO_31
													//
													// Note: MACMINI (without _NNN) selects the default model (last one i.e. MACMINI_51).

#define DEBUG_SMBIOS						0	// Set to 0 by default. Change this to 1 when things don't seem to work for you.


//-------------------------------------------------------------- PLATFORM.C ----------------------------------------------------------------


#if USE_STATIC_SMBIOS_DATA
	// Do nothing.
#elif OVERRIDE_DYNAMIC_MEMORY_DETECTION
												// Setup RAM module info. Please note that you may have to expand this when you have more RAM modules.
	#define STATIC_RAM_SLOTS				4	// Number of RAM slots on mainboard.

	#define STATIC_RAM_VENDORS				{ "Corsair", "N/A", "Corsair", "N/A", 0 }	// Use "N/A" for empty RAM banks.

	#define DYNAMIC_RAM_OVERRIDE_TYPE		0	// Set to 0 by default. See libsaio/platform.h for supported values.

	#define DYNAMIC_RAM_OVERRIDE_SIZE		0	// Set to 0 by default. Change this to 1 when you want to use override values (see below).

#if DYNAMIC_RAM_OVERRIDE_SIZE
	#define DYNAMIC_RAM_OVERRIDE_SIZES	{ SMB_MEM_SIZE_2GB, SMB_MEM_BANK_EMPTY, SMB_MEM_SIZE_2GB, SMB_MEM_BANK_EMPTY, 0 } // See libsaio/platform.h for other values.
#endif

	#define DYNAMIC_RAM_OVERRIDE_FREQUENCY	0	// Set to 0 by default. Change this to the frequency that you want to use as override value.

	#define STATIC_RAM_PART_NUMBERS			{ "PartNum#0", "N/A", "PartNum#2", "N/A", 0 }	// Use "N/A" for empty RAM banks.

	#define STATIC_RAM_SERIAL_NUMBERS		{ "Serial#0", "N/A", "Serial#2", "N/A", 0 }		// Use "N/A" for empty RAM banks.
#endif

#define INCLUDE_MPS_TABLE					0	// Set to 0 by default. Change this to 1 when you want to include the MP table.

#define DEBUG_PLATFORM						0	// Set to 0 by default. Change this to 1 when things don't seem to work for you.


//================================================================= END ====================================================================

