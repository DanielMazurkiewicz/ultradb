/*
  Author: Daniel Mazurkiewicz
  Licence: MIT


  Macros summary:
    n_callWithNoArgs(result, napiFunc, napiThis, statusName)

    n_objCreate(variableName, statusName)
    n_objPropertyGet(variableName, obj, propertyName, statusName)
    n_objPropertySet(obj, propertyName, variableName, statusName)
    n_objPropertyDel(obj, propertyName, statusName)
    n_objAssignFunction(obj, tempNapiVariable, functionName, statusName)

    n_getGlobal(variableName, statusName)
    n_getThis(variableName, numberOfArguments, expectedNumberOfArguments, statusName)
    n_getArguments(arguments, numberOfArguments, expectedNumberOfArguments, statusName)
    n_getThisAndArguments(this, arguments, numberOfArguments, expectedNumberOfArguments, statusName)

    n_getStringUtf8Length(variableName, napiText, statusName)
    n_getStringUtf8ZLength(variableName, napiText, statusName)
    n_getStringUtf8Z(variableName, napiText, length, statusName)
    n_getBuffer(variableName, lengthVariableName, napiArrayBuffer, statusName)
    n_getBufferPointer(variableName, napiBuffer, statusName)
    n_getArrayBuffer(variableName, lengthVariableName, napiArrayBuffer, statusName)
    n_getArrayBufferPointer(variableName, napiArrayBuffer, statusName)

    n_getBoolean(variableName, napiVariableName, statusName)

    n_getU32(variableName, napiVariableName, statusName)
    n_getS32(variableName, napiVariableName, statusName)
    n_getU64(variableName, napiVariableName, statusName)
    n_getS64(variableName, napiVariableName, statusName)
    n_getF64(variableName, napiVariableName, statusName)

    n_getU32toMem(variableName, napiVariableName, statusName)
    n_getS32toMem(variableName, napiVariableName, statusName)
    n_getU64toMem(variableName, napiVariableName, statusName)
    n_getS64toMem(variableName, napiVariableName, statusName)
    n_getF64toMem(variableName, napiVariableName, statusName)

    n_newBuffer(variableName, buffer, length, statusName)
    n_newObject(variableName, statusName)
    n_newFunction(variableName, functionName, statusName)
    n_newStringUtf8(variableName, text, length, statusName)
    n_newStringLatin1(variableName, text, length, statusName)
    n_newS32(variableName, value, statusName)
    n_newU32(variableName, value, statusName)
    n_newS64(variableName, value, statusName)
    n_newU64(variableName, value, statusName)
    n_newF64(variableName, value, statusName)
    n_newBoolean(variableName, value, statusName)

    n_assignArrayBuffer(variableName, buffer, length, statusName)

    n_setUndefined(variableName, statusName)
    n_setNull(variableName, statusName)

    throwError(object, status)
    var(variableName)
    function(functionName) ex. function(addNumbers) { }

  ---
  statusName is optional parameter, if provided then assertion against correct result of operation is performed

*/



#define function_prepare(name) napi_value name (napi_env env, napi_callback_info info)
#define function(name) function_prepare(name)




#define n_callWithNoArg_NoStatusCheck(result, napiFunc, napiThis) \
    napi_call_function(env, napiThis, napiFunc, 0, NULL, &result); \
	
#define n_callWithNoArg_StatusCheck(result, napiFunc, napiThis, statusName) \
    statusName = napi_call_function(env, napiThis, napiFunc, 0, NULL, &result); \
    assert(statusName == napi_ok);

#define n_callWithNoArg_getMacro(_1,_2,_3,_4,NAME,...) NAME
#define n_callWithNoArg(...) n_callWithNoArg_getMacro(__VA_ARGS__, n_callWithNoArg_StatusCheck, n_callWithNoArg_NoStatusCheck)(__VA_ARGS__)





#define n_objCreate_NoStatusCheck(variableName) \
    napi_value variableName; \
    napi_create_object(env, &variableName); \
	
