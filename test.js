

const UltraDB = require('./index.js');

const ultradb = UltraDB('./test.txt', 2048);

console.log(ultradb);

console.log(ultradb.close());
