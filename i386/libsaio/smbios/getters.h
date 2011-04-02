/*
 * The first SMBIOS patcher was developped by mackerintel in 2008, which 
 * was ripped apart and rewritten by Master Chief for Revolution in 2009.
 *
 * Updates:
 *
 *			- Dynamic and static SMBIOS data gathering added by DHP in 2010.
 *			- Complete rewrite / overhaul by DHP in Februari 2011.
 *			- Coding style change by DHP in Februari 2011.
 *
 * Credits:
 *			- macerintel (see note in source code)
 *			- blackosx, DB1, dgsga, FKA, humph, scrax and STLVNUB (testers).
 */


#define SMBIOS_SEARCH_BASE      0x000F0000
#define SMBIOS_SEARCH_END       0x000FFFFF
#define SMBIOS_ANCHOR			0x5f4d535f // '_SM_' in Little Endian.
#define SMBIOS_MPS_ANCHOR		0x5f504d5f // '_MP_' in Little Endian. 

#define NOT_AVAILABLE	"N/A"

#ifndef STATIC_RAM_TYPE
	#define STATIC_RAM_TYPE	SMB_MEM_TYPE_DDR3
#endif

#ifndef STATIC_RAM_SPEED
	#define STATIC_RAM_SPEED 1066
#endif

/*==============================================================================
 * On non-EFI systems, like our hackintosh, the SMBIOS Entry Point Structure can 
 * be located by searching for the anchor-string on paragraph (16-byte) 
 * boundaries within the physical memory address range 000F0000h to 000FFFFFh.
 */

static inline struct SMBEntryPoint * getEPSAddress(void)
{
	_SMBIOS_DEBUG_DUMP("in getEPSAddress()\n");

	void *baseAddress = (void *)SMBIOS_SEARCH_BASE;

	for(; baseAddress <= (void *)SMBIOS_SEARCH_END; baseAddress += 16)
	{
		if (*(uint32_t *)baseAddress == SMBIOS_ANCHOR) // _SM_
		{
			if (checksum8(baseAddress, sizeof(struct SMBEntryPoint)) == 0)
			{
#if INCLUDE_MP_TABLE
				// SMBIOS table located. Use this address as starting point.
				void * mpsAddress = baseAddress;

				// Now search for the Multi Processor table.
				for(; mpsAddress <= (void *)SMBIOS_SEARCH_END; mpsAddress += 16)
				{
					if (*(uint32_t *)mpsAddress == SMBIOS_MPS_ANCHOR)
					{
						gPlatform.MP.BaseAddress = (uint32_t)mpsAddress;

						_SMBIOS_DEBUG_DUMP("SMBIOS baseAddress: 0x%8x\n", baseAddress);
						_SMBIOS_DEBUG_DUMP("MultiP baseAddress: 0x%8x\n", mpsAddress);
						_SMBIOS_DEBUG_SLEEP(10);

						break;
					}
				}
#endif // INCLUDE_MP_TABLE

				return baseAddress;
			}
			else
			{
				verbose("Found SMBIOS anchor with bad checksum. ");
			}
		}
	}

	verbose("Unable to locate SMBIOS table anchor.\n");
	sleep(5);

	return 0;
}


//==============================================================================
// Originally developed, for the first SMBIOS patcher, by macerintel in 2008. 
// Refactorized by DHP in Februari 2010 (also renamed in 2011).

static const char * getOverrideString(const char * aKeyString)
{
	int i = 0;
	const SMBPropertyData * keyValues;

#if OVERRIDE_DYNAMIC_PRODUCT_DETECTION
	keyValues = STATIC_DEFAULT_DATA;
#else
	if (gPlatform.CPU.Mobile)
    {
        if (gPlatform.CPU.NumCores > 1)
		{
			keyValues = MacBookPro;
		}
		else
		{
			keyValues = MacBook;
		}
	}
	else
	{
		switch (gPlatform.CPU.NumCores)
		{
			case 1:		keyValues = Macmini;
				break;
			case 2:		keyValues = iMac;
				break;
			default:	keyValues = MacPro;
				break;
        }
    }
#endif
	for (i = 0; keyValues[i].key[0]; i++)
	{
		if (!strcmp(keyValues[i].key, aKeyString))
		{
			return keyValues[i].value;
		}
	}

	_SMBIOS_DEBUG_DUMP("Error: no default for '%s' known\n", aKeyString);
	_SMBIOS_DEBUG_SLEEP(5);

	return "";
}


//==============================================================================

static int getFSBFrequency(void)
{
	_SMBIOS_DEBUG_DUMP("In getFSBFrequency() = %d Hz\n", (gPlatform.CPU.FSBFrequency / 1000000));

    return gPlatform.CPU.FSBFrequency / 1000000;
}


//==============================================================================

