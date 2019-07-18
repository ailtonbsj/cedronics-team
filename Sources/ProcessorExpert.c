/* ###################################################################
 **     Filename    : ProcessorExpert.c
 **     Project     : ProcessorExpert
 **     Processor   : MKL25Z128VLK4
 **     Version     : Driver 01.01
 **     Compiler    : GNU C Compiler
 **     Date/Time   : 2013-09-12, 20:37, # CodeGen: 0
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
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

/* User includes (#include below this line is not maintained by Processor Expert) */

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
int detLinePos = 0;
int detLineNeg = 0;
int detectLine = 0;
int servoI = 18450;
int servoE = 18950;

/* Modulo Controle e opções */
long velocidade = 1;

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
	/**/
	//Serial1_Disable();
	/**/
	Analog1_Start();
	/* Modulo Tracao */
	TracaoEnable_PutVal(1);
	Tracao1_PutVal(0);
	Tracao2_PutVal(0);
	Tracao3_PutVal(0);
	TracaoPWM1_SetDutyUS(19000);

	while (TRUE) {
		short cont;
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
				//linha[cont - 18] = (((double) 4 / (maiorAmostra - menorAmostra)) * (linhaBruta[cont] - menorAmostra));
			}

			maiorAmostra = 0;
			menorAmostra = 65535;
			Analog1_Enable();
			Analog1_Start();
			CameraTimer_Enable();

			/* Modulo servo Motor */
			//TEMPO = 18.45ms  18.7ms   18.95ms    18450-18950
			for (cont = 0; cont <= 89; cont++) {
				if (linha[cont] == 0 && linha[cont + 1] == 0
						&& linha[cont + 2] == 0) {
					detLinePos = cont;
					break;
				}
			}
			for (cont = 91; cont >= 2; cont--) {
				if (linha[cont] == 0 && linha[cont - 1] == 0
						&& linha[cont - 2] == 0) {
					detLineNeg = cont;
					break;
				}
			}
			detectLine = ((detLineNeg-detLinePos)/2)+detLinePos;

			Servo1_SetDutyUS(((double)(90-detectLine)*((double)(servoE-servoI)/90))+servoI);

			/* Modulo Controle e opções */
			if (SW1_GetVal() == 1) {
				TracaoPWM1_SetDutyUS(12000);
			}

			/* Modulo Sinal via Serial
			for (cont = 0; cont <= 91; cont++) {
				switch (linha[cont]) {
				case 0:
					Serial1_SendChar('0');
					break;
				case 1:
					Serial1_SendChar('1');
					break;
				case 2:
					Serial1_SendChar('2');
					break;
				case 3:
					Serial1_SendChar('3');
					break;
				case 4:
					Serial1_SendChar('4');
					break;
				case 5:
					Serial1_SendChar('5');
					break;

				}
				delay(3000);
			}
			Serial1_SendChar('\n');
			delay(3000);
			Serial1_SendChar('\r');
			delay(3000);
			 FIM Modulo Sinal via Serial */
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
