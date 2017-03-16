define LINKFLAGS

ifneq ($$(strip $$($(T)_STATICLIBS)),)
_$(T)_LINK+=-Wl,-Bstatic $$(addprefix -l,$$($(T)_STATICLIBS)) -Wl,-Bdynamic
endif

ifneq ($$(strip $$($(T)_LIBS)),)
_$(T)_LINK+=$$(addprefix -l,$$($(T)_LIBS))
endif

ifneq ($$(strip $$($(T)_$$(PLATFORM)_LIBS)),)
_$(T)_LINK+=$$(addprefix -l,$$($(T)_$$(PLATFORM)_LIBS))
endif

endef
