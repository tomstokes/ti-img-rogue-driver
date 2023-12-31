/******************************************************************************
@Title          Odin system control register definitions
@Copyright      Copyright (c) Imagination Technologies Ltd. All Rights Reserved
@Description    Odin FPGA register defs for IMG 3rd generation TCF

	Auto generated headers, eg. odn_core.h:
		regconv -d . -a 8 odn_core.def

	Source files :
		odn_core.def
		mca_debug.def
		sai_rx_debug.def
		sai_tx_debug.def
		ad_tx.def

	Changes:
		Removed obsolete copyright dates
		Changed lower case to upper case
			(eg. odn_core changed to ODN_CORE)
		Changed PVR5__ to ODN_
		Merged multiple .def files into one header

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
******************************************************************************/

/* tab size 4 */

#ifndef _ODIN_REGS_H_
#define _ODIN_REGS_H_

/******************************
  Generated from: odn_core.def
*******************************/

/*
	Register ID
*/
#define ODN_CORE_ID 0x0000
#define ODN_ID_VARIANT_MASK 0x0000FFFFU
#define ODN_ID_VARIANT_SHIFT 0
#define ODN_ID_VARIANT_SIGNED 0

#define ODN_ID_ID_MASK 0xFFFF0000U
#define ODN_ID_ID_SHIFT 16
#define ODN_ID_ID_SIGNED 0

/*
	Register REL
*/
#define ODN_CORE_REL 0x0004
#define ODN_REL_MINOR_MASK 0x0000FFFFU
#define ODN_REL_MINOR_SHIFT 0
#define ODN_REL_MINOR_SIGNED 0

#define ODN_REL_MAJOR_MASK 0xFFFF0000U
#define ODN_REL_MAJOR_SHIFT 16
#define ODN_REL_MAJOR_SIGNED 0

/*
	Register CHANGE_SET
*/
#define ODN_CORE_CHANGE_SET 0x0008
#define ODN_CHANGE_SET_SET_MASK 0xFFFFFFFFU
#define ODN_CHANGE_SET_SET_SHIFT 0
#define ODN_CHANGE_SET_SET_SIGNED 0

/*
	Register USER_ID
*/
#define ODN_CORE_USER_ID 0x000C
#define ODN_USER_ID_ID_MASK 0x000000FFU
#define ODN_USER_ID_ID_SHIFT 0
#define ODN_USER_ID_ID_SIGNED 0

/*
	Register USER_BUILD
*/
#define ODN_CORE_USER_BUILD 0x0010
#define ODN_USER_BUILD_BUILD_MASK 0xFFFFFFFFU
#define ODN_USER_BUILD_BUILD_SHIFT 0
#define ODN_USER_BUILD_BUILD_SIGNED 0

/*
	Register SW_IF_VERSION
*/
#define ODN_CORE_SW_IF_VERSION 0x0014
#define ODN_SW_IF_VERSION_VERSION_MASK 0x0000FFFFU
#define ODN_SW_IF_VERSION_VERSION_SHIFT 0
#define ODN_SW_IF_VERSION_VERSION_SIGNED 0

/*
	Register INTERNAL_RESETN
*/
#define ODN_CORE_INTERNAL_RESETN 0x0080
#define ODN_INTERNAL_RESETN_DDR_MASK 0x00000001U
#define ODN_INTERNAL_RESETN_DDR_SHIFT 0
#define ODN_INTERNAL_RESETN_DDR_SIGNED 0

#define ODN_INTERNAL_RESETN_MIG0_MASK 0x00000002U
#define ODN_INTERNAL_RESETN_MIG0_SHIFT 1
#define ODN_INTERNAL_RESETN_MIG0_SIGNED 0

#define ODN_INTERNAL_RESETN_MIG1_MASK 0x00000004U
#define ODN_INTERNAL_RESETN_MIG1_SHIFT 2
#define ODN_INTERNAL_RESETN_MIG1_SIGNED 0

#define ODN_INTERNAL_RESETN_PDP1_MASK 0x00000008U
#define ODN_INTERNAL_RESETN_PDP1_SHIFT 3
#define ODN_INTERNAL_RESETN_PDP1_SIGNED 0

#define ODN_INTERNAL_RESETN_PDP2_MASK 0x00000010U
#define ODN_INTERNAL_RESETN_PDP2_SHIFT 4
#define ODN_INTERNAL_RESETN_PDP2_SIGNED 0

#define ODN_INTERNAL_RESETN_PERIP_MASK 0x00000020U
#define ODN_INTERNAL_RESETN_PERIP_SHIFT 5
#define ODN_INTERNAL_RESETN_PERIP_SIGNED 0

#define ODN_INTERNAL_RESETN_GIST_MASK 0x00000040U
#define ODN_INTERNAL_RESETN_GIST_SHIFT 6
#define ODN_INTERNAL_RESETN_GIST_SIGNED 0

#define ODN_INTERNAL_RESETN_PIKE_MASK 0x00000080U
#define ODN_INTERNAL_RESETN_PIKE_SHIFT 7
#define ODN_INTERNAL_RESETN_PIKE_SIGNED 0

/*
	Register EXTERNAL_RESETN
*/
#define ODN_CORE_EXTERNAL_RESETN 0x0084
#define ODN_EXTERNAL_RESETN_DUT_MASK 0x00000001U
#define ODN_EXTERNAL_RESETN_DUT_SHIFT 0
#define ODN_EXTERNAL_RESETN_DUT_SIGNED 0

#define ODN_EXTERNAL_RESETN_DUT_SPI_MASK 0x00000002U
#define ODN_EXTERNAL_RESETN_DUT_SPI_SHIFT 1
#define ODN_EXTERNAL_RESETN_DUT_SPI_SIGNED 0

#define ODN_EXTERNAL_RESETN_DUT_PEP_DDR_MASK 0x00000004U
#define ODN_EXTERNAL_RESETN_DUT_PEP_DDR_SHIFT 2
#define ODN_EXTERNAL_RESETN_DUT_PEP_DDR_SIGNED 0

#define ODN_EXTERNAL_RESETN_DUT_IF_MASK 0x00000008U
#define ODN_EXTERNAL_RESETN_DUT_IF_SHIFT 3
#define ODN_EXTERNAL_RESETN_DUT_IF_SIGNED 0

#define ODN_EXTERNAL_RESETN_DUT1_MASK 0x00000010U
#define ODN_EXTERNAL_RESETN_DUT1_SHIFT 4
#define ODN_EXTERNAL_RESETN_DUT1_SIGNED 0

#define ODN_EXTERNAL_RESETN_DUT2_MASK 0x00000020U
#define ODN_EXTERNAL_RESETN_DUT2_SHIFT 5
#define ODN_EXTERNAL_RESETN_DUT2_SIGNED 0

