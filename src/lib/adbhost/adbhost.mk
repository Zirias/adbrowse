PPP:= src$(PSEP)lib$(PSEP)adbhost

T:= adbhost
adbhost_SRCDIR:= $(PPP)
adbhost_MODULES:= adbshellcommand bufreader
adbhost_PLATFORMMODULES:= adbconnect
adbhost_DEFINES:= -DBUILDING_ADBHOST
adbhost_DEPS:= abrutil
adbhost_LIBS:= abrutil
adbhost_V_MAJ:= 0
adbhost_V_MIN:= 0
adbhost_V_REV:= 1
$(eval $(LIBRULES))

