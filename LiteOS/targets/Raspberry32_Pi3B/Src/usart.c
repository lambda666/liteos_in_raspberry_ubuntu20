/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Usart Init Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-07-19
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

#include <los_hwi.h>
#include "usart.h"
#include "register_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

MINIUART_INFO *miniUart;

#define GPIO_MODE_INPUT     0
#define GPIO_MODE_OUTPUT    1
#define GPIO_ALT_FUNC0      4
#define GPIO_ALT_FUNC1      5
#define GPIO_ALT_FUNC2      6
#define GPIO_ALT_FUNC3      7
#define GPIO_ALT_FUNC4      3
#define GPIO_ALT_FUNC5      2
#define GPIO_FSEL_MASK      0x7
#define MINI_UART_TX        14
#define MINI_UART_RX        15
#define PER_GPFSEL_GPIONUM  10
#define GPIO_FSEL_BITNUM    3

STATIC VOID MiniUartInit(VOID)
{
    UINT32 value;
    GPIO_INFO *gpio = GPIO_REG_BASE;

    value = gpio->GPFSEL[1];
    value &= ~(GPIO_FSEL_MASK << ((MINI_UART_TX % PER_GPFSEL_GPIONUM) * GPIO_FSEL_BITNUM));
    value |= GPIO_ALT_FUNC5 << ((MINI_UART_TX % PER_GPFSEL_GPIONUM) * GPIO_FSEL_BITNUM) ;
    value &= ~(GPIO_FSEL_MASK << ((MINI_UART_RX % PER_GPFSEL_GPIONUM) * GPIO_FSEL_BITNUM));
    value |= GPIO_ALT_FUNC5 << ((MINI_UART_RX % PER_GPFSEL_GPIONUM) * GPIO_FSEL_BITNUM) ;
    gpio->GPFSEL[1] = value;

    gpio->GPPUD = 0;
    gpio->GPPUDCLK[0] = (1 << MINI_UART_TX) | (1 << MINI_UART_RX);
    gpio->GPPUDCLK[0] = 0;

    miniUart = MINI_UART;
    *((volatile UINT32 *)(AUX_ENABLES)) |= 1;   /* Mini UART enable */
    miniUart->LCR = 3;    /* UART works in 8-bit mode */
    miniUart->IIR = 0;    /* disable receive interrupt,transmit interrupt */
    miniUart->CNTL = 0;   /* disable receive,transmit */
    miniUart->MCR = 0;    /* RTS set 0 */
    miniUart->IER = 0xC6; /* Enable FIFO, Clear FIFO */
    miniUart->BAUD = 270; /* baudrate = system_clock_freq/(8 * baudrate_reg + 1) */
    miniUart->CNTL = 3;   /* enable receive,transmit */
}

STATIC VOID MiniUartWriteChar(const CHAR c)
{
    while (!(miniUart->LSR & UART_TXEMPTY_FLAG)) {};
    miniUart->IO = c;
}

STATIC UINT8 MiniUartReadChar(VOID)
{
    UINT8 ch = 0xFF;
    if (miniUart->LSR & UART_RXREADY_FLAG) {
        ch = (UINT8)(miniUart->IO & 0xFF);
    }
    return ch;
}

STATIC VOID UartHandler(VOID)
{
    (VOID)uart_getc();
}

STATIC INT32 MiniUartHwi(VOID)
{
    UINT32 ret;
    ret = LOS_HwiCreate(NUM_HAL_INTERRUPT_UART, 0, 0, UartHandler, NULL);
    if (ret != LOS_OK) {
        PRINT_ERR("%s,%d, uart interrupt created error:%x\n", __FUNCTION__, __LINE__, ret);
    } else {
        miniUart->IIR = 0x1;
        if (miniUart->LSR & UART_RXREADY_FLAG) {
            (VOID)(miniUart->IO & 0xFF);
        }
        LOS_HwiEnable(NUM_HAL_INTERRUPT_UART);
    }
    return ret;
}

UartControllerOps g_armGenericUart = {
    .uartInit = MiniUartInit,
    .uartWriteChar = MiniUartWriteChar,
    .uartReadChar = MiniUartReadChar,
    .uartHwiCreate = MiniUartHwi
};

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

