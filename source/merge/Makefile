# Makefile for Dynamips 0.2.8
# Copyright (c) 2005-2006 Christophe Fillot.

# Replace x86 by amd64 for a build on x86_64.
# Use "nojit" for architectures that are not x86 or x86_64.
DYNAMIPS_ARCH?=x86
#DYNAMIPS_ARCH?=amd64
#DYNAMIPS_ARCH?=nojit

ifeq ($(DYNAMIPS_ARCH),ppc32)
DYNAMIPS_ARCH=x86
endif

# Do you want to use lib (for 32 bit compiling) or lib64
DYNAMIPS_LIB?=lib


# Get include files from the current directory and from the common directory
INCLUDE+=-I. -I../common -I../contrib

# Change this to 0 if your system doesn't support RFC 2553 extensions
HAS_RFC2553?=0

# Change this to 1 if your system has libpcap-0.9.4 or better 
# (WinPcap is used for Cygwin)
HAS_PCAP?=1
#BIOCFEEDBACK?=1

# Change this to 1 if your system has posix_memalign
#HAS_POSIX_MEMALIGN?=1
HAS_POSIX_MEMALIGN?=1

# Current dynamips release
VERSION_TRAIN?=0.2.16
VERSION_SUB?=-experimental\(merge\ uppc\ smips\)Build-1

VERSION=$(VERSION_TRAIN)$(VERSION_SUB)
VERSION_DEV=$(VERSION_TRAIN)-$(shell date +%Y%m%d-%H)

# If make cannot find a file, try the common directory
VPATH = ../common ../contrib

# Executable binary extension
DESTDIR?=/usr
BIN_EXT?=.exe

CC=gcc
LD=ld
RM=rm
TAR=tar
CP=cp
LEX=flex
MIPS64_ARCH_INC_FILE=\"mips64_$(DYNAMIPS_ARCH)_trans.h\"
PPC32_ARCH_INC_FILE=\"ppc32_$(DYNAMIPS_ARCH)_trans.h\"
OSNAME=MinGW

# CFLAGS+= $(INCLUDE) -Wall -O2 -fomit-frame-pointer  \
# CFLAGS+= $(INCLUDE) -O0 -pg -g 
# CFLAGS+= $(INCLUDE) -Wall -O2 -march=native -fomit-frame-pointer  \
# CFLAGS+= $(INCLUDE) -Wall -O2 -pg -g -march=native -fomit-frame-pointer  \
#	-flto -ffast-math -mfpmath=387 \
# CFLAGS+= $(INCLUDE) -Wall -O2 -fomit-frame-pointer  \

#-fomit-frame-pointer
CFLAGS+= $(INCLUDE) -Wall -Ofast -g  \
	-DJIT_ARCH=\"$(DYNAMIPS_ARCH)\" -DJIT_CPU=CPU_$(DYNAMIPS_ARCH) \
	-DMIPS64_ARCH_INC_FILE=$(MIPS64_ARCH_INC_FILE) \
	-DPPC32_ARCH_INC_FILE=$(PPC32_ARCH_INC_FILE) \
	-DDYNAMIPS_VERSION=\"$(VERSION)\" \
	-D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE \
	-DHAS_RFC2553=$(HAS_RFC2553) \
	-DHAS_POSIX_MEMALIGN=$(HAS_POSIX_MEMALIGN)

#PCAP_LIB=/usr/local/lib/libpcap.a
#PCAP_LIB?=-lwpcap

ifeq ($(shell uname), FreeBSD)
   PTHREAD_LIBS?=-pthread
   LOCALBASE?=/usr/local
   OSNAME=FreeBSD
   CFLAGS+=-I$(LOCALBASE)/include -I$(LOCALBASE)/include/libelf \
	$(PTHREAD_CFLAGS) -D_FILE_OFFSET_BITS=64
   LIBS=-L$(LOCALBASE)/lib -L. -L../contrib/libelf-0.8.13/lib/ -lelf $(PTHREAD_LIBS) $(LDFLAGS)
