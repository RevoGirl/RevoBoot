/*
 * Copyright (c) 2005 Apple Computer, Inc. All rights reserved.
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
 */

#include "platform.h"

#if 1
/*
 
 Structures for a Flattened Device Tree 
 */

#define kPropNameLength 32

typedef struct DeviceTreeNodeProperty
{
	char				name[kPropNameLength];	// NUL terminated property name
	unsigned long		length;					// Length (bytes) of following prop value
	// unsigned long       value[1];
												// Variable length value of property
												// Padded to a multiple of a longword?
} DeviceTreeNodeProperty;

typedef struct OpaqueDTEntry
{
	unsigned long				nProperties;	// Number of props[] elements (0 => end)
	unsigned long				nChildren;		// Number of children[] elements
	// DeviceTreeNodeProperty	props[];
												// array size == nProperties
												// DeviceTreeNode      children[];
												// array size == nChildren
} DeviceTreeNode;

typedef char DTPropertyNameBuf[32];

// Entry Name Definitions (Entry Names are C-Strings).
enum {
    kDTMaxEntryNameLength = 31					// Max length of a C-String Entry Name (terminator not included).
};

// Length of DTEntryNameBuf = kDTMaxEntryNameLength + 1.
typedef char DTEntryNameBuf[32];
#endif

#include "libsaio.h"
#include "device_tree.h"

#if DEBUG
	#define DPRINTF(args...) printf(args)
	void DT__PrintTree(Node *node);
#else
	#define DPRINTF(args...)
#endif

#define RoundToLong(x)	(((x) + 3) & ~3)

static struct _DTSizeInfo
{
	uint32_t	numNodes;
	uint32_t	numProperties;
	uint32_t	totalPropertySize;
} DTInfo;

#define kAllocSize 4096

static Node * freeNodes, *allocedNodes;
static Property *freeProperties, *allocedProperties;


//==============================================================================

Property * DT__AddProperty(Node *node, const char *name, uint32_t length, void *value)
{
    Property *prop;

    DPRINTF("DT__AddProperty([Node '%s'], '%s', %d, 0x%x)\n", DT__GetName(node), name, length, value);
    if (freeProperties == NULL) {
        void *buf = malloc(kAllocSize);
        int i;
        
        DPRINTF("Allocating more free properties\n");
        if (buf == 0) return 0;
        bzero(buf, kAllocSize);
        // Use the first property to record the allocated buffer
        // for later freeing.
        prop = (Property *)buf;
        prop->next = allocedProperties;
        allocedProperties = prop;
        prop->value = buf;
        prop++;
        for (i=1; i<(kAllocSize / sizeof(Property)); i++) {
            prop->next = freeProperties;
            freeProperties = prop;
            prop++;
        }
    }
    prop = freeProperties;
    freeProperties = prop->next;

    prop->name = name;
    prop->length = length;
    prop->value = value;

    // Always add to end of list
    if (node->properties == 0) {
        node->properties = prop;
    } else {
        node->last_prop->next = prop;
    }
    node->last_prop = prop;
    prop->next = 0;

    DPRINTF("Done [0x%x]\n", prop);
    
    DTInfo.numProperties++;
    DTInfo.totalPropertySize += RoundToLong(length);

    return prop;
}


//==============================================================================

Node * DT__AddChild(Node *parent, char *name)
{
	Node *node;

	if (freeNodes == NULL)
	{
		void *buf = malloc(kAllocSize);

		if (buf == 0)
		{
			return 0;
		}

		int i;

		DPRINTF("Allocating more free nodes\n");

		bzero(buf, kAllocSize);
		node = (Node *)buf;

		// Use the first node to record the allocated buffer for later freeing.
		node->next = allocedNodes;
		allocedNodes = node;
		node->children = (Node *)buf;
		node++;

		for (i = 1; i < (kAllocSize / sizeof(Node)); i++)
		{
			node->next = freeNodes;
			freeNodes = node;
			node++;
		}
	}

    DPRINTF("DT__AddChild(0x%x, '%s')\n", parent, name);
    node = freeNodes;
    freeNodes = node->next;
    DPRINTF("Got free node 0x%x\n", node);
    DPRINTF("prop = 0x%x, children = 0x%x, next = 0x%x\n", node->properties, node->children, node->next);

    if (parent == NULL)
	{
        gPlatform.DT.RootNode = node;
        node->next = 0;
    }
	else
	{
        node->next = parent->children;
        parent->children = node;
    }

    DTInfo.numNodes++;
    DT__AddProperty(node, "name", strlen(name) + 1, name);

    return node;
}


