#DIRECTIVES = -DWINDOWS=1
CXXFLAGS = -O2 -g -Wall -I. -Ilib -Iinclude $(DIRECTIVES) `icu-config  --cppflags`
LDFLAGS = `icu-config  --ldflags`

OBJS = lib/socket.o lib/socketexception.o server/serversocket.o \
	server/rokserver.o server/rokconnection.o config.o debug.o RokDB.o \
	appmutex.o main.o

LIBS = -lpthread

TARGET = RokDB

all: rokdbclient $(TARGET)

$(TARGET):	$(OBJS)
	$(CXX) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LIBS)
	mv $(TARGET) bin/

rokdbclient:
	cd client && make

valgrind: $(TARGET)
	valgrind --leak-check=full ./bin/$(TARGET)
	
run: $(TARGET) rokdbclient
	./bin/$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
	cd client && make clean
