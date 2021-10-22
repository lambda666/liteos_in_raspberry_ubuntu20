/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Main Process
 * Author: Huawei LiteOS Team
 * Create: 2021-09-14
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

#include "main.h"
#include "usart.h"
#include "los_task_pri.h"
#include "arch/canary.h"

STATIC VOID WdtDisable(VOID)
{
    if (REG32_READ(RTC_WDT_PROTECT) != WDT_WRWITE_MASK) {
        REG32_WRITE(RTC_WDT_PROTECT, WDT_WRWITE_MASK);
    }
    REG32_WRITE(RTC_WDT_FEED, (1 << RTC_CNTL_WDT_FEED_BIT));
    REG32_WRITE(RTC_WDT_CFG0, ~(1 << RTC_CNTL_WDT_FLASHBOOT_MOD_EN));
    REG32_WRITE(RTC_WDT_CFG0, ~(1 << RTC_CNTL_WDT_FLASHBOOT_MOD_EN));
    REG32_WRITE(RTC_WDT_PROTECT, 0);
}

STATIC VOID BssClean(VOID)
{
    memset_s(&_sbss, (&_ebss - &_sbss), 0, (&_ebss - &_sbss));
}

INT32 main(VOID)
{
    BssClean();
    WdtDisable();

    __asm__ __volatile__("mov sp, %0\n" : : "r"(&__init_stack_e));

#ifdef __GNUC__
    ArchStackGuardInit();
#endif
    OsSetMainTask();
    OsCurrTaskSet(OsGetMainTask());

    PRINT_RELEASE("\n********Hello Huawei LiteOS********\n"
                  "\nLiteOS Kernel Version : %s\n"
                  "build data : %s %s\n\n"
                  "**********************************\n",
                  HW_LITEOS_KERNEL_VERSION_STRING, __DATE__, __TIME__);

    UINT32 ret = OsMain();
    if (ret != LOS_OK) {
        return LOS_NOK;
    }

    OsStart();

    return LOS_OK;
}
