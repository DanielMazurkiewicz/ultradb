#define n_getS32toS16inMemory(address, napiValue, tempValue) \
    S32 tempValue; \
    n_getS32(tempValue, napiValue); \
    *((S16*)(address)) = tempValue; \


function (addS16) { // (number)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 1, status);
    getLocalData(thisJS, localData, result, status);

    size_t documentLength = sizeOfS16;
    documentReserveSpaceHidden("addS16", localData, fileData, freeSpace, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, documentLength, documentLengthOffset)

    n_getS32toS16inMemory(localData->fileData.pointer + freeSpace, args[0], tempS32)

    documentUnhide(localData, documentAddress);

    n_newU64(result, documentAddress + localData->fileData.header->baseDescriptor, status);
    return result;
}

function (addS16Fixed) { // (number, length_of_document) - creates document with float 64 bit, reserves extra space for being able to modify document later
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);
    getLocalData(thisJS, localData, result, status);

    size_t documentLength;
    n_getU64(documentLength, args[1]);

    if (sizeOfS16 > documentLength) {
      n_setUndefined(result);
      return result;
    }

    documentReserveSpaceHidden("addS16Fixed", localData, fileData, freeSpace, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, documentLength, documentLengthOffset)

    n_getS32toS16inMemory(localData->fileData.pointer + freeSpace, args[0], tempS32);

    documentUnhide(localData, documentAddress);

    n_newU64(result, documentAddress + localData->fileData.header->baseDescriptor, status);
    return result;
}

function (getS16) { // (documentId)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 1, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor documentId;
    n_getU64(documentId, args[0]);

    documentGetAddressAndFlags("getS16", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentReturnNullIfHidden(documentFlags, result);
    documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
    if (documentLength < sizeOfS16) {
      n_setUndefined(result);
      return result;
    }
    documentAddressStartValidate(documentAddressStart, result);

    n_newS32(result, *((S16*)(fileData.pointer + documentAddressStart)), status);

    return result;
}

function (setS16) { // (documentId, number)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor  documentId;
    n_getU64(documentId, args[0]);

    documentGetAddressAndFlags("setS16", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentReturnNullIfHidden(documentFlags, result);
    documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
    if (sizeOfS16 > documentLength) {
      n_setUndefined(result);
      return result;
    }
    documentAddressStartValidate(documentAddressStart, result);



    n_getS32toS16inMemory(localData->fileData.pointer + documentAddressStart, args[1], tempS32);

    n_newBoolean(result, true, status);
    return result;
}




function (addS16At) { // (number, offset_within_document) - creates document with floar 64 bit number and leaves begining of document empty
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);
    getLocalData(thisJS, localData, result, status);

    DocumentAddress at;
    n_getU64(at, args[1]);

    size_t documentLength =  sizeOfS16 + at;

    documentReserveSpaceHidden("addS16At", localData, fileData, freeSpace, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, documentLength, documentLengthOffset)

    n_getS32toS16inMemory(localData->fileData.pointer + freeSpace + at, args[0], tempS32);

    documentUnhide(localData, documentAddress);

    n_newU64(result, documentAddress + localData->fileData.header->baseDescriptor, status);
    return result;
}


function (addS16FixedAt) { // (number, length_of_document, offset_within_document) - creates document with text, reserves extra space for being able to modify document later
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 4, status);
    getLocalData(thisJS, localData, result, status);

    DocumentAddress at;
    n_getU64(at, args[2]);

    size_t documentLength;
    n_getU64(documentLength, args[1]);

    if (sizeOfS16 + at > documentLength) {
      n_setUndefined(result);
      return result;
    }

    documentReserveSpaceHidden("addS16FixedAt", localData, fileData, freeSpace, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, documentLength, documentLengthOffset)

    n_getS32toS16inMemory(localData->fileData.pointer + freeSpace + at, args[0], tempS32);

    documentUnhide(localData, documentAddress);

    n_newU64(result, documentAddress + localData->fileData.header->baseDescriptor, status);
    return result;
}


function (getS16At) { // (documentId, at)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor  documentId;
    n_getU64(documentId, args[0]);


    documentGetAddressAndFlags("getS16At", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentReturnNullIfHidden(documentFlags, result);
    documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
    documentAddressStartValidate(documentAddressStart, result);

    DocumentAddress at;
    n_getU64(at, args[1]);

    if (sizeOfS16 + at > documentLength) {
      n_setUndefined(result);
      return result;
    }

    n_newS32(result, *((S16*)(fileData.pointer + documentAddressStart + at)), status);

    return result;
}


function (setS16At) { // (documentId, number, at)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 3, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor  documentId;
    n_getU64(documentId, args[0]);

    documentGetAddressAndFlags("setS16At", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentReturnNullIfHidden(documentFlags, result);
    documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
    documentAddressStartValidate(documentAddressStart, result);


    DocumentAddress at;
    n_getU64(at, args[2]);

    if (sizeOfS16 + at > documentLength) {
      n_setUndefined(result);
      return result;
    }

    n_getS32toS16inMemory(localData->fileData.pointer + documentAddressStart + at, args[1], tempS32);

    n_newBoolean(result, true, status);
    return result;
}


function (partGetS16) { // (documentInfoBuffer, at)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);

    getLocalData(thisJS, localData, result, status);

    DocumentInfo* documentInfo;
    n_getBufferPointer(documentInfo, args[0]);

    DocumentAddress at;
    n_getU64(at, args[1]);

    if (at + sizeOfS16 > documentInfo->length) {
      n_setUndefined(result);
      return result;
    }

    n_newS32(result, *((S16*)(localData->fileData.pointer + documentInfo->start + at)), status);

    return result;
}


function (partSetS16) { // (documentInfoBuffer, number, at)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 4, status);
    getLocalData(thisJS, localData, result, status);

    DocumentInfo* documentInfo;
    n_getBufferPointer(documentInfo, args[0]);

    DocumentAddress at;
    n_getU64(at, args[2]);

    if (at + sizeOfS16 > documentInfo->length) {
      n_setUndefined(result);
      return result;
    }

    n_getS32toS16inMemory(localData->fileData.pointer + documentInfo->start + at, args[1], tempS32);

    n_newBoolean(result, true, status);
    return result;
}



#define methodsS16(obj, methodFunction, status) \
  n_objAssignFunction(obj, methodFunction, addS16, status); \
  n_objAssignFunction(obj, methodFunction, addS16Fixed, status); \
  n_objAssignFunction(obj, methodFunction, getS16, status); \
  n_objAssignFunction(obj, methodFunction, setS16, status); \
  \
  n_objAssignFunction(obj, methodFunction, addS16At, status); \
  n_objAssignFunction(obj, methodFunction, addS16FixedAt, status); \
  n_objAssignFunction(obj, methodFunction, getS16At, status); \
  n_objAssignFunction(obj, methodFunction, setS16At, status); \
  \
  n_objAssignFunction(obj, methodFunction, partGetS16, status); \
  n_objAssignFunction(obj, methodFunction, partSetS16, status); \

