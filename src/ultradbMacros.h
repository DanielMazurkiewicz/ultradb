
#define adjustDatabaseSize(localData, fileData, freeSpaceNew, fileSizeShared, fileSizeNew, source, sourceDetails) \
      if (freeSpaceNew > localData->fileSize) { \
        fileSizeShared = localData->sharedData->fileSize; \
        if (freeSpaceNew > fileSizeShared) { \
          fileSizeNew = freeSpaceNew; \
\
          if (freeSpaceNew % localData->pageSize == 0) { \
            fileSizeNew += localData->pageSize; \
          } else { \
            fileSizeNew /= localData->pageSize; \
            fileSizeNew = (fileSizeNew + 1) * localData->pageSize; \
          } \
\
          if (fallocate(localData->fileDescriptor, 0, fileSizeShared, fileSizeNew - fileSizeShared) == -1) { \
            pthread_mutex_unlock(&localData->sharedData->mutexForNewDocument); \
            finishWithError(source, sourceDetails, "fallocate", "", 1); \
          } \
\
          fileData.pointer = mremap(fileData.pointer, localData->fileSize, fileSizeNew, MREMAP_MAYMOVE); \
          if (fileData.pointer == MAP_FAILED) { \
            pthread_mutex_unlock(&localData->sharedData->mutexForNewDocument); \
            finishWithError(source, sourceDetails, "mremap(1)", "", 1); \
          } \
          localData->sharedData->fileSize = fileSizeNew; \
          localData->fileSize = fileSizeNew; \
        } else { \
          fileData.pointer = mremap(fileData.pointer, localData->fileSize, fileSizeShared, MREMAP_MAYMOVE); \
          if (fileData.pointer == MAP_FAILED) { \
            pthread_mutex_unlock(&localData->sharedData->mutexForNewDocument); \
            finishWithError(source, sourceDetails, "mremap(2)", "", 1); \
          } \
          localData->fileSize = fileSizeShared; \
        } \
        localData->fileData = fileData; \
      }





#define waitTill(condition) \
      while (condition) { \
        nanosleep((const struct timespec[]){{0, 1000000L}}, NULL); \
      };

#define finishWithError(source, sourceDetails, message, messageDetails, useErrMessage) \
      result = prepareError(env, source, sourceDetails, message, messageDetails, useErrMessage); \
      throwError(result); \
      n_newBoolean(result, false); \
      return result;




#define closeAssert(fileDescriptor, source, sourceDetails, message, messageDetails) \
      if (close(fileDescriptor) == -1) { \
        finishWithError(source, sourceDetails, message, messageDetails, 1); \
      }


#define shmdtAssert(data, source, sourceDetails, message, messageDetails) \
      if (shmdt((POINTER)data) == -1) { \
        finishWithError(source, sourceDetails, message, messageDetails, 1); \
      }

#define shmctlAssert(shmid, source, sourceDetails, message, messageDetails) \
      if (shmctl(shmid, IPC_RMID, NULL) == -1) { \
        finishWithError(source, sourceDetails, message, messageDetails, 1); \
      }


#define pthread_mutexattr_destroyAssert(attribute, source, sourceDetails, message, messageDetails) \
      if (pthread_mutexattr_destroy(attribute) == -1) { \
        finishWithError(source, sourceDetails, message, messageDetails, 1); \
      }


#define pthread_mutex_destroyAssert(mutex, source, sourceDetails, message, messageDetails) \
      if (pthread_mutex_destroy(mutex) == -1) { \
        finishWithError(source, sourceDetails, message, messageDetails, 1); \
      }


#define pthread_mutex_CLEANUP(sharedData, mutex, attr, source, sourceDetails, messageDetails) \
      pthread_mutex_destroyAssert(&sharedData->mutex, source, sourceDetails, "pthread_mutex_destroy", messageDetails); \
      pthread_mutexattr_destroyAssert(&sharedData->attr, source, sourceDetails, "pthread_mutexattr_destroy", messageDetails);


#define shm_CLEANUP(sharedData, sharedDataShmid, source, sourceDetails, messageDetails) \
      shmdtAssert(sharedData, source, sourceDetails, "shmdt", messageDetails); \
      shmctlAssert(sharedDataShmid, source, sourceDetails, "shmctl", messageDetails);




