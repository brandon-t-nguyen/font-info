CC = gcc
LD = ld
CFLAGS =-Wall -Wextra -Wno-unused-parameter -Wno-ignored-qualifiers -g -I/usr/include/freetype2 -lfreetype
SOURCES = *.c
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

$(EXECUTABLE):
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(SOURCES) 

$(VISUALIZE):
	$(CC) $(CFLAGS) -lncurses -D VISUALIZE -o $(VISUALIZE) $(SOURCES)

clean:
	rm *.o $(EXECUTABLE) $(VISUALIZE) $(TESTLOG)


run: $(EXECUTABLE)
	@./$(EXECUTABLE) ${arg}

test: $(EXECUTABLE)
	@./$(EXECUTABLE) $(TESTFONT)
debug: $(EXECUTABLE)
	gdb --args ./$(EXECUTABLE) ${arg} $(TESTFONT)
mem: $(EXECUTABLE)
	valgrind -v --leak-check=yes --undef-value-errors=no ./$(EXECUTABLE) $(TESTFONT)


testvis: $(VISUALIZE)
	./$(VISUALIZE) $(FONT1) $(FONT2) $(LETTER)
debugvis: $(VISUALIZE)
	gdb --args ./$(VISUALIZE) $(FONT1) $(FONT2) $(LETTER)
memvis: $(VISUALIZE)
	valgrind -v --leak-check=yes --undef-value-errors=no ./$(VISUALIZE) $(FONT1) $(FONT2) $(LETTER)
