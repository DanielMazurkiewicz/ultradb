/*
  Author: Daniel Mazurkiewicz
  Licence: MIT


  Macros summary:
    objCreate(variableName, statusName)
    objPropertyGet(variableName, obj, propertyName, statusName)
    objPropertySet(obj, propertyName, variableName, statusName)
    objPropertyDel(obj, propertyName, statusName)

    getThis(variableName, numberOfArguments, expectedNumberOfArguments, statusName)
    getArguments(arguments, numberOfArguments, expectedNumberOfArguments, statusName)

    getStringUtf8Length(variableName, napiText, statusName)
    getStringUtf8(variableName, napiText, length, statusName)
    getArrayBuffer(variableName, lengthVariableName, napiArrayBuffer, statusName)
    getI32(variableName, napiVariableName, statusName)

    newObject(variableName, statusName)
    newFunction(variableName, functionName, statusName)
    newStringUtf8(variableName, text, length, statusName)
    newStringLatin1(variableName, text, length, statusName)
    newI32(variableName, value, statusName)
    newDouble(variableName, value, statusName)

    assignArrayBuffer(variableName, buffer, length, statusName)

    var(variableName)

  ---
  statusName is optional parameter, if provided then assertion against correct result of operation is performed

*/


#define objCreate_NoStatusCheck(variableName) \
    napi_value variableName; \
    napi_create_object(env, &variableName); \
	
#define objCreate_StatusCheck(variableName, statusName) \
    napi_value variableName; \
    statusName = napi_create_object(env, &variableName); \
    assert(statusName == napi_ok);

#define objCreate_getMacro(_1,_2,NAME,...) NAME
#define objCreate(...) objCreate_getMacro(__VA_ARGS__, objCreate_StatusCheck, objCreate_NoStatusCheck)(__VA_ARGS__)




#define objPropertyGet_NoStatusCheck(variableName, obj, propertyName) \
    napi_get_named_property(env, obj, propertyName, &variableName);
	
#define objPropertyGet_StatusCheck(variableName, obj, propertyName, statusName) \
    statusName = napi_get_named_property(env, obj, propertyName, &variableName); \
    assert(statusName == napi_ok);

#define objPropertyGet_getMacro(_1,_2,_3,_4,NAME,...) NAME
#define objPropertyGet(...) objPropertyGet_getMacro(__VA_ARGS__, objPropertyGet_StatusCheck, objPropertyGet_NoStatusCheck)(__VA_ARGS__)




#define objPropertySet_NoStatusCheck(obj, propertyName, variableName) \
    napi_set_named_property(env, obj, propertyName, variableName);

#define objPropertySet_StatusCheck(obj, propertyName, variableName, statusName) \
    statusName = napi_set_named_property(env, obj, propertyName, variableName); \
    assert(statusName == napi_ok);

#define objPropertySet_getMacro(_1,_2,_3,_4,NAME,...) NAME
#define objPropertySet(...) objPropertySet_getMacro(__VA_ARGS__, objPropertySet_StatusCheck, objPropertySet_NoStatusCheck)(__VA_ARGS__)




#define objPropertyDel_NoStatusCheck(obj, propertyName) \
    napi_delete_property(env, obj, propertyName, NULL);

#define objPropertyDel_StatusCheck(obj, propertyName, statusName) \
    statusName = napi_delete_property(env, obj, propertyName, NULL); \
    assert(statusName == napi_ok);

#define objPropertyDel_getMacro(_1,_2,_3,NAME,...) NAME
#define objPropertyDel(...) objPropertyDel_getMacro(__VA_ARGS__, objPropertyDel_StatusCheck, objPropertyDel_NoStatusCheck)(__VA_ARGS__)




#define getThis_NoStatusCheck(variableName, numberOfArguments, expectedNumberOfArguments) \
    size_t numberOfArguments = expectedNumberOfArguments; \
    napi_value variableName; \
    napi_get_cb_info(env, info, &numberOfArguments, nullptr, &variableName, nullptr);
	
#define getThis_StatusCheck(variableName, numberOfArguments, expectedNumberOfArguments, statusName) \
    size_t numberOfArguments = expectedNumberOfArguments; \
    napi_value variableName; \
    statusName = napi_get_cb_info(env, info, &numberOfArguments, nullptr, &variableName, nullptr); \
    assert(statusName == napi_ok);