#define freeSharedData(sharedData, shmid, source, messageDetails) \
    if (sharedData->numberOfConnected == 1) { \
      sharedData->numberOfConnected = -1; \
      pthread_mutex_unlock(&sharedData->mutex); \
\
      pthread_mutex_CLEANUP(sharedData, mutex, attr, source, "numberOfConnected=1", messageDetails); \
      pthread_mutex_CLEANUP(sharedData, mutexForNewDocument, attrForNewDocument, source, "numberOfConnected=1(1)", messageDetails); \
      shm_CLEANUP(sharedData, shmid, source, "numberOfConnected=1", messageDetails); \
\
    } else if (sharedData->numberOfConnected > 1) { \
      sharedData->numberOfConnected--; \
      pthread_mutex_unlock(&sharedData->mutex); \
      shmdtAssert(sharedData, source, "numberOfConnected>1", "shmdt", messageDetails); \
\
    } else { \
      pthread_mutex_unlock(&sharedData->mutex); \
    }




#define getLocalData(thisJS, localData, result, status) \
    LocalData* localData; \
    n_objPropertyGet(result, thisJS, "_", status); \
    n_getArrayBufferPointer(localData, result, status); \

#define documentReserveSpace_masks(source, localData, fileData_, freeSpace_, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, documentLength, documentLengthOffset, mask8, mask16, mask32, mask64) \
    DbFileDataMapping fileData_ = localData->fileData; \
\
    pthread_mutex_lock(&localData->sharedData->mutexForNewDocument); \
    DocumentAddress freeSpace_ = fileData.header->freeSpace; \
    DocumentAddress documentLengthOffset = freeSpace + documentLength; \
    DocumentAddress freeSpaceNew = documentLengthOffset; \
    DocumentAddress documentAddress; \
    size_t fileSizeNew; \
    size_t fileSizeShared; \
\
    if (documentLength < 32) { \
      documentAddress = freeSpaceNew; \
      freeSpaceNew += 1; \
      adjustDatabaseSize(localData, fileData, freeSpaceNew, fileSizeShared, fileSizeNew, source, "documentLength < 32"); \
      fileData.u8[documentLengthOffset] = documentLength | mask8; \
    } else if (documentLength < 8192) { \
      freeSpaceNew += 2; \
      documentAddress = freeSpaceNew - 1; \
      adjustDatabaseSize(localData, fileData, freeSpaceNew, fileSizeShared, fileSizeNew, source, "documentLength < 8192"); \
      *((U16*)(fileData.pointer + documentLengthOffset)) = documentLength | mask16; \
    } else if (documentLength < 536870912) { \
      freeSpaceNew += 4; \
      documentAddress = freeSpaceNew - 1; \
      adjustDatabaseSize(localData, fileData, freeSpaceNew, fileSizeShared, fileSizeNew, source, "documentLength < 536870912"); \
      *((U32*)(fileData.pointer + documentLengthOffset)) = documentLength | mask32; \
    } else { \
      freeSpaceNew += 8; \
      documentAddress = freeSpaceNew - 1; \
      adjustDatabaseSize(localData, fileData, freeSpaceNew, fileSizeShared, fileSizeNew, source, "documentLength >= 536870912"); \
      *((U64*)(fileData.pointer + documentLengthOffset)) = documentLength | mask64; \
    } \
    fileData.header->freeSpace = freeSpaceNew; \
    pthread_mutex_unlock(&localData->sharedData->mutexForNewDocument);


#define documentReserveSpaceHidden(source, localData, fileData, freeSpace, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, documentLength, documentLengthOffset) \
    documentReserveSpace_masks(source, localData, fileData, freeSpace, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, documentLength, documentLengthOffset, 0x80, 0xA000, 0xC0000000, 0xE000000000000000);

#define documentReserveSpace(source, localData, fileData, freeSpace, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, documentLength, documentLengthOffset) \
    documentReserveSpace_masks(source, localData, fileData, freeSpace, freeSpaceNew, fileSizeNew, fileSizeShared, documentAddress, documentLength, documentLengthOffset, 0x00, 0x2000, 0x40000000, 0x6000000000000000);


#define documentUnhide(localData, documentAddress) \
    localData->fileData.u8[documentAddress] &= 0x7f;


#define documentIsHidden(documentFlags) \
    (documentFlags & 0x80) != 0

#define documentHide(localData, documentAddress) \
    localData->fileData.u8[documentAddress] |= 0x80;

#define documentReturnNullIfHidden(documentFlags, result) \
    if (documentIsHidden(documentFlags)) { \
      n_setNull(result); \
      return result; \
    }

#define documentGetAddressAndFlags(source, documentId, localData, fileData, fileSizeShared, documentFlags, documentAddress, result) \
    DbFileDataMapping fileData = localData->fileData; \
