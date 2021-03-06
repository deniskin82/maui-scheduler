# Makefile for Maui
# Copyright 1999 - 2010  Cluster Resources, Inc, All Rights Reserved
# Copyright 2016         Temple HPC Team

TOP =..
INC =$(TOP)/include
LIB =$(TOP)/lib
BIN =$(TOP)/bin

SCHEDIP=-I$(INC)
SHELL  =/bin/sh

SERVER   =maui
SCHEDLIB =maui

MBINS    =$(SERVER) mclient mprof
LIBS     =-l$(SCHEDLIB) $(REGEXLIB) $(SECLIB) -lm

LP   =-L$(LIB) $(SCHEDLP) $(REGEXLP) $(PBSLDFLAGS)
IP   =$(SCHEDIP) $(REGEXIP) $(PBSCFLAGS)
DEFS =-D$(OPSYS) $(PBSDEF) $(OSDEF) $(REGEXDEF) $(SECDEF) $(DAEMONDEF)

MCLIENTS =canceljob changeparam checkjob checknode diagnose releasehold \
          releaseres resetstats runjob schedctl sethold setqos setres setspri \
          showbf showconfig showgrid showhold showstart showq showtasks showres \
          showstate showstats mschedctl mjobctl mnodectl mdiag mstat mbal

LIBMAUI  =libmaui.a
LOBJECTS =MSU.o MSec.o MS3I.o MXML.o MConst.o \
          MFile.o MConfig.o MQOS.o MLog.o MCred.o MSched.o MAM.o MSR.o MRes.o \
          MUser.o MGroup.o MAcct.o MClass.o MTime.o MNode.o MConst.o MComConst.o MACL.o MStats.o \
          MLocal.o MUtil.o MTrace.o MAppSim.o MQueue.o MLimit.o MSim.o \
          MBF.o MJob.o MPriority.o MPolicy.o MPreempt.o MFS.o MPar.o MRM.o \
          MPBSI.o MWikiI.o MSys.o MCP.o MUI.o $(SSSOBJ)
SOBJECTS =Server.o UserI.o
POBJECTS =mprof.o

CP =cp
RM =rm -f
LD =$(CC)
CP =cp
LN =ln

CFLAGS = $(OSCCFLAGS)

default:
	@echo "Please run make only in the top-level directory"

.PHONY: all dep setup default \
	install install_lib install_server \
	clean clean_lib clean_server \
	uninstall uninstall_lib uninstall_server

all:    $(LIBMAUI) $(MBINS)

setup:  
	mkdir -p ./bin $(TOP)/include     
	(cd $(TOP);mkdir -p spool stats log traces tools)
	chmod 1777 $(TOP)/spool

install: install_lib install_server

install_lib: all
	mkdir -p $(INST_DIR)/bin $(INST_DIR)/sbin $(INST_DIR)/include $(INST_DIR)/lib
	$(CP) $(LIB)/$(LIBMAUI) $(INST_DIR)/lib
	$(CP) $(INC)/moab.h $(INST_DIR)/include/moab.h

install_server: all
	mkdir -p $(INST_DIR)/bin $(INST_DIR)/sbin $(INST_DIR)/include $(INST_DIR)/lib $(INST_DIR)/lib/systemd/system
	mkdir -p $(MSCHED_HOME)/log $(MSCHED_HOME)/traces $(MSCHED_HOME)/stats $(MSCHED_HOME)/spool $(MSCHED_HOME)/tools

	chmod 1777 $(MSCHED_HOME)/spool

	cp $(BIN)/mclient $(INST_DIR)/bin      
	cp $(BIN)/mprof $(INST_DIR)/bin   
	cp $(BIN)/maui $(INST_DIR)/sbin
	cp $(INC)/moab.h $(INST_DIR)/include
	cp $(LIB)/lib$(SCHEDLIB).a $(INST_DIR)/lib
	cp $(TOP)/maui-scheduler.service $(INST_DIR)/lib/systemd/system/

	(cd $(INST_DIR)/bin; \
	for i in $(MCLIENTS); do \
          if [ ! -f $$i ]; \
            then ln -f mclient $$i || true; \
          fi; \
	done)

	if [ ! -f $(MSCHED_HOME)/maui.cfg ]; \
	  then cp $(TOP)/maui.cfg.dist $(MSCHED_HOME)/maui.cfg; \
	fi

	if [ ! -f $(MSCHED_HOME)/maui-private.cfg ]; \
          then cp $(TOP)/maui-private.cfg.dist $(MSCHED_HOME)/maui-private.cfg; \
        fi

#	(cd ../..;ln -sf etc/maui.cfg;ln -sf etc/maui-private.cfg)


.c.o:
	$(CC) $(IP) $(DEFS) $(CFLAGS) $(SOURCE) -c $<

dep .depend:
	$(TOP)/tools/fastdep.exe $(IP) -- *.c > .depend

sinclude .depend

$(LIBMAUI): $(LOBJECTS)
	ar rcs $(LIB)/$(LIBMAUI) $(LOBJECTS)

$(SERVER): $(SOBJECTS) $(LIBMAUI)
	$(LD) $(SOBJECTS) -o $(BIN)/$@ $(LIBS) $(OSLDFLAGS) $(LP)
	chmod 711 $(BIN)/$@ || true

mclient: mclient.o $(LIBMAUI)
	$(LD) mclient.o -o $(BIN)/$@ $(OSLDFLAGS) $(LIBS)    $(LP)
	chmod 751 $(BIN)/$@
	for i in $(MCLIENTS); do \
	  ln -f $(BIN)/$@ $(BIN)/$$i; \
	  chmod 751 $(BIN)/$$i; \
	done

mprof:  $(POBJECTS) $(LIBMAUI)
	$(LD) $(POBJECTS) -o $(BIN)/$@ $(OSLDFLAGS) $(LIBS)     $(LP)
	chmod 751 $(BIN)/$@

#------------------------------------------------------------------------------
# cleanup
#------------------------------------------------------------------------------

clean: clean_lib clean_server

uninstall: uninstall_lib uninstall_server

clean_lib:
	$(RM) *.o $(LIB)/$(LIBMAUI)

clean_server:
	$(RM) *.o $(BIN)/maui $(BIN)/mclient $(BIN)/mprof
	cd $(BIN) && rm -f $(MCLIENTS)

uninstall_lib:
	$(RM) $(INST_DIR)/lib/$(LIBMAUI)
	$(RM) $(INST_DIR)/include/moab.h

uninstall_server:
	$(RM) $(INST_DIR)/bin/mclient
	$(RM) $(INST_DIR)/bin/mprof
	$(RM) $(INST_DIR)/sbin/maui



