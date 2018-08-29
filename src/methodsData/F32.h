#define n_getF64toF32inMemory(address, napiValue, tempValue) \
    F64 tempValue; \
    n_getF64(tempValue, napiValue); \
    *((F32*)(address)) = tempValue; \


function (addF32) { // (number)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 1, status);
    getLocalData(thisJS, localData, result, status);

    size_t documentLength = sizeOfF32;
    documentReserveSpaceHidden("addF32", localData, fileData, freeSpace, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, documentLength, documentLengthOffset)

    n_getF64toF32inMemory(localData->fileData.pointer + freeSpace, args[0], tempF64)

    documentUnhide(localData, documentAddress);

    n_newU64(result, documentAddress + localData->fileData.header->baseDescriptor, status);
    return result;
}

function (addF32Fixed) { // (number, length_of_document) - creates document with float 64 bit, reserves extra space for being able to modify document later
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);
    getLocalData(thisJS, localData, result, status);

    size_t documentLength;
    n_getU64(documentLength, args[1]);

    if (sizeOfF32 > documentLength) {
      n_setUndefined(result);
      return result;
    }

    documentReserveSpaceHidden("addF32Fixed", localData, fileData, freeSpace, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, documentLength, documentLengthOffset)

    n_getF64toF32inMemory(localData->fileData.pointer + freeSpace, args[0], tempF64);

    documentUnhide(localData, documentAddress);

    n_newS64(result, documentAddress + localData->fileData.header->baseDescriptor, status);
    return result;
}

function (getF32) { // (documentId)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 1, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor documentId;
    n_getU64(documentId, args[0]);

    documentGetAddressAndFlags("getF32", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentReturnNullIfHidden(documentFlags, result);
    documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
    if (documentLength < sizeOfF32) {
      n_setUndefined(result);
      return result;
    }
    documentAddressStartValidate(documentAddressStart, result);

    n_newF64(result, *((F32*)(fileData.pointer + documentAddressStart)), status);

    return result;
}

function (setF32) { // (documentId, number)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor  documentId;
    n_getU64(documentId, args[0]);

    documentGetAddressAndFlags("setF32", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentReturnNullIfHidden(documentFlags, result);
    documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
    if (sizeOfF32 > documentLength) {
      n_setUndefined(result);
      return result;
    }
    documentAddressStartValidate(documentAddressStart, result);



    n_getF64toF32inMemory(localData->fileData.pointer + documentAddressStart, args[1], tempF64);

    n_newBoolean(result, true, status);
    return result;
}




function (addF32At) { // (number, offset_within_document) - creates document with floar 64 bit number and leaves begining of document empty
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);
    getLocalData(thisJS, localData, result, status);

    DocumentAddress at;
    n_getU64(at, args[1]);

    size_t documentLength =  sizeOfF32 + at;

    documentReserveSpaceHidden("addF32At", localData, fileData, freeSpace, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, documentLength, documentLengthOffset)

    n_getF64toF32inMemory(localData->fileData.pointer + freeSpace + at, args[0], tempF64);

    documentUnhide(localData, documentAddress);

    n_newU64(result, documentAddress + localData->fileData.header->baseDescriptor, status);
    return result;
}


function (addF32FixedAt) { // (number, length_of_document, offset_within_document) - creates document with text, reserves extra space for being able to modify document later
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 4, status);
    getLocalData(thisJS, localData, result, status);

    DocumentAddress at;
    n_getU64(at, args[2]);

    size_t documentLength;
    n_getU64(documentLength, args[1]);

    if (sizeOfF32 + at > documentLength) {
      n_setUndefined(result);
      return result;
    }

    documentReserveSpaceHidden("addF32FixedAt", localData, fileData, freeSpace, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, documentLength, documentLengthOffset)

    n_getF64toF32inMemory(localData->fileData.pointer + freeSpace + at, args[0], tempF64);

    documentUnhide(localData, documentAddress);

    n_newU64(result, documentAddress + localData->fileData.header->baseDescriptor, status);
    return result;
}


function (getF32At) { // (documentId, at)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor  documentId;
    n_getU64(documentId, args[0]);


    documentGetAddressAndFlags("getF32At", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentReturnNullIfHidden(documentFlags, result);
    documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
    documentAddressStartValidate(documentAddressStart, result);

    DocumentAddress at;
    n_getU64(at, args[1]);

    if (sizeOfF32 + at > documentLength) {
      n_setUndefined(result);
      return result;
    }

    n_newF64(result, *((F32*)(fileData.pointer + documentAddressStart + at)), status);

    return result;
}


function (setF32At) { // (documentId, number, at)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 3, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor  documentId;
    n_getU64(documentId, args[0]);

    documentGetAddressAndFlags("setF32At", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentReturnNullIfHidden(documentFlags, result);
    documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
    documentAddressStartValidate(documentAddressStart, result);


    DocumentAddress at;
    n_getU64(at, args[2]);

    if (sizeOfF32 + at > documentLength) {
      n_setUndefined(result);
      return result;
    }

    n_getF64toF32inMemory(localData->fileData.pointer + documentAddressStart + at, args[1], tempF64);

    n_newBoolean(result, true, status);
    return result;
}


function (partGetF32) { // (documentInfoBuffer, at)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);

    getLocalData(thisJS, localData, result, status);

    DocumentInfo* documentInfo;
    n_getBufferPointer(documentInfo, args[0]);

    DocumentAddress at;
    n_getU64(at, args[1]);

    if (at + sizeOfF32 > documentInfo->length) {
      n_setUndefined(result);
      return result;
    }

    n_newF64(result, *((F32*)(localData->fileData.pointer + documentInfo->start + at)), status);

    return result;
}


function (partSetF32) { // (documentInfoBuffer, number, at)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 4, status);
    getLocalData(thisJS, localData, result, status);

    DocumentInfo* documentInfo;
    n_getBufferPointer(documentInfo, args[0]);

    DocumentAddress at;
    n_getU64(at, args[2]);

    if (at + sizeOfF32 > documentInfo->length) {
      n_setUndefined(result);
      return result;
    }

    n_getF64toF32inMemory(localData->fileData.pointer + documentInfo->start + at, args[1], tempF64);

    n_newBoolean(result, true, status);
    return result;
}



#define methodsF32(obj, methodFunction, status) \
  n_objAssignFunction(obj, methodFunction, addF32, status); \
  n_objAssignFunction(obj, methodFunction, addF32Fixed, status); \
  n_objAssignFunction(obj, methodFunction, getF32, status); \
  n_objAssignFunction(obj, methodFunction, setF32, status); \
  \
  n_objAssignFunction(obj, methodFunction, addF32At, status); \
  n_objAssignFunction(obj, methodFunction, addF32FixedAt, status); \
  n_objAssignFunction(obj, methodFunction, getF32At, status); \
  n_objAssignFunction(obj, methodFunction, setF32At, status); \
  \
  n_objAssignFunction(obj, methodFunction, partGetF32, status); \
  n_objAssignFunction(obj, methodFunction, partSetF32, status); \

