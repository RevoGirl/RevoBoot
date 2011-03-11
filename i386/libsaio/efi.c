/*
 *
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. The rights granted to you under the License
 * may not be used to create, or enable the creation or redistribution of,
 * unlawful or unlicensed copies of an Apple operating system, or to
 * circumvent, violate, or enable the circumvention or violation of, any
 * terms of an Apple operating system software license agreement.
 * 
 * Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * Portions of source code Copyright (c) 2007 by David F. Elliott,
 * additional changes by: Macerintel (2008), Master Chief (2009) and 
 * Rekursor in 2009. All rights reserved.
 *
 * EFI implementation for Revolution Copyright (c) 2010 by DHP.
 * All rights reserved.
 *
 */


#include "platform.h"
#include "efi/fake_efi.h"

#define INCLUDE_EFI_DATA	1

#include "../config/data.h"


//==============================================================================

void initEFITree(void)
{
	_EFI_DEBUG_DUMP("Entering initEFITree(%x)\n", gPlatform.ACPI.Guid.Data1);

	static char ACPI[] = "ACPI";

	// The required information should be added to private_data.h
	static EFI_CHAR16 const MODEL_NAME[]			= STATIC_MODEL_NAME;
	static EFI_CHAR16 const SYSTEM_SERIAL_NUMBER[]	= STATIC_SYSTEM_SERIAL_NUMBER;

	DT__Initialize(); // Add and initialize gPlatform.DT.RootNode
	
	/*
	 * The root node is available until the call to DT__Finalize, or the first call 
	 * to DT__AddChild with NULL as first argument. Which we don't do and thus we 
	 * can use it in the meantime, instead of defining a local / global variable.
	 */

	DT__AddProperty(gPlatform.DT.RootNode, "model", 5, ACPI);
	DT__AddProperty(gPlatform.DT.RootNode, "compatible", 5, ACPI);
	
	Node * efiNode = DT__AddChild(gPlatform.DT.RootNode, "efi");

	DT__AddProperty(efiNode, "firmware-abi", 6, (gPlatform.ArchCPUType == CPU_TYPE_X86_64) ? "EFI64" : "EFI32");
    DT__AddProperty(efiNode, "firmware-revision", sizeof(FIRMWARE_REVISION), (EFI_UINT32*) &FIRMWARE_REVISION);
    DT__AddProperty(efiNode, "firmware-vendor", sizeof(FIRMWARE_VENDOR) + 1, (EFI_CHAR16*) FIRMWARE_VENDOR);

	// Initialize a global var, used by function setupEFITables later on, to
	// add the address to the boot arguments (done to speed up the process).
	gPlatform.EFI.Nodes.RuntimeServices = DT__AddChild(efiNode, "runtime-services");

	// Initialize a global var, used by function addConfigurationTable later on, 
	// to add the SMBIOS and ACPI tables (done to speed up the process).
	gPlatform.EFI.Nodes.ConfigurationTable = DT__AddChild(efiNode, "configuration-table");

	Node * platformNode = DT__AddChild(efiNode, "platform");

	gPlatform.EFI.Nodes.Platform = platformNode;

	// Satisfying AppleACPIPlatform.kext
	static EFI_UINT8 const DEVICE_PATHS_SUPPORTED[] = { 0x01, 0x00, 0x00, 0x00 };

	DT__AddProperty(platformNode, "DevicePathsSupported", sizeof(DEVICE_PATHS_SUPPORTED), &DEVICE_PATHS_SUPPORTED);

	// The use of sizeof() here is mandatory (to prevent breakage).
	DT__AddProperty(platformNode, "Model", sizeof(MODEL_NAME), (EFI_CHAR16*) MODEL_NAME);
    DT__AddProperty(platformNode, "SystemSerialNumber", sizeof(SYSTEM_SERIAL_NUMBER), (EFI_CHAR16*) SYSTEM_SERIAL_NUMBER);

	if (gPlatform.CPU.FSBFrequency)
	{
		_EFI_DEBUG_DUMP("Adding FSBFrequency property (%dMHz)\n", (gPlatform.CPU.FSBFrequency / 1000));
		DT__AddProperty(platformNode, "FSBFrequency", sizeof(uint64_t), &gPlatform.CPU.FSBFrequency);
	}

	Node * chosenNode = DT__AddChild(gPlatform.DT.RootNode, "chosen");

	if (chosenNode == 0)
	{
		stop("Couldn't create /chosen node"); // Mimics boot.efi
	}

	gPlatform.EFI.Nodes.MemoryMap = DT__AddChild(chosenNode, "memory-map");

	// Adding the root path for kextcache.
	DT__AddProperty(chosenNode, "boot-device-path", 38, ((gPlatform.OSType & 3) == 3) ? "\\boot.efi" : "\\System\\Library\\CoreServices\\boot.efi");

	// Adding the default kernel name (mach_kernel) for kextcache.
	DT__AddProperty(chosenNode, "boot-file", sizeof(bootInfo->bootFile), bootInfo->bootFile);

#if APPLE_STYLE_EFI

	static EFI_UINT8 const BOOT_FILE_PATH[] =
	{
		0x04, 0x04, 0x50, 0x00, 0x5c, 0x00, 0x53, 0x00, 0x79, 0x00, 0x73, 0x00, 0x74, 0x00, 0x65, 0x00, 
		0x6d, 0x00, 0x5c, 0x00, 0x4c, 0x00, 0x69, 0x00, 0x62, 0x00, 0x72, 0x00, 0x61, 0x00, 0x72, 0x00,
		0x79, 0x00, 0x5c, 0x00, 0x43, 0x00, 0x6f, 0x00, 0x72, 0x00, 0x65, 0x00, 0x53, 0x00, 0x65, 0x00,
		0x72, 0x00, 0x76, 0x00, 0x69, 0x00, 0x63, 0x00, 0x65, 0x00, 0x73, 0x00, 0x5c, 0x00, 0x62, 0x00, 
		0x6f, 0x00, 0x6f, 0x00, 0x74, 0x00, 0x2e, 0x00, 0x65, 0x00, 0x66, 0x00, 0x69, 0x00, 0x00, 0x00, 
		0x7f, 0xff, 0x04, 0x00
	};

	DT__AddProperty(chosenNode, "boot-file-path", sizeof(BOOT_FILE_PATH), &BOOT_FILE_PATH);

	static EFI_UINT8 const BOOT_ARGS[] = { 0x00 };

	DT__AddProperty(chosenNode, "boot-args", sizeof(BOOT_ARGS), &BOOT_ARGS);

	/* Adding kIOHibernateMachineSignatureKey (IOHibernatePrivate.h).
	 *
	 * This 'Hardware Signature' (offset 8 in the FACS table) is calculated by the BIOS on a best effort 
	 * basis to indicate the base hardware configuration of the system such that different base hardware 
	 * configurations  can have different hardware signature values. OSPM uses this information in waking 
	 * from an S4 state, by comparing the current hardware signature to the signature values saved in the 
	 * non-volatile sleep image. If the values are not the same, OSPM assumes that the saved non-volatile 
	 * image is from a different hardware configuration and cannot be restored.
	 */
	
	static EFI_UINT8 const MACHINE_SIGNATURE[] = { 0x00, 0x00, 0x00, 0x00 };

	DT__AddProperty(chosenNode, "machine-signature", sizeof(MACHINE_SIGNATURE), &MACHINE_SIGNATURE);

	// Adding the options node breaks AppleEFINVRAM (missing hardware UUID).
	// Node *optionsNode = DT__AddChild(gPlatform.DT.RootNode, "options");
	// DT__AddProperty(optionsNode, "EFICapsuleResult", 4, "STAR"); // 53 54 41 52

#endif

	gPlatform.EFI.Nodes.Chosen = chosenNode;

#if INJECT_EFI_DEVICE_PROPERTIES

	static EFI_UINT8 const EFI_DEVICE_PROPERTIES[] = 
	{
		STATIC_EFI_DEVICE_PROPERTIES
	};

	// DT__AddProperty(efiNode, "device-properties", sizeof(EFI_DEVICE_PROPERTIES), &EFI_DEVICE_PROPERTIES);
	DT__AddProperty(efiNode, "device-properties", sizeof(EFI_DEVICE_PROPERTIES), (EFI_CHAR8*)EFI_DEVICE_PROPERTIES);
#endif

	_EFI_DEBUG_DUMP("Exiting initEFITree()\n");
	_EFI_DEBUG_SLEEP(5);
}