static int getCPUFrequency(void)
{
	_SMBIOS_DEBUG_DUMP("In getCPUFrequency() = %d Hz\n", (gPlatform.CPU.CPUFrequency / 1000000));

    return gPlatform.CPU.CPUFrequency / 1000000;
}


//==============================================================================

static int getQPISpeed(void)
{
	_SMBIOS_DEBUG_DUMP("In getQPISpeed() = %d\n", gPlatform.CPU.QPISpeed);

	return gPlatform.CPU.QPISpeed; // QuickPath Interconnect Speed.
}


//==============================================================================

static int getCPUType(void)
{
	_SMBIOS_DEBUG_DUMP("In getCPUType() = 0x%x\n", gPlatform.CPU.Type);

	return gPlatform.CPU.Type;
}


#if OVERRIDE_DYNAMIC_MEMORY_DETECTION
//==============================================================================
// Helper function.

int getSlotNumber(int structureIndex)
{
	_SMBIOS_DEBUG_DUMP("In getSlotNumber(%d)\n", structureIndex);

	// Limit structureIndex to STATIC_RAM_SLOTS (defined in: config/settings.h)
	if (structureIndex > gPlatform.RAM.SlotCount)
	{
		structureIndex = 0;
	}

#if DEBUG_SMBIOS
	sleep(1);	// Silent sleep (for debug only).
#endif

	return structureIndex;
}
#endif


#if USE_STATIC_RAM_SIZE
//==============================================================================

static int getRAMSize(int structureIndex, void * structurePtr)
{
	struct SMBMemoryDevice * module = (SMBMemoryDevice *) structurePtr;
	printf("module->memorySize: %x\n", module->memorySize);

#if OVERRIDE_DYNAMIC_MEMORY_DETECTION
	_SMBIOS_DEBUG_DUMP("In getRAMSize(%d) = %d\n", structureIndex, gPlatform.RAM.MODULE[getSlotNumber(structureIndex)].Size);

	return gPlatform.RAM.MODULE[getSlotNumber(structureIndex)].Size;
#else
	return 2048;
#endif
}
#endif


//==============================================================================

static int getRAMType(int structureIndex, void * structurePtr)
{
	/* struct SMBMemoryDevice * module = (SMBMemoryDevice *) structurePtr;
	printf("module->memoryType: %x\n", module->memoryType); // , SMBMemoryDeviceTypes[module->memoryType]);
	sleep(5); */

	_SMBIOS_DEBUG_DUMP("In getRAMType() = %s\n", SMBMemoryDeviceTypes[STATIC_RAM_TYPE]);

	return STATIC_RAM_TYPE;
}


//==============================================================================

static int getRAMFrequency(void)
{
	_SMBIOS_DEBUG_DUMP("In getRAMFrequency() = %d\n", STATIC_RAM_SPEED);

	return STATIC_RAM_SPEED;
}


//==============================================================================

static const char * getRAMVendor(int structureIndex, void * structurePtr)
{
	/* struct SMBMemoryDevice * module = (SMBMemoryDevice *) structurePtr;
	printf("module->memoryType: %\n", module->memoryType); */

#if OVERRIDE_DYNAMIC_MEMORY_DETECTION
	_SMBIOS_DEBUG_DUMP("In getRAMVendor(%d) = %s\n", structureIndex, gPlatform.RAM.MODULE[getSlotNumber(structureIndex)].Vendor);

	return gPlatform.RAM.MODULE[getSlotNumber(structureIndex)].Vendor;
#else
	_SMBIOS_DEBUG_DUMP("In getRAMVendor(%d) = N/A\n", structureIndex);

	return NOT_AVAILABLE;
#endif
}


//==============================================================================

static const char * getRAMSerialNumber(int structureIndex, void * structurePtr)
{
#if OVERRIDE_DYNAMIC_MEMORY_DETECTION
	_SMBIOS_DEBUG_DUMP("In getRAMSerialNumber(%d) = %s\n", structureIndex, gPlatform.RAM.MODULE[getSlotNumber(structureIndex)].SerialNumber);

	return gPlatform.RAM.MODULE[getSlotNumber(structureIndex)].SerialNumber;
#else
	_SMBIOS_DEBUG_DUMP("In getRAMSerialNumber(%d) = N/A\n", structureIndex);

	return NOT_AVAILABLE;
#endif
}


//==============================================================================

static const char * getRAMPartNumber(int structureIndex, void * structurePtr)
{
#if OVERRIDE_DYNAMIC_MEMORY_DETECTION
	_SMBIOS_DEBUG_DUMP("In getRAMPartNumber(%d) = %s\n", structureIndex, gPlatform.RAM.MODULE[getSlotNumber(structureIndex)].PartNumber);

	return gPlatform.RAM.MODULE[getSlotNumber(structureIndex)].PartNumber;
#else
	_SMBIOS_DEBUG_DUMP("In getRAMPartNumber(%d) = N/A\n", structureIndex);

	return NOT_AVAILABLE;
#endif
}


