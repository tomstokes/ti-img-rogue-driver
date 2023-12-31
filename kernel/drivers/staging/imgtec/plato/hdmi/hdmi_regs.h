/*************************************************************************/ /*!
@Copyright      Copyright (c) Imagination Technologies Ltd. All Rights Reserved
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

#ifndef _HDMI_REGS_H
#define _HDMI_REGS_H

#define SET_FIELD(start, mask, value) ((value << start) & mask)
#define GET_FIELD(value, start, mask) ((value & mask) >> start)

/* Hardware Register definitions
 * NOTE: These are HDMI core address offsets, SoC level offset will be * 4
 */

/* Identification registers */
#define HDMI_ID_DESIGN_ID_OFFSET (0x0000)

#define HDMI_ID_REVISION_ID_OFFSET (0x0001)

#define HDMI_ID_PRODUCT_ID0_OFFSET (0x0002)

#define HDMI_ID_PRODUCT_ID1_OFFSET (0x0003)
#define HDMI_ID_PRODUCT_ID1_TX_START (0)
#define HDMI_ID_PRODUCT_ID1_TX_MASK (0x1)
#define HDMI_ID_PRODUCT_ID1_RX_START (1)
#define HDMI_ID_PRODUCT_ID1_RX_MASK (0x2)
#define HDMI_ID_PRODUCT_ID1_HDCP_START (6)
#define HDMI_ID_PRODUCT_ID1_HDCP_MASK (0xC0)

#define HDMI_ID_CONFIG0_ID_OFFSET (0x0004)
#define HDMI_ID_CONFIG0_ID_HDCP_START (0)
#define HDMI_ID_CONFIG0_ID_HDCP_MASK (0x1)
#define HDMI_ID_CONFIG0_ID_CEC_START (1)
#define HDMI_ID_CONFIG0_ID_CEC_MASK (0x2)
#define HDMI_ID_CONFIG0_ID_CSC_START (2)
#define HDMI_ID_CONFIG0_ID_CSC_MASK (0x4)
#define HDMI_ID_CONFIG0_ID_HDMI14_START (3)
#define HDMI_ID_CONFIG0_ID_HDMI14_MASK (0x8)
#define HDMI_ID_CONFIG0_ID_AUDI2S_START (4)
#define HDMI_ID_CONFIG0_ID_AUDI2S_MASK (0x10)
#define HDMI_ID_CONFIG0_ID_AUDSPDIF_START (5)
#define HDMI_ID_CONFIG0_ID_AUDSPDIF_MASK (0x20)
#define HDMI_ID_CONFIG0_ID_PREPEN_START (7)
#define HDMI_ID_CONFIG0_ID_PREPEN_MASK (0x80)

#define HDMI_ID_CONFIG1_ID_OFFSET (0x0005)
#define HDMI_ID_CONFIG1_ID_CONFAPB_START (1)
#define HDMI_ID_CONFIG1_ID_CONFAPB_MASK (0x2)
#define HDMI_ID_CONFIG1_ID_HDMI20_START (5)
#define HDMI_ID_CONFIG1_ID_HDMI20_MASK (0x20)
#define HDMI_ID_CONFIG1_ID_HDCP22_START (6)
#define HDMI_ID_CONFIG1_ID_HDCP22_MASK (0x40)

#define HDMI_ID_CONFIG2_ID_OFFSET (0x0006)

#define HDMI_ID_CONFIG3_ID_OFFSET (0x0007)

/* Interrupt Registers */
#define HDMI_IH_FC_STAT0_OFFSET (0x0100)

#define HDMI_IH_FC_STAT1_OFFSET (0x0101)

#define HDMI_IH_FC_STAT2_OFFSET (0x0102)

#define HDMI_IH_AS_STAT0_OFFSET (0x0103)

#define HDMI_IH_PHY_STAT0_OFFSET (0x0104)
#define HDMI_IH_PHY_STAT0_HPD_START (0)
#define HDMI_IH_PHY_STAT0_HPD_MASK (0x1)

