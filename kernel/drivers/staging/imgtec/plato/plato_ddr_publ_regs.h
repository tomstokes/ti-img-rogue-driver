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

#ifndef _PLATO_DDR_PUBL_REGS_H_
#define _PLATO_DDR_PUBL_REGS_H_

#define PLATO_DDR_PUBL_RIDR_OFFSET (0x0000)
#define PLATO_DDR_PUBL_PIR_OFFSET (0x0004) // PHY init
#define PLATO_DDR_PUBL_PGCR0_OFFSET (0x0010) // PHY General configuration
#define PLATO_DDR_PUBL_PGCR1_OFFSET (0x0014)
#define PLATO_DDR_PUBL_PGCR2_OFFSET (0x0018)
#define PLATO_DDR_PUBL_PGCR3_OFFSET (0x001C)
#define PLATO_DDR_PUBL_PGCR4_OFFSET (0x0020)
#define PLATO_DDR_PUBL_PGCR5_OFFSET (0x0024)
#define PLATO_DDR_PUBL_PGCR6_OFFSET (0x0028)
#define PLATO_DDR_PUBL_PGCR7_OFFSET (0x002C)
#define PLATO_DDR_PUBL_PGSR0_OFFSET (0x0030) // PHY general status
#define PLATO_DDR_PUBL_PGSR1_OFFSET (0x0034)

#define PLATO_DDR_PUBL_PTR0_OFFSET (0x0040)
#define PLATO_DDR_PUBL_PTR1_OFFSET (0x0044)
#define PLATO_DDR_PUBL_PTR2_OFFSET (0x0048)
#define PLATO_DDR_PUBL_PTR3_OFFSET (0x004C)
#define PLATO_DDR_PUBL_PTR4_OFFSET (0x0050)

#define PLATO_DDR_PUBL_DXCCR_OFFSET (0x0088) // DXCCR
#define PLATO_DDR_PUBL_DSGCR_OFFSET (0x0090) // DDR System general configuration

#define PLATO_DDR_PUBL_DCR_OFFSET (0x0100) // DRAM Configuration

#define PLATO_DDR_PUBL_DTPR0_OFFSET (0x0110)
#define PLATO_DDR_PUBL_DTPR1_OFFSET (0x0114)
#define PLATO_DDR_PUBL_DTPR2_OFFSET (0x0118)
#define PLATO_DDR_PUBL_DTPR3_OFFSET (0x011C)
#define PLATO_DDR_PUBL_DTPR4_OFFSET (0x0120)
#define PLATO_DDR_PUBL_DTPR5_OFFSET (0x0124)
#define PLATO_DDR_PUBL_DTPR6_OFFSET (0x0128)

#define PLATO_DDR_PUBL_RDIMMGCR0_OFFSET (0x0140)
#define PLATO_DDR_PUBL_RDIMMGCR1_OFFSET (0x0144)
#define PLATO_DDR_PUBL_RDIMMGCR2_OFFSET (0x0148)

#define PLATO_DDR_PUBL_RDIMMCR0_OFFSET (0x0150)
#define PLATO_DDR_PUBL_RDIMMCR1_OFFSET (0x0154)
#define PLATO_DDR_PUBL_RDIMMCR2_OFFSET (0x0158)
#define PLATO_DDR_PUBL_RDIMMCR3_OFFSET (0x015C)
#define PLATO_DDR_PUBL_RDIMMCR4_OFFSET (0x0160)

#define PLATO_DDR_PUBL_SCHCR0_OFFSET (0x0168)
#define PLATO_DDR_PUBL_SCHCR1_OFFSET (0x016C)

#define PLATO_DDR_PUBL_MR0_OFFSET (0x0180)
#define PLATO_DDR_PUBL_MR1_OFFSET (0x0184)
#define PLATO_DDR_PUBL_MR2_OFFSET (0x0188)
#define PLATO_DDR_PUBL_MR3_OFFSET (0x018C)
#define PLATO_DDR_PUBL_MR4_OFFSET (0x0190)
#define PLATO_DDR_PUBL_MR5_OFFSET (0x0194)
#define PLATO_DDR_PUBL_MR6_OFFSET (0x0198)
#define PLATO_DDR_PUBL_MR7_OFFSET (0x019C)

#define PLATO_DDR_PUBL_MR11_OFFSET (0x01AC)

#define PLATO_DDR_PUBL_DTCR0_OFFSET (0x0200)
#define PLATO_DDR_PUBL_DTCR1_OFFSET (0x0204)

#define PLATO_DDR_PUBL_DATX8_OFFSET (0x0700)
// DATX8 registers have 0x100 bytes per lane, reg is register number
#define PLATO_DDR_PUBL_DATX_OFFSET(offset, lane, reg) \
	(offset + (lane * 0x100) + (reg * 4))
#define PLATO_DDR_PUBL_DATX_LANE_COUNT (8)

// DATX8 Bit delay line registers
#define PLATO_DDR_PUBL_DX0BDLR0_OFFSET (0x0740)
#define PLATO_DDR_PUBL_DXBDLR_REGS_PER_LANE (3)
#define PLATO_DDR_PUBL_DXnBDLR_OFFSET(lane, reg) \
	PLATO_DDR_PUBL_DATX_OFFSET(PLATO_DDR_PUBL_DX0BDLR0_OFFSET, lane, reg)

// DATX8 General Status Registers
#define PLATO_DDR_PUBL_DX0GSR0_OFFSET (0x07E0)
#define PLATO_DDR_PUBL_DXGSR_REGS_PER_LANE (4)
#define PLATO_DDR_PUBL_DXnGSR_OFFSET(lane, reg) \
	PLATO_DDR_PUBL_DATX_OFFSET(PLATO_DDR_PUBL_DX0GSR0_OFFSET, lane, reg)

#endif
