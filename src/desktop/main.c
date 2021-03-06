#include "cc65-charmap.h"

#include <cbm.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

#include <ultimem.h>
#include <libgfx.h>
#include <ingle.h>

#include "obj.h"
#include "error.h"
#include "message.h"
#include "obj.h"
#include "box.h"
#include "window.h"

#include "desktop.h"
#include "ultifs.h"
#include "wrap-ultifs.h"

void
init_memory ()
{
    * (char *) 0x9ff2 = 0xff;
    *ULTIMEM_BLK5 = *ULTIMEM_BLK3 + 1;
    _heapadd ((void *) 0xa000, 0x2000); /* BANK5 */
    _heapadd ((void *) 0x400, 0xc00);   /* +3K */
}

void
shift_charset ()
{
    short old_bank = *ULTIMEM_BLK5;
    int i;
    char * charset = (char *) 0xa000;

    *ULTIMEM_BLK5 = FONT_BANK;
    for (i = 0; i < 2048; i++)
        charset[i] <<= 4;
    *ULTIMEM_BLK5 = old_bank;
}

void
init_console ()
{
    shift_charset ();
    gfx_clear_screen (0);
    gfx_init ();
    gfx_set_font (charset_4x8, 2, FONT_BANK);
}

int
main (int argc, char ** argv)
{
    init_memory ();
    init_console ();
    w_ultifs_mount ();
    start_desktop ();

    return 0;
}
