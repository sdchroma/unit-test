/**
  ******************************************************************************
  * @file    os_common.h
  * @author  Mehmet Alinbay
  * @version V0.1.1
             Initial Release
  * @date    18-01-2017
  * @brief   OS common header file for all modules
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 </center></h2>
  *
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __OS_COMMON_H
#define __OS_COMMON_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* C-LIB */
#include  "string.h"
#include  "stdint.h"
#include  "stdio.h"
/* OS Configuration */
#include  "os_conf.h"
//
#include  "os_cirbuf.h"

/** @addtogroup OS
  * @{
  */
/* OS constants --------------------------------------------------------*/
/** @defgroup OS_Constants OS Exported Constants
  * @brief OS_Constants : OS Releated Constants
  * @{
  */

#define osMaxOSMessageDataLen   64

//TIMEOUTs
#define ONEMILISECTIMEOUT     1
#define TENMILISECTIMEOUT     (10 * ONEMILISECTIMEOUT)
#define ONEHUNDREDMILISECTIMEOUT  (100 * ONEMILISECTIMEOUT)
#define ONESECTIMEOUT         (1000 * ONEMILISECTIMEOUT)
#define TENSECTIMEOUT         (10 * ONESECTIMEOUT)

/**
  * @}
  */
/* OS Exported types ---------------------------------------------------*/
/** @defgroup OS_Exported_Types OS Exported Types
  * @{
  */

/**
  * @brief OS Core Status Code
  */
typedef enum
{
  osCoreReset = 0,
  osCoreRun    ,
}osCoreStatus_t;

/**
  * @brief OS Timeout Type
  */
#define osTimerInfinite   (-1)

/**
  * @brief OS Status Code
  */
typedef enum
{
  osOK     = 0x00,
  osError  = -1,
  osBusy   = -2,
}osStatus_t;

/**
  * @brief OS Task State
  */
typedef enum
{
  osTaskState_Reset = 0,
  osTaskState_Idle  ,
  osTaskState_Active,
  osTaskState_Error ,
}osTaskState_t;

/**
  * @brief OS Error Code
  */
typedef enum
{
  osErrorWrongTask = 0x00,
  osErrorWrongSignal,
  osErrorMessageQueFull,
  osErrorMessageNotSent,
  osErrorUndefined,
}osErrorType_t;

/**
  * @brief OS Signal Timeout ID
  */
typedef void *osSignalTimeoutId_t;

/**
  * @brief OS Message Type
  */
typedef struct
{
  uint8_t   allocated;
  uint32_t  destTask;
  uint32_t  signal;
  uint8_t   data[osMaxOSMessageDataLen];
  uint8_t   data_length;
}osMessage_t;

/**
  * @brief OS Function Typedef
  */
typedef void(*osInitTaskFunc_t)(void);
typedef void(*osRunTaskFunc_t)(osMessage_t *message);
typedef void(*osPeriodicFunc_t)(uint32_t arg);

/**
  * @}
  */

/* OS Exported Macros --------------------------------------------------------*/
/** @defgroup OS_Exported_Macros OS Exported Macros
  * @{
  */
#define mCOUNTOFARRAY(x)    ( sizeof(x) / sizeof(x[0]) )
#define mBITSET(REG,BIT)    ( REG |= (1U<<BIT) )
#define mBITRESET(REG,BIT)  ( REG &= ~(1U<<BIT) )
#define mBITMASK(REG,BIT)   ( REG & (1U<<BIT) )
#define mBITREAD(REG,BIT)   ( (REG & (1U<<BIT)) ? 1U : 0U )
/**
  * @}
  */

/* OS Exported Functions -----------------------------------------------------*/
/** @addtogroup OS_Functions OS Exported Functions
  * @{
  */
void pOS_InitCore(void);
void pOS_Scheduler(void);
void pOS_Tick(void);
//
uint32_t pOS_AddTask(osInitTaskFunc_t  init,
                     osRunTaskFunc_t   run);
osStatus_t pOS_SendSignal( uint32_t destTask,
                           uint32_t signal,
                           uint8_t *data,
                           uint8_t data_length);
//
int32_t pOS_StartSignalTimeout( int32_t   repeat,
                                uint32_t  destTask,
                                uint32_t  signal,
                                uint32_t  period );
int32_t pOS_StopSignalTimeout( int32_t stoID );
//
int32_t pOS_StartPeriodicFunc( osPeriodicFunc_t callback,
                               uint32_t         arg,
                               int32_t          repeat,
                               uint32_t         period );
int32_t pOS_StopPeriodicFunc( osPeriodicFunc_t callback );
int32_t pOS_RestartPeriodicFunc( osPeriodicFunc_t callback );
//
void pOS_LogError(osErrorType_t error);

/**
  * @}
  */

#ifdef OS_I
/* OS HAL Functions ----------------------------------------------------------*/
/** @defgroup OS_HAL_Functions OS HAL Functions
  * @brief CPU Specifisic Functions
  * @{
  */
void pOS_HAL_InitTick(void);
void pOS_HAL_DisableInterrupt(void);
void pOS_HAL_EnableInterrupt(void);
void pOS_HAL_ResetWDT(void);
void pOS_HAL_InitWDT(void);
/**
  * @}
  */

/* OS Private types ----------------------------------------------------------*/
/** @defgroup OS_Private_Types OS Private Types
  * @{
  */

/**
  * @brief OS TaskObject
  */
typedef struct
{
  osInitTaskFunc_t  initTask;
  osRunTaskFunc_t   runTask;
}osTask_t;

/**
  * @brief OS PeriodicFunctionObject
  */
typedef struct
{
  osPeriodicFunc_t  callback;
  int32_t           repeat;
  uint32_t          pending;
  uint32_t          overload;
  uint32_t          period;
  uint32_t          remaining;
  uint32_t          arg;
}osPeriodicFunction_t;

/**
  * @brief OS Control Struct
  */
typedef struct
{
  osCoreStatus_t        status;
  uint32_t              tick;
  uint16_t              maxPendingMessage;
  uint32_t              totalOSError;
  uint32_t              logOSError[osErrorUndefined];
  osMessage_t           *osMessageHead;
  osMessage_t           *osMessageTail;
  uint32_t              osMessageInUse;
  osMessage_t           osMessagePool[osMaxMessagePool];
  osPeriodicFunction_t  osPeriodicFunctionPool[osMaxPeriodicFunPool];
}osCore_t;

/**
  * @}
  */

/**
  * @}
  */

#endif

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __OS_COMMON_H */

/*****************************END OF FILE**************************************/
