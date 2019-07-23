/* ###################################################################
 **     Filename    : Events.c
 **     Project     : ProcessorExpert
 **     Processor   : MKL25Z128VLK4
 **     Component   : Events
 **     Version     : Driver 01.00
 **     Compiler    : GNU C Compiler
 **     Date/Time   : 2013-09-18, 19:23, # CodeGen: 0
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
#include <stdlib.h>

/* Modulo Camera */
extern byte cameraClock;
extern byte cameraCont;
extern byte cameraFinished;
extern unsigned short int linhaBruta[100];
extern uint8 maiorAmostra;
extern uint8 menorAmostra;
uint8 amostra;

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
void Analog1_OnEnd(void) {
	/* Write your code here ... */
	Analog1_GetValue8(&amostra);
	if (cameraCont >= 14 && cameraCont <= 113) {
		linhaBruta[cameraCont-14] = amostra;
		if (amostra > maiorAmostra) //&& amostra != 0
			maiorAmostra = amostra;
		if (amostra < menorAmostra) //&& amostra != 0
			menorAmostra = amostra;
	}
}

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
void Analog1_OnCalibrationEnd(void) {
	/* Write your code here ... */
}

/*
 ** ===================================================================
 **     Event       :  CameraTimer1_OnInterrupt (module Events)
 **
 **     Component   :  CameraTimer1 [TimerInt]
 **     Description :
 **         When a timer interrupt occurs this event is called (only
 **         when the component is enabled - <Enable> and the events are
 **         enabled - <EnableEvent>). This event is enabled only if a
 **         <interrupt service/event> is enabled.
 **     Parameters  : None
 **     Returns     : Nothing
 ** ===================================================================
 */
void CameraTimer1_OnInterrupt(void) {
	/* Write your code here ... */
	if (cameraCont <= 128) {
		if (cameraClock == 1) {
			if (cameraCont <= 1) {
				CameraSI1_PutVal(0);
			}
			cameraCont++;
		}
		cameraClock = !cameraClock;
	} else {
		cameraClock = 0;
		cameraCont = 0;
		CameraClock1_PutVal(cameraClock);
		CameraTimer1_Disable();
		Analog1_Stop();
		Analog1_Disable();
		cameraFinished = 1;
	}
	if (cameraCont == 0 && cameraClock == 0) {
		CameraSI1_PutVal(1);
	}
	CameraClock1_PutVal(cameraClock);
}

/*
 ** ===================================================================
 **     Event       :  DetectCurve1_OnInterrupt (module Events)
 **
 **     Component   :  DetectCurve1 [TimerInt]
 **     Description :
 **         When a timer interrupt occurs this event is called (only
 **         when the component is enabled - <Enable> and the events are
 **         enabled - <EnableEvent>). This event is enabled only if a
 **         <interrupt service/event> is enabled.
 **     Parameters  : None
 **     Returns     : Nothing
 ** ===================================================================
 */

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