#define n_objCreate_StatusCheck(variableName, statusName) \
    napi_value variableName; \
    statusName = napi_create_object(env, &variableName); \
    assert(statusName == napi_ok);

#define n_objCreate_getMacro(_1,_2,NAME,...) NAME
#define n_objCreate(...) n_objCreate_getMacro(__VA_ARGS__, n_objCreate_StatusCheck, n_objCreate_NoStatusCheck)(__VA_ARGS__)




#define n_objPropertyGet_NoStatusCheck(variableName, obj, propertyName) \
    napi_get_named_property(env, obj, propertyName, &variableName);
	
#define n_objPropertyGet_StatusCheck(variableName, obj, propertyName, statusName) \
    statusName = napi_get_named_property(env, obj, propertyName, &variableName); \
    assert(statusName == napi_ok);

#define n_objPropertyGet_getMacro(_1,_2,_3,_4,NAME,...) NAME
#define n_objPropertyGet(...) n_objPropertyGet_getMacro(__VA_ARGS__, n_objPropertyGet_StatusCheck, n_objPropertyGet_NoStatusCheck)(__VA_ARGS__)




#define n_objPropertySet_NoStatusCheck(obj, propertyName, variableName) \
    napi_set_named_property(env, obj, propertyName, variableName);

#define n_objPropertySet_StatusCheck(obj, propertyName, variableName, statusName) \
    statusName = napi_set_named_property(env, obj, propertyName, variableName); \
    assert(statusName == napi_ok);

#define n_objPropertySet_getMacro(_1,_2,_3,_4,NAME,...) NAME
#define n_objPropertySet(...) n_objPropertySet_getMacro(__VA_ARGS__, n_objPropertySet_StatusCheck, n_objPropertySet_NoStatusCheck)(__VA_ARGS__)




#define n_objPropertyDel_NoStatusCheck(obj, propertyName) \
    napi_delete_property(env, obj, propertyName, NULL);

#define n_objPropertyDel_StatusCheck(obj, propertyName, statusName) \
    statusName = napi_delete_property(env, obj, propertyName, NULL); \
    assert(statusName == napi_ok);

#define n_objPropertyDel_getMacro(_1,_2,_3,NAME,...) NAME
#define n_objPropertyDel(...) n_objPropertyDel_getMacro(__VA_ARGS__, n_objPropertyDel_StatusCheck, n_objPropertyDel_NoStatusCheck)(__VA_ARGS__)




#define n_getGlobal_NoStatusCheck(variableName) \
    napi_value variableName; \
    napi_get_global(env, &variableName);
	
#define n_getGlobal_StatusCheck(variableName, statusName) \
    napi_value variableName; \
    statusName = napi_get_global(env, &variableName); \
    assert(statusName == napi_ok);

#define n_getGlobal_getMacro(_1,_2,NAME,...) NAME
#define n_getGlobal(...) n_getGlobal_getMacro(__VA_ARGS__, n_getGlobal_StatusCheck, n_getGlobal_NoStatusCheck)(__VA_ARGS__)




#define n_getThis_NoStatusCheck(variableName, numberOfArguments, expectedNumberOfArguments) \
    size_t numberOfArguments = expectedNumberOfArguments; \
    napi_value variableName; \
    napi_get_cb_info(env, info, &numberOfArguments, nullptr, &variableName, nullptr);
	
#define n_getThis_StatusCheck(variableName, numberOfArguments, expectedNumberOfArguments, statusName) \
    size_t numberOfArguments = expectedNumberOfArguments; \
    napi_value variableName; \
    statusName = napi_get_cb_info(env, info, &numberOfArguments, nullptr, &variableName, nullptr); \
    assert(statusName == napi_ok);

#define n_getThis_getMacro(_1,_2,_3,_4,NAME,...) NAME
#define n_getThis(...) n_getThis_getMacro(__VA_ARGS__, n_getThis_StatusCheck, n_getThis_NoStatusCheck)(__VA_ARGS__)