//==============================================================================

void DT__FreeProperty(Property *prop)
{
    prop->next = freeProperties;
    freeProperties = prop;
}


//==============================================================================

void DT__FreeNode(Node *node)
{
    node->next = freeNodes;
    freeNodes = node;
}


//==============================================================================

void DT__Initialize(void)
{
    DPRINTF("DT__Initialize\n");
    
    freeNodes = 0;
    allocedNodes = 0;
    freeProperties = 0;
    allocedProperties = 0;
    
    DTInfo.numNodes = 0;
    DTInfo.numProperties = 0;
    DTInfo.totalPropertySize = 0;
    
    gPlatform.DT.RootNode = DT__AddChild(NULL, "/");

	if (gPlatform.DT.RootNode == 0)
	{
		stop("Couldn't create root node"); // Mimics boot.efi
	}

    DPRINTF("DT__Initialize done\n");
}


//==============================================================================
// Free up memory used by in-memory representation of device tree.

void DT__Finalize(void)
{
	Node *node;
	Property *prop;

	DPRINTF("DT__Finalize\n");

	for (prop = allocedProperties; prop != NULL; prop = prop->next)
	{
		free(prop->value);
	}

	allocedProperties = NULL;
	freeProperties = NULL;

	for (node = allocedNodes; node != NULL; node = node->next)
	{
		free((void *)node->children);
	}

	allocedNodes = NULL;
	freeNodes = NULL;
	gPlatform.DT.RootNode = NULL;
    
	// XXX leaks any created strings
	DTInfo.numNodes = 0;
	DTInfo.numProperties = 0;
	DTInfo.totalPropertySize = 0;
}


//==============================================================================

static void * FlattenNodes(Node *node, void *buffer)
{
	Property *prop;
	DeviceTreeNode *flatNode;
	DeviceTreeNodeProperty *flatProp;
	int count;

	if (node == 0)
	{
		return buffer;
	}

	flatNode = (DeviceTreeNode *)buffer;
	buffer += sizeof(DeviceTreeNode);

	for (count = 0, prop = node->properties; prop != 0; count++, prop = prop->next)
	{
		flatProp = (DeviceTreeNodeProperty *)buffer;
		strcpy(flatProp->name, prop->name);
		flatProp->length = prop->length;
		buffer += sizeof(DeviceTreeNodeProperty);
		bcopy(prop->value, buffer, prop->length);
		buffer += RoundToLong(prop->length);
	}

	flatNode->nProperties = count;

	for (count = 0, node = node->children; node != 0; count++, node = node->next)
	{
		buffer = FlattenNodes(node, buffer);
	}

	flatNode->nChildren = count;

	return buffer;
}


/*==============================================================================
 * Flatten the in-memory representation of the device tree
 * into a binary DT block.
 * To get the buffer size needed, call with result = 0.
 * To have a buffer allocated for you, call with *result = 0.
 * To use your own buffer, call with *result = &buffer.
 */

void DT__FlattenDeviceTree(void **buffer_p, uint32_t *length)
{
	uint32_t totalSize;
	void * buf;

	DPRINTF("DT__FlattenDeviceTree(0x%x, 0x%x)\n", buffer_p, length);
#if DEBUG
	if (buffer_p) 
	{
		DT__PrintTree(gPlatform.DT.RootNode);
	}
#endif
    
	totalSize = DTInfo.numNodes * sizeof(DeviceTreeNode) + 
        DTInfo.numProperties * sizeof(DeviceTreeNodeProperty) +
        DTInfo.totalPropertySize;

	DPRINTF("Total size 0x%x\n", totalSize);

	if (buffer_p != 0)
	{
		if (totalSize == 0)
		{
			buf = 0;
		}
		else
		{
			if (*buffer_p == 0)
			{
				buf = malloc(totalSize);
			}
			else
			{
				buf = *buffer_p;
			}

			bzero(buf, totalSize);

			FlattenNodes(gPlatform.DT.RootNode, buf);
        }

		*buffer_p = buf;
	}

	if (length)
	{
		*length = totalSize;
	}
}