#define HDMI_IH_I2CM_STAT0_OFFSET (0x0105)
#define HDMI_IH_I2CM_STAT0_I2CMASTERERROR_START (0)
#define HDMI_IH_I2CM_STAT0_I2CMASTERERROR_MASK (0x1)
#define HDMI_IH_I2CM_STAT0_I2CMASTERDONE_START (1)
#define HDMI_IH_I2CM_STAT0_I2CMASTERDONE_MASK (0x2)
#define HDMI_IH_I2CM_STAT0_SCDC_READREQ_START (2)
#define HDMI_IH_I2CM_STAT0_SCDC_READREQ_MASK (0x4)

#define HDMI_IH_CEC_STAT0_OFFSET (0x0106)

#define HDMI_IH_VP_STAT0_OFFSET (0x0107)

#define HDMI_IH_I2CMPHY_STAT0_OFFSET (0x0108)
#define HDMI_IH_I2CMPHY_STAT0_I2CMPHYERROR_START (0)
#define HDMI_IH_I2CMPHY_STAT0_I2CMPHYERROR_MASK (0x1)
#define HDMI_IH_I2CMPHY_STAT0_I2CMPHYDONE_START (1)
#define HDMI_IH_I2CMPHY_STAT0_I2CMPHYDONE_MASK (0x2)

#define HDMI_IH_AHBDMAAUD_STAT0_OFFSET (0x0109)

#define HDMI_IH_DECODE_OFFSET (0x0170)
#define HDMI_IH_DECODE_PHY_START (3)
#define HDMI_IH_DECODE_PHY_MASK (0x8)

#define HDMI_IH_MUTE_FC_STAT0_OFFSET (0x0180)

#define HDMI_IH_MUTE_FC_STAT1_OFFSET (0x0181)

#define HDMI_IH_MUTE_FC_STAT2_OFFSET (0x0182)

#define HDMI_IH_MUTE_AS_STAT0_OFFSET (0x0183)

#define HDMI_IH_MUTE_PHY_STAT0_OFFSET (0x0184)

#define HDMI_IH_MUTE_I2CM_STAT0_OFFSET (0x0185)

#define HDMI_IH_MUTE_CEC_STAT0_OFFSET (0x0186)

#define HDMI_IH_MUTE_VP_STAT0_OFFSET (0x0187)

#define HDMI_IH_MUTE_I2CMPHY_STAT0_OFFSET (0x0188)

#define HDMI_IH_MUTE_AHBDMAAUD_STAT0_OFFSET (0x0189)

#define HDMI_IH_MUTE_OFFSET (0x01ff)
#define HDMI_IH_MUTE_ALL_START (0)
#define HDMI_IH_MUTE_ALL_MASK (0x3)

/* Video Sampler registers */
#define HDMI_TX_INVID0_OFFSET (0x0200)
#define HDMI_TX_INVID0_VIDEO_MAPPING_START (0)
#define HDMI_TX_INVID0_VIDEO_MAPPING_MASK (0x1F)
#define HDMI_TX_INVID0_INTERNAL_DE_GEN_START (7)
#define HDMI_TX_INVID0_INTERNAL_DE_GEN_MASK (0x80)

#define HDMI_TX_INSTUFFING_OFFSET (0x201)

#define HDMI_TX_GYDATA0_OFFSET (0x202)

#define HDMI_TX_GYDATA1_OFFSET (0x203)

#define HDMI_TX_RCRDATA0_OFFSET (0x204)

#define HDMI_TX_RCRDATA1_OFFSET (0x205)

#define HDMI_TX_BCBDATA0_OFFSET (0x206)

#define HDMI_TX_BCBDATA1_OFFSET (0x207)

/* Video Packetizer */
#define HDMI_VP_STATUS_OFFSET (0x0800)

#define HDMI_VP_PR_CD_OFFSET (0x0801)
#define HDMI_VP_PR_CD_DESIRED_PR_FACTOR_START (0)
#define HDMI_VP_PR_CD_DESIRED_PR_FACTOR_MASK (0xF)
#define HDMI_VP_PR_CD_COLOR_DEPTH_START (4)
#define HDMI_VP_PR_CD_COLOR_DEPTH_MASK (0xF0)

