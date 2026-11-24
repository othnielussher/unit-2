/*
 *	Project Owner	: TU Chemnitz
 *	Project Name	: ASE Tutorial Unit-2
 *	File Name		: siu.c
 *	Author			: ASE Admin
 *  Created on		: 25 Feb 2024
 */

/* Includes ******************************************************************/
#include "siu.h"

/*
 * @brief	SIU pin configuration function
 *
 * @param	void
 * @retval	void
 */
void SIU_Init(void){

	/* Input pin configurations */
	SIU.PCR[PCR_BT1].R = 0x0100;
	SIU.PCR[PCR_BT2].R = 0x0100;
	SIU.PCR[PCR_SW1].R = 0x0100;
	SIU.PCR[PCR_SW2].R = 0x0100;
	SIU.PCR[PCR_SW3].R = 0x0100;
	SIU.PCR[PCR_SW4].R = 0x0100;


	/* Output pin configurations */
	SIU.PCR[PCR_LED_P].R = 0x0200;
	SIU.PCR[PCR_LED_Rx].R = 0x0200;
	SIU.PCR[PCR_LED_Tx].R = 0x0200;
	SIU.PCR[PCR_LED_U1].R = 0x0200;
	SIU.PCR[PCR_LED_U2].R = 0x0200;
	SIU.PCR[PCR_LED_U3].R = 0x0200;


	/* Analog pin configurations */
	SIU.PCR[PCR_POT].R = 0x2500;
	SIU.PCR[PCR_LDR].R = 0x2500;



	/* TO-DO: your task implementations **************************************/

	/* Setup CAN controller pins */

	/* TX - transmitting */
	SIU.PCR[PCR_CAN_TX].B.PA  = 1;	/* select alternate function 1 (CAN0 TX) */
	SIU.PCR[PCR_CAN_TX].B.OBE = 1;	/* output buffer on - we drive this pin */
	SIU.PCR[PCR_CAN_TX].B.IBE = 0;	/* input buffer off - we never read it */

	/* RX - receiving */
	SIU.PCR[PCR_CAN_RX].B.PA  = 1;	/* select alternate function 1 (CAN0 RX) */
	SIU.PCR[PCR_CAN_RX].B.OBE = 0;	/* output buffer off - we never drive this pin */
	SIU.PCR[PCR_CAN_RX].B.IBE = 1;	/* input buffer on - we read the bus level */

	SIU.PSMI[0].B.PADSEL = 0x1;
	SIU.PSMI[33].B.PADSEL = 0x1;

	/*************************************************************************/
}