\
    DocumentAddress     documentAddress; \
\
    documentAddress = documentId - fileData.header->baseDescriptor; \
    if (documentAddress < sizeof(DatabaseHeader)) { \
      n_setUndefined(result); \
      return result; \
    } \
\
    if (documentAddress >= fileData.header->freeSpace) { \
      n_setUndefined(result); \
      return result; \
    } \
\
    size_t fileSizeShared; \
    if (documentAddress >= localData->fileSize) { \
\
      pthread_mutex_lock(&localData->sharedData->mutexForNewDocument); \
      fileSizeShared = localData->sharedData->fileSize; \
      pthread_mutex_unlock(&localData->sharedData->mutexForNewDocument); \
\
      fileData.pointer = mremap(fileData.pointer, localData->fileSize, fileSizeShared, MREMAP_MAYMOVE); \
      if (fileData.pointer == MAP_FAILED) { \
        finishWithError(source, "documentAddress >= localData->fileSize", "mremap", "", 1); \
      } \
      localData->fileSize = fileSizeShared; \
      localData->fileData = fileData; \
    } \
\
    U8 documentFlags = localData->fileData.u8[documentAddress];


#define documentGetLengthAndStart(fileData, documentFlags, documentLength, documentAddressStart, documentAddress, result) \
    size_t documentLength; \
    size_t documentAddressStart; \
\
    switch (documentFlags & 0x60) { \
      case 0x00: \
        documentLength = documentFlags & 0x1f; \
        documentAddressStart = documentAddress - documentLength; \
        break; \
\
      case 0x20: \
        documentAddress -= 1; \
        if (documentAddress < sizeof(DatabaseHeader)) { \
          n_setUndefined(result); \
          return result; \
        } \
\
        documentLength = *((U16*)(fileData.pointer + documentAddress)) & 0x1fff; \
        documentAddressStart = documentAddress - documentLength; \
        break; \
\
      case 0x40: \
        documentAddress -= 3; \
        if (documentAddress < sizeof(DatabaseHeader)) { \
          n_setUndefined(result); \
          return result; \
        } \
\
        documentLength = *((U32*)(fileData.pointer + documentAddress)) & 0x1fffffff; \
        documentAddressStart = documentAddress - documentLength; \
        break; \
\
      case 0x60: \
        documentAddress -= 7; \
        if (documentAddress < sizeof(DatabaseHeader)) { \
          n_setUndefined(result); \
          return result; \
        } \
\
        documentLength = *((U64*)(fileData.pointer + documentAddress)) & 0x1fffffffffffffff; \
        documentAddressStart = documentAddress - documentLength; \
        break; \
    } \


#define documentAddressStartValidate(documentAddressStart, result) \
    if (documentAddressStart < sizeof(DatabaseHeader)) { \
      n_setUndefined(result); \
      return result; \
    } \



#define documentIdChecksum32(documentId, documentIdChecksum, fileData, documentIdChecksumHelper) \
    DocumentDescriptor documentIdChecksumHelper = documentId ^ fileData.header->checksumKey; \
    U32 documentIdChecksum = documentIdChecksumHelper & 0xffffffff; \
\
    documentIdChecksum = (documentIdChecksum >> 8) + ((documentIdChecksum & 0xff) << 24); \
    documentIdChecksumHelper >>= 32; \
    documentIdChecksum ^= documentIdChecksumHelper; \


#define documentIdChecksum16(documentId, documentIdChecksum, fileData, documentIdChecksumHelper) \
    documentIdChecksum32(documentId, documentIdChecksum, fileData, documentIdChecksumHelper); \
\
    documentIdChecksumHelper = documentIdChecksum >> 16; \
\
    documentIdChecksum &= 0xffff; \
    documentIdChecksum = (documentIdChecksum >> 4) + ((documentIdChecksum & 0xf) << 12); \
    documentIdChecksum ^= documentIdChecksumHelper; \


#define documentIdChecksum8(documentId, documentIdChecksum, fileData, documentIdChecksumHelper) \
    documentIdChecksum16(documentId, documentIdChecksum, fileData, documentIdChecksumHelper); \
\
    documentIdChecksumHelper = documentIdChecksum >> 8; \
\
    documentIdChecksum &= 0xff; \
    documentIdChecksum = (documentIdChecksum >> 2) + ((documentIdChecksum & 0x3) << 6); \
    documentIdChecksum ^= documentIdChecksumHelper; \

