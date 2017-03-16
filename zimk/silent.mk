V?= 0

ifeq ($(V),1)
VCC:=
VAS:=
VDEP:=
VLD:=
VAR:=
VRES:=
VCCLD:=
VSTRP:=
VMD:=
VGEN:=
VGENT:=
VR:=
else
VCC=	@echo "   [CC]   $@"
VAS=	@echo "   [AS]   $@"
VDEP=	@echo "   [DEP]  $@"
VLD=	@echo "   [LD]   $@"
VAR=	@echo "   [AR]   $@"
VRES=   @echo "   [RES]  $@"
VCCLD=	@echo "   [CCLD] $@"
VSTRP=  @echo "   [STRP] $<"
VMD=	@echo "   [MD]   $@"
VGEN=	@echo "   [GEN]  $@"
VGENT=	@echo "   [GEN]  $@: $(VTAGS)"
VR:=	@
endif

# vim: noet:si:ts=8:sts=8:sw=8
