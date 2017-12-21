package main

import (
	"bytes"
	"fmt"
	"io/ioutil"
	"os"
)

func main() {
	file, err := os.Open(os.Args[1])
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

	huffman := NewHuffman(data)

	compressed := huffman.compress(data)

	fileComp, err := os.OpenFile(os.Args[1]+".comp", os.O_WRONLY|os.O_CREATE, filestat.Mode())
	if err != nil {
		panic(err)
	}
	defer fileComp.Close()

	fileComp.Write(compressed)

	decompressed := huffman.decompress(compressed)
	fileDeComp, err := os.OpenFile(os.Args[1]+".decomp", os.O_WRONLY|os.O_CREATE, filestat.Mode())
	if err != nil {
		panic(err)
	}
	defer fileDeComp.Close()

	fileDeComp.Write(decompressed)

	if bytes.Compare(data, decompressed) == 0 {
		fmt.Println("OK")
	} else {
		fmt.Println("Not OK")
	}
}