#define n_getArguments_NoStatusCheck(arguments, numberOfArguments, expectedNumberOfArguments) \
    size_t numberOfArguments = expectedNumberOfArguments; \
    napi_value arguments[expectedNumberOfArguments]; \
    napi_get_cb_info(env, info, &numberOfArguments, arguments, nullptr, nullptr);
	
#define n_getArguments_StatusCheck(arguments, numberOfArguments, expectedNumberOfArguments, statusName) \
    size_t numberOfArguments = expectedNumberOfArguments; \
    napi_value arguments[expectedNumberOfArguments]; \
    statusName = napi_get_cb_info(env, info, &numberOfArguments, arguments, nullptr, nullptr); \
    assert(statusName == napi_ok);

#define n_getArguments_getMacro(_1,_2,_3,_4,NAME,...) NAME
#define n_getArguments(...) n_getArguments_getMacro(__VA_ARGS__, n_getArguments_StatusCheck, n_getArguments_NoStatusCheck)(__VA_ARGS__)




#define n_getThisAndArguments_NoStatusCheck(this, arguments, numberOfArguments, expectedNumberOfArguments) \
    size_t numberOfArguments = expectedNumberOfArguments; \
    napi_value this; \
    napi_value arguments[expectedNumberOfArguments]; \
    napi_get_cb_info(env, info, &numberOfArguments, arguments, &this, nullptr);
	
#define n_getThisAndArguments_StatusCheck(this, arguments, numberOfArguments, expectedNumberOfArguments, statusName) \
    size_t numberOfArguments = expectedNumberOfArguments; \
    napi_value this; \
    napi_value arguments[expectedNumberOfArguments]; \
    statusName = napi_get_cb_info(env, info, &numberOfArguments, arguments, &this, nullptr); \
    assert(statusName == napi_ok);

#define n_getThisAndArguments_getMacro(_1,_2,_3,_4,_5,NAME,...) NAME
#define n_getThisAndArguments(...) n_getThisAndArguments_getMacro(__VA_ARGS__, n_getThisAndArguments_StatusCheck, n_getThisAndArguments_NoStatusCheck)(__VA_ARGS__)




#define n_getStringUtf8Length_NoStatusCheck(variableName, napiText) \
    napi_get_value_string_utf8(env, napiText, NULL, 0, &variableName);

#define n_getStringUtf8Length_StatusCheck(variableName, napiText, statusName) \
    statusName = napi_get_value_string_utf8(env, napiText, NULL, 0, &variableName); \
    assert(statusName == napi_ok);

#define n_getStringUtf8Length_getMacro(_1,_2,_3,NAME,...) NAME
#define n_getStringUtf8Length(...) n_getStringUtf8Length_getMacro(__VA_ARGS__, n_getStringUtf8Length_StatusCheck, n_getStringUtf8Length_NoStatusCheck)(__VA_ARGS__)




#define n_getStringUtf8ZLength_NoStatusCheck(variableName, napiText) \
    napi_get_value_string_utf8(env, napiText, NULL, 0, &variableName); \
    variableName++

#define n_getStringUtf8ZLength_StatusCheck(variableName, napiText, statusName) \
    statusName = napi_get_value_string_utf8(env, napiText, NULL, 0, &variableName); \
    assert(statusName == napi_ok); \
    variableName++

#define n_getStringUtf8ZLength_getMacro(_1,_2,_3,NAME,...) NAME
#define n_getStringUtf8ZLength(...) n_getStringUtf8ZLength_getMacro(__VA_ARGS__, n_getStringUtf8ZLength_StatusCheck, n_getStringUtf8ZLength_NoStatusCheck)(__VA_ARGS__)




#define n_getStringUtf8Z_NoStatusCheck(variableName, napiText, length) \
    napi_get_value_string_utf8(env, napiText, variableName, length, 0);

#define n_getStringUtf8Z_StatusCheck(variableName, napiText, length, statusName) \
    statusName = napi_get_value_string_utf8(env, napiText, variableName, length, 0); \
    assert(statusName == napi_ok);

