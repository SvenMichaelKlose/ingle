#include <string.h>

#include <lib/ingle/ingle.h>

#include "desktop.h"

void
save_desktop_state ()
{
    memcpy ((void *) 0x120, (void *) 0x9ff0, 16);
    *(unsigned *) 0x128 = DESKTOP_BANK;
    save_state ((unsigned) desktop_loop, INGLE_FULL_STATE_COPY);
}
