package main

import "errors"

type cypher struct {
	key    []byte
	xkey   []byte
	rounds int
}

// Rotate word left 1 byte
func rotw(w []byte) {
	temp := w[0]
	w[0] = w[1]
	w[1] = w[2]
	w[2] = w[3]
	w[3] = temp
}

// Returns substitute byte according to Rijndael's S-box
func getSboxSub(b byte) byte {
	return sbox[b]
}

// Returns substitute byte according to Rijndael's inverse S-box
func getInvSboxSub(b byte) byte {
	return rsbox[b]
}

// Substitutes each byte according to Rijndael's S-box
func subBytes(s []byte) {
	for i, v := range s {
		s[i] = getSboxSub(v)
	}
}

// Substitutes each byte according to Rijndael's inverse S-box
func subBytesInv(s []byte) {
	for i, v := range s {
		s[i] = getInvSboxSub(v)
	}
}

// Returns round constant
// Pre: 0 < round <= 10
func getRcon(round int) byte {
	return rcon_const[round-1]
}

// Expands key according to the Rijndael key schedule
// Pre: key is 16, 24 or 32 bytes in length
func expandKey(key []byte) []byte {
	nk := len(key) / 4
	xKey := make([]byte, 16*(rounds[len(key)]+1))

	// Beginning of the expanded key is the same
	i := 0
	for ; i < nk; i++ {
		xKey[(4*i)+0] = key[(4*i)+0]
		xKey[(4*i)+1] = key[(4*i)+1]
		xKey[(4*i)+2] = key[(4*i)+2]
		xKey[(4*i)+3] = key[(4*i)+3]
	}

	temp := make([]byte, 4)
	for ; i < len(xKey)/4; i++ {
		temp[0] = xKey[4*(i-1)+0]
		temp[1] = xKey[4*(i-1)+1]
		temp[2] = xKey[4*(i-1)+2]
		temp[3] = xKey[4*(i-1)+3]

		if i%nk == 0 {
			// Rotate word
			rotw(temp)
			// Sub word
			subBytes(temp)
			// Rcon
			temp[0] = temp[0] ^ getRcon(i/nk)
		} else if nk > 6 && i%nk == 4 {
			// Sub word
			subBytes(temp)
		}
		xKey[(4*i)+0] = xKey[4*(i-nk)+0] ^ temp[0]
		xKey[(4*i)+1] = xKey[4*(i-nk)+1] ^ temp[1]
		xKey[(4*i)+2] = xKey[4*(i-nk)+2] ^ temp[2]
		xKey[(4*i)+3] = xKey[4*(i-nk)+3] ^ temp[3]
	}
	return xKey
}

// Adds (xor) state to round key
func addRoundKey(s, w []byte) {
	for i, v := range s {
		s[i] = v ^ w[i]
	}
}

// Shifts rows to the right, by one more byte for each row
func shiftRows(s []byte) {
	t := make([]byte, 4)
	for i := 0; i < 4; i++ {
		t[0] = s[5*i]
		t[1] = s[((5*i)+4)%16]
		t[2] = s[((5*i)+8)%16]
		t[3] = s[((5*i)+12)%16]

		s[i] = t[0]
		s[i+4] = t[1]
		s[i+8] = t[2]
		s[i+12] = t[3]
	}
}

// Shifts rows to the left, by one more byte for each row
func shiftRowsInv(s []byte) {
	t := make([]byte, 4)
	for i := 0; i < 4; i++ {
		t[0] = s[(16-(3*i))%16]
		t[1] = s[(20-(3*i))%16]
		t[2] = s[(24-(3*i))%16]
		t[3] = s[12-(3*i)]

		s[i] = t[0]
		s[i+4] = t[1]
		s[i+8] = t[2]
		s[i+12] = t[3]
	}
}

// Galois Field (2^8) Multiplication of two Bytes
func gfMul(a, b byte) byte {
	var prod byte = 0
	var h byte
	for i := 0; i < 8; i++ {
		if (b & 0x01) != 0 {
			prod ^= a
		}
		h = a >> 7
		a <<= 1
		if h != 0 {
			a ^= 0x1B
		}
		b >>= 1
	}
	return prod
}

