#define resizeIfNecesarryDbFile(db, data, newFreeSpace, result) \
      if (newFreeSpace > db->dataLength) { \
        size_t newDataLength = newFreeSpace; \
\
        if (newFreeSpace % db->pageSize == 0) { \
          newDataLength += db->pageSize; \
        } else { \
          newDataLength /= db->pageSize; \
          newDataLength = (newDataLength + 1) * db->pageSize; \
        }\
\
        if (fallocate(db->fileDescriptor, 0, db->dataLength, newDataLength - db->dataLength) == -1) { \
          pthread_mutex_unlock(&db->mutexForNewDocument); \
          raiseError(env, thisJS, "add(*)->fallocate"); \
          setUndefined(result); \
          return result; \
        } \
\
        data.pointer = mremap(data.pointer, db->dataLength, newDataLength, MREMAP_MAYMOVE); \
        if (data.pointer == MAP_FAILED) { \
          pthread_mutex_unlock(&db->mutexForNewDocument); \
          raiseError(env, thisJS, "add(*)->mremap"); \
          setUndefined(result); \
          return result; \
        } \
        db->dataLength = newDataLength; \
        db->data = data; \
      }


