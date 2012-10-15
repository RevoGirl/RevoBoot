/*
 * The first SMBIOS patcher was developped by mackerintel in 2008, which 
 * was ripped apart and rewritten by Master Chief for Revolution in 2009.
 *
 * Updates:
 *
 *			- Dynamic and static SMBIOS data gathering added by DHP in 2010.
 *			- Complete rewrite / overhaul by DHP in Februari 2011.
 *			- Coding style changes by DHP in Februari 2011.
 *
 * Credits:
 *			- Valv (see note in source code)
 *			- blackosx, DB1, dgsga, FKA, humph, scrax and STLVNUB (testers).
 */


#if OVERRIDE_DYNAMIC_PRODUCT_DETECTION
	#define TARGET_MODEL		STATIC_SMBIOS_MODEL_ID
#else
	#define TARGET_MODEL		63				// All models for dynamic runs (without override).
#endif


// ------------------------------------------------------------
// Thanks to valv for bringing this under my attention.

typedef struct
{
    const char * key;
    const char * value;
} SMBPropertyData;


#if TARGET_MODEL & IMAC							// 1
// ------------------------------------------------------------
static const SMBPropertyData const iMac[] =
{
	{	"SMBbiosVendor",      "Apple Inc."				},
	{	"SMBbiosVersion",     "IM121.88Z.0047.B1D.1110171110"		},
	{	"SMBbiosDate",        "10/17/2011"				},
	{	"SMBmanufacter",      "Apple Inc."				},
	{	"SMBproductName",     "iMac12,2"				},
	{	"SMBsystemVersion",   "1.0"					},
	{	"SMBserial",          STATIC_SMSERIALNUMBER			},
	{	"SMBfamily",          "Mac"					},
	{	"SMBboardManufacter", "Apple Inc."				},
	{	"SMBboardProduct",    "Mac-942B59F58194171B"			},

/*	{	"SMBbiosVendor",      "Apple Inc."				},
	{	"SMBbiosVersion",     "IM111.88Z.0034.B00.0910301727"		},
	{	"SMBbiosDate",        "10/30/2009"				},
	{	"SMBmanufacter",      "Apple Inc."				},
	{	"SMBproductName",     "iMac11,1"				},
	{	"SMBsystemVersion",   "1.0"					},
	{	"SMBserial",          STATIC_SMSERIALNUMBER			},
	{	"SMBfamily",          "Mac"					},
	{	"SMBboardManufacter", "Apple Inc."				},
	{	"SMBboardProduct",    "Mac-F2268DAE"				}, */

/*	{	"SMBbiosVendor",      "Apple Inc."				},
	{	"SMBbiosVersion",     "IM121.88Z.0047.B1D.1110171110"		},
	{	"SMBbiosDate",        "10/17/2011"				},
	{	"SMBmanufacter",      "Apple Inc."				},
	{	"SMBproductName",     "iMac12,1"				},
	{	"SMBsystemVersion",   "1.0"					},
	{	"SMBserial",          STATIC_SMSERIALNUMBER			},
	{	"SMBfamily",          "Mac"					},
	{	"SMBboardManufacter", "Apple Inc."				},
	{	"SMBboardProduct",    "Mac-942B5BF58194151B"			}, */

	/* Intel Core i7-3770 @ 3.40 GHz - 4 Cores / 8 Threads.
	{	"SMBbiosVendor",      "Apple Inc."				},
	{	"SMBbiosVersion",     "IM131.88Z.00CE.B00.1203281326"		},
	{	"SMBbiosDate",        "03/28/2012"				},
	{	"SMBmanufacter",      "Apple Inc."				},
	{	"SMBproductName",     "iMac13,2"				},
	{	"SMBsystemVersion",   "1.0"					},
	{	"SMBserial",          STATIC_SMSERIALNUMBER			},
	{	"SMBfamily",          "Mac"					},
	{	"SMBboardManufacter", "Apple Inc."				},
	{	"SMBboardProduct",    "Mac-FC02E91DDD3FA6A4"			}, */

	{	"", "" }
};
// ------------------------------------------------------------
#endif


