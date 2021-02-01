#ifndef __HISIIMAGE_H__
#define __HISIIMAGE_H__

#define IVE_MMZ_FREE(phy,vir)\
    do{\
        if ((0 != (phy)) && (0 != (vir)))\
        {\
            HI_MPI_SYS_MmzFree((phy),(HI_VOID *)(HI_UL)(vir));\
            (phy) = 0;\
            (vir) = 0;\
        }\
    }while(0)


#endif