#include <stdlib.h>

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <node_api.h>
#include <assert.h>

#include <pthread.h>

#include "napiMacros.h"
#include "ultradbMacros.h"


#include <stdio.h>

typedef long long DocumentDescriptor;

enum NodeLocationType_e { LEFT, RIGHT };
typedef enum NodeLocationType_e NodeLocationType;


struct DatabaseHeader_s {
  DocumentDescriptor freeSpace;       // address of first free space to place a document
  DocumentDescriptor baseDescriptor;  // real address = documentDescriptor - baseDescriptor;
  DocumentDescriptor rootDocument;    // document descriptor of root document
};
typedef struct DatabaseHeader_s DatabaseHeader;


struct i48_s {
  unsigned      high;
  unsigned long low;
};
typedef struct i48_s U48;
typedef struct i48_s S48;

typedef unsigned char            U8;
typedef signed char              S8;
typedef unsigned                 U16;
typedef signed                   S16;
typedef unsigned long            U32;
typedef long                     S32;
typedef unsigned long long       U64;
typedef long long                S64;

union Data_u {
  void*                     pointer;
  DatabaseHeader*           header;
  char*      str;
  U8*         u8;
  S8*         s8;
  U16*       u16;
  S16*       s16;
  U32*       u32;
  S32*       s32;
  U48*       u48;
  S48*       s48;
  U64*       u64;
  S64*       s64;
};
typedef union Data_u Data;


union DocumentSize_u {
  unsigned char*            u8;
  unsigned*                 u16;
  unsigned long*            u32;
  unsigned long long*       u64;
};
typedef union DocumentSize_u DocumentSize;


struct Database_s {
    char*                   fileName;
    unsigned long           numberOfConnected;
    pthread_mutex_t         mutex;
    pthread_mutex_t         mutexForNewDocument;
    //pthread_cond_t	        cond;


    int                     fileDescriptor;
    Data                    data;
    size_t                  dataLength;
    unsigned int            pageSize;

    struct Database_s*           left;
    struct Database_s*           right;

    struct Database_s*           parent;
    NodeLocationType             side;

};
typedef struct Database_s Database;





Database*         databasesRoot;
pthread_mutex_t   databasesMutex = PTHREAD_MUTEX_INITIALIZER;


/*
  Supported data types
  Utf8
  Utf16
  Latin1
  Buffer
  U8
  U16
  U32
  U64
  S8
  S16
  S32
  S64
  F32
  F64

  const data = [
    {
      name: 'left',
      type: 'S64'
    },
    {
      name: 'right',
      type: 'S64'
    }
  ]


*/


napi_value raiseError(napi_env env, napi_value thisObj, char *errorSource) {
    napi_status status;

    char* errorMessage;
    errorMessage = strerror(errno);

    objCreate(errorObj, status);
    var(error);

    newStringUtf8(error, errorSource, strlen(errorSource), status);
    objPropertySet(errorObj, "source", error, status);

    newStringLatin1(error, errorMessage, strlen(errorMessage), status);
    objPropertySet(errorObj, "message", error, status);

    objPropertySet(thisObj, "error", errorObj, status);
    return errorObj;
}


napi_value tStart(napi_env env, const napi_callback_info info) {
    napi_status status;
    getThis(thisJS, numberOfArguments, 0, status);

    Database* currentDb;
    var(propertyData);
    objPropertyGet(propertyData, thisJS, "_", status);
    getArrayBufferPointer(currentDb, propertyData, status);

    pthread_mutex_lock(&currentDb->mutex);
}

napi_value tEnd(napi_env env, const napi_callback_info info) {
    napi_status status;
    getThis(thisJS, numberOfArguments, 0, status);

    Database* currentDb;
    var(propertyData);
    objPropertyGet(propertyData, thisJS, "_", status);
    getArrayBufferPointer(currentDb, propertyData, status);

    pthread_mutex_unlock(&currentDb->mutex);
}


