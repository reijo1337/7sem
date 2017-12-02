package main

import (
	"encoding/base32"
	"flag"
	"fmt"
	"io/ioutil"
	"os"
)

func main() {
	message := flag.String("t", "", "message for encrypting")
	filename := flag.String("f", "", "name of file for encrypting")

	flag.Parse()

	if *message != "" {
		cypher, err := NewCypher()
		if err != nil {
			panic(err)
		}

		msg := []byte(*message)
		fmt.Printf("Initial message:\n%s\n%X\n\n", msg, msg)

		enc := cypher.Encrypt(msg)
		fmt.Printf("Encryped message:\n%X\n\n", enc)

		dec, err := cypher.Decrypt(enc)
		if err != nil {
			panic(err)
		}
		fmt.Printf("Decryped message:\n%s\n%X\n\n", dec, dec)

	} else if *filename != "" {
		cypher, err := NewCypher()
		if err != nil {
			panic(err)
		}

		file, err := os.Open(*filename)
		if err != nil {
			panic(err)
		}
		defer file.Close()

		filestat, err := file.Stat()
		if err != nil {
			panic(err)
		}

		data, err := ioutil.ReadAll(file)
		if err != nil {
			panic(err)
		}

		// Encoding
		msg := make([]byte, base32.StdEncoding.EncodedLen(len(data)))
		base32.StdEncoding.Encode(msg, data)
		enc := cypher.Encrypt(msg)

		fileEnc, err := os.OpenFile("enc-"+*filename, os.O_WRONLY|os.O_CREATE, filestat.Mode())
		if err != nil {
			panic(err)
		}
		defer fileEnc.Close()

		fileEnc.Write(enc)

		// Decoding
		dec, err := cypher.Decrypt(enc)
		if err != nil {
			panic(err)
		}

		fileDec, err := os.OpenFile("dec-"+*filename, os.O_WRONLY|os.O_CREATE, filestat.Mode())
		if err != nil {
			panic(err)
		}
		defer fileDec.Close()

		out := make([]byte, base32.StdEncoding.DecodedLen(len(dec)))
		_, err = base32.StdEncoding.Decode(out, dec)
		if err != nil {
			panic(err)
		}

		fileDec.Write(out)
	} else {
		fmt.Println("Usage: go run *.go <-t=\"message\" | -f=\"filename\">")
	}
}
