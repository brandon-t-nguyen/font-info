#include <gtest/gtest.h>
#include <string>

#include <bimage.h>
#include <bmask.h>

#include <alg.h>

extern std::string fontdir;
class Algorithm_Test : public :: testing :: Test
{
protected:
    Algorithm alg = nullptr;
    virtual void SetUp()
    {
        alg = Alg_getInstance();
    }

    virtual void TearDown()
    {
        Alg_doneInstance(alg);
    }
};

enum comp
{
    comp_eq,
    comp_gt,
    comp_lt,
    comp_ge,
    comp_le
};

struct BinaryMetricTestcase
{
public:
    Metric metric;
    comp comparator;
    const char * font1file;
    const char * font2file;
};

#define ARRAY_LENGTH(x) (sizeof(x)/sizeof(x[0]))

BinaryMetricTestcase binaryCases[] =
{
    { Metric_Width, comp_gt, "StalinistOne-Regular.ttf", "BenchNine-Regular.ttf" },
    { Metric_Height, comp_gt, "BenchNine-Regular.ttf", "Teko-Regular.ttf" },
    { Metric_AspectRatio, comp_gt, "SixCaps.ttf", "Asset.ttf" },
    { Metric_xHeight, comp_gt, "SixCaps.ttf", "Teko-Regular.ttf" },
    { Metric_Density, comp_gt, "Chango-Regular.ttf", "Codystar-Regular.ttf" },
    { Metric_Slant, comp_gt, "MonsieurLaDoulaise-Regular.ttf", "Roboto-Regular.ttf" },
    { Metric_Curve, comp_gt, "Amita-Regular.ttf", "Aldrich-Regular.ttf" },
    { Metric_Serif, comp_gt, "Arvo-Regular.ttf", "SourceSansPro-Regular.ttf" },
};

TEST_F(Algorithm_Test,BinaryMetricTests)
{

    for( unsigned int i = 0; i < ARRAY_LENGTH(binaryCases); i++ )
    {
        BinaryMetricTestcase & testcase = binaryCases[i];

        BT_Error error;
        BT_Face font1, font2;
        double font1score, font2score;

        font1 = BT_Face_new( &error, (fontdir + testcase.font1file).c_str(), 12 );
        if(!font1)
            FAIL() << "Font 1 (" << testcase.font1file << ") is borked";

        font2 = BT_Face_new( &error, (fontdir + testcase.font2file).c_str(), 12 );
        if(!font2)
            FAIL() << "Font 2 (" << testcase.font2file << ") is borked";

        font1score = Alg_calculateMetric( alg, font1, testcase.metric );
        font2score = Alg_calculateMetric( alg, font2, testcase.metric );

        switch(testcase.comparator)
        {
        case comp_eq:
            EXPECT_DOUBLE_EQ( font1score, font2score )
                << "!("<<testcase.font1file<<" == "<<testcase.font2file<<") "<< Metric_toString(testcase.metric) << std::endl;
            break;
        case comp_gt:
            EXPECT_GT( font1score, font2score )
                << "!("<<testcase.font1file<<" > "<<testcase.font2file<<") "<< Metric_toString(testcase.metric) << std::endl;
            break;
        case comp_lt:
            EXPECT_LT( font1score, font2score )
                << "!("<<testcase.font1file<<" < "<<testcase.font2file<<") "<< Metric_toString(testcase.metric) << std::endl;
            break;
        case comp_ge:
            EXPECT_GE( font1score, font2score )
                << "!("<<testcase.font1file<<" >= "<<testcase.font2file<<") "<< Metric_toString(testcase.metric) << std::endl;
            break;
        case comp_le:
            EXPECT_LE( font1score, font2score )
                << "!("<<testcase.font1file<<" <= "<<testcase.font2file<<") "<< Metric_toString(testcase.metric) << std::endl;
            break;
        }

        BT_Face_delete(font1);
        BT_Face_delete(font2);
    }

}

/*
TEST_F(Algorithm_Test,BinaryMetricTests)
{
    BT_Error error;
    BT_Face narrowFont = BT_Face_new( &error, (fontdir + "BenchNine-Regular.ttf").c_str(), 12 );
    BT_Face wideFont = BT_Face_new( &error, (fontdir + "StalinistOne-Regular.ttf").c_str(), 12 );

    double narrowScore, wideScore;
    narrowScore = Alg_calculateMetric( alg, narrowFont, Metric_Width );
    wideScore = Alg_calculateMetric( alg, wideFont, Metric_Width );

    ASSERT_GT( wideScore, narrowScore );

    BT_Face_delete(narrowFont);
    BT_Face_delete(wideFont);
}
*/
