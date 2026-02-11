PLATFORMS := esp32s3

.PHONY: $(PLATFORMS) clean qemu

.DEFAULT_GOAL := help

help:
	@echo "Available platforms:"
	@for p in $(PLATFORMS); do echo "  $$p"; done

$(PLATFORMS):
	$(MAKE) -f arch/$@/Makefile $(MAKECMDGOALS)

clean:
	rm -rf ../build/*
