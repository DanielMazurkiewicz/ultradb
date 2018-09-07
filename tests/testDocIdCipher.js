const crypto = require('crypto');

function DocIdCipher(key, bitSize = 128) {
  var cipher, decipher, aes = 'aes' + bitSize;

  this.generateKey = () => {
    key = crypto.randomBytes(16);
    cipher = crypto.createCipher(aes, key);
    decipher = crypto.createDecipher(aes, key);

    cipher.setAutoPadding(false);
    decipher.setAutoPadding(false);
    return key;
  }

  this.docIdEncrypt = (documentId, salt = 0) => {
    const documentIdAsTypedArray = new Float64Array([documentId, salt]);
    return cipher.update(documentIdAsTypedArray);
  }

  this.docIdDecrypt = (documentIdEncrypted, withSalt) => {
    const decrypted = decipher.update(documentIdEncrypted);
    if (withSalt) return new Float64Array(decrypted.buffer);
    return (new Float64Array(decrypted.buffer))[0];
  }

  this.key = () => key;

  if (!key) {
    this.generateKey();
  }
}


cipher = new DocIdCipher(null, 128);

var docId = 1;
var encrypted = cipher.docIdEncrypt(docId);
var decrypted = cipher.docIdDecrypt(encrypted);

console.log(encrypted.length);
console.log(docId, decrypted);