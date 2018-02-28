typedef char*                STRZ;
typedef char*                STR;
typedef char*                UTF8Z;
typedef char*                UTF8;
typedef uint16_t*            UTF16;
typedef void*                POINTER;
typedef uint8_t              U8;
typedef int8_t               S8;
typedef uint16_t             U16;
typedef int16_t              S16;
typedef uint32_t             U32;
typedef int32_t              S32;
typedef uint64_t             U64;
typedef int64_t              S64;

struct i48_s {
  U16   high;
  U32   low;
};
typedef struct i48_s U48;
typedef struct i48_s S48;

struct i24_s {
  U8    high;
  U16   low;
};
typedef struct i24_s U24;
typedef struct i24_s S24;

#define getU24(destination, source) \
  destination = source->low; \
  destination |= (source->high) << 16;

#define setU24(destination, source) \
  source->low = destination & 0xffff; \
  source->high = (destination << 16) & 0xff;


#define getU48(destination, source) \
  destination = source->low; \
  destination |= (source->high) << 32;

#define setU48(destination, source) \
  source->low = destination & 0xffffffff; \
  source->high = (destination << 32) & 0xffff;
