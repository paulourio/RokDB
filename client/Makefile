CXXFLAGS = -O2 -g -Wall -I../lib -I../include -DNOCONFIG=1 \
	`icu-config  --cppflags`
LDFLAGS = `icu-config  --ldflags`

OBJS = clientsocket.o ../lib/socket.o ../lib/socketexception.o \
	debug.o main.o

LIBS = -lpthread

TARGET = rdbclient

$(TARGET):	$(OBJS)
	$(CXX) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LIBS)
	mv $(TARGET) ../bin/

all: $(TARGET)

valgrind: $(TARGET)
	valgrind --leak-check=full ../bin/$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
