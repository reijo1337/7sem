package main

import (
	"bufio"
	"fmt"
	"net"
	"os"
	"strconv"
	"strings"
)

func handleConnection(conn net.Conn) {
	name := conn.RemoteAddr().String()

	fmt.Printf("%+v connected\n", name)
	conn.Write([]byte("Hello, " + name + "\n\r"))

	defer conn.Close()

	scanner := bufio.NewScanner(conn)
	for scanner.Scan() {
		text := scanner.Text()
		if text == "Exit" {
			conn.Write([]byte("Bye\n\r"))
			fmt.Println(name, "disconnected")
			break
		} else if text != "" {
			fmt.Println(name, "enters", text)
			conn.Write([]byte("You enter " + text + "\n\r"))

			reqNum, _ := strconv.Atoi(text)

			file, err := os.Open("ip")
			if err != nil {
				panic(err)
			}
			defer file.Close()

			i := 0
			fileScanner := bufio.NewScanner(file)

			for fileScanner.Scan() {
				ipAdr := fileScanner.Text()
				i++
				if i == reqNum {
					fmt.Println(ipAdr)
					splitedIP := strings.Split(ipAdr, ".")

					// Вывод в двоичном виде
					intPart, _ := strconv.Atoi(splitedIP[0])
					conn.Write([]byte(strconv.FormatInt(int64(intPart), 2)))
					for i := 1; i < len(splitedIP); i++ {
						intPart, _ := strconv.Atoi(splitedIP[i])
						conn.Write([]byte("." + strconv.FormatInt(int64(intPart), 2)))
					}
					conn.Write([]byte("\n"))

					// Вывод в восмиричном виде
					intPart, _ = strconv.Atoi(splitedIP[0])
					conn.Write([]byte(strconv.FormatInt(int64(intPart), 8)))
					for i := 1; i < len(splitedIP); i++ {
						intPart, _ := strconv.Atoi(splitedIP[i])
						conn.Write([]byte("." + strconv.FormatInt(int64(intPart), 8)))
					}
					conn.Write([]byte("\n"))

					// Вывод в шестнадцатиричном виде
					intPart, _ = strconv.Atoi(splitedIP[0])
					conn.Write([]byte(strconv.FormatInt(int64(intPart), 16)))
					for i := 1; i < len(splitedIP); i++ {
						intPart, _ := strconv.Atoi(splitedIP[i])
						conn.Write([]byte("." + strconv.FormatInt(int64(intPart), 16)))
					}
					conn.Write([]byte("\n"))
					continue
				}
			}
		}
	}
}

func main() {
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
