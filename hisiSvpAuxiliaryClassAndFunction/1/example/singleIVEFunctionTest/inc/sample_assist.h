#ifndef _SAMPLE_ASSIST_H_
#define _SAMPLE_ASSIST_H_

	
#include "hi_comm_ive.h"
#include "mpi_ive.h"

#define CLIP(a, maxv, minv)		 (((a)>(maxv)) ? (maxv) : (((a) < (minv)) ? (minv) : (a)))

HI_U32 HI_CalcStride(HI_U32 u32Width, HI_U32 u32Align);
HI_S32 HI_CreateIveImage(IVE_IMAGE_S *pstImage,IVE_IMAGE_TYPE_E enType, HI_U32 u32Width, HI_U32 u32Height);
HI_S64 HI_GetTickCount(HI_VOID);
HI_DOUBLE HI_GetTickFrequency(HI_VOID);
HI_S32 HI_GenRand(HI_S32 s32Max, HI_S32 s32Min);

#endif /*_SAMPLE_ASSIST_H_*/