#define HDMI_VP_STUFF_OFFSET (0x0802)
#define HDMI_VP_STUFF_PR_STUFFING_START (0)
#define HDMI_VP_STUFF_PR_STUFFING_MASK (0x1)
#define HDMI_VP_STUFF_PP_STUFFING_START (1)
#define HDMI_VP_STUFF_PP_STUFFING_MASK (0x2)
#define HDMI_VP_STUFF_YCC422_STUFFING_START (2)
#define HDMI_VP_STUFF_YCC422_STUFFING_MASK (0x4)
#define HDMI_VP_STUFF_ICX_GOTO_P0_ST_START (3)
#define HDMI_VP_STUFF_ICX_GOTO_P0_ST_MASK (0x8)
#define HDMI_VP_STUFF_IFIX_PP_TO_LAST_START (4)
#define HDMI_VP_STUFF_IFIX_PP_TO_LAST_MASK (0x10)
#define HDMI_VP_STUFF_IDEFAULT_PHASE_START (5)
#define HDMI_VP_STUFF_IDEFAULT_PHASE_MASK (0x20)

#define HDMI_VP_REMAP_OFFSET (0x0803)
#define HDMI_VP_REMAP_YCC422_SIZE_START (0)
#define HDMI_VP_REMAP_YCC422_SIZE_MASK (0x3)

#define HDMI_VP_CONF_OFFSET (0x0804)
#define HDMI_VP_CONF_OUTPUT_SELECTOR_START (0)
#define HDMI_VP_CONF_OUTPUT_SELECTOR_MASK (0x3)
#define HDMI_VP_CONF_BYPASS_SELECT_START (2)
#define HDMI_VP_CONF_BYPASS_SELECT_MASK (0x4)
#define HDMI_VP_CONF_YCC422_EN_START (3)
#define HDMI_VP_CONF_YCC422_EN_MASK (0x8)
#define HDMI_VP_CONF_PR_EN_START (4)
#define HDMI_VP_CONF_PR_EN_MASK (0x10)
#define HDMI_VP_CONF_PP_EN_START (5)
#define HDMI_VP_CONF_PP_EN_MASK (0x20)
#define HDMI_VP_CONF_BYPASS_EN_START (6)
#define HDMI_VP_CONF_BYPASS_EN_MASK (0x40)

#define HDMI_VP_MASK_OFFSET (0x0807)

/* Frame Composer */
#define HDMI_FC_INVIDCONF_OFFSET (0x1000)
#define HDMI_FC_INVIDCONF_IN_I_P_START (0)
#define HDMI_FC_INVIDCONF_IN_I_P_MASK (0x1)
#define HDMI_FC_INVIDCONF_R_V_BLANK_IN_OSC_START (1)
#define HDMI_FC_INVIDCONF_R_V_BLANK_IN_OSC_MASK (0x2)
#define HDMI_FC_INVIDCONF_DVI_MODEZ_START (3)
#define HDMI_FC_INVIDCONF_DVI_MODEZ_MASK (0x8)
#define HDMI_FC_INVIDCONF_DE_IN_POLARITY_START (4)
#define HDMI_FC_INVIDCONF_DE_IN_POLARITY_MASK (0x10)
#define HDMI_FC_INVIDCONF_HSYNC_IN_POLARITY_START (5)
#define HDMI_FC_INVIDCONF_HSYNC_IN_POLARITY_MASK (0x20)
#define HDMI_FC_INVIDCONF_VSYNC_IN_POLARITY_START (6)
#define HDMI_FC_INVIDCONF_VSYNC_IN_POLARITY_MASK (0x40)
#define HDMI_FC_INVIDCONF_HDCP_KEEPOUT_START (7)
#define HDMI_FC_INVIDCONF_HDCP_KEEPOUT_MASK (0x80)

