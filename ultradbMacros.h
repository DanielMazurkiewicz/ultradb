
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
      newBoolean(result, false); \
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