napi_value addUtf8(napi_env env, const napi_callback_info info) {
    napi_status status;
    var(result);
    getThisAndArguments(thisJS, args, argsCount, 1, status);

    Database* db;
    var(propertyData);
    objPropertyGet(propertyData, thisJS, "_", status);
    getArrayBufferPointer(db, propertyData, status);


    size_t documentLength;
    getStringUtf8Length(documentLength, args[0]);
    documentLength++;


    pthread_mutex_lock(&db->mutexForNewDocument);
    Data data = db->data;

    DocumentDescriptor freeSpace = data.header->freeSpace;
    DocumentDescriptor documentLengthOffset = freeSpace + documentLength;
    DocumentDescriptor newFreeSpace = documentLengthOffset;
    DocumentDescriptor documentId;



    if (documentLength < 32) {
      documentId = newFreeSpace;
      newFreeSpace += 1;
      resizeIfNecesarryDbFile(db, data, newFreeSpace, result);
      data.u8[documentLengthOffset] = documentLength | 0x80;                                    // deleted at the start
    } else if (documentLength < 8192) {
      newFreeSpace += 2;
      documentId = newFreeSpace - 1;
      resizeIfNecesarryDbFile(db, data, newFreeSpace, result);
      *((U16*)(data.pointer + documentLengthOffset)) = documentLength | 0xA000;                 // deleted at the start
    } else if (documentLength < 536870912) {
      newFreeSpace += 4;
      documentId = newFreeSpace - 1;
      resizeIfNecesarryDbFile(db, data, newFreeSpace, result);
      *((U32*)(data.pointer + documentLengthOffset)) = documentLength | 0xC0000000;             // deleted at the start
    } else {
      newFreeSpace += 8;
      documentId = newFreeSpace - 1;
      resizeIfNecesarryDbFile(db, data, newFreeSpace, result);
      *((U64*)(data.pointer + documentLengthOffset)) = documentLength | 0xE000000000000000;     // deleted at the start
    }
    data.header->freeSpace = newFreeSpace;
    pthread_mutex_unlock(&db->mutexForNewDocument);


    getStringUtf8((db->data.str + freeSpace), args[0], documentLength);

    db->data.u8[documentId] &= 0x7f; // document written and available as not deleted

    newS64(result, documentId + db->data.header->baseDescriptor, status);
    return result;

}

napi_value getUtf8(napi_env env, const napi_callback_info info) {
    napi_status status;
    var(result);
    getThisAndArguments(thisJS, args, argsCount, 1, status);

    Database* db;
    var(propertyData);
    objPropertyGet(propertyData, thisJS, "_", status);
    getArrayBufferPointer(db, propertyData, status);

    DocumentDescriptor descriptor;
    getS64(descriptor, args[0]);


    descriptor += db->data.header->baseDescriptor;
    if (descriptor < sizeof(DatabaseHeader) || descriptor >= db->data.header->freeSpace) {
      raiseError(env, thisJS, "get(*)-><invalid document descriptor#1>");
      setUndefined(result);
      return result;
    }

    U8 docFlags = db->data.u8[descriptor];


    if ((docFlags & 0x80) != 0) { // document is deleted
      setUndefined(result);
      return result;
    }
    size_t documentLength;
    size_t documentStart;

    switch (docFlags & 0x60) {
      case 0x00:
        documentLength = docFlags & 0x1f;
        documentStart = descriptor - documentLength;
        break;
      case 0x20:
        descriptor -= 1;
        if (descriptor < sizeof(DatabaseHeader)) {
          raiseError(env, thisJS, "get(*)-><invalid document descriptor#2>");
          setUndefined(result);
          return result;
        }

        documentLength = *((U16*)(db->data.pointer + descriptor)) & 0x1fff;
        documentStart = descriptor - documentLength;
        break;
      case 0x40:
        descriptor -= 3;
        if (descriptor < sizeof(DatabaseHeader)) {
          raiseError(env, thisJS, "get(*)-><invalid document descriptor#3>");
          setUndefined(result);
          return result;
        }

        documentLength = *((U32*)(db->data.pointer + descriptor)) & 0x1fffffff;
        documentStart = descriptor - documentLength;

        break;
      case 0x60:
        descriptor -= 7;
        if (descriptor < sizeof(DatabaseHeader)) {
          raiseError(env, thisJS, "get(*)-><invalid document descriptor#4>");
          setUndefined(result);
          return result;
        }

        documentLength = *((U64*)(db->data.pointer + descriptor)) & 0x1fffffffffffffff;
        documentStart = descriptor - documentLength;

        break;
    }


    if (documentStart < sizeof(DatabaseHeader)) {
      raiseError(env, thisJS, "get(*)-><invalid document descriptor#5>");
      setUndefined(result);
      return result;
    }
    newStringUtf8(result, db->data.pointer + documentStart, strlen(db->data.pointer + documentStart), status);

    return result;
}

napi_value setUtf8(napi_env env, const napi_callback_info info) {

}

napi_value delUtf8(napi_env env, const napi_callback_info info) {

}

napi_value movUtf8(napi_env env, const napi_callback_info info) {

}

napi_value cpyUtf8(napi_env env, const napi_callback_info info) {

}

// ---

napi_value getPart(napi_env env, const napi_callback_info info) {

}

