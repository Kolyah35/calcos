PLATFORMS := avr

.PHONY: $(PLATFORMS) clean qemu

.DEFAULT_GOAL := help

help:
	@echo "Available platforms:"
	@for p in $(PLATFORMS); do echo "  $$p"; done
	@echo "make [platform]"

$(PLATFORMS):
	$(MAKE) -f src/arch/$@/Makefile $(MAKECMDGOALS)

clean:
	rm -rf build/*
