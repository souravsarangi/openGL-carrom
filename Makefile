CC = g++
CFLAGS = -Wall
PROG = out

SRCS = carrom.cpp
LIBS = -lglut -lGL -lGLU -lm

all: $(PROG)

$(PROG):	$(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)

clean:
	rm -f $(PROG)
