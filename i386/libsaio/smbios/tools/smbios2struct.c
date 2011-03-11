/***
  *
  * Name        : smbios2struct (pipe output to file)
  * Version     : 1.0.1
  * Type        : Command line tool
  * Description : Tool to extract / convert the SMBIOS data into a structure for Revolution's smbios/data.h
  *
  * Copyright   : DutchHockeyPro (c) 2010
  *
  * Compile with: cc -I . smbios2struct.c -o smbios2struct -Wall -framework IOKit -framework CoreFoundation
  *
  */

#include <stdio.h>
#include <stdlib.h>

#include <IOKit/IOKitLib.h>
#include <CoreFoundation/CoreFoundation.h>

#include "essentials.h"

#define DEBUG	1 // Set to 1 for additional (debug) output
#define	round2(x, m)	(((x) + (m / 2)) & ~(m - 1)) // Taken from saio_types.h

int main(int argc, char * argv[])
{
	int index = 0;
	mach_port_t		masterPort;
    io_service_t	service = MACH_PORT_NULL;
	CFDataRef		dataRef;

	UInt16 maxStructureSize, structureCount = 0;

	IOMasterPort(MACH_PORT_NULL, &masterPort);
	service = IOServiceGetMatchingService(masterPort, IOServiceMatching("AppleSMBIOS"));

    if (service)
	{
#if DEBUG
		printf("\nHave AppleSMBIOS\n");
#endif
		dataRef = (CFDataRef) IORegistryEntryCreateCFProperty(service, 
															  CFSTR("SMBIOS-EPS"), 
															  kCFAllocatorDefault, 
															  kNilOptions);

		if (dataRef)
		{
			// We could use isA_CFData(dataRef) {} here, but that would force us to include 
			// another file, which in my opinion would be overkill (dataRef should always be there).
#if DEBUG
			printf("Have SMBIOS-EPS dataRef\n\n");
#endif
			struct SMBEntryPoint * smbiosEPSData = (struct SMBEntryPoint *) CFDataGetBytePtr(dataRef);
#if DEBUG
			// _SM_ 
			smbiosEPSData->anchor[4] = 0; // Prevent garbage output.
			printf("anchor            : %s\n", smbiosEPSData->anchor);
			printf("checksum          : 0x%x\n", smbiosEPSData->checksum);
			printf("entryPointLength  : 0x%x\n", smbiosEPSData->entryPointLength);
			printf("majorVersion      : 0x%02x\n", smbiosEPSData->majorVersion);
			printf("minorVersion      : 0x%02x\n", smbiosEPSData->minorVersion);
#endif
			maxStructureSize = smbiosEPSData->maxStructureSize;
#if DEBUG
			printf("maxStructureSize  : 0x%04x\n", maxStructureSize);
			printf("entryPointRevision: 0x%02x\n", smbiosEPSData->entryPointRevision);
			printf("formattedArea     : %s\n\n", smbiosEPSData->formattedArea);

			// _DMI_
			smbiosEPSData->dmi.anchor[5] = 0; // Prevent garbage output.
			printf("dmi.anchor        : %s\n", smbiosEPSData->dmi.anchor);
			printf("dmi.checksum      : 0x%02x\n", smbiosEPSData->dmi.checksum);
			printf("dmi.tableLength   : 0x%04x\n", smbiosEPSData->dmi.tableLength);
			printf("dmi.tableAddress  : 0x%08x\n", (unsigned int)smbiosEPSData->dmi.tableAddress);
#endif
			structureCount = smbiosEPSData->dmi.structureCount;
#if DEBUG
			printf("dmi.structureCount: 0x%04x\n", structureCount);
			printf("dmi.bcdRevision   : 0x%x\n", smbiosEPSData->dmi.bcdRevision);
#endif
			// Done with dataRef / release it.
			CFRelease(dataRef);
		}
#if DEBUG			
		printf("\n");
#endif

		dataRef = (CFDataRef) IORegistryEntryCreateCFProperty(service, 
															  CFSTR("SMBIOS"), 
															  kCFAllocatorDefault, 
															  kNilOptions);

		if (dataRef)
		{
			// We could use isA_CFData(dataRef) {} here, but that would force us to include another
			// header file, which in my opinion would be overkill as dataRef should always be there.
#if DEBUG
			printf("Have SMBIOS dataRef\n");
#endif
			UInt32 * smbiosData = (UInt32 *) CFDataGetBytePtr(dataRef);
			UInt16 numBytes = (int) CFDataGetLength(dataRef);

#if DEBUG
			printf("Number of bytes   : %d\n", numBytes);
#endif
			UInt16 length = round2((numBytes / 4), 4); // Is this right?

			printf("\n#define	SM_STRUCTURE_SIZE\t%d\n", maxStructureSize);
			printf("#define	DMI_STRUCTURE_COUNT\t%d\n", structureCount);
			printf("\n");
			printf("/* SMBIOS data (0x%04x / %d values) converted with smbios2struct into little endian format. */\n", length, length);
			printf("static uint32_t SMBIOS_Table[] = \n");
			printf("{\n\t/* 0x0000 */\t");

			do {
				printf("0x%08x", (unsigned int)smbiosData[index++]);

				if (index < length)
				{
					printf(", ");
				}
				
				// We want a newline character after every 8th double-word.
				if ((index % 8) == 0)
				{
					printf("\n\t/* 0x%04x */\t", (index * 4));
				}
				
			} while (index < length);

			printf("\n};\n");

			// Done with dataRef / release it.
			CFRelease(dataRef);
		}

		// Done with the service / release it.
		IOObjectRelease(service);
    }

	exit(0);
}