#if TARGET_MODEL & MACBOOK						// 2
// ------------------------------------------------------------
static const SMBPropertyData const MacBook[] =
{
	{	"SMBbiosVendor",      "Apple Inc."				},
	{	"SMBbiosVersion",     "MB41.88Z.0073.B00.0809221748"		},
	{	"SMBbiosDate",        "09/22/2008"				},
	{	"SMBmanufacter",      "Apple Inc."				},
	{	"SMBproductName",     "MacBook4,1"				},
	{	"SMBsystemVersion",   "1.0"					},
	{	"SMBserial",          STATIC_SMSERIALNUMBER			},
	{	"SMBfamily",          " MacBook"				},
	{	"SMBboardManufacter", "Apple Inc."				},
	{	"SMBboardProduct",    "Mac-F42D89C8"				},

	{	"", "" }
};
// ------------------------------------------------------------
#endif


#if TARGET_MODEL & MACBOOK_AIR					// 4
// ------------------------------------------------------------
static const SMBPropertyData const MacBookAir[] =
{	// Intel Core i5-2557M @ 1.7GHz (2 cores - 4 threads)
	// Intel Core i7-2677M @ 1.8GHz (2 cores - 4 threads)
	{	"SMBbiosVendor",      "Apple Inc."				},
	{	"SMBbiosVersion",     "MBA41.88Z.0077.B08.1109011050"		},
	{	"SMBbiosDate",        "09/01/2011"				},
	{	"SMBmanufacter",      "Apple Inc."				},
	{	"SMBproductName",     "MacBookAir4,2"				},
	{	"SMBsystemVersion",   "1.0"					},
	{	"SMBserial",          STATIC_SMSERIALNUMBER			},
	{	"SMBfamily",          "MacBookAir"				},
	{	"SMBboardManufacter", "Apple Inc."				},
	{	"SMBboardProduct",    "Mac-742912EFDBEE19B3"			},

	// Intel Core i5-2467M @ 1.6GHz (2 cores - 4 threads)
	// Intel Core i7-2677M @ 1.8GHz (2 cores - 4 threads)
/*	{	"SMBbiosVendor",      "Apple Inc."				},
	{	"SMBbiosVersion",     "MBA41.88Z.0077.B08.1109011050"		},
	{	"SMBbiosDate",        "09/01/2011"				},
	{	"SMBmanufacter",      "Apple Inc."				},
	{	"SMBproductName",     "MacBookAir4,1"				},
	{	"SMBsystemVersion",   "1.0"					},
	{	"SMBserial",          STATIC_SMSERIALNUMBER			},
	{	"SMBfamily",          "MacBookAir"				},
	{	"SMBboardManufacter", "Apple Inc."				},
	{	"SMBboardProduct",    "Mac-C08A6BB70A942AC2"			}, */

	{	"", ""	}
};
// ------------------------------------------------------------
#endif


