/*
 *  platform.h
 */

#ifndef __LIBSAIO_PLATFORM_H
#define __LIBSAIO_PLATFORM_H

#include "libsaio.h"
#include "cpu/essentials.h"
#include "efi/essentials.h"
#include "efi/efi.h"
#include "device_tree.h"

//------------------------------------------------------------------------------

#define	LEOPARD					1	// Not supported.
#define	SNOW_LEOPARD			2	// The default.
#define LION					3	// For future use only.

//------------------------------------------------------------------------------

#define SMB_MEM_TYPE_DDR2		19
#define SMB_MEM_TYPE_FBDIMM		20
#define SMB_MEM_TYPE_DDR3		24

//------------------------------------------------------------------------------

#define SMB_MEM_BANK_EMPTY      1	// For empty slots.
#define SMB_MEM_SIZE_1GB		1024
#define SMB_MEM_SIZE_2GB		(SMB_MEM_SIZE_1GB * 2)
#define SMB_MEM_SIZE_4GB		(SMB_MEM_SIZE_1GB * 4)
#define SMB_MEM_SIZE_8GB		(SMB_MEM_SIZE_1GB * 8)
#define SMB_MEM_SIZE_16GB		(SMB_MEM_SIZE_1GB * 16)
#define SMB_MEM_SIZE_32GB		(SMB_MEM_SIZE_1GB * 32)	// May not be supported!

//------------------------------------------------------------------------------

#define IMAC					1
#define MACBOOK					2
#define MACBOOKPRO				4
#define MACMINI					8
#define MACPRO					16

//------------------------------------------------------------------------------

#define kKernelCachePath		"/System/Library/Caches/com.apple.kext.caches/Startup"
#define kKernelCache			"kernelcache"

//------------------------------------------------------------------------------

#define EFI_SMBIOS_TABLE_GUID	{ 0xeb9d2d31, 0x2d88, 0x11d3, { 0x9a, 0x16, 0x0, 0x90, 0x27, 0x3f, 0xc1, 0x4d } }

#define EFI_ACPI_20_TABLE_GUID	{ 0x8868e871, 0xe4f1, 0x11d3, { 0xbc, 0x22, 0x0, 0x80, 0xc7, 0x3c, 0x88, 0x81 } }

#if INCLUDE_MPS_TABLE
	#define EFI_MPS_TABLE_GUID	{ 0xeb9d2d2f, 0x2d88, 0x11d3, { 0x9a, 0x16, 0x0, 0x90, 0x27, 0x3f, 0xc1, 0x4d } }
#endif

//------------------------------------------------------------------------------
// Number of physical memory slots.

#ifdef STATIC_RAM_SLOTS
	#define MAX_SLOTS	STATIC_RAM_SLOTS // Defined in config/settings.h
#else
	#define MAX_SLOTS	4
#endif

//==============================================================================

typedef struct _RamSlotInfo_t
{
	uint32_t		Size;					// Module size in MB
	uint32_t		Frequency;				// Frequency in Mhz.
	const char *	Vendor;
	const char *	PartNumber;
	const char *	SerialNumber;
	char *			spd;					// SPD Dump
	bool			InUse;
	uint8_t			Type;
	uint8_t			BankConnections;		// Table type 6, see (3.3.7)
	uint8_t			BankConnCnt;
} RamSlotInfo_t;


//==============================================================================

