function (hide) {
    napi_status status;
    var(result);
    getThisAndArguments(thisJS, args, argsCount, 1, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor  documentId;
    getS64(documentId, args[0]);

    documentGetAddressAndFlags("hide", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentHide(localData, documentAddress)

    newBoolean(result, true, status);
    return result;
}

function (unhide) {
    napi_status status;
    var(result);
    getThisAndArguments(thisJS, args, argsCount, 1, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor  documentId;
    getS64(documentId, args[0]);

    documentGetAddressAndFlags("unhide", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentUnhide(localData, documentAddress)

    newBoolean(result, true, status);
    return result;
}




#define methodsVisibility(obj, methodFunction, status) \
  objAssignFunction(obj, methodFunction, hide, status); \
  objAssignFunction(obj, methodFunction, unhide, status); \

