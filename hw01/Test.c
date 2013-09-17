#include <stdio.h>

#define FLAGS_ECHO 0x00000001
#define IS_FLAGS_ECHO(a) (a & FLAGS_ECHO)
#define SET_FLAGS_ECHO(a) (a |= FLAGS_ECHO)
#define CLR_FLAGS_ECHO(a) (a &= ~FLAGS_ECHO)


