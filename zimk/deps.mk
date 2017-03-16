define BUILDDEPS

_$(T)_DEPS :=
_$(T)_DEPS += $$(foreach dep,$$($(T)_DEPS),$$($$(dep)_LIB))
_$(T)_DEPS += $$(foreach dep,$$($(T)_STATICDEPS),$$($$(dep)_STATICLIB))

endef

# vim: noet:si:ts=8:sts=8:sw=8