else
ifeq ($(shell uname), Linux)
   PTHREAD_LIBS?=-lpthread
#   PCAP_LIB=-lpcap
   OSNAME=Linux
   CFLAGS+=-I/usr/include -I. $(PTHREAD_CFLAGS)
   LIBS=-L/usr/lib -L. -ldl -lrt -L../contrib/libelf-0.8.13/lib/ -lelf $(PTHREAD_LIBS)
   DESTDIR=/usr
else
ifeq ($(shell uname -s), Darwin)
   OSNAME=Darwin
   CFLAGS+=-I/usr/local/include -I/opt/local/include -mdynamic-no-pic -D_FILE_OFFSET_BITS=64
   LIBS=-L/usr/local/lib -L/opt/local/lib -L. -ldl -L../contrib/libelf-0.8.13/lib/ -lelf -lpthread
ifeq ($(DYNAMIPS_ARCH),amd64)
   CFLAGS+= -DMAC64HACK
endif
else
ifeq ($(shell uname -s), SunOS)
   OSNAME=SunOS
   CFLAGS+=-I/usr/local/include -DINADDR_NONE=0xFFFFFFFF \
	-I /opt/csw/include -DSUNOS
   LIBS=-L/usr/local/lib -L. -ldl -lelf -lpthread -L/opt/csw/lib \
	-lsocket -lnsl -lresolv
   PCAP_LIB=/opt/csw/lib/libpcap.a
else
ifeq ($(shell uname -o), Cygwin)
   OSNAME=Windows
   CFLAGS+=-I/usr/local/include -I/usr/local/include/libelf -I/usr/include -I/usr/include/libelf \
	-I/usr/include/winpcap -DCYGWIN -static -static-libgcc
   LIBS=-L/usr/local/lib -L/usr/lib -L. -lintl -lelf -lpthread
   PCAP_LIB=-lpacket -lwpcap
else
   CFLAGS+=-I/usr/include/libelf -D_FILE_OFFSET_BITS=64
   zLIBS=-L. -lelf -lpthread -lws2_32 -lwsock32 -lmingwex  ..\contrib\drmingw-0.8.0-win32\lib\libexchndl.a -static-libgcc -static
   LIBS=-L. -lpthread -lws2_32 -lwsock32 -lmingwex  ..\contrib\drmingw-0.8.0-win32\lib\libexchndl.a -static-libgcc -static
endif
endif
endif
endif
endif

CFLAGS+=-DOSNAME=$(OSNAME)

PROG=dynamips$(BIN_EXT)
PACKAGE=$(PROG)-$(VERSION)
ARCHIVE=$(PACKAGE).tar.gz

PACKAGE_DEV=$(PROG)-$(VERSION_DEV)
ARCHIVE_DEV=$(PACKAGE_DEV).tar.gz

# Header and source files
HDR=mempool.h registry.h rbtree.h hash.h utils.h parser.h plugin.h \
	crc.h sbox.h base64.h \
	net.h net_io.h net_io_bridge.h net_io_filter.h \
	atm.h atm_vsar.h atm_bridge.h frame_relay.h eth_switch.h \
	ptask.h timer.h dev_vtty.h hypervisor.h dynamips.h insn_lookup.h \
	vm.h cpu.h jit_op.h memory.h device.h \
	mips64.h mips64_mem.h mips64_exec.h mips64_jit.h mips64_cp0.h \
	ppc32.h ppc32_mem.h ppc32_exec.h ppc32_jit.h ppc32_vmtest.h \
	nmc93cX6.h cisco_eeprom.h cisco_card.h dev_ds1620.h dev_rom.h \
	pci_dev.h pci_io.h dev_mpc860.h dev_gt.h dev_mv64460.h dev_plx.h \
	dev_dec21140.h dev_am79c971.h dev_i8254x.h dev_i8255x.h \
	dev_mueslix.h dev_nm_16esw.h dev_wic_serial.h \
	dev_c7200.h dev_c7200_mpfpga.h \
	dev_c3600.h dev_c3600_iofpga.h dev_c3600_bay.h \
	dev_c2691.h dev_c2691_iofpga.h \
	dev_c3725.h dev_c3725_iofpga.h \
	dev_c3745.h dev_c3745_iofpga.h \
	dev_c2600.h dev_c2600_iofpga.h \
	dev_c1700.h dev_c1700_iofpga.h \
	dev_c6msfc1.h dev_c6msfc1_mpfpga.h \
	dev_c6sup1.h dev_c6sup1_mpfpga.h \
	rommon_var.h get_cpu_time.h

