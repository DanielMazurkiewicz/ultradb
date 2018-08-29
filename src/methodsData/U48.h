#define n_getU64toU48inMemory(address, napiValue, tempValue) \
    U64 tempValue; \
    n_getU64(tempValue, napiValue); \
    *((U32*)(address)) = tempValue; \
    *((U16*)(address + sizeOfU32)) = tempValue >> (sizeOfU32 * 8); \


#define n_newU64fromU48inMemory(napiValue, address, tempValue) \
    U64 tempValue = *((U16*)(address + sizeOfU32)); \
    tempValue <<= sizeOfU32 * 8; \
    tempValue |= *((U32*)(address)); \
    n_newU64(napiValue, tempValue, status); \


function (addU48) { // (number)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 1, status);
    getLocalData(thisJS, localData, result, status);

    size_t documentLength = sizeOfU48;
    documentReserveSpaceHidden("addU48", localData, fileData, freeSpace, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, documentLength, documentLengthOffset)

    n_getU64toU48inMemory(localData->fileData.pointer + freeSpace, args[0], tempU64)

    documentUnhide(localData, documentAddress);

    n_newU64(result, documentAddress + localData->fileData.header->baseDescriptor, status);
    return result;
}

function (addU48Fixed) { // (number, length_of_document) - creates document with float 64 bit, reserves extra space for being able to modify document later
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);
    getLocalData(thisJS, localData, result, status);

    size_t documentLength;
    n_getU64(documentLength, args[1]);

    if (sizeOfU48 > documentLength) {
      n_setUndefined(result);
      return result;
    }

    documentReserveSpaceHidden("addU48Fixed", localData, fileData, freeSpace, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, documentLength, documentLengthOffset)

    n_getU64toU48inMemory(localData->fileData.pointer + freeSpace, args[0], tempU64);

    documentUnhide(localData, documentAddress);

    n_newU64(result, documentAddress + localData->fileData.header->baseDescriptor, status);
    return result;
}

function (getU48) { // (documentId)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 1, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor documentId;
    n_getU64(documentId, args[0]);

    documentGetAddressAndFlags("getU48", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentReturnNullIfHidden(documentFlags, result);
    documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
    if (documentLength < sizeOfU48) {
      n_setUndefined(result);
      return result;
    }
    documentAddressStartValidate(documentAddressStart, result);

    n_newU64fromU48inMemory(result, fileData.pointer + documentAddressStart, tempU64);

    return result;
}

function (setU48) { // (documentId, number)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor  documentId;
    n_getU64(documentId, args[0]);

    documentGetAddressAndFlags("setU48", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentReturnNullIfHidden(documentFlags, result);
    documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
    if (sizeOfU48 > documentLength) {
      n_setUndefined(result);
      return result;
    }
    documentAddressStartValidate(documentAddressStart, result);



    n_getU64toU48inMemory(localData->fileData.pointer + documentAddressStart, args[1], tempU64);

    n_newBoolean(result, true, status);
    return result;
}




function (addU48At) { // (number, offset_within_document) - creates document with floar 64 bit number and leaves begining of document empty
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);
    getLocalData(thisJS, localData, result, status);

    DocumentAddress at;
    n_getU64(at, args[1]);

    size_t documentLength =  sizeOfU48 + at;

    documentReserveSpaceHidden("addU48At", localData, fileData, freeSpace, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, documentLength, documentLengthOffset)

    n_getU64toU48inMemory(localData->fileData.pointer + freeSpace + at, args[0], tempU64);

    documentUnhide(localData, documentAddress);

    n_newU64(result, documentAddress + localData->fileData.header->baseDescriptor, status);
    return result;
}


function (addU48FixedAt) { // (number, length_of_document, offset_within_document) - creates document with text, reserves extra space for being able to modify document later
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 4, status);
    getLocalData(thisJS, localData, result, status);

    DocumentAddress at;
    n_getU64(at, args[2]);

    size_t documentLength;
    n_getU64(documentLength, args[1]);

    if (sizeOfU48 + at > documentLength) {
      n_setUndefined(result);
      return result;
    }

    documentReserveSpaceHidden("addU48FixedAt", localData, fileData, freeSpace, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, documentLength, documentLengthOffset)

    n_getU64toU48inMemory(localData->fileData.pointer + freeSpace + at, args[0], tempU64);

    documentUnhide(localData, documentAddress);

    n_newU64(result, documentAddress + localData->fileData.header->baseDescriptor, status);
    return result;
}


function (getU48At) { // (documentId, at)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor  documentId;
    n_getU64(documentId, args[0]);


    documentGetAddressAndFlags("getU48At", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentReturnNullIfHidden(documentFlags, result);
    documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
    documentAddressStartValidate(documentAddressStart, result);

    DocumentAddress at;
    n_getU64(at, args[1]);

    if (sizeOfU48 + at > documentLength) {
      n_setUndefined(result);
      return result;
    }

    n_newU64fromU48inMemory(result, fileData.pointer + documentAddressStart + at, tempU64);

    return result;
}


function (setU48At) { // (documentId, number, at)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 3, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor  documentId;
    n_getU64(documentId, args[0]);

    documentGetAddressAndFlags("setU48At", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentReturnNullIfHidden(documentFlags, result);
    documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
    documentAddressStartValidate(documentAddressStart, result);


    DocumentAddress at;
    n_getU64(at, args[2]);

    if (sizeOfU48 + at > documentLength) {
      n_setUndefined(result);
      return result;
    }

    n_getU64toU48inMemory(localData->fileData.pointer + documentAddressStart + at, args[1], tempU64);

    n_newBoolean(result, true, status);
    return result;
}


function (partGetU48) { // (documentInfoBuffer, at)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);

    getLocalData(thisJS, localData, result, status);

    DocumentInfo* documentInfo;
    n_getBufferPointer(documentInfo, args[0]);

    DocumentAddress at;
    n_getU64(at, args[1]);

    if (at + sizeOfU48 > documentInfo->length) {
      n_setUndefined(result);
      return result;
    }

    n_newU64fromU48inMemory(result, localData->fileData.pointer + documentInfo->start + at, tempU64);

    return result;
}


function (partSetU48) { // (documentInfoBuffer, number, at)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 4, status);
    getLocalData(thisJS, localData, result, status);

    DocumentInfo* documentInfo;
    n_getBufferPointer(documentInfo, args[0]);

    DocumentAddress at;
    n_getU64(at, args[2]);

    if (at + sizeOfU48 > documentInfo->length) {
      n_setUndefined(result);
      return result;
    }

    n_getU64toU48inMemory(localData->fileData.pointer + documentInfo->start + at, args[1], tempU64);

    n_newBoolean(result, true, status);
    return result;
}



#define methodsU48(obj, methodFunction, status) \
  n_objAssignFunction(obj, methodFunction, addU48, status); \
  n_objAssignFunction(obj, methodFunction, addU48Fixed, status); \
  n_objAssignFunction(obj, methodFunction, getU48, status); \
  n_objAssignFunction(obj, methodFunction, setU48, status); \
  \
  n_objAssignFunction(obj, methodFunction, addU48At, status); \
  n_objAssignFunction(obj, methodFunction, addU48FixedAt, status); \
  n_objAssignFunction(obj, methodFunction, getU48At, status); \
  n_objAssignFunction(obj, methodFunction, setU48At, status); \
  \
  n_objAssignFunction(obj, methodFunction, partGetU48, status); \
  n_objAssignFunction(obj, methodFunction, partSetU48, status); \

