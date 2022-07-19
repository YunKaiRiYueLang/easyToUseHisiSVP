
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
#endif