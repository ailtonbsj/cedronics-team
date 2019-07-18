/* ###################################################################
 **     Filename    : ProcessorExpert.c
 **     Project     : ProcessorExpert
 **     Processor   : MKL25Z128VLK4
 **     Version     : Driver 01.01
 **     Compiler    : GNU C Compiler
 **     Date/Time   : 2013-09-04, 21:50, # CodeGen: 0
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
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

/* User includes (#include below this line is not maintained by Processor Expert) */
byte clockCam = 0;
byte contClock = 0;
byte idleCam = 0;
unsigned long amostra = 0;
unsigned long linhaBruta[128];
unsigned long maiorValorAmostra = 0;
unsigned long menorValorAmostra = 65535;
unsigned long limiar;
unsigned short linhaLimiada[128];

/* Valores para o servo esta entre 17.7ms    18.2ms  18.7ms   19.2ms  19.7ms */
unsigned long tempoDuty = 18700; //variavel de controle do servo

/* funcao de delay - use delay(inteiro,inteiro) */
void delay(int a, int b) {
	int conta;
	int contb;
	for (conta = 0; conta <= a; conta++) {
		for (contb = 0; contb <= b; contb++) {
		}
	}
}

/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
	/* Write your local variable definition here */
	unsigned char debouncee = 255; //Debounce para Controle de Servo-diretor

	/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
	PE_low_level_init();
	/*** End of Processor Expert internal initialization.                    ***/

	/* Write your code here */
	/* For example: for(;;) { } */
	//A01_Start();
	A1_PMW_C4_SetDutyUS(0);
	B1_PMW_C2_SetDutyUS(0);
	while (TRUE) {
		

		/* Controle de Motores Tração */
		A2_C3_PutVal(0);
		B2_C1_PutVal(0);
		Enable_E21_PutVal(1); //habilita tração

		/* Controle de Servo-diretor com Bt1 e Bt2
		 * TEMPO = 18.45ms  18.7ms   18.95ms       */
		if (Switch1_GetVal()) {
			debouncee--;
			if (debouncee == 0) {
				if (ButtonA_GetVal() && tempoDuty <= 18950) {
					tempoDuty += 10;
				}
				if (ButtonB_GetVal() && tempoDuty >= 18450) {
					tempoDuty -= 10;
				}

			}
		}
		Servo_B0_SetDutyUS(tempoDuty);
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
