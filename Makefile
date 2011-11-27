#DIRECTIVES = -DWINDOWS=1
CXXFLAGS = -O2 -g -Wall -I. -Ilib -Iinclude $(DIRECTIVES) `icu-config  --cppflags`
LDFLAGS = `icu-config  --ldflags`

OBJS = lib/socket.o lib/socketexception.o server/serversocket.o \
	server/rokserver.o server/rokconnection.o access/rokaccess.o \
	protocol/protocol.o protocol/protocolv1.o \
	config.o debug.o utils.o RokDB.o appmutex.o main.o

LIBS = -lpthread

TARGET = RokDB

all: rokdbclient $(TARGET)

$(TARGET):	$(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) $(LIBS) -o $(TARGET)
	mv $(TARGET) bin/

rokdbclient:
	cd client && make

valgrind: $(TARGET) rokdbclient
	valgrind --leak-check=full ./bin/$(TARGET)
	
run: $(TARGET) rokdbclient
	./bin/$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
	cd client && make clean
