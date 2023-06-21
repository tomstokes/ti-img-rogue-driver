/*************************************************************************/ /*!
@File
@Title          Rgx debug information
@Copyright      Copyright (c) Imagination Technologies Ltd. All Rights Reserved
@Description    RGX debugging functions
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
//#define PVR_DPF_FUNCTION_TRACE_ON 1
#undef PVR_DPF_FUNCTION_TRACE_ON

#include "img_defs.h"
#include "rgxdefs_km.h"
#include "rgxdevice.h"
#include "rgxmem.h"
#include "allocmem.h"
#include "cache_km.h"
#include "osfunc.h"
#include "oskm_apphint.h"

#include "rgxdebug_common.h"
#include "pvrversion.h"
#include "pvr_debug.h"
#include "srvkm.h"
#include "rgxutils.h"
#include "tlstream.h"
#include "rgxfwutils.h"
#include "pvrsrv.h"
#include "services_km.h"

#include "devicemem.h"
#include "devicemem_pdump.h"
#include "devicemem_utils.h"
#include "rgx_fwif_km.h"
#include "rgx_fwif_sf.h"
#include "debug_common.h"

#include "rgxta3d.h"
#if defined(SUPPORT_RGXKICKSYNC_BRIDGE)
#include "rgxkicksync.h"
#endif
#include "rgxcompute.h"
#include "rgxtdmtransfer.h"
#include "rgxtimecorr.h"
#include "rgx_options.h"
#include "rgxinit.h"
#include "rgxlayer_impl.h"
#include "devicemem_history_server.h"

#define DD_SUMMARY_INDENT ""

#define RGX_DEBUG_STR_SIZE (150U)

#define RGX_TEXAS_BIF0_ID (0)
#define RGX_TEXAS_BIF1_ID (1)

/*
 *  The first 7 or 8 cat bases are memory contexts used for PM
 *  or firmware. The rest are application contexts. The numbering
 *  is zero-based.
 */
#if defined(SUPPORT_TRUSTED_DEVICE)
#define MAX_RESERVED_FW_MMU_CONTEXT (7)
#else
#define MAX_RESERVED_FW_MMU_CONTEXT (6)
#endif

static const IMG_CHAR *const pszPowStateName[] = {
#define X(NAME) #NAME,
	RGXFWIF_POW_STATES
#undef X
};

static const IMG_FLAGS2DESC asHwrState2Description[] = {
	{ RGXFWIF_HWR_HARDWARE_OK, " HWR OK;" },
	{ RGXFWIF_HWR_RESET_IN_PROGRESS, " Reset ongoing;" },
	{ RGXFWIF_HWR_GENERAL_LOCKUP, " General lockup;" },
	{ RGXFWIF_HWR_DM_RUNNING_OK, " DM running ok;" },
	{ RGXFWIF_HWR_DM_STALLING, " DM stalling;" },
	{ RGXFWIF_HWR_FW_FAULT, " FW fault;" },
	{ RGXFWIF_HWR_RESTART_REQUESTED, " Restart requested;" },
};

static const IMG_FLAGS2DESC asDmState2Description[] = {
	{ RGXFWIF_DM_STATE_READY_FOR_HWR, " ready for hwr;" },
	{ RGXFWIF_DM_STATE_NEEDS_SKIP, " needs skip;" },
	{ RGXFWIF_DM_STATE_NEEDS_PR_CLEANUP, " needs PR cleanup;" },
	{ RGXFWIF_DM_STATE_NEEDS_TRACE_CLEAR, " needs trace clear;" },
	{ RGXFWIF_DM_STATE_GUILTY_LOCKUP, " guilty lockup;" },
	{ RGXFWIF_DM_STATE_INNOCENT_LOCKUP, " innocent lockup;" },
	{ RGXFWIF_DM_STATE_GUILTY_OVERRUNING, " guilty overrunning;" },
	{ RGXFWIF_DM_STATE_INNOCENT_OVERRUNING, " innocent overrunning;" },
	{ RGXFWIF_DM_STATE_GPU_ECC_HWR, " GPU ECC hwr;" },
};

static const IMG_CHAR
	*const apszFwOsStateName[RGXFW_CONNECTION_FW_STATE_COUNT] = {
		"offline", "ready", "active", "offloading"
	};

#if defined(PVR_ENABLE_PHR)
static const IMG_FLAGS2DESC asPHRConfig2Description[] = {
	{ BIT_ULL(RGXFWIF_PHR_MODE_OFF), "off" },
	{ BIT_ULL(RGXFWIF_PHR_MODE_RD_RESET), "reset RD hardware" },
	{ BIT_ULL(RGXFWIF_PHR_MODE_FULL_RESET), "full gpu reset " },
};
#endif

/*!
*******************************************************************************

 @Function	_RGXDecodeMMULevel

 @Description

 Return the name for the MMU level that faulted.

 @Input ui32MMULevel	 - MMU level

 @Return   IMG_CHAR* to the sting describing the MMU level that faulted.

******************************************************************************/
static const IMG_CHAR *_RGXDecodeMMULevel(IMG_UINT32 ui32MMULevel)
{
	const IMG_CHAR *pszMMULevel = "";

	switch (ui32MMULevel) {
	case 0x0:
		pszMMULevel = " (Page Table)";
		break;
	case 0x1:
		pszMMULevel = " (Page Directory)";
		break;
	case 0x2:
		pszMMULevel = " (Page Catalog)";
		break;
	case 0x3:
		pszMMULevel = " (Cat Base Reg)";
		break;
	}

	return pszMMULevel;
}

/*!
*******************************************************************************

 @Function	_RGXDecodeMMUReqTags

 @Description

 Decodes the MMU Tag ID and Sideband data fields from RGX_CR_MMU_FAULT_META_STATUS and
 RGX_CR_MMU_FAULT_STATUS regs.

 @Input ui32TagID           - Tag ID value
 @Input ui32BIFModule       - BIF module
 @Input bRead               - Read flag
 @Input bWriteBack          - Write Back flag
 @Output ppszTagID          - Decoded string from the Tag ID
 @Output ppszTagSB          - Decoded string from the Tag SB
 @Output pszScratchBuf      - Buffer provided to the function to generate the debug strings
 @Input ui32ScratchBufSize  - Size of the provided buffer

 @Return   void

******************************************************************************/
#define RGX_TEXAS_BIF0_MCU_L1_TAG_LAST__SERIES8 (12)
#define RGX_TEXAS_BIF0_TAG_IPFID_ARRAY_FIRST__SERIES8 (15)
#define RGX_TEXAS_BIF0_MCU_L1_TAG_LAST__ALBIORIX (6)
#define RGX_TEXAS_BIF0_TAG_IPFID_ARRAY_FIRST__ALBIORIX (9)
#define RGX_TEXAS_BIF0_TAG_IPFID_ARRAY_LAST (33)
#define RGX_TEXAS_BIF0_TAG_RTU_RAC_FIRST (41)
#define RGX_TEXAS_BIF0_TAG_RTU_RAC_LAST (48)
#define RGX_TEXAS_BIF0_TAG_LAST (51)

#define RGX_TEXAS_BIF1_TAG_LAST (26)

#define RGX_JONES_BIF_IPP_TAG (0)
#define RGX_JONES_BIF_DCE_TAG_FIRST (1)
#define RGX_JONES_BIF_DCE_TAG_LAST (14)
#define RGX_JONES_BIF_TDM_TAG_FIRST (15)
#define RGX_JONES_BIF_TDM_TAG_LAST (19)
#define RGX_JONES_BIF_PM_TAG (20)
#define RGX_JONES_BIF_CDM_TAG_FIRST (21)
#define RGX_JONES_BIF_CDM_TAG_LAST (31)
#define RGX_JONES_BIF_META_TAG (32)
#define RGX_JONES_BIF_META_DMA_TAG (33)
#define RGX_JONES_BIF_TE_TAG_FIRST (34)
#define RGX_JONES_BIF_TE_TAG_LAST (47)
#define RGX_JONES_BIF_RTU_TAG_FIRST (48)
#define RGX_JONES_BIF_RTU_TAG_LAST (53)
#define RGX_JONES_BIF_RPM_TAG (54)
#define RGX_JONES_BIF_TAG_LAST (54)

/* The MCU L1 requestors are common to all Texas BIFs so put them
 * in their own function. */
static INLINE void _RGXDecodeMMUReqMCULevel1(PVRSRV_RGXDEV_INFO *psDevInfo,
					     IMG_UINT32 ui32TagID,
					     IMG_CHAR **ppszTagSB)
{
	if (RGX_IS_FEATURE_SUPPORTED(psDevInfo, ALBIORIX_TOP_INFRASTRUCTURE)) {
		switch (ui32TagID) {
		case 0:
			*ppszTagSB = "IP0 PDS";
			break;
		case 1:
			*ppszTagSB = "IP0 Global";
			break;
		case 2:
			*ppszTagSB = "IP1 PDS";
			break;
		case 3:
			*ppszTagSB = "IP1 Global";
			break;
		case 4:
			*ppszTagSB = "IP2 PDS";
			break;
		case 5:
			*ppszTagSB = "IP2 Global";
			break;
		}
	} else {
		switch (ui32TagID) {
		case 0:
			*ppszTagSB = "IP0 PDS";
			break;
		case 1:
			*ppszTagSB = "IP0 Global";
			break;
		case 2:
			*ppszTagSB = "IP0 BSC";
			break;
		case 3:
			*ppszTagSB = "IP0 Constants";
			break;

		case 4:
			*ppszTagSB = "IP1 PDS";
			break;
		case 5:
			*ppszTagSB = "IP1 Global";
			break;
		case 6:
			*ppszTagSB = "IP1 BSC";
			break;
		case 7:
			*ppszTagSB = "IP1 Constants";
			break;

		case 8:
			*ppszTagSB = "IP2 PDS";
			break;
		case 9:
			*ppszTagSB = "IP2 Global";
			break;
		case 10:
			*ppszTagSB = "IP2 BSC";
			break;
		case 11:
			*ppszTagSB = "IP2 Constants";
			break;
		}
	}
}

