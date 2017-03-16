PP:= src$(PSEP)tests

T:= test_1
test_1_SRCDIR:= $(PP)
test_1_MODULES:= test_1
test_1_LIBS:= adbhost
test_1_DEPS:= adbhost
test_1_BUILDWITH:= tests
test_1_STRIPWITH:= striptests
$(eval $(BINRULES))

.PHONY: striptests
