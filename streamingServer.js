var express = require('express');
var app = express();
var http = require('http').Server(app);
var io = require('socket.io')(http);
var fs = require('fs');
var path = require('path');
var SerialPort = require('serialport');
var spawn = require('child_process').spawn;
var proc;
var timeout;
var forward = '1', reverse = '2', left = '3', right = '4', stop = '5';
 
var port = new SerialPort('/dev/ttyUSB0', {
  baudRate: 9600,
  autoOpen: false,
});

//******************************************* serial port to arduino **********************************
port.open((error) => {
  if (error) {
    console.log('error opening port', error.message);
  }
  else {
    console.log('opened serial port to arduino');
  }
});

app.use('/', express.static(path.join(__dirname, 'stream')));
 
//****************************************** default web page *****************************************
app.get('/', function(req, res) {
  console.log('get index');
  res.sendFile(__dirname + '/index.html');
});
 
//*************************************** socket control **********************************************
var sockets = {};
io.on('connection', function(socket) {
 
  sockets[socket.id] = socket;
  console.log("Total clients connected : ", Object.keys(sockets).length);
 
  socket.on('disconnect', function() {
    delete sockets[socket.id];
  });

  socket.on('left', function() { 
    console.log('left');
    port.write(left);
  });

  socket.on('right', function() {
    console.log('right');
    port.write(right);
  });

  socket.on('forward', function() { 
    console.log('forward');
    port.write(forward);
  });

  socket.on('reverse', function() {
    console.log('reverse');
    port.write(reverse);
  });

  socket.on('stop', function() {
    console.log('stop');
    port.write(stop);
  });
});
 
http.listen(3000, function() {
  console.log('listening on *:3000');
});