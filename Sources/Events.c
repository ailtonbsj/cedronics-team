/* ###################################################################
 **     Filename    : Events.c
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
 ** @file Events.c
 ** @version 01.00
 ** @brief
 **         This is user's event module.
 **         Put your event handler code here.
 */
/*!
 **  @addtogroup Events_module Events module documentation
 **  @{
 */
/* MODULE Events */

#include "Cpu.h"
#include "Events.h"

#ifdef __cplusplus
extern "C" {
#endif 

/* User includes (#include below this line is not maintained by Processor Expert) */
extern byte clockCam;
extern byte contClock;
extern byte idleCam;
extern unsigned long amostra;
extern unsigned long linhaBruta[128];
extern unsigned long maiorValorAmostra;
extern unsigned long menorValorAmostra;

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
void Cpu_OnNMIINT(void) {
	/* Write your code here ... */
}

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
void AO1_OnEnd(void) {
	/* Write your code here ... */
	if (contClock >= 10 && contClock <= 118) {
		AO1_GetValue(&amostra);
		linhaBruta[contClock - 10] = amostra;

		if (amostra > maiorValorAmostra)
			maiorValorAmostra = amostra;
		if ((amostra < menorValorAmostra) && (amostra != 0))
			menorValorAmostra = amostra;

	} else {
		maiorValorAmostra = 0;
		menorValorAmostra = 65535;
	}
}

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
void A01_OnEnd(void)
{
  /* Write your code here ... */
}

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
void A01_OnCalibrationEnd(void)
{
  /* Write your code here ... */
}

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
void TimerCam1_OnInterrupt(void)
{
  /* Write your code here ... */
	if (contClock <= 128) {
			if (clockCam == 1) {
				if (contClock <= 1)
					SI1_PutVal(0);
				contClock++;
			}

			clockCam = !clockCam;
		} else {
			clockCam = 0;
			idleCam++;
			if (idleCam >= 6) {
				contClock = 0;
				idleCam = 0;
			}
		}
		if (contClock == 0 && clockCam == 0) {
			SI1_PutVal(1);
		}
		Clock1_PutVal(clockCam);
}

/* END Events */

#ifdef __cplusplus
} /* extern "C" */
#endif 

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
