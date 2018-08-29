

typedef U64 DocumentDescriptor;
typedef U64 DocumentAddress;

struct DocumentInfo_s {
    DocumentDescriptor  id;
    DocumentAddress     start;
    size_t              length;
};
typedef struct DocumentInfo_s DocumentInfo;



struct DatabaseHeader_s {
  U64                 flags;                  // not used yet
  U64                 checksumKey;            // checksum key
  DocumentAddress     freeSpace;              // address of first free space to place a document
  DocumentDescriptor  baseDescriptor;         // real address = documentDescriptor - baseDescriptor;
  DocumentAddress     rootDocument;           // location of root document in database

  // for future use, for rolling databases (new records replaces oldest ones when rich certain size of db)
  DocumentAddress     freeSpacePrevious;
  DocumentDescriptor  baseDescriptorPrevious;
};
typedef struct DatabaseHeader_s DatabaseHeader;



union DbFileDataMapping_u {
    POINTER                 pointer;
    DatabaseHeader*         header;
    STR                     str;
    U8*                     u8;
};
typedef union DbFileDataMapping_u DbFileDataMapping;



struct SharedData_s {
    S32                     numberOfConnected;
    pthread_mutex_t         mutex;
    pthread_mutexattr_t     attr;

    pthread_mutex_t         mutexForNewDocument;
    pthread_mutexattr_t     attrForNewDocument;

    size_t                  fileSize;
};
typedef struct SharedData_s SharedData;  



struct LocalData_s {
    SharedData*             sharedData;
    S32                     sharedDataShmid;

    DbFileDataMapping       fileData;
    S32                     fileDescriptor;
    //size_t                  fileDataLength;
    size_t                  fileSize;
    U32                     pageSize;

};
typedef struct LocalData_s LocalData;  


