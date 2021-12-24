#include "iveFunctions.hpp"

void theChosenGauss(hisiImage &src, hisiImage &dst)
{
    HI_S8 mask[25] = {
        1, 4, 7, 4, 1,
        4, 16, 26, 16, 4,
        7, 26, 41, 26, 7,
        4, 16, 26, 16, 4,
        1, 4, 7, 4, 1};
    HI_U8 norm = 8;
    int needBlock = 1;
    // doIveFilter(hiSrc,hiDst,mask,norm,needBlock)
    iveFilter(src, dst, mask, norm, needBlock);
}
bool iveDMA2(IVE_DATA_S &src, IVE_DATA_S &dst, int mode, int needBlock)
{

    IVE_HANDLE handle;
    IVE_DMA_CTRL_S ctrl;
    //1 byte 32x1～1920x1080
    if (src.u32Height < 1 || src.u32Height > 1080 || src.u32Width < 32 || src.u32Width > 1920)
    {
        printf("function: %s,line: %d,图像尺寸超出范围,需要为32x1～1920x1080,输入的为src w %d h %d s %d dst w %d h %d s %d\n",
               __FUNCTION__, __LINE__, src.u32Width, src.u32Height, src.u32Stride, dst.u32Width, dst.u32Height, dst.u32Stride);
        return false;
    }
    if (src.u64PhyAddr == 0 || src.u64VirAddr == 0 || dst.u64PhyAddr == 0 || dst.u64VirAddr == 0)
    {
        printf("function: %s,line: %d,图像地址不能为0。src phy: %x vir: %x dst phy: %x vir: %x\n",
               __FUNCTION__, __LINE__, src.u64PhyAddr, src.u64VirAddr, dst.u64PhyAddr, dst.u64VirAddr);
        return false;
    }
    switch ((IVE_DMA_MODE_E)mode)
    {
    case IVE_DMA_MODE_DIRECT_COPY:
        /* code */
        {
            if (src.u32Height != dst.u32Height || src.u32Width != dst.u32Width)
            {
                printf("图像宽高不符合要求，IVE_DMA_MODE_DIRECT_COPY下，分辨率应该相等\n");
                return false;
            }
            ctrl.enMode = IVE_DMA_MODE_DIRECT_COPY;
            int s32ret = HI_MPI_IVE_DMA(&handle, &src, &dst, &ctrl, (HI_BOOL)needBlock);
            CHECK_IVE_FUNCTION_RET("HI_MPI_IVE_DMA", s32ret, false);
            BLOCK_IVE_FUNCTION_RET(needBlock, handle, false);
            return true;
        }
    default:
        printf("未实现该模式\n");
        return false;
    }
    return true;
}

// IVE_DMA_MODE_DIRECT_COPY.并且只把一个图像从一处拷贝到另一处
void iveDMA(hisiImage &src1, hisiImage &dst, int mode, int needBlock)
{
    IVE_HANDLE dmaHandle;
    IVE_DATA_S ivesrc1 = src1.getIVEData();
    IVE_DATA_S ivedst = dst.getIVEData();
    IVE_DMA_CTRL_S ctrl;
    memset(&ctrl, 0, sizeof(IVE_DMA_CTRL_S));
    ctrl.enMode = (IVE_DMA_MODE_E)mode;
    int s32ret = HI_MPI_IVE_DMA(&dmaHandle, &ivesrc1, &ivedst, &ctrl, (HI_BOOL)needBlock);
    CHECK_IVE_FUNCTION("HI_MPI_IVE_DMA", s32ret);
    BLOCK_IVE_FUNCTION(needBlock, dmaHandle);
}