#define n_getStringUtf8Z_getMacro(_1,_2,_3,_4,NAME,...) NAME
#define n_getStringUtf8Z(...) n_getStringUtf8Z_getMacro(__VA_ARGS__, n_getStringUtf8Z_StatusCheck, n_getStringUtf8Z_NoStatusCheck)(__VA_ARGS__)




#define n_getBuffer_NoStatusCheck(variableName, lengthVariableName, napiBuffer) \
    napi_get_buffer_info(env, napiBuffer, &variableName, &lengthVariableName);

#define n_getBuffer_StatusCheck(variableName, lengthVariableName, napiBuffer, statusName) \
    statusName = napi_get_buffer_info(env, napiBuffer, &variableName, &lengthVariableName); \
    assert(statusName == napi_ok);

#define n_getBuffer_getMacro(_1,_2,_3,_4,NAME,...) NAME
#define n_getBuffer(...) n_getBuffer_getMacro(__VA_ARGS__, n_getBuffer_StatusCheck, n_getBuffer_NoStatusCheck)(__VA_ARGS__)


#define n_getBufferPointer_NoStatusCheck(variableName, napiBuffer) \
    napi_get_buffer_info(env, napiBuffer, &variableName, NULL);

#define n_getBufferPointer_StatusCheck(variableName, napiBuffer, statusName) \
    statusName = napi_get_buffer_info(env, napiBuffer, &variableName, NULL); \
    assert(statusName == napi_ok);

#define n_getBufferPointer_getMacro(_1,_2,_3,NAME,...) NAME
#define n_getBufferPointer(...) n_getBufferPointer_getMacro(__VA_ARGS__, n_getBufferPointer_StatusCheck, n_getBufferPointer_NoStatusCheck)(__VA_ARGS__)




#define n_getArrayBuffer_NoStatusCheck(variableName, lengthVariableName, napiArrayBuffer) \
    napi_get_arraybuffer_info(env, napiArrayBuffer, &variableName, &lengthVariableName);

#define n_getArrayBuffer_StatusCheck(variableName, lengthVariableName, napiArrayBuffer, statusName) \
    statusName = napi_get_arraybuffer_info(env, napiArrayBuffer, &variableName, &lengthVariableName); \
    assert(statusName == napi_ok);

#define n_getArrayBuffer_getMacro(_1,_2,_3,_4,NAME,...) NAME
#define n_getArrayBuffer(...) n_getArrayBuffer_getMacro(__VA_ARGS__, n_getArrayBuffer_StatusCheck, n_getArrayBuffer_NoStatusCheck)(__VA_ARGS__)




#define n_getArrayBufferPointer_NoStatusCheck(variableName, napiArrayBuffer) \
    napi_get_arraybuffer_info(env, napiArrayBuffer, &variableName, NULL);

#define n_getArrayBufferPointer_StatusCheck(variableName, napiArrayBuffer, statusName) \
    statusName = napi_get_arraybuffer_info(env, napiArrayBuffer, &variableName, NULL); \
    assert(statusName == napi_ok);

#define n_getArrayBufferPointer_getMacro(_1,_2,_3,NAME,...) NAME
#define n_getArrayBufferPointer(...) n_getArrayBufferPointer_getMacro(__VA_ARGS__, n_getArrayBufferPointer_StatusCheck, n_getArrayBufferPointer_NoStatusCheck)(__VA_ARGS__)




#define n_getS32_NoStatusCheck(variableName, napiVariableName) \
    napi_get_value_int32(env, napiVariableName, &variableName);

#define n_getS32_StatusCheck(variableName, napiVariableName, statusName) \
    statusName = napi_get_value_int32(env, napiVariableName, &variableName); \
    assert(statusName == napi_ok);

#define n_getS32_getMacro(_1,_2,_3,NAME,...) NAME
#define n_getS32(...) n_getS32_getMacro(__VA_ARGS__, n_getS32_StatusCheck, n_getS32_NoStatusCheck)(__VA_ARGS__)




