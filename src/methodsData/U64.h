

function (addU64) { // (number)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 1, status);
    getLocalData(thisJS, localData, result, status);

    size_t documentLength = sizeOfU64;
    documentReserveSpaceHidden("addU64", localData, fileData, freeSpace, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, documentLength, documentLengthOffset)

    n_getU64toMem((U64*)(localData->fileData.pointer + freeSpace), args[0]);

    documentUnhide(localData, documentAddress);

    n_newU64(result, documentAddress + localData->fileData.header->baseDescriptor, status);
    return result;
}

function (addU64Fixed) { // (number, length_of_document) - creates document with float 64 bit, reserves extra space for being able to modify document later
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);
    getLocalData(thisJS, localData, result, status);

    size_t documentLength;
    n_getU64(documentLength, args[1]);

    if (sizeOfU64 > documentLength) {
      n_setUndefined(result);
      return result;
    }

    documentReserveSpaceHidden("addU64Fixed", localData, fileData, freeSpace, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, documentLength, documentLengthOffset)

    n_getU64toMem((U64*)(localData->fileData.pointer + freeSpace), args[0]);

    documentUnhide(localData, documentAddress);

    n_newU64(result, documentAddress + localData->fileData.header->baseDescriptor, status);
    return result;
}

function (getU64) { // (documentId)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 1, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor documentId;
    n_getU64(documentId, args[0]);

    documentGetAddressAndFlags("getU64", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentReturnNullIfHidden(documentFlags, result);
    documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
    if (documentLength < sizeOfU64) {
      n_setUndefined(result);
      return result;
    }
    documentAddressStartValidate(documentAddressStart, result);

    n_newU64(result, *((U64*)(fileData.pointer + documentAddressStart)), status);

    return result;
}

function (setU64) { // (documentId, number)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor  documentId;
    n_getU64(documentId, args[0]);

    documentGetAddressAndFlags("setU64", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentReturnNullIfHidden(documentFlags, result);
    documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
    if (sizeOfU64 > documentLength) {
      n_setUndefined(result);
      return result;
    }
    documentAddressStartValidate(documentAddressStart, result);



    n_getU64toMem((U64*)(localData->fileData.pointer + documentAddressStart), args[1]);

    n_newBoolean(result, true, status);
    return result;
}




function (addU64At) { // (number, offset_within_document) - creates document with floar 64 bit number and leaves begining of document empty
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);
    getLocalData(thisJS, localData, result, status);

    DocumentAddress at;
    n_getU64(at, args[1]);

    size_t documentLength =  sizeOfU64 + at;

    documentReserveSpaceHidden("addU64At", localData, fileData, freeSpace, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, documentLength, documentLengthOffset)

    n_getU64toMem((U64*)(localData->fileData.pointer + freeSpace + at), args[0]);

    documentUnhide(localData, documentAddress);

    n_newU64(result, documentAddress + localData->fileData.header->baseDescriptor, status);
    return result;
}


function (addU64FixedAt) { // (number, length_of_document, offset_within_document) - creates document with text, reserves extra space for being able to modify document later
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 4, status);
    getLocalData(thisJS, localData, result, status);

    DocumentAddress at;
    n_getU64(at, args[2]);

    size_t documentLength;
    n_getU64(documentLength, args[1]);

    if (sizeOfU64 + at > documentLength) {
      n_setUndefined(result);
      return result;
    }

    documentReserveSpaceHidden("addU64FixedAt", localData, fileData, freeSpace, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, documentLength, documentLengthOffset)

    n_getU64toMem((U64*)(localData->fileData.pointer + freeSpace + at), args[0]);

    documentUnhide(localData, documentAddress);

    n_newU64(result, documentAddress + localData->fileData.header->baseDescriptor, status);
    return result;
}


function (getU64At) { // (documentId, at)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor  documentId;
    n_getU64(documentId, args[0]);


    documentGetAddressAndFlags("getU64At", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentReturnNullIfHidden(documentFlags, result);
    documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
    documentAddressStartValidate(documentAddressStart, result);

    DocumentAddress at;
    n_getU64(at, args[1]);

    if (sizeOfU64 + at > documentLength) {
      n_setUndefined(result);
      return result;
    }

    n_newU64(result, *((U64*)(fileData.pointer + documentAddressStart + at)), status);

    return result;
}


function (setU64At) { // (documentId, number, at)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 3, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor  documentId;
    n_getU64(documentId, args[0]);

    documentGetAddressAndFlags("setU64At", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentReturnNullIfHidden(documentFlags, result);
    documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
    documentAddressStartValidate(documentAddressStart, result);


    DocumentAddress at;
    n_getU64(at, args[2]);

    if (sizeOfU64 + at > documentLength) {
      n_setUndefined(result);
      return result;
    }

    n_getU64toMem((U64*)(localData->fileData.pointer + documentAddressStart + at), args[1]);

    n_newBoolean(result, true, status);
    return result;
}


function (partGetU64) { // (documentInfoBuffer, at)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);

    getLocalData(thisJS, localData, result, status);

    DocumentInfo* documentInfo;
    n_getBufferPointer(documentInfo, args[0]);

    DocumentAddress at;
    n_getU64(at, args[1]);

    if (at + sizeOfU64 > documentInfo->length) {
      n_setUndefined(result);
      return result;
    }

    n_newU64(result, *((U64*)(localData->fileData.pointer + documentInfo->start + at)), status);

    return result;
}


function (partSetU64) { // (documentInfoBuffer, number, at)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 4, status);
    getLocalData(thisJS, localData, result, status);

    DocumentInfo* documentInfo;
    n_getBufferPointer(documentInfo, args[0]);

    DocumentAddress at;
    n_getU64(at, args[2]);

    if (at + sizeOfU64 > documentInfo->length) {
      n_setUndefined(result);
      return result;
    }

    n_getU64toMem((U64*)(localData->fileData.pointer + documentInfo->start + at), args[1], status);

    n_newBoolean(result, true, status);
    return result;
}



#define methodsU64(obj, methodFunction, status) \
  n_objAssignFunction(obj, methodFunction, addU64, status); \
  n_objAssignFunction(obj, methodFunction, addU64Fixed, status); \
  n_objAssignFunction(obj, methodFunction, getU64, status); \
  n_objAssignFunction(obj, methodFunction, setU64, status); \
  \
  n_objAssignFunction(obj, methodFunction, addU64At, status); \
  n_objAssignFunction(obj, methodFunction, addU64FixedAt, status); \
  n_objAssignFunction(obj, methodFunction, getU64At, status); \
  n_objAssignFunction(obj, methodFunction, setU64At, status); \
  \
  n_objAssignFunction(obj, methodFunction, partGetU64, status); \
  n_objAssignFunction(obj, methodFunction, partSetU64, status); \