SOURCES=mempool.c registry.c rbtree.c hash.c sbox.c utils.c parser.c \
	plugin.c ptask.c timer.c crc.c base64.c \
	net.c net_io.c net_io_bridge.c net_io_filter.c \
	atm.c atm_vsar.c atm_bridge.c frame_relay.c eth_switch.c \
	dynamips.c insn_lookup.c vm.c cpu.c jit_op.c \
	mips64.c mips64_mem.c mips64_cp0.c mips64_jit.c mips64_exec.c \
	ppc32.c ppc32_mem.c ppc32_jit.c ppc32_exec.c ppc32_vmtest.c \
	memory.c device.c nmc93cX6.c cisco_eeprom.c cisco_card.c \
	pci_dev.c pci_io.c \
	dev_zero.c dev_bswap.c old_dev_vtty.c dev_ram.c dev_rom.c dev_nvram.c \
	dev_bootflash.c dev_flash.c dev_mpc860.c dev_ds1620.c \
	dev_remote.c dev_clpd6729.c dev_pcmcia_disk.c dev_gt.c dev_mv64460.c \
	dev_plx.c dev_dec21x50.c dev_pericom.c dev_ti2050b.c dev_ap1011.c \
	dev_plx6520cb.c dev_ns16552.c \
	dev_dec21140.c dev_am79c971.c dev_i8254x.c dev_i8255x.c \
	dev_mueslix.c dev_wic_serial.c \
	dev_c3600.c dev_c3600_bay.c dev_c3600_iofpga.c \
	dev_c3600_eth.c dev_c3600_serial.c \
	dev_c7200.c dev_c7200_iofpga.c dev_c7200_mpfpga.c \
	dev_c7200_sram.c dev_c7200_eth.c dev_c7200_serial.c dev_c7200_pos.c \
	dev_c7200_bri.c dev_c7200_jcpa.c \
	dev_c2691.c dev_c2691_iofpga.c dev_c2691_eth.c dev_c2691_serial.c \
	dev_c2691_wic.c dev_c2691_pcmod.c \
	dev_c3725.c dev_c3725_iofpga.c dev_c3725_eth.c dev_c3725_serial.c \
	dev_c3725_wic.c dev_c3725_pcmod.c \
	dev_c3745.c dev_c3745_iofpga.c dev_c3745_eth.c dev_c3745_serial.c \
	dev_c3745_wic.c dev_c3745_pcmod.c \
	dev_c2600.c dev_c2600_pci.c dev_c2600_iofpga.c \
	dev_c2600_eth.c dev_c2600_pcmod.c dev_c2600_wic.c \
	dev_c1700.c dev_c1700_iofpga.c dev_c1700_eth.c dev_c1700_wic.c \
	dev_c6msfc1.c dev_c6msfc1_iofpga.c dev_c6msfc1_mpfpga.c \
	dev_c6sup1.c dev_c6sup1_iofpga.c dev_c6sup1_mpfpga.c \
	dev_nm_16esw.c dev_pa_a1.c dev_pa_mc8te1.c \
	dev_sb1.c dev_sb1_io.c dev_sb1_pci.c hypervisor.c \
	hv_nio.c hv_nio_bridge.c \
	hv_frsw.c hv_atmsw.c hv_atm_bridge.c hv_ethsw.c \
	hv_vm.c hv_vm_debug.c hv_store.c \
	hv_c7200.c hv_c3600.c hv_c2691.c hv_c3725.c hv_c3745.c \
	hv_c2600.c hv_c1700.c \
	rommon_var.c get_cpu_time.c \
	fs_mbr.c fs_fat.c fs_nvram.c \
	tcb.c \
	inet_aton.c sendrecv.c mman.c fsync.c usleep.c localtime_r.c

