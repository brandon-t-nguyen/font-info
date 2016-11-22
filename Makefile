CC = g++
LD = ld
CFLAGS =-Wall -Wextra -Wno-unused-parameter -Wno-ignored-qualifiers -g -I/usr/include/freetype2 -lfreetype
SOURCES = src/*.c
INCLUDE = inc
EXECUTABLE = font-info
VISUALIZE = vis
#TESTFONT = ~/fonts/google/Roboto-Regular.ttf
#TESTFONT = ~/fonts/google/Pacifico.ttf
#TESTFONT = ~/fonts/google/Sacramento-Regular.ttf
#TESTFONT = ~/fonts/google/Strait-Regular.ttf
#TESTFONT = ~/fonts/google/Amita-Regular.ttf
#TESTFONT = ~/fonts/google/Aldrich-Regular.ttf
TESTFONT = ~/fonts/google/*.ttf

#FONT1 = ~/fonts/google/MonsieurLaDoulaise-Regular.ttf
#FONT2 = ~/fonts/google/Roboto-Regular.ttf
#FONT1 = ~/fonts/google/Amita-Regular.ttf
#FONT2 = ~/fonts/google/Aldrich-Regular.ttf
FONT1 = ~/fonts/google/PlayfairDisplay-Regular.ttf
FONT2 = ~/fonts/google/Roboto-Regular.ttf
LETTER = T

MAIN = main.c

TEST_DIR = test
GTEST_EXEC = font-test
GTEST_SRC = $(TEST_DIR)/*.cpp

$(EXECUTABLE):
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(MAIN) $(SOURCES) -I $(INCLUDE)

$(VISUALIZE):
	$(CC) $(CFLAGS) -lncurses -D VISUALIZE -o $(VISUALIZE) $(MAIN) $(SOURCES) -I $(INCLUDE)

$(GTEST_EXEC):
	$(CC) $(CFLAGS) -o $(GTEST_EXEC) $(SOURCES) -I $(INCLUDE) $(GTEST_SRC) -lgtest

clean:
	rm *.o $(EXECUTABLE) $(VISUALIZE) $(TESTLOG) $(GTEST_EXEC)


run: $(EXECUTABLE)
	@./$(EXECUTABLE) ${arg}

test: $(EXECUTABLE)
	@./$(EXECUTABLE) $(TESTFONT)
debug: $(EXECUTABLE)
	gdb --args ./$(EXECUTABLE) ${arg} $(TESTFONT)
mem: $(EXECUTABLE)
	valgrind -v --leak-check=yes --undef-value-errors=no ./$(EXECUTABLE) $(TESTFONT)

gtest: $(GTEST_EXEC)
	@./$(GTEST_EXEC) ~/fonts/google/

testvis: $(VISUALIZE)
	./$(VISUALIZE) $(FONT1) $(FONT2) $(LETTER)
debugvis: $(VISUALIZE)
	gdb --args ./$(VISUALIZE) $(FONT1) $(FONT2) $(LETTER)
memvis: $(VISUALIZE)
	valgrind -v --leak-check=yes --undef-value-errors=no ./$(VISUALIZE) $(FONT1) $(FONT2) $(LETTER)