#if TARGET_MODEL & MACBOOK_PRO					// 8
// ------------------------------------------------------------
static const SMBPropertyData const MacBookPro[] = // smc-version: 1.9.0027.0
{
/*	{	"SMBbiosVendor",	"Apple Inc."				},
	{	"SMBbiosVersion",	"MBP61.88Z.0057.B0C.1007261552"		},
	{	"SMBbiosDate",		"07/26/2010"				},
	{	"SMBmanufacter",	"Apple Inc."				},
	{	"SMBproductName",	"MacBookPro6,1"				},
	{	"SMBsystemVersion",	"1.0"					},
	{	"SMBserial",		STATIC_SMSERIALNUMBER			},
	{	"SMBfamily",		"MacBookPro"				},
	{	"SMBboardManufacter",	"Apple Inc."				},
	{	"SMBboardProduct",	"Mac-F22589C8"				}, */

/*	{	"SMBbiosVendor",	"Apple Inc."				},
	{	"SMBbiosVersion",	"MBP81.88Z.0047.B24.1110141131"		},
	{	"SMBbiosDate",		"10/14/2-11"				},
	{	"SMBmanufacter",	"Apple Inc."				},
	{	"SMBproductName",	"MacBookPro8,1"				},
	{	"SMBsystemVersion",	"1.0"					},
	{	"SMBserial",		STATIC_SMSERIALNUMBER			},
	{	"SMBfamily",		"MacBook Pro"				},
	{	"SMBboardManufacter",	"Apple Inc."				},
	{	"SMBboardProduct",	"Mac-94245B3640C91C81"			}, */

/*	{	"SMBbiosVendor",	"Apple Inc."				},
	{	"SMBbiosVersion",	"MBP81.88Z.0047.B24.1110141131"		},
	{	"SMBbiosDate",		"10/14/2011"				},
	{	"SMBmanufacter",	"Apple Inc."				},
	{	"SMBproductName",	"MacBookPro8,2"				},
	{	"SMBsystemVersion",	"1.0"					},
	{	"SMBserial",		STATIC_SMSERIALNUMBER			},
	{	"SMBfamily",		"MacBook Pro"				},
	{	"SMBboardManufacter",	"Apple Inc."				},
	{	"SMBboardProduct",		"Mac-94245A3940C91C80"		}, */

	{	"SMBbiosVendor",	"Apple Inc."				},
	{	"SMBbiosVersion",	"MBP81.88Z.0047.B24.1110141131"		},
	{	"SMBbiosDate",		"10/14/2011"				},
	{	"SMBmanufacter",	"Apple Inc."				},
	{	"SMBproductName",	"MacBookPro8,3"				},
	{	"SMBsystemVersion",	"1.0"					},
	{	"SMBserial",		STATIC_SMSERIALNUMBER			},
	{	"SMBfamily",		"MacBook Pro"				},
	{	"SMBboardManufacter",	"Apple Inc."				},
	{	"SMBboardProduct",	"Mac-942459F5819B171B"			},

	/* Intel Core i7-3820QM @ 2.70 GHz - 4 cores / 8 threads.
	{	"SMBbiosVendor",	"Apple Inc."				},
	{	"SMBbiosVersion",	"MBP91.88Z.00D3.B00.1203211536"		},
	{	"SMBbiosDate",		"21/03/2012"				},
	{	"SMBmanufacter",	"Apple Inc."				},
	{	"SMBproductName",	"MacBookPro9,1"				},
	{	"SMBsystemVersion",	"1.0"					},
	{	"SMBserial",		STATIC_SMSERIALNUMBER			},
	{	"SMBfamily",		"MacBook Pro"				},
	{	"SMBboardManufacter",	"Apple Inc."				},
	{	"SMBboardProduct",	"Mac-4B7AC7E43945597E"			}, */

	/* Intel Core i7-3720QM @ 2.60 GHz - 4 cores / 8 threads.
	{	"SMBbiosVendor",	"Apple Inc."				},
	{	"SMBbiosVersion",	"MBP101.88Z.00EE.B00.1205101839"	},
	{	"SMBbiosDate",		"10/05/2012"				},
	{	"SMBmanufacter",	"Apple Inc."				},
	{	"SMBproductName",	"MacBookPro10,1"			},
	{	"SMBsystemVersion",	"1.0"					},
	{	"SMBserial",		STATIC_SMSERIALNUMBER			},
	{	"SMBfamily",		"MacBook Pro"				},
	{	"SMBboardManufacter",	"Apple Inc."				},
	{	"SMBboardProduct",	"Mac-C3EC7CD22292981F"			}, */
	
	{	"", ""	}
};
// ------------------------------------------------------------
#endif


#if TARGET_MODEL & MACMINI						// 16
// ------------------------------------------------------------
static const SMBPropertyData const Macmini[] =
{	// Intel Core i7-2635QM @ 2.0GHz (4 cores - 8 threads)
	{	"SMBbiosVendor",      "Apple Inc."				},
	{	"SMBbiosVersion",     "MM51.88Z.0077.B10.1201241549"		},
	{	"SMBbiosDate",        "01/24/2012"				},
	{	"SMBmanufacter",      "Apple Inc."				},
	{	"SMBproductName",     "Macmini5,3"				},
	{	"SMBsystemVersion",   "1.0"					},
	{	"SMBserial",          STATIC_SMSERIALNUMBER			},
	{	"SMBfamily",          "Macmini"					},
	{	"SMBboardManufacter", "Apple Inc."				},
	{	"SMBboardProduct",    "Mac-7BA5B2794B2CDB12"			},

	// Intel Core i5-2520M @ 2.5GHz (2 cores - 4 threads)
/*	{	"SMBbiosVendor",      "Apple Inc."				},
	{	"SMBbiosVersion",     "MM51.88Z.0077.B10.1201241549"		},
	{	"SMBbiosDate",        "01/24/2012"				},
	{	"SMBmanufacter",      "Apple Inc."				},
	{	"SMBproductName",     "Macmini5,2"				},
	{	"SMBsystemVersion",   "1.0"					},
	{	"SMBserial",          STATIC_SMSERIALNUMBER			},
	{	"SMBfamily",          "Macmini"					},
	{	"SMBboardManufacter", "Apple Inc."				},
	{	"SMBboardProduct",    "Mac-4BC72D62AD45599E"			}, */
	
	// Intel Core i5-2415M @ 2.3 GHz (2 cores - 4 theeads)
/*	{	"SMBbiosVendor",      "Apple Inc."				},
	{	"SMBbiosVersion",     "MM51.88Z.0077.B10.1201241549"		},
	{	"SMBbiosDate",        "01/24/2012"				},
	{	"SMBmanufacter",      "Apple Inc."				},
	{	"SMBproductName",     "Macmini5,1"				},
	{	"SMBsystemVersion",   "1.0"					},
	{	"SMBserial",          STATIC_SMSERIALNUMBER			},
	{	"SMBfamily",          "Macmini"					},
	{	"SMBboardManufacter", "Apple Inc."				},
	{	"SMBboardProduct",    "Mac-8ED6AF5B48C039E1"			}, */

    {	"", "" }
};
// ------------------------------------------------------------
#endif