/*
	Register EXTERNAL_RESET
*/
#define ODN_CORE_EXTERNAL_RESET 0x0088
#define ODN_EXTERNAL_RESET_PVT_CAL_MASK 0x00000001U
#define ODN_EXTERNAL_RESET_PVT_CAL_SHIFT 0
#define ODN_EXTERNAL_RESET_PVT_CAL_SIGNED 0

#define ODN_EXTERNAL_RESET_PLL_MASK 0x00000002U
#define ODN_EXTERNAL_RESET_PLL_SHIFT 1
#define ODN_EXTERNAL_RESET_PLL_SIGNED 0

/*
	Register INTERNAL_AUTO_RESETN
*/
#define ODN_CORE_INTERNAL_AUTO_RESETN 0x008C
#define ODN_INTERNAL_AUTO_RESETN_AUX_MASK 0x00000001U
#define ODN_INTERNAL_AUTO_RESETN_AUX_SHIFT 0
#define ODN_INTERNAL_AUTO_RESETN_AUX_SIGNED 0

/*
	Register CLK_GEN_RESET
*/
#define ODN_CORE_CLK_GEN_RESET 0x0090
#define ODN_CLK_GEN_RESET_DUT_CORE_MMCM_MASK 0x00000001U
#define ODN_CLK_GEN_RESET_DUT_CORE_MMCM_SHIFT 0
#define ODN_CLK_GEN_RESET_DUT_CORE_MMCM_SIGNED 0

#define ODN_CLK_GEN_RESET_DUT_IF_MMCM_MASK 0x00000002U
#define ODN_CLK_GEN_RESET_DUT_IF_MMCM_SHIFT 1
#define ODN_CLK_GEN_RESET_DUT_IF_MMCM_SIGNED 0

#define ODN_CLK_GEN_RESET_MULTI_MMCM_MASK 0x00000004U
#define ODN_CLK_GEN_RESET_MULTI_MMCM_SHIFT 2
#define ODN_CLK_GEN_RESET_MULTI_MMCM_SIGNED 0

#define ODN_CLK_GEN_RESET_PDP_MMCM_MASK 0x00000008U
#define ODN_CLK_GEN_RESET_PDP_MMCM_SHIFT 3
#define ODN_CLK_GEN_RESET_PDP_MMCM_SIGNED 0

/*
	Register INTERRUPT_STATUS
*/
#define ODN_CORE_INTERRUPT_STATUS 0x0100
#define ODN_INTERRUPT_STATUS_DUT_MASK 0x00000001U
#define ODN_INTERRUPT_STATUS_DUT_SHIFT 0
#define ODN_INTERRUPT_STATUS_DUT_SIGNED 0

#define ODN_INTERRUPT_STATUS_PDP1_MASK 0x00000002U
#define ODN_INTERRUPT_STATUS_PDP1_SHIFT 1
#define ODN_INTERRUPT_STATUS_PDP1_SIGNED 0

#define ODN_INTERRUPT_STATUS_PDP2_MASK 0x00000004U
#define ODN_INTERRUPT_STATUS_PDP2_SHIFT 2
#define ODN_INTERRUPT_STATUS_PDP2_SIGNED 0

#define ODN_INTERRUPT_STATUS_PERIP_MASK 0x00000008U
#define ODN_INTERRUPT_STATUS_PERIP_SHIFT 3
#define ODN_INTERRUPT_STATUS_PERIP_SIGNED 0

#define ODN_INTERRUPT_STATUS_UART_MASK 0x00000010U
#define ODN_INTERRUPT_STATUS_UART_SHIFT 4
#define ODN_INTERRUPT_STATUS_UART_SIGNED 0

#define ODN_INTERRUPT_STATUS_GIST_IN_LNK_ERR_MASK 0x00000020U
#define ODN_INTERRUPT_STATUS_GIST_IN_LNK_ERR_SHIFT 5
#define ODN_INTERRUPT_STATUS_GIST_IN_LNK_ERR_SIGNED 0

#define ODN_INTERRUPT_STATUS_GIST_IN_MB_ERR_MASK 0x00000040U
#define ODN_INTERRUPT_STATUS_GIST_IN_MB_ERR_SHIFT 6
#define ODN_INTERRUPT_STATUS_GIST_IN_MB_ERR_SIGNED 0

#define ODN_INTERRUPT_STATUS_GIST_OUT_LNK_ERR_MASK 0x00000080U
#define ODN_INTERRUPT_STATUS_GIST_OUT_LNK_ERR_SHIFT 7
#define ODN_INTERRUPT_STATUS_GIST_OUT_LNK_ERR_SIGNED 0

#define ODN_INTERRUPT_STATUS_GIST_OUT_MB_ERR_MASK 0x00000100U
#define ODN_INTERRUPT_STATUS_GIST_OUT_MB_ERR_SHIFT 8
#define ODN_INTERRUPT_STATUS_GIST_OUT_MB_ERR_SIGNED 0

#define ODN_INTERRUPT_STATUS_DUT2_MASK 0x00000200U
#define ODN_INTERRUPT_STATUS_DUT2_SHIFT 9
#define ODN_INTERRUPT_STATUS_DUT2_SIGNED 0

#define ODN_INTERRUPT_STATUS_AXI_LOCKUP_PROTECTION_MASK 0x00000400U
#define ODN_INTERRUPT_STATUS_AXI_LOCKUP_PROTECTION_SHIFT 10
#define ODN_INTERRUPT_STATUS_AXI_LOCKUP_PROTECTION_SIGNED 0

#define ODN_INTERRUPT_STATUS_CDMA_MASK 0x00001800U
#define ODN_INTERRUPT_STATUS_CDMA_SHIFT 11
#define ODN_INTERRUPT_STATUS_CDMA_SIGNED 0

#define ODN_INTERRUPT_STATUS_OS_IRQ_MASK 0x001FE000U
#define ODN_INTERRUPT_STATUS_OS_IRQ_SHIFT 13
#define ODN_INTERRUPT_STATUS_OS_IRQ_SIGNED 0

#define ODN_INTERRUPT_STATUS_IRQ_TEST_MASK 0x40000000U
#define ODN_INTERRUPT_STATUS_IRQ_TEST_SHIFT 30
#define ODN_INTERRUPT_STATUS_IRQ_TEST_SIGNED 0

#define ODN_INTERRUPT_STATUS_MASTER_STATUS_MASK 0x80000000U
#define ODN_INTERRUPT_STATUS_MASTER_STATUS_SHIFT 31
#define ODN_INTERRUPT_STATUS_MASTER_STATUS_SIGNED 0

