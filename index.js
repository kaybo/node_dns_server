var helperMethod = require('./helper.js');
var express = require('express');


var app = express();

app.get('/', (req, res) => {
   res.send('DNS server. Please use /finddomain/ to lookup a hostname');
   
});

app.get('/finddomain/:id', (req, res)=>{
    console.log(req.params.id);
    resIP = helperMethod.hostNameLookUp(req.params.id);
    res.send(resIP);
});

var server = app.listen(8081, function () {
   var host = server.address().address
   var port = server.address().port
   
   console.log('dns server is listening at http://%s:%s', host, port)
})