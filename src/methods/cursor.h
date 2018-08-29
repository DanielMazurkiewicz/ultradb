function (previousOfAll) {
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 1, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor  documentId;
    n_getU64(documentId, args[0]);

    documentGetAddressAndFlags("previousOfAll", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
    documentAddressStartValidate(--documentAddressStart, result);

    n_newU64(result, documentAddressStart + localData->fileData.header->baseDescriptor, status);
    return result;
}

function (previous) {
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 1, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor  documentId;
    n_getU64(documentId, args[0]);

    documentGetAddressAndFlags("previous", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    do {
      documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
      documentAddress = documentAddressStart - 1;
      documentAddressStartValidate(documentAddress, result);
      documentFlags = localData->fileData.u8[documentAddress];
    } while (documentIsHidden(documentFlags));

    n_newU64(result, documentAddress + localData->fileData.header->baseDescriptor, status);
    return result;
}


function (lastOfAll) {
    napi_status status;
    var(result);
    n_getThis(thisJS, numberOfArguments, 0, status);

    getLocalData(thisJS, localData, result, status);

    DbFileDataMapping fileData = localData->fileData;
    pthread_mutex_lock(&localData->sharedData->mutexForNewDocument);
    DocumentAddress freeSpace = fileData.header->freeSpace;
    pthread_mutex_unlock(&localData->sharedData->mutexForNewDocument);

    if (freeSpace <= sizeof(DatabaseHeader)) {
      n_setUndefined(result);
      return result;
    }
    n_newU64(result, fileData.header->baseDescriptor + freeSpace - 1, status);
    return result;
}


function (last) {
    napi_status status;
    var(result);
    n_getThis(thisJS, numberOfArguments, 0, status);

    getLocalData(thisJS, localData, result, status);

    DbFileDataMapping fileData = localData->fileData;
    pthread_mutex_lock(&localData->sharedData->mutexForNewDocument);
    DocumentAddress documentAddress = fileData.header->freeSpace;
    pthread_mutex_unlock(&localData->sharedData->mutexForNewDocument);

    if (documentAddress <= sizeof(DatabaseHeader)) {
      n_setUndefined(result);
      return result;
    }
    documentAddress--;
    U8 documentFlags = fileData.u8[documentAddress];
    while (documentIsHidden(documentFlags)) {
      documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
      documentAddress = documentAddressStart - 1;
      documentAddressStartValidate(documentAddress, result);
      documentFlags = localData->fileData.u8[documentAddress];
    }

    n_newU64(result, documentAddress + fileData.header->baseDescriptor, status);
    return result;
}





#define methodsCursor(obj, methodFunction, status) \
  n_objAssignFunction(obj, methodFunction, last, status); \
  n_objAssignFunction(obj, methodFunction, previous, status); \
\
  n_objAssignFunction(obj, methodFunction, lastOfAll, status); \
  n_objAssignFunction(obj, methodFunction, previousOfAll, status); \