#define n_getS32toMem_NoStatusCheck(variableName, napiVariableName) \
    napi_get_value_int32(env, napiVariableName, variableName);

#define n_getS32toMem_StatusCheck(variableName, napiVariableName, statusName) \
    statusName = napi_get_value_int32(env, napiVariableName, variableName); \
    assert(statusName == napi_ok);

#define n_getS32toMem_getMacro(_1,_2,_3,NAME,...) NAME
#define n_getS32toMem(...) n_getS32_getMacro(__VA_ARGS__, n_getS32toMem_StatusCheck, n_getS32toMem_NoStatusCheck)(__VA_ARGS__)




#define n_getU32_NoStatusCheck(variableName, napiVariableName) \
    napi_get_value_uint32(env, napiVariableName, &variableName);

#define n_getU32_StatusCheck(variableName, napiVariableName, statusName) \
    statusName = napi_get_value_uint32(env, napiVariableName, &variableName); \
    assert(statusName == napi_ok);

#define n_getU32_getMacro(_1,_2,_3,NAME,...) NAME
#define n_getU32(...) n_getU32_getMacro(__VA_ARGS__, n_getU32_StatusCheck, n_getU32_NoStatusCheck)(__VA_ARGS__)




#define n_getU32toMem_NoStatusCheck(variableName, napiVariableName) \
    napi_get_value_uint32(env, napiVariableName, variableName);

#define n_getU32toMem_StatusCheck(variableName, napiVariableName, statusName) \
    statusName = napi_get_value_uint32(env, napiVariableName, variableName); \
    assert(statusName == napi_ok);

#define n_getU32toMem_getMacro(_1,_2,_3,NAME,...) NAME
#define n_getU32toMem(...) n_getU32toMem_getMacro(__VA_ARGS__, n_getU32toMem_StatusCheck, n_getU32toMem_NoStatusCheck)(__VA_ARGS__)




#define n_getS64_NoStatusCheck(variableName, napiVariableName) \
    napi_get_value_int64(env, napiVariableName, &variableName);

#define n_getS64_StatusCheck(variableName, napiVariableName, statusName) \
    statusName = napi_get_value_int64(env, napiVariableName, &variableName); \
    assert(statusName == napi_ok);

#define n_getS64_getMacro(_1,_2,_3,NAME,...) NAME
#define n_getS64(...) n_getS64_getMacro(__VA_ARGS__, n_getS64_StatusCheck, n_getS64_NoStatusCheck)(__VA_ARGS__)




#define n_getS64toMem_NoStatusCheck(variableName, napiVariableName) \
    napi_get_value_int64(env, napiVariableName, variableName);

#define n_getS64toMem_StatusCheck(variableName, napiVariableName, statusName) \
    statusName = napi_get_value_int64(env, napiVariableName, variableName); \
    assert(statusName == napi_ok);

#define n_getS64toMem_getMacro(_1,_2,_3,NAME,...) NAME
#define n_getS64toMem(...) n_getS64toMem_getMacro(__VA_ARGS__, n_getS64toMem_StatusCheck, n_getS64toMem_NoStatusCheck)(__VA_ARGS__)





//Same as n_getS64 since for readibility reasons only
#define n_getU64_NoStatusCheck(variableName, napiVariableName) \
    napi_get_value_int64(env, napiVariableName, &variableName);

#define n_getU64_StatusCheck(variableName, napiVariableName, statusName) \
    statusName = napi_get_value_int64(env, napiVariableName, &variableName); \
    assert(statusName == napi_ok);

#define n_getU64_getMacro(_1,_2,_3,NAME,...) NAME
#define n_getU64(...) n_getU64_getMacro(__VA_ARGS__, n_getU64_StatusCheck, n_getU64_NoStatusCheck)(__VA_ARGS__)




#define n_getU64toMem_NoStatusCheck(variableName, napiVariableName) \
    napi_get_value_int64(env, napiVariableName, variableName);

