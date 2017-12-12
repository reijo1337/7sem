package main

import (
	"bufio"
	"crypto"
	"crypto/rand"
	"crypto/rsa"
	"crypto/sha256"
	"fmt"
	"io"
	"os"
)

var (
	rng io.Reader
)

func sign(filename string, rsaPrivateKey *rsa.PrivateKey) ([]byte, error) {
	f, err := os.Open(filename)
	if err != nil {
		return nil, err
	}
	defer f.Close()

	h := sha256.New()
	if _, err := io.Copy(h, f); err != nil {
		return nil, err
	}

	hashed := h.Sum(nil)

	signature, err := rsa.SignPKCS1v15(rng, rsaPrivateKey, crypto.SHA256, hashed[:])
	if err != nil {
		return nil, err
	}

	return signature, nil
}

func check(filename string, signature []byte, rsaPublicKey *rsa.PublicKey) error {
	f, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer f.Close()

	h := sha256.New()
	if _, err := io.Copy(h, f); err != nil {
		return err
	}

	hashed := h.Sum(nil)

	return rsa.VerifyPKCS1v15(rsaPublicKey, crypto.SHA256, hashed[:], signature)
}

func main() {
	if os.Args[1] == "" {
		fmt.Println("You must set file name")
		return
	}

	rng := rand.Reader

	rsaPrivateKey, err := rsa.GenerateKey(rng, 1024)
	if err != nil {
		panic(err)
	}

	rsaPublicKey := rsaPrivateKey.PublicKey

	signature, err := sign(os.Args[1], rsaPrivateKey)
	if err != nil {
		panic(err)
	}

	reader := bufio.NewReader(os.Stdin)
	fmt.Print("Verify? [y/n]: ")
	text, _ := reader.ReadString('\n')
	if text == "y\n" {
		err = check(os.Args[1], signature, &rsaPublicKey)
		if err != nil {
			fmt.Println("Verification failed")
		} else {
			fmt.Println("Verification OK")
		}
	}
}
