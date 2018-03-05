function (previousOfAll) {
    napi_status status;
    var(result);
    getThisAndArguments(thisJS, args, argsCount, 1, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor  documentId;
    getS64(documentId, args[0]);

    documentGetAddressAndFlags("previousOfAll", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
    documentAddressStartValidate(--documentAddressStart, result);

    newS64(result, documentAddressStart + localData->fileData.header->baseDescriptor, status);
    return result;
}

function (previous) {
    napi_status status;
    var(result);
    getThisAndArguments(thisJS, args, argsCount, 1, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor  documentId;
    getS64(documentId, args[0]);

    documentGetAddressAndFlags("previous", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    do {
      documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
      documentAddress = documentAddressStart - 1;
      documentAddressStartValidate(documentAddress, result);
      documentFlags = localData->fileData.u8[documentAddress];
    } while (documentIsHidden(documentFlags));

    newS64(result, documentAddress + localData->fileData.header->baseDescriptor, status);
    return result;
}


function (lastOfAll) {
    napi_status status;
    var(result);
    getThis(thisJS, numberOfArguments, 0, status);

    getLocalData(thisJS, localData, result, status);

    DbFileDataMapping fileData = localData->fileData;
    pthread_mutex_lock(&localData->sharedData->mutexForNewDocument);
    DocumentAddress freeSpace = fileData.header->freeSpace;
    pthread_mutex_unlock(&localData->sharedData->mutexForNewDocument);

    if (freeSpace <= sizeof(DatabaseHeader)) {
      setUndefined(result);
      return result;
    }
    newS64(result, fileData.header->baseDescriptor + freeSpace - 1, status);
    return result;
}


function (last) {
    napi_status status;
    var(result);
    getThis(thisJS, numberOfArguments, 0, status);

    getLocalData(thisJS, localData, result, status);

    DbFileDataMapping fileData = localData->fileData;
    pthread_mutex_lock(&localData->sharedData->mutexForNewDocument);
    DocumentAddress documentAddress = fileData.header->freeSpace;
    pthread_mutex_unlock(&localData->sharedData->mutexForNewDocument);

    if (documentAddress <= sizeof(DatabaseHeader)) {
      setUndefined(result);
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

    newS64(result, documentAddress + fileData.header->baseDescriptor, status);
    return result;
}





#define methodsCursor(obj, methodFunction, status) \
  objAssignFunction(obj, methodFunction, last, status); \
  objAssignFunction(obj, methodFunction, previous, status); \
\
  objAssignFunction(obj, methodFunction, lastOfAll, status); \
  objAssignFunction(obj, methodFunction, previousOfAll, status); \