#define n_getU64toMem_StatusCheck(variableName, napiVariableName, statusName) \
    statusName = napi_get_value_int64(env, napiVariableName, variableName); \
    assert(statusName == napi_ok);

#define n_getU64toMem_getMacro(_1,_2,_3,NAME,...) NAME
#define n_getU64toMem(...) n_getU64toMem_getMacro(__VA_ARGS__, n_getU64toMem_StatusCheck, n_getU64toMem_NoStatusCheck)(__VA_ARGS__)





#define n_getF64_NoStatusCheck(variableName, napiVariableName) \
    napi_get_value_double(env, napiVariableName, &variableName);

#define n_getF64_StatusCheck(variableName, napiVariableName, statusName) \
    statusName = napi_get_value_double(env, napiVariableName, &variableName); \
    assert(statusName == napi_ok);

#define n_getF64_getMacro(_1,_2,_3,NAME,...) NAME
#define n_getF64(...) n_getF64_getMacro(__VA_ARGS__, n_getF64_StatusCheck, n_getF64_NoStatusCheck)(__VA_ARGS__)


#define n_getF64toMem_NoStatusCheck(variableName, napiVariableName) \
    napi_get_value_double(env, napiVariableName, variableName);

#define n_getF64toMem_StatusCheck(variableName, napiVariableName, statusName) \
    statusName = napi_get_value_double(env, napiVariableName, variableName); \
    assert(statusName == napi_ok);

#define n_getF64toMem_getMacro(_1,_2,_3,NAME,...) NAME
#define n_getF64toMem(...) n_getF64toMem_getMacro(__VA_ARGS__, n_getF64toMem_StatusCheck, n_getF64toMem_NoStatusCheck)(__VA_ARGS__)



#define n_getBoolean_NoStatusCheck(variableName, napiVariableName) \
    napi_get_value_bool(env, napiVariableName, &variableName);

#define n_getBoolean_StatusCheck(variableName, napiVariableName, statusName) \
    statusName = napi_get_value_bool(env, napiVariableName, &variableName); \
    assert(statusName == napi_ok);

#define n_getBoolean_getMacro(_1,_2,_3,NAME,...) NAME
#define n_getBoolean(...) n_getBoolean_getMacro(__VA_ARGS__, n_getBoolean_StatusCheck, n_getBoolean_NoStatusCheck)(__VA_ARGS__)




#define n_newObject_NoStatusCheck(variableName) \
    napi_create_object(env, &variableName);

#define n_newObject_StatusCheck(variableName, statusName) \
    statusName = napi_create_object(env, &variableName); \
    assert(statusName == napi_ok);

#define n_newObject_getMacro(_1,_2,NAME,...) NAME
#define n_newObject(...) n_newObject_getMacro(__VA_ARGS__, n_newObject_StatusCheck, n_newObject_NoStatusCheck)(__VA_ARGS__)




#define n_newFunction_NoStatusCheck(variableName, functionName) \
    napi_create_function(env, "", NAPI_AUTO_LENGTH, functionName, nullptr, &variableName);

#define n_newFunction_StatusCheck(variableName, functionName, statusName) \
    statusName = napi_create_function(env, "", NAPI_AUTO_LENGTH, functionName, nullptr, &variableName); \
    assert(statusName == napi_ok);

#define n_newFunction_getMacro(_1,_2,_3,NAME,...) NAME
#define n_newFunction(...) n_newFunction_getMacro(__VA_ARGS__, n_newFunction_StatusCheck, n_newFunction_NoStatusCheck)(__VA_ARGS__)




#define n_newStringUtf8_NoStatusCheck(variableName, text, length) \
    napi_create_string_utf8(env, text, length, &variableName);
	
#define n_newStringUtf8_StatusCheck(variableName, text, length, statusName) \
    statusName = napi_create_string_utf8(env, text, length, &variableName); \
    assert(statusName == napi_ok);

