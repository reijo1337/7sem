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
	length := len(src)
	if length%32 != 0 {
		length = 32 * (len(src)/32 + 1)
	}
	b := make([]byte, length)
	copy(b, src)

	ret := make([]byte, 0)

	for i := 0; i < length/32; i++ {
		encBlock := c.encryptBlock(b[32*i : 32*(i+1)])
		for j := 0; j < len(encBlock); j++ {
			ret = append(ret, encBlock[j])
		}
	}
	fmt.Println(len(ret))

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
	if len(src)%32 != 0 {
		return nil, errors.New("source is an incorrect length, must be a multiple of 32 bytes")
	}

	b := make([]byte, len(src))
	copy(b, src)

	ret := make([]byte, 0)

	for i := 0; i < len(src)/32; i++ {
		decBlock := c.decryptBlock(b[32*i : 32*(i+1)])
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
