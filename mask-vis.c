#ifdef VISUALIZE

#include <time.h>

#include <ncurses.h>
#include "btypeface.h"
#include "bimage.h"
#include "bmask.h"


static int curve_tl[] = {
                           2,  2,  1,  2,  2,
                           2, -9,  1, -9,  2,
                          -9, -9,  1, -9, -9,
                           2, -9,  1, -9,  2,
                           2,  2,  1,  2,  2,
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

void compare2( B_Image image1, B_Image image2, B_Mask mask, int row, int col )
{
    int height1 = B_Image_getHeight( image1 );
    int width1 = B_Image_getWidth( image1 );
    int height2 = B_Image_getHeight( image2 );
    int width2 = B_Image_getWidth( image2 );
    int height = height1>height2?height1:height2;
    int width = width1>width2?width1:width2;

    struct timespec req = {0,50000000};
    //struct timespec req = {0,5000000};
    //struct timespec req = {0,10000000};
    //struct timespec req = {0,500000};
    struct timespec rem;

    int image1R, image1C;
    int image2R, image2C;
    int base1R, base1C;
    int base2R, base2C;



    image1R = row+1;
    image2R = image1R + height1 + 5;
    image1C = col;
    image2C = col;

    base1R = image1R;
    base2R = image2R;
    base1C= col + width + 10;
    base2C = base1C;

    move(row,image1C);
    printw("Before mask correlation:");

    move(row,base1C);
    printw("After mask correlation:");

    for( int r = 0; r < height; r++ )
    {
        for( int c = 0; c < width; c++ )
        {
            if( c < width1 && r < height1 )
            {
                render(image1,image1R,image1C);
                renderMask(mask, image1R+r, image1C+c, image1R, image1C, image1R+height1-1, col+width1-1 );

                int pixel = B_Mask_correlatePixel( mask, image1, r, c);
                if( pixel > 255 )
                    pixel = 255;
                else if( pixel < 0 )
                    pixel = 0;

                move(base1R+r,base1C+c);
                addch(B_Image_grayToAscii(pixel));
            }

            if( c < width2 && r < height2 )
            {
                render(image2,image2R,image2C);
                renderMask(mask, image2R+r, image2C+c, image2R, image2C, image2R+height2-1, col+width2-1 );

                int pixel = B_Mask_correlatePixel( mask, image2, r, c);
                if( pixel > 255 )
                    pixel = 255;
                else if( pixel < 0 )
                    pixel = 0;

                move(base2R+r,base2C+c);
                addch(B_Image_grayToAscii(pixel));
            }

            //nanosleep(&req,&rem);
            refresh();
            if( req.tv_nsec > 5000000)
                req.tv_nsec -= 100000;
        }
    }
    addch('\n');
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

void visualize( BT_Face face1, BT_Face face2, int charcode )
{
    B_Mask curve = B_Mask_new( curve_tl, 1, 5, 5 );

    initscr();
    printw("Hello World !!!");
    const B_Image c1 = BT_Face_getCharEdge( face1, charcode );
    const B_Image c2  = BT_Face_getCharEdge( face2, charcode );

    for( int i = 0; i < 4; i++ )
    {
        compare2( c1, c2, curve, 0, 0 );
        B_Mask curveTemp = B_Mask_rotate( curve );
        B_Mask_delete( curve );
        curve = curveTemp;
        getch();
    }

    B_Mask_delete( curve );
    endwin();
}

#endif
