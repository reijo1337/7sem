var http = require('http');

http.createServer(
	function (req, res) {
		res.writeHead(200, {'Content-Type': 'text-html'});
		res.end('<h1>Hello World</h1>');
	}).listen(8080);

console.log('Server running...');