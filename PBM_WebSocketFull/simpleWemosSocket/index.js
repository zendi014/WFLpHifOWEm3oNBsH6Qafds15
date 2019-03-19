var express = require('express');
var app = express();
var bodyParser = require('body-parser');
var path = require('path');

var PORT = process.env.PORT || 2208;


var http = require('http');
// var WebSocket = require('ws');
var server = http.createServer(app);
const  io = require('socket.io')(server);

var ip = require("ip");



app.use('/static',express.static(path.join(__dirname, 'public')))
  .set('views', path.join(__dirname, 'views'))
  .engine('html', require('ejs').renderFile)
  .set('view engine', 'html');

app.use(bodyParser.json());










io.sockets.on('connection', function (socket) {

    socket.on('connection', function(data){
        io.sockets.emit("message", data);//push data to webapp
    });


    socket.on('web_app_soi', function(res_data){
        io.sockets.emit("web_app_soi", res_data);
    });


    socket.on('res_web_app_soi', function(res_data){
        io.sockets.emit('res_web_app_soi', res_data);
    });

    socket.emit('message', 'hello from SOCKET server ');

    socket.on('disconnect', function(data){
        console.log(data);
    });



    //1 listen dr cleint web
    socket.on('web_emit_to_srv', function(data){//listen data from webapp
        console.log(data);
        io.sockets.emit("srv_emit_to_wemos", data);//push data to wemos
    });

    socket.on('wemos_emit_to_srv', function(data){//listen data from wemos
        io.sockets.emit("srv_emit_to_web", data);//push data to webapp
    });


});










// app.get('/bootstrap_test', function(req, res){
//     dt = {
//             title : "Realtime PSO-IK of Humanoid 10 DoF Simulator",
//             name: 'Integrated Socket.io based Three.js, Node.js and Express.js',
//             content: "Klik The Robot Body Part in The Left Side Bar <br/> Hold (SPACE) for Play and Stop The Animation"
//     }
//     res.render('bootstrap_test', dt);
// });

app.get('/', function (req, res) {
    res.render('index', { name: 'Integrated Socket.io, Node.js and Express.js'});
});


server.listen(PORT, function(){
  console.log('server run on '+ip.address()+":"+server.address().port);//+" "+__dirname);
});
