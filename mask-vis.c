#ifdef VISUALIZE

#include <ncurses.h>
#include "btypeface.h"
#include "bimage.h"
#include "bmask.h"


static int edge_mat[] = {
                             -1, -1, -1,
                             -1,  8, -1,
                             -1, -1, -1,
                        };

static int curve_tl[] = {
                          -9, -9, -9,  1,  1,
                          -9, -9,  3,  7,  7,
                          -9,  3,  7,  3, -9,
                           1,  7,  3, -9, -9,
                           1,  7, -9, -9, -9,
                        };

void visualize( BT_Face face, int charcode )
{
    initscr();
    printw("Hello World !!!");
    getch();
    endwin();
}

#endif
