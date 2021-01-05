const addon = require('./build/Release/test.node');
console.log('testing')
console.log('This should be eight:', addon.add('test arg1', 'test arg2'));