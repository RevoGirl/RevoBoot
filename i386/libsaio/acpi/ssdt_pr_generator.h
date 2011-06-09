/*
 *	Original source code developed by DHP in May 2011.
 *
 *	Updates:
 *
 *			- First bug fixes / comments added by DHP in June 2011.
 *			- ...
 *			- ...
 *
 *	TODO:
 *
 *			- Using one fixed turbo mode is currently not supported (take care of this).
 *			- Figure out why we need the 4 extra bytes (sorry, I forgot it).
 *
 *	Credits:
 *			- Thanks to flAked for helping me with tiny ssdt_pr.dsl
 *			- Jeroen and Mike (testers).
 *
 *	Notes:
 *
 *			- We (currently) only support Sandy Bridge processors.
 *
 */


#include "cpu/proc_reg.h"


extern uint8_t getTDP(void);


//==============================================================================

void generateSSDT_PR(void)
{
	//--------------------------------------------------------------------------
	// Our AML data blocks.

	uint8_t SSDT_PM_HEADER[] =
	{
		/* 0000 */	0x53, 0x53, 0x44, 0x54, 0x24, 0x00, 0x00, 0x00, 
		/* 0008 */	0x01, 0xFF, 0x41, 0x50, 0x50, 0x4C, 0x45, 0x20, 
		/* 0010 */	0x43, 0x70, 0x75, 0x50, 0x6D, 0x00, 0x00, 0x00, 
		/* 0018 */	0x00, 0x10, 0x00, 0x00, 0x49, 0x4E, 0x54, 0x4C, 
		/* 0020 */	0x16, 0x03, 0x11, 0x20
	};
	
	uint8_t SCOPE_PR_CPU0[] =						// Scope (\_PR.CPU0) { }
	{
		/* 0000 */	0x10, 0xFF, 0xFF, 0x2E, 0x5F, 0x50, 0x52, 0x5F, 
		/* 0008 */	0x43, 0x50, 0x55, 0x30
	};
	
	#define INDEX_OF_SCOPE_LENGTH		0x01	// Points to 0xFF 0xFF in SCOPE_PR_CPU0

	uint8_t NAME_APSN[] =						// Name (APSN, 0xFF)
	{
		/* 0000 */	0x08, 0x41, 0x50, 0x53, 0x4E, 0x0A, 0xFF
	};

	#define INDEX_OF_APSN				0x06	// Points to 0xFF in NAME_APSN

	uint8_t NAME_APSS[] =						// Name (APSS, Package(0xFF) { })
	{
		/* 0000 */	0x08, 0x41, 0x50, 0x53, 0x53, 0x12, 0xFF, 0xFF, 
		/* 0008 */	0xFF
	};

	#define INDEX_OF_PACKAGE_LENGTH		0x06	// Points to 0xFF 0xFF in NAME_APSS
	#define INDEX_OF_P_STATES			0x08	// Points to 0xFF (first one) in NAME_APSS

	uint8_t PACKAGE_P_STATE[] =					// Package(0x06) { ... }
	{
		/* 0000 */	0x12, 0x14, 0x06, 0x0B, 0x00, 0x00, 0x0C, 0x00, 
		/* 0008 */	0x00, 0x00, 0x00, 0x0A, 0x0A, 0x0A, 0x0A, 0x0B, 
		/* 0010 */	0x00, 0x00, 0x0B, 0x00, 0x00
	};
	
	uint8_t METHOD_ACST[] =						// Method (ACST, Package(NN) { ... }
	{
		/* 0000 */	0x14, 0x49, 0x08, 0x41, 0x43, 0x53, 0x54, 0x00,
		/* 0008 */	0xA4, 0x12, 0x40, 0x08, 0x06, 0x01, 0x0A, 0x04, 
		/* 0010 */	0x12, 0x1D, 0x04, 0x11, 0x14, 0x0A, 0x11, 0x82, 
		/* 0018 */	0x0C, 0x00, 0x7F, 0x01, 0x02, 0x01, 0x00, 0x00, 
		/* 0020 */	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79, 0x00, 
		/* 0028 */	0x01, 0x0A, 0x03, 0x0B, 0xE8, 0x03, 0x12, 0x1E, 
		/* 0030 */	0x04, 0x11, 0x14, 0x0A, 0x11, 0x82, 0x0C, 0x00, 
		/* 0038 */	0x7F, 0x01, 0x02, 0x03, 0x10, 0x00, 0x00, 0x00, 
		/* 0040 */	0x00, 0x00, 0x00, 0x00, 0x79, 0x00, 0x0A, 0x03, 
		/* 0048 */	0x0A, 0xCD, 0x0B, 0xF4, 0x01, 0x12, 0x1E, 0x04, 
		/* 0050 */	0x11, 0x14, 0x0A, 0x11, 0x82, 0x0C, 0x00, 0x7F, 
		/* 0058 */	0x01, 0x02, 0x03, 0x20, 0x00, 0x00, 0x00, 0x00, 
		/* 0060 */	0x00, 0x00, 0x00, 0x79, 0x00, 0x0A, 0x06, 0x0A, 
		/* 0068 */	0xF5, 0x0B, 0x5E, 0x01, 0x12, 0x1D, 0x04, 0x11, 
		/* 0070 */	0x14, 0x0A, 0x11, 0x82, 0x0C, 0x00, 0x7F, 0x01, 
		/* 0078 */	0x02, 0x03, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 
		/* 0080 */	0x00, 0x00, 0x79, 0x00, 0x0A, 0x07, 0x0A, 0xF5, 
		/* 0088 */	0x0A, 0xC8
	};
	
	uint8_t SCOPE_CPU_N[] =						// Scope (\_PR.CPUn) { ... }
	{
		/* 0000 */	0x10, 0x22, 0x5C, 0x2E, 0x5F, 0x50, 0x52, 0x5F, 
		/* 0008 */	0x43, 0x50, 0x55, 0x31, 0x14, 0x16, 0x41, 0x50, 
		/* 0010 */	0x53, 0x53, 0x00, 0xA4, 0x5C, 0x2F, 0x03, 0x5F, 
		/* 0018 */	0x50, 0x52, 0x5F, 0x43, 0x50, 0x55, 0x30, 0x41, 
		/* 0020 */	0x50, 0x53, 0x53
	};

	typedef struct acpi_2_pss
	{
		char		ignored_1[4];				// type, length and package items.
		uint16_t	Frequency;
		char		ignored_2;					// var type
		uint32_t	Power;
		char		ignored_3[5];				// var type, latency, var type, latency, var type.
		uint16_t	Ratio;
		char		ignored_4;					// var type
		uint16_t	Status;
	} __attribute__((packed)) ACPI_PSS;

	//--------------------------------------------------------------------------
	// Initialization.

	uint16_t	i	= 0;
	uint32_t	tdp = (getTDP() * 1000); // See: i386/libsaio/cpu/Intel/cpu.c

	struct acpi_2_ssdt * header = (struct acpi_2_ssdt *) SSDT_PM_HEADER;

	uint8_t		numberOfTurboRatios	= ((rdmsr64(IA32_MISC_ENABLES) >> 32) & 0x40) ? 0 : 4;
	uint8_t		numberOfPStates		= (gPlatform.CPU.MaxBusRatio - gPlatform.CPU.MinBusRatio) + numberOfTurboRatios + 1;

	//--------------------------------------------------------------------------
	// The first step is to calculate the length of the buffer for the AML code.
	
	uint32_t bufferSize =	header->Length + 
							sizeof(SCOPE_PR_CPU0) + 
							sizeof(NAME_APSN) + 
							sizeof(NAME_APSS) + 
							(sizeof(PACKAGE_P_STATE) * numberOfPStates) + 
							sizeof(METHOD_ACST) + 
							(sizeof(SCOPE_CPU_N) * (gPlatform.CPU.NumThreads - 1));

	void * buffer = malloc(bufferSize);
	void * bufferPointer = buffer;

	bzero(buffer, bufferSize); // Clear AML buffer.

	//--------------------------------------------------------------------------
	// Here we copy the header into the newly created buffer.
	
	bcopy(SSDT_PM_HEADER, buffer, sizeof(SSDT_PM_HEADER));
	bufferPointer += sizeof(SSDT_PM_HEADER);

	//--------------------------------------------------------------------------
	// Taking care of the Scope size.

	uint16_t size =	sizeof(SCOPE_PR_CPU0) + 
					sizeof(NAME_APSN) + 
					sizeof(NAME_APSS) + 
					(sizeof(PACKAGE_P_STATE) * numberOfPStates) +
					sizeof(METHOD_ACST);
	
	SCOPE_PR_CPU0[ INDEX_OF_SCOPE_LENGTH ]		= (0x40 | (size & 0x0f)) - 1;
	SCOPE_PR_CPU0[ INDEX_OF_SCOPE_LENGTH + 1 ]	= ((size >> 4) & 0xff);

	//--------------------------------------------------------------------------
	// Here we add the following AML code:
	//
	//	Scope (\_PR.CPU0)
	//	{
	//

	bcopy(SCOPE_PR_CPU0, bufferPointer, sizeof(SCOPE_PR_CPU0));
	bufferPointer += sizeof(SCOPE_PR_CPU0);
	
	//--------------------------------------------------------------------------
	// This step adds the following AML code:
	//
	//		Name (APSN, NN)
	
	// Are all core ratios set to the same value?
	if (gPlatform.CPU.CoreTurboRatio[0] == gPlatform.CPU.CoreTurboRatio[1] == 
		gPlatform.CPU.CoreTurboRatio[2] == gPlatform.CPU.CoreTurboRatio[3])
	{
		// Yes. Results in: Name (APSN, One).
		NAME_APSN[ INDEX_OF_APSN ] = 1; // DHP: We might not want to do this!

		// Is Turbo enabled /supported?
		if (numberOfTurboRatios)
		{
			// Yes. Limit the number of Turbo P-States to one.
			numberOfTurboRatios = 1;
		}
	}
	else
	{
		// Setting NN in: Name (APSN, NN)
		NAME_APSN[ INDEX_OF_APSN ] = numberOfTurboRatios;
	}

	bcopy(NAME_APSN, bufferPointer, sizeof(NAME_APSN));
	bufferPointer += sizeof(NAME_APSN);

	//--------------------------------------------------------------------------
	// Taking care of the Package size.
	
	size = (sizeof(PACKAGE_P_STATE) * numberOfPStates) + 4; // DHP: Why do we need the 4 extra bytes here?
	
	NAME_APSS[ INDEX_OF_PACKAGE_LENGTH ]		= (0x40 | (size & 0x0f)) - 1;
	NAME_APSS[ INDEX_OF_PACKAGE_LENGTH + 1 ]	= ((size >> 4) & 0xff);

	//--------------------------------------------------------------------------
	// This step adds the following AML code:
	//
	//		Name (APSS, Package (NN)
	//		{

	// Setting NN in: Name (APSS, Package (NN)
	NAME_APSS[ INDEX_OF_P_STATES ] = numberOfPStates;

	bcopy(NAME_APSS, bufferPointer, sizeof(NAME_APSS));
	bufferPointer += sizeof(NAME_APSS);

	//--------------------------------------------------------------------------
	// This step injects the AML code with the P-States:
	//
	//		Package (0x06) { 0xNNNN, 0xNNNN, 10, 10, 0xNN00, 0xNN00 }
	//		../..

	uint16_t	ratio, frequency, status;

	float m, power;

	struct acpi_2_pss * aPSS = (struct acpi_2_pss *) PACKAGE_P_STATE;

	//--------------------------------------------------------------------------
	// First the turbo P-States.

	for (i = 0; i < numberOfTurboRatios; i++)
	{
		ratio		= gPlatform.CPU.CoreTurboRatio[i];
		frequency	= (ratio * 100);
		ratio		= status = (ratio << 8);

		aPSS->Frequency	= frequency;
		aPSS->Power		= (uint32_t) tdp;		// Turbo States use max-power.
		aPSS->Ratio		= ratio;
		aPSS->Status	= ratio;
		
		bcopy(PACKAGE_P_STATE, bufferPointer, sizeof(PACKAGE_P_STATE));
		bufferPointer += sizeof(PACKAGE_P_STATE);
	}
	
	uint8_t	maxRatio = gPlatform.CPU.MaxBusRatio; // Warning: This one can be 59!
	
	i = gPlatform.CPU.MaxBusRatio;

	//--------------------------------------------------------------------------
	// And now the normal P-States.

	for (; i >= gPlatform.CPU.MinBusRatio; i--)
	{
		ratio		= i;
		frequency	= (i * 100);

		m			= ((1.1 - ((maxRatio - ratio) * 0.00625)) / 1.1);
		power		= (((float)ratio / maxRatio) * (m * m) * tdp);

		ratio		= status = (ratio << 8);

		aPSS->Frequency	= frequency;
		aPSS->Power		= (uint32_t) power;
		aPSS->Ratio		= ratio;
		aPSS->Status	= ratio;
	
		bcopy(PACKAGE_P_STATE, bufferPointer, sizeof(PACKAGE_P_STATE));
		bufferPointer += sizeof(PACKAGE_P_STATE);
	}

	//--------------------------------------------------------------------------
	// This step injects the following AML code:
	//
	//	Method (ACST, 0, NotSerialized)
	//	{
	//		Return (Package (0x06)
	//		{
	//			One,
	//			0x04,
	//			Package (0x04)
	//			{
	//				ResourceTemplate ()
	//				{
	//						Register (FFixedHW,
	//							0x01,
	//							0x02,
	//							0x0000000000000000,
	//							0x01,
	//							)
	//				},
	//
	//				One,
	//				0x03,
	//				0x03E8
	//			},
	//
	//			Package (0x04)
	//			{	
	//				ResourceTemplate ()
	//				{
	//						Register (FFixedHW,
	//							0x01,
	//							0x02,
	//							0x0000000000000010,
	//							0x03,
	//							)
	//				},
	//
	//				0x03,
	//				0xCD, 
	//				0x01F4
	//			},
	//
	//			Package (0x04)
	//			{	
	//				ResourceTemplate ()
	//				{
	//						Register (FFixedHW,
	//							0x01,
	//							0x02,
	//							0x0000000000000020,
	//							0x03,
	//							)
	//				},
	//
	//				0x06,
	//				0xF5, 
	//				0x015E
	//			},
	//
	//			Package (0x04)
	//			{	
	//				ResourceTemplate ()
	//				{
	//						Register (FFixedHW,
	//							0x01,
	//							0x02,
	//							0x0000000000000030,
	//							0x03,
	//							)
	//				},
	//
	//				0x07,
	//				0xF5, 
	//				0xC8
	//			},
	//		})
	//	}

	bcopy(METHOD_ACST, bufferPointer, sizeof(METHOD_ACST));
	bufferPointer += sizeof(METHOD_ACST);

	//--------------------------------------------------------------------------
	// This step adds the following AML code - one for each logical core:
	//
	//	Scope (\_PR.CPUn)
	//	{
	//		Method (APSS, 0, NotSerialized)
	//		{
	//			Return (\_PR.CPU0.APSS)
	//		}
	//	}

	for (i = 1; i < gPlatform.CPU.NumThreads; i++)
	{
		SCOPE_CPU_N[0x0B] = (0x30 + i);			// Set CPU number.
		bcopy(SCOPE_CPU_N, bufferPointer, sizeof(SCOPE_CPU_N));
		bufferPointer += sizeof(SCOPE_CPU_N);
	}

	//--------------------------------------------------------------------------
	// Here we generate a new checksum.
	// Note: The length and checksum must be the same as in ssdt_pr.aml
	
	header = (struct acpi_2_ssdt *) buffer;
	
	header->Length		= bufferSize;
	header->Checksum	= 0;
	header->Checksum	= 256 - checksum8(buffer, header->Length);

	//--------------------------------------------------------------------------
	// Updating customTables with the required data.

	customTables[SSDT_PR].table			= (void *)(uint32_t)buffer;
	customTables[SSDT_PR].tableLength	= bufferSize;
	customTables[SSDT_PR].loaded		= true;	// Simulate a file load - to let it call free()
}
