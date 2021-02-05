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

#include <cstdlib>
#include <math.h>
#include <string.h>
#include "segyio/segydef.h"

namespace SegyIO {

void gSegyBSWAP(SegyBinaryHeader* pHead)
{
  pHead->jobid  = gBSWAP(pHead->jobid);
  pHead->lino   = gBSWAP(pHead->lino);
  pHead->reno   = gBSWAP(pHead->reno);
  pHead->ntrpr  = gBSWAP(pHead->ntrpr);
  pHead->nart   = gBSWAP(pHead->nart);
  pHead->hdt    = gBSWAP(pHead->hdt);
  pHead->dto    = gBSWAP(pHead->dto);
  pHead->hns    = gBSWAP(pHead->hns);
  pHead->nso    = gBSWAP(pHead->nso);
  pHead->format = gBSWAP(pHead->format);
  pHead->fold   = gBSWAP(pHead->fold);
  pHead->tsort  = gBSWAP(pHead->tsort);
  pHead->vscode = gBSWAP(pHead->vscode);
  pHead->hsfs   = gBSWAP(pHead->hsfs);
  pHead->hsfe   = gBSWAP(pHead->hsfe);
  pHead->hslen  = gBSWAP(pHead->hslen);
  pHead->hslen  = gBSWAP(pHead->hstyp);
  pHead->schn   = gBSWAP(pHead->schn);
  pHead->hstas  = gBSWAP(pHead->hstas);
  pHead->hstae  = gBSWAP(pHead->hstae);
  pHead->htatyp = gBSWAP(pHead->htatyp);
  pHead->hcorr  = gBSWAP(pHead->hcorr);
  pHead->bgrcv  = gBSWAP(pHead->bgrcv);
  pHead->rcvm   = gBSWAP(pHead->rcvm);
  pHead->mfeet  = gBSWAP(pHead->mfeet);
  pHead->polyt  = gBSWAP(pHead->polyt);
  pHead->vpol   = gBSWAP(pHead->vpol);
}

void gSegyBSWAP(SegyTraceHeader* pTraceHead)
{
  pTraceHead->tracl  = gBSWAP(pTraceHead->tracl);
  pTraceHead->tracr  = gBSWAP(pTraceHead->tracr);
  pTraceHead->fldr   = gBSWAP(pTraceHead->fldr);
  pTraceHead->tracf  = gBSWAP(pTraceHead->tracf);
  pTraceHead->ep     = gBSWAP(pTraceHead->ep);
  pTraceHead->cdp    = gBSWAP(pTraceHead->cdp);
  pTraceHead->cdpt   = gBSWAP(pTraceHead->cdpt);
  pTraceHead->trid   = gBSWAP(pTraceHead->trid);
  pTraceHead->nvs    = gBSWAP(pTraceHead->nvs);
  pTraceHead->nhs    = gBSWAP(pTraceHead->nhs);
  pTraceHead->duse   = gBSWAP(pTraceHead->duse);
  pTraceHead->offset = gBSWAP(pTraceHead->offset);
  pTraceHead->gelev  = gBSWAP(pTraceHead->gelev);
  pTraceHead->selev  = gBSWAP(pTraceHead->selev);
  pTraceHead->sdepth = gBSWAP(pTraceHead->sdepth);
  pTraceHead->gdel   = gBSWAP(pTraceHead->gdel);
  pTraceHead->sdel   = gBSWAP(pTraceHead->sdel);
  pTraceHead->swdep  = gBSWAP(pTraceHead->swdep);
  pTraceHead->gwdep  = gBSWAP(pTraceHead->gwdep);
  pTraceHead->scalel = gBSWAP(pTraceHead->scalel);
  pTraceHead->scalco = gBSWAP(pTraceHead->scalco);
  pTraceHead->sx     = gBSWAP(pTraceHead->sx);
  pTraceHead->sy     = gBSWAP(pTraceHead->sy);
  pTraceHead->gx     = gBSWAP(pTraceHead->gx);
  pTraceHead->gy     = gBSWAP(pTraceHead->gy);
  pTraceHead->counit = gBSWAP(pTraceHead->counit);
  pTraceHead->wevel  = gBSWAP(pTraceHead->wevel);
  pTraceHead->swevel = gBSWAP(pTraceHead->swevel);
  pTraceHead->sut    = gBSWAP(pTraceHead->sut);
  pTraceHead->gut    = gBSWAP(pTraceHead->gut);
  pTraceHead->sstat  = gBSWAP(pTraceHead->sstat);
  pTraceHead->gstat  = gBSWAP(pTraceHead->gstat);
  pTraceHead->tstat  = gBSWAP(pTraceHead->tstat);
  pTraceHead->laga   = gBSWAP(pTraceHead->laga);
  pTraceHead->lagb   = gBSWAP(pTraceHead->lagb);
  pTraceHead->delrt  = gBSWAP(pTraceHead->delrt);
  pTraceHead->muts   = gBSWAP(pTraceHead->muts);
  pTraceHead->mute   = gBSWAP(pTraceHead->mute);
  pTraceHead->ns     = gBSWAP(pTraceHead->ns);
  pTraceHead->dt     = gBSWAP(pTraceHead->dt);
  pTraceHead->gain   = gBSWAP(pTraceHead->gain);
  pTraceHead->igc    = gBSWAP(pTraceHead->igc);
  pTraceHead->igi    = gBSWAP(pTraceHead->igi);
  pTraceHead->corr   = gBSWAP(pTraceHead->corr);
  pTraceHead->sfs    = gBSWAP(pTraceHead->sfs);
  pTraceHead->sfe    = gBSWAP(pTraceHead->sfe);
  pTraceHead->slen   = gBSWAP(pTraceHead->slen);
  pTraceHead->styp   = gBSWAP(pTraceHead->styp);
  pTraceHead->stas   = gBSWAP(pTraceHead->stas);
  pTraceHead->stae   = gBSWAP(pTraceHead->stae);
  pTraceHead->tatyp  = gBSWAP(pTraceHead->tatyp);
  pTraceHead->afilf  = gBSWAP(pTraceHead->afilf);
  pTraceHead->afils  = gBSWAP(pTraceHead->afils);
  pTraceHead->nofilf = gBSWAP(pTraceHead->nofilf);
  pTraceHead->nofils = gBSWAP(pTraceHead->nofils);
  pTraceHead->lcf    = gBSWAP(pTraceHead->lcf);
  pTraceHead->hcf    = gBSWAP(pTraceHead->hcf);
  pTraceHead->lcs    = gBSWAP(pTraceHead->lcs);
  pTraceHead->hcs    = gBSWAP(pTraceHead->hcs);
  pTraceHead->year   = gBSWAP(pTraceHead->year);
  pTraceHead->day    = gBSWAP(pTraceHead->day);
  pTraceHead->minute = gBSWAP(pTraceHead->minute);
  pTraceHead->sec    = gBSWAP(pTraceHead->sec);
  pTraceHead->timbas = gBSWAP(pTraceHead->timbas);
  pTraceHead->trwf   = gBSWAP(pTraceHead->trwf);
  pTraceHead->grnors = gBSWAP(pTraceHead->grnors);
  pTraceHead->grnofr = gBSWAP(pTraceHead->grnofr);
  pTraceHead->grnlof = gBSWAP(pTraceHead->grnlof);
  pTraceHead->gaps   = gBSWAP(pTraceHead->gaps);
  pTraceHead->otrav  = gBSWAP(pTraceHead->otrav);
}

void gSegyDecodeASCII(UINT8* pASCII, const uword length)
{
  static UINT8 map[] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x09, 0x06, 0x07,
    0x97, 0x8d, 0x8e, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11,
    0x12, 0x13, 0x9d, 0x85, 0x08, 0x87, 0x18, 0x19, 0x92, 0x8f,
    0x1c, 0x1d, 0x1e, 0x1f, 0x80, 0x81, 0x82, 0x83, 0x84, 0x0a,
    0x17, 0x1b, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x05, 0x06, 0x07,
    0x90, 0x91, 0x16, 0x93, 0x94, 0x95, 0x96, 0x04, 0x98, 0x99,
    0x9a, 0x9b, 0x14, 0x15, 0x9e, 0x1a, 0x20, 0xa0, 0xa1, 0xa2,
    0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xd5, 0x2e, 0x3c, 0x28,
    0x2b, 0x7c, 0x26, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf,
    0xb0, 0xb1, 0x21, 0x24, 0x2a, 0x29, 0x3b, 0x7e, 0x2d, 0x2f,
    0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xcb, 0x2c,
    0x25, 0x5f, 0x3e, 0x3f, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf,
    0xc0, 0xc1, 0xc2, 0x60, 0x3a, 0x23, 0x40, 0x27, 0x3d, 0x22,
    0xc3, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
    0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0x6a, 0x6b, 0x6c,
    0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x5e, 0xcc, 0xcd, 0xce,
    0xcf, 0xd0, 0xd1, 0xe5, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
    0x79, 0x7a, 0xd2, 0xd3, 0xd4, 0x5b, 0xd6, 0xd7, 0xd8, 0xd9,
    0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf, 0xe0, 0xe1, 0xe2, 0xe3,
    0xe4, 0x5d, 0xe6, 0xe7, 0x7b, 0x41, 0x42, 0x43, 0x44, 0x45,
    0x46, 0x47, 0x48, 0x49, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed,
    0x7d, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, 0x51, 0x52,
    0xee, 0xef, 0xf0, 0xf1, 0xf2, 0xf3, 0x5c, 0x9f, 0x53, 0x54,
    0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0xf4, 0xf5, 0xf6, 0xf7,
    0xf8, 0xf9, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
    0x38, 0x39, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff
  };
  for (uword i = 0; i < length; ++i)
    pASCII[i] = map[ (uword)( pASCII[i] ) ];
}

