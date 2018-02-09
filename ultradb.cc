#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#include <node_api.h>
#include <assert.h>



napi_value close(napi_env env, const napi_callback_info info) {
    napi_status status;
    const size_t argc_expected = 0;
    size_t argc = argc_expected;
    napi_value es_this;
    status = napi_get_cb_info(env, info, &argc, nullptr, &es_this, nullptr);
    assert(status == napi_ok);

    napi_value rawData;
    status = napi_get_named_property(env, es_this, "rawData", &rawData);
    assert(status == napi_ok);

    void *p;
    size_t pLength;

    status = napi_get_arraybuffer_info(env,
                                      rawData,
                                      &p,
                                      &pLength);
    assert(status == napi_ok);

    if (munmap(p, pLength) == -1) {
      napi_value error;
      status = napi_create_string_utf8(env, "munmap", 6, &error);
      assert(status == napi_ok);
      status = napi_set_named_property(env, es_this, "error", error);
      assert(status == napi_ok);
    }

    return es_this;
}

napi_value CreateObject(napi_env env, const napi_callback_info info) {
  napi_status status;

  size_t argc = 2;
  napi_value args[2];
  status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
  assert(status == napi_ok);


  napi_value obj;
  status = napi_create_object(env, &obj);
  assert(status == napi_ok);

  status = napi_set_named_property(env, obj, "dbFileName", args[0]);
  assert(status == napi_ok);

  size_t strLength;
  napi_get_value_string_utf8(env, args[0], NULL, 0, &strLength);
  assert(status == napi_ok);
  char fileName[strLength + 1];
  napi_get_value_string_utf8(env, args[0], fileName, strLength + 1, 0);


  napi_value error;


  int fd;
  fd = open(fileName, O_RDONLY);
  if (fd == -1) {
    status = napi_create_string_utf8(env, "opening file", 12, &error);
    assert(status == napi_ok);
    status = napi_set_named_property(env, obj, "error", error);
    assert(status == napi_ok);
    return obj;
  }

  struct stat sb;
  if (fstat(fd, &sb) == -1) {
    status = napi_create_string_utf8(env, "fstat", 5, &error);
    assert(status == napi_ok);
    status = napi_set_named_property(env, obj, "error", error);
    assert(status == napi_ok);
    return obj;
  }


  if (!S_ISREG(sb.st_mode)) {
    status = napi_create_string_utf8(env, "not a file", 10, &error);
    assert(status == napi_ok);
    status = napi_set_named_property(env, obj, "error", error);
    assert(status == napi_ok);
    return obj;
  }

  void *p;
  p = mmap (0, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);

  if (p == MAP_FAILED) {
    status = napi_create_string_utf8(env, "mmap", 4, &error);
    assert(status == napi_ok);
    status = napi_set_named_property(env, obj, "error", error);
    assert(status == napi_ok);
    return obj;
  }

  if (close(fd) == -1) {
    status = napi_create_string_utf8(env, "close", 5, &error);
    assert(status == napi_ok);
    status = napi_set_named_property(env, obj, "error", error);
    assert(status == napi_ok);
    return obj;
  }

  napi_value fileData;
  status = napi_create_external_arraybuffer(env,
                                 p,
                                 sb.st_size,
                                 0,
                                 NULL,
                                 &fileData);
  assert(status == napi_ok);
  status = napi_set_named_property(env, obj, "rawData", fileData);
  assert(status == napi_ok);

  napi_value closeProperty;
  status = napi_create_function(env, "", NAPI_AUTO_LENGTH, close, nullptr, &closeProperty);
  assert(status == napi_ok);
  status = napi_set_named_property(env, obj, "close", closeProperty);
  assert(status == napi_ok);

  //off_t len;

  return obj;
}

napi_value Init(napi_env env, napi_value exports) {
  napi_value new_exports;
  napi_status status =
      napi_create_function(env, "", NAPI_AUTO_LENGTH, CreateObject, nullptr, &new_exports);
  assert(status == napi_ok);
  return new_exports;
}

NAPI_MODULE(ultradb, Init)
