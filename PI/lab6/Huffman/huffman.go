package main

import (
	"bytes"
	"container/heap"
	"strconv"
)

type Huffman struct {
	count map[byte]int
	pq    PriorityQueue
	tree  *Node
}

func NewHuffman(data []byte) *Huffman {
	count := make(map[byte]int)
	for _, b := range data {
		_, ok := count[b]
		if ok {
			count[b]++
		} else {
			count[b] = 1
		}
	}
	pq := make(PriorityQueue, len(count))
	i := 0
	for value, priority := range count {
		valNode := &Node{
			left:  nil,
			right: nil,
			val:   value,
			count: priority,
		}
		pq[i] = &Item{
			value:    valNode,
			priority: priority,
			index:    i,
		}
		i++
	}
	return &Huffman{count, pq, nil}
}

type Node struct {
	left  *Node
	right *Node
	val   byte
	count int
	code  string
}

func (n *Node) buildCode(code string) {
	n.code = code
	if n.left != nil {
		n.left.buildCode(code + "0")
	}
	if n.right != nil {
		n.right.buildCode(code + "1")
	}
}

func (h *Huffman) makeTree() *Node {
	var ret *Node
	heap.Init(&h.pq)
	for h.pq.Len() > 0 {
		left := heap.Pop(&h.pq).(*Item)
		right := heap.Pop(&h.pq).(*Item)

		newNode := &Node{
			left:  left.value,
			right: right.value,
			val:   0,
			count: right.priority + left.priority,
		}

		if h.pq.Len() > 0 {
			newItem := &Item{
				value:    newNode,
				priority: newNode.count,
				index:    h.pq.Len(),
			}
			heap.Push(&h.pq, newItem)
		} else {
			ret = newNode
		}
	}
	return ret
}

func (h *Huffman) compress(data []byte) []byte {
	root := h.makeTree()
	root.buildCode("")
	h.tree = root
	codeMap := make(map[byte]string)
	h.makeCodes(root, &codeMap)
	str := ""
	for _, b := range data {
		str += codeMap[b]
	}

	byteString := make([]string, 0)
	i := 0
	for ; i < len(str)/8; i++ {
		byteString = append(byteString, str[i*8:(i+1)*8])
	}
	byteString = append(byteString, str[i*8:])

	ret := make([]byte, 0)

	for _, v := range byteString {
		s, err := strconv.ParseUint(v, 2, 8)
		if err != nil {
			panic(err)
		}
		ret = append(ret, byte(s))
	}

	return ret
}

func (h *Huffman) decompress(data []byte) []byte {
	node := h.tree
	ret := make([]byte, 0)
	r := New(bytes.NewBuffer(data))

	for i := 0; i < len(data); i++ {
		for j := 0; j < 8; j++ {
			bit, _ := r.ReadBit()
			if bit {
				node = node.right
			} else {
				node = node.left
			}

			if node.left == nil && node.right == nil {
				ret = append(ret, node.val)
				node = h.tree
			}
		}
	}

	return ret
}

func (h *Huffman) makeCodes(node *Node, codeMap *map[byte]string) {
	if node.left == nil && node.right == nil {
		(*codeMap)[node.val] = node.code
	} else {
		if node.left != nil {
			h.makeCodes(node.left, codeMap)
		}
		if node.right != nil {
			h.makeCodes(node.right, codeMap)
		}
	}
}