//==============================================================================
// Stage two EFI initialization (after getting data from com.apple.Boot.plist).

void updateEFITree(char *rootUUID)
{
	_EFI_DEBUG_DUMP("In updateEFITree(%s)\n", rootUUID);

	static EFI_CHAR8 const SYSTEM_ID[] = STATIC_SYSTEM_ID;

	// This is your hardcoded SYSTEM_ID (I have mine in private_data.h).
	EFI_UINT32 * targetUUID = (EFI_UINT32 *) &SYSTEM_ID;

#if UNUSED_EFI_CODE
	// Feature to set your own, uuidgen generated string in com.apple.Boot.plist
	// Note: Unsupported feature in Revolution (you need to compile it anyway).
	
	const char* userDefinedUUID = newStringForKey("SystemID", &bootInfo->bootConfig);
	
    if (userDefinedUUID)
    {
		targetUUID = getUUIDFromString(userDefinedUUID);
		
		if (targetUUID)
		{
			verbose("Customizing SystemID with: %s\n", userDefinedUUID);
		}
		
		free ((void*) userDefinedUUID);
    }
	
    setupDeviceProperties(gPlatform.DT.RootNode); // /efi/device-properties.
#endif

	DT__AddProperty(gPlatform.EFI.Nodes.Platform, "system-id", 16, targetUUID);
	// gPlatform.EFI.Nodes.Platform = NULL;

	setRootUUID(gPlatform.EFI.Nodes.Chosen, rootUUID);
	// gPlatform.EFI.Nodes.Chosen = NULL;

	/* 
	 * Check the architectures' target (boot) mode against the chosen setting of  
	 * EFI_64_BIT, which may not fit well and throw the dreadful
	 * "tsc_init: EFI not supported" (due to a mismatch of the chosen/supported 
	 * boot/EFI mode) or KP with a page fault after booting with EFI_64_BIT set 
	 * to 0 on a 64-bit platform (can be done - read my comment in platform.c).
	 */

	if (bootArgs->efiMode != ((EFI_64_BIT) ? 64 : 32))
	{
		stop("EFI_64_BIT setting (%d) error detected (%d)!\n", EFI_64_BIT, bootArgs->efiMode);
	}

	_EFI_DEBUG_SLEEP(5);
}


