/*
 *	Project Owner	: TU Chemnitz
 *	Project Name	: ASE Tutorial Unit-2
 *	File Name		: main.c
 *	Author			: ASE Admin
 *  Created on		: 25 Feb 2024
 */

/* Includes ******************************************************************/
#include "init.h"


/* Global variables **********************************************************/
int speed = 0;
int rpm = 0;

/* "volatile" = may change inside an interrupt, so the compiler must re-read it every time */
volatile int engine_running = 0;	/* 0 = idle mode, 1 = engine on (sending messages)    */
volatile int send_due = 0;			/* set every 100 ms by PIT channel 2                    */
volatile int blink_on = 0;			/* toggled every 500 ms by PIT channel 3 (indicators)   */
int rpm_step = -250;				/* RPM goes down first, then up, then down ...          */


void can_send_engine_on(void){
	/* ID, frame format and length (0) were pre-configured in CAN_Init(); no payload */
	CAN_0.BUF[MB_ENGINE_ON].MSG_CS.B.CODE = CAN_CODE_TX_DATA;	/* "send it now" */
}

void can_send_speed(int value){
	CAN_0.BUF[MB_SPEED].DATA.B[0] = (uint8_t)value;			/* speed in km/h */
	CAN_0.BUF[MB_SPEED].MSG_CS.B.CODE = CAN_CODE_TX_DATA;
}

void can_send_rpm(int value){
	/* little endian: low byte first, then high byte */
	CAN_0.BUF[MB_RPM].DATA.B[0] = (uint8_t)(value & 0xFF);			/* lower byte  */
	CAN_0.BUF[MB_RPM].DATA.B[1] = (uint8_t)((value >> 8) & 0xFF);	/* higher byte */
	CAN_0.BUF[MB_RPM].MSG_CS.B.CODE = CAN_CODE_TX_DATA;
}

void can_send_lights(void){
	/* Table 1: SW1 = right, SW2 = left, both = hazard */
	uint8_t state = 0x00;
	if (SW2) state |= 0x01;		/* left indicator  */
	if (SW1) state |= 0x02;		/* right indicator -> both = 0x03 = hazard */

	/* blink: onboard LEDs and dashboard indicators on/off together */
	if (!blink_on) state = 0x00;
	LED_U1 = (state & 0x01) ? 1 : 0;
	LED_U3 = (state & 0x02) ? 1 : 0;

	CAN_0.BUF[MB_LIGHTS].DATA.B[0] = state;
	CAN_0.BUF[MB_LIGHTS].MSG_CS.B.CODE = CAN_CODE_TX_DATA;
}

void can_send(void){
	/* idle mode, or the 100 ms period is not over yet -> send nothing */
	if (!engine_running || !send_due) {
		return;
	}
	send_due = 0;

	/* potentiometer 0..4095 -> speed 0..240 km/h */
	speed = (POT * 240) / 4095;

	can_send_engine_on();
	can_send_speed(speed);
	can_send_rpm(rpm);
	can_send_lights();

	/* toggle once per batch (toggling once per message = 4 toggles = no visible change) */
	LED_Tx = !LED_Tx;
}


/*
 * @brief	Main program
 *
 * @param	void
 * @retval	int
 */
int main(void) {
	/* peripherals initialization */
	peripheralsInit();



	/* TO-DO: your task implementations **************************************/

	/* Configure and start timer channels (the argument is the period in ms) */
	/* PIT timer channel 1, period = 1000 ms -> RPM step */
	PIT_ConfigureTimer(1, 1000);
	PIT_StartTimer(1);

	/* PIT timer channel 2, period = 100 ms -> send all messages */
	PIT_ConfigureTimer(2, 100);
	PIT_StartTimer(2);

	/* PIT timer channel 3, period = 500 ms -> indicator blinking */
	PIT_ConfigureTimer(3, 500);
	PIT_StartTimer(3);

	/*************************************************************************/


	/* main program */
	while(1){
		/* System function */
		systemFunction();

		/* TO-DO: your task implementations **********************************/

		can_send();

		/*********************************************************************/

		/* 10 ms OS delay */
		osalThreadDelayMilliseconds(10UL);
	}
}


/*
 * @brief	CAN receive interrupt
 *
 * @param   CANDriver   canp	The CAN controller which received a message
 * @param	CANRxFrame	crfp	The message which was received
 * @retval	void
 */
void can_receive(CANRxFrame crfp) {
    /* Task 1: any message that passed the filter -> toggle LED_Rx */
    LED_Rx = !LED_Rx;

    if(crfp.IDE == CAN_IDE_STD) // a standard message frame has been received
    {
        if (crfp.SID == 0x11)        /* "Start Engine" */
        {
            /* Task 1 asked to toggle LED_U1 here; in Task 3 LED_U1 is the left
               indicator, so 0x11 now starts the engine instead */
            if (!engine_running) {
                rpm = 8000;
                rpm_step = -250;
                engine_running = 1;
                LED_P = 1;
            }
        }
        else if (crfp.SID == 0x10)   /* "Stop Engine" */
        {
            engine_running = 0;
            LED_P = 0;
            LED_U1 = 0;
            LED_U3 = 0;
        }
    }
}


/*
 * @brief	PIT timer channel 1 IRQ callback
 *
 * @param	void
 * @retval	void
 */
void PIT_Channel_1(void){
	/* every second: 8000 -> 0 in steps of 250, then back up to 8000, forever */
	if (!engine_running) return;

	rpm += rpm_step;
	if (rpm <= 0)    { rpm = 0;    rpm_step = +250; }
	if (rpm >= 8000) { rpm = 8000; rpm_step = -250; }

}


/*
 * @brief	PIT timer channel 2 IRQ callback
 *
 * @param	void
 * @retval	void
 */
void PIT_Channel_2(void){
	send_due = 1;	/* tell the main loop: 100 ms are over, send again */

}


/*
 * @brief	PIT timer channel 3 IRQ callback
 *
 * @param	void
 * @retval	void
 */
void PIT_Channel_3(void){
	blink_on = !blink_on;	/* indicator on/off phase */

}
