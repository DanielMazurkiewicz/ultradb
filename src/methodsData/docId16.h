
function (docIdVerify16) { // (documentInfoBuffer, at)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);

    getLocalData(thisJS, localData, result, status);

    DocumentInfo* documentInfo;
    n_getBufferPointer(documentInfo, args[0]);

    DocumentAddress at;
    n_getU64(at, args[1]);

    if (at + sizeOfU16 > documentInfo->length) {
      n_setUndefined(result);
      return result;
    }

    documentIdChecksum16(documentInfo->id, documentIdChecksum, localData->fileData, documentIdChecksumHelper);
    if (*((U16*)(localData->fileData.pointer + documentInfo->start + at)) == documentIdChecksum) {
        n_newBoolean(result, true, status);
    } else {
        n_newBoolean(result, false, status);
    }

    return result;
}


function (docIdCheckSet16) { // (documentInfoBuffer, at)
    napi_status status;
    var(result);
    n_getThisAndArguments(thisJS, args, argsCount, 2, status);
    getLocalData(thisJS, localData, result, status);

    DocumentInfo* documentInfo;
    n_getBufferPointer(documentInfo, args[0]);

    DocumentAddress at;
    n_getU64(at, args[1]);

    if (at + sizeOfU16 > documentInfo->length) {
      n_setUndefined(result);
      return result;
    }

    documentIdChecksum16(documentInfo->id, documentIdChecksum, localData->fileData, documentIdChecksumHelper);
    *((U16*)(localData->fileData.pointer + documentInfo->start + at)) = documentIdChecksum;

    n_newBoolean(result, true, status);
    return result;
}



#define methodsDocId16(obj, methodFunction, status) \
  n_objAssignFunction(obj, methodFunction, docIdVerify16, status); \
  n_objAssignFunction(obj, methodFunction, docIdCheckSet16, status); \