//==============================================================================

char * DT__GetName(Node *node)
{
    Property *prop;

    //DPRINTF("DT__GetName(0x%x)\n", node);
    //DPRINTF("Node properties = 0x%x\n", node->properties);
    for (prop = node->properties; prop; prop = prop->next) {
        //DPRINTF("Prop '%s'\n", prop->name);
        if (strcmp(prop->name, "name") == 0) {
            return prop->value;
        }
    }
    //DPRINTF("DT__GetName returns 0\n");
    return "(null)";
}


//==============================================================================

Node * DT__FindNode(char *path, bool createIfMissing)
{
    Node *node, *child;
    DTPropertyNameBuf nameBuf;
    char *bp;
    int i;

    DPRINTF("DT__FindNode('%s', %d)\n", path, createIfMissing);
    
    // Start at root
    node = gPlatform.DT.RootNode;
    DPRINTF("root = 0x%x\n", node);

    while (node) {
        // Skip leading slash
        while (*path == '/')
		{
			path++;
		}

        for (i = 0, bp = nameBuf; ++i < kDTMaxEntryNameLength && *path && *path != '/'; bp++, path++)
		{
			*bp = *path;
		}

        *bp = '\0';

        if (nameBuf[0] == '\0')
		{
            // last path entry
            break;
        }
        DPRINTF("Node '%s'\n", nameBuf);

        for (child = node->children; child != 0; child = child->next)
		{
            DPRINTF("Child 0x%x\n", child);

            if (strcmp(DT__GetName(child), nameBuf) == 0)
			{
                break;
            }
        }

        if (child == 0 && createIfMissing)
		{
            DPRINTF("Creating node\n");
            char *str = malloc(strlen(nameBuf) + 1);
            // XXX this will leak
            strcpy(str, nameBuf);

            child = DT__AddChild(node, str);
        }

        node = child;
    }

    return node;
}

#if DEBUG


//==============================================================================

