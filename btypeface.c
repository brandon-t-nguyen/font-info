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
    B_Image glyphs[NUM_GLYPHS];
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
    }
    ++libRefCount;


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
        int index = BT_glyphHash(c);
        // load and extract the glyph from the slot
        ft_error = FT_Load_Char( ft_face, c, FT_LOAD_DEFAULT );
        if( ft_error )
        {
            *errorHandle = BT_Err_Load_Char;
            fprintf(stderr, "Unable to load char '%c' of font face \"%s\"", c, fontFilePath);
            face->glyphs[index] = NULL;
            continue;
        }
        // render
        ft_error = FT_Render_Glyph( ft_face->glyph, FT_RENDER_MODE_NORMAL );
        if ( ft_error )
        {
            *errorHandle = BT_Err_Render_Char;
            fprintf(stderr, "Unable to render char '%c' of font face \"%s\"", c, fontFilePath);
            face->glyphs[index] = NULL;
            continue;
        }
        B_Image image = B_Image_new( ft_face->glyph->bitmap.width, ft_face->glyph->bitmap.rows );
        B_Image_drawBitmap( image, ft_face->glyph->bitmap.buffer,
                            ft_face->glyph->bitmap.width, ft_face->glyph->bitmap.rows,
                            0, 0 );
        face->glyphs[index] = image;

    }

    return face;
}

void BT_Face_delete( BT_Face face )
{
    for( int i = 0; i < NUM_GLYPHS; i++ )
    {
        B_Image_delete(face->glyphs[i]);
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

B_Image BT_Face_getChar( const BT_Face face, const int code )
{
    const B_Image image = face->glyphs[BT_glyphHash(code)];
    return image;
}

