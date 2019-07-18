/* ###################################################################
**     Filename    : Events.h
**     Project     : ProcessorExpert
**     Processor   : MKL25Z128VLK4
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2013-09-04, 21:50, # CodeGen: 0
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
#include "Servo_B0.h"
#include "PwmLdd1.h"
#include "TU1.h"
#include "A2_C3.h"
#include "BitIoLdd1.h"
#include "B2_C1.h"
#include "BitIoLdd2.h"
#include "Enable_E21.h"
#include "BitIoLdd3.h"
#include "Fault_E20.h"
#include "BitIoLdd4.h"
#include "ButtonA.h"
#include "BitIoLdd5.h"
#include "ButtonB.h"
#include "BitIoLdd6.h"
#include "Switch1.h"
#include "BitIoLdd7.h"
#include "A1_PMW_C4.h"
#include "PwmLdd2.h"
#include "TU2.h"
#include "B1_PMW_C2.h"
#include "PwmLdd3.h"
#include "Clock1.h"
#include "BitIoLdd8.h"
#include "SI1.h"
#include "BitIoLdd9.h"
#include "A01.h"
#include "AdcLdd1.h"
#include "TimerCam1.h"
#include "TimerIntLdd1.h"
#include "TU3.h"

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
**     Event       :  TimerCam1_OnInterrupt (module Events)
**
**     Component   :  TimerCam1 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void TimerCam1_OnInterrupt(void);

void AO1_OnEnd(void);
/*
** ===================================================================
**     Event       :  AO1_OnEnd (module Events)
**
**     Component   :  AO1 [ADC]
**     Description :
**         This event is called after the measurement (which consists
**         of <1 or more conversions>) is/are finished.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

/*
** ===================================================================
**     Event       :  AO1_OnCalibrationEnd (module Events)
**
**     Component   :  AO1 [ADC]
**     Description :
**         This event is called when the calibration has been finished.
**         User should check if the calibration pass or fail by
**         Calibration status method./nThis event is enabled only if
**         the <Interrupt service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void A01_OnEnd(void);
/*
** ===================================================================
**     Event       :  A01_OnEnd (module Events)
**
**     Component   :  A01 [ADC]
**     Description :
**         This event is called after the measurement (which consists
**         of <1 or more conversions>) is/are finished.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void A01_OnCalibrationEnd(void);
/*
** ===================================================================
**     Event       :  A01_OnCalibrationEnd (module Events)
**
**     Component   :  A01 [ADC]
**     Description :
**         This event is called when the calibration has been finished.
**         User should check if the calibration pass or fail by
**         Calibration status method./nThis event is enabled only if
**         the <Interrupt service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

/*
** ===================================================================
**     Event       :  TI1_OnInterrupt (module Events)
**
**     Component   :  TimerCam1 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void TI1_OnInterrupt(void);

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
