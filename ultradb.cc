#include <fcntl.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <node_api.h>
#include <assert.h>

#include "napiMacros.h"


napi_value close(napi_env env, const napi_callback_info info) {
    napi_status status;
    getThis(thisObj, numberOfArguments, 0, status);

    var(rawData);
    objPropertyGet(rawData, thisObj, "data");

    var(fd);
    objPropertyGet(fd, thisObj, "fileDescriptor");
    int fileDescriptor;
    getI32(fileDescriptor, fd, status);


    void *data;
    size_t dataLength;

    getArrayBuffer(data, dataLength, rawData, status);

    var(error);
    if (munmap(data, dataLength) == -1) {
      newStringUtf8(error, "munmap", 6, status);
      objPropertySet(thisObj, "error", error, status);
      return thisObj;
    }



    if (close(fileDescriptor) == -1) {
      newStringUtf8(error, "close", 5, status);
      objPropertySet(thisObj, "error", error, status);
      return thisObj;
    }


    return thisObj;
}



napi_value CreateObject(napi_env env, const napi_callback_info info) {
  napi_status status;

  getArguments(args, argsCount, 2, status);
  objCreate(obj, status);

  objPropertySet(obj, "dbFileName", args[0], status);

  size_t strLength;
  getStringUtf8Length(strLength, args[0]);

  char fileName[strLength + 1];
  getStringUtf8(fileName, args[0], strLength);

  var(error);
  int fileDescriptor;
  fileDescriptor = open(fileName, O_RDWR | O_CREAT, S_IRWXU | S_IRGRP | S_IROTH);

  if (fileDescriptor == -1) {
    newStringUtf8(error, "opening file", 12, status);
    objPropertySet(obj, "error", error, status);
    return obj;
  }

  struct stat sb;
  if (fstat(fileDescriptor, &sb) == -1) {
    newStringUtf8(error, "fstat", 5, status);
    objPropertySet(obj, "error", error, status);
    return obj;
  }


  if (!S_ISREG(sb.st_mode)) {
    newStringUtf8(error, "not a file", 10, status);
    objPropertySet(obj, "error", error, status);
    return obj;
  }

  void *data;
  data = mmap (0, sb.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fileDescriptor, 0);

  if (data == MAP_FAILED) {
    newStringUtf8(error, "mmap", 4, status);
    objPropertySet(obj, "error", error, status);
    return obj;
  }



  var(fd)
  newI32(fd, fileDescriptor);
  objPropertySet(obj, "fileDescriptor", fd, status);

  var(rawData);
  assignArrayBuffer(rawData, data, sb.st_size, status);
  objPropertySet(obj, "data", rawData, status);


  var(closeFunction);
  newFunction(closeFunction, close, status);
  objPropertySet(obj, "close", closeFunction, status);

  return obj;


}

napi_value Init(napi_env env, napi_value exports) {
  napi_status status;
  var(new_exports);
  newFunction(new_exports, CreateObject, status);
  return new_exports;
}

NAPI_MODULE(ultradb, Init)