static void _RGXDecodeMMUReqTags(PVRSRV_RGXDEV_INFO *psDevInfo,
				 IMG_UINT32 ui32TagID, IMG_UINT32 ui32BIFModule,
				 IMG_BOOL bRead, IMG_BOOL bWriteBack,
				 IMG_BOOL bFBMFault, IMG_CHAR **ppszTagID,
				 IMG_CHAR **ppszTagSB, IMG_CHAR *pszScratchBuf,
				 IMG_UINT32 ui32ScratchBufSize)
{
	IMG_UINT32 ui32BIFsPerSPU = 2;
	IMG_CHAR *pszTagID = "-";
	IMG_CHAR *pszTagSB = "-";

	PVR_ASSERT(ppszTagID != NULL);
	PVR_ASSERT(ppszTagSB != NULL);

	if (RGX_IS_FEATURE_SUPPORTED(psDevInfo, ALBIORIX_TOP_INFRASTRUCTURE)) {
		ui32BIFsPerSPU = 4;
	}

	if (bFBMFault) {
		pszTagID = "FBM";
		if (bWriteBack) {
			pszTagSB = "Header/state cache request";
		}
	} else if (RGX_IS_FEATURE_VALUE_SUPPORTED(psDevInfo, NUM_SPU) &&
		   ui32BIFModule < RGX_GET_FEATURE_VALUE(psDevInfo, NUM_SPU) *
					   ui32BIFsPerSPU) {
		if ((ui32BIFModule % ui32BIFsPerSPU) == 0) {
			IMG_UINT32 ui32Tag_RGX_TEXAS_BIF0_MCU_L1_TAG_LAST =
				(RGX_IS_FEATURE_SUPPORTED(
					psDevInfo,
					ALBIORIX_TOP_INFRASTRUCTURE)) ?
					RGX_TEXAS_BIF0_MCU_L1_TAG_LAST__ALBIORIX :
					RGX_TEXAS_BIF0_MCU_L1_TAG_LAST__SERIES8;
			IMG_UINT32 ui32Tag_RGX_TEXAS_BIF0_TAG_IPFID_ARRAY_FIRST =
				(RGX_IS_FEATURE_SUPPORTED(
					psDevInfo,
					ALBIORIX_TOP_INFRASTRUCTURE)) ?
					RGX_TEXAS_BIF0_TAG_IPFID_ARRAY_FIRST__ALBIORIX :
					RGX_TEXAS_BIF0_TAG_IPFID_ARRAY_FIRST__SERIES8;

			/* Texas 0 BIF */
			if (ui32TagID <
			    ui32Tag_RGX_TEXAS_BIF0_MCU_L1_TAG_LAST) {
				pszTagID = "MCU L1";
				_RGXDecodeMMUReqMCULevel1(psDevInfo, ui32TagID,
							  &pszTagSB);
			} else if (ui32TagID <
				   ui32Tag_RGX_TEXAS_BIF0_TAG_IPFID_ARRAY_FIRST) {
				if (RGX_IS_FEATURE_SUPPORTED(
					    psDevInfo,
					    ALBIORIX_TOP_INFRASTRUCTURE)) {
					switch (ui32TagID) {
					case 6:
						pszTagID = "TCU L1";
						break;
					case 7:
					case 8:
						pszTagID = "PBE0";
						break;
					}
				} else {
					switch (ui32TagID) {
					case 12:
						pszTagID = "TCU L1";
						break;
					case 13:
					case 14:
						pszTagID = "PBE0";
						break;
					}
				}
			} else if (ui32TagID <=
				   RGX_TEXAS_BIF0_TAG_IPFID_ARRAY_LAST) {
				pszTagID = "IPF ID Array";
			} else if (ui32TagID <
				   RGX_TEXAS_BIF0_TAG_RTU_RAC_FIRST) {
				switch (ui32TagID) {
				case 34:
					pszTagID = "IPF_CPF";
					break;
				case 35:
					pszTagID = "PPP";
					break;
				case 36:
				case 37:
					pszTagID = "ISP0 ID Array";
					break;
				case 38:
				case 39:
					pszTagID = "ISP2 ID Array";
					break;
				case 40:
					pszTagID = "VCE RTC";
					break;
				}
			} else if (ui32TagID <=
				   RGX_TEXAS_BIF0_TAG_RTU_RAC_LAST) {
				pszTagID = "RTU RAC";
			} else if (ui32TagID <= RGX_TEXAS_BIF0_TAG_LAST) {
				switch (ui32TagID) {
				case 49:
					pszTagID = "VCE AMC";
					break;
				case 50:
				case 51:
					pszTagID = "SHF";
					break;
				}
			} else {
				PVR_DPF((PVR_DBG_WARNING,
					 "%s: Unidentified Texas BIF Tag ID: %d",
					 __func__, ui32TagID));
			}
		} else if ((ui32BIFModule % ui32BIFsPerSPU) == 1) {
			IMG_UINT32 ui32Tag_RGX_TEXAS_BIF0_MCU_L1_TAG_LAST =
				(RGX_IS_FEATURE_SUPPORTED(
					psDevInfo,
					ALBIORIX_TOP_INFRASTRUCTURE)) ?
					RGX_TEXAS_BIF0_MCU_L1_TAG_LAST__ALBIORIX :
					RGX_TEXAS_BIF0_MCU_L1_TAG_LAST__SERIES8;

			/* Texas 1 BIF */
			if (ui32TagID <
			    ui32Tag_RGX_TEXAS_BIF0_MCU_L1_TAG_LAST) {
				pszTagID = "MCU L1";
				_RGXDecodeMMUReqMCULevel1(psDevInfo, ui32TagID,
							  &pszTagSB);
			} else if (ui32TagID <= RGX_TEXAS_BIF1_TAG_LAST) {
				switch (ui32TagID) {
				/** Albiorix/NUM_TPU_PER_SPU > 1 **/
				case 6:
				case 7:
					pszTagID = "BSC";
					break;
				/** All cores **/
				case 12:
					pszTagID = "TCU L1";
					break;
				case 13:
					pszTagID = "TPF";
					break;
				case 14:
					pszTagID = "TPF CPF";
					break;
				case 15:
				case 16:
					pszTagID = "PBE1";
					break;
				case 17:
					pszTagID = "PDSRW cache";
					break;
				case 18:
					pszTagID = "PDS";
					break;
				case 19:
				case 20:
					pszTagID = "ISP1 ID Array";
					break;
				case 21:
					pszTagID = "USC L2";
					break;
				case 22:
					pszTagID = "VDM L2";
					break;
				case 23:
					pszTagID = "RTU FBA L2";
					break;
				case 24:
					pszTagID = "RTU SHR L2";
					break;
				case 25:
					pszTagID = "RTU SHG L2";
					break;
				case 26:
					pszTagID = "RTU TUL L2";
					break;
				}
			} else {
				PVR_DPF((PVR_DBG_WARNING,
					 "%s: Unidentified Texas BIF Tag ID: %d",
					 __func__, ui32TagID));
			}
		}
	} else if (ui32BIFModule ==
		   RGX_GET_FEATURE_VALUE(psDevInfo, NUM_SPU) * ui32BIFsPerSPU) {
		/* Jones BIF */

		if ((ui32TagID >= RGX_JONES_BIF_DCE_TAG_FIRST) &&
		    (ui32TagID <= RGX_JONES_BIF_DCE_TAG_LAST)) {
			pszTagID = "DCE";
		} else if ((ui32TagID >= RGX_JONES_BIF_TDM_TAG_FIRST) &&
			   (ui32TagID <= RGX_JONES_BIF_TDM_TAG_LAST)) {
			pszTagID = "TDM";
		} else if ((ui32TagID >= RGX_JONES_BIF_CDM_TAG_FIRST) &&
			   (ui32TagID <= RGX_JONES_BIF_CDM_TAG_LAST)) {
			pszTagID = "CDM";
		} else if ((ui32TagID >= RGX_JONES_BIF_TE_TAG_FIRST) &&
			   (ui32TagID <= RGX_JONES_BIF_TE_TAG_LAST)) {
			pszTagID = "Tiling Engine (TE3)";
		} else if ((ui32TagID >= RGX_JONES_BIF_RTU_TAG_FIRST) &&
			   (ui32TagID <= RGX_JONES_BIF_RTU_TAG_LAST)) {
			pszTagID = "RTU";
		} else if (ui32TagID <= RGX_JONES_BIF_TAG_LAST) {
			switch (ui32TagID) {
			case RGX_JONES_BIF_IPP_TAG:
				pszTagID = "IPP";
				break;
			case RGX_JONES_BIF_PM_TAG:
				pszTagID = "PM";
				break;
			case RGX_JONES_BIF_META_TAG:
				pszTagID = "META";
				break;
			case RGX_JONES_BIF_META_DMA_TAG:
				pszTagID = "META DMA";
				break;
			case RGX_JONES_BIF_RPM_TAG:
				pszTagID = "RPM";
				break;
			}
		} else {
			PVR_DPF((PVR_DBG_WARNING,
				 "%s: Unidentified Jones BIF Tag ID: %d",
				 __func__, ui32TagID));
		}
	} else if (bWriteBack) {
		pszTagID = "";
		pszTagSB = "Writeback of dirty cacheline";
	} else {
		PVR_DPF((PVR_DBG_WARNING, "%s: Unidentified BIF Module: %d",
			 __func__, ui32BIFModule));
	}

	*ppszTagID = pszTagID;
	*ppszTagSB = pszTagSB;
}

/*!
*******************************************************************************

 @Function	_RGXDumpRGXMMUFaultStatus

 @Description

 Dump MMU Fault status in human readable form.

 @Input pfnDumpDebugPrintf   - The debug printf function
 @Input pvDumpDebugFile      - Optional file identifier to be passed to the
                               'printf' function if required
 @Input psDevInfo            - RGX device info
 @Input ui64MMUStatus        - MMU Status register value
 @Input pszMetaOrCore        - string representing call is for META or MMU core
 @Return   void

******************************************************************************/
static void _RGXDumpRGXMMUFaultStatus(DUMPDEBUG_PRINTF_FUNC *pfnDumpDebugPrintf,
				      void *pvDumpDebugFile,
				      PVRSRV_RGXDEV_INFO *psDevInfo,
				      const IMG_UINT64 aui64MMUStatus[],
				      const IMG_PCHAR pszMetaOrCore,
				      const IMG_CHAR *pszIndent)
{
	if (aui64MMUStatus[0] == 0x0) {
		PVR_DUMPDEBUG_LOG("%sMMU (%s) - OK", pszIndent, pszMetaOrCore);
	} else {
		IMG_UINT32 ui32PC =
			(aui64MMUStatus[0] &
			 ~RGX_CR_MMU_FAULT_STATUS1_CONTEXT_CLRMSK) >>
			RGX_CR_MMU_FAULT_STATUS1_CONTEXT_SHIFT;
		IMG_UINT64 ui64Addr =
			((aui64MMUStatus[0] &
			  ~RGX_CR_MMU_FAULT_STATUS1_ADDRESS_CLRMSK) >>
			 RGX_CR_MMU_FAULT_STATUS1_ADDRESS_SHIFT)
			<< 4; /* align shift */
		IMG_UINT32 ui32Requester =
			(aui64MMUStatus[0] &
			 ~RGX_CR_MMU_FAULT_STATUS1_REQ_ID_CLRMSK) >>
			RGX_CR_MMU_FAULT_STATUS1_REQ_ID_SHIFT;
		IMG_UINT32 ui32MMULevel =
			(aui64MMUStatus[0] &
			 ~RGX_CR_MMU_FAULT_STATUS1_LEVEL_CLRMSK) >>
			RGX_CR_MMU_FAULT_STATUS1_LEVEL_SHIFT;
		IMG_BOOL bRead = (aui64MMUStatus[0] &
				  RGX_CR_MMU_FAULT_STATUS1_RNW_EN) != 0;
		IMG_BOOL bFault = (aui64MMUStatus[0] &
				   RGX_CR_MMU_FAULT_STATUS1_FAULT_EN) != 0;
		IMG_BOOL bROFault = ((aui64MMUStatus[0] &
				      ~RGX_CR_MMU_FAULT_STATUS1_TYPE_CLRMSK) >>
				     RGX_CR_MMU_FAULT_STATUS1_TYPE_SHIFT) ==
				    0x2;
		IMG_BOOL bProtFault =
			((aui64MMUStatus[0] &
			  ~RGX_CR_MMU_FAULT_STATUS1_TYPE_CLRMSK) >>
			 RGX_CR_MMU_FAULT_STATUS1_TYPE_SHIFT) == 0x3;
		IMG_UINT32 ui32BIFModule;
		IMG_BOOL bWriteBack, bFBMFault;
		IMG_CHAR aszScratch[RGX_DEBUG_STR_SIZE];
		IMG_CHAR *pszTagID = NULL;
		IMG_CHAR *pszTagSB = NULL;
		const IMG_PCHAR pszMetaOrRiscv =
			RGX_IS_FEATURE_VALUE_SUPPORTED(psDevInfo, META) ?
				"META" :
				"RISCV";

		if (RGX_IS_FEATURE_SUPPORTED(psDevInfo,
					     ALBIORIX_TOP_INFRASTRUCTURE)) {
			ui32BIFModule =
				(aui64MMUStatus[1] &
				 ~RGX_CR_MMU_FAULT_STATUS2__ALBTOP__BIF_ID_CLRMSK) >>
				RGX_CR_MMU_FAULT_STATUS2__ALBTOP__BIF_ID_SHIFT;
			bWriteBack =
				(aui64MMUStatus[1] &
				 RGX_CR_MMU_FAULT_STATUS2__ALBTOP__WRITEBACK_EN) !=
				0;
			bFBMFault =
				(aui64MMUStatus[1] &
				 RGX_CR_MMU_FAULT_STATUS2__ALBTOP__FBM_FAULT_EN) !=
				0;
		} else {
			ui32BIFModule =
				(aui64MMUStatus[1] &
				 ~RGX_CR_MMU_FAULT_STATUS2_BIF_ID_CLRMSK) >>
				RGX_CR_MMU_FAULT_STATUS2_BIF_ID_SHIFT;
			bWriteBack = (aui64MMUStatus[1] &
				      RGX_CR_MMU_FAULT_STATUS2_WRITEBACK_EN) !=
				     0;
			bFBMFault = (aui64MMUStatus[1] &
				     RGX_CR_MMU_FAULT_STATUS2_FBM_FAULT_EN) !=
				    0;
		}

		if (strcmp(pszMetaOrCore, "Core") != 0) {
			ui32PC =
				(aui64MMUStatus[0] &
				 ~RGX_CR_MMU_FAULT_STATUS_META_CONTEXT_CLRMSK) >>
				RGX_CR_MMU_FAULT_STATUS_META_CONTEXT_SHIFT;
			ui64Addr =
				((aui64MMUStatus[0] &
				  ~RGX_CR_MMU_FAULT_STATUS_META_ADDRESS_CLRMSK) >>
				 RGX_CR_MMU_FAULT_STATUS_META_ADDRESS_SHIFT)
				<< 4; /* align shift */
			ui32Requester =
				(aui64MMUStatus[0] &
				 ~RGX_CR_MMU_FAULT_STATUS_META_REQ_ID_CLRMSK) >>
				RGX_CR_MMU_FAULT_STATUS_META_REQ_ID_SHIFT;
			ui32MMULevel =
				(aui64MMUStatus[0] &
				 ~RGX_CR_MMU_FAULT_STATUS_META_LEVEL_CLRMSK) >>
				RGX_CR_MMU_FAULT_STATUS_META_LEVEL_SHIFT;
			bRead = (aui64MMUStatus[0] &
				 RGX_CR_MMU_FAULT_STATUS_META_RNW_EN) != 0;
			bFault = (aui64MMUStatus[0] &
				  RGX_CR_MMU_FAULT_STATUS_META_FAULT_EN) != 0;
			bROFault =
				((aui64MMUStatus[0] &
				  ~RGX_CR_MMU_FAULT_STATUS_META_TYPE_CLRMSK) >>
				 RGX_CR_MMU_FAULT_STATUS_META_TYPE_SHIFT) ==
				0x2;
			bProtFault =
				((aui64MMUStatus[0] &
				  ~RGX_CR_MMU_FAULT_STATUS_META_TYPE_CLRMSK) >>
				 RGX_CR_MMU_FAULT_STATUS_META_TYPE_SHIFT) ==
				0x3;
		} else {
			_RGXDecodeMMUReqTags(psDevInfo, ui32Requester,
					     ui32BIFModule, bRead, bWriteBack,
					     bFBMFault, &pszTagID, &pszTagSB,
					     aszScratch, RGX_DEBUG_STR_SIZE);
		}

		PVR_DUMPDEBUG_LOG("%sMMU (%s) - FAULT:", pszIndent,
				  pszMetaOrCore);
		PVR_DUMPDEBUG_LOG(
			"%s  * MMU status (0x%016" IMG_UINT64_FMTSPECX
			" | 0x%08" IMG_UINT64_FMTSPECX
			"): PC = %d, %s 0x%010" IMG_UINT64_FMTSPECX
			", %s(%s)%s%s%s%s.",
			pszIndent, aui64MMUStatus[0], aui64MMUStatus[1], ui32PC,
			(bRead) ? "Reading from" : "Writing to", ui64Addr,
			(pszTagID) ? pszTagID : pszMetaOrRiscv,
			(pszTagSB) ? pszTagSB : "-", (bFault) ? ", Fault" : "",
			(bROFault) ? ", Read Only fault" : "",
			(bProtFault) ? ", PM/FW core protection fault" : "",
			_RGXDecodeMMULevel(ui32MMULevel));
	}
}

static_assert((RGX_CR_MMU_FAULT_STATUS1_CONTEXT_CLRMSK ==
	       RGX_CR_MMU_FAULT_STATUS_META_CONTEXT_CLRMSK),
	      "RGX_CR_MMU_FAULT_STATUS_META mismatch!");
static_assert((RGX_CR_MMU_FAULT_STATUS1_CONTEXT_SHIFT ==
	       RGX_CR_MMU_FAULT_STATUS_META_CONTEXT_SHIFT),
	      "RGX_CR_MMU_FAULT_STATUS_META mismatch!");
static_assert((RGX_CR_MMU_FAULT_STATUS1_ADDRESS_CLRMSK ==
	       RGX_CR_MMU_FAULT_STATUS_META_ADDRESS_CLRMSK),
	      "RGX_CR_MMU_FAULT_STATUS_META mismatch!");
static_assert((RGX_CR_MMU_FAULT_STATUS1_ADDRESS_SHIFT ==
	       RGX_CR_MMU_FAULT_STATUS_META_ADDRESS_SHIFT),
	      "RGX_CR_MMU_FAULT_STATUS_META mismatch!");
