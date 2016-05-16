var serialport = require('serialport');// include the library
var SerialPort = serialport.SerialPort; // make a local instance of it
var myPort = new SerialPort("/dev/ttyUSB0", { baudRate: 38400,
// look for return and newline at the end of each data packet:
parser: serialport.parsers.readline("\n")});

var express = require('express');
var app = express();
var http = require('http').Server(app);
var path = require('path');

myPort.on('data', sendSerialData);

function sendSerialData(data) {
	   console.log(data);
}

app.use(express.static(path.join(__dirname, 'stream')));
app.get('/',function(req,res,next){
res.sendFile(__dirname + '/index.html');
	});

app.post('/start/:id',function(req,res,next){
	console.log("Command Received");
	console.log(req.params.id);
	myPort.write(req.params.id);
	res.send();
	});

http.listen(3000, function() {
	console.log('listening on *:3000');
});