void DT__PrintNode(Node *node, int level)
{
    char spaces[10], *cp = spaces;
    Property *prop;

    if (level > 9) level = 9;
    while (level--) *cp++ = ' ';
    *cp = '\0';

    printf("%s===Node===\n", spaces);
    for (prop = node->properties; prop; prop = prop->next) {
        char c = *((char *)prop->value);
        if (prop->length < 64 && (
            strcmp(prop->name, "name") == 0 || 
            (c >= '0' && c <= '9') ||
            (c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') || c == '_')) {
            printf("%s Property '%s' [%d] = '%s'\n", spaces, prop->name, prop->length, prop->value);
        } else {
            printf("%s Property '%s' [%d] = (data)\n", spaces, prop->name, prop->length);
        }
    }
    printf("%s==========\n", spaces);
}


//==============================================================================

static void _PrintTree(Node *node, int level)
{
    DT__PrintNode(node, level);
    level++;
    for (node = node->children; node; node = node->next)
        _PrintTree(node, level);
}


//==============================================================================

void DT__PrintTree(Node *node)
{
	if (node == 0)
	{
		node = gPlatform.DT.RootNode;
	}

	_PrintTree(node, 0);
}


//==============================================================================

void DT__PrintFlattenedNode(DTEntry entry, int level)
{
    char spaces[10], *cp = spaces;
    DTPropertyIterator	propIter;
    char *name;
    void *prop;
    int propSize;

    if (level > 9) level = 9;
    while (level--) *cp++ = ' ';
    *cp = '\0';

    printf("%s===Entry %p===\n", spaces, entry);
    if (kSuccess != DTCreatePropertyIterator(entry, &propIter)) {
        printf("Couldn't create property iterator\n");
        return;
    }
    while( kSuccess == DTIterateProperties( propIter, &name)) {
        if(  kSuccess != DTGetProperty( entry, name, &prop, &propSize ))
            continue;
        printf("%s Property %s = %s\n", spaces, name, prop);
    }
    DTDisposePropertyIterator(propIter);

    printf("%s==========\n", spaces);
}


//==============================================================================

static void _PrintFlattenedTree(DTEntry entry, int level)
{
    DTEntryIterator entryIter;

    PrintFlattenedNode(entry, level);

    if (kSuccess != DTCreateEntryIterator(entry, &entryIter)) {
            printf("Couldn't create entry iterator\n");
            return;
    }
    level++;
    while (kSuccess == DTIterateEntries( entryIter, &entry )) {
        _PrintFlattenedTree(entry, level);
    }
    DTDisposeEntryIterator(entryIter);
}


//==============================================================================

void DT__PrintFlattenedTree(DTEntry entry)
{
    _PrintFlattenedTree(entry, 0);
}


//==============================================================================

int main(int argc, char **argv)
{
    DTEntry                             dtEntry;
    DTPropertyIterator	                propIter;
    DTEntryIterator                     entryIter;
    void				*prop;
    int					propSize;
    char				*name;
    void *flatTree;
    uint32_t flatSize;

    Node *node;

    node = AddChild(NULL, "device-tree");
    AddProperty(node, "potato", 4, "foo");
    AddProperty(node, "chemistry", 4, "bar");
    AddProperty(node, "physics", 4, "baz");

    node = AddChild(node, "dev");
    AddProperty(node, "one", 4, "one");
    AddProperty(node, "two", 4, "two");
    AddProperty(node, "three", 6, "three");

    node = AddChild(gPlatform.DT.RootNode, "foo");
    AddProperty(node, "aaa", 4, "aab");
    AddProperty(node, "bbb", 4, "bbc");
    AddProperty(node, "cccc", 6, "ccccd");

    node = FindNode("/this/is/a/test", 1);
    AddProperty(node, "dddd", 12, "abcdefghijk");

    printf("In-memory tree:\n\n");

    PrintTree(gPlatform.DT.RootNode);

    FlattenDeviceTree(&flatTree, &flatSize);

    printf("Flat tree = %p, size %d\n", flatTree, flatSize);

    dtEntry = (DTEntry)flatTree;

    printf("\n\nPrinting flat tree\n\n");

    DTInit(dtEntry);

    PrintFlattenedTree((DTEntry)flatTree);
#if 0
        printf("=== Entry %p ===\n", dtEntry);
        if (kSuccess != DTCreatePropertyIterator(dtEntry, &propIter)) {
            printf("Couldn't create property iterator\n");
            return 1;
        }
        while( kSuccess == DTIterateProperties( propIter, &name)) {
            if(  kSuccess != DTGetProperty( dtEntry, name, &prop, &propSize ))
                continue;
            printf(" Property %s = %s\n", name, prop);
        }
        DTDisposePropertyIterator(propIter);
        printf("========\n");

    if (kSuccess != DTCreateEntryIterator(dtEntry, &entryIter)) {
            printf("Couldn't create entry iterator\n");
            return 1;
    }
    while (kSuccess == DTIterateEntries( entryIter, &dtEntry )) {
        printf("=== Entry %p ===\n", dtEntry);

        if (kSuccess != DTCreatePropertyIterator(dtEntry, &propIter)) {
            printf("Couldn't create property iterator\n");
            return 1;
        }
        while( kSuccess == DTIterateProperties( propIter, &name)) {
            if(  kSuccess != DTGetProperty( dtEntry, name, &prop, &propSize ))
                continue;
            printf(" Property %s = %s\n", name, prop);
        }
        DTDisposePropertyIterator(propIter);
        printf("========\n");
    }
    DTDisposeEntryIterator(entryIter);
#endif

    return 0;
}

#endif

