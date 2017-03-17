PPP:= src$(PSEP)lib$(PSEP)abrutil

T:= abrutil
abrutil_SRCDIR:= $(PPP)
abrutil_MODULES:= string stringbuilder list
abrutil_DEFINES:= -DBUILDING_ABRUTIL
abrutil_V_MAJ:= 0
abrutil_V_MIN:= 0
abrutil_V_REV:= 1
$(eval $(LIBRULES))
