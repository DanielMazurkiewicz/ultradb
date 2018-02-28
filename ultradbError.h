
napi_value prepareError(napi_env env, UTF8Z source, UTF8Z sourceDetails, UTF8Z message, UTF8Z messageDetails, int useErrMessage) {
    napi_status status;


    objCreate(errorObj, status);
    var(error);

    newStringUtf8(error, "ultradb", strlen("ultradb"), status);
    objPropertySet(errorObj, "from", error, status);


    newStringUtf8(error, source, strlen(source), status);
    objPropertySet(errorObj, "source", error, status);


    newStringUtf8(error, sourceDetails, strlen(sourceDetails), status);
    objPropertySet(errorObj, "sourceDetails", error, status);


    newStringLatin1(error, message, strlen(message), status);
    objPropertySet(errorObj, "message", error, status);


    newStringUtf8(error, messageDetails, strlen(messageDetails), status);
    objPropertySet(errorObj, "messageDetails", error, status);


    if (useErrMessage != NULL) {
      char* osErrorMessage;
      osErrorMessage = strerror(errno);

      newStringLatin1(error, osErrorMessage, strlen(osErrorMessage), status);
      objPropertySet(errorObj, "osErrorMessage", error, status);
    }

    return errorObj;
}

