#include "btypeface.h"
#include "bimage.h"

// POD to hold both the glyph image and useful slot info
typedef struct BT_Glyph_str
{
    FT_Glyph glyph;
    FT_Glyph_Metrics metrics;

}BT_Glyph;

struct BT_Face_Rec_str
{
    FT_Library lib;
    FT_Face face;
    FT_Glyph glyphs[NUM_GLYPHS];
};

static int BT_glyphHash( int charCode )
{
    return (charCode - FIRST_GLYPH);
}


static FT_Library lib = NULL;
static int libRefCount = 0;

BT_Face BT_Face_new( BT_Error *errorHandle, const char * fontFilePath, int pointSize )
{
    *errorHandle = BT_Err_Ok;

    // setup our FT face
    FT_Face ft_face;
    FT_Error ft_error;

    // singleton FT_Library
    if( !lib )
    {
        ft_error = FT_Init_FreeType( &lib );
        ++libRefCount;
    }


    ft_error = FT_New_Face( lib, fontFilePath, 0, &ft_face );
    if( ft_error == FT_Err_Unknown_File_Format )
    {
        *errorHandle = BT_Err_Unknown_File_Format;
        fprintf(stderr, "Unable to load font face\"%s\": unknown format\n", fontFilePath);
        return NULL;
    }
    else if(ft_error)
    {
        *errorHandle = BT_Err_Unknown_Error;
        fprintf(stderr, "Unable to load loading font face\"%s\": unknown ft_error\n", fontFilePath);
        return NULL;
    }

    ft_error = FT_Set_Char_Size( ft_face, 0, point(pointSize), DPI, DPI);

    BT_Face face = malloc(sizeof(BT_Face_Rec));
    face->lib = lib;
    face->face = ft_face;

    // load the glyphs by character
    for( int c = FIRST_GLYPH; c <= LAST_GLYPH; c++ )
    {
        // load and extract the glyph from the slot
        ft_error = FT_Load_Char( ft_face, c, FT_LOAD_DEFAULT );
        if( ft_error )
        {
            *errorHandle = BT_Err_Load_Char;
            free(face);
            fprintf(stderr, "Unable to load char '%c' of font face \"%s\"", c, fontFilePath);
            return NULL;
        }
        ft_error = FT_Get_Glyph( ft_face->glyph, &face->glyphs[BT_glyphHash(c)]);
        if( ft_error )
        {
            *errorHandle = BT_Err_Cache_Char;
            free(face);
            fprintf(stderr, "Unable to cache char '%c' of font face \"%s\"", c, fontFilePath);
            return NULL;
        }

        // render
        ft_error = FT_Glyph_To_Bitmap( &face->glyphs[BT_glyphHash(c)], FT_RENDER_MODE_NORMAL, 0, 1 );
        if ( ft_error )
        {
            *errorHandle = BT_Err_Render_Char;
            free(face);
            fprintf(stderr, "Unable to render char '%c' of font face \"%s\"", c, fontFilePath);
            return NULL;
        }
    }

    return face;
}

void BT_Face_delete( BT_Face face )
{
    for( int i = 0; i < NUM_GLYPHS; i++ )
    {
        FT_Done_Glyph(face->glyphs[i]);
    }
    FT_Done_Face( face->face );
    free(face);

    --libRefCount;
    if( libRefCount == 0 )
    {
        FT_Done_FreeType( lib );
        lib = NULL;
    }


}

FT_Glyph BT_Face_getGlyph( BT_Face face, int charcode )
{
    return face->glyphs[BT_glyphHash(charcode)];
}



B_Image BT_Face_renderString( BT_Face face, const char * string )
{
    if(!string)
        return NULL;
    int length = strlen(string);
    // this is adapted from their tutorial on with just kerning
    FT_Face ftFace;
    FT_Bool kerningEnabled;
    FT_UInt thisGlyphIndex;
    FT_UInt lastGlyphIndex;

    int pen_x, pen_y;

    pen_x = 0;
    pen_y = 0;
    ftFace = face->face;
    kerningEnabled = FT_HAS_KERNING( ftFace );
    lastGlyphIndex = 0;

    // we need to get the total advance of the string
    FT_UInt maxHeight = 0;
    FT_UInt maxYMax= 0;
    FT_UInt totalWidth = 0;
    for( int n = 0; n < length; n++ )
    {
        thisGlyphIndex = FT_Get_Char_Index( ftFace, string[n] );
        if( kerningEnabled && lastGlyphIndex && thisGlyphIndex )
        {
            FT_Vector delta;
            FT_Get_Kerning( ftFace, lastGlyphIndex, thisGlyphIndex,
                            FT_KERNING_DEFAULT, &delta );
            pen_x += delta.x >> 6;  // fractional 26.6: get the 6 fraction bits out
        }
        int glyphHash = BT_glyphHash(string[n]);

        pen_x += face->glyphs[glyphHash]->advance.x >> 16;    // fractional 16.16

        FT_BBox cbox;
        FT_Glyph_Get_CBox( face->glyphs[glyphHash], FT_GLYPH_BBOX_TRUNCATE, &cbox );
        FT_UInt boxHeight = (cbox.yMax) - (cbox.yMin);
        if( boxHeight > maxHeight )
            maxHeight = boxHeight;
        if( cbox.yMax > maxYMax )
            maxYMax = cbox.yMax;
        totalWidth += cbox.xMax - cbox.xMin;
        totalWidth += face->glyphs[glyphHash]->advance.x >> 16;

        lastGlyphIndex = thisGlyphIndex;
    }

    pen_x = 0;
    //pen_y = maxHeight-maxYMax;
    pen_y = maxHeight;
    B_Image image = B_Image_new( totalWidth, maxHeight );

    // now render
    for( int n = 0; n < length; n++ )
    {
        thisGlyphIndex = FT_Get_Char_Index( ftFace, string[n] );
        if( kerningEnabled && lastGlyphIndex && thisGlyphIndex )
        {
            FT_Vector delta;
            FT_Get_Kerning( ftFace, lastGlyphIndex, thisGlyphIndex,
                            FT_KERNING_DEFAULT, &delta );
            pen_x += delta.x >> 6;  // fractional 26.6: get the 6 fraction bits out
        }
        FT_Glyph glyph = face->glyphs[BT_glyphHash(string[n])];
        FT_BitmapGlyph bitmapGlyph = (FT_BitmapGlyph)glyph;

        // draw
        B_Image_drawBitmap( image, bitmapGlyph->bitmap.buffer,
                            bitmapGlyph->bitmap.width, bitmapGlyph->bitmap.rows,
                            pen_x - bitmapGlyph->left, pen_y - bitmapGlyph->top );
        //B_Image_fprint( image, stdout );
        pen_x += glyph->advance.x >> 16;    // fractional 16.16
        lastGlyphIndex = thisGlyphIndex;
    }

    return image;
}


// internal: needed for renderChar
B_Image BT_Face_renderGlyph( const FT_Glyph glyph )
{
    FT_BitmapGlyph bitmapGlyph = (FT_BitmapGlyph)glyph;
    B_Image image = B_Image_new( bitmapGlyph->bitmap.width, bitmapGlyph->bitmap.rows );
    B_Image_drawBitmap( image, bitmapGlyph->bitmap.buffer,
                        bitmapGlyph->bitmap.width, bitmapGlyph->bitmap.rows,
                        0, 0 );
    return image;
}


B_Image BT_Face_renderChar( BT_Face face, const int code )
{
    B_Image image = BT_Face_renderGlyph( face->glyphs[BT_glyphHash(code)] );
    return image;
}
