package main

import (
	"fmt"
	"io"
	"net"
	"os"
)

const (
	localHost    = "localhost"
	defLocalPort = "7071"

	remoteHost = "localhost"
	remotePort = "8080"

	fileName = "Testify.mp3"
	//fileName   = "test"
	bufferSize = 1024
)

func main() {
	conn, err := net.Dial("tcp", "127.0.0.1:8080")
	errorExit(err)
	defer conn.Close()

	fmt.Fprintln(conn, fileName)
	file, err := os.OpenFile(fileName, os.O_RDWR|os.O_CREATE, 0755)
	errorExit(err)
	n, err := io.Copy(file, conn)
	errorExit(err)
	fmt.Println("File downloaded. Total size:", n, "bytes")

}

func errorExit(err error) {
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}
}
