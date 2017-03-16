define LIBRULES
$(OBJRULES)

$(T)_TARGET ?= $(T)
$(T)_V_MAJ ?= 1
$(T)_V_MIN ?= 0
$(T)_V_REV ?= 0
$(T)_TGTDIR ?= $$(LIBDIR)
$(T)_BINDIR ?= $$(BINDIR)
$(T)_CFLAGS_SHARED ?= $$($(T)_CFLAGS) -fPIC
$(T)_BUILDWITH ?= all
$(T)_BUILDSTATICWITH ?= staticlibs
$(T)_STRIPWITH ?= strip

$(T)_STATICLIB := $$($(T)_TGTDIR)$$(PSEP)lib$(T).a

$(BUILDDEPS)
$(LINKFLAGS)

ifeq ($$(PLATFORM),win32)
$(T)_LIB := $$($(T)_BINDIR)$$(PSEP)$(T)-$$($(T)_V_MAJ).dll

else
_$(T)_V := $$($(T)_V_MAJ).$$($(T)_V_MIN).$$($(T)_V_REV)
_$(T)_LIB_FULL := $$($(T)_TGTDIR)$$(PSEP)lib$(T).so.$$(_$(T)_V)
_$(T)_LIB_MAJ := $$($(T)_TGTDIR)$$(PSEP)lib$(T).so.$$($(T)_V_MAJ)
$(T)_LIB := $$($(T)_TGTDIR)$$(PSEP)lib$(T).so

endif

OUTFILES := $$($(T)_LIB) $$($(T)_STATICLIB)
$(DIRRULES)

ifeq ($$(PLATFORM),win32)
$$($(T)_STATICLIB): $$($(T)_LIB)

$$($(T)_LIB): $$($(T)_SOBJS) $$(_$(T)_DEPS) | $$(_$(T)_DIRS)
	$$(VCCLD)
	$$(VR)$$(CROSS_COMPILE)$$(CC) -shared -o$$@ \
		-Wl,--out-implib,$$($(T)_TGTDIR)$$(PSEP)lib$(T).a \
		-Wl,--output-def,$$($(T)_TGTDIR)$$(PSEP)$(T).def \
		$$($(T)_$$(PLATFORM)_CFLAGS) $$($(T)_CFLAGS) $$(CFLAGS) \
		$$($(T)_$$(PLATFORM)_LDFLAGS) $$($(T)_LDFLAGS) $$(LDFLAGS) \
		$$($(T)_SOBJS) $$(_$(T)_LINK)

else
$$($(T)_STATICLIB): $$($(T)_OBJS) | $$(_$(T)_DIRS)
	$$(VAR)
	$$(VR)$$(CROSS_COMPILE)$$(AR) rcs $$@1 $$^
	$$(VR)$$(RMF) $$@
	$$(VR)$$(MV) $$@1 $$@

$$($(T)_LIB): $$(_$(T)_LIB_MAJ)
	$$(VR)ln -fs lib$(T).so.$$($(T)_V_MAJ) $$@

$$(_$(T)_LIB_MAJ): $$(_$(T)_LIB_FULL)
	$$(VR)ln -fs lib$(T).so.$$(_$(T)_V) $$@

$$(_$(T)_LIB_FULL): $$($(T)_SOBJS) $$(_$(T)_DEPS) | $$(_$(T)_DIRS)
	$$(VCCLD)
	$$(VR)$$(CROSS_COMPILE)$$(CC) -shared -o$$@ \
		-Wl,-soname,lib$(T).so.$$($(T)_V_MAJ) \
		$$($(T)_$$(PLATFORM)_CFLAGS) $$($(T)_CFLAGS) $$(CFLAGS) \
		$$($(T)_$$(PLATFORM)_LDFLAGS) $$($(T)_LDFLAGS) $$(LDFLAGS) \
		$$($(T)_SOBJS) $$(_$(T)_LINK)

endif

$(T): $$($(T)_LIB)

static_$(T): $$($(T)_STATICLIB)

.PHONY: $(T) static_$(T)

ifneq ($$(strip $$($(T)_BUILDWITH)),)
$$($(T)_BUILDWITH):: $$($(T)_LIB)

endif

ifneq ($$(strip $$($(T)_BUILDSTATICWITH)),)
$$($(T)_BUILDSTATICWITH):: $$($(T)_STATICLIB)

endif

ifneq ($$(strip $$($(T)_STRIPWITH)),)
$$($(T)_STRIPWITH):: $$($(T)_LIB)
	$$(VSTRP)
	$$(VR)$$(CROSS_COMPILE)$$(STRIP) --strip-unneeded $$<

ifneq ($$(strip $$($(T)_BUILDSTATICWITH)),)
$$($(T)_STRIPWITH):: $$($(T)_STATICLIB)
	$$(VSTRP)
	$$(VR)$$(CROSS_COMPILE)$$(STRIP) --strip-unneeded $$<


endif

endif

endef

# vim: noet:si:ts=8:sts=8:sw=8