#define getThis_getMacro(_1,_2,_3,_4,NAME,...) NAME
#define getThis(...) getThis_getMacro(__VA_ARGS__, getThis_StatusCheck, getThis_NoStatusCheck)(__VA_ARGS__)




#define getArguments_NoStatusCheck(arguments, numberOfArguments, expectedNumberOfArguments) \
    size_t numberOfArguments = expectedNumberOfArguments; \
    napi_value arguments[expectedNumberOfArguments]; \
    napi_get_cb_info(env, info, &numberOfArguments, arguments, nullptr, nullptr);
	
#define getArguments_StatusCheck(arguments, numberOfArguments, expectedNumberOfArguments, statusName) \
    size_t numberOfArguments = expectedNumberOfArguments; \
    napi_value arguments[expectedNumberOfArguments]; \
    statusName = napi_get_cb_info(env, info, &numberOfArguments, arguments, nullptr, nullptr); \
    assert(statusName == napi_ok);

#define getArguments_getMacro(_1,_2,_3,_4,NAME,...) NAME
#define getArguments(...) getArguments_getMacro(__VA_ARGS__, getArguments_StatusCheck, getArguments_NoStatusCheck)(__VA_ARGS__)



#define getStringUtf8Length_NoStatusCheck(variableName, napiText) \
    napi_get_value_string_utf8(env, napiText, NULL, 0, &variableName);

#define getStringUtf8Length_StatusCheck(variableName, napiText, statusName) \
    statusName = napi_get_value_string_utf8(env, napiText, NULL, 0, &variableName); \
    assert(statusName == napi_ok);

#define getStringUtf8Length_getMacro(_1,_2,_3,NAME,...) NAME
#define getStringUtf8Length(...) getStringUtf8Length_getMacro(__VA_ARGS__, getStringUtf8Length_StatusCheck, getStringUtf8Length_NoStatusCheck)(__VA_ARGS__)




#define getStringUtf8_NoStatusCheck(variableName, napiText, length) \
    napi_get_value_string_utf8(env, napiText, variableName, length + 1, 0);

#define getStringUtf8_StatusCheck(variableName, napiText, length, statusName) \
    statusName = napi_get_value_string_utf8(env, napiText, variableName, length + 1, 0); \
    assert(statusName == napi_ok);

#define getStringUtf8_getMacro(_1,_2,_3,_4,NAME,...) NAME
#define getStringUtf8(...) getStringUtf8_getMacro(__VA_ARGS__, getStringUtf8_StatusCheck, getStringUtf8_NoStatusCheck)(__VA_ARGS__)




#define getArrayBuffer_NoStatusCheck(variableName, lengthVariableName, napiArrayBuffer) \
    napi_get_arraybuffer_info(env, napiArrayBuffer, &variableName, &lengthVariableName);

#define getArrayBuffer_StatusCheck(variableName, lengthVariableName, napiArrayBuffer, statusName) \
    statusName = napi_get_arraybuffer_info(env, napiArrayBuffer, &variableName, &lengthVariableName); \
    assert(statusName == napi_ok);

#define getArrayBuffer_getMacro(_1,_2,_3,_4,NAME,...) NAME
#define getArrayBuffer(...) getArrayBuffer_getMacro(__VA_ARGS__, getArrayBuffer_StatusCheck, getArrayBuffer_NoStatusCheck)(__VA_ARGS__)




#define getI32_NoStatusCheck(variableName, napiVariableName) \
    napi_get_value_int32(env, napiVariableName, &variableName);

#define getI32_StatusCheck(variableName, napiVariableName, statusName) \
    statusName = napi_get_value_int32(env, napiVariableName, &variableName); \
    assert(statusName == napi_ok);

#define getI32_getMacro(_1,_2,_3,NAME,...) NAME
#define getI32(...) getI32_getMacro(__VA_ARGS__, getI32_StatusCheck, getI32_NoStatusCheck)(__VA_ARGS__)




#define getU32_NoStatusCheck(variableName, napiVariableName) \
    napi_get_value_uint32(env, napiVariableName, &variableName);

#define getU32_StatusCheck(variableName, napiVariableName, statusName) \
    statusName = napi_get_value_uint32(env, napiVariableName, &variableName); \
    assert(statusName == napi_ok);

