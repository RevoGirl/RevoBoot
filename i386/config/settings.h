/*
 * Copyright (c) 2009 Master Chief. All rights reserved.
 *
 * Note: This is an essential part of the build process for RevoBoot v1.0.14 and greater.
 *
 *
 * Latest cleanups and additional directives added by DHP in 2011.
 * Static CPU data simplified by DHP in Juni 2011 (thanks to MC and flAked for the idea).
 * Automatic creation / injection of SSDT_PR.aml added by DHP in June 2011.
 * New compiler directive (BOOT_TURBO_BOOST_RATIO) added by Jeroen (June 2011).
 */


//--------------------------------------------------------------- ACPI.C -------------------------------------------------------------------


#define ACPI_10_SUPPORT						0	// Set to 0 by Default. Set to 1 for ACPI 1.0 compliant BIOS versions.
												//
												// Note: Must go first (before acpi/essentials) since it is used there,
												//       and requires you to set PATCH_ACPI_TABLE_DATA to 1.


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
	#define	STATIC_ACPI_BASE_ADDRESS		0x000f0420	// Set DEBUG_ACPI to 1 to get this address.
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


#define STATIC_ECDS_TABLE_INJECTION			0	// Set to 0 by default. Use 1 when want to inject a custom ECDT table.
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


#define LOAD_DSDT_TABLE_FROM_EXTRA_ACPI		1	// Set to 1 by default. Use 0 only when your configuration can do 
												// without patching the ACPI tables i.e. when you don't need to 
												// inject your modified DSDT table from /Extra/ACPI/
												//
												// Note: Don't forget to set PATCH_ACPI_TABLE_DATA to 1.


#define LOAD_SSDT_TABLE_FROM_EXTRA_ACPI		0	// Set to 0 by default. Use 1 when you know how to override / patch 
												// your SSDT table, by injection the modifications from /Extra/ACPI/
												//
												// Note: Don't forget to set PATCH_ACPI_TABLE_DATA to 1.


#define LOAD_EXTRA_ACPI_TABLES				(LOAD_DSDT_TABLE_FROM_EXTRA_ACPI || LOAD_SSDT_TABLE_FROM_EXTRA_ACPI)


#define DROP_SSDT_TABLES					0	// Set to 0 by default. Use 1 with caution (might disable SpeedStep).
												//
												// Note: Don't forget to set PATCH_ACPI_TABLE_DATA to 1.


#define AUTOMATIC_SSDT_PR_CREATION			0	// Set to 0 by default. Change to 1 when you want RevoBoot to generate and inject SSDT_PR for you.
												//
												// Note: Optional feature for 'Sandy Bridge' based configurations.


#define AUTOMATIC_PROCESSOR_BLOCK_CREATION	0	// Set to 0 by default. Change to 1 when you want to inject processor blocks into SSDT_PR.
												//
												// Note: You can have Processor() {} blocks in your DSDT / SSDT so there is really 
												//		 no reliable way of knowing when to inject them automatically.


#define REPLACE_EXISTING_SSDT_TABLES		0	// Set to 0 by default. Use 1 with caution (might disable SpeedStep).
												//
												// Note: Don't forget to set PATCH_ACPI_TABLE_DATA to 1.


#define	APPLE_STYLE_ACPI					0	// Set to 0 by default. Use 1 to change the OEMID's to Mac likes.
												//
												// Note: Don't forget to set PATCH_ACPI_TABLE_DATA to 1.


#define DEBUG_ACPI							0	// Set to 0 by default. Use 1 when things don't seem to work for you.


//--------------------------------------------------------------- BOOT.C -------------------------------------------------------------------


#define PRE_LINKED_KERNEL_SUPPORT			1	// Set to 1 by default. Change this to 0 to disable the use of pre-linked kernels.

#define MUST_ENABLE_A20						0	// Set to 0 by default. Change this to 1 when your hardware requires it.

#define SAFE_MALLOC							0	// Set to 0 by default. Change this to 1 when booting halts with a memory allocation error.

#define DEBUG_BOOT							0	// Set to 0 by default. Change this to 1 when things don't seem to work for you.


