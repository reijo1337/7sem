package main

import (
	"crypto/tls"

	gomail "gopkg.in/gomail.v2"
)

func main() {
	m := gomail.NewMessage()
	m.SetHeader("From", "tantsevov@yandex.ru")
	m.SetHeader("To", "tantsevov@gmail.com")
	m.SetHeader("Subject", "Hello!")
	m.SetBody("text/html", "Hello World of smtp!")

	d := gomail.NewDialer("smtp.yandex.ru", 465, "tantsevov", Password)
	d.TLSConfig = &tls.Config{InsecureSkipVerify: true}

	// Send the email to Bob, Cora and Dan.
	if err := d.DialAndSend(m); err != nil {
		panic(err)
	}
}
