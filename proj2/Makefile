PROG = proj2
EXEC = $(PROG)
sources = $(PROG).c data.c program.c

CXX = gcc
RM = rm -f

CFLAGS = -std=gnu99 -Wall -Wextra -Werror -pedantic
LDFLAGS = -lrt -pthread

OBJFILES = $(sources:.c=.o)

.PHONY : all

all : $(EXEC) 

%.o : %.c 
	$(CXX) $(CFLAGS) -c $< -o $@

$(EXEC) : $(OBJFILES)
	$(CXX) $(CFLAGS) -o $@ $(OBJFILES) $(LDFLAGS)

clean:
	$(RM) *.o core *.out

cleanall: clean
	$(RM) $(EXEC)