//---------------------------------------------------------------- CPU.C -------------------------------------------------------------------


#define USE_STATIC_CPU_DATA					0	// Set to 0 by default (dynamic data collection). Change this to 1 to use static data.

#define CPU_VENDOR_ID						CPU_VENDOR_INTEL // CPU_VENDOR_AMD is not supported.

#define OC_BUSRATIO_CORRECTION				0	// Set to 0 by default. Change this to busratio-100 (OC'ed systems with a changed busratio).

#define NUMBER_OF_TURBO_STATES				4	// Set to 4 by default.

#define BOOT_TURBO_RATIO					0	// Set to 0 by default. Change this to the desired (and supported) turbo multiplier.
												//
												// Example:  0x2800 for 4.0 GHz on a i7-2600.

#define DEBUG_CPU							0	// Set to 0 by default. Change this to 1 when things don't seem to work for you.
                                                // Note: CPU info data will not be displayed when USE_STATIC_CPU_DATA is set to 1

#if DEBUG_CPU
	#define DEBUG_CPU_TURBO_RATIOS			0	// Set to 0 by default. Change this to 1 when you want to check the core ratio.

	#define DEBUG_CST_SUPPORT				0	// Set to 0 by default. Change this to 1 to check the in BIOS enabled C-States.

	#define DEBUG_TSS_SUPPORT				0	// Set to 0 by default. Change this to 1 to check the T-State Clock Modulation.

	#define DEBUG_CPU_TDP					0	// Set to 0 by default. Change this to 1 when you want to check the TDP.
#endif

//---------------------------------------------------------- CPU/STATIC_DATA.C -------------------------------------------------------------


#define STATIC_CPU_Type						0x703			// kSMBTypeOemProcessorType - used in: libsaio/SMBIOS/dynamic_data.h

#define STATIC_CPU_NumCores					4				// Used in: i386/libsaio/ACPI/ssdt_pr_generator.h
															//
															// Note: Also used in cpu.c and platform.c for both static / dynamic CPU data.

#define STATIC_CPU_NumThreads				8				// Used in: i386/libsaio/ACPI/ssdt_pr_generator.h

#define STATIC_CPU_FSBFrequency				100000000ULL	// 9 digits + ULL - used in: i386/libsaio/efi.c

#define STATIC_CPU_QPISpeed					0				// kSMBTypeOemProcessorBusSpeed (0 for Sandy Bridge / Jaketown).


//--------------------------------------------------------------- DISK.C -------------------------------------------------------------------


#define EFI_SYSTEM_PARTITION_SUPPORT		0	// Set to 0 by default. Set this to 1 when your system boots from the hidden EFI partition.

#define LEGACY_BIOS_READ_SUPPORT			0	// Set to 0 by default. Change this to 1 for crappy old BIOSes.

#define DEBUG_DISK							0	// Set to 0 by default. Change it to 1 when things don't seem to work for you.


//------------------------------------------------------------- DRIVERS.C -------------------------------------------------------------------


#define DEBUG_DRIVERS						0	// Set to 0 by default. Change it to 1 when things don't seem to work for you.


//---------------------------------------------------------------- EFI.C -------------------------------------------------------------------


#define APPLE_STYLE_EFI						1	// Set to 1 by default. Change this to 1 to add additional 'Mac-like' properties.

#define INJECT_EFI_DEVICE_PROPERTIES		0	// Set to 0 by default. Change this to 1 when you need to inject 'device-properties'.

#define EFI_64_BIT							1	// Set to 1 by default for EFI64 on 64-bit platforms. Supporting both 
												// 32 and 64-bit boot modes (using arch=i386/x86_64 under Kernel Flags).
												// 
												// Change this to 0 for 32-bit only platforms (think Intel Atom CPU) 
												// or when you want to boot with EFI32 (for testing) on a 64-bit 
												// platform, but then you must make a small change in platform.c (see comment in file).
												//
												// Note: Do not change this setting, unless you know what you are doing.

#define STATIC_MODEL_NAME					{ 'i', 'M', 'a', 'c', '1', '2', ',', '2' }

