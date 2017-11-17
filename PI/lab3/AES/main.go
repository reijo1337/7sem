package main

import (
	"io/ioutil"
	"os"
	"flag"
	"crypto/rand"
	"fmt"
)

func main() {
	message := flag.String("t", "", "message for encrypting")
	filename := flag.String("f", "", "name of file for encrypting")

	flag.Parse()

	if (*message != "") {
		key := make([]byte, 32)
		rand.Read(key)
		fmt.Printf("Key: %X\n\n", key)
	
		cypher, err := NewCypher(key)
		if err != nil {
			fmt.Println("Error:", err.Error())
			return
		}
	
		msg := []byte(*message)
		fmt.Printf("Initial message:\n%s\n%X\n\n", msg, msg)
	
		enc := cypher.Encrypt(msg)
		fmt.Printf("Encryped message:\n%X\n\n", enc)
	
		dec, err := cypher.Decrypt(enc)
		if err != nil {
			fmt.Println("Error:", err.Error())
			return
		}
		fmt.Printf("Decryped message:\n%s\n%X\n\n", dec, dec)
	} else if (*filename != "") {
		file, err := os.Open(*filename)
		if err != nil {
			panic(err)
		}
		defer file.Close()

		filestat, err := file.Stat()
		if err != nil {
			panic(err)
		}

		msg, err := ioutil.ReadAll(file)
		if err != nil {
			panic(err)
		}

		key := make([]byte, 32)
		rand.Read(key)
		fmt.Printf("Key: %X\n\n", key)
	
		cypher, err := NewCypher(key)
		if err != nil {
			panic(err)
		}

		enc := cypher.Encrypt(msg)
		fileEnc, err := os.OpenFile("enc-" + *filename, os.O_WRONLY | os.O_CREATE, filestat.Mode())
		if err != nil {
			panic(err)
		}
		defer fileEnc.Close()

		fileEnc.Write(enc)

		dec, err := cypher.Decrypt(enc)
		if err != nil {
			panic(err)
		}

		fileDec, err := os.OpenFile("dec-" + *filename, os.O_WRONLY | os.O_CREATE, filestat.Mode())
		if err != nil {
			panic(err)
		}
		defer fileDec.Close()

		fileDec.Write(dec)
	} else {
		fmt.Println("Usage: go run *.go <-t=\"message\" | -f=\"filename\">")
	}
}