#define HDMI_FC_INHACTIV0_OFFSET (0x1001)
#define HDMI_FC_INHACTIV0_H_IN_ACTIV_START (0)
#define HDMI_FC_INHACTIV0_H_IN_ACTIV_MASK (0xFF)

#define HDMI_FC_INHACTIV1_OFFSET (0x1002)
#define HDMI_FC_INHACTIV1_H_IN_ACTIV_UPPER_START (0)
#define HDMI_FC_INHACTIV1_H_IN_ACTIV_UPPER_MASK (0x3F)

#define HDMI_FC_INHBLANK0_OFFSET (0x1003)
#define HDMI_FC_INHBLANK0_H_IN_BLANK_START (0)
#define HDMI_FC_INHBLANK0_H_IN_BLANK_MASK (0xFF)

#define HDMI_FC_INHBLANK1_OFFSET (0x1004)
#define HDMI_FC_INHBLANK1_H_IN_BLANK_UPPER_START (0)
#define HDMI_FC_INHBLANK1_H_IN_BLANK_UPPER_MASK (0x3F)

#define HDMI_FC_INVACTIV0_OFFSET (0x1005)
#define HDMI_FC_INVACTIV0_V_IN_ACTIV_START (0)
#define HDMI_FC_INVACTIV0_V_IN_ACTIV_MASK (0xFF)

#define HDMI_FC_INVACTIV1_OFFSET (0x1006)
#define HDMI_FC_INVACTIV1_V_IN_ACTIV_UPPER_START (0)
#define HDMI_FC_INVACTIV1_V_IN_ACTIV_UPPER_MASK (0x3F)

#define HDMI_FC_INVBLANK_OFFSET (0x1007)
#define HDMI_FC_INVBLANK_V_IN_BLANK_START (0)
#define HDMI_FC_INVBLANK_V_IN_BLANK_MASK (0xFF)

#define HDMI_FC_HSYNCINDELAY0_OFFSET (0x1008)
#define HDMI_FC_HSYNCINDELAY0_H_IN_DELAY_START (0)
#define HDMI_FC_HSYNCINDELAY0_H_IN_DELAY_MASK (0xFF)

#define HDMI_FC_HSYNCINDELAY1_OFFSET (0x1009)
#define HDMI_FC_HSYNCINDELAY1_H_IN_DELAY_UPPER_START (0)
#define HDMI_FC_HSYNCINDELAY1_H_IN_DELAY_UPPER_MASK (0x3F)

#define HDMI_FC_HSYNCINWIDTH0_OFFSET (0x100a)
#define HDMI_FC_HSYNCINWIDTH0_H_IN_WIDTH_START (0)
#define HDMI_FC_HSYNCINWIDTH0_H_IN_WIDTH_MASK (0xFF)

#define HDMI_FC_HSYNCINWIDTH1_OFFSET (0x100b)
#define HDMI_FC_HSYNCINWIDTH1_H_IN_WIDTH_UPPER_START (0)
#define HDMI_FC_HSYNCINWIDTH1_H_IN_WIDTH_UPPER_MASK (0x3)

#define HDMI_FC_VSYNCINDELAY_OFFSET (0x100c)
#define HDMI_FC_VSYNCINDELAY_V_IN_DELAY_START (0)
#define HDMI_FC_VSYNCINDELAY_V_IN_DELAY_MASK (0xFF)

#define HDMI_FC_VSYNCINWIDTH_OFFSET (0x100d)
#define HDMI_FC_VSYNCINWIDTH_V_IN_WIDTH_START (0)
#define HDMI_FC_VSYNCINWIDTH_V_IN_WIDTH_MASK (0x3F)

#define HDMI_FC_INFREQ0_OFFSET (0x100e)

#define HDMI_FC_INFREQ1_OFFSET (0x100f)

#define HDMI_FC_INFREQ2_OFFSET (0x1010)

#define HDMI_FC_CTRLDUR_OFFSET (0x1011)

#define HDMI_FC_EXCTRLDUR_OFFSET (0x1012)

#define HDMI_FC_EXCTRLSPAC_OFFSET (0x1013)

#define HDMI_FC_CH0PREAM_OFFSET (0x1014)

