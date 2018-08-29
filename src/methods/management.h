function (isEmpty) {
    napi_status status;
    var(result);
    n_getThis(thisJS, numberOfArguments, 0, status);

    getLocalData(thisJS, localData, result, status);

    if (localData->fileData.header->freeSpace == sizeof(DatabaseHeader)) {
      n_newBoolean(result, true, status);
    } else {
      n_newBoolean(result, false, status);
    }

    return result;
}

function (roll) {
    napi_status status;
    var(result);
    n_getThis(thisJS, numberOfArguments, 0, status);

    getLocalData(thisJS, localData, result, status);
    DatabaseHeader* header = localData->fileData.header;

    header->freeSpacePrevious = header->freeSpace;
    header->baseDescriptorPrevious = header->baseDescriptor;

    header->freeSpace = sizeof(DatabaseHeader);
    header->baseDescriptor += header->freeSpacePrevious - sizeof(DatabaseHeader);
    if (header->rootDocument != 0) { //TODO: copy root document and set new rootDocument address
    }

    n_newBoolean(result, true, status);

    return result;
}

function (setRoot) { // (documentId)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 1, status);

    getLocalData(thisJS, localData, result, status);
    //DatabaseHeader* header = localData->fileData.header;

    DocumentDescriptor  documentId;
    n_getU64(localData->fileData.header->rootDocument, args[0]);

    n_newBoolean(result, true, status);
    return result;
}

function (getRoot) {
    napi_status status;
    var(result);
    n_getThis(thisJS, numberOfArguments, 0, status);

    getLocalData(thisJS, localData, result, status);
    //DatabaseHeader* header = localData->fileData.header;

    n_newU64(result, localData->fileData.header->rootDocument, status);

    return result;
}



#define methodsManagement(obj, methodFunction, status) \
  n_objAssignFunction(obj, methodFunction, isEmpty, status); \
  n_objAssignFunction(obj, methodFunction, roll, status); \
  n_objAssignFunction(obj, methodFunction, setRoot, status); \
  n_objAssignFunction(obj, methodFunction, getRoot, status); \