#define getU32_getMacro(_1,_2,_3,NAME,...) NAME
#define getU32(...) getU32_getMacro(__VA_ARGS__, getU32_StatusCheck, getU32_NoStatusCheck)(__VA_ARGS__)




#define newObject_NoStatusCheck(variableName) \
    napi_create_object(env, &variableName);

#define newObject_StatusCheck(variableName, statusName) \
    statusName = napi_create_object(env, &variableName); \
    assert(statusName == napi_ok);

#define newObject_getMacro(_1,_2,NAME,...) NAME
#define newObject(...) newObject_getMacro(__VA_ARGS__, newObject_StatusCheck, newObject_NoStatusCheck)(__VA_ARGS__)




#define newFunction_NoStatusCheck(variableName, functionName) \
    napi_create_function(env, "", NAPI_AUTO_LENGTH, functionName, nullptr, &variableName);

#define newFunction_StatusCheck(variableName, functionName, statusName) \
    statusName = napi_create_function(env, "", NAPI_AUTO_LENGTH, functionName, nullptr, &variableName); \
    assert(statusName == napi_ok);

#define newFunction_getMacro(_1,_2,_3,NAME,...) NAME
#define newFunction(...) newFunction_getMacro(__VA_ARGS__, newFunction_StatusCheck, newFunction_NoStatusCheck)(__VA_ARGS__)




#define newStringUtf8_NoStatusCheck(variableName, text, length) \
    napi_create_string_utf8(env, text, length, &variableName);
	
#define newStringUtf8_StatusCheck(variableName, text, length, statusName) \
    statusName = napi_create_string_utf8(env, text, length, &variableName); \
    assert(statusName == napi_ok);

#define newStringUtf8_getMacro(_1,_2,_3,_4,NAME,...) NAME
#define newStringUtf8(...) newStringUtf8_getMacro(__VA_ARGS__, newStringUtf8_StatusCheck, newStringUtf8_NoStatusCheck)(__VA_ARGS__)



#define newStringLatin1_NoStatusCheck(variableName, text, length) \
    napi_create_string_latin1(env, text, length, &variableName);
	
#define newStringLatin1_StatusCheck(variableName, text, length, statusName) \
    statusName = napi_create_string_latin1(env, text, length, &variableName); \
    assert(statusName == napi_ok);

#define newStringLatin1_getMacro(_1,_2,_3,_4,NAME,...) NAME
#define newStringLatin1(...) newStringLatin1_getMacro(__VA_ARGS__, newStringLatin1_StatusCheck, newStringLatin1_NoStatusCheck)(__VA_ARGS__)




#define newI32_NoStatusCheck(variableName, value) \
    napi_create_int32(env, value, &variableName)
	
#define newI32_StatusCheck(variableName, value, statusName) \
    statusName = napi_create_int32(env, value, &variableName) \
    assert(statusName == napi_ok);

#define newI32_getMacro(_1,_2,_3,NAME,...) NAME
#define newI32(...) newI32_getMacro(__VA_ARGS__, newI32_StatusCheck, newI32_NoStatusCheck)(__VA_ARGS__)




#define newDouble_NoStatusCheck(variableName, value) \
    napi_create_double(env, value, &variableName)
	
#define newDoubleStatusCheck(variableName, value, statusName) \
    statusName = napi_create_double(env, value, &variableName) \
    assert(statusName == napi_ok);

#define newDouble_getMacro(_1,_2,_3,NAME,...) NAME
#define newDouble(...) newDouble_getMacro(__VA_ARGS__, newDouble_StatusCheck, newDouble_NoStatusCheck)(__VA_ARGS__)




#define assignArrayBuffer_NoStatusCheck(variableName, buffer, length) \
    napi_create_external_arraybuffer(env, buffer, length, 0, NULL, &variableName);
	
#define assignArrayBuffer_StatusCheck(variableName, buffer, length, statusName) \
    statusName = napi_create_external_arraybuffer(env, buffer, length, 0, NULL, &variableName); \
    assert(statusName == napi_ok);

#define assignArrayBuffer_getMacro(_1,_2,_3,_4,NAME,...) NAME
#define assignArrayBuffer(...) assignArrayBuffer_getMacro(__VA_ARGS__, assignArrayBuffer_StatusCheck, assignArrayBuffer_NoStatusCheck)(__VA_ARGS__)




#define var(variableName) \
    napi_value variableName;
