/*
 * Copyright (c) 1999-2003 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * Portions Copyright (c) 1999-2003 Apple Computer, Inc.  All Rights
 * Reserved.  This file contains Original Code and/or Modifications of
 * Original Code as defined in and that are subject to the Apple Public
 * Source License Version 2.0 (the "License").  You may not use this file
 * except in compliance with the License.  Please obtain a copy of the
 * License at http://www.apple.com/publicsource and read it before using
 * this file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE OR NON- INFRINGEMENT.  Please see the
 * License for the specific language governing rights and limitations
 * under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 *
 * Copyright 1993 NeXT, Inc. All rights reserved.
 *
 * Refactorizing done by DHP in 2011.
 */

#include "boot.h"
#include "bootlogo.h"
#include "appleClut8.h"
#include "bootstruct.h"


//==============================================================================

unsigned long lookUpCLUTIndex(unsigned char index, unsigned char depth)
{
	long colorIndex = (index * 3);
	long red   = appleClut8[ colorIndex   ];
	long green = appleClut8[ colorIndex++ ];
	long blue  = appleClut8[ colorIndex++ ];

	return (red << 16) | (green << 8) | blue;
}


//==============================================================================

int convertImage(unsigned short width, unsigned short height, const unsigned char *imageData, unsigned char **newImageData)
{
	int index = 0;
	int size = (width * height); // 16384
	int depth = VIDEO(depth);

	unsigned char *img = 0;
	unsigned long *img32;

	switch (depth)
	{
		case 32:
			img32 = malloc(size * 4);

			if (!img32)
			{		
				break;
			}

			for (; index < size; index++)
			{
				img32[index] = lookUpCLUTIndex(imageData[index], depth);
			}

			img = (unsigned char *)img32;
			break;
	}

	*newImageData = img;

	return 0;
}


//==============================================================================

void * stosl(void * dst, long val, long len)
{
	asm volatile ("rep; stosl"
				  : "=c" (len), "=D" (dst)
				  : "0" (len), "1" (dst), "a" (val)
				  : "memory" );

	return dst;
}


//==============================================================================

void drawColorRectangle(unsigned short x, unsigned short y, unsigned short width, unsigned short height, unsigned char colorIndex)
{
	long color = lookUpCLUTIndex(colorIndex, VIDEO(depth));
	long pixelBytes = VIDEO(depth) / 8;

	char * vram  = (char *) VIDEO(baseAddr) + VIDEO(rowBytes) * y + pixelBytes * x;

	width = MIN(width, VIDEO(width) - x);
	height = MIN(height, VIDEO(height) - y);

	int rem = (pixelBytes * width) % 4;	
	int length = pixelBytes * width / 4;

	while (height--)
	{
		bcopy(&color, vram, rem);
		stosl(vram + rem, color, length);
		vram += VIDEO(rowBytes);
	}
}


//==============================================================================

void drawDataRectangle(unsigned short  x, unsigned short  y, unsigned short width, unsigned short  height, unsigned char * data)
{
	unsigned short drawWidth;

	long pixelBytes = VIDEO(depth) / 8;

	unsigned char * vram = (unsigned char *) VIDEO(baseAddr) + VIDEO(rowBytes) * y + pixelBytes * x;
	
	drawWidth = MIN(width, VIDEO(width) - x);
	height = MIN(height, VIDEO(height) - y);

	while (height--)
	{
		bcopy( data, vram, drawWidth * pixelBytes );
		vram += VIDEO(rowBytes);
		data += width * pixelBytes;
	}
}


//==============================================================================

char * decodeRLE(const void * rleData, int rleBlocks, int outBytes)
{
	char *out, *cp;

	struct RLEBlock
	{
		unsigned char count;
		unsigned char value;
	} * bp = (struct RLEBlock *) rleData;

	out = cp = (char *) malloc(outBytes);

	if (out == NULL)
	{
		return NULL;
	}
	
	while (rleBlocks--)
	{
		memset( cp, bp->value, bp->count );
		cp += bp->count;
		bp++;
	}

	return out;
}


//==============================================================================

void showBootLogo()
{
	uint8_t *bootImageData = NULL;
	uint8_t *appleBootLogo = (uint8_t *) decodeRLE(appleLogoRLE, 686, 16384); 

	setVideoMode(GRAPHICS_MODE);
	// Fill the background to 75% grey (same as BootX). 
	drawColorRectangle(0, 0, VIDEO(width), VIDEO(height), 0x01);

	convertImage(APPLE_LOGO_WIDTH, APPLE_LOGO_HEIGHT, appleBootLogo, &bootImageData);

	drawDataRectangle(APPLE_LOGO_X, APPLE_LOGO_Y, APPLE_LOGO_WIDTH, APPLE_LOGO_HEIGHT, bootImageData);

	free(bootImageData);
	free(appleBootLogo); 
}
