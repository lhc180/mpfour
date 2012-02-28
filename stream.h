#ifndef __STREAM_H__
#define __STREAM_H__

// 类型定义.
#ifndef _STDINT_H
typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned uint32_t;
typedef long long int64_t;
typedef unsigned long long uint64_t;
typedef unsigned long size_t;
#endif

// 打开方式.
#define MODE_READ             (1)
#define MODE_WRITE            (2)
#define MODE_READWRITEFILTER  (3)
#define MODE_EXISTING         (4)
#define MODE_CREATE           (8)

// 读写结构定义.
typedef struct stream {
   void* (*open)(struct stream *stream_s, const char* filename, int mode);
   int (*read)(struct stream *stream_s, void* buf, int size);
   int (*write)(struct stream *stream_s, void *buf, int size);
   int (*peek)(struct stream *stream_s, void* buf, int size);
   uint64_t (*seek)(struct stream *stream_s, int64_t offset, int whence);
   uint64_t (*tell)(struct stream *stream_s);
   int (*close)(struct stream *stream_s);
   void* opaque;
} stream_t;

// 读写宏定义.
#define stream_open(s, filename, mode) ((stream_t*)s)->open(((stream_t*)s), filename, mode)
#define stream_read(s, buf, size) ((stream_t*)s)->read(((stream_t*)s), buf, size)
#define stream_write(s, buf, size) ((stream_t*)s)->write(((stream_t*)s), buf, size)
#define stream_peek(s, buf, size) ((stream_t*)s)->peek(((stream_t*)s), buf, size)
#define stream_seek(s, offset, whence) ((stream_t*)s)->seek(((stream_t*)s), offset, whence)
#define stream_tell(s) ((stream_t*)s)->tell(((stream_t*)s))
#define stream_close(s) ((stream_t*)s)->close(((stream_t*)s))

// 文件读写.
void* file_open(stream_t *stream_s, const char* filename, int mode);
int file_read(stream_t *stream_s, void* buf, int size);
int file_write(stream_t *stream_s, void *buf, int size);
int file_peek(stream_t *stream_s, void* buf, int size);
uint64_t file_seek(stream_t *stream_s, int64_t offset, int whence);
uint64_t file_tell(stream_t *stream_s);
int file_close(stream_t *stream_s);

// 创建文件读写流.
stream_t* create_file_stream();
void destory_file_stream(stream_t* stream_s);


// 一个带读写缓冲的文件stream实现.
#define READ_BUFFER_SIZE   10485760
#define WRITE_BUFFER_SIZE  10485760

typedef struct buf_stream {
   stream_t s;

   struct read_buf {
      void* buf;        // 读缓冲.
      int64_t bufsize;  // 缓冲大小.
      int64_t offset;   // 在文件中的offset.
   } read_buf_s;

   struct write_buf {
      void* buf;        // 写缓冲.
      int64_t bufsize;  // 缓冲大小.
      int64_t offset;   // 写文件的offset.
   } write_buf_s;

   uint64_t offset;     // 当前读写指针在文件中的offset.

} buf_stream_t;

// 带缓冲的读写实现.
int buf_file_read(stream_t *stream_s, void* buf, int size);
int buf_file_write(stream_t *stream_s, void *buf, int size);
int buf_file_peek(stream_t *stream_s, void* buf, int size);
uint64_t buf_file_seek(stream_t *stream_s, int64_t offset, int whence);
int buf_file_close(stream_t *stream_s);

// 创建带缓冲的文件读写流.
stream_t* create_buf_file_stream();
void destory_buf_file_stream(stream_t* stream_s);

// 大小端读写操作实现.
#define LIL_ENDIAN	1234
#define BIG_ENDIAN	4321
/* #define BYTEORDER    1234 ? 让下面宏根据系统进行判断大小端. */

#ifndef BYTEORDER
#if defined(__hppa__) || \
   defined(__m68k__) || defined(mc68000) || defined(_M_M68K) || \
   (defined(__MIPS__) && defined(__MISPEB__)) || \
   defined(__ppc__) || defined(__POWERPC__) || defined(_M_PPC) || \
   defined(__sparc__)
#define BYTEORDER	BIG_ENDIAN
#else
#define BYTEORDER	LIL_ENDIAN
#endif
#endif /* !BYTEORDER */

uint16_t Swap16(uint16_t x);
uint32_t Swap32(uint32_t x);
uint64_t Swap64(uint64_t x);

#if BYTEORDER == LIL_ENDIAN
#define SwapLE16(X)	(X)
#define SwapLE32(X)	(X)
#define SwapLE64(X)	(X)
#define SwapBE16(X)	Swap16(X)
#define SwapBE32(X)	Swap32(X)
#define SwapBE64(X)	Swap64(X)
#else
#define SwapLE16(X)	Swap16(X)
#define SwapLE32(X)	Swap32(X)
#define SwapLE64(X)	Swap64(X)
#define SwapBE16(X)	(X)
#define SwapBE32(X)	(X)
#define SwapBE64(X)	(X)
#endif

uint16_t read_le16(stream_t *src);
uint16_t read_be16(stream_t *src);
uint32_t read_le32(stream_t *src);
uint32_t read_be32(stream_t *src);
uint64_t read_le64(stream_t *src);
uint64_t read_be64(stream_t *src);

int write_le16(stream_t *dst, uint16_t value);
int write_be16(stream_t *dst, uint16_t value);
int write_le32(stream_t *dst, uint32_t value);
int write_be32(stream_t *dst, uint32_t value);
int write_le64(stream_t *dst, uint64_t value);
int write_be64(stream_t *dst, uint64_t value);

#endif // __STREAM_H__
