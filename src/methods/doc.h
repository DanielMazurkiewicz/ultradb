
function (docAddHidden) { // (documentSize)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 1, status);
    getLocalData(thisJS, localData, result, status);

    size_t documentLength;
    n_getS64(documentLength, args[0]);

    documentReserveSpaceHidden("docAddHidden", localData, fileData, freeSpace, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, documentLength, documentLengthOffset)

    DocumentInfo documentInfo;
    documentInfo.id = documentAddress + localData->fileData.header->baseDescriptor;
    documentInfo.start = freeSpace;
    documentInfo.length = documentLength;
    n_newBuffer(result, &documentInfo, sizeof (documentInfo));
    return result;
}

function (docAdd) { // (documentSize)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 1, status);
    getLocalData(thisJS, localData, result, status);

    size_t documentLength;
    n_getS64(documentLength, args[0]);

    documentReserveSpace("docAdd", localData, fileData, freeSpace, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, documentLength, documentLengthOffset)

    DocumentInfo documentInfo;
    documentInfo.id = documentAddress + localData->fileData.header->baseDescriptor;
    documentInfo.start = freeSpace;
    documentInfo.length = documentLength;
    n_newBuffer(result, &documentInfo, sizeof (documentInfo));
    return result;
}

function (docGet) { // (documentId)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 1, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor  documentId;
    n_getS64(documentId, args[0]);

    documentGetAddressAndFlags("docGet", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentReturnNullIfHidden(documentFlags, result);
    documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
    documentAddressStartValidate(documentAddressStart, result);

    DocumentInfo documentInfo;
    documentInfo.id = documentId;
    documentInfo.start = documentAddressStart;
    documentInfo.length = documentLength;
    return result;
}

function (docGetHiddenAndVisible) { // (documentId)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 1, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor  documentId;
    n_getU64(documentId, args[0]);

    documentGetAddressAndFlags("docGetHiddenAndVisible", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
    documentAddressStartValidate(documentAddressStart, result);

    DocumentInfo documentInfo;
    documentInfo.id = documentId;
    documentInfo.start = documentAddressStart;
    documentInfo.length = documentLength;
    return result;
}


function (docGetId) { // (documentInfoBuffer)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 1, status);
    getLocalData(thisJS, localData, result, status);

    DocumentInfo* documentInfo;
    n_getBufferPointer(documentInfo, args[0]);

    n_newU64(result, documentInfo->id);
    return result;
}



#define methodsDoc(obj, methodFunction, status) \
  n_objAssignFunction(obj, methodFunction, docAddHidden, status); \
  n_objAssignFunction(obj, methodFunction, docAdd, status); \
  n_objAssignFunction(obj, methodFunction, docGet, status); \
  n_objAssignFunction(obj, methodFunction, docGetHiddenAndVisible, status); \
  n_objAssignFunction(obj, methodFunction, docGetId, status); \
