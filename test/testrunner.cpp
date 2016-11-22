#include <gtest/gtest.h>
#include <string>

std::string fontdir;
int main(int argc, char *argv[])
{
    fontdir = std::string(argv[1]);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
