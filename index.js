var path = require('path');
var crypto = require('crypto');
var ultradb = require('./build/Release/ultradb.node');
//var ultradb = require('./build/Debug/ultradb.node');
var onExit = require('./onExit');


var defaultPageSize = 16384;
var retries = 100;

var dbs = {};

onExit(function() {
  var path;
  for (path in dbs) {
    try {
      dbs[path]._close();
    } catch (error) {
      console.log(' === ERROR while closing: ' + path + ' ===');
      console.log(error);
    }
  }
});


function getDb(path, pageSize) {
  var db;
  for (var i = 0; i < retries; i++) {
    db = ultradb(path, pageSize, () => {
      return crypto.randomBytes(8);
    });
    if (db !== false) return db;
  }
  throw {from: 'ultradb', source: "CreateObject", sourceDetails: "getDb", message: "all retries failed", messageDetails: rootPath + ':' + path};
}


var UltraDB = function(dbPath, pageSize) {
  dbPath = path.resolve(dbPath);
  var db = dbs[dbPath];
  if (db) {
    db._connections++;
    return db;
  } 

  pageSize = pageSize || defaultPageSize;
  var db = getDb(dbPath, pageSize);
  db._connections = 1;
  db._path = dbPath;

  db.close = function() {
    if (db._connections == 1) {
      db._close();
      delete db._connections;
      delete db._close;
      delete db.close;
      delete dbs[dbPath];
    } else {
      db._connections--;
    }
  }

  db.default = function(dataFormat) {
    for (methodName in this) {
      if (methodName.includes(dataFormat)) {
        this[methodName.replace(dataFormat, '')] = this[methodName];
      }
    }
    return this;
  }

  dbs[dbPath] = db;
  return db;
}



UltraDB.prepareMultiTransaction = function() {
  return Array.prototype.slice.call(arguments).sort(function(a, b) {
    return a._key - b._key;
  });
}


UltraDB.start = function(multiTransaction) {
  var t = 0, l = multiTransaction.length;
  for (; t < l; t++) {
    multiTransaction[t].start();
  }
}

UltraDB.stop = function(multiTransaction) {
  var t = multiTransaction.length;
  for (; t >=0; --t) {
    multiTransaction[t].stop();
  }
}

module.exports = UltraDB;