#include <gtest/gtest.h>

#include <bimage.h>

class B_Image_Test : public :: testing :: Test
{
protected:
    B_Image image;

    virtual void SetUp()
    {
        image = B_Image_new( 3,5 );
    }

    virtual void TearDown()
    {
        B_Image_delete( image );
    }
};

TEST_F(B_Image_Test,getsetPixel)
{
    uint8_t topLeft, topRight, botLeft, botRight;
    B_Image_setPixel( image, 0, 0, 'A' );
    B_Image_setPixel( image, 2, 0, 'B' );
    B_Image_setPixel( image, 0, 4, 'C' );
    B_Image_setPixel( image, 2, 4, 'D' );
    topLeft  = B_Image_getPixel( image, 0, 0 );
    topRight = B_Image_getPixel( image, 2, 0 );
    botLeft  = B_Image_getPixel( image, 0, 4 );
    botRight = B_Image_getPixel( image, 2, 4 );
    ASSERT_EQ( 'A', topLeft  );
    ASSERT_EQ( 'B', topRight );
    ASSERT_EQ( 'C', botLeft  );
    ASSERT_EQ( 'D', botRight );
}

TEST_F(B_Image_Test,drawBitmap)
{
    uint8_t buffer[] = {
                            'a','b','c',
                            'd','e','f',
                            'g','h','i',
                            'j','k','l',
                            'm','n','o'
                       };
    B_Image_drawBitmap( image, buffer, B_Image_getWidth(image), B_Image_getHeight(image), 0, 0 );

    uint8_t topLeft, topRight, botLeft, botRight;
    topLeft  = B_Image_getPixel( image, 0, 0 );
    topRight = B_Image_getPixel( image, 2, 0 );
    botLeft  = B_Image_getPixel( image, 0, 4 );
    botRight = B_Image_getPixel( image, 2, 4 );
    ASSERT_EQ( 'a', topLeft  );
    ASSERT_EQ( 'c', topRight );
    ASSERT_EQ( 'm', botLeft  );
    ASSERT_EQ( 'o', botRight );
}
