package main

import (
	"crypto/rand"
	"math/big"
	numRand "math/rand"
	"time"
)

func getE(euler *big.Int) *big.Int {
	i := Exp[numRand.Intn(2)]
	return big.NewInt(i)
}

func getKeys() (e, d, n *big.Int) {
	numRand.Seed(time.Now().UnixNano())
	p, err := rand.Prime(rand.Reader, Size)
	if err != nil {
		panic(err)
	}

	q, err := rand.Prime(rand.Reader, Size)
	if err != nil {
		panic(err)
	}

	var op1 big.Int
	n = op1.Mul(p, q)

	// Euler
	var op2 big.Int
	first := op2.Sub(p, big.NewInt(int64(1)))
	var op3 big.Int
	second := op3.Sub(q, big.NewInt(int64(1)))
	var op4 big.Int
	euler := op4.Mul(first, second)

	// Getting e
	e = getE(euler)

	var op5 big.Int
	d = op5.ModInverse(e, euler)

	return e, d, n
}
