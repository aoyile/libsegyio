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

#ifndef LIB_SEGYIO_SEGYDEF_H
#define LIB_SEGYIO_SEGYDEF_H

#include "segyio/config.h"
#include "segyio/bitop.h"

namespace SegyIO {

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define SEGY_FMT_IBM_FLOAT32  1
#define SEGY_FMT_FIXED_INT32  2 /* fixed point, 4 byte (32 bits) */
#define SEGY_FMT_FIXED_INT16  3 /* fixed point, 2 byte (16 bits) */
#define SEGY_FMT_GAIN_CODE    4 /* fixed point w/gain code, 4 byte (32 bits) */
#define SEGY_FMT_IEEE_FLOAT32 5 /* IEEE floating point, 4 byte (32 bits) */
#define SEGY_FMT_FIXED_INT8   8 /* two's complement integer, 1 byte (8 bits) */

struct __SegyBinaryHeader
{
  INT32 jobid;  /* job identification number */
  INT32 lino;   /* line number (only one line per reel) */
  INT32 reno;   /* reel number */
  INT16 ntrpr;  /* number of data traceCount per record */
  INT16 nart;   /* number of auxiliary traceCount per record */
  UINT16 hdt;   /* sample interval in micro secs for this reel */
  UINT16 dto;   /* same for original field recording */
  UINT16 hns;   /* number of samples per trace for this reel */
  UINT16 nso;   /* same for original field recording */
  INT16 format; /* data sample format code:
                    1 = IBM floating point, 4 byte (32 bits)
                    2 = fixed point, 4 byte (32 bits)
                    3 = fixed point, 2 byte (16 bits)
                    4 = fixed point w/gain code, 4 byte (32 bits)
                    5 = IEEE floating point, 4 byte (32 bits)
                    8 = two's complement integer, 1 byte (8 bits)*/
  INT16 fold;  /* CDP fold expected per CDP ensemble */
  INT16 tsort; /* trace sorting code:
                    1 = as recorded (no sorting)
                    2 = CDP ensemble
                    3 = single fold continuous profile
                    4 = horizontally stacked */
  INT16 vscode; /* vertical sum code: 1 = no sum 2 = two sum ... N = N sum (N = 32,767) */
  INT16 hsfs;   /* sweep frequency at start */
  INT16 hsfe;   /* sweep frequency at end */
  INT16 hslen;  /* sweep length (ms) */
  INT16 hstyp;  /* sweep type code:
                    1 = linear
                    2 = parabolic
                    3 = exponential
                    4 = other */
  INT16 schn;   /* trace number of sweep channel */
  INT16 hstas;  /* sweep trace taper length at start if tapered (the taper starts at
                   zero time and is effective for this length) */
  INT16 hstae;  /* sweep trace taper length at end (the ending
      taper starts at sweep length minus the taper length at end) */
  INT16 htatyp; /* sweep trace taper type code: 1 = linear 2 = cos-squared 3 = other */
  INT16 hcorr;  /* correlated data traceCount code: 1 = no 2 = yes */
  INT16 bgrcv;  /* binary gain recovered code: 1 = yes 2 = no */
  INT16 rcvm;   /* amplitude recovery method code: 1 = none 2 = spherical divergence
                    3 = AGC 4 = other */
  INT16 mfeet;  /* measurement system code: 1 = meters 2 = feet */
  INT16 polyt;  /* impulse signal polarity code:
                    1 = increase in pressure or upward geophone case movement gives
                      negative number on tape
                    2 = increase in pressure or upward geophone case movement gives
                      positive number on tape */
  INT16 vpol;    /* vibratory polarity code:
                     code  seismic signal lags pilot by
                     1  337.5 to  22.5 degrees
                     2   22.5 to  67.5 degrees
                     3   67.5 to 112.5 degrees
                     4  112.5 to 157.5 degrees
                     5  157.5 to 202.5 degrees
                     6  202.5 to 247.5 degrees
                     7  247.5 to 292.5 degrees
                     8  293.5 to 337.5 degrees */
  INT16 hunass[170]; /* unassigned */
};

struct __SegyTraceHeader
{
  INT32 tracl;  /* Trace sequence number within line--numbers continue to increase if the
                   same line continues across multiple SEG Y files.*/
  INT32 tracr;  /* Trace sequence number within SEG Y file---each file starts with trace
                   sequence one*/
  INT32 fldr;  /* Original field record number */
  INT32 tracf; /* Trace number within original field record */
  INT32 ep;    /* energy source point number---Used when more than one record occurs
                  at the same effective surface location.*/
  INT32 cdp;   /* Ensemble number (i.e. CDP, CMP, CRP,...) */
  INT32 cdpt;  /* trace number within the ensemble---each ensemble starts with trace
                  number one.*/
  INT16 trid;  /* trace identification code:*/
  INT16 nvs;   /* Number of vertically summed traceCount yielding this trace. (1 is one trace,
                  2 is two summed traceCount, etc.)*/
  INT16 nhs;   /* Number of horizontally summed traceCount yielding this trace. (1 is one trace
                  2 is two summed traceCount, etc.)*/
  INT16 duse;   /* Data use: 1 = Production 2 = Test */
  INT32 offset; /* Distance from the center of the source point to the center of the
                   receiver group (negative if opposite to direction in which the line
                   was shot).*/
  INT32 gelev;  /* Receiver group elevation from sea level (all elevations above the
                   Vertical datum are positive and below are negative).*/
  INT32 selev;  /* Surface elevation at source. */
  INT32 sdepth; /* Source depth below surface (a positive number). */
  INT32 gdel;   /* Datum elevation at receiver group. */
  INT32 sdel;   /* Datum elevation at source. */
  INT32 swdep;  /* Water depth at source. */
  INT32 gwdep;  /* Water depth at receiver group. */
  INT16 scalel; /* Scalar to be applied to the previous 7 entries to give the real value.
                   Scalar = 1, +10, +100, +1000, +10000. If positive, scalar is used as a
                   multiplier, if negative, scalar is used as a divisor.*/
  INT16 scalco; /* Scalar to be applied to the next 4 entries to give the real value.
                    Scalar = 1, +10, +100, +1000, +10000. If positive, scalar is used as a
                    multiplier,if negative, scalar is used as a divisor.*/
  INT32  sx;  /* Source coordinate - X */
  INT32  sy;  /* Source coordinate - Y */
  INT32  gx;  /* Group coordinate - X */
  INT32  gy;  /* Group coordinate - Y */
  INT16 counit;  /* Coordinate units: (for previous 4 entries and for the 7 entries
                    before scalel)
                    1 = Length (meters or feet)
                    2 = Seconds of arc
                    3 = Decimal degrees
                    4 = Degrees, minutes, seconds (DMS) */
  INT16 wevel;  /* Weathering velocity. */
  INT16 swevel; /* Subweathering velocity. */
  INT16 sut;    /* Uphole time at source in milliseconds. */
  INT16 gut;    /* Uphole time at receiver group in milliseconds. */
  INT16 sstat;  /* Source static correction in milliseconds. */
  INT16 gstat;  /* Group static correction  in milliseconds.*/
  INT16 tstat;  /* Total static applied  in milliseconds.
                   (Zero if no static has been applied.)*/
  INT16 laga;   /* Lag time A, time in ms between end of 240-
                   byte trace identification header and time
                   break, positive if time break occurs after
                   end of header, time break is defined as
                   the initiation pulse which maybe recorded
                   on an auxiliary trace or as otherwise
                   specified by the recording system */
  INT16 lagb;   /* lag time B, time in ms between the time break
                   and the initiation time of the energy source,
                   may be positive or negative */
  INT16 delrt;  /* delay recording time, time in ms between
                   initiation time of energy source and time
                   when recording of data samples begins
                   (for deep water work if recording does not
                   start at zero time) */
  INT16 muts;  /* mute time--start */
  INT16 mute;  /* mute time--end */
  UINT16 ns;   /* number of samples in this trace */
  UINT16 dt;   /* sample interval; in micro-seconds */
  INT16 gain;  /* gain type of field instruments code:
                   1 = fixed
                   2 = binary
                   3 = floating point
                   4 ---- N = optional use */
  INT16 igc;   /* instrument gain constant */
  INT16 igi;   /* instrument early or initial gain */
  INT16 corr;  /* correlated: 1 = no 2 = yes */
  INT16 sfs;   /* sweep frequency at start */
  INT16 sfe;   /* sweep frequency at end */
  INT16 slen;  /* sweep length in ms */
  INT16 styp;  /* sweep type code:1 = linear 2 = cos-squared 3 = other */
  INT16 stas;  /* sweep trace length at start in ms */
  INT16 stae;  /* sweep trace length at end in ms */
  INT16 tatyp; /* taper type: 1=linear, 2=cos^2, 3=other */
  INT16 afilf; /* alias filter frequency if used */
  INT16 afils; /* alias filter slope */
  INT16 nofilf; /* notch filter frequency if used */
  INT16 nofils; /* notch filter slope */
  INT16 lcf;    /* low cut frequency if used */
  INT16 hcf;    /* high cut frequncy if used */
  INT16 lcs;    /* low cut slope */
  INT16 hcs;    /* high cut slope */
  INT16 year;   /* year data recorded */
  INT16 day;    /* day of year */
  INT16 hour;   /* hour of day (24 hour clock) */
  INT16 minute; /* minute of hour */
  INT16 sec;    /* second of minute */
  INT16 timbas; /* time basis code: 1 = local 2 = GMT 3 = other */
  INT16 trwf;   /* trace weighting factor, defined as 1/2^N volts for the least sigificant bit */
  INT16 grnors; /* geophone group number of roll switch position one */
  INT16 grnofr; /* geophone group number of trace one within original field record */
  INT16 grnlof; /* geophone group number of last trace within original field record */
  INT16 gaps;   /* gap length (total number of groups dropped) */
  INT16 otrav;  /* overtravel taper code:1 = down (or behind) 2 = up (or ahead) */

  INT16 unass[30];
};

#ifdef __cplusplus
}
#endif /* __cplusplus */

typedef __SegyBinaryHeader SegyBinaryHeader;

typedef __SegyTraceHeader  SegyTraceHeader;

void gSegyBSWAP(SegyBinaryHeader* pHead);

void gSegyBSWAP(SegyTraceHeader* pTraceHead);

void gSegyDecodeASCII(UINT8* pASCII, const uword length);

void gSegyEncodeASCII(UINT8* pASCII, const uword length);

} /*namespace SegyIO*/

#endif // LIB_SEGYIO_SEGYDEF_H