napi_value setPart(napi_env env, const napi_callback_info info) {

}



napi_value close(napi_env env, const napi_callback_info info) {
    napi_status status;
    getThis(thisJS, numberOfArguments, 0, status);

    Database* currentDb;
    var(propertyData);
    objPropertyGet(propertyData, thisJS, "_", status);
    getArrayBufferPointer(currentDb, propertyData, status);

    var(propName);
    newStringUtf8(propName, "_", 1, status);
    objPropertyDel(thisJS, propName, status);

    pthread_mutex_lock(&databasesMutex);

    if (currentDb->numberOfConnected > 1) {
      currentDb->numberOfConnected--;
    } else { 

      // remove from BST:

      Database* replacementDb;
      Database* repinDb;
      int childrenNumber = 0;

      if ( currentDb->left != NULL ) childrenNumber++;
      if ( currentDb->right != NULL ) childrenNumber++;

      if (childrenNumber == 2) {
        replacementDb = currentDb->right;
        if (replacementDb->left == NULL) { 
          if (currentDb->parent == NULL) { // it is a root
            databasesRoot = replacementDb;
            replacementDb->parent = NULL;
          } else {
            if (currentDb->side == LEFT) {
              currentDb->parent->left = replacementDb;
              replacementDb->side = LEFT;
            } else {
              currentDb->parent->right = replacementDb;            
              replacementDb->side = RIGHT;
            }
            replacementDb->parent = currentDb->parent;
          }

          replacementDb->left = currentDb->left;

        } else {
          while (replacementDb->left != NULL) {
            replacementDb = replacementDb->left;
          }

          repinDb = replacementDb->right;
          if (repinDb != NULL) {
            repinDb->parent = replacementDb->parent;
            repinDb->side = LEFT;
            repinDb->parent->left = repinDb;
          }

          if (currentDb->parent == NULL) { // it is a root
            databasesRoot = replacementDb;
            replacementDb->parent = NULL;
          } else {
            replacementDb->parent = currentDb->parent;
            replacementDb->side = currentDb->side;
          }
          replacementDb->left = currentDb->left;
          replacementDb->right = currentDb->right;
        }
      } else if (childrenNumber == 1) {
        replacementDb = currentDb->left;
        if (replacementDb == NULL) { //assume right
          replacementDb = currentDb->right;
        }

        if (currentDb->parent == NULL) { // it is a root
          databasesRoot = replacementDb;
          replacementDb->parent = NULL;
        } else {
          if (currentDb->side == LEFT) {
            currentDb->parent->left = replacementDb;
            replacementDb->side = LEFT;
          } else {
            currentDb->parent->right = replacementDb;            
            replacementDb->side = RIGHT;
          }
          replacementDb->parent = currentDb->parent;
        }        

      } else {
        if (currentDb->parent == NULL) { // it is a root
          databasesRoot = NULL;
        } else {
          if (currentDb->side == LEFT) {
            currentDb->parent->left = NULL;
          } else {
            currentDb->parent->right = NULL;
          }
        }
      }


      // free resources:

      if (munmap(currentDb->data.pointer, currentDb->dataLength) == -1) {
        raiseError(env, thisJS, "close->munmap");
        free(currentDb->fileName);
        free(currentDb);
        return thisJS;
      }

      if (close(currentDb->fileDescriptor) == -1) {
        raiseError(env, thisJS, "close->close");
        free(currentDb->fileName);
        free(currentDb);
        return thisJS;
      }

      free(currentDb->fileName);
      free(currentDb);
    }
    pthread_mutex_unlock(&databasesMutex);


    return thisJS;
}



