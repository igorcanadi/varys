SHELL = /bin/sh
CC = g++
INCLUDES = -I./lib/mysql++-3.1.0/include/mysql++
LIBS = -L ./lib/mysql++-3.1.0 -lmysqlpp `net-snmp-config --libs`
CCFLAGS = -g $(INCLUDES) -DMYSQLPP_MYSQL_HEADERS_BURIED
LDFLAGS = -g
OBJS = \
    mysql_output.o \
    main.o \
    sensor.o \
    snmp_sensor.o

srcdir = src

all: varys

varys: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

%.o : $(srcdir)/%.cpp
	$(CC) $(CCFLAGS) $(LDFLAGS) -c $< -o $@

clean:
	rm *.o
