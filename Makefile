CC = gcc
LD = ld
CFLAGS =-Wall -Wextra -Wno-unused-parameter -g -I/usr/include/freetype2 -lfreetype
SOURCES = *.c
EXECUTABLE = font-info
TESTFONT = ~/fonts/google/Roboto-Regular.ttf
#TESTFONT = ~/fonts/google/Pacifico.ttf
#TESTFONT = ~/fonts/google/Sacramento-Regular.ttf
#TESTFONT = ~/fonts/google/Strait-Regular.ttf
#TESTFONT = ~/fonts/google/Amita-Regular.ttf
#TESTFONT = ~/fonts/google/Aldrich-Regular.ttf
#TESTFONT = ~/fonts/google/*.ttf

build: $(OBJECTS)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(SOURCES) 
$(EXECUTABLE):
	make build

clean:
	rm *.o $(EXECUTABLE) $(TESTLOG)


run: $(EXECUTABLE)
	./$(EXECUTABLE) ${arg}

test: $(EXECUTABLE)
	./$(EXECUTABLE) $(TESTFONT)

mem:
	valgrind -v --leak-check=yes --undef-value-errors=no ./$(EXECUTABLE) $(TESTFONT)

debug:
	gdb --args ./$(EXECUTABLE) ${arg} $(TESTFONT)
