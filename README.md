# ultradb
Probably the fastest node database in the world, blazing simple, uses nodejs n-api interface, designed to work in node clusters. It is also very low level database, so inappropriate handling might lead to data corruption. Read documentation carefully before you use it. It also uses current NAPI interface, which isn't officialy tagged as stable.




## Load database library

```
const UltraDB = require('ultradb');
```


## Open database

```
const testDb = UltraDB('./myDatabase.udb');

```

This will open database. 


```
const test1Db = UltraDB('./myDatabase.udb, 2048');

```

This will open database with specified page size, default is 16384.


## Close database

```
testDb.close();
```

Database doesn't require to be closed so in most of cases you can skip this, however, if you're going to open dynamically a lot of databases it is good practice to close the ones you're not using any more.


## Add document (utf8 string with zero at its end)

```
const documentId = testDb.addUtf8z("My frirst ultradb document");
```

This will add new document to database, and if successfull, it will return its id;


## Get document (utf8 string with zero at its end)

```
const documentContent = testDb.getUtf8z(documentId);
```

This will retrieve document from database. 
If document is deleted it will return "null"
If document doesn't exist it wil return "undefined"

Note that "documentId" parameter is examined only briefly, so providing appropriate value is on your responsibility. Providing inappropriate value might lead to strange behaviour.
Note that it is your responsibility to use appropriate method to read appropriate data type, database doesn't encode document type.

## Database access synchronization

```
testDb.start();

//code that needs to be run exclusively on database so other processes have to wait

testDb.stop();
```

Method "start" informs that transaction has started, and method "stop" informs that transaction has ended.
Note that simple adding new documents to database and reading them is process safe, so there is no need to use transactions. Same if your node application is single process accessing particular database.


### Multiple databases synchronization

```
const testDb1 = UltraDb('./testdb1.udb');
const testDb2 = UltraDb('./testdb2.udb');
const testDb3 = UltraDb('./testdb3.udb');
const multiTransaction123 = UltraDB.prepareMultiTransaction(testDb1, testDb2, testDb3);

// ...

UltraDB.start(multiTransaction123);

//code that needs to be run exclusively on given set of databases

UltraDB.stop(multiTransaction123);
```

Using this methods prevents cross transaction lock while transaction on multiple databases.


## In next releases:
 - roll method, that will allow to start overwriting oldest documents, while keeping database size
 - methods to update, add, delete documents in 8, 16, 32, 64 bits signed and unsigned integers and UTF16


