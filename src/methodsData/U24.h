#define n_getU32toU24inMemory(address, napiValue, tempValue) \
    U32 tempValue; \
    n_getU32(tempValue, napiValue); \
    *((U16*)(address)) = tempValue; \
    *((U8*)(address + sizeOfU16)) = tempValue >> (sizeOfU16 * 8); \

/*
    can do with mask, because there is a guarantee that always one byte will ba available after document
*/
#define n_newU32fromU24inMemory(napiValue, address, tempValue) \
    U32 tempValue = *((U32*)(address)); \
    tempValue &= 0x00ffffff; \
    n_newU32(napiValue, tempValue, status); \


function (addU24) { // (number)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 1, status);
    getLocalData(thisJS, localData, result, status);

    size_t documentLength = sizeOfU24;
    documentReserveSpaceHidden("addU24", localData, fileData, freeSpace, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, documentLength, documentLengthOffset)

    n_getU32toU24inMemory(localData->fileData.pointer + freeSpace, args[0], tempU32)

    documentUnhide(localData, documentAddress);

    n_newU64(result, documentAddress + localData->fileData.header->baseDescriptor, status);
    return result;
}

function (addU24Fixed) { // (number, length_of_document) - creates document with float 64 bit, reserves extra space for being able to modify document later
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);
    getLocalData(thisJS, localData, result, status);

    size_t documentLength;
    n_getU64(documentLength, args[1]);

    if (sizeOfU24 > documentLength) {
      n_setUndefined(result);
      return result;
    }

    documentReserveSpaceHidden("addU24Fixed", localData, fileData, freeSpace, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, documentLength, documentLengthOffset)

    n_getU32toU24inMemory(localData->fileData.pointer + freeSpace, args[0], tempU32);

    documentUnhide(localData, documentAddress);

    n_newU64(result, documentAddress + localData->fileData.header->baseDescriptor, status);
    return result;
}

function (getU24) { // (documentId)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 1, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor documentId;
    n_getU64(documentId, args[0]);

    documentGetAddressAndFlags("getU24", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentReturnNullIfHidden(documentFlags, result);
    documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
    if (documentLength < sizeOfU24) {
      n_setUndefined(result);
      return result;
    }
    documentAddressStartValidate(documentAddressStart, result);

    n_newU32fromU24inMemory(result, fileData.pointer + documentAddressStart, tempU32);

    return result;
}

function (setU24) { // (documentId, number)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor  documentId;
    n_getU64(documentId, args[0]);

    documentGetAddressAndFlags("setU24", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentReturnNullIfHidden(documentFlags, result);
    documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
    if (sizeOfU24 > documentLength) {
      n_setUndefined(result);
      return result;
    }
    documentAddressStartValidate(documentAddressStart, result);



    n_getU32toU24inMemory(localData->fileData.pointer + documentAddressStart, args[1], tempU32);

    n_newBoolean(result, true, status);
    return result;
}




function (addU24At) { // (number, offset_within_document) - creates document with floar 64 bit number and leaves begining of document empty
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);
    getLocalData(thisJS, localData, result, status);

    DocumentAddress at;
    n_getU64(at, args[1]);

    size_t documentLength =  sizeOfU24 + at;

    documentReserveSpaceHidden("addU24At", localData, fileData, freeSpace, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, documentLength, documentLengthOffset)

    n_getU32toU24inMemory(localData->fileData.pointer + freeSpace + at, args[0], tempU32);

    documentUnhide(localData, documentAddress);

    n_newU64(result, documentAddress + localData->fileData.header->baseDescriptor, status);
    return result;
}


function (addU24FixedAt) { // (number, length_of_document, offset_within_document) - creates document with text, reserves extra space for being able to modify document later
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 4, status);
    getLocalData(thisJS, localData, result, status);

    DocumentAddress at;
    n_getU64(at, args[2]);

    size_t documentLength;
    n_getU64(documentLength, args[1]);

    if (sizeOfU24 + at > documentLength) {
      n_setUndefined(result);
      return result;
    }

    documentReserveSpaceHidden("addU24FixedAt", localData, fileData, freeSpace, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, documentLength, documentLengthOffset)

    n_getU32toU24inMemory(localData->fileData.pointer + freeSpace + at, args[0], tempU32);

    documentUnhide(localData, documentAddress);

    n_newU64(result, documentAddress + localData->fileData.header->baseDescriptor, status);
    return result;
}


function (getU24At) { // (documentId, at)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor  documentId;
    n_getU64(documentId, args[0]);


    documentGetAddressAndFlags("getU24At", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentReturnNullIfHidden(documentFlags, result);
    documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
    documentAddressStartValidate(documentAddressStart, result);

    DocumentAddress at;
    n_getU64(at, args[1]);

    if (sizeOfU24 + at > documentLength) {
      n_setUndefined(result);
      return result;
    }

    n_newU32fromU24inMemory(result, fileData.pointer + documentAddressStart + at, tempU32);

    return result;
}


function (setU24At) { // (documentId, number, at)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 3, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor  documentId;
    n_getU64(documentId, args[0]);

    documentGetAddressAndFlags("setU24At", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentReturnNullIfHidden(documentFlags, result);
    documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
    documentAddressStartValidate(documentAddressStart, result);


    DocumentAddress at;
    n_getU64(at, args[2]);

    if (sizeOfU24 + at > documentLength) {
      n_setUndefined(result);
      return result;
    }

    n_getU32toU24inMemory(localData->fileData.pointer + documentAddressStart + at, args[1], tempU32);

    n_newBoolean(result, true, status);
    return result;
}


function (partGetU24) { // (documentInfoBuffer, at)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);

    getLocalData(thisJS, localData, result, status);

    DocumentInfo* documentInfo;
    n_getBufferPointer(documentInfo, args[0]);

    DocumentAddress at;
    n_getU64(at, args[1]);

    if (at + sizeOfU24 > documentInfo->length) {
      n_setUndefined(result);
      return result;
    }

    n_newU32fromU24inMemory(result, localData->fileData.pointer + documentInfo->start + at, tempU32);

    return result;
}


function (partSetU24) { // (documentInfoBuffer, number, at)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 4, status);
    getLocalData(thisJS, localData, result, status);

    DocumentInfo* documentInfo;
    n_getBufferPointer(documentInfo, args[0]);

    DocumentAddress at;
    n_getU64(at, args[2]);

    if (at + sizeOfU24 > documentInfo->length) {
      n_setUndefined(result);
      return result;
    }

    n_getU32toU24inMemory(localData->fileData.pointer + documentInfo->start + at, args[1], tempU32);

    n_newBoolean(result, true, status);
    return result;
}



#define methodsU24(obj, methodFunction, status) \
  n_objAssignFunction(obj, methodFunction, addU24, status); \
  n_objAssignFunction(obj, methodFunction, addU24Fixed, status); \
  n_objAssignFunction(obj, methodFunction, getU24, status); \
  n_objAssignFunction(obj, methodFunction, setU24, status); \
  \
  n_objAssignFunction(obj, methodFunction, addU24At, status); \
  n_objAssignFunction(obj, methodFunction, addU24FixedAt, status); \
  n_objAssignFunction(obj, methodFunction, getU24At, status); \
  n_objAssignFunction(obj, methodFunction, setU24At, status); \
  \
  n_objAssignFunction(obj, methodFunction, partGetU24, status); \
  n_objAssignFunction(obj, methodFunction, partSetU24, status); \

