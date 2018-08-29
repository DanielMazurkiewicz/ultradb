function (start) {
    napi_status status;
    var(result);
    n_getThis(thisJS, numberOfArguments, 0, status);

    getLocalData(thisJS, localData, result, status);

    pthread_mutex_lock(&localData->sharedData->mutex);

    n_newBoolean(result, true, status);
    return result;
}


function (stop) {
    napi_status status;
    var(result);
    n_getThis(thisJS, numberOfArguments, 0, status);

    getLocalData(thisJS, localData, result, status);

    pthread_mutex_unlock(&localData->sharedData->mutex);

    n_newBoolean(result, true, status);
    return result;
}



#define methodsTransaction(obj, methodFunction, status) \
  n_objAssignFunction(obj, methodFunction, start, status); \
  n_objAssignFunction(obj, methodFunction, stop, status); \

