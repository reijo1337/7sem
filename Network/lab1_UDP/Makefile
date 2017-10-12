BUILD_DIR   = ./build
CFLAGS	    = -Wall -pedantic -ansi

# linking both client and server, just after cleaning old stuff
all: clean udp_client.o udp_echo.o
	gcc $(BUILD_DIR)/udp_client.o -o $(BUILD_DIR)/udp_client $(LDFLAGS)
	gcc $(BUILD_DIR)/udp_echo.o -o $(BUILD_DIR)/udp_echo $(LDFLAGS)
	
# compiling client side
udp_client.o: udp_client.c
	gcc -c udp_client.c -o $(BUILD_DIR)/udp_client.o $(CFLAGS)
	
# compiling server side
udp_echo.o: udp_echo.c
	gcc -c udp_echo.c -o $(BUILD_DIR)/udp_echo.o $(CFLAGS)

# removing old stuff
clean:
	rm -rfv $(BUILD_DIR)/*