bool iveAnd(const hisiImage &src1, const hisiImage &src2, hisiImage &dst, int needBlock)
{
    if (src1.iveImg.enType != IVE_IMAGE_TYPE_U8C1 || src2.iveImg.enType != IVE_IMAGE_TYPE_U8C1 ||
        dst.iveImg.enType != IVE_IMAGE_TYPE_U8C1)
    {
        errorCode("type error", 0);
        return false;
    }
    if (src1.iveImg.u32Width < 64 || src2.iveImg.u32Width < 64 || dst.iveImg.u32Width < 64 ||
        src1.iveImg.u32Height > 1080 || src2.iveImg.u32Height > 1080 || dst.iveImg.u32Height > 1080)
    {
        errorCode("size transcend\n", 0);
        printf("src1 w h %d %d\n", src1.iveImg.u32Width, src1.iveImg.u32Height);
        printf("src2 w h %d %d\n", src2.iveImg.u32Width, src2.iveImg.u32Height);
        printf("dst w h %d %d\n", dst.iveImg.u32Width, dst.iveImg.u32Height);
        fflush(stdout);
        return false;
    }
    if (src1.iveImg.u32Width != dst.iveImg.u32Width || src1.iveImg.u32Height != dst.iveImg.u32Height ||
        src2.iveImg.u32Width != dst.iveImg.u32Width || src2.iveImg.u32Height != dst.iveImg.u32Height)
    {
        errorCode("src1 dst size !=", 0);
        return false;
    }
    IVE_HANDLE handle;
    IVE_IMAGE_S ivesrc1 = src1.getIVEImage();
    IVE_IMAGE_S ivesrc2 = src2.getIVEImage();
    IVE_IMAGE_S ivesrc3 = dst.getIVEImage();
    int s32ret = HI_MPI_IVE_And(&handle, &ivesrc1, &ivesrc2, &ivesrc3, (HI_BOOL)needBlock);
    CHECK_IVE_FUNCTION_RET("HI_MPI_IVE_And", s32ret, false);
    BLOCK_IVE_FUNCTION(needBlock, handle);
    return true;
}

