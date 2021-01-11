var Mutex = require('async-mutex').Mutex;
var helperMethod = require('./helper.js');
var express = require('express');
var app = express();

var cache = []; //used for storing domain name and ip address
const mutex = new Mutex();

app.get('/', (req, res) => {
   res.send('DNS server. Please use /finddomain/ to lookup a hostname');
   
});

app.get('/finddomain/:id', (req, res)=>{
    var returnIPAddress = '';
    mutex.acquire().then((release) => {
        cache.forEach(obj => {
            if(String(req.params.id.trim()) === String(obj.name.trim())){
                returnIPAddress = obj.data;
            }
        });

        if(returnIPAddress !== ''){
            res.send('Found in cache: ' + returnIPAddress);
        }else{
            resObj = helperMethod.hostNameLookUp(req.params.id);
                //todo: fix here
            cache.push(resObj);
            res.send('Not found in cache: ' + resObj.data);
        }
        release();
    });
});

var server = app.listen(8081, function () {
   var host = server.address().address
   var port = server.address().port
   
   console.log('dns server is listening at http://%s:%s', host, port);
});

const cacheTimer = () =>{
    mutex.acquire().then((release) => {
        // todo: add more here
        if(cache.length > 0){
            cache = cache.map(obj =>{
                let updatedObj = obj;
                updatedObj.ttl = updatedObj.ttl - 5;
                return updatedObj;
            }).filter(obj =>{
                if(obj.ttl > 0){
                    return obj;
                }
            });
        }
        console.log('Inside Cache: ' + JSON.stringify(cache));
        release();
    });
    
};

setInterval(cacheTimer, 5000);