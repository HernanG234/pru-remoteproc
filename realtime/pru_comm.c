#include <stdint.h>
#include <string.h>
#include <pru_cfg.h>
#include <pru_intc.h>
#include <pru_rpmsg.h>
#include "resource_table.h"

register volatile unsigned __R31;
#define VIRTIO_CONFIG_S_DRIVER_OK 4

struct pru_rpmsg_transport transport;
unsigned short rpmsg_src,rpmsg_dst;
unsigned char buffer[RPMSG_BUF_SIZE], *pbuffer;
unsigned short buffer_state=0,plen=0;

unsigned short rpmsg_init(void){
 volatile unsigned char *status;
 static unsigned short rpmsg_state=0;

 switch (rpmsg_state){
 case 0:
 CT_INTC.SICR_bit.STS_CLR_IDX = FROM_ARM_HOST;
 rpmsg_state = 1;
 break;
 case 1:
 status = &resourceTable.rpmsg_vdev.status;
 if((*status & VIRTIO_CONFIG_S_DRIVER_OK)){ 

 pru_rpmsg_init(&transport, &resourceTable.rpmsg_vring0, &resourceTable.rpmsg_vring1, TO_ARM_HOST, FROM_ARM_HOST);
 rpmsg_state = 2;
 }
 break;
 case 2:
 if (pru_rpmsg_channel(RPMSG_NS_CREATE, &transport, CHAN_NAME, CHAN_DESC, CHAN_PORT) == PRU_RPMSG_SUCCESS){ rpmsg_state = 3;
 }
 break;
 }
 return(rpmsg_state);
}

void listen(void){
 if(buffer_state == 1) return;
 if(! (__R31 & HOST_INT)) return;
 if (pru_rpmsg_receive(&transport, &rpmsg_src, &rpmsg_dst, buffer, &plen) == PRU_RPMSG_SUCCESS){
 buffer_state = 1;
 pbuffer = buffer; 
 return;
 }
 CT_INTC.SICR_bit.STS_CLR_IDX = FROM_ARM_HOST; 
 return;
}

void parse_the_message(unsigned char *action_state){
 static unsigned short command_state=0;
 if(buffer_state == 0) return;
 for(;plen > 0;plen--,pbuffer++){
 switch(command_state){
 case 0:
 if(*pbuffer == 'a') command_state = 1;
 break;
 case 1:
 if(*pbuffer == 'f') command_state = 2;
 break;
 case 2:
 *action_state = *pbuffer;
 command_state = 0;
 break;
 default:
 command_state = 0;
 break;

 }
 }
 buffer_state = 0;
}

unsigned short send(char *buf){
 return(pru_rpmsg_send(&transport,rpmsg_dst, rpmsg_src, buf,strlen(buf)));

}
