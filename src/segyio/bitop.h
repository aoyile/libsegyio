/*******************************************************************************
 *
 *                  libsegyio Copyright (C) 2021 aoyile
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 ******************************************************************************/

#ifndef LIB_SEGYIO_BITOP_H
#define LIB_SEGYIO_BITOP_H

#include "segyio/config.h"
#include <cstdlib>

namespace SegyIO {

typedef std::int8_t   INT8;
typedef std::uint8_t  UINT8;
typedef std::int16_t  INT16;
typedef std::uint16_t UINT16;
typedef std::int32_t  INT32;
typedef std::uint32_t UINT32;
typedef std::int64_t  INT64;
typedef std::uint64_t UINT64;
typedef float         FLOAT32;
typedef double        FLOAT64;

#define SEGYIO_BIG_ENDIAN  0x00u
#define SEGYIO_LIT_ENDIAN  0x01u

UINT16 gEndianLOCAL(void);
UINT16 gEndianBSWAP(UINT16 endian);

UINT16   gBSWAP(const UINT16 bits);
UINT32   gBSWAP(const UINT32 bits);
UINT64   gBSWAP(const UINT64 bits);
INT16    gBSWAP(const INT16 bits);
INT32    gBSWAP(const INT32 bits);
INT64    gBSWAP(const INT64 bits);
FLOAT32  gBSWAP(const FLOAT32 bits);
FLOAT64  gBSWAP(const FLOAT64 bits);

FLOAT32 gIBMtoIEEE(FLOAT32 f);
FLOAT32 gIEEEtoIBM(FLOAT32 f);

FLOAT64 gIBMtoIEEE(FLOAT64 f);
FLOAT64 gIEEEtoIBM(FLOAT64 f);

} /*namespace SegyIO*/

#endif // LIB_SEGYIO_BITOP_H
