#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define CASSY_DEV "/dev/rpmsg_pru30"
#define SIZE 100

int main(void)
{
 //struct pollfd CASSY;
 int retval,CASSY,flags,k;
 unsigned char buf[SIZE];

 if((CASSY = open(CASSY_DEV, O_RDWR)) < 0) exit (0);
 flags = fcntl(CASSY, F_GETFL, 0);
 fcntl(CASSY, F_SETFL, flags | O_NONBLOCK);

 retval = write(CASSY, "afS", 3);
 while((retval = read(CASSY, buf, SIZE)) > 0){
 printf("retval = %d, buf = %s\n",retval,buf);
 }
 for(k=0;k<10000000;k++);
 retval = write(CASSY, "afs", 3);
 while((retval = read(CASSY, buf, SIZE)) > 0){
 printf("retval = %d, buf = %s\n",retval,buf);
 }

 close(CASSY);
 return 0;
}
