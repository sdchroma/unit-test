/**
  ******************************************************************************
  * @file    os_conf.h
  * @author  Mehmet Alinbay
  * @version V0.0.1
             Initial Release
  * @date    18-01-2017
  * @brief   OS Configuration Header
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 </center></h2>
  *
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __OS_CONF_H
#define __OS_CONF_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/** @addtogroup OS
  * @{
  */
/* OS Runtime Constants ------------------------------------------------------*/
/** @defgroup OS_Runtime_Constants OS Runtime Constants
  * @{
  */
//Define Max. OS Messages Here
#define osMaxMessagePool        10
#define osMaxTaskFunPool        10
#define osMaxPeriodicFunPool    10
#define osMaxSignalTimeoutPool  10
/**
  * @}
  */
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __OS_CONF_H */

/*****************************END OF FILE**************************************/
