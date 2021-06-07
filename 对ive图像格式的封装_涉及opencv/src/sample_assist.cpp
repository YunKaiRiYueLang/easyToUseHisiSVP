
#include "sample_assist.h"
#include "hi_comm_sys.h"
#include "mpi_sys.h"
#include "sample_define.h"
#include <stdlib.h>
#if defined WIN32 || defined _WIN32 || defined WINCE
#include <Windows.h>
#else
#include <time.h>
#endif
#include"debug.h"//试了自己定义的宏
HI_U32 HI_CalcStride(HI_U32 u32Width, HI_U32 u32Align)
{
	HI_U32 u16stride = u32Width + (u32Align - u32Width % u32Align) % u32Align;
	return u16stride;
}
HI_S32 HI_CreateIveImage(IVE_IMAGE_S *pstImage, IVE_IMAGE_TYPE_E enType, HI_U32 u32Width, HI_U32 u32Height)
{
	HI_U32 u32Stride;
	HI_S32 s32Succ;

	HI_CHECK_ET_NULL_RET(pstImage, HI_FAILURE);

	pstImage->enType = enType;
	pstImage->u32Width = u32Width;
	pstImage->u32Height = u32Height;

	u32Stride = HI_CalcStride(u32Width, HI_IVE2_STRIDE_ALIGN);
	s32Succ = HI_SUCCESS;

	switch (enType)
	{
	case IVE_IMAGE_TYPE_U8C1:
	case IVE_IMAGE_TYPE_S8C1:
	{
		int ret = HI_MPI_SYS_MmzAlloc(&pstImage->au64PhyAddr[0], (HI_VOID **)&(pstImage->au64VirAddr[0]),
									  HI_NULL, HI_NULL, u32Stride * u32Height);
		if (ret != 0)
		{
			printf("error code :%x\n", ret);
			return -1;
		}
		pstImage->au32Stride[0] = u32Stride;
	}
	break;
	case IVE_IMAGE_TYPE_S16C1:
	case IVE_IMAGE_TYPE_U16C1:
	{
		int ret = HI_MPI_SYS_MmzAlloc(&pstImage->au64PhyAddr[0], (HI_VOID **)&(pstImage->au64VirAddr[0]),
									  HI_NULL, HI_NULL, u32Stride * u32Height * 2);
		if (ret != 0)
		{
			printf("error code :%x\n", ret);
			return -1;
		}
		pstImage->au32Stride[0] = u32Stride;
	}
	break;
	case IVE_IMAGE_TYPE_U8C3_PLANAR:
	{
		int channelLength = u32Stride * u32Height;
		unsigned int size = channelLength * 3;
		int ret = HI_MPI_SYS_MmzAlloc(&pstImage->au64PhyAddr[0], (HI_VOID **)&(pstImage->au64VirAddr[0]),
									  HI_NULL, HI_NULL, size);
		CHECK_HISI_FUNCTION("HI_MPI_SYS_MmzAlloc", ret);
		pstImage->au64PhyAddr[1] = pstImage->au64PhyAddr[0] + channelLength;
		pstImage->au64PhyAddr[2] = pstImage->au64PhyAddr[1] + channelLength;
		pstImage->au64VirAddr[1] = pstImage->au64VirAddr[0] + channelLength;
		pstImage->au64VirAddr[2] = pstImage->au64VirAddr[1] + channelLength;
		pstImage->au32Stride[0] = u32Stride;
		pstImage->au32Stride[1] = pstImage->au32Stride[0];
		pstImage->au32Stride[2] = pstImage->au32Stride[0];
		// printf("create u8c3 planar\n");
	}
	break;
	case IVE_IMAGE_TYPE_YUV420SP:
	{
		pstImage->au32Stride[0] = u32Stride;
		unsigned int u32Size = pstImage->au32Stride[0] * pstImage->u32Height * 3 / 2;
		int s32Ret = HI_MPI_SYS_MmzAlloc(&pstImage->au64PhyAddr[0], (HI_VOID **)&pstImage->au64VirAddr[0], NULL, HI_NULL, u32Size);
		if (s32Ret != HI_SUCCESS)
		{
			SAMPLE_PRT("Mmz Alloc fail,Error(%#x)\n", s32Ret);
			return s32Ret;
		}
		pstImage->au32Stride[1] = pstImage->au32Stride[0];
		pstImage->au64PhyAddr[1] = pstImage->au64PhyAddr[0] + pstImage->au32Stride[0] * pstImage->u32Height;
		pstImage->au64VirAddr[1] = pstImage->au64VirAddr[0] + pstImage->au32Stride[0] * pstImage->u32Height;
	}
	break;
	case IVE_IMAGE_TYPE_U8C3_PACKAGE:
	{
		unsigned int u32Size = pstImage->au32Stride[0] * pstImage->u32Height * 3;
		int64_t s32Ret = HI_MPI_SYS_MmzAlloc(&pstImage->au64PhyAddr[0], (HI_VOID **)&pstImage->au64VirAddr[0], NULL, HI_NULL, u32Size);
		if (s32Ret != HI_SUCCESS)
		{
			SAMPLE_PRT("Mmz Alloc fail,Error(%#x)\n", s32Ret);
			return s32Ret;
		}
		pstImage->au64VirAddr[1] = pstImage->au64VirAddr[0] + 1;
		pstImage->au64VirAddr[2] = pstImage->au64VirAddr[1] + 1;
		pstImage->au64PhyAddr[1] = pstImage->au64PhyAddr[0] + 1;
		pstImage->au64PhyAddr[2] = pstImage->au64PhyAddr[1] + 1;
		pstImage->au32Stride[1] = pstImage->au32Stride[0];
		pstImage->au32Stride[2] = pstImage->au32Stride[0];
	}
	break;
	case IVE_IMAGE_TYPE_S32C1:
	case IVE_IMAGE_TYPE_U32C1:
	{
		unsigned int u32Size = pstImage->au32Stride[0] * pstImage->u32Height*sizeof(HI_U32);
		printf("mmz size:%d\n", u32Size);
		int s32Ret = HI_MPI_SYS_MmzAlloc(&pstImage->au64PhyAddr[0], (HI_VOID **)&pstImage->au64VirAddr[0], NULL, HI_NULL, u32Size);
		if (s32Ret != HI_SUCCESS)
		{
			SAMPLE_PRT("Mmz Alloc fail,Error(%#x)\n", s32Ret);
			return s32Ret;
		}
	}
	break;
	default:
	{
		s32Succ = HI_FAILURE;
	}
	break;
	}

	return s32Succ;
}