/*
	Register INTERRUPT_ENABLE
*/
#define ODN_CORE_INTERRUPT_ENABLE 0x0104
#define ODN_INTERRUPT_ENABLE_DUT_MASK 0x00000001U
#define ODN_INTERRUPT_ENABLE_DUT_SHIFT 0
#define ODN_INTERRUPT_ENABLE_DUT_SIGNED 0

#define ODN_INTERRUPT_ENABLE_PDP1_MASK 0x00000002U
#define ODN_INTERRUPT_ENABLE_PDP1_SHIFT 1
#define ODN_INTERRUPT_ENABLE_PDP1_SIGNED 0

#define ODN_INTERRUPT_ENABLE_PDP2_MASK 0x00000004U
#define ODN_INTERRUPT_ENABLE_PDP2_SHIFT 2
#define ODN_INTERRUPT_ENABLE_PDP2_SIGNED 0

#define ODN_INTERRUPT_ENABLE_PERIP_MASK 0x00000008U
#define ODN_INTERRUPT_ENABLE_PERIP_SHIFT 3
#define ODN_INTERRUPT_ENABLE_PERIP_SIGNED 0

#define ODN_INTERRUPT_ENABLE_UART_MASK 0x00000010U
#define ODN_INTERRUPT_ENABLE_UART_SHIFT 4
#define ODN_INTERRUPT_ENABLE_UART_SIGNED 0

#define ODN_INTERRUPT_ENABLE_GIST_IN_LNK_ERR_MASK 0x00000020U
#define ODN_INTERRUPT_ENABLE_GIST_IN_LNK_ERR_SHIFT 5
#define ODN_INTERRUPT_ENABLE_GIST_IN_LNK_ERR_SIGNED 0

#define ODN_INTERRUPT_ENABLE_GIST_IN_MB_ERR_MASK 0x00000040U
#define ODN_INTERRUPT_ENABLE_GIST_IN_MB_ERR_SHIFT 6
#define ODN_INTERRUPT_ENABLE_GIST_IN_MB_ERR_SIGNED 0

#define ODN_INTERRUPT_ENABLE_GIST_OUT_LNK_ERR_MASK 0x00000080U
#define ODN_INTERRUPT_ENABLE_GIST_OUT_LNK_ERR_SHIFT 7
#define ODN_INTERRUPT_ENABLE_GIST_OUT_LNK_ERR_SIGNED 0

#define ODN_INTERRUPT_ENABLE_GIST_OUT_MB_ERR_MASK 0x00000100U
#define ODN_INTERRUPT_ENABLE_GIST_OUT_MB_ERR_SHIFT 8
#define ODN_INTERRUPT_ENABLE_GIST_OUT_MB_ERR_SIGNED 0

#define ODN_INTERRUPT_ENABLE_DUT2_MASK 0x00000200U
#define ODN_INTERRUPT_ENABLE_DUT2_SHIFT 9
#define ODN_INTERRUPT_ENABLE_DUT2_SIGNED 0

#define ODN_INTERRUPT_ENABLE_AXI_LOCKUP_PROTECTION_MASK 0x00000400U
#define ODN_INTERRUPT_ENABLE_AXI_LOCKUP_PROTECTION_SHIFT 10
#define ODN_INTERRUPT_ENABLE_AXI_LOCKUP_PROTECTION_SIGNED 0

#define ODN_INTERRUPT_ENABLE_CDMA_MASK 0x00001800U
#define ODN_INTERRUPT_ENABLE_CDMA_SHIFT 11
#define ODN_INTERRUPT_ENABLE_CDMA_SIGNED 0

#define ODN_INTERRUPT_ENABLE_OS_IRQ_MASK 0x001FE000U
#define ODN_INTERRUPT_ENABLE_OS_IRQ_SHIFT 13
#define ODN_INTERRUPT_ENABLE_OS_IRQ_SIGNED 0

#define ODN_INTERRUPT_ENABLE_IRQ_TEST_MASK 0x40000000U
#define ODN_INTERRUPT_ENABLE_IRQ_TEST_SHIFT 30
#define ODN_INTERRUPT_ENABLE_IRQ_TEST_SIGNED 0

#define ODN_INTERRUPT_ENABLE_MASTER_ENABLE_MASK 0x80000000U
#define ODN_INTERRUPT_ENABLE_MASTER_ENABLE_SHIFT 31
#define ODN_INTERRUPT_ENABLE_MASTER_ENABLE_SIGNED 0

/*
	Register INTERRUPT_CLR
*/
#define ODN_CORE_INTERRUPT_CLR 0x010C
#define ODN_INTERRUPT_CLR_DUT_MASK 0x00000001U
#define ODN_INTERRUPT_CLR_DUT_SHIFT 0
#define ODN_INTERRUPT_CLR_DUT_SIGNED 0

#define ODN_INTERRUPT_CLR_PDP1_MASK 0x00000002U
#define ODN_INTERRUPT_CLR_PDP1_SHIFT 1
#define ODN_INTERRUPT_CLR_PDP1_SIGNED 0

#define ODN_INTERRUPT_CLR_PDP2_MASK 0x00000004U
#define ODN_INTERRUPT_CLR_PDP2_SHIFT 2
#define ODN_INTERRUPT_CLR_PDP2_SIGNED 0

#define ODN_INTERRUPT_CLR_PERIP_MASK 0x00000008U
#define ODN_INTERRUPT_CLR_PERIP_SHIFT 3
#define ODN_INTERRUPT_CLR_PERIP_SIGNED 0

#define ODN_INTERRUPT_CLR_UART_MASK 0x00000010U
#define ODN_INTERRUPT_CLR_UART_SHIFT 4
#define ODN_INTERRUPT_CLR_UART_SIGNED 0

#define ODN_INTERRUPT_CLR_GIST_IN_LNK_ERR_MASK 0x00000020U
#define ODN_INTERRUPT_CLR_GIST_IN_LNK_ERR_SHIFT 5
#define ODN_INTERRUPT_CLR_GIST_IN_LNK_ERR_SIGNED 0

#define ODN_INTERRUPT_CLR_GIST_IN_MB_ERR_MASK 0x00000040U
#define ODN_INTERRUPT_CLR_GIST_IN_MB_ERR_SHIFT 6
#define ODN_INTERRUPT_CLR_GIST_IN_MB_ERR_SIGNED 0

#define ODN_INTERRUPT_CLR_GIST_OUT_LNK_ERR_MASK 0x00000080U
#define ODN_INTERRUPT_CLR_GIST_OUT_LNK_ERR_SHIFT 7
#define ODN_INTERRUPT_CLR_GIST_OUT_LNK_ERR_SIGNED 0

