define BINRULES
$(OBJRULES)

$(T)_TARGET ?= $(T)
$(T)_TGTDIR ?= $$(BINDIR)
$(T)_BUILDWITH ?= all
$(T)_STRIPWITH ?= strip

$(BUILDDEPS)
$(LINKFLAGS)

$(T)_EXE := $$($(T)_TGTDIR)$$(PSEP)$$($(T)_TARGET)$$(EXE)

$(T): $$($(T)_EXE)

.PHONY: $(T)

OUTFILES := $$($(T)_EXE)
$(DIRRULES)

ifneq ($$(strip $$($(T)_BUILDWITH)),)
$$($(T)_BUILDWITH):: $$($(T)_EXE)

endif

ifneq ($$(strip $$($(T)_STRIPWITH)),)
$$($(T)_STRIPWITH):: $$($(T)_EXE)
	$$(VSTRP)
	$$(VR)$$(CROSS_COMPILE)$$(STRIP) --strip-all $$<

endif

$$($(T)_EXE): $$($(T)_OBJS) $$(_$(T)_DEPS) | $$(_$(T)_DIRS)
	$$(VCCLD)
	$$(VR)$$(CROSS_COMPILE)$$(CC) -o$$@ \
		$$($(T)_$$(PLATFORM)_CFLAGS) $$($(T)_CFLAGS) $$(CFLAGS) \
		$$($(T)_$$(PLATFORM)_LDFLAGS) $$($(T)_LDFLAGS) $$(LDFLAGS) \
		$$($(T)_OBJS) $$(_$(T)_LINK)

endef

# vim: noet:si:ts=8:sts=8:sw=8
