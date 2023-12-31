/*******************************************************************************
@File
@Title          Client bridge header for pdump
@Copyright      Copyright (c) Imagination Technologies Ltd. All Rights Reserved
@Description    Exports the client bridge functions for pdump
@License        Dual MIT/GPLv2

The contents of this file are subject to the MIT license as set out below.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

Alternatively, the contents of this file may be used under the terms of
the GNU General Public License Version 2 ("GPL") in which case the provisions
of GPL are applicable instead of those above.

If you wish to allow use of your version of this file only under the terms of
GPL, and not to allow others to use your version of this file under the terms
of the MIT license, indicate your decision by deleting the provisions above
and replace them with the notice and other provisions required by GPL as set
out in the file called "GPL-COPYING" included in this distribution. If you do
not delete the provisions above, a recipient may use your version of this file
under the terms of either the MIT license or GPL.

This License is also included in this distribution in the file called
"MIT-COPYING".

EXCEPT AS OTHERWISE STATED IN A NEGOTIATED AGREEMENT: (A) THE SOFTWARE IS
PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT; AND (B) IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*******************************************************************************/

#ifndef CLIENT_PDUMP_BRIDGE_H
#define CLIENT_PDUMP_BRIDGE_H

#include "img_defs.h"
#include "pvrsrv_error.h"

#if defined(PVR_INDIRECT_BRIDGE_CLIENTS)
#include "pvr_bridge_client.h"
#include "pvr_bridge.h"
#endif

#include "common_pdump_bridge.h"

IMG_INTERNAL PVRSRV_ERROR BridgePDumpImageDescriptor(
	IMG_HANDLE hBridge, IMG_HANDLE hDevmemCtx, IMG_UINT32 ui32StringSize,
	const IMG_CHAR *puiFileName, IMG_DEV_VIRTADDR sDataDevAddr,
	IMG_UINT32 ui32DataSize, IMG_UINT32 ui32LogicalWidth,
	IMG_UINT32 ui32LogicalHeight, IMG_UINT32 ui32PhysicalWidth,
	IMG_UINT32 ui32PhysicalHeight, PDUMP_PIXEL_FORMAT ePixelFormat,
	IMG_MEMLAYOUT eMemLayout, IMG_FB_COMPRESSION eFBCompression,
	const IMG_UINT32 *pui32FBCClearColour, PDUMP_FBC_SWIZZLE eeFBCSwizzle,
	IMG_DEV_VIRTADDR sHeaderDevAddr, IMG_UINT32 ui32HeaderSize,
	IMG_UINT32 ui32PDumpFlags);

IMG_INTERNAL PVRSRV_ERROR BridgePVRSRVPDumpComment(IMG_HANDLE hBridge,
						   IMG_UINT32 ui32CommentSize,
						   IMG_CHAR *puiComment,
						   IMG_UINT32 ui32Flags);

IMG_INTERNAL PVRSRV_ERROR BridgePVRSRVPDumpSetFrame(IMG_HANDLE hBridge,
						    IMG_UINT32 ui32Frame);

IMG_INTERNAL PVRSRV_ERROR BridgePDumpDataDescriptor(
	IMG_HANDLE hBridge, IMG_HANDLE hDevmemCtx, IMG_UINT32 ui32StringSize,
	const IMG_CHAR *puiFileName, IMG_DEV_VIRTADDR sDataDevAddr,
	IMG_UINT32 ui32DataSize, IMG_UINT32 ui32HeaderType,
	IMG_UINT32 ui32ElementType, IMG_UINT32 ui32ElementCount,
	IMG_UINT32 ui32PDumpFlags);

#endif /* CLIENT_PDUMP_BRIDGE_H */