#define HDMI_FC_CH1PREAM_OFFSET (0x1015)

#define HDMI_FC_CH2PREAM_OFFSET (0x1016)

#define HDMI_FC_AVICONF3_OFFSET (0x1017)

#define HDMI_FC_GCP_OFFSET (0x1018)

#define HDMI_FC_AVICONF0_OFFSET (0x1019)
#define HDMI_FC_AVICONF0_RGBYCC_START (0)
#define HDMI_FC_AVICONF0_RGBYCC_MASK (0x3)
#define HDMI_FC_AVICONF0_VBAR_VALID_START (2)
#define HDMI_FC_AVICONF0_VBAR_VALID_MASK (0x4)
#define HDMI_FC_AVICONF0_HBAR_VALID_START (3)
#define HDMI_FC_AVICONF0_HBAR_VALID_MASK (0x8)
#define HDMI_FC_AVICONF0_SCAN_INFO_START (4)
#define HDMI_FC_AVICONF0_SCAN_INFO_MASK (0x30)
#define HDMI_FC_AVICONF0_ACTIVE_FORMAT_PRESENT_START (6)
#define HDMI_FC_AVICONF0_ACTIVE_FORMAT_PRESENT_MASK (0x40)
#define HDMI_FC_AVICONF0_RGBYCC_2_START (7)
#define HDMI_FC_AVICONF0_RGBYCC_2_MASK (0x80)

#define HDMI_FC_AVICONF1_OFFSET (0x101a)
#define HDMI_FC_AVICONF1_ACTIVE_ASPECT_RATIO_START (0)
#define HDMI_FC_AVICONF1_ACTIVE_ASPECT_RATIO_MASK (0xF)
#define HDMI_FC_AVICONF1_PIC_ASPECT_RATIO_START (4)
#define HDMI_FC_AVICONF1_PIC_ASPECT_RATIO_MASK (0x30)
#define HDMI_FC_AVICONF1_COLORIMETRY_START (6)
#define HDMI_FC_AVICONF1_COLORIMETRY_MASK (0xC0)

#define HDMI_FC_AVICONF2_OFFSET (0x101b)
#define HDMI_FC_AVICONF2_NON_UNIFORM_PIC_SCALING_START (0)
#define HDMI_FC_AVICONF2_NON_UNIFORM_PIC_SCALING_MASK (0x3)
#define HDMI_FC_AVICONF2_QUANTIZATION_RANGE_START (2)
#define HDMI_FC_AVICONF2_QUANTIZATION_RANGE_MASK (0xC)
#define HDMI_FC_AVICONF2_EXT_COLORIMETRY_START (4)
#define HDMI_FC_AVICONF2_EXT_COLORIMETRY_MASK (0x70)
#define HDMI_FC_AVICONF2_IT_CONTENT_START (7)
#define HDMI_FC_AVICONF2_IT_CONTENT_MASK (0x80)

#define HDMI_FC_AVIVID_OFFSET (0x101c)
#define HDMI_FC_AVIVID_START (0)
#define HDMI_FC_AVIVID_MASK (0xFF)

#define HDMI_FC_AVIETB0_OFFSET (0x101d)

#define HDMI_FC_AVIETB1_OFFSET (0x101e)

#define HDMI_FC_AVISBB0_OFFSET (0x101f)

#define HDMI_FC_AVISBB1_OFFSET (0x1020)

#define HDMI_FC_AVIELB0_OFFSET (0x1021)

#define HDMI_FC_AVIELB1_OFFSET (0x1022)

#define HDMI_FC_AVISRB0_OFFSET (0x1023)

#define HDMI_FC_AVISRB1_OFFSET (0x1024)

/* A lot of registers related to audio, ACP, ISRC, etc */

#define HDMI_FC_PRCONF_OFFSET (0x10e0)
#define HDMI_FC_PRCONF_OUTPUT_PR_FACTOR_START (0)
#define HDMI_FC_PRCONF_OUTPUT_PR_FACTOR_MASK (0xF)
#define HDMI_FC_PRCONF_INCOMING_PR_FACTOR_START (4)
#define HDMI_FC_PRCONF_INCOMING_PR_FACTOR_MASK (0xF0)

