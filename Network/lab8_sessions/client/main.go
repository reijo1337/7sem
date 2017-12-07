package main

import "net"
import "fmt"
import "time"

func main() {
	conn, err := net.Dial("tcp", "127.0.0.1:8080")
	if err != nil {
		panic(err)
	}
	defer conn.Close()

	for i := 0; i < 5; i++ {
		fmt.Fprintln(conn, "check")
		time.Sleep(4 * time.Second)
	}

	time.Sleep(8 * time.Second)
}
