const UltraDB = require('../index.js');
const testDb = UltraDB('./dbTestCursor.udb').default('Utf8z');

//Feed database with data if it is fresh made
if (testDb.isEmpty()) {
  console.log(' ==== initial data feeding ====')
  var docId;
  for (let k = 0; k < 10; k++) {
    docId = testDb.add(`K: ${k}`);
  }
  testDb.hide(docId); // hide last document
}

//Go through all documents, except hidden ones
console.log(' ==== last - previous ====');
for (let id = testDb.last(); id != undefined; id = testDb.previous(id)) {
  console.log(testDb.get(id));
}

//Go through all documents, also hidden ones
console.log(' ==== lastOfAll - previousOfAll ====');
for (let id = testDb.lastOfAll(); id != undefined; id = testDb.previousOfAll(id)) {
  console.log(testDb.get(id)); //reading hidden one should return "null"
}
