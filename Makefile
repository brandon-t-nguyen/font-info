CC = gcc
LD = ld
CFLAGS =-Wall -Wextra -g -I/usr/include/freetype2 -lfreetype
SOURCES = *.c
EXECUTABLE = font-info
#TESTFONT = /usr/share/fonts/TTF/Oxygen-Sans.ttf
#TESTFONT = /usr/share/fonts/TTF/DejaVuSans.ttf
#TESTFONT = ~/google-fonts/apache/roboto/Roboto-Regular.ttf
TESTFONT = ~/google-fonts/ofl/pacifico/Pacifico.ttf

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
	valgrind -v --leak-check=yes ./$(EXECUTABLE) $(TESTFONT)

debug:
	gdb --args ./$(EXECUTABLE) ${arg} $(TESTFONT)
