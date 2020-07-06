#ifndef _PDCLIB_TSS_H
#define _PDCLIB_TSS_H _PDCLIB_TSS_H

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

extern tss_dtor_t tss_dtors[FLS_MAXIMUM_AVAILABLE];

#ifdef __cplusplus
}
#endif

#endif