#define ODN_INTERRUPT_CLR_GIST_OUT_MB_ERR_MASK 0x00000100U
#define ODN_INTERRUPT_CLR_GIST_OUT_MB_ERR_SHIFT 8
#define ODN_INTERRUPT_CLR_GIST_OUT_MB_ERR_SIGNED 0

#define ODN_INTERRUPT_CLR_DUT2_MASK 0x00000200U
#define ODN_INTERRUPT_CLR_DUT2_SHIFT 9
#define PVR5__INTERRUPT_CLR_DUT2_SIGNED 0

#define ODN_INTERRUPT_CLR_AXI_LOCKUP_PROTECTION_MASK 0x00000400U
#define ODN_INTERRUPT_CLR_AXI_LOCKUP_PROTECTION_SHIFT 10
#define ODN_INTERRUPT_CLR_AXI_LOCKUP_PROTECTION_SIGNED 0

#define ODN_INTERRUPT_CLR_CDMA_MASK 0x00001800U
#define ODN_INTERRUPT_CLR_CDMA_SHIFT 11
#define ODN_INTERRUPT_CLR_CDMA_SIGNED 0

#define ODN_INTERRUPT_CLR_OS_IRQ_MASK 0x001FE000U
#define ODN_INTERRUPT_CLR_OS_IRQ_SHIFT 13
#define ODN_INTERRUPT_CLR_OS_IRQ_SIGNED 0

#define ODN_INTERRUPT_CLR_IRQ_TEST_MASK 0x40000000U
#define ODN_INTERRUPT_CLR_IRQ_TEST_SHIFT 30
#define ODN_INTERRUPT_CLR_IRQ_TEST_SIGNED 0

#define ODN_INTERRUPT_CLR_MASTER_CLEAR_MASK 0x80000000U
#define ODN_INTERRUPT_CLR_MASTER_CLEAR_SHIFT 31
#define ODN_INTERRUPT_CLR_MASTER_CLEAR_SIGNED 0

/*
	Register INTERRUPT_TEST
*/
#define ODN_CORE_INTERRUPT_TEST 0x0110
#define ODN_INTERRUPT_TEST_INTERRUPT_TEST_MASK 0x00000001U
#define ODN_INTERRUPT_TEST_INTERRUPT_TEST_SHIFT 0
#define ODN_INTERRUPT_TEST_INTERRUPT_TEST_SIGNED 0

/*
	Register INTERRUPT_TIMEOUT_CLR
*/
#define ODN_CORE_INTERRUPT_TIMEOUT_CLR 0x0114
#define ODN_INTERRUPT_TIMEOUT_CLR_INTERRUPT_MST_TIMEOUT_CLR_MASK 0x00000002U
#define ODN_INTERRUPT_TIMEOUT_CLR_INTERRUPT_MST_TIMEOUT_CLR_SHIFT 1
#define ODN_INTERRUPT_TIMEOUT_CLR_INTERRUPT_MST_TIMEOUT_CLR_SIGNED 0

#define ODN_INTERRUPT_TIMEOUT_CLR_INTERRUPT_MST_TIMEOUT_MASK 0x00000001U
#define ODN_INTERRUPT_TIMEOUT_CLR_INTERRUPT_MST_TIMEOUT_SHIFT 0
#define ODN_INTERRUPT_TIMEOUT_CLR_INTERRUPT_MST_TIMEOUT_SIGNED 0

/*
	Register INTERRUPT_TIMEOUT
*/
#define ODN_CORE_INTERRUPT_TIMEOUT 0x0118
#define ODN_INTERRUPT_TIMEOUT_INTERRUPT_TIMEOUT_THRESHOLD_COUNTER_MASK \
	0xFFFFFFFFU
#define ODN_INTERRUPT_TIMEOUT_INTERRUPT_TIMEOUT_THRESHOLD_COUNTER_SHIFT 0
#define ODN_INTERRUPT_TIMEOUT_INTERRUPT_TIMEOUT_THRESHOLD_COUNTER_SIGNED 0
/*
	Register SYSTEM_ID
*/
#define ODN_CORE_SYSTEM_ID 0x011C
#define ODN_SYSTEM_ID_ID_MASK 0x0000FFFFU
#define ODN_SYSTEM_ID_ID_SHIFT 0
#define ODN_SYSTEM_ID_ID_SIGNED 0

/*
	Register SUPPORTED_FEATURES
*/
#define ODN_CORE_SUPPORTED_FEATURES 0x0120
#define ODN_SUPPORTED_FEATURES_UNIMPLEMENTED_FREATURES_MASK 0xFFFFFFFEU
#define ODN_SUPPORTED_FEATURES_UNIMPLEMENTED_FREATURES_SHIFT 1
#define ODN_SUPPORTED_FEATURES_UNIMPLEMENTED_FREATURES_SIGNED 0

#define ODN_SUPPORTED_FEATURES_2X_CDMA_AND_IRQS_MASK 0x00000001U
#define ODN_SUPPORTED_FEATURES_2X_CDMA_AND_IRQS_SHIFT 0
#define ODN_SUPPORTED_FEATURES_2X_CDMA_AND_IRQS_SIGNED 0

/*
	Register NUM_GPIO
*/
#define ODN_CORE_NUM_GPIO 0x0180
#define ODN_NUM_GPIO_NUMBER_MASK 0x0000000FU
#define ODN_NUM_GPIO_NUMBER_SHIFT 0
#define ODN_NUM_GPIO_NUMBER_SIGNED 0

/*
	Register GPIO_EN
*/
#define ODN_CORE_GPIO_EN 0x0184
#define ODN_GPIO_EN_DIRECTION_MASK 0x000000FFU
#define ODN_GPIO_EN_DIRECTION_SHIFT 0
#define ODN_GPIO_EN_DIRECTION_SIGNED 0

/*
	Register GPIO
*/
#define ODN_CORE_GPIO 0x0188
#define ODN_GPIO_GPIO_MASK 0x000000FFU
#define ODN_GPIO_GPIO_SHIFT 0
#define ODN_GPIO_GPIO_SIGNED 0

/*
	Register NUM_DUT_CTRL
*/
#define ODN_CORE_NUM_DUT_CTRL 0x0190
#define ODN_NUM_DUT_CTRL_NUM_PINS_MASK 0xFFFFFFFFU
#define ODN_NUM_DUT_CTRL_NUM_PINS_SHIFT 0
#define ODN_NUM_DUT_CTRL_NUM_PINS_SIGNED 0

