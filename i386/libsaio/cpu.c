/*
 * Copyright (c) 2009 by Master Chief.
 *
 * Refactoring for Revolution done by DHP in 2010/2011.
 */


#include "platform.h"


#if USE_STATIC_CPU_DATA

	#if CPU_VENDOR_ID == CPU_VENDOR_INTEL
		#include "cpu/intel/static_data.h"
	#else
		#include "cpu/amd/static_data.h"
	#endif

#else

	#if CPU_VENDOR_ID == CPU_VENDOR_INTEL
		#include "cpu/intel/dynamic_data.h"
	#else
		#include "cpu/amd/dynamic_data.h"
	#endif

#endif
