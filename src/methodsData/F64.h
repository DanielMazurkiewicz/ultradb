

function (addF64) { // (number)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 1, status);
    getLocalData(thisJS, localData, result, status);

    size_t documentLength = sizeOfF64;
    documentReserveSpaceHidden("addF64", localData, fileData, freeSpace, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, documentLength, documentLengthOffset)

    n_getF64toMem((F64*)(localData->fileData.pointer + freeSpace), args[0]);

    documentUnhide(localData, documentAddress);

    n_newU64(result, documentAddress + localData->fileData.header->baseDescriptor, status);
    return result;
}

function (addF64Fixed) { // (number, length_of_document) - creates document with float 64 bit, reserves extra space for being able to modify document later
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);
    getLocalData(thisJS, localData, result, status);

    size_t documentLength;
    n_getU64(documentLength, args[1]);

    if (sizeOfF64 > documentLength) {
      n_setUndefined(result);
      return result;
    }

    documentReserveSpaceHidden("addF64Fixed", localData, fileData, freeSpace, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, documentLength, documentLengthOffset)

    n_getF64toMem((F64*)(localData->fileData.pointer + freeSpace), args[0]);

    documentUnhide(localData, documentAddress);

    n_newS64(result, documentAddress + localData->fileData.header->baseDescriptor, status);
    return result;
}

function (getF64) { // (documentId)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 1, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor documentId;
    n_getU64(documentId, args[0]);

    documentGetAddressAndFlags("getF64", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentReturnNullIfHidden(documentFlags, result);
    documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
    if (documentLength < sizeOfF64) {
      n_setUndefined(result);
      return result;
    }
    documentAddressStartValidate(documentAddressStart, result);

    n_newF64(result, *((F64*)(fileData.pointer + documentAddressStart)), status);

    return result;
}

function (setF64) { // (documentId, number)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor  documentId;
    n_getU64(documentId, args[0]);

    documentGetAddressAndFlags("setF64", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentReturnNullIfHidden(documentFlags, result);
    documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
    if (sizeOfF64 > documentLength) {
      n_setUndefined(result);
      return result;
    }
    documentAddressStartValidate(documentAddressStart, result);



    n_getF64toMem((F64*)(localData->fileData.pointer + documentAddressStart), args[1]);

    n_newBoolean(result, true, status);
    return result;
}




function (addF64At) { // (number, offset_within_document) - creates document with floar 64 bit number and leaves begining of document empty
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);
    getLocalData(thisJS, localData, result, status);

    DocumentAddress at;
    n_getU64(at, args[1]);

    size_t documentLength =  sizeOfF64 + at;

    documentReserveSpaceHidden("addF64At", localData, fileData, freeSpace, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, documentLength, documentLengthOffset)

    n_getF64toMem((F64*)(localData->fileData.pointer + freeSpace + at), args[0]);

    documentUnhide(localData, documentAddress);

    n_newU64(result, documentAddress + localData->fileData.header->baseDescriptor, status);
    return result;
}


function (addF64FixedAt) { // (number, length_of_document, offset_within_document) - creates document with text, reserves extra space for being able to modify document later
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 4, status);
    getLocalData(thisJS, localData, result, status);

    DocumentAddress at;
    n_getU64(at, args[2]);

    size_t documentLength;
    n_getU64(documentLength, args[1]);

    if (sizeOfF64 + at > documentLength) {
      n_setUndefined(result);
      return result;
    }

    documentReserveSpaceHidden("addF64FixedAt", localData, fileData, freeSpace, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, documentLength, documentLengthOffset)

    n_getF64toMem((F64*)(localData->fileData.pointer + freeSpace + at), args[0]);

    documentUnhide(localData, documentAddress);

    n_newU64(result, documentAddress + localData->fileData.header->baseDescriptor, status);
    return result;
}


function (getF64At) { // (documentId, at)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor  documentId;
    n_getU64(documentId, args[0]);


    documentGetAddressAndFlags("getF64At", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentReturnNullIfHidden(documentFlags, result);
    documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
    documentAddressStartValidate(documentAddressStart, result);

    DocumentAddress at;
    n_getU64(at, args[1]);

    if (sizeOfF64 + at > documentLength) {
      n_setUndefined(result);
      return result;
    }

    n_newF64(result, *((F64*)(fileData.pointer + documentAddressStart + at)), status);

    return result;
}


function (setF64At) { // (documentId, number, at)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 3, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor  documentId;
    n_getU64(documentId, args[0]);

    documentGetAddressAndFlags("setF64At", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentReturnNullIfHidden(documentFlags, result);
    documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
    documentAddressStartValidate(documentAddressStart, result);


    DocumentAddress at;
    n_getU64(at, args[2]);

    if (sizeOfF64 + at > documentLength) {
      n_setUndefined(result);
      return result;
    }

    n_getF64toMem((F64*)(localData->fileData.pointer + documentAddressStart + at), args[1]);

    n_newBoolean(result, true, status);
    return result;
}


function (partGetF64) { // (documentInfoBuffer, at)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);

    getLocalData(thisJS, localData, result, status);

    DocumentInfo* documentInfo;
    n_getBufferPointer(documentInfo, args[0]);

    DocumentAddress at;
    n_getU64(at, args[1]);

    if (at + sizeOfF64 > documentInfo->length) {
      n_setUndefined(result);
      return result;
    }

    n_newF64(result, *((F64*)(localData->fileData.pointer + documentInfo->start + at)), status);

    return result;
}


function (partSetF64) { // (documentInfoBuffer, number, at)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 4, status);
    getLocalData(thisJS, localData, result, status);

    DocumentInfo* documentInfo;
    n_getBufferPointer(documentInfo, args[0]);

    DocumentAddress at;
    n_getU64(at, args[2]);

    if (at + sizeOfF64 > documentInfo->length) {
      n_setUndefined(result);
      return result;
    }

    n_getF64toMem((F64*)(localData->fileData.pointer + documentInfo->start + at), args[1], status);

    n_newBoolean(result, true, status);
    return result;
}



#define methodsF64(obj, methodFunction, status) \
  n_objAssignFunction(obj, methodFunction, addF64, status); \
  n_objAssignFunction(obj, methodFunction, addF64Fixed, status); \
  n_objAssignFunction(obj, methodFunction, getF64, status); \
  n_objAssignFunction(obj, methodFunction, setF64, status); \
  \
  n_objAssignFunction(obj, methodFunction, addF64At, status); \
  n_objAssignFunction(obj, methodFunction, addF64FixedAt, status); \
  n_objAssignFunction(obj, methodFunction, getF64At, status); \
  n_objAssignFunction(obj, methodFunction, setF64At, status); \
  \
  n_objAssignFunction(obj, methodFunction, partGetF64, status); \
  n_objAssignFunction(obj, methodFunction, partSetF64, status); \