bool IVEAnd(const hisiImage &src1, const hisiImage &src2, hisiImage &dst, HI_BOOL needblock)
{
    if (src1.iveImg.enType != IVE_IMAGE_TYPE_U8C1 || src2.iveImg.enType != IVE_IMAGE_TYPE_U8C1 ||
        dst.iveImg.enType != IVE_IMAGE_TYPE_U8C1)
    {
        errorCode("type error", 0);
        return false;
    }
    if (src1.iveImg.u32Width < 64 || src2.iveImg.u32Width < 64 || dst.iveImg.u32Width < 64 ||
        src1.iveImg.u32Height > 1080 || src2.iveImg.u32Height > 1080 || dst.iveImg.u32Height > 1080)
    {
        errorCode("size transcend\n", 0);
        printf("src1 w h %d %d\n", src1.iveImg.u32Width, src1.iveImg.u32Height);
        printf("src2 w h %d %d\n", src2.iveImg.u32Width, src2.iveImg.u32Height);
        printf("dst w h %d %d\n", dst.iveImg.u32Width, dst.iveImg.u32Height);
        return false;
    }

    IVE_HANDLE handle;
    IVE_IMAGE_S ivesrc1 = src1.getIVEImage();
    IVE_IMAGE_S ivesrc2 = src2.getIVEImage();
    IVE_IMAGE_S ivesrc3 = dst.getIVEImage();
    int s32ret = HI_MPI_IVE_And(&handle, &ivesrc1, &ivesrc2, &ivesrc3, needblock);
    if (0 != s32ret)
    {
        printf("line:%d,HI_MPI_IVE_And error：%x\n", s32ret);
        return false;
    }
    if (needblock)
    {
        HI_BOOL finish;
        HI_BOOL block = HI_TRUE;
        do
        {
            s32ret = HI_MPI_IVE_Query(handle, &finish, block);
            if (0 != s32ret)
            {
                printf("line:%d,HI_MPI_IVE_And error：%x\n", s32ret);
                return false;
            }
        } while (!finish);
    }
    return true;
}
void iveAdd(const hisiImage &src1, const hisiImage &src2, hisiImage &dst, const unsigned short x, const unsigned short y, const int bInstant)
{
    if (src1.iveImg.enType != IVE_IMAGE_TYPE_U8C1 || src2.iveImg.enType != IVE_IMAGE_TYPE_U8C1 || dst.iveImg.enType != IVE_IMAGE_TYPE_U8C1)
    {
        errorCode("iveAdd input image format error", 0);
        return;
    }
    IVE_ADD_CTRL_S pstAddCtrl;
    pstAddCtrl.u0q16X = x;
    pstAddCtrl.u0q16Y = y;
    IVE_HANDLE handle;
    IVE_IMAGE_S img1 = src1.getIVEImage();
    IVE_IMAGE_S img2 = src2.getIVEImage();
    IVE_IMAGE_S img3 = dst.getIVEImage();
    int s32ret = HI_MPI_IVE_Add(&handle, &img1, &img2, &img3, &pstAddCtrl, (HI_BOOL)bInstant);
    CHECK_IVE_FUNCTION("HI_MPI_IVE_Add", s32ret);
    BLOCK_IVE_FUNCTION(bInstant, handle);
}
bool iveAdd(const hisiImage &src1, const hisiImage &src2, hisiImage &dst, const unsigned short x, const unsigned short y, const HI_BOOL bInstant)
{
    if (src1.iveImg.enType != IVE_IMAGE_TYPE_U8C1 || src2.iveImg.enType != IVE_IMAGE_TYPE_U8C1 || dst.iveImg.enType != IVE_IMAGE_TYPE_U8C1)
    {
        errorCode("iveAdd input image format error", 0);
        return false;
    }
    if (src1.iveImg.u32Width != dst.iveImg.u32Width || src1.iveImg.u32Height != dst.iveImg.u32Height ||
        src2.iveImg.u32Height != dst.iveImg.u32Height || src2.iveImg.u32Width != dst.iveImg.u32Width)
    {
        errorCode("siez error src dst !=", 0);
        return false;
    }
    IVE_ADD_CTRL_S pstAddCtrl;
    pstAddCtrl.u0q16X = x;
    pstAddCtrl.u0q16Y = y;
    IVE_HANDLE handle;
    IVE_IMAGE_S img1 = src1.getIVEImage();
    IVE_IMAGE_S img2 = src2.getIVEImage();
    IVE_IMAGE_S img3 = dst.getIVEImage();
    int s32ret = HI_MPI_IVE_Add(&handle, &img1, &img2, &img3, &pstAddCtrl, bInstant);
    if (0 != s32ret)
    {
        printf("HI_MPI_IVE_Add error occured,code:%x\n", s32ret);
        return false;
    }
    if (bInstant)
    {
        HI_BOOL finish;
        HI_BOOL block = HI_TRUE;
        do
        {
            s32ret = HI_MPI_IVE_Query(handle, &finish, block);
            if (0 != s32ret)
            {
                printf("line:%d,HI_MPI_IVE_And error：%x\n", s32ret);
                return false;
            }
        } while (!finish);
    }
    return true;
}
void iveSub(const hisiImage &hisrc1, const hisiImage &hisrc2, hisiImage &hidst, int mode, int needBlock)
{
    if (hisrc1.iveImg.enType != IVE_IMAGE_TYPE_U8C1 || hisrc2.iveImg.enType != IVE_IMAGE_TYPE_U8C1)
    {
        errorCode("iveSub input image format error", 0);
        return;
    }
    if (mode == 0 && hidst.iveImg.enType != IVE_IMAGE_TYPE_U8C1)
    {
        errorCode("iveSub dst image format error", 0);
        return;
    }
    if (mode == 1 && hidst.iveImg.enType != IVE_IMAGE_TYPE_S8C1)
    {
        errorCode("iveSub dst image format error", 0);
        return;
    }

    IVE_IMAGE_S src1 = hisrc1.getIVEImage();
    IVE_IMAGE_S src2 = hisrc2.getIVEImage();
    IVE_IMAGE_S dst = hidst.getIVEImage();
    IVE_SUB_CTRL_S ctrl;
    ctrl.enMode = (IVE_SUB_MODE_E)mode;
    IVE_HANDLE subHandle;
    int s32Ret = HI_MPI_IVE_Sub(&subHandle, &src1, &src2, &dst, &ctrl, (HI_BOOL)needBlock);
    CHECK_IVE_FUNCTION("HI_MPI_IVE_Sub", s32Ret);
    BLOCK_IVE_FUNCTION(needBlock, subHandle);
}
// 不需要的输出可以是空图像
// format,src: u8c1;dstH,dstV:s16c1
//
void iveSobel(const hisiImage &src, hisiImage &dstH, hisiImage &dstV, signed char mask[25], int mode, int needblock)
{
    CHECK_HISIIMAGE(src, 1920, 1024);
    if (src.iveImg.enType != IVE_IMAGE_TYPE_U8C1 || (dstH.iveImg.enType != IVE_IMAGE_TYPE_S16C1 && dstV.iveImg.enType != IVE_IMAGE_TYPE_S16C1))
    {
        errorCode("iveSobel input image format error", 0);
        return;
    }
    IVE_IMAGE_S stSrc = src.getIVEImage();
    IVE_IMAGE_S stDstH = dstH.getIVEImage();
    IVE_IMAGE_S stDstV = dstV.getIVEImage();
    IVE_HANDLE iveHandle;
    IVE_SOBEL_CTRL_S ctrl;
    memcpy(ctrl.as8Mask, mask, sizeof(HI_S8) * 25);
    ctrl.enOutCtrl = (IVE_SOBEL_OUT_CTRL_E)mode;
    int s32Result = HI_MPI_IVE_Sobel(&iveHandle, &stSrc, IVE_SOBEL_OUT_CTRL_VER != ctrl.enOutCtrl ? &stDstH : HI_NULL, IVE_SOBEL_OUT_CTRL_HOR != ctrl.enOutCtrl ? &stDstV : HI_NULL, &ctrl, (HI_BOOL)needblock);
    CHECK_IVE_FUNCTION("HI_MPI_IVE_Sobel", s32Result);
    BLOCK_IVE_FUNCTION(needblock, iveHandle);
}
void iveSobel(const hisiImage &src, hisiImage &dstH, hisiImage &dstV, IVE_SOBEL_CTRL_S &ctrl, HI_BOOL needblock)
{
    IVE_SRC_IMAGE_S stSrc = src.getIVEImage();
    IVE_DST_IMAGE_S stDstH = dstH.getIVEImage();
    IVE_DST_IMAGE_S stDstV = dstV.getIVEImage();
    IVE_HANDLE handle;
    int s32Result = HI_MPI_IVE_Sobel(&handle, &stSrc, IVE_SOBEL_OUT_CTRL_VER != ctrl.enOutCtrl ? &stDstH : HI_NULL, IVE_SOBEL_OUT_CTRL_HOR != ctrl.enOutCtrl ? &stDstV : HI_NULL, &ctrl, needblock);
    if (0 != s32Result)
    {
        printf("line%d HI_MPI_IVE_Sobel:%x\n", __LINE__, s32Result);
        return;
    }
    if (needblock)
    {
        HI_BOOL finish = (HI_BOOL)0;
        HI_BOOL block = (HI_BOOL)1;
        do
        {
            HI_MPI_IVE_Query(handle, &finish, block);
            // printf("延时\n");
            // usleep(5);
        } while (!finish);
    }
}

