/*******************************************************************************
@File
@Title          Client bridge header for cache
@Copyright      Copyright (c) Imagination Technologies Ltd. All Rights Reserved
@Description    Exports the client bridge functions for cache
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

#ifndef CLIENT_CACHE_BRIDGE_H
#define CLIENT_CACHE_BRIDGE_H

#include "img_defs.h"
#include "pvrsrv_error.h"

#if defined(PVR_INDIRECT_BRIDGE_CLIENTS)
#include "pvr_bridge_client.h"
#include "pvr_bridge.h"
#endif

#include "common_cache_bridge.h"

IMG_INTERNAL PVRSRV_ERROR
BridgeCacheOpQueue(IMG_HANDLE hBridge, IMG_UINT32 ui32NumCacheOps,
		   IMG_HANDLE *phPMR, IMG_UINT64 *pui64Address,
		   IMG_DEVMEM_OFFSET_T *puiOffset, IMG_DEVMEM_SIZE_T *puiSize,
		   PVRSRV_CACHE_OP *piuCacheOp, IMG_UINT32 ui32OpTimeline);

IMG_INTERNAL PVRSRV_ERROR BridgeCacheOpExec(IMG_HANDLE hBridge, IMG_HANDLE hPMR,
					    IMG_UINT64 ui64Address,
					    IMG_DEVMEM_OFFSET_T uiOffset,
					    IMG_DEVMEM_SIZE_T uiSize,
					    PVRSRV_CACHE_OP iuCacheOp);

IMG_INTERNAL PVRSRV_ERROR BridgeCacheOpLog(IMG_HANDLE hBridge, IMG_HANDLE hPMR,
					   IMG_UINT64 ui64Address,
					   IMG_DEVMEM_OFFSET_T uiOffset,
					   IMG_DEVMEM_SIZE_T uiSize,
					   IMG_INT64 i64StartTime,
					   IMG_INT64 i64EndTime,
					   PVRSRV_CACHE_OP iuCacheOp);

#endif /* CLIENT_CACHE_BRIDGE_H */