napi_value CreateObject(napi_env env, const napi_callback_info info) {
    napi_status status;

    getArguments(args, argsCount, 3, status);
    objCreate(obj, status);

    size_t strLength;
    getStringUtf8Length(strLength, args[0]);

    char* fileName = (char*) malloc(strLength + 1);
    getStringUtf8Z(fileName, args[0], strLength);


    Database* currentDb;
    Database* previousDb;
    bool      newDb;

    pthread_mutex_lock(&databasesMutex);
    if (databasesRoot == NULL) {
      currentDb = (Database*) calloc(1, sizeof(Database));
      if (currentDb == NULL) {
        pthread_mutex_unlock(&databasesMutex);
        raiseError(env, obj, "(CreateObject)->calloc(1-root)");
        return obj;
      }


      databasesRoot = currentDb;
      newDb = true;
    } else {
      currentDb = databasesRoot;
      int strCmpResult;
      while (currentDb != NULL) {
        strCmpResult = strcmp(fileName, currentDb->fileName);

        if (strCmpResult > 0) {
          previousDb = currentDb;
          currentDb = currentDb->right;
        } else if (strCmpResult < 0) {
          previousDb = currentDb;
          currentDb = currentDb->left;
        } else {
          break;
        }
      }

      if (currentDb == NULL) {
        currentDb = (Database*) calloc(1, sizeof(Database));
        if (currentDb == NULL) {
          pthread_mutex_unlock(&databasesMutex);
          raiseError(env, obj, "(CreateObject)->calloc(2)");
          return obj;
        }


        if (strCmpResult < 0) {
          previousDb->left = currentDb;
          currentDb->side = LEFT;
        } else {
          previousDb->right = currentDb;
          currentDb->side = RIGHT;
        }
        currentDb->parent = previousDb;
        currentDb->mutex = PTHREAD_MUTEX_INITIALIZER;
        currentDb->mutexForNewDocument = PTHREAD_MUTEX_INITIALIZER;

        newDb = true;
      }
    }


    var(rawData);

    assignArrayBuffer(rawData, currentDb, sizeof(Database), status);
    objPropertySet(obj, "_", rawData, status);


    if (newDb) { // initialize database;
      currentDb->fileName = fileName;
      currentDb->fileDescriptor = open(fileName, O_RDWR | O_CREAT, S_IRWXU | S_IRGRP | S_IROTH);
      if (currentDb->fileDescriptor == -1) {
        pthread_mutex_unlock(&databasesMutex);
        raiseError(env, obj, "(CreateObject)->open");
        return obj;
      }

      struct stat sb;
      if (fstat(currentDb->fileDescriptor, &sb) == -1) {
        pthread_mutex_unlock(&databasesMutex);
        raiseError(env, obj, "(CreateObject)->fstat");
        return obj;
      }

      if (!S_ISREG(sb.st_mode)) {
        pthread_mutex_unlock(&databasesMutex);
        raiseError(env, obj, "(CreateObject)->(NotAfile)");
        return obj;
      }

      getU32(currentDb->pageSize, args[1]);

      if (sb.st_size == 0) {
        if (fallocate(currentDb->fileDescriptor, 0, 0, currentDb->pageSize) == -1) {
          pthread_mutex_unlock(&databasesMutex);
          raiseError(env, obj, "(CreateObject)->fallocate");
          return obj;
        }
        currentDb->data.pointer = mmap (0, currentDb->pageSize, PROT_READ|PROT_WRITE, MAP_SHARED, currentDb->fileDescriptor, 0);
        currentDb->dataLength = currentDb->pageSize;
      } else {
        currentDb->data.pointer = mmap (0, sb.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, currentDb->fileDescriptor, 0);
        currentDb->dataLength = sb.st_size;
      }


      if (currentDb->data.pointer == MAP_FAILED) {
        pthread_mutex_unlock(&databasesMutex);
        raiseError(env, obj, "(CreateObject)->mmap");
        return obj;
      }

      if (sb.st_size == 0) {
        DatabaseHeader* header = currentDb->data.header;
        header->freeSpace = sizeof(DatabaseHeader);
        header->baseDescriptor = 0;
        header->rootDocument = 0;
      }

    } else {
      free(fileName);
    }
    currentDb->numberOfConnected++;

    pthread_mutex_unlock(&databasesMutex);









    var(methodFunction);
    newFunction(methodFunction, close, status);
    objPropertySet(obj, "close", methodFunction, status);


    newFunction(methodFunction, addUtf8, status);
    objPropertySet(obj, "addUtf8", methodFunction, status);

    newFunction(methodFunction, getUtf8, status);
    objPropertySet(obj, "getUtf8", methodFunction, status);

    newFunction(methodFunction, setUtf8, status);
    objPropertySet(obj, "setUtf8", methodFunction, status);

    newFunction(methodFunction, delUtf8, status);
    objPropertySet(obj, "delUtf8", methodFunction, status);

    newFunction(methodFunction, movUtf8, status);
    objPropertySet(obj, "movUtf8", methodFunction, status);

    newFunction(methodFunction, cpyUtf8, status);
    objPropertySet(obj, "cpyUtf8", methodFunction, status);



    newFunction(methodFunction, tStart, status);
    objPropertySet(obj, "tStart", methodFunction, status);

    newFunction(methodFunction, tEnd, status);
    objPropertySet(obj, "tEnd", methodFunction, status);

    return obj;
}

napi_value Init(napi_env env, napi_value exports) {
  napi_status status;
  var(new_exports);
  newFunction(new_exports, CreateObject, status);
  return new_exports;
}

NAPI_MODULE(ultradb, Init)
