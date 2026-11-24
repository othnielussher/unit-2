/*
 *	Project Owner	: TU Chemnitz
 *	Project Name	: ASE Tutorial Unit-2
 *	File Name		: can.c
 *	Author			: ASE Admin
 *  Created on		: 25 Feb 2024
 */

/* Includes ******************************************************************/
#include "can.h"
#include "xpc56el.h"

/*
 * @brief	CAN0 initialization function
 *
 * @param	void
 * @retval	void
 */
void CAN_Init(void){
	can_lld_start(&CAND1, &can_config_cfg0);


	/* TO-DO: pre-configure message buffer(s) for sending here ***********************/

	/* Engine On: ID 0xDA0 > 0x7FF -> does not fit in 11 bits -> extended frame */
	CAN_0.BUF[MB_ENGINE_ON].MSG_CS.B.CODE   = CAN_CODE_TX_INACTIVE;
	CAN_0.BUF[MB_ENGINE_ON].MSG_CS.B.IDE    = 1;		/* extended frame */
	CAN_0.BUF[MB_ENGINE_ON].MSG_CS.B.SRR    = 1;		/* must be 1 when sending extended frames */
	CAN_0.BUF[MB_ENGINE_ON].MSG_ID.B.STD_ID = 0;		/* unused field -> 0 */
	CAN_0.BUF[MB_ENGINE_ON].MSG_ID.B.EXT_ID = 0xDA0;
	CAN_0.BUF[MB_ENGINE_ON].MSG_CS.B.RTR    = 0;		/* data frame */
	CAN_0.BUF[MB_ENGINE_ON].MSG_CS.B.LENGTH = 0;		/* no payload */

	/* Speed: ID 0x5A0 <= 0x7FF -> standard frame */
	CAN_0.BUF[MB_SPEED].MSG_CS.B.CODE   = CAN_CODE_TX_INACTIVE;
	CAN_0.BUF[MB_SPEED].MSG_CS.B.IDE    = 0;
	CAN_0.BUF[MB_SPEED].MSG_ID.B.STD_ID = 0x5A0;
	CAN_0.BUF[MB_SPEED].MSG_ID.B.EXT_ID = 0;
	CAN_0.BUF[MB_SPEED].MSG_CS.B.RTR    = 0;
	CAN_0.BUF[MB_SPEED].MSG_CS.B.LENGTH = 1;

	/* RPM: ID 0x280 -> standard frame */
	CAN_0.BUF[MB_RPM].MSG_CS.B.CODE   = CAN_CODE_TX_INACTIVE;
	CAN_0.BUF[MB_RPM].MSG_CS.B.IDE    = 0;
	CAN_0.BUF[MB_RPM].MSG_ID.B.STD_ID = 0x280;
	CAN_0.BUF[MB_RPM].MSG_ID.B.EXT_ID = 0;
	CAN_0.BUF[MB_RPM].MSG_CS.B.RTR    = 0;
	CAN_0.BUF[MB_RPM].MSG_CS.B.LENGTH = 2;

	/* Lights: ID 0x470 -> standard frame */
	CAN_0.BUF[MB_LIGHTS].MSG_CS.B.CODE   = CAN_CODE_TX_INACTIVE;
	CAN_0.BUF[MB_LIGHTS].MSG_CS.B.IDE    = 0;
	CAN_0.BUF[MB_LIGHTS].MSG_ID.B.STD_ID = 0x470;
	CAN_0.BUF[MB_LIGHTS].MSG_ID.B.EXT_ID = 0;
	CAN_0.BUF[MB_LIGHTS].MSG_CS.B.RTR    = 0;
	CAN_0.BUF[MB_LIGHTS].MSG_CS.B.LENGTH = 1;

	/*************************************************************************/
}


/*
 * @brief	CAN message transmit function
 *
 * @param	CANTxFrame	txMsg
 * @param	canmbx_t	mbx
 * @retval	void
 */
void CAN_TxMessage(CANTxFrame txMsg, canmbx_t mbx){
	// nop
}
