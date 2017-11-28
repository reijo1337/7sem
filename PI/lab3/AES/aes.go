package main

import "errors"

type cypher struct {
	key    []byte
	xkey   []byte
	rounds int
}

func rotw(w []byte) {
	temp := w[0]
	w[0] = w[1]
	w[1] = w[2]
	w[2] = w[3]
	w[3] = temp
}

func getSboxSub(b byte) byte {
	return sbox[b]
}

func getInvSboxSub(b byte) byte {
	return rsbox[b]
}

// Подстановка байтов из таблицы
func subBytes(s []byte) {
	for i, v := range s {
		s[i] = getSboxSub(v)
	}
}

func subBytesInv(s []byte) {
	for i, v := range s {
		s[i] = getInvSboxSub(v)
	}
}

func getRcon(round int) byte {
	return rcon_const[round-1]
}

// Вычисление раундовых ключей (расширение ключа)
func expandKey(key []byte) []byte {
	nk := len(key) / 4
	xKey := make([]byte, 16*(rounds[len(key)]+1))

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
			rotw(temp)
			subBytes(temp)
			temp[0] = temp[0] ^ getRcon(i/nk)
		} else if nk == 8 && i%nk == 4 {
			subBytes(temp)
		}
		xKey[(4*i)+0] = xKey[4*(i-nk)+0] ^ temp[0]
		xKey[(4*i)+1] = xKey[4*(i-nk)+1] ^ temp[1]
		xKey[(4*i)+2] = xKey[4*(i-nk)+2] ^ temp[2]
		xKey[(4*i)+3] = xKey[4*(i-nk)+3] ^ temp[3]
	}
	return xKey
}

// добавляет (xor) к состоянию ключ текущего раунда
func addRoundKey(s, w []byte) {
	for i, v := range s {
		s[i] = v ^ w[i]
	}
}

// Сдвиг строки
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

// Умножение двух байтов в поле GF(2^8)
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

// Умножение столбцов
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

// Инверсированное умножение столбцов
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

// Зашифровка блока
func (c *cypher) encryptBlock(s []byte) {
	addRoundKey(s, c.xkey)							// добавление ключа к блоку
	for i := 1; i < c.rounds; i++ {
		subBytes(s)									// Замена байтов по таблице
		shiftRows(s)								// Cвдиг строк
		mixColumns(s)								// Умножение столбцов
		addRoundKey(s, c.xkey[i*16:])
	}

	subBytes(s)
	shiftRows(s)
	addRoundKey(s, c.xkey[len(c.xkey)-16:])
}

// Расшифровка блока
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

// Создание нового шифровщика
func NewCypher(key []byte) (*cypher, error) {
	if len(key) != 16 && len(key) != 24 && len(key) != 32 {
		return nil, errors.New("incorrect key length, must be 16, 24 or 32 bytes")
	}

	return &cypher{key, expandKey(key), rounds[len(key)]}, nil
}

// Шифровка слайса байтов
func (c *cypher) Encrypt(src []byte) []byte {
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

// Убирает нули
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

// Дешифровка сообщений
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