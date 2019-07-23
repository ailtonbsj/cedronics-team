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
byte cameraClock = 0;
byte cameraCont = 0;
byte cameraFinished = 0;
unsigned short int linhaBruta[100];
uint8 maiorAmostra = 0;
uint8 menorAmostra = 255;

unsigned short int linha[100];

int velocidade = 0;

int restoCont = 0;
int flagI = 0;
int flagF = 0;
int posL = 0;
int posR = 0;
int esquerdoDireitoFlag = 0;
int esquerdoDireitoFlagAnterior = 0;
int posicaoCarro = 0;
int giroServo = 0;

#define CENTRO_SERVO 18700
#define LIDERDADE_SERVO 270
#define ESQUERDA_SERVO (CENTRO_SERVO-LIDERDADE_SERVO)
#define DIREITO_SERVO (CENTRO_SERVO+LIDERDADE_SERVO)

#define OUTSIDE_LINE 0
#define INSIDE_LINE 1
#define ONE_LINE 2
#define TWO_LINE 3

int acenderLeds(uint8 num) {
	if (num & 1)
		LED1_PutVal(TRUE);
	else
		LED1_PutVal(FALSE);
	if (num & 2)
		LED2_PutVal(TRUE);
	else
		LED2_PutVal(FALSE);
	if (num & 4)
		LED3_PutVal(TRUE);
	else
		LED3_PutVal(FALSE);
	if (num & 8)
		LED4_PutVal(TRUE);
	else
		LED4_PutVal(FALSE);
	return 0;
}

int rangeLeds(int8 valor, int8 min, int8 max) {
	int8 a = (int8) (((float) 4 / (max - min)) * (valor - min));
	switch (a) {
	case 0:
		acenderLeds(0);
		break;
	case 1:
		acenderLeds(1);
		break;
	case 2:
		acenderLeds(3);
		break;
	case 3:
		acenderLeds(7);
		break;
	case 4:
		acenderLeds(15);
		break;
	}
	return 0;
}

int captaValueSwitch() {
	int saida = 0;
	if (SW1_GetVal()) {
		saida = saida | 1;
	}
	if (SW2_GetVal()) {
		saida = saida | 2;
	}
	if (SW3_GetVal()) {
		saida = saida | 4;
	}
	if (SW4_GetVal()) {
		saida = saida | 8;
	}
	return saida;
}

#define SENTIDO_L 0
#define SENTIDO_R 1
void setServo(int16 giro) {
	if (giro < ESQUERDA_SERVO) {
		giro = ESQUERDA_SERVO;
	}
	if (giro > DIREITO_SERVO) {
		giro = DIREITO_SERVO;
	}
	Servo1_SetDutyUS(giro);
}

void setTracao(int16 motor1, int16 motor2){
	if(motor1 <= 0) motor1 = 1;
	if(motor1 >= 1000) motor1 = 999;
	if(motor2 <= 0) motor2 = 1;
	if(motor2 >= 1000) motor2 = 999;
	TracaoA1PWM_SetDutyUS(motor1);
	TracaoB1PWM_SetDutyUS(motor2);
}