/**
     * @brief 
     * 
     * @param src 
     * @param dst   
     * @param thrV 
     * @param minV 
     * @param maxV 
     * @param mode 
     * @param needBlock 
     */
void iveThresh(const hisiImage &src, hisiImage &dst, unsigned char thrV, unsigned char minV, unsigned char maxV, int mode, int needBlock)
{
    IVE_THRESH_CTRL_S threshCtrl;
    threshCtrl.enMode = (IVE_THRESH_MODE_E)mode;
    threshCtrl.u8LowThr = thrV;
    threshCtrl.u8MinVal = minV;
    threshCtrl.u8MaxVal = maxV;
    IVE_HANDLE threshHandle;
    IVE_IMAGE_S iveSrc = src.getIVEImage();
    IVE_IMAGE_S iveDst = dst.getIVEImage();
    int s32ret = HI_MPI_IVE_Thresh(&threshHandle, &iveSrc, &iveDst, &threshCtrl, (HI_BOOL)needBlock);
    CHECK_IVE_FUNCTION("HI_MPI_IVE_Thresh", s32ret);
    BLOCK_IVE_FUNCTION(needBlock, threshHandle);
}

/*控制参数太多，先保留ive类型的控制参数*/
void iveThreshS16(const hisiImage &src, hisiImage &dst, IVE_THRESH_S16_CTRL_S &ctrl, HI_BOOL needblock)
{
    if (src.iveImg.enType != IVE_IMAGE_TYPE_S16C1)
    {
        errorCode("iveThreshS16 input image format error\n input src must be IVE_IMAGE_TYPE_S16C1", 0);
        return;
    }
    IVE_IMAGE_S stSrc = src.getIVEImage();
    IVE_IMAGE_S stDst = dst.getIVEImage();
    IVE_HANDLE handle;
    int s32Result = HI_MPI_IVE_Thresh_S16(&handle, &stSrc, &stDst, &ctrl, needblock);
    if (0 != s32Result)
    {
        printf("\033[31m line %d HI_MPI_IVE_Sobel %x \n\033[0m", __LINE__, s32Result);
        return;
    }
    if (needblock)
    {
        HI_BOOL finish = (HI_BOOL)0;
        HI_BOOL block = (HI_BOOL)1;
        do
        {
            HI_MPI_IVE_Query(handle, &finish, block);
            // printf("延时\n");
            // usleep(5);
        } while (!finish);
    }
}

