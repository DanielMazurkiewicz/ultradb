

const UltraDB = require('./index.js');

const ultradb = UltraDB('./test.txt', 2048);

console.log(ultradb);


for (let k = 0; k < 1000; k++) {
  var text = '';
  for (let i = 0; i < 2000; i++) {
    text += Math.random();
  }

  var id = ultradb.addUtf8(text);
  var result = ultradb.getUtf8(id); 


/*  console.log(text);
  console.log('-----');
  console.log(result);
*/
  console.log(text == result);
}

console.log(ultradb.close());