/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
	/* Write your local variable definition here */
	byte l = 0;
	int16 servo;
	int16 tracao1;
	int16 tracao2;
	int16 myoutput = 50;
	//PID
	double err = 0;
	double previos_err = 0;
	double integral = 0;
	double derivative = 0;
	
	#define WIDTH_TRACK 63
	#define LOCKIN_OFF 0
	#define LOCKIN_ON 1
	
	uint8 ladoFlag = LOCKIN_OFF;
	int8 width_incluido = WIDTH_TRACK;
	

	//## VARIAVEIS
	/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
	PE_low_level_init();
	/*** End of Processor Expert internal initialization.                    ***/

	/* Write your code here */
	/* For example: for(;;) { } */

	velocidade = captaValueSwitch() * 100;

	Analog1_Start();
	TracaoEnable_PutVal(1);
	TracaoA1PWM_SetDutyUS(velocidade);
	TracaoB1PWM_SetDutyUS(velocidade);
	TracaoA2_PutVal(0);
	TracaoB2_PutVal(0);

	while (TRUE) {
		if (cameraFinished == 1) {
			cameraFinished = 0;

			//processing
			if (maiorAmostra <= 50) {
				acenderLeds(0);
			} else {				
				//redimensiona sinal
				for (l = 0; l < 100; l++) {
					linha[l] = (((float) 255/(maiorAmostra - menorAmostra))*(linhaBruta[l]-menorAmostra)) > 176;
				}
				//find border
				uint8 contZeros = 0;
				int16 holes[10];
				int8 contHoles = -1;
				int16 falls[10];
				int8 contFalls = -1;
				uint8 zerosL = 0;
				uint8 zerosR = 0;
				uint8 i;
				for(i=0;i<99;i++){
					if(linha[i] == 1 && linha[i+1] == 0){
						contFalls++;
						falls[contFalls] = i;
						contZeros++;
					}
					else if(linha[i] == 0 && linha[i+1] == 1){
						contFalls++;
						falls[contFalls] = i;
						if(contZeros < 7 && contZeros != 0){
							contHoles++;
							holes[contHoles] = i-1;
						}
						contZeros = 0;
					}
					if(linha[i] == 0){
						if(contZeros > 0) contZeros++;
						if(i < 50) zerosL++;
						else zerosR++;
					}
				}
				int16 borda1 = -1000;
				int16 borda2 = 1000;
				if(contHoles > -1){
					for(i=0;i<=contHoles;i++){
						if(holes[i] < 50){
							if(holes[i] > borda1) borda1 = holes[i];
						}
						else {
							if(holes[i] < borda2) borda2 = holes[i];
						}
					}
				}
				else {
					for(i=0;i<=contFalls;i++){
						if(falls[i] < 50){
							if(falls[i] > borda1) borda1 = falls[i];
						}
						else{
							if(falls[i] < borda2) borda2 = falls[i];
						}
					}
				}
				
//				//ALGORTIMO TIPO 1 FUNDO PRETO
//				#define WIDTH_TRACK_MIN 60
//				#define WIDTH_TRACK_MAX 70
//				#define WIDTH_TRACK 63
//				#define ZEROS_MAX 17
//				if(borda1 != -1000 && borda2 != 1000){
//					int16 width;
//					width = borda2 - borda1;
//					if(!(width >= WIDTH_TRACK_MIN && width <= WIDTH_TRACK_MAX)){
//						if((50-borda1) < (borda2-50)) borda2 = borda1 + WIDTH_TRACK;
//						else borda1 = borda2 - WIDTH_TRACK;
//					}
//				}
//				else if(borda1 != -1000){
//					if(zerosR > ZEROS_MAX) borda2 = borda1 - WIDTH_TRACK;
//					else borda2 = borda1 + WIDTH_TRACK;
//				}
//				else if(borda2 != 1000){
//					if(zerosL > ZEROS_MAX) borda1 = borda2 + WIDTH_TRACK;
//					else borda1 = borda2 - WIDTH_TRACK;
//				}
				
				//ALGORITMO TIPO 2 FUNDO BRANCO
				#define WIDTH_TRACK_MIN 55
				#define WIDTH_TRACK_MAX 73
				if(borda1 != -1000 && borda2 != 1000){
					int8 width;
					width = borda2 - borda1;
					if(width >= WIDTH_TRACK_MIN && width <= WIDTH_TRACK_MAX){
						ladoFlag = LOCKIN_OFF;
					}
					else {
						if((50-borda1) < (borda2-50)) borda2 = 1000;
						else borda1 = -1000;
					}
				}
				
				
				
			    if((borda1 != -1000 && borda2 == 1000) || (borda1 == -1000 && borda2 != 1000)){
			        if(ladoFlag == LOCKIN_OFF){
			            if(borda1 != -1000){
			                width_incluido = WIDTH_TRACK;
			            }else{
			                width_incluido = -WIDTH_TRACK;
			                borda1 = borda2;
			            }
			            ladoFlag = LOCKIN_ON;
			        } else {
			            if(borda2 != 1000) borda1 = borda2;
			        }
			        borda2 = borda1 + width_incluido;
			    }
				
				myoutput = (float)(borda1+borda2)/2;
				
				
				
				if(myoutput > 48 && myoutput <= 52) acenderLeds(0b0110);
				else if(myoutput > 25 && myoutput <= 48) acenderLeds(0b1100);
				else if(myoutput > 0 && myoutput <= 25) acenderLeds(0b1000);
				else if(myoutput > 52 && myoutput <= 75) acenderLeds(0b0011);
				else if(myoutput > 75 && myoutput < 100) acenderLeds(0b0001);
				else if(myoutput <=0 || myoutput >= 100) acenderLeds(0);
				
				
				#define KP 2.3
				#define KD 0
				#define KI 0
				err = 50 - myoutput;
				derivative = (err - previos_err);
				integral = integral + err;
				myoutput = KP*err + KI*integral + KD*derivative;
				
				#define MAX_TRACAO 900
				#define MIN_TRACAO 1000
				
				servo = ESQUERDA_SERVO + (LIDERDADE_SERVO * 2 * (0.5 + ((float) myoutput/100)));
				
				tracao1 = MAX_TRACAO+(MIN_TRACAO-MAX_TRACAO)*(((float) -err/100) + 0.5);
				tracao2 = MAX_TRACAO+(MIN_TRACAO-MAX_TRACAO)*(((float) err/100) + 0.5);
				
				setServo(servo);
				setTracao(tracao1,tracao2);
				previos_err = err;
				
			}
			
			maiorAmostra = 0;
			menorAmostra = 255;
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
