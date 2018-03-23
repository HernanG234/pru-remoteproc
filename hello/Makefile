hello: hello.c
	gcc hello.c -o hello

PRU_RPMSG_ROOT:= /usr/lib/ti/pru-software-support-package/
PRU_INCLUDE:= --include_path=/usr/include/arm-linux-gnueabihf/ --include_path=$(PRU_RPMSG_ROOT)include/ --include_path=$(PRU_RPMSG_ROOT)include/am335x/
CODE_ROOT:=/root/hello/
PRU0_ROOT:= $(CODE_ROOT)pru0/
PRU1_ROOT:= $(CODE_ROOT)pru1/
LINKER_CMD_FILE:= hello.cmd
PRU_TOOLS:=/usr/bin/

CFLAGS=-v3 -O2 --endian=little --hardware_mac=on

LDFLAGS+= -L.


am335x-pru0-fw: $(CODE_ROOT)main.c
	$(PRU_TOOLS)clpru $(CFLAGS) $(PRU_INCLUDE) -ppd -ppa -fe $(PRU0_ROOT)/main0.object $(CODE_ROOT)main.c -D PRU0
	$(PRU_TOOLS)clpru -z $(LINKER_CMD_FILE) -o $(PRU0_ROOT)am335x-pru0-fw $(PRU0_ROOT)/main0.object -l$(PRU_RPMSG_ROOT)lib/rpmsg_lib.lib

am335x-pru1-fw: $(CODE_ROOT)main.c
	$(PRU_TOOLS)clpru $(CFLAGS) $(PRU_INCLUDE) -ppd -ppa -fe $(PRU1_ROOT)/main1.object $(CODE_ROOT)main.c -D PRU1
	$(PRU_TOOLS)clpru -z $(LINKER_CMD_FILE) -o $(PRU1_ROOT)am335x-pru1-fw $(PRU1_ROOT)/main1.object -l$(PRU_RPMSG_ROOT)lib/rpmsg_lib.lib

install: am335x-pru0-fw am335x-pru1-fw
	dmesg --clear
	cp $(PRU0_ROOT)/am335x-pru0-fw /lib/firmware
	cp $(PRU1_ROOT)/am335x-pru1-fw /lib/firmware
	rmmod -f pru_rproc
	dmesg
	echo 'separator'
	echo 'separator'
	dmesg --clear
	modprobe pru_rproc
	dmesg
clean:
	rm $(PRU0_ROOT)/am335x-pru0-fw
	rm $(PRU1_ROOT)/am335x-pru1-fw
	rm $(PRU0_ROOT)/*.object
	rm $(PRU1_ROOT)/*.object
	rm *.pp

#stuff from the TI makefile that might be needed later
#CFLAGS=-v3 -O2 --display_error_number --endian=little --hardware_mac=on --obj_directory=$(GEN_DIR) --pp_directory=$(GEN_DIR) -ppd -ppa
#LFLAGS=--reread_libs --warn_sections --stack_size=$(STACK_SIZE) --heap_size=$(HEAP_SIZE)

#link
#$(PRU_CGT)/bin/clpru $(CFLAGS) -z -i$(PRU_CGT)/lib $(PRU_CGT)/include $(LFLAGS) -o $(TARGET) $(OBJECTS) -m$(MAP) $(LINKER_COMMAND_FILE) --library=libc.a $(LIBS)

#compile
#	$(PRU_CGT)/bin/clpru --include_path=$(PRU_CGT)/include $(INCLUDE) $(CFLAGS) -fe $@ $<