/*
	Register DUT_CTRL1
*/
#define ODN_CORE_DUT_CTRL1 0x0194
#define ODN_DUT_CTRL1_CONTROL1_MASK 0x3FFFFFFFU
#define ODN_DUT_CTRL1_CONTROL1_SHIFT 0
#define ODN_DUT_CTRL1_CONTROL1_SIGNED 0

#define ODN_DUT_CTRL1_FBDC_BYPASS_MASK 0x40000000U
#define ODN_DUT_CTRL1_FBDC_BYPASS_SHIFT 30
#define ODN_DUT_CTRL1_FBDC_BYPASS_SIGNED 0

#define ODN_DUT_CTRL1_DUT_MST_OFFSET_MASK 0x80000000U
#define ODN_DUT_CTRL1_DUT_MST_OFFSET_SHIFT 31
#define ODN_DUT_CTRL1_DUT_MST_OFFSET_SIGNED 0

/*
	Register DUT_CTRL2
*/
#define ODN_CORE_DUT_CTRL2 0x0198
#define ODN_DUT_CTRL2_CONTROL2_MASK 0xFFFFFFFFU
#define ODN_DUT_CTRL2_CONTROL2_SHIFT 0
#define ODN_DUT_CTRL2_CONTROL2_SIGNED 0

/*
	Register NUM_DUT_STAT
*/
#define ODN_CORE_NUM_DUT_STAT 0x019C
#define ODN_NUM_DUT_STAT_NUM_PINS_MASK 0xFFFFFFFFU
#define ODN_NUM_DUT_STAT_NUM_PINS_SHIFT 0
#define ODN_NUM_DUT_STAT_NUM_PINS_SIGNED 0

/*
	Register DUT_STAT1
*/
#define ODN_CORE_DUT_STAT1 0x01A0
#define ODN_DUT_STAT1_STATUS1_MASK 0xFFFFFFFFU
#define ODN_DUT_STAT1_STATUS1_SHIFT 0
#define ODN_DUT_STAT1_STATUS1_SIGNED 0

/*
	Register DUT_STAT2
*/
#define ODN_CORE_DUT_STAT2 0x01A4
#define ODN_DUT_STAT2_STATUS2_MASK 0xFFFFFFFFU
#define ODN_DUT_STAT2_STATUS2_SHIFT 0
#define ODN_DUT_STAT2_STATUS2_SIGNED 0

/*
	Register DASH_LEDS
*/
#define ODN_CORE_DASH_LEDS 0x01A8
#define ODN_DASH_LEDS_REPA_MASK 0xFFF00000U
#define ODN_DASH_LEDS_REPA_SHIFT 20
#define ODN_DASH_LEDS_REPA_SIGNED 0

#define ODN_DASH_LEDS_PIKE_MASK 0x00000FFFU
#define ODN_DASH_LEDS_PIKE_SHIFT 0
#define ODN_DASH_LEDS_PIKE_SIGNED 0

/*
	Register DUT_CLK_INFO
*/
#define ODN_CORE_DUT_CLK_INFO 0x01B0
#define ODN_DUT_CLK_INFO_CORE_MASK 0x0000FFFFU
#define ODN_DUT_CLK_INFO_CORE_SHIFT 0
#define ODN_DUT_CLK_INFO_CORE_SIGNED 0

#define ODN_DUT_CLK_INFO_MEM_MASK 0xFFFF0000U
#define ODN_DUT_CLK_INFO_MEM_SHIFT 16
#define ODN_DUT_CLK_INFO_MEM_SIGNED 0

/*
	Register DUT_CLK_PHSE
*/
#define ODN_CORE_DUT_CLK_PHSE 0x01B4
#define ODN_DUT_CLK_PHSE_MEM_REQ_MASK 0x0000FFFFU
#define ODN_DUT_CLK_PHSE_MEM_REQ_SHIFT 0
#define ODN_DUT_CLK_PHSE_MEM_REQ_SIGNED 0

#define ODN_DUT_CLK_PHSE_MEM_RD_MASK 0xFFFF0000U
#define ODN_DUT_CLK_PHSE_MEM_RD_SHIFT 16
#define ODN_DUT_CLK_PHSE_MEM_RD_SIGNED 0

/*
	Register CORE_STATUS
*/
#define ODN_CORE_CORE_STATUS 0x0200
#define ODN_CORE_STATUS_PCIE_USER_LNK_UP_MASK 0x00000001U
#define ODN_CORE_STATUS_PCIE_USER_LNK_UP_SHIFT 0
#define ODN_CORE_STATUS_PCIE_USER_LNK_UP_SIGNED 0

#define ODN_CORE_STATUS_MIG_C0_MMCM_LOCKED_MASK 0x00000010U
#define ODN_CORE_STATUS_MIG_C0_MMCM_LOCKED_SHIFT 4
#define ODN_CORE_STATUS_MIG_C0_MMCM_LOCKED_SIGNED 0

#define ODN_CORE_STATUS_MIG_C0_INIT_CALIB_COMPLETE_MASK 0x00000020U
#define ODN_CORE_STATUS_MIG_C0_INIT_CALIB_COMPLETE_SHIFT 5
#define ODN_CORE_STATUS_MIG_C0_INIT_CALIB_COMPLETE_SIGNED 0

#define ODN_CORE_STATUS_MIG_C1_MMCM_LOCKED_MASK 0x00000040U
#define ODN_CORE_STATUS_MIG_C1_MMCM_LOCKED_SHIFT 6
#define ODN_CORE_STATUS_MIG_C1_MMCM_LOCKED_SIGNED 0

#define ODN_CORE_STATUS_MIG_C1_INIT_CALIB_COMPLETE_MASK 0x00000080U
#define ODN_CORE_STATUS_MIG_C1_INIT_CALIB_COMPLETE_SHIFT 7
#define ODN_CORE_STATUS_MIG_C1_INIT_CALIB_COMPLETE_SIGNED 0

#define ODN_CORE_STATUS_PERIP_IMG2AXI_IDLE_MASK 0x00000100U
#define ODN_CORE_STATUS_PERIP_IMG2AXI_IDLE_SHIFT 8
#define ODN_CORE_STATUS_PERIP_IMG2AXI_IDLE_SIGNED 0

#define ODN_CORE_STATUS_PERIP_AXI2IMG_IDLE_MASK 0x00000200U
#define ODN_CORE_STATUS_PERIP_AXI2IMG_IDLE_SHIFT 9
#define ODN_CORE_STATUS_PERIP_AXI2IMG_IDLE_SIGNED 0

#define ODN_CORE_STATUS_GIST_SLV_C2C_CONFIG_ERROR_OUT_MASK 0x00001000U
#define ODN_CORE_STATUS_GIST_SLV_C2C_CONFIG_ERROR_OUT_SHIFT 12
#define ODN_CORE_STATUS_GIST_SLV_C2C_CONFIG_ERROR_OUT_SIGNED 0