static_assert((RGX_CR_MMU_FAULT_STATUS1_REQ_ID_CLRMSK ==
	       RGX_CR_MMU_FAULT_STATUS_META_REQ_ID_CLRMSK),
	      "RGX_CR_MMU_FAULT_STATUS_META mismatch!");
static_assert((RGX_CR_MMU_FAULT_STATUS1_REQ_ID_SHIFT ==
	       RGX_CR_MMU_FAULT_STATUS_META_REQ_ID_SHIFT),
	      "RGX_CR_MMU_FAULT_STATUS_META mismatch!");
static_assert((RGX_CR_MMU_FAULT_STATUS1_LEVEL_CLRMSK ==
	       RGX_CR_MMU_FAULT_STATUS_META_LEVEL_CLRMSK),
	      "RGX_CR_MMU_FAULT_STATUS_META mismatch!");
static_assert((RGX_CR_MMU_FAULT_STATUS1_LEVEL_SHIFT ==
	       RGX_CR_MMU_FAULT_STATUS_META_LEVEL_SHIFT),
	      "RGX_CR_MMU_FAULT_STATUS_META mismatch!");
static_assert((RGX_CR_MMU_FAULT_STATUS1_RNW_EN ==
	       RGX_CR_MMU_FAULT_STATUS_META_RNW_EN),
	      "RGX_CR_MMU_FAULT_STATUS_META mismatch!");
static_assert((RGX_CR_MMU_FAULT_STATUS1_FAULT_EN ==
	       RGX_CR_MMU_FAULT_STATUS_META_FAULT_EN),
	      "RGX_CR_MMU_FAULT_STATUS_META mismatch!");
static_assert((RGX_CR_MMU_FAULT_STATUS1_TYPE_CLRMSK ==
	       RGX_CR_MMU_FAULT_STATUS_META_TYPE_CLRMSK),
	      "RGX_CR_MMU_FAULT_STATUS_META mismatch!");
static_assert((RGX_CR_MMU_FAULT_STATUS1_TYPE_SHIFT ==
	       RGX_CR_MMU_FAULT_STATUS_META_TYPE_SHIFT),
	      "RGX_CR_MMU_FAULT_STATUS_META mismatch!");

static const IMG_FLAGS2DESC asHWErrorState[] = {
	{ RGX_HW_ERR_NA, "N/A" },
	{ RGX_HW_ERR_PRIMID_FAILURE_DURING_DMKILL,
	  "Primitive ID failure during DM kill." },
};

#if !defined(NO_HARDWARE)
static inline IMG_CHAR const *_GetRISCVException(IMG_UINT32 ui32Mcause)
{
	switch (ui32Mcause) {
#define X(value, fatal, description)        \
	case value:                         \
		if (fatal)                  \
			return description; \
		return NULL;

		RGXRISCVFW_MCAUSE_TABLE
#undef X

	default:
		PVR_DPF((PVR_DBG_WARNING,
			 "Invalid RISC-V FW mcause value 0x%08x", ui32Mcause));
		return NULL;
	}
}
#endif /* !defined(NO_HARDWARE) */

/*
 *  Translate ID code to descriptive string.
 *  Returns on the first match.
 */
static void _ID2Description(IMG_CHAR *psDesc, IMG_UINT32 ui32DescSize,
			    const IMG_FLAGS2DESC *psConvTable,
			    IMG_UINT32 ui32TableSize, IMG_UINT32 ui32ID)
{
	IMG_UINT32 ui32Idx;

	for (ui32Idx = 0; ui32Idx < ui32TableSize; ui32Idx++) {
		if (ui32ID == psConvTable[ui32Idx].uiFlag) {
			OSStringLCopy(psDesc, psConvTable[ui32Idx].pszLabel,
				      ui32DescSize);
			return;
		}
	}
}

/*!
*******************************************************************************

 @Function	_RGXDumpFWAssert

 @Description

 Dump FW assert strings when a thread asserts.

 @Input pfnDumpDebugPrintf   - The debug printf function
 @Input pvDumpDebugFile      - Optional file identifier to be passed to the
                               'printf' function if required
 @Input psRGXFWIfTraceBufCtl - RGX FW trace buffer

 @Return   void

******************************************************************************/
static void _RGXDumpFWAssert(DUMPDEBUG_PRINTF_FUNC *pfnDumpDebugPrintf,
			     void *pvDumpDebugFile,
			     const RGXFWIF_TRACEBUF *psRGXFWIfTraceBufCtl)
{
	const IMG_CHAR *pszTraceAssertPath;
	const IMG_CHAR *pszTraceAssertInfo;
	IMG_INT32 ui32TraceAssertLine;
	IMG_UINT32 i;

	for (i = 0; i < RGXFW_THREAD_NUM; i++) {
		pszTraceAssertPath =
			psRGXFWIfTraceBufCtl->sTraceBuf[i].sAssertBuf.szPath;
		pszTraceAssertInfo =
			psRGXFWIfTraceBufCtl->sTraceBuf[i].sAssertBuf.szInfo;
		ui32TraceAssertLine = psRGXFWIfTraceBufCtl->sTraceBuf[i]
					      .sAssertBuf.ui32LineNum;

		/* print non-null assert strings */
		if (*pszTraceAssertInfo) {
			PVR_DUMPDEBUG_LOG("FW-T%d Assert: %s (%s:%d)", i,
					  pszTraceAssertInfo,
					  pszTraceAssertPath,
					  ui32TraceAssertLine);
		}
	}
}

/*!
*******************************************************************************

 @Function	_RGXDumpFWFaults

 @Description

 Dump FW assert strings when a thread asserts.

 @Input pfnDumpDebugPrintf   - The debug printf function
 @Input pvDumpDebugFile      - Optional file identifier to be passed to the
                               'printf' function if required
 @Input psFwSysData       - RGX FW shared system data

 @Return   void

******************************************************************************/
static void _RGXDumpFWFaults(DUMPDEBUG_PRINTF_FUNC *pfnDumpDebugPrintf,
			     void *pvDumpDebugFile,
			     const RGXFWIF_SYSDATA *psFwSysData)
{
	if (psFwSysData->ui32FWFaults > 0) {
		IMG_UINT32 ui32StartFault =
			psFwSysData->ui32FWFaults - RGXFWIF_FWFAULTINFO_MAX;
		IMG_UINT32 ui32EndFault = psFwSysData->ui32FWFaults - 1;
		IMG_UINT32 ui32Index;

		if (psFwSysData->ui32FWFaults < RGXFWIF_FWFAULTINFO_MAX) {
			ui32StartFault = 0;
		}

		for (ui32Index = ui32StartFault; ui32Index <= ui32EndFault;
		     ui32Index++) {
			const RGX_FWFAULTINFO *psFaultInfo =
				&psFwSysData
					 ->sFaultInfo[ui32Index %
						      RGXFWIF_FWFAULTINFO_MAX];
			IMG_UINT64 ui64Seconds, ui64Nanoseconds;

			/* Split OS timestamp in seconds and nanoseconds */
			RGXConvertOSTimestampToSAndNS(psFaultInfo->ui64OSTimer,
						      &ui64Seconds,
						      &ui64Nanoseconds);

			PVR_DUMPDEBUG_LOG("FW Fault %d: %s (%s:%d)",
					  ui32Index + 1,
					  psFaultInfo->sFaultBuf.szInfo,
					  psFaultInfo->sFaultBuf.szPath,
					  psFaultInfo->sFaultBuf.ui32LineNum);
			PVR_DUMPDEBUG_LOG(
				"            Data = 0x%08x, CRTimer = 0x%012" IMG_UINT64_FMTSPECx
				", OSTimer = %" IMG_UINT64_FMTSPEC
				".%09" IMG_UINT64_FMTSPEC,
				psFaultInfo->ui32Data, psFaultInfo->ui64CRTimer,
				ui64Seconds, ui64Nanoseconds);
		}
	}
}

static void _RGXDumpFWPoll(DUMPDEBUG_PRINTF_FUNC *pfnDumpDebugPrintf,
			   void *pvDumpDebugFile,
			   const RGXFWIF_SYSDATA *psFwSysData)
{
	IMG_UINT32 i;
	for (i = 0; i < RGXFW_THREAD_NUM; i++) {
		if (psFwSysData->aui32CrPollAddr[i]) {
			PVR_DUMPDEBUG_LOG(
				"T%u polling %s (reg:0x%08X mask:0x%08X)", i,
				((psFwSysData->aui32CrPollAddr[i] &
				  RGXFW_POLL_TYPE_SET) ?
					 ("set") :
					 ("unset")),
				psFwSysData->aui32CrPollAddr[i] &
					~RGXFW_POLL_TYPE_SET,
				psFwSysData->aui32CrPollMask[i]);
		}
	}
}

