

function (addBuffer) { // (buffer, length)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);
    getLocalData(thisJS, localData, result, status);

    S64 length;
    n_getS64(length, args[1]);

    U8* buffer;
    size_t bufferLength;
    n_getBuffer(buffer, bufferLength, args[0], status);

    if (length < 0) {
      length = bufferLength;
    }

    documentReserveSpaceHidden("addBuffer", localData, fileData, freeSpace, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, length, documentLengthOffset);
    if (length < bufferLength) {
      memcpy(localData->fileData.str + freeSpace, buffer, length);
    } else {
      memcpy(localData->fileData.str + freeSpace, buffer, bufferLength);
    }

    documentUnhide(localData, documentAddress);

    n_newU64(result, documentAddress + localData->fileData.header->baseDescriptor, status);
    return result;
}

function (addBufferFixed) { // (buffer, length, length_of_document) - creates document with buffer, reserves extra space for being able to modify document later

    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 3, status);
    getLocalData(thisJS, localData, result, status);

    S64 length;
    n_getS64(length, args[1]);

    U8* buffer;
    size_t bufferLength;
    n_getBuffer(buffer, bufferLength, args[0], status);

    if (length < 0) {
      length = bufferLength;
    }

    size_t documentLength;
    n_getU64(documentLength, args[2]);

    if (length > documentLength) {
      n_setUndefined(result);
      return result; 
    }

    documentReserveSpaceHidden("addBufferFixed", localData, fileData, freeSpace, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, documentLength, documentLengthOffset)
    if (length < bufferLength) {
      memcpy(localData->fileData.str + freeSpace, buffer, length);
    } else {
      memcpy(localData->fileData.str + freeSpace, buffer, bufferLength);
    }

    documentUnhide(localData, documentAddress);

    n_newU64(result, documentAddress + localData->fileData.header->baseDescriptor, status);
    return result;
}

function (getBuffer) { // (documentId, length)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor  documentId;
    n_getU64(documentId, args[0]);

    documentGetAddressAndFlags("getBuffer", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentReturnNullIfHidden(documentFlags, result);
    documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
    documentAddressStartValidate(documentAddressStart, result);

    S64 length;
    n_getS64(length, args[1]);
    if (length < 0) {
      length = documentLength;
    } else if (length > documentLength) {
      n_setUndefined(result);
      return result; 
    }

    n_newBuffer(result, fileData.pointer + documentAddressStart, length);

    return result;
}

function (setBuffer) { // (documentId, buffer, length)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 3, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor  documentId;
    n_getU64(documentId, args[0]);

    documentGetAddressAndFlags("setBuffer", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentReturnNullIfHidden(documentFlags, result);
    documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
    documentAddressStartValidate(documentAddressStart, result);

    S64 length;
    n_getS64(length, args[2]);

    U8* buffer;
    size_t bufferLength;
    n_getBuffer(buffer, bufferLength, args[1], status);

    if (length < 0) {
      length = bufferLength;
    }

    if (length > documentLength) {
      n_setUndefined(result);
      return result;
    }

    if (length < bufferLength) {
      memcpy(localData->fileData.str + documentAddressStart, buffer, length);
    } else {
      memcpy(localData->fileData.str + documentAddressStart, buffer, bufferLength);
    }

    n_newBoolean(result, true, status);
    return result;
}






function (addBufferAt) { // (buffer, length, offset_within_document) - creates document with buffer and leaves begining of document empty
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 3, status);
    getLocalData(thisJS, localData, result, status);

    S64 length;
    n_getS64(length, args[1]);

    U8* buffer;
    size_t bufferLength;
    n_getBuffer(buffer, bufferLength, args[0], status);

    if (length < 0) {
      length = bufferLength;
    }

    DocumentAddress at;
    n_getU64(at, args[2]);

    size_t documentLength =  length + at;

    documentReserveSpaceHidden("addBufferAt", localData, fileData, freeSpace, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, documentLength, documentLengthOffset);
    if (length < bufferLength) {
      memcpy(localData->fileData.str + freeSpace + at, buffer, length);
    } else {
      memcpy(localData->fileData.str + freeSpace + at, buffer, bufferLength);
    }

    documentUnhide(localData, documentAddress);

    n_newU64(result, documentAddress + localData->fileData.header->baseDescriptor, status);
    return result;
}


function (addBufferFixedAt) { // (buffer, length, length_of_document, offset_within_document) - creates document with buffer, reserves extra space for being able to modify document later

    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 4, status);
    getLocalData(thisJS, localData, result, status);

    S64 length;
    n_getS64(length, args[1]);

    U8* buffer;
    size_t bufferLength;
    n_getBuffer(buffer, bufferLength, args[0], status);

    if (length < 0) {
      length = bufferLength;
    }

    size_t documentLength;
    n_getU64(documentLength, args[2]);


    DocumentAddress at;
    n_getU64(at, args[3]);

    if (length + at > documentLength) {
      n_setUndefined(result);
      return result;
    }

    documentReserveSpaceHidden("addBufferFixedAt", localData, fileData, freeSpace, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, documentLength, documentLengthOffset)
    if (length < bufferLength) {
      memcpy(localData->fileData.str + freeSpace + at, buffer, length);
    } else {
      memcpy(localData->fileData.str + freeSpace + at, buffer, bufferLength);
    }

    documentUnhide(localData, documentAddress);

    n_newU64(result, documentAddress + localData->fileData.header->baseDescriptor, status);
    return result;
}


