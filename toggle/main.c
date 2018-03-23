#include <stdint.h>
#include <pru_cfg.h>

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

void main(void)
{
 unsigned short toggle=0;
 unsigned char action_state=0;
 CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;
 
 while (1) { //this will be replaced with the real-time loop.
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
 }
}
