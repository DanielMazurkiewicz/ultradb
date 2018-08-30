const UltraDB = require('../index.js');
const testDb = UltraDB('./dbTestMisc.udb');
console.log(testDb._path);

var docInfo = testDb.docAdd(1000);
console.log(docInfo);
console.log(testDb.docGetId(docInfo));

testDb.partSetUtf8z(docInfo, 'test text', 30, 30);
console.log(testDb.partGetUtf8z(docInfo, 30));

testDb.partSetF64(docInfo, 1.23, 20);
console.log(testDb.partGetF64(docInfo, 20));


testDb.docIdCheckSet32(docInfo, 0);
console.log(testDb.docIdVerify32(docInfo, 0));
console.log(testDb.docIdVerify32(docInfo, 1));




var id = testDb.addF64(8.12);
console.log(testDb.getF64(id));

var id = testDb.addU64(9.12);
console.log(testDb.getU64(id));

testDb.setF64(id, 19.12);
console.log(testDb.getF64(id));


testDb.setF64At(id, 9.12, 0);
console.log(testDb.getF64At(id, 0));
console.log(testDb.getF64At(id, 1));

var id = testDb.addF64At(4.12, 10);
console.log(testDb.getF64At(id, 10));

var id = testDb.addF32At(5.12, 10);
console.log(testDb.getF32At(id, 10));


var id = testDb.addU8At(5.12, 10);
console.log(testDb.getU8At(id, 10));

var id = testDb.addU8At(257.12, 10);
console.log(testDb.getU8At(id, 10));

var id = testDb.addS8At(-34, 10);
console.log(testDb.getS8At(id, 10));

var id = testDb.addU24At(16000000, 10);
console.log(testDb.getU24At(id, 10));


var id = testDb.addS24At(-8000000, 10);
console.log(testDb.getS24At(id, 10));

var id = testDb.addS24At(8000000, 10);
console.log(testDb.getS24At(id, 10));


var id = testDb.addU48At(20000000000, 10);
console.log(testDb.getU48At(id, 10));

var id = testDb.addS48At(-200000000010, 10);
console.log(testDb.getS48At(id, 10));

var id = testDb.addS48At(200000000010, 10);
console.log(testDb.getS48At(id, 10));


const crypto = require('crypto');
const randomData = crypto.randomBytes(10);
console.log(randomData);
var id = testDb.addBufferAt(randomData, 8, 10);
console.log(testDb.getBufferAt(id, 8, 10));
console.log(testDb.getBuffer(id, -1));