function (getBufferAt) { // (documentId, length, at)

    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 3, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor  documentId;
    n_getU64(documentId, args[0]);

    documentGetAddressAndFlags("getBufferAt", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentReturnNullIfHidden(documentFlags, result);
    documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
    documentAddressStartValidate(documentAddressStart, result);
  
    DocumentAddress at;
    n_getU64(at, args[2]);
  
    if (at > documentLength) {
      n_setUndefined(result);
      return result;
    }

    S64 length;
    n_getS64(length, args[1]);
    if (length < 0) {
      length = documentLength - at;
    } else if (length + at > documentLength) {
      n_setUndefined(result);
      return result; 
    }

    n_newBuffer(result, fileData.pointer + documentAddressStart + at, length);

    return result;
}


function (setBufferAt) { // (documentId, buffer, length, at)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 4, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor  documentId;
    n_getU64(documentId, args[0]);

    documentGetAddressAndFlags("setBuffer", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentReturnNullIfHidden(documentFlags, result);
    documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
    documentAddressStartValidate(documentAddressStart, result);

    S64 length;
    n_getS64(length, args[2]);

    U8* buffer;
    size_t bufferLength;
    n_getBuffer(buffer, bufferLength, args[1], status);

    if (length < 0) {
      length = bufferLength;
    }

    DocumentAddress at;
    n_getU64(at, args[3]);

    if (length + at > documentLength) {
      n_setUndefined(result);
      return result;
    }

    if (length < bufferLength) {
      memcpy(localData->fileData.str + documentAddressStart, buffer, length);
    } else {
      memcpy(localData->fileData.str + documentAddressStart, buffer, bufferLength);
    }

    n_newBoolean(result, true, status);
    return result;
}


function (partGetBuffer) { // (documentInfoBuffer, length, at)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 3, status);
    getLocalData(thisJS, localData, result, status);

    DocumentInfo* documentInfo;
    n_getBufferPointer(documentInfo, args[0]);

    DocumentAddress at;
    n_getU64(at, args[2]);

    size_t documentLength = documentInfo->length;

    if (at > documentLength) {
      n_setUndefined(result);
      return result;
    }

    S64 length;
    n_getS64(length, args[1]);
    if (length < 0) {
      length = documentLength - at;
    } else if (length + at > documentLength) {
      n_setUndefined(result);
      return result; 
    }

    n_newBuffer(result, localData->fileData.str + documentInfo->start + at, length);

    return result;
}


function (partSetBuffer) { // (documentInfoBuffer, buffer, length, at)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 4, status);
    getLocalData(thisJS, localData, result, status);

    DocumentInfo* documentInfo;
    n_getBufferPointer(documentInfo, args[0]);

    S64 length;
    n_getS64(length, args[2]);

    U8* buffer;
    size_t bufferLength;
    n_getBuffer(buffer, bufferLength, args[1], status);

    if (length < 0) {
      length = bufferLength;
    }

    DocumentAddress at;
    n_getU64(at, args[3]);

    if (length + at > documentInfo->length) {
      n_setUndefined(result);
      return result;
    }

    if (length < bufferLength) {
      memcpy(localData->fileData.str + documentInfo->start + at, buffer, length);
    } else {
      memcpy(localData->fileData.str + documentInfo->start + at, buffer, bufferLength);
    }

    n_newBoolean(result, true, status);
    return result;
}



#define methodsBuffer(obj, methodFunction, status) \
  n_objAssignFunction(obj, methodFunction, addBuffer, status); \
  n_objAssignFunction(obj, methodFunction, addBufferFixed, status); \
  n_objAssignFunction(obj, methodFunction, getBuffer, status); \
  n_objAssignFunction(obj, methodFunction, setBuffer, status); \
  \
  n_objAssignFunction(obj, methodFunction, addBufferAt, status); \
  n_objAssignFunction(obj, methodFunction, addBufferFixedAt, status); \
  n_objAssignFunction(obj, methodFunction, getBufferAt, status); \
  n_objAssignFunction(obj, methodFunction, setBufferAt, status); \
  \
  n_objAssignFunction(obj, methodFunction, partGetBuffer, status); \
  n_objAssignFunction(obj, methodFunction, partSetBuffer, status); \