#define n_newStringUtf8_getMacro(_1,_2,_3,_4,NAME,...) NAME
#define n_newStringUtf8(...) n_newStringUtf8_getMacro(__VA_ARGS__, n_newStringUtf8_StatusCheck, n_newStringUtf8_NoStatusCheck)(__VA_ARGS__)




#define n_newStringLatin1_NoStatusCheck(variableName, text, length) \
    napi_create_string_latin1(env, text, length, &variableName);
	
#define n_newStringLatin1_StatusCheck(variableName, text, length, statusName) \
    statusName = napi_create_string_latin1(env, text, length, &variableName); \
    assert(statusName == napi_ok);

#define n_newStringLatin1_getMacro(_1,_2,_3,_4,NAME,...) NAME
#define n_newStringLatin1(...) n_newStringLatin1_getMacro(__VA_ARGS__, n_newStringLatin1_StatusCheck, n_newStringLatin1_NoStatusCheck)(__VA_ARGS__)




#define n_newS32_NoStatusCheck(variableName, value) \
    napi_create_int32(env, value, &variableName);
	
#define n_newS32_StatusCheck(variableName, value, statusName) \
    statusName = napi_create_int32(env, value, &variableName); \
    assert(statusName == napi_ok);

#define n_newS32_getMacro(_1,_2,_3,NAME,...) NAME
#define n_newS32(...) n_newS32_getMacro(__VA_ARGS__, n_newS32_StatusCheck, n_newS32_NoStatusCheck)(__VA_ARGS__)




#define n_newU32_NoStatusCheck(variableName, value) \
    napi_create_uint32(env, value, &variableName);
	
#define n_newU32_StatusCheck(variableName, value, statusName) \
    statusName = napi_create_uint32(env, value, &variableName); \
    assert(statusName == napi_ok);

#define n_newU32_getMacro(_1,_2,_3,NAME,...) NAME
#define n_newU32(...) n_newU32_getMacro(__VA_ARGS__, n_newU32_StatusCheck, n_newU32_NoStatusCheck)(__VA_ARGS__)




#define n_newU64_NoStatusCheck(variableName, value) \
    napi_create_int64(env, value, &variableName);
	
#define n_newU64_StatusCheck(variableName, value, statusName) \
    statusName = napi_create_int64(env, value, &variableName); \
    assert(statusName == napi_ok);

#define n_newU64_getMacro(_1,_2,_3,NAME,...) NAME
#define n_newU64(...) n_newU64_getMacro(__VA_ARGS__, n_newU64_StatusCheck, n_newU64_NoStatusCheck)(__VA_ARGS__)




#define n_newS64_NoStatusCheck(variableName, value) \
    napi_create_int64(env, value, &variableName);
	
#define n_newS64_StatusCheck(variableName, value, statusName) \
    statusName = napi_create_int64(env, value, &variableName); \
    assert(statusName == napi_ok);

#define n_newS64_getMacro(_1,_2,_3,NAME,...) NAME
#define n_newS64(...) n_newS64_getMacro(__VA_ARGS__, n_newS64_StatusCheck, n_newS64_NoStatusCheck)(__VA_ARGS__)




#define n_newF64_NoStatusCheck(variableName, value) \
    napi_create_double(env, value, &variableName);
	
#define n_newF64_StatusCheck(variableName, value, statusName) \
    statusName = napi_create_double(env, value, &variableName); \
    assert(statusName == napi_ok);

#define n_newF64_getMacro(_1,_2,_3,NAME,...) NAME
#define n_newF64(...) n_newF64_getMacro(__VA_ARGS__, n_newF64_StatusCheck, n_newF64_NoStatusCheck)(__VA_ARGS__)




#define n_newBoolean_NoStatusCheck(variableName, value) \
    napi_get_boolean(env, value, &variableName);
	
#define n_newBoolean_StatusCheck(variableName, value, statusName) \
    statusName = napi_get_boolean(env, value, &variableName); \
    assert(statusName == napi_ok);

