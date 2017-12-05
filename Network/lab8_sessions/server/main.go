package main

import (
	"bufio"
	"fmt"
	"net"
	"strings"
	"time"
)

func handleConnection(conn net.Conn) {
	name := conn.RemoteAddr().String()

	fmt.Printf("%+v connected\n", name)

	if len(FreeID) == 0 {
		conn.Write([]byte("There is no free ID"))
		conn.Close()
		return
	}

	ID := FreeID[0]
	FreeID = FreeID[1:]

	for {
		out, err := bufio.NewReader(conn).ReadString('\n')
		out = strings.Replace(out, "\n", "", -1)

		if err != nil {
			fmt.Println(err)
			fmt.Println(name + " connection close")
			conn.Close()
			FreeID = append(FreeID, ID)
			return
		}

		fmt.Println(name, "send", out)

		time.Sleep(time.Second * dT)
	}
}

func main() {
	FreeID = append(FreeID, "9EDDB952BD")
	FreeID = append(FreeID, "56AEC790CF")
	FreeID = append(FreeID, "A5226CA405")
	FreeID = append(FreeID, "5510FED259")
	FreeID = append(FreeID, "A595E119CE")

	listner, err := net.Listen("tcp", ":8080")
	if err != nil {
		panic(err)
	}
	for {
		conn, err := listner.Accept()
		if err != nil {
			panic(err)
		}
		go handleConnection(conn)
	}
}