# Profiling
#SOURCES += profiler.c
#CFLAGS += -p -DPROFILE -DPROFILE_FILE=\"$(PROG).profile\"

ifeq ($(DYNAMIPS_ARCH),x86)
HDR += x86-codegen.h mips64_x86_trans.h ppc32_x86_trans.h
SOURCES += mips64_x86_trans.c ppc32_x86_trans.c
#CFLAGS += -m32
endif

ifeq ($(DYNAMIPS_ARCH),amd64)
HDR += x86-codegen.h amd64-codegen.h mips64_amd64_trans.h ppc32_amd64_trans.h
SOURCES += mips64_amd64_trans.c ppc32_amd64_trans.c
CFLAGS += -m64
endif

ifeq ($(DYNAMIPS_ARCH),nojit)
HDR += mips64_nojit_trans.h ppc32_nojit_trans.h
SOURCES += mips64_nojit_trans.c ppc32_nojit_trans.c
endif

# RAW Ethernet support for Linux
ifeq ($(shell uname), Linux)
CFLAGS += -DLINUX_ETH
HDR += linux_eth.h
SOURCES += linux_eth.c
endif

# Generic Ethernet support with libpcap (0.9+)
ifeq ($(HAS_PCAP), 1)
CFLAGS += -DGEN_ETH
HDR += gen_eth.h
SOURCES += gen_eth.c

LIBS += $(PCAP_LIB)
endif

C_OBJS=$(SOURCES:.c=.o)
A_OBJS=$(ASMSRC:.S=.o)
LEX_C=$(LEX_SOURCES:.l=.c)

SUPPL=mips_mts.c Makefile ChangeLog COPYING README README.hypervisor TODO \
	dynamips.1 nvram_export.1 hypervisor_mode.7
#	mips64_microcode ppc32_microcode debian/

FILE_LIST := $(HDR) $(SOURCES) $(SUPPL) \
	x86-codegen.h amd64-codegen.h \
	mips64_x86_trans.c mips64_x86_trans.h \
	mips64_amd64_trans.c mips64_amd64_trans.h \
	mips64_nojit_trans.c mips64_nojit_trans.h \
	ppc32_x86_trans.c ppc32_x86_trans.h \
	ppc32_amd64_trans.c ppc32_amd64_trans.h \
	ppc32_nojit_trans.c ppc32_nojit_trans.h \
	linux_eth.c linux_eth.h gen_eth.c gen_eth.h \
	profiler.c profiler_resolve.pl bin2c.c rom2c.c \
	nvram_export.c udp_send.c udp_recv.c

.PHONY: all
# all: $(PROG) nvram_export$(BIN_EXT) udp_send$(BIN_EXT) udp_recv$(BIN_EXT)
all: $(PROG) udp_send$(BIN_EXT) udp_recv$(BIN_EXT) slirp_rdr$(BIN_EXT)

$(PROG): mips64_microcode_dump.inc ppc32_microcode_dump.inc \
	$(LEX_C) $(C_OBJS) $(A_OBJS)
	@echo "Linking $@"
	$(CC) $(CFLAGS) -o $@ $(C_OBJS) $(A_OBJS) -lelf -L../contrib/libelf-0.8.13/lib/ $(LIBS)

udp_send$(BIN_EXT): udp_send.o net.o crc.o utils.o inet_aton.o mman.o
	@echo "Linking $@"
	$(CC) -Wall $(CFLAGS) -o $@ udp_send.o net.o crc.o utils.o inet_aton.o mman.o localtime_r.o $(LIBS)

