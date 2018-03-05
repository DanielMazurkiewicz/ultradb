function (isEmpty) {
    napi_status status;
    var(result);
    getThis(thisJS, numberOfArguments, 0, status);

    getLocalData(thisJS, localData, result, status);

    if (localData->fileData.header->freeSpace == sizeof(DatabaseHeader)) {
      newBoolean(result, true, status);
    } else {
      newBoolean(result, false, status);
    }

    return result;
}

function (roll) {
    napi_status status;
    var(result);
    getThis(thisJS, numberOfArguments, 0, status);

    getLocalData(thisJS, localData, result, status);
    DatabaseHeader* header = localData->fileData.header;

    header->freeSpacePrevious = header->freeSpace;
    header->baseDescriptorPrevious = header->baseDescriptor;

    header->freeSpace = sizeof(DatabaseHeader);
    header->baseDescriptor += header->freeSpacePrevious - sizeof(DatabaseHeader);
    if (header->rootDocument != 0) { //TODO: copy root document and set new rootDocument address
    }

    newBoolean(result, true, status);

    return result;
}




#define methodsManagement(obj, methodFunction, status) \
  objAssignFunction(obj, methodFunction, isEmpty, status); \
  objAssignFunction(obj, methodFunction, roll, status); \

