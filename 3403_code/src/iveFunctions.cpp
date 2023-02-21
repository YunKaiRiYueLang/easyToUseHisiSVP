
// #define hi_3403
// #define Debug
#ifdef hi_3403
#include "ot_common_ive.h"
#include "ot_common_svp.h"
#include "ss_mpi_ive.h"
#endif
#include <cstdio>

/*******************for debug***************************************/
#ifdef Debug
#define errorCode(msg, index) printf("\033[31m function:%s line:%d %s code:%x \n\033[0m", __FUNCTION__, __LINE__, msg, index)
#define CHECK_ALIGN(stride, align)                           \
    if (stride % align != 0)                                 \
    {                                                        \
        errorCode("csc image stride must align with 16", 0); \
    }
#define CHECK_IVE_FUNCTION(name, code) \
    if (0 != code)                     \
    {                                  \
        errorCode(name, code);         \
        return;                        \
    }
/*---*/
#define CHECK_ALIGN3(IMG, align)          \
    CHECK_ALIGN(IMG.phys_addr[0], align); \
    CHECK_ALIGN(IMG.phys_addr[1], align); \
    CHECK_ALIGN(IMG.phys_addr[2], align);

#define CHECK_SIZE(src, w1, h1, w2, h2)                                         \
    if (src.height < h1 || src.height > h2 || src.width < w1 || src.width > w2) \
    {                                                                           \
        printf("src image with error size %d %d\n", src.width, src.height);     \
        return false;                                                           \
    }

#else
#define CHECK_ALIGN(stride, align)
#define CHECK_ALIGN3(IMG, align)
#define CHECK_SIZE(src, w1, h1, w2, h2)
#define CHECK_IVE_FUNCTION(name, code)
#endif
/*******************for debug***************************************/

#define BLOCK_IVE_FUNCTION(condition, handle)         \
    if (condition)                                    \
    {                                                 \
        td_bool finish = (td_bool)0;                  \
        td_bool block = (td_bool)1;                   \
        do                                            \
        {                                             \
            ss_mpi_ive_query(handle, &finish, block); \
        } while (!finish);                            \
    }

const unsigned int align16 = 16;
//
#ifdef hi_3403
bool iveCsc(ot_svp_img &src, ot_svp_img &dst, const int mode, const int needBlock)
{
    CHECK_ALIGN3(src, align16);
    CHECK_ALIGN3(dst, align16);
    CHECK_SIZE(src, 16, 16, 4096, 4096);
    CHECK_SIZE(dst, 16, 16, 4096, 4096);
#ifdef Debug
    if (src.type != OT_SVP_IMG_TYPE_YUV420SP && src.type != OT_SVP_IMG_TYPE_YUV422SP &&
        src.type != OT_SVP_IMG_TYPE_U8C3_PLANAR && src.type != OT_SVP_IMG_TYPE_U8C3_PACKAGE)
    {
        errorCode("input image src with error formart\n", 0);
    }
    if (dst.type != OT_SVP_IMG_TYPE_YUV420SP && dst.type != OT_SVP_IMG_TYPE_YUV422SP &&
        dst.type != OT_SVP_IMG_TYPE_U8C3_PLANAR && dst.type != OT_SVP_IMG_TYPE_U8C3_PACKAGE)
    {
        errorCode("input image dst with error formart\n", 0);
    }
    if (mode < 0 || mode > 11)
    {
        errorCode("csc mode error", 0);
    }
#endif
    ot_ive_csc_ctrl ctrl;
    ctrl.mode = static_cast<ot_ive_csc_mode>(mode);
    int handle = 0;
    int s32Result = ss_mpi_ive_csc(&handle, &src, &dst, &ctrl, td_bool(needBlock));
    CHECK_IVE_FUNCTION("ss_mpi_ive_csc", s32Result);
    BLOCK_IVE_FUNCTION(needBlock, handle);
}
// transform hisi yuv (BT 709 :420 sp / 422 sp ) to rgb (0~255) package;
// vir_addr  phy_addr  w h s 
bool yuv2rgbIve(ot_video_frame_info *frame_info, void *rgb)
{
    
    hisiImage hisiSrc(*frame_info);
    hisiImage hisiDst;
    bool ret = false;
    hisiDst.create(frame_info->video_frame.width, frame_info->video_frame.height, OT_SVP_IMG_TYPE_U8C3_PACKAGE);
    if (hisiSrc.iveImg.type == OT_SVP_IMG_TYPE_YUV420SP || hisiSrc.iveImg.type == OT_SVP_IMG_TYPE_YUV422SP)
    {
        ret = iveCsc(hisiSrc, hisiDst, 3, 1);
    }
    else
    {
        errorCode("do not support the pixel format", 0);
    }
    if (ret)
    {
        memcpy(rgb,(void*)hisiDst.iveImg.virt_addr[0],hisiDst.iveImg.stride[0]*hisiDst.iveImg.height*3);
    }
    return true;
}
/**
 * @brief
 *
 * @param src
 * @param dst
 * @param mask kernel value
 * @param norm normalization value
 * @param isInstant  designate 0 or 1
 * @return true
 * @return false
 */
