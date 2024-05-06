#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <ingle/cc65-charmap.h>
#include <term/libterm.h>
#include <lineedit/liblineedit.h>
#include <text/line.h>

#include "screen.h"


char columns = 40;
char rows = 24;

unsigned char ypos = 0;
int changes_first;
int changes_last;

unsigned old_ystart = 0;
unsigned ystart;
char * status = "";


void
gotoxy (char x, char y)
{
    term_put (TERM_SET_CURSOR);
    term_put (x + 1);
    term_put (y + 1);
}

void
set_cursor (void)
{
    gotoxy (xpos, ypos);
}

void
disable_cursor ()
{
    term_puts ("\x1b[?25l");
}

void
enable_cursor ()
{
    term_puts ("\x1b[?25h");
}

void
print_linebuf ()
{
    char *  data = (char *) &linebuf;
    int     len = linebuf_length;

    if (len > columns)
        len = columns;

    while (len--)
        term_put (*data++);
}

// Called by liblineedit.
void
linebuf_redraw ()
{
    disable_cursor ();
    gotoxy (0, ypos);

    print_linebuf ();
    if (linebuf_length < columns)
        term_put (TERM_CLEAR_TO_EOL);

    set_cursor ();
    enable_cursor ();
}

void
line_redraw (line * l)
{
    char *  data = l->data;
    char    len  = l->length;

    if (len > columns)
        len = columns;

    while (len--)
        term_put (*data++);
}

void
print_status ()
{
    disable_cursor ();
    gotoxy (0, rows - 1);
    term_puts (status);
    term_put (TERM_CLEAR_TO_EOL);
    set_cursor ();
    enable_cursor ();
}

char txt_memleft[16];

void
screen_set_status (char * msg)
{
    status = msg;
    print_status ();
    gotoxy (0, rows - 2);
    term_puts ("\x1b[6m");
    gotoxy (35, rows - 2);
    sprintf (txt_memleft, "%D", _heapmemavail ());
    term_puts (txt_memleft);
    term_puts ("\x1b[27m");
}

void
update_screen_offset ()
{
    if (ystart > linenr)
        ystart = linenr;
    else if ((ystart + rows - 3) < linenr)
        ystart = linenr - rows + 3;
}

void
screen_redraw ()
{
    line *   l;
    unsigned y;
    unsigned n;

    update_screen_offset ();
    disable_cursor ();
    l = line_get (ystart);

    for (y = 0; y < rows - 2; y++) {
        if (changes_first > -1) { 
            n = ystart + y;
            if (n < changes_first || n > changes_last)
                goto next;
        }

        gotoxy (0, y);
        if (l) {
            line_redraw (l);
            if (l->length < columns)
                term_put (TERM_CLEAR_TO_EOL);
        } else {
            term_put ('~');
            term_put (TERM_CLEAR_TO_EOL);
        }

next:
        if (l)
            l = l->next;
    }

    print_status ();

    ypos = linenr - ystart;
    set_cursor ();
    enable_cursor ();

    changes_first = changes_last = -1;
}

void
screen_update ()
{
    update_screen_offset ();

    if (old_ystart != ystart || changes_first >= 0) {
        changes_first = changes_last = -1;
        screen_redraw ();
    }
    old_ystart = ystart;

    ypos = linenr - ystart;
    set_cursor ();
}

void
screen_init ()
{
    ystart = 0;
    changes_first = changes_last = -1;
}