static void _RGXDumpFWHWRInfo(DUMPDEBUG_PRINTF_FUNC *pfnDumpDebugPrintf,
			      void *pvDumpDebugFile,
			      const RGXFWIF_SYSDATA *psFwSysData,
			      const RGXFWIF_HWRINFOBUF *psHWRInfoBuf,
			      PVRSRV_RGXDEV_INFO *psDevInfo)
{
	IMG_BOOL bAnyLocked = IMG_FALSE;
	IMG_UINT32 dm, i;
	IMG_UINT32 ui32LineSize;
	IMG_CHAR *pszLine, *pszTemp;
	const IMG_CHAR *apszDmNames[RGXFWIF_DM_MAX] = {
		"GP",  "TDM",	"GEOM",	 "3D",	 "CDM",
		"RAY", "GEOM2", "GEOM3", "GEOM4"
	};
	const IMG_CHAR szMsgHeader[] = "Number of HWR: ";
	const IMG_CHAR szMsgFalse[] = "FALSE(";
	IMG_CHAR *pszLockupType = "";
	const IMG_UINT32 ui32MsgHeaderCharCount =
		ARRAY_SIZE(szMsgHeader) - 1; /* size includes the null */
	const IMG_UINT32 ui32MsgFalseCharCount = ARRAY_SIZE(szMsgFalse) - 1;
	IMG_UINT32 ui32HWRRecoveryFlags;
	IMG_UINT32 ui32ReadIndex;

	for (dm = 0; dm < psDevInfo->sDevFeatureCfg.ui32MAXDMCount; dm++) {
		if (psHWRInfoBuf->aui32HwrDmLockedUpCount[dm] ||
		    psHWRInfoBuf->aui32HwrDmOverranCount[dm]) {
			bAnyLocked = IMG_TRUE;
			break;
		}
	}

	if (!PVRSRV_VZ_MODE_IS(GUEST) && !bAnyLocked &&
	    (psFwSysData->ui32HWRStateFlags & RGXFWIF_HWR_HARDWARE_OK)) {
		/* No HWR situation, print nothing */
		return;
	}

	if (PVRSRV_VZ_MODE_IS(GUEST)) {
		IMG_BOOL bAnyHWROccured = IMG_FALSE;

		for (dm = 0; dm < psDevInfo->sDevFeatureCfg.ui32MAXDMCount;
		     dm++) {
			if (psHWRInfoBuf->aui32HwrDmRecoveredCount[dm] != 0 ||
			    psHWRInfoBuf->aui32HwrDmLockedUpCount[dm] != 0 ||
			    psHWRInfoBuf->aui32HwrDmOverranCount[dm] != 0) {
				bAnyHWROccured = IMG_TRUE;
				break;
			}
		}

		if (!bAnyHWROccured) {
			return;
		}
	}

	ui32LineSize =
		sizeof(IMG_CHAR) *
		(ui32MsgHeaderCharCount +
		 (psDevInfo->sDevFeatureCfg.ui32MAXDMCount *
		  (5 /*DM name + left parenthesis*/ +
		   10 /*UINT32 max num of digits*/ + 1 /*slash*/ +
		   10 /*UINT32 max num of digits*/ +
		   3 /*right parenthesis + comma + space*/)) +
		 ui32MsgFalseCharCount + 1 +
		 (psDevInfo->sDevFeatureCfg.ui32MAXDMCount * 6) + 1
		 /* 'FALSE(' + ')' + (UINT16 max num + comma) per DM + \0 */
		);

	pszLine = OSAllocMem(ui32LineSize);
	if (pszLine == NULL) {
		PVR_DPF((PVR_DBG_ERROR,
			 "%s: Out of mem allocating line string (size: %d)",
			 __func__, ui32LineSize));
		return;
	}

	OSStringLCopy(pszLine, szMsgHeader, ui32LineSize);
	pszTemp = pszLine + ui32MsgHeaderCharCount;

	for (dm = 0; dm < psDevInfo->sDevFeatureCfg.ui32MAXDMCount; dm++) {
		pszTemp += OSSNPrintf(
			pszTemp,
			4 + 10 + 1 + 10 + 1 + 10 + 1 + 1 + 1 + 1
			/* (name + left parenthesis) + UINT32 + slash + UINT32 + plus + UINT32 + right parenthesis + comma + space + \0 */
			,
			"%s(%u/%u+%u), ", apszDmNames[dm],
			psHWRInfoBuf->aui32HwrDmRecoveredCount[dm],
			psHWRInfoBuf->aui32HwrDmLockedUpCount[dm],
			psHWRInfoBuf->aui32HwrDmOverranCount[dm]);
	}

	OSStringLCat(pszLine, szMsgFalse, ui32LineSize);
	pszTemp += ui32MsgFalseCharCount;

	for (dm = 0; dm < psDevInfo->sDevFeatureCfg.ui32MAXDMCount; dm++) {
		pszTemp += OSSNPrintf(
			pszTemp, 10 + 1 + 1 /* UINT32 max num + comma + \0 */,
			(dm < psDevInfo->sDevFeatureCfg.ui32MAXDMCount - 1 ?
				 "%u," :
				 "%u)"),
			psHWRInfoBuf->aui32HwrDmFalseDetectCount[dm]);
	}

	PVR_DUMPDEBUG_LOG("%s", pszLine);

	OSFreeMem(pszLine);

	/* Print out per HWR info */
	for (dm = 0; dm < psDevInfo->sDevFeatureCfg.ui32MAXDMCount; dm++) {
		if (dm == RGXFWIF_DM_GP) {
			PVR_DUMPDEBUG_LOG("DM %d (GP)", dm);
		} else {
			if (!PVRSRV_VZ_MODE_IS(GUEST)) {
				IMG_UINT32 ui32HWRRecoveryFlags =
					psFwSysData->aui32HWRRecoveryFlags[dm];
				IMG_CHAR sPerDmHwrDescription[RGX_DEBUG_STR_SIZE];
				sPerDmHwrDescription[0] = '\0';

				if (ui32HWRRecoveryFlags ==
				    RGXFWIF_DM_STATE_WORKING) {
					OSStringLCopy(sPerDmHwrDescription,
						      " working;",
						      RGX_DEBUG_STR_SIZE);
				} else {
					DebugCommonFlagStrings(
						sPerDmHwrDescription,
						RGX_DEBUG_STR_SIZE,
						asDmState2Description,
						ARRAY_SIZE(
							asDmState2Description),
						ui32HWRRecoveryFlags);
				}
				PVR_DUMPDEBUG_LOG("DM %d (HWRflags 0x%08x:%s)",
						  dm, ui32HWRRecoveryFlags,
						  sPerDmHwrDescription);
			} else {
				PVR_DUMPDEBUG_LOG("DM %d", dm);
			}
		}

		ui32ReadIndex = 0;
		for (i = 0; i < RGXFWIF_HWINFO_MAX; i++) {
			IMG_BOOL bPMFault = IMG_FALSE;
			IMG_UINT32 ui32PC;
			IMG_UINT32 ui32PageSize = 0;
			IMG_DEV_PHYADDR sPCDevPAddr = { 0 };
			const RGX_HWRINFO *psHWRInfo =
				&psHWRInfoBuf->sHWRInfo[ui32ReadIndex];

			if ((psHWRInfo->eDM == dm) &&
			    (psHWRInfo->ui32HWRNumber != 0)) {
				IMG_CHAR aui8RecoveryNum[10 + 10 + 1];
				IMG_UINT64 ui64Seconds, ui64Nanoseconds;
				IMG_BOOL bPageFault = IMG_FALSE;
				IMG_DEV_VIRTADDR sFaultDevVAddr;

				/* Split OS timestamp in seconds and nanoseconds */
				RGXConvertOSTimestampToSAndNS(
					psHWRInfo->ui64OSTimer, &ui64Seconds,
					&ui64Nanoseconds);

				ui32HWRRecoveryFlags =
					psHWRInfo->ui32HWRRecoveryFlags;
				if (ui32HWRRecoveryFlags &
				    RGXFWIF_DM_STATE_GUILTY_LOCKUP) {
					pszLockupType = ", Guilty Lockup";
				} else if (ui32HWRRecoveryFlags &
					   RGXFWIF_DM_STATE_INNOCENT_LOCKUP) {
					pszLockupType = ", Innocent Lockup";
				} else if (ui32HWRRecoveryFlags &
					   RGXFWIF_DM_STATE_GUILTY_OVERRUNING) {
					pszLockupType = ", Guilty Overrun";
				} else if (ui32HWRRecoveryFlags &
					   RGXFWIF_DM_STATE_INNOCENT_OVERRUNING) {
					pszLockupType = ", Innocent Overrun";
				} else if (ui32HWRRecoveryFlags &
					   RGXFWIF_DM_STATE_HARD_CONTEXT_SWITCH) {
					pszLockupType = ", Hard Context Switch";
				} else if (ui32HWRRecoveryFlags &
					   RGXFWIF_DM_STATE_GPU_ECC_HWR) {
					pszLockupType = ", GPU ECC HWR";
				}

				OSSNPrintf(aui8RecoveryNum,
					   sizeof(aui8RecoveryNum),
					   "Recovery %d:",
					   psHWRInfo->ui32HWRNumber);
				if (RGX_IS_FEATURE_SUPPORTED(
					    psDevInfo, GPU_MULTICORE_SUPPORT)) {
					PVR_DUMPDEBUG_LOG(
						"  %s Core = %u, PID = %u / %s, frame = %d, HWRTData = 0x%08X, EventStatus = 0x%08X%s",
						aui8RecoveryNum,
						psHWRInfo->ui32CoreID,
						psHWRInfo->ui32PID,
						psHWRInfo->szProcName,
						psHWRInfo->ui32FrameNum,
						psHWRInfo->ui32ActiveHWRTData,
						psHWRInfo->ui32EventStatus,
						pszLockupType);
				} else {
					PVR_DUMPDEBUG_LOG(
						"  %s PID = %u / %s, frame = %d, HWRTData = 0x%08X, EventStatus = 0x%08X%s",
						aui8RecoveryNum,
						psHWRInfo->ui32PID,
						psHWRInfo->szProcName,
						psHWRInfo->ui32FrameNum,
						psHWRInfo->ui32ActiveHWRTData,
						psHWRInfo->ui32EventStatus,
						pszLockupType);
				}

				if (psHWRInfo->eHWErrorCode != RGX_HW_ERR_NA) {
					IMG_CHAR sHWDebugInfo[RGX_DEBUG_STR_SIZE] =
						"";

					_ID2Description(
						sHWDebugInfo,
						RGX_DEBUG_STR_SIZE,
						asHWErrorState,
						ARRAY_SIZE(asHWErrorState),
						psHWRInfo->eHWErrorCode);
					PVR_DUMPDEBUG_LOG(
						"  HW error code = 0x%X: %s",
						psHWRInfo->eHWErrorCode,
						sHWDebugInfo);
				}

				pszTemp = &aui8RecoveryNum[0];
				while (*pszTemp != '\0') {
					*pszTemp++ = ' ';
				}

				/* There's currently no time correlation for the Guest OSes on the Firmware so there's no point printing OS Timestamps on Guests */
				if (!PVRSRV_VZ_MODE_IS(GUEST)) {
					PVR_DUMPDEBUG_LOG(
						"  %s CRTimer = 0x%012" IMG_UINT64_FMTSPECX
						", OSTimer = %" IMG_UINT64_FMTSPEC
						".%09" IMG_UINT64_FMTSPEC
						", CyclesElapsed = %" IMG_INT64_FMTSPECd,
						aui8RecoveryNum,
						psHWRInfo->ui64CRTimer,
						ui64Seconds, ui64Nanoseconds,
						(psHWRInfo->ui64CRTimer -
						 psHWRInfo->ui64CRTimeOfKick) *
							256);
				} else {
					PVR_DUMPDEBUG_LOG(
						"  %s CRTimer = 0x%012" IMG_UINT64_FMTSPECX
						", CyclesElapsed = %" IMG_INT64_FMTSPECd,
						aui8RecoveryNum,
						psHWRInfo->ui64CRTimer,
						(psHWRInfo->ui64CRTimer -
						 psHWRInfo->ui64CRTimeOfKick) *
							256);
				}

				if (psHWRInfo->ui64CRTimeHWResetFinish != 0) {
					if (psHWRInfo->ui64CRTimeFreelistReady !=
					    0) {
						/* If ui64CRTimeFreelistReady is less than ui64CRTimeHWResetFinish it means APM kicked in and the time is not valid. */
						if (psHWRInfo->ui64CRTimeHWResetFinish <
						    psHWRInfo
							    ->ui64CRTimeFreelistReady) {
							PVR_DUMPDEBUG_LOG(
								"  %s PreResetTimeInCycles = %" IMG_INT64_FMTSPECd
								", HWResetTimeInCycles = %" IMG_INT64_FMTSPECd
								", FreelistReconTimeInCycles = %" IMG_INT64_FMTSPECd
								", TotalRecoveryTimeInCycles = %" IMG_INT64_FMTSPECd,
								aui8RecoveryNum,
								(psHWRInfo->ui64CRTimeHWResetStart -
								 psHWRInfo
									 ->ui64CRTimer) *
									256,
								(psHWRInfo->ui64CRTimeHWResetFinish -
								 psHWRInfo
									 ->ui64CRTimeHWResetStart) *
									256,
								(psHWRInfo->ui64CRTimeFreelistReady -
								 psHWRInfo
									 ->ui64CRTimeHWResetFinish) *
									256,
								(psHWRInfo->ui64CRTimeFreelistReady -
								 psHWRInfo
									 ->ui64CRTimer) *
									256);
						} else {
							PVR_DUMPDEBUG_LOG(
								"  %s PreResetTimeInCycles = %" IMG_INT64_FMTSPECd
								", HWResetTimeInCycles = %" IMG_INT64_FMTSPECd
								", FreelistReconTimeInCycles = <not_timed>, TotalResetTimeInCycles = %" IMG_INT64_FMTSPECd,
								aui8RecoveryNum,
								(psHWRInfo->ui64CRTimeHWResetStart -
								 psHWRInfo
									 ->ui64CRTimer) *
									256,
								(psHWRInfo->ui64CRTimeHWResetFinish -
								 psHWRInfo
									 ->ui64CRTimeHWResetStart) *
									256,
								(psHWRInfo->ui64CRTimeHWResetFinish -
								 psHWRInfo
									 ->ui64CRTimer) *
									256);
						}
					} else {
						PVR_DUMPDEBUG_LOG(
							"  %s PreResetTimeInCycles = %" IMG_INT64_FMTSPECd
							", HWResetTimeInCycles = %" IMG_INT64_FMTSPECd
							", TotalResetTimeInCycles = %" IMG_INT64_FMTSPECd,
							aui8RecoveryNum,
							(psHWRInfo->ui64CRTimeHWResetStart -
							 psHWRInfo->ui64CRTimer) *
								256,
							(psHWRInfo->ui64CRTimeHWResetFinish -
							 psHWRInfo
								 ->ui64CRTimeHWResetStart) *
								256,
							(psHWRInfo->ui64CRTimeHWResetFinish -
							 psHWRInfo->ui64CRTimer) *
								256);
					}
				}

				if (RGX_IS_ERN_SUPPORTED(psDevInfo, 65104)) {
					PVR_DUMPDEBUG_LOG(
						"    Active PDS DM USCs = 0x%08x",
						psHWRInfo->ui32PDSActiveDMUSCs);
				}

				if (RGX_IS_ERN_SUPPORTED(psDevInfo, 69700)) {
					PVR_DUMPDEBUG_LOG(
						"    DMs stalled waiting on PDS Store space = 0x%08x",
						psHWRInfo->ui32PDSStalledDMs);
				}

				switch (psHWRInfo->eHWRType) {
				case RGX_HWRTYPE_ECCFAULT: {
					PVR_DUMPDEBUG_LOG(
						"    ECC fault GPU=0x%08x",
						psHWRInfo->uHWRData.sECCInfo
							.ui32FaultGPU);
				} break;

				case RGX_HWRTYPE_MMUFAULT: {
					if (RGX_IS_FEATURE_SUPPORTED(
						    psDevInfo,
						    S7_TOP_INFRASTRUCTURE)) {
						_RGXDumpRGXMMUFaultStatus(
							pfnDumpDebugPrintf,
							pvDumpDebugFile,
							psDevInfo,
							&psHWRInfo->uHWRData
								 .sMMUInfo
								 .aui64MMUStatus
									 [0],
							"Core",
							DD_NORMAL_INDENT);

						bPageFault = IMG_TRUE;
						sFaultDevVAddr.uiAddr =
							psHWRInfo->uHWRData
								.sMMUInfo
								.aui64MMUStatus
									[0];
						sFaultDevVAddr.uiAddr &=
							~RGX_CR_MMU_FAULT_STATUS1_ADDRESS_CLRMSK;
						sFaultDevVAddr.uiAddr >>=
							RGX_CR_MMU_FAULT_STATUS1_ADDRESS_SHIFT;
						sFaultDevVAddr.uiAddr <<=
							4; /* align shift */
						ui32PC =
							(psHWRInfo->uHWRData
								 .sMMUInfo
								 .aui64MMUStatus
									 [0] &
							 ~RGX_CR_MMU_FAULT_STATUS1_CONTEXT_CLRMSK) >>
							RGX_CR_MMU_FAULT_STATUS1_CONTEXT_SHIFT;
						bPMFault = (ui32PC <= 8);
						sPCDevPAddr.uiAddr =
							psHWRInfo->uHWRData
								.sMMUInfo
								.ui64PCAddress;
					}
				} break;

				case RGX_HWRTYPE_MMUMETAFAULT: {
					if (RGX_IS_FEATURE_SUPPORTED(
						    psDevInfo,
						    S7_TOP_INFRASTRUCTURE)) {
						const IMG_PCHAR pszMetaOrRiscv =
							RGX_IS_FEATURE_VALUE_SUPPORTED(
								psDevInfo,
								META) ?
								"Meta" :
								"RiscV";

						_RGXDumpRGXMMUFaultStatus(
							pfnDumpDebugPrintf,
							pvDumpDebugFile,
							psDevInfo,
							&psHWRInfo->uHWRData
								 .sMMUInfo
								 .aui64MMUStatus
									 [0],
							pszMetaOrRiscv,
							DD_NORMAL_INDENT);

						bPageFault = IMG_TRUE;
						sFaultDevVAddr.uiAddr =
							psHWRInfo->uHWRData
								.sMMUInfo
								.aui64MMUStatus
									[0];
						sFaultDevVAddr.uiAddr &=
							~RGX_CR_MMU_FAULT_STATUS1_ADDRESS_CLRMSK;
						sFaultDevVAddr.uiAddr >>=
							RGX_CR_MMU_FAULT_STATUS1_ADDRESS_SHIFT;
						sFaultDevVAddr.uiAddr <<=
							4; /* align shift */
						sPCDevPAddr.uiAddr =
							psHWRInfo->uHWRData
								.sMMUInfo
								.ui64PCAddress;
					}
				} break;

				case RGX_HWRTYPE_POLLFAILURE: {
					PVR_DUMPDEBUG_LOG(
						"    T%u polling %s (reg:0x%08X mask:0x%08X last:0x%08X)",
						psHWRInfo->uHWRData.sPollInfo
							.ui32ThreadNum,
						((psHWRInfo->uHWRData.sPollInfo
							  .ui32CrPollAddr &
						  RGXFW_POLL_TYPE_SET) ?
							 ("set") :
							 ("unset")),
						psHWRInfo->uHWRData.sPollInfo
								.ui32CrPollAddr &
							~RGXFW_POLL_TYPE_SET,
						psHWRInfo->uHWRData.sPollInfo
							.ui32CrPollMask,
						psHWRInfo->uHWRData.sPollInfo
							.ui32CrPollLastValue);
				} break;

				case RGX_HWRTYPE_OVERRUN:
				case RGX_HWRTYPE_UNKNOWNFAILURE: {
					/* Nothing to dump */
				} break;

				default: {
					PVR_DUMPDEBUG_LOG(
						"    Unknown HWR Info type: 0x%x",
						psHWRInfo->eHWRType);
				} break;
				}

				if (bPageFault) {
					RGXDumpFaultInfo(
						psDevInfo, pfnDumpDebugPrintf,
						pvDumpDebugFile, psHWRInfo,
						ui32ReadIndex, &sFaultDevVAddr,
						&sPCDevPAddr, bPMFault,
						ui32PageSize);
				}
			}

			if (ui32ReadIndex == RGXFWIF_HWINFO_MAX_FIRST - 1)
				ui32ReadIndex = psHWRInfoBuf->ui32WriteIndex;
			else
				ui32ReadIndex = (ui32ReadIndex + 1) -
						(ui32ReadIndex /
						 RGXFWIF_HWINFO_LAST_INDEX) *
							RGXFWIF_HWINFO_MAX_LAST;
		}
	}
}

