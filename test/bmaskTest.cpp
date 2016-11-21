#include <gtest/gtest.h>

#include <bimage.h>
#include <bmask.h>
static const int matrix[] = {
                                      1,  2,  3,
                                      1,  2,  3,
                                      1,  2,  3,
                            };
static const uint8_t buffer[] = {
                                        100,  50, 100,
                                         50, 100,  50,
                                        100,  50, 100,
                                };
class B_Mask_Test : public :: testing :: Test
{
protected:
    B_Image image;
    B_Mask  mask;

    virtual void SetUp()
    {
        mask = B_Mask_new( matrix, 1, 3, 3);

        image = B_Image_new( 3,3 );
        B_Image_drawBitmap( image, buffer, 3, 3, 0, 0);
    }

    virtual void TearDown()
    {
        B_Mask_delete( mask );
        B_Image_delete( image );
    }
};

TEST_F(B_Mask_Test,convolvePixel)
{
    int result = B_Mask_convolvePixel( mask, image, 1, 1 );
    ASSERT_EQ( 77, result );
}

TEST_F(B_Mask_Test,correlatePixel)
{
    int result = B_Mask_correlatePixel( mask, image, 1, 1 );
    ASSERT_EQ( 77, result );
}

TEST_F(B_Mask_Test,rotate)
{
    B_Mask rotate = B_Mask_rotate( mask );
    const int * mat = B_Mask_getMatrix( rotate );

    ASSERT_EQ( 1, mat[0] );
    ASSERT_EQ( 1, mat[1] );
    ASSERT_EQ( 1, mat[2] );

    ASSERT_EQ( 2, mat[3] );
    ASSERT_EQ( 2, mat[4] );
    ASSERT_EQ( 2, mat[5] );

    ASSERT_EQ( 3, mat[6] );
    ASSERT_EQ( 3, mat[7] );
    ASSERT_EQ( 3, mat[8] );

    B_Mask_delete( rotate );
}

TEST_F(B_Mask_Test,transpose)
{
    B_Mask transpose = B_Mask_transpose( mask );
    const int * mat = B_Mask_getMatrix( transpose );

    ASSERT_EQ( 1, mat[0] );
    ASSERT_EQ( 1, mat[1] );
    ASSERT_EQ( 1, mat[2] );

    ASSERT_EQ( 2, mat[3] );
    ASSERT_EQ( 2, mat[4] );
    ASSERT_EQ( 2, mat[5] );

    ASSERT_EQ( 3, mat[6] );
    ASSERT_EQ( 3, mat[7] );
    ASSERT_EQ( 3, mat[8] );

    B_Mask_delete( transpose );
}

TEST_F(B_Mask_Test,flipHor)
{
    B_Mask flipHor = B_Mask_flipHor( mask );
    const int * mat = B_Mask_getMatrix( flipHor );

    ASSERT_EQ( 3, mat[0] );
    ASSERT_EQ( 2, mat[1] );
    ASSERT_EQ( 1, mat[2] );

    ASSERT_EQ( 3, mat[3] );
    ASSERT_EQ( 2, mat[4] );
    ASSERT_EQ( 1, mat[5] );

    ASSERT_EQ( 3, mat[6] );
    ASSERT_EQ( 2, mat[7] );
    ASSERT_EQ( 1, mat[8] );

    B_Mask_delete( flipHor );
}
