BUILD_DIR   = ./build
CFLAGS = -Wall -pedantic -ansi

# this perfoms linking all the stuff
all: clean client.o server.o
	@echo === Linking client and server ===
	gcc $(BUILD_DIR)/client.o -o $(BUILD_DIR)/client $(LDFLAGS)
	gcc $(BUILD_DIR)/server.o -o $(BUILD_DIR)/server $(LDFLAGS)
	
# these simply compiles tcp_client.c to object file
client.o: client.c
	@echo === Compiling client side ===
	gcc -c client.c -o $(BUILD_DIR)/client.o $(CFLAGS)
	
server.o: server.c
	@echo === Compiling server side ===
	gcc -c server.c -o $(BUILD_DIR)/server.o $(CFLAGS)
	
clean:
	@echo === Removing old builds ===
	rm -rfv $(BUILD_DIR)
	@echo === Creating new build dir ===
	mkdir $(BUILD_DIR)
