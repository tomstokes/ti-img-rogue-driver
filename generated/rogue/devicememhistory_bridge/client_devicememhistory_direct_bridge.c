/*******************************************************************************
@File
@Title          Direct client bridge for devicememhistory
@Copyright      Copyright (c) Imagination Technologies Ltd. All Rights Reserved
@Description    Implements the client side of the bridge for devicememhistory
                which is used in calls from Server context.
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

#include "client_devicememhistory_bridge.h"
#include "img_defs.h"
#include "pvr_debug.h"

/* Module specific includes */
#include "img_types.h"
#include "img_defs.h"
#include "devicemem_typedefs.h"

#include "pmr.h"
#include "devicemem_history_server.h"

IMG_INTERNAL PVRSRV_ERROR BridgeDevicememHistoryMap(
	IMG_HANDLE hBridge, IMG_HANDLE hPMR, IMG_DEVMEM_SIZE_T uiOffset,
	IMG_DEV_VIRTADDR sDevVAddr, IMG_DEVMEM_SIZE_T uiSize,
	const IMG_CHAR *puiText, IMG_UINT32 ui32Log2PageSize,
	IMG_UINT32 ui32AllocationIndex, IMG_UINT32 *pui32AllocationIndexOut)
{
	PVRSRV_ERROR eError;
	PMR *psPMRInt;
	PVR_UNREFERENCED_PARAMETER(hBridge);

	psPMRInt = (PMR *)hPMR;

	eError = DevicememHistoryMapKM(psPMRInt, uiOffset, sDevVAddr, uiSize,
				       puiText, ui32Log2PageSize,
				       ui32AllocationIndex,
				       pui32AllocationIndexOut);

	return eError;
}

IMG_INTERNAL PVRSRV_ERROR BridgeDevicememHistoryUnmap(
	IMG_HANDLE hBridge, IMG_HANDLE hPMR, IMG_DEVMEM_SIZE_T uiOffset,
	IMG_DEV_VIRTADDR sDevVAddr, IMG_DEVMEM_SIZE_T uiSize,
	const IMG_CHAR *puiText, IMG_UINT32 ui32Log2PageSize,
	IMG_UINT32 ui32AllocationIndex, IMG_UINT32 *pui32AllocationIndexOut)
{
	PVRSRV_ERROR eError;
	PMR *psPMRInt;
	PVR_UNREFERENCED_PARAMETER(hBridge);

	psPMRInt = (PMR *)hPMR;

	eError = DevicememHistoryUnmapKM(psPMRInt, uiOffset, sDevVAddr, uiSize,
					 puiText, ui32Log2PageSize,
					 ui32AllocationIndex,
					 pui32AllocationIndexOut);

	return eError;
}

IMG_INTERNAL PVRSRV_ERROR BridgeDevicememHistoryMapVRange(
	IMG_HANDLE hBridge, IMG_DEV_VIRTADDR sBaseDevVAddr,
	IMG_UINT32 ui32ui32StartPage, IMG_UINT32 ui32NumPages,
	IMG_DEVMEM_SIZE_T uiAllocSize, const IMG_CHAR *puiText,
	IMG_UINT32 ui32Log2PageSize, IMG_UINT32 ui32AllocationIndex,
	IMG_UINT32 *pui32AllocationIndexOut)
{
	PVRSRV_ERROR eError;

	eError = DevicememHistoryMapVRangeKM(
		NULL, (PVRSRV_DEVICE_NODE *)((void *)hBridge), sBaseDevVAddr,
		ui32ui32StartPage, ui32NumPages, uiAllocSize, puiText,
		ui32Log2PageSize, ui32AllocationIndex, pui32AllocationIndexOut);

	return eError;
}

IMG_INTERNAL PVRSRV_ERROR BridgeDevicememHistoryUnmapVRange(
	IMG_HANDLE hBridge, IMG_DEV_VIRTADDR sBaseDevVAddr,
	IMG_UINT32 ui32ui32StartPage, IMG_UINT32 ui32NumPages,
	IMG_DEVMEM_SIZE_T uiAllocSize, const IMG_CHAR *puiText,
	IMG_UINT32 ui32Log2PageSize, IMG_UINT32 ui32AllocationIndex,
	IMG_UINT32 *pui32AllocationIndexOut)
{
	PVRSRV_ERROR eError;

	eError = DevicememHistoryUnmapVRangeKM(
		NULL, (PVRSRV_DEVICE_NODE *)((void *)hBridge), sBaseDevVAddr,
		ui32ui32StartPage, ui32NumPages, uiAllocSize, puiText,
		ui32Log2PageSize, ui32AllocationIndex, pui32AllocationIndexOut);

	return eError;
}

IMG_INTERNAL PVRSRV_ERROR BridgeDevicememHistorySparseChange(
	IMG_HANDLE hBridge, IMG_HANDLE hPMR, IMG_DEVMEM_SIZE_T uiOffset,
	IMG_DEV_VIRTADDR sDevVAddr, IMG_DEVMEM_SIZE_T uiSize,
	const IMG_CHAR *puiText, IMG_UINT32 ui32Log2PageSize,
	IMG_UINT32 ui32AllocPageCount, IMG_UINT32 *pui32AllocPageIndices,
	IMG_UINT32 ui32FreePageCount, IMG_UINT32 *pui32FreePageIndices,
	IMG_UINT32 ui32AllocationIndex, IMG_UINT32 *pui32AllocationIndexOut)
{
	PVRSRV_ERROR eError;
	PMR *psPMRInt;
	PVR_UNREFERENCED_PARAMETER(hBridge);

	psPMRInt = (PMR *)hPMR;

	eError = DevicememHistorySparseChangeKM(
		psPMRInt, uiOffset, sDevVAddr, uiSize, puiText,
		ui32Log2PageSize, ui32AllocPageCount, pui32AllocPageIndices,
		ui32FreePageCount, pui32FreePageIndices, ui32AllocationIndex,
		pui32AllocationIndexOut);

	return eError;
}
