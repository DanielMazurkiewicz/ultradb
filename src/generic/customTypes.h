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
typedef float                F32;
typedef double               F64;
typedef __int128             S128;
typedef unsigned __int128    U128;

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

#define t_getU24(destination, source) \
  destination = source->low; \
  destination |= (source->high) << 16;

#define t_setU24(destination, source) \
  source->low = destination & 0xffff; \
  source->high = (destination << 16) & 0xff;


#define t_getU48(destination, source) \
  destination = source->low; \
  destination |= (source->high) << 32;

#define t_setU48(destination, source) \
  source->low = destination & 0xffffffff; \
  source->high = (destination << 32) & 0xffff;

#define sizeOfU128 16
#define sizeOfS128 16

#define sizeOfF64 8
#define sizeOfU64 8
#define sizeOfS64 8

#define sizeOfU53 8 //max safe integer in javascript
#define sizeOfS53 8

#define sizeOfU48 6
#define sizeOfS48 6

#define sizeOfF32 4
#define sizeOfU32 4
#define sizeOfS32 4

#define sizeOfU24 3
#define sizeOfS24 3

#define sizeOfU16 2
#define sizeOfS16 2

#define sizeOfU8 1
#define sizeOfS8 1
