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


typedef unsigned long long DocumentDescriptor;

enum NodeLocationType_e { LEFT, RIGHT };
typedef enum NodeLocationType_e NodeLocationType;


struct DatabaseHeader_s {
  DocumentDescriptor freeSpace;       // address of first free space to place a document
  DocumentDescriptor baseDescriptor;  // real address = documentDescriptor - baseDescriptor;
  DocumentDescriptor rootDocument;
};
typedef struct DatabaseHeader_s DatabaseHeader;



struct Database_s {
    char*                   fileName;
    unsigned long           numberOfConnected;
    pthread_mutex_t         mutex;

    int                     fileDescriptor;
    DatabaseHeader*         data;
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



napi_value add(napi_env env, const napi_callback_info info) {

}

napi_value get(napi_env env, const napi_callback_info info) {

}

napi_value set(napi_env env, const napi_callback_info info) {

}

napi_value del(napi_env env, const napi_callback_info info) {

}

napi_value mov(napi_env env, const napi_callback_info info) {

}

napi_value cpy(napi_env env, const napi_callback_info info) {

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

      if (munmap(currentDb->data, currentDb->dataLength) == -1) {
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
    getStringUtf8(fileName, args[0], strLength);


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


      if (sb.st_size == 0) {
        getU32(currentDb->pageSize, args[1]);
        if (fallocate(currentDb->fileDescriptor, 0, 0, currentDb->pageSize) == -1) {
          pthread_mutex_unlock(&databasesMutex);
          raiseError(env, obj, "(CreateObject)->fallocate");
          return obj;
        }
        currentDb->data = (DatabaseHeader*) mmap (0, currentDb->pageSize, PROT_READ|PROT_WRITE, MAP_SHARED, currentDb->fileDescriptor, 0);
        currentDb->dataLength = currentDb->pageSize;
      } else {
        currentDb->data = (DatabaseHeader*) mmap (0, sb.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, currentDb->fileDescriptor, 0);
        currentDb->dataLength = sb.st_size;
      }


      if (currentDb->data == MAP_FAILED) {
        pthread_mutex_unlock(&databasesMutex);
        raiseError(env, obj, "(CreateObject)->mmap");
        return obj;
      }

      if (sb.st_size == 0) {
        DatabaseHeader* data = currentDb->data;
        data->freeSpace = sizeof(DatabaseHeader);
        data->baseDescriptor = 0;
        data->rootDocument = 0;
      }

    } else {
      free(fileName);
    }
    currentDb->numberOfConnected++;

    pthread_mutex_unlock(&databasesMutex);









    var(methodFunction);
    newFunction(methodFunction, close, status);
    objPropertySet(obj, "close", methodFunction, status);


    newFunction(methodFunction, add, status);
    objPropertySet(obj, "add", methodFunction, status);

    newFunction(methodFunction, get, status);
    objPropertySet(obj, "get", methodFunction, status);

    newFunction(methodFunction, set, status);
    objPropertySet(obj, "set", methodFunction, status);

    newFunction(methodFunction, del, status);
    objPropertySet(obj, "del", methodFunction, status);

    newFunction(methodFunction, mov, status);
    objPropertySet(obj, "mov", methodFunction, status);

    newFunction(methodFunction, cpy, status);
    objPropertySet(obj, "cpy", methodFunction, status);

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