#define n_newBoolean_getMacro(_1,_2,_3,NAME,...) NAME
#define n_newBoolean(...) n_newBoolean_getMacro(__VA_ARGS__, n_newBoolean_StatusCheck, n_newBoolean_NoStatusCheck)(__VA_ARGS__)



#define n_setUndefined_NoStatusCheck(variableName) \
    napi_get_undefined(env, &variableName);
	
#define n_setUndefined_StatusCheck(variableName, statusName) \
    statusName = napi_get_undefined(env, &variableName); \
    assert(statusName == napi_ok);

#define n_setUndefined_getMacro(_1,_2,NAME,...) NAME
#define n_setUndefined(...) n_setUndefined_getMacro(__VA_ARGS__, n_setUndefined_StatusCheck, n_setUndefined_NoStatusCheck)(__VA_ARGS__)




#define n_setNull_NoStatusCheck(variableName) \
    napi_get_null(env, &variableName);
	
#define n_setNull_StatusCheck(variableName, statusName) \
    statusName = napi_get_null(env, &variableName); \
    assert(statusName == napi_ok);

#define n_setNull_getMacro(_1,_2,NAME,...) NAME
#define n_setNull(...) n_setNull_getMacro(__VA_ARGS__, n_setNull_StatusCheck, n_setNull_NoStatusCheck)(__VA_ARGS__)




#define n_assignArrayBuffer_NoStatusCheck(variableName, buffer, length) \
    napi_create_external_arraybuffer(env, buffer, length, 0, NULL, &variableName);
	
#define n_assignArrayBuffer_StatusCheck(variableName, buffer, length, statusName) \
    statusName = napi_create_external_arraybuffer(env, buffer, length, 0, NULL, &variableName); \
    assert(statusName == napi_ok);

#define n_assignArrayBuffer_getMacro(_1,_2,_3,_4,NAME,...) NAME
#define n_assignArrayBuffer(...) n_assignArrayBuffer_getMacro(__VA_ARGS__, n_assignArrayBuffer_StatusCheck, n_assignArrayBuffer_NoStatusCheck)(__VA_ARGS__)



#define n_newBuffer_NoStatusCheck(variableName, buffer, length) \
    napi_create_buffer_copy(env, length, buffer, NULL, &variableName);
	
#define n_newBuffer_StatusCheck(variableName, buffer, length, statusName) \
    statusName = napi_create_buffer_copy(env, length, buffer, NULL, &variableName); \
    assert(statusName == napi_ok);

#define n_newBuffer_getMacro(_1,_2,_3,_4,NAME,...) NAME
#define n_newBuffer(...) n_newBuffer_getMacro(__VA_ARGS__, n_newBuffer_StatusCheck, n_newBuffer_NoStatusCheck)(__VA_ARGS__)







#define throwError_NoStatusCheck(variableName) \
    napi_throw(env, variableName);
	
#define throwError_StatusCheck(variableName, statusName) \
    statusName = napi_throw(env, variableName); \
    assert(statusName == napi_ok);

#define throwError_getMacro(_1,_2,NAME,...) NAME
#define throwError(...) throwError_getMacro(__VA_ARGS__, throwError_StatusCheck, throwError_NoStatusCheck)(__VA_ARGS__)




#define n_objAssignFunction_NoStatusCheck(obj, methodFunction, functionName) \
    n_newFunction(methodFunction, functionName); \
    n_objPropertySet(obj, #functionName, methodFunction);
	
#define n_objAssignFunction_StatusCheck(obj, methodFunction, functionName, statusName) \
    n_newFunction(methodFunction, functionName, statusName); \
    n_objPropertySet(obj, #functionName, methodFunction, statusName);

#define n_objAssignFunction_getMacro(_1,_2,_3,_4,NAME,...) NAME
#define n_objAssignFunction(...) n_objAssignFunction_getMacro(__VA_ARGS__, n_objAssignFunction_StatusCheck, n_objAssignFunction_NoStatusCheck)(__VA_ARGS__)





#define var(variableName) \
    napi_value variableName;