/**
     * @brief 根据不同的模板系数，实现不同的滤波
     * 
     */
void iveFilter(const hisiImage &hisrc, hisiImage &hidst, signed char mask[25], unsigned char norm, int needBlock)
{
    IVE_IMAGE_S src = hisrc.getIVEImage();
    CHECK_HISIIMAGE(src,1920,1024);
    IVE_IMAGE_S dst = hidst.getIVEImage();
    CHECK_HISIIMAGE(dst,1920,1024);
    IVE_HANDLE filterHandle;
    IVE_FILTER_CTRL_S ctrl;
    memcpy(ctrl.as8Mask, mask, 25);
    ctrl.u8Norm = norm;
    int s32Result = HI_MPI_IVE_Filter(&filterHandle, &src, &dst, &ctrl, (HI_BOOL)needBlock);
    CHECK_IVE_FUNCTION("HI_MPI_IVE_Filter", s32Result);
    BLOCK_IVE_FUNCTION(needBlock, filterHandle);
}

/**
     * @brief 
     * 
     * @param hisrc 
     * @param hidst 
     * @param mode 
     * @param needBlock 
     */
void iveOrdStatFilter(const hisiImage &hisrc, hisiImage &hidst, int mode, int needBlock)
{
    IVE_IMAGE_S src = hisrc.getIVEImage();
    IVE_IMAGE_S dst = hidst.getIVEImage();
    IVE_ORD_STAT_FILTER_CTRL_S ctrl;
    ctrl.enMode = (IVE_ORD_STAT_FILTER_MODE_E)mode;
    IVE_HANDLE osfHandle;
    int s32Ret = HI_MPI_IVE_OrdStatFilter(&osfHandle, &src, &dst, &ctrl, (HI_BOOL)needBlock);
    CHECK_IVE_FUNCTION("HI_MPI_IVE_OrdStatFilter", s32Ret);
    BLOCK_IVE_FUNCTION(needBlock, osfHandle);
}

