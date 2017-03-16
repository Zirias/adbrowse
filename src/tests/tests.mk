PP:= src$(PSEP)tests

T:= test_1
test_1_SRCDIR:= $(PP)
test_1_MODULES:= test_1
test_1_STATICLIBS:= adbhost
test_1_STATICDEPS:= adbhost
test_1_BUILDWITH:= tests
test_1_STRIPWITH:=
$(eval $(BINRULES))