#define STATIC_SMSERIALNUMBER				"SOMESRLNUMBR"

#define STATIC_SYSTEM_SERIAL_NUMBER			{ 'S', 'O', 'M', 'E', 'S', 'R', 'L', 'N', 'U', 'M', 'B', 'R' }

#define STATIC_SYSTEM_ID					{ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F }

#define DEBUG_EFI							0	// Set to 0 by default. Change this to 1 when things don't seem to work for you.

#define EFI_DEBUG_MODE						0	// Set to 0 by default (for OS X 10.7 LION only).


//------------------------------------------------------------- GRAPICS.C ------------------------------------------------------------------


#define STATIC_SCREEN_WIDTH					1600

#define STATIC_SCREEN_HEIGHT				1200


//-------------------------------------------------------------- SMBIOS.C ------------------------------------------------------------------


#define USE_STATIC_SMBIOS_DATA				0	// Set to 0 by default (dynamic data collection). Change this to 1 to use static data.


#define OVERRIDE_DYNAMIC_MEMORY_DETECTION	0	// Set to 0 by default. Change this to 0 only when your SMBIOS data (type 17) is fine!
												// Note: Defaults to n MB 1066 DDR3 when set to 0 (preventing errors in Profile Manager).


#define OVERRIDE_DYNAMIC_PRODUCT_DETECTION	0	// Set to 0 by default. Change this to 1 when you want to use a predefined product type.
												// Note: Defaults to STATIC_MAC_PRODUCT_NAME when set to 0.

#if OVERRIDE_DYNAMIC_PRODUCT_DETECTION
	#define STATIC_SMBIOS_MODEL_ID			IMAC	// Supported models: IMAC, MACBOOK, MACBOOKPRO, MACMINI or MACPRO
#endif

#define DEBUG_SMBIOS						0	// Set to 0 by default. Change this to 1 when things don't seem to work for you.


//-------------------------------------------------------------- PLATFORM.C ----------------------------------------------------------------


#define STATIC_MAC_PRODUCT_NAME				"iMac12,2"	// New default for Sandy Bridge configurations.

#if USE_STATIC_SMBIOS_DATA
// Do nothing.
#elif OVERRIDE_DYNAMIC_MEMORY_DETECTION
// Setup RAM module info. Please note that you may have to expand this when you have more RAM modules.
	#define STATIC_RAM_SLOTS				4	// Number of RAM slots on mainboard.

	#define STATIC_RAM_VENDORS				{ "Corsair", "N/A", "Corsair", "N/A", 0 }	// Use "N/A" for empty RAM banks.

	#define DYNAMIC_RAM_OVERRIDE_TYPE		0	// Set to 0 by default. See libsaio/platform.h for supported values.

	#define DYNAMIC_RAM_OVERRIDE_SIZE		0	// Set to 0 by default. Change this to 1 when you want to use override values (see below).

#if DYNAMIC_RAM_OVERRIDE_SIZE
	#define DYNAMIC_RAM_OVERRIDE_SIZES		{ SMB_MEM_SIZE_2GB, SMB_MEM_BANK_EMPTY, SMB_MEM_SIZE_2GB, SMB_MEM_BANK_EMPTY, 0 } // See libsaio/platform.h for other values.
#endif

	#define DYNAMIC_RAM_OVERRIDE_FREQUENCY	0	// Set to 0 by default. Change this to the frequency that you want to use as override value.

	#define STATIC_RAM_PART_NUMBERS			{ "CMX4GX3M2B2000C9", "N/A", "CMX4GX3M2B2000C9", "N/A", 0 }	// Use "N/A" for empty RAM banks.

	#define STATIC_RAM_SERIAL_NUMBERS		{ "Serial#0", "N/A", "Serial#2", "N/A", 0 }	// Use "N/A" for empty RAM banks.
#endif

#define INCLUDE_MPS_TABLE					0	// Set to 0 by default. Change this to 1 when you want to include the MP table.

#define DEBUG_PLATFORM						0	// Set to 0 by default. Change this to 1 when things don't seem to work for you.


//================================================================= END ====================================================================

