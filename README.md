# CalcOS

CalcOS is a minimalist OS designed for microcontrollers and small systems, focused on a minimal architecture. The system is not built around dynamically loadable modules or plugin-style extensibility. Components are ststaically integrated and composed at compile time. The project explores what a modern embedded OS should look like if there is only essential things: tasks, drivers (SPI, UART, I2C, I2S, LCD), UI and applications.

## The idea
The idea for CalcOS started with a basic pocket calculator, that could only perform `+`, `-`, `*`, `/`, `sqrt()` and nothing more. Looking at it, I wondered "What if the hardware inside were upgraded while the external form remained the same? What if device kept its physical sipmlicity, but internally became capable of far more?". 

## TODO
* [ ] UART, SPI, I2C, Timer, GPIO, LCD abstraction for **most** MCUs.
* [ ] Support for both 1-bit and 16-bit displays (and other).
* [ ] Custom hardware configuration (matrix keyboard, display pins, sdcard pins configuration)
* [ ] Power management
* [ ] Scheduler / threading
* [ ] SD Card filesystem
* [ ] Store system settings in EEPROM
* [ ] TP4056 support (STDBY and CHRG pins) 
* [ ] Immediate UI framework
    * [ ] Calculator keypad navigation
    * [ ] Dirty region render optimization
* [ ] Static app registration via linker sections
* [ ] Static system applications.
    * [ ] Home launcher
    * [ ] Scientific calculator
    * [ ] Tasks, Notes, Calendar
    * [ ] File manager
    * [ ] Settings
    * [ ] Stopwatch
    * [ ] 2FA authentificator
    * [ ] UART monitor
    * [ ] avrdudess port
* [ ] AVR support.
    * [ ] Custom bootloader to take control over WDT for panic.
* [ ] ESP32(S2/S3/C3) support.
    * [ ] Memory layout.
    * [ ] PSRAM support.
    * [ ] Wi-Fi support.
        * [ ] Static Espressif lib link
            * [ ] FreeRTOS fake functions definition 
    * [ ] J2ME runtime for external applications.
        * [ ] CDLC 1.0
        * [ ] MIDP 2.0
        * [ ] JSR-75 (FileConnection)
        * [ ] JSR-82 (Bluetooth)
        * [ ] JSR-135 (Mobile Media)
        * [ ] JSR-184 (Mobile 3D Graphics)
        * [ ] JSR-211 (Content Handler)
* [ ] Panic screen implementation
* [ ] Heap usage statistics
* [ ] Desktop companion app
    * [ ] Firmware compiling/flashing
    * [ ] Screencast
    * [ ] Input
    * [ ] Files/RMS manipulation
* [ ] Porting guide