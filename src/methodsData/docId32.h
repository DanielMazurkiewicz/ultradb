
function (docIdVerify32) { // (documentInfoBuffer, at)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);

    getLocalData(thisJS, localData, result, status);

    DocumentInfo* documentInfo;
    n_getBufferPointer(documentInfo, args[0]);

    DocumentAddress at;
    n_getU64(at, args[1]);

    if (at + sizeOfU32 > documentInfo->length) {
      n_setUndefined(result);
      return result;
    }

    documentIdChecksum32(documentInfo->id, documentIdChecksum, localData->fileData, documentIdChecksumHelper);
    if (*((U32*)(localData->fileData.pointer + documentInfo->start + at)) == documentIdChecksum) {
        n_newBoolean(result, true, status);
    } else {
        n_newBoolean(result, false, status);
    }

    return result;
}


function (docIdCheckSet32) { // (documentInfoBuffer, at)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);
    getLocalData(thisJS, localData, result, status);

    DocumentInfo* documentInfo;
    n_getBufferPointer(documentInfo, args[0]);

    DocumentAddress at;
    n_getU64(at, args[1]);

    if (at + sizeOfU32 > documentInfo->length) {
      n_setUndefined(result);
      return result;
    }

    documentIdChecksum32(documentInfo->id, documentIdChecksum, localData->fileData, documentIdChecksumHelper);
    *((U32*)(localData->fileData.pointer + documentInfo->start + at)) = documentIdChecksum;

    n_newBoolean(result, true, status);
    return result;
}



#define methodsDocId32(obj, methodFunction, status) \
  n_objAssignFunction(obj, methodFunction, docIdVerify32, status); \
  n_objAssignFunction(obj, methodFunction, docIdCheckSet32, status); \