#define ODN_CORE_STATUS_GIST_MST_C2C_CONFIG_ERROR_OUT_MASK 0x00002000U
#define ODN_CORE_STATUS_GIST_MST_C2C_CONFIG_ERROR_OUT_SHIFT 13
#define ODN_CORE_STATUS_GIST_MST_C2C_CONFIG_ERROR_OUT_SIGNED 0

/*
	Register CORE_CONTROL
*/
#define ODN_CORE_CORE_CONTROL 0x0204
#define ODN_CORE_CONTROL_BAR4_OFFSET_MASK 0x0000001FU
#define ODN_CORE_CONTROL_BAR4_OFFSET_SHIFT 0
#define ODN_CORE_CONTROL_BAR4_OFFSET_SIGNED 0

#define ODN_CORE_CONTROL_HDMI_MONITOR_OVERRIDE_MASK 0x00000300U
#define ODN_CORE_CONTROL_HDMI_MONITOR_OVERRIDE_SHIFT 8
#define ODN_CORE_CONTROL_HDMI_MONITOR_OVERRIDE_SIGNED 0

#define ODN_CORE_CONTROL_HDMI_MODULE_EN_MASK 0x00001C00U
#define ODN_CORE_CONTROL_HDMI_MODULE_EN_SHIFT 10
#define ODN_CORE_CONTROL_HDMI_MODULE_EN_SIGNED 0

#define ODN_CORE_CONTROL_MCU_COMMUNICATOR_EN_MASK 0x00002000U
#define ODN_CORE_CONTROL_MCU_COMMUNICATOR_EN_SHIFT 13
#define ODN_CORE_CONTROL_MCU_COMMUNICATOR_EN_SIGNED 0

#define ODN_CORE_CONTROL_PDP1_OFFSET_MASK 0x00070000U
#define ODN_CORE_CONTROL_PDP1_OFFSET_SHIFT 16
#define ODN_CORE_CONTROL_PDP1_OFFSET_SIGNED 0

#define ODN_CORE_CONTROL_PDP2_OFFSET_MASK 0x00700000U
#define ODN_CORE_CONTROL_PDP2_OFFSET_SHIFT 20
#define ODN_CORE_CONTROL_PDP2_OFFSET_SIGNED 0

#define ODN_CORE_CONTROL_DUT_OFFSET_MASK 0x07000000U
#define ODN_CORE_CONTROL_DUT_OFFSET_SHIFT 24
#define ODN_CORE_CONTROL_DUT_OFFSET_SIGNED 0

/*
	Register REG_BANK_STATUS
*/
#define ODN_CORE_REG_BANK_STATUS 0x0208
#define ODN_REG_BANK_STATUS_ARB_SLV_RD_TIMEOUT_MASK 0xFFFFFFFFU
#define ODN_REG_BANK_STATUS_ARB_SLV_RD_TIMEOUT_SHIFT 0
#define ODN_REG_BANK_STATUS_ARB_SLV_RD_TIMEOUT_SIGNED 0

/*
	Register MMCM_LOCK_STATUS
*/
#define ODN_CORE_MMCM_LOCK_STATUS 0x020C
#define ODN_MMCM_LOCK_STATUS_DUT_CORE_MASK 0x00000001U
#define ODN_MMCM_LOCK_STATUS_DUT_CORE_SHIFT 0
#define ODN_MMCM_LOCK_STATUS_DUT_CORE_SIGNED 0

#define ODN_MMCM_LOCK_STATUS_DUT_IF_MASK 0x00000002U
#define ODN_MMCM_LOCK_STATUS_DUT_IF_SHIFT 1
#define ODN_MMCM_LOCK_STATUS_DUT_IF_SIGNED 0

#define ODN_MMCM_LOCK_STATUS_MULTI_MASK 0x00000004U
#define ODN_MMCM_LOCK_STATUS_MULTI_SHIFT 2
#define ODN_MMCM_LOCK_STATUS_MULTI_SIGNED 0

#define ODN_MMCM_LOCK_STATUS_PDPP_MASK 0x00000008U
#define ODN_MMCM_LOCK_STATUS_PDPP_SHIFT 3
#define ODN_MMCM_LOCK_STATUS_PDPP_SIGNED 0

/*
	Register GIST_STATUS
*/
#define ODN_CORE_GIST_STATUS 0x0210
#define ODN_GIST_STATUS_MST_MASK 0x000001FFU
#define ODN_GIST_STATUS_MST_SHIFT 0
#define ODN_GIST_STATUS_MST_SIGNED 0

#define ODN_GIST_STATUS_SLV_MASK 0x001FF000U
#define ODN_GIST_STATUS_SLV_SHIFT 12
#define ODN_GIST_STATUS_SLV_SIGNED 0

#define ODN_GIST_STATUS_SLV_OUT_MASK 0x03000000U
#define ODN_GIST_STATUS_SLV_OUT_SHIFT 24
#define ODN_GIST_STATUS_SLV_OUT_SIGNED 0

#define ODN_GIST_STATUS_MST_OUT_MASK 0x70000000U
#define ODN_GIST_STATUS_MST_OUT_SHIFT 28
#define ODN_GIST_STATUS_MST_OUT_SIGNED 0

/*
	Register DUT_MST_ADD
*/
#define ODN_CORE_DUT_MST_ADD 0x0214
#define ODN_DUT_MST_ADD_SLV_OUT_MASK 0x0000003FU
#define ODN_DUT_MST_ADD_SLV_OUT_SHIFT 0
#define ODN_DUT_MST_ADD_SLV_OUT_SIGNED 0

/*
	Register DUT_MULTIPLX_INFO
*/
#define ODN_CORE_DUT_MULTIPLX_INFO 0x0218
#define ODN_DUT_MULTIPLX_INFO_MEM_MASK 0x000000FFU
#define ODN_DUT_MULTIPLX_INFO_MEM_SHIFT 0
#define ODN_DUT_MULTIPLX_INFO_MEM_SIGNED 0

/****************************
  Generated from: ad_tx.def
*****************************/

/*
	Register ADT_CONTROL
*/
#define ODN_AD_TX_DEBUG_ADT_CONTROL 0x0000
#define ODN_SET_ADTX_READY_MASK 0x00000004U
#define ODN_SET_ADTX_READY_SHIFT 2
#define ODN_SET_ADTX_READY_SIGNED 0

#define ODN_SEND_ALIGN_DATA_MASK 0x00000002U
#define ODN_SEND_ALIGN_DATA_SHIFT 1
#define ODN_SEND_ALIGN_DATA_SIGNED 0

