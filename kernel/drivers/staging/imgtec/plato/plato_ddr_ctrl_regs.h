/*************************************************************************/ /*!
@Title          Plato DDR CTRL register definitions
@Copyright      Copyright (c) Imagination Technologies Ltd. All Rights Reserved
@Description
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

#ifndef _PLATO_DDR_CTRL_REGS_H_
#define _PLATO_DDR_CTRL_REGS_H_

#define PLATO_DDR_CTRL_MSTR 0x000 /* Master Register */
#define PLATO_DDR_CTRL_STAT 0x004 /* Operating Mode Status Register */
#define PLATO_DDR_CTRL_MRCTRL0 \
	0x0010 /* Mode Register Read/Write Control Register 0. Note: Do not enable more than one of the following... */
#define PLATO_DDR_CTRL_MRCTRL1 \
	0x0014 /* Mode Register Read/Write Control Register 1 */
#define PLATO_DDR_CTRL_MRSTAT \
	0x0018 /* Mode Register Read/Write Status Register */
#define PLATO_DDR_CTRL_MRCTRL2 \
	0x001c /* Mode Register Read/Write Control Register 2 */
#define PLATO_DDR_CTRL_DERATEEN 0x0020 /* Temperature Derate Enable Register */
#define PLATO_DDR_CTRL_DERATEINT \
	0x0024 /* Temperature Derate Interval Register */
#define PLATO_DDR_CTRL_PWRCTL 0x0030 /* Low Power Control Register */
#define PLATO_DDR_CTRL_PWRTMG 0x0034 /* Low Power Timing Register */
#define PLATO_DDR_CTRL_HWLPCTL 0x0038 /* Hardware Low Power Control Register */
#define PLATO_DDR_CTRL_RFSHCTL0 0x0050 /* Refresh Control Register 0 */
#define PLATO_DDR_CTRL_RFSHCTL1 0x0054 /* Refresh Control Register 1 */
#define PLATO_DDR_CTRL_RFSHCTL2 0x0058 /* Refresh Control Register 2 */
#define PLATO_DDR_CTRL_RFSHCTL3 0x0060 /* Refresh Control Register 0 */
#define PLATO_DDR_CTRL_RFSHTMG 0x0064 /* Refresh Timing Register */
#define PLATO_DDR_CTRL_CRCPARCTL0 0x00c0 /* CRC Parity Control Register0 */
#define PLATO_DDR_CTRL_CRCPARCTL1 0x00c4 /* CRC Parity Control Register1 */
#define PLATO_DDR_CTRL_CRCPARSTAT 0x00cc /* CRC Parity Status Register */
#define PLATO_DDR_CTRL_INIT0 0x00d0 /* SDRAM Initialization Register 0 */
#define PLATO_DDR_CTRL_INIT1 0x00d4 /* SDRAM Initialization Register 1 */
#define PLATO_DDR_CTRL_INIT2 0x00d8 /* SDRAM Initialization Register 2 */
#define PLATO_DDR_CTRL_INIT3 0x00dc /* SDRAM Initialization Register 3 */
#define PLATO_DDR_CTRL_INIT4 0x00e0 /* SDRAM Initialization Register 4 */
#define PLATO_DDR_CTRL_INIT5 0x00e4 /* SDRAM Initialization Register 5 */
#define PLATO_DDR_CTRL_INIT6 0x00e8 /* SDRAM Initialization Register 6 */
#define PLATO_DDR_CTRL_INIT7 0x00ec /* SDRAM Initialization Register 7 */
#define PLATO_DDR_CTRL_DIMMCTL 0x00f0 /* DIMM Control Register */
#define PLATO_DDR_CTRL_RANKCTL 0x00f4 /* Rank Control Register */
#define PLATO_DDR_CTRL_DRAMTMG0 0x00100 /* SDRAM Timing Register 0 */
#define PLATO_DDR_CTRL_DRAMTMG1 0x00104 /* SDRAM Timing Register 1 */
#define PLATO_DDR_CTRL_DRAMTMG2 0x00108 /* SDRAM Timing Register 2 */
#define PLATO_DDR_CTRL_DRAMTMG3 0x0010c /* SDRAM Timing Register 3 */
#define PLATO_DDR_CTRL_DRAMTMG4 0x00110 /* SDRAM Timing Register 4 */
#define PLATO_DDR_CTRL_DRAMTMG5 0x00114 /* SDRAM Timing Register 5 */
#define PLATO_DDR_CTRL_DRAMTMG6 0x00118 /* SDRAM Timing Register 6 */
#define PLATO_DDR_CTRL_DRAMTMG7 0x0011c /* SDRAM Timing Register 7 */
#define PLATO_DDR_CTRL_DRAMTMG8 0x00120 /* SDRAM Timing Register 8 */
#define PLATO_DDR_CTRL_DRAMTMG9 0x00124 /* SDRAM Timing Register 9 */
#define PLATO_DDR_CTRL_DRAMTMG10 0x00128 /* SDRAM Timing Register 10 */
#define PLATO_DDR_CTRL_DRAMTMG11 0x0012c /* SDRAM Timing Register 11 */
#define PLATO_DDR_CTRL_DRAMTMG12 0x00130 /* SDRAM Timing Register 12 */
#define PLATO_DDR_CTRL_ZQCTL0 0x00180 /* ZQ Control Register 0 */
#define PLATO_DDR_CTRL_ZQCTL1 0x00184 /* ZQ Control Register 1 */
#define PLATO_DDR_CTRL_ZQCTL2 0x00188 /* ZQ Control Register 2 */
#define PLATO_DDR_CTRL_ZQSTAT 0x0018c /* ZQ Status Register */
#define PLATO_DDR_CTRL_DFITMG0 0x00190 /* DFI Timing Register 0 */
#define PLATO_DDR_CTRL_DFITMG1 0x00194 /* DFI Timing Register 1 */
#define PLATO_DDR_CTRL_DFILPCFG0 \
	0x00198 /* DFI Low Power Configuration Register 0 */
