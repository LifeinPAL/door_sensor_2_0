/**@file
 *
 */
 


#ifndef __MAIN_H__
#define __MAIN_H__


/* private include */
#include "common_abbr.h"


/* export typedef */

typedef enum{
    LTS_NO_BOND = 0x55,
    LTS_BOND_CONN = 0x5A,
    LTS_BOND_UNCONN = 0xAA
}long_term_status_t;

typedef enum{
	SM_IDLE_READY,
	SM_MISSION_COMPLETE,
	SM_AUTHENTICATION,
	SM_COMMAND_EXECUTION,
	SM_DATA_SENDING,
	SM_RESPONSE_HANDLING
}status_machine_t;

typedef enum{
	USER_SUCCESS,
	USER_FAILED,
	USER_TIMEOUT,
	USER_INVALID = 0xFF
}U_FUN_STATUS;


/* export variable */

extern long_term_status_t 	g_lts;
extern status_machine_t 	g_sm;
extern const char* g_string_status[4];












/**< private include >*/
#include "nrf.h"


/**< global define >*/
typedef struct{
	const char* p_frame_head;
	uint8_t door_state;
	uint16_t battery_voltage;
	uint8_t wkp_source;
	uint16_t timestamp;
	uint32_t g_packet_no;
	uint8_t err_code;
	const char* p_frame_end;	
}g_data_collection;



/**< private define >*/



/**< export variable >*/


/**< export function prototype. */


#endif // __MAIN_H__


