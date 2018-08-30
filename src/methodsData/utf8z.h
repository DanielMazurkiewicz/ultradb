

function (addUtf8z) { // (text)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 1, status);
    getLocalData(thisJS, localData, result, status);

    size_t documentLength;
    n_getStringUtf8ZLength(documentLength, args[0]);

    documentReserveSpaceHidden("addUtf8z", localData, fileData, freeSpace, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, documentLength, documentLengthOffset)

    n_getStringUtf8Z((localData->fileData.str + freeSpace), args[0], documentLength);

    documentUnhide(localData, documentAddress);

    n_newU64(result, documentAddress + localData->fileData.header->baseDescriptor, status);
    return result;
}

function (addUtf8zFixed) { // (text, text_length_max, length_of_document) - creates document with text, reserves extra space for being able to modify document later
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 3, status);
    getLocalData(thisJS, localData, result, status);

    size_t textLengthMax;
    n_getU64(textLengthMax, args[1]);

    size_t textLength;
    n_getStringUtf8ZLength(textLength, args[0]);

    if (textLength > textLengthMax) {
      n_setUndefined(result);
      return result;
    }

    size_t documentLength;
    n_getU64(documentLength, args[2]);

    if (textLength > documentLength) {
      n_setUndefined(result);
      return result;
    }

    documentReserveSpaceHidden("addUtf8zFixed", localData, fileData, freeSpace, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, documentLength, documentLengthOffset)

    n_getStringUtf8Z((localData->fileData.str + freeSpace), args[0], textLength);

    documentUnhide(localData, documentAddress);

    n_newU64(result, documentAddress + localData->fileData.header->baseDescriptor, status);
    return result;
}

function (getUtf8z) { // (documentId)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 1, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor  documentId;
    n_getU64(documentId, args[0]);

    documentGetAddressAndFlags("getUtf8z", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentReturnNullIfHidden(documentFlags, result);
    documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
    documentAddressStartValidate(documentAddressStart, result);

    n_newStringUtf8(result, fileData.pointer + documentAddressStart, strlen(fileData.pointer + documentAddressStart), status);

    return result;
}

function (setUtf8z) { // (documentId, text)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor  documentId;
    n_getU64(documentId, args[0]);

    documentGetAddressAndFlags("setUtf8z", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentReturnNullIfHidden(documentFlags, result);
    documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
    documentAddressStartValidate(documentAddressStart, result);


    size_t textLength;
    n_getStringUtf8ZLength(textLength, args[1]);

    if (textLength > documentLength) {
      n_setUndefined(result);
      return result;
    }

    n_getStringUtf8Z((localData->fileData.str + documentAddressStart), args[1], textLength);

    n_newBoolean(result, true, status);
    return result;
}






function (addUtf8zAt) { // (text, offset_within_document) - creates document with text and leaves begining of document empty
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);
    getLocalData(thisJS, localData, result, status);

    size_t textLength;
    n_getStringUtf8ZLength(textLength, args[0]);

    DocumentAddress at;
    n_getU64(at, args[1]);

    size_t documentLength =  textLength + at;

    documentReserveSpaceHidden("addUtf8zAt", localData, fileData, freeSpace, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, documentLength, documentLengthOffset)

    n_getStringUtf8Z((localData->fileData.str + freeSpace + at), args[0], textLength);

    documentUnhide(localData, documentAddress);

    n_newU64(result, documentAddress + localData->fileData.header->baseDescriptor, status);
    return result;
}


function (addUtf8zFixedAt) { // (text, text_length_max, length_of_document, offset_within_document) - creates document with text, reserves extra space for being able to modify document later
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 4, status);
    getLocalData(thisJS, localData, result, status);

    DocumentAddress at;
    n_getU64(at, args[3]);

    size_t documentLength;
    n_getU64(documentLength, args[2]);

    size_t textLengthMax;
    n_getU64(textLengthMax, args[1]);

    size_t textLength;
    n_getStringUtf8ZLength(textLength, args[0]);

    if (textLength > textLengthMax) {
      n_setUndefined(result);
      return result;
    }

    if (textLength + at > documentLength) {
      n_setUndefined(result);
      return result;
    }

    documentReserveSpaceHidden("addUtf8zFixedAt", localData, fileData, freeSpace, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, documentLength, documentLengthOffset)

    n_getStringUtf8Z((localData->fileData.str + freeSpace + at), args[0], textLength);

    documentUnhide(localData, documentAddress);

    n_newU64(result, documentAddress + localData->fileData.header->baseDescriptor, status);
    return result;
}


