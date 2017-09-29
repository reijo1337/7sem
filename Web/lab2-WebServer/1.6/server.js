var express = require('express');
var fs = require('fs');
var path = require('path');
var bodyParser = require('body-parser')
var app = express();

app.use(express.static(path.join(__dirname,'static')));
app.set('views', __dirname + '/static/html');
app.engine('html', require('ejs').renderFile);
app.set('view engine', 'html');

app.use(bodyParser.json());

app.get('/hack', 
	function (req, res) {
		res.render('index')
	});

app.get('/', 
	function (req, res) {
		res.sendFile(__dirname + '/static/html/hack.txt')
	});

app.get('/normalhack',
	function(req, res) {
		fs.readFile(__dirname + '/static/html/hack.txt', 'utf8',
			function(err, data) {
				if (err) {
					console.log('Could not find or open file for reading\n');
				} else {
					res.writeHead(200, {'Content-Type': 'text/html'});
					res.end(data);
				}
			});
	});

app.get('/img',
	function(req, res) {
		res.sendFile(__dirname + '/static/img/img.jpg')
	});

app.post('/file',
	function(req, res) {
		fs.writeFile(__dirname + '/static/file.txt', 'Hello Node.js', (err) => {
			if (err) throw err;
		});
		res.send("File was created or rewrited");
	});

app.get('/file',
	function(req, res) {
		res.sendFile(__dirname + '/static/html/file.txt')
	});

var server = app.listen(8080, 
	function () {
		var host = server.address().address
		var port = server.address().port
		console.log('Example app listening at http://%s:%s', host, port)
});