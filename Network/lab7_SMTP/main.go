package main

import (
	"crypto/tls"
	"fmt"
	"os"
	"sort"
	"strings"

	gomail "gopkg.in/gomail.v2"
)

func main() {
	messages := make([]string, 0)
	keyword := os.Args[4]
	for i := 1; i < 4; i++ {
		if strings.Contains(os.Args[i], keyword) {
			messages = append(messages, os.Args[i])
		}
	}

	if len(messages) == 0 {
		fmt.Println("There are no text with given keyword")
		return
	}
	sort.Slice(messages, func(i, j int) bool { return strings.Count(messages[i], keyword) > strings.Count(messages[j], keyword) })

	body := ""

	for _, mess := range messages {
		body += mess + "\n"
	}

	m := gomail.NewMessage()
	m.SetHeader("From", "tantsevov@yandex.ru")
	m.SetHeader("To", "tantsevov@gmail.com")
	m.SetHeader("Subject", "test!")
	m.SetBody("text/html", body)

	d := gomail.NewDialer("smtp.yandex.ru", 465, "tantsevov", Password)
	d.TLSConfig = &tls.Config{InsecureSkipVerify: true}

	if err := d.DialAndSend(m); err != nil {
		panic(err)
	}
}