#define ODN_ENABLE_FLUSHING_MASK 0x00000001U
#define ODN_ENABLE_FLUSHING_SHIFT 0
#define ODN_ENABLE_FLUSHING_SIGNED 0

/*
	Register ADT_STATUS
*/
#define ODN_AD_TX_DEBUG_ADT_STATUS 0x0004
#define ODN_REQUEST_COMPLETE_MASK 0x00000001U
#define ODN_REQUEST_COMPLETE_SHIFT 0
#define ODN_REQUEST_COMPLETE_SIGNED 0

/******************************
 Generated from: mca_debug.def
*******************************/

/*
	Register MCA_CONTROL
*/
#define ODN_MCA_DEBUG_MCA_CONTROL 0x0000
#define ODN_ALIGN_START_MASK 0x00000001U
#define ODN_ALIGN_START_SHIFT 0
#define ODN_ALIGN_START_SIGNED 0

/*
	Register MCA_STATUS
*/
#define ODN_MCA_DEBUG_MCA_STATUS 0x0004
#define ODN_TCHECK_SDEBUG_MASK 0x40000000U
#define ODN_TCHECK_SDEBUG_SHIFT 30
#define ODN_TCHECK_SDEBUG_SIGNED 0

#define ODN_CHECK_SDEBUG_MASK 0x20000000U
#define ODN_CHECK_SDEBUG_SHIFT 29
#define ODN_CHECK_SDEBUG_SIGNED 0

#define ODN_ALIGN_SDEBUG_MASK 0x10000000U
#define ODN_ALIGN_SDEBUG_SHIFT 28
#define ODN_ALIGN_SDEBUG_SIGNED 0

#define ODN_FWAIT_SDEBUG_MASK 0x08000000U
#define ODN_FWAIT_SDEBUG_SHIFT 27
#define ODN_FWAIT_SDEBUG_SIGNED 0

#define ODN_IDLE_SDEBUG_MASK 0x04000000U
#define ODN_IDLE_SDEBUG_SHIFT 26
#define ODN_IDLE_SDEBUG_SIGNED 0

#define ODN_FIFO_FULL_MASK 0x03FF0000U
#define ODN_FIFO_FULL_SHIFT 16
#define ODN_FIFO_FULL_SIGNED 0

#define ODN_FIFO_EMPTY_MASK 0x0000FFC0U
#define ODN_FIFO_EMPTY_SHIFT 6
#define ODN_FIFO_EMPTY_SIGNED 0

#define ODN_TAG_CHECK_ERROR_MASK 0x00000020U
#define ODN_TAG_CHECK_ERROR_SHIFT 5
#define ODN_TAG_CHECK_ERROR_SIGNED 0

#define ODN_ALIGN_CHECK_ERROR_MASK 0x00000010U
#define ODN_ALIGN_CHECK_ERROR_SHIFT 4
#define ODN_ALIGN_CHECK_ERROR_SIGNED 0

#define ODN_ALIGN_ERROR_MASK 0x00000008U
#define ODN_ALIGN_ERROR_SHIFT 3
#define ODN_ALIGN_ERROR_SIGNED 0

#define ODN_TAG_CHECKING_OK_MASK 0x00000004U
#define ODN_TAG_CHECKING_OK_SHIFT 2
#define ODN_TAG_CHECKING_OK_SIGNED 0

#define ODN_ALIGN_CHECK_OK_MASK 0x00000002U
#define ODN_ALIGN_CHECK_OK_SHIFT 1
#define ODN_ALIGN_CHECK_OK_SIGNED 0

#define ODN_ALIGNMENT_FOUND_MASK 0x00000001U
#define ODN_ALIGNMENT_FOUND_SHIFT 0
#define ODN_ALIGNMENT_FOUND_SIGNED 0

/*********************************
 Generated from: sai_rx_debug.def
**********************************/

/*
	Register SIG_RESULT
*/
#define ODN_SAI_RX_DEBUG_SIG_RESULT 0x0000
#define ODN_SIG_RESULT_VALUE_MASK 0xFFFFFFFFU
#define ODN_SIG_RESULT_VALUE_SHIFT 0
#define ODN_SIG_RESULT_VALUE_SIGNED 0

/*
	Register INIT_SIG
*/
#define ODN_SAI_RX_DEBUG_INIT_SIG 0x0004
#define ODN_INIT_SIG_VALUE_MASK 0x00000001U
#define ODN_INIT_SIG_VALUE_SHIFT 0
#define ODN_INIT_SIG_VALUE_SIGNED 0

/*
	Register SAI_BYPASS
*/
#define ODN_SAI_RX_DEBUG_SAI_BYPASS 0x0008
#define ODN_BYPASS_CLK_TAPS_VALUE_MASK 0x000003FFU
#define ODN_BYPASS_CLK_TAPS_VALUE_SHIFT 0
#define ODN_BYPASS_CLK_TAPS_VALUE_SIGNED 0

#define ODN_BYPASS_SET_MASK 0x00010000U
#define ODN_BYPASS_SET_SHIFT 16
#define ODN_BYPASS_SET_SIGNED 0

#define ODN_BYPASS_EN_MASK 0x00100000U
#define ODN_BYPASS_EN_SHIFT 20
#define ODN_BYPASS_EN_SIGNED 0

#define ODN_EN_STATUS_MASK 0x01000000U
#define ODN_EN_STATUS_SHIFT 24
#define ODN_EN_STATUS_SIGNED 0

/*
	Register SAI_CLK_TAPS
*/
#define ODN_SAI_RX_DEBUG_SAI_CLK_TAPS 0x000C
#define ODN_CLK_TAPS_VALUE_MASK 0x000003FFU
#define ODN_CLK_TAPS_VALUE_SHIFT 0
#define ODN_CLK_TAPS_VALUE_SIGNED 0

#define ODN_TRAINING_COMPLETE_MASK 0x00010000U
#define ODN_TRAINING_COMPLETE_SHIFT 16
#define ODN_TRAINING_COMPLETE_SIGNED 0

/*
	Register SAI_EYES
*/
#define ODN_SAI_RX_DEBUG_SAI_EYES 0x0010
#define ODN_MIN_EYE_END_MASK 0x0000FFFFU
#define ODN_MIN_EYE_END_SHIFT 0
#define ODN_MIN_EYE_END_SIGNED 0

#define ODN_MAX_EYE_START_MASK 0xFFFF0000U
#define ODN_MAX_EYE_START_SHIFT 16
#define ODN_MAX_EYE_START_SIGNED 0

/*
	Register SAI_DDR_INVERT
*/
#define ODN_SAI_RX_DEBUG_SAI_DDR_INVERT 0x0014
#define ODN_DDR_INVERT_MASK 0x00000001U
#define ODN_DDR_INVERT_SHIFT 0
#define ODN_DDR_INVERT_SIGNED 0