#define HDMI_FC_SCRAMBLER_CTRL_OFFSET (0x10e1)

#define HDMI_FC_MULTISTREAM_CTRL_OFFSET (0x10e2)

#define HDMI_FC_PACKET_TX_EN_OFFSET (0x10e3)

#define HDMI_FC_DBGFORCE_OFFSET (0x1200)
#define HDMI_FC_DBGFORCE_FORCE_VIDEO_START (0)
#define HDMI_FC_DBGFORCE_FORCE_VIDEO_MASK (0x1)
#define HDMI_FC_DBGFORCE_FORCE_AUDIO_START (4)
#define HDMI_FC_DBGFORCE_FORCE_AUDIO_MASK (0x10)

#define HDMI_FC_DBGTMDS_0_OFFSET (0x1219)

#define HDMI_FC_DBGTMDS_1_OFFSET (0x121A)

#define HDMI_FC_DBGTMDS_2_OFFSET (0x121B)

/* HDMI Source PHY */
#define HDMI_PHY_CONF0_OFFSET (0x3000)
#define HDMI_PHY_CONF0_SELDIPIF_START (0)
#define HDMI_PHY_CONF0_SELDIPIF_MASK (0x1)
#define HDMI_PHY_CONF0_SELDATAENPOL_START (1)
#define HDMI_PHY_CONF0_SELDATAENPOL_MASK (0x2)
#define HDMI_PHY_CONF0_ENHPDRXSENSE_START (2)
#define HDMI_PHY_CONF0_ENHPDRXSENSE_MASK (0x4)
#define HDMI_PHY_CONF0_TXPWRON_START (3)
#define HDMI_PHY_CONF0_TXPWRON_MASK (0x8)
#define HDMI_PHY_CONF0_PDDQ_START (4)
#define HDMI_PHY_CONF0_PDDQ_MASK (0x10)
#define HDMI_PHY_CONF0_SVSRET_START (5)
#define HDMI_PHY_CONF0_SVSRET_MASK (0x20)
#define HDMI_PHY_CONF0_ENTMDS_START (6)
#define HDMI_PHY_CONF0_ENTMDS_MASK (0x40)
#define HDMI_PHY_CONF0_PDZ_START (7)
#define HDMI_PHY_CONF0_PDZ_MASK (0x80)

#define HDMI_PHY_TST0_OFFSET (0x3001)

#define HDMI_PHY_TST1_OFFSET (0x3002)

#define HDMI_PHY_TST2_OFFSET (0x3003)

#define HDMI_PHY_STAT0_OFFSET (0x3004)
#define HDMI_PHY_STAT0_TX_PHY_LOCK_START (0)
#define HDMI_PHY_STAT0_TX_PHY_LOCK_MASK (0x1)
#define HDMI_PHY_STAT0_HPD_START (1)
#define HDMI_PHY_STAT0_HPD_MASK (0x2)

#define HDMI_PHY_INT0_OFFSET (0x3005)

#define HDMI_PHY_MASK0_OFFSET (0x3006)

#define HDMI_PHY_POL0_OFFSET (0x3007)
#define HDMI_PHY_POL0_HPD_START (1)
#define HDMI_PHY_POL0_HPD_MASK (0x2)

/* I2C Master PHY */
#define HDMI_PHY_I2CM_SLAVE_OFFSET (0x3020)
#define HDMI_PHY_I2CM_SLAVE_SLAVEADDR_START (0)
#define HDMI_PHY_I2CM_SLAVE_SLAVEADDR_MASK (0x7F)

#define HDMI_PHY_I2CM_ADDRESS_OFFSET (0x3021)
#define HDMI_PHY_I2CM_ADDRESS_START (0)
#define HDMI_PHY_I2CM_ADDRESS_MASK (0xFF)