HI_S32 comp(const HI_VOID *a, const HI_VOID *b)
{
	return *(HI_U8 *)(*(HI_U32 *)b) - *(HI_U8 *)(*(HI_U32 *)a);
}

HI_S64 HI_GetTickCount(HI_VOID)
{
	HI_S64 s32Tick = 0;

#if defined WIN32 || defined _WIN32 || defined WINCE
	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);
	s32Tick = (HI_S64)counter.QuadPart;
#elif defined __linux || defined __linux__
	struct timespec tp;
	clock_gettime(CLOCK_MONOTONIC, &tp);
	s32Tick = (HI_S64)tp.tv_sec * 1000000000 + tp.tv_nsec;
#else
	struct timeval tv;
	struct timezone tz;
	gettimeofday(&tv, &tz);
	s32Tick = (HI_S64)tv.tv_sec * 1000000 + tv.tv_usec;
#endif

	return s32Tick;
}

HI_DOUBLE HI_GetTickFrequency(HI_VOID)
{
	HI_DOUBLE dFre = 1.0;
#if defined WIN32 || defined _WIN32 || defined WINCE
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	dFre = (HI_DOUBLE)freq.QuadPart;
#elif defined __linux || defined __linux__
	dFre = 1e9;
#else
	dFre = 1e6;
#endif

	return (dFre * 1e-6);
}

HI_S32 HI_GenRand(HI_S32 s32Max, HI_S32 s32Min)
{
	HI_S32 s32Result = 0;

	if (s32Min >= 0)
	{
		s32Result = s32Min + rand() % (s32Max - s32Min + 1);
	}
	else
	{
		s32Result = rand() % (s32Max - s32Min + 1);
		s32Result = s32Result > s32Max ? s32Max - s32Result : s32Result;
	}

	return s32Result;
}
