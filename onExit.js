// https://stackoverflow.com/questions/14031763/doing-a-cleanup-action-just-before-node-js-exits

module.exports = function (callback) {

  // attach user callback to the process event emitter
  process.on('cleanup',callback);

  // do app specific cleaning before exiting
  process.on('exit', function () {
    process.emit('cleanup');
  });

  // catch ctrl+c event and exit normally
  process.on('SIGINT', function () {
    process.exit(2);
  });

  //catch uncaught exceptions, trace, then exit normally
  process.on('uncaughtException', function(e) {
    console.log('========================================');
    console.log('Uncaught Exception...');
    console.log(e.stack);
    console.log('========================================');
    process.exit(99);
  });

};