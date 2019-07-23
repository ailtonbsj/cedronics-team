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
#include "SW2.h"
#include "BitIoLdd9.h"
#include "SW3.h"
#include "BitIoLdd10.h"
#include "SW4.h"
#include "BitIoLdd11.h"
#include "LED1.h"
#include "BitIoLdd8.h"
#include "TU4.h"
#include "LED2.h"
#include "BitIoLdd4.h"
#include "LED3.h"
#include "BitIoLdd12.h"
#include "LED4.h"
#include "BitIoLdd13.h"
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
#include <stdlib.h>

//## MINHAS DEFINICOES
#define ALTURA_BORDAS 4500
#define LARGURA_TRACK 30

#define CENTRO_SERVO 18720 //18720
#define LIDERDADE_SERVO 300
#define ESQUERDA_SERVO (CENTRO_SERVO-LIDERDADE_SERVO)
#define DIREITO_SERVO (CENTRO_SERVO+LIDERDADE_SERVO)

//## VARIAVEIS

unsigned long linha[118];
unsigned long maiorAmostra = 0;
unsigned long menorAmostra = 65535;

/* Modulo Camera */
byte cameraClock = 0;
byte cameraCont = 0;
byte cameraFinished = 0; //flag
unsigned long linhaBruta[128];

/* Modulo Servo */
unsigned long tempoDuty = 18700;
short detectLine = 0;
short numLine = 0; //Quatidade de linha encontradas
short widLine = 0; //Tamanho da linha

/* Modulo Controle */
int TracaoCurveMenor = 12000;
int TracaoCurveMaior = 12000;
int TracaoReta = 12000;
short divisao = 4;
short limiador = 2;
long cortador = 0;
//short shutdown = 0;
//short finishLine = 0;
//short contFinishLine = 0;

/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
	/* Write your local variable definition here */

	//## VARIAVEIS
	/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
	PE_low_level_init();
	/*** End of Processor Expert internal initialization.                    ***/

	/* Write your code here */
	/* For example: for(;;) { } */

	//##INICIALIZAR
	Analog1_Start();

	TracaoEnable_PutVal(1);
	TracaoA1PWM_SetDutyUS(800);
	TracaoB1PWM_SetDutyUS(800);
	TracaoA2_PutVal(0);
	TracaoB2_PutVal(0);

	//## LOOP
	while (TRUE) {
		//## VARIAVEIS LOCAIS
		int contIni = 0;
		int cortador = 0;
		int contPixel = 0;
		int esquerdoDireitoFlag = 0;
		int contFin = 0;
		int restoCont = 0;

		//## LEITURA CAMERA DESATIVADA
		if (cameraFinished == 1) {
			cameraFinished = 0;
			maiorAmostra = 0;
			menorAmostra = 65535;

			//## PROCESSAMENTO ...
			for (contIni = 6; contIni <= 123; contIni++) {
				linha[contIni - 6] = abs(
						(signed long) linhaBruta[contIni + 1]
								- linhaBruta[contIni]);
				if (linha[contIni - 6] > maiorAmostra) {
					maiorAmostra = linha[contIni - 6];
				}
				if (linha[contIni - 6] < menorAmostra) {
					menorAmostra = linha[contIni - 6];
				}
			}
			linha[0] = 0;
			linha[1] = ALTURA_BORDAS;
			linha[116] = ALTURA_BORDAS;
			linha[117] = 0;

			cortador = (maiorAmostra - menorAmostra) / 2;
			contPixel = 0;

			for (contIni = 59, contFin = 59; contFin < 118;
					contIni--, contFin++) {
				if (linha[contIni] > cortador) {
					esquerdoDireitoFlag = 0;
					break;
				}
				if (linha[contFin] > cortador) {
					esquerdoDireitoFlag = 1;
					break;
				}
				contPixel++;
			}

			restoCont = abs(contPixel - LARGURA_TRACK);

			

			if (restoCont < 2) {
				restoCont = (((float) LIDERDADE_SERVO / LARGURA_TRACK) * restoCont);
				LED1_PutVal(1);
				LED2_PutVal(1);
				LED3_PutVal(1);
				LED4_PutVal(1);
			} else {
				restoCont = (((float) LIDERDADE_SERVO / LARGURA_TRACK) * restoCont);
				LED1_PutVal(0);
				LED2_PutVal(0);
				LED3_PutVal(0);
				LED4_PutVal(0);

			}

			//## CONTROLE E SEGURANÇA DO SERVO
			if (esquerdoDireitoFlag == 0) {
				restoCont = CENTRO_SERVO - restoCont;
				if (restoCont > DIREITO_SERVO) {
					restoCont = DIREITO_SERVO;
				} else if (restoCont < ESQUERDA_SERVO) {
					restoCont = ESQUERDA_SERVO;
				}
				Servo1_SetDutyUS(restoCont);
			} else {
				restoCont = CENTRO_SERVO + restoCont;
				if (restoCont > DIREITO_SERVO) {
					restoCont = DIREITO_SERVO;
				} else if (restoCont < ESQUERDA_SERVO) {
					restoCont = ESQUERDA_SERVO;
				}
				Servo1_SetDutyUS(restoCont);
			}

			//## INICIALIZANDO LEITURA CAMERA
			maiorAmostra = 0;
			menorAmostra = 65535;
			Analog1_Enable();
			Analog1_Start();
			CameraTimer1_Enable();
		}
		//## LEITURA CAMERA ATIVADA
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
