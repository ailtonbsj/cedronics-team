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

//## VARIAVEIS
unsigned long linha[100];
unsigned long maiorAmostra = 0;
unsigned long menorAmostra = 4294967295;
unsigned long maiAmostra = 0;
unsigned long menAmostra = 4294967295;
byte cameraClock = 0;
byte cameraCont = 0;
byte cameraFinished = 0;
unsigned long linhaBruta[128];

#define CENTRO_SERVO 18720
#define LIDERDADE_SERVO 270
#define ESQUERDA_SERVO (CENTRO_SERVO-LIDERDADE_SERVO)
#define DIREITO_SERVO (CENTRO_SERVO+LIDERDADE_SERVO)
#define TRACAO 650
#define ULTIMA_BORDA 4500

int largura[3];
int contLarg = 0;
int contPixel = 0;

int contIni = 0;
int contFin = 0;
int cortador = 0;

int restoCont = 0;
int flagI = 0;
int flagF = 0;
int posL = 0;
int posR = 0;
int esquerdoDireitoFlag = 0;
int esquerdoDireitoFlagAnterior = 0;
int posicaoCarro = 0;
int giroServo = 0;
int velocidade = 0;

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

	if (SW1_GetVal()) {
		velocidade = velocidade | 0b00000001;
	}
	if (SW2_GetVal()) {
		velocidade = velocidade | 0b00000010;
	}
	if (SW3_GetVal()) {
		velocidade = velocidade | 0b00000100;
	}
	if (SW4_GetVal()) {
		velocidade = velocidade | 0b00001000;
	}
	velocidade = velocidade * 100;

	Analog1_Start();
	TracaoEnable_PutVal(1);
	TracaoA1PWM_SetDutyUS(velocidade);
	TracaoB1PWM_SetDutyUS(velocidade);
	TracaoA2_PutVal(0);
	TracaoB2_PutVal(0);

	while (TRUE) {

		//## LEITURA CAMERA DESATIVADA
		if (cameraFinished == 1) {
			cameraFinished = 0;
			/* processing dados camera BEGIN */
			maiAmostra = 0;
			menAmostra = 4294967295;
			for (contIni = 14; contIni <= 113; contIni++) {
				linha[contIni - 14] = abs(
						(signed long) linhaBruta[contIni + 1]
								- linhaBruta[contIni]);
				if (linha[contIni - 14] > maiAmostra) {
					maiAmostra = linha[contIni - 14];
				}
				if (linha[contIni - 14] < menAmostra) {
					menAmostra = linha[contIni - 14];
				}
			}
			cortador = (maiAmostra + menAmostra) / 2;
			//linha[0] = 0;
			//linha[1] = ULTIMA_BORDA;
			//linha[116] = ULTIMA_BORDA;
			//linha[117] = 0;
			/* processing dados camera END */
			/* detecção largura BEGIN */
			if (contLarg < 13) {
				if (contLarg >= 10) {
					for (contIni = 49, contFin = 50; contFin <= 99;
							contIni--, contFin++) {
						if (linha[contIni] > cortador && flagI == 0) {
							posL = contIni;
							flagI = 1;
						}
						if (linha[contFin] > cortador && flagF == 0) {
							posR = contFin;
							flagF = 1;
						}
					}
					flagF = 0;
					flagI = 0;
					largura[contLarg - 10] = abs(posR - posL);
				}
				if (contLarg == 12) {
					largura[0] = (largura[0] + largura[1] + largura[2]) / 3;
					largura[1] = largura[0] / 2;
				}
				contLarg++;
			}
			/* detecção largura END */
			/*  BEGIN */
			contPixel = 0;
			for (contIni = 49, contFin = 50; contFin <= 99;
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
			posicaoCarro = abs(largura[1] - contPixel);
			if (cortador > 1000) {
				if (posicaoCarro < 10) {
					giroServo = ((float) LIDERDADE_SERVO / largura[1])
							* posicaoCarro;
					LED1_PutVal(0);
					LED2_PutVal(0);
					LED3_PutVal(0);
					LED4_PutVal(0);
					TracaoA1PWM_SetDutyUS(velocidade);
					TracaoB1PWM_SetDutyUS(velocidade);
				} else {
					if (esquerdoDireitoFlagAnterior != esquerdoDireitoFlag) {
						esquerdoDireitoFlag = esquerdoDireitoFlagAnterior;
						
						if (esquerdoDireitoFlag == 1) {
							TracaoA1PWM_SetDutyUS(velocidade-150);
							TracaoB1PWM_SetDutyUS(velocidade+150);
						} else {
							TracaoA1PWM_SetDutyUS(velocidade+150);
							TracaoB1PWM_SetDutyUS(velocidade-150);
						}
						
					}
					giroServo = LIDERDADE_SERVO;
					LED1_PutVal(1);
					LED2_PutVal(1);
					LED3_PutVal(1);
					LED4_PutVal(1);
				}
				esquerdoDireitoFlagAnterior = esquerdoDireitoFlag;
			} else {
				giroServo = LIDERDADE_SERVO;
				esquerdoDireitoFlag = esquerdoDireitoFlagAnterior;
			}
			//## CONTROLE E SEGURANÇA DO SERVO
			if (esquerdoDireitoFlag == 0) {
				restoCont = CENTRO_SERVO - giroServo;
				if (restoCont > DIREITO_SERVO) {
					restoCont = DIREITO_SERVO;
				} else if (restoCont < ESQUERDA_SERVO) {
					restoCont = ESQUERDA_SERVO;
				}
				Servo1_SetDutyUS(restoCont);
			} else {
				restoCont = CENTRO_SERVO + giroServo;
				if (restoCont > DIREITO_SERVO) {
					restoCont = DIREITO_SERVO;
				} else if (restoCont < ESQUERDA_SERVO) {
					restoCont = ESQUERDA_SERVO;
				}
				Servo1_SetDutyUS(restoCont);
			}

			//## INICIALIZANDO LEITURA CAMERA
			maiorAmostra = 0;
			menorAmostra = 4294967295;
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