/**
     * @brief 
     * 
     * @param hisrc 
     * @param hidst 
     * @param modex 
     * @param needBlock 
     */
bool iveCSC(const hisiImage &hisrc, hisiImage &hidst, int mode, int needBlock)
{

    CHECK_IVE_IMAGE_SIZE_RET(hisrc.iveImg.u32Width, hisrc.iveImg.u32Height, 64, 64, 1920, 1920, false);
    CHECK_IVE_IMAGE_SIZE_RET(hidst.iveImg.u32Width, hidst.iveImg.u32Height, 64, 64, 1920, 1920, false);
    switch ((IVE_CSC_MODE_E)mode)
    {
    case IVE_CSC_MODE_PIC_BT709_YUV2RGB:
    {
        CHECK_STRIDE(hisrc.iveImg.au32Stride[0], 16, false);
        CHECK_STRIDE(hisrc.iveImg.au32Stride[1], 16, false);
        CHECK_STRIDE(hidst.iveImg.au32Stride[0], 16, false);
        if (hidst.iveImg.enType == IVE_IMAGE_TYPE_U8C3_PLANAR)
        {
            CHECK_STRIDE(hidst.iveImg.au32Stride[1], 16, false);
            CHECK_STRIDE(hidst.iveImg.au32Stride[2], 16, false);
        }
        IVE_IMAGE_S ivesrc = hisrc.getIVEImage();
        IVE_IMAGE_S ivedst = hidst.getIVEImage();
        IVE_HANDLE cschandle;
        IVE_CSC_CTRL_S cscCtrl;
        cscCtrl.enMode = IVE_CSC_MODE_PIC_BT709_YUV2RGB;
        int s32ret = HI_MPI_IVE_CSC(&cschandle, &ivesrc, &ivedst, &cscCtrl, (HI_BOOL)needBlock);
        CHECK_IVE_FUNCTION_RET("HI_MPI_IVE_CSC", s32ret, false);
        BLOCK_IVE_FUNCTION_RET(needBlock, cschandle, false);
    }
    //需要check
    break;
    case IVE_CSC_MODE_PIC_BT709_RGB2YUV:
        printf("待完善的函数\n");
        break;
    default:
        printf("待完善的函数\n");
        break;
    }
}

/**
     * @brief 
     * 
     * @param src 
     * @param dst 
     * @param modex 
     * @param needBlock 
     * @return true 
     * @return false 
     */
#define IVE_INTEG_MINW 32
#define IVE_INTEG_MINH 16
#define IVE_INTEG_MAXW 1920
#define IVE_INTEG_MAXH 1080
bool iveInteg(const hisiImage &hisrc, hisiImage &hidst, int mode, int needBlock)
{
    CHECK_IVE_IMAGE_SIZE_RET(hisrc.iveImg.u32Width, hisrc.iveImg.u32Height,
                             IVE_INTEG_MINW, IVE_INTEG_MINH, IVE_INTEG_MAXW, IVE_INTEG_MAXH, false);
    CHECK_IVE_IMAGE_SIZE_RET(hidst.iveImg.u32Width, hidst.iveImg.u32Height,
                             IVE_INTEG_MINW, IVE_INTEG_MINH, IVE_INTEG_MAXW, IVE_INTEG_MAXH, false);
    CHECK_STRIDE(hisrc.iveImg.au32Stride[0], 16, fasle);
    CHECK_STRIDE(hidst.iveImg.au32Stride[0], 16, fasle);
    IVE_IMAGE_S ivesrc = hisrc.getIVEImage();
    IVE_IMAGE_S ivedst = hidst.getIVEImage();
    IVE_HANDLE integHandel;
    IVE_INTEG_CTRL_S ctrl;
    ctrl.enOutCtrl = (IVE_INTEG_OUT_CTRL_E)mode;
    switch (IVE_INTEG_OUT_CTRL_E(mode))
    {
    case IVE_INTEG_OUT_CTRL_SUM:
    {
        int s32ret = HI_MPI_IVE_Integ(&integHandel, &ivesrc, &ivedst, &ctrl, (HI_BOOL)needBlock);
        CHECK_IVE_FUNCTION_RET("HI_MPI_IVE_CSC", s32ret, false);
        BLOCK_IVE_FUNCTION_RET(needBlock, integHandel, false);
    }
    break;
    default:
    {
        printf("该计算方式未实现\n");
        printf("已经实现：IVE_INTEG_OUT_CTRL_SUM\n");
    }
    break;
    }
    return true;
}

