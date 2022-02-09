/**
  ******************************************************************************
  * @file    os_hal.c
  * @author  Mehmet Alinbay
  * @version v0.0.1
             Initial Release
  * @date    14-11-2019
  * @brief   OS MCU Specific Functions
  *
  @verbatim
  @endverbatim
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2019 </center></h2>
  *
  *
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#define OS_I
#include  "os_common.h"

/* === BOARD SPECIFIC CONFIGURATION CODE BEGIN === */
#ifndef PC_UNIT_TEST
/* MCU Specific Includes */
#include "stm32f0xx.h"
#endif
/* ==== BOARD SPECIFIC CONFIGURATION CODE END ==== */

/** @addtogroup OS_HAL_Functions OS HAL Functions
  * @{
  */

/**
  * @brief  Inits Timer
  * @note   Init Timer for 1mS Interrupt
  * @param  None
  * @retval None
  */
void pOS_HAL_InitTick(void)
{
#ifndef PC_UNIT_TEST
  SysTick_Config(SystemCoreClock / 1000);
  NVIC_SetPriority(SysTick_IRQn, 0);
  NVIC_EnableIRQ(SysTick_IRQn);
#endif
}

/**
  * @brief  Disables All Interrupt
  * @param  None
  * @retval None
  */
void pOS_HAL_DisableInterrupt(void)
{
#ifndef PC_UNIT_TEST
  __disable_irq();
#endif
}

/**
  * @brief  Enables All Interrupt
  * @param  None
  * @retval None
  */
void pOS_HAL_EnableInterrupt(void)
{
#ifndef PC_UNIT_TEST
  __enable_irq();
#endif
}

/**
  * @brief  Init WDT
  * @param  None
  * @retval None
  */
void pOS_HAL_InitWDT(void)
{
#ifndef PC_UNIT_TEST
#endif
}

/**
  * @brief  Reset WDT, This function called by pOS_Scheduler
  * @param  None
  * @retval None
  */
void pOS_HAL_ResetWDT(void)
{

}

/**
  * @}
  */