function (getUtf8zAt) { // (documentId, at)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor  documentId;
    n_getU64(documentId, args[0]);


    documentGetAddressAndFlags("getUtf8zAt", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentReturnNullIfHidden(documentFlags, result);
    documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
    documentAddressStartValidate(documentAddressStart, result);

    DocumentAddress at;
    n_getU64(at, args[1]);

    if (at >= documentLength) {
      n_setUndefined(result);
      return result;
    }

    n_newStringUtf8(result, fileData.pointer + documentAddressStart + at, strlen(fileData.pointer + documentAddressStart + at), status);

    return result;
}


function (setUtf8zAt) { // (documentId, text, text_length_max, at)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 4, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor  documentId;
    n_getU64(documentId, args[0]);

    documentGetAddressAndFlags("setUtf8zAt", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentReturnNullIfHidden(documentFlags, result);
    documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
    documentAddressStartValidate(documentAddressStart, result);


    size_t textLength;
    n_getStringUtf8ZLength(textLength, args[1]);

    size_t textLengthMax;
    n_getU64(textLengthMax, args[2]);


    DocumentAddress at;
    n_getU64(at, args[3]);


    if (textLength > textLengthMax) {
      n_setUndefined(result);
      return result;
    }

    if (textLength + at > documentLength) {
      n_setUndefined(result);
      return result;
    }

    n_getStringUtf8Z((localData->fileData.str + documentAddressStart + at), args[1], textLength);

    n_newBoolean(result, true, status);
    return result;
}


function (partGetUtf8z) { // (documentInfoBuffer, at)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);
    getLocalData(thisJS, localData, result, status);

    DocumentInfo* documentInfo;
    n_getBufferPointer(documentInfo, args[0]);

    DocumentAddress at;
    n_getU64(at, args[1]);

    if (at >= documentInfo->length) {
      n_setUndefined(result);
      return result;
    }

    n_newStringUtf8(result, localData->fileData.str + documentInfo->start + at, strlen(localData->fileData.str + documentInfo->start + at), status);

    return result;
}


function (partSetUtf8z) { // (documentInfoBuffer, text, text_length_max, at)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 4, status);
    getLocalData(thisJS, localData, result, status);

    DocumentInfo* documentInfo;
    n_getBufferPointer(documentInfo, args[0]);

    DocumentAddress at;
    n_getU64(at, args[3]);

    if (at >= documentInfo->length) {
      n_setUndefined(result);
      return result;
    }

    size_t textLength;
    n_getStringUtf8ZLength(textLength, args[1]);

    size_t textLengthMax;
    n_getU64(textLengthMax, args[2]);

    if (textLength > textLengthMax) {
      n_setUndefined(result);
      return result;
    }

    if (textLength + at > documentInfo->length) {
      n_setUndefined(result);
      return result;
    }

    n_getStringUtf8Z((localData->fileData.str + documentInfo->start + at), args[1], textLength);

    n_newBoolean(result, true, status);
    return result;
}



#define methodsUtf8z(obj, methodFunction, status) \
  n_objAssignFunction(obj, methodFunction, addUtf8z, status); \
  n_objAssignFunction(obj, methodFunction, addUtf8zFixed, status); \
  n_objAssignFunction(obj, methodFunction, getUtf8z, status); \
  n_objAssignFunction(obj, methodFunction, setUtf8z, status); \
  \
  n_objAssignFunction(obj, methodFunction, addUtf8zAt, status); \
  n_objAssignFunction(obj, methodFunction, addUtf8zFixedAt, status); \
  n_objAssignFunction(obj, methodFunction, getUtf8zAt, status); \
  n_objAssignFunction(obj, methodFunction, setUtf8zAt, status); \
  \
  n_objAssignFunction(obj, methodFunction, partGetUtf8z, status); \
  n_objAssignFunction(obj, methodFunction, partSetUtf8z, status); \