#define HDMI_PHY_I2CM_DATAO_1_OFFSET (0x3022)
#define HDMI_PHY_I2CM_DATAO_1_START (0)
#define HDMI_PHY_I2CM_DATAO_1_MASK (0xFF)

#define HDMI_PHY_I2CM_DATAO_0_OFFSET (0x3023)
#define HDMI_PHY_I2CM_DATAO_0_START (0)
#define HDMI_PHY_I2CM_DATAO_0_MASK (0xFF)

#define HDMI_PHY_I2CM_DATAI_1_OFFSET (0x3024)
#define HDMI_PHY_I2CM_DATAI_1_DATAI_START (0)
#define HDMI_PHY_I2CM_DATAI_1_DATAI_MASK (0xFF)

#define HDMI_PHY_I2CM_DATAI_0_OFFSET (0x3025)
#define HDMI_PHY_I2CM_DATAI_0_DATAI_START (0)
#define HDMI_PHY_I2CM_DATAI_0_DATAI_MASK (0xFF)

#define HDMI_PHY_I2CM_OPERATION_OFFSET (0x3026)
#define HDMI_PHY_I2CM_OPERATION_RD_START (0)
#define HDMI_PHY_I2CM_OPERATION_RD_MASK (0x1)
#define HDMI_PHY_I2CM_OPERATION_WR_START (4)
#define HDMI_PHY_I2CM_OPERATION_WR_MASK (0x10)
// Many more...

/* Audio Sampler */
#define HDMI_AUDIO_SAMPLER_OFFSET (0x3100)

/* Audio packetizer */
#define HDMI_AUD_N1_OFFSET (0x3200)

#define HDMI_AUD_N2_OFFSET (0x3201)

#define HDMI_AUD_N3_OFFSET (0x3202)

#define HDMI_AUD_CTS1_OFFSET (0x3203)

#define HDMI_AUD_CTS2_OFFSET (0x3204)

#define HDMI_AUD_CTS3_OFFSET (0x3205)

#define HDMI_AUD_INPUTCLKFS_OFFSET (0x3206)

/* Generic parallel audio interface */
#define HDMI_PARALLEL_AUDIO_INTF_OFFFSET (0x3500)

/* Audio DMA */
#define HDMI_AUDIO_DMA_OFFSET (0x3600)

/* Main Controller Registers */
#define HDMI_MC_CLKDIS_OFFSET (0x4001)
#define HDMI_MC_CLKDIS_PIXELCLK_DIS_START (0)
#define HDMI_MC_CLKDIS_PIXELCLK_DIS_MASK (0x1)
#define HDMI_MC_CLKDIS_TMDSCLK_DIS_START (1)
#define HDMI_MC_CLKDIS_TMDSCLK_DIS_MASK (0x2)
#define HDMI_MC_CLKDIS_PREPCLK_DIS_START (2)
#define HDMI_MC_CLKDIS_PREPCLK_DIS_MASK (0x4)
#define HDMI_MC_CLKDIS_AUDCLK_DIS_START (3)
#define HDMI_MC_CLKDIS_AUDCLK_DIS_MASK (0x8)
#define HDMI_MC_CLKDIS_CSCCLK_DIS_START (4)
#define HDMI_MC_CLKDIS_CSCCLK_DIS_MASK (0x10)
#define HDMI_MC_CLKDIS_CECCLK_DIS_START (5)
#define HDMI_MC_CLKDIS_CECCLK_DIS_MASK (0x20)
#define HDMI_MC_CLKDIS_HDCPCLK_DIS_START (6)
#define HDMI_MC_CLKDIS_HDCPCLK_DIS_MASK (0x40)

#define HDMI_MC_SWRSTZREQ_OFFSET (0x4002)
#define HDMI_MC_OPCTRL_OFFSET (0x4003)
#define HDMI_MC_FLOWCTRL_OFFSET (0x4004)

#define HDMI_MC_PHYRSTZ_OFFSET (0x4005)
#define HDMI_MC_PHYRSTZ_PHYRSTZ_START (0)
#define HDMI_MC_PHYRSTZ_PHYRSTZ_MASK (0x1)

