package main

import (
	"errors"
	"fmt"
	"math/big"
)

type cypher struct {
	e *big.Int
	d *big.Int
	n *big.Int
}

func NewCypher() (*cypher, error) {
	e, d, n := getKeys()

	return &cypher{e, d, n}, nil
}

func (c *cypher) Encrypt(src []byte) []byte {
	ret := make([]byte, 0)

	for _, b := range src {
		en := make([]byte, 1)
		en[0] = b
		encBlock := c.encryptBlock(en)
		for j := 0; j < len(encBlock); j++ {
			ret = append(ret, encBlock[j])
		}
	}

	return ret
}

func (c *cypher) encryptBlock(src []byte) []byte {
	byTbi := new(big.Int)
	byTbi.SetBytes(src)

	ret := new(big.Int)
	ret.Exp(byTbi, c.e, c.n)

	return ret.Bytes()
}

func (c *cypher) Decrypt(src []byte) ([]byte, error) {
	l := Size / 4
	if len(src)%l != 0 {
		fmt.Println(len(src))
		return nil, errors.New("source is an incorrect length, must be a multiple of 32 bytes")
	}

	ret := make([]byte, 0)
	for i := 0; i < len(src)/l; i++ {
		decBlock := c.decryptBlock(src[l*i : l*(i+1)])
		for j := 0; j < len(decBlock); j++ {
			ret = append(ret, decBlock[j])
		}
	}

	return removePadding(ret), nil
}

func (c *cypher) decryptBlock(src []byte) []byte {
	byTbi := new(big.Int)
	byTbi.SetBytes(src)

	ret := new(big.Int)
	ret.Exp(byTbi, c.d, c.n)

	return ret.Bytes()
}

func removePadding(b []byte) []byte {
	idx := 0
	for i := len(b) - 1; i >= 0; i-- {
		if b[i] != 0x00 {
			idx = i + 1
			break
		}
	}
	return b[:idx]
}
