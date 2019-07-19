/* ###################################################################
 **     Filename    : ProcessorExpert.c
 **     Project     : ProcessorExpert
 **     Processor   : MKL25Z128VLK4
 **     Version     : Driver 01.01
 **     Compiler    : GNU C Compiler
 **     Date/Time   : 2013-09-18, 19:23, # CodeGen: 0
 **     Abstract    :
 **         Main module.
 **         This module contains user's application code.
 **     Settings    :
 **     Contents    :
 **         No public methods
 **
 ** ###################################################################*/
/*!
 ** @file ProcessorExpert.c
 ** @version 01.01
 ** @brief
 **         Main module.
 **         This module contains user's application code.
 */
/*!
 **  @addtogroup ProcessorExpert_module ProcessorExpert module documentation
 **  @{
 */
/* MODULE ProcessorExpert */

/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "CameraClock1.h"
#include "BitIoLdd1.h"
#include "CameraSI1.h"
#include "BitIoLdd2.h"
#include "CameraTimer1.h"
#include "TimerIntLdd1.h"
#include "TU1.h"
#include "Analog1.h"
#include "AdcLdd1.h"
#include "TracaoA1PWM.h"
#include "PwmLdd1.h"
#include "TU2.h"
#include "TracaoB2.h"
#include "BitIoLdd5.h"
#include "TracaoEnable.h"
#include "BitIoLdd6.h"
#include "Servo1.h"
#include "PwmLdd2.h"
#include "TU3.h"
#include "SW1.h"
#include "BitIoLdd7.h"
#include "LED1.h"
#include "BitIoLdd8.h"
#include "DetectCurve1.h"
#include "TimerIntLdd2.h"
#include "TU4.h"
#include "LED2.h"
#include "BitIoLdd4.h"
#include "TracaoA2.h"
#include "BitIoLdd3.h"
#include "TracaoB1PWM.h"
#include "PwmLdd3.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

/* User includes (#include below this line is not maintained by Processor Expert) */

#define CENTRO 18700
#define ESQUERDO 18400
#define DIREITO 19000

/* Modulo Camera */
byte cameraClock = 0;
byte cameraCont = 0;
byte cameraFinished = 0;
unsigned long linhaBruta[128];
unsigned long linha[92];
unsigned long maiorAmostra = 0;
unsigned long menorAmostra = 65535;

/* Modulo Servo */
unsigned long tempoDuty = 18700;
short detFirstLine = 0;
short detSeconLine = 0;
short detectLine = 0;

/* Modulo Controle */
short velocCurve = 0;
short curve = 0;

/* funcao Delay */
void delay(int valor) {
	int cont;
	for (cont = 0; cont <= valor; cont++) {
	}
}

/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
	/* Write your local variable definition here */

	/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
	PE_low_level_init();
	/*** End of Processor Expert internal initialization.                    ***/

	/* Write your code here */
	/* For example: for(;;) { } */

	/* Modulo de Acionamento de Componentes */
	Analog1_Start();

	/* Modulo Tracao */
	TracaoEnable_PutVal(1);
	TracaoA1PWM_SetDutyUS(14000);
	TracaoB1PWM_SetDutyUS(14000);
	TracaoA2_PutVal(0);
	TracaoB2_PutVal(0);
	while (TRUE) {
		int cont;

		/* Modulo Camera */
		if (cameraFinished == 1) {
			cameraFinished = 0;
			for (cont = 18; cont <= 109; cont++) {
				if ((((double) 4 / (maiorAmostra - menorAmostra))
						* (linhaBruta[cont] - menorAmostra)) <= 1) {
					linha[cont - 18] = 0;
				} else {
					linha[cont - 18] = 1;
				}
			}

			/* Modulo servo Motor */
			/* Tempo usando atualmente 18400--18700--19000 */
			for (cont = 0; cont <= 89; cont++) {
				if ((linha[cont] == 0) && (linha[cont + 1] == 0)
						&& (linha[cont + 2] == 0)) {
					detectLine = cont;
					break;
				}
			}
			/* Modulo Controle */
			if (detectLine < 16) {
				//CURVE
				Servo1_SetDutyUS(DIREITO);
				TracaoA1PWM_SetDutyUS(14000);
				TracaoB1PWM_SetDutyUS(18000);
			} else if (detectLine <= 30) {
				//REACT
				Servo1_SetDutyUS(CENTRO+6);
			} else if (detectLine > 75) {
				//CURVE
				Servo1_SetDutyUS(ESQUERDO);

				TracaoA1PWM_SetDutyUS(18000);
				TracaoB1PWM_SetDutyUS(14000);
			} else if (detectLine > 61) {
				//REACT
				Servo1_SetDutyUS(CENTRO-6);
			} else {
				//STRAIGHT
				TracaoA1PWM_SetDutyUS(10000);
				TracaoB1PWM_SetDutyUS(10000);
			}
			//Servo1_SetDutyUS( ((double) 6.6666 * (90 - (detectLine))) + 18400);

			maiorAmostra = 0;
			menorAmostra = 65535;
			Analog1_Enable();
			Analog1_Start();
			CameraTimer1_Enable();
		}

	}

	/*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END ProcessorExpert */
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
