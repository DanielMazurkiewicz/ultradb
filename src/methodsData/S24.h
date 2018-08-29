#define n_getS32toS24inMemory(address, napiValue, tempValue) \
    S32 tempValue; \
    n_getS32(tempValue, napiValue); \
    *((U16*)(address)) = tempValue; \
    *((U8*)(address + sizeOfU16)) = tempValue >> (sizeOfU16 * 8); \

/*
    can do with mask, because there is a guarantee that always one byte will ba available after document
*/
#define n_newS32fromS24inMemory(napiValue, address, tempValue) \
    S32 tempValue = *((S32*)(address)); \
    if (tempValue & 0x00800000) { \
        tempValue |= 0xff000000; \
    } else { \
        tempValue &= 0x00ffffff; \
    } \
    n_newS32(napiValue, tempValue, status); \


function (addS24) { // (number)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 1, status);
    getLocalData(thisJS, localData, result, status);

    size_t documentLength = sizeOfS24;
    documentReserveSpaceHidden("addS24", localData, fileData, freeSpace, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, documentLength, documentLengthOffset)

    n_getS32toS24inMemory(localData->fileData.pointer + freeSpace, args[0], tempS32)

    documentUnhide(localData, documentAddress);

    n_newU64(result, documentAddress + localData->fileData.header->baseDescriptor, status);
    return result;
}

function (addS24Fixed) { // (number, length_of_document) - creates document with float 64 bit, reserves extra space for being able to modify document later
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);
    getLocalData(thisJS, localData, result, status);

    size_t documentLength;
    n_getU64(documentLength, args[1]);

    if (sizeOfS24 > documentLength) {
      n_setUndefined(result);
      return result;
    }

    documentReserveSpaceHidden("addS24Fixed", localData, fileData, freeSpace, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, documentLength, documentLengthOffset)

    n_getS32toS24inMemory(localData->fileData.pointer + freeSpace, args[0], tempS32);

    documentUnhide(localData, documentAddress);

    n_newU64(result, documentAddress + localData->fileData.header->baseDescriptor, status);
    return result;
}

function (getS24) { // (documentId)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 1, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor documentId;
    n_getU64(documentId, args[0]);

    documentGetAddressAndFlags("getS24", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentReturnNullIfHidden(documentFlags, result);
    documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
    if (documentLength < sizeOfS24) {
      n_setUndefined(result);
      return result;
    }
    documentAddressStartValidate(documentAddressStart, result);

    n_newS32fromS24inMemory(result, fileData.pointer + documentAddressStart, tempS32);

    return result;
}

function (setS24) { // (documentId, number)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor  documentId;
    n_getU64(documentId, args[0]);

    documentGetAddressAndFlags("setS24", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentReturnNullIfHidden(documentFlags, result);
    documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
    if (sizeOfS24 > documentLength) {
      n_setUndefined(result);
      return result;
    }
    documentAddressStartValidate(documentAddressStart, result);



    n_getS32toS24inMemory(localData->fileData.pointer + documentAddressStart, args[1], tempS32);

    n_newBoolean(result, true, status);
    return result;
}




function (addS24At) { // (number, offset_within_document) - creates document with floar 64 bit number and leaves begining of document empty
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);
    getLocalData(thisJS, localData, result, status);

    DocumentAddress at;
    n_getU64(at, args[1]);

    size_t documentLength =  sizeOfS24 + at;

    documentReserveSpaceHidden("addS24At", localData, fileData, freeSpace, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, documentLength, documentLengthOffset)

    n_getS32toS24inMemory(localData->fileData.pointer + freeSpace + at, args[0], tempS32);

    documentUnhide(localData, documentAddress);

    n_newU64(result, documentAddress + localData->fileData.header->baseDescriptor, status);
    return result;
}


function (addS24FixedAt) { // (number, length_of_document, offset_within_document) - creates document with text, reserves extra space for being able to modify document later
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 4, status);
    getLocalData(thisJS, localData, result, status);

    DocumentAddress at;
    n_getU64(at, args[2]);

    size_t documentLength;
    n_getU64(documentLength, args[1]);

    if (sizeOfS24 + at > documentLength) {
      n_setUndefined(result);
      return result;
    }

    documentReserveSpaceHidden("addS24FixedAt", localData, fileData, freeSpace, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, documentLength, documentLengthOffset)

    n_getS32toS24inMemory(localData->fileData.pointer + freeSpace + at, args[0], tempS32);

    documentUnhide(localData, documentAddress);

    n_newU64(result, documentAddress + localData->fileData.header->baseDescriptor, status);
    return result;
}


function (getS24At) { // (documentId, at)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor  documentId;
    n_getU64(documentId, args[0]);


    documentGetAddressAndFlags("getS24At", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentReturnNullIfHidden(documentFlags, result);
    documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
    documentAddressStartValidate(documentAddressStart, result);

    DocumentAddress at;
    n_getU64(at, args[1]);

    if (sizeOfS24 + at > documentLength) {
      n_setUndefined(result);
      return result;
    }

    n_newS32fromS24inMemory(result, fileData.pointer + documentAddressStart + at, tempS32);

    return result;
}


function (setS24At) { // (documentId, number, at)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 3, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor  documentId;
    n_getU64(documentId, args[0]);

    documentGetAddressAndFlags("setS24At", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentReturnNullIfHidden(documentFlags, result);
    documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
    documentAddressStartValidate(documentAddressStart, result);


    DocumentAddress at;
    n_getU64(at, args[2]);

    if (sizeOfS24 + at > documentLength) {
      n_setUndefined(result);
      return result;
    }

    n_getS32toS24inMemory(localData->fileData.pointer + documentAddressStart + at, args[1], tempS32);

    n_newBoolean(result, true, status);
    return result;
}


function (partGetS24) { // (documentInfoBuffer, at)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);

    getLocalData(thisJS, localData, result, status);

    DocumentInfo* documentInfo;
    n_getBufferPointer(documentInfo, args[0]);

    DocumentAddress at;
    n_getU64(at, args[1]);

    if (at + sizeOfS24 > documentInfo->length) {
      n_setUndefined(result);
      return result;
    }

    n_newS32fromS24inMemory(result, localData->fileData.pointer + documentInfo->start + at, tempS32);

    return result;
}


function (partSetS24) { // (documentInfoBuffer, number, at)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 4, status);
    getLocalData(thisJS, localData, result, status);

    DocumentInfo* documentInfo;
    n_getBufferPointer(documentInfo, args[0]);

    DocumentAddress at;
    n_getU64(at, args[2]);

    if (at + sizeOfS24 > documentInfo->length) {
      n_setUndefined(result);
      return result;
    }

    n_getS32toS24inMemory(localData->fileData.pointer + documentInfo->start + at, args[1], tempS32);

    n_newBoolean(result, true, status);
    return result;
}



#define methodsS24(obj, methodFunction, status) \
  n_objAssignFunction(obj, methodFunction, addS24, status); \
  n_objAssignFunction(obj, methodFunction, addS24Fixed, status); \
  n_objAssignFunction(obj, methodFunction, getS24, status); \
  n_objAssignFunction(obj, methodFunction, setS24, status); \
  \
  n_objAssignFunction(obj, methodFunction, addS24At, status); \
  n_objAssignFunction(obj, methodFunction, addS24FixedAt, status); \
  n_objAssignFunction(obj, methodFunction, getS24At, status); \
  n_objAssignFunction(obj, methodFunction, setS24At, status); \
  \
  n_objAssignFunction(obj, methodFunction, partGetS24, status); \
  n_objAssignFunction(obj, methodFunction, partSetS24, status); \

