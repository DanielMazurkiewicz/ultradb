#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <node_api.h>
#include <assert.h>

#include "napiMacros.h"


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


napi_value add(napi_env env, const napi_callback_info info) {
    napi_status status;
    getThis(thisObj, numberOfArguments, 0, status);

    var(rawData);
    objPropertyGet(rawData, thisObj, "data");

    void *data;
    size_t dataLength;

    getArrayBuffer(data, dataLength, rawData, status);

    return thisObj;
}

napi_value get(napi_env env, const napi_callback_info info) {
    napi_status status;
    getThis(thisObj, numberOfArguments, 0, status);

    var(rawData);
    objPropertyGet(rawData, thisObj, "data");

    void *data;
    size_t dataLength;

    getArrayBuffer(data, dataLength, rawData, status);

    return thisObj;
}

napi_value set(napi_env env, const napi_callback_info info) {
    napi_status status;
    getThis(thisObj, numberOfArguments, 0, status);

    var(rawData);
    objPropertyGet(rawData, thisObj, "data");

    void *data;
    size_t dataLength;

    getArrayBuffer(data, dataLength, rawData, status);

    return thisObj;
}

napi_value del(napi_env env, const napi_callback_info info) {
    napi_status status;
    getThis(thisObj, numberOfArguments, 0, status);

    var(rawData);
    objPropertyGet(rawData, thisObj, "data");

    void *data;
    size_t dataLength;

    getArrayBuffer(data, dataLength, rawData, status);

    return thisObj;
}

napi_value mov(napi_env env, const napi_callback_info info) {
    napi_status status;
    getThis(thisObj, numberOfArguments, 0, status);

    var(rawData);
    objPropertyGet(rawData, thisObj, "data");

    void *data;
    size_t dataLength;

    getArrayBuffer(data, dataLength, rawData, status);

    return thisObj;
}

napi_value cpy(napi_env env, const napi_callback_info info) {
    napi_status status;
    getThis(thisObj, numberOfArguments, 0, status);

    var(rawData);
    objPropertyGet(rawData, thisObj, "data");

    void *data;
    size_t dataLength;

    getArrayBuffer(data, dataLength, rawData, status);

    return thisObj;
}


napi_value close(napi_env env, const napi_callback_info info) {
    napi_status status;
    getThis(thisObj, numberOfArguments, 0, status);

    var(fd);
    objPropertyGet(fd, thisObj, "fd");
    int fileDescriptor;
    getI32(fileDescriptor, fd, status);

    var(rawData);
    objPropertyGet(rawData, thisObj, "data");

    void *data;
    size_t dataLength;

    getArrayBuffer(data, dataLength, rawData, status);

    var(propName);
    newStringUtf8(propName, "fd", 2, status);
    objPropertyDel(thisObj, propName, status);

    newStringUtf8(propName, "data", 4, status);
    objPropertyDel(thisObj, propName, status);


    if (munmap(data, dataLength) == -1) {
      raiseError(env, thisObj, "close->munmap");
      return thisObj;
    }

    if (close(fileDescriptor) == -1) {
      raiseError(env, thisObj, "close->close");
      return thisObj;
    }

    return thisObj;
}



napi_value CreateObject(napi_env env, const napi_callback_info info) {
  napi_status status;

  getArguments(args, argsCount, 2, status);
  objCreate(obj, status);

  objPropertySet(obj, "dbFileName", args[0], status);
  objPropertySet(obj, "page", args[1], status);

  size_t strLength;
  getStringUtf8Length(strLength, args[0]);

  char fileName[strLength + 1];
  getStringUtf8(fileName, args[0], strLength);

  int fileDescriptor;
  fileDescriptor = open(fileName, O_RDWR | O_CREAT, S_IRWXU | S_IRGRP | S_IROTH);

  if (fileDescriptor == -1) {
    raiseError(env, obj, "(CreateObject)->open");
    return obj;
  }

  struct stat sb;
  if (fstat(fileDescriptor, &sb) == -1) {
    raiseError(env, obj, "(CreateObject)->fstat");
    return obj;
  }


  if (!S_ISREG(sb.st_mode)) {
    raiseError(env, obj, "(CreateObject)->(NotAfile)");
    return obj;
  }


  void *data;
  unsigned int dataLength;
  if (sb.st_size == 0) {
    unsigned int pageSize;
    getU32(pageSize, args[1]);
    if (fallocate(fileDescriptor, 0, 0, pageSize) == -1) {
      raiseError(env, obj, "(CreateObject)->fallocate");
      return obj;
    }
    data = mmap (0, pageSize, PROT_READ|PROT_WRITE, MAP_SHARED, fileDescriptor, 0);
    dataLength = pageSize;
  } else {
    data = mmap (0, sb.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fileDescriptor, 0);
    dataLength = sb.st_size;
  }


  if (data == MAP_FAILED) {
    raiseError(env, obj, "(CreateObject)->mmap");
    return obj;
  }



  var(fd)
  newI32(fd, fileDescriptor);
  objPropertySet(obj, "fd", fd, status);

  var(rawData);
  assignArrayBuffer(rawData, data, dataLength, status);
  objPropertySet(obj, "data", rawData, status);

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

  return obj;


}

napi_value Init(napi_env env, napi_value exports) {
  napi_status status;
  var(new_exports);
  newFunction(new_exports, CreateObject, status);
  return new_exports;
}

NAPI_MODULE(ultradb, Init)