void gSegyEncodeASCII(UINT8* pASCII, const uword length)
{
  static UINT8 map[] = {
    0x00, 0x01, 0x02, 0x03, 0x37, 0x2d, 0x2e, 0x2f,
    0x16, 0x05, 0x25, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11,
    0x12, 0x13, 0x3c, 0x3d, 0x32, 0x26, 0x18, 0x19, 0x3f, 0x27,
    0x1c, 0x1d, 0x1e, 0x1f, 0x40, 0x5a, 0x7f, 0x7b, 0x5b, 0x6c,
    0x50, 0x7d, 0x4d, 0x5d, 0x5c, 0x4e, 0x6b, 0x60, 0x4b, 0x61,
    0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9,
    0x7a, 0x5e, 0x4c, 0x7e, 0x6e, 0x6f, 0x7c, 0xc1, 0xc2, 0xc3,
    0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xd1, 0xd2, 0xd3, 0xd4,
    0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6,
    0xe7, 0xe8, 0xe9, 0xad, 0xe0, 0xbd, 0x9a, 0x6d, 0x79, 0x81,
    0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x91, 0x92,
    0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0xa2, 0xa3, 0xa4,
    0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xc0, 0x4f, 0xd0, 0x5f, 0x07,
    0x20, 0x21, 0x22, 0x23, 0x24, 0x15, 0x06, 0x17, 0x28, 0x29,
    0x2a, 0x2b, 0x2c, 0x09, 0x0a, 0x1b, 0x30, 0x31, 0x1a, 0x33,
    0x34, 0x35, 0x36, 0x08, 0x38, 0x39, 0x3a, 0x3b, 0x04, 0x14,
    0x3e, 0xe1, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
    0x49, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
    0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x70, 0x71,
    0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x80, 0x8a, 0x8b,
    0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x6a, 0x9b, 0x9c, 0x9d, 0x9e,
    0x9f, 0xa0, 0xaa, 0xab, 0xac, 0x4a, 0xae, 0xaf, 0xb0, 0xb1,
    0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbb,
    0xbc, 0xa1, 0xbe, 0xbf, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf,
    0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf, 0xea, 0xeb, 0xec, 0xed,
    0xee, 0xef, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff
  };
  for (uword i = 0; i < length; ++i)
    pASCII[i] = map[ (uword)( pASCII[i] ) ];
}

} /*namespace SegyIO*/