#if TARGET_MODEL & MACPRO						// 32
// ------------------------------------------------------------
static const SMBPropertyData const MacPro[] =
{
	{	"SMBbiosVendor",      "Apple Computer, Inc."			},
	{	"SMBbiosVersion",     "MP31.88Z.006C.B05.0802291410"		},
	{	"SMBbiosDate",        "08/03/2010"				},
	{	"SMBmanufacter",      "Apple Computer, Inc."			},
	{	"SMBproductName",     "MacPro3,1"				},
	{	"SMBsystemVersion",   "1.0"					},
	{	"SMBserial",          STATIC_SMSERIALNUMBER			},
	{	"SMBfamily",          "MacPro"					},
	{	"SMBboardManufacter", "Apple Computer, Inc."			},
	{	"SMBboardProduct",    "Mac-F4208DC8"				},

/*	{	"SMBbiosVendor",      "Apple Computer, Inc."			},
	{	"SMBbiosVersion",     "MP41.88Z.0081.B04.0903051113"		},
	{	"SMBbiosDate",        "11/06/2009"				},
	{	"SMBmanufacter",      "Apple Inc."				},
	{	"SMBproductName",     "MacPro4,1"				},
	{	"SMBsystemVersion",   "1.0"					},
	{	"SMBserial",          STATIC_SMSERIALNUMBER			},
	{	"SMBfamily",          "MacPro"					},
	{	"SMBboardManufacter", "Apple Inc."				},
	{	"SMBboardProduct",    "Mac-F221BEC8"				}, */

/*	{	"SMBbiosVendor",      "Apple Computer, Inc."			},
	{	"SMBbiosVersion",     "MP51.88Z.007F.B03.1010071432"		},
	{	"SMBbiosDate",        "10/07/2010"				},
	{	"SMBmanufacter",      "Apple Inc."				},
	{	"SMBproductName",     "MacPro5,1"				},
	{	"SMBsystemVersion",   "1.0"					},
	{	"SMBserial",          STATIC_SMSERIALNUMBER			},
	{	"SMBfamily",          "MacPro"					},
	{	"SMBboardManufacter", "Apple Inc."				},
	{	"SMBboardProduct",    "Mac-F221BEC8"				}, */

	{	"", ""	}
};
// ------------------------------------------------------------
#endif


#if OVERRIDE_DYNAMIC_PRODUCT_DETECTION
	#if TARGET_MODEL == IMAC
		#define STATIC_DEFAULT_DATA		iMac
	#elif TARGET_MODEL == MACBOOK
		#define STATIC_DEFAULT_DATA		MacBook
	#elif TARGET_MODEL == MACBOOK_AIR
		#define STATIC_DEFAULT_DATA		MacBookAir
	#elif TARGET_MODEL == MACBOOK_PRO
		#define STATIC_DEFAULT_DATA		MacBookPro
	#elif TARGET_MODEL == MACMINI
		#define STATIC_DEFAULT_DATA		Macmini
	#elif TARGET_MODEL == MACPRO
		#define STATIC_DEFAULT_DATA		MacPro
	#endif
#endif
