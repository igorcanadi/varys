SHELL = /bin/sh
CC = g++
INCLUDES = -I./lib/mysql++-3.1.0/include
LIBS = -L./lib/mysql++-3.1.0/lib -lmysqlpp
CCFLAGS = -g $(INCLUDES)
LDFLAGS = -g
OBJS = \
    mysql_output.o \
    main.o

srcdir = src/

all: varys

varys: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

%.o : $(srcdir)/%.cpp
	$(CC) $(CCFLAGS) $(LDFLAGS) -c $< -o $@


