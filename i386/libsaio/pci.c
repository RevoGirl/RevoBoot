/*
 *
 * Copyright 2008 by Islam M. Ahmed Zaid. All rights reserved.
 *
 * Refactoring done by DHP for Revolution in 2011.
 *
 */

#include "libsaio.h"
#include "pci.h"

//==============================================================================

uint32_t pciConfigRead(uint8_t readType, uint32_t pciAddress, uint8_t pciRegister)
{
	uint32_t data = -1;

	pciAddress |= (pciRegister & ~3);
	outl(PCI_ADDR_REG, pciAddress);

	switch (readType)
	{
		case READ_BYTE: 
				data = inb(PCI_DATA_REG + (pciRegister & 3));
			break;
		
		case READ_WORD: 
				data = inw(PCI_DATA_REG + (pciRegister & 2));
			break;

		case READ_LONG: 
				data = inl(PCI_DATA_REG);
			break;
	}

	return data;
}