typedef struct _PlatformInfo_t
{
	cpu_type_t			ArchCPUType;				// Replacement for gArchCPUType

	int					AddressWidth;

	int					Type;						// Initialized in platform.c to 2 for MacBook [Pro/Air]  
													// or 1 for desktop PC's like: MacPro, Mac mini and iMac.

	long				LastKernelAddr;				// Used in AllocateKernelMemory
	
	char *				RevolutionVersionInfo;		// Introduced in Revolution v0.6.45

	int					OSType;						// Type of Operating System.

	char *				OSVersion;					// OS version initialized with "10.6" in platform.c and 
													// later updated in boot.c with the actual version info.

	char *				ModelID;					// Initialized in platform.c and used in boot.c

	char *				KextFileName;				// Initialized and used in drivers.c
	char *				KextFileSpec;				// Initialized and used in drivers.c
	char *				KextPlistSpec;				// Initialized and used in drivers.c

	char *				KernelCachePath;			// Initialized in platform.c and used in boot.c, driver.c

	int					BIOSDevice;					// Initialized in platform.c (formely know as gBIOSDev).

	BVRef				BootVolume;					// Initialized in disk.c
	BVRef				BootPartitionChain;			// Initialized in sys.c
	BVRef				RootVolume;					// Initialized in disk.c (used in sys.c).

	struct ACPI										// Used in acpi_patcher.h
	{
		uint8_t			Type;						// System type. Referring to FACP->PM_Profile.

		EFI_GUID		Guid;

		EFI_UINT		BaseAddress;				// Either 32 or 64-bit (depends on platform info).
	} ACPI;

	// Copied from: chameleon/i386/libsaio/platform.h
	struct CPU
	{
		bool		Mobile;					// Set to true (in cpu/dynamic_data.h) for Mobile CPU's.
		uint16_t	Type;					// CPU type ('cpu-type') used in the SMBIOS patcher.
		uint32_t	Features;				// CPU Features like MMX, SSE2, VT, MobileCPU
		uint32_t	Vendor;					// Vendor
		uint32_t	Signature;				// Signature
		uint32_t	Stepping;				// Stepping
		uint32_t	Model;					// Model
		uint32_t	ExtModel;				// Extended Model
		uint32_t	Family;					// Family
		uint32_t	ExtFamily;				// Extended Family
		uint32_t	NumCores;				// Number of cores per package
		uint32_t	NumThreads;				// Number of threads per package

#if USE_STATIC_CPU_DATA == 0 && DEBUG_CPU
		uint8_t		CurrCoef;				// Current Multiplier (busratio).
		uint8_t		MaxCoef;				// Max multiplier
		uint8_t		CurrDiv;
		uint8_t		MaxDiv;
#endif
		uint64_t	TSCFrequency;			// TSC Frequency Hz
		uint64_t	FSBFrequency;			// FSB Frequency Hz
		uint64_t	CPUFrequency;			// CPU Frequency Hz
		uint32_t	QPISpeed;				// QuickPath Interconnect Bus Speed
		uint8_t		CoreTurboRatio[1];		// Expanded in: libsaio/cpu_intel/dynamic_data.h

		char		BrandString[48];		// Brand/frequency string
		uint32_t	ID[MAX_CPUID_LEAVES][4];	// CPUID 0..4, 80..81 Raw Values
	} CPU;

    struct DMI								// Patch by: Asere / Rekursor.
	{
		int				RAMSlotsPopulated;	// Number of memory slots populated by SMBIOS.
		int				RAMSlotsSupported;	// Total number of on-board memory slots.
		int				RAMSlotsActive;		// Total number of active slots (with module inserted).
		int				MODULE[MAX_SLOTS];	// Information and SPD mapping for each slot.
	} DMI;

#if INCLUDE_MPS_TABLE
    struct MPS								// Multi Procesor Table
	{
		EFI_GUID		Guid;
		
		EFI_UINT		BaseAddress;		// Either 32 or 64-bit (depends on platform info).
	} MPS;
#endif // INCLUDE_MP_TABLE

    struct RAM								// Patch by: Asere / Rekursor.
	{
		uint64_t		Frequency;			// RAM Frequency
		uint32_t		Divider;			// Memory divider
		uint8_t			CAS;				// CAS 1/2/2.5/3/4/5/6/7
		uint8_t			TRC;
		uint8_t			TRP;
		uint8_t			RAS;
		uint8_t			Channels;			// Channel Configuration Single, Dual or Triple
		uint8_t			SlotCount;			// Previously NoSlots; - Maximum no of slots available
		uint8_t			Type;				// Standard SMBIOS v2.5 Memory Type
		/* Remove me */
	 	char *			BrandString;		// Branding String Memory Controller
		RamSlotInfo_t	MODULE[MAX_SLOTS];	// Information about each slot
	} RAM;

    struct DT								// Device Tree
	{
			Node *	RootNode;				// Path: /
	} DT;

    struct EFI
	{
		EFI_SYSTEM_TABLE *	SystemTable;	// Used to re-calculate the checksum at the end of the run.

		struct Nodes
		{
			Node *	Chosen;					// Path: /chosen
			Node *	MemoryMap;				// Path: /chosen/memory-map

											// EFI specific stuff.
			Node *	ConfigurationTable;		// Path: /efi/configuration-table
			Node *	RuntimeServices;		// Path: /efi/runtime-services
			Node *	Platform;				// Path: /efi/platform
		} Nodes;
	} EFI;

	struct SMBIOS
	{
		EFI_GUID		Guid;

		EFI_UINT		BaseAddress;		// Either 32 or 64-bit (depends on platform info).

		/* UInt8	Anchor[4];				// DHP: Not used yet =]\._oOo_./[=
		UInt8	Checksum;
		UInt8	EntryPointLength;
		UInt8	MajorVersion;
		UInt8	MinorVersion;
		UInt16	MaxStructureSize;
		UInt8	EntryPointRevision;
		UInt8	FormattedArea[5];

		struct DMI
		{
			UInt8	Anchor[5];
			UInt8	Checksum;
			UInt16	TableLength;
			UInt32	TableAddress;
			UInt16	StructureCount;
			UInt8	CcdRevision;
		} DMI; */
	} SMBIOS;

} PlatformInfo_t;


//------------------------------------------------------------------------------

/* cpu/static_data.h & cpu/dynamic_data.h */
extern void initCPUStruct(void);

/* platform.c */
extern cpu_type_t gArchCPUType;	// DHP: Fix / remove me!

extern PlatformInfo_t	gPlatform;

extern void initPlatform(int biosDevice);

extern cpu_type_t getArchCPUType(void);

#endif /* !__LIBSAIO_PLATFORM_H */
