


function (addUtf8z) {
    napi_status status;
    var(result);
    getThisAndArguments(thisJS, args, argsCount, 1, status);
    getLocalData(thisJS, localData, result, status);

    size_t documentLength;
    getStringUtf8ZLength(documentLength, args[0]);

    documentReserveSpaceHidden("addUtf8z", localData, fileData, freeSpace, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, documentLength, documentLengthOffset)

    getStringUtf8Z((localData->fileData.str + freeSpace), args[0], documentLength);

    documentUnhide(localData, documentAddress);

    newS64(result, documentAddress + localData->fileData.header->baseDescriptor, status);
    return result;
}




function (getUtf8z) {
    napi_status status;
    var(result);
    getThisAndArguments(thisJS, args, argsCount, 1, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor  documentId;
    getS64(documentId, args[0]);

    documentGetAddressAndFlags("getUtf8z", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentReturnNullIfHidden(documentFlags, result);
    documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result);
    documentAddressStartValidate(documentAddressStart, result);

    newStringUtf8(result, fileData.pointer + documentAddressStart, strlen(fileData.pointer + documentAddressStart), status);

    return result;
}





#define methodsUtf8z(obj, methodFunction, status) \
  objAssignFunction(obj, methodFunction, addUtf8z, status); \
  objAssignFunction(obj, methodFunction, getUtf8z, status); \


/*
  objAssignFunction(obj, methodFunction, setUtf8z, status); \
  objAssignFunction(obj, methodFunction, setPartUtf8z, status); \
  objAssignFunction(obj, methodFunction, getPartUtf8z, status); \
*/


/*
  objAssignFunction(obj, methodFunction, start, status); \
  objAssignFunction(obj, methodFunction, stop, status); \
*/



/*
  objAssignFunction(obj, methodFunction, addEmpty, status); \
  objAssignFunction(obj, methodFunction, cpy, status); \
  objAssignFunction(obj, methodFunction, del, status); \
  objAssignFunction(obj, methodFunction, undelete, status); \
*/