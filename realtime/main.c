

#include <stdint.h>
#include <pru_cfg.h>
#include <pru_ctrl.h>

volatile register unsigned __R30;
volatile register unsigned __R31;

unsigned short rpmsg_init(void);
void parse_the_message(unsigned char *action_state);
unsigned short send(char *buf);
void listen(void);

#ifdef PRU0
#define SET_MASK (1<<14)
#define CLEAR_MASK ~(1<<14)
#define MESSAGE "PRU0 responding"
#endif
#ifdef PRU1
#define SET_MASK 0
#define CLEAR_MASK ~0
#define MESSAGE "PRU1 responding"
#endif

#define LOOP 10000 //5 ns * 10000 = .05 msec cycle time

void main(void)
{
 unsigned short toggle=0;
 unsigned char action_state=0;
 CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;
 #ifdef PRU0
 PRU0_CTRL.CTRL_bit.CTR_EN = 1; //turn on cycle counter
 #endif
 #ifdef PRU1
 PRU1_CTRL.CTRL_bit.CTR_EN = 1; //turn on cycle counter
 #endif
 while (1) {
 //here's where you insert real-time code
 if(rpmsg_init() == 3){
 listen();
 parse_the_message(&action_state);
 switch(action_state){
 case 'S':
 __R30 ^= SET_MASK;
 if(toggle == 0){
 toggle = 1;
 send(MESSAGE);
 }
 break;
 case 's':
 __R30 &= CLEAR_MASK;
 action_state = 0;
 break;
 }
 }

 #ifdef PRU0
 while(PRU0_CTRL.CYCLE < LOOP);
 PRU0_CTRL.CYCLE = 0; //reset the cycle counter/stall counter
 #endif
 #ifdef PRU1
 while(PRU1_CTRL.CYCLE < LOOP);
 PRU1_CTRL.CYCLE = 0; //reset the cycle counter/stall counter
 #endif
 }
}


