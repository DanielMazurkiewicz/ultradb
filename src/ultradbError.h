
napi_value prepareError(napi_env env, UTF8Z source, UTF8Z sourceDetails, UTF8Z message, UTF8Z messageDetails, int useErrMessage) {
    napi_status status;


    n_objCreate(errorObj, status);
    var(error);

    n_newStringUtf8(error, "ultradb", strlen("ultradb"), status);
    n_objPropertySet(errorObj, "from", error, status);


    n_newStringUtf8(error, source, strlen(source), status);
    n_objPropertySet(errorObj, "source", error, status);


    n_newStringUtf8(error, sourceDetails, strlen(sourceDetails), status);
    n_objPropertySet(errorObj, "sourceDetails", error, status);


    n_newStringLatin1(error, message, strlen(message), status);
    n_objPropertySet(errorObj, "message", error, status);


    n_newStringUtf8(error, messageDetails, strlen(messageDetails), status);
    n_objPropertySet(errorObj, "messageDetails", error, status);


    if (useErrMessage != NULL) {
      char* osErrorMessage;
      osErrorMessage = strerror(errno);

      n_newStringLatin1(error, osErrorMessage, strlen(osErrorMessage), status);
      n_objPropertySet(errorObj, "osErrorMessage", error, status);
    }

    return errorObj;
}

