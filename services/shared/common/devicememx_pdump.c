/*************************************************************************/ /*!
@File
@Title          Shared X device memory management PDump functions
@Copyright      Copyright (c) Imagination Technologies Ltd. All Rights Reserved
@Description    Implements common (client & server) PDump functions for the
                memory management code
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
*/ /**************************************************************************/

#if defined(PDUMP)

#include "devicememx_pdump.h"
#include "pdump.h"
#include "client_pdumpmm_bridge.h"
#include "devicemem_utils.h"

IMG_INTERNAL void DevmemXPDumpLoadMem(DEVMEMX_PHYSDESC *psMemDescPhys,
				      IMG_DEVMEM_OFFSET_T uiOffset,
				      IMG_DEVMEM_SIZE_T uiSize,
				      PDUMP_FLAGS_T uiPDumpFlags)
{
	PVRSRV_ERROR eError;

	PVR_ASSERT(uiSize != 0);
	PVR_ASSERT(uiOffset + uiSize <= (psMemDescPhys->uiNumPages
					 << psMemDescPhys->uiLog2PageSize));

	eError = BridgePMRPDumpLoadMem(
		GetBridgeHandle(psMemDescPhys->hConnection),
		psMemDescPhys->hPMR, uiOffset, uiSize, uiPDumpFlags, IMG_FALSE);
	PVR_LOG_IF_ERROR(eError, "BridgePMRPDumpLoadMem");
}

#endif
