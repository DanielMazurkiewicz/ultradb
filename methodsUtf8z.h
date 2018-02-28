function (addUtf8z) {
    napi_status status;
    var(result);
    getThisAndArguments(thisJS, args, argsCount, 1, status);

    LocalData* localData;
    objPropertyGet(result, thisJS, "_", status);
    getArrayBufferPointer(localData, result, status);


    size_t documentLength;
    getStringUtf8ZLength(documentLength, args[0]);

//==========================
    pthread_mutex_lock(&localData->sharedData->mutexForNewDocument);
    DbFileDataMapping fileData = localData->fileData;

    DocumentAddress freeSpace = fileData.header->freeSpace;
    DocumentAddress documentLengthOffset = freeSpace + documentLength;
    DocumentAddress freeSpaceNew = documentLengthOffset;
    DocumentAddress documentId;
    size_t fileSizeNew;
    size_t fileSizeShared;


    if (documentLength < 32) {
      documentId = freeSpaceNew;
      freeSpaceNew += 1;
      adjustDatabaseSize(localData, fileData, freeSpaceNew, fileSizeShared, fileSizeNew, "addUtf8z", "documentLength < 32");
      fileData.u8[documentLengthOffset] = documentLength | 0x80;                                    // deleted at the start
    } else if (documentLength < 8192) {
      freeSpaceNew += 2;
      documentId = freeSpaceNew - 1;
      adjustDatabaseSize(localData, fileData, freeSpaceNew, fileSizeShared, fileSizeNew, "addUtf8z", "documentLength < 8192");
      *((U16*)(fileData.pointer + documentLengthOffset)) = documentLength | 0xA000;                 // deleted at the start
    } else if (documentLength < 536870912) {
      freeSpaceNew += 4;
      documentId = freeSpaceNew - 1;
      adjustDatabaseSize(localData, fileData, freeSpaceNew, fileSizeShared, fileSizeNew, "addUtf8z", "documentLength < 536870912");
      *((U32*)(fileData.pointer + documentLengthOffset)) = documentLength | 0xC0000000;             // deleted at the start
    } else {
      freeSpaceNew += 8;
      documentId = freeSpaceNew - 1;
      adjustDatabaseSize(localData, fileData, freeSpaceNew, fileSizeShared, fileSizeNew, "addUtf8z", "documentLength >= 536870912");
      *((U64*)(fileData.pointer + documentLengthOffset)) = documentLength | 0xE000000000000000;     // deleted at the start
    }
    fileData.header->freeSpace = freeSpaceNew;
    pthread_mutex_unlock(&localData->sharedData->mutexForNewDocument);
//==========================

    getStringUtf8Z((localData->fileData.str + freeSpace), args[0], documentLength);

    localData->fileData.u8[documentId] &= 0x7f; // document written and available as not deleted

    newS64(result, documentId + localData->fileData.header->baseDescriptor, status);
    return result;
}




function (getUtf8z) {
    napi_status status;
    var(result);
    getThisAndArguments(thisJS, args, argsCount, 1, status);

    LocalData* localData;
    objPropertyGet(result, thisJS, "_", status);
    getArrayBufferPointer(localData, result, status);


//==========================
    DbFileDataMapping fileData = localData->fileData;

    DocumentAddress documentAddress;
    getS64(documentAddress, args[0]);


    documentAddress -= fileData.header->baseDescriptor;
    if (documentAddress < sizeof(DatabaseHeader)) {
      setUndefined(result);
      return result;
    }

    if (documentAddress >= fileData.header->freeSpace) {
      setUndefined(result);
      return result;
    }

    size_t fileSizeShared;
    if (documentAddress >= localData->fileSize) {

      pthread_mutex_lock(&localData->sharedData->mutexForNewDocument); //To guarantee that readen file length is consistent, especially on 32 bit systems
      fileSizeShared = localData->sharedData->fileSize;
      pthread_mutex_unlock(&localData->sharedData->mutexForNewDocument);

      fileData.pointer = mremap(fileData.pointer, localData->fileSize, fileSizeShared, MREMAP_MAYMOVE);
      if (fileData.pointer == MAP_FAILED) {
        finishWithError("getUtf8z", "documentAddress >= localData->fileSize", "mremap", "", 1);
      } 
      localData->fileSize = fileSizeShared;
      localData->fileData = fileData;
    }


    U8 docFlags = localData->fileData.u8[documentAddress];
//==========================

    if ((docFlags & 0x80) != 0) { // document is deleted
      setNull(result);
      return result;
    }
//==========================
    size_t documentLength;
    size_t documentStart;

    switch (docFlags & 0x60) {
      case 0x00:
        documentLength = docFlags & 0x1f;
        documentStart = documentAddress - documentLength;
        break;
      case 0x20:
        documentAddress -= 1;
        if (documentAddress < sizeof(DatabaseHeader)) {
          setUndefined(result);
          return result;
        }

        documentLength = *((U16*)(fileData.pointer + documentAddress)) & 0x1fff;
        documentStart = documentAddress - documentLength;
        break;
      case 0x40:
        documentAddress -= 3;
        if (documentAddress < sizeof(DatabaseHeader)) {
          setUndefined(result);
          return result;
        }

        documentLength = *((U32*)(fileData.pointer + documentAddress)) & 0x1fffffff;
        documentStart = documentAddress - documentLength;

        break;
      case 0x60:
        documentAddress -= 7;
        if (documentAddress < sizeof(DatabaseHeader)) {
          setUndefined(result);
          return result;
        }

        documentLength = *((U64*)(fileData.pointer + documentAddress)) & 0x1fffffffffffffff;
        documentStart = documentAddress - documentLength;

        break;
    }


    if (documentStart < sizeof(DatabaseHeader)) {
      setUndefined(result);
      return result;
    }
//==========================

    newStringUtf8(result, fileData.pointer + documentStart, strlen(fileData.pointer + documentStart), status);

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