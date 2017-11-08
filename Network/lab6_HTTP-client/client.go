package main

import (
	"fmt"
	"io/ioutil"
	"net/http"
)

func runGet(url string) {
	fmt.Println(url)
	resp, err := http.Get(url)
	if err != nil {
		fmt.Println("error happend", err)
		return
	}
	defer resp.Body.Close() // важный пункт!

	respBody, err := ioutil.ReadAll(resp.Body)

	fmt.Printf("http.Get body %#v\n\n\n", string(respBody))
}

func main() {
	runGet("https://www.google.ru/")
	runGet("https://ya.ru/")
	runGet("http://127.0.0.1:8080/")
}
