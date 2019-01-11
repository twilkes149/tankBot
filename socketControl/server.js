var express = require('express');
var app = express();
var http = require('http').Server(app);
var io = require('socket.io')(http);

app.use('/', express.static(path.join(__dirname, 'stream')));

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
  });

  socket.on('right', function() {
    console.log('right');    
  });

  socket.on('forward', function() { 
    console.log('forward');    
  });

  socket.on('reverse', function() {
    console.log('reverse');    
  });

  socket.on('stop', function() {
    console.log('stop');    
  });
});
 
http.listen(3000, function() {
  console.log('listening on *:3000');
});