//==============================================================================
// Called from initEFITree() and execKernel() in boot.c

bool setRootUUID(Node *chosenNode, char *rootUUID)
{
	// Only accept a UUID with the correct length.
	if (strlen(rootUUID) == 36)
	{
		_EFI_DEBUG_DUMP("Initializing property boot-uuid (%s)\n", rootUUID);
		
		DT__AddProperty(chosenNode, "boot-uuid", 37, rootUUID);
		
		return 0;
	}	
	
	return -1;
}


//==============================================================================
// Called from execKernel() in boot.c

void finalizeEFITree(void)
{
	_EFI_DEBUG_DUMP("Calling setupEFITables(");

	setupEFITables();

	_EFI_DEBUG_DUMP("done).\nCalling setupSMBIOS(");

	setupSMBIOS();

	_EFI_DEBUG_DUMP("done).\nAdding EFI configuration table for SMBIOS(");

	addConfigurationTable(&gPlatform.SMBIOS.Guid, &gPlatform.SMBIOS.BaseAddress, NULL);

	_EFI_DEBUG_DUMP("done)\nCalling setupACPI(");

	// DHP: Pfff. Setting DEBUG to 1 in acpi_patcher.c breaks our layout!
	setupACPI();

	_EFI_DEBUG_DUMP("done).\nAdding EFI configuration table for ACPI(");

	addConfigurationTable(&gPlatform.ACPI.Guid, &gPlatform.ACPI.BaseAddress, "ACPI_20");

	_EFI_DEBUG_DUMP("done).\nFixing checksum... ");

	// Now fixup the CRC32 and we're done here.
	gPlatform.EFI.SystemTable->Hdr.CRC32 = 0;
	gPlatform.EFI.SystemTable->Hdr.CRC32 = crc32(0L, 
												 gPlatform.EFI.SystemTable, 
												 gPlatform.EFI.SystemTable->Hdr.HeaderSize);

	_EFI_DEBUG_DUMP("(done).\n");
	_EFI_DEBUG_SLEEP(10);
}
