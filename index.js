const addon = require('./build/Release/test.node');
console.log('testing')
console.log('This should be eight:', addon.add('google.com', '198.41.0.4'));