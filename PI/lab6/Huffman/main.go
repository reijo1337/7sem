package main

import (
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

	fmt.Fprintln(fileComp, huffman.ignore)
	huffman.writeTree(fileComp, filestat)
	fileComp.Write(compressed)

	fmt.Fscanln(fileComp, huffman.ignore)
	fileComp.Close()
	huffman.readTree()
	decompressed := huffman.decompress(compressed)
	fileDeComp, err := os.OpenFile(os.Args[1]+".decomp", os.O_WRONLY|os.O_CREATE, filestat.Mode())
	if err != nil {
		panic(err)
	}
	defer fileDeComp.Close()

	fileDeComp.Write(decompressed)
}