#if defined(SUPPORT_VALIDATION)
static void _RGXDumpFWKickCountInfo(DUMPDEBUG_PRINTF_FUNC *pfnDumpDebugPrintf,
				    void *pvDumpDebugFile,
				    const RGXFWIF_OSDATA *psFwOsData,
				    PVRSRV_RGXDEV_INFO *psDevInfo)
{
	IMG_UINT32 ui32DMIndex, ui32LineSize;
	IMG_CHAR *pszLine, *pszTemp;
	const IMG_CHAR *apszDmNames[RGXFWIF_DM_MAX] = {
		"GP",  "TDM",	"GEOM",	 "3D",	 "CDM",
		"RAY", "GEOM2", "GEOM3", "GEOM4"
	};
	const IMG_CHAR szKicksHeader[] = "RGX Kicks: ";
	const IMG_CHAR szPipelineKicksHeader[] = "RGX Kicks(pipelined): ";
	const IMG_UINT32 ui32KicksHeaderCharCount =
		ARRAY_SIZE(szKicksHeader) - 1; /* size includes the null */
	const IMG_UINT32 ui32PipelineKicksHeaderCharCount =
		ARRAY_SIZE(szPipelineKicksHeader) -
		1; /* size includes the null */

	ui32LineSize =
		sizeof(IMG_CHAR) *
		(MAX(ui32KicksHeaderCharCount,
		     ui32PipelineKicksHeaderCharCount) +
		 (psDevInfo->sDevFeatureCfg.ui32MAXDMCount *
		  (5 /*DM name + equal sign*/ +
		   10 /*UINT32 max num of digits*/ + 3 /*comma + space*/)) +
		 1); /* \0 */

	pszLine = OSAllocMem(ui32LineSize);
	if (pszLine == NULL) {
		PVR_DPF((PVR_DBG_ERROR,
			 "%s: Out of mem allocating line string (size: %d)",
			 __func__, ui32LineSize));
		return;
	}

	/* Print the number of kicks in general... */
	OSStringLCopy(pszLine, szKicksHeader, ui32LineSize);
	pszTemp = pszLine + ui32KicksHeaderCharCount;

	for (ui32DMIndex = 1 /*Skip GP*/;
	     ui32DMIndex < psDevInfo->sDevFeatureCfg.ui32MAXDMCount;
	     ui32DMIndex++) {
		pszTemp += OSSNPrintf(
			pszTemp,
			5 + 1 + 10 + 1 + 1 + 1
			/* name + equal sign + UINT32 + comma + space + \0 */,
			"%s=%u, ", apszDmNames[ui32DMIndex],
			psFwOsData->aui32KickCount[ui32DMIndex]);
	}

	/* Go back 2 spaces and remove the last comma+space... */
	pszTemp -= 2;
	*pszTemp = '\0';

	PVR_DUMPDEBUG_LOG("%s", pszLine);

	/* Print the number of pipeline kicks... */
	if (RGX_IS_FEATURE_VALUE_SUPPORTED(psDevInfo,
					   PIPELINED_DATAMASTERS_VERSION) &&
	    RGX_GET_FEATURE_VALUE(psDevInfo, PIPELINED_DATAMASTERS_VERSION) >
		    0) {
		OSStringLCopy(pszLine, szPipelineKicksHeader, ui32LineSize);
		pszTemp = pszLine + ui32PipelineKicksHeaderCharCount;

		for (ui32DMIndex = 1 /*Skip GP*/;
		     ui32DMIndex < psDevInfo->sDevFeatureCfg.ui32MAXDMCount;
		     ui32DMIndex++) {
			pszTemp += OSSNPrintf(
				pszTemp,
				5 + 1 + 10 + 1 + 1 + 1
				/* name + equal sign + UINT32 + comma + space + \0 */,
				"%s=%u, ", apszDmNames[ui32DMIndex],
				psFwOsData->aui32KickPipelineCount[ui32DMIndex]);
		}

		/* Go back 2 spaces and remove the last comma+space... */
		pszTemp -= 2;
		*pszTemp = '\0';

		PVR_DUMPDEBUG_LOG("%s", pszLine);
	}

	OSFreeMem(pszLine);
}
#endif

#if !defined(NO_HARDWARE)

/*!
*******************************************************************************

 @Function	_CheckForPendingPage

 @Description

 Check if the MMU indicates it is blocked on a pending page
 MMU4 does not support pending pages, so return false.

 @Input psDevInfo	 - RGX device info

 @Return   IMG_BOOL      - IMG_TRUE if there is a pending page

******************************************************************************/
static INLINE IMG_BOOL _CheckForPendingPage(PVRSRV_RGXDEV_INFO *psDevInfo)
{
	/* MMU4 doesn't support pending pages */
	return (RGX_GET_FEATURE_VALUE(psDevInfo, MMU_VERSION) < 4) &&
	       (OSReadHWReg32(psDevInfo->pvRegsBaseKM, RGX_CR_MMU_ENTRY) &
		RGX_CR_MMU_ENTRY_PENDING_EN);
}

/*!
*******************************************************************************

 @Function	_GetPendingPageInfo

 @Description

 Get information about the pending page from the MMU status registers

 @Input psDevInfo	 - RGX device info
 @Output psDevVAddr      - The device virtual address of the pending MMU address translation
 @Output pui32CatBase    - The page catalog base

 @Return   void

******************************************************************************/
static void _GetPendingPageInfo(PVRSRV_RGXDEV_INFO *psDevInfo,
				IMG_DEV_VIRTADDR *psDevVAddr,
				IMG_UINT32 *pui32CatBase)
{
	IMG_UINT64 ui64BIFMMUEntryStatus;

	ui64BIFMMUEntryStatus =
		OSReadHWReg64(psDevInfo->pvRegsBaseKM, RGX_CR_MMU_ENTRY_STATUS);

	psDevVAddr->uiAddr = (ui64BIFMMUEntryStatus &
			      ~RGX_CR_MMU_ENTRY_STATUS_ADDRESS_CLRMSK);

	*pui32CatBase = (ui64BIFMMUEntryStatus &
			 ~RGX_CR_MMU_ENTRY_STATUS_CONTEXT_ID_CLRMSK) >>
			RGX_CR_MMU_ENTRY_STATUS_CONTEXT_ID_SHIFT;
}

#endif