#define ODN_OVERIDE_VALUE_MASK 0x00010000U
#define ODN_OVERIDE_VALUE_SHIFT 16
#define ODN_OVERIDE_VALUE_SIGNED 0

#define ODN_INVERT_OVERIDE_MASK 0x00100000U
#define ODN_INVERT_OVERIDE_SHIFT 20
#define ODN_INVERT_OVERIDE_SIGNED 0

/*
	Register SAI_TRAIN_ACK
*/
#define ODN_SAI_RX_DEBUG_SAI_TRAIN_ACK 0x0018
#define ODN_TRAIN_ACK_FAIL_MASK 0x00000001U
#define ODN_TRAIN_ACK_FAIL_SHIFT 0
#define ODN_TRAIN_ACK_FAIL_SIGNED 0

#define ODN_TRAIN_ACK_FAIL_COUNT_MASK 0x000000F0U
#define ODN_TRAIN_ACK_FAIL_COUNT_SHIFT 4
#define ODN_TRAIN_ACK_FAIL_COUNT_SIGNED 0

#define ODN_TRAIN_ACK_COMPLETE_MASK 0x00000100U
#define ODN_TRAIN_ACK_COMPLETE_SHIFT 8
#define ODN_TRAIN_ACK_COMPLETE_SIGNED 0

#define ODN_TRAIN_ACK_OVERIDE_MASK 0x00001000U
#define ODN_TRAIN_ACK_OVERIDE_SHIFT 12
#define ODN_TRAIN_ACK_OVERIDE_SIGNED 0

/*
	Register SAI_TRAIN_ACK_COUNT
*/
#define ODN_SAI_RX_DEBUG_SAI_TRAIN_ACK_COUNT 0x001C
#define ODN_TRAIN_COUNT_MASK 0xFFFFFFFFU
#define ODN_TRAIN_COUNT_SHIFT 0
#define ODN_TRAIN_COUNT_SIGNED 0

/*
	Register SAI_CHANNEL_NUMBER
*/
#define ODN_SAI_RX_DEBUG_SAI_CHANNEL_NUMBER 0x0020
#define ODN_CHANNEL_NUMBER_MASK 0x0000FFFFU
#define ODN_CHANNEL_NUMBER_SHIFT 0
#define ODN_CHANNEL_NUMBER_SIGNED 0

/*
	Register SAI_CHANNEL_EYE_START
*/
#define ODN_SAI_RX_DEBUG_SAI_CHANNEL_EYE_START 0x0024
#define ODN_CHANNEL_EYE_START_MASK 0xFFFFFFFFU
#define ODN_CHANNEL_EYE_START_SHIFT 0
#define ODN_CHANNEL_EYE_START_SIGNED 0

/*
	Register SAI_CHANNEL_EYE_END
*/
#define ODN_SAI_RX_DEBUG_SAI_CHANNEL_EYE_END 0x0028
#define ODN_CHANNEL_EYE_END_MASK 0xFFFFFFFFU
#define ODN_CHANNEL_EYE_END_SHIFT 0
#define ODN_CHANNEL_EYE_END_SIGNED 0

/*
	Register SAI_CHANNEL_EYE_PATTERN
*/
#define ODN_SAI_RX_DEBUG_SAI_CHANNEL_EYE_PATTERN 0x002C
#define ODN_CHANNEL_EYE_PATTERN_MASK 0xFFFFFFFFU
#define ODN_CHANNEL_EYE_PATTERN_SHIFT 0
#define ODN_CHANNEL_EYE_PATTERN_SIGNED 0

/*
	Register SAI_CHANNEL_EYE_DEBUG
*/
#define ODN_SAI_RX_DEBUG_SAI_CHANNEL_EYE_DEBUG 0x0030
#define ODN_CHANNEL_EYE_SENSE_MASK 0x00000001U
#define ODN_CHANNEL_EYE_SENSE_SHIFT 0
#define ODN_CHANNEL_EYE_SENSE_SIGNED 0

#define ODN_CHANNEL_EYE_COMPLETE_MASK 0x00000002U
#define ODN_CHANNEL_EYE_COMPLETE_SHIFT 1
#define ODN_CHANNEL_EYE_COMPLETE_SIGNED 0

/*********************************
 Generated from: sai_tx_debug.def
**********************************/

/*
	Register SIG_RESULT
*/
#define ODN_SAI_TX_DEBUG_SIG_RESULT 0x0000
#define ODN_TX_SIG_RESULT_VALUE_MASK 0xFFFFFFFFU
#define ODN_TX_SIG_RESULT_VALUE_SHIFT 0
#define ODN_TX_SIG_RESULT_VALUE_SIGNED 0

/*
	Register INIT_SIG
*/
#define ODN_SAI_TX_DEBUG_INIT_SIG 0x0004
#define ODN_TX_INIT_SIG_VALUE_MASK 0x00000001U
#define ODN_TX_INIT_SIG_VALUE_SHIFT 0
#define ODN_TX_INIT_SIG_VALUE_SIGNED 0

/*
	Register SAI_BYPASS
*/
#define ODN_SAI_TX_DEBUG_SAI_BYPASS 0x0008
#define ODN_TX_BYPASS_EN_MASK 0x00000001U
#define ODN_TX_BYPASS_EN_SHIFT 0
#define ODN_TX_BYPASS_EN_SIGNED 0

#define ODN_TX_ACK_RESEND_MASK 0x00000002U
#define ODN_TX_ACK_RESEND_SHIFT 1
#define ODN_TX_ACK_RESEND_SIGNED 0

#define ODN_TX_DISABLE_ACK_SEND_MASK 0x00000004U
#define ODN_TX_DISABLE_ACK_SEND_SHIFT 2
#define ODN_TX_DISABLE_ACK_SEND_SIGNED 0

/*
	Register SAI_STATUS
*/
#define ODN_SAI_TX_DEBUG_SAI_STATUS 0x000C
#define ODN_TX_TRAINING_COMPLETE_MASK 0x00000001U
#define ODN_TX_TRAINING_COMPLETE_SHIFT 0
#define ODN_TX_TRAINING_COMPLETE_SIGNED 0

#define ODN_TX_TRAINING_ACK_COMPLETE_MASK 0x00000002U
#define ODN_TX_TRAINING_ACK_COMPLETE_SHIFT 1
#define ODN_TX_TRAINING_ACK_COMPLETE_SIGNED 0

#endif /* _ODIN_REGS_H_ */

/******************************************************************************
 End of file (odin_regs.h)
******************************************************************************/