#define IVE_16BitTo8Bit_MINW 16
#define IVE_16BitTo8Bit_MINH 16
#define IVE_INTEG_MAXW 1920
#define IVE_INTEG_MAXH 1080
bool ive16BitTo8Bit(const hisiImage &hisisrc, hisiImage &hisidst, int mode, int needblock, unsigned short u16Denominator, unsigned char u8Numerator, signed char s8bias)
{
    if (s8bias < -128 || s8bias > 127)
    {
        printf("s8bias out of range\n");
        return false;
    }
    if (u8Numerator < 0 || u8Numerator > 255)
    {
        printf("u8Numerator out of range\n");
        return false;
    }
    if (((u16Denominator < u8Numerator) && (u16Denominator < 1)) || u16Denominator > 65535)
    {
        printf("u16Denominator out of range\n");
        return false;
    }
    if (mode > 3)
    {
        printf("mode out of range\n");
        return false;
    }
    if (hisisrc.iveImg.au32Stride[0] % 2 != 0)
    {
        printf("Address is not aligned with 2");
        return false;
    }
    if ((hisisrc.iveImg.enType != IVE_IMAGE_TYPE_S16C1 && hisisrc.iveImg.enType != IVE_IMAGE_TYPE_U16C1) ||
        (hisidst.iveImg.enType != IVE_IMAGE_TYPE_S8C1 && hisidst.iveImg.enType != IVE_IMAGE_TYPE_U8C1))
    {
        printf("image format error\n");
        return false;
    }
    CHECK_IVE_IMAGE_SIZE_RET(hisisrc.iveImg.u32Width, hisisrc.iveImg.u32Height,
                             IVE_16BitTo8Bit_MINW, IVE_16BitTo8Bit_MINH, IVE_INTEG_MAXW, IVE_INTEG_MAXH, false);
    CHECK_IVE_IMAGE_SIZE_RET(hisidst.iveImg.u32Width, hisidst.iveImg.u32Height,
                             IVE_16BitTo8Bit_MINW, IVE_16BitTo8Bit_MINH, IVE_INTEG_MAXW, IVE_INTEG_MAXH, false);
    IVE_IMAGE_S ivesrc = hisisrc.getIVEImage();
    IVE_IMAGE_S ivedst = hisidst.getIVEImage();
    IVE_16BIT_TO_8BIT_CTRL_S ctrl;
    ctrl.enMode = (IVE_16BIT_TO_8BIT_MODE_E)mode;
    ctrl.s8Bias = s8bias;
    ctrl.u16Denominator = u16Denominator;
    ctrl.u8Numerator = u8Numerator;
    IVE_HANDLE handle;
    int s32ret = HI_MPI_IVE_16BitTo8Bit(&handle, &ivesrc, &ivedst, &ctrl, (HI_BOOL)needblock);
    CHECK_IVE_FUNCTION_RET("HI_MPI_IVE_16BitTo8Bit", s32ret, false);
    BLOCK_IVE_FUNCTION_RET(needblock, handle, false);
    return true;
}