void RGXDumpRGXDebugSummary(DUMPDEBUG_PRINTF_FUNC *pfnDumpDebugPrintf,
			    void *pvDumpDebugFile,
			    PVRSRV_RGXDEV_INFO *psDevInfo,
			    IMG_BOOL bRGXPoweredON)
{
	IMG_CHAR *pszState, *pszReason;
	const RGXFWIF_SYSDATA *psFwSysData = psDevInfo->psRGXFWIfFwSysData;
	const RGXFWIF_TRACEBUF *psRGXFWIfTraceBufCtl =
		psDevInfo->psRGXFWIfTraceBufCtl;
	IMG_UINT32 ui32DriverID;
	const RGXFWIF_RUNTIME_CFG *psRuntimeCfg =
		psDevInfo->psRGXFWIfRuntimeCfg;
	/* space for the current clock speed and 3 previous */
	RGXFWIF_TIME_CORR asTimeCorrs[4];
	IMG_UINT32 ui32NumClockSpeedChanges;

#if defined(NO_HARDWARE)
	PVR_UNREFERENCED_PARAMETER(bRGXPoweredON);
#else
	if ((bRGXPoweredON) && !PVRSRV_VZ_MODE_IS(GUEST)) {
		if (RGX_IS_FEATURE_SUPPORTED(psDevInfo,
					     S7_TOP_INFRASTRUCTURE)) {
			IMG_UINT64 aui64RegValMMUStatus[2];
			const IMG_PCHAR pszMetaOrRiscv =
				RGX_IS_FEATURE_VALUE_SUPPORTED(psDevInfo,
							       META) ?
					"Meta" :
					"RiscV";

			aui64RegValMMUStatus[0] =
				OSReadHWReg64(psDevInfo->pvRegsBaseKM,
					      RGX_CR_MMU_FAULT_STATUS1);
			aui64RegValMMUStatus[1] =
				OSReadHWReg64(psDevInfo->pvRegsBaseKM,
					      RGX_CR_MMU_FAULT_STATUS2);
			_RGXDumpRGXMMUFaultStatus(pfnDumpDebugPrintf,
						  pvDumpDebugFile, psDevInfo,
						  &aui64RegValMMUStatus[0],
						  "Core", DD_SUMMARY_INDENT);

			aui64RegValMMUStatus[0] =
				OSReadHWReg64(psDevInfo->pvRegsBaseKM,
					      RGX_CR_MMU_FAULT_STATUS_META);
			_RGXDumpRGXMMUFaultStatus(pfnDumpDebugPrintf,
						  pvDumpDebugFile, psDevInfo,
						  &aui64RegValMMUStatus[0],
						  pszMetaOrRiscv,
						  DD_SUMMARY_INDENT);
		}

		if (_CheckForPendingPage(psDevInfo)) {
			IMG_UINT32 ui32CatBase;
			IMG_DEV_VIRTADDR sDevVAddr;

			PVR_DUMPDEBUG_LOG("MMU Pending page: Yes");

			_GetPendingPageInfo(psDevInfo, &sDevVAddr,
					    &ui32CatBase);

			if (ui32CatBase <= MAX_RESERVED_FW_MMU_CONTEXT) {
				PVR_DUMPDEBUG_LOG(
					"Cannot check address on PM cat base %u",
					ui32CatBase);
			} else {
				IMG_UINT64 ui64CBaseMapping;
				IMG_DEV_PHYADDR sPCDevPAddr;
				MMU_FAULT_DATA sFaultData;
				IMG_BOOL bIsValid;
				IMG_UINT32 ui32CBaseMapCtxReg;

				if (RGX_GET_FEATURE_VALUE(
					    psDevInfo, HOST_SECURITY_VERSION) >
				    1) {
					ui32CBaseMapCtxReg =
						RGX_CR_MMU_CBASE_MAPPING_CONTEXT__HOST_SECURITY_GT1_AND_MH_PASID_WIDTH_LT6_AND_MMU_GE4;

					OSWriteUncheckedHWReg32(
						psDevInfo->pvSecureRegsBaseKM,
						ui32CBaseMapCtxReg,
						ui32CatBase);

					ui64CBaseMapping = OSReadUncheckedHWReg64(
						psDevInfo->pvSecureRegsBaseKM,
						RGX_CR_MMU_CBASE_MAPPING__HOST_SECURITY_GT1);
					sPCDevPAddr.uiAddr =
						(((ui64CBaseMapping &
						   ~RGX_CR_MMU_CBASE_MAPPING__HOST_SECURITY_GT1__BASE_ADDR_CLRMSK) >>
						  RGX_CR_MMU_CBASE_MAPPING__HOST_SECURITY_GT1__BASE_ADDR_SHIFT)
						 << RGX_CR_MMU_CBASE_MAPPING__HOST_SECURITY_GT1__BASE_ADDR_ALIGNSHIFT);
					bIsValid = !(
						ui64CBaseMapping &
						RGX_CR_MMU_CBASE_MAPPING__HOST_SECURITY_GT1__INVALID_EN);
				} else {
					ui32CBaseMapCtxReg =
						RGX_CR_MMU_CBASE_MAPPING_CONTEXT;

					OSWriteUncheckedHWReg32(
						psDevInfo->pvSecureRegsBaseKM,
						ui32CBaseMapCtxReg,
						ui32CatBase);

					ui64CBaseMapping = OSReadUncheckedHWReg64(
						psDevInfo->pvSecureRegsBaseKM,
						RGX_CR_MMU_CBASE_MAPPING);
					sPCDevPAddr.uiAddr =
						(((ui64CBaseMapping &
						   ~RGX_CR_MMU_CBASE_MAPPING_BASE_ADDR_CLRMSK) >>
						  RGX_CR_MMU_CBASE_MAPPING_BASE_ADDR_SHIFT)
						 << RGX_CR_MMU_CBASE_MAPPING_BASE_ADDR_ALIGNSHIFT);
					bIsValid = !(
						ui64CBaseMapping &
						RGX_CR_MMU_CBASE_MAPPING_INVALID_EN);
				}

				PVR_DUMPDEBUG_LOG(
					"Checking device virtual address " IMG_DEV_VIRTADDR_FMTSPEC
					" on cat base %u. PC Addr = 0x%" IMG_UINT64_FMTSPECx
					" is %s",
					sDevVAddr.uiAddr, ui32CatBase,
					sPCDevPAddr.uiAddr,
					bIsValid ? "valid" : "invalid");
				RGXCheckFaultAddress(psDevInfo, &sDevVAddr,
						     &sPCDevPAddr, &sFaultData);
				RGXDumpFaultAddressHostView(&sFaultData,
							    pfnDumpDebugPrintf,
							    pvDumpDebugFile,
							    DD_SUMMARY_INDENT);
			}
		}
	}
#endif /* NO_HARDWARE */

	/* Firmware state */
	switch (OSAtomicRead(&psDevInfo->psDeviceNode->eHealthStatus)) {
	case PVRSRV_DEVICE_HEALTH_STATUS_OK:
		pszState = "OK";
		break;
	case PVRSRV_DEVICE_HEALTH_STATUS_NOT_RESPONDING:
		pszState = "NOT RESPONDING";
		break;
	case PVRSRV_DEVICE_HEALTH_STATUS_DEAD:
		pszState = "DEAD";
		break;
	case PVRSRV_DEVICE_HEALTH_STATUS_FAULT:
		pszState = "FAULT";
		break;
	case PVRSRV_DEVICE_HEALTH_STATUS_UNDEFINED:
		pszState = "UNDEFINED";
		break;
	default:
		pszState = "UNKNOWN";
		break;
	}

	switch (OSAtomicRead(&psDevInfo->psDeviceNode->eHealthReason)) {
	case PVRSRV_DEVICE_HEALTH_REASON_NONE:
		pszReason = "";
		break;
	case PVRSRV_DEVICE_HEALTH_REASON_ASSERTED:
		pszReason = " - Asserted";
		break;
	case PVRSRV_DEVICE_HEALTH_REASON_POLL_FAILING:
		pszReason = " - Poll failing";
		break;
	case PVRSRV_DEVICE_HEALTH_REASON_TIMEOUTS:
		pszReason = " - Global Event Object timeouts rising";
		break;
	case PVRSRV_DEVICE_HEALTH_REASON_QUEUE_CORRUPT:
		pszReason = " - KCCB offset invalid";
		break;
	case PVRSRV_DEVICE_HEALTH_REASON_QUEUE_STALLED:
		pszReason = " - KCCB stalled";
		break;
	case PVRSRV_DEVICE_HEALTH_REASON_IDLING:
		pszReason = " - Idling";
		break;
	case PVRSRV_DEVICE_HEALTH_REASON_RESTARTING:
		pszReason = " - Restarting";
		break;
	case PVRSRV_DEVICE_HEALTH_REASON_MISSING_INTERRUPTS:
		pszReason = " - Missing interrupts";
		break;
	default:
		pszReason = " - Unknown reason";
		break;
	}

#if !defined(NO_HARDWARE)
	/* Determine the type virtualisation support used */
#if defined(RGX_NUM_DRIVERS_SUPPORTED) && (RGX_NUM_DRIVERS_SUPPORTED > 1)
	if (!PVRSRV_VZ_MODE_IS(NATIVE)) {
#if defined(RGX_VZ_STATIC_CARVEOUT_FW_HEAPS)
#if defined(SUPPORT_AUTOVZ)
#if defined(SUPPORT_AUTOVZ_HW_REGS)
		PVR_DUMPDEBUG_LOG(
			"RGX Virtualisation type: AutoVz with HW register support");
#else
		PVR_DUMPDEBUG_LOG(
			"RGX Virtualisation type: AutoVz with shared memory");
#endif /* defined(SUPPORT_AUTOVZ_HW_REGS) */
#else
		PVR_DUMPDEBUG_LOG(
			"RGX Virtualisation type: Hypervisor-assisted with static Fw heap allocation");
#endif /* defined(SUPPORT_AUTOVZ) */
#else
		PVR_DUMPDEBUG_LOG(
			"RGX Virtualisation type: Hypervisor-assisted with dynamic Fw heap allocation");
#endif /* defined(RGX_VZ_STATIC_CARVEOUT_FW_HEAPS) */
	}
#endif /* (RGX_NUM_DRIVERS_SUPPORTED > 1) */

#if defined(RGX_VZ_STATIC_CARVEOUT_FW_HEAPS) || \
	(defined(RGX_NUM_DRIVERS_SUPPORTED) &&  \
	 (RGX_NUM_DRIVERS_SUPPORTED > 1))
	if (!PVRSRV_VZ_MODE_IS(NATIVE)) {
		RGXFWIF_CONNECTION_FW_STATE eFwState =
			KM_GET_FW_CONNECTION(psDevInfo);
		RGXFWIF_CONNECTION_OS_STATE eOsState =
			KM_GET_OS_CONNECTION(psDevInfo);

		PVR_DUMPDEBUG_LOG(
			"RGX Virtualisation firmware connection state: %s (Fw=%s; OS=%s)",
			((eFwState == RGXFW_CONNECTION_FW_ACTIVE) &&
			 (eOsState == RGXFW_CONNECTION_OS_ACTIVE)) ?
				("UP") :
				("DOWN"),
			(eFwState < RGXFW_CONNECTION_FW_STATE_COUNT) ?
				(apszFwOsStateName[eFwState]) :
				("invalid"),
			(eOsState < RGXFW_CONNECTION_OS_STATE_COUNT) ?
				(apszFwOsStateName[eOsState]) :
				("invalid"));
	}
#endif

#if defined(SUPPORT_AUTOVZ) && defined(RGX_NUM_DRIVERS_SUPPORTED) && \
	(RGX_NUM_DRIVERS_SUPPORTED > 1)
	if (!PVRSRV_VZ_MODE_IS(NATIVE)) {
		IMG_UINT32 ui32FwAliveTS = KM_GET_FW_ALIVE_TOKEN(psDevInfo);
		IMG_UINT32 ui32OsAliveTS = KM_GET_OS_ALIVE_TOKEN(psDevInfo);

		PVR_DUMPDEBUG_LOG(
			"RGX Virtualisation watchdog timestamps (in GPU timer ticks): Fw=%u; OS=%u; diff(FW, OS) = %u",
			ui32FwAliveTS, ui32OsAliveTS,
			ui32FwAliveTS - ui32OsAliveTS);
	}
#endif
#endif /* !defined(NO_HARDWARE) */

	if (!PVRSRV_VZ_MODE_IS(GUEST)) {
		IMG_CHAR sHwrStateDescription[RGX_DEBUG_STR_SIZE];
		IMG_BOOL bDriverIsolationEnabled = IMG_FALSE;
		IMG_UINT32 ui32HostIsolationGroup;

		if (psFwSysData == NULL) {
			/* can't dump any more information */
			PVR_DUMPDEBUG_LOG("RGX FW State: %s%s", pszState,
					  pszReason);
			return;
		}

		sHwrStateDescription[0] = '\0';

		DebugCommonFlagStrings(sHwrStateDescription, RGX_DEBUG_STR_SIZE,
				       asHwrState2Description,
				       ARRAY_SIZE(asHwrState2Description),
				       psFwSysData->ui32HWRStateFlags);
		PVR_DUMPDEBUG_LOG("RGX FW State: %s%s (HWRState 0x%08x:%s)",
				  pszState, pszReason,
				  psFwSysData->ui32HWRStateFlags,
				  sHwrStateDescription);
		PVR_DUMPDEBUG_LOG(
			"RGX FW Power State: %s (APM %s: %d ok, %d denied, %d non-idle, %d retry, %d other, %d total. Latency: %u ms)",
			(psFwSysData->ePowState < ARRAY_SIZE(pszPowStateName) ?
				 pszPowStateName[psFwSysData->ePowState] :
				 "???"),
			(psDevInfo->pvAPMISRData) ? "enabled" : "disabled",
			psDevInfo->ui32ActivePMReqOk -
				psDevInfo->ui32ActivePMReqNonIdle,
			psDevInfo->ui32ActivePMReqDenied,
			psDevInfo->ui32ActivePMReqNonIdle,
			psDevInfo->ui32ActivePMReqRetry,
			psDevInfo->ui32ActivePMReqTotal -
				psDevInfo->ui32ActivePMReqOk -
				psDevInfo->ui32ActivePMReqDenied -
				psDevInfo->ui32ActivePMReqRetry -
				psDevInfo->ui32ActivePMReqNonIdle,
			psDevInfo->ui32ActivePMReqTotal,
			psRuntimeCfg->ui32ActivePMLatencyms);

		ui32NumClockSpeedChanges = (IMG_UINT32)OSAtomicRead(
			&psDevInfo->psDeviceNode->iNumClockSpeedChanges);
		RGXGetTimeCorrData(psDevInfo->psDeviceNode, asTimeCorrs,
				   ARRAY_SIZE(asTimeCorrs));

		PVR_DUMPDEBUG_LOG(
			"RGX DVFS: %u frequency changes. "
			"Current frequency: %u.%03u MHz (sampled at %" IMG_UINT64_FMTSPEC
			" ns). "
			"FW frequency: %u.%03u MHz.",
			ui32NumClockSpeedChanges,
			asTimeCorrs[0].ui32CoreClockSpeed / 1000000,
			(asTimeCorrs[0].ui32CoreClockSpeed / 1000) % 1000,
			asTimeCorrs[0].ui64OSTimeStamp,
			psRuntimeCfg->ui32CoreClockSpeed / 1000000,
			(psRuntimeCfg->ui32CoreClockSpeed / 1000) % 1000);
		if (ui32NumClockSpeedChanges > 0) {
			PVR_DUMPDEBUG_LOG(
				"          Previous frequencies: %u.%03u, %u.%03u, %u.%03u MHz (Sampled at "
				"%" IMG_UINT64_FMTSPEC ", %" IMG_UINT64_FMTSPEC
				", %" IMG_UINT64_FMTSPEC ")",
				asTimeCorrs[1].ui32CoreClockSpeed / 1000000,
				(asTimeCorrs[1].ui32CoreClockSpeed / 1000) %
					1000,
				asTimeCorrs[2].ui32CoreClockSpeed / 1000000,
				(asTimeCorrs[2].ui32CoreClockSpeed / 1000) %
					1000,
				asTimeCorrs[3].ui32CoreClockSpeed / 1000000,
				(asTimeCorrs[3].ui32CoreClockSpeed / 1000) %
					1000,
				asTimeCorrs[1].ui64OSTimeStamp,
				asTimeCorrs[2].ui64OSTimeStamp,
				asTimeCorrs[3].ui64OSTimeStamp);
		}

		ui32HostIsolationGroup =
			psDevInfo->psRGXFWIfRuntimeCfg
				->aui32DriverIsolationGroup[RGXFW_HOST_DRIVER_ID];

		FOREACH_SUPPORTED_DRIVER(ui32DriverID)
		{
			RGXFWIF_OS_RUNTIME_FLAGS sFwRunFlags =
				psFwSysData
					->asOsRuntimeFlagsMirror[ui32DriverID];
			IMG_UINT32 ui32IsolationGroup =
				psDevInfo->psRGXFWIfRuntimeCfg
					->aui32DriverIsolationGroup[ui32DriverID];
			IMG_BOOL bMTSEnabled = IMG_FALSE;

#if !defined(NO_HARDWARE)
			if (bRGXPoweredON) {
				bMTSEnabled =
					(!RGX_IS_FEATURE_SUPPORTED(
						psDevInfo,
						GPU_VIRTUALISATION)) ?
						IMG_TRUE :
						((OSReadHWReg32(
							  psDevInfo->pvRegsBaseKM,
							  RGX_CR_MTS_SCHEDULE_ENABLE) &
						  BIT(ui32DriverID)) != 0);
			}
#endif

			PVR_DUMPDEBUG_LOG(
				"RGX FW OS %u - State: %s; Freelists: %s%s; Priority: %u; Isolation group: %u; %s",
				ui32DriverID,
				apszFwOsStateName[sFwRunFlags.bfOsState],
				(sFwRunFlags.bfFLOk) ? "Ok" : "Not Ok",
				(sFwRunFlags.bfFLGrowPending) ?
					"; Grow Request Pending" :
					"",
				psDevInfo->psRGXFWIfRuntimeCfg
					->aui32DriverPriority[ui32DriverID],
				ui32IsolationGroup,
				(bMTSEnabled) ? "MTS on;" : "MTS off;");

			if (ui32IsolationGroup != ui32HostIsolationGroup) {
				bDriverIsolationEnabled = IMG_TRUE;
			}
		}

#if defined(PVR_ENABLE_PHR)
		{
			IMG_CHAR sPHRConfigDescription[RGX_DEBUG_STR_SIZE];

			sPHRConfigDescription[0] = '\0';
			DebugCommonFlagStrings(
				sPHRConfigDescription, RGX_DEBUG_STR_SIZE,
				asPHRConfig2Description,
				ARRAY_SIZE(asPHRConfig2Description),
				BIT_ULL(psDevInfo->psRGXFWIfRuntimeCfg
						->ui32PHRMode));

			PVR_DUMPDEBUG_LOG(
				"RGX PHR configuration: (%d) %s",
				psDevInfo->psRGXFWIfRuntimeCfg->ui32PHRMode,
				sPHRConfigDescription);
		}
#endif

		if (bDriverIsolationEnabled) {
			PVR_DUMPDEBUG_LOG(
				"RGX Hard Context Switch deadline: %u ms",
				psDevInfo->psRGXFWIfRuntimeCfg
					->ui32HCSDeadlineMS);
		}

		_RGXDumpFWAssert(pfnDumpDebugPrintf, pvDumpDebugFile,
				 psRGXFWIfTraceBufCtl);
		_RGXDumpFWFaults(pfnDumpDebugPrintf, pvDumpDebugFile,
				 psFwSysData);
		_RGXDumpFWPoll(pfnDumpDebugPrintf, pvDumpDebugFile,
			       psFwSysData);
	} else {
		PVR_DUMPDEBUG_LOG(
			"RGX FW State: Unavailable under Guest Mode of operation");
		PVR_DUMPDEBUG_LOG(
			"RGX FW Power State: Unavailable under Guest Mode of operation");
	}

	_RGXDumpFWHWRInfo(pfnDumpDebugPrintf, pvDumpDebugFile, psFwSysData,
			  psDevInfo->psRGXFWIfHWRInfoBufCtl, psDevInfo);
#if defined(SUPPORT_VALIDATION)
	_RGXDumpFWKickCountInfo(pfnDumpDebugPrintf, pvDumpDebugFile,
				psDevInfo->psRGXFWIfFwOsData, psDevInfo);
#endif

#if defined(SUPPORT_RGXFW_STATS_FRAMEWORK)
	/* Dump all non-zero values in lines of 8... */
	{
		IMG_CHAR pszLine[(9 * RGXFWIF_STATS_FRAMEWORK_LINESIZE) + 1];
		const IMG_UINT32 *pui32FWStatsBuf =
			psFwSysData->aui32FWStatsBuf;
		IMG_UINT32 ui32Index1, ui32Index2;

		PVR_DUMPDEBUG_LOG(
			"STATS[START]: RGXFWIF_STATS_FRAMEWORK_MAX=%d",
			RGXFWIF_STATS_FRAMEWORK_MAX);
		for (ui32Index1 = 0; ui32Index1 < RGXFWIF_STATS_FRAMEWORK_MAX;
		     ui32Index1 += RGXFWIF_STATS_FRAMEWORK_LINESIZE) {
			IMG_UINT32 ui32OrOfValues = 0;
			IMG_CHAR *pszBuf = pszLine;

			/* Print all values in this line and skip if all zero... */
			for (ui32Index2 = 0;
			     ui32Index2 < RGXFWIF_STATS_FRAMEWORK_LINESIZE;
			     ui32Index2++) {
				ui32OrOfValues |=
					pui32FWStatsBuf[ui32Index1 + ui32Index2];
				OSSNPrintf(pszBuf, 9 + 1, " %08x",
					   pui32FWStatsBuf[ui32Index1 +
							   ui32Index2]);
				pszBuf += 9; /* write over the '\0' */
			}

			if (ui32OrOfValues != 0) {
				PVR_DUMPDEBUG_LOG("STATS[%08x]:%s", ui32Index1,
						  pszLine);
			}
		}
		PVR_DUMPDEBUG_LOG("STATS[END]");
	}
#endif
}

