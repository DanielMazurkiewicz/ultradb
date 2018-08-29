#include <stdlib.h>
//#include <unistd.h>
//#include <time.h>

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


//#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <node_api.h>
#include <assert.h>

#include <pthread.h>


//#include <sys/ipc.h>
#include <sys/shm.h>


//#include <stdio.h>


#pragma GCC diagnostic ignored "-Wwrite-strings"

#include "generic/customTypes.h"
#include "generic/napiMacros.h"

#include "ultradbTypes.h"
#include "ultradbMacros.h"
#include "ultradbError.h"

#include "methods/management.h"
#include "methods/transaction.h"
#include "methods/cursor.h"
#include "methods/visibility.h"
#include "methods/doc.h"

#include "methodsData/utf8z.h"
#include "methodsData/F64.h"
#include "methodsData/F32.h"
#include "methodsData/U64.h"
#include "methodsData/S64.h"
#include "methodsData/U32.h"
#include "methodsData/S32.h"
#include "methodsData/U16.h"
#include "methodsData/S16.h"
#include "methodsData/U8.h"
#include "methodsData/S8.h"

#include "methodsData/U24.h"
#include "methodsData/U48.h"
#include "methodsData/S24.h"
#include "methodsData/S48.h"

#include "methodsData/docId32.h"
#include "methodsData/docId16.h"
#include "methodsData/docId8.h"

function(_close) {
    napi_status status;
    var(result);

    n_getThis(thisJS, numberOfArguments, 0, status);

    LocalData* localData;
    n_objPropertyGet(result, thisJS, "_", status);
    n_getArrayBufferPointer(localData, result, status);

    n_newStringUtf8(result, "_", 1, status);
    n_objPropertyDel(thisJS, result, status);

    n_objPropertyGet(result, thisJS, "_path", status);
    size_t dbFullPathLength;

    n_getStringUtf8ZLength(dbFullPathLength, result, status);

    char dbFullPath[ dbFullPathLength ];
    n_getStringUtf8Z(dbFullPath, result, dbFullPathLength);

    SharedData* sharedData = localData->sharedData;

    pthread_mutex_lock(&sharedData->mutex);


    freeSharedData(sharedData, localData->sharedDataShmid, "close", dbFullPath);

    if (munmap(localData->fileData.pointer, localData->fileSize) == -1) {
      closeAssert(localData->fileDescriptor, "close", "munmap", "close", dbFullPath);
      finishWithError("close", "munmap", "", dbFullPath, 1);
    }

    closeAssert(localData->fileDescriptor, "close", "close", "", dbFullPath);
    free(localData);

}

