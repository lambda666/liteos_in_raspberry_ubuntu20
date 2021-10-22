/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Mux Test Case
 * Author: Huawei LiteOS Team
 * Create: 2021-06-02
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
 * HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ---------------------------------------------------------------------------
 */

#include "it_los_mux.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

static VOID TaskF01(VOID)
{
    UINT32 ret;

    ICUNIT_ASSERT_EQUAL_VOID(g_testCount, 1, g_testCount);

    ret = LOS_MuxPend(g_mutexTest, LOS_WAIT_FOREVER);
    ICUNIT_ASSERT_EQUAL_VOID(ret, LOS_OK, ret);

    g_testCount++;

    ret = LOS_MuxPost(g_mutexTest);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT);

    ICUNIT_GOTO_EQUAL(g_testCount, 2, g_testCount, EXIT); // g_testCount assert equal 2
    g_testCount++;

    (VOID)LOS_TaskDelay(50); // delay 50 ticks
    return;
EXIT:
    (VOID)LOS_MuxPost(g_mutexTest);
    return;
}

VOID TaskF02(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S task1;

    g_testCount++;

    task1.pfnTaskEntry = (TSK_ENTRY_FUNC)TaskF01;
    task1.pcName       = "ItLosMux005_2";
    task1.uwStackSize  = TASK_STACK_SIZE_TEST;
    task1.usTaskPrio   = TASK_PRIO_TEST - 3; // TASK_PRIO_TEST - 3 has higher priority than TASK_PRIO_TEST
    task1.uwResved = 0;
#ifdef LOSCFG_KERNEL_SMP
    task1.usCpuAffiMask = CPUID_TO_AFFI_MASK(ArchCurrCpuid());
#endif
    ret = LOS_TaskCreate(&g_testTaskID02, &task1);
    ICUNIT_ASSERT_EQUAL_VOID(ret, LOS_OK, ret);

    (VOID)LOS_TaskDelay(1);

    ret = LOS_MuxPend(g_mutexTest, LOS_WAIT_FOREVER);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT);

    ret = LOS_MuxPost(g_mutexTest);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT1);
    ICUNIT_GOTO_EQUAL(g_testCount, 3, g_testCount, EXIT); // g_testCount assert equal 3
    g_testCount++;
    return;
EXIT1:
    (VOID)LOS_MuxPost(g_mutexTest);
EXIT:
    return;
}

static UINT32 TestCase(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S stTask = {0};

    g_testCount = 0;

    ret = LOS_MuxCreate(&g_mutexTest);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);

    stTask.pfnTaskEntry = (TSK_ENTRY_FUNC)TaskF02;
    stTask.usTaskPrio   = (TASK_PRIO_TEST - 1);
    stTask.pcName       = "ItLosMux005_1";
    stTask.uwStackSize  = TASK_STACK_SIZE_TEST;
    stTask.uwResved = 0;
#ifdef LOSCFG_KERNEL_SMP
    stTask.usCpuAffiMask = CPUID_TO_AFFI_MASK(ArchCurrCpuid());
#endif
    ret = LOS_TaskCreate(&g_testTaskID01, &stTask);
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT);

    (VOID)LOS_TaskDelay(1);
    TestExtraTaskDelay(10); // delay 10 ticks
    ICUNIT_GOTO_EQUAL(g_testCount, 4, g_testCount, EXIT1); // g_testCount assert equal 4
EXIT1:
    (VOID)LOS_TaskDelete(g_testTaskID02);
    ret = LOS_TaskDelete(g_testTaskID01);
#if LOSCFG_COMPAT_POSIX
    ICUNIT_GOTO_EQUAL(ret, LOS_OK, ret, EXIT);
#endif
EXIT:
    ret = LOS_MuxDelete(g_mutexTest);
    ICUNIT_ASSERT_EQUAL(ret, LOS_OK, ret);
    return LOS_OK;
}

/**
 * @ingroup TEST_MUX
 * @par TestCase_Number
 * ItLosMux005
 * @par TestCase_TestCase_Type
 * Function test
 * @brief Mux API function test.
 * @par TestCase_Pretreatment_Condition
 * The LiteOS is running properly.
 * @par TestCase_Test_Steps
 * The Pend and Post tasks of different priorities have the same mutex.
 * @par TestCase_Expected_Result
 * test passed
 * @par TestCase_Level
 * Level 0
 * @par TestCase_Automated
 * true
 * @par TestCase_Remark
 * null
 */

VOID ItLosMux005(VOID)
{
    TEST_ADD_CASE("ItLosMux005", TestCase, TEST_LOS, TEST_MUX, TEST_LEVEL0, TEST_FUNCTION);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
