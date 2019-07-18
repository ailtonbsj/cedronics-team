/* ###################################################################
**     Filename    : Events.h
**     Project     : ProcessorExpert
**     Processor   : MKL25Z128VLK4
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2013-09-12, 20:37, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         Cpu_OnNMIINT - void Cpu_OnNMIINT(void);
**
** ###################################################################*/
/*!
** @file Events.h
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         

#ifndef __Events_H
#define __Events_H
/* MODULE Events */

#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "CameraClock1.h"
#include "BitIoLdd1.h"
#include "CameraSI1.h"
#include "BitIoLdd2.h"
#include "Analog1.h"
#include "AdcLdd1.h"
#include "Serial1.h"
#include "ASerialLdd1.h"
#include "CameraTimer.h"
#include "TimerIntLdd1.h"
#include "TU1.h"
#include "Servo1.h"
#include "PwmLdd1.h"
#include "TU2.h"
#include "Tracao1.h"
#include "BitIoLdd3.h"
#include "Tracao2.h"
#include "BitIoLdd4.h"
#include "TracaoPWM1.h"
#include "PwmLdd2.h"
#include "TU3.h"
#include "TracaoEnable.h"
#include "BitIoLdd5.h"
#include "SW1.h"
#include "BitIoLdd6.h"
#include "Tracao3.h"
#include "BitIoLdd7.h"

#ifdef __cplusplus
extern "C" {
#endif 

/*
** ===================================================================
**     Event       :  Cpu_OnNMIINT (module Events)
**
**     Component   :  Cpu [MKL25Z128LK4]
**     Description :
**         This event is called when the Non maskable interrupt had
**         occurred. This event is automatically enabled when the <NMI
**         interrupt> property is set to 'Enabled'.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void Cpu_OnNMIINT(void);


/*
** ===================================================================
**     Event       :  CameraTimer_OnInterrupt (module Events)
**
**     Component   :  CameraTimer [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void CameraTimer_OnInterrupt(void);

void Analog1_OnEnd(void);
/*
** ===================================================================
**     Event       :  Analog1_OnEnd (module Events)
**
**     Component   :  Analog1 [ADC]
**     Description :
**         This event is called after the measurement (which consists
**         of <1 or more conversions>) is/are finished.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void Analog1_OnCalibrationEnd(void);
/*
** ===================================================================
**     Event       :  Analog1_OnCalibrationEnd (module Events)
**
**     Component   :  Analog1 [ADC]
**     Description :
**         This event is called when the calibration has been finished.
**         User should check if the calibration pass or fail by
**         Calibration status method./nThis event is enabled only if
**         the <Interrupt service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

/* END Events */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

#endif 
/* ifndef __Events_H*/
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.2 [05.07]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