function(CreateObject) { // (path, pageSize, newDbCallback)

    napi_status status;
    var(result);

    n_getArguments(args, argsCount, 3, status);


    //==========================


    size_t dbFullPathLength;
    n_getStringUtf8ZLength(dbFullPathLength, args[0]);

    char dbFullPath[ dbFullPathLength ];
    n_getStringUtf8Z(dbFullPath, args[0], dbFullPathLength);



    //==========================


    S32 fileDescriptor = open(dbFullPath, O_RDWR | O_CREAT, S_IRWXU | S_IRGRP | S_IROTH);
    if (fileDescriptor == -1) {
      finishWithError("CreateObject", "open", "problem with opening database", dbFullPath, 1);
    }


    //==========================


    SharedData* sharedData;
    key_t sharedDataKey = ftok(dbFullPath, 'A'); //Associated db
    S32 sharedDataShmid = shmget(sharedDataKey, sizeof(SharedData), 0644 | IPC_CREAT | IPC_EXCL);
    if (sharedDataShmid == -1) {
      if (errno == EEXIST) {
        sharedDataShmid = shmget(sharedDataKey, sizeof(SharedData), 0644);
        sharedData = (SharedData*) shmat(sharedDataShmid, (void *)0, 0);

        waitTill(sharedData->numberOfConnected < 1 && sharedData->numberOfConnected >= 0);
        if (sharedData->numberOfConnected < 0) {

          shmdtAssert(sharedData, "CreateObject", "numberOfConnected", "shmdt", dbFullPath);
          shmctlAssert(sharedDataShmid, "CreateObject", "numberOfConnected", "shmctl", dbFullPath);
          closeAssert(fileDescriptor, "CreateObject", "numberOfConnected", "close", dbFullPath);


          n_newBoolean(result, false, status);
          nanosleep((const struct timespec[]){{0, 5000000L}}, NULL);
          return result;
        }

        pthread_mutex_lock(&sharedData->mutex);
        sharedData->numberOfConnected++;            // increment is not multiprocessor safe
      } else {
        finishWithError("CreateObject", "shmget", "asociacion problem", dbFullPath, 1);
      }
    } else { //new shared memory created
      sharedData = (SharedData*) shmat(sharedDataShmid, (void *)0, 0);



      if (pthread_mutexattr_init(&sharedData->attr) == -1) {
        shm_CLEANUP(sharedData, sharedDataShmid, "CreateObject", "pthread_mutexattr_init", dbFullPath);
        closeAssert(fileDescriptor, "CreateObject", "pthread_mutexattr_init", "close", dbFullPath);
        finishWithError("CreateObject", "pthread_mutexattr_init", "", dbFullPath, 1);
      }

      if (pthread_mutexattr_setpshared(&sharedData->attr, PTHREAD_PROCESS_SHARED) == -1) {
        pthread_mutexattr_destroyAssert(&sharedData->attr, "CreateObject", "pthread_mutexattr_setpshared", "pthread_mutexattr_destroy", dbFullPath);
        shm_CLEANUP(sharedData, sharedDataShmid, "CreateObject", "pthread_mutexattr_setpshared", dbFullPath);
        closeAssert(fileDescriptor, "CreateObject", "pthread_mutexattr_setpshared", "close", dbFullPath);
        finishWithError("CreateObject", "pthread_mutexattr_setpshared", "", dbFullPath, 1);
      }

      if (pthread_mutex_init(&sharedData->mutex, &sharedData->attr) == -1) {
        pthread_mutexattr_destroyAssert(&sharedData->attr, "CreateObject", "pthread_mutex_init", "pthread_mutexattr_destroy", dbFullPath);
        shm_CLEANUP(sharedData, sharedDataShmid, "CreateObject", "pthread_mutex_init", dbFullPath);
        closeAssert(fileDescriptor, "CreateObject", "pthread_mutex_init", "close", dbFullPath);
        finishWithError("CreateObject", "pthread_mutex_init", "", dbFullPath, 1);
      }

      //-----

      if (pthread_mutexattr_init(&sharedData->attrForNewDocument) == -1) {
        pthread_mutex_CLEANUP(sharedData, mutex, attr, "CreateObject", "pthread_mutexattr_init(1)", dbFullPath);
        shm_CLEANUP(sharedData, sharedDataShmid, "CreateObject", "pthread_mutexattr_init(1)", dbFullPath);
        closeAssert(fileDescriptor, "CreateObject", "pthread_mutexattr_init(1)", "close", dbFullPath);
        finishWithError("CreateObject", "pthread_mutexattr_init(1)", "", dbFullPath, 1);
      }

      if (pthread_mutexattr_setpshared(&sharedData->attrForNewDocument, PTHREAD_PROCESS_SHARED) == -1) {
        pthread_mutex_CLEANUP(sharedData, mutex, attr, "CreateObject", "pthread_mutexattr_setpshared(1)", dbFullPath);
        pthread_mutexattr_destroyAssert(&sharedData->attrForNewDocument, "CreateObject", "pthread_mutexattr_setpshared(1)", "pthread_mutexattr_destroy", dbFullPath);
        shm_CLEANUP(sharedData, sharedDataShmid, "CreateObject", "pthread_mutexattr_setpshared(1)", dbFullPath);
        closeAssert(fileDescriptor, "CreateObject", "pthread_mutexattr_setpshared(1)", "close", dbFullPath);
        finishWithError("CreateObject", "pthread_mutexattr_setpshared(1)", "", dbFullPath, 1);
      }

      if (pthread_mutex_init(&sharedData->mutexForNewDocument, &sharedData->attr) == -1) {
        pthread_mutex_CLEANUP(sharedData, mutex, attr, "CreateObject", "pthread_mutex_init(1)", dbFullPath);
        pthread_mutexattr_destroyAssert(&sharedData->attrForNewDocument, "CreateObject", "pthread_mutex_init(1)", "pthread_mutexattr_destroy", dbFullPath);
        shm_CLEANUP(sharedData, sharedDataShmid, "CreateObject", "pthread_mutex_init(1)", dbFullPath);
        closeAssert(fileDescriptor, "CreateObject", "pthread_mutex_init(1)", "close", dbFullPath);
        finishWithError("CreateObject", "pthread_mutex_init(1)", "", dbFullPath, 1);
      }


      sharedData->numberOfConnected = 1;
      pthread_mutex_lock(&sharedData->mutex);
    }




    //==========================


    struct stat sb;
    if (fstat(fileDescriptor, &sb) == -1) {
      pthread_mutex_unlock(&sharedData->mutex);
      freeSharedData(sharedData, sharedDataShmid, "CreateObject(fstat)", dbFullPath);
      closeAssert(fileDescriptor, "CreateObject", "fstat", "close", dbFullPath);
      finishWithError("CreateObject", "fstat", "", dbFullPath, 1);
    }

    if (!S_ISREG(sb.st_mode)) {
      freeSharedData(sharedData, sharedDataShmid, "CreateObject(S_ISREG)", dbFullPath);
      closeAssert(fileDescriptor, "CreateObject", "S_ISREG", "close", dbFullPath);
      finishWithError("CreateObject", "S_ISREG", "", dbFullPath, 1);
    }

    DbFileDataMapping fileData;
    size_t fileSize;
    U32 pageSize;
    n_getU32(pageSize, args[1], status);

    if (sb.st_size == 0) {
      if (fallocate(fileDescriptor, 0, 0, pageSize) == -1) {
        freeSharedData(sharedData, sharedDataShmid, "CreateObject(fallocate)", dbFullPath);
        closeAssert(fileDescriptor, "CreateObject", "fallocate", "close", dbFullPath);
        finishWithError("CreateObject", "fallocate", "", dbFullPath, 1);
      }
      fileData.pointer = mmap (0, pageSize, PROT_READ|PROT_WRITE, MAP_SHARED, fileDescriptor, 0);
      fileSize = pageSize;
    } else {
      fileData.pointer = mmap (0, sb.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fileDescriptor, 0);
      fileSize = sb.st_size;
    }


    if (fileData.pointer == MAP_FAILED) {
      freeSharedData(sharedData, sharedDataShmid, "CreateObject(mmap)", dbFullPath);
      closeAssert(fileDescriptor, "CreateObject", "mmap", "close", dbFullPath);
      finishWithError("CreateObject", "mmap", "", dbFullPath, 1);
    }


    //==========================


    if (sb.st_size == 0) {
      DatabaseHeader* header = fileData.header;
      header->freeSpace = sizeof(DatabaseHeader);
      header->baseDescriptor = 0;
      header->rootDocument = 0;
      header->freeSpacePrevious = 0;

      n_getGlobal(global, status)
      n_callWithNoArg(result, args[2], global, status);
      U64* checksumKey;
      n_getBufferPointer(checksumKey, result, status)
      header->checksumKey = *checksumKey;
    }
    sharedData->fileSize = fileSize;


    pthread_mutex_unlock(&sharedData->mutex);


    LocalData* localData = (LocalData*) malloc(sizeof(LocalData));

    localData->sharedData = sharedData;
    localData->sharedDataShmid = sharedDataShmid;
    localData->fileData = fileData;
    localData->fileSize = fileSize;
    localData->pageSize = pageSize;
    localData->fileDescriptor = fileDescriptor;


    //==========================


    n_objCreate(obj, status);


    n_assignArrayBuffer(result, localData, sizeof(LocalData), status);
    n_objPropertySet(obj, "_", result, status);

    n_newS64(result, sharedDataKey, status);
    n_objPropertySet(obj, "_key", result, status);
    n_objPropertySet(obj, "_path", args[0], status);


    var(methodFunction);

    n_objAssignFunction(obj, methodFunction, _close, status);

    methodsManagement(obj, methodFunction, status);
    methodsTransaction(obj, methodFunction, status);
    methodsCursor(obj, methodFunction, status);
    methodsVisibility(obj, methodFunction, status);
    methodsDoc(obj, methodFunction, status);

    methodsUtf8z(obj, methodFunction, status);
    methodsF64(obj, methodFunction, status);
    methodsF32(obj, methodFunction, status);
    methodsU64(obj, methodFunction, status);
    methodsU32(obj, methodFunction, status);
    methodsU16(obj, methodFunction, status);
    methodsU8(obj, methodFunction, status);
    methodsS64(obj, methodFunction, status);
    methodsS32(obj, methodFunction, status);
    methodsS16(obj, methodFunction, status);
    methodsS8(obj, methodFunction, status);

    methodsU24(obj, methodFunction, status);
    methodsU48(obj, methodFunction, status);
    methodsS24(obj, methodFunction, status);
    methodsS48(obj, methodFunction, status);

    methodsDocId32(obj, methodFunction, status);
    methodsDocId16(obj, methodFunction, status);
    methodsDocId8(obj, methodFunction, status);

    return obj;
}

napi_value Init(napi_env env, napi_value exports) {
  napi_status status;
  var(new_exports);
  n_newFunction(new_exports, CreateObject, status);
  return new_exports;
}

NAPI_MODULE(ultradb, Init)
