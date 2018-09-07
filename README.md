# UltraDb
Probably the fastest node database in the world, blazing simple, uses nodejs n-api interface, designed to work in node clusters. It is also very low level database, so inappropriate handling might lead to data corruption. Read documentation carefully before you use it. 

Works on 64 bit linux platforms. Other platforms not tested.


# What for is this another database?

Some examples of usage:
 * logging data
 * storing sessions, large numger of documents
 * building custom database data structures (trees, chain lists, ...)
 * microservices
 * services
 * storing training data of ai, ann, ... storing trained results...
 * building more advanced databases with ultradb as their backend
 * everywhere where you need document ID without additional querying for it (you receive document ID always as a result of adding document to database)

It:
 * is small - arround 300kB
 * is written in C for node js intentionally - it is not a wrapper, or port of another database
 * is multicore processors friendly - scales up with node processes, multiple node instances/forks can work simultaneously on same database file
 * is pretty simple and straight forward - not intended to hide any kind of abstraction from you
 * supports text, integers, floats and data buffers
 * supports cursors that allows to iterate through documents starting from newest and allows paging
 * is VERY FAST, try it on SSD drive, compare with other databases, tell me the results - I'm sure you'll be pleased when you'll compare it with other databases for node js
 * is Binary JSON (BSON) and other binary formats friendly

If performance and/or costs of your infrastructure is for you a key factor you should consider use of Ultra DB. It is simple to use library, but requires little more attention from developer and more extensive testing, since there is not much control over what you do with database (for example there is no guarantee that database will trigger an error when trying to update document with non existing ID within a range of IDs in database, more over it might corrupt database). But in return you get a database, that runs within node process (no extra process, no extra overheads for TCP/IP stack, no waisteless copying data, no parsing queries, no switching to additional db tasks, no callbacks and promisses overheads ... and so on). With very little effort you can make pretty advanced services serving many thousands requests a second on a single cheap VPS machine.

# Help highly appreciated! :-)

Feel free to drop a line of code, write tests, prepare documentation, tutorials, guidelines, come up with ideas and so on!

# Important notes

It is unsafe to allow access to documents by their id from any kind of public api (unless all documents are fixed size and you control modulo of document ID). In case of public API additional step of document id validation should be taken (combining document ID and random key saved in the document as public document ID, accessing via another fixed size documents lookup database, hashing id or hasing id combined with kind of checksum ... and so on). Current API profides set of document ID checksum functions (8 - 32 bit) and unique key for checksums for each database file - this will be helpfull to prevent accessing non existing documents.

If document id's are used internally only and you have full control of how are they used, then you don't have to worry about above.

If you need full ACID database you have to design data and recovery mechanisms yourself. But if you only add new records and read them, you're safe, you might consider this database kinda of ACID in that case.

If you're going to use db in nodejs cluster make sure you use transactions where it needs to be synchronized across processes.

Since transactions are blocking other processes that want to use particular database file/files it is a good idea to split data to databases for different document types each.

Newly added documents are hidden till their content is filled into a database.

64 bit integers currently are actually limited to 53 bits. This is due javascript limitation for numbers, not database itself.

Before you start, check examples located in "tests" directory on github repo:
https://github.com/DanielMazurkiewicz/ultradb/tree/master/tests

# Database file structure

```
<header> <document1 data> <document1 length> ... <documentN data> <documentN length>
```

Document ID points indirectly to address of most significant byte of document length. Most significant byte contains additional 3 bit information:
 * most significant bit if set to 1 means that document is hidden
 * second and third most significant bit encodes document length field size:
    - 00 : 1 byte
    - 01 : 2 bytes
    - 10 : 4 bytes
    - 11 : 8 bytes

Since Document ID is indirect address it is technically possible to remove phisically documents from the beginning of database while keeping same Document IDs.


# Usage

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

This will open database with specified page size, default is 16384. Page size has an impact on performance, the bigger the better performance. The best if it is multiplication of expected average document size.


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

If document is hidden it will return "null"

If document doesn't exist it will return "undefined"

Note that "documentId" parameter is examined only briefly, so providing appropriate value is on your responsibility. Providing inappropriate value might lead to strange behaviour.

Note that it is your responsibility to use appropriate method to read appropriate data type, database doesn't encode document type.


## Default documents data format

```
const testDb = UltraDB('./dbTestCursor.udb').default('Utf8z');
```

Adds shorten names for methods containing given default string. For above example:
```
const documentId = testDb.add('some document');
```
will be equivalent to
```
const documentId = testDb.addUtf8z('some document');
```

Method returns current database instance


## Checking if database contains documents

```
testDb.isEmpty();
```
Returns true if database is empty


## Getting lattest added document identifier

