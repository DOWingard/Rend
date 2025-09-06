#ifndef LS_C_COMPANY_H
#define LS_C_COMPANY_H

#include "../APIDef.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    LS_API typedef struct LSCompany
    {
        uint64_t (*getId)(struct LSCompany *self);

        void *inner;

    } LSCompany;

    LS_API LSCompany *CreateLSCompany();

    LS_API LSCompany *CreateLSCompanyEx(uint64_t id);

    LS_API void FreeLSCompany(LSCompany *company);

#ifdef __cplusplus
} // end extern "C"
#endif

#endif // LS_C_COMPANY_H
