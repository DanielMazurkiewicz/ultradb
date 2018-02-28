const cluster = require('cluster');
const numCPUs = require('os').cpus().length;


if (cluster.isMaster) {
  console.log(`Master ${process.pid} is running`);

  // Fork workers.
  for (let i = 0; i < numCPUs; i++) {
    cluster.fork();
  }

  cluster.on('exit', (worker, code, signal) => {
    console.log(`Worker ${worker.process.pid} died`);
  });
} else {
  console.log(`Worker ${process.pid} started`);

  const UltraDB = require('../index.js');
  const testDb = UltraDB('./dbTestClusterUtf8z.txt');


  for (let k = 0; k < 10000; k++) {
    var text = `PID: ${process.pid} K: ${k}`;

    var id = testDb.addUtf8z(text);
    var result = testDb.getUtf8z(id);
    if (text !== result) {
      console.log('=================');
      console.log('!!Test did not pass!!! Id:' + id);
      console.log('----Expected-----');
      console.log(text);
      console.log('----Got----------');
      console.log(result);
    }
  }

}