// Mix columns as coefficients of a polynomial over Rijndael's Galois field
func mixColumns(s []byte) {
	a := make([]byte, 4)
	for i := 0; i < 4; i++ {
		a[0] = s[(4*i)+0]
		a[1] = s[(4*i)+1]
		a[2] = s[(4*i)+2]
		a[3] = s[(4*i)+3]

		s[(4*i)+0] = gfMul(0x02, a[0]) ^ gfMul(0x03, a[1]) ^ a[2] ^ a[3] /* 2*a0 + 3*a1 + a2 + a3 */
		s[(4*i)+1] = gfMul(0x02, a[1]) ^ gfMul(0x03, a[2]) ^ a[3] ^ a[0] /* 2*a1 + 3*a2 + a3 + a0 */
		s[(4*i)+2] = gfMul(0x02, a[2]) ^ gfMul(0x03, a[3]) ^ a[0] ^ a[1] /* 2*a2 + 3*a3 + a0 + a1 */
		s[(4*i)+3] = gfMul(0x02, a[3]) ^ gfMul(0x03, a[0]) ^ a[1] ^ a[2] /* 2*a3 + 3*a0 + a1 + a2 */
	}
}

// Inverse of mix columns
func mixColumnsInv(s []byte) {
	a := make([]byte, 4)
	for i := 0; i < 4; i++ {
		a[0] = s[(4*i)+0]
		a[1] = s[(4*i)+1]
		a[2] = s[(4*i)+2]
		a[3] = s[(4*i)+3]

		s[(4*i)+0] = gfMul(0x0E, a[0]) ^ gfMul(0x0B, a[1]) ^ gfMul(0x0D, a[2]) ^ gfMul(0x09, a[3]) /* 14*a0 + 11*a1 + 13*a2 + 9*a3 */
		s[(4*i)+1] = gfMul(0x0E, a[1]) ^ gfMul(0x0B, a[2]) ^ gfMul(0x0D, a[3]) ^ gfMul(0x09, a[0]) /* 14*a1 + 11*a2 + 13*a3 + 9*a0 */
		s[(4*i)+2] = gfMul(0x0E, a[2]) ^ gfMul(0x0B, a[3]) ^ gfMul(0x0D, a[0]) ^ gfMul(0x09, a[1]) /* 14*a2 + 11*a3 + 13*a0 + 9*a1 */
		s[(4*i)+3] = gfMul(0x0E, a[3]) ^ gfMul(0x0B, a[0]) ^ gfMul(0x0D, a[1]) ^ gfMul(0x09, a[2]) /* 14*a3 + 11*a0 + 13*a1 + 9*a2 */
	}
}

// Encrypts one 16 byte block in place in memory
func (c *cypher) encryptBlock(s []byte) {
	addRoundKey(s, c.xkey)
	for i := 1; i < c.rounds; i++ {
		subBytes(s)
		shiftRows(s)
		mixColumns(s)
		addRoundKey(s, c.xkey[i*16:])
	}

	subBytes(s)
	shiftRows(s)
	addRoundKey(s, c.xkey[len(c.xkey)-16:])
}

// Decrypts one 16 byte block in place in memory
func (c *cypher) decryptBlock(s []byte) {
	addRoundKey(s, c.xkey[len(c.xkey)-16:])
	for i := 1; i < c.rounds; i++ {
		shiftRowsInv(s)
		subBytesInv(s)
		addRoundKey(s, c.xkey[len(c.xkey)-((i+1)*16):])
		mixColumnsInv(s)
	}

	shiftRowsInv(s)
	subBytesInv(s)
	addRoundKey(s, c.xkey)
}

// Returns pointer to new cypher
func NewCypher(key []byte) (*cypher, error) {
	// Check key length
	if len(key) != 16 && len(key) != 24 && len(key) != 32 {
		return nil, errors.New("incorrect key length, must be 16, 24 or 32 bytes")
	}

	return &cypher{key, expandKey(key), rounds[len(key)]}, nil
}

// Returns src encrypted using aes with key key
func (c *cypher) Encrypt(src []byte) []byte {
	// Calculate length of src with padding
	length := len(src)
	if length%16 != 0 {
		length = 16 * (len(src)/16 + 1)
	}
	b := make([]byte, length)
	copy(b, src)

	for i := 0; i < length/16; i++ {
		c.encryptBlock(b[16*i : 16*(i+1)])
	}
	return b
}

// Removes 0x00s at the end of b
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

// Returns src decrypted using aes with key key
// Will throw an error is len(src) isn't a multiple of 16
func (c *cypher) Decrypt(src []byte) ([]byte, error) {
	if len(src)%16 != 0 {
		return nil, errors.New("source is an incorrect length, must be a multiple of 16 bytes")
	}

	b := make([]byte, len(src))
	copy(b, src)

	for i := 0; i < len(b)/16; i++ {
		c.decryptBlock(b[16*i : 16*(i+1)])
	}
	return removePadding(b), nil
}