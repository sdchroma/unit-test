/**
  ******************************************************************************
  * @file    os.c
  * @author  Mehmet Alinbay
  * @version V0.0.1
             Initial Release
  * @date    18-01-2017
  * @brief   OS Core Functions
  *
  @verbatim
  @endverbatim
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 </center></h2>
  *
  *
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#define OS_I
#include  "os_common.h"

/** @defgroup OS OS
  * @brief OS Functions-1
  * @{
  */

/* Private variables ---------------------------------------------------------*/
/** @defgroup OS_Private_Variables OS Private Variables
  * @{
  */
/** @addtogroup OS_Core_Variables OS Core Variables
  * @{
  */
osCore_t  osCore;
osTask_t  osTaskPool[osMaxTaskFunPool];

/**
  * @}
  */

/**
  * @}
  */

/* Private function prototypes -----------------------------------------------*/
/** @defgroup OS_Private_Function_Prototypes OS Private Function Prototypes
  * @{
  */
/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/
/** @defgroup OS_Private_Functions OS Private Functions
  * @{
  */
/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup OS_Exported_Functions OS Exported Functions
  * @{
  */

/**
  * @brief  Inits OS Core variables
  * @note   Should be Called at startup
  * @param  None
  * @retval None
  */
void pOS_InitCore(void)
{
  uint16_t counter = 0;
  osCoreStatus_t local_status_flag = osCoreReset;

  if(osCore.status == osCoreRun)                //if OS Running, Stop it
  {
    local_status_flag = osCore.status;          //backup OS status
    osCore.status = osCoreReset;              //stop OS
  }
  // Clear OS Core Variables
  memset(&osCore, 0x00, sizeof(osCore));
  osCore.osMessageHead = &osCore.osMessagePool[0];
  osCore.osMessageTail = &osCore.osMessagePool[0];

  // Call Init Tasks
  counter = 0;
  while(osTaskPool[counter].initTask != NULL)
  {
    osTaskPool[counter].initTask();
    counter++;
  }

  // Re-Load OS Status
  if(local_status_flag != osCoreReset)
  {
    osCore.status = local_status_flag;
  }
  pOS_HAL_InitTick();
  pOS_HAL_InitWDT();
  osCore.status = osCoreRun;
}

/**
  * @brief  OS Add Task Function, finds an empty slot, places task
  * @param  init    : Task Init Function
  * @param  run     : Task Message Handler Dunction
  * @retval Status Flag enumeration value:
  *          - 0    : Not applicable
  *          - else : Task Added
  */
uint32_t pOS_AddTask(osInitTaskFunc_t  init,
                     osRunTaskFunc_t   run)
{
  osTask_t *to = &osTaskPool[0];
  if( (init == NULL) || (run == NULL) )
  {
    return 0;
  }
  pOS_HAL_DisableInterrupt();
  //
  while( to < &osTaskPool[osMaxTaskFunPool] )
  {
    if( ( to->initTask == NULL ) && ( to->runTask == NULL ) )
    {
      to->initTask = init;
      to->runTask = run;
      pOS_HAL_EnableInterrupt();
      return (uint32_t)to;
    }
    to++;
  }
  pOS_HAL_EnableInterrupt();
  return 0;
}

/**
  * @brief  OS Send Signal Function, finds an empty slot, places signal
  * @param  destTask    : Signal Dest. Task
  * @param  signal      : Signal to Send
  * @param  data        : Pointer of Message Data
  * @param  data_length : Message Data Length
  * @retval Status Flag enumeration value:
  *          - OK:    Signal placed
  *          - ERROR: Not applicable
  */
osStatus_t pOS_SendSignal(uint32_t  destTask,
                          uint32_t  signal,
                          uint8_t   *data,
                          uint8_t   data_length)
{
  int freesize = osMaxMessagePool - osCore.osMessageInUse;
  if( (data_length > osMaxOSMessageDataLen) ||
      (freesize <= 0) )
  {
    return osError;
  }
  pOS_HAL_DisableInterrupt();
  //
  osCore.osMessageTail->allocated   = 1;
  osCore.osMessageTail->destTask    = destTask;
  osCore.osMessageTail->signal      = signal;
  osCore.osMessageTail->data_length = data_length;
  if( data_length )
    memcpy(&osCore.osMessageTail->data[0] , data, data_length);
  osCore.osMessageTail++;
  if( osCore.osMessageTail >= &osCore.osMessagePool[osMaxMessagePool] )
    osCore.osMessageTail = &osCore.osMessagePool[0];
  //
  osCore.osMessageInUse++;
  if( osCore.osMessageInUse > osCore.maxPendingMessage )
    osCore.maxPendingMessage = osCore.osMessageInUse;
  //
  pOS_HAL_EnableInterrupt();
  return osOK;
}

/**
  * @brief  OS Start Periodic Function
  * @param  callback    : Callback Function after Timer elapsed
  * @param  arg         : Argument Passed to Callback Function
  * @param  repeat      : Timeout Repeat Counter Infinite/One-Shot/X-Shot
  * @param  period      : Timeout Period (mSec)
  * @retval osPeriodicFunId_t
  *          - 0        : Timeout placed
  *          - -1       : Not applicable (Pool full, wrong parameter)
  */
int32_t pOS_StartPeriodicFunc( osPeriodicFunc_t   callback,
                               uint32_t           arg,
                               int32_t            repeat,
                               uint32_t           period )
{
  osPeriodicFunction_t *pfo;
  if( (period == 0) || (callback == NULL) || (repeat == 0) )
    return -1;
  pfo = &osCore.osPeriodicFunctionPool[0];
  while( pfo < &osCore.osPeriodicFunctionPool[osMaxPeriodicFunPool] )
  {
    if( pfo->period == 0 )
    {
      pOS_HAL_DisableInterrupt();
      pfo->callback   = callback;
      pfo->arg        = arg;
      pfo->repeat     = repeat;
      pfo->period     = period;
      pfo->remaining  = pfo->period;
      pOS_HAL_EnableInterrupt();
      return 0;
    }
    pfo++;
  }
  return -1;
}

/**
  * @brief  OS Stop Periodic Function
  * @param  osPeriodicFunId_t : Periodic Function ID
  * @retval Status Flag enumeration value:
  *          - OK:    Periodic Function stopped
  *          - ERROR: Not applicable
  */
int32_t pOS_StopPeriodicFunc( osPeriodicFunc_t callback )
{
  osPeriodicFunction_t *pfo;
  if( callback == NULL )
    return -1;
  pfo = &osCore.osPeriodicFunctionPool[0];
  while( pfo < &osCore.osPeriodicFunctionPool[osMaxPeriodicFunPool] )
  {
    if( pfo->callback == callback )
    {
      pOS_HAL_DisableInterrupt();
      memset(pfo,0x00,sizeof(osPeriodicFunction_t));
      pOS_HAL_EnableInterrupt();
      return 0;
    }
    pfo++;
  }
  return -1;
}

/**
  * @brief  OS Restart Periodic Function Timer
  * @param  osPeriodicFunId_t : Periodic Function ID
  * @retval Status Flag enumeration value:
  *          - OK:    Timeout stopped
  *          - ERROR: Not applicable
  */
int32_t pOS_RestartPeriodicFunc( osPeriodicFunc_t callback )
{
  osPeriodicFunction_t *pfo;
  if( callback == NULL )
    return -1;
  pfo = &osCore.osPeriodicFunctionPool[0];
  while( pfo < &osCore.osPeriodicFunctionPool[osMaxPeriodicFunPool] )
  {
    if( pfo->callback == callback )
    {
      pOS_HAL_DisableInterrupt();
      pfo->remaining = pfo->period;
      pOS_HAL_EnableInterrupt();
      return 0;
    }
    pfo++;
  }
  return -1;
}

/**
  * @brief  OS Tick Program
  * @note   Should be Called every mSec
  * @param  None
  * @retval None
  */
void pOS_Tick(void)
{
  osPeriodicFunction_t *pfo;
  if(osCore.status != osCoreRun)   //do not start before OS init
    return;
  //
  osCore.tick++;
  // Check Periodic Function Timer(s)
  pfo = &osCore.osPeriodicFunctionPool[0];
  while( pfo < &osCore.osPeriodicFunctionPool[osMaxPeriodicFunPool] )
  {
    if( ( pfo->remaining != 0 ) && ( pfo->period != 0 ) )
    {
      if((--pfo->remaining) == 0)
      { //Check Type of Signal Timeout & Re-Load if Needed
        //if(pfo->type == osTimerPeriodic)
        //{
        //  pfo->remaining = pfo->period;
        //  pfo->pending++;
        //}
      }
    }
    pfo++;
  }
}

/**
  * @brief  OS Scheduler Function, Calls Periodic Functions, Manages Signals
  * @note   Should be Called in while superloop
  * @param  None
  * @retval None
  */
void pOS_Scheduler(void)
{
  osPeriodicFunction_t *pfo;
  osTask_t  *to;
  if(osCore.status != osCoreRun)   //do not start before OS init
    return;
  // Periodic Non-Critic Function Calls
  pfo = &osCore.osPeriodicFunctionPool[0];
  while( pfo < &osCore.osPeriodicFunctionPool[osMaxPeriodicFunPool] )
  {
    if( ( pfo->remaining == 0 ) && ( pfo->period != 0 ) )
    {
      pfo->callback(pfo->arg);
      if( pfo->repeat > 0 )
        pfo->repeat--;
      if( pfo->repeat != 0 )
      {
        pfo->remaining = pfo->period;
      }
      else
      {
        if( pOS_StopPeriodicFunc( pfo->callback ) == -1 )
          ;//TODO Fatal Error
      }
    }
    pfo++;
  }
  // Signal Calls
  if(osCore.osMessageHead->allocated != 0)
  {
    to = (osTask_t*)&osTaskPool[0];
    while( to < &osTaskPool[osMaxTaskFunPool] )
    {
      if((uint32_t)to == osCore.osMessageHead->destTask)
      {
        to->runTask(osCore.osMessageHead);
        break;
      }
      to++;
    }
    pOS_HAL_DisableInterrupt();
    memset(osCore.osMessageHead, 0x00, sizeof(osMessage_t));
    osCore.osMessageHead++;
    if(osCore.osMessageHead >= &osCore.osMessagePool[osMaxMessagePool])
      osCore.osMessageHead = &osCore.osMessagePool[0];
    osCore.osMessageInUse--;
    pOS_HAL_EnableInterrupt();
  }
  // WDT
  pOS_HAL_ResetWDT();
}

/**
  * @brief  OS Error Logger
  * @note   Error List enum is in os_common.h
  * @param  Error Type
  * @retval None
  */
void pOS_LogError(osErrorType_t error)
{
  osCore.totalOSError++;
  osCore.logOSError[error]++;
  switch(error)
  {
    case osErrorWrongTask:
    break;
    case osErrorWrongSignal:
    break;
    case osErrorMessageQueFull:
    break;
  }
}

/**
  * @}
  */

/**
  * @}
  */

/****************************** END OF FILE ***********************************/
