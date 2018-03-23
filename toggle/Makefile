toggle: toggle.c
 gcc toggle.c -o toggle

CODE_ROOT:=/root/toggle/
LINKER_CMD_FILE:= toggle.cmd

PRU_RPMSG_ROOT:= /usr/lib/ti/pru-software-support-package/
PRU_INCLUDE:= --include_path=/usr/include/arm-linux-gnueabihf/ --include_path=$(PRU_RPMSG_ROOT)include/ --include_path=$(PRU_RPMSG_ROOT)include/am335x/
PRU0_ROOT:= $(CODE_ROOT)pru0/
PRU1_ROOT:= $(CODE_ROOT)pru1/
PRU_TOOLS:=/usr/bin/

CFLAGS=-v3 -O2 --endian=little --hardware_mac=on

LDFLAGS+= -L.

test1.object: test1.c
 $(PRU_TOOLS)clpru $(CFLAGS) $(PRU_INCLUDE) -ppd -ppa -fe test1.object test1.c
test2.object: test2.c
 $(PRU_TOOLS)clpru $(CFLAGS) $(PRU_INCLUDE) -ppd -ppa -fe test2.object test2.c
test: test1.object test2.object
 $(PRU_TOOLS)clpru -z $(LINKER_CMD_FILE) -o test test1.object test2.object -l$(PRU_RPMSG_ROOT)lib/rpmsg_lib.lib

$(PRU0_ROOT)pru_comm0.object: $(CODE_ROOT)pru_comm.c
 $(PRU_TOOLS)clpru $(CFLAGS) $(PRU_INCLUDE) -ppd -ppa -fe $(PRU0_ROOT)pru_comm0.object $(CODE_ROOT)pru_comm.c -D PRU0

$(PRU0_ROOT)main0.object: $(CODE_ROOT)main.c
 $(PRU_TOOLS)clpru $(CFLAGS) $(PRU_INCLUDE) -ppd -ppa -fe $(PRU0_ROOT)main0.object $(CODE_ROOT)main.c -D PRU0

am335x-pru0-fw: $(PRU0_ROOT)main0.object $(PRU0_ROOT)pru_comm0.object
 $(PRU_TOOLS)clpru -z $(LINKER_CMD_FILE) -o $(PRU0_ROOT)am335x-pru0-fw $(PRU0_ROOT)main0.object $(PRU0_ROOT)pru_comm0.object -l$(PRU_RPMSG_ROOT)lib/rpmsg_lib.lib

$(PRU1_ROOT)pru_comm1.object: $(CODE_ROOT)pru_comm.c
 $(PRU_TOOLS)clpru $(CFLAGS) $(PRU_INCLUDE) -ppd -ppa -fe $(PRU1_ROOT)pru_comm1.object $(CODE_ROOT)pru_comm.c -D PRU1

$(PRU1_ROOT)main1.object: $(CODE_ROOT)main.c
 $(PRU_TOOLS)clpru $(CFLAGS) $(PRU_INCLUDE) -ppd -ppa -fe $(PRU1_ROOT)main1.object $(CODE_ROOT)main.c -D PRU1

am335x-pru1-fw: $(PRU1_ROOT)main1.object $(PRU1_ROOT)pru_comm1.object
 $(PRU_TOOLS)clpru -z $(LINKER_CMD_FILE) -o $(PRU1_ROOT)am335x-pru1-fw $(PRU1_ROOT)main1.object $(PRU1_ROOT)pru_comm1.object -l$(PRU_RPMSG_ROOT)lib/rpmsg_lib.lib

install: am335x-pru0-fw am335x-pru1-fw
 dmesg --clear
 cp $(PRU0_ROOT)/am335x-pru0-fw /lib/firmware
 cp $(PRU1_ROOT)/am335x-pru1-fw /lib/firmware
 rmmod -f pru_rproc
# dmesg
# echo 'separator'
# echo 'separator'
# dmesg --clear
 modprobe pru_rproc
# dmesg
 config-pin P8.12 pruout
clean:
 rm $(PRU0_ROOT)/am335x-pru0-fw
 rm $(PRU1_ROOT)/am335x-pru1-fw
 rm $(PRU0_ROOT)/*.object
 rm $(PRU1_ROOT)/*.object
 rm *.pp
 config-pin P8.12 default