udp_recv$(BIN_EXT): udp_recv.o net.o crc.o utils.o inet_aton.o mman.o
	@echo "Linking $@"
	$(CC) -Wall $(CFLAGS) -o $@ udp_recv.o net.o crc.o utils.o inet_aton.o mman.o localtime_r.o $(LIBS)

slirp_rdr$(BIN_EXT): slirp_rdr.o net.o crc.o utils.o inet_aton.o mman.o
	@echo "Linking $@"
	$(CC) -Wall $(CFLAGS) -o $@ slirp_rdr.o net.o crc.o utils.o inet_aton.o mman.o localtime_r.o -L../contrib/slirp -lslirp -liphlpapi $(LIBS)

rom2c$(BIN_EXT): 
	@echo "Linking $@"
	gcc ..\common\rom2c.c -o rom2c.exe -I. -I..\common -L../contrib/libelf-0.8.13/lib/ -lelf

mips64_microcode_dump.inc: rom2c$(BIN_EXT) 
	./rom2c ..\microcode\mips64_test mips64_microcode_dump.inc 0xbfc00000


ppc32_microcode_dump.inc: rom2c$(BIN_EXT) 
	./rom2c ..\microcode\ppc32_test2 ppc32_microcode_dump.inc 0xfff00000

asmdefs$(BIN_EXT): asmdefs.c mips64.h
	@echo "Linking $@"
	@$(CC) -Wall $(CFLAGS) -o $@ asmdefs.c

asmdefs.h: asmdefs$(BIN_EXT)
	@echo "Building assembly definitions header file"
	@./asmdefs

nvram_export$(BIN_EXT): nvram_export.o fs_nvram.o dev_nvram.o vm.o device.o
	@echo "Linking $@"
	$(CC) -Wall $(CFLAGS) -o $@ nvram_export.o fs_nvram.o dev_nvram.o vm.o device.o $(LIBS)


install: $(PROG) nvram_export
	@echo "Installing"
	install -d $(DESTDIR)/bin $(DESTDIR)/man/man1 $(DESTDIR)/man/man7 $(DESTDIR)/etc
	install dynamips nvram_export   $(DESTDIR)/bin
	install -m644 dynamips.1        $(DESTDIR)/man/man1
	install -m644 nvram_export.1    $(DESTDIR)/man/man1
	install -m644 hypervisor_mode.7 $(DESTDIR)/man/man7
# install -m644 example         $(DESTDIR)/etc/dynamips


.PHONY: clean
clean:
	$(RM) -f rom2c$(BIN_EXT) microcode_dump.inc asmdefs$(BIN_EXT) \
	asmdefs.h $(C_OBJS) $(A_OBJS) $(PROG) nvram_export \
	mips64_microcode_dump.inc ppc32_microcode_dump.inc
	$(RM) -f *~

.PHONY: package
package:
	@mkdir -p distrib/$(PACKAGE)
	@$(CP) -r $(FILE_LIST) distrib/$(PACKAGE)
	@cd distrib ; $(TAR) czf $(ARCHIVE) $(PACKAGE)

.PHONY: packdev
packdev:
	@mkdir -p distrib/$(PACKAGE_DEV)
	@$(CP) -r $(FILE_LIST) distrib/$(PACKAGE_DEV)
	@cd distrib ; $(TAR) czf $(ARCHIVE_DEV) $(PACKAGE_DEV)

.SUFFIXES: .c .h .S .l .y .o

.S.o:
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $*.o $<
#	@echo "Assembling $<"
#	@$(CC) $(CFLAGS) $(INCLUDE) -c -o $*.o $<

.c.o:
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $*.o $<
#	@echo "Compiling $<"
#	@$(CC) $(CFLAGS) $(INCLUDE) -c -o $*.o $<

.l.c:
	$(LEX) -o$*.c $<
