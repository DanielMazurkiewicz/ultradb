#define n_getS64toS48inMemory(address, napiValue, tempValue) \
    S64 tempValue; \
    n_getS64(tempValue, napiValue); \
    *((U32*)(address)) = tempValue; \
    *((U16*)(address + sizeOfU32)) = tempValue >> (sizeOfU32 * 8); \

/*
    can do with mask, because there is a guarantee that always one byte will ba available after document
*/
#define n_newS64fromS48inMemory(napiValue, address, tempValue) \
    S64 tempValue = *((U16*)(address + sizeOfU32)); \
    tempValue <<= sizeOfU32 * 8; \
    tempValue |= *((U32*)(address)); \
    if (tempValue & 0x0000800000000000) { \
        tempValue |= 0xffff000000000000; \
    } \
    n_newS64(napiValue, tempValue, status); \


function (addS48) { // (number)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 1, status);
    getLocalData(thisJS, localData, result, status);

    size_t documentLength = sizeOfS48;
    documentReserveSpaceHidden("addS48", localData, fileData, freeSpace, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, documentLength, documentLengthOffset)

    n_getS64toS48inMemory(localData->fileData.pointer + freeSpace, args[0], tempS64)

    documentUnhide(localData, documentAddress);

    n_newU64(result, documentAddress + localData->fileData.header->baseDescriptor, status);
    return result;
}

function (addS48Fixed) { // (number, length_of_document) - creates document with float 64 bit, reserves extra space for being able to modify document later
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);
    getLocalData(thisJS, localData, result, status);

    size_t documentLength;
    n_getU64(documentLength, args[1]);

    if (sizeOfS48 > documentLength) {
      n_setUndefined(result);
      return result;
    }

    documentReserveSpaceHidden("addS48Fixed", localData, fileData, freeSpace, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, documentLength, documentLengthOffset)

    n_getS64toS48inMemory(localData->fileData.pointer + freeSpace, args[0], tempS64);

    documentUnhide(localData, documentAddress);

    n_newU64(result, documentAddress + localData->fileData.header->baseDescriptor, status);
    return result;
}

function (getS48) { // (documentId)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 1, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor documentId;
    n_getU64(documentId, args[0]);

    documentGetAddressAndFlags("getS48", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentReturnNullIfHidden(documentFlags, result);
    documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
    if (documentLength < sizeOfS48) {
      n_setUndefined(result);
      return result;
    }
    documentAddressStartValidate(documentAddressStart, result);

    n_newS64fromS48inMemory(result, fileData.pointer + documentAddressStart, tempS64);

    return result;
}

function (setS48) { // (documentId, number)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor  documentId;
    n_getU64(documentId, args[0]);

    documentGetAddressAndFlags("setS48", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentReturnNullIfHidden(documentFlags, result);
    documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
    if (sizeOfS48 > documentLength) {
      n_setUndefined(result);
      return result;
    }
    documentAddressStartValidate(documentAddressStart, result);



    n_getS64toS48inMemory(localData->fileData.pointer + documentAddressStart, args[1], tempS64);

    n_newBoolean(result, true, status);
    return result;
}




function (addS48At) { // (number, offset_within_document) - creates document with floar 64 bit number and leaves begining of document empty
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);
    getLocalData(thisJS, localData, result, status);

    DocumentAddress at;
    n_getU64(at, args[1]);

    size_t documentLength =  sizeOfS48 + at;

    documentReserveSpaceHidden("addS48At", localData, fileData, freeSpace, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, documentLength, documentLengthOffset)

    n_getS64toS48inMemory(localData->fileData.pointer + freeSpace + at, args[0], tempS64);

    documentUnhide(localData, documentAddress);

    n_newU64(result, documentAddress + localData->fileData.header->baseDescriptor, status);
    return result;
}


function (addS48FixedAt) { // (number, length_of_document, offset_within_document) - creates document with text, reserves extra space for being able to modify document later
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 4, status);
    getLocalData(thisJS, localData, result, status);

    DocumentAddress at;
    n_getU64(at, args[2]);

    size_t documentLength;
    n_getU64(documentLength, args[1]);

    if (sizeOfS48 + at > documentLength) {
      n_setUndefined(result);
      return result;
    }

    documentReserveSpaceHidden("addS48FixedAt", localData, fileData, freeSpace, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, documentLength, documentLengthOffset)

    n_getS64toS48inMemory(localData->fileData.pointer + freeSpace + at, args[0], tempS64);

    documentUnhide(localData, documentAddress);

    n_newU64(result, documentAddress + localData->fileData.header->baseDescriptor, status);
    return result;
}


function (getS48At) { // (documentId, at)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor  documentId;
    n_getU64(documentId, args[0]);


    documentGetAddressAndFlags("getS48At", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentReturnNullIfHidden(documentFlags, result);
    documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
    documentAddressStartValidate(documentAddressStart, result);

    DocumentAddress at;
    n_getU64(at, args[1]);

    if (sizeOfS48 + at > documentLength) {
      n_setUndefined(result);
      return result;
    }

    n_newS64fromS48inMemory(result, fileData.pointer + documentAddressStart + at, tempS64);

    return result;
}


function (setS48At) { // (documentId, number, at)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 3, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor  documentId;
    n_getU64(documentId, args[0]);

    documentGetAddressAndFlags("setS48At", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentReturnNullIfHidden(documentFlags, result);
    documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
    documentAddressStartValidate(documentAddressStart, result);


    DocumentAddress at;
    n_getU64(at, args[2]);

    if (sizeOfS48 + at > documentLength) {
      n_setUndefined(result);
      return result;
    }

    n_getS64toS48inMemory(localData->fileData.pointer + documentAddressStart + at, args[1], tempS64);

    n_newBoolean(result, true, status);
    return result;
}


function (partGetS48) { // (documentInfoBuffer, at)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);

    getLocalData(thisJS, localData, result, status);

    DocumentInfo* documentInfo;
    n_getBufferPointer(documentInfo, args[0]);

    DocumentAddress at;
    n_getU64(at, args[1]);

    if (at + sizeOfS48 > documentInfo->length) {
      n_setUndefined(result);
      return result;
    }

    n_newS64fromS48inMemory(result, localData->fileData.pointer + documentInfo->start + at, tempS64);

    return result;
}


function (partSetS48) { // (documentInfoBuffer, number, at)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 4, status);
    getLocalData(thisJS, localData, result, status);

    DocumentInfo* documentInfo;
    n_getBufferPointer(documentInfo, args[0]);

    DocumentAddress at;
    n_getU64(at, args[2]);

    if (at + sizeOfS48 > documentInfo->length) {
      n_setUndefined(result);
      return result;
    }

    n_getS64toS48inMemory(localData->fileData.pointer + documentInfo->start + at, args[1], tempS64);

    n_newBoolean(result, true, status);
    return result;
}



#define methodsS48(obj, methodFunction, status) \
  n_objAssignFunction(obj, methodFunction, addS48, status); \
  n_objAssignFunction(obj, methodFunction, addS48Fixed, status); \
  n_objAssignFunction(obj, methodFunction, getS48, status); \
  n_objAssignFunction(obj, methodFunction, setS48, status); \
  \
  n_objAssignFunction(obj, methodFunction, addS48At, status); \
  n_objAssignFunction(obj, methodFunction, addS48FixedAt, status); \
  n_objAssignFunction(obj, methodFunction, getS48At, status); \
  n_objAssignFunction(obj, methodFunction, setS48At, status); \
  \
  n_objAssignFunction(obj, methodFunction, partGetS48, status); \
  n_objAssignFunction(obj, methodFunction, partSetS48, status); \

