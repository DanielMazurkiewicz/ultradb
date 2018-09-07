const UltraDB = require('../index.js');
const testDb = UltraDB('./dbTestDocIdObsfucation.udb');

const crypto = require('crypto');
const randomData = crypto.randomBytes(64); // you must supply minimum 64 random bytes to generate key

var key = testDb.docIdGenerateKey(8, 20, randomData); // 8 bits, 20 rounds key

for (var documentId = 0; documentId < 257; documentId++) {
  var encrypted = testDb.docIdEncrypt(documentId, key);
  var decrypted = testDb.docIdDecrypt(encrypted, key);
  if (documentId !== decrypted) {
    console.log("out of ciphering bitspace:"); // should appear only when docId is above 255 since we use 8 bit docId's
  }

  console.log('documentId:', documentId, 'encrypted:', encrypted, 'decrypted:', decrypted)
}


// in case you need safe ciphering consider "testDocIdCipher.js" as an example