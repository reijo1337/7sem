var fs = require('fs');
var http = require('http');
var fileName = "/static/html/index.html";

http.createServer(
	function(req, res) {
		fs.readFile(__dirname + fileName, 'utf8',
			function (err, data) {
				if (err) {
					console.log('Could not find or open file for reading\n');
				} else {
					res.writeHead(200, {'Content-Type': 'text/html'});
					res.end(data);
				}
			});
	}).listen(8080);

console.log('Server running...');