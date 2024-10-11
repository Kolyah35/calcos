ifeq ($(OS),Windows_NT)
    SYSTEM += WIN32
    ifeq ($(PROCESSOR_ARCHITEW6432),AMD64)
        SYSTEM_ARCH += AMD64
    else
        ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
            SYSTEM_ARCH += AMD64
        endif
        ifeq ($(PROCESSOR_ARCHITECTURE),x86)
            SYSTEM_ARCH += IA32
        endif
    endif
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        SYSTEM += LINUX
    endif
    ifeq ($(UNAME_S),Darwin)
        SYSTEM += OSX
    endif
    UNAME_P := $(shell uname -p)
    ifeq ($(UNAME_P),x86_64)
        SYSTEM_ARCH += AMD64
    endif
    ifneq ($(filter %86,$(UNAME_P)),)
        SYSTEM_ARCH += IA32
    endif
    ifneq ($(filter arm%,$(UNAME_P)),)
        SYSTEM_ARCH += ARM
    endif
endif
