/*
 *	Project Owner	: TU Chemnitz
 *	Project Name	: ASE Tutorial Unit-2
 *	File Name		: can.h
 *	Author			: ASE Admin
 *  Created on		: 25 Feb 2024
 */

#ifndef CAN_H_
#define CAN_H_

/* Includes ******************************************************************/
#include "can_lld.h"
#include "can_lld_cfg.h"


/* TO-DO: your task implementations ******************************************/

/* Macro definitions *********************************************************/
/*
 * Accept only IDs 0x10 and 0x11 (11-bit base frame IDs):
 *   0x10 = 000 0001 0000
 *   0x11 = 000 0001 0001
 *                      ^ only bit 0 differs
 * Mask:       1 where the bits are the same, 0 where they differ -> 111 1111 1110 = 0x7FE
 * Acceptance: the common bit values (don't-care bit set to 0)     -> 000 0001 0000 = 0x010
 */
#define	CAN_MASK_REGISTER			0x7FE
#define	CAN_ACCEPTANCE_REGISTER  	0x010

/* Message buffers used for sending (task sheet: choose indices 8 - 14) */
#define	MB_ENGINE_ON	8	/* ID 0xDA0, extended frame, 0 bytes */
#define	MB_SPEED		9	/* ID 0x5A0, standard frame, 1 byte  */
#define	MB_RPM			10	/* ID 0x280, standard frame, 2 bytes */
#define	MB_LIGHTS		11	/* ID 0x470, standard frame, 1 byte  */

/* FlexCAN CODE values for Tx buffers (can_buffer.pdf, Table 400) */
#define	CAN_CODE_TX_INACTIVE	0x8	/* 1000: buffer not taking part in sending */
#define	CAN_CODE_TX_DATA		0xC	/* 1100: transmit data frame once */


/*****************************************************************************/


/* Function prototypes *******************************************************/
void CAN_Init(void);
void CAN_TxMessage(CANTxFrame txMsg, canmbx_t mbx);

#endif /* CAN_H_ */