#if !defined(NO_HARDWARE)
static void
_RGXDumpMetaSPExtraDebugInfo(DUMPDEBUG_PRINTF_FUNC *pfnDumpDebugPrintf,
			     void *pvDumpDebugFile,
			     PVRSRV_RGXDEV_INFO *psDevInfo)
{
/* List of extra META Slave Port debug registers */
/* Order in these two initialisers must match */
#define RGX_META_SP_EXTRA_DEBUG         \
	X(RGX_CR_META_SP_MSLVCTRL0)     \
	X(RGX_CR_META_SP_MSLVCTRL1)     \
	X(RGX_CR_META_SP_MSLVDATAX)     \
	X(RGX_CR_META_SP_MSLVIRQSTATUS) \
	X(RGX_CR_META_SP_MSLVIRQENABLE) \
	X(RGX_CR_META_SP_MSLVIRQLEVEL)

#define RGX_META_SP_EXTRA_DEBUG__HOST_SECURITY_V1_AND_METAREG_UNPACKED_ACCESSES \
	X(RGX_CR_META_SP_MSLVCTRL0__HOST_SECURITY_V1_AND_METAREG_UNPACKED)      \
	X(RGX_CR_META_SP_MSLVCTRL1__HOST_SECURITY_V1_AND_METAREG_UNPACKED)      \
	X(RGX_CR_META_SP_MSLVDATAX__HOST_SECURITY_V1_AND_METAREG_UNPACKED)      \
	X(RGX_CR_META_SP_MSLVIRQSTATUS__HOST_SECURITY_V1_AND_METAREG_UNPACKED)  \
	X(RGX_CR_META_SP_MSLVIRQENABLE__HOST_SECURITY_V1_AND_METAREG_UNPACKED)  \
	X(RGX_CR_META_SP_MSLVIRQLEVEL__HOST_SECURITY_V1_AND_METAREG_UNPACKED)

#define RGX_META_SP_EXTRA_DEBUG__HOST_SECURITY_GT1_AND_METAREG_UNPACKED_ACCESSES \
	X(RGX_CR_META_SP_MSLVCTRL0__HOST_SECURITY_GT1_AND_METAREG_UNPACKED)      \
	X(RGX_CR_META_SP_MSLVCTRL1__HOST_SECURITY_GT1_AND_METAREG_UNPACKED)      \
	X(RGX_CR_META_SP_MSLVDATAX__HOST_SECURITY_GT1_AND_METAREG_UNPACKED)      \
	X(RGX_CR_META_SP_MSLVIRQSTATUS__HOST_SECURITY_GT1_AND_METAREG_UNPACKED)  \
	X(RGX_CR_META_SP_MSLVIRQENABLE__HOST_SECURITY_GT1_AND_METAREG_UNPACKED)  \
	X(RGX_CR_META_SP_MSLVIRQLEVEL__HOST_SECURITY_GT1_AND_METAREG_UNPACKED)

	IMG_UINT32 ui32Idx;
	IMG_UINT32 ui32RegVal;
	IMG_UINT32 ui32RegAddr;

	const IMG_UINT32 *pui32DebugRegAddr;
	const IMG_UINT32 aui32DebugRegAddr[] = {
#define X(A) A,
		RGX_META_SP_EXTRA_DEBUG
#undef X
	};
	const IMG_UINT32 aui32DebugRegAddrUAHSV1[] = {
#define X(A) A,
		RGX_META_SP_EXTRA_DEBUG__HOST_SECURITY_V1_AND_METAREG_UNPACKED_ACCESSES
#undef X
	};

	const IMG_UINT32 aui32DebugRegAddrUAHSGT1[] = {
#define X(A) A,
		RGX_META_SP_EXTRA_DEBUG__HOST_SECURITY_GT1_AND_METAREG_UNPACKED_ACCESSES
#undef X
	};

	const IMG_CHAR *apszDebugRegName[] = {
#define X(A) #A,
		RGX_META_SP_EXTRA_DEBUG
#undef X
	};

	PVR_DUMPDEBUG_LOG("META Slave Port extra debug:");

	/* array of register offset values depends on feature. But don't augment names in apszDebugRegName */
	PVR_ASSERT(sizeof(aui32DebugRegAddrUAHSGT1) ==
		   sizeof(aui32DebugRegAddr));
	PVR_ASSERT(sizeof(aui32DebugRegAddrUAHSV1) ==
		   sizeof(aui32DebugRegAddr));
	pui32DebugRegAddr =
		RGX_IS_FEATURE_SUPPORTED(psDevInfo,
					 META_REGISTER_UNPACKED_ACCESSES) ?
			((RGX_GET_FEATURE_VALUE(psDevInfo,
						HOST_SECURITY_VERSION) > 1) ?
				 (aui32DebugRegAddrUAHSGT1) :
				 (aui32DebugRegAddrUAHSV1)) :
			aui32DebugRegAddr;

	/* dump set of Slave Port debug registers */
	for (ui32Idx = 0;
	     ui32Idx < sizeof(aui32DebugRegAddr) / sizeof(IMG_UINT32);
	     ui32Idx++) {
		const IMG_CHAR *pszRegName = apszDebugRegName[ui32Idx];

		ui32RegAddr = pui32DebugRegAddr[ui32Idx];
		ui32RegVal = OSReadUncheckedHWReg32(
			psDevInfo->pvSecureRegsBaseKM, ui32RegAddr);
		PVR_DUMPDEBUG_LOG("  * %s: 0x%8.8X", pszRegName, ui32RegVal);
	}
}
#endif /* !defined(NO_HARDWARE) */

