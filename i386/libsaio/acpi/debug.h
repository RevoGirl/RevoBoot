/*
 * Copyright 2009 by Master Chief.
 *
 * Refactored (dynamic and static ACPI patching) by DHP in 2010/2011
 */

#ifndef __LIBSAIO_ACPI_DEBUG_H
#define __LIBSAIO_ACPI_DEBUG_H

#if DEBUG_ACPI

	//------------------------- Used in acpi/patcher.h -------------------------

	#define _ACPI_GET(xtr, x, y) strncpy(str, x, y); str[y] = '\0'

	// #define _ACPI_DEBUG_DUMP(x...) printf(x)
	// #define _ACPI_DEBUG_SLEEP(seconds) printf("Sleeping for %d seconds...\n", seconds); sleep(seconds)
	#define _ACPI_DEBUG_DUMP_TABLENAME(table, tableName, index) dumpTableName((void *)table, (char *)tableName, (int)index)

	#define _ACPI_DUMP_RSDP_TABLE(table, header) tableDumpRSDP((struct acpi_2_rsdp *)table, (char *)header)
	#define _ACPI_DUMP_XSDT_TABLE(table, header) tableDumpXSDT((struct acpi_2_xsdt *)table, (char *)header)

	//--------------------------------------------------------------------------

	char str[9] = "";
	char * tableName[] = { 0, 0, 0, 0, 0 };


	//==========================================================================

	void dumpTableName(void *table, char * tableName, int index)
	{
		strncpy((char *)tableName, (char *)table, 4);
		tableName[4] = '\0';

		printf("Found tableSignature(%d): %s ", (index + 1), tableName);
		sleep(1);
	}


	//==========================================================================

	void tableDumpRSDP(struct acpi_2_rsdp * rsdp, char * header)
	{
		printf("\n%s RSDP:\n", header);
		printf("=======================\n");
		_ACPI_GET(str, rsdp->Signature, 8);
		printf("rdsp->Signature       : %s\n", str);
		printf("rdsp->Checksum        : 0x%x\n", rsdp->Checksum);
		_ACPI_GET(str, rsdp->OEMID, 6);
		printf("rdsp->OEMID           : %s\n", str);
		printf("rsdp->Revision        : %d\n", rsdp->Revision);
		printf("rsdp->RsdtAddress     : 0x%x\n", rsdp->RsdtAddress);
	
		if (rsdp->Revision)
		{
			printf("rsdp->Length          : 0x%x\n", rsdp->Length);
			printf("rsdp->XsdtAddress     : 0x%x\n", rsdp->XsdtAddress);
			printf("rdsp->ExtendedChecksum: 0x%x\n", rsdp->ExtendedChecksum);
			_ACPI_GET(str, rsdp->Reserved, 3);
			printf("rdsp->Reserved        : %s\n", str);
		}
		
		_ACPI_DEBUG_SLEEP(5);
	}


	//==========================================================================

	void tableDumpXSDT(struct acpi_2_xsdt * table, char * header)
	{
		_ACPI_GET(str, table->Signature, 4);
		printf("\n%s %s:\n", header, str);
		printf("=======================\n");
		printf("xsdt->Signature       : %s\n", str);
		printf("xsdt->Length          : 0x%x\n", table->Length);
		printf("xsdt->Revision        : %d\n", table->Revision);
		printf("xsdt->Checksum        : 0x%x\n", table->Checksum);
		_ACPI_GET(str, table->OEMID, 6);
		printf("xsdt->OEMID           : %s\n", str);
		_ACPI_GET(str, table->OEMTableID, 8);
		printf("xsdt->OEMTableId      : %s\n", str);
		printf("xsdt->OEMRevision     : 0x%x\n", table->OEMRevision);
		printf("xsdt->CreatorId       : 0x%x\n", table->CreatorID);
		printf("xsdt->CreatorRevision : 0x%x\n", table->CreatorRevision);

		_ACPI_DEBUG_SLEEP(5);
	}

#else

	//--------------------------- Void replacements ----------------------------

	#define _ACPI_GET(xtr, x, y)

	// define _ACPI_DEBUG_DUMP(x...)
	// #define _ACPI_DEBUG_SLEEP(seconds)
	#define _ACPI_DEBUG_DUMP_TABLENAME(table, tableName, index)

	#define _ACPI_DUMP_RSDP_TABLE(table, header)
	#define _ACPI_DUMP_XSDT_TABLE(table, header)

	//--------------------------------------------------------------------------

#endif


#endif /* !__LIBSAIO_ACPI_DEBUG_H */
