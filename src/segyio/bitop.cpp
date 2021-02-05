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

#include "segyio/bitop.h"

namespace SegyIO {

#ifndef  __builtin_bswap16
#define __builtin_bswap16( bits__ )                                            \
  ((((bits__) >> 8) & 0xffu) | (((bits__) & 0xffu) << 8))
#endif

#ifndef  __builtin_bswap32
#define __builtin_bswap32( bits__ )                                            \
    ((((bits__) & 0xff000000u) >> 24) | (((bits__) & 0x00ff0000u) >>  8)       \
  | (((bits__) & 0x0000ff00u) <<  8) | (((bits__) & 0x000000ffu) << 24))
#endif

#ifndef __builtin_bswap64
#define __builtin_bswap64( bits__ )                                            \
    ((((bits__) & 0xff00000000000000ull) >> 56)                                \
  | (((bits__) & 0x00ff000000000000ull) >> 40)                                 \
  | (((bits__) & 0x0000ff0000000000ull) >> 24)                                 \
  | (((bits__) & 0x000000ff00000000ull) >> 8)                                  \
  | (((bits__) & 0x00000000ff000000ull) << 8)                                  \
  | (((bits__) & 0x0000000000ff0000ull) << 24)                                 \
  | (((bits__) & 0x000000000000ff00ull) << 40)                                 \
  | (((bits__) & 0x00000000000000ffull) << 56))
#endif

UINT16 gEndianLOCAL(void)
{
  union { UINT16 word; UINT8 byte[2]; } check;
  check.word = 0x0001u;
  return check.byte[0];
}

UINT16 gEndianBSWAP(UINT16 endian)
{
  return (endian == SEGYIO_BIG_ENDIAN ? SEGYIO_LIT_ENDIAN : SEGYIO_BIG_ENDIAN);
}

UINT16 gBSWAP(const UINT16 bits) { return __builtin_bswap16(bits); }
UINT32 gBSWAP(const UINT32 bits) { return __builtin_bswap32(bits); }
UINT64 gBSWAP(const UINT64 bits) { return __builtin_bswap64(bits); }
INT16  gBSWAP(const INT16  bits) { return __builtin_bswap16(bits); }
INT32  gBSWAP(const INT32  bits) { return __builtin_bswap32(bits); }
INT64  gBSWAP(const INT64  bits) { return __builtin_bswap64(bits); }

FLOAT32  gBSWAP(const FLOAT32 bits)
{
  union { UINT32 bits; FLOAT32 real; } exchange;
  exchange.real = bits;
  exchange.bits = __builtin_bswap32(exchange.bits);
  return exchange.real;
}

FLOAT64 gBSWAP(const FLOAT64 bits)
{
  union { UINT64 bits; FLOAT64 real; } exchange;
  exchange.real = bits;
  exchange.bits = __builtin_bswap64(exchange.bits);
  return exchange.real;
}

FLOAT32 gIBMtoIEEE(FLOAT32 f)
{
  INT32 fconv, fmant, t;
  union { UINT8 byte[2]; UINT16 word; } endian;
  union { UINT32 bits; FLOAT32 real; } exchange;
  endian.word = 0x0001;

  exchange.real = f;
  fconv = exchange.bits;
  if (endian.byte[0] == 1) {
      fconv = ( fconv<<24)        |
              ((fconv>>24)&0xff)  |
              ((fconv&0xff00)<<8) |
              ((fconv&0xff0000)>>8);
  }
  /* (__builtin_bswap32(fconv)); */

  if (fconv) {
      fmant = 0x00ffffff & fconv;
      if (fmant == 0) return 0.0/0.0;
      t = (UINT32) ((0x7f000000 & fconv) >> 22) - 130;
      while (!(fmant & 0x00800000)) { --t; fmant <<= 1; }
      if (t > 254)
          fconv = (0x80000000 & fconv) | 0x7f7fffff;
      else if
          (t <= 0) fconv = 0;
      else
          fconv = (0x80000000 & fconv) |(t << 23)|(0x007fffff & fmant);
  }
  exchange.bits = fconv;
  return exchange.real;
}

FLOAT32 gIEEEtoIBM(FLOAT32 f)
{
  INT32 fconv, fmant, t;
  union { UINT8 byte[2]; UINT16 word; } endian;
  union { UINT32 bits; FLOAT32 real; } exchange;
  endian.word = 0x0001;

  exchange.real = f;
  fconv = exchange.bits;
  if (fconv) {
      fmant = (0x007fffff & fconv) | 0x00800000;
      t = (int) ((0x7f800000 & fconv) >> 23) - 126;
      while (t & 0x3) { ++t; fmant >>= 1; }
      fconv = (0x80000000 & fconv) | (((t>>2) + 64) << 24) | fmant;
  }
  if(endian.byte[0] == 1) {
      fconv =  (fconv<<24)        |
               ((fconv>>24)&0xff)  |
               ((fconv&0xff00)<<8) |
               ((fconv&0xff0000)>>8);
  }

  exchange.bits = fconv;
  return exchange.real;
}

FLOAT64 gIBMtoIEEE(FLOAT64 f);
FLOAT64 gIEEEtoIBM(FLOAT64 f);

} /*namespace SegyIO*/
