/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Xtensa Lx6 Canary Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-09-07
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --------------------------------------------------------------------------- */

#include "arch/canary.h"
#include "arch/regs.h"
#include "stdlib.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifdef __GNUC__
/* stack protector */
UINTPTR __stack_chk_guard = 0x000a0dff;

STATIC UINT32 GetCcount(VOID)
{
    UINT32 intSave;

    __asm__ __volatile__("rsr %0, ccount" : "=a"(intSave) :);

    return intSave;
}
/*
 * If the SP compiling options:-fstack-protector-strong or -fstack-protector-all is enabled,
 * We recommend to implement true random number generator function for __stack_chk_guard
 * value to replace the function implementation template shown as below.
 */
#pragma GCC push_options
#pragma GCC optimize ("-fno-stack-protector")
LITE_OS_SEC_TEXT_INIT WEAK VOID ArchStackGuardInit(VOID)
{
    int rnd;
    UINT32 seed;

    seed = GetCcount();
    srand(seed);
    rnd = rand();
    __stack_chk_guard = (UINTPTR)rnd;
}
#pragma GCC pop_options
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */