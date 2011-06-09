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
#define RAM_SLOT_EMPTY	""

#ifndef DYNAMIC_RAM_OVERRIDE_TYPE
	#define DYNAMIC_RAM_OVERRIDE_TYPE	SMB_MEM_TYPE_DDR3
#endif

#ifndef DYNAMIC_RAM_OVERRIDE_FREQUENCY
	#define DYNAMIC_RAM_OVERRIDE_FREQUENCY 1066
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
	
	#if OVERRIDE_PRODUCT_NAME
		if (!strcmp(aKeyString, "SMBproductName"))
		{
			return STATIC_MAC_PRODUCT_NAME;
		},
	#endif
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

static int getCPUType(void)
{
	_SMBIOS_DEBUG_DUMP("In getCPUType() = 0x%x\n", gPlatform.CPU.Type);

	return gPlatform.CPU.Type;
}


//==============================================================================
#if OVERRIDE_DYNAMIC_MEMORY_DETECTION

int getSlotNumber(int slotNumber)
{
	_SMBIOS_DEBUG_DUMP("In getSlotNumber(%d)\n", slotNumber);

	// Return -1 for empty slots.
	if (gPlatform.RAM.MODULE[slotNumber].InUse == false)
	{
		return -1;
	}

	// Limit structureIndex to STATIC_RAM_SLOTS (defined in: config/settings.h)
	if (slotNumber > gPlatform.RAM.SlotCount)
	{
		slotNumber = 0;
	}

#if DEBUG_SMBIOS
	// sleep(1);	// Silent sleep (for debug only / slows down the process).
#endif

	return slotNumber;
}
#endif


//==============================================================================
#if DYNAMIC_RAM_OVERRIDE_SIZE

static int getRAMSize(int structureIndex)
{
	int slotNumber = getSlotNumber(structureIndex);

	_SMBIOS_DEBUG_DUMP("In getRAMSize(%d) = %d\n", structureIndex, gPlatform.RAM.MODULE[slotNumber].Size);

	if (gPlatform.RAM.MODULE[slotNumber].Size == SMB_MEM_BANK_EMPTY)
	{
		return 0;
	}

	return gPlatform.RAM.MODULE[slotNumber].Size;
}
#endif


//==============================================================================
#if DYNAMIC_RAM_OVERRIDE_TYPE

static int getRAMType(void)
{
	_SMBIOS_DEBUG_DUMP("In getRAMType() = %s\n", SMBMemoryDeviceTypes[DYNAMIC_RAM_OVERRIDE_TYPE]);

	return DYNAMIC_RAM_OVERRIDE_TYPE;
}
#endif


//==============================================================================
#if DYNAMIC_RAM_OVERRIDE_FREQUENCY

static int getRAMFrequency(void)
{
	_SMBIOS_DEBUG_DUMP("In getRAMFrequency() = %d\n", DYNAMIC_RAM_OVERRIDE_FREQUENCY);

	return DYNAMIC_RAM_OVERRIDE_FREQUENCY;
}
#endif


//==============================================================================

static const char * getRAMVendor(int structureIndex, void * structurePtr)
{
#if OVERRIDE_DYNAMIC_MEMORY_DETECTION
	_SMBIOS_DEBUG_DUMP("In getRAMVendor(%d)", structureIndex);

	int slotNumber = getSlotNumber(structureIndex);

	if (slotNumber == -1)
	{
		_SMBIOS_DEBUG_DUMP("\n");

		return RAM_SLOT_EMPTY;
	}

	_SMBIOS_DEBUG_DUMP(" = %s\n", gPlatform.RAM.MODULE[slotNumber].Vendor);

	return gPlatform.RAM.MODULE[slotNumber].Vendor;
#else
	_SMBIOS_DEBUG_DUMP("In getRAMVendor(%d) = N/A\n", structureIndex);

	return NOT_AVAILABLE;
#endif
}


//==============================================================================

static const char * getRAMSerialNumber(int structureIndex, void * structurePtr)
{
#if OVERRIDE_DYNAMIC_MEMORY_DETECTION
	_SMBIOS_DEBUG_DUMP("In getRAMSerialNumber(%d)", structureIndex);

	int slotNumber = getSlotNumber(structureIndex);

	if (slotNumber == -1)
	{
		_SMBIOS_DEBUG_DUMP("\n");

		return RAM_SLOT_EMPTY;
	}

	_SMBIOS_DEBUG_DUMP(" = %s\n", gPlatform.RAM.MODULE[slotNumber].SerialNumber);

	return gPlatform.RAM.MODULE[slotNumber].SerialNumber;
#else
	_SMBIOS_DEBUG_DUMP("In getRAMSerialNumber(%d) = N/A\n", structureIndex);

	return NOT_AVAILABLE;
#endif
}


//==============================================================================

static const char * getRAMPartNumber(int structureIndex, void * structurePtr)
{
#if OVERRIDE_DYNAMIC_MEMORY_DETECTION
	_SMBIOS_DEBUG_DUMP("In getRAMPartNumber(%d)", structureIndex);

	int slotNumber = getSlotNumber(structureIndex);

	if (slotNumber == -1)
	{
		_SMBIOS_DEBUG_DUMP("\n");
		
		return RAM_SLOT_EMPTY;
	}

	_SMBIOS_DEBUG_DUMP(" = %s\n", gPlatform.RAM.MODULE[slotNumber].PartNumber);

	return gPlatform.RAM.MODULE[slotNumber].PartNumber;
#else
	_SMBIOS_DEBUG_DUMP("In getRAMPartNumber(%d) = N/A\n", structureIndex);

	return NOT_AVAILABLE;
#endif
}


