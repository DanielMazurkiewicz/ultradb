#define n_getU32toU16inMemory(address, napiValue, tempValue) \
    U32 tempValue; \
    n_getU32(tempValue, napiValue); \
    *((U16*)(address)) = tempValue; \


function (addU16) { // (number)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 1, status);
    getLocalData(thisJS, localData, result, status);

    size_t documentLength = sizeOfU16;
    documentReserveSpaceHidden("addU16", localData, fileData, freeSpace, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, documentLength, documentLengthOffset)

    n_getU32toU16inMemory(localData->fileData.pointer + freeSpace, args[0], tempU32)

    documentUnhide(localData, documentAddress);

    n_newU64(result, documentAddress + localData->fileData.header->baseDescriptor, status);
    return result;
}

function (addU16Fixed) { // (number, length_of_document) - creates document with float 64 bit, reserves extra space for being able to modify document later
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);
    getLocalData(thisJS, localData, result, status);

    size_t documentLength;
    n_getU64(documentLength, args[1]);

    if (sizeOfU16 > documentLength) {
      n_setUndefined(result);
      return result;
    }

    documentReserveSpaceHidden("addU16Fixed", localData, fileData, freeSpace, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, documentLength, documentLengthOffset)

    n_getU32toU16inMemory(localData->fileData.pointer + freeSpace, args[0], tempU32);

    documentUnhide(localData, documentAddress);

    n_newU64(result, documentAddress + localData->fileData.header->baseDescriptor, status);
    return result;
}

function (getU16) { // (documentId)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 1, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor documentId;
    n_getU64(documentId, args[0]);

    documentGetAddressAndFlags("getU16", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentReturnNullIfHidden(documentFlags, result);
    documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
    if (documentLength < sizeOfU16) {
      n_setUndefined(result);
      return result;
    }
    documentAddressStartValidate(documentAddressStart, result);

    n_newU32(result, *((U16*)(fileData.pointer + documentAddressStart)), status);

    return result;
}

function (setU16) { // (documentId, number)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor  documentId;
    n_getU64(documentId, args[0]);

    documentGetAddressAndFlags("setU16", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentReturnNullIfHidden(documentFlags, result);
    documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
    if (sizeOfU16 > documentLength) {
      n_setUndefined(result);
      return result;
    }
    documentAddressStartValidate(documentAddressStart, result);



    n_getU32toU16inMemory(localData->fileData.pointer + documentAddressStart, args[1], tempU32);

    n_newBoolean(result, true, status);
    return result;
}




function (addU16At) { // (number, offset_within_document) - creates document with floar 64 bit number and leaves begining of document empty
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);
    getLocalData(thisJS, localData, result, status);

    DocumentAddress at;
    n_getU64(at, args[1]);

    size_t documentLength =  sizeOfU16 + at;

    documentReserveSpaceHidden("addU16At", localData, fileData, freeSpace, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, documentLength, documentLengthOffset)

    n_getU32toU16inMemory(localData->fileData.pointer + freeSpace + at, args[0], tempU32);

    documentUnhide(localData, documentAddress);

    n_newU64(result, documentAddress + localData->fileData.header->baseDescriptor, status);
    return result;
}


function (addU16FixedAt) { // (number, length_of_document, offset_within_document) - creates document with text, reserves extra space for being able to modify document later
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 4, status);
    getLocalData(thisJS, localData, result, status);

    DocumentAddress at;
    n_getU64(at, args[2]);

    size_t documentLength;
    n_getU64(documentLength, args[1]);

    if (sizeOfU16 + at > documentLength) {
      n_setUndefined(result);
      return result;
    }

    documentReserveSpaceHidden("addU16FixedAt", localData, fileData, freeSpace, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, documentLength, documentLengthOffset)

    n_getU32toU16inMemory(localData->fileData.pointer + freeSpace + at, args[0], tempU32);

    documentUnhide(localData, documentAddress);

    n_newU64(result, documentAddress + localData->fileData.header->baseDescriptor, status);
    return result;
}


function (getU16At) { // (documentId, at)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor  documentId;
    n_getU64(documentId, args[0]);


    documentGetAddressAndFlags("getU16At", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentReturnNullIfHidden(documentFlags, result);
    documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
    documentAddressStartValidate(documentAddressStart, result);

    DocumentAddress at;
    n_getU64(at, args[1]);

    if (sizeOfU16 + at > documentLength) {
      n_setUndefined(result);
      return result;
    }

    n_newU32(result, *((U16*)(fileData.pointer + documentAddressStart + at)), status);

    return result;
}


function (setU16At) { // (documentId, number, at)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 3, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor  documentId;
    n_getU64(documentId, args[0]);

    documentGetAddressAndFlags("setU16At", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentReturnNullIfHidden(documentFlags, result);
    documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
    documentAddressStartValidate(documentAddressStart, result);


    DocumentAddress at;
    n_getU64(at, args[2]);

    if (sizeOfU16 + at > documentLength) {
      n_setUndefined(result);
      return result;
    }

    n_getU32toU16inMemory(localData->fileData.pointer + documentAddressStart + at, args[1], tempU32);

    n_newBoolean(result, true, status);
    return result;
}


function (partGetU16) { // (documentInfoBuffer, at)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);

    getLocalData(thisJS, localData, result, status);

    DocumentInfo* documentInfo;
    n_getBufferPointer(documentInfo, args[0]);

    DocumentAddress at;
    n_getU64(at, args[1]);

    if (at + sizeOfU16 > documentInfo->length) {
      n_setUndefined(result);
      return result;
    }

    n_newU32(result, *((U16*)(localData->fileData.pointer + documentInfo->start + at)), status);

    return result;
}


function (partSetU16) { // (documentInfoBuffer, number, at)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 4, status);
    getLocalData(thisJS, localData, result, status);

    DocumentInfo* documentInfo;
    n_getBufferPointer(documentInfo, args[0]);

    DocumentAddress at;
    n_getU64(at, args[2]);

    if (at + sizeOfU16 > documentInfo->length) {
      n_setUndefined(result);
      return result;
    }

    n_getU32toU16inMemory(localData->fileData.pointer + documentInfo->start + at, args[1], tempU32);

    n_newBoolean(result, true, status);
    return result;
}



#define methodsU16(obj, methodFunction, status) \
  n_objAssignFunction(obj, methodFunction, addU16, status); \
  n_objAssignFunction(obj, methodFunction, addU16Fixed, status); \
  n_objAssignFunction(obj, methodFunction, getU16, status); \
  n_objAssignFunction(obj, methodFunction, setU16, status); \
  \
  n_objAssignFunction(obj, methodFunction, addU16At, status); \
  n_objAssignFunction(obj, methodFunction, addU16FixedAt, status); \
  n_objAssignFunction(obj, methodFunction, getU16At, status); \
  n_objAssignFunction(obj, methodFunction, setU16At, status); \
  \
  n_objAssignFunction(obj, methodFunction, partGetU16, status); \
  n_objAssignFunction(obj, methodFunction, partSetU16, status); \

