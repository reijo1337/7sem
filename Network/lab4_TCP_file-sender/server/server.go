package main

import (
	"bufio"
	"fmt"
	"io"
	"net"
	"os"
	"strings"
)

func handleConnection(conn net.Conn) {
	name := conn.RemoteAddr().String()
	fmt.Printf("%+v connected\n", name)

	defer conn.Close()

	fileName, err := bufio.NewReader(conn).ReadString('\n')
	fileName = strings.Replace(fileName, "\n", "", -1)

	fmt.Println(name, "requested", fileName)

	file, err := os.Open(fileName)
	if err != nil {
		panic(err)
	}
	defer file.Close()

	n, err := io.Copy(conn, file)
	if err != nil {
		panic(err)
	}
	fmt.Println(n, "bytes sent")
}

func main() {
	listner, err := net.Listen("tcp", ":8080")
	if err != nil {
		panic(err)
	}
	fmt.Println("Server started at port :8080")
	for {
		conn, err := listner.Accept()
		if err != nil {
			panic(err)
		}
		go handleConnection(conn)
	}
}