#define PLATO_DDR_CTRL_DFILPCFG1 \
	0x0019c /* DFI Low Power Configuration Register 1 */
#define PLATO_DDR_CTRL_DFIUPD0 0x001a0 /* DFI Update Register 0 */
#define PLATO_DDR_CTRL_DFIUPD1 0x001a4 /* DFI Update Register 1 */
#define PLATO_DDR_CTRL_DFIUPD2 0x001a8 /* DFI Update Register 2 */
#define PLATO_DDR_CTRL_DFIUPD3 0x001ac /* DFI Update Register 3 */
#define PLATO_DDR_CTRL_DFIMISC 0x001b0 /* DFI Miscellaneous Control Register */
#define PLATO_DDR_CTRL_DFIUPD4 0x001b8 /* DFI Update Register 4 */
#define PLATO_DDR_CTRL_DBICTL 0x001c0 /* DM/DBI Control Register */
#define PLATO_DDR_CTRL_ADDRMAP0 0x00200 /* Address Map Register 0 */
#define PLATO_DDR_CTRL_ADDRMAP1 0x00204 /* Address Map Register 1 */
#define PLATO_DDR_CTRL_ADDRMAP2 0x00208 /* Address Map Register 2 */
#define PLATO_DDR_CTRL_ADDRMAP3 0x0020c /* Address Map Register 3 */
#define PLATO_DDR_CTRL_ADDRMAP4 0x00210 /* Address Map Register 4 */
#define PLATO_DDR_CTRL_ADDRMAP5 0x00214 /* Address Map Register 5 */
#define PLATO_DDR_CTRL_ADDRMAP6 0x00218 /* Address Map Register 6 */
#define PLATO_DDR_CTRL_ADDRMAP7 0x0021c /* Address Map Register 7 */
#define PLATO_DDR_CTRL_ADDRMAP8 0x00220 /* Address Map Register 8 */
#define PLATO_DDR_CTRL_ODTCFG 0x00240 /* ODT Configuration Register */
#define PLATO_DDR_CTRL_ODTMAP 0x00244 /* ODT/Rank Map Register */
#define PLATO_DDR_CTRL_SCHED 0x00250 /* Scheduler Control Register */
#define PLATO_DDR_CTRL_SCHED1 0x00254 /* Scheduler Control Register 1 */
#define PLATO_DDR_CTRL_PERFHPR1 0x0025c /* High Priority Read CAM Register 1 */
#define PLATO_DDR_CTRL_PERFLPR1 0x00264 /* Low Priority Read CAM Register 1 */
#define PLATO_DDR_CTRL_PERFWR1 0x0026c /* Write CAM Register 1 */
#define PLATO_DDR_CTRL_DBG0 0x00300 /* Debug Register 0 */
#define PLATO_DDR_CTRL_DBG1 0x00304 /* Debug Register 1 */
#define PLATO_DDR_CTRL_DBGCAM 0x00308 /* CAM Debug Register */
#define PLATO_DDR_CTRL_DBGCMD 0x0030c /* Command Debug Register */
#define PLATO_DDR_CTRL_DBGSTAT 0x00310 /* Status Debug Register */
#define PLATO_DDR_CTRL_SWCTL \
	0x00320 /* Software register programming control enable */
#define PLATO_DDR_CTRL_SWSTAT \
	0x00324 /* Software register programming control status */

/* Multi port registers */
#define PLATO_DDR_CTRL_PSTAT 0x003fc
#define PLATO_DDR_CTRL_PCCFG 0x00400
#define PLATO_DDR_CTRL_PCFGR 0x00404
#define PLATO_DDR_CTRL_PCFGW 0x00408
#define PLATO_DDR_CTRL_PCFGC 0x0040c
#define PLATO_DDR_CTRL_PCFGIDMASKCH_START \
	0x00410 /* Offset + m*0x08, where m is the virtual channel */
#define PLATO_DDR_CTRL_PCFGIDVALUECH_START \
	0x00414 /* Offset + m*0x08, where m is the virtual channel */
#define PLATO_DDR_CTRL_PCTRL 0x00490

#endif /* _PLATO_DDR_CTRL_REGS_H_ */
