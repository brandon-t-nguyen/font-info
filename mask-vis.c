#ifdef VISUALIZE

#include <time.h>

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

static int y,x;

void renderMask( B_Mask mask, int row, int col, int minRow, int minCol, int maxRow, int maxCol )
{
    int height = B_Mask_getHeight(mask);
    int width = B_Mask_getWidth(mask);
    int baseR = row-height/2;
    int baseC = col-width/2;
    int * matrix = B_Mask_getMatrix(mask);
    for( int r = 0, setR = baseR; r < height; r++, setR++ )
    {
        if( setR < minRow || setR > maxRow )
            continue;
        for( int c=0,setC=baseC; c < width; c++,setC++ )
        {
            if( setC < minCol || setC > maxCol )
                continue;
            move(setR,setC);
            int value = matrix[c + r*width];
            if( value > 10 )
            {
                value = 9;
            }

            if( value < 0 )
            {
                //value = -value;
                //addch( (value+'0') | A_UNDERLINE );
                addch( '-'  );
            }
            else
            {
                addch( (value+'0') | A_REVERSE );
            }
        }
    }
}

void render( B_Image image, int row, int col )
{
    move(row,col);
    for( int r = 0; r < B_Image_getHeight( image ); r++ )
    {
        for( int c = 0; c < B_Image_getWidth( image ); c++ )
        {
            move(row+r,col+c);
            addch(B_Image_grayToAscii(B_Image_getPixel(image, c, r)));
        }
    }
}

void compare( B_Image image, B_Mask mask, int row, int col )
{
    struct timespec req = {0,50000000};
    //struct timespec req = {0,5000000};
    //struct timespec req = {0,10000000};
    //struct timespec req = {0,500000};
    struct timespec rem;

    int imageR, imageC;
    int baseR, baseC;

    move(row,col);

    printw("Before mask correlation:");
    move(row+1,col);
    getyx(stdscr,imageR,imageC);
    render(image,imageR,imageC);

    getyx(stdscr,baseR,baseC);
    baseR = imageR;
    baseC += 20;
    move(row, baseC);
    printw("After mask correlation:");

    int height = B_Image_getHeight( image );
    int width = B_Image_getWidth( image );
    for( int r = 0; r < height; r++ )
    {
        for( int c = 0; c < width; c++ )
        {
            move(row,col);
            printw("Before mask correlation:");
            render(image,imageR,imageC);
            renderMask(mask, imageR+r, imageC+c, imageR, imageC, imageR+height-1, col+width-1 );

            move(baseR-1,baseC);
            printw("After mask correlation:");

            int pixel = B_Mask_correlatePixel( mask, image, r, c);
            if( pixel > 255 )
                pixel = 255;
            else if( pixel < 0 )
                pixel = 0;

            move(baseR+r,baseC+c);
            addch(B_Image_grayToAscii(pixel));
            nanosleep(&req,&rem);
            refresh();
            if( req.tv_nsec > 5000000)
                req.tv_nsec -= 100000;
        }
    }
    addch('\n');
}

void visualize( BT_Face face, int charcode )
{
    B_Mask curve = B_Mask_new( curve_tl, 1, 5, 5 );
    B_Mask edge = B_Mask_new( edge, 1, 3, 3 );

    initscr();
    printw("Hello World !!!");

    const B_Image character = BT_Face_getChar( face, charcode );

/*
    B_Image outline = B_Mask_convolve( edge, character );
    compare( outline, curve, 0, 0 );
    B_Image_delete(outline);
*/
    compare( character, curve, 0, 0 );
    getch();
    endwin();

    B_Mask_delete( curve );
    B_Mask_delete( edge );
}

#endif