bool iveFilter(hisiImage &src, hisiImage &dst, signed char mask[25], unsigned char norm, int isInstant)
{
    ot_svp_img iveSrc = src.getIVEImage();
    ot_svp_img iveDst = dst.getIVEImage();
    if (iveSrc.type != OT_SVP_IMG_TYPE_U8C1 && iveSrc.type != OT_SVP_IMG_TYPE_YUV420SP && iveSrc.type != OT_SVP_IMG_TYPE_YUV422SP)
    {
        errorCode("\nsrc image type error,type must be u8c1,yuv420sp,yuv422sp\n", -1);
    }
    if (iveSrc.type != iveDst.type)
    {
        errorCode("\nerror: src iamge type != dst image type\n", -2);
    }
    CHECK_SIZE(iveSrc, 64, 64, 1920, 1024);
    CHECK_SIZE(iveDst, 64, 64, 1920, 1024);
    if (iveSrc.width != iveDst.width || iveSrc.height != iveDst.height)
    {
        errorCode("\nsrc dst must have the same image size\n", -3);
    }
    int handle;
    ot_ive_filter_ctrl ctrl;
    ctrl.norm = norm;
    memcpy(ctrl.mask, mask, 25);
    int retCode = 0;
    retCode = ss_mpi_ive_filter(&handle, &iveSrc, &iveDst, &ctrl, static_cast<td_bool>(isInstant));
    CHECK_IVE_FUNCTION_RET("ss_mpi_ive_filter", retCode, false);
    BLOCK_IVE_FUNCTION(isInstant, handle);
    return true;
}

bool iveCanny(hisiImage &src, hisiImage &dst, signed char mask[25], int high_th = 150, int low_th = 50, int isInstant = 0)
{
    ot_svp_img svpSrc = src.getIVEImage();
    ot_svp_img svpDst = dst.getIVEImage();
    CHECK_SIZE(svpSrc, 64, 64, 1920, 1024);
    CHECK_SIZE(svpDst, 64, 64, 1920, 1024);
    if (svpSrc.type != OT_SVP_IMG_TYPE_U8C1)
    {
        errorCode("\nsrc image type error,type must be u8c1\n", -1);
    }
    if (svpSrc.type != svpDst.type)
    {
        errorCode("\nerror: src iamge type != dst image type\n", -2);
    }
    ot_svp_mem_info stack;
    stack.size = svpSrc.width * svpSrc.height * sizeof(ot_svp_point_u16) + sizeof(ot_ive_canny_stack_size);
    stack.size = HI_CalcStride(stack.size, 16); //
    int ret = ss_mpi_sys_mmz_alloc(&(stack.phys_addr), (void **)&(stack.virt_addr),
                                   TD_NULL, TD_NULL, stack.size);
    if (ret != 0)
    {
        errorCode("ss_mpi_sys_mmz_alloc", ret);
        return false;
    }
    ot_ive_canny_hys_edge_ctrl ctrl;
    ctrl.mem.size = HI_CalcStride(svpSrc.width * svpSrc.height * 3, 16);
    ret = ss_mpi_sys_mmz_alloc(&(ctrl.mem.phys_addr), (void **)&(ctrl.mem.virt_addr),
                               TD_NULL, TD_NULL, ctrl.mem.size);
    if (ret != 0)
    {
        errorCode("ss_mpi_sys_mmz_alloc", ret);
        return false;
    }
    int handle;
    ctrl.high_threshold = high_th;
    ctrl.low_threshold = low_th;
    memcpy(ctrl.mask, mask, 25);
    ret = ss_mpi_ive_canny_hys_edge(&handle, &svpSrc, &svpDst, &stack, &ctrl, static_cast<td_bool>(isInstant));
    BLOCK_IVE_FUNCTION(isInstant, handle);
    CHECK_IVE_FUNCTION_RET("ss_mpi_ive_canny_hys_edge", ret, false);

    ret = ss_mpi_ive_canny_edge(&svpDst, &stack);
    CHECK_IVE_FUNCTION_RET("ss_mpi_ive_canny_edge", ret, false);
    ss_mpi_sys_mmz_free((td_phys_addr_t)(ctrl.mem.phys_addr), (td_void *)(td_uintptr_t)(ctrl.mem.virt_addr));
    ss_mpi_sys_mmz_free((td_phys_addr_t)(stack.phys_addr), (td_void *)(td_uintptr_t)(stack.virt_addr));
}

// note:
// #define STB_IMAGE_WRITE_IMPLEMENTATION
// #include "stb_image_write.h"
void MediaPatrol(int roiWidth, int roiHeight, uint8_t *yuvBuffer, uint8_t **dstEdgeBuffer, int high_th = 150, int low_th = 50)
{
    hisiImage src;
    src.create(roiWidth, roiHeight, OT_SVP_IMG_TYPE_U8C1);
    hisiImage dst;
    dst.create(roiWidth, roiHeight, OT_SVP_IMG_TYPE_U8C1);
    signed char ps8Mask[25] = {0, 0, 0, 0, 0,
                               0, -1, 0, 1, 0,
                               0, -2, 0, 2, 0,
                               0, -1, 0, 1, 0,
                               0, 0, 0, 0, 0};
    signed char mask[25] = {0, 0, 0, 0, 0,
                            0, 1, 2, 1, 0,
                            0, 2, 4, 2, 0,
                            0, 1, 2, 1, 0,
                            0, 0, 0, 0, 0};
    int size = dst.iveImg.height * dst.iveImg.width;//64*64
    memcpy((void*)src.iveImg.virt_addr[0],yuvBuffer,size);                       
    iveFilter(src, dst, mask, 4, 0);
    iveCanny(src, dst, ps8Mask, high_th, low_th, 1);
    *dstEdgeBuffer = (uint8_t *)malloc(size);
    memcpy(*dstEdgeBuffer, (void *)(dst.iveImg.virt_addr[0]), size);
    // stbi_write_bmp("/mnt/wbj/src.bmp",src.iveImg.stride[0],src.iveImg.height,1,(void*)src.iveImg.virt_addr[0]);
    // stbi_write_bmp("/mnt/wbj/edge.bmp",dst.iveImg.stride[0],dst.iveImg.height,1,*dstEdgeBuffer);
}

#endif