#ifndef _ARCH_H_
#define _ARCH_H_

#if defined(__AVR_ARCH__)
#   if __AVR_ARCH__ == 2
#       define __ARCH__ "avr2"
#   elif __AVR_ARCH__ == 25
#       define __ARCH__ "avr25"
#   elif __AVR_ARCH__ == 3
#       define __ARCH__ "avr3"
#   elif __AVR_ARCH__ == 31
#       define __ARCH__ "avr31"
#   elif __AVR_ARCH__ == 35
#       define __ARCH__ "avr35"
#   elif __AVR_ARCH__ == 4
#       define __ARCH__ "avr4"
#   elif __AVR_ARCH__ == 5
#       define __ARCH__ "avr5"
#   elif __AVR_ARCH__ == 51
#       define __ARCH__ "avr51"
#   elif __AVR_ARCH__ == 6
#       define __ARCH__ "avr6"
#   elif __AVR_ARCH__ == 100
#       define __ARCH__ "avrtiny"
#   elif __AVR_ARCH__ == 102
#       define __ARCH__ "avrxmega2"
#   elif __AVR_ARCH__ == 103
#       define __ARCH__ "avrxmega3"
#   elif __AVR_ARCH__ == 104
#       define __ARCH__ "avrxmega4"
#   elif __AVR_ARCH__ == 105
#       define __ARCH__ "avrxmega5"
#   elif __AVR_ARCH__ == 106
#       define __ARCH__ "avrxmega6"
#   elif __AVR_ARCH__ == 107
#       define __ARCH__ "avrxmega7"
#   else
#       define __ARCH__ "avr"
#   endif

#   include <avr/version.h>
#   define __RUNTIME_INFO__ "AVR-LibC "__AVR_LIBC_VERSION_STRING__
#else
#   define __ARCH__ "unknown"
#endif

#endif /* _ARCH_H_ */