SHELL = /bin/sh
CC = g++
INCLUDES = 
LIBS = -lmysqlpp `net-snmp-config --libs` -lboost_thread -lpthread
CCFLAGS = -g $(INCLUDES) -DMYSQLPP_MYSQL_HEADERS_BURIED
LDFLAGS = -g
OBJS = \
    mysql_output.o \
    main.o \
    sensor.o \
    snmp_sensor.o \
    sensor_manager.o \
    output_manager.o

srcdir = src

all: varys 

varys: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

%.o: $(srcdir)/%.cpp
	$(CC) $(CCFLAGS) $(LDFLAGS) -c $< -o $@

clean:
	rm *.o