#define HDMI_MC_LOCKONCLOCK_OFFSET (0x4006)
#define HDMI_MC_HEACPHY_RST_OFFSET (0x4007)
#define HDMI_MC_LOCKONCLOCK_2_OFFSET (0x4008)
#define HDMI_MC_SWRSTZREQ_2_OFFSET (0x4009)

/* Color Space Converter */
#define HDMI_COLOR_SPACE_CONVERTER_OFFSET (0x4100)

/* HDCP Encryption */
#define HDMI_HDCP_ENCRYPTION_OFFSET (0x5000)

/* I2C Master (E-EDID/EDDC) */
#define HDMI_I2CM_SLAVE_OFFSET (0x7e00)

#define HDMI_I2CM_ADDRESS_OFFSET (0x7e01)

#define HDMI_I2CM_DATAO_OFFSET (0x7e02)

#define HDMI_I2CM_DATAI_OFFSET (0x7e03)

#define HDMI_I2CM_OPERATION_OFFSET (0x7e04)
#define HDMI_I2CM_OPERATION_RD_START (0)
#define HDMI_I2CM_OPERATION_RD_MASK (0x1)
#define HDMI_I2CM_OPERATION_RD_EXT_START (1)
#define HDMI_I2CM_OPERATION_RD_EXT_MASK (0x2)
#define HDMI_I2CM_OPERATION_RD_8_START (2)
#define HDMI_I2CM_OPERATION_RD_8_MASK (0x4)
#define HDMI_I2CM_OPERATION_RD_8_EXT_START (3)
#define HDMI_I2CM_OPERATION_RD_8_EXT_MASK (0x8)
#define HDMI_I2CM_OPERATION_WR_START (4)
#define HDMI_I2CM_OPERATION_WR_MASK (0x10)

#define HDMI_I2CM_INT_OFFSET (0x7e05)
#define HDMI_I2CM_INT_DONE_MASK_START (2)
#define HDMI_I2CM_INT_DONE_MASK_MASK (0x4)
#define HDMI_I2CM_INT_READ_REQ_MASK_START (6)
#define HDMI_I2CM_INT_READ_REQ_MASK_MASK (0x40)

#define HDMI_I2CM_CTLINT_OFFSET (0x7e06)
#define HDMI_I2CM_CTLINT_ARB_MASK_START (2)
#define HDMI_I2CM_CTLINT_ARB_MASK_MASK (0x4)
#define HDMI_I2CM_CTLINT_NACK_MASK_START (6)
#define HDMI_I2CM_CTLINT_NACK_MASK_MASK (0x40)

#define HDMI_I2CM_DIV_OFFSET (0x7e07)
#define HDMI_I2CM_DIV_FAST_STD_MODE_START (3)
#define HDMI_I2CM_DIV_FAST_STD_MODE_MASK (0x8)

#define HDMI_I2CM_SEGADDR_OFFSET (0x7e08)

#define HDMI_I2CM_SOFTRSTZ_OFFSET (0x7e09)

#define HDMI_I2CM_SEGPTR_OFFSET (0x7e0a)

#define HDMI_I2CM_SS_SCL_HCNT_1_OFFSET (0x7e0b)

#define HDMI_I2CM_SS_SCL_HCNT_0_OFFSET (0x7e0c)

#define HDMI_I2CM_SS_SCL_LCNT_1_OFFSET (0x7e0d)

#define HDMI_I2CM_SS_SCL_LCNT_0_OFFSET (0x7e0e)

#define HDMI_I2CM_FS_SCL_HCNT_1_OFFSET (0x7e0f)

#define HDMI_I2CM_FS_SCL_HCNT_0_OFFSET (0x7e10)

#define HDMI_I2CM_FS_SCL_LCNT_1_OFFSET (0x7e11)

#define HDMI_I2CM_FS_SCL_LCNT_0_OFFSET (0x7e12)

#define HDMI_I2CM_SDA_HOLD_OFFSET (0x7e13)

#define HDMI_I2CM_SCDC_READ_UPDATE_OFFSET (0x7e14)

#endif
