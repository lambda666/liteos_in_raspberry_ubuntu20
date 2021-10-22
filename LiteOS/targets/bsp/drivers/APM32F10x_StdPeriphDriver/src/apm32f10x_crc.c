/*!
 * @file      apm32f10x_crc.c
 *
 * @brief     This file provides all the CRC firmware functions
 *
 * @version   V1.0.1
 *
 * @date      2021-03-23
 *
 */

#include "apm32f10x_crc.h"

/** @addtogroup Peripherals_Library Standard Peripheral Library
  @{
*/

/** @addtogroup CRC_Driver CRC Driver
  @{
*/

/** @addtogroup CRC_Fuctions Fuctions
  @{
*/

/*!
 * @brief     Reset CRC data register.
 *
 * @param     None
 *
 * @retval    None
 *
 * @note
 */
void CRC_ResetDATA(void)
{
    CRC->CTRL_B.RST = BIT_SET;
}

/*!
 * @brief     Calculate CRC of a 32bit data word.
 *
 * @param     data: a data word to compute its CRC.
 *                  This parameter can be a 32bit value:
 *
 * @retval    A 32-bit CRC value
 */
uint32_t CRC_CalculateCRC(uint32_t data)
{
    CRC->DATA = data;

    return (CRC->DATA);
}

/*!
 * @brief     Computes the 32-bit CRC of a given buffer of data word(32-bit).
 *
 * @param     buf: Pointer to the buffer containing the data to be computed.
 *
 * @param     bufLen: The length of buffer which is computed.
 *
 * @retval    A 32-bit CRC value
 */
uint32_t CRC_CalculateBlockCRC(uint32_t *buf, uint32_t bufLen)
{
    while(bufLen--)
    {
        CRC->DATA = *buf++;
    }

    return (CRC->DATA);
}

/*!
 * @brief     Returns the current CRC value.
 *
 * @param     None
 *
 * @retval    A 32-bit CRC value
 */
uint32_t CRC_ReadCRC(void)
{
    return (CRC->DATA);
}

/*!
 * @brief     Saves a 8bit data in the Independent Data register(INDATA).
 *
 * @param     inData: a 8-bit value to be stored in the ID register
 *
 * @retval    None
 */
void CRC_WriteIDRegister(uint8_t inData)
{
    CRC->INDATA = inData;
}

/*!
 * @brief      Reads a 8-bit data saved in the Independent Data register(INDATA).
 *
 * @param      None
 *
 * @retval     a 8-bit value from the INDATA register
 */
uint8_t CRC_ReadIDRegister(void)
{
    return (CRC->INDATA);
}

/**@} end of group CRC_Fuctions*/
/**@} end of group CRC_Driver*/
/**@} end of group Peripherals_Library*/