/* Helper macros to emit data */
#define REG32_FMTSPEC "%-30s: 0x%08X"
#define REG64_FMTSPEC "%-30s: 0x%016" IMG_UINT64_FMTSPECX
#define DDLOG32(R)                           \
	PVR_DUMPDEBUG_LOG(REG32_FMTSPEC, #R, \
			  OSReadHWReg32(pvRegsBaseKM, RGX_CR_##R));
#define DDLOG64(R)                           \
	PVR_DUMPDEBUG_LOG(REG64_FMTSPEC, #R, \
			  OSReadHWReg64(pvRegsBaseKM, RGX_CR_##R));
#define DDLOGUNCHECKED64(R)                  \
	PVR_DUMPDEBUG_LOG(REG64_FMTSPEC, #R, \
			  OSReadUncheckedHWReg64(pvRegsBaseKM, RGX_CR_##R));
#define DDLOG32_DPX(R)                       \
	PVR_DUMPDEBUG_LOG(REG32_FMTSPEC, #R, \
			  OSReadHWReg32(pvRegsBaseKM, DPX_CR_##R));
#define DDLOG64_DPX(R)                       \
	PVR_DUMPDEBUG_LOG(REG64_FMTSPEC, #R, \
			  OSReadHWReg64(pvRegsBaseKM, DPX_CR_##R));
#define DDLOGVAL32(S, V) PVR_DUMPDEBUG_LOG(REG32_FMTSPEC, S, V);

#if !defined(NO_HARDWARE)
static PVRSRV_ERROR RGXDumpRISCVState(DUMPDEBUG_PRINTF_FUNC *pfnDumpDebugPrintf,
				      void *pvDumpDebugFile,
				      PVRSRV_RGXDEV_INFO *psDevInfo)
{
	void __iomem *pvRegsBaseKM = psDevInfo->pvSecureRegsBaseKM;
	RGXRISCVFW_STATE sRiscvState;
	const IMG_CHAR *pszException;
	PVRSRV_ERROR eError;

	/* Limit dump to what is currently being used */
	if (RGX_GET_FEATURE_VALUE(psDevInfo, HOST_SECURITY_VERSION) >= 4) {
		DDLOGUNCHECKED64(FWCORE_ADDR_REMAP_CONFIG4__HOST_SECURITY_GE4);
		DDLOGUNCHECKED64(FWCORE_ADDR_REMAP_CONFIG5__HOST_SECURITY_GE4);
		DDLOGUNCHECKED64(FWCORE_ADDR_REMAP_CONFIG6__HOST_SECURITY_GE4);
		DDLOGUNCHECKED64(FWCORE_ADDR_REMAP_CONFIG12__HOST_SECURITY_GE4);
		DDLOGUNCHECKED64(FWCORE_ADDR_REMAP_CONFIG13__HOST_SECURITY_GE4);
		DDLOGUNCHECKED64(FWCORE_ADDR_REMAP_CONFIG14__HOST_SECURITY_GE4);
	} else {
		DDLOG64(FWCORE_ADDR_REMAP_CONFIG4);
		DDLOG64(FWCORE_ADDR_REMAP_CONFIG5);
		DDLOG64(FWCORE_ADDR_REMAP_CONFIG6);
		DDLOG64(FWCORE_ADDR_REMAP_CONFIG12);
		DDLOG64(FWCORE_ADDR_REMAP_CONFIG13);
		DDLOG64(FWCORE_ADDR_REMAP_CONFIG14);
	}

	PVR_DUMPDEBUG_LOG("---- [ RISC-V internal state ] ----");

#if defined(SUPPORT_VALIDATION) || defined(SUPPORT_RISCV_GDB)
	if (RGXRiscvIsHalted(psDevInfo)) {
		/* Avoid resuming the RISC-V FW as most operations
		 * on the debug module require a halted core */
		PVR_DUMPDEBUG_LOG("(skipping as RISC-V found halted)");
		return PVRSRV_OK;
	}
#endif

	eError = RGXRiscvHalt(psDevInfo);
	PVR_GOTO_IF_ERROR(eError, _RISCVDMError);

#define X(name, address)                                                 \
	eError = RGXRiscvReadReg(psDevInfo, address, &sRiscvState.name); \
	PVR_LOG_GOTO_IF_ERROR(eError, "RGXRiscvReadReg", _RISCVDMError); \
	DDLOGVAL32(#name, sRiscvState.name);

	RGXRISCVFW_DEBUG_DUMP_REGISTERS
#undef X

	eError = RGXRiscvResume(psDevInfo);
	PVR_GOTO_IF_ERROR(eError, _RISCVDMError);

	pszException = _GetRISCVException(sRiscvState.mcause);
	if (pszException != NULL) {
		PVR_DUMPDEBUG_LOG("RISC-V FW hit an exception: %s",
				  pszException);

		eError = RGXValidateFWImage(pfnDumpDebugPrintf, pvDumpDebugFile,
					    psDevInfo);
		if (eError != PVRSRV_OK) {
			PVR_DUMPDEBUG_LOG(
				"Failed to validate any FW code corruption");
		}
	}

	return PVRSRV_OK;

_RISCVDMError:
	PVR_DPF((PVR_DBG_ERROR, "Failed to communicate with the Debug Module"));

	return eError;
}
#endif /* !defined(NO_HARDWARE) */

PVRSRV_ERROR RGXDumpRGXRegisters(DUMPDEBUG_PRINTF_FUNC *pfnDumpDebugPrintf,
				 void *pvDumpDebugFile,
				 PVRSRV_RGXDEV_INFO *psDevInfo)
{
#if defined(NO_HARDWARE)
	PVR_DUMPDEBUG_LOG("------[ RGX registers ]------");
	PVR_DUMPDEBUG_LOG("(Not supported for NO_HARDWARE builds)");

	return PVRSRV_OK;
#else /* !defined(NO_HARDWARE) */
	IMG_UINT32 ui32Meta = RGX_IS_FEATURE_VALUE_SUPPORTED(psDevInfo, META) ?
				      RGX_GET_FEATURE_VALUE(psDevInfo, META) :
				      0;
	IMG_UINT32 ui32RegVal;
	IMG_BOOL bFirmwarePerf;
	IMG_BOOL bMulticore =
		RGX_IS_FEATURE_SUPPORTED(psDevInfo, GPU_MULTICORE_SUPPORT);
	void __iomem *pvRegsBaseKM = psDevInfo->pvRegsBaseKM;
	PVRSRV_ERROR eError;

	PVR_DUMPDEBUG_LOG("------[ RGX registers ]------");
	PVR_DUMPDEBUG_LOG("RGX Register Base Address (Linear):   0x%p",
			  psDevInfo->pvRegsBaseKM);
	PVR_DUMPDEBUG_LOG("RGX Register Base Address (Physical): 0x%08lX",
			  (unsigned long)psDevInfo->sRegsPhysBase.uiAddr);

	if (RGX_GET_FEATURE_VALUE(psDevInfo, HOST_SECURITY_VERSION) > 1) {
		PVR_DUMPDEBUG_LOG(
			"RGX Host Secure Register Base Address (Linear):   0x%p",
			psDevInfo->pvSecureRegsBaseKM);
		PVR_DUMPDEBUG_LOG(
			"RGX Host Secure Register Base Address (Physical): 0x%08lX",
			(unsigned long)psDevInfo->sRegsPhysBase.uiAddr +
				RGX_HOST_SECURE_REGBANK_OFFSET);
	}

	if (RGX_IS_FEATURE_VALUE_SUPPORTED(psDevInfo, META)) {
		IMG_UINT32 ui32MSlvCtrl1Reg =
			RGX_IS_FEATURE_SUPPORTED(
				psDevInfo, META_REGISTER_UNPACKED_ACCESSES) ?
				((RGX_GET_FEATURE_VALUE(psDevInfo,
							HOST_SECURITY_VERSION) >
				  1) ?
					 RGX_CR_META_SP_MSLVCTRL1__HOST_SECURITY_GT1_AND_METAREG_UNPACKED :
					 RGX_CR_META_SP_MSLVCTRL1__HOST_SECURITY_V1_AND_METAREG_UNPACKED) :
				RGX_CR_META_SP_MSLVCTRL1;

		/* Forcing bit 6 of MslvCtrl1 to 0 to avoid internal reg read going through the core */
		OSWriteUncheckedHWReg32(psDevInfo->pvSecureRegsBaseKM,
					ui32MSlvCtrl1Reg, 0x0);
	}

	/* Check if firmware perf was set at Init time */
	bFirmwarePerf = (psDevInfo->psRGXFWIfSysInit->eFirmwarePerf !=
			 FW_PERF_CONF_NONE);

	DDLOG64(CORE_ID);

	if (bMulticore) {
		DDLOG64(MULTICORE);
		DDLOG32(MULTICORE_SYSTEM);
		DDLOG32(MULTICORE_DOMAIN);
	}
	DDLOG32(EVENT_STATUS);
	DDLOG64(TIMER);
	DDLOG64(CLK_CTRL0);
	DDLOG64(CLK_STATUS0);
	DDLOG64(CLK_CTRL1);
	DDLOG64(CLK_STATUS1);
	DDLOG32(CLK_CTRL2);
	DDLOG32(CLK_STATUS2);
	DDLOG64(MMU_FAULT_STATUS1);
	DDLOG64(MMU_FAULT_STATUS2);
	DDLOG64(MMU_FAULT_STATUS_PM);
	DDLOG64(MMU_FAULT_STATUS_META);
	DDLOG64(SLC_STATUS1);
	DDLOG64(SLC_STATUS2);
	DDLOG64(SLC_STATUS_DEBUG);
	DDLOG64(MMU_STATUS);
	DDLOG32(BIF_PFS);
	DDLOG32(BIF_TEXAS0_PFS);
	DDLOG32(BIF_TEXAS1_PFS);
	DDLOG32(BIF_OUTSTANDING_READ);
	DDLOG32(BIF_TEXAS0_OUTSTANDING_READ);
	DDLOG32(BIF_TEXAS1_OUTSTANDING_READ);
	DDLOG32(FBCDC_IDLE);
	DDLOG32(FBCDC_STATUS);
	DDLOG32(SPU_ENABLE);

	DDLOG64(CONTEXT_MAPPING0);
	DDLOG64(CONTEXT_MAPPING2);
	DDLOG64(CONTEXT_MAPPING3);
	DDLOG64(CONTEXT_MAPPING4);

	if (bMulticore) {
#if !defined(RGX_CR_MULTICORE_AXI)
#define RGX_CR_MULTICORE_AXI (0x2508U)
#define RGX_CR_MULTICORE_AXI_ERROR (0x2510U)
#endif
		DDLOG32(MULTICORE_AXI);
		DDLOG32(MULTICORE_AXI_ERROR);
		DDLOG32(MULTICORE_TDM_CTRL_COMMON);
		DDLOG32(MULTICORE_FRAGMENT_CTRL_COMMON);
		DDLOG32(MULTICORE_COMPUTE_CTRL_COMMON);
	}

	DDLOG32(PERF_PHASE_2D);
	DDLOG32(PERF_CYCLE_2D_TOTAL);
	DDLOG32(PERF_PHASE_GEOM);
	DDLOG32(PERF_CYCLE_GEOM_TOTAL);
	DDLOG32(PERF_PHASE_FRAG);
	DDLOG32(PERF_CYCLE_FRAG_TOTAL);
	DDLOG32(PERF_CYCLE_GEOM_OR_FRAG_TOTAL);
	DDLOG32(PERF_CYCLE_GEOM_AND_FRAG_TOTAL);
	DDLOG32(PERF_PHASE_COMP);
	DDLOG32(PERF_CYCLE_COMP_TOTAL);
	DDLOG32(PM_PARTIAL_RENDER_ENABLE);

	DDLOG32(ISP_RENDER);
	DDLOG32(ISP_CTL);

	DDLOG32(MTS_INTCTX);
	DDLOG32(MTS_BGCTX);
	DDLOG32(MTS_BGCTX_COUNTED_SCHEDULE);
	DDLOG32(MTS_SCHEDULE);
	DDLOG32(MTS_GPU_INT_STATUS);

	DDLOG32(CDM_CONTEXT_STORE_STATUS);
	DDLOG64(CDM_CONTEXT_PDS0);
	DDLOG64(CDM_CONTEXT_PDS1);
	DDLOG64(CDM_TERMINATE_PDS);
	DDLOG64(CDM_TERMINATE_PDS1);
	DDLOG64(CDM_CONTEXT_LOAD_PDS0);
	DDLOG64(CDM_CONTEXT_LOAD_PDS1);

	DDLOG32(JONES_IDLE);
	DDLOG32(SLC_IDLE);
	DDLOG32(SLC_FAULT_STOP_STATUS);

	DDLOG64(SCRATCH0);
	DDLOG64(SCRATCH1);
	DDLOG64(SCRATCH2);
	DDLOG64(SCRATCH3);
	DDLOG64(SCRATCH4);
	DDLOG64(SCRATCH5);
	DDLOG64(SCRATCH6);
	DDLOG64(SCRATCH7);
	DDLOG64(SCRATCH8);
	DDLOG64(SCRATCH9);
	DDLOG64(SCRATCH10);
	DDLOG64(SCRATCH11);
	DDLOG64(SCRATCH12);
	DDLOG64(SCRATCH13);
	DDLOG64(SCRATCH14);
	DDLOG64(SCRATCH15);
	DDLOG32(IRQ_OS0_EVENT_STATUS);

	if (ui32Meta) {
		IMG_BOOL bIsT0Enabled = IMG_FALSE, bIsFWFaulted = IMG_FALSE;
		IMG_UINT32 ui32MSlvIrqStatusReg =
			RGX_IS_FEATURE_SUPPORTED(
				psDevInfo, META_REGISTER_UNPACKED_ACCESSES) ?
				((RGX_GET_FEATURE_VALUE(psDevInfo,
							HOST_SECURITY_VERSION) >
				  1) ?
					 RGX_CR_META_SP_MSLVIRQSTATUS__HOST_SECURITY_GT1_AND_METAREG_UNPACKED :
					 RGX_CR_META_SP_MSLVIRQSTATUS__HOST_SECURITY_V1_AND_METAREG_UNPACKED) :
				RGX_CR_META_SP_MSLVIRQSTATUS;

		PVR_DUMPDEBUG_LOG(
			REG32_FMTSPEC, "META_SP_MSLVIRQSTATUS",
			OSReadUncheckedHWReg32(psDevInfo->pvSecureRegsBaseKM,
					       ui32MSlvIrqStatusReg));

		eError = RGXReadFWModuleAddr(psDevInfo, META_CR_T0ENABLE_OFFSET,
					     &ui32RegVal);
		PVR_LOG_GOTO_IF_ERROR(eError, "RGXReadFWModuleAddr",
				      _METASPError);
		DDLOGVAL32("T0 TXENABLE", ui32RegVal);
		if (ui32RegVal & META_CR_TXENABLE_ENABLE_BIT) {
			bIsT0Enabled = IMG_TRUE;
		}

		eError = RGXReadFWModuleAddr(psDevInfo, META_CR_T0STATUS_OFFSET,
					     &ui32RegVal);
		PVR_LOG_GOTO_IF_ERROR(eError, "RGXReadFWModuleAddr",
				      _METASPError);
		DDLOGVAL32("T0 TXSTATUS", ui32RegVal);

		/* check for FW fault */
		if (((ui32RegVal >> 20) & 0x3) == 0x2) {
			bIsFWFaulted = IMG_TRUE;
		}

		eError = RGXReadFWModuleAddr(psDevInfo, META_CR_T0DEFR_OFFSET,
					     &ui32RegVal);
		PVR_LOG_GOTO_IF_ERROR(eError, "RGXReadFWModuleAddr",
				      _METASPError);
		DDLOGVAL32("T0 TXDEFR", ui32RegVal);

		eError = RGXReadMetaCoreReg(psDevInfo, META_CR_THR0_PC,
					    &ui32RegVal);
		PVR_LOG_GOTO_IF_ERROR(eError, "RGXReadMetaCoreReg",
				      _METASPError);
		DDLOGVAL32("T0 PC", ui32RegVal);

		eError = RGXReadMetaCoreReg(psDevInfo, META_CR_THR0_PCX,
					    &ui32RegVal);
		PVR_LOG_GOTO_IF_ERROR(eError, "RGXReadMetaCoreReg",
				      _METASPError);
		DDLOGVAL32("T0 PCX", ui32RegVal);

		eError = RGXReadMetaCoreReg(psDevInfo, META_CR_THR0_SP,
					    &ui32RegVal);
		PVR_LOG_GOTO_IF_ERROR(eError, "RGXReadMetaCoreReg",
				      _METASPError);
		DDLOGVAL32("T0 SP", ui32RegVal);

		if ((ui32Meta == MTP218) || (ui32Meta == MTP219)) {
			eError = RGXReadFWModuleAddr(psDevInfo,
						     META_CR_T1ENABLE_OFFSET,
						     &ui32RegVal);
			PVR_LOG_GOTO_IF_ERROR(eError, "RGXReadFWModuleAddr",
					      _METASPError);
			DDLOGVAL32("T1 TXENABLE", ui32RegVal);

			eError = RGXReadFWModuleAddr(psDevInfo,
						     META_CR_T1STATUS_OFFSET,
						     &ui32RegVal);
			PVR_LOG_GOTO_IF_ERROR(eError, "RGXReadFWModuleAddr",
					      _METASPError);
			DDLOGVAL32("T1 TXSTATUS", ui32RegVal);

			eError = RGXReadFWModuleAddr(
				psDevInfo, META_CR_T1DEFR_OFFSET, &ui32RegVal);
			PVR_LOG_GOTO_IF_ERROR(eError, "RGXReadFWModuleAddr",
					      _METASPError);
			DDLOGVAL32("T1 TXDEFR", ui32RegVal);

			eError = RGXReadMetaCoreReg(psDevInfo, META_CR_THR1_PC,
						    &ui32RegVal);
			PVR_LOG_GOTO_IF_ERROR(eError, "RGXReadMetaCoreReg",
					      _METASPError);
			DDLOGVAL32("T1 PC", ui32RegVal);

			eError = RGXReadMetaCoreReg(psDevInfo, META_CR_THR1_PCX,
						    &ui32RegVal);
			PVR_LOG_GOTO_IF_ERROR(eError, "RGXReadMetaCoreReg",
					      _METASPError);
			DDLOGVAL32("T1 PCX", ui32RegVal);

			eError = RGXReadMetaCoreReg(psDevInfo, META_CR_THR1_SP,
						    &ui32RegVal);
			PVR_LOG_GOTO_IF_ERROR(eError, "RGXReadMetaCoreReg",
					      _METASPError);
			DDLOGVAL32("T1 SP", ui32RegVal);
		}

		if (bFirmwarePerf) {
			eError = RGXReadFWModuleAddr(
				psDevInfo, META_CR_PERF_COUNT0, &ui32RegVal);
			PVR_LOG_GOTO_IF_ERROR(eError, "RGXReadFWModuleAddr",
					      _METASPError);
			DDLOGVAL32("META_CR_PERF_COUNT0", ui32RegVal);

			eError = RGXReadFWModuleAddr(
				psDevInfo, META_CR_PERF_COUNT1, &ui32RegVal);
			PVR_LOG_GOTO_IF_ERROR(eError, "RGXReadFWModuleAddr",
					      _METASPError);
			DDLOGVAL32("META_CR_PERF_COUNT1", ui32RegVal);
		}

		if (bIsT0Enabled & bIsFWFaulted) {
			eError = RGXValidateFWImage(pfnDumpDebugPrintf,
						    pvDumpDebugFile, psDevInfo);
			if (eError != PVRSRV_OK) {
				PVR_DUMPDEBUG_LOG(
					"Failed to validate any FW code corruption");
			}
		} else if (bIsFWFaulted) {
			PVR_DUMPDEBUG_LOG(
				"Skipping FW code memory corruption checking as META is disabled");
		}
	}

	if (RGX_IS_FEATURE_SUPPORTED(psDevInfo, RISCV_FW_PROCESSOR)) {
		eError = RGXDumpRISCVState(pfnDumpDebugPrintf, pvDumpDebugFile,
					   psDevInfo);
		PVR_RETURN_IF_ERROR(eError);
	}

	return PVRSRV_OK;

_METASPError:
	PVR_DUMPDEBUG_LOG("Dump Slave Port debug information");
	_RGXDumpMetaSPExtraDebugInfo(pfnDumpDebugPrintf, pvDumpDebugFile,
				     psDevInfo);

	return eError;
#endif /* defined(NO_HARDWARE) */
}

#undef REG32_FMTSPEC
#undef REG64_FMTSPEC
#undef DDLOG32
#undef DDLOG64
#undef DDLOG32_DPX
#undef DDLOG64_DPX
#undef DDLOGVAL32

void RGXDumpAllContextInfo(PVRSRV_RGXDEV_INFO *psDevInfo,
			   DUMPDEBUG_PRINTF_FUNC *pfnDumpDebugPrintf,
			   void *pvDumpDebugFile, IMG_UINT32 ui32VerbLevel)
{
	DumpRenderCtxtsInfo(psDevInfo, pfnDumpDebugPrintf, pvDumpDebugFile,
			    ui32VerbLevel);
#if defined(SUPPORT_RGXKICKSYNC_BRIDGE)
	DumpKickSyncCtxtsInfo(psDevInfo, pfnDumpDebugPrintf, pvDumpDebugFile,
			      ui32VerbLevel);
#endif
	if (RGX_IS_FEATURE_SUPPORTED(psDevInfo, COMPUTE)) {
		DumpComputeCtxtsInfo(psDevInfo, pfnDumpDebugPrintf,
				     pvDumpDebugFile, ui32VerbLevel);
	}
	if (RGX_IS_FEATURE_SUPPORTED(psDevInfo, FASTRENDER_DM)) {
		DumpTDMTransferCtxtsInfo(psDevInfo, pfnDumpDebugPrintf,
					 pvDumpDebugFile, ui32VerbLevel);
	}
}

/******************************************************************************
 End of file (rgxdebug.c)
******************************************************************************/
