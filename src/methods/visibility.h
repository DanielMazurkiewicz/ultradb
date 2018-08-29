function (hide) {
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 1, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor  documentId;
    n_getU64(documentId, args[0]);

    documentGetAddressAndFlags("hide", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentHide(localData, documentAddress)

    n_newBoolean(result, true, status);
    return result;
}

function (unhide) {
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 1, status);

    getLocalData(thisJS, localData, result, status);

    DocumentDescriptor  documentId;
    n_getU64(documentId, args[0]);

    documentGetAddressAndFlags("unhide", documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result);
    documentUnhide(localData, documentAddress)

    n_newBoolean(result, true, status);
    return result;
}




#define methodsVisibility(obj, methodFunction, status) \
  n_objAssignFunction(obj, methodFunction, hide, status); \
  n_objAssignFunction(obj, methodFunction, unhide, status); \