```
const documentId = testDb.last();
```
It returns "undefined" if there is no documents in database.
This method skips all hidden documents. If you need to get latest document, including hidden ones use:
```
const documentId = testDb.lastOfAll();
```


## Getting previously added document identifier

```
const previousDocumentId = testDb.previous(documentId);
```
It returns "undefined" if there is no previous document to given one
This method skips all hidden documents. If you need to get previous document, including hidden ones use:
```
const previousDocumentId = testDb.previousOfAll(documentId);
```

Example: Iterate through all documents
```
for (let id = testDb.last(); id != undefined; id = testDb.previous(id)) {
  console.log(testDb.get(id));
}
```

Note that "documentId" parameter is examined only briefly, so providing appropriate value is on your responsibility. Providing inappropriate value might lead to strange behaviour.

## Rolling database

```
testDb.roll();
```

Writes new documents from begining of database file (with a guarantee of assigning non existing yet new document id's for new documents). It replaces oldest added documents in database.


## Hiding document

```
testDb.hide(documentId);
```

Note that "documentId" parameter is examined only briefly, so providing appropriate value is on your responsibility. Providing inappropriate value might lead to strange behaviour or database corruption.


## Unhiding document

```
testDb.unhide(documentId);
```

Note that "documentId" parameter is examined only briefly, so providing appropriate value is on your responsibility. Providing inappropriate value might lead to strange behaviour or database corruption.


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

# Full API list

(methods with skipped description simply have same functionality as previously described, but on different size of data)

## Document
### docAddHidden(documentSize)
creates empty hidden document and returns documentInfo reference

### docAdd(documentSize)
creates empty document and returns documentInfo reference

### docGet(documentId)
returns documentInfo reference for given documentId

### docGetHiddenAndVisible(documentId)
returns documentInfo reference for given documentId for both, visible and hidden documents

### docGetId(documentInfo)
returns id of document based on given documentInfo reference


## Document ID obsfucation
### docIdGenerateKey(bits, rounds, randomData)
creates key which is a Buffer type containing instructions for encrypting/decrypting documentID, bits defines expected maximum size of documentID (6-64 bits), rounds defines ciphering complexity - the higher number the harder to crack (0 gives the weakest ciphering), randomData - at least 64 bytes of random buffer data has to be provided in order to be able to generate key - the more rounds the more data should be provided (approximatly each round should have additional 24 bytes of random data)

### docIdEncrypt(documentId, key)
encrypts given documentId with given key, returns Number which is a ciphered representation of documentId

### docIdDecrypt(encryptedDocumentId, key)
decrypts given encryptedDocumentID with given key, returns Number which is decrypted documentID


## Database info and management
### isEmpty()
returns true if database is empty

### roll()
rolls database, new documents will replace oldest documents in database

### setRoot(documentId)
allows to set top level document if someone need a document with some kind of settings, or keeping any kind of structures where entry document is required

### getRoot()
retrieves id of current root document



## Document ID operations - 32 bit
### docIdVerify32(documentInfo, offset_within_document)
validates 32 bit checksum of document ID, provided indirectly by documentInfo, at given position

### docIdCheckSet32(documentInfo, offset_within_document)
sets 32 bit checksum for document provided by documentInfo reference, checksum is set at given position, but it is recommended for checksum not to be last element in the document.

## Document ID operations - 16 bit
### docIdVerify16(documentInfo, offset_within_document)
### docIdCheckSet16(documentInfo, offset_within_document)



## Document ID operations - 8 bit
### docIdVerify8(documentInfo, offset_within_document)
### docIdCheckSet8(documentInfo, offset_within_document)



## Data operations - buffer in Buffer format
### addBuffer(buffer, length)
adds new document with given buffer, returns id of created document, if length is -1 adds entire buffer, otherwise adds begining number of bytes from buffer and if length is bigger than buffer length, rest of space is left empty

### addBufferFixed(buffer, length, length_of_document)
adds new document with given buffer, document is fixed length, remaining space after buffer is empty, writes only as many bytes as provided in "length" property (if -1 writes all of them, if bigger then length of buffer, then writes as much as available and rest remains empty), returns id of created document

### getBuffer(documentId, length)
returns to buffer "length" number of bytes of document with given id, if length is -1 then reads all of document content

### setBuffer(documentId, buffer, length)
sets/replaces buffer of document with given id, length points how many bytes to write (if -1 writes all of them, if bigger then length of buffer, then writes as much as available and rest remains unchanged)

### addBufferAt(buffer, length, offset_within_document)
creates document with buffer and leaves begining of document empty, returns newly created document id, length points how many bytes to write (if -1 writes all of them, if bigger then length of buffer, then writes as much as available and rest remains empty)

### addBufferFixedAt(buffer, length, length_of_document, offset_within_document)
creates document with buffer placed in the middle of document, document is fixed size, returns newly created document id, length points how many bytes to write (if -1 writes all of them, if bigger then length of buffer, then writes as much as available and rest remains empty)

### getBufferAt(documentId, length, offset_within_document)
returns buffer of document with given id at given position, if length is -1 then reads all content from given offset position

### setBufferAt(documentId, buffer, length, offset_within_document)
sets/replaces buffer placed in the middle of document, length points how many bytes to write (if -1 writes all of them, if bigger then length of buffer, then writes as much as available and rest remains unchanged)

### partGetBuffer(documentInfo, length, offset_within_document)
returns buffer located at given position in document based on given documentInfo (see "Document API"), if length is -1 then reads all content from given offset position

### partSetBuffer(documentInfo, buffer, length, offset_within_document)
sets/replaces buffer located at given position in document based on given documentInfo, length points how many bytes to write (if -1 writes all of them, if bigger then length of buffer, then writes as much as available and rest remains unchanged)



## Data operations - text in Utf8z format
### addUtf8z(text)
adds new document with given text, returns id of created document

### addUtf8zFixed(text, text_length_max, length_of_document)
adds new document with given text, checks if it fits given maximum length fot text in utf8 format, document is fixed length, remaining space after text is empty. returns id of created document

### getUtf8z(documentId)
returns text of document with given id

### setUtf8z(documentId, text)
sets/replaces text of document with given id

### addUtf8zAt(text, offset_within_document)
creates document with text and leaves begining of document empty, returns newly created document id

### addUtf8zFixedAt(text, text_length_max, length_of_document, offset_within_document)
creates document with text placed in the middle of document, document is fixed size, returns newly created document id

### getUtf8zAt(documentId, offset_within_document)
returns text of document with given id at given position

### setUtf8zAt(documentId, text, text_length_max, offset_within_document)
sets/replaces text placed in the middle of document

### partGetUtf8z(documentInfo, offset_within_document)
returns text located at given position in document based on given documentInfo (see "Document API")

### partSetUtf8z(documentInfo, text, text_length_max, offset_within_document)
sets/replaces text located at given position in document based on given documentInfo (see "Document API"), validates if it fits maximum length



## Data operations - 64 bit float numbers
### addF64(number)
adds new document with given number, returns id of created document

### addF64Fixed(number, length_of_document)
adds new document with given number, document is fixed length, remaining space after number is empty. returns id of created document

### getF64(documentId)
returns number of document with given id

### setF64(documentId, number)
sets/replaces number of document with given id

### addF64At(number, offset_within_document)
creates document with number and leaves begining of document empty, returns newly created document id

### addF64FixedAt(number, length_of_document, offset_within_document)
creates document with number placed in the middle of document, document is fixed size, returns newly created document id

### getF64At(documentId, offset_within_document)
returns number of document with given id at given position

### setF64At(documentId, number, offset_within_document)
sets/replaces number placed in the middle of document

### partGetF64(documentInfo, offset_within_document)
returns number located at given position in document based on given documentInfo (see "Document API")

### partSetF64(documentInfo, number, offset_within_document)
sets/replaces number located at given position in document based on given documentInfo (see "Document API")

## Data operations - 32 bit float numbers
### addF32(number)
### addF32Fixed(number, length_of_document)
### getF32(documentId)
### setF32(documentId, number)
### addF32At(number, offset_within_document)
### addF32FixedAt(number, length_of_document, offset_within_document)
### getF32At(documentId, offset_within_document)
### setF32At(documentId, number, offset_within_document)
### partGetF32(documentInfo, offset_within_document)
### partSetF32(documentInfo, number, offset_within_document)

## Data operations - 64 bit unsigned integer numbers
### addU64(number)
### addU64Fixed(number, length_of_document)
### getU64(documentId)
### setU64(documentId, number)
### addU64At(number, offset_within_document)
### addU64FixedAt(number, length_of_document, offset_within_document)
### getU64At(documentId, offset_within_document)
### setU64At(documentId, number, offset_within_document)
### partGetU64(documentInfo, offset_within_document)
### partSetU64(documentInfo, number, offset_within_document)

## Data operations - 48 bit unsigned integer numbers
### addU48(number)
### addU48Fixed(number, length_of_document)
### getU48(documentId)
### setU48(documentId, number)
### addU48At(number, offset_within_document)
### addU48FixedAt(number, length_of_document, offset_within_document)
### getU48At(documentId, offset_within_document)
### setU48At(documentId, number, offset_within_document)
### partGetU48(documentInfo, offset_within_document)
### partSetU48(documentInfo, number, offset_within_document)

## Data operations - 32 bit unsigned integer numbers
### addU32(number)
### addU32Fixed(number, length_of_document)
### getU32(documentId)
### setU32(documentId, number)
### addU32At(number, offset_within_document)
### addU32FixedAt(number, length_of_document, offset_within_document)
### getU32At(documentId, offset_within_document)
### setU32At(documentId, number, offset_within_document)
### partGetU32(documentInfo, offset_within_document)
### partSetU32(documentInfo, number, offset_within_document)

## Data operations - 24 bit unsigned integer numbers
### addU24(number)
### addU24Fixed(number, length_of_document)
### getU24(documentId)
### setU24(documentId, number)
### addU24At(number, offset_within_document)
### addU24FixedAt(number, length_of_document, offset_within_document)
### getU24At(documentId, offset_within_document)
### setU24At(documentId, number, offset_within_document)
### partGetU24(documentInfo, offset_within_document)
### partSetU24(documentInfo, number, offset_within_document)

## Data operations - 16 bit unsigned integer numbers
### addU16(number)
### addU16Fixed(number, length_of_document)
### getU16(documentId)
### setU16(documentId, number)
### addU16At(number, offset_within_document)
### addU16FixedAt(number, length_of_document, offset_within_document)
### getU16At(documentId, offset_within_document)
### setU16At(documentId, number, offset_within_document)
### partGetU16(documentInfo, offset_within_document)
### partSetU16(documentInfo, number, offset_within_document)

## Data operations - 8 bit unsigned integer numbers
### addU8(number)
### addU8Fixed(number, length_of_document)
### getU8(documentId)
### setU8(documentId, number)
### addU8At(number, offset_within_document)
### addU8FixedAt(number, length_of_document, offset_within_document)
### getU8At(documentId, offset_within_document)
### setU8At(documentId, number, offset_within_document)
### partGetU8(documentInfo, offset_within_document)
### partSetU8(documentInfo, number, offset_within_document)




## Data operations - 64 bit signed integer numbers
### addS64(number)
### addS64Fixed(number, length_of_document)
### getS64(documentId)
### setS64(documentId, number)
### addS64At(number, offset_within_document)
### addS64FixedAt(number, length_of_document, offset_within_document)
### getS64At(documentId, offset_within_document)
### setS64At(documentId, number, offset_within_document)
### partGetS64(documentInfo, offset_within_document)
### partSetS64(documentInfo, number, offset_within_document)

## Data operations - 48 bit signed integer numbers
### addS48(number)
### addS48Fixed(number, length_of_document)
### getS48(documentId)
### setS48(documentId, number)
### addS48At(number, offset_within_document)
### addS48FixedAt(number, length_of_document, offset_within_document)
### getS48At(documentId, offset_within_document)
### setS48At(documentId, number, offset_within_document)
### partGetS48(documentInfo, offset_within_document)
### partSetS48(documentInfo, number, offset_within_document)

## Data operations - 32 bit signed integer numbers
### addS32(number)
### addS32Fixed(number, length_of_document)
### getS32(documentId)
### setS32(documentId, number)
### addS32At(number, offset_within_document)
### addS32FixedAt(number, length_of_document, offset_within_document)
### getS32At(documentId, offset_within_document)
### setS32At(documentId, number, offset_within_document)
### partGetS32(documentInfo, offset_within_document)
### partSetS32(documentInfo, number, offset_within_document)

## Data operations - 24 bit signed integer numbers
### addS24(number)
### addS24Fixed(number, length_of_document)
### getS24(documentId)
### setS24(documentId, number)
### addS24At(number, offset_within_document)
### addS24FixedAt(number, length_of_document, offset_within_document)
### getS24At(documentId, offset_within_document)
### setS24At(documentId, number, offset_within_document)
### partGetS24(documentInfo, offset_within_document)
### partSetS24(documentInfo, number, offset_within_document)

## Data operations - 16 bit signed integer numbers
### addS16(number)
### addS16Fixed(number, length_of_document)
### getS16(documentId)
### setS16(documentId, number)
### addS16At(number, offset_within_document)
### addS16FixedAt(number, length_of_document, offset_within_document)
### getS16At(documentId, offset_within_document)
### setS16At(documentId, number, offset_within_document)
### partGetS16(documentInfo, offset_within_document)
### partSetS16(documentInfo, number, offset_within_document)

## Data operations - 8 bit signed integer numbers
### addS8(number)
### addS8Fixed(number, length_of_document)
### getS8(documentId)
### setS8(documentId, number)
### addS8At(number, offset_within_document)
### addS8FixedAt(number, length_of_document, offset_within_document)
### getS8At(documentId, offset_within_document)
### setS8At(documentId, number, offset_within_document)
### partGetS8(documentInfo, offset_within_document)
### partSetS8(documentInfo, number, offset_within_document)








# Android termux prerequisites:

```
pkg install python2
pkg install make
pkg install clang
```

Currently not working on Android due to Androids lack of support for sys/shm. :(


