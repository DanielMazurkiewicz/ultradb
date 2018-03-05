function (start) {
    napi_status status;
    var(result);
    getThis(thisJS, numberOfArguments, 0, status);

    getLocalData(thisJS, localData, result, status);

    pthread_mutex_lock(&localData->sharedData->mutex);

    newBoolean(result, true, status);
    return result;
}


function (stop) {
    napi_status status;
    var(result);
    getThis(thisJS, numberOfArguments, 0, status);

    getLocalData(thisJS, localData, result, status);

    pthread_mutex_unlock(&localData->sharedData->mutex);

    newBoolean(result, true, status);
    return result;
}



#define methodsTransaction(obj, methodFunction, status) \
  objAssignFunction(obj, methodFunction, start, status); \
  objAssignFunction(obj, methodFunction, stop, status); \

