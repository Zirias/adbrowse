_DIRECTORIES :=

define DIRRULES

_$(T)_DIRS := $$(sort $$(dir $$(OUTFILES)))

_NEWDIRS := $$(foreach _dir,$$(_$(T)_DIRS), \
	$$(if $$(findstring $$(_dir),$$(_DIRECTORIES)),,$$(_dir)))

_DIRECTORIES += $$(_NEWDIRS)

$$(_NEWDIRS):
	$$(VMD)
	$$(VR)$$(MDP) $$@

endef

# vim: noet:si:ts=8:sts=8:sw=8
