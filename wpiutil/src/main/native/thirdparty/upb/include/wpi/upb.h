/* Amalgamated source file */

/*
 * This is where we define internal portability macros used across upb.
 *
 * All of these macros are undef'd in undef.inc to avoid leaking them to users.
 *
 * The correct usage is:
 *
 *   #include "upb/foobar.h"
 *   #include "upb/baz.h"
 *
 *   // MUST be last included header.
 *   #include "upb/port/def.inc"
 *
 *   // Code for this file.
 *   // <...>
 *
 *   // Can be omitted for .c files, required for .h.
 *   #include "upb/port/undef.inc"
 *
 * This file is private and must not be included by users!
 */

#if !((defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L) || \
      (defined(__cplusplus) && __cplusplus >= 201402L) ||           \
      (defined(_MSC_VER) && _MSC_VER >= 1900))
#error upb requires C99 or C++14 or MSVC >= 2015.
#endif

#ifdef __GCC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wgnu-flexible-array-initializer"
#pragma GCC diagnostic ignored "-Wc11-extensions"
#pragma GCC diagnostic ignored "-Wsign-compare"
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-flexible-array-initializer"
#pragma clang diagnostic ignored "-Wc11-extensions"
#pragma clang diagnostic ignored "-Wsign-compare"
#endif

// Portable check for GCC minimum version:
// https://gcc.gnu.org/onlinedocs/cpp/Common-Predefined-Macros.html
#if defined(__GNUC__) && defined(__GNUC_MINOR__) && defined(__GNUC_PATCHLEVEL__)
#define WPI_UPB_GNUC_MIN(x, y) \
  (__GNUC__ > (x) || __GNUC__ == (x) && __GNUC_MINOR__ >= (y))
#else
#define WPI_UPB_GNUC_MIN(x, y) 0
#endif

#include <assert.h>
#include <setjmp.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#ifndef UINTPTR_MAX
Error, UINTPTR_MAX is undefined
#endif

#if UINTPTR_MAX == 0xffffffff
#define WPI_UPB_SIZE(size32, size64) size32
#else
#define WPI_UPB_SIZE(size32, size64) size64
#endif

/* If we always read/write as a consistent type to each address, this shouldn't
 * violate aliasing.
 */
#define WPI_UPB_PTR_AT(msg, ofs, type) ((type*)((char*)(msg) + (ofs)))

#define WPI_UPB_MAPTYPE_STRING 0

// WPI_UPB_EXPORT: always generate a public symbol.
#if defined(__GNUC__) || defined(__clang__)
#define WPI_UPB_EXPORT __attribute__((visibility("default"))) __attribute__((used))
#else
#define WPI_UPB_EXPORT
#endif

// WPI_UPB_INLINE: inline if possible, emit standalone code if required.
#ifdef __cplusplus
#define WPI_UPB_INLINE inline
#elif defined (__GNUC__) || defined(__clang__)
#define WPI_UPB_INLINE static __inline__
#else
#define WPI_UPB_INLINE static
#endif

#ifdef WPI_UPB_BUILD_API
#define WPI_UPB_API WPI_UPB_EXPORT
#define WPI_UPB_API_INLINE WPI_UPB_EXPORT
#else
#define WPI_UPB_API
#define WPI_UPB_API_INLINE WPI_UPB_INLINE
#endif

#define WPI_UPB_MALLOC_ALIGN 8
#define WPI_UPB_ALIGN_UP(size, align) (((size) + (align) - 1) / (align) * (align))
#define WPI_UPB_ALIGN_DOWN(size, align) ((size) / (align) * (align))
#define WPI_UPB_ALIGN_MALLOC(size) WPI_UPB_ALIGN_UP(size, WPI_UPB_MALLOC_ALIGN)
#ifdef __clang__
#define WPI_UPB_ALIGN_OF(type) _Alignof(type)
#else
#define WPI_UPB_ALIGN_OF(type) offsetof (struct { char c; type member; }, member)
#endif

#ifdef _MSC_VER
// Some versions of our Windows compiler don't support the C11 syntax.
#define WPI_UPB_ALIGN_AS(x) __declspec(align(x))
#else
#define WPI_UPB_ALIGN_AS(x) _Alignas(x)
#endif

// Hints to the compiler about likely/unlikely branches.
#if defined (__GNUC__) || defined(__clang__)
#define WPI_UPB_LIKELY(x) __builtin_expect((bool)(x), 1)
#define WPI_UPB_UNLIKELY(x) __builtin_expect((bool)(x), 0)
#else
#define WPI_UPB_LIKELY(x) (x)
#define WPI_UPB_UNLIKELY(x) (x)
#endif

// Macros for function attributes on compilers that support them.
#ifdef __GNUC__
#define WPI_UPB_FORCEINLINE __inline__ __attribute__((always_inline)) static
#define WPI_UPB_NOINLINE __attribute__((noinline))
#define WPI_UPB_NORETURN __attribute__((__noreturn__))
#define WPI_UPB_PRINTF(str, first_vararg) __attribute__((format (printf, str, first_vararg)))
#elif defined(_MSC_VER)
#define WPI_UPB_NOINLINE
#define WPI_UPB_FORCEINLINE static
#define WPI_UPB_NORETURN __declspec(noreturn)
#define WPI_UPB_PRINTF(str, first_vararg)
#else  /* !defined(__GNUC__) */
#define WPI_UPB_FORCEINLINE static
#define WPI_UPB_NOINLINE
#define WPI_UPB_NORETURN
#define WPI_UPB_PRINTF(str, first_vararg)
#endif

#define WPI_UPB_MAX(x, y) ((x) > (y) ? (x) : (y))
#define WPI_UPB_MIN(x, y) ((x) < (y) ? (x) : (y))

#define WPI_UPB_UNUSED(var) (void)var

// WPI_UPB_ASSUME(): in release mode, we tell the compiler to assume this is true.
#ifdef NDEBUG
#ifdef __GNUC__
#define WPI_UPB_ASSUME(expr) if (!(expr)) __builtin_unreachable()
#elif defined _MSC_VER
#define WPI_UPB_ASSUME(expr) if (!(expr)) __assume(0)
#else
#define WPI_UPB_ASSUME(expr) do {} while (false && (expr))
#endif
#else
#define WPI_UPB_ASSUME(expr) assert(expr)
#endif

/* WPI_UPB_ASSERT(): in release mode, we use the expression without letting it be
 * evaluated.  This prevents "unused variable" warnings. */
#ifdef NDEBUG
#define WPI_UPB_ASSERT(expr) do {} while (false && (expr))
#else
#define WPI_UPB_ASSERT(expr) assert(expr)
#endif

#if defined(__GNUC__) || defined(__clang__)
#define WPI_UPB_UNREACHABLE() do { assert(0); __builtin_unreachable(); } while(0)
#elif defined(_MSC_VER)
#define WPI_UPB_UNREACHABLE() \
  do {                    \
    assert(0);            \
    __assume(0);          \
  } while (0)
#else
#define WPI_UPB_UNREACHABLE() do { assert(0); } while(0)
#endif

/* WPI_UPB_SETJMP() / WPI_UPB_LONGJMP(): avoid setting/restoring signal mask. */
#ifdef __APPLE__
#define WPI_UPB_SETJMP(buf) _setjmp(buf)
#define WPI_UPB_LONGJMP(buf, val) _longjmp(buf, val)
#else
#define WPI_UPB_SETJMP(buf) setjmp(buf)
#define WPI_UPB_LONGJMP(buf, val) longjmp(buf, val)
#endif

#ifdef __GNUC__
#define WPI_UPB_USE_C11_ATOMICS
#define WPI_UPB_ATOMIC(T) _Atomic(T)
#else
#define WPI_UPB_ATOMIC(T) T
#endif

/* WPI_UPB_PTRADD(ptr, ofs): add pointer while avoiding "NULL + 0" UB */
#define WPI_UPB_PTRADD(ptr, ofs) ((ofs) ? (ptr) + (ofs) : (ptr))

#define WPI_UPB_PRIVATE(x) x##_dont_copy_me__wpi_upb_internal_use_only

#ifdef WPI_UPB_ALLOW_PRIVATE_ACCESS__FOR_BITS_ONLY
#define WPI_UPB_ONLYBITS(x) x
#else
#define WPI_UPB_ONLYBITS(x) WPI_UPB_PRIVATE(x)
#endif

/* Configure whether fasttable is switched on or not. *************************/

#ifdef __has_attribute
#define WPI_UPB_HAS_ATTRIBUTE(x) __has_attribute(x)
#else
#define WPI_UPB_HAS_ATTRIBUTE(x) 0
#endif

#if WPI_UPB_HAS_ATTRIBUTE(musttail)
#define WPI_UPB_MUSTTAIL __attribute__((musttail))
#else
#define WPI_UPB_MUSTTAIL
#endif

#undef WPI_UPB_HAS_ATTRIBUTE

/* This check is not fully robust: it does not require that we have "musttail"
 * support available. We need tail calls to avoid consuming arbitrary amounts
 * of stack space.
 *
 * GCC/Clang can mostly be trusted to generate tail calls as long as
 * optimization is enabled, but, debug builds will not generate tail calls
 * unless "musttail" is available.
 *
 * We should probably either:
 *   1. require that the compiler supports musttail.
 *   2. add some fallback code for when musttail isn't available (ie. return
 *      instead of tail calling). This is safe and portable, but this comes at
 *      a CPU cost.
 */
#if (defined(__x86_64__) || defined(__aarch64__)) && defined(__GNUC__)
#define WPI_UPB_FASTTABLE_SUPPORTED 1
#else
#define WPI_UPB_FASTTABLE_SUPPORTED 0
#endif

/* define WPI_UPB_ENABLE_FASTTABLE to force fast table support.
 * This is useful when we want to ensure we are really getting fasttable,
 * for example for testing or benchmarking. */
#if defined(WPI_UPB_ENABLE_FASTTABLE)
#if !WPI_UPB_FASTTABLE_SUPPORTED
#error fasttable is x86-64/ARM64 only and requires GCC or Clang.
#endif
#define WPI_UPB_FASTTABLE 1
/* Define WPI_UPB_TRY_ENABLE_FASTTABLE to use fasttable if possible.
 * This is useful for releasing code that might be used on multiple platforms,
 * for example the PHP or Ruby C extensions. */
#elif defined(WPI_UPB_TRY_ENABLE_FASTTABLE)
#define WPI_UPB_FASTTABLE WPI_UPB_FASTTABLE_SUPPORTED
#else
#define WPI_UPB_FASTTABLE 0
#endif

/* WPI_UPB_FASTTABLE_INIT() allows protos compiled for fasttable to gracefully
 * degrade to non-fasttable if the runtime or platform do not support it. */
#if !WPI_UPB_FASTTABLE
#define WPI_UPB_FASTTABLE_INIT(...)
#define WPI_UPB_FASTTABLE_MASK(mask) ((uint8_t)(-1))
#else
#define WPI_UPB_FASTTABLE_INIT(...) __VA_ARGS__
#define WPI_UPB_FASTTABLE_MASK(mask) mask
#endif

#undef WPI_UPB_FASTTABLE_SUPPORTED

/* ASAN poisoning (for arena).
 * If using UPB from an interpreted language like Ruby, a build of the
 * interpreter compiled with ASAN enabled must be used in order to get sane and
 * expected behavior.
 */

/* Due to preprocessor limitations, the conditional logic for setting
 * UPN_CLANG_ASAN below cannot be consolidated into a portable one-liner.
 * See https://gcc.gnu.org/onlinedocs/cpp/_005f_005fhas_005fattribute.html.
 */
#if defined(__has_feature)
#if __has_feature(address_sanitizer)
#define WPI_UPB_CLANG_ASAN 1
#else
#define WPI_UPB_CLANG_ASAN 0
#endif
#else
#define WPI_UPB_CLANG_ASAN 0
#endif

#if defined(__SANITIZE_ADDRESS__) || WPI_UPB_CLANG_ASAN
#define WPI_UPB_ASAN 1
#define WPI_UPB_ASAN_GUARD_SIZE 32
#ifdef __cplusplus
    extern "C" {
#endif
void __asan_poison_memory_region(void const volatile *addr, size_t size);
void __asan_unpoison_memory_region(void const volatile *addr, size_t size);
#ifdef __cplusplus
}  /* extern "C" */
#endif
#define WPI_UPB_POISON_MEMORY_REGION(addr, size) \
  __asan_poison_memory_region((addr), (size))
#define WPI_UPB_UNPOISON_MEMORY_REGION(addr, size) \
  __asan_unpoison_memory_region((addr), (size))
#else
#define WPI_UPB_ASAN 0
#define WPI_UPB_ASAN_GUARD_SIZE 0
#define WPI_UPB_POISON_MEMORY_REGION(addr, size) \
  ((void)(addr), (void)(size))
#define WPI_UPB_UNPOISON_MEMORY_REGION(addr, size) \
  ((void)(addr), (void)(size))
#endif

/* Disable proto2 arena behavior (TEMPORARY) **********************************/

#ifdef WPI_UPB_DISABLE_CLOSED_ENUM_CHECKING
#define WPI_UPB_TREAT_CLOSED_ENUMS_LIKE_OPEN 1
#else
#define WPI_UPB_TREAT_CLOSED_ENUMS_LIKE_OPEN 0
#endif

#if defined(__cplusplus)
#if defined(__clang__) || WPI_UPB_GNUC_MIN(6, 0)
// https://gcc.gnu.org/gcc-6/changes.html
#if __cplusplus >= 201402L
#define WPI_UPB_DEPRECATED [[deprecated]]
#else
#define WPI_UPB_DEPRECATED __attribute__((deprecated))
#endif
#else
#define WPI_UPB_DEPRECATED
#endif
#else
#define WPI_UPB_DEPRECATED
#endif

// begin:google_only
// #define WPI_UPB_IS_GOOGLE3
// end:google_only

#if defined(WPI_UPB_IS_GOOGLE3) && !defined(WPI_UPB_BOOTSTRAP_STAGE0)
#define WPI_UPB_DESC(sym) proto2_##sym
#define WPI_UPB_DESC_MINITABLE(sym) &proto2__##sym##_msg_init
#elif defined(WPI_UPB_BOOTSTRAP_STAGE0)
#define WPI_UPB_DESC(sym) google_protobuf_##sym
#define WPI_UPB_DESC_MINITABLE(sym) google__protobuf__##sym##_msg_init()
#else
#define WPI_UPB_DESC(sym) google_protobuf_##sym
#define WPI_UPB_DESC_MINITABLE(sym) &google__protobuf__##sym##_msg_init
#endif


// Linker arrays combine elements from multiple translation units into a single
// array that can be iterated over at runtime.
//
// It is an alternative to pre-main "registration" functions.
//
// Usage:
//
//   // In N translation units.
//   WPI_UPB_LINKARR_APPEND(foo_array) static int elems[3] = {1, 2, 3};
//
//   // At runtime:
//   WPI_UPB_LINKARR_DECLARE(foo_array, int);
//
//   void f() {
//     const int* start = WPI_UPB_LINKARR_START(foo_array);
//     const int* stop = WPI_UPB_LINKARR_STOP(foo_array);
//     for (const int* p = start; p < stop; p++) {
//       // Windows can introduce zero padding, so we have to skip zeroes.
//       if (*p != 0) {
//         vec.push_back(*p);
//       }
//     }
//   }

#if defined(__ELF__) || defined(__wasm__)

#define WPI_UPB_LINKARR_APPEND(name) \
  __attribute__((retain, used, section("linkarr_" #name)))
#define WPI_UPB_LINKARR_DECLARE(name, type)     \
  extern type const __start_linkarr_##name; \
  extern type const __stop_linkarr_##name;  \
  WPI_UPB_LINKARR_APPEND(name) type WPI_UPB_linkarr_internal_empty_##name[1]
#define WPI_UPB_LINKARR_START(name) (&__start_linkarr_##name)
#define WPI_UPB_LINKARR_STOP(name) (&__stop_linkarr_##name)

#elif defined(__MACH__)

/* As described in: https://stackoverflow.com/a/22366882 */
#define WPI_UPB_LINKARR_APPEND(name) \
  __attribute__((retain, used, section("__DATA,__la_" #name)))
#define WPI_UPB_LINKARR_DECLARE(name, type)           \
  extern type const __start_linkarr_##name __asm( \
      "section$start$__DATA$__la_" #name);        \
  extern type const __stop_linkarr_##name __asm(  \
      "section$end$__DATA$"                       \
      "__la_" #name);                             \
  WPI_UPB_LINKARR_APPEND(name) type WPI_UPB_linkarr_internal_empty_##name[1]
#define WPI_UPB_LINKARR_START(name) (&__start_linkarr_##name)
#define WPI_UPB_LINKARR_STOP(name) (&__stop_linkarr_##name)

#elif defined(_MSC_VER) && defined(__clang__)

/* See:
 *   https://devblogs.microsoft.com/oldnewthing/20181107-00/?p=100155
 *   https://devblogs.microsoft.com/oldnewthing/20181108-00/?p=100165
 *   https://devblogs.microsoft.com/oldnewthing/20181109-00/?p=100175 */

// Usage of __attribute__ here probably means this is Clang-specific, and would
// not work on MSVC.
#define WPI_UPB_LINKARR_APPEND(name) \
  __declspec(allocate("la_" #name "$j")) __attribute__((retain, used))
#define WPI_UPB_LINKARR_DECLARE(name, type)                               \
  __declspec(allocate("la_" #name "$a")) type __start_linkarr_##name; \
  __declspec(allocate("la_" #name "$z")) type __stop_linkarr_##name;  \
  WPI_UPB_LINKARR_APPEND(name) type WPI_UPB_linkarr_internal_empty_##name[1] = {0}
#define WPI_UPB_LINKARR_START(name) (&__start_linkarr_##name)
#define WPI_UPB_LINKARR_STOP(name) (&__stop_linkarr_##name)

#else

// Linker arrays are not supported on this platform.  Make appends a no-op but
// don't define the other macros.
#define WPI_UPB_LINKARR_APPEND(name)

#endif

#ifndef WPI_UPB_BASE_STATUS_H_
#define WPI_UPB_BASE_STATUS_H_

#include <stdarg.h>

// Must be last.

#define _kUpb_Status_MaxMessage 511

typedef struct {
  bool ok;
  char msg[_kUpb_Status_MaxMessage];  // Error message; NULL-terminated.
} wpi_upb_Status;

#ifdef __cplusplus
extern "C" {
#endif

WPI_UPB_API const char* wpi_upb_Status_ErrorMessage(const wpi_upb_Status* status);
WPI_UPB_API bool wpi_upb_Status_IsOk(const wpi_upb_Status* status);

// These are no-op if |status| is NULL.
WPI_UPB_API void wpi_upb_Status_Clear(wpi_upb_Status* status);
void wpi_upb_Status_SetErrorMessage(wpi_upb_Status* status, const char* msg);
void wpi_upb_Status_SetErrorFormat(wpi_upb_Status* status, const char* fmt, ...)
    WPI_UPB_PRINTF(2, 3);
void wpi_upb_Status_VSetErrorFormat(wpi_upb_Status* status, const char* fmt,
                                va_list args) WPI_UPB_PRINTF(2, 0);
void wpi_upb_Status_VAppendErrorFormat(wpi_upb_Status* status, const char* fmt,
                                   va_list args) WPI_UPB_PRINTF(2, 0);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_BASE_STATUS_H_ */

#ifndef WPI_UPB_GENERATED_CODE_SUPPORT_H_
#define WPI_UPB_GENERATED_CODE_SUPPORT_H_

// IWYU pragma: begin_exports

#ifndef WPI_UPB_BASE_UPCAST_H_
#define WPI_UPB_BASE_UPCAST_H_

// Must be last.

// This macro provides a way to upcast message pointers in a way that is
// somewhat more bulletproof than blindly casting a pointer. Example:
//
// typedef struct {
//   wpi_upb_Message WPI_UPB_PRIVATE(base);
// } pkg_FooMessage;
//
// void f(pkg_FooMessage* msg) {
//   wpi_upb_Decode(WPI_UPB_UPCAST(msg), ...);
// }

#define WPI_UPB_UPCAST(x) (&(x)->base##_dont_copy_me__wpi_upb_internal_use_only)


#endif /* WPI_UPB_BASE_UPCAST_H_ */

#ifndef WPI_UPB_MESSAGE_ACCESSORS_H_
#define WPI_UPB_MESSAGE_ACCESSORS_H_

#ifndef WPI_UPB_BASE_STRING_VIEW_H_
#define WPI_UPB_BASE_STRING_VIEW_H_

#include <string.h>

// Must be last.

#define WPI_UPB_STRINGVIEW_INIT(ptr, len) \
  { ptr, len }

#define WPI_UPB_STRINGVIEW_FORMAT "%.*s"
#define WPI_UPB_STRINGVIEW_ARGS(view) (int)(view).size, (view).data

// LINT.IfChange(struct_definition)
typedef struct {
  const char* data;
  size_t size;
} wpi_upb_StringView;

#ifdef __cplusplus
extern "C" {
#endif

WPI_UPB_API_INLINE wpi_upb_StringView wpi_upb_StringView_FromDataAndSize(const char* data,
                                                             size_t size) {
  wpi_upb_StringView ret;
  ret.data = data;
  ret.size = size;
  return ret;
}

WPI_UPB_INLINE wpi_upb_StringView wpi_upb_StringView_FromString(const char* data) {
  return wpi_upb_StringView_FromDataAndSize(data, strlen(data));
}

WPI_UPB_INLINE bool wpi_upb_StringView_IsEqual(wpi_upb_StringView a, wpi_upb_StringView b) {
  return (a.size == b.size) && (!a.size || !memcmp(a.data, b.data, a.size));
}

// LINT.ThenChange(
//  GoogleInternalName1,
//  //depot/google3/third_party/upb/bits/golang/accessor.go:map_go_string,
//  //depot/google3/third_party/upb/bits/typescript/string_view.ts
// )

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_BASE_STRING_VIEW_H_ */

/* wpi_upb_Arena is a specific allocator implementation that uses arena allocation.
 * The user provides an allocator that will be used to allocate the underlying
 * arena blocks.  Arenas by nature do not require the individual allocations
 * to be freed.  However the Arena does allow users to register cleanup
 * functions that will run when the arena is destroyed.
 *
 * A wpi_upb_Arena is *not* thread-safe.
 *
 * You could write a thread-safe arena allocator that satisfies the
 * wpi_upb_alloc interface, but it would not be as efficient for the
 * single-threaded case. */

#ifndef WPI_UPB_MEM_ARENA_H_
#define WPI_UPB_MEM_ARENA_H_

#include <stddef.h>
#include <stdint.h>


#ifndef WPI_UPB_MEM_ALLOC_H_
#define WPI_UPB_MEM_ALLOC_H_

// Must be last.

#ifdef __cplusplus
extern "C" {
#endif

typedef struct wpi_upb_alloc wpi_upb_alloc;

/* A combined `malloc()`/`free()` function.
 * If `size` is 0 then the function acts like `free()`, otherwise it acts like
 * `realloc()`.  Only `oldsize` bytes from a previous allocation are
 * preserved. */
typedef void* wpi_upb_alloc_func(wpi_upb_alloc* alloc, void* ptr, size_t oldsize,
                             size_t size);

/* A wpi_upb_alloc is a possibly-stateful allocator object.
 *
 * It could either be an arena allocator (which doesn't require individual
 * `free()` calls) or a regular `malloc()` (which does).  The client must
 * therefore free memory unless it knows that the allocator is an arena
 * allocator. */
struct wpi_upb_alloc {
  wpi_upb_alloc_func* func;
};

WPI_UPB_INLINE void* wpi_upb_malloc(wpi_upb_alloc* alloc, size_t size) {
  WPI_UPB_ASSERT(alloc);
  return alloc->func(alloc, NULL, 0, size);
}

WPI_UPB_INLINE void* wpi_upb_realloc(wpi_upb_alloc* alloc, void* ptr, size_t oldsize,
                             size_t size) {
  WPI_UPB_ASSERT(alloc);
  return alloc->func(alloc, ptr, oldsize, size);
}

WPI_UPB_INLINE void wpi_upb_free(wpi_upb_alloc* alloc, void* ptr) {
  WPI_UPB_ASSERT(alloc);
  alloc->func(alloc, ptr, 0, 0);
}

// The global allocator used by upb. Uses the standard malloc()/free().

extern wpi_upb_alloc wpi_upb_alloc_global;

/* Functions that hard-code the global malloc.
 *
 * We still get benefit because we can put custom logic into our global
 * allocator, like injecting out-of-memory faults in debug/testing builds. */

WPI_UPB_INLINE void* wpi_upb_gmalloc(size_t size) {
  return wpi_upb_malloc(&wpi_upb_alloc_global, size);
}

WPI_UPB_INLINE void* wpi_upb_grealloc(void* ptr, size_t oldsize, size_t size) {
  return wpi_upb_realloc(&wpi_upb_alloc_global, ptr, oldsize, size);
}

WPI_UPB_INLINE void wpi_upb_gfree(void* ptr) { wpi_upb_free(&wpi_upb_alloc_global, ptr); }

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_MEM_ALLOC_H_ */

#ifndef WPI_UPB_MEM_INTERNAL_ARENA_H_
#define WPI_UPB_MEM_INTERNAL_ARENA_H_

#include <stddef.h>
#include <stdint.h>
#include <string.h>

// Must be last.

// This is QUITE an ugly hack, which specifies the number of pointers needed
// to equal (or exceed) the storage required for one wpi_upb_Arena.
//
// We need this because the decoder inlines a wpi_upb_Arena for performance but
// the full struct is not visible outside of arena.c. Yes, I know, it's awful.
#define WPI_UPB_ARENA_SIZE_HACK 7

// LINT.IfChange(wpi_upb_Arena)

struct wpi_upb_Arena {
  char* WPI_UPB_ONLYBITS(ptr);
  char* WPI_UPB_ONLYBITS(end);
};

// LINT.ThenChange(//depot/google3/third_party/upb/bits/typescript/arena.ts:wpi_upb_Arena)

#ifdef __cplusplus
extern "C" {
#endif

void WPI_UPB_PRIVATE(_wpi_upb_Arena_SwapIn)(struct wpi_upb_Arena* des,
                                    const struct wpi_upb_Arena* src);
void WPI_UPB_PRIVATE(_wpi_upb_Arena_SwapOut)(struct wpi_upb_Arena* des,
                                     const struct wpi_upb_Arena* src);

// Returns whether |ptr| was allocated directly by |a| (so care must be used
// with fused arenas).
WPI_UPB_API bool WPI_UPB_ONLYBITS(_wpi_upb_Arena_Contains)(const struct wpi_upb_Arena* a,
                                               void* ptr);

WPI_UPB_INLINE size_t WPI_UPB_PRIVATE(_wpi_upb_ArenaHas)(const struct wpi_upb_Arena* a) {
  return (size_t)(a->WPI_UPB_ONLYBITS(end) - a->WPI_UPB_ONLYBITS(ptr));
}

WPI_UPB_API_INLINE void* wpi_upb_Arena_Malloc(struct wpi_upb_Arena* a, size_t size) {
  void* WPI_UPB_PRIVATE(_wpi_upb_Arena_SlowMalloc)(struct wpi_upb_Arena * a, size_t size);

  size = WPI_UPB_ALIGN_MALLOC(size);
  const size_t span = size + WPI_UPB_ASAN_GUARD_SIZE;
  if (WPI_UPB_UNLIKELY(WPI_UPB_PRIVATE(_wpi_upb_ArenaHas)(a) < span)) {
    return WPI_UPB_PRIVATE(_wpi_upb_Arena_SlowMalloc)(a, span);
  }

  // We have enough space to do a fast malloc.
  void* ret = a->WPI_UPB_ONLYBITS(ptr);
  WPI_UPB_ASSERT(WPI_UPB_ALIGN_MALLOC((uintptr_t)ret) == (uintptr_t)ret);
  WPI_UPB_ASSERT(WPI_UPB_ALIGN_MALLOC(size) == size);
  WPI_UPB_UNPOISON_MEMORY_REGION(ret, size);

  a->WPI_UPB_ONLYBITS(ptr) += span;

  return ret;
}

WPI_UPB_API_INLINE void* wpi_upb_Arena_Realloc(struct wpi_upb_Arena* a, void* ptr,
                                       size_t oldsize, size_t size) {
  oldsize = WPI_UPB_ALIGN_MALLOC(oldsize);
  size = WPI_UPB_ALIGN_MALLOC(size);
  bool is_most_recent_alloc =
      (uintptr_t)ptr + oldsize == (uintptr_t)a->WPI_UPB_ONLYBITS(ptr);

  if (is_most_recent_alloc) {
    ptrdiff_t diff = size - oldsize;
    if ((ptrdiff_t)WPI_UPB_PRIVATE(_wpi_upb_ArenaHas)(a) >= diff) {
      a->WPI_UPB_ONLYBITS(ptr) += diff;
      return ptr;
    }
  } else if (size <= oldsize) {
    return ptr;
  }

  void* ret = wpi_upb_Arena_Malloc(a, size);

  if (ret && oldsize > 0) {
    memcpy(ret, ptr, WPI_UPB_MIN(oldsize, size));
  }

  return ret;
}

WPI_UPB_API_INLINE void wpi_upb_Arena_ShrinkLast(struct wpi_upb_Arena* a, void* ptr,
                                         size_t oldsize, size_t size) {
  oldsize = WPI_UPB_ALIGN_MALLOC(oldsize);
  size = WPI_UPB_ALIGN_MALLOC(size);
  // Must be the last alloc.
  WPI_UPB_ASSERT((char*)ptr + oldsize ==
             a->WPI_UPB_ONLYBITS(ptr) - WPI_UPB_ASAN_GUARD_SIZE);
  WPI_UPB_ASSERT(size <= oldsize);
  a->WPI_UPB_ONLYBITS(ptr) = (char*)ptr + size;
}

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_MEM_INTERNAL_ARENA_H_ */

// Must be last.

typedef struct wpi_upb_Arena wpi_upb_Arena;

#ifdef __cplusplus
extern "C" {
#endif

// Creates an arena from the given initial block (if any -- n may be 0).
// Additional blocks will be allocated from |alloc|.  If |alloc| is NULL, this
// is a fixed-size arena and cannot grow.
WPI_UPB_API wpi_upb_Arena* wpi_upb_Arena_Init(void* mem, size_t n, wpi_upb_alloc* alloc);

WPI_UPB_API void wpi_upb_Arena_Free(wpi_upb_Arena* a);
WPI_UPB_API bool wpi_upb_Arena_Fuse(wpi_upb_Arena* a, wpi_upb_Arena* b);

bool wpi_upb_Arena_IncRefFor(wpi_upb_Arena* a, const void* owner);
void wpi_upb_Arena_DecRefFor(wpi_upb_Arena* a, const void* owner);

size_t wpi_upb_Arena_SpaceAllocated(wpi_upb_Arena* a, size_t* fused_count);
uint32_t wpi_upb_Arena_DebugRefCount(wpi_upb_Arena* a);

WPI_UPB_API_INLINE wpi_upb_Arena* wpi_upb_Arena_New(void) {
  return wpi_upb_Arena_Init(NULL, 0, &wpi_upb_alloc_global);
}

WPI_UPB_API_INLINE void* wpi_upb_Arena_Malloc(struct wpi_upb_Arena* a, size_t size);

WPI_UPB_API_INLINE void* wpi_upb_Arena_Realloc(wpi_upb_Arena* a, void* ptr, size_t oldsize,
                                       size_t size);

// Sets the maximum block size for all arenas. This is a global configuration
// setting that will affect all existing and future arenas. If
// wpi_upb_Arena_Malloc() is called with a size larger than this, we will exceed
// this size and allocate a larger block.
//
// This API is meant for experimentation only. It will likely be removed in
// the future.
void wpi_upb_Arena_SetMaxBlockSize(size_t max);

// Shrinks the last alloc from arena.
// REQUIRES: (ptr, oldsize) was the last malloc/realloc from this arena.
// We could also add a wpi_upb_Arena_TryShrinkLast() which is simply a no-op if
// this was not the last alloc.
WPI_UPB_API_INLINE void wpi_upb_Arena_ShrinkLast(wpi_upb_Arena* a, void* ptr,
                                         size_t oldsize, size_t size);

#ifdef WPI_UPB_TRACING_ENABLED
void wpi_upb_Arena_SetTraceHandler(void (*initArenaTraceHandler)(const wpi_upb_Arena*,
                                                             size_t size),
                               void (*fuseArenaTraceHandler)(const wpi_upb_Arena*,
                                                             const wpi_upb_Arena*),
                               void (*freeArenaTraceHandler)(const wpi_upb_Arena*));
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_MEM_ARENA_H_ */

#ifndef WPI_UPB_MESSAGE_ARRAY_H_
#define WPI_UPB_MESSAGE_ARRAY_H_

#include <stddef.h>


#ifndef WPI_UPB_BASE_DESCRIPTOR_CONSTANTS_H_
#define WPI_UPB_BASE_DESCRIPTOR_CONSTANTS_H_

// Must be last.

// The types a field can have. Note that this list is not identical to the
// types defined in descriptor.proto, which gives INT32 and SINT32 separate
// types (we distinguish the two with the "integer encoding" enum below).
// This enum is an internal convenience only and has no meaning outside of upb.
typedef enum {
  kUpb_CType_Bool = 1,
  kUpb_CType_Float = 2,
  kUpb_CType_Int32 = 3,
  kUpb_CType_UInt32 = 4,
  kUpb_CType_Enum = 5,  // Enum values are int32. TODO: rename
  kUpb_CType_Message = 6,
  kUpb_CType_Double = 7,
  kUpb_CType_Int64 = 8,
  kUpb_CType_UInt64 = 9,
  kUpb_CType_String = 10,
  kUpb_CType_Bytes = 11
} wpi_upb_CType;

// The repeated-ness of each field; this matches descriptor.proto.
typedef enum {
  kUpb_Label_Optional = 1,
  kUpb_Label_Required = 2,
  kUpb_Label_Repeated = 3
} wpi_upb_Label;

// Descriptor types, as defined in descriptor.proto.
typedef enum {
  kUpb_FieldType_Double = 1,
  kUpb_FieldType_Float = 2,
  kUpb_FieldType_Int64 = 3,
  kUpb_FieldType_UInt64 = 4,
  kUpb_FieldType_Int32 = 5,
  kUpb_FieldType_Fixed64 = 6,
  kUpb_FieldType_Fixed32 = 7,
  kUpb_FieldType_Bool = 8,
  kUpb_FieldType_String = 9,
  kUpb_FieldType_Group = 10,
  kUpb_FieldType_Message = 11,
  kUpb_FieldType_Bytes = 12,
  kUpb_FieldType_UInt32 = 13,
  kUpb_FieldType_Enum = 14,
  kUpb_FieldType_SFixed32 = 15,
  kUpb_FieldType_SFixed64 = 16,
  kUpb_FieldType_SInt32 = 17,
  kUpb_FieldType_SInt64 = 18,
} wpi_upb_FieldType;

#define kUpb_FieldType_SizeOf 19

#ifdef __cplusplus
extern "C" {
#endif

// Convert from wpi_upb_FieldType to wpi_upb_CType
WPI_UPB_INLINE wpi_upb_CType wpi_upb_FieldType_CType(wpi_upb_FieldType field_type) {
  static const wpi_upb_CType c_type[] = {
      kUpb_CType_Double,   // kUpb_FieldType_Double
      kUpb_CType_Float,    // kUpb_FieldType_Float
      kUpb_CType_Int64,    // kUpb_FieldType_Int64
      kUpb_CType_UInt64,   // kUpb_FieldType_UInt64
      kUpb_CType_Int32,    // kUpb_FieldType_Int32
      kUpb_CType_UInt64,   // kUpb_FieldType_Fixed64
      kUpb_CType_UInt32,   // kUpb_FieldType_Fixed32
      kUpb_CType_Bool,     // kUpb_FieldType_Bool
      kUpb_CType_String,   // kUpb_FieldType_String
      kUpb_CType_Message,  // kUpb_FieldType_Group
      kUpb_CType_Message,  // kUpb_FieldType_Message
      kUpb_CType_Bytes,    // kUpb_FieldType_Bytes
      kUpb_CType_UInt32,   // kUpb_FieldType_UInt32
      kUpb_CType_Enum,     // kUpb_FieldType_Enum
      kUpb_CType_Int32,    // kUpb_FieldType_SFixed32
      kUpb_CType_Int64,    // kUpb_FieldType_SFixed64
      kUpb_CType_Int32,    // kUpb_FieldType_SInt32
      kUpb_CType_Int64,    // kUpb_FieldType_SInt64
  };

  // -1 here because the enum is one-based but the table is zero-based.
  return c_type[field_type - 1];
}

WPI_UPB_INLINE bool wpi_upb_FieldType_IsPackable(wpi_upb_FieldType field_type) {
  // clang-format off
  const unsigned kUnpackableTypes =
      (1 << kUpb_FieldType_String) |
      (1 << kUpb_FieldType_Bytes) |
      (1 << kUpb_FieldType_Message) |
      (1 << kUpb_FieldType_Group);
  // clang-format on
  return (1 << field_type) & ~kUnpackableTypes;
}

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_BASE_DESCRIPTOR_CONSTANTS_H_ */

#ifndef WPI_UPB_MESSAGE_INTERNAL_ARRAY_H_
#define WPI_UPB_MESSAGE_INTERNAL_ARRAY_H_

#include <stdint.h>
#include <string.h>


// Must be last.

#define _WPI_UPB_ARRAY_MASK_IMM 0x4  // Frozen/immutable bit.
#define _WPI_UPB_ARRAY_MASK_LG2 0x3  // Encoded elem size.
#define _WPI_UPB_ARRAY_MASK_ALL (_WPI_UPB_ARRAY_MASK_IMM | _WPI_UPB_ARRAY_MASK_LG2)

#ifdef __cplusplus
extern "C" {
#endif

// LINT.IfChange(wpi_upb_Array)

// Our internal representation for repeated fields.
struct wpi_upb_Array {
  // This is a tagged pointer. Bits #0 and #1 encode the elem size as follows:
  //   0 maps to elem size 1
  //   1 maps to elem size 4
  //   2 maps to elem size 8
  //   3 maps to elem size 16
  //
  // Bit #2 contains the frozen/immutable flag.
  uintptr_t WPI_UPB_ONLYBITS(data);

  size_t WPI_UPB_ONLYBITS(size);     // The number of elements in the array.
  size_t WPI_UPB_PRIVATE(capacity);  // Allocated storage. Measured in elements.
};

WPI_UPB_INLINE void WPI_UPB_PRIVATE(_wpi_upb_Array_ShallowFreeze)(struct wpi_upb_Array* arr) {
  arr->WPI_UPB_ONLYBITS(data) |= _WPI_UPB_ARRAY_MASK_IMM;
}

WPI_UPB_API_INLINE bool wpi_upb_Array_IsFrozen(const struct wpi_upb_Array* arr) {
  return (arr->WPI_UPB_ONLYBITS(data) & _WPI_UPB_ARRAY_MASK_IMM) != 0;
}

WPI_UPB_INLINE void WPI_UPB_PRIVATE(_wpi_upb_Array_SetTaggedPtr)(struct wpi_upb_Array* array,
                                                     void* data, size_t lg2) {
  WPI_UPB_ASSERT(lg2 != 1);
  WPI_UPB_ASSERT(lg2 <= 4);
  const size_t bits = lg2 - (lg2 != 0);
  array->WPI_UPB_ONLYBITS(data) = (uintptr_t)data | bits;
}

WPI_UPB_INLINE size_t
WPI_UPB_PRIVATE(_wpi_upb_Array_ElemSizeLg2)(const struct wpi_upb_Array* array) {
  const size_t bits = array->WPI_UPB_ONLYBITS(data) & _WPI_UPB_ARRAY_MASK_LG2;
  const size_t lg2 = bits + (bits != 0);
  return lg2;
}

WPI_UPB_API_INLINE const void* wpi_upb_Array_DataPtr(const struct wpi_upb_Array* array) {
  WPI_UPB_PRIVATE(_wpi_upb_Array_ElemSizeLg2)(array);  // Check assertions.
  return (void*)(array->WPI_UPB_ONLYBITS(data) & ~(uintptr_t)_WPI_UPB_ARRAY_MASK_ALL);
}

WPI_UPB_API_INLINE void* wpi_upb_Array_MutableDataPtr(struct wpi_upb_Array* array) {
  return (void*)wpi_upb_Array_DataPtr(array);
}

WPI_UPB_INLINE struct wpi_upb_Array* WPI_UPB_PRIVATE(_wpi_upb_Array_New)(wpi_upb_Arena* arena,
                                                         size_t init_capacity,
                                                         int elem_size_lg2) {
  WPI_UPB_ASSERT(elem_size_lg2 != 1);
  WPI_UPB_ASSERT(elem_size_lg2 <= 4);
  const size_t array_size =
      WPI_UPB_ALIGN_UP(sizeof(struct wpi_upb_Array), WPI_UPB_MALLOC_ALIGN);
  const size_t bytes = array_size + (init_capacity << elem_size_lg2);
  struct wpi_upb_Array* array = (struct wpi_upb_Array*)wpi_upb_Arena_Malloc(arena, bytes);
  if (!array) return NULL;
  WPI_UPB_PRIVATE(_wpi_upb_Array_SetTaggedPtr)
  (array, WPI_UPB_PTR_AT(array, array_size, void), elem_size_lg2);
  array->WPI_UPB_ONLYBITS(size) = 0;
  array->WPI_UPB_PRIVATE(capacity) = init_capacity;
  return array;
}

// Resizes the capacity of the array to be at least min_size.
bool WPI_UPB_PRIVATE(_wpi_upb_Array_Realloc)(struct wpi_upb_Array* array, size_t min_size,
                                     wpi_upb_Arena* arena);

WPI_UPB_API_INLINE bool wpi_upb_Array_Reserve(struct wpi_upb_Array* array, size_t size,
                                      wpi_upb_Arena* arena) {
  WPI_UPB_ASSERT(!wpi_upb_Array_IsFrozen(array));
  if (array->WPI_UPB_PRIVATE(capacity) < size)
    return WPI_UPB_PRIVATE(_wpi_upb_Array_Realloc)(array, size, arena);
  return true;
}

// Resize without initializing new elements.
WPI_UPB_INLINE bool WPI_UPB_PRIVATE(_wpi_upb_Array_ResizeUninitialized)(
    struct wpi_upb_Array* array, size_t size, wpi_upb_Arena* arena) {
  WPI_UPB_ASSERT(!wpi_upb_Array_IsFrozen(array));
  WPI_UPB_ASSERT(size <= array->WPI_UPB_ONLYBITS(size) ||
             arena);  // Allow NULL arena when shrinking.
  if (!wpi_upb_Array_Reserve(array, size, arena)) return false;
  array->WPI_UPB_ONLYBITS(size) = size;
  return true;
}

// This function is intended for situations where elem_size is compile-time
// constant or a known expression of the form (1 << lg2), so that the expression
// i*elem_size does not result in an actual multiplication.
WPI_UPB_INLINE void WPI_UPB_PRIVATE(_wpi_upb_Array_Set)(struct wpi_upb_Array* array, size_t i,
                                            const void* data,
                                            size_t elem_size) {
  WPI_UPB_ASSERT(!wpi_upb_Array_IsFrozen(array));
  WPI_UPB_ASSERT(i < array->WPI_UPB_ONLYBITS(size));
  WPI_UPB_ASSERT(elem_size == 1U << WPI_UPB_PRIVATE(_wpi_upb_Array_ElemSizeLg2)(array));
  char* arr_data = (char*)wpi_upb_Array_MutableDataPtr(array);
  memcpy(arr_data + (i * elem_size), data, elem_size);
}

WPI_UPB_API_INLINE size_t wpi_upb_Array_Size(const struct wpi_upb_Array* arr) {
  return arr->WPI_UPB_ONLYBITS(size);
}

// LINT.ThenChange(GoogleInternalName0)

#ifdef __cplusplus
} /* extern "C" */
#endif

#undef _WPI_UPB_ARRAY_MASK_IMM
#undef _WPI_UPB_ARRAY_MASK_LG2
#undef _WPI_UPB_ARRAY_MASK_ALL


#endif /* WPI_UPB_MESSAGE_INTERNAL_ARRAY_H_ */

// Users should include array.h or map.h instead.
// IWYU pragma: private, include "upb/message/array.h"

#ifndef WPI_UPB_MESSAGE_VALUE_H_
#define WPI_UPB_MESSAGE_VALUE_H_

#include <stdint.h>
#include <string.h>


// Must be last.

#ifdef __cplusplus
extern "C" {
#endif

typedef union {
  bool bool_val;
  float float_val;
  double double_val;
  int32_t int32_val;
  int64_t int64_val;
  uint32_t uint32_val;
  uint64_t uint64_val;
  const struct wpi_upb_Array* array_val;
  const struct wpi_upb_Map* map_val;
  const struct wpi_upb_Message* msg_val;
  wpi_upb_StringView str_val;

  // EXPERIMENTAL: A tagged wpi_upb_Message*.  Users must use this instead of
  // msg_val if unlinked sub-messages may possibly be in use.  See the
  // documentation in kUpb_DecodeOption_ExperimentalAllowUnlinked for more
  // information.
  uintptr_t tagged_msg_val;  // wpi_upb_TaggedMessagePtr
} wpi_upb_MessageValue;

WPI_UPB_API_INLINE wpi_upb_MessageValue wpi_upb_MessageValue_Zero(void) {
  wpi_upb_MessageValue zero;
  memset(&zero, 0, sizeof(zero));
  return zero;
}

typedef union {
  struct wpi_upb_Array* array;
  struct wpi_upb_Map* map;
  struct wpi_upb_Message* msg;
} wpi_upb_MutableMessageValue;

WPI_UPB_API_INLINE wpi_upb_MutableMessageValue wpi_upb_MutableMessageValue_Zero(void) {
  wpi_upb_MutableMessageValue zero;
  memset(&zero, 0, sizeof(zero));
  return zero;
}

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_MESSAGE_VALUE_H_ */

#ifndef WPI_UPB_MINI_TABLE_FIELD_H_
#define WPI_UPB_MINI_TABLE_FIELD_H_

#include <stdint.h>


#ifndef WPI_UPB_MINI_TABLE_INTERNAL_FIELD_H_
#define WPI_UPB_MINI_TABLE_INTERNAL_FIELD_H_

#include <stddef.h>
#include <stdint.h>


#ifndef WPI_UPB_MINI_TABLE_INTERNAL_SIZE_LOG2_H_
#define WPI_UPB_MINI_TABLE_INTERNAL_SIZE_LOG2_H_

#include <stddef.h>
#include <stdint.h>


// Must be last.

#ifdef __cplusplus
extern "C" {
#endif

// Return the log2 of the storage size in bytes for a wpi_upb_CType
WPI_UPB_INLINE int WPI_UPB_PRIVATE(_wpi_upb_CType_SizeLg2)(wpi_upb_CType c_type) {
  static const int8_t size[] = {
      0,               // kUpb_CType_Bool
      2,               // kUpb_CType_Float
      2,               // kUpb_CType_Int32
      2,               // kUpb_CType_UInt32
      2,               // kUpb_CType_Enum
      WPI_UPB_SIZE(2, 3),  // kUpb_CType_Message
      3,               // kUpb_CType_Double
      3,               // kUpb_CType_Int64
      3,               // kUpb_CType_UInt64
      WPI_UPB_SIZE(3, 4),  // kUpb_CType_String
      WPI_UPB_SIZE(3, 4),  // kUpb_CType_Bytes
  };

  // -1 here because the enum is one-based but the table is zero-based.
  return size[c_type - 1];
}

// Return the log2 of the storage size in bytes for a wpi_upb_FieldType
WPI_UPB_INLINE int WPI_UPB_PRIVATE(_wpi_upb_FieldType_SizeLg2)(wpi_upb_FieldType field_type) {
  static const int8_t size[] = {
      3,               // kUpb_FieldType_Double
      2,               // kUpb_FieldType_Float
      3,               // kUpb_FieldType_Int64
      3,               // kUpb_FieldType_UInt64
      2,               // kUpb_FieldType_Int32
      3,               // kUpb_FieldType_Fixed64
      2,               // kUpb_FieldType_Fixed32
      0,               // kUpb_FieldType_Bool
      WPI_UPB_SIZE(3, 4),  // kUpb_FieldType_String
      WPI_UPB_SIZE(2, 3),  // kUpb_FieldType_Group
      WPI_UPB_SIZE(2, 3),  // kUpb_FieldType_Message
      WPI_UPB_SIZE(3, 4),  // kUpb_FieldType_Bytes
      2,               // kUpb_FieldType_UInt32
      2,               // kUpb_FieldType_Enum
      2,               // kUpb_FieldType_SFixed32
      3,               // kUpb_FieldType_SFixed64
      2,               // kUpb_FieldType_SInt32
      3,               // kUpb_FieldType_SInt64
  };

  // -1 here because the enum is one-based but the table is zero-based.
  return size[field_type - 1];
}

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_MINI_TABLE_INTERNAL_SIZE_LOG2_H_ */

// Must be last.

// LINT.IfChange(struct_definition)
struct wpi_upb_MiniTableField {
  uint32_t WPI_UPB_ONLYBITS(number);
  uint16_t WPI_UPB_ONLYBITS(offset);
  int16_t presence;  // If >0, hasbit_index.  If <0, ~oneof_index

  // Indexes into `wpi_upb_MiniTable.subs`
  // Will be set to `kUpb_NoSub` if `descriptortype` != MESSAGE/GROUP/ENUM
  uint16_t WPI_UPB_PRIVATE(submsg_index);

  uint8_t WPI_UPB_PRIVATE(descriptortype);

  // wpi_upb_FieldMode | wpi_upb_LabelFlags | (wpi_upb_FieldRep << kUpb_FieldRep_Shift)
  uint8_t WPI_UPB_ONLYBITS(mode);
};

#define kUpb_NoSub ((uint16_t) - 1)

typedef enum {
  kUpb_FieldMode_Map = 0,
  kUpb_FieldMode_Array = 1,
  kUpb_FieldMode_Scalar = 2,
} wpi_upb_FieldMode;

// Mask to isolate the wpi_upb_FieldMode from field.mode.
#define kUpb_FieldMode_Mask 3

// Extra flags on the mode field.
typedef enum {
  kUpb_LabelFlags_IsPacked = 4,
  kUpb_LabelFlags_IsExtension = 8,
  // Indicates that this descriptor type is an "alternate type":
  //   - for Int32, this indicates that the actual type is Enum (but was
  //     rewritten to Int32 because it is an open enum that requires no check).
  //   - for Bytes, this indicates that the actual type is String (but does
  //     not require any UTF-8 check).
  kUpb_LabelFlags_IsAlternate = 16,
} wpi_upb_LabelFlags;

// Note: we sort by this number when calculating layout order.
typedef enum {
  kUpb_FieldRep_1Byte = 0,
  kUpb_FieldRep_4Byte = 1,
  kUpb_FieldRep_StringView = 2,
  kUpb_FieldRep_8Byte = 3,

  kUpb_FieldRep_NativePointer =
      WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte),
  kUpb_FieldRep_Max = kUpb_FieldRep_8Byte,
} wpi_upb_FieldRep;

#define kUpb_FieldRep_Shift 6

#ifdef __cplusplus
extern "C" {
#endif

WPI_UPB_INLINE wpi_upb_FieldMode
WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_Mode)(const struct wpi_upb_MiniTableField* f) {
  return (wpi_upb_FieldMode)(f->WPI_UPB_ONLYBITS(mode) & kUpb_FieldMode_Mask);
}

WPI_UPB_INLINE wpi_upb_FieldRep
WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_GetRep)(const struct wpi_upb_MiniTableField* f) {
  return (wpi_upb_FieldRep)(f->WPI_UPB_ONLYBITS(mode) >> kUpb_FieldRep_Shift);
}

WPI_UPB_API_INLINE bool wpi_upb_MiniTableField_IsArray(
    const struct wpi_upb_MiniTableField* f) {
  return WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_Mode)(f) == kUpb_FieldMode_Array;
}

WPI_UPB_API_INLINE bool wpi_upb_MiniTableField_IsMap(
    const struct wpi_upb_MiniTableField* f) {
  return WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_Mode)(f) == kUpb_FieldMode_Map;
}

WPI_UPB_API_INLINE bool wpi_upb_MiniTableField_IsScalar(
    const struct wpi_upb_MiniTableField* f) {
  return WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_Mode)(f) == kUpb_FieldMode_Scalar;
}

WPI_UPB_INLINE bool WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_IsAlternate)(
    const struct wpi_upb_MiniTableField* f) {
  return (f->WPI_UPB_ONLYBITS(mode) & kUpb_LabelFlags_IsAlternate) != 0;
}

WPI_UPB_API_INLINE bool wpi_upb_MiniTableField_IsExtension(
    const struct wpi_upb_MiniTableField* f) {
  return (f->WPI_UPB_ONLYBITS(mode) & kUpb_LabelFlags_IsExtension) != 0;
}

WPI_UPB_API_INLINE bool wpi_upb_MiniTableField_IsPacked(
    const struct wpi_upb_MiniTableField* f) {
  return (f->WPI_UPB_ONLYBITS(mode) & kUpb_LabelFlags_IsPacked) != 0;
}

WPI_UPB_API_INLINE wpi_upb_FieldType
wpi_upb_MiniTableField_Type(const struct wpi_upb_MiniTableField* f) {
  const wpi_upb_FieldType type = (wpi_upb_FieldType)f->WPI_UPB_PRIVATE(descriptortype);
  if (WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_IsAlternate)(f)) {
    if (type == kUpb_FieldType_Int32) return kUpb_FieldType_Enum;
    if (type == kUpb_FieldType_Bytes) return kUpb_FieldType_String;
    WPI_UPB_ASSERT(false);
  }
  return type;
}

WPI_UPB_API_INLINE
wpi_upb_CType wpi_upb_MiniTableField_CType(const struct wpi_upb_MiniTableField* f) {
  return wpi_upb_FieldType_CType(wpi_upb_MiniTableField_Type(f));
}

WPI_UPB_INLINE bool WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_HasHasbit)(
    const struct wpi_upb_MiniTableField* f) {
  return f->presence > 0;
}

WPI_UPB_INLINE char WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_HasbitMask)(
    const struct wpi_upb_MiniTableField* f) {
  WPI_UPB_ASSERT(WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_HasHasbit)(f));
  const size_t index = f->presence;
  return 1 << (index % 8);
}

WPI_UPB_INLINE size_t WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_HasbitOffset)(
    const struct wpi_upb_MiniTableField* f) {
  WPI_UPB_ASSERT(WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_HasHasbit)(f));
  const size_t index = f->presence;
  return index / 8;
}

WPI_UPB_API_INLINE bool wpi_upb_MiniTableField_IsClosedEnum(
    const struct wpi_upb_MiniTableField* f) {
  return f->WPI_UPB_PRIVATE(descriptortype) == kUpb_FieldType_Enum;
}

WPI_UPB_API_INLINE bool wpi_upb_MiniTableField_IsInOneof(
    const struct wpi_upb_MiniTableField* f) {
  return f->presence < 0;
}

WPI_UPB_API_INLINE bool wpi_upb_MiniTableField_IsSubMessage(
    const struct wpi_upb_MiniTableField* f) {
  return f->WPI_UPB_PRIVATE(descriptortype) == kUpb_FieldType_Message ||
         f->WPI_UPB_PRIVATE(descriptortype) == kUpb_FieldType_Group;
}

WPI_UPB_API_INLINE bool wpi_upb_MiniTableField_HasPresence(
    const struct wpi_upb_MiniTableField* f) {
  if (wpi_upb_MiniTableField_IsExtension(f)) {
    return wpi_upb_MiniTableField_IsScalar(f);
  } else {
    return f->presence != 0;
  }
}

WPI_UPB_API_INLINE uint32_t
wpi_upb_MiniTableField_Number(const struct wpi_upb_MiniTableField* f) {
  return f->WPI_UPB_ONLYBITS(number);
}

WPI_UPB_INLINE uint16_t
WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_Offset)(const struct wpi_upb_MiniTableField* f) {
  return f->WPI_UPB_ONLYBITS(offset);
}

WPI_UPB_INLINE size_t WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_OneofOffset)(
    const struct wpi_upb_MiniTableField* f) {
  WPI_UPB_ASSERT(wpi_upb_MiniTableField_IsInOneof(f));
  return ~(ptrdiff_t)f->presence;
}

WPI_UPB_INLINE void WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_CheckIsArray)(
    const struct wpi_upb_MiniTableField* f) {
  WPI_UPB_ASSUME(WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_GetRep)(f) ==
             kUpb_FieldRep_NativePointer);
  WPI_UPB_ASSUME(wpi_upb_MiniTableField_IsArray(f));
  WPI_UPB_ASSUME(f->presence == 0);
}

WPI_UPB_INLINE void WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_CheckIsMap)(
    const struct wpi_upb_MiniTableField* f) {
  WPI_UPB_ASSUME(WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_GetRep)(f) ==
             kUpb_FieldRep_NativePointer);
  WPI_UPB_ASSUME(wpi_upb_MiniTableField_IsMap(f));
  WPI_UPB_ASSUME(f->presence == 0);
}

WPI_UPB_INLINE size_t WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_ElemSizeLg2)(
    const struct wpi_upb_MiniTableField* f) {
  const wpi_upb_FieldType field_type = wpi_upb_MiniTableField_Type(f);
  return WPI_UPB_PRIVATE(_wpi_upb_FieldType_SizeLg2)(field_type);
}

// LINT.ThenChange(//depot/google3/third_party/upb/bits/typescript/mini_table_field.ts)

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_MINI_TABLE_INTERNAL_FIELD_H_ */

// Must be last.

typedef struct wpi_upb_MiniTableField wpi_upb_MiniTableField;

#ifdef __cplusplus
extern "C" {
#endif

WPI_UPB_API_INLINE wpi_upb_CType wpi_upb_MiniTableField_CType(const wpi_upb_MiniTableField* f);

WPI_UPB_API_INLINE bool wpi_upb_MiniTableField_HasPresence(const wpi_upb_MiniTableField* f);

WPI_UPB_API_INLINE bool wpi_upb_MiniTableField_IsArray(const wpi_upb_MiniTableField* f);

WPI_UPB_API_INLINE bool wpi_upb_MiniTableField_IsClosedEnum(
    const wpi_upb_MiniTableField* f);

WPI_UPB_API_INLINE bool wpi_upb_MiniTableField_IsExtension(const wpi_upb_MiniTableField* f);

WPI_UPB_API_INLINE bool wpi_upb_MiniTableField_IsInOneof(const wpi_upb_MiniTableField* f);

WPI_UPB_API_INLINE bool wpi_upb_MiniTableField_IsMap(const wpi_upb_MiniTableField* f);

WPI_UPB_API_INLINE bool wpi_upb_MiniTableField_IsPacked(const wpi_upb_MiniTableField* f);

WPI_UPB_API_INLINE bool wpi_upb_MiniTableField_IsScalar(const wpi_upb_MiniTableField* f);

WPI_UPB_API_INLINE bool wpi_upb_MiniTableField_IsSubMessage(
    const wpi_upb_MiniTableField* f);

WPI_UPB_API_INLINE uint32_t wpi_upb_MiniTableField_Number(const wpi_upb_MiniTableField* f);

WPI_UPB_API_INLINE wpi_upb_FieldType
wpi_upb_MiniTableField_Type(const wpi_upb_MiniTableField* f);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_MINI_TABLE_FIELD_H_ */

#ifndef WPI_UPB_MINI_TABLE_MESSAGE_H_
#define WPI_UPB_MINI_TABLE_MESSAGE_H_


#ifndef WPI_UPB_MINI_TABLE_ENUM_H_
#define WPI_UPB_MINI_TABLE_ENUM_H_

#include <stdint.h>


#ifndef WPI_UPB_MINI_TABLE_INTERNAL_ENUM_H_
#define WPI_UPB_MINI_TABLE_INTERNAL_ENUM_H_

#include <stdint.h>

// Must be last.

#ifdef __GCC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wc99-extensions"
#endif
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc99-extensions"
#endif

struct wpi_upb_MiniTableEnum {
  uint32_t WPI_UPB_PRIVATE(mask_limit);   // Highest that can be tested with mask.
  uint32_t WPI_UPB_PRIVATE(value_count);  // Number of values after the bitfield.
  uint32_t WPI_UPB_PRIVATE(data)[];       // Bitmask + enumerated values follow.
};

#ifdef __GCC__
#pragma GCC diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic pop
#endif

#ifdef __cplusplus
extern "C" {
#endif

WPI_UPB_API_INLINE bool wpi_upb_MiniTableEnum_CheckValue(
    const struct wpi_upb_MiniTableEnum* e, uint32_t val) {
  if (WPI_UPB_LIKELY(val < 64)) {
    const uint64_t mask =
        e->WPI_UPB_PRIVATE(data)[0] | ((uint64_t)e->WPI_UPB_PRIVATE(data)[1] << 32);
    const uint64_t bit = 1ULL << val;
    return (mask & bit) != 0;
  }
  if (WPI_UPB_LIKELY(val < e->WPI_UPB_PRIVATE(mask_limit))) {
    const uint32_t mask = e->WPI_UPB_PRIVATE(data)[val / 32];
    const uint32_t bit = 1ULL << (val % 32);
    return (mask & bit) != 0;
  }

  // OPT: binary search long lists?
  const uint32_t* start =
      &e->WPI_UPB_PRIVATE(data)[e->WPI_UPB_PRIVATE(mask_limit) / 32];
  const uint32_t* limit = &e->WPI_UPB_PRIVATE(
      data)[e->WPI_UPB_PRIVATE(mask_limit) / 32 + e->WPI_UPB_PRIVATE(value_count)];
  for (const uint32_t* p = start; p < limit; p++) {
    if (*p == val) return true;
  }
  return false;
}

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_MINI_TABLE_INTERNAL_ENUM_H_ */

// Must be last

typedef struct wpi_upb_MiniTableEnum wpi_upb_MiniTableEnum;

#ifdef __cplusplus
extern "C" {
#endif

// Validates enum value against range defined by enum mini table.
WPI_UPB_API_INLINE bool wpi_upb_MiniTableEnum_CheckValue(const wpi_upb_MiniTableEnum* e,
                                                 uint32_t val);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_MINI_TABLE_ENUM_H_ */

#ifndef WPI_UPB_MINI_TABLE_INTERNAL_MESSAGE_H_
#define WPI_UPB_MINI_TABLE_INTERNAL_MESSAGE_H_

#include <stddef.h>
#include <stdint.h>


#ifndef WPI_UPB_MINI_TABLE_INTERNAL_SUB_H_
#define WPI_UPB_MINI_TABLE_INTERNAL_SUB_H_

// Must be last.

typedef union {
  const struct wpi_upb_MiniTable* const* WPI_UPB_PRIVATE(submsg);
  const struct wpi_upb_MiniTableEnum* WPI_UPB_PRIVATE(subenum);
} wpi_upb_MiniTableSubInternal;

union wpi_upb_MiniTableSub {
  const struct wpi_upb_MiniTable* WPI_UPB_PRIVATE(submsg);
  const struct wpi_upb_MiniTableEnum* WPI_UPB_PRIVATE(subenum);
};

#ifdef __cplusplus
extern "C" {
#endif

WPI_UPB_API_INLINE union wpi_upb_MiniTableSub wpi_upb_MiniTableSub_FromEnum(
    const struct wpi_upb_MiniTableEnum* subenum) {
  union wpi_upb_MiniTableSub out;
  out.WPI_UPB_PRIVATE(subenum) = subenum;
  return out;
}

WPI_UPB_API_INLINE union wpi_upb_MiniTableSub wpi_upb_MiniTableSub_FromMessage(
    const struct wpi_upb_MiniTable* submsg) {
  union wpi_upb_MiniTableSub out;
  out.WPI_UPB_PRIVATE(submsg) = submsg;
  return out;
}

WPI_UPB_API_INLINE const struct wpi_upb_MiniTableEnum* wpi_upb_MiniTableSub_Enum(
    const union wpi_upb_MiniTableSub sub) {
  return sub.WPI_UPB_PRIVATE(subenum);
}

WPI_UPB_API_INLINE const struct wpi_upb_MiniTable* wpi_upb_MiniTableSub_Message(
    const union wpi_upb_MiniTableSub sub) {
  return sub.WPI_UPB_PRIVATE(submsg);
}

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_MINI_TABLE_INTERNAL_SUB_H_ */

// Must be last.

struct wpi_upb_Decoder;
struct wpi_upb_Message;
typedef const char* _wpi_upb_FieldParser(struct wpi_upb_Decoder* d, const char* ptr,
                                     struct wpi_upb_Message* msg, intptr_t table,
                                     uint64_t hasbits, uint64_t data);
typedef struct {
  uint64_t field_data;
  _wpi_upb_FieldParser* field_parser;
} _wpi_upb_FastTable_Entry;

typedef enum {
  kUpb_ExtMode_NonExtendable = 0,  // Non-extendable message.
  kUpb_ExtMode_Extendable = 1,     // Normal extendable message.
  kUpb_ExtMode_IsMessageSet = 2,   // MessageSet message.
  kUpb_ExtMode_IsMessageSet_ITEM =
      3,  // MessageSet item (temporary only, see decode.c)

  // During table building we steal a bit to indicate that the message is a map
  // entry.  *Only* used during table building!
  kUpb_ExtMode_IsMapEntry = 4,
} wpi_upb_ExtMode;

// wpi_upb_MiniTable represents the memory layout of a given wpi_upb_MessageDef.
// The members are public so generated code can initialize them,
// but users MUST NOT directly read or write any of its members.

// LINT.IfChange(minitable_struct_definition)
struct wpi_upb_MiniTable {
  const wpi_upb_MiniTableSubInternal* WPI_UPB_PRIVATE(subs);
  const struct wpi_upb_MiniTableField* WPI_UPB_ONLYBITS(fields);

  // Must be aligned to sizeof(void*). Doesn't include internal members like
  // unknown fields, extension dict, pointer to msglayout, etc.
  uint16_t WPI_UPB_PRIVATE(size);

  uint16_t WPI_UPB_ONLYBITS(field_count);

  uint8_t WPI_UPB_PRIVATE(ext);  // wpi_upb_ExtMode, uint8_t here so sizeof(ext) == 1
  uint8_t WPI_UPB_PRIVATE(dense_below);
  uint8_t WPI_UPB_PRIVATE(table_mask);
  uint8_t WPI_UPB_PRIVATE(required_count);  // Required fields have the low hasbits.

#ifdef WPI_UPB_TRACING_ENABLED
  const char* WPI_UPB_PRIVATE(full_name);
#endif

#ifdef WPI_UPB_FASTTABLE_ENABLED
  // To statically initialize the tables of variable length, we need a flexible
  // array member, and we need to compile in gnu99 mode (constant initialization
  // of flexible array members is a GNU extension, not in C99 unfortunately.
  _wpi_upb_FastTable_Entry WPI_UPB_PRIVATE(fasttable)[];
#endif
};
// LINT.ThenChange(//depot/google3/third_party/upb/bits/typescript/mini_table.ts)

#ifdef __cplusplus
extern "C" {
#endif

WPI_UPB_INLINE const struct wpi_upb_MiniTable* WPI_UPB_PRIVATE(
    _wpi_upb_MiniTable_StrongReference)(const struct wpi_upb_MiniTable* mt) {
#if defined(__GNUC__)
  __asm__("" : : "r"(mt));
#else
  const struct wpi_upb_MiniTable* volatile unused = mt;
  (void)&unused;  // Use address to avoid an extra load of "unused".
#endif
  return mt;
}

WPI_UPB_INLINE const struct wpi_upb_MiniTable* WPI_UPB_PRIVATE(_wpi_upb_MiniTable_Empty)(void) {
  extern const struct wpi_upb_MiniTable WPI_UPB_PRIVATE(_kUpb_MiniTable_Empty);

  return &WPI_UPB_PRIVATE(_kUpb_MiniTable_Empty);
}

WPI_UPB_API_INLINE int wpi_upb_MiniTable_FieldCount(const struct wpi_upb_MiniTable* m) {
  return m->WPI_UPB_ONLYBITS(field_count);
}

WPI_UPB_INLINE bool WPI_UPB_PRIVATE(_wpi_upb_MiniTable_IsEmpty)(
    const struct wpi_upb_MiniTable* m) {
  extern const struct wpi_upb_MiniTable WPI_UPB_PRIVATE(_kUpb_MiniTable_Empty);

  return m == &WPI_UPB_PRIVATE(_kUpb_MiniTable_Empty);
}

WPI_UPB_API_INLINE const struct wpi_upb_MiniTableField* wpi_upb_MiniTable_GetFieldByIndex(
    const struct wpi_upb_MiniTable* m, uint32_t i) {
  return &m->WPI_UPB_ONLYBITS(fields)[i];
}

WPI_UPB_INLINE const struct wpi_upb_MiniTable* WPI_UPB_PRIVATE(
    _wpi_upb_MiniTable_GetSubTableByIndex)(const struct wpi_upb_MiniTable* m,
                                       uint32_t i) {
  return *m->WPI_UPB_PRIVATE(subs)[i].WPI_UPB_PRIVATE(submsg);
}

WPI_UPB_API_INLINE const struct wpi_upb_MiniTable* wpi_upb_MiniTable_SubMessage(
    const struct wpi_upb_MiniTable* m, const struct wpi_upb_MiniTableField* f) {
  if (wpi_upb_MiniTableField_CType(f) != kUpb_CType_Message) {
    return NULL;
  }
  return WPI_UPB_PRIVATE(_wpi_upb_MiniTable_GetSubTableByIndex)(
      m, f->WPI_UPB_PRIVATE(submsg_index));
}

WPI_UPB_API_INLINE const struct wpi_upb_MiniTable* wpi_upb_MiniTable_GetSubMessageTable(
    const struct wpi_upb_MiniTable* m, const struct wpi_upb_MiniTableField* f) {
  WPI_UPB_ASSUME(wpi_upb_MiniTableField_CType(f) == kUpb_CType_Message);
  const struct wpi_upb_MiniTable* ret = wpi_upb_MiniTable_SubMessage(m, f);
  WPI_UPB_ASSUME(ret);
  return WPI_UPB_PRIVATE(_wpi_upb_MiniTable_IsEmpty)(ret) ? NULL : ret;
}

WPI_UPB_API_INLINE bool wpi_upb_MiniTable_FieldIsLinked(
    const struct wpi_upb_MiniTable* m, const struct wpi_upb_MiniTableField* f) {
  return wpi_upb_MiniTable_GetSubMessageTable(m, f) != NULL;
}

WPI_UPB_API_INLINE const struct wpi_upb_MiniTable* wpi_upb_MiniTable_MapEntrySubMessage(
    const struct wpi_upb_MiniTable* m, const struct wpi_upb_MiniTableField* f) {
  WPI_UPB_ASSERT(wpi_upb_MiniTable_FieldIsLinked(m, f));  // Map entries must be linked.
  WPI_UPB_ASSERT(wpi_upb_MiniTableField_IsMap(f));        // Function precondition.
  return wpi_upb_MiniTable_SubMessage(m, f);
}

WPI_UPB_API_INLINE const struct wpi_upb_MiniTableEnum* wpi_upb_MiniTable_GetSubEnumTable(
    const struct wpi_upb_MiniTable* m, const struct wpi_upb_MiniTableField* f) {
  WPI_UPB_ASSERT(wpi_upb_MiniTableField_CType(f) == kUpb_CType_Enum);
  return m->WPI_UPB_PRIVATE(subs)[f->WPI_UPB_PRIVATE(submsg_index)].WPI_UPB_PRIVATE(
      subenum);
}

WPI_UPB_API_INLINE const struct wpi_upb_MiniTableField* wpi_upb_MiniTable_MapKey(
    const struct wpi_upb_MiniTable* m) {
  WPI_UPB_ASSERT(wpi_upb_MiniTable_FieldCount(m) == 2);
  const struct wpi_upb_MiniTableField* f = wpi_upb_MiniTable_GetFieldByIndex(m, 0);
  WPI_UPB_ASSERT(wpi_upb_MiniTableField_Number(f) == 1);
  return f;
}

WPI_UPB_API_INLINE const struct wpi_upb_MiniTableField* wpi_upb_MiniTable_MapValue(
    const struct wpi_upb_MiniTable* m) {
  WPI_UPB_ASSERT(wpi_upb_MiniTable_FieldCount(m) == 2);
  const struct wpi_upb_MiniTableField* f = wpi_upb_MiniTable_GetFieldByIndex(m, 1);
  WPI_UPB_ASSERT(wpi_upb_MiniTableField_Number(f) == 2);
  return f;
}

// Computes a bitmask in which the |m->required_count| lowest bits are set.
//
// Sample output:
//    RequiredMask(1) => 0b1 (0x1)
//    RequiredMask(5) => 0b11111 (0x1f)
WPI_UPB_INLINE uint64_t
WPI_UPB_PRIVATE(_wpi_upb_MiniTable_RequiredMask)(const struct wpi_upb_MiniTable* m) {
  int n = m->WPI_UPB_PRIVATE(required_count);
  WPI_UPB_ASSERT(0 < n && n <= 64);
  return (1ULL << n) - 1;
}

#ifdef WPI_UPB_TRACING_ENABLED
WPI_UPB_INLINE const char* wpi_upb_MiniTable_FullName(
    const struct wpi_upb_MiniTable* mini_table) {
  return mini_table->WPI_UPB_PRIVATE(full_name);
}
// Initializes tracing proto name from language runtimes that construct
// mini tables dynamically at runtime. The runtime is responsible for passing
// controlling lifetime of name such as storing in same arena as mini_table.
WPI_UPB_INLINE void wpi_upb_MiniTable_SetFullName(struct wpi_upb_MiniTable* mini_table,
                                          const char* full_name) {
  mini_table->WPI_UPB_PRIVATE(full_name) = full_name;
}
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_MINI_TABLE_INTERNAL_MESSAGE_H_ */

// Must be last.

typedef struct wpi_upb_MiniTable wpi_upb_MiniTable;

#ifdef __cplusplus
extern "C" {
#endif

WPI_UPB_API const wpi_upb_MiniTableField* wpi_upb_MiniTable_FindFieldByNumber(
    const wpi_upb_MiniTable* m, uint32_t number);

WPI_UPB_API_INLINE const wpi_upb_MiniTableField* wpi_upb_MiniTable_GetFieldByIndex(
    const wpi_upb_MiniTable* m, uint32_t index);

WPI_UPB_API_INLINE int wpi_upb_MiniTable_FieldCount(const wpi_upb_MiniTable* m);

// DEPRECATED: use wpi_upb_MiniTable_SubMessage() instead
// Returns the MiniTable for a message field, NULL if the field is unlinked.
WPI_UPB_API_INLINE const wpi_upb_MiniTable* wpi_upb_MiniTable_GetSubMessageTable(
    const wpi_upb_MiniTable* m, const wpi_upb_MiniTableField* f);

// Returns the MiniTable for a message field if it is a submessage, otherwise
// returns NULL.
//
// WARNING: if dynamic tree shaking is in use, the return value may be the
// "empty", zero-field placeholder message instead of the real message type.
// If the message is later linked, this function will begin returning the real
// message type.
WPI_UPB_API_INLINE const wpi_upb_MiniTable* wpi_upb_MiniTable_SubMessage(
    const wpi_upb_MiniTable* m, const wpi_upb_MiniTableField* f);

// Returns the MiniTable for a map field.  The given field must refer to a map.
WPI_UPB_API_INLINE const wpi_upb_MiniTable* wpi_upb_MiniTable_MapEntrySubMessage(
    const wpi_upb_MiniTable* m, const wpi_upb_MiniTableField* f);

// Returns the MiniTableEnum for a message field, NULL if the field is unlinked.
WPI_UPB_API_INLINE const wpi_upb_MiniTableEnum* wpi_upb_MiniTable_GetSubEnumTable(
    const wpi_upb_MiniTable* m, const wpi_upb_MiniTableField* f);

// Returns the MiniTableField for the key of a map.
WPI_UPB_API_INLINE const wpi_upb_MiniTableField* wpi_upb_MiniTable_MapKey(
    const wpi_upb_MiniTable* m);

// Returns the MiniTableField for the value of a map.
WPI_UPB_API_INLINE const wpi_upb_MiniTableField* wpi_upb_MiniTable_MapValue(
    const wpi_upb_MiniTable* m);

// Returns true if this MiniTable field is linked to a MiniTable for the
// sub-message.
WPI_UPB_API_INLINE bool wpi_upb_MiniTable_FieldIsLinked(const wpi_upb_MiniTable* m,
                                                const wpi_upb_MiniTableField* f);

// If this field is in a oneof, returns the first field in the oneof.
//
// Otherwise returns NULL.
//
// Usage:
//   const wpi_upb_MiniTableField* field = wpi_upb_MiniTable_GetOneof(m, f);
//   do {
//       ..
//   } while (wpi_upb_MiniTable_NextOneofField(m, &field);
//
const wpi_upb_MiniTableField* wpi_upb_MiniTable_GetOneof(const wpi_upb_MiniTable* m,
                                                 const wpi_upb_MiniTableField* f);

// Iterates to the next field in the oneof. If this is the last field in the
// oneof, returns false. The ordering of fields in the oneof is not
// guaranteed.
// REQUIRES: |f| is the field initialized by wpi_upb_MiniTable_GetOneof and updated
//           by prior wpi_upb_MiniTable_NextOneofField calls.
bool wpi_upb_MiniTable_NextOneofField(const wpi_upb_MiniTable* m,
                                  const wpi_upb_MiniTableField** f);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_MINI_TABLE_MESSAGE_H_ */

// Must be last.

typedef struct wpi_upb_Array wpi_upb_Array;

#ifdef __cplusplus
extern "C" {
#endif

// Creates a new array on the given arena that holds elements of this type.
WPI_UPB_API wpi_upb_Array* wpi_upb_Array_New(wpi_upb_Arena* a, wpi_upb_CType type);

// Returns the number of elements in the array.
WPI_UPB_API_INLINE size_t wpi_upb_Array_Size(const wpi_upb_Array* arr);

// Returns the given element, which must be within the array's current size.
WPI_UPB_API wpi_upb_MessageValue wpi_upb_Array_Get(const wpi_upb_Array* arr, size_t i);

// Returns a mutating pointer to the given element, which must be within the
// array's current size.
WPI_UPB_API wpi_upb_MutableMessageValue wpi_upb_Array_GetMutable(wpi_upb_Array* arr, size_t i);

// Sets the given element, which must be within the array's current size.
WPI_UPB_API void wpi_upb_Array_Set(wpi_upb_Array* arr, size_t i, wpi_upb_MessageValue val);

// Appends an element to the array. Returns false on allocation failure.
WPI_UPB_API bool wpi_upb_Array_Append(wpi_upb_Array* array, wpi_upb_MessageValue val,
                              wpi_upb_Arena* arena);

// Moves elements within the array using memmove().
// Like memmove(), the source and destination elements may be overlapping.
WPI_UPB_API void wpi_upb_Array_Move(wpi_upb_Array* array, size_t dst_idx, size_t src_idx,
                            size_t count);

// Inserts one or more empty elements into the array.
// Existing elements are shifted right.
// The new elements have undefined state and must be set with `wpi_upb_Array_Set()`.
// REQUIRES: `i <= wpi_upb_Array_Size(arr)`
WPI_UPB_API bool wpi_upb_Array_Insert(wpi_upb_Array* array, size_t i, size_t count,
                              wpi_upb_Arena* arena);

// Deletes one or more elements from the array.
// Existing elements are shifted left.
// REQUIRES: `i + count <= wpi_upb_Array_Size(arr)`
WPI_UPB_API void wpi_upb_Array_Delete(wpi_upb_Array* array, size_t i, size_t count);

// Reserves |size| elements of storage for the array.
WPI_UPB_API_INLINE bool wpi_upb_Array_Reserve(struct wpi_upb_Array* array, size_t size,
                                      wpi_upb_Arena* arena);

// Changes the size of a vector. New elements are initialized to NULL/0.
// Returns false on allocation failure.
WPI_UPB_API bool wpi_upb_Array_Resize(wpi_upb_Array* array, size_t size, wpi_upb_Arena* arena);

// Returns pointer to array data.
WPI_UPB_API_INLINE const void* wpi_upb_Array_DataPtr(const wpi_upb_Array* arr);

// Returns mutable pointer to array data.
WPI_UPB_API_INLINE void* wpi_upb_Array_MutableDataPtr(wpi_upb_Array* arr);

// Mark an array and all of its descendents as frozen/immutable.
// If the array elements are messages then |m| must point to the minitable for
// those messages. Otherwise |m| must be NULL.
WPI_UPB_API void wpi_upb_Array_Freeze(wpi_upb_Array* arr, const wpi_upb_MiniTable* m);

// Returns whether an array has been frozen.
WPI_UPB_API_INLINE bool wpi_upb_Array_IsFrozen(const wpi_upb_Array* arr);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_MESSAGE_ARRAY_H_ */

#ifndef WPI_UPB_MESSAGE_INTERNAL_ACCESSORS_H_
#define WPI_UPB_MESSAGE_INTERNAL_ACCESSORS_H_

#include <stddef.h>
#include <stdint.h>
#include <string.h>


#ifndef WPI_UPB_BASE_INTERNAL_ENDIAN_H_
#define WPI_UPB_BASE_INTERNAL_ENDIAN_H_

#include <stdint.h>

// Must be last.

#ifdef __cplusplus
extern "C" {
#endif

WPI_UPB_INLINE bool wpi_upb_IsLittleEndian(void) {
  const int x = 1;
  return *(char*)&x == 1;
}

WPI_UPB_INLINE uint32_t wpi_upb_BigEndian32(uint32_t val) {
  if (wpi_upb_IsLittleEndian()) return val;

  return ((val & 0xff) << 24) | ((val & 0xff00) << 8) |
         ((val & 0xff0000) >> 8) | ((val & 0xff000000) >> 24);
}

WPI_UPB_INLINE uint64_t wpi_upb_BigEndian64(uint64_t val) {
  if (wpi_upb_IsLittleEndian()) return val;

  const uint64_t hi = ((uint64_t)wpi_upb_BigEndian32((uint32_t)val)) << 32;
  const uint64_t lo = wpi_upb_BigEndian32((uint32_t)(val >> 32));
  return hi | lo;
}

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_BASE_INTERNAL_ENDIAN_H_ */

#ifndef WPI_UPB_MESSAGE_INTERNAL_EXTENSION_H_
#define WPI_UPB_MESSAGE_INTERNAL_EXTENSION_H_

#include <stddef.h>


#ifndef WPI_UPB_MINI_TABLE_EXTENSION_H_
#define WPI_UPB_MINI_TABLE_EXTENSION_H_

#include <stdint.h>


#ifndef WPI_UPB_MINI_TABLE_INTERNAL_EXTENSION_H_
#define WPI_UPB_MINI_TABLE_INTERNAL_EXTENSION_H_

#include <stddef.h>
#include <stdint.h>


// Must be last.

struct wpi_upb_MiniTableExtension {
  // Do not move this field. We need to be able to alias pointers.
  struct wpi_upb_MiniTableField WPI_UPB_PRIVATE(field);

  const struct wpi_upb_MiniTable* WPI_UPB_PRIVATE(extendee);
  union wpi_upb_MiniTableSub WPI_UPB_PRIVATE(sub);  // NULL unless submsg or proto2 enum
};

#ifdef __cplusplus
extern "C" {
#endif

WPI_UPB_API_INLINE wpi_upb_CType
wpi_upb_MiniTableExtension_CType(const struct wpi_upb_MiniTableExtension* e) {
  return wpi_upb_MiniTableField_CType(&e->WPI_UPB_PRIVATE(field));
}

WPI_UPB_API_INLINE uint32_t
wpi_upb_MiniTableExtension_Number(const struct wpi_upb_MiniTableExtension* e) {
  return e->WPI_UPB_PRIVATE(field).WPI_UPB_ONLYBITS(number);
}

WPI_UPB_API_INLINE const struct wpi_upb_MiniTable* wpi_upb_MiniTableExtension_GetSubMessage(
    const struct wpi_upb_MiniTableExtension* e) {
  if (wpi_upb_MiniTableExtension_CType(e) != kUpb_CType_Message) {
    return NULL;
  }
  return wpi_upb_MiniTableSub_Message(e->WPI_UPB_PRIVATE(sub));
}

WPI_UPB_API_INLINE void wpi_upb_MiniTableExtension_SetSubMessage(
    struct wpi_upb_MiniTableExtension* e, const struct wpi_upb_MiniTable* m) {
  e->WPI_UPB_PRIVATE(sub).WPI_UPB_PRIVATE(submsg) = m;
}

WPI_UPB_INLINE wpi_upb_FieldRep WPI_UPB_PRIVATE(_wpi_upb_MiniTableExtension_GetRep)(
    const struct wpi_upb_MiniTableExtension* e) {
  return WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_GetRep)(&e->WPI_UPB_PRIVATE(field));
}

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_MINI_TABLE_INTERNAL_EXTENSION_H_ */

// Must be last.

typedef struct wpi_upb_MiniTableExtension wpi_upb_MiniTableExtension;

#ifdef __cplusplus
extern "C" {
#endif

WPI_UPB_API_INLINE wpi_upb_CType
wpi_upb_MiniTableExtension_CType(const wpi_upb_MiniTableExtension* e);

WPI_UPB_API_INLINE uint32_t
wpi_upb_MiniTableExtension_Number(const wpi_upb_MiniTableExtension* e);

WPI_UPB_API_INLINE const wpi_upb_MiniTable* wpi_upb_MiniTableExtension_GetSubMessage(
    const wpi_upb_MiniTableExtension* e);

WPI_UPB_API_INLINE void wpi_upb_MiniTableExtension_SetSubMessage(
    wpi_upb_MiniTableExtension* e, const wpi_upb_MiniTable* m);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_MINI_TABLE_EXTENSION_H_ */

// Must be last.

// The internal representation of an extension is self-describing: it contains
// enough information that we can serialize it to binary format without needing
// to look it up in a wpi_upb_ExtensionRegistry.
//
// This representation allocates 16 bytes to data on 64-bit platforms.
// This is rather wasteful for scalars (in the extreme case of bool,
// it wastes 15 bytes). We accept this because we expect messages to be
// the most common extension type.
typedef struct {
  const wpi_upb_MiniTableExtension* ext;
  wpi_upb_MessageValue data;
} wpi_upb_Extension;

#ifdef __cplusplus
extern "C" {
#endif

// Adds the given extension data to the given message.
// |ext| is copied into the message instance.
// This logically replaces any previously-added extension with this number.
wpi_upb_Extension* WPI_UPB_PRIVATE(_wpi_upb_Message_GetOrCreateExtension)(
    struct wpi_upb_Message* msg, const wpi_upb_MiniTableExtension* ext,
    wpi_upb_Arena* arena);

// Returns an array of extensions for this message.
// Note: the array is ordered in reverse relative to the order of creation.
const wpi_upb_Extension* WPI_UPB_PRIVATE(_wpi_upb_Message_Getexts)(
    const struct wpi_upb_Message* msg, size_t* count);

// Returns an extension for a message with a given mini table,
// or NULL if no extension exists with this mini table.
const wpi_upb_Extension* WPI_UPB_PRIVATE(_wpi_upb_Message_Getext)(
    const struct wpi_upb_Message* msg, const wpi_upb_MiniTableExtension* ext);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_MESSAGE_INTERNAL_EXTENSION_H_ */

#ifndef WPI_UPB_MESSAGE_INTERNAL_MAP_H_
#define WPI_UPB_MESSAGE_INTERNAL_MAP_H_

#include <stddef.h>
#include <string.h>


#ifndef WPI_UPB_HASH_STR_TABLE_H_
#define WPI_UPB_HASH_STR_TABLE_H_


/*
 * wpi_upb_table
 *
 * This header is INTERNAL-ONLY!  Its interfaces are not public or stable!
 * This file defines very fast int->wpi_upb_value (inttable) and string->wpi_upb_value
 * (strtable) hash tables.
 *
 * The table uses chained scatter with Brent's variation (inspired by the Lua
 * implementation of hash tables).  The hash function for strings is Austin
 * Appleby's "MurmurHash."
 *
 * The inttable uses uintptr_t as its key, which guarantees it can be used to
 * store pointers or integers of at least 32 bits (upb isn't really useful on
 * systems where sizeof(void*) < 4).
 *
 * The table must be homogeneous (all values of the same type).  In debug
 * mode, we check this on insert and lookup.
 */

#ifndef WPI_UPB_HASH_COMMON_H_
#define WPI_UPB_HASH_COMMON_H_

#include <string.h>


// Must be last.

#ifdef __cplusplus
extern "C" {
#endif

/* wpi_upb_value ******************************************************************/

typedef struct {
  uint64_t val;
} wpi_upb_value;

WPI_UPB_INLINE void _wpi_upb_value_setval(wpi_upb_value* v, uint64_t val) { v->val = val; }

/* For each value ctype, define the following set of functions:
 *
 * // Get/set an int32 from a wpi_upb_value.
 * int32_t wpi_upb_value_getint32(wpi_upb_value val);
 * void wpi_upb_value_setint32(wpi_upb_value *val, int32_t cval);
 *
 * // Construct a new wpi_upb_value from an int32.
 * wpi_upb_value wpi_upb_value_int32(int32_t val); */
#define FUNCS(name, membername, type_t, converter)                   \
  WPI_UPB_INLINE void wpi_upb_value_set##name(wpi_upb_value* val, type_t cval) { \
    val->val = (converter)cval;                                      \
  }                                                                  \
  WPI_UPB_INLINE wpi_upb_value wpi_upb_value_##name(type_t val) {                \
    wpi_upb_value ret;                                                   \
    wpi_upb_value_set##name(&ret, val);                                  \
    return ret;                                                      \
  }                                                                  \
  WPI_UPB_INLINE type_t wpi_upb_value_get##name(wpi_upb_value val) {             \
    return (type_t)(converter)val.val;                               \
  }

FUNCS(int32, int32, int32_t, int32_t)
FUNCS(int64, int64, int64_t, int64_t)
FUNCS(uint32, uint32, uint32_t, uint32_t)
FUNCS(uint64, uint64, uint64_t, uint64_t)
FUNCS(bool, _bool, bool, bool)
FUNCS(cstr, cstr, char*, uintptr_t)
FUNCS(uintptr, uptr, uintptr_t, uintptr_t)
FUNCS(ptr, ptr, void*, uintptr_t)
FUNCS(constptr, constptr, const void*, uintptr_t)

#undef FUNCS

WPI_UPB_INLINE void wpi_upb_value_setfloat(wpi_upb_value* val, float cval) {
  memcpy(&val->val, &cval, sizeof(cval));
}

WPI_UPB_INLINE void wpi_upb_value_setdouble(wpi_upb_value* val, double cval) {
  memcpy(&val->val, &cval, sizeof(cval));
}

WPI_UPB_INLINE wpi_upb_value wpi_upb_value_float(float cval) {
  wpi_upb_value ret;
  wpi_upb_value_setfloat(&ret, cval);
  return ret;
}

WPI_UPB_INLINE wpi_upb_value wpi_upb_value_double(double cval) {
  wpi_upb_value ret;
  wpi_upb_value_setdouble(&ret, cval);
  return ret;
}

/* wpi_upb_tabkey *****************************************************************/

/* Either:
 *   1. an actual integer key, or
 *   2. a pointer to a string prefixed by its uint32_t length, owned by us.
 *
 * ...depending on whether this is a string table or an int table.  We would
 * make this a union of those two types, but C89 doesn't support statically
 * initializing a non-first union member. */
typedef uintptr_t wpi_upb_tabkey;

WPI_UPB_INLINE char* wpi_upb_tabstr(wpi_upb_tabkey key, uint32_t* len) {
  char* mem = (char*)key;
  if (len) memcpy(len, mem, sizeof(*len));
  return mem + sizeof(*len);
}

WPI_UPB_INLINE wpi_upb_StringView wpi_upb_tabstrview(wpi_upb_tabkey key) {
  wpi_upb_StringView ret;
  uint32_t len;
  ret.data = wpi_upb_tabstr(key, &len);
  ret.size = len;
  return ret;
}

/* wpi_upb_tabval *****************************************************************/

typedef struct wpi_upb_tabval {
  uint64_t val;
} wpi_upb_tabval;

#define WPI_UPB_TABVALUE_EMPTY_INIT \
  { -1 }

/* wpi_upb_table ******************************************************************/

typedef struct _wpi_upb_tabent {
  wpi_upb_tabkey key;
  wpi_upb_tabval val;

  /* Internal chaining.  This is const so we can create static initializers for
   * tables.  We cast away const sometimes, but *only* when the containing
   * wpi_upb_table is known to be non-const.  This requires a bit of care, but
   * the subtlety is confined to table.c. */
  const struct _wpi_upb_tabent* next;
} wpi_upb_tabent;

typedef struct {
  size_t count;       /* Number of entries in the hash part. */
  uint32_t mask;      /* Mask to turn hash value -> bucket. */
  uint32_t max_count; /* Max count before we hit our load limit. */
  uint8_t size_lg2;   /* Size of the hashtable part is 2^size_lg2 entries. */
  wpi_upb_tabent* entries;
} wpi_upb_table;

WPI_UPB_INLINE size_t wpi_upb_table_size(const wpi_upb_table* t) {
  return t->size_lg2 ? 1 << t->size_lg2 : 0;
}

// Internal-only functions, in .h file only out of necessity.

WPI_UPB_INLINE bool wpi_upb_tabent_isempty(const wpi_upb_tabent* e) { return e->key == 0; }

uint32_t _wpi_upb_Hash(const void* p, size_t n, uint64_t seed);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_HASH_COMMON_H_ */

// Must be last.

typedef struct {
  wpi_upb_table t;
} wpi_upb_strtable;

#ifdef __cplusplus
extern "C" {
#endif

// Initialize a table. If memory allocation failed, false is returned and
// the table is uninitialized.
bool wpi_upb_strtable_init(wpi_upb_strtable* table, size_t expected_size, wpi_upb_Arena* a);

// Returns the number of values in the table.
WPI_UPB_INLINE size_t wpi_upb_strtable_count(const wpi_upb_strtable* t) {
  return t->t.count;
}

void wpi_upb_strtable_clear(wpi_upb_strtable* t);

// Inserts the given key into the hashtable with the given value.
// The key must not already exist in the hash table. The key is not required
// to be NULL-terminated, and the table will make an internal copy of the key.
//
// If a table resize was required but memory allocation failed, false is
// returned and the table is unchanged. */
bool wpi_upb_strtable_insert(wpi_upb_strtable* t, const char* key, size_t len,
                         wpi_upb_value val, wpi_upb_Arena* a);

// Looks up key in this table, returning "true" if the key was found.
// If v is non-NULL, copies the value for this key into *v.
bool wpi_upb_strtable_lookup2(const wpi_upb_strtable* t, const char* key, size_t len,
                          wpi_upb_value* v);

// For NULL-terminated strings.
WPI_UPB_INLINE bool wpi_upb_strtable_lookup(const wpi_upb_strtable* t, const char* key,
                                    wpi_upb_value* v) {
  return wpi_upb_strtable_lookup2(t, key, strlen(key), v);
}

// Removes an item from the table. Returns true if the remove was successful,
// and stores the removed item in *val if non-NULL.
bool wpi_upb_strtable_remove2(wpi_upb_strtable* t, const char* key, size_t len,
                          wpi_upb_value* val);

WPI_UPB_INLINE bool wpi_upb_strtable_remove(wpi_upb_strtable* t, const char* key,
                                    wpi_upb_value* v) {
  return wpi_upb_strtable_remove2(t, key, strlen(key), v);
}

// Exposed for testing only.
bool wpi_upb_strtable_resize(wpi_upb_strtable* t, size_t size_lg2, wpi_upb_Arena* a);

/* Iteration over strtable:
 *
 *   intptr_t iter = WPI_UPB_STRTABLE_BEGIN;
 *   wpi_upb_StringView key;
 *   wpi_upb_value val;
 *   while (wpi_upb_strtable_next2(t, &key, &val, &iter)) {
 *      // ...
 *   }
 */

#define WPI_UPB_STRTABLE_BEGIN -1

bool wpi_upb_strtable_next2(const wpi_upb_strtable* t, wpi_upb_StringView* key,
                        wpi_upb_value* val, intptr_t* iter);
void wpi_upb_strtable_removeiter(wpi_upb_strtable* t, intptr_t* iter);
void wpi_upb_strtable_setentryvalue(wpi_upb_strtable* t, intptr_t iter, wpi_upb_value v);

/* DEPRECATED iterators, slated for removal.
 *
 * Iterators for string tables.  We are subject to some kind of unusual
 * design constraints:
 *
 * For high-level languages:
 *  - we must be able to guarantee that we don't crash or corrupt memory even if
 *    the program accesses an invalidated iterator.
 *
 * For C++11 range-based for:
 *  - iterators must be copyable
 *  - iterators must be comparable
 *  - it must be possible to construct an "end" value.
 *
 * Iteration order is undefined.
 *
 * Modifying the table invalidates iterators.  wpi_upb_{str,int}table_done() is
 * guaranteed to work even on an invalidated iterator, as long as the table it
 * is iterating over has not been freed.  Calling next() or accessing data from
 * an invalidated iterator yields unspecified elements from the table, but it is
 * guaranteed not to crash and to return real table elements (except when done()
 * is true). */
/* wpi_upb_strtable_iter **********************************************************/

/*   wpi_upb_strtable_iter i;
 *   wpi_upb_strtable_begin(&i, t);
 *   for(; !wpi_upb_strtable_done(&i); wpi_upb_strtable_next(&i)) {
 *     const char *key = wpi_upb_strtable_iter_key(&i);
 *     const wpi_upb_value val = wpi_upb_strtable_iter_value(&i);
 *     // ...
 *   }
 */

typedef struct {
  const wpi_upb_strtable* t;
  size_t index;
} wpi_upb_strtable_iter;

WPI_UPB_INLINE const wpi_upb_tabent* str_tabent(const wpi_upb_strtable_iter* i) {
  return &i->t->t.entries[i->index];
}

void wpi_upb_strtable_begin(wpi_upb_strtable_iter* i, const wpi_upb_strtable* t);
void wpi_upb_strtable_next(wpi_upb_strtable_iter* i);
bool wpi_upb_strtable_done(const wpi_upb_strtable_iter* i);
wpi_upb_StringView wpi_upb_strtable_iter_key(const wpi_upb_strtable_iter* i);
wpi_upb_value wpi_upb_strtable_iter_value(const wpi_upb_strtable_iter* i);
void wpi_upb_strtable_iter_setdone(wpi_upb_strtable_iter* i);
bool wpi_upb_strtable_iter_isequal(const wpi_upb_strtable_iter* i1,
                               const wpi_upb_strtable_iter* i2);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_HASH_STR_TABLE_H_ */

// Must be last.

typedef enum {
  kUpb_MapInsertStatus_Inserted = 0,
  kUpb_MapInsertStatus_Replaced = 1,
  kUpb_MapInsertStatus_OutOfMemory = 2,
} wpi_upb_MapInsertStatus;

// EVERYTHING BELOW THIS LINE IS INTERNAL - DO NOT USE /////////////////////////

struct wpi_upb_Map {
  // Size of key and val, based on the map type.
  // Strings are represented as '0' because they must be handled specially.
  char key_size;
  char val_size;
  bool WPI_UPB_PRIVATE(is_frozen);

  wpi_upb_strtable table;
};

#ifdef __cplusplus
extern "C" {
#endif

WPI_UPB_INLINE void WPI_UPB_PRIVATE(_wpi_upb_Map_ShallowFreeze)(struct wpi_upb_Map* map) {
  map->WPI_UPB_PRIVATE(is_frozen) = true;
}

WPI_UPB_API_INLINE bool wpi_upb_Map_IsFrozen(const struct wpi_upb_Map* map) {
  return map->WPI_UPB_PRIVATE(is_frozen);
}

// Converting between internal table representation and user values.
//
// _wpi_upb_map_tokey() and _wpi_upb_map_fromkey() are inverses.
// _wpi_upb_map_tovalue() and _wpi_upb_map_fromvalue() are inverses.
//
// These functions account for the fact that strings are treated differently
// from other types when stored in a map.

WPI_UPB_INLINE wpi_upb_StringView _wpi_upb_map_tokey(const void* key, size_t size) {
  if (size == WPI_UPB_MAPTYPE_STRING) {
    return *(wpi_upb_StringView*)key;
  } else {
    return wpi_upb_StringView_FromDataAndSize((const char*)key, size);
  }
}

WPI_UPB_INLINE void _wpi_upb_map_fromkey(wpi_upb_StringView key, void* out, size_t size) {
  if (size == WPI_UPB_MAPTYPE_STRING) {
    memcpy(out, &key, sizeof(key));
  } else {
    memcpy(out, key.data, size);
  }
}

WPI_UPB_INLINE bool _wpi_upb_map_tovalue(const void* val, size_t size,
                                 wpi_upb_value* msgval, wpi_upb_Arena* a) {
  if (size == WPI_UPB_MAPTYPE_STRING) {
    wpi_upb_StringView* strp = (wpi_upb_StringView*)wpi_upb_Arena_Malloc(a, sizeof(*strp));
    if (!strp) return false;
    *strp = *(wpi_upb_StringView*)val;
    *msgval = wpi_upb_value_ptr(strp);
  } else {
    memcpy(msgval, val, size);
  }
  return true;
}

WPI_UPB_INLINE void _wpi_upb_map_fromvalue(wpi_upb_value val, void* out, size_t size) {
  if (size == WPI_UPB_MAPTYPE_STRING) {
    const wpi_upb_StringView* strp = (const wpi_upb_StringView*)wpi_upb_value_getptr(val);
    memcpy(out, strp, sizeof(wpi_upb_StringView));
  } else {
    memcpy(out, &val, size);
  }
}

WPI_UPB_INLINE void* _wpi_upb_map_next(const struct wpi_upb_Map* map, size_t* iter) {
  wpi_upb_strtable_iter it;
  it.t = &map->table;
  it.index = *iter;
  wpi_upb_strtable_next(&it);
  *iter = it.index;
  if (wpi_upb_strtable_done(&it)) return NULL;
  return (void*)str_tabent(&it);
}

WPI_UPB_INLINE void _wpi_upb_Map_Clear(struct wpi_upb_Map* map) {
  WPI_UPB_ASSERT(!wpi_upb_Map_IsFrozen(map));

  wpi_upb_strtable_clear(&map->table);
}

WPI_UPB_INLINE bool _wpi_upb_Map_Delete(struct wpi_upb_Map* map, const void* key,
                                size_t key_size, wpi_upb_value* val) {
  WPI_UPB_ASSERT(!wpi_upb_Map_IsFrozen(map));

  wpi_upb_StringView k = _wpi_upb_map_tokey(key, key_size);
  return wpi_upb_strtable_remove2(&map->table, k.data, k.size, val);
}

WPI_UPB_INLINE bool _wpi_upb_Map_Get(const struct wpi_upb_Map* map, const void* key,
                             size_t key_size, void* val, size_t val_size) {
  wpi_upb_value tabval;
  wpi_upb_StringView k = _wpi_upb_map_tokey(key, key_size);
  bool ret = wpi_upb_strtable_lookup2(&map->table, k.data, k.size, &tabval);
  if (ret && val) {
    _wpi_upb_map_fromvalue(tabval, val, val_size);
  }
  return ret;
}

WPI_UPB_INLINE wpi_upb_MapInsertStatus _wpi_upb_Map_Insert(struct wpi_upb_Map* map,
                                               const void* key, size_t key_size,
                                               void* val, size_t val_size,
                                               wpi_upb_Arena* a) {
  WPI_UPB_ASSERT(!wpi_upb_Map_IsFrozen(map));

  wpi_upb_StringView strkey = _wpi_upb_map_tokey(key, key_size);
  wpi_upb_value tabval = {0};
  if (!_wpi_upb_map_tovalue(val, val_size, &tabval, a)) {
    return kUpb_MapInsertStatus_OutOfMemory;
  }

  // TODO: add overwrite operation to minimize number of lookups.
  bool removed =
      wpi_upb_strtable_remove2(&map->table, strkey.data, strkey.size, NULL);
  if (!wpi_upb_strtable_insert(&map->table, strkey.data, strkey.size, tabval, a)) {
    return kUpb_MapInsertStatus_OutOfMemory;
  }
  return removed ? kUpb_MapInsertStatus_Replaced
                 : kUpb_MapInsertStatus_Inserted;
}

WPI_UPB_INLINE size_t _wpi_upb_Map_Size(const struct wpi_upb_Map* map) {
  return map->table.t.count;
}

// Strings/bytes are special-cased in maps.
extern char _wpi_upb_Map_CTypeSizeTable[12];

WPI_UPB_INLINE size_t _wpi_upb_Map_CTypeSize(wpi_upb_CType ctype) {
  return _wpi_upb_Map_CTypeSizeTable[ctype];
}

// Creates a new map on the given arena with this key/value type.
struct wpi_upb_Map* _wpi_upb_Map_New(wpi_upb_Arena* a, size_t key_size, size_t value_size);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_MESSAGE_INTERNAL_MAP_H_ */

/*
** Our memory representation for parsing tables and messages themselves.
** Functions in this file are used by generated code and possibly reflection.
**
** The definitions in this file are internal to upb.
**/

#ifndef WPI_UPB_MESSAGE_INTERNAL_MESSAGE_H_
#define WPI_UPB_MESSAGE_INTERNAL_MESSAGE_H_

#include <stdlib.h>
#include <string.h>


// Must be last.

#ifdef __cplusplus
extern "C" {
#endif

extern const float kUpb_FltInfinity;
extern const double kUpb_Infinity;
extern const double kUpb_NaN;

// Internal members of a wpi_upb_Message that track unknown fields and/or
// extensions. We can change this without breaking binary compatibility.

typedef struct wpi_upb_Message_Internal {
  // Total size of this structure, including the data that follows.
  // Must be aligned to 8, which is alignof(wpi_upb_Extension)
  uint32_t size;

  /* Offsets relative to the beginning of this structure.
   *
   * Unknown data grows forward from the beginning to unknown_end.
   * Extension data grows backward from size to ext_begin.
   * When the two meet, we're out of data and have to realloc.
   *
   * If we imagine that the final member of this struct is:
   *   char data[size - overhead];  // overhead = sizeof(wpi_upb_Message_Internal)
   *
   * Then we have:
   *   unknown data: data[0 .. (unknown_end - overhead)]
   *   extensions data: data[(ext_begin - overhead) .. (size - overhead)] */
  uint32_t unknown_end;
  uint32_t ext_begin;
  // Data follows, as if there were an array:
  //   char data[size - sizeof(wpi_upb_Message_Internal)];
} wpi_upb_Message_Internal;

#ifdef WPI_UPB_TRACING_ENABLED
WPI_UPB_API void wpi_upb_Message_LogNewMessage(const wpi_upb_MiniTable* m,
                                       const wpi_upb_Arena* arena);
WPI_UPB_API void wpi_upb_Message_SetNewMessageTraceHandler(
    void (*handler)(const wpi_upb_MiniTable*, const wpi_upb_Arena*));
#endif  // WPI_UPB_TRACING_ENABLED

// Inline version wpi_upb_Message_New(), for internal use.
WPI_UPB_INLINE struct wpi_upb_Message* _wpi_upb_Message_New(const wpi_upb_MiniTable* m,
                                                wpi_upb_Arena* a) {
#ifdef WPI_UPB_TRACING_ENABLED
  wpi_upb_Message_LogNewMessage(m, a);
#endif  // WPI_UPB_TRACING_ENABLED

  const int size = m->WPI_UPB_PRIVATE(size);
  struct wpi_upb_Message* msg = (struct wpi_upb_Message*)wpi_upb_Arena_Malloc(a, size);
  if (WPI_UPB_UNLIKELY(!msg)) return NULL;
  memset(msg, 0, size);
  return msg;
}

// Discards the unknown fields for this message only.
void _wpi_upb_Message_DiscardUnknown_shallow(struct wpi_upb_Message* msg);

// Adds unknown data (serialized protobuf data) to the given message.
// The data is copied into the message instance.
bool WPI_UPB_PRIVATE(_wpi_upb_Message_AddUnknown)(struct wpi_upb_Message* msg,
                                          const char* data, size_t len,
                                          wpi_upb_Arena* arena);

bool WPI_UPB_PRIVATE(_wpi_upb_Message_Realloc)(struct wpi_upb_Message* msg, size_t need,
                                       wpi_upb_Arena* arena);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_MESSAGE_INTERNAL_MESSAGE_H_ */

#ifndef WPI_UPB_MINI_TABLE_INTERNAL_TAGGED_PTR_H_
#define WPI_UPB_MINI_TABLE_INTERNAL_TAGGED_PTR_H_

#include <stdint.h>


// Must be last.

#ifdef __cplusplus
extern "C" {
#endif

// Internal-only because empty messages cannot be created by the user.
WPI_UPB_INLINE uintptr_t
WPI_UPB_PRIVATE(_wpi_upb_TaggedMessagePtr_Pack)(struct wpi_upb_Message* ptr, bool empty) {
  WPI_UPB_ASSERT(((uintptr_t)ptr & 1) == 0);
  return (uintptr_t)ptr | (empty ? 1 : 0);
}

WPI_UPB_API_INLINE bool wpi_upb_TaggedMessagePtr_IsEmpty(uintptr_t ptr) {
  return ptr & 1;
}

WPI_UPB_INLINE struct wpi_upb_Message* WPI_UPB_PRIVATE(_wpi_upb_TaggedMessagePtr_GetMessage)(
    uintptr_t ptr) {
  return (struct wpi_upb_Message*)(ptr & ~(uintptr_t)1);
}

WPI_UPB_API_INLINE struct wpi_upb_Message* wpi_upb_TaggedMessagePtr_GetNonEmptyMessage(
    uintptr_t ptr) {
  WPI_UPB_ASSERT(!wpi_upb_TaggedMessagePtr_IsEmpty(ptr));
  return WPI_UPB_PRIVATE(_wpi_upb_TaggedMessagePtr_GetMessage)(ptr);
}

WPI_UPB_INLINE struct wpi_upb_Message* WPI_UPB_PRIVATE(
    _wpi_upb_TaggedMessagePtr_GetEmptyMessage)(uintptr_t ptr) {
  WPI_UPB_ASSERT(wpi_upb_TaggedMessagePtr_IsEmpty(ptr));
  return WPI_UPB_PRIVATE(_wpi_upb_TaggedMessagePtr_GetMessage)(ptr);
}

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_MINI_TABLE_INTERNAL_TAGGED_PTR_H_ */

#ifndef WPI_UPB_MESSAGE_INTERNAL_TYPES_H_
#define WPI_UPB_MESSAGE_INTERNAL_TYPES_H_

#include <stdint.h>

// Must be last.

#define WPI_UPB_OPAQUE(x) x##_opaque

struct wpi_upb_Message {
  union {
    uintptr_t WPI_UPB_OPAQUE(internal);  // tagged pointer, low bit == frozen
    double d;  // Forces same size for 32-bit/64-bit builds
  };
};

#ifdef __cplusplus
extern "C" {
#endif

WPI_UPB_INLINE void WPI_UPB_PRIVATE(_wpi_upb_Message_ShallowFreeze)(
    struct wpi_upb_Message* msg) {
  msg->WPI_UPB_OPAQUE(internal) |= 1ULL;
}

WPI_UPB_API_INLINE bool wpi_upb_Message_IsFrozen(const struct wpi_upb_Message* msg) {
  return (msg->WPI_UPB_OPAQUE(internal) & 1ULL) != 0;
}

WPI_UPB_INLINE struct wpi_upb_Message_Internal* WPI_UPB_PRIVATE(_wpi_upb_Message_GetInternal)(
    const struct wpi_upb_Message* msg) {
  const uintptr_t tmp = msg->WPI_UPB_OPAQUE(internal) & ~1ULL;
  return (struct wpi_upb_Message_Internal*)tmp;
}

WPI_UPB_INLINE void WPI_UPB_PRIVATE(_wpi_upb_Message_SetInternal)(
    struct wpi_upb_Message* msg, struct wpi_upb_Message_Internal* internal) {
  WPI_UPB_ASSERT(!wpi_upb_Message_IsFrozen(msg));
  msg->WPI_UPB_OPAQUE(internal) = (uintptr_t)internal;
}

#ifdef __cplusplus
} /* extern "C" */
#endif

#undef WPI_UPB_OPAQUE


#endif /* WPI_UPB_MESSAGE_INTERNAL_TYPES_H_ */

// Must be last.

#if defined(__GNUC__) && !defined(__clang__)
// GCC raises incorrect warnings in these functions.  It thinks that we are
// overrunning buffers, but we carefully write the functions in this file to
// guarantee that this is impossible.  GCC gets this wrong due it its failure
// to perform constant propagation as we expect:
//   - https://gcc.gnu.org/bugzilla/show_bug.cgi?id=108217
//   - https://gcc.gnu.org/bugzilla/show_bug.cgi?id=108226
//
// Unfortunately this also indicates that GCC is not optimizing away the
// switch() in cases where it should be, compromising the performance.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
#pragma GCC diagnostic ignored "-Wstringop-overflow"
#if __GNUC__ >= 11
#pragma GCC diagnostic ignored "-Wstringop-overread"
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

// LINT.IfChange(presence_logic)

// Hasbit access ///////////////////////////////////////////////////////////////

WPI_UPB_INLINE bool WPI_UPB_PRIVATE(_wpi_upb_Message_GetHasbit)(
    const struct wpi_upb_Message* msg, const wpi_upb_MiniTableField* f) {
  const size_t offset = WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_HasbitOffset)(f);
  const char mask = WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_HasbitMask)(f);

  return (*WPI_UPB_PTR_AT(msg, offset, const char) & mask) != 0;
}

WPI_UPB_INLINE void WPI_UPB_PRIVATE(_wpi_upb_Message_SetHasbit)(
    const struct wpi_upb_Message* msg, const wpi_upb_MiniTableField* f) {
  const size_t offset = WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_HasbitOffset)(f);
  const char mask = WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_HasbitMask)(f);

  (*WPI_UPB_PTR_AT(msg, offset, char)) |= mask;
}

WPI_UPB_INLINE void WPI_UPB_PRIVATE(_wpi_upb_Message_ClearHasbit)(
    const struct wpi_upb_Message* msg, const wpi_upb_MiniTableField* f) {
  const size_t offset = WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_HasbitOffset)(f);
  const char mask = WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_HasbitMask)(f);

  (*WPI_UPB_PTR_AT(msg, offset, char)) &= ~mask;
}

// Oneof case access ///////////////////////////////////////////////////////////

WPI_UPB_INLINE uint32_t* WPI_UPB_PRIVATE(_wpi_upb_Message_OneofCasePtr)(
    struct wpi_upb_Message* msg, const wpi_upb_MiniTableField* f) {
  return WPI_UPB_PTR_AT(msg, WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_OneofOffset)(f),
                    uint32_t);
}

WPI_UPB_INLINE uint32_t WPI_UPB_PRIVATE(_wpi_upb_Message_GetOneofCase)(
    const struct wpi_upb_Message* msg, const wpi_upb_MiniTableField* f) {
  const uint32_t* ptr =
      WPI_UPB_PRIVATE(_wpi_upb_Message_OneofCasePtr)((struct wpi_upb_Message*)msg, f);

  return *ptr;
}

WPI_UPB_INLINE void WPI_UPB_PRIVATE(_wpi_upb_Message_SetOneofCase)(
    struct wpi_upb_Message* msg, const wpi_upb_MiniTableField* f) {
  uint32_t* ptr = WPI_UPB_PRIVATE(_wpi_upb_Message_OneofCasePtr)(msg, f);

  *ptr = wpi_upb_MiniTableField_Number(f);
}

// Returns true if the given field is the current oneof case.
// Does nothing if it is not the current oneof case.
WPI_UPB_INLINE bool WPI_UPB_PRIVATE(_wpi_upb_Message_ClearOneofCase)(
    struct wpi_upb_Message* msg, const wpi_upb_MiniTableField* f) {
  uint32_t* ptr = WPI_UPB_PRIVATE(_wpi_upb_Message_OneofCasePtr)(msg, f);

  if (*ptr != wpi_upb_MiniTableField_Number(f)) return false;
  *ptr = 0;
  return true;
}

WPI_UPB_API_INLINE uint32_t wpi_upb_Message_WhichOneofFieldNumber(
    const struct wpi_upb_Message* message, const wpi_upb_MiniTableField* oneof_field) {
  WPI_UPB_ASSUME(wpi_upb_MiniTableField_IsInOneof(oneof_field));
  return WPI_UPB_PRIVATE(_wpi_upb_Message_GetOneofCase)(message, oneof_field);
}

WPI_UPB_API_INLINE const wpi_upb_MiniTableField* wpi_upb_Message_WhichOneof(
    const struct wpi_upb_Message* msg, const wpi_upb_MiniTable* m,
    const wpi_upb_MiniTableField* f) {
  uint32_t field_number = wpi_upb_Message_WhichOneofFieldNumber(msg, f);
  if (field_number == 0) {
    // No field in the oneof is set.
    return NULL;
  }
  return wpi_upb_MiniTable_FindFieldByNumber(m, field_number);
}

// LINT.ThenChange(GoogleInternalName2)

// Returns false if the message is missing any of its required fields.
WPI_UPB_INLINE bool WPI_UPB_PRIVATE(_wpi_upb_Message_IsInitializedShallow)(
    const struct wpi_upb_Message* msg, const wpi_upb_MiniTable* m) {
  uint64_t bits;
  memcpy(&bits, msg + 1, sizeof(bits));
  bits = wpi_upb_BigEndian64(bits);
  return (WPI_UPB_PRIVATE(_wpi_upb_MiniTable_RequiredMask)(m) & ~bits) == 0;
}

WPI_UPB_INLINE void* WPI_UPB_PRIVATE(_wpi_upb_Message_MutableDataPtr)(
    struct wpi_upb_Message* msg, const wpi_upb_MiniTableField* f) {
  return (char*)msg + f->WPI_UPB_ONLYBITS(offset);
}

WPI_UPB_INLINE const void* WPI_UPB_PRIVATE(_wpi_upb_Message_DataPtr)(
    const struct wpi_upb_Message* msg, const wpi_upb_MiniTableField* f) {
  return (const char*)msg + f->WPI_UPB_ONLYBITS(offset);
}

WPI_UPB_INLINE void WPI_UPB_PRIVATE(_wpi_upb_Message_SetPresence)(
    struct wpi_upb_Message* msg, const wpi_upb_MiniTableField* f) {
  if (WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_HasHasbit)(f)) {
    WPI_UPB_PRIVATE(_wpi_upb_Message_SetHasbit)(msg, f);
  } else if (wpi_upb_MiniTableField_IsInOneof(f)) {
    WPI_UPB_PRIVATE(_wpi_upb_Message_SetOneofCase)(msg, f);
  }
}

WPI_UPB_INLINE void WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_DataCopy)(
    const wpi_upb_MiniTableField* f, void* to, const void* from) {
  switch (WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_GetRep)(f)) {
    case kUpb_FieldRep_1Byte:
      memcpy(to, from, 1);
      return;
    case kUpb_FieldRep_4Byte:
      memcpy(to, from, 4);
      return;
    case kUpb_FieldRep_8Byte:
      memcpy(to, from, 8);
      return;
    case kUpb_FieldRep_StringView: {
      memcpy(to, from, sizeof(wpi_upb_StringView));
      return;
    }
  }
  WPI_UPB_UNREACHABLE();
}

WPI_UPB_INLINE bool WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_DataEquals)(
    const wpi_upb_MiniTableField* f, const void* a, const void* b) {
  switch (WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_GetRep)(f)) {
    case kUpb_FieldRep_1Byte:
      return memcmp(a, b, 1) == 0;
    case kUpb_FieldRep_4Byte:
      return memcmp(a, b, 4) == 0;
    case kUpb_FieldRep_8Byte:
      return memcmp(a, b, 8) == 0;
    case kUpb_FieldRep_StringView: {
      const wpi_upb_StringView sa = *(const wpi_upb_StringView*)a;
      const wpi_upb_StringView sb = *(const wpi_upb_StringView*)b;
      return wpi_upb_StringView_IsEqual(sa, sb);
    }
  }
  WPI_UPB_UNREACHABLE();
}

WPI_UPB_INLINE void WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_DataClear)(
    const wpi_upb_MiniTableField* f, void* val) {
  const char zero[16] = {0};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_DataCopy)(f, val, zero);
}

WPI_UPB_INLINE bool WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_DataIsZero)(
    const wpi_upb_MiniTableField* f, const void* val) {
  const char zero[16] = {0};
  return WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_DataEquals)(f, val, zero);
}

// Here we define universal getter/setter functions for message fields.
// These look very branchy and inefficient, but as long as the MiniTableField
// values are known at compile time, all the branches are optimized away and
// we are left with ideal code.  This can happen either through through
// literals or WPI_UPB_ASSUME():
//
//   // Via struct literals.
//   bool FooMessage_set_bool_field(const wpi_upb_Message* msg, bool val) {
//     const wpi_upb_MiniTableField field = {1, 0, 0, /* etc... */};
//     // All value in "field" are compile-time known.
//     wpi_upb_Message_SetBaseField(msg, &field, &value);
//   }
//
//   // Via WPI_UPB_ASSUME().
//   WPI_UPB_INLINE bool wpi_upb_Message_SetBool(wpi_upb_Message* msg,
//                                       const wpi_upb_MiniTableField* field,
//                                       bool value, wpi_upb_Arena* a) {
//     WPI_UPB_ASSUME(field->WPI_UPB_PRIVATE(descriptortype) == kUpb_FieldType_Bool);
//     WPI_UPB_ASSUME(WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_GetRep)(field) ==
//                kUpb_FieldRep_1Byte);
//     wpi_upb_Message_SetField(msg, field, &value, a);
//   }
//
// As a result, we can use these universal getters/setters for *all* message
// accessors: generated code, MiniTable accessors, and reflection.  The only
// exception is the binary encoder/decoder, which need to be a bit more clever
// about how they read/write the message data, for efficiency.
//
// These functions work on both extensions and non-extensions. If the field
// of a setter is known to be a non-extension, the arena may be NULL and the
// returned bool value may be ignored since it will always succeed.

WPI_UPB_API_INLINE bool wpi_upb_Message_HasBaseField(const struct wpi_upb_Message* msg,
                                             const wpi_upb_MiniTableField* field) {
  WPI_UPB_ASSERT(wpi_upb_MiniTableField_HasPresence(field));
  WPI_UPB_ASSUME(!wpi_upb_MiniTableField_IsExtension(field));
  if (wpi_upb_MiniTableField_IsInOneof(field)) {
    return WPI_UPB_PRIVATE(_wpi_upb_Message_GetOneofCase)(msg, field) ==
           wpi_upb_MiniTableField_Number(field);
  } else {
    return WPI_UPB_PRIVATE(_wpi_upb_Message_GetHasbit)(msg, field);
  }
}

WPI_UPB_API_INLINE bool wpi_upb_Message_HasExtension(const struct wpi_upb_Message* msg,
                                             const wpi_upb_MiniTableExtension* e) {
  WPI_UPB_ASSERT(wpi_upb_MiniTableField_HasPresence(&e->WPI_UPB_PRIVATE(field)));
  return WPI_UPB_PRIVATE(_wpi_upb_Message_Getext)(msg, e) != NULL;
}

WPI_UPB_FORCEINLINE void _wpi_upb_Message_GetNonExtensionField(
    const struct wpi_upb_Message* msg, const wpi_upb_MiniTableField* field,
    const void* default_val, void* val) {
  WPI_UPB_ASSUME(!wpi_upb_MiniTableField_IsExtension(field));
  if ((wpi_upb_MiniTableField_IsInOneof(field) ||
       !WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_DataIsZero)(field, default_val)) &&
      !wpi_upb_Message_HasBaseField(msg, field)) {
    WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_DataCopy)(field, val, default_val);
    return;
  }
  WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_DataCopy)
  (field, val, WPI_UPB_PRIVATE(_wpi_upb_Message_DataPtr)(msg, field));
}

WPI_UPB_INLINE void _wpi_upb_Message_GetExtensionField(
    const struct wpi_upb_Message* msg, const wpi_upb_MiniTableExtension* mt_ext,
    const void* default_val, void* val) {
  const wpi_upb_Extension* ext = WPI_UPB_PRIVATE(_wpi_upb_Message_Getext)(msg, mt_ext);
  const wpi_upb_MiniTableField* f = &mt_ext->WPI_UPB_PRIVATE(field);
  WPI_UPB_ASSUME(wpi_upb_MiniTableField_IsExtension(f));

  if (ext) {
    WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_DataCopy)(f, val, &ext->data);
  } else {
    WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_DataCopy)(f, val, default_val);
  }
}

// NOTE: The default_val is only used for fields that support presence.
// For repeated/map fields, the resulting wpi_upb_Array*/wpi_upb_Map* can be NULL if a
// wpi_upb_Array/wpi_upb_Map has not been allocated yet. Array/map fields do not have
// presence, so this is semantically identical to a pointer to an empty
// array/map, and must be treated the same for all semantic purposes.
WPI_UPB_API_INLINE wpi_upb_MessageValue wpi_upb_Message_GetField(
    const struct wpi_upb_Message* msg, const wpi_upb_MiniTableField* field,
    wpi_upb_MessageValue default_val) {
  wpi_upb_MessageValue ret;
  if (wpi_upb_MiniTableField_IsExtension(field)) {
    _wpi_upb_Message_GetExtensionField(msg, (wpi_upb_MiniTableExtension*)field,
                                   &default_val, &ret);
  } else {
    _wpi_upb_Message_GetNonExtensionField(msg, field, &default_val, &ret);
  }
  return ret;
}

WPI_UPB_API_INLINE void wpi_upb_Message_SetBaseField(struct wpi_upb_Message* msg,
                                             const wpi_upb_MiniTableField* f,
                                             const void* val) {
  WPI_UPB_ASSERT(!wpi_upb_Message_IsFrozen(msg));
  WPI_UPB_ASSUME(!wpi_upb_MiniTableField_IsExtension(f));
  WPI_UPB_PRIVATE(_wpi_upb_Message_SetPresence)(msg, f);
  WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_DataCopy)
  (f, WPI_UPB_PRIVATE(_wpi_upb_Message_MutableDataPtr)(msg, f), val);
}

WPI_UPB_API_INLINE bool wpi_upb_Message_SetExtension(struct wpi_upb_Message* msg,
                                             const wpi_upb_MiniTableExtension* e,
                                             const void* val, wpi_upb_Arena* a) {
  WPI_UPB_ASSERT(!wpi_upb_Message_IsFrozen(msg));
  WPI_UPB_ASSERT(a);
  wpi_upb_Extension* ext =
      WPI_UPB_PRIVATE(_wpi_upb_Message_GetOrCreateExtension)(msg, e, a);
  if (!ext) return false;
  WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_DataCopy)
  (&e->WPI_UPB_PRIVATE(field), &ext->data, val);
  return true;
}

// Sets the value of the given field in the given msg. The return value is true
// if the operation completed successfully, or false if memory allocation
// failed.
WPI_UPB_INLINE bool WPI_UPB_PRIVATE(_wpi_upb_Message_SetField)(struct wpi_upb_Message* msg,
                                                   const wpi_upb_MiniTableField* f,
                                                   wpi_upb_MessageValue val,
                                                   wpi_upb_Arena* a) {
  if (wpi_upb_MiniTableField_IsExtension(f)) {
    const wpi_upb_MiniTableExtension* ext = (const wpi_upb_MiniTableExtension*)f;
    return wpi_upb_Message_SetExtension(msg, ext, &val, a);
  } else {
    wpi_upb_Message_SetBaseField(msg, f, &val);
    return true;
  }
}

WPI_UPB_API_INLINE const wpi_upb_Array* wpi_upb_Message_GetArray(
    const struct wpi_upb_Message* msg, const wpi_upb_MiniTableField* f) {
  WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_CheckIsArray)(f);
  wpi_upb_Array* ret;
  const wpi_upb_Array* default_val = NULL;
  _wpi_upb_Message_GetNonExtensionField(msg, f, &default_val, &ret);
  return ret;
}

WPI_UPB_API_INLINE bool wpi_upb_Message_GetBool(const struct wpi_upb_Message* msg,
                                        const wpi_upb_MiniTableField* f,
                                        bool default_val) {
  WPI_UPB_ASSUME(wpi_upb_MiniTableField_CType(f) == kUpb_CType_Bool);
  WPI_UPB_ASSUME(wpi_upb_MiniTableField_IsScalar(f));
  WPI_UPB_ASSUME(WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_GetRep)(f) == kUpb_FieldRep_1Byte);
  wpi_upb_MessageValue def;
  def.bool_val = default_val;
  return wpi_upb_Message_GetField(msg, f, def).bool_val;
}

WPI_UPB_API_INLINE double wpi_upb_Message_GetDouble(const struct wpi_upb_Message* msg,
                                            const wpi_upb_MiniTableField* f,
                                            double default_val) {
  WPI_UPB_ASSUME(wpi_upb_MiniTableField_CType(f) == kUpb_CType_Double);
  WPI_UPB_ASSUME(wpi_upb_MiniTableField_IsScalar(f));
  WPI_UPB_ASSUME(WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_GetRep)(f) == kUpb_FieldRep_8Byte);

  wpi_upb_MessageValue def;
  def.double_val = default_val;
  return wpi_upb_Message_GetField(msg, f, def).double_val;
}

WPI_UPB_API_INLINE float wpi_upb_Message_GetFloat(const struct wpi_upb_Message* msg,
                                          const wpi_upb_MiniTableField* f,
                                          float default_val) {
  WPI_UPB_ASSUME(wpi_upb_MiniTableField_CType(f) == kUpb_CType_Float);
  WPI_UPB_ASSUME(wpi_upb_MiniTableField_IsScalar(f));
  WPI_UPB_ASSUME(WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_GetRep)(f) == kUpb_FieldRep_4Byte);

  wpi_upb_MessageValue def;
  def.float_val = default_val;
  return wpi_upb_Message_GetField(msg, f, def).float_val;
}

WPI_UPB_API_INLINE int32_t wpi_upb_Message_GetInt32(const struct wpi_upb_Message* msg,
                                            const wpi_upb_MiniTableField* f,
                                            int32_t default_val) {
  WPI_UPB_ASSUME(wpi_upb_MiniTableField_CType(f) == kUpb_CType_Int32 ||
             wpi_upb_MiniTableField_CType(f) == kUpb_CType_Enum);
  WPI_UPB_ASSUME(wpi_upb_MiniTableField_IsScalar(f));
  WPI_UPB_ASSUME(WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_GetRep)(f) == kUpb_FieldRep_4Byte);

  wpi_upb_MessageValue def;
  def.int32_val = default_val;
  return wpi_upb_Message_GetField(msg, f, def).int32_val;
}

WPI_UPB_API_INLINE int64_t wpi_upb_Message_GetInt64(const struct wpi_upb_Message* msg,
                                            const wpi_upb_MiniTableField* f,
                                            int64_t default_val) {
  WPI_UPB_ASSUME(wpi_upb_MiniTableField_CType(f) == kUpb_CType_Int64);
  WPI_UPB_ASSUME(wpi_upb_MiniTableField_IsScalar(f));
  WPI_UPB_ASSUME(WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_GetRep)(f) == kUpb_FieldRep_8Byte);

  wpi_upb_MessageValue def;
  def.int64_val = default_val;
  return wpi_upb_Message_GetField(msg, f, def).int64_val;
}

WPI_UPB_INLINE void WPI_UPB_PRIVATE(_wpi_upb_Message_AssertMapIsUntagged)(
    const struct wpi_upb_Message* msg, const wpi_upb_MiniTableField* field) {
  WPI_UPB_UNUSED(msg);
  WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_CheckIsMap)(field);
#ifndef NDEBUG
  uintptr_t default_val = 0;
  uintptr_t tagged;
  _wpi_upb_Message_GetNonExtensionField(msg, field, &default_val, &tagged);
  WPI_UPB_ASSERT(!wpi_upb_TaggedMessagePtr_IsEmpty(tagged));
#endif
}

WPI_UPB_API_INLINE const struct wpi_upb_Map* wpi_upb_Message_GetMap(
    const struct wpi_upb_Message* msg, const wpi_upb_MiniTableField* f) {
  WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_CheckIsMap)(f);
  WPI_UPB_PRIVATE(_wpi_upb_Message_AssertMapIsUntagged)(msg, f);
  struct wpi_upb_Map* ret;
  const struct wpi_upb_Map* default_val = NULL;
  _wpi_upb_Message_GetNonExtensionField(msg, f, &default_val, &ret);
  return ret;
}

WPI_UPB_API_INLINE uintptr_t wpi_upb_Message_GetTaggedMessagePtr(
    const struct wpi_upb_Message* msg, const wpi_upb_MiniTableField* f,
    struct wpi_upb_Message* default_val) {
  WPI_UPB_ASSUME(wpi_upb_MiniTableField_CType(f) == kUpb_CType_Message);
  WPI_UPB_ASSUME(WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_GetRep)(f) ==
             WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte));
  WPI_UPB_ASSUME(wpi_upb_MiniTableField_IsScalar(f));
  uintptr_t tagged;
  _wpi_upb_Message_GetNonExtensionField(msg, f, &default_val, &tagged);
  return tagged;
}

// For internal use only; users cannot set tagged messages because only the
// parser and the message copier are allowed to directly create an empty
// message.
WPI_UPB_INLINE void WPI_UPB_PRIVATE(_wpi_upb_Message_SetTaggedMessagePtr)(
    struct wpi_upb_Message* msg, const wpi_upb_MiniTableField* f,
    uintptr_t sub_message) {
  WPI_UPB_ASSUME(wpi_upb_MiniTableField_CType(f) == kUpb_CType_Message);
  WPI_UPB_ASSUME(WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_GetRep)(f) ==
             WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte));
  WPI_UPB_ASSUME(wpi_upb_MiniTableField_IsScalar(f));
  wpi_upb_Message_SetBaseField(msg, f, &sub_message);
}

WPI_UPB_API_INLINE const struct wpi_upb_Message* wpi_upb_Message_GetMessage(
    const struct wpi_upb_Message* msg, const wpi_upb_MiniTableField* f) {
  uintptr_t tagged = wpi_upb_Message_GetTaggedMessagePtr(msg, f, NULL);
  return wpi_upb_TaggedMessagePtr_GetNonEmptyMessage(tagged);
}

WPI_UPB_API_INLINE wpi_upb_Array* wpi_upb_Message_GetMutableArray(
    struct wpi_upb_Message* msg, const wpi_upb_MiniTableField* f) {
  WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_CheckIsArray)(f);
  return (wpi_upb_Array*)wpi_upb_Message_GetArray(msg, f);
}

WPI_UPB_API_INLINE struct wpi_upb_Map* wpi_upb_Message_GetMutableMap(
    struct wpi_upb_Message* msg, const wpi_upb_MiniTableField* f) {
  return (struct wpi_upb_Map*)wpi_upb_Message_GetMap(msg, f);
}

WPI_UPB_API_INLINE struct wpi_upb_Message* wpi_upb_Message_GetMutableMessage(
    struct wpi_upb_Message* msg, const wpi_upb_MiniTableField* f) {
  return (struct wpi_upb_Message*)wpi_upb_Message_GetMessage(msg, f);
}

WPI_UPB_API_INLINE wpi_upb_Array* wpi_upb_Message_GetOrCreateMutableArray(
    struct wpi_upb_Message* msg, const wpi_upb_MiniTableField* f, wpi_upb_Arena* arena) {
  WPI_UPB_ASSERT(arena);
  WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_CheckIsArray)(f);
  wpi_upb_Array* array = wpi_upb_Message_GetMutableArray(msg, f);
  if (!array) {
    array = WPI_UPB_PRIVATE(_wpi_upb_Array_New)(
        arena, 4, WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_ElemSizeLg2)(f));
    // Check again due to: https://godbolt.org/z/7WfaoKG1r
    WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_CheckIsArray)(f);
    wpi_upb_MessageValue val;
    val.array_val = array;
    WPI_UPB_PRIVATE(_wpi_upb_Message_SetField)(msg, f, val, arena);
  }
  return array;
}

WPI_UPB_INLINE struct wpi_upb_Map* _wpi_upb_Message_GetOrCreateMutableMap(
    struct wpi_upb_Message* msg, const wpi_upb_MiniTableField* field, size_t key_size,
    size_t val_size, wpi_upb_Arena* arena) {
  WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_CheckIsMap)(field);
  WPI_UPB_PRIVATE(_wpi_upb_Message_AssertMapIsUntagged)(msg, field);
  struct wpi_upb_Map* map = NULL;
  struct wpi_upb_Map* default_map_value = NULL;
  _wpi_upb_Message_GetNonExtensionField(msg, field, &default_map_value, &map);
  if (!map) {
    map = _wpi_upb_Map_New(arena, key_size, val_size);
    // Check again due to: https://godbolt.org/z/7WfaoKG1r
    WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_CheckIsMap)(field);
    wpi_upb_Message_SetBaseField(msg, field, &map);
  }
  return map;
}

WPI_UPB_API_INLINE struct wpi_upb_Map* wpi_upb_Message_GetOrCreateMutableMap(
    struct wpi_upb_Message* msg, const wpi_upb_MiniTable* map_entry_mini_table,
    const wpi_upb_MiniTableField* f, wpi_upb_Arena* arena) {
  WPI_UPB_ASSUME(wpi_upb_MiniTableField_CType(f) == kUpb_CType_Message);
  const wpi_upb_MiniTableField* map_entry_key_field =
      &map_entry_mini_table->WPI_UPB_ONLYBITS(fields)[0];
  const wpi_upb_MiniTableField* map_entry_value_field =
      &map_entry_mini_table->WPI_UPB_ONLYBITS(fields)[1];
  return _wpi_upb_Message_GetOrCreateMutableMap(
      msg, f, _wpi_upb_Map_CTypeSize(wpi_upb_MiniTableField_CType(map_entry_key_field)),
      _wpi_upb_Map_CTypeSize(wpi_upb_MiniTableField_CType(map_entry_value_field)),
      arena);
}

WPI_UPB_API_INLINE struct wpi_upb_Message* wpi_upb_Message_GetOrCreateMutableMessage(
    struct wpi_upb_Message* msg, const wpi_upb_MiniTable* mini_table,
    const wpi_upb_MiniTableField* f, wpi_upb_Arena* arena) {
  WPI_UPB_ASSERT(arena);
  WPI_UPB_ASSUME(wpi_upb_MiniTableField_CType(f) == kUpb_CType_Message);
  WPI_UPB_ASSUME(!wpi_upb_MiniTableField_IsExtension(f));
  struct wpi_upb_Message* sub_message =
      *WPI_UPB_PTR_AT(msg, f->WPI_UPB_ONLYBITS(offset), struct wpi_upb_Message*);
  if (!sub_message) {
    const wpi_upb_MiniTable* sub_mini_table =
        wpi_upb_MiniTable_SubMessage(mini_table, f);
    WPI_UPB_ASSERT(sub_mini_table);
    sub_message = _wpi_upb_Message_New(sub_mini_table, arena);
    *WPI_UPB_PTR_AT(msg, f->WPI_UPB_ONLYBITS(offset), struct wpi_upb_Message*) =
        sub_message;
    WPI_UPB_PRIVATE(_wpi_upb_Message_SetPresence)(msg, f);
  }
  return sub_message;
}

WPI_UPB_API_INLINE wpi_upb_StringView
wpi_upb_Message_GetString(const struct wpi_upb_Message* msg,
                      const wpi_upb_MiniTableField* f, wpi_upb_StringView default_val) {
  WPI_UPB_ASSUME(wpi_upb_MiniTableField_CType(f) == kUpb_CType_String ||
             wpi_upb_MiniTableField_CType(f) == kUpb_CType_Bytes);
  WPI_UPB_ASSUME(WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_GetRep)(f) ==
             kUpb_FieldRep_StringView);

  wpi_upb_MessageValue def;
  def.str_val = default_val;
  return wpi_upb_Message_GetField(msg, f, def).str_val;
}

WPI_UPB_API_INLINE uint32_t wpi_upb_Message_GetUInt32(const struct wpi_upb_Message* msg,
                                              const wpi_upb_MiniTableField* f,
                                              uint32_t default_val) {
  WPI_UPB_ASSUME(wpi_upb_MiniTableField_CType(f) == kUpb_CType_UInt32);
  WPI_UPB_ASSUME(wpi_upb_MiniTableField_IsScalar(f));
  WPI_UPB_ASSUME(WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_GetRep)(f) == kUpb_FieldRep_4Byte);

  wpi_upb_MessageValue def;
  def.uint32_val = default_val;
  return wpi_upb_Message_GetField(msg, f, def).uint32_val;
}

WPI_UPB_API_INLINE uint64_t wpi_upb_Message_GetUInt64(const struct wpi_upb_Message* msg,
                                              const wpi_upb_MiniTableField* f,
                                              uint64_t default_val) {
  WPI_UPB_ASSUME(wpi_upb_MiniTableField_CType(f) == kUpb_CType_UInt64);
  WPI_UPB_ASSUME(wpi_upb_MiniTableField_IsScalar(f));
  WPI_UPB_ASSUME(WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_GetRep)(f) == kUpb_FieldRep_8Byte);

  wpi_upb_MessageValue def;
  def.uint64_val = default_val;
  return wpi_upb_Message_GetField(msg, f, def).uint64_val;
}

// BaseField Setters ///////////////////////////////////////////////////////////

WPI_UPB_API_INLINE void wpi_upb_Message_SetBaseFieldBool(struct wpi_upb_Message* msg,
                                                 const wpi_upb_MiniTableField* f,
                                                 bool value) {
  WPI_UPB_ASSUME(wpi_upb_MiniTableField_CType(f) == kUpb_CType_Bool);
  WPI_UPB_ASSUME(wpi_upb_MiniTableField_IsScalar(f));
  WPI_UPB_ASSUME(WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_GetRep)(f) == kUpb_FieldRep_1Byte);
  wpi_upb_Message_SetBaseField(msg, f, &value);
}

WPI_UPB_API_INLINE void wpi_upb_Message_SetBaseFieldDouble(struct wpi_upb_Message* msg,
                                                   const wpi_upb_MiniTableField* f,
                                                   double value) {
  WPI_UPB_ASSUME(wpi_upb_MiniTableField_CType(f) == kUpb_CType_Double);
  WPI_UPB_ASSUME(wpi_upb_MiniTableField_IsScalar(f));
  WPI_UPB_ASSUME(WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_GetRep)(f) == kUpb_FieldRep_8Byte);
  wpi_upb_Message_SetBaseField(msg, f, &value);
}

WPI_UPB_API_INLINE void wpi_upb_Message_SetBaseFieldFloat(struct wpi_upb_Message* msg,
                                                  const wpi_upb_MiniTableField* f,
                                                  float value) {
  WPI_UPB_ASSUME(wpi_upb_MiniTableField_CType(f) == kUpb_CType_Float);
  WPI_UPB_ASSUME(wpi_upb_MiniTableField_IsScalar(f));
  WPI_UPB_ASSUME(WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_GetRep)(f) == kUpb_FieldRep_4Byte);
  wpi_upb_Message_SetBaseField(msg, f, &value);
}

WPI_UPB_API_INLINE void wpi_upb_Message_SetBaseFieldInt32(struct wpi_upb_Message* msg,
                                                  const wpi_upb_MiniTableField* f,
                                                  int32_t value) {
  WPI_UPB_ASSUME(wpi_upb_MiniTableField_CType(f) == kUpb_CType_Int32 ||
             wpi_upb_MiniTableField_CType(f) == kUpb_CType_Enum);
  WPI_UPB_ASSUME(wpi_upb_MiniTableField_IsScalar(f));
  WPI_UPB_ASSUME(WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_GetRep)(f) == kUpb_FieldRep_4Byte);
  wpi_upb_Message_SetBaseField(msg, f, &value);
}

WPI_UPB_API_INLINE void wpi_upb_Message_SetBaseFieldInt64(struct wpi_upb_Message* msg,
                                                  const wpi_upb_MiniTableField* f,
                                                  int64_t value) {
  WPI_UPB_ASSUME(wpi_upb_MiniTableField_CType(f) == kUpb_CType_Int64);
  WPI_UPB_ASSUME(wpi_upb_MiniTableField_IsScalar(f));
  WPI_UPB_ASSUME(WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_GetRep)(f) == kUpb_FieldRep_8Byte);
  wpi_upb_Message_SetBaseField(msg, f, &value);
}

WPI_UPB_API_INLINE void wpi_upb_Message_SetBaseFieldString(struct wpi_upb_Message* msg,
                                                   const wpi_upb_MiniTableField* f,
                                                   wpi_upb_StringView value) {
  WPI_UPB_ASSUME(wpi_upb_MiniTableField_CType(f) == kUpb_CType_String ||
             wpi_upb_MiniTableField_CType(f) == kUpb_CType_Bytes);
  WPI_UPB_ASSUME(wpi_upb_MiniTableField_IsScalar(f));
  WPI_UPB_ASSUME(WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_GetRep)(f) ==
             kUpb_FieldRep_StringView);
  wpi_upb_Message_SetBaseField(msg, f, &value);
}

WPI_UPB_API_INLINE void wpi_upb_Message_SetBaseFieldUInt32(struct wpi_upb_Message* msg,
                                                   const wpi_upb_MiniTableField* f,
                                                   uint32_t value) {
  WPI_UPB_ASSUME(wpi_upb_MiniTableField_CType(f) == kUpb_CType_UInt32);
  WPI_UPB_ASSUME(wpi_upb_MiniTableField_IsScalar(f));
  WPI_UPB_ASSUME(WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_GetRep)(f) == kUpb_FieldRep_4Byte);
  wpi_upb_Message_SetBaseField(msg, f, &value);
}

WPI_UPB_API_INLINE void wpi_upb_Message_SetBaseFieldUInt64(struct wpi_upb_Message* msg,
                                                   const wpi_upb_MiniTableField* f,
                                                   uint64_t value) {
  WPI_UPB_ASSUME(wpi_upb_MiniTableField_CType(f) == kUpb_CType_UInt64);
  WPI_UPB_ASSUME(wpi_upb_MiniTableField_IsScalar(f));
  WPI_UPB_ASSUME(WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_GetRep)(f) == kUpb_FieldRep_8Byte);
  wpi_upb_Message_SetBaseField(msg, f, &value);
}

WPI_UPB_API_INLINE void wpi_upb_Message_SetClosedEnum(struct wpi_upb_Message* msg,
                                              const wpi_upb_MiniTable* m,
                                              const wpi_upb_MiniTableField* f,
                                              int32_t value) {
  WPI_UPB_ASSERT(wpi_upb_MiniTableField_IsClosedEnum(f));
  WPI_UPB_ASSUME(WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_GetRep)(f) == kUpb_FieldRep_4Byte);
  WPI_UPB_ASSERT(
      wpi_upb_MiniTableEnum_CheckValue(wpi_upb_MiniTable_GetSubEnumTable(m, f), value));
  wpi_upb_Message_SetBaseField(msg, f, &value);
}

// Extension Setters ///////////////////////////////////////////////////////////

WPI_UPB_API_INLINE bool wpi_upb_Message_SetExtensionBool(
    struct wpi_upb_Message* msg, const wpi_upb_MiniTableExtension* e, bool value,
    wpi_upb_Arena* a) {
  WPI_UPB_ASSUME(wpi_upb_MiniTableExtension_CType(e) == kUpb_CType_Bool);
  WPI_UPB_ASSUME(WPI_UPB_PRIVATE(_wpi_upb_MiniTableExtension_GetRep)(e) ==
             kUpb_FieldRep_1Byte);
  return wpi_upb_Message_SetExtension(msg, e, &value, a);
}

WPI_UPB_API_INLINE bool wpi_upb_Message_SetExtensionDouble(
    struct wpi_upb_Message* msg, const wpi_upb_MiniTableExtension* e, double value,
    wpi_upb_Arena* a) {
  WPI_UPB_ASSUME(wpi_upb_MiniTableExtension_CType(e) == kUpb_CType_Double);
  WPI_UPB_ASSUME(WPI_UPB_PRIVATE(_wpi_upb_MiniTableExtension_GetRep)(e) ==
             kUpb_FieldRep_8Byte);
  return wpi_upb_Message_SetExtension(msg, e, &value, a);
}

WPI_UPB_API_INLINE bool wpi_upb_Message_SetExtensionFloat(
    struct wpi_upb_Message* msg, const wpi_upb_MiniTableExtension* e, float value,
    wpi_upb_Arena* a) {
  WPI_UPB_ASSUME(wpi_upb_MiniTableExtension_CType(e) == kUpb_CType_Float);
  WPI_UPB_ASSUME(WPI_UPB_PRIVATE(_wpi_upb_MiniTableExtension_GetRep)(e) ==
             kUpb_FieldRep_4Byte);
  return wpi_upb_Message_SetExtension(msg, e, &value, a);
}

WPI_UPB_API_INLINE bool wpi_upb_Message_SetExtensionInt32(
    struct wpi_upb_Message* msg, const wpi_upb_MiniTableExtension* e, int32_t value,
    wpi_upb_Arena* a) {
  WPI_UPB_ASSUME(wpi_upb_MiniTableExtension_CType(e) == kUpb_CType_Int32 ||
             wpi_upb_MiniTableExtension_CType(e) == kUpb_CType_Enum);
  WPI_UPB_ASSUME(WPI_UPB_PRIVATE(_wpi_upb_MiniTableExtension_GetRep)(e) ==
             kUpb_FieldRep_4Byte);
  return wpi_upb_Message_SetExtension(msg, e, &value, a);
}

WPI_UPB_API_INLINE bool wpi_upb_Message_SetExtensionInt64(
    struct wpi_upb_Message* msg, const wpi_upb_MiniTableExtension* e, int64_t value,
    wpi_upb_Arena* a) {
  WPI_UPB_ASSUME(wpi_upb_MiniTableExtension_CType(e) == kUpb_CType_Int64);
  WPI_UPB_ASSUME(WPI_UPB_PRIVATE(_wpi_upb_MiniTableExtension_GetRep)(e) ==
             kUpb_FieldRep_8Byte);
  return wpi_upb_Message_SetExtension(msg, e, &value, a);
}

WPI_UPB_API_INLINE bool wpi_upb_Message_SetExtensionString(
    struct wpi_upb_Message* msg, const wpi_upb_MiniTableExtension* e,
    wpi_upb_StringView value, wpi_upb_Arena* a) {
  WPI_UPB_ASSUME(wpi_upb_MiniTableExtension_CType(e) == kUpb_CType_String ||
             wpi_upb_MiniTableExtension_CType(e) == kUpb_CType_Bytes);
  WPI_UPB_ASSUME(WPI_UPB_PRIVATE(_wpi_upb_MiniTableExtension_GetRep)(e) ==
             kUpb_FieldRep_StringView);
  return wpi_upb_Message_SetExtension(msg, e, &value, a);
}

WPI_UPB_API_INLINE bool wpi_upb_Message_SetExtensionUInt32(
    struct wpi_upb_Message* msg, const wpi_upb_MiniTableExtension* e, uint32_t value,
    wpi_upb_Arena* a) {
  WPI_UPB_ASSUME(wpi_upb_MiniTableExtension_CType(e) == kUpb_CType_UInt32);
  WPI_UPB_ASSUME(WPI_UPB_PRIVATE(_wpi_upb_MiniTableExtension_GetRep)(e) ==
             kUpb_FieldRep_4Byte);
  return wpi_upb_Message_SetExtension(msg, e, &value, a);
}

WPI_UPB_API_INLINE bool wpi_upb_Message_SetExtensionUInt64(
    struct wpi_upb_Message* msg, const wpi_upb_MiniTableExtension* e, uint64_t value,
    wpi_upb_Arena* a) {
  WPI_UPB_ASSUME(wpi_upb_MiniTableExtension_CType(e) == kUpb_CType_UInt64);
  WPI_UPB_ASSUME(WPI_UPB_PRIVATE(_wpi_upb_MiniTableExtension_GetRep)(e) ==
             kUpb_FieldRep_8Byte);
  return wpi_upb_Message_SetExtension(msg, e, &value, a);
}

// Universal Setters ///////////////////////////////////////////////////////////

WPI_UPB_API_INLINE bool wpi_upb_Message_SetBool(struct wpi_upb_Message* msg,
                                        const wpi_upb_MiniTableField* f, bool value,
                                        wpi_upb_Arena* a) {
  return wpi_upb_MiniTableField_IsExtension(f)
             ? wpi_upb_Message_SetExtensionBool(
                   msg, (const wpi_upb_MiniTableExtension*)f, value, a)
             : (wpi_upb_Message_SetBaseFieldBool(msg, f, value), true);
}

WPI_UPB_API_INLINE bool wpi_upb_Message_SetDouble(struct wpi_upb_Message* msg,
                                          const wpi_upb_MiniTableField* f,
                                          double value, wpi_upb_Arena* a) {
  return wpi_upb_MiniTableField_IsExtension(f)
             ? wpi_upb_Message_SetExtensionDouble(
                   msg, (const wpi_upb_MiniTableExtension*)f, value, a)
             : (wpi_upb_Message_SetBaseFieldDouble(msg, f, value), true);
}

WPI_UPB_API_INLINE bool wpi_upb_Message_SetFloat(struct wpi_upb_Message* msg,
                                         const wpi_upb_MiniTableField* f,
                                         float value, wpi_upb_Arena* a) {
  return wpi_upb_MiniTableField_IsExtension(f)
             ? wpi_upb_Message_SetExtensionFloat(
                   msg, (const wpi_upb_MiniTableExtension*)f, value, a)
             : (wpi_upb_Message_SetBaseFieldFloat(msg, f, value), true);
}

WPI_UPB_API_INLINE bool wpi_upb_Message_SetInt32(struct wpi_upb_Message* msg,
                                         const wpi_upb_MiniTableField* f,
                                         int32_t value, wpi_upb_Arena* a) {
  return wpi_upb_MiniTableField_IsExtension(f)
             ? wpi_upb_Message_SetExtensionInt32(
                   msg, (const wpi_upb_MiniTableExtension*)f, value, a)
             : (wpi_upb_Message_SetBaseFieldInt32(msg, f, value), true);
}

WPI_UPB_API_INLINE bool wpi_upb_Message_SetInt64(struct wpi_upb_Message* msg,
                                         const wpi_upb_MiniTableField* f,
                                         int64_t value, wpi_upb_Arena* a) {
  return wpi_upb_MiniTableField_IsExtension(f)
             ? wpi_upb_Message_SetExtensionInt64(
                   msg, (const wpi_upb_MiniTableExtension*)f, value, a)
             : (wpi_upb_Message_SetBaseFieldInt64(msg, f, value), true);
}

// Sets the value of a message-typed field. The mini_tables of `msg` and
// `value` must have been linked for this to work correctly.
WPI_UPB_API_INLINE void wpi_upb_Message_SetMessage(struct wpi_upb_Message* msg,
                                           const wpi_upb_MiniTableField* f,
                                           struct wpi_upb_Message* value) {
  WPI_UPB_PRIVATE(_wpi_upb_Message_SetTaggedMessagePtr)
  (msg, f, WPI_UPB_PRIVATE(_wpi_upb_TaggedMessagePtr_Pack)(value, false));
}

// Sets the value of a `string` or `bytes` field. The bytes of the value are not
// copied, so it is the caller's responsibility to ensure that they remain valid
// for the lifetime of `msg`. That might be done by copying them into the given
// arena, or by fusing that arena with the arena the bytes live in, for example.
WPI_UPB_API_INLINE bool wpi_upb_Message_SetString(struct wpi_upb_Message* msg,
                                          const wpi_upb_MiniTableField* f,
                                          wpi_upb_StringView value, wpi_upb_Arena* a) {
  return wpi_upb_MiniTableField_IsExtension(f)
             ? wpi_upb_Message_SetExtensionString(
                   msg, (const wpi_upb_MiniTableExtension*)f, value, a)
             : (wpi_upb_Message_SetBaseFieldString(msg, f, value), true);
}

WPI_UPB_API_INLINE bool wpi_upb_Message_SetUInt32(struct wpi_upb_Message* msg,
                                          const wpi_upb_MiniTableField* f,
                                          uint32_t value, wpi_upb_Arena* a) {
  return wpi_upb_MiniTableField_IsExtension(f)
             ? wpi_upb_Message_SetExtensionUInt32(
                   msg, (const wpi_upb_MiniTableExtension*)f, value, a)
             : (wpi_upb_Message_SetBaseFieldUInt32(msg, f, value), true);
}

WPI_UPB_API_INLINE bool wpi_upb_Message_SetUInt64(struct wpi_upb_Message* msg,
                                          const wpi_upb_MiniTableField* f,
                                          uint64_t value, wpi_upb_Arena* a) {
  return wpi_upb_MiniTableField_IsExtension(f)
             ? wpi_upb_Message_SetExtensionUInt64(
                   msg, (const wpi_upb_MiniTableExtension*)f, value, a)
             : (wpi_upb_Message_SetBaseFieldUInt64(msg, f, value), true);
}

WPI_UPB_API_INLINE void wpi_upb_Message_Clear(struct wpi_upb_Message* msg,
                                      const wpi_upb_MiniTable* m) {
  WPI_UPB_ASSERT(!wpi_upb_Message_IsFrozen(msg));
  wpi_upb_Message_Internal* in = WPI_UPB_PRIVATE(_wpi_upb_Message_GetInternal)(msg);
  memset(msg, 0, m->WPI_UPB_PRIVATE(size));
  if (in) {
    // Reset the internal buffer to empty.
    in->unknown_end = sizeof(wpi_upb_Message_Internal);
    in->ext_begin = in->size;
    WPI_UPB_PRIVATE(_wpi_upb_Message_SetInternal)(msg, in);
  }
}

WPI_UPB_API_INLINE void wpi_upb_Message_ClearBaseField(struct wpi_upb_Message* msg,
                                               const wpi_upb_MiniTableField* f) {
  WPI_UPB_ASSERT(!wpi_upb_Message_IsFrozen(msg));
  if (WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_HasHasbit)(f)) {
    WPI_UPB_PRIVATE(_wpi_upb_Message_ClearHasbit)(msg, f);
  } else if (wpi_upb_MiniTableField_IsInOneof(f)) {
    uint32_t* ptr = WPI_UPB_PRIVATE(_wpi_upb_Message_OneofCasePtr)(msg, f);
    if (*ptr != wpi_upb_MiniTableField_Number(f)) return;
    *ptr = 0;
  }
  const char zeros[16] = {0};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_DataCopy)
  (f, WPI_UPB_PRIVATE(_wpi_upb_Message_MutableDataPtr)(msg, f), zeros);
}

WPI_UPB_API_INLINE void wpi_upb_Message_ClearExtension(
    struct wpi_upb_Message* msg, const wpi_upb_MiniTableExtension* e) {
  WPI_UPB_ASSERT(!wpi_upb_Message_IsFrozen(msg));
  wpi_upb_Message_Internal* in = WPI_UPB_PRIVATE(_wpi_upb_Message_GetInternal)(msg);
  if (!in) return;
  const wpi_upb_Extension* base = WPI_UPB_PTR_AT(in, in->ext_begin, wpi_upb_Extension);
  wpi_upb_Extension* ext = (wpi_upb_Extension*)WPI_UPB_PRIVATE(_wpi_upb_Message_Getext)(msg, e);
  if (ext) {
    *ext = *base;
    in->ext_begin += sizeof(wpi_upb_Extension);
  }
}

WPI_UPB_API_INLINE void wpi_upb_Message_ClearOneof(struct wpi_upb_Message* msg,
                                           const wpi_upb_MiniTable* m,
                                           const wpi_upb_MiniTableField* f) {
  WPI_UPB_ASSERT(!wpi_upb_Message_IsFrozen(msg));
  uint32_t field_number = wpi_upb_Message_WhichOneofFieldNumber(msg, f);
  if (field_number == 0) {
    // No field in the oneof is set.
    return;
  }

  const wpi_upb_MiniTableField* field =
      wpi_upb_MiniTable_FindFieldByNumber(m, field_number);
  wpi_upb_Message_ClearBaseField(msg, field);
}

WPI_UPB_API_INLINE void* wpi_upb_Message_ResizeArrayUninitialized(
    struct wpi_upb_Message* msg, const wpi_upb_MiniTableField* f, size_t size,
    wpi_upb_Arena* arena) {
  WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_CheckIsArray)(f);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(msg, f, arena);
  if (!arr || !WPI_UPB_PRIVATE(_wpi_upb_Array_ResizeUninitialized)(arr, size, arena)) {
    return NULL;
  }
  return wpi_upb_Array_MutableDataPtr(arr);
}

#ifdef __cplusplus
} /* extern "C" */
#endif

#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif


#endif  // WPI_UPB_MESSAGE_INTERNAL_ACCESSORS_H_

#ifndef WPI_UPB_MESSAGE_MAP_H_
#define WPI_UPB_MESSAGE_MAP_H_

#include <stddef.h>


// Must be last.

typedef struct wpi_upb_Map wpi_upb_Map;

#ifdef __cplusplus
extern "C" {
#endif

// Creates a new map on the given arena with the given key/value size.
WPI_UPB_API wpi_upb_Map* wpi_upb_Map_New(wpi_upb_Arena* a, wpi_upb_CType key_type,
                             wpi_upb_CType value_type);

// Returns the number of entries in the map.
WPI_UPB_API size_t wpi_upb_Map_Size(const wpi_upb_Map* map);

// Stores a value for the given key into |*val| (or the zero value if the key is
// not present). Returns whether the key was present. The |val| pointer may be
// NULL, in which case the function tests whether the given key is present.
WPI_UPB_API bool wpi_upb_Map_Get(const wpi_upb_Map* map, wpi_upb_MessageValue key,
                         wpi_upb_MessageValue* val);

// Removes all entries in the map.
WPI_UPB_API void wpi_upb_Map_Clear(wpi_upb_Map* map);

// Sets the given key to the given value, returning whether the key was inserted
// or replaced. If the key was inserted, then any existing iterators will be
// invalidated.
WPI_UPB_API wpi_upb_MapInsertStatus wpi_upb_Map_Insert(wpi_upb_Map* map, wpi_upb_MessageValue key,
                                           wpi_upb_MessageValue val,
                                           wpi_upb_Arena* arena);

// Sets the given key to the given value. Returns false if memory allocation
// failed. If the key is newly inserted, then any existing iterators will be
// invalidated.
WPI_UPB_API_INLINE bool wpi_upb_Map_Set(wpi_upb_Map* map, wpi_upb_MessageValue key,
                                wpi_upb_MessageValue val, wpi_upb_Arena* arena) {
  return wpi_upb_Map_Insert(map, key, val, arena) !=
         kUpb_MapInsertStatus_OutOfMemory;
}

// Deletes this key from the table. Returns true if the key was present.
// If present and |val| is non-NULL, stores the deleted value.
WPI_UPB_API bool wpi_upb_Map_Delete(wpi_upb_Map* map, wpi_upb_MessageValue key,
                            wpi_upb_MessageValue* val);

// Map iteration:
//
// size_t iter = kUpb_Map_Begin;
// wpi_upb_MessageValue key, val;
// while (wpi_upb_Map_Next(map, &key, &val, &iter)) {
//   ...
// }

#define kUpb_Map_Begin ((size_t) - 1)

// Advances to the next entry. Returns false if no more entries are present.
// Otherwise returns true and populates both *key and *value.
WPI_UPB_API bool wpi_upb_Map_Next(const wpi_upb_Map* map, wpi_upb_MessageValue* key,
                          wpi_upb_MessageValue* val, size_t* iter);

// Sets the value for the entry pointed to by iter.
// WARNING: this does not currently work for string values!
WPI_UPB_API void wpi_upb_Map_SetEntryValue(wpi_upb_Map* map, size_t iter,
                                   wpi_upb_MessageValue val);

// DEPRECATED iterator, slated for removal.

/* Map iteration:
 *
 * size_t iter = kUpb_Map_Begin;
 * while (wpi_upb_MapIterator_Next(map, &iter)) {
 *   wpi_upb_MessageValue key = wpi_upb_MapIterator_Key(map, iter);
 *   wpi_upb_MessageValue val = wpi_upb_MapIterator_Value(map, iter);
 * }
 */

// Advances to the next entry. Returns false if no more entries are present.
WPI_UPB_API bool wpi_upb_MapIterator_Next(const wpi_upb_Map* map, size_t* iter);

// Returns true if the iterator still points to a valid entry, or false if the
// iterator is past the last element. It is an error to call this function with
// kUpb_Map_Begin (you must call next() at least once first).
WPI_UPB_API bool wpi_upb_MapIterator_Done(const wpi_upb_Map* map, size_t iter);

// Returns the key and value for this entry of the map.
WPI_UPB_API wpi_upb_MessageValue wpi_upb_MapIterator_Key(const wpi_upb_Map* map, size_t iter);
WPI_UPB_API wpi_upb_MessageValue wpi_upb_MapIterator_Value(const wpi_upb_Map* map, size_t iter);

// Mark a map and all of its descendents as frozen/immutable.
// If the map values are messages then |m| must point to the minitable for
// those messages. Otherwise |m| must be NULL.
WPI_UPB_API void wpi_upb_Map_Freeze(wpi_upb_Map* map, const wpi_upb_MiniTable* m);

// Returns whether a map has been frozen.
WPI_UPB_API_INLINE bool wpi_upb_Map_IsFrozen(const wpi_upb_Map* map);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_MESSAGE_MAP_H_ */

// Public APIs for message operations that do not depend on the schema.
//
// MiniTable-based accessors live in accessors.h.

#ifndef WPI_UPB_MESSAGE_MESSAGE_H_
#define WPI_UPB_MESSAGE_MESSAGE_H_

#include <stddef.h>


// Must be last.

typedef struct wpi_upb_Message wpi_upb_Message;

#ifdef __cplusplus
extern "C" {
#endif

// Creates a new message with the given mini_table on the given arena.
WPI_UPB_API wpi_upb_Message* wpi_upb_Message_New(const wpi_upb_MiniTable* m, wpi_upb_Arena* arena);

// Returns a reference to the message's unknown data.
const char* wpi_upb_Message_GetUnknown(const wpi_upb_Message* msg, size_t* len);

// Removes partial unknown data from message.
void wpi_upb_Message_DeleteUnknown(wpi_upb_Message* msg, const char* data, size_t len);

// Returns the number of extensions present in this message.
size_t wpi_upb_Message_ExtensionCount(const wpi_upb_Message* msg);

// Mark a message and all of its descendents as frozen/immutable.
WPI_UPB_API void wpi_upb_Message_Freeze(wpi_upb_Message* msg, const wpi_upb_MiniTable* m);

// Returns whether a message has been frozen.
WPI_UPB_API_INLINE bool wpi_upb_Message_IsFrozen(const wpi_upb_Message* msg);

#ifdef WPI_UPB_TRACING_ENABLED
WPI_UPB_API void wpi_upb_Message_LogNewMessage(const wpi_upb_MiniTable* m,
                                       const wpi_upb_Arena* arena);

WPI_UPB_API void wpi_upb_Message_SetNewMessageTraceHandler(
    void (*handler)(const wpi_upb_MiniTable* m, const wpi_upb_Arena* arena));
#endif  // WPI_UPB_TRACING_ENABLED

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_MESSAGE_MESSAGE_H_ */

#ifndef WPI_UPB_MINI_TABLE_TAGGED_PTR_H_
#define WPI_UPB_MINI_TABLE_TAGGED_PTR_H_

#include <stdint.h>


// Must be last.

// When a wpi_upb_Message* is stored in a message, array, or map, it is stored in a
// tagged form. If the tag bit is set, the referenced wpi_upb_Message is of type
// _kUpb_MiniTable_Empty (a sentinel message type with no fields) instead of
// that field's true message type. This forms the basis of what we call
// "dynamic tree shaking."
//
// See the documentation for kUpb_DecodeOption_ExperimentalAllowUnlinked for
// more information.

typedef uintptr_t wpi_upb_TaggedMessagePtr;

#ifdef __cplusplus
extern "C" {
#endif

// Users who enable unlinked sub-messages must use this to test whether a
// message is empty before accessing it. If a message is empty, it must be
// first promoted using the interfaces in message/promote.h.
WPI_UPB_API_INLINE bool wpi_upb_TaggedMessagePtr_IsEmpty(wpi_upb_TaggedMessagePtr ptr);

WPI_UPB_API_INLINE wpi_upb_Message* wpi_upb_TaggedMessagePtr_GetNonEmptyMessage(
    wpi_upb_TaggedMessagePtr ptr);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_MINI_TABLE_TAGGED_PTR_H_ */

// Must be last.

#ifdef __cplusplus
extern "C" {
#endif

// Functions ending in BaseField() take a (wpi_upb_MiniTableField*) argument
// and work only on non-extension fields.
//
// Functions ending in Extension() take a (wpi_upb_MiniTableExtension*) argument
// and work only on extensions.

WPI_UPB_API_INLINE void wpi_upb_Message_Clear(wpi_upb_Message* msg, const wpi_upb_MiniTable* m);

WPI_UPB_API_INLINE void wpi_upb_Message_ClearBaseField(wpi_upb_Message* msg,
                                               const wpi_upb_MiniTableField* f);

WPI_UPB_API_INLINE void wpi_upb_Message_ClearExtension(wpi_upb_Message* msg,
                                               const wpi_upb_MiniTableExtension* e);

WPI_UPB_API_INLINE void wpi_upb_Message_ClearOneof(wpi_upb_Message* msg,
                                           const wpi_upb_MiniTable* m,
                                           const wpi_upb_MiniTableField* f);

WPI_UPB_API_INLINE bool wpi_upb_Message_HasBaseField(const wpi_upb_Message* msg,
                                             const wpi_upb_MiniTableField* f);

WPI_UPB_API_INLINE bool wpi_upb_Message_HasExtension(const wpi_upb_Message* msg,
                                             const wpi_upb_MiniTableExtension* e);

WPI_UPB_API_INLINE wpi_upb_MessageValue
wpi_upb_Message_GetField(const wpi_upb_Message* msg, const wpi_upb_MiniTableField* f,
                     wpi_upb_MessageValue default_val);

WPI_UPB_API_INLINE wpi_upb_TaggedMessagePtr wpi_upb_Message_GetTaggedMessagePtr(
    const wpi_upb_Message* msg, const wpi_upb_MiniTableField* field,
    wpi_upb_Message* default_val);

WPI_UPB_API_INLINE const wpi_upb_Array* wpi_upb_Message_GetArray(
    const wpi_upb_Message* msg, const wpi_upb_MiniTableField* f);

WPI_UPB_API_INLINE bool wpi_upb_Message_GetBool(const wpi_upb_Message* msg,
                                        const wpi_upb_MiniTableField* f,
                                        bool default_val);

WPI_UPB_API_INLINE double wpi_upb_Message_GetDouble(const wpi_upb_Message* msg,
                                            const wpi_upb_MiniTableField* field,
                                            double default_val);

WPI_UPB_API_INLINE float wpi_upb_Message_GetFloat(const wpi_upb_Message* msg,
                                          const wpi_upb_MiniTableField* f,
                                          float default_val);

WPI_UPB_API_INLINE int32_t wpi_upb_Message_GetInt32(const wpi_upb_Message* msg,
                                            const wpi_upb_MiniTableField* f,
                                            int32_t default_val);

WPI_UPB_API_INLINE int64_t wpi_upb_Message_GetInt64(const wpi_upb_Message* msg,
                                            const wpi_upb_MiniTableField* f,
                                            int64_t default_val);

WPI_UPB_API_INLINE const wpi_upb_Map* wpi_upb_Message_GetMap(const wpi_upb_Message* msg,
                                                 const wpi_upb_MiniTableField* f);

WPI_UPB_API_INLINE const wpi_upb_Message* wpi_upb_Message_GetMessage(
    const wpi_upb_Message* msg, const wpi_upb_MiniTableField* f);

WPI_UPB_API_INLINE wpi_upb_Array* wpi_upb_Message_GetMutableArray(
    wpi_upb_Message* msg, const wpi_upb_MiniTableField* f);

WPI_UPB_API_INLINE wpi_upb_Map* wpi_upb_Message_GetMutableMap(wpi_upb_Message* msg,
                                                  const wpi_upb_MiniTableField* f);

WPI_UPB_API_INLINE wpi_upb_Message* wpi_upb_Message_GetMutableMessage(
    wpi_upb_Message* msg, const wpi_upb_MiniTableField* f);

WPI_UPB_API_INLINE wpi_upb_Array* wpi_upb_Message_GetOrCreateMutableArray(
    wpi_upb_Message* msg, const wpi_upb_MiniTableField* f, wpi_upb_Arena* arena);

WPI_UPB_API_INLINE wpi_upb_Map* wpi_upb_Message_GetOrCreateMutableMap(
    wpi_upb_Message* msg, const wpi_upb_MiniTable* map_entry_mini_table,
    const wpi_upb_MiniTableField* f, wpi_upb_Arena* arena);

WPI_UPB_API_INLINE wpi_upb_Message* wpi_upb_Message_GetOrCreateMutableMessage(
    wpi_upb_Message* msg, const wpi_upb_MiniTable* mini_table,
    const wpi_upb_MiniTableField* f, wpi_upb_Arena* arena);

WPI_UPB_API_INLINE wpi_upb_StringView
wpi_upb_Message_GetString(const wpi_upb_Message* msg, const wpi_upb_MiniTableField* field,
                      wpi_upb_StringView default_val);

WPI_UPB_API_INLINE uint32_t wpi_upb_Message_GetUInt32(const wpi_upb_Message* msg,
                                              const wpi_upb_MiniTableField* f,
                                              uint32_t default_val);

WPI_UPB_API_INLINE uint64_t wpi_upb_Message_GetUInt64(const wpi_upb_Message* msg,
                                              const wpi_upb_MiniTableField* f,
                                              uint64_t default_val);

WPI_UPB_API_INLINE void wpi_upb_Message_SetClosedEnum(
    wpi_upb_Message* msg, const wpi_upb_MiniTable* msg_mini_table,
    const wpi_upb_MiniTableField* f, int32_t value);

// BaseField Setters ///////////////////////////////////////////////////////////

WPI_UPB_API_INLINE void wpi_upb_Message_SetBaseField(wpi_upb_Message* msg,
                                             const wpi_upb_MiniTableField* f,
                                             const void* val);

WPI_UPB_API_INLINE void wpi_upb_Message_SetBaseFieldBool(struct wpi_upb_Message* msg,
                                                 const wpi_upb_MiniTableField* f,
                                                 bool value);

WPI_UPB_API_INLINE void wpi_upb_Message_SetBaseFieldDouble(struct wpi_upb_Message* msg,
                                                   const wpi_upb_MiniTableField* f,
                                                   double value);

WPI_UPB_API_INLINE void wpi_upb_Message_SetBaseFieldFloat(struct wpi_upb_Message* msg,
                                                  const wpi_upb_MiniTableField* f,
                                                  float value);

WPI_UPB_API_INLINE void wpi_upb_Message_SetBaseFieldInt32(struct wpi_upb_Message* msg,
                                                  const wpi_upb_MiniTableField* f,
                                                  int32_t value);

WPI_UPB_API_INLINE void wpi_upb_Message_SetBaseFieldInt64(struct wpi_upb_Message* msg,
                                                  const wpi_upb_MiniTableField* f,
                                                  int64_t value);

WPI_UPB_API_INLINE void wpi_upb_Message_SetBaseFieldString(struct wpi_upb_Message* msg,
                                                   const wpi_upb_MiniTableField* f,
                                                   wpi_upb_StringView value);

WPI_UPB_API_INLINE void wpi_upb_Message_SetBaseFieldUInt32(struct wpi_upb_Message* msg,
                                                   const wpi_upb_MiniTableField* f,
                                                   uint32_t value);

WPI_UPB_API_INLINE void wpi_upb_Message_SetBaseFieldUInt64(struct wpi_upb_Message* msg,
                                                   const wpi_upb_MiniTableField* f,
                                                   uint64_t value);

// Extension Setters ///////////////////////////////////////////////////////////

WPI_UPB_API_INLINE bool wpi_upb_Message_SetExtension(wpi_upb_Message* msg,
                                             const wpi_upb_MiniTableExtension* e,
                                             const void* value, wpi_upb_Arena* a);

WPI_UPB_API_INLINE bool wpi_upb_Message_SetExtensionBool(
    struct wpi_upb_Message* msg, const wpi_upb_MiniTableExtension* e, bool value,
    wpi_upb_Arena* a);

WPI_UPB_API_INLINE bool wpi_upb_Message_SetExtensionDouble(
    struct wpi_upb_Message* msg, const wpi_upb_MiniTableExtension* e, double value,
    wpi_upb_Arena* a);

WPI_UPB_API_INLINE bool wpi_upb_Message_SetExtensionFloat(
    struct wpi_upb_Message* msg, const wpi_upb_MiniTableExtension* e, float value,
    wpi_upb_Arena* a);

WPI_UPB_API_INLINE bool wpi_upb_Message_SetExtensionInt32(
    struct wpi_upb_Message* msg, const wpi_upb_MiniTableExtension* e, int32_t value,
    wpi_upb_Arena* a);

WPI_UPB_API_INLINE bool wpi_upb_Message_SetExtensionInt64(
    struct wpi_upb_Message* msg, const wpi_upb_MiniTableExtension* e, int64_t value,
    wpi_upb_Arena* a);

WPI_UPB_API_INLINE bool wpi_upb_Message_SetExtensionString(
    struct wpi_upb_Message* msg, const wpi_upb_MiniTableExtension* e,
    wpi_upb_StringView value, wpi_upb_Arena* a);

WPI_UPB_API_INLINE bool wpi_upb_Message_SetExtensionUInt32(
    struct wpi_upb_Message* msg, const wpi_upb_MiniTableExtension* e, uint32_t value,
    wpi_upb_Arena* a);

WPI_UPB_API_INLINE bool wpi_upb_Message_SetExtensionUInt64(
    struct wpi_upb_Message* msg, const wpi_upb_MiniTableExtension* e, uint64_t value,
    wpi_upb_Arena* a);

// Universal Setters ///////////////////////////////////////////////////////////

WPI_UPB_API_INLINE bool wpi_upb_Message_SetBool(wpi_upb_Message* msg,
                                        const wpi_upb_MiniTableField* f, bool value,
                                        wpi_upb_Arena* a);

WPI_UPB_API_INLINE bool wpi_upb_Message_SetDouble(wpi_upb_Message* msg,
                                          const wpi_upb_MiniTableField* f,
                                          double value, wpi_upb_Arena* a);

WPI_UPB_API_INLINE bool wpi_upb_Message_SetFloat(wpi_upb_Message* msg,
                                         const wpi_upb_MiniTableField* f,
                                         float value, wpi_upb_Arena* a);

WPI_UPB_API_INLINE bool wpi_upb_Message_SetInt32(wpi_upb_Message* msg,
                                         const wpi_upb_MiniTableField* f,
                                         int32_t value, wpi_upb_Arena* a);

WPI_UPB_API_INLINE bool wpi_upb_Message_SetInt64(wpi_upb_Message* msg,
                                         const wpi_upb_MiniTableField* f,
                                         int64_t value, wpi_upb_Arena* a);

WPI_UPB_API_INLINE void wpi_upb_Message_SetMessage(wpi_upb_Message* msg,
                                           const wpi_upb_MiniTableField* f,
                                           wpi_upb_Message* value);

WPI_UPB_API_INLINE bool wpi_upb_Message_SetString(wpi_upb_Message* msg,
                                          const wpi_upb_MiniTableField* f,
                                          wpi_upb_StringView value, wpi_upb_Arena* a);

WPI_UPB_API_INLINE bool wpi_upb_Message_SetUInt32(wpi_upb_Message* msg,
                                          const wpi_upb_MiniTableField* f,
                                          uint32_t value, wpi_upb_Arena* a);

WPI_UPB_API_INLINE bool wpi_upb_Message_SetUInt64(wpi_upb_Message* msg,
                                          const wpi_upb_MiniTableField* f,
                                          uint64_t value, wpi_upb_Arena* a);

////////////////////////////////////////////////////////////////////////////////

WPI_UPB_API_INLINE void* wpi_upb_Message_ResizeArrayUninitialized(
    wpi_upb_Message* msg, const wpi_upb_MiniTableField* f, size_t size,
    wpi_upb_Arena* arena);

WPI_UPB_API_INLINE uint32_t wpi_upb_Message_WhichOneofFieldNumber(
    const wpi_upb_Message* message, const wpi_upb_MiniTableField* oneof_field);

// For a field `f` which is in a oneof, return the field of that
// oneof that is actually set (or NULL if none).
WPI_UPB_API_INLINE const wpi_upb_MiniTableField* wpi_upb_Message_WhichOneof(
    const wpi_upb_Message* msg, const wpi_upb_MiniTable* m,
    const wpi_upb_MiniTableField* f);

// Updates a map entry given an entry message.
bool wpi_upb_Message_SetMapEntry(wpi_upb_Map* map, const wpi_upb_MiniTable* mini_table,
                             const wpi_upb_MiniTableField* field,
                             wpi_upb_Message* map_entry_message, wpi_upb_Arena* arena);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif  // WPI_UPB_MESSAGE_ACCESSORS_H_

// These functions are only used by generated code.

#ifndef WPI_UPB_MESSAGE_MAP_GENCODE_UTIL_H_
#define WPI_UPB_MESSAGE_MAP_GENCODE_UTIL_H_


// Must be last.

#ifdef __cplusplus
extern "C" {
#endif

// Message map operations, these get the map from the message first.

WPI_UPB_INLINE void _wpi_upb_msg_map_key(const void* msg, void* key, size_t size) {
  const wpi_upb_tabent* ent = (const wpi_upb_tabent*)msg;
  uint32_t u32len;
  wpi_upb_StringView k;
  k.data = wpi_upb_tabstr(ent->key, &u32len);
  k.size = u32len;
  _wpi_upb_map_fromkey(k, key, size);
}

WPI_UPB_INLINE void _wpi_upb_msg_map_value(const void* msg, void* val, size_t size) {
  const wpi_upb_tabent* ent = (const wpi_upb_tabent*)msg;
  wpi_upb_value v = {ent->val.val};
  _wpi_upb_map_fromvalue(v, val, size);
}

WPI_UPB_INLINE void _wpi_upb_msg_map_set_value(void* msg, const void* val,
                                       size_t size) {
  wpi_upb_tabent* ent = (wpi_upb_tabent*)msg;
  // This is like _wpi_upb_map_tovalue() except the entry already exists
  // so we can reuse the allocated wpi_upb_StringView for string fields.
  if (size == WPI_UPB_MAPTYPE_STRING) {
    wpi_upb_StringView* strp = (wpi_upb_StringView*)(uintptr_t)ent->val.val;
    memcpy(strp, val, sizeof(*strp));
  } else {
    memcpy(&ent->val.val, val, size);
  }
}

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_MESSAGE_MAP_GENCODE_UTIL_H_ */

#ifndef WPI_UPB_MINI_TABLE_DECODE_H_
#define WPI_UPB_MINI_TABLE_DECODE_H_


#ifndef WPI_UPB_MINI_TABLE_SUB_H_
#define WPI_UPB_MINI_TABLE_SUB_H_


// Must be last.

typedef union wpi_upb_MiniTableSub wpi_upb_MiniTableSub;

#ifdef __cplusplus
extern "C" {
#endif

// Constructors

WPI_UPB_API_INLINE wpi_upb_MiniTableSub
wpi_upb_MiniTableSub_FromEnum(const wpi_upb_MiniTableEnum* subenum);

WPI_UPB_API_INLINE wpi_upb_MiniTableSub
wpi_upb_MiniTableSub_FromMessage(const wpi_upb_MiniTable* submsg);

// Getters

WPI_UPB_API_INLINE const wpi_upb_MiniTableEnum* wpi_upb_MiniTableSub_Enum(
    wpi_upb_MiniTableSub sub);

WPI_UPB_API_INLINE const wpi_upb_MiniTable* wpi_upb_MiniTableSub_Message(
    wpi_upb_MiniTableSub sub);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_MINI_TABLE_SUB_H_ */

// Export the newer headers, for legacy users.  New users should include the
// more specific headers directly.
// IWYU pragma: begin_exports

#ifndef WPI_UPB_MINI_DESCRIPTOR_BUILD_ENUM_H_
#define WPI_UPB_MINI_DESCRIPTOR_BUILD_ENUM_H_


// Must be last.

#ifdef __cplusplus
extern "C" {
#endif

// Builds a wpi_upb_MiniTableEnum from an enum mini descriptor.
// The mini descriptor must be for an enum, not a message.
WPI_UPB_API wpi_upb_MiniTableEnum* wpi_upb_MiniTableEnum_Build(const char* data, size_t len,
                                                   wpi_upb_Arena* arena,
                                                   wpi_upb_Status* status);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif  // WPI_UPB_MINI_DESCRIPTOR_BUILD_ENUM_H_

// Functions for linking MiniTables together once they are built from a
// MiniDescriptor.
//
// These functions have names like wpi_upb_MiniTable_Link() because they operate on
// MiniTables.  We put them here, rather than in the mini_table/ directory,
// because they are only needed when building MiniTables from MiniDescriptors.
// The interfaces in mini_table/ assume that MiniTables are immutable.

#ifndef WPI_UPB_MINI_DESCRIPTOR_LINK_H_
#define WPI_UPB_MINI_DESCRIPTOR_LINK_H_


// Must be last.

#ifdef __cplusplus
extern "C" {
#endif

// Links a sub-message field to a MiniTable for that sub-message. If a
// sub-message field is not linked, it will be treated as an unknown field
// during parsing, and setting the field will not be allowed. It is possible
// to link the message field later, at which point it will no longer be treated
// as unknown. However there is no synchronization for this operation, which
// means parallel mutation requires external synchronization.
// Returns success/failure.
WPI_UPB_API bool wpi_upb_MiniTable_SetSubMessage(wpi_upb_MiniTable* table,
                                         wpi_upb_MiniTableField* field,
                                         const wpi_upb_MiniTable* sub);

// Links an enum field to a MiniTable for that enum.
// All enum fields must be linked prior to parsing.
// Returns success/failure.
WPI_UPB_API bool wpi_upb_MiniTable_SetSubEnum(wpi_upb_MiniTable* table,
                                      wpi_upb_MiniTableField* field,
                                      const wpi_upb_MiniTableEnum* sub);

// Returns a list of fields that require linking at runtime, to connect the
// MiniTable to its sub-messages and sub-enums.  The list of fields will be
// written to the `subs` array, which must have been allocated by the caller
// and must be large enough to hold a list of all fields in the message.
//
// The order of the fields returned by this function is significant: it matches
// the order expected by wpi_upb_MiniTable_Link() below.
//
// The return value packs the sub-message count and sub-enum count into a single
// integer like so:
//  return (msg_count << 16) | enum_count;
WPI_UPB_API uint32_t wpi_upb_MiniTable_GetSubList(const wpi_upb_MiniTable* mt,
                                          const wpi_upb_MiniTableField** subs);

// Links a message to its sub-messages and sub-enums.  The caller must pass
// arrays of sub-tables and sub-enums, in the same length and order as is
// returned by wpi_upb_MiniTable_GetSubList() above.  However, individual elements
// of the sub_tables may be NULL if those sub-messages were tree shaken.
//
// Returns false if either array is too short, or if any of the tables fails
// to link.
WPI_UPB_API bool wpi_upb_MiniTable_Link(wpi_upb_MiniTable* mt,
                                const wpi_upb_MiniTable** sub_tables,
                                size_t sub_table_count,
                                const wpi_upb_MiniTableEnum** sub_enums,
                                size_t sub_enum_count);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif  // WPI_UPB_MINI_DESCRIPTOR_LINK_H_
// IWYU pragma: end_exports

// Must be last.

typedef enum {
  kUpb_MiniTablePlatform_32Bit,
  kUpb_MiniTablePlatform_64Bit,
  kUpb_MiniTablePlatform_Native =
      WPI_UPB_SIZE(kUpb_MiniTablePlatform_32Bit, kUpb_MiniTablePlatform_64Bit),
} wpi_upb_MiniTablePlatform;

#ifdef __cplusplus
extern "C" {
#endif

// Builds a mini table from the data encoded in the buffer [data, len]. If any
// errors occur, returns NULL and sets a status message. In the success case,
// the caller must call wpi_upb_MiniTable_SetSub*() for all message or proto2 enum
// fields to link the table to the appropriate sub-tables.
wpi_upb_MiniTable* _wpi_upb_MiniTable_Build(const char* data, size_t len,
                                    wpi_upb_MiniTablePlatform platform,
                                    wpi_upb_Arena* arena, wpi_upb_Status* status);

WPI_UPB_API_INLINE wpi_upb_MiniTable* wpi_upb_MiniTable_Build(const char* data, size_t len,
                                                  wpi_upb_Arena* arena,
                                                  wpi_upb_Status* status) {
  return _wpi_upb_MiniTable_Build(data, len, kUpb_MiniTablePlatform_Native, arena,
                              status);
}

// Initializes a MiniTableExtension buffer that has already been allocated.
// This is needed by wpi_upb_FileDef and wpi_upb_MessageDef, which allocate all of the
// extensions together in a single contiguous array.
const char* _wpi_upb_MiniTableExtension_Init(const char* data, size_t len,
                                         wpi_upb_MiniTableExtension* ext,
                                         const wpi_upb_MiniTable* extendee,
                                         wpi_upb_MiniTableSub sub,
                                         wpi_upb_MiniTablePlatform platform,
                                         wpi_upb_Status* status);

WPI_UPB_API_INLINE const char* wpi_upb_MiniTableExtension_Init(
    const char* data, size_t len, wpi_upb_MiniTableExtension* ext,
    const wpi_upb_MiniTable* extendee, wpi_upb_MiniTableSub sub, wpi_upb_Status* status) {
  return _wpi_upb_MiniTableExtension_Init(data, len, ext, extendee, sub,
                                      kUpb_MiniTablePlatform_Native, status);
}

WPI_UPB_API wpi_upb_MiniTableExtension* _wpi_upb_MiniTableExtension_Build(
    const char* data, size_t len, const wpi_upb_MiniTable* extendee,
    wpi_upb_MiniTableSub sub, wpi_upb_MiniTablePlatform platform, wpi_upb_Arena* arena,
    wpi_upb_Status* status);

WPI_UPB_API_INLINE wpi_upb_MiniTableExtension* wpi_upb_MiniTableExtension_Build(
    const char* data, size_t len, const wpi_upb_MiniTable* extendee,
    wpi_upb_Arena* arena, wpi_upb_Status* status) {
  wpi_upb_MiniTableSub sub = wpi_upb_MiniTableSub_FromMessage(NULL);
  return _wpi_upb_MiniTableExtension_Build(
      data, len, extendee, sub, kUpb_MiniTablePlatform_Native, arena, status);
}

WPI_UPB_API_INLINE wpi_upb_MiniTableExtension* wpi_upb_MiniTableExtension_BuildMessage(
    const char* data, size_t len, const wpi_upb_MiniTable* extendee,
    wpi_upb_MiniTable* submsg, wpi_upb_Arena* arena, wpi_upb_Status* status) {
  wpi_upb_MiniTableSub sub = wpi_upb_MiniTableSub_FromMessage(submsg);
  return _wpi_upb_MiniTableExtension_Build(
      data, len, extendee, sub, kUpb_MiniTablePlatform_Native, arena, status);
}

WPI_UPB_API_INLINE wpi_upb_MiniTableExtension* wpi_upb_MiniTableExtension_BuildEnum(
    const char* data, size_t len, const wpi_upb_MiniTable* extendee,
    wpi_upb_MiniTableEnum* subenum, wpi_upb_Arena* arena, wpi_upb_Status* status) {
  wpi_upb_MiniTableSub sub = wpi_upb_MiniTableSub_FromEnum(subenum);
  return _wpi_upb_MiniTableExtension_Build(
      data, len, extendee, sub, kUpb_MiniTablePlatform_Native, arena, status);
}

// Like wpi_upb_MiniTable_Build(), but the user provides a buffer of layout data so
// it can be reused from call to call, avoiding repeated realloc()/free().
//
// The caller owns `*buf` both before and after the call, and must free() it
// when it is no longer in use.  The function will realloc() `*buf` as
// necessary, updating `*size` accordingly.
wpi_upb_MiniTable* wpi_upb_MiniTable_BuildWithBuf(const char* data, size_t len,
                                          wpi_upb_MiniTablePlatform platform,
                                          wpi_upb_Arena* arena, void** buf,
                                          size_t* buf_size, wpi_upb_Status* status);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_MINI_TABLE_DECODE_H_ */

#ifndef WPI_UPB_MINI_TABLE_EXTENSION_REGISTRY_H_
#define WPI_UPB_MINI_TABLE_EXTENSION_REGISTRY_H_


// Must be last.

#ifdef __cplusplus
extern "C" {
#endif

/* Extension registry: a dynamic data structure that stores a map of:
 *   (wpi_upb_MiniTable, number) -> extension info
 *
 * wpi_upb_decode() uses wpi_upb_ExtensionRegistry to look up extensions while parsing
 * binary format.
 *
 * wpi_upb_ExtensionRegistry is part of the mini-table (msglayout) family of
 * objects. Like all mini-table objects, it is suitable for reflection-less
 * builds that do not want to expose names into the binary.
 *
 * Unlike most mini-table types, wpi_upb_ExtensionRegistry requires dynamic memory
 * allocation and dynamic initialization:
 * * If reflection is being used, then wpi_upb_DefPool will construct an appropriate
 *   wpi_upb_ExtensionRegistry automatically.
 * * For a mini-table only build, the user must manually construct the
 *   wpi_upb_ExtensionRegistry and populate it with all of the extensions the user
 * cares about.
 * * A third alternative is to manually unpack relevant extensions after the
 *   main parse is complete, similar to how Any works. This is perhaps the
 *   nicest solution from the perspective of reducing dependencies, avoiding
 *   dynamic memory allocation, and avoiding the need to parse uninteresting
 *   extensions.  The downsides are:
 *     (1) parse errors are not caught during the main parse
 *     (2) the CPU hit of parsing comes during access, which could cause an
 *         undesirable stutter in application performance.
 *
 * Users cannot directly get or put into this map. Users can only add the
 * extensions from a generated module and pass the extension registry to the
 * binary decoder.
 *
 * A wpi_upb_DefPool provides a wpi_upb_ExtensionRegistry, so any users who use
 * reflection do not need to populate a wpi_upb_ExtensionRegistry directly.
 */

typedef struct wpi_upb_ExtensionRegistry wpi_upb_ExtensionRegistry;

// Creates a wpi_upb_ExtensionRegistry in the given arena.
// The arena must outlive any use of the extreg.
WPI_UPB_API wpi_upb_ExtensionRegistry* wpi_upb_ExtensionRegistry_New(wpi_upb_Arena* arena);

WPI_UPB_API bool wpi_upb_ExtensionRegistry_Add(wpi_upb_ExtensionRegistry* r,
                                       const wpi_upb_MiniTableExtension* e);

// Adds the given extension info for the array |e| of size |count| into the
// registry. If there are any errors, the entire array is backed out.
// The extensions must outlive the registry.
// Possible errors include OOM or an extension number that already exists.
// TODO: There is currently no way to know the exact reason for failure.
bool wpi_upb_ExtensionRegistry_AddArray(wpi_upb_ExtensionRegistry* r,
                                    const wpi_upb_MiniTableExtension** e,
                                    size_t count);

#ifdef WPI_UPB_LINKARR_DECLARE

// Adds all extensions linked into the binary into the registry.  The set of
// linked extensions is assembled by the linker using linker arrays.  This
// will likely not work properly if the extensions are split across multiple
// shared libraries.
//
// Returns true if all extensions were added successfully, false on out of
// memory or if any extensions were already present.
//
// This API is currently not available on MSVC (though it *is* available on
// Windows using clang-cl).
WPI_UPB_API bool wpi_upb_ExtensionRegistry_AddAllLinkedExtensions(
    wpi_upb_ExtensionRegistry* r);

#endif  // WPI_UPB_LINKARR_DECLARE

// Looks up the extension (if any) defined for message type |t| and field
// number |num|. Returns the extension if found, otherwise NULL.
WPI_UPB_API const wpi_upb_MiniTableExtension* wpi_upb_ExtensionRegistry_Lookup(
    const wpi_upb_ExtensionRegistry* r, const wpi_upb_MiniTable* t, uint32_t num);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_MINI_TABLE_EXTENSION_REGISTRY_H_ */

#ifndef WPI_UPB_MINI_TABLE_FILE_H_
#define WPI_UPB_MINI_TABLE_FILE_H_


#ifndef WPI_UPB_MINI_TABLE_INTERNAL_FILE_H_
#define WPI_UPB_MINI_TABLE_INTERNAL_FILE_H_

// Must be last.

struct wpi_upb_MiniTableFile {
  const struct wpi_upb_MiniTable** WPI_UPB_PRIVATE(msgs);
  const struct wpi_upb_MiniTableEnum** WPI_UPB_PRIVATE(enums);
  const struct wpi_upb_MiniTableExtension** WPI_UPB_PRIVATE(exts);
  int WPI_UPB_PRIVATE(msg_count);
  int WPI_UPB_PRIVATE(enum_count);
  int WPI_UPB_PRIVATE(ext_count);
};

#ifdef __cplusplus
extern "C" {
#endif

WPI_UPB_API_INLINE int wpi_upb_MiniTableFile_EnumCount(
    const struct wpi_upb_MiniTableFile* f) {
  return f->WPI_UPB_PRIVATE(enum_count);
}

WPI_UPB_API_INLINE int wpi_upb_MiniTableFile_ExtensionCount(
    const struct wpi_upb_MiniTableFile* f) {
  return f->WPI_UPB_PRIVATE(ext_count);
}

WPI_UPB_API_INLINE int wpi_upb_MiniTableFile_MessageCount(
    const struct wpi_upb_MiniTableFile* f) {
  return f->WPI_UPB_PRIVATE(msg_count);
}

WPI_UPB_API_INLINE const struct wpi_upb_MiniTableEnum* wpi_upb_MiniTableFile_Enum(
    const struct wpi_upb_MiniTableFile* f, int i) {
  WPI_UPB_ASSERT(i < f->WPI_UPB_PRIVATE(enum_count));
  return f->WPI_UPB_PRIVATE(enums)[i];
}

WPI_UPB_API_INLINE const struct wpi_upb_MiniTableExtension* wpi_upb_MiniTableFile_Extension(
    const struct wpi_upb_MiniTableFile* f, int i) {
  WPI_UPB_ASSERT(i < f->WPI_UPB_PRIVATE(ext_count));
  return f->WPI_UPB_PRIVATE(exts)[i];
}

WPI_UPB_API_INLINE const struct wpi_upb_MiniTable* wpi_upb_MiniTableFile_Message(
    const struct wpi_upb_MiniTableFile* f, int i) {
  WPI_UPB_ASSERT(i < f->WPI_UPB_PRIVATE(msg_count));
  return f->WPI_UPB_PRIVATE(msgs)[i];
}

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_MINI_TABLE_INTERNAL_FILE_H_ */

// Must be last.

typedef struct wpi_upb_MiniTableFile wpi_upb_MiniTableFile;

#ifdef __cplusplus
extern "C" {
#endif

WPI_UPB_API_INLINE const wpi_upb_MiniTableEnum* wpi_upb_MiniTableFile_Enum(
    const wpi_upb_MiniTableFile* f, int i);

WPI_UPB_API_INLINE int wpi_upb_MiniTableFile_EnumCount(const wpi_upb_MiniTableFile* f);

WPI_UPB_API_INLINE const wpi_upb_MiniTableExtension* wpi_upb_MiniTableFile_Extension(
    const wpi_upb_MiniTableFile* f, int i);

WPI_UPB_API_INLINE int wpi_upb_MiniTableFile_ExtensionCount(const wpi_upb_MiniTableFile* f);

WPI_UPB_API_INLINE const wpi_upb_MiniTable* wpi_upb_MiniTableFile_Message(
    const wpi_upb_MiniTableFile* f, int i);

WPI_UPB_API_INLINE int wpi_upb_MiniTableFile_MessageCount(const wpi_upb_MiniTableFile* f);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_MINI_TABLE_FILE_H_ */

// wpi_upb_decode: parsing into a wpi_upb_Message using a wpi_upb_MiniTable.

#ifndef WPI_UPB_WIRE_DECODE_H_
#define WPI_UPB_WIRE_DECODE_H_

#include <stddef.h>
#include <stdint.h>


// Must be last.

#ifdef __cplusplus
extern "C" {
#endif

enum {
  /* If set, strings will alias the input buffer instead of copying into the
   * arena. */
  kUpb_DecodeOption_AliasString = 1,

  /* If set, the parse will return failure if any message is missing any
   * required fields when the message data ends.  The parse will still continue,
   * and the failure will only be reported at the end.
   *
   * IMPORTANT CAVEATS:
   *
   * 1. This can throw a false positive failure if an incomplete message is seen
   *    on the wire but is later completed when the sub-message occurs again.
   *    For this reason, a second pass is required to verify a failure, to be
   *    truly robust.
   *
   * 2. This can return a false success if you are decoding into a message that
   *    already has some sub-message fields present.  If the sub-message does
   *    not occur in the binary payload, we will never visit it and discover the
   *    incomplete sub-message.  For this reason, this check is only useful for
   *    implemting ParseFromString() semantics.  For MergeFromString(), a
   *    post-parse validation step will always be necessary. */
  kUpb_DecodeOption_CheckRequired = 2,

  /* EXPERIMENTAL:
   *
   * If set, the parser will allow parsing of sub-message fields that were not
   * previously linked using wpi_upb_MiniTable_SetSubMessage().  The data will be
   * parsed into an internal "empty" message type that cannot be accessed
   * directly, but can be later promoted into the true message type if the
   * sub-message fields are linked at a later time.
   *
   * Users should set this option if they intend to perform dynamic tree shaking
   * and promoting using the interfaces in message/promote.h.  If this option is
   * enabled, it is important that the resulting messages are only accessed by
   * code that is aware of promotion rules:
   *
   * 1. Message pointers in wpi_upb_Message, wpi_upb_Array, and wpi_upb_Map are represented
   *    by a tagged pointer wpi_upb_TaggedMessagePointer.  The tag indicates whether
   *    the message uses the internal "empty" type.
   *
   * 2. Any code *reading* these message pointers must test whether the "empty"
   *    tag bit is set, using the interfaces in mini_table/types.h.  However
   *    writing of message pointers should always use plain wpi_upb_Message*, since
   *    users are not allowed to create "empty" messages.
   *
   * 3. It is always safe to test whether a field is present or test the array
   *    length; these interfaces will reflect that empty messages are present,
   *    even though their data cannot be accessed without promoting first.
   *
   * 4. If a message pointer is indeed tagged as empty, the message may not be
   *    accessed directly, only promoted through the interfaces in
   *    message/promote.h.
   *
   * 5. Tagged/empty messages may never be created by the user.  They may only
   *    be created by the parser or the message-copying logic in message/copy.h.
   */
  kUpb_DecodeOption_ExperimentalAllowUnlinked = 4,

  /* EXPERIMENTAL:
   *
   * If set, decoding will enforce UTF-8 validation for string fields, even for
   * proto2 or fields with `features.utf8_validation = NONE`. Normally, only
   * proto3 string fields will be validated for UTF-8. Decoding will return
   * kUpb_DecodeStatus_BadUtf8 for non-UTF-8 strings, which is the same behavior
   * as non-UTF-8 proto3 string fields.
   */
  kUpb_DecodeOption_AlwaysValidateUtf8 = 8,
};

WPI_UPB_INLINE uint32_t wpi_upb_DecodeOptions_MaxDepth(uint16_t depth) {
  return (uint32_t)depth << 16;
}

WPI_UPB_INLINE uint16_t wpi_upb_DecodeOptions_GetMaxDepth(uint32_t options) {
  return options >> 16;
}

// Enforce an upper bound on recursion depth.
WPI_UPB_INLINE int wpi_upb_Decode_LimitDepth(uint32_t decode_options, uint32_t limit) {
  uint32_t max_depth = wpi_upb_DecodeOptions_GetMaxDepth(decode_options);
  if (max_depth > limit) max_depth = limit;
  return wpi_upb_DecodeOptions_MaxDepth(max_depth) | (decode_options & 0xffff);
}

// LINT.IfChange
typedef enum {
  kUpb_DecodeStatus_Ok = 0,
  kUpb_DecodeStatus_Malformed = 1,    // Wire format was corrupt
  kUpb_DecodeStatus_OutOfMemory = 2,  // Arena alloc failed
  kUpb_DecodeStatus_BadUtf8 = 3,      // String field had bad UTF-8
  kUpb_DecodeStatus_MaxDepthExceeded =
      4,  // Exceeded wpi_upb_DecodeOptions_MaxDepth

  // kUpb_DecodeOption_CheckRequired failed (see above), but the parse otherwise
  // succeeded.
  kUpb_DecodeStatus_MissingRequired = 5,

  // Unlinked sub-message field was present, but
  // kUpb_DecodeOptions_ExperimentalAllowUnlinked was not specified in the list
  // of options.
  kUpb_DecodeStatus_UnlinkedSubMessage = 6,
} wpi_upb_DecodeStatus;
// LINT.ThenChange(//depot/google3/third_party/protobuf/rust/upb.rs:decode_status)

WPI_UPB_API wpi_upb_DecodeStatus wpi_upb_Decode(const char* buf, size_t size,
                                    wpi_upb_Message* msg, const wpi_upb_MiniTable* mt,
                                    const wpi_upb_ExtensionRegistry* extreg,
                                    int options, wpi_upb_Arena* arena);

// Same as wpi_upb_Decode but with a varint-encoded length prepended.
// On success 'num_bytes_read' will be set to the how many bytes were read,
// on failure the contents of num_bytes_read is undefined.
WPI_UPB_API wpi_upb_DecodeStatus wpi_upb_DecodeLengthPrefixed(
    const char* buf, size_t size, wpi_upb_Message* msg, size_t* num_bytes_read,
    const wpi_upb_MiniTable* mt, const wpi_upb_ExtensionRegistry* extreg, int options,
    wpi_upb_Arena* arena);

// Utility function for wrapper languages to get an error string from a
// wpi_upb_DecodeStatus.
WPI_UPB_API const char* wpi_upb_DecodeStatus_String(wpi_upb_DecodeStatus status);
#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_WIRE_DECODE_H_ */

// wpi_upb_Encode: parsing from a wpi_upb_Message using a wpi_upb_MiniTable.

#ifndef WPI_UPB_WIRE_ENCODE_H_
#define WPI_UPB_WIRE_ENCODE_H_

#include <stddef.h>
#include <stdint.h>


// Must be last.

#ifdef __cplusplus
extern "C" {
#endif

enum {
  /* If set, the results of serializing will be deterministic across all
   * instances of this binary. There are no guarantees across different
   * binary builds.
   *
   * If your proto contains maps, the encoder will need to malloc()/free()
   * memory during encode. */
  kUpb_EncodeOption_Deterministic = 1,

  // When set, unknown fields are not encoded.
  kUpb_EncodeOption_SkipUnknown = 2,

  // When set, the encode will fail if any required fields are missing.
  kUpb_EncodeOption_CheckRequired = 4,
};

// LINT.IfChange
typedef enum {
  kUpb_EncodeStatus_Ok = 0,
  kUpb_EncodeStatus_OutOfMemory = 1,  // Arena alloc failed
  kUpb_EncodeStatus_MaxDepthExceeded = 2,

  // kUpb_EncodeOption_CheckRequired failed but the parse otherwise succeeded.
  kUpb_EncodeStatus_MissingRequired = 3,
} wpi_upb_EncodeStatus;
// LINT.ThenChange(//depot/google3/third_party/protobuf/rust/upb.rs:encode_status)

WPI_UPB_INLINE uint32_t wpi_upb_EncodeOptions_MaxDepth(uint16_t depth) {
  return (uint32_t)depth << 16;
}

WPI_UPB_INLINE uint16_t wpi_upb_EncodeOptions_GetMaxDepth(uint32_t options) {
  return options >> 16;
}

// Enforce an upper bound on recursion depth.
WPI_UPB_INLINE int wpi_upb_Encode_LimitDepth(uint32_t encode_options, uint32_t limit) {
  uint32_t max_depth = wpi_upb_EncodeOptions_GetMaxDepth(encode_options);
  if (max_depth > limit) max_depth = limit;
  return wpi_upb_EncodeOptions_MaxDepth(max_depth) | (encode_options & 0xffff);
}

WPI_UPB_API wpi_upb_EncodeStatus wpi_upb_Encode(const wpi_upb_Message* msg,
                                    const wpi_upb_MiniTable* l, int options,
                                    wpi_upb_Arena* arena, char** buf, size_t* size);

// Encodes the message prepended by a varint of the serialized length.
WPI_UPB_API wpi_upb_EncodeStatus wpi_upb_EncodeLengthPrefixed(const wpi_upb_Message* msg,
                                                  const wpi_upb_MiniTable* l,
                                                  int options, wpi_upb_Arena* arena,
                                                  char** buf, size_t* size);
// Utility function for wrapper languages to get an error string from a
// wpi_upb_EncodeStatus.
WPI_UPB_API const char* wpi_upb_EncodeStatus_String(wpi_upb_EncodeStatus status);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_WIRE_ENCODE_H_ */

// These are the specialized field parser functions for the fast parser.
// Generated tables will refer to these by name.
//
// The function names are encoded with names like:
//
//   //  123 4
//   wpi_upb_pss_1bt();   // Parse singular string, 1 byte tag.
//
// In position 1:
//   - 'p' for parse, most function use this
//   - 'c' for copy, for when we are copying strings instead of aliasing
//
// In position 2 (cardinality):
//   - 's' for singular, with or without hasbit
//   - 'o' for oneof
//   - 'r' for non-packed repeated
//   - 'p' for packed repeated
//
// In position 3 (type):
//   - 'b1' for bool
//   - 'v4' for 4-byte varint
//   - 'v8' for 8-byte varint
//   - 'z4' for zig-zag-encoded 4-byte varint
//   - 'z8' for zig-zag-encoded 8-byte varint
//   - 'f4' for 4-byte fixed
//   - 'f8' for 8-byte fixed
//   - 'm' for sub-message
//   - 's' for string (validate UTF-8)
//   - 'b' for bytes
//
// In position 4 (tag length):
//   - '1' for one-byte tags (field numbers 1-15)
//   - '2' for two-byte tags (field numbers 16-2048)

#ifndef WPI_UPB_WIRE_INTERNAL_DECODE_FAST_H_
#define WPI_UPB_WIRE_INTERNAL_DECODE_FAST_H_


// Must be last.

#ifdef __cplusplus
extern "C" {
#endif

struct wpi_upb_Decoder;

// The fallback, generic parsing function that can handle any field type.
// This just uses the regular (non-fast) parser to parse a single field.
const char* _wpi_upb_FastDecoder_DecodeGeneric(struct wpi_upb_Decoder* d,
                                           const char* ptr, wpi_upb_Message* msg,
                                           intptr_t table, uint64_t hasbits,
                                           uint64_t data);

#define WPI_UPB_PARSE_PARAMS                                                    \
  struct wpi_upb_Decoder *d, const char *ptr, wpi_upb_Message *msg, intptr_t table, \
      uint64_t hasbits, uint64_t data

/* primitive fields ***********************************************************/

#define F(card, type, valbytes, tagbytes) \
  const char* wpi_upb_p##card##type##valbytes##_##tagbytes##bt(WPI_UPB_PARSE_PARAMS);

#define TYPES(card, tagbytes) \
  F(card, b, 1, tagbytes)     \
  F(card, v, 4, tagbytes)     \
  F(card, v, 8, tagbytes)     \
  F(card, z, 4, tagbytes)     \
  F(card, z, 8, tagbytes)     \
  F(card, f, 4, tagbytes)     \
  F(card, f, 8, tagbytes)

#define TAGBYTES(card) \
  TYPES(card, 1)       \
  TYPES(card, 2)

TAGBYTES(s)
TAGBYTES(o)
TAGBYTES(r)
TAGBYTES(p)

#undef F
#undef TYPES
#undef TAGBYTES

/* string fields **************************************************************/

#define F(card, tagbytes, type)                                     \
  const char* wpi_upb_p##card##type##_##tagbytes##bt(WPI_UPB_PARSE_PARAMS); \
  const char* wpi_upb_c##card##type##_##tagbytes##bt(WPI_UPB_PARSE_PARAMS);

#define UTF8(card, tagbytes) \
  F(card, tagbytes, s)       \
  F(card, tagbytes, b)

#define TAGBYTES(card) \
  UTF8(card, 1)        \
  UTF8(card, 2)

TAGBYTES(s)
TAGBYTES(o)
TAGBYTES(r)

#undef F
#undef UTF8
#undef TAGBYTES

/* sub-message fields *********************************************************/

#define F(card, tagbytes, size_ceil, ceil_arg) \
  const char* wpi_upb_p##card##m_##tagbytes##bt_max##size_ceil##b(WPI_UPB_PARSE_PARAMS);

#define SIZES(card, tagbytes) \
  F(card, tagbytes, 64, 64)   \
  F(card, tagbytes, 128, 128) \
  F(card, tagbytes, 192, 192) \
  F(card, tagbytes, 256, 256) \
  F(card, tagbytes, max, -1)

#define TAGBYTES(card) \
  SIZES(card, 1)       \
  SIZES(card, 2)

TAGBYTES(s)
TAGBYTES(o)
TAGBYTES(r)

#undef F
#undef SIZES
#undef TAGBYTES

#undef WPI_UPB_PARSE_PARAMS

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_WIRE_INTERNAL_DECODE_FAST_H_ */
// IWYU pragma: end_exports

#endif  // WPI_UPB_GENERATED_CODE_SUPPORT_H_
/* This file was generated by wpi_upb_generator from the input file:
 *
 *     google/protobuf/descriptor.proto
 *
 * Do not edit -- your changes will be discarded when the file is
 * regenerated.
 * NO CHECKED-IN PROTOBUF GENCODE */

#ifndef GOOGLE_PROTOBUF_DESCRIPTOR_PROTO_WPI_UPB_MINITABLE_H_
#define GOOGLE_PROTOBUF_DESCRIPTOR_PROTO_WPI_UPB_MINITABLE_H_


// Must be last.

#ifdef __cplusplus
extern "C" {
#endif

extern const wpi_upb_MiniTable google__protobuf__FileDescriptorSet_msg_init;
extern const wpi_upb_MiniTable* google__protobuf__FileDescriptorSet_msg_init_ptr;
extern const wpi_upb_MiniTable google__protobuf__FileDescriptorProto_msg_init;
extern const wpi_upb_MiniTable* google__protobuf__FileDescriptorProto_msg_init_ptr;
extern const wpi_upb_MiniTable google__protobuf__DescriptorProto_msg_init;
extern const wpi_upb_MiniTable* google__protobuf__DescriptorProto_msg_init_ptr;
extern const wpi_upb_MiniTable google__protobuf__DescriptorProto__ExtensionRange_msg_init;
extern const wpi_upb_MiniTable* google__protobuf__DescriptorProto__ExtensionRange_msg_init_ptr;
extern const wpi_upb_MiniTable google__protobuf__DescriptorProto__ReservedRange_msg_init;
extern const wpi_upb_MiniTable* google__protobuf__DescriptorProto__ReservedRange_msg_init_ptr;
extern const wpi_upb_MiniTable google__protobuf__ExtensionRangeOptions_msg_init;
extern const wpi_upb_MiniTable* google__protobuf__ExtensionRangeOptions_msg_init_ptr;
extern const wpi_upb_MiniTable google__protobuf__ExtensionRangeOptions__Declaration_msg_init;
extern const wpi_upb_MiniTable* google__protobuf__ExtensionRangeOptions__Declaration_msg_init_ptr;
extern const wpi_upb_MiniTable google__protobuf__FieldDescriptorProto_msg_init;
extern const wpi_upb_MiniTable* google__protobuf__FieldDescriptorProto_msg_init_ptr;
extern const wpi_upb_MiniTable google__protobuf__OneofDescriptorProto_msg_init;
extern const wpi_upb_MiniTable* google__protobuf__OneofDescriptorProto_msg_init_ptr;
extern const wpi_upb_MiniTable google__protobuf__EnumDescriptorProto_msg_init;
extern const wpi_upb_MiniTable* google__protobuf__EnumDescriptorProto_msg_init_ptr;
extern const wpi_upb_MiniTable google__protobuf__EnumDescriptorProto__EnumReservedRange_msg_init;
extern const wpi_upb_MiniTable* google__protobuf__EnumDescriptorProto__EnumReservedRange_msg_init_ptr;
extern const wpi_upb_MiniTable google__protobuf__EnumValueDescriptorProto_msg_init;
extern const wpi_upb_MiniTable* google__protobuf__EnumValueDescriptorProto_msg_init_ptr;
extern const wpi_upb_MiniTable google__protobuf__ServiceDescriptorProto_msg_init;
extern const wpi_upb_MiniTable* google__protobuf__ServiceDescriptorProto_msg_init_ptr;
extern const wpi_upb_MiniTable google__protobuf__MethodDescriptorProto_msg_init;
extern const wpi_upb_MiniTable* google__protobuf__MethodDescriptorProto_msg_init_ptr;
extern const wpi_upb_MiniTable google__protobuf__FileOptions_msg_init;
extern const wpi_upb_MiniTable* google__protobuf__FileOptions_msg_init_ptr;
extern const wpi_upb_MiniTable google__protobuf__MessageOptions_msg_init;
extern const wpi_upb_MiniTable* google__protobuf__MessageOptions_msg_init_ptr;
extern const wpi_upb_MiniTable google__protobuf__FieldOptions_msg_init;
extern const wpi_upb_MiniTable* google__protobuf__FieldOptions_msg_init_ptr;
extern const wpi_upb_MiniTable google__protobuf__FieldOptions__EditionDefault_msg_init;
extern const wpi_upb_MiniTable* google__protobuf__FieldOptions__EditionDefault_msg_init_ptr;
extern const wpi_upb_MiniTable google__protobuf__FieldOptions__FeatureSupport_msg_init;
extern const wpi_upb_MiniTable* google__protobuf__FieldOptions__FeatureSupport_msg_init_ptr;
extern const wpi_upb_MiniTable google__protobuf__OneofOptions_msg_init;
extern const wpi_upb_MiniTable* google__protobuf__OneofOptions_msg_init_ptr;
extern const wpi_upb_MiniTable google__protobuf__EnumOptions_msg_init;
extern const wpi_upb_MiniTable* google__protobuf__EnumOptions_msg_init_ptr;
extern const wpi_upb_MiniTable google__protobuf__EnumValueOptions_msg_init;
extern const wpi_upb_MiniTable* google__protobuf__EnumValueOptions_msg_init_ptr;
extern const wpi_upb_MiniTable google__protobuf__ServiceOptions_msg_init;
extern const wpi_upb_MiniTable* google__protobuf__ServiceOptions_msg_init_ptr;
extern const wpi_upb_MiniTable google__protobuf__MethodOptions_msg_init;
extern const wpi_upb_MiniTable* google__protobuf__MethodOptions_msg_init_ptr;
extern const wpi_upb_MiniTable google__protobuf__UninterpretedOption_msg_init;
extern const wpi_upb_MiniTable* google__protobuf__UninterpretedOption_msg_init_ptr;
extern const wpi_upb_MiniTable google__protobuf__UninterpretedOption__NamePart_msg_init;
extern const wpi_upb_MiniTable* google__protobuf__UninterpretedOption__NamePart_msg_init_ptr;
extern const wpi_upb_MiniTable google__protobuf__FeatureSet_msg_init;
extern const wpi_upb_MiniTable* google__protobuf__FeatureSet_msg_init_ptr;
extern const wpi_upb_MiniTable google__protobuf__FeatureSetDefaults_msg_init;
extern const wpi_upb_MiniTable* google__protobuf__FeatureSetDefaults_msg_init_ptr;
extern const wpi_upb_MiniTable google__protobuf__FeatureSetDefaults__FeatureSetEditionDefault_msg_init;
extern const wpi_upb_MiniTable* google__protobuf__FeatureSetDefaults__FeatureSetEditionDefault_msg_init_ptr;
extern const wpi_upb_MiniTable google__protobuf__SourceCodeInfo_msg_init;
extern const wpi_upb_MiniTable* google__protobuf__SourceCodeInfo_msg_init_ptr;
extern const wpi_upb_MiniTable google__protobuf__SourceCodeInfo__Location_msg_init;
extern const wpi_upb_MiniTable* google__protobuf__SourceCodeInfo__Location_msg_init_ptr;
extern const wpi_upb_MiniTable google__protobuf__GeneratedCodeInfo_msg_init;
extern const wpi_upb_MiniTable* google__protobuf__GeneratedCodeInfo_msg_init_ptr;
extern const wpi_upb_MiniTable google__protobuf__GeneratedCodeInfo__Annotation_msg_init;
extern const wpi_upb_MiniTable* google__protobuf__GeneratedCodeInfo__Annotation_msg_init_ptr;

extern const wpi_upb_MiniTableEnum google_protobuf_Edition_enum_init;
extern const wpi_upb_MiniTableEnum google_protobuf_ExtensionRangeOptions_VerificationState_enum_init;
extern const wpi_upb_MiniTableEnum google_protobuf_FeatureSet_EnumType_enum_init;
extern const wpi_upb_MiniTableEnum google_protobuf_FeatureSet_FieldPresence_enum_init;
extern const wpi_upb_MiniTableEnum google_protobuf_FeatureSet_JsonFormat_enum_init;
extern const wpi_upb_MiniTableEnum google_protobuf_FeatureSet_MessageEncoding_enum_init;
extern const wpi_upb_MiniTableEnum google_protobuf_FeatureSet_RepeatedFieldEncoding_enum_init;
extern const wpi_upb_MiniTableEnum google_protobuf_FeatureSet_Utf8Validation_enum_init;
extern const wpi_upb_MiniTableEnum google_protobuf_FieldDescriptorProto_Label_enum_init;
extern const wpi_upb_MiniTableEnum google_protobuf_FieldDescriptorProto_Type_enum_init;
extern const wpi_upb_MiniTableEnum google_protobuf_FieldOptions_CType_enum_init;
extern const wpi_upb_MiniTableEnum google_protobuf_FieldOptions_JSType_enum_init;
extern const wpi_upb_MiniTableEnum google_protobuf_FieldOptions_OptionRetention_enum_init;
extern const wpi_upb_MiniTableEnum google_protobuf_FieldOptions_OptionTargetType_enum_init;
extern const wpi_upb_MiniTableEnum google_protobuf_FileOptions_OptimizeMode_enum_init;
extern const wpi_upb_MiniTableEnum google_protobuf_GeneratedCodeInfo_Annotation_Semantic_enum_init;
extern const wpi_upb_MiniTableEnum google_protobuf_MethodOptions_IdempotencyLevel_enum_init;
extern const wpi_upb_MiniTableFile google_protobuf_descriptor_proto_wpi_upb_file_layout;

#ifdef __cplusplus
}  /* extern "C" */
#endif


#endif  /* GOOGLE_PROTOBUF_DESCRIPTOR_PROTO_WPI_UPB_MINITABLE_H_ */

#ifndef WPI_UPB_WIRE_EPS_COPY_INPUT_STREAM_H_
#define WPI_UPB_WIRE_EPS_COPY_INPUT_STREAM_H_

#include <string.h>


// Must be last.

#ifdef __cplusplus
extern "C" {
#endif

// The maximum number of bytes a single protobuf field can take up in the
// wire format.  We only want to do one bounds check per field, so the input
// stream guarantees that after wpi_upb_EpsCopyInputStream_IsDone() is called,
// the decoder can read this many bytes without performing another bounds
// check.  The stream will copy into a patch buffer as necessary to guarantee
// this invariant.
#define kUpb_EpsCopyInputStream_SlopBytes 16

enum {
  kUpb_EpsCopyInputStream_NoAliasing = 0,
  kUpb_EpsCopyInputStream_OnPatch = 1,
  kUpb_EpsCopyInputStream_NoDelta = 2
};

typedef struct {
  const char* end;        // Can read up to SlopBytes bytes beyond this.
  const char* limit_ptr;  // For bounds checks, = end + WPI_UPB_MIN(limit, 0)
  uintptr_t aliasing;
  int limit;   // Submessage limit relative to end
  bool error;  // To distinguish between EOF and error.
  char patch[kUpb_EpsCopyInputStream_SlopBytes * 2];
} wpi_upb_EpsCopyInputStream;

// Returns true if the stream is in the error state. A stream enters the error
// state when the user reads past a limit (caught in IsDone()) or the
// ZeroCopyInputStream returns an error.
WPI_UPB_INLINE bool wpi_upb_EpsCopyInputStream_IsError(wpi_upb_EpsCopyInputStream* e) {
  return e->error;
}

typedef const char* wpi_upb_EpsCopyInputStream_BufferFlipCallback(
    wpi_upb_EpsCopyInputStream* e, const char* old_end, const char* new_start);

typedef const char* wpi_upb_EpsCopyInputStream_IsDoneFallbackFunc(
    wpi_upb_EpsCopyInputStream* e, const char* ptr, int overrun);

// Initializes a wpi_upb_EpsCopyInputStream using the contents of the buffer
// [*ptr, size].  Updates `*ptr` as necessary to guarantee that at least
// kUpb_EpsCopyInputStream_SlopBytes are available to read.
WPI_UPB_INLINE void wpi_upb_EpsCopyInputStream_Init(wpi_upb_EpsCopyInputStream* e,
                                            const char** ptr, size_t size,
                                            bool enable_aliasing) {
  if (size <= kUpb_EpsCopyInputStream_SlopBytes) {
    memset(&e->patch, 0, 32);
    if (size) memcpy(&e->patch, *ptr, size);
    e->aliasing = enable_aliasing ? (uintptr_t)*ptr - (uintptr_t)e->patch
                                  : kUpb_EpsCopyInputStream_NoAliasing;
    *ptr = e->patch;
    e->end = *ptr + size;
    e->limit = 0;
  } else {
    e->end = *ptr + size - kUpb_EpsCopyInputStream_SlopBytes;
    e->limit = kUpb_EpsCopyInputStream_SlopBytes;
    e->aliasing = enable_aliasing ? kUpb_EpsCopyInputStream_NoDelta
                                  : kUpb_EpsCopyInputStream_NoAliasing;
  }
  e->limit_ptr = e->end;
  e->error = false;
}

typedef enum {
  // The current stream position is at a limit.
  kUpb_IsDoneStatus_Done,

  // The current stream position is not at a limit.
  kUpb_IsDoneStatus_NotDone,

  // The current stream position is not at a limit, and the stream needs to
  // be flipped to a new buffer before more data can be read.
  kUpb_IsDoneStatus_NeedFallback,
} wpi_upb_IsDoneStatus;

// Returns the status of the current stream position.  This is a low-level
// function, it is simpler to call wpi_upb_EpsCopyInputStream_IsDone() if possible.
WPI_UPB_INLINE wpi_upb_IsDoneStatus wpi_upb_EpsCopyInputStream_IsDoneStatus(
    wpi_upb_EpsCopyInputStream* e, const char* ptr, int* overrun) {
  *overrun = ptr - e->end;
  if (WPI_UPB_LIKELY(ptr < e->limit_ptr)) {
    return kUpb_IsDoneStatus_NotDone;
  } else if (WPI_UPB_LIKELY(*overrun == e->limit)) {
    return kUpb_IsDoneStatus_Done;
  } else {
    return kUpb_IsDoneStatus_NeedFallback;
  }
}

// Returns true if the stream has hit a limit, either the current delimited
// limit or the overall end-of-stream. As a side effect, this function may flip
// the pointer to a new buffer if there are less than
// kUpb_EpsCopyInputStream_SlopBytes of data to be read in the current buffer.
//
// Postcondition: if the function returns false, there are at least
// kUpb_EpsCopyInputStream_SlopBytes of data available to read at *ptr.
WPI_UPB_INLINE bool wpi_upb_EpsCopyInputStream_IsDoneWithCallback(
    wpi_upb_EpsCopyInputStream* e, const char** ptr,
    wpi_upb_EpsCopyInputStream_IsDoneFallbackFunc* func) {
  int overrun;
  switch (wpi_upb_EpsCopyInputStream_IsDoneStatus(e, *ptr, &overrun)) {
    case kUpb_IsDoneStatus_Done:
      return true;
    case kUpb_IsDoneStatus_NotDone:
      return false;
    case kUpb_IsDoneStatus_NeedFallback:
      *ptr = func(e, *ptr, overrun);
      return *ptr == NULL;
  }
  WPI_UPB_UNREACHABLE();
}

const char* _wpi_upb_EpsCopyInputStream_IsDoneFallbackNoCallback(
    wpi_upb_EpsCopyInputStream* e, const char* ptr, int overrun);

// A simpler version of IsDoneWithCallback() that does not support a buffer flip
// callback. Useful in cases where we do not need to insert custom logic at
// every buffer flip.
//
// If this returns true, the user must call wpi_upb_EpsCopyInputStream_IsError()
// to distinguish between EOF and error.
WPI_UPB_INLINE bool wpi_upb_EpsCopyInputStream_IsDone(wpi_upb_EpsCopyInputStream* e,
                                              const char** ptr) {
  return wpi_upb_EpsCopyInputStream_IsDoneWithCallback(
      e, ptr, _wpi_upb_EpsCopyInputStream_IsDoneFallbackNoCallback);
}

// Returns the total number of bytes that are safe to read from the current
// buffer without reading uninitialized or unallocated memory.
//
// Note that this check does not respect any semantic limits on the stream,
// either limits from PushLimit() or the overall stream end, so some of these
// bytes may have unpredictable, nonsense values in them. The guarantee is only
// that the bytes are valid to read from the perspective of the C language
// (ie. you can read without triggering UBSAN or ASAN).
WPI_UPB_INLINE size_t wpi_upb_EpsCopyInputStream_BytesAvailable(
    wpi_upb_EpsCopyInputStream* e, const char* ptr) {
  return (e->end - ptr) + kUpb_EpsCopyInputStream_SlopBytes;
}

// Returns true if the given delimited field size is valid (it does not extend
// beyond any previously-pushed limits).  `ptr` should point to the beginning
// of the field data, after the delimited size.
//
// Note that this does *not* guarantee that all of the data for this field is in
// the current buffer.
WPI_UPB_INLINE bool wpi_upb_EpsCopyInputStream_CheckSize(
    const wpi_upb_EpsCopyInputStream* e, const char* ptr, int size) {
  WPI_UPB_ASSERT(size >= 0);
  return ptr - e->end + size <= e->limit;
}

WPI_UPB_INLINE bool _wpi_upb_EpsCopyInputStream_CheckSizeAvailable(
    wpi_upb_EpsCopyInputStream* e, const char* ptr, int size, bool submessage) {
  // This is one extra branch compared to the more normal:
  //   return (size_t)(end - ptr) < size;
  // However it is one less computation if we are just about to use "ptr + len":
  //   https://godbolt.org/z/35YGPz
  // In microbenchmarks this shows a small improvement.
  uintptr_t uptr = (uintptr_t)ptr;
  uintptr_t uend = (uintptr_t)e->limit_ptr;
  uintptr_t res = uptr + (size_t)size;
  if (!submessage) uend += kUpb_EpsCopyInputStream_SlopBytes;
  // NOTE: this check depends on having a linear address space.  This is not
  // technically guaranteed by uintptr_t.
  bool ret = res >= uptr && res <= uend;
  if (size < 0) WPI_UPB_ASSERT(!ret);
  return ret;
}

// Returns true if the given delimited field size is valid (it does not extend
// beyond any previously-pushed limited) *and* all of the data for this field is
// available to be read in the current buffer.
//
// If the size is negative, this function will always return false. This
// property can be useful in some cases.
WPI_UPB_INLINE bool wpi_upb_EpsCopyInputStream_CheckDataSizeAvailable(
    wpi_upb_EpsCopyInputStream* e, const char* ptr, int size) {
  return _wpi_upb_EpsCopyInputStream_CheckSizeAvailable(e, ptr, size, false);
}

// Returns true if the given sub-message size is valid (it does not extend
// beyond any previously-pushed limited) *and* all of the data for this
// sub-message is available to be parsed in the current buffer.
//
// This implies that all fields from the sub-message can be parsed from the
// current buffer while maintaining the invariant that we always have at least
// kUpb_EpsCopyInputStream_SlopBytes of data available past the beginning of
// any individual field start.
//
// If the size is negative, this function will always return false. This
// property can be useful in some cases.
WPI_UPB_INLINE bool wpi_upb_EpsCopyInputStream_CheckSubMessageSizeAvailable(
    wpi_upb_EpsCopyInputStream* e, const char* ptr, int size) {
  return _wpi_upb_EpsCopyInputStream_CheckSizeAvailable(e, ptr, size, true);
}

// Returns true if aliasing_enabled=true was passed to
// wpi_upb_EpsCopyInputStream_Init() when this stream was initialized.
WPI_UPB_INLINE bool wpi_upb_EpsCopyInputStream_AliasingEnabled(
    wpi_upb_EpsCopyInputStream* e) {
  return e->aliasing != kUpb_EpsCopyInputStream_NoAliasing;
}

// Returns true if aliasing_enabled=true was passed to
// wpi_upb_EpsCopyInputStream_Init() when this stream was initialized *and* we can
// alias into the region [ptr, size] in an input buffer.
WPI_UPB_INLINE bool wpi_upb_EpsCopyInputStream_AliasingAvailable(
    wpi_upb_EpsCopyInputStream* e, const char* ptr, size_t size) {
  // When EpsCopyInputStream supports streaming, this will need to become a
  // runtime check.
  return wpi_upb_EpsCopyInputStream_CheckDataSizeAvailable(e, ptr, size) &&
         e->aliasing >= kUpb_EpsCopyInputStream_NoDelta;
}

// Returns a pointer into an input buffer that corresponds to the parsing
// pointer `ptr`.  The returned pointer may be the same as `ptr`, but also may
// be different if we are currently parsing out of the patch buffer.
//
// REQUIRES: Aliasing must be available for the given pointer. If the input is a
// flat buffer and aliasing is enabled, then aliasing will always be available.
WPI_UPB_INLINE const char* wpi_upb_EpsCopyInputStream_GetAliasedPtr(
    wpi_upb_EpsCopyInputStream* e, const char* ptr) {
  WPI_UPB_ASSUME(wpi_upb_EpsCopyInputStream_AliasingAvailable(e, ptr, 0));
  uintptr_t delta =
      e->aliasing == kUpb_EpsCopyInputStream_NoDelta ? 0 : e->aliasing;
  return (const char*)((uintptr_t)ptr + delta);
}

// Reads string data from the input, aliasing into the input buffer instead of
// copying. The parsing pointer is passed in `*ptr`, and will be updated if
// necessary to point to the actual input buffer. Returns the new parsing
// pointer, which will be advanced past the string data.
//
// REQUIRES: Aliasing must be available for this data region (test with
// wpi_upb_EpsCopyInputStream_AliasingAvailable().
WPI_UPB_INLINE const char* wpi_upb_EpsCopyInputStream_ReadStringAliased(
    wpi_upb_EpsCopyInputStream* e, const char** ptr, size_t size) {
  WPI_UPB_ASSUME(wpi_upb_EpsCopyInputStream_AliasingAvailable(e, *ptr, size));
  const char* ret = *ptr + size;
  *ptr = wpi_upb_EpsCopyInputStream_GetAliasedPtr(e, *ptr);
  WPI_UPB_ASSUME(ret != NULL);
  return ret;
}

// Skips `size` bytes of data from the input and returns a pointer past the end.
// Returns NULL on end of stream or error.
WPI_UPB_INLINE const char* wpi_upb_EpsCopyInputStream_Skip(wpi_upb_EpsCopyInputStream* e,
                                                   const char* ptr, int size) {
  if (!wpi_upb_EpsCopyInputStream_CheckDataSizeAvailable(e, ptr, size)) return NULL;
  return ptr + size;
}

// Copies `size` bytes of data from the input `ptr` into the buffer `to`, and
// returns a pointer past the end. Returns NULL on end of stream or error.
WPI_UPB_INLINE const char* wpi_upb_EpsCopyInputStream_Copy(wpi_upb_EpsCopyInputStream* e,
                                                   const char* ptr, void* to,
                                                   int size) {
  if (!wpi_upb_EpsCopyInputStream_CheckDataSizeAvailable(e, ptr, size)) return NULL;
  memcpy(to, ptr, size);
  return ptr + size;
}

// Reads string data from the stream and advances the pointer accordingly.
// If aliasing was enabled when the stream was initialized, then the returned
// pointer will point into the input buffer if possible, otherwise new data
// will be allocated from arena and copied into. We may be forced to copy even
// if aliasing was enabled if the input data spans input buffers.
//
// Returns NULL if memory allocation failed, or we reached a premature EOF.
WPI_UPB_INLINE const char* wpi_upb_EpsCopyInputStream_ReadString(
    wpi_upb_EpsCopyInputStream* e, const char** ptr, size_t size,
    wpi_upb_Arena* arena) {
  if (wpi_upb_EpsCopyInputStream_AliasingAvailable(e, *ptr, size)) {
    return wpi_upb_EpsCopyInputStream_ReadStringAliased(e, ptr, size);
  } else {
    // We need to allocate and copy.
    if (!wpi_upb_EpsCopyInputStream_CheckDataSizeAvailable(e, *ptr, size)) {
      return NULL;
    }
    WPI_UPB_ASSERT(arena);
    char* data = (char*)wpi_upb_Arena_Malloc(arena, size);
    if (!data) return NULL;
    const char* ret = wpi_upb_EpsCopyInputStream_Copy(e, *ptr, data, size);
    *ptr = data;
    return ret;
  }
}

WPI_UPB_INLINE void _wpi_upb_EpsCopyInputStream_CheckLimit(wpi_upb_EpsCopyInputStream* e) {
  WPI_UPB_ASSERT(e->limit_ptr == e->end + WPI_UPB_MIN(0, e->limit));
}

// Pushes a limit onto the stack of limits for the current stream.  The limit
// will extend for `size` bytes beyond the position in `ptr`.  Future calls to
// wpi_upb_EpsCopyInputStream_IsDone() will return `true` when the stream position
// reaches this limit.
//
// Returns a delta that the caller must store and supply to PopLimit() below.
WPI_UPB_INLINE int wpi_upb_EpsCopyInputStream_PushLimit(wpi_upb_EpsCopyInputStream* e,
                                                const char* ptr, int size) {
  int limit = size + (int)(ptr - e->end);
  int delta = e->limit - limit;
  _wpi_upb_EpsCopyInputStream_CheckLimit(e);
  WPI_UPB_ASSERT(limit <= e->limit);
  e->limit = limit;
  e->limit_ptr = e->end + WPI_UPB_MIN(0, limit);
  _wpi_upb_EpsCopyInputStream_CheckLimit(e);
  return delta;
}

// Pops the last limit that was pushed on this stream.  This may only be called
// once IsDone() returns true.  The user must pass the delta that was returned
// from PushLimit().
WPI_UPB_INLINE void wpi_upb_EpsCopyInputStream_PopLimit(wpi_upb_EpsCopyInputStream* e,
                                                const char* ptr,
                                                int saved_delta) {
  WPI_UPB_ASSERT(ptr - e->end == e->limit);
  _wpi_upb_EpsCopyInputStream_CheckLimit(e);
  e->limit += saved_delta;
  e->limit_ptr = e->end + WPI_UPB_MIN(0, e->limit);
  _wpi_upb_EpsCopyInputStream_CheckLimit(e);
}

WPI_UPB_INLINE const char* _wpi_upb_EpsCopyInputStream_IsDoneFallbackInline(
    wpi_upb_EpsCopyInputStream* e, const char* ptr, int overrun,
    wpi_upb_EpsCopyInputStream_BufferFlipCallback* callback) {
  if (overrun < e->limit) {
    // Need to copy remaining data into patch buffer.
    WPI_UPB_ASSERT(overrun < kUpb_EpsCopyInputStream_SlopBytes);
    const char* old_end = ptr;
    const char* new_start = &e->patch[0] + overrun;
    memset(e->patch + kUpb_EpsCopyInputStream_SlopBytes, 0,
           kUpb_EpsCopyInputStream_SlopBytes);
    memcpy(e->patch, e->end, kUpb_EpsCopyInputStream_SlopBytes);
    ptr = new_start;
    e->end = &e->patch[kUpb_EpsCopyInputStream_SlopBytes];
    e->limit -= kUpb_EpsCopyInputStream_SlopBytes;
    e->limit_ptr = e->end + e->limit;
    WPI_UPB_ASSERT(ptr < e->limit_ptr);
    if (e->aliasing != kUpb_EpsCopyInputStream_NoAliasing) {
      e->aliasing = (uintptr_t)old_end - (uintptr_t)new_start;
    }
    return callback(e, old_end, new_start);
  } else {
    WPI_UPB_ASSERT(overrun > e->limit);
    e->error = true;
    return callback(e, NULL, NULL);
  }
}

typedef const char* wpi_upb_EpsCopyInputStream_ParseDelimitedFunc(
    wpi_upb_EpsCopyInputStream* e, const char* ptr, void* ctx);

// Tries to perform a fast-path handling of the given delimited message data.
// If the sub-message beginning at `*ptr` and extending for `len` is short and
// fits within this buffer, calls `func` with `ctx` as a parameter, where the
// pushing and popping of limits is handled automatically and with lower cost
// than the normal PushLimit()/PopLimit() sequence.
WPI_UPB_FORCEINLINE bool wpi_upb_EpsCopyInputStream_TryParseDelimitedFast(
    wpi_upb_EpsCopyInputStream* e, const char** ptr, int len,
    wpi_upb_EpsCopyInputStream_ParseDelimitedFunc* func, void* ctx) {
  if (!wpi_upb_EpsCopyInputStream_CheckSubMessageSizeAvailable(e, *ptr, len)) {
    return false;
  }

  // Fast case: Sub-message is <128 bytes and fits in the current buffer.
  // This means we can preserve limit/limit_ptr verbatim.
  const char* saved_limit_ptr = e->limit_ptr;
  int saved_limit = e->limit;
  e->limit_ptr = *ptr + len;
  e->limit = e->limit_ptr - e->end;
  WPI_UPB_ASSERT(e->limit_ptr == e->end + WPI_UPB_MIN(0, e->limit));
  *ptr = func(e, *ptr, ctx);
  e->limit_ptr = saved_limit_ptr;
  e->limit = saved_limit;
  WPI_UPB_ASSERT(e->limit_ptr == e->end + WPI_UPB_MIN(0, e->limit));
  return true;
}

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif  // WPI_UPB_WIRE_EPS_COPY_INPUT_STREAM_H_

#ifndef WPI_UPB_PORT_ATOMIC_H_
#define WPI_UPB_PORT_ATOMIC_H_


#ifdef WPI_UPB_USE_C11_ATOMICS

// IWYU pragma: begin_exports
#include <stdatomic.h>
#include <stdbool.h>
// IWYU pragma: end_exports

#define wpi_upb_Atomic_Init(addr, val) atomic_init(addr, val)
#define wpi_upb_Atomic_Load(addr, order) atomic_load_explicit(addr, order)
#define wpi_upb_Atomic_Store(addr, val, order) \
  atomic_store_explicit(addr, val, order)
#define wpi_upb_Atomic_Add(addr, val, order) \
  atomic_fetch_add_explicit(addr, val, order)
#define wpi_upb_Atomic_Sub(addr, val, order) \
  atomic_fetch_sub_explicit(addr, val, order)
#define wpi_upb_Atomic_Exchange(addr, val, order) \
  atomic_exchange_explicit(addr, val, order)
#define wpi_upb_Atomic_CompareExchangeStrong(addr, expected, desired,      \
                                         success_order, failure_order) \
  atomic_compare_exchange_strong_explicit(addr, expected, desired,     \
                                          success_order, failure_order)
#define wpi_upb_Atomic_CompareExchangeWeak(addr, expected, desired, success_order, \
                                       failure_order)                          \
  atomic_compare_exchange_weak_explicit(addr, expected, desired,               \
                                        success_order, failure_order)

#else  // !WPI_UPB_USE_C11_ATOMICS

#include <string.h>

#define wpi_upb_Atomic_Init(addr, val) (*addr = val)
#define wpi_upb_Atomic_Load(addr, order) (*addr)
#define wpi_upb_Atomic_Store(addr, val, order) (*(addr) = val)
#define wpi_upb_Atomic_Add(addr, val, order) (*(addr) += val)
#define wpi_upb_Atomic_Sub(addr, val, order) (*(addr) -= val)

WPI_UPB_INLINE void* _wpi_upb_NonAtomic_Exchange(void* addr, void* value) {
  void* old;
  memcpy(&old, addr, sizeof(value));
  memcpy(addr, &value, sizeof(value));
  return old;
}

#define wpi_upb_Atomic_Exchange(addr, val, order) _wpi_upb_NonAtomic_Exchange(addr, val)

// `addr` and `expected` are logically double pointers.
WPI_UPB_INLINE bool _wpi_upb_NonAtomic_CompareExchangeStrongP(void* addr,
                                                      void* expected,
                                                      void* desired) {
  if (memcmp(addr, expected, sizeof(desired)) == 0) {
    memcpy(addr, &desired, sizeof(desired));
    return true;
  } else {
    memcpy(expected, addr, sizeof(desired));
    return false;
  }
}

#define wpi_upb_Atomic_CompareExchangeStrong(addr, expected, desired,      \
                                         success_order, failure_order) \
  _wpi_upb_NonAtomic_CompareExchangeStrongP((void*)addr, (void*)expected,  \
                                        (void*)desired)
#define wpi_upb_Atomic_CompareExchangeWeak(addr, expected, desired, success_order, \
                                       failure_order)                          \
  wpi_upb_Atomic_CompareExchangeStrong(addr, expected, desired, 0, 0)

#endif


#endif  // WPI_UPB_PORT_ATOMIC_H_

#ifndef WPI_UPB_MESSAGE_COMPAT_H_
#define WPI_UPB_MESSAGE_COMPAT_H_

#include <stdint.h>


// Must be last.

// upb does not support mixing minitables from different sources but these
// functions are still used by some existing users so for now we make them
// available here. This may or may not change in the future so do not add
// them to new code.

#ifdef __cplusplus
extern "C" {
#endif

const wpi_upb_MiniTableExtension* wpi_upb_Message_ExtensionByIndex(
    const wpi_upb_Message* msg, size_t index);

// Returns the minitable with the given field number, or NULL on failure.
const wpi_upb_MiniTableExtension* wpi_upb_Message_FindExtensionByNumber(
    const wpi_upb_Message* msg, uint32_t field_number);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_MESSAGE_COMPAT_H_ */

// EVERYTHING BELOW THIS LINE IS INTERNAL - DO NOT USE /////////////////////////

#ifndef WPI_UPB_MESSAGE_INTERNAL_MAP_SORTER_H_
#define WPI_UPB_MESSAGE_INTERNAL_MAP_SORTER_H_

#include <stdlib.h>


#ifndef WPI_UPB_MESSAGE_INTERNAL_MAP_ENTRY_H_
#define WPI_UPB_MESSAGE_INTERNAL_MAP_ENTRY_H_

#include <stdint.h>


// Map entries aren't actually stored for map fields, they are only used during
// parsing. (It helps a lot if all map entry messages have the same layout.)
// The mini_table layout code will ensure that all map entries have this layout.
//
// Note that users can and do create map entries directly, which will also use
// this layout.

typedef struct {
  struct wpi_upb_Message message;
  // We only need 2 hasbits max, but due to alignment we'll use 8 bytes here,
  // and the uint64_t helps make this clear.
  uint64_t hasbits;
  union {
    wpi_upb_StringView str;  // For str/bytes.
    wpi_upb_value val;       // For all other types.
    double d[2];         // Padding for 32-bit builds.
  } k;
  union {
    wpi_upb_StringView str;  // For str/bytes.
    wpi_upb_value val;       // For all other types.
    double d[2];         // Padding for 32-bit builds.
  } v;
} wpi_upb_MapEntry;

#endif  // WPI_UPB_MESSAGE_INTERNAL_MAP_ENTRY_H_

// Must be last.

#ifdef __cplusplus
extern "C" {
#endif

// _wpi_upb_mapsorter sorts maps and provides ordered iteration over the entries.
// Since maps can be recursive (map values can be messages which contain other
// maps), _wpi_upb_mapsorter can contain a stack of maps.

typedef struct {
  void const** entries;
  int size;
  int cap;
} _wpi_upb_mapsorter;

typedef struct {
  int start;
  int pos;
  int end;
} _wpi_upb_sortedmap;

WPI_UPB_INLINE void _wpi_upb_mapsorter_init(_wpi_upb_mapsorter* s) {
  s->entries = NULL;
  s->size = 0;
  s->cap = 0;
}

WPI_UPB_INLINE void _wpi_upb_mapsorter_destroy(_wpi_upb_mapsorter* s) {
  if (s->entries) wpi_upb_gfree(s->entries);
}

WPI_UPB_INLINE bool _wpi_upb_sortedmap_next(_wpi_upb_mapsorter* s,
                                    const struct wpi_upb_Map* map,
                                    _wpi_upb_sortedmap* sorted, wpi_upb_MapEntry* ent) {
  if (sorted->pos == sorted->end) return false;
  const wpi_upb_tabent* tabent = (const wpi_upb_tabent*)s->entries[sorted->pos++];
  wpi_upb_StringView key = wpi_upb_tabstrview(tabent->key);
  _wpi_upb_map_fromkey(key, &ent->k, map->key_size);
  wpi_upb_value val = {tabent->val.val};
  _wpi_upb_map_fromvalue(val, &ent->v, map->val_size);
  return true;
}

WPI_UPB_INLINE bool _wpi_upb_sortedmap_nextext(_wpi_upb_mapsorter* s,
                                       _wpi_upb_sortedmap* sorted,
                                       const wpi_upb_Extension** ext) {
  if (sorted->pos == sorted->end) return false;
  *ext = (const wpi_upb_Extension*)s->entries[sorted->pos++];
  return true;
}

WPI_UPB_INLINE void _wpi_upb_mapsorter_popmap(_wpi_upb_mapsorter* s,
                                      _wpi_upb_sortedmap* sorted) {
  s->size = sorted->start;
}

bool _wpi_upb_mapsorter_pushmap(_wpi_upb_mapsorter* s, wpi_upb_FieldType key_type,
                            const struct wpi_upb_Map* map, _wpi_upb_sortedmap* sorted);

bool _wpi_upb_mapsorter_pushexts(_wpi_upb_mapsorter* s, const wpi_upb_Extension* exts,
                             size_t count, _wpi_upb_sortedmap* sorted);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_MESSAGE_INTERNAL_MAP_SORTER_H_ */

#ifndef WPI_UPB_BASE_INTERNAL_LOG2_H_
#define WPI_UPB_BASE_INTERNAL_LOG2_H_

// Must be last.

#ifdef __cplusplus
extern "C" {
#endif

WPI_UPB_INLINE int wpi_upb_Log2Ceiling(int x) {
  if (x <= 1) return 0;
#ifdef __GNUC__
  return 32 - __builtin_clz(x - 1);
#else
  int lg2 = 0;
  while ((1 << lg2) < x) lg2++;
  return lg2;
#endif
}

WPI_UPB_INLINE int wpi_upb_Log2CeilingSize(int x) { return 1 << wpi_upb_Log2Ceiling(x); }

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_BASE_INTERNAL_LOG2_H_ */

#ifndef WPI_UPB_MESSAGE_COMPARE_H_
#define WPI_UPB_MESSAGE_COMPARE_H_

#include <stddef.h>


// Must be last.

enum {
  // If set, wpi_upb_Message_IsEqual() will attempt to compare unknown fields.
  // By its very nature this comparison is inexact.
  kUpb_CompareOption_IncludeUnknownFields = (1 << 0)
};

#ifdef __cplusplus
extern "C" {
#endif

// Returns true if no known fields or extensions are set in the message.
WPI_UPB_API bool wpi_upb_Message_IsEmpty(const wpi_upb_Message* msg,
                                 const wpi_upb_MiniTable* m);

WPI_UPB_API bool wpi_upb_Message_IsEqual(const wpi_upb_Message* msg1,
                                 const wpi_upb_Message* msg2,
                                 const wpi_upb_MiniTable* m, int options);

// If |ctype| is a message then |m| must point to its minitable.
WPI_UPB_API_INLINE bool wpi_upb_MessageValue_IsEqual(wpi_upb_MessageValue val1,
                                             wpi_upb_MessageValue val2,
                                             wpi_upb_CType ctype,
                                             const wpi_upb_MiniTable* m,
                                             int options) {
  switch (ctype) {
    case kUpb_CType_Bool:
      return val1.bool_val == val2.bool_val;

    case kUpb_CType_Float:
    case kUpb_CType_Int32:
    case kUpb_CType_UInt32:
    case kUpb_CType_Enum:
      return val1.int32_val == val2.int32_val;

    case kUpb_CType_Double:
    case kUpb_CType_Int64:
    case kUpb_CType_UInt64:
      return val1.int64_val == val2.int64_val;

    case kUpb_CType_String:
    case kUpb_CType_Bytes:
      return wpi_upb_StringView_IsEqual(val1.str_val, val2.str_val);

    case kUpb_CType_Message:
      return wpi_upb_Message_IsEqual(val1.msg_val, val2.msg_val, m, options);

    default:
      WPI_UPB_UNREACHABLE();
      return false;
  }
}

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif  // WPI_UPB_MESSAGE_COMPARE_H_

#ifndef WPI_UPB_MESSAGE_INTERNAL_COMPARE_UNKNOWN_H_
#define WPI_UPB_MESSAGE_INTERNAL_COMPARE_UNKNOWN_H_

#include <stddef.h>

// Must be last.

#ifdef __cplusplus
extern "C" {
#endif

// Returns true if unknown fields from the two messages are equal when sorted
// and varints are made canonical.
//
// This function is discouraged, as the comparison is inherently lossy without
// schema data:
//
//  1. We don't know whether delimited fields are sub-messages. Unknown
//     sub-messages will therefore not have their fields sorted and varints
//     canonicalized.
//  2. We don't know about oneof/non-repeated fields, which should semantically
//     discard every value except the last.

typedef enum {
  kUpb_UnknownCompareResult_Equal = 0,
  kUpb_UnknownCompareResult_NotEqual = 1,
  kUpb_UnknownCompareResult_OutOfMemory = 2,
  kUpb_UnknownCompareResult_MaxDepthExceeded = 3,
} wpi_upb_UnknownCompareResult;

wpi_upb_UnknownCompareResult WPI_UPB_PRIVATE(_wpi_upb_Message_UnknownFieldsAreEqual)(
    const char* buf1, size_t size1, const char* buf2, size_t size2,
    int max_depth);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_MESSAGE_INTERNAL_COMPARE_UNKNOWN_H_ */

#ifndef THIRD_PARTY_WPI_UPB_WPI_UPB_MESSAGE_INTERNAL_ITERATOR_H_
#define THIRD_PARTY_WPI_UPB_WPI_UPB_MESSAGE_INTERNAL_ITERATOR_H_

#include <stddef.h>


// Must be last.

#define kUpb_BaseField_Begin ((size_t)-1)
#define kUpb_Extension_Begin ((size_t)-1)

bool WPI_UPB_PRIVATE(_wpi_upb_Message_NextBaseField)(const wpi_upb_Message* msg,
                                             const wpi_upb_MiniTable* m,
                                             const wpi_upb_MiniTableField** out_f,
                                             wpi_upb_MessageValue* out_v,
                                             size_t* iter);

bool WPI_UPB_PRIVATE(_wpi_upb_Message_NextExtension)(
    const wpi_upb_Message* msg, const wpi_upb_MiniTable* m,
    const wpi_upb_MiniTableExtension** out_e, wpi_upb_MessageValue* out_v,
    size_t* iter);
#endif  // THIRD_PARTY_WPI_UPB_WPI_UPB_MESSAGE_INTERNAL_ITERATOR_H_

#ifndef WPI_UPB_MESSAGE_COPY_H_
#define WPI_UPB_MESSAGE_COPY_H_


// Must be last.

#ifdef __cplusplus
extern "C" {
#endif

// Deep clones a message using the provided target arena.
wpi_upb_Message* wpi_upb_Message_DeepClone(const wpi_upb_Message* msg,
                                   const wpi_upb_MiniTable* m, wpi_upb_Arena* arena);

// Shallow clones a message using the provided target arena.
wpi_upb_Message* wpi_upb_Message_ShallowClone(const wpi_upb_Message* msg,
                                      const wpi_upb_MiniTable* m, wpi_upb_Arena* arena);

// Deep clones array contents.
wpi_upb_Array* wpi_upb_Array_DeepClone(const wpi_upb_Array* array, wpi_upb_CType value_type,
                               const wpi_upb_MiniTable* sub, wpi_upb_Arena* arena);

// Deep clones map contents.
wpi_upb_Map* wpi_upb_Map_DeepClone(const wpi_upb_Map* map, wpi_upb_CType key_type,
                           wpi_upb_CType value_type,
                           const wpi_upb_MiniTable* map_entry_table,
                           wpi_upb_Arena* arena);

// Deep copies the message from src to dst.
bool wpi_upb_Message_DeepCopy(wpi_upb_Message* dst, const wpi_upb_Message* src,
                          const wpi_upb_MiniTable* m, wpi_upb_Arena* arena);

// Shallow copies the message from src to dst.
void wpi_upb_Message_ShallowCopy(wpi_upb_Message* dst, const wpi_upb_Message* src,
                             const wpi_upb_MiniTable* m);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif  // WPI_UPB_MESSAGE_COPY_H_
#ifndef THIRD_PARTY_WPI_UPB_WPI_UPB_MESSAGE_MERGE_H_
#define THIRD_PARTY_WPI_UPB_WPI_UPB_MESSAGE_MERGE_H_


// Must be last.

#ifdef __cplusplus
extern "C" {
#endif

WPI_UPB_API bool wpi_upb_Message_MergeFrom(wpi_upb_Message* dst, const wpi_upb_Message* src,
                                   const wpi_upb_MiniTable* mt,
                                   const wpi_upb_ExtensionRegistry* extreg,
                                   wpi_upb_Arena* arena);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif  // THIRD_PARTY_WPI_UPB_WPI_UPB_MESSAGE_MERGE_H_

#ifndef WPI_UPB_MINI_DESCRIPTOR_INTERNAL_BASE92_H_
#define WPI_UPB_MINI_DESCRIPTOR_INTERNAL_BASE92_H_

#include <stdint.h>


// Must be last.

#ifdef __cplusplus
extern "C" {
#endif

WPI_UPB_INLINE char _wpi_upb_ToBase92(int8_t ch) {
  extern const char _kUpb_ToBase92[];
  WPI_UPB_ASSERT(0 <= ch && ch < 92);
  return _kUpb_ToBase92[ch];
}

WPI_UPB_INLINE char _wpi_upb_FromBase92(uint8_t ch) {
  extern const int8_t _kUpb_FromBase92[];
  if (' ' > ch || ch > '~') return -1;
  return _kUpb_FromBase92[ch - ' '];
}

WPI_UPB_INLINE const char* _wpi_upb_Base92_DecodeVarint(const char* ptr,
                                                const char* end, char first_ch,
                                                uint8_t min, uint8_t max,
                                                uint32_t* out_val) {
  uint32_t val = 0;
  uint32_t shift = 0;
  const int bits_per_char =
      wpi_upb_Log2Ceiling(_wpi_upb_FromBase92(max) - _wpi_upb_FromBase92(min));
  char ch = first_ch;
  while (1) {
    uint32_t bits = _wpi_upb_FromBase92(ch) - _wpi_upb_FromBase92(min);
    val |= bits << shift;
    if (ptr == end || *ptr < min || max < *ptr) {
      *out_val = val;
      WPI_UPB_ASSUME(ptr != NULL);
      return ptr;
    }
    ch = *ptr++;
    shift += bits_per_char;
    if (shift >= 32) return NULL;
  }
}

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif  // WPI_UPB_MINI_DESCRIPTOR_INTERNAL_BASE92_H_

#ifndef WPI_UPB_MINI_DESCRIPTOR_INTERNAL_DECODER_H_
#define WPI_UPB_MINI_DESCRIPTOR_INTERNAL_DECODER_H_


// Must be last.

// wpi_upb_MdDecoder: used internally for decoding MiniDescriptors for messages,
// extensions, and enums.
typedef struct {
  const char* end;
  wpi_upb_Status* status;
  jmp_buf err;
} wpi_upb_MdDecoder;

WPI_UPB_PRINTF(2, 3)
WPI_UPB_NORETURN WPI_UPB_INLINE void wpi_upb_MdDecoder_ErrorJmp(wpi_upb_MdDecoder* d,
                                                    const char* fmt, ...) {
  if (d->status) {
    va_list argp;
    wpi_upb_Status_SetErrorMessage(d->status, "Error building mini table: ");
    va_start(argp, fmt);
    wpi_upb_Status_VAppendErrorFormat(d->status, fmt, argp);
    va_end(argp);
  }
  WPI_UPB_LONGJMP(d->err, 1);
}

WPI_UPB_INLINE void wpi_upb_MdDecoder_CheckOutOfMemory(wpi_upb_MdDecoder* d,
                                               const void* ptr) {
  if (!ptr) wpi_upb_MdDecoder_ErrorJmp(d, "Out of memory");
}

WPI_UPB_INLINE const char* wpi_upb_MdDecoder_DecodeBase92Varint(
    wpi_upb_MdDecoder* d, const char* ptr, char first_ch, uint8_t min, uint8_t max,
    uint32_t* out_val) {
  ptr = _wpi_upb_Base92_DecodeVarint(ptr, d->end, first_ch, min, max, out_val);
  if (!ptr) wpi_upb_MdDecoder_ErrorJmp(d, "Overlong varint");
  return ptr;
}


#endif  // WPI_UPB_MINI_DESCRIPTOR_INTERNAL_DECODER_H_

#ifndef WPI_UPB_MINI_DESCRIPTOR_INTERNAL_WIRE_CONSTANTS_H_
#define WPI_UPB_MINI_DESCRIPTOR_INTERNAL_WIRE_CONSTANTS_H_


// Must be last.

typedef enum {
  kUpb_EncodedType_Double = 0,
  kUpb_EncodedType_Float = 1,
  kUpb_EncodedType_Fixed32 = 2,
  kUpb_EncodedType_Fixed64 = 3,
  kUpb_EncodedType_SFixed32 = 4,
  kUpb_EncodedType_SFixed64 = 5,
  kUpb_EncodedType_Int32 = 6,
  kUpb_EncodedType_UInt32 = 7,
  kUpb_EncodedType_SInt32 = 8,
  kUpb_EncodedType_Int64 = 9,
  kUpb_EncodedType_UInt64 = 10,
  kUpb_EncodedType_SInt64 = 11,
  kUpb_EncodedType_OpenEnum = 12,
  kUpb_EncodedType_Bool = 13,
  kUpb_EncodedType_Bytes = 14,
  kUpb_EncodedType_String = 15,
  kUpb_EncodedType_Group = 16,
  kUpb_EncodedType_Message = 17,
  kUpb_EncodedType_ClosedEnum = 18,

  kUpb_EncodedType_RepeatedBase = 20,
} wpi_upb_EncodedType;

typedef enum {
  kUpb_EncodedFieldModifier_FlipPacked = 1 << 0,
  kUpb_EncodedFieldModifier_IsRequired = 1 << 1,
  kUpb_EncodedFieldModifier_IsProto3Singular = 1 << 2,
  kUpb_EncodedFieldModifier_FlipValidateUtf8 = 1 << 3,
} wpi_upb_EncodedFieldModifier;

enum {
  kUpb_EncodedValue_MinField = ' ',
  kUpb_EncodedValue_MaxField = 'I',
  kUpb_EncodedValue_MinModifier = 'L',
  kUpb_EncodedValue_MaxModifier = '[',
  kUpb_EncodedValue_End = '^',
  kUpb_EncodedValue_MinSkip = '_',
  kUpb_EncodedValue_MaxSkip = '~',
  kUpb_EncodedValue_OneofSeparator = '~',
  kUpb_EncodedValue_FieldSeparator = '|',
  kUpb_EncodedValue_MinOneofField = ' ',
  kUpb_EncodedValue_MaxOneofField = 'b',
  kUpb_EncodedValue_MaxEnumMask = 'A',
};

enum {
  kUpb_EncodedVersion_EnumV1 = '!',
  kUpb_EncodedVersion_ExtensionV1 = '#',
  kUpb_EncodedVersion_MapV1 = '%',
  kUpb_EncodedVersion_MessageV1 = '$',
  kUpb_EncodedVersion_MessageSetV1 = '&',
};


#endif  // WPI_UPB_MINI_DESCRIPTOR_INTERNAL_WIRE_CONSTANTS_H_

#ifndef WPI_UPB_MINI_DESCRIPTOR_INTERNAL_MODIFIERS_H_
#define WPI_UPB_MINI_DESCRIPTOR_INTERNAL_MODIFIERS_H_

// Must be last.

typedef enum {
  kUpb_FieldModifier_IsRepeated = 1 << 0,
  kUpb_FieldModifier_IsPacked = 1 << 1,
  kUpb_FieldModifier_IsClosedEnum = 1 << 2,
  kUpb_FieldModifier_IsProto3Singular = 1 << 3,
  kUpb_FieldModifier_IsRequired = 1 << 4,
  kUpb_FieldModifier_ValidateUtf8 = 1 << 5,
} kUpb_FieldModifier;

// These modifiers are also used on the wire.
typedef enum {
  kUpb_MessageModifier_ValidateUtf8 = 1 << 0,
  kUpb_MessageModifier_DefaultIsPacked = 1 << 1,
  kUpb_MessageModifier_IsExtendable = 1 << 2,
} kUpb_MessageModifier;


#endif  // WPI_UPB_MINI_DESCRIPTOR_INTERNAL_MODIFIERS_H_

#ifndef WPI_UPB_MINI_TABLE_COMPAT_H_
#define WPI_UPB_MINI_TABLE_COMPAT_H_


// Must be last.

// upb does not support mixing minitables from different sources but these
// functions are still used by some existing users so for now we make them
// available here. This may or may not change in the future so do not add
// them to new code.

#ifdef __cplusplus
extern "C" {
#endif

// Checks if memory layout of src is compatible with dst.
bool wpi_upb_MiniTable_Compatible(const wpi_upb_MiniTable* src,
                              const wpi_upb_MiniTable* dst);

typedef enum {
  kUpb_MiniTableEquals_NotEqual,
  kUpb_MiniTableEquals_Equal,
  kUpb_MiniTableEquals_OutOfMemory,
} wpi_upb_MiniTableEquals_Status;

// Checks equality of mini tables originating from different language runtimes.
wpi_upb_MiniTableEquals_Status wpi_upb_MiniTable_Equals(const wpi_upb_MiniTable* src,
                                                const wpi_upb_MiniTable* dst);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_MINI_TABLE_COMPAT_H_ */

#ifndef WPI_UPB_HASH_INT_TABLE_H_
#define WPI_UPB_HASH_INT_TABLE_H_


// Must be last.

typedef struct {
  wpi_upb_table t;              // For entries that don't fit in the array part.
  const wpi_upb_tabval* array;  // Array part of the table. See const note above.
  size_t array_size;        // Array part size.
  size_t array_count;       // Array part number of elements.
} wpi_upb_inttable;

#ifdef __cplusplus
extern "C" {
#endif

// Initialize a table. If memory allocation failed, false is returned and
// the table is uninitialized.
bool wpi_upb_inttable_init(wpi_upb_inttable* table, wpi_upb_Arena* a);

// Returns the number of values in the table.
size_t wpi_upb_inttable_count(const wpi_upb_inttable* t);

// Inserts the given key into the hashtable with the given value.
// The key must not already exist in the hash table.
// The value must not be UINTPTR_MAX.
//
// If a table resize was required but memory allocation failed, false is
// returned and the table is unchanged.
bool wpi_upb_inttable_insert(wpi_upb_inttable* t, uintptr_t key, wpi_upb_value val,
                         wpi_upb_Arena* a);

// Looks up key in this table, returning "true" if the key was found.
// If v is non-NULL, copies the value for this key into *v.
bool wpi_upb_inttable_lookup(const wpi_upb_inttable* t, uintptr_t key, wpi_upb_value* v);

// Removes an item from the table. Returns true if the remove was successful,
// and stores the removed item in *val if non-NULL.
bool wpi_upb_inttable_remove(wpi_upb_inttable* t, uintptr_t key, wpi_upb_value* val);

// Updates an existing entry in an inttable.
// If the entry does not exist, returns false and does nothing.
// Unlike insert/remove, this does not invalidate iterators.
bool wpi_upb_inttable_replace(wpi_upb_inttable* t, uintptr_t key, wpi_upb_value val);

// Optimizes the table for the current set of entries, for both memory use and
// lookup time. Client should call this after all entries have been inserted;
// inserting more entries is legal, but will likely require a table resize.
void wpi_upb_inttable_compact(wpi_upb_inttable* t, wpi_upb_Arena* a);

// Iteration over inttable:
//
//   intptr_t iter = WPI_UPB_INTTABLE_BEGIN;
//   uintptr_t key;
//   wpi_upb_value val;
//   while (wpi_upb_inttable_next(t, &key, &val, &iter)) {
//      // ...
//   }

#define WPI_UPB_INTTABLE_BEGIN -1

bool wpi_upb_inttable_next(const wpi_upb_inttable* t, uintptr_t* key, wpi_upb_value* val,
                       intptr_t* iter);
void wpi_upb_inttable_removeiter(wpi_upb_inttable* t, intptr_t* iter);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_HASH_INT_TABLE_H_ */

#ifndef WPI_UPB_WIRE_INTERNAL_CONSTANTS_H_
#define WPI_UPB_WIRE_INTERNAL_CONSTANTS_H_

#define kUpb_WireFormat_DefaultDepthLimit 100

// MessageSet wire format is:
//   message MessageSet {
//     repeated group Item = 1 {
//       required int32 type_id = 2;
//       required bytes message = 3;
//     }
//   }

enum {
  kUpb_MsgSet_Item = 1,
  kUpb_MsgSet_TypeId = 2,
  kUpb_MsgSet_Message = 3,
};

#endif /* WPI_UPB_WIRE_INTERNAL_CONSTANTS_H_ */

/*
 * Internal implementation details of the decoder that are shared between
 * decode.c and decode_fast.c.
 */

#ifndef WPI_UPB_WIRE_INTERNAL_DECODER_H_
#define WPI_UPB_WIRE_INTERNAL_DECODER_H_

#include "wpi/utf8_range.h"

// Must be last.

#define DECODE_NOGROUP (uint32_t) - 1

typedef struct wpi_upb_Decoder {
  wpi_upb_EpsCopyInputStream input;
  const wpi_upb_ExtensionRegistry* extreg;
  const char* unknown;       // Start of unknown data, preserve at buffer flip
  wpi_upb_Message* unknown_msg;  // Pointer to preserve data to
  int depth;                 // Tracks recursion depth to bound stack usage.
  uint32_t end_group;  // field number of END_GROUP tag, else DECODE_NOGROUP.
  uint16_t options;
  bool missing_required;
  union {
    wpi_upb_Arena arena;
    void* foo[WPI_UPB_ARENA_SIZE_HACK];
  };
  wpi_upb_DecodeStatus status;
  jmp_buf err;

#ifndef NDEBUG
  const char* debug_tagstart;
  const char* debug_valstart;
#endif
} wpi_upb_Decoder;

/* Error function that will abort decoding with longjmp(). We can't declare this
 * WPI_UPB_NORETURN, even though it is appropriate, because if we do then compilers
 * will "helpfully" refuse to tailcall to it
 * (see: https://stackoverflow.com/a/55657013), which will defeat a major goal
 * of our optimizations. That is also why we must declare it in a separate file,
 * otherwise the compiler will see that it calls longjmp() and deduce that it is
 * noreturn. */
const char* _wpi_upb_FastDecoder_ErrorJmp(wpi_upb_Decoder* d, int status);

extern const uint8_t wpi_upb_utf8_offsets[];

WPI_UPB_INLINE
bool _wpi_upb_Decoder_VerifyUtf8Inline(const char* ptr, int len) {
  return utf8_range_IsValid(ptr, len);
}

const char* _wpi_upb_Decoder_CheckRequired(wpi_upb_Decoder* d, const char* ptr,
                                       const wpi_upb_Message* msg,
                                       const wpi_upb_MiniTable* m);

/* x86-64 pointers always have the high 16 bits matching. So we can shift
 * left 8 and right 8 without loss of information. */
WPI_UPB_INLINE intptr_t decode_totable(const wpi_upb_MiniTable* tablep) {
  return ((intptr_t)tablep << 8) | tablep->WPI_UPB_PRIVATE(table_mask);
}

WPI_UPB_INLINE const wpi_upb_MiniTable* decode_totablep(intptr_t table) {
  return (const wpi_upb_MiniTable*)(table >> 8);
}

const char* _wpi_upb_Decoder_IsDoneFallback(wpi_upb_EpsCopyInputStream* e,
                                        const char* ptr, int overrun);

WPI_UPB_INLINE bool _wpi_upb_Decoder_IsDone(wpi_upb_Decoder* d, const char** ptr) {
  return wpi_upb_EpsCopyInputStream_IsDoneWithCallback(
      &d->input, ptr, &_wpi_upb_Decoder_IsDoneFallback);
}

WPI_UPB_INLINE const char* _wpi_upb_Decoder_BufferFlipCallback(
    wpi_upb_EpsCopyInputStream* e, const char* old_end, const char* new_start) {
  wpi_upb_Decoder* d = (wpi_upb_Decoder*)e;
  if (!old_end) _wpi_upb_FastDecoder_ErrorJmp(d, kUpb_DecodeStatus_Malformed);

  if (d->unknown) {
    if (!WPI_UPB_PRIVATE(_wpi_upb_Message_AddUnknown)(
            d->unknown_msg, d->unknown, old_end - d->unknown, &d->arena)) {
      _wpi_upb_FastDecoder_ErrorJmp(d, kUpb_DecodeStatus_OutOfMemory);
    }
    d->unknown = new_start;
  }
  return new_start;
}

#if WPI_UPB_FASTTABLE
WPI_UPB_INLINE
const char* _wpi_upb_FastDecoder_TagDispatch(wpi_upb_Decoder* d, const char* ptr,
                                         wpi_upb_Message* msg, intptr_t table,
                                         uint64_t hasbits, uint64_t tag) {
  const wpi_upb_MiniTable* table_p = decode_totablep(table);
  uint8_t mask = table;
  uint64_t data;
  size_t idx = tag & mask;
  WPI_UPB_ASSUME((idx & 7) == 0);
  idx >>= 3;
  data = table_p->WPI_UPB_PRIVATE(fasttable)[idx].field_data ^ tag;
  WPI_UPB_MUSTTAIL return table_p->WPI_UPB_PRIVATE(fasttable)[idx].field_parser(
      d, ptr, msg, table, hasbits, data);
}
#endif

WPI_UPB_INLINE uint32_t _wpi_upb_FastDecoder_LoadTag(const char* ptr) {
  uint16_t tag;
  memcpy(&tag, ptr, 2);
  return tag;
}


#endif /* WPI_UPB_WIRE_INTERNAL_DECODER_H_ */

#ifndef WPI_UPB_WIRE_READER_H_
#define WPI_UPB_WIRE_READER_H_


#ifndef WPI_UPB_WIRE_INTERNAL_READER_H_
#define WPI_UPB_WIRE_INTERNAL_READER_H_

// Must be last.

#define kUpb_WireReader_WireTypeBits 3
#define kUpb_WireReader_WireTypeMask 7

typedef struct {
  const char* ptr;
  uint64_t val;
} WPI_UPB_PRIVATE(_wpi_upb_WireReader_LongVarint);

#ifdef __cplusplus
extern "C" {
#endif

WPI_UPB_PRIVATE(_wpi_upb_WireReader_LongVarint)
WPI_UPB_PRIVATE(_wpi_upb_WireReader_ReadLongVarint)(const char* ptr, uint64_t val);

WPI_UPB_FORCEINLINE const char* WPI_UPB_PRIVATE(_wpi_upb_WireReader_ReadVarint)(
    const char* ptr, uint64_t* val, int maxlen, uint64_t maxval) {
  uint64_t byte = (uint8_t)*ptr;
  if (WPI_UPB_LIKELY((byte & 0x80) == 0)) {
    *val = (uint32_t)byte;
    return ptr + 1;
  }
  const char* start = ptr;
  WPI_UPB_PRIVATE(_wpi_upb_WireReader_LongVarint)
  res = WPI_UPB_PRIVATE(_wpi_upb_WireReader_ReadLongVarint)(ptr, byte);
  if (!res.ptr || (maxlen < 10 && res.ptr - start > maxlen) ||
      res.val > maxval) {
    return NULL;  // Malformed.
  }
  *val = res.val;
  return res.ptr;
}

WPI_UPB_API_INLINE uint32_t wpi_upb_WireReader_GetFieldNumber(uint32_t tag) {
  return tag >> kUpb_WireReader_WireTypeBits;
}

WPI_UPB_API_INLINE uint8_t wpi_upb_WireReader_GetWireType(uint32_t tag) {
  return tag & kUpb_WireReader_WireTypeMask;
}

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif  // WPI_UPB_WIRE_INTERNAL_READER_H_

#ifndef WPI_UPB_WIRE_TYPES_H_
#define WPI_UPB_WIRE_TYPES_H_

// A list of types as they are encoded on the wire.
typedef enum {
  kUpb_WireType_Varint = 0,
  kUpb_WireType_64Bit = 1,
  kUpb_WireType_Delimited = 2,
  kUpb_WireType_StartGroup = 3,
  kUpb_WireType_EndGroup = 4,
  kUpb_WireType_32Bit = 5
} wpi_upb_WireType;

#endif /* WPI_UPB_WIRE_TYPES_H_ */

// Must be last.

// The wpi_upb_WireReader interface is suitable for general-purpose parsing of
// protobuf binary wire format. It is designed to be used along with
// wpi_upb_EpsCopyInputStream for buffering, and all parsing routines in this file
// assume that at least kUpb_EpsCopyInputStream_SlopBytes worth of data is
// available to read without any bounds checks.

#ifdef __cplusplus
extern "C" {
#endif

// Parses a tag into `tag`, and returns a pointer past the end of the tag, or
// NULL if there was an error in the tag data.
//
// REQUIRES: there must be at least 10 bytes of data available at `ptr`.
// Bounds checks must be performed before calling this function, preferably
// by calling wpi_upb_EpsCopyInputStream_IsDone().
WPI_UPB_FORCEINLINE const char* wpi_upb_WireReader_ReadTag(const char* ptr,
                                                   uint32_t* tag) {
  uint64_t val;
  ptr = WPI_UPB_PRIVATE(_wpi_upb_WireReader_ReadVarint)(ptr, &val, 5, UINT32_MAX);
  if (!ptr) return NULL;
  *tag = val;
  return ptr;
}

// Given a tag, returns the field number.
WPI_UPB_API_INLINE uint32_t wpi_upb_WireReader_GetFieldNumber(uint32_t tag);

// Given a tag, returns the wire type.
WPI_UPB_API_INLINE uint8_t wpi_upb_WireReader_GetWireType(uint32_t tag);

WPI_UPB_INLINE const char* wpi_upb_WireReader_ReadVarint(const char* ptr,
                                                 uint64_t* val) {
  return WPI_UPB_PRIVATE(_wpi_upb_WireReader_ReadVarint)(ptr, val, 10, UINT64_MAX);
}

// Skips data for a varint, returning a pointer past the end of the varint, or
// NULL if there was an error in the varint data.
//
// REQUIRES: there must be at least 10 bytes of data available at `ptr`.
// Bounds checks must be performed before calling this function, preferably
// by calling wpi_upb_EpsCopyInputStream_IsDone().
WPI_UPB_INLINE const char* wpi_upb_WireReader_SkipVarint(const char* ptr) {
  uint64_t val;
  return wpi_upb_WireReader_ReadVarint(ptr, &val);
}

// Reads a varint indicating the size of a delimited field into `size`, or
// NULL if there was an error in the varint data.
//
// REQUIRES: there must be at least 10 bytes of data available at `ptr`.
// Bounds checks must be performed before calling this function, preferably
// by calling wpi_upb_EpsCopyInputStream_IsDone().
WPI_UPB_INLINE const char* wpi_upb_WireReader_ReadSize(const char* ptr, int* size) {
  uint64_t size64;
  ptr = wpi_upb_WireReader_ReadVarint(ptr, &size64);
  if (!ptr || size64 >= INT32_MAX) return NULL;
  *size = size64;
  return ptr;
}

// Reads a fixed32 field, performing byte swapping if necessary.
//
// REQUIRES: there must be at least 4 bytes of data available at `ptr`.
// Bounds checks must be performed before calling this function, preferably
// by calling wpi_upb_EpsCopyInputStream_IsDone().
WPI_UPB_INLINE const char* wpi_upb_WireReader_ReadFixed32(const char* ptr, void* val) {
  uint32_t uval;
  memcpy(&uval, ptr, 4);
  uval = wpi_upb_BigEndian32(uval);
  memcpy(val, &uval, 4);
  return ptr + 4;
}

// Reads a fixed64 field, performing byte swapping if necessary.
//
// REQUIRES: there must be at least 4 bytes of data available at `ptr`.
// Bounds checks must be performed before calling this function, preferably
// by calling wpi_upb_EpsCopyInputStream_IsDone().
WPI_UPB_INLINE const char* wpi_upb_WireReader_ReadFixed64(const char* ptr, void* val) {
  uint64_t uval;
  memcpy(&uval, ptr, 8);
  uval = wpi_upb_BigEndian64(uval);
  memcpy(val, &uval, 8);
  return ptr + 8;
}

const char* WPI_UPB_PRIVATE(_wpi_upb_WireReader_SkipGroup)(
    const char* ptr, uint32_t tag, int depth_limit,
    wpi_upb_EpsCopyInputStream* stream);

// Skips data for a group, returning a pointer past the end of the group, or
// NULL if there was an error parsing the group.  The `tag` argument should be
// the start group tag that begins the group.  The `depth_limit` argument
// indicates how many levels of recursion the group is allowed to have before
// reporting a parse error (this limit exists to protect against stack
// overflow).
//
// TODO: evaluate how the depth_limit should be specified. Do users need
// control over this?
WPI_UPB_INLINE const char* wpi_upb_WireReader_SkipGroup(
    const char* ptr, uint32_t tag, wpi_upb_EpsCopyInputStream* stream) {
  return WPI_UPB_PRIVATE(_wpi_upb_WireReader_SkipGroup)(ptr, tag, 100, stream);
}

WPI_UPB_INLINE const char* _wpi_upb_WireReader_SkipValue(
    const char* ptr, uint32_t tag, int depth_limit,
    wpi_upb_EpsCopyInputStream* stream) {
  switch (wpi_upb_WireReader_GetWireType(tag)) {
    case kUpb_WireType_Varint:
      return wpi_upb_WireReader_SkipVarint(ptr);
    case kUpb_WireType_32Bit:
      return ptr + 4;
    case kUpb_WireType_64Bit:
      return ptr + 8;
    case kUpb_WireType_Delimited: {
      int size;
      ptr = wpi_upb_WireReader_ReadSize(ptr, &size);
      if (!ptr) return NULL;
      ptr += size;
      return ptr;
    }
    case kUpb_WireType_StartGroup:
      return WPI_UPB_PRIVATE(_wpi_upb_WireReader_SkipGroup)(ptr, tag, depth_limit,
                                                    stream);
    case kUpb_WireType_EndGroup:
      return NULL;  // Should be handled before now.
    default:
      return NULL;  // Unknown wire type.
  }
}

// Skips data for a wire value of any type, returning a pointer past the end of
// the data, or NULL if there was an error parsing the group. The `tag` argument
// should be the tag that was just parsed. The `depth_limit` argument indicates
// how many levels of recursion a group is allowed to have before reporting a
// parse error (this limit exists to protect against stack overflow).
//
// REQUIRES: there must be at least 10 bytes of data available at `ptr`.
// Bounds checks must be performed before calling this function, preferably
// by calling wpi_upb_EpsCopyInputStream_IsDone().
//
// TODO: evaluate how the depth_limit should be specified. Do users need
// control over this?
WPI_UPB_INLINE const char* wpi_upb_WireReader_SkipValue(
    const char* ptr, uint32_t tag, wpi_upb_EpsCopyInputStream* stream) {
  return _wpi_upb_WireReader_SkipValue(ptr, tag, 100, stream);
}

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif  // WPI_UPB_WIRE_READER_H_

#ifndef WPI_UPB_LEX_ATOI_H_
#define WPI_UPB_LEX_ATOI_H_

#include <stdint.h>

// Must be last.

#ifdef __cplusplus
extern "C" {
#endif

// We use these hand-written routines instead of strto[u]l() because the "long
// long" variants aren't in c89. Also our version allows setting a ptr limit.
// Return the new position of the pointer after parsing the int, or NULL on
// integer overflow.

const char* wpi_upb_BufToUint64(const char* ptr, const char* end, uint64_t* val);
const char* wpi_upb_BufToInt64(const char* ptr, const char* end, int64_t* val,
                           bool* is_neg);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_LEX_ATOI_H_ */

#ifndef WPI_UPB_LEX_ROUND_TRIP_H_
#define WPI_UPB_LEX_ROUND_TRIP_H_

// Must be last.

// Encodes a float or double that is round-trippable, but as short as possible.
// These routines are not fully optimal (not guaranteed to be shortest), but are
// short-ish and match the implementation that has been used in protobuf since
// the beginning.

// The given buffer size must be at least kUpb_RoundTripBufferSize.
enum { kUpb_RoundTripBufferSize = 32 };

#ifdef __cplusplus
extern "C" {
#endif

void _wpi_upb_EncodeRoundTripDouble(double val, char* buf, size_t size);
void _wpi_upb_EncodeRoundTripFloat(float val, char* buf, size_t size);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_LEX_ROUND_TRIP_H_ */

#ifndef WPI_UPB_LEX_STRTOD_H_
#define WPI_UPB_LEX_STRTOD_H_

// Must be last.

#ifdef __cplusplus
extern "C" {
#endif

double _wpi_upb_NoLocaleStrtod(const char *str, char **endptr);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_LEX_STRTOD_H_ */

#ifndef WPI_UPB_LEX_UNICODE_H_
#define WPI_UPB_LEX_UNICODE_H_

#include <stdint.h>

// Must be last.

#ifdef __cplusplus
extern "C" {
#endif

// Returns true iff a codepoint is the value for a high surrogate.
WPI_UPB_INLINE bool wpi_upb_Unicode_IsHigh(uint32_t cp) {
  return (cp >= 0xd800 && cp <= 0xdbff);
}

// Returns true iff a codepoint is the value for a low surrogate.
WPI_UPB_INLINE bool wpi_upb_Unicode_IsLow(uint32_t cp) {
  return (cp >= 0xdc00 && cp <= 0xdfff);
}

// Returns the high 16-bit surrogate value for a supplementary codepoint.
// Does not sanity-check the input.
WPI_UPB_INLINE uint16_t wpi_upb_Unicode_ToHigh(uint32_t cp) {
  return (cp >> 10) + 0xd7c0;
}

// Returns the low 16-bit surrogate value for a supplementary codepoint.
// Does not sanity-check the input.
WPI_UPB_INLINE uint16_t wpi_upb_Unicode_ToLow(uint32_t cp) {
  return (cp & 0x3ff) | 0xdc00;
}

// Returns the 32-bit value corresponding to a pair of 16-bit surrogates.
// Does not sanity-check the input.
WPI_UPB_INLINE uint32_t wpi_upb_Unicode_FromPair(uint32_t high, uint32_t low) {
  return ((high & 0x3ff) << 10) + (low & 0x3ff) + 0x10000;
}

// Outputs a codepoint as UTF8.
// Returns the number of bytes written (1-4 on success, 0 on error).
// Does not sanity-check the input. Specifically does not check for surrogates.
int wpi_upb_Unicode_ToUTF8(uint32_t cp, char* out);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_LEX_UNICODE_H_ */

#ifndef WPI_UPB_MINI_DESCRIPTOR_INTERNAL_ENCODE_H_
#define WPI_UPB_MINI_DESCRIPTOR_INTERNAL_ENCODE_H_

#include <stdint.h>


// Must be last.

// If the input buffer has at least this many bytes available, the encoder call
// is guaranteed to succeed (as long as field number order is maintained).
#define kUpb_MtDataEncoder_MinSize 16

typedef struct {
  char* end;  // Limit of the buffer passed as a parameter.
  // Aliased to internal-only members in .cc.
  char internal[32];
} wpi_upb_MtDataEncoder;

#ifdef __cplusplus
extern "C" {
#endif

// Encodes field/oneof information for a given message.  The sequence of calls
// should look like:
//
//   wpi_upb_MtDataEncoder e;
//   char buf[256];
//   char* ptr = buf;
//   e.end = ptr + sizeof(buf);
//   unit64_t msg_mod = ...; // bitwise & of kUpb_MessageModifiers or zero
//   ptr = wpi_upb_MtDataEncoder_StartMessage(&e, ptr, msg_mod);
//   // Fields *must* be in field number order.
//   ptr = wpi_upb_MtDataEncoder_PutField(&e, ptr, ...);
//   ptr = wpi_upb_MtDataEncoder_PutField(&e, ptr, ...);
//   ptr = wpi_upb_MtDataEncoder_PutField(&e, ptr, ...);
//
//   // If oneofs are present.  Oneofs must be encoded after regular fields.
//   ptr = wpi_upb_MiniTable_StartOneof(&e, ptr)
//   ptr = wpi_upb_MiniTable_PutOneofField(&e, ptr, ...);
//   ptr = wpi_upb_MiniTable_PutOneofField(&e, ptr, ...);
//
//   ptr = wpi_upb_MiniTable_StartOneof(&e, ptr);
//   ptr = wpi_upb_MiniTable_PutOneofField(&e, ptr, ...);
//   ptr = wpi_upb_MiniTable_PutOneofField(&e, ptr, ...);
//
// Oneofs must be encoded after all regular fields.
char* wpi_upb_MtDataEncoder_StartMessage(wpi_upb_MtDataEncoder* e, char* ptr,
                                     uint64_t msg_mod);
char* wpi_upb_MtDataEncoder_PutField(wpi_upb_MtDataEncoder* e, char* ptr,
                                 wpi_upb_FieldType type, uint32_t field_num,
                                 uint64_t field_mod);
char* wpi_upb_MtDataEncoder_StartOneof(wpi_upb_MtDataEncoder* e, char* ptr);
char* wpi_upb_MtDataEncoder_PutOneofField(wpi_upb_MtDataEncoder* e, char* ptr,
                                      uint32_t field_num);

// Encodes the set of values for a given enum. The values must be given in
// order (after casting to uint32_t), and repeats are not allowed.
char* wpi_upb_MtDataEncoder_StartEnum(wpi_upb_MtDataEncoder* e, char* ptr);
char* wpi_upb_MtDataEncoder_PutEnumValue(wpi_upb_MtDataEncoder* e, char* ptr,
                                     uint32_t val);
char* wpi_upb_MtDataEncoder_EndEnum(wpi_upb_MtDataEncoder* e, char* ptr);

// Encodes an entire mini descriptor for an extension.
char* wpi_upb_MtDataEncoder_EncodeExtension(wpi_upb_MtDataEncoder* e, char* ptr,
                                        wpi_upb_FieldType type, uint32_t field_num,
                                        uint64_t field_mod);

// Encodes an entire mini descriptor for a map.
char* wpi_upb_MtDataEncoder_EncodeMap(wpi_upb_MtDataEncoder* e, char* ptr,
                                  wpi_upb_FieldType key_type,
                                  wpi_upb_FieldType value_type, uint64_t key_mod,
                                  uint64_t value_mod);

// Encodes an entire mini descriptor for a message set.
char* wpi_upb_MtDataEncoder_EncodeMessageSet(wpi_upb_MtDataEncoder* e, char* ptr);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_MINI_DESCRIPTOR_INTERNAL_ENCODE_H_ */

#ifndef WPI_UPB_REFLECTION_DEF_POOL_INTERNAL_H_
#define WPI_UPB_REFLECTION_DEF_POOL_INTERNAL_H_


// IWYU pragma: private, include "upb/reflection/def.h"

#ifndef WPI_UPB_REFLECTION_DEF_POOL_H_
#define WPI_UPB_REFLECTION_DEF_POOL_H_


// IWYU pragma: private, include "upb/reflection/def.h"

// Declarations common to all public def types.

#ifndef WPI_UPB_REFLECTION_COMMON_H_
#define WPI_UPB_REFLECTION_COMMON_H_

// begin:google_only
// #ifndef WPI_UPB_BOOTSTRAP_STAGE0
// #include "net/proto2/proto/descriptor.upb.h"
// #else
// #include "google/protobuf/descriptor.upb.h"
// #endif
// end:google_only

// begin:github_only
/* This file was generated by wpi_upb_generator from the input file:
 *
 *     google/protobuf/descriptor.proto
 *
 * Do not edit -- your changes will be discarded when the file is
 * regenerated.
 * NO CHECKED-IN PROTOBUF GENCODE */

#ifndef GOOGLE_PROTOBUF_DESCRIPTOR_PROTO_WPI_UPB_H_
#define GOOGLE_PROTOBUF_DESCRIPTOR_PROTO_WPI_UPB_H_



// Must be last.

#ifdef __cplusplus
extern "C" {
#endif

typedef struct google_protobuf_FileDescriptorSet { wpi_upb_Message WPI_UPB_PRIVATE(base); } google_protobuf_FileDescriptorSet;
typedef struct google_protobuf_FileDescriptorProto { wpi_upb_Message WPI_UPB_PRIVATE(base); } google_protobuf_FileDescriptorProto;
typedef struct google_protobuf_DescriptorProto { wpi_upb_Message WPI_UPB_PRIVATE(base); } google_protobuf_DescriptorProto;
typedef struct google_protobuf_DescriptorProto_ExtensionRange { wpi_upb_Message WPI_UPB_PRIVATE(base); } google_protobuf_DescriptorProto_ExtensionRange;
typedef struct google_protobuf_DescriptorProto_ReservedRange { wpi_upb_Message WPI_UPB_PRIVATE(base); } google_protobuf_DescriptorProto_ReservedRange;
typedef struct google_protobuf_ExtensionRangeOptions { wpi_upb_Message WPI_UPB_PRIVATE(base); } google_protobuf_ExtensionRangeOptions;
typedef struct google_protobuf_ExtensionRangeOptions_Declaration { wpi_upb_Message WPI_UPB_PRIVATE(base); } google_protobuf_ExtensionRangeOptions_Declaration;
typedef struct google_protobuf_FieldDescriptorProto { wpi_upb_Message WPI_UPB_PRIVATE(base); } google_protobuf_FieldDescriptorProto;
typedef struct google_protobuf_OneofDescriptorProto { wpi_upb_Message WPI_UPB_PRIVATE(base); } google_protobuf_OneofDescriptorProto;
typedef struct google_protobuf_EnumDescriptorProto { wpi_upb_Message WPI_UPB_PRIVATE(base); } google_protobuf_EnumDescriptorProto;
typedef struct google_protobuf_EnumDescriptorProto_EnumReservedRange { wpi_upb_Message WPI_UPB_PRIVATE(base); } google_protobuf_EnumDescriptorProto_EnumReservedRange;
typedef struct google_protobuf_EnumValueDescriptorProto { wpi_upb_Message WPI_UPB_PRIVATE(base); } google_protobuf_EnumValueDescriptorProto;
typedef struct google_protobuf_ServiceDescriptorProto { wpi_upb_Message WPI_UPB_PRIVATE(base); } google_protobuf_ServiceDescriptorProto;
typedef struct google_protobuf_MethodDescriptorProto { wpi_upb_Message WPI_UPB_PRIVATE(base); } google_protobuf_MethodDescriptorProto;
typedef struct google_protobuf_FileOptions { wpi_upb_Message WPI_UPB_PRIVATE(base); } google_protobuf_FileOptions;
typedef struct google_protobuf_MessageOptions { wpi_upb_Message WPI_UPB_PRIVATE(base); } google_protobuf_MessageOptions;
typedef struct google_protobuf_FieldOptions { wpi_upb_Message WPI_UPB_PRIVATE(base); } google_protobuf_FieldOptions;
typedef struct google_protobuf_FieldOptions_EditionDefault { wpi_upb_Message WPI_UPB_PRIVATE(base); } google_protobuf_FieldOptions_EditionDefault;
typedef struct google_protobuf_FieldOptions_FeatureSupport { wpi_upb_Message WPI_UPB_PRIVATE(base); } google_protobuf_FieldOptions_FeatureSupport;
typedef struct google_protobuf_OneofOptions { wpi_upb_Message WPI_UPB_PRIVATE(base); } google_protobuf_OneofOptions;
typedef struct google_protobuf_EnumOptions { wpi_upb_Message WPI_UPB_PRIVATE(base); } google_protobuf_EnumOptions;
typedef struct google_protobuf_EnumValueOptions { wpi_upb_Message WPI_UPB_PRIVATE(base); } google_protobuf_EnumValueOptions;
typedef struct google_protobuf_ServiceOptions { wpi_upb_Message WPI_UPB_PRIVATE(base); } google_protobuf_ServiceOptions;
typedef struct google_protobuf_MethodOptions { wpi_upb_Message WPI_UPB_PRIVATE(base); } google_protobuf_MethodOptions;
typedef struct google_protobuf_UninterpretedOption { wpi_upb_Message WPI_UPB_PRIVATE(base); } google_protobuf_UninterpretedOption;
typedef struct google_protobuf_UninterpretedOption_NamePart { wpi_upb_Message WPI_UPB_PRIVATE(base); } google_protobuf_UninterpretedOption_NamePart;
typedef struct google_protobuf_FeatureSet { wpi_upb_Message WPI_UPB_PRIVATE(base); } google_protobuf_FeatureSet;
typedef struct google_protobuf_FeatureSetDefaults { wpi_upb_Message WPI_UPB_PRIVATE(base); } google_protobuf_FeatureSetDefaults;
typedef struct google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault { wpi_upb_Message WPI_UPB_PRIVATE(base); } google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault;
typedef struct google_protobuf_SourceCodeInfo { wpi_upb_Message WPI_UPB_PRIVATE(base); } google_protobuf_SourceCodeInfo;
typedef struct google_protobuf_SourceCodeInfo_Location { wpi_upb_Message WPI_UPB_PRIVATE(base); } google_protobuf_SourceCodeInfo_Location;
typedef struct google_protobuf_GeneratedCodeInfo { wpi_upb_Message WPI_UPB_PRIVATE(base); } google_protobuf_GeneratedCodeInfo;
typedef struct google_protobuf_GeneratedCodeInfo_Annotation { wpi_upb_Message WPI_UPB_PRIVATE(base); } google_protobuf_GeneratedCodeInfo_Annotation;

typedef enum {
  google_protobuf_EDITION_UNKNOWN = 0,
  google_protobuf_EDITION_1_TEST_ONLY = 1,
  google_protobuf_EDITION_2_TEST_ONLY = 2,
  google_protobuf_EDITION_LEGACY = 900,
  google_protobuf_EDITION_PROTO2 = 998,
  google_protobuf_EDITION_PROTO3 = 999,
  google_protobuf_EDITION_2023 = 1000,
  google_protobuf_EDITION_2024 = 1001,
  google_protobuf_EDITION_99997_TEST_ONLY = 99997,
  google_protobuf_EDITION_99998_TEST_ONLY = 99998,
  google_protobuf_EDITION_99999_TEST_ONLY = 99999,
  google_protobuf_EDITION_MAX = 2147483647
} google_protobuf_Edition;

typedef enum {
  google_protobuf_ExtensionRangeOptions_DECLARATION = 0,
  google_protobuf_ExtensionRangeOptions_UNVERIFIED = 1
} google_protobuf_ExtensionRangeOptions_VerificationState;

typedef enum {
  google_protobuf_FeatureSet_ENUM_TYPE_UNKNOWN = 0,
  google_protobuf_FeatureSet_OPEN = 1,
  google_protobuf_FeatureSet_CLOSED = 2
} google_protobuf_FeatureSet_EnumType;

typedef enum {
  google_protobuf_FeatureSet_FIELD_PRESENCE_UNKNOWN = 0,
  google_protobuf_FeatureSet_EXPLICIT = 1,
  google_protobuf_FeatureSet_IMPLICIT = 2,
  google_protobuf_FeatureSet_LEGACY_REQUIRED = 3
} google_protobuf_FeatureSet_FieldPresence;

typedef enum {
  google_protobuf_FeatureSet_JSON_FORMAT_UNKNOWN = 0,
  google_protobuf_FeatureSet_ALLOW = 1,
  google_protobuf_FeatureSet_LEGACY_BEST_EFFORT = 2
} google_protobuf_FeatureSet_JsonFormat;

typedef enum {
  google_protobuf_FeatureSet_MESSAGE_ENCODING_UNKNOWN = 0,
  google_protobuf_FeatureSet_LENGTH_PREFIXED = 1,
  google_protobuf_FeatureSet_DELIMITED = 2
} google_protobuf_FeatureSet_MessageEncoding;

typedef enum {
  google_protobuf_FeatureSet_REPEATED_FIELD_ENCODING_UNKNOWN = 0,
  google_protobuf_FeatureSet_PACKED = 1,
  google_protobuf_FeatureSet_EXPANDED = 2
} google_protobuf_FeatureSet_RepeatedFieldEncoding;

typedef enum {
  google_protobuf_FeatureSet_UTF8_VALIDATION_UNKNOWN = 0,
  google_protobuf_FeatureSet_VERIFY = 2,
  google_protobuf_FeatureSet_NONE = 3
} google_protobuf_FeatureSet_Utf8Validation;

typedef enum {
  google_protobuf_FieldDescriptorProto_LABEL_OPTIONAL = 1,
  google_protobuf_FieldDescriptorProto_LABEL_REQUIRED = 2,
  google_protobuf_FieldDescriptorProto_LABEL_REPEATED = 3
} google_protobuf_FieldDescriptorProto_Label;

typedef enum {
  google_protobuf_FieldDescriptorProto_TYPE_DOUBLE = 1,
  google_protobuf_FieldDescriptorProto_TYPE_FLOAT = 2,
  google_protobuf_FieldDescriptorProto_TYPE_INT64 = 3,
  google_protobuf_FieldDescriptorProto_TYPE_UINT64 = 4,
  google_protobuf_FieldDescriptorProto_TYPE_INT32 = 5,
  google_protobuf_FieldDescriptorProto_TYPE_FIXED64 = 6,
  google_protobuf_FieldDescriptorProto_TYPE_FIXED32 = 7,
  google_protobuf_FieldDescriptorProto_TYPE_BOOL = 8,
  google_protobuf_FieldDescriptorProto_TYPE_STRING = 9,
  google_protobuf_FieldDescriptorProto_TYPE_GROUP = 10,
  google_protobuf_FieldDescriptorProto_TYPE_MESSAGE = 11,
  google_protobuf_FieldDescriptorProto_TYPE_BYTES = 12,
  google_protobuf_FieldDescriptorProto_TYPE_UINT32 = 13,
  google_protobuf_FieldDescriptorProto_TYPE_ENUM = 14,
  google_protobuf_FieldDescriptorProto_TYPE_SFIXED32 = 15,
  google_protobuf_FieldDescriptorProto_TYPE_SFIXED64 = 16,
  google_protobuf_FieldDescriptorProto_TYPE_SINT32 = 17,
  google_protobuf_FieldDescriptorProto_TYPE_SINT64 = 18
} google_protobuf_FieldDescriptorProto_Type;

typedef enum {
  google_protobuf_FieldOptions_STRING = 0,
  google_protobuf_FieldOptions_CORD = 1,
  google_protobuf_FieldOptions_STRING_PIECE = 2
} google_protobuf_FieldOptions_CType;

typedef enum {
  google_protobuf_FieldOptions_JS_NORMAL = 0,
  google_protobuf_FieldOptions_JS_STRING = 1,
  google_protobuf_FieldOptions_JS_NUMBER = 2
} google_protobuf_FieldOptions_JSType;

typedef enum {
  google_protobuf_FieldOptions_RETENTION_UNKNOWN = 0,
  google_protobuf_FieldOptions_RETENTION_RUNTIME = 1,
  google_protobuf_FieldOptions_RETENTION_SOURCE = 2
} google_protobuf_FieldOptions_OptionRetention;

typedef enum {
  google_protobuf_FieldOptions_TARGET_TYPE_UNKNOWN = 0,
  google_protobuf_FieldOptions_TARGET_TYPE_FILE = 1,
  google_protobuf_FieldOptions_TARGET_TYPE_EXTENSION_RANGE = 2,
  google_protobuf_FieldOptions_TARGET_TYPE_MESSAGE = 3,
  google_protobuf_FieldOptions_TARGET_TYPE_FIELD = 4,
  google_protobuf_FieldOptions_TARGET_TYPE_ONEOF = 5,
  google_protobuf_FieldOptions_TARGET_TYPE_ENUM = 6,
  google_protobuf_FieldOptions_TARGET_TYPE_ENUM_ENTRY = 7,
  google_protobuf_FieldOptions_TARGET_TYPE_SERVICE = 8,
  google_protobuf_FieldOptions_TARGET_TYPE_METHOD = 9
} google_protobuf_FieldOptions_OptionTargetType;

typedef enum {
  google_protobuf_FileOptions_SPEED = 1,
  google_protobuf_FileOptions_CODE_SIZE = 2,
  google_protobuf_FileOptions_LITE_RUNTIME = 3
} google_protobuf_FileOptions_OptimizeMode;

typedef enum {
  google_protobuf_GeneratedCodeInfo_Annotation_NONE = 0,
  google_protobuf_GeneratedCodeInfo_Annotation_SET = 1,
  google_protobuf_GeneratedCodeInfo_Annotation_ALIAS = 2
} google_protobuf_GeneratedCodeInfo_Annotation_Semantic;

typedef enum {
  google_protobuf_MethodOptions_IDEMPOTENCY_UNKNOWN = 0,
  google_protobuf_MethodOptions_NO_SIDE_EFFECTS = 1,
  google_protobuf_MethodOptions_IDEMPOTENT = 2
} google_protobuf_MethodOptions_IdempotencyLevel;



/* google.protobuf.FileDescriptorSet */

WPI_UPB_INLINE google_protobuf_FileDescriptorSet* google_protobuf_FileDescriptorSet_new(wpi_upb_Arena* arena) {
  return (google_protobuf_FileDescriptorSet*)_wpi_upb_Message_New(&google__protobuf__FileDescriptorSet_msg_init, arena);
}
WPI_UPB_INLINE google_protobuf_FileDescriptorSet* google_protobuf_FileDescriptorSet_parse(const char* buf, size_t size, wpi_upb_Arena* arena) {
  google_protobuf_FileDescriptorSet* ret = google_protobuf_FileDescriptorSet_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__FileDescriptorSet_msg_init, NULL, 0, arena) !=
      kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE google_protobuf_FileDescriptorSet* google_protobuf_FileDescriptorSet_parse_ex(const char* buf, size_t size,
                           const wpi_upb_ExtensionRegistry* extreg,
                           int options, wpi_upb_Arena* arena) {
  google_protobuf_FileDescriptorSet* ret = google_protobuf_FileDescriptorSet_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__FileDescriptorSet_msg_init, extreg, options,
                 arena) != kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE char* google_protobuf_FileDescriptorSet_serialize(const google_protobuf_FileDescriptorSet* msg, wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__FileDescriptorSet_msg_init, 0, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE char* google_protobuf_FileDescriptorSet_serialize_ex(const google_protobuf_FileDescriptorSet* msg, int options,
                                 wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__FileDescriptorSet_msg_init, options, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE void google_protobuf_FileDescriptorSet_clear_file(google_protobuf_FileDescriptorSet* msg) {
  const wpi_upb_MiniTableField field = {1, 8, 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_FileDescriptorProto* const* google_protobuf_FileDescriptorSet_file(const google_protobuf_FileDescriptorSet* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {1, 8, 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FileDescriptorProto_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (const google_protobuf_FileDescriptorProto* const*)wpi_upb_Array_DataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE const wpi_upb_Array* _google_protobuf_FileDescriptorSet_file_wpi_upb_array(const google_protobuf_FileDescriptorSet* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {1, 8, 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FileDescriptorProto_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE wpi_upb_Array* _google_protobuf_FileDescriptorSet_file_mutable_wpi_upb_array(google_protobuf_FileDescriptorSet* msg, size_t* size, wpi_upb_Arena* arena) {
  const wpi_upb_MiniTableField field = {1, 8, 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FileDescriptorProto_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(WPI_UPB_UPCAST(msg),
                                                       &field, arena);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}

WPI_UPB_INLINE google_protobuf_FileDescriptorProto** google_protobuf_FileDescriptorSet_mutable_file(google_protobuf_FileDescriptorSet* msg, size_t* size) {
  wpi_upb_MiniTableField field = {1, 8, 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FileDescriptorProto_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetMutableArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (google_protobuf_FileDescriptorProto**)wpi_upb_Array_MutableDataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE google_protobuf_FileDescriptorProto** google_protobuf_FileDescriptorSet_resize_file(google_protobuf_FileDescriptorSet* msg, size_t size, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {1, 8, 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return (google_protobuf_FileDescriptorProto**)wpi_upb_Message_ResizeArrayUninitialized(WPI_UPB_UPCAST(msg),
                                                   &field, size, arena);
}
WPI_UPB_INLINE struct google_protobuf_FileDescriptorProto* google_protobuf_FileDescriptorSet_add_file(google_protobuf_FileDescriptorSet* msg, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {1, 8, 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FileDescriptorProto_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(
      WPI_UPB_UPCAST(msg), &field, arena);
  if (!arr || !WPI_UPB_PRIVATE(_wpi_upb_Array_ResizeUninitialized)(
                  arr, arr->WPI_UPB_PRIVATE(size) + 1, arena)) {
    return NULL;
  }
  struct google_protobuf_FileDescriptorProto* sub = (struct google_protobuf_FileDescriptorProto*)_wpi_upb_Message_New(&google__protobuf__FileDescriptorProto_msg_init, arena);
  if (!arr || !sub) return NULL;
  WPI_UPB_PRIVATE(_wpi_upb_Array_Set)
  (arr, arr->WPI_UPB_PRIVATE(size) - 1, &sub, sizeof(sub));
  return sub;
}

/* google.protobuf.FileDescriptorProto */

WPI_UPB_INLINE google_protobuf_FileDescriptorProto* google_protobuf_FileDescriptorProto_new(wpi_upb_Arena* arena) {
  return (google_protobuf_FileDescriptorProto*)_wpi_upb_Message_New(&google__protobuf__FileDescriptorProto_msg_init, arena);
}
WPI_UPB_INLINE google_protobuf_FileDescriptorProto* google_protobuf_FileDescriptorProto_parse(const char* buf, size_t size, wpi_upb_Arena* arena) {
  google_protobuf_FileDescriptorProto* ret = google_protobuf_FileDescriptorProto_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__FileDescriptorProto_msg_init, NULL, 0, arena) !=
      kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE google_protobuf_FileDescriptorProto* google_protobuf_FileDescriptorProto_parse_ex(const char* buf, size_t size,
                           const wpi_upb_ExtensionRegistry* extreg,
                           int options, wpi_upb_Arena* arena) {
  google_protobuf_FileDescriptorProto* ret = google_protobuf_FileDescriptorProto_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__FileDescriptorProto_msg_init, extreg, options,
                 arena) != kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE char* google_protobuf_FileDescriptorProto_serialize(const google_protobuf_FileDescriptorProto* msg, wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__FileDescriptorProto_msg_init, 0, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE char* google_protobuf_FileDescriptorProto_serialize_ex(const google_protobuf_FileDescriptorProto* msg, int options,
                                 wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__FileDescriptorProto_msg_init, options, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE void google_protobuf_FileDescriptorProto_clear_name(google_protobuf_FileDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(52, 16), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE wpi_upb_StringView google_protobuf_FileDescriptorProto_name(const google_protobuf_FileDescriptorProto* msg) {
  wpi_upb_StringView default_val = wpi_upb_StringView_FromString("");
  wpi_upb_StringView ret;
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(52, 16), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FileDescriptorProto_has_name(const google_protobuf_FileDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(52, 16), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FileDescriptorProto_clear_package(google_protobuf_FileDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(60, 32), 65, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE wpi_upb_StringView google_protobuf_FileDescriptorProto_package(const google_protobuf_FileDescriptorProto* msg) {
  wpi_upb_StringView default_val = wpi_upb_StringView_FromString("");
  wpi_upb_StringView ret;
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(60, 32), 65, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FileDescriptorProto_has_package(const google_protobuf_FileDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(60, 32), 65, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FileDescriptorProto_clear_dependency(google_protobuf_FileDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(12, 48), 0, kUpb_NoSub, 12, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsAlternate | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE wpi_upb_StringView const* google_protobuf_FileDescriptorProto_dependency(const google_protobuf_FileDescriptorProto* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(12, 48), 0, kUpb_NoSub, 12, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsAlternate | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (wpi_upb_StringView const*)wpi_upb_Array_DataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE const wpi_upb_Array* _google_protobuf_FileDescriptorProto_dependency_wpi_upb_array(const google_protobuf_FileDescriptorProto* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(12, 48), 0, kUpb_NoSub, 12, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsAlternate | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE wpi_upb_Array* _google_protobuf_FileDescriptorProto_dependency_mutable_wpi_upb_array(google_protobuf_FileDescriptorProto* msg, size_t* size, wpi_upb_Arena* arena) {
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(12, 48), 0, kUpb_NoSub, 12, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsAlternate | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(WPI_UPB_UPCAST(msg),
                                                       &field, arena);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE void google_protobuf_FileDescriptorProto_clear_message_type(google_protobuf_FileDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {4, WPI_UPB_SIZE(16, 56), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_DescriptorProto* const* google_protobuf_FileDescriptorProto_message_type(const google_protobuf_FileDescriptorProto* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {4, WPI_UPB_SIZE(16, 56), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__DescriptorProto_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (const google_protobuf_DescriptorProto* const*)wpi_upb_Array_DataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE const wpi_upb_Array* _google_protobuf_FileDescriptorProto_message_type_wpi_upb_array(const google_protobuf_FileDescriptorProto* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {4, WPI_UPB_SIZE(16, 56), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__DescriptorProto_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE wpi_upb_Array* _google_protobuf_FileDescriptorProto_message_type_mutable_wpi_upb_array(google_protobuf_FileDescriptorProto* msg, size_t* size, wpi_upb_Arena* arena) {
  const wpi_upb_MiniTableField field = {4, WPI_UPB_SIZE(16, 56), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__DescriptorProto_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(WPI_UPB_UPCAST(msg),
                                                       &field, arena);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE void google_protobuf_FileDescriptorProto_clear_enum_type(google_protobuf_FileDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {5, WPI_UPB_SIZE(20, 64), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_EnumDescriptorProto* const* google_protobuf_FileDescriptorProto_enum_type(const google_protobuf_FileDescriptorProto* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {5, WPI_UPB_SIZE(20, 64), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__EnumDescriptorProto_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (const google_protobuf_EnumDescriptorProto* const*)wpi_upb_Array_DataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE const wpi_upb_Array* _google_protobuf_FileDescriptorProto_enum_type_wpi_upb_array(const google_protobuf_FileDescriptorProto* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {5, WPI_UPB_SIZE(20, 64), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__EnumDescriptorProto_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE wpi_upb_Array* _google_protobuf_FileDescriptorProto_enum_type_mutable_wpi_upb_array(google_protobuf_FileDescriptorProto* msg, size_t* size, wpi_upb_Arena* arena) {
  const wpi_upb_MiniTableField field = {5, WPI_UPB_SIZE(20, 64), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__EnumDescriptorProto_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(WPI_UPB_UPCAST(msg),
                                                       &field, arena);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE void google_protobuf_FileDescriptorProto_clear_service(google_protobuf_FileDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {6, WPI_UPB_SIZE(24, 72), 0, 2, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_ServiceDescriptorProto* const* google_protobuf_FileDescriptorProto_service(const google_protobuf_FileDescriptorProto* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {6, WPI_UPB_SIZE(24, 72), 0, 2, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__ServiceDescriptorProto_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (const google_protobuf_ServiceDescriptorProto* const*)wpi_upb_Array_DataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE const wpi_upb_Array* _google_protobuf_FileDescriptorProto_service_wpi_upb_array(const google_protobuf_FileDescriptorProto* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {6, WPI_UPB_SIZE(24, 72), 0, 2, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__ServiceDescriptorProto_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE wpi_upb_Array* _google_protobuf_FileDescriptorProto_service_mutable_wpi_upb_array(google_protobuf_FileDescriptorProto* msg, size_t* size, wpi_upb_Arena* arena) {
  const wpi_upb_MiniTableField field = {6, WPI_UPB_SIZE(24, 72), 0, 2, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__ServiceDescriptorProto_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(WPI_UPB_UPCAST(msg),
                                                       &field, arena);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE void google_protobuf_FileDescriptorProto_clear_extension(google_protobuf_FileDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {7, WPI_UPB_SIZE(28, 80), 0, 3, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_FieldDescriptorProto* const* google_protobuf_FileDescriptorProto_extension(const google_protobuf_FileDescriptorProto* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {7, WPI_UPB_SIZE(28, 80), 0, 3, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FieldDescriptorProto_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (const google_protobuf_FieldDescriptorProto* const*)wpi_upb_Array_DataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE const wpi_upb_Array* _google_protobuf_FileDescriptorProto_extension_wpi_upb_array(const google_protobuf_FileDescriptorProto* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {7, WPI_UPB_SIZE(28, 80), 0, 3, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FieldDescriptorProto_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE wpi_upb_Array* _google_protobuf_FileDescriptorProto_extension_mutable_wpi_upb_array(google_protobuf_FileDescriptorProto* msg, size_t* size, wpi_upb_Arena* arena) {
  const wpi_upb_MiniTableField field = {7, WPI_UPB_SIZE(28, 80), 0, 3, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FieldDescriptorProto_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(WPI_UPB_UPCAST(msg),
                                                       &field, arena);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE void google_protobuf_FileDescriptorProto_clear_options(google_protobuf_FileDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {8, WPI_UPB_SIZE(32, 88), 66, 4, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_FileOptions* google_protobuf_FileDescriptorProto_options(const google_protobuf_FileDescriptorProto* msg) {
  const google_protobuf_FileOptions* default_val = NULL;
  const google_protobuf_FileOptions* ret;
  const wpi_upb_MiniTableField field = {8, WPI_UPB_SIZE(32, 88), 66, 4, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FileOptions_msg_init);
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FileDescriptorProto_has_options(const google_protobuf_FileDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {8, WPI_UPB_SIZE(32, 88), 66, 4, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FileDescriptorProto_clear_source_code_info(google_protobuf_FileDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {9, WPI_UPB_SIZE(36, 96), 67, 5, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_SourceCodeInfo* google_protobuf_FileDescriptorProto_source_code_info(const google_protobuf_FileDescriptorProto* msg) {
  const google_protobuf_SourceCodeInfo* default_val = NULL;
  const google_protobuf_SourceCodeInfo* ret;
  const wpi_upb_MiniTableField field = {9, WPI_UPB_SIZE(36, 96), 67, 5, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__SourceCodeInfo_msg_init);
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FileDescriptorProto_has_source_code_info(const google_protobuf_FileDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {9, WPI_UPB_SIZE(36, 96), 67, 5, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FileDescriptorProto_clear_public_dependency(google_protobuf_FileDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {10, WPI_UPB_SIZE(40, 104), 0, kUpb_NoSub, 5, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE int32_t const* google_protobuf_FileDescriptorProto_public_dependency(const google_protobuf_FileDescriptorProto* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {10, WPI_UPB_SIZE(40, 104), 0, kUpb_NoSub, 5, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (int32_t const*)wpi_upb_Array_DataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE const wpi_upb_Array* _google_protobuf_FileDescriptorProto_public_dependency_wpi_upb_array(const google_protobuf_FileDescriptorProto* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {10, WPI_UPB_SIZE(40, 104), 0, kUpb_NoSub, 5, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE wpi_upb_Array* _google_protobuf_FileDescriptorProto_public_dependency_mutable_wpi_upb_array(google_protobuf_FileDescriptorProto* msg, size_t* size, wpi_upb_Arena* arena) {
  const wpi_upb_MiniTableField field = {10, WPI_UPB_SIZE(40, 104), 0, kUpb_NoSub, 5, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(WPI_UPB_UPCAST(msg),
                                                       &field, arena);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE void google_protobuf_FileDescriptorProto_clear_weak_dependency(google_protobuf_FileDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {11, WPI_UPB_SIZE(44, 112), 0, kUpb_NoSub, 5, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE int32_t const* google_protobuf_FileDescriptorProto_weak_dependency(const google_protobuf_FileDescriptorProto* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {11, WPI_UPB_SIZE(44, 112), 0, kUpb_NoSub, 5, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (int32_t const*)wpi_upb_Array_DataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE const wpi_upb_Array* _google_protobuf_FileDescriptorProto_weak_dependency_wpi_upb_array(const google_protobuf_FileDescriptorProto* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {11, WPI_UPB_SIZE(44, 112), 0, kUpb_NoSub, 5, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE wpi_upb_Array* _google_protobuf_FileDescriptorProto_weak_dependency_mutable_wpi_upb_array(google_protobuf_FileDescriptorProto* msg, size_t* size, wpi_upb_Arena* arena) {
  const wpi_upb_MiniTableField field = {11, WPI_UPB_SIZE(44, 112), 0, kUpb_NoSub, 5, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(WPI_UPB_UPCAST(msg),
                                                       &field, arena);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE void google_protobuf_FileDescriptorProto_clear_syntax(google_protobuf_FileDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {12, WPI_UPB_SIZE(68, 120), 68, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE wpi_upb_StringView google_protobuf_FileDescriptorProto_syntax(const google_protobuf_FileDescriptorProto* msg) {
  wpi_upb_StringView default_val = wpi_upb_StringView_FromString("");
  wpi_upb_StringView ret;
  const wpi_upb_MiniTableField field = {12, WPI_UPB_SIZE(68, 120), 68, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FileDescriptorProto_has_syntax(const google_protobuf_FileDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {12, WPI_UPB_SIZE(68, 120), 68, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FileDescriptorProto_clear_edition(google_protobuf_FileDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {14, WPI_UPB_SIZE(48, 12), 69, 6, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE int32_t google_protobuf_FileDescriptorProto_edition(const google_protobuf_FileDescriptorProto* msg) {
  int32_t default_val = 0;
  int32_t ret;
  const wpi_upb_MiniTableField field = {14, WPI_UPB_SIZE(48, 12), 69, 6, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FileDescriptorProto_has_edition(const google_protobuf_FileDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {14, WPI_UPB_SIZE(48, 12), 69, 6, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}

WPI_UPB_INLINE void google_protobuf_FileDescriptorProto_set_name(google_protobuf_FileDescriptorProto *msg, wpi_upb_StringView value) {
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(52, 16), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_FileDescriptorProto_set_package(google_protobuf_FileDescriptorProto *msg, wpi_upb_StringView value) {
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(60, 32), 65, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE wpi_upb_StringView* google_protobuf_FileDescriptorProto_mutable_dependency(google_protobuf_FileDescriptorProto* msg, size_t* size) {
  wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(12, 48), 0, kUpb_NoSub, 12, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsAlternate | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Array* arr = wpi_upb_Message_GetMutableArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (wpi_upb_StringView*)wpi_upb_Array_MutableDataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE wpi_upb_StringView* google_protobuf_FileDescriptorProto_resize_dependency(google_protobuf_FileDescriptorProto* msg, size_t size, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(12, 48), 0, kUpb_NoSub, 12, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsAlternate | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return (wpi_upb_StringView*)wpi_upb_Message_ResizeArrayUninitialized(WPI_UPB_UPCAST(msg),
                                                   &field, size, arena);
}
WPI_UPB_INLINE bool google_protobuf_FileDescriptorProto_add_dependency(google_protobuf_FileDescriptorProto* msg, wpi_upb_StringView val, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(12, 48), 0, kUpb_NoSub, 12, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsAlternate | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(
      WPI_UPB_UPCAST(msg), &field, arena);
  if (!arr || !WPI_UPB_PRIVATE(_wpi_upb_Array_ResizeUninitialized)(
                  arr, arr->WPI_UPB_PRIVATE(size) + 1, arena)) {
    return false;
  }
  WPI_UPB_PRIVATE(_wpi_upb_Array_Set)
  (arr, arr->WPI_UPB_PRIVATE(size) - 1, &val, sizeof(val));
  return true;
}
WPI_UPB_INLINE google_protobuf_DescriptorProto** google_protobuf_FileDescriptorProto_mutable_message_type(google_protobuf_FileDescriptorProto* msg, size_t* size) {
  wpi_upb_MiniTableField field = {4, WPI_UPB_SIZE(16, 56), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__DescriptorProto_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetMutableArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (google_protobuf_DescriptorProto**)wpi_upb_Array_MutableDataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE google_protobuf_DescriptorProto** google_protobuf_FileDescriptorProto_resize_message_type(google_protobuf_FileDescriptorProto* msg, size_t size, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {4, WPI_UPB_SIZE(16, 56), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return (google_protobuf_DescriptorProto**)wpi_upb_Message_ResizeArrayUninitialized(WPI_UPB_UPCAST(msg),
                                                   &field, size, arena);
}
WPI_UPB_INLINE struct google_protobuf_DescriptorProto* google_protobuf_FileDescriptorProto_add_message_type(google_protobuf_FileDescriptorProto* msg, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {4, WPI_UPB_SIZE(16, 56), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__DescriptorProto_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(
      WPI_UPB_UPCAST(msg), &field, arena);
  if (!arr || !WPI_UPB_PRIVATE(_wpi_upb_Array_ResizeUninitialized)(
                  arr, arr->WPI_UPB_PRIVATE(size) + 1, arena)) {
    return NULL;
  }
  struct google_protobuf_DescriptorProto* sub = (struct google_protobuf_DescriptorProto*)_wpi_upb_Message_New(&google__protobuf__DescriptorProto_msg_init, arena);
  if (!arr || !sub) return NULL;
  WPI_UPB_PRIVATE(_wpi_upb_Array_Set)
  (arr, arr->WPI_UPB_PRIVATE(size) - 1, &sub, sizeof(sub));
  return sub;
}
WPI_UPB_INLINE google_protobuf_EnumDescriptorProto** google_protobuf_FileDescriptorProto_mutable_enum_type(google_protobuf_FileDescriptorProto* msg, size_t* size) {
  wpi_upb_MiniTableField field = {5, WPI_UPB_SIZE(20, 64), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__EnumDescriptorProto_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetMutableArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (google_protobuf_EnumDescriptorProto**)wpi_upb_Array_MutableDataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE google_protobuf_EnumDescriptorProto** google_protobuf_FileDescriptorProto_resize_enum_type(google_protobuf_FileDescriptorProto* msg, size_t size, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {5, WPI_UPB_SIZE(20, 64), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return (google_protobuf_EnumDescriptorProto**)wpi_upb_Message_ResizeArrayUninitialized(WPI_UPB_UPCAST(msg),
                                                   &field, size, arena);
}
WPI_UPB_INLINE struct google_protobuf_EnumDescriptorProto* google_protobuf_FileDescriptorProto_add_enum_type(google_protobuf_FileDescriptorProto* msg, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {5, WPI_UPB_SIZE(20, 64), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__EnumDescriptorProto_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(
      WPI_UPB_UPCAST(msg), &field, arena);
  if (!arr || !WPI_UPB_PRIVATE(_wpi_upb_Array_ResizeUninitialized)(
                  arr, arr->WPI_UPB_PRIVATE(size) + 1, arena)) {
    return NULL;
  }
  struct google_protobuf_EnumDescriptorProto* sub = (struct google_protobuf_EnumDescriptorProto*)_wpi_upb_Message_New(&google__protobuf__EnumDescriptorProto_msg_init, arena);
  if (!arr || !sub) return NULL;
  WPI_UPB_PRIVATE(_wpi_upb_Array_Set)
  (arr, arr->WPI_UPB_PRIVATE(size) - 1, &sub, sizeof(sub));
  return sub;
}
WPI_UPB_INLINE google_protobuf_ServiceDescriptorProto** google_protobuf_FileDescriptorProto_mutable_service(google_protobuf_FileDescriptorProto* msg, size_t* size) {
  wpi_upb_MiniTableField field = {6, WPI_UPB_SIZE(24, 72), 0, 2, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__ServiceDescriptorProto_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetMutableArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (google_protobuf_ServiceDescriptorProto**)wpi_upb_Array_MutableDataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE google_protobuf_ServiceDescriptorProto** google_protobuf_FileDescriptorProto_resize_service(google_protobuf_FileDescriptorProto* msg, size_t size, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {6, WPI_UPB_SIZE(24, 72), 0, 2, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return (google_protobuf_ServiceDescriptorProto**)wpi_upb_Message_ResizeArrayUninitialized(WPI_UPB_UPCAST(msg),
                                                   &field, size, arena);
}
WPI_UPB_INLINE struct google_protobuf_ServiceDescriptorProto* google_protobuf_FileDescriptorProto_add_service(google_protobuf_FileDescriptorProto* msg, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {6, WPI_UPB_SIZE(24, 72), 0, 2, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__ServiceDescriptorProto_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(
      WPI_UPB_UPCAST(msg), &field, arena);
  if (!arr || !WPI_UPB_PRIVATE(_wpi_upb_Array_ResizeUninitialized)(
                  arr, arr->WPI_UPB_PRIVATE(size) + 1, arena)) {
    return NULL;
  }
  struct google_protobuf_ServiceDescriptorProto* sub = (struct google_protobuf_ServiceDescriptorProto*)_wpi_upb_Message_New(&google__protobuf__ServiceDescriptorProto_msg_init, arena);
  if (!arr || !sub) return NULL;
  WPI_UPB_PRIVATE(_wpi_upb_Array_Set)
  (arr, arr->WPI_UPB_PRIVATE(size) - 1, &sub, sizeof(sub));
  return sub;
}
WPI_UPB_INLINE google_protobuf_FieldDescriptorProto** google_protobuf_FileDescriptorProto_mutable_extension(google_protobuf_FileDescriptorProto* msg, size_t* size) {
  wpi_upb_MiniTableField field = {7, WPI_UPB_SIZE(28, 80), 0, 3, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FieldDescriptorProto_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetMutableArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (google_protobuf_FieldDescriptorProto**)wpi_upb_Array_MutableDataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE google_protobuf_FieldDescriptorProto** google_protobuf_FileDescriptorProto_resize_extension(google_protobuf_FileDescriptorProto* msg, size_t size, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {7, WPI_UPB_SIZE(28, 80), 0, 3, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return (google_protobuf_FieldDescriptorProto**)wpi_upb_Message_ResizeArrayUninitialized(WPI_UPB_UPCAST(msg),
                                                   &field, size, arena);
}
WPI_UPB_INLINE struct google_protobuf_FieldDescriptorProto* google_protobuf_FileDescriptorProto_add_extension(google_protobuf_FileDescriptorProto* msg, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {7, WPI_UPB_SIZE(28, 80), 0, 3, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FieldDescriptorProto_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(
      WPI_UPB_UPCAST(msg), &field, arena);
  if (!arr || !WPI_UPB_PRIVATE(_wpi_upb_Array_ResizeUninitialized)(
                  arr, arr->WPI_UPB_PRIVATE(size) + 1, arena)) {
    return NULL;
  }
  struct google_protobuf_FieldDescriptorProto* sub = (struct google_protobuf_FieldDescriptorProto*)_wpi_upb_Message_New(&google__protobuf__FieldDescriptorProto_msg_init, arena);
  if (!arr || !sub) return NULL;
  WPI_UPB_PRIVATE(_wpi_upb_Array_Set)
  (arr, arr->WPI_UPB_PRIVATE(size) - 1, &sub, sizeof(sub));
  return sub;
}
WPI_UPB_INLINE void google_protobuf_FileDescriptorProto_set_options(google_protobuf_FileDescriptorProto *msg, google_protobuf_FileOptions* value) {
  const wpi_upb_MiniTableField field = {8, WPI_UPB_SIZE(32, 88), 66, 4, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FileOptions_msg_init);
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE struct google_protobuf_FileOptions* google_protobuf_FileDescriptorProto_mutable_options(google_protobuf_FileDescriptorProto* msg, wpi_upb_Arena* arena) {
  struct google_protobuf_FileOptions* sub = (struct google_protobuf_FileOptions*)google_protobuf_FileDescriptorProto_options(msg);
  if (sub == NULL) {
    sub = (struct google_protobuf_FileOptions*)_wpi_upb_Message_New(&google__protobuf__FileOptions_msg_init, arena);
    if (sub) google_protobuf_FileDescriptorProto_set_options(msg, sub);
  }
  return sub;
}
WPI_UPB_INLINE void google_protobuf_FileDescriptorProto_set_source_code_info(google_protobuf_FileDescriptorProto *msg, google_protobuf_SourceCodeInfo* value) {
  const wpi_upb_MiniTableField field = {9, WPI_UPB_SIZE(36, 96), 67, 5, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__SourceCodeInfo_msg_init);
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE struct google_protobuf_SourceCodeInfo* google_protobuf_FileDescriptorProto_mutable_source_code_info(google_protobuf_FileDescriptorProto* msg, wpi_upb_Arena* arena) {
  struct google_protobuf_SourceCodeInfo* sub = (struct google_protobuf_SourceCodeInfo*)google_protobuf_FileDescriptorProto_source_code_info(msg);
  if (sub == NULL) {
    sub = (struct google_protobuf_SourceCodeInfo*)_wpi_upb_Message_New(&google__protobuf__SourceCodeInfo_msg_init, arena);
    if (sub) google_protobuf_FileDescriptorProto_set_source_code_info(msg, sub);
  }
  return sub;
}
WPI_UPB_INLINE int32_t* google_protobuf_FileDescriptorProto_mutable_public_dependency(google_protobuf_FileDescriptorProto* msg, size_t* size) {
  wpi_upb_MiniTableField field = {10, WPI_UPB_SIZE(40, 104), 0, kUpb_NoSub, 5, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Array* arr = wpi_upb_Message_GetMutableArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (int32_t*)wpi_upb_Array_MutableDataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE int32_t* google_protobuf_FileDescriptorProto_resize_public_dependency(google_protobuf_FileDescriptorProto* msg, size_t size, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {10, WPI_UPB_SIZE(40, 104), 0, kUpb_NoSub, 5, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return (int32_t*)wpi_upb_Message_ResizeArrayUninitialized(WPI_UPB_UPCAST(msg),
                                                   &field, size, arena);
}
WPI_UPB_INLINE bool google_protobuf_FileDescriptorProto_add_public_dependency(google_protobuf_FileDescriptorProto* msg, int32_t val, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {10, WPI_UPB_SIZE(40, 104), 0, kUpb_NoSub, 5, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(
      WPI_UPB_UPCAST(msg), &field, arena);
  if (!arr || !WPI_UPB_PRIVATE(_wpi_upb_Array_ResizeUninitialized)(
                  arr, arr->WPI_UPB_PRIVATE(size) + 1, arena)) {
    return false;
  }
  WPI_UPB_PRIVATE(_wpi_upb_Array_Set)
  (arr, arr->WPI_UPB_PRIVATE(size) - 1, &val, sizeof(val));
  return true;
}
WPI_UPB_INLINE int32_t* google_protobuf_FileDescriptorProto_mutable_weak_dependency(google_protobuf_FileDescriptorProto* msg, size_t* size) {
  wpi_upb_MiniTableField field = {11, WPI_UPB_SIZE(44, 112), 0, kUpb_NoSub, 5, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Array* arr = wpi_upb_Message_GetMutableArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (int32_t*)wpi_upb_Array_MutableDataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE int32_t* google_protobuf_FileDescriptorProto_resize_weak_dependency(google_protobuf_FileDescriptorProto* msg, size_t size, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {11, WPI_UPB_SIZE(44, 112), 0, kUpb_NoSub, 5, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return (int32_t*)wpi_upb_Message_ResizeArrayUninitialized(WPI_UPB_UPCAST(msg),
                                                   &field, size, arena);
}
WPI_UPB_INLINE bool google_protobuf_FileDescriptorProto_add_weak_dependency(google_protobuf_FileDescriptorProto* msg, int32_t val, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {11, WPI_UPB_SIZE(44, 112), 0, kUpb_NoSub, 5, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(
      WPI_UPB_UPCAST(msg), &field, arena);
  if (!arr || !WPI_UPB_PRIVATE(_wpi_upb_Array_ResizeUninitialized)(
                  arr, arr->WPI_UPB_PRIVATE(size) + 1, arena)) {
    return false;
  }
  WPI_UPB_PRIVATE(_wpi_upb_Array_Set)
  (arr, arr->WPI_UPB_PRIVATE(size) - 1, &val, sizeof(val));
  return true;
}
WPI_UPB_INLINE void google_protobuf_FileDescriptorProto_set_syntax(google_protobuf_FileDescriptorProto *msg, wpi_upb_StringView value) {
  const wpi_upb_MiniTableField field = {12, WPI_UPB_SIZE(68, 120), 68, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_FileDescriptorProto_set_edition(google_protobuf_FileDescriptorProto *msg, int32_t value) {
  const wpi_upb_MiniTableField field = {14, WPI_UPB_SIZE(48, 12), 69, 6, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}

/* google.protobuf.DescriptorProto */

WPI_UPB_INLINE google_protobuf_DescriptorProto* google_protobuf_DescriptorProto_new(wpi_upb_Arena* arena) {
  return (google_protobuf_DescriptorProto*)_wpi_upb_Message_New(&google__protobuf__DescriptorProto_msg_init, arena);
}
WPI_UPB_INLINE google_protobuf_DescriptorProto* google_protobuf_DescriptorProto_parse(const char* buf, size_t size, wpi_upb_Arena* arena) {
  google_protobuf_DescriptorProto* ret = google_protobuf_DescriptorProto_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__DescriptorProto_msg_init, NULL, 0, arena) !=
      kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE google_protobuf_DescriptorProto* google_protobuf_DescriptorProto_parse_ex(const char* buf, size_t size,
                           const wpi_upb_ExtensionRegistry* extreg,
                           int options, wpi_upb_Arena* arena) {
  google_protobuf_DescriptorProto* ret = google_protobuf_DescriptorProto_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__DescriptorProto_msg_init, extreg, options,
                 arena) != kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE char* google_protobuf_DescriptorProto_serialize(const google_protobuf_DescriptorProto* msg, wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__DescriptorProto_msg_init, 0, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE char* google_protobuf_DescriptorProto_serialize_ex(const google_protobuf_DescriptorProto* msg, int options,
                                 wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__DescriptorProto_msg_init, options, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE void google_protobuf_DescriptorProto_clear_name(google_protobuf_DescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(48, 16), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE wpi_upb_StringView google_protobuf_DescriptorProto_name(const google_protobuf_DescriptorProto* msg) {
  wpi_upb_StringView default_val = wpi_upb_StringView_FromString("");
  wpi_upb_StringView ret;
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(48, 16), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_DescriptorProto_has_name(const google_protobuf_DescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(48, 16), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_DescriptorProto_clear_field(google_protobuf_DescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(12, 32), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_FieldDescriptorProto* const* google_protobuf_DescriptorProto_field(const google_protobuf_DescriptorProto* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(12, 32), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FieldDescriptorProto_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (const google_protobuf_FieldDescriptorProto* const*)wpi_upb_Array_DataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE const wpi_upb_Array* _google_protobuf_DescriptorProto_field_wpi_upb_array(const google_protobuf_DescriptorProto* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(12, 32), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FieldDescriptorProto_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE wpi_upb_Array* _google_protobuf_DescriptorProto_field_mutable_wpi_upb_array(google_protobuf_DescriptorProto* msg, size_t* size, wpi_upb_Arena* arena) {
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(12, 32), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FieldDescriptorProto_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(WPI_UPB_UPCAST(msg),
                                                       &field, arena);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE void google_protobuf_DescriptorProto_clear_nested_type(google_protobuf_DescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(16, 40), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_DescriptorProto* const* google_protobuf_DescriptorProto_nested_type(const google_protobuf_DescriptorProto* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(16, 40), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__DescriptorProto_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (const google_protobuf_DescriptorProto* const*)wpi_upb_Array_DataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE const wpi_upb_Array* _google_protobuf_DescriptorProto_nested_type_wpi_upb_array(const google_protobuf_DescriptorProto* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(16, 40), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__DescriptorProto_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE wpi_upb_Array* _google_protobuf_DescriptorProto_nested_type_mutable_wpi_upb_array(google_protobuf_DescriptorProto* msg, size_t* size, wpi_upb_Arena* arena) {
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(16, 40), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__DescriptorProto_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(WPI_UPB_UPCAST(msg),
                                                       &field, arena);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE void google_protobuf_DescriptorProto_clear_enum_type(google_protobuf_DescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {4, WPI_UPB_SIZE(20, 48), 0, 2, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_EnumDescriptorProto* const* google_protobuf_DescriptorProto_enum_type(const google_protobuf_DescriptorProto* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {4, WPI_UPB_SIZE(20, 48), 0, 2, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__EnumDescriptorProto_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (const google_protobuf_EnumDescriptorProto* const*)wpi_upb_Array_DataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE const wpi_upb_Array* _google_protobuf_DescriptorProto_enum_type_wpi_upb_array(const google_protobuf_DescriptorProto* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {4, WPI_UPB_SIZE(20, 48), 0, 2, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__EnumDescriptorProto_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE wpi_upb_Array* _google_protobuf_DescriptorProto_enum_type_mutable_wpi_upb_array(google_protobuf_DescriptorProto* msg, size_t* size, wpi_upb_Arena* arena) {
  const wpi_upb_MiniTableField field = {4, WPI_UPB_SIZE(20, 48), 0, 2, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__EnumDescriptorProto_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(WPI_UPB_UPCAST(msg),
                                                       &field, arena);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE void google_protobuf_DescriptorProto_clear_extension_range(google_protobuf_DescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {5, WPI_UPB_SIZE(24, 56), 0, 3, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_DescriptorProto_ExtensionRange* const* google_protobuf_DescriptorProto_extension_range(const google_protobuf_DescriptorProto* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {5, WPI_UPB_SIZE(24, 56), 0, 3, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__DescriptorProto__ExtensionRange_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (const google_protobuf_DescriptorProto_ExtensionRange* const*)wpi_upb_Array_DataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE const wpi_upb_Array* _google_protobuf_DescriptorProto_extension_range_wpi_upb_array(const google_protobuf_DescriptorProto* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {5, WPI_UPB_SIZE(24, 56), 0, 3, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__DescriptorProto__ExtensionRange_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE wpi_upb_Array* _google_protobuf_DescriptorProto_extension_range_mutable_wpi_upb_array(google_protobuf_DescriptorProto* msg, size_t* size, wpi_upb_Arena* arena) {
  const wpi_upb_MiniTableField field = {5, WPI_UPB_SIZE(24, 56), 0, 3, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__DescriptorProto__ExtensionRange_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(WPI_UPB_UPCAST(msg),
                                                       &field, arena);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE void google_protobuf_DescriptorProto_clear_extension(google_protobuf_DescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {6, WPI_UPB_SIZE(28, 64), 0, 4, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_FieldDescriptorProto* const* google_protobuf_DescriptorProto_extension(const google_protobuf_DescriptorProto* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {6, WPI_UPB_SIZE(28, 64), 0, 4, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FieldDescriptorProto_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (const google_protobuf_FieldDescriptorProto* const*)wpi_upb_Array_DataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE const wpi_upb_Array* _google_protobuf_DescriptorProto_extension_wpi_upb_array(const google_protobuf_DescriptorProto* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {6, WPI_UPB_SIZE(28, 64), 0, 4, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FieldDescriptorProto_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE wpi_upb_Array* _google_protobuf_DescriptorProto_extension_mutable_wpi_upb_array(google_protobuf_DescriptorProto* msg, size_t* size, wpi_upb_Arena* arena) {
  const wpi_upb_MiniTableField field = {6, WPI_UPB_SIZE(28, 64), 0, 4, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FieldDescriptorProto_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(WPI_UPB_UPCAST(msg),
                                                       &field, arena);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE void google_protobuf_DescriptorProto_clear_options(google_protobuf_DescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {7, WPI_UPB_SIZE(32, 72), 65, 5, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_MessageOptions* google_protobuf_DescriptorProto_options(const google_protobuf_DescriptorProto* msg) {
  const google_protobuf_MessageOptions* default_val = NULL;
  const google_protobuf_MessageOptions* ret;
  const wpi_upb_MiniTableField field = {7, WPI_UPB_SIZE(32, 72), 65, 5, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__MessageOptions_msg_init);
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_DescriptorProto_has_options(const google_protobuf_DescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {7, WPI_UPB_SIZE(32, 72), 65, 5, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_DescriptorProto_clear_oneof_decl(google_protobuf_DescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {8, WPI_UPB_SIZE(36, 80), 0, 6, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_OneofDescriptorProto* const* google_protobuf_DescriptorProto_oneof_decl(const google_protobuf_DescriptorProto* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {8, WPI_UPB_SIZE(36, 80), 0, 6, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__OneofDescriptorProto_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (const google_protobuf_OneofDescriptorProto* const*)wpi_upb_Array_DataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE const wpi_upb_Array* _google_protobuf_DescriptorProto_oneof_decl_wpi_upb_array(const google_protobuf_DescriptorProto* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {8, WPI_UPB_SIZE(36, 80), 0, 6, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__OneofDescriptorProto_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE wpi_upb_Array* _google_protobuf_DescriptorProto_oneof_decl_mutable_wpi_upb_array(google_protobuf_DescriptorProto* msg, size_t* size, wpi_upb_Arena* arena) {
  const wpi_upb_MiniTableField field = {8, WPI_UPB_SIZE(36, 80), 0, 6, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__OneofDescriptorProto_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(WPI_UPB_UPCAST(msg),
                                                       &field, arena);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE void google_protobuf_DescriptorProto_clear_reserved_range(google_protobuf_DescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {9, WPI_UPB_SIZE(40, 88), 0, 7, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_DescriptorProto_ReservedRange* const* google_protobuf_DescriptorProto_reserved_range(const google_protobuf_DescriptorProto* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {9, WPI_UPB_SIZE(40, 88), 0, 7, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__DescriptorProto__ReservedRange_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (const google_protobuf_DescriptorProto_ReservedRange* const*)wpi_upb_Array_DataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE const wpi_upb_Array* _google_protobuf_DescriptorProto_reserved_range_wpi_upb_array(const google_protobuf_DescriptorProto* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {9, WPI_UPB_SIZE(40, 88), 0, 7, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__DescriptorProto__ReservedRange_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE wpi_upb_Array* _google_protobuf_DescriptorProto_reserved_range_mutable_wpi_upb_array(google_protobuf_DescriptorProto* msg, size_t* size, wpi_upb_Arena* arena) {
  const wpi_upb_MiniTableField field = {9, WPI_UPB_SIZE(40, 88), 0, 7, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__DescriptorProto__ReservedRange_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(WPI_UPB_UPCAST(msg),
                                                       &field, arena);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE void google_protobuf_DescriptorProto_clear_reserved_name(google_protobuf_DescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {10, WPI_UPB_SIZE(44, 96), 0, kUpb_NoSub, 12, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsAlternate | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE wpi_upb_StringView const* google_protobuf_DescriptorProto_reserved_name(const google_protobuf_DescriptorProto* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {10, WPI_UPB_SIZE(44, 96), 0, kUpb_NoSub, 12, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsAlternate | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (wpi_upb_StringView const*)wpi_upb_Array_DataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE const wpi_upb_Array* _google_protobuf_DescriptorProto_reserved_name_wpi_upb_array(const google_protobuf_DescriptorProto* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {10, WPI_UPB_SIZE(44, 96), 0, kUpb_NoSub, 12, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsAlternate | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE wpi_upb_Array* _google_protobuf_DescriptorProto_reserved_name_mutable_wpi_upb_array(google_protobuf_DescriptorProto* msg, size_t* size, wpi_upb_Arena* arena) {
  const wpi_upb_MiniTableField field = {10, WPI_UPB_SIZE(44, 96), 0, kUpb_NoSub, 12, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsAlternate | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(WPI_UPB_UPCAST(msg),
                                                       &field, arena);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}

WPI_UPB_INLINE void google_protobuf_DescriptorProto_set_name(google_protobuf_DescriptorProto *msg, wpi_upb_StringView value) {
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(48, 16), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE google_protobuf_FieldDescriptorProto** google_protobuf_DescriptorProto_mutable_field(google_protobuf_DescriptorProto* msg, size_t* size) {
  wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(12, 32), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FieldDescriptorProto_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetMutableArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (google_protobuf_FieldDescriptorProto**)wpi_upb_Array_MutableDataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE google_protobuf_FieldDescriptorProto** google_protobuf_DescriptorProto_resize_field(google_protobuf_DescriptorProto* msg, size_t size, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(12, 32), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return (google_protobuf_FieldDescriptorProto**)wpi_upb_Message_ResizeArrayUninitialized(WPI_UPB_UPCAST(msg),
                                                   &field, size, arena);
}
WPI_UPB_INLINE struct google_protobuf_FieldDescriptorProto* google_protobuf_DescriptorProto_add_field(google_protobuf_DescriptorProto* msg, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(12, 32), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FieldDescriptorProto_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(
      WPI_UPB_UPCAST(msg), &field, arena);
  if (!arr || !WPI_UPB_PRIVATE(_wpi_upb_Array_ResizeUninitialized)(
                  arr, arr->WPI_UPB_PRIVATE(size) + 1, arena)) {
    return NULL;
  }
  struct google_protobuf_FieldDescriptorProto* sub = (struct google_protobuf_FieldDescriptorProto*)_wpi_upb_Message_New(&google__protobuf__FieldDescriptorProto_msg_init, arena);
  if (!arr || !sub) return NULL;
  WPI_UPB_PRIVATE(_wpi_upb_Array_Set)
  (arr, arr->WPI_UPB_PRIVATE(size) - 1, &sub, sizeof(sub));
  return sub;
}
WPI_UPB_INLINE google_protobuf_DescriptorProto** google_protobuf_DescriptorProto_mutable_nested_type(google_protobuf_DescriptorProto* msg, size_t* size) {
  wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(16, 40), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__DescriptorProto_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetMutableArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (google_protobuf_DescriptorProto**)wpi_upb_Array_MutableDataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE google_protobuf_DescriptorProto** google_protobuf_DescriptorProto_resize_nested_type(google_protobuf_DescriptorProto* msg, size_t size, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(16, 40), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return (google_protobuf_DescriptorProto**)wpi_upb_Message_ResizeArrayUninitialized(WPI_UPB_UPCAST(msg),
                                                   &field, size, arena);
}
WPI_UPB_INLINE struct google_protobuf_DescriptorProto* google_protobuf_DescriptorProto_add_nested_type(google_protobuf_DescriptorProto* msg, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(16, 40), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__DescriptorProto_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(
      WPI_UPB_UPCAST(msg), &field, arena);
  if (!arr || !WPI_UPB_PRIVATE(_wpi_upb_Array_ResizeUninitialized)(
                  arr, arr->WPI_UPB_PRIVATE(size) + 1, arena)) {
    return NULL;
  }
  struct google_protobuf_DescriptorProto* sub = (struct google_protobuf_DescriptorProto*)_wpi_upb_Message_New(&google__protobuf__DescriptorProto_msg_init, arena);
  if (!arr || !sub) return NULL;
  WPI_UPB_PRIVATE(_wpi_upb_Array_Set)
  (arr, arr->WPI_UPB_PRIVATE(size) - 1, &sub, sizeof(sub));
  return sub;
}
WPI_UPB_INLINE google_protobuf_EnumDescriptorProto** google_protobuf_DescriptorProto_mutable_enum_type(google_protobuf_DescriptorProto* msg, size_t* size) {
  wpi_upb_MiniTableField field = {4, WPI_UPB_SIZE(20, 48), 0, 2, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__EnumDescriptorProto_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetMutableArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (google_protobuf_EnumDescriptorProto**)wpi_upb_Array_MutableDataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE google_protobuf_EnumDescriptorProto** google_protobuf_DescriptorProto_resize_enum_type(google_protobuf_DescriptorProto* msg, size_t size, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {4, WPI_UPB_SIZE(20, 48), 0, 2, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return (google_protobuf_EnumDescriptorProto**)wpi_upb_Message_ResizeArrayUninitialized(WPI_UPB_UPCAST(msg),
                                                   &field, size, arena);
}
WPI_UPB_INLINE struct google_protobuf_EnumDescriptorProto* google_protobuf_DescriptorProto_add_enum_type(google_protobuf_DescriptorProto* msg, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {4, WPI_UPB_SIZE(20, 48), 0, 2, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__EnumDescriptorProto_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(
      WPI_UPB_UPCAST(msg), &field, arena);
  if (!arr || !WPI_UPB_PRIVATE(_wpi_upb_Array_ResizeUninitialized)(
                  arr, arr->WPI_UPB_PRIVATE(size) + 1, arena)) {
    return NULL;
  }
  struct google_protobuf_EnumDescriptorProto* sub = (struct google_protobuf_EnumDescriptorProto*)_wpi_upb_Message_New(&google__protobuf__EnumDescriptorProto_msg_init, arena);
  if (!arr || !sub) return NULL;
  WPI_UPB_PRIVATE(_wpi_upb_Array_Set)
  (arr, arr->WPI_UPB_PRIVATE(size) - 1, &sub, sizeof(sub));
  return sub;
}
WPI_UPB_INLINE google_protobuf_DescriptorProto_ExtensionRange** google_protobuf_DescriptorProto_mutable_extension_range(google_protobuf_DescriptorProto* msg, size_t* size) {
  wpi_upb_MiniTableField field = {5, WPI_UPB_SIZE(24, 56), 0, 3, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__DescriptorProto__ExtensionRange_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetMutableArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (google_protobuf_DescriptorProto_ExtensionRange**)wpi_upb_Array_MutableDataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE google_protobuf_DescriptorProto_ExtensionRange** google_protobuf_DescriptorProto_resize_extension_range(google_protobuf_DescriptorProto* msg, size_t size, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {5, WPI_UPB_SIZE(24, 56), 0, 3, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return (google_protobuf_DescriptorProto_ExtensionRange**)wpi_upb_Message_ResizeArrayUninitialized(WPI_UPB_UPCAST(msg),
                                                   &field, size, arena);
}
WPI_UPB_INLINE struct google_protobuf_DescriptorProto_ExtensionRange* google_protobuf_DescriptorProto_add_extension_range(google_protobuf_DescriptorProto* msg, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {5, WPI_UPB_SIZE(24, 56), 0, 3, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__DescriptorProto__ExtensionRange_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(
      WPI_UPB_UPCAST(msg), &field, arena);
  if (!arr || !WPI_UPB_PRIVATE(_wpi_upb_Array_ResizeUninitialized)(
                  arr, arr->WPI_UPB_PRIVATE(size) + 1, arena)) {
    return NULL;
  }
  struct google_protobuf_DescriptorProto_ExtensionRange* sub = (struct google_protobuf_DescriptorProto_ExtensionRange*)_wpi_upb_Message_New(&google__protobuf__DescriptorProto__ExtensionRange_msg_init, arena);
  if (!arr || !sub) return NULL;
  WPI_UPB_PRIVATE(_wpi_upb_Array_Set)
  (arr, arr->WPI_UPB_PRIVATE(size) - 1, &sub, sizeof(sub));
  return sub;
}
WPI_UPB_INLINE google_protobuf_FieldDescriptorProto** google_protobuf_DescriptorProto_mutable_extension(google_protobuf_DescriptorProto* msg, size_t* size) {
  wpi_upb_MiniTableField field = {6, WPI_UPB_SIZE(28, 64), 0, 4, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FieldDescriptorProto_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetMutableArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (google_protobuf_FieldDescriptorProto**)wpi_upb_Array_MutableDataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE google_protobuf_FieldDescriptorProto** google_protobuf_DescriptorProto_resize_extension(google_protobuf_DescriptorProto* msg, size_t size, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {6, WPI_UPB_SIZE(28, 64), 0, 4, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return (google_protobuf_FieldDescriptorProto**)wpi_upb_Message_ResizeArrayUninitialized(WPI_UPB_UPCAST(msg),
                                                   &field, size, arena);
}
WPI_UPB_INLINE struct google_protobuf_FieldDescriptorProto* google_protobuf_DescriptorProto_add_extension(google_protobuf_DescriptorProto* msg, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {6, WPI_UPB_SIZE(28, 64), 0, 4, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FieldDescriptorProto_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(
      WPI_UPB_UPCAST(msg), &field, arena);
  if (!arr || !WPI_UPB_PRIVATE(_wpi_upb_Array_ResizeUninitialized)(
                  arr, arr->WPI_UPB_PRIVATE(size) + 1, arena)) {
    return NULL;
  }
  struct google_protobuf_FieldDescriptorProto* sub = (struct google_protobuf_FieldDescriptorProto*)_wpi_upb_Message_New(&google__protobuf__FieldDescriptorProto_msg_init, arena);
  if (!arr || !sub) return NULL;
  WPI_UPB_PRIVATE(_wpi_upb_Array_Set)
  (arr, arr->WPI_UPB_PRIVATE(size) - 1, &sub, sizeof(sub));
  return sub;
}
WPI_UPB_INLINE void google_protobuf_DescriptorProto_set_options(google_protobuf_DescriptorProto *msg, google_protobuf_MessageOptions* value) {
  const wpi_upb_MiniTableField field = {7, WPI_UPB_SIZE(32, 72), 65, 5, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__MessageOptions_msg_init);
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE struct google_protobuf_MessageOptions* google_protobuf_DescriptorProto_mutable_options(google_protobuf_DescriptorProto* msg, wpi_upb_Arena* arena) {
  struct google_protobuf_MessageOptions* sub = (struct google_protobuf_MessageOptions*)google_protobuf_DescriptorProto_options(msg);
  if (sub == NULL) {
    sub = (struct google_protobuf_MessageOptions*)_wpi_upb_Message_New(&google__protobuf__MessageOptions_msg_init, arena);
    if (sub) google_protobuf_DescriptorProto_set_options(msg, sub);
  }
  return sub;
}
WPI_UPB_INLINE google_protobuf_OneofDescriptorProto** google_protobuf_DescriptorProto_mutable_oneof_decl(google_protobuf_DescriptorProto* msg, size_t* size) {
  wpi_upb_MiniTableField field = {8, WPI_UPB_SIZE(36, 80), 0, 6, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__OneofDescriptorProto_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetMutableArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (google_protobuf_OneofDescriptorProto**)wpi_upb_Array_MutableDataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE google_protobuf_OneofDescriptorProto** google_protobuf_DescriptorProto_resize_oneof_decl(google_protobuf_DescriptorProto* msg, size_t size, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {8, WPI_UPB_SIZE(36, 80), 0, 6, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return (google_protobuf_OneofDescriptorProto**)wpi_upb_Message_ResizeArrayUninitialized(WPI_UPB_UPCAST(msg),
                                                   &field, size, arena);
}
WPI_UPB_INLINE struct google_protobuf_OneofDescriptorProto* google_protobuf_DescriptorProto_add_oneof_decl(google_protobuf_DescriptorProto* msg, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {8, WPI_UPB_SIZE(36, 80), 0, 6, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__OneofDescriptorProto_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(
      WPI_UPB_UPCAST(msg), &field, arena);
  if (!arr || !WPI_UPB_PRIVATE(_wpi_upb_Array_ResizeUninitialized)(
                  arr, arr->WPI_UPB_PRIVATE(size) + 1, arena)) {
    return NULL;
  }
  struct google_protobuf_OneofDescriptorProto* sub = (struct google_protobuf_OneofDescriptorProto*)_wpi_upb_Message_New(&google__protobuf__OneofDescriptorProto_msg_init, arena);
  if (!arr || !sub) return NULL;
  WPI_UPB_PRIVATE(_wpi_upb_Array_Set)
  (arr, arr->WPI_UPB_PRIVATE(size) - 1, &sub, sizeof(sub));
  return sub;
}
WPI_UPB_INLINE google_protobuf_DescriptorProto_ReservedRange** google_protobuf_DescriptorProto_mutable_reserved_range(google_protobuf_DescriptorProto* msg, size_t* size) {
  wpi_upb_MiniTableField field = {9, WPI_UPB_SIZE(40, 88), 0, 7, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__DescriptorProto__ReservedRange_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetMutableArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (google_protobuf_DescriptorProto_ReservedRange**)wpi_upb_Array_MutableDataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE google_protobuf_DescriptorProto_ReservedRange** google_protobuf_DescriptorProto_resize_reserved_range(google_protobuf_DescriptorProto* msg, size_t size, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {9, WPI_UPB_SIZE(40, 88), 0, 7, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return (google_protobuf_DescriptorProto_ReservedRange**)wpi_upb_Message_ResizeArrayUninitialized(WPI_UPB_UPCAST(msg),
                                                   &field, size, arena);
}
WPI_UPB_INLINE struct google_protobuf_DescriptorProto_ReservedRange* google_protobuf_DescriptorProto_add_reserved_range(google_protobuf_DescriptorProto* msg, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {9, WPI_UPB_SIZE(40, 88), 0, 7, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__DescriptorProto__ReservedRange_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(
      WPI_UPB_UPCAST(msg), &field, arena);
  if (!arr || !WPI_UPB_PRIVATE(_wpi_upb_Array_ResizeUninitialized)(
                  arr, arr->WPI_UPB_PRIVATE(size) + 1, arena)) {
    return NULL;
  }
  struct google_protobuf_DescriptorProto_ReservedRange* sub = (struct google_protobuf_DescriptorProto_ReservedRange*)_wpi_upb_Message_New(&google__protobuf__DescriptorProto__ReservedRange_msg_init, arena);
  if (!arr || !sub) return NULL;
  WPI_UPB_PRIVATE(_wpi_upb_Array_Set)
  (arr, arr->WPI_UPB_PRIVATE(size) - 1, &sub, sizeof(sub));
  return sub;
}
WPI_UPB_INLINE wpi_upb_StringView* google_protobuf_DescriptorProto_mutable_reserved_name(google_protobuf_DescriptorProto* msg, size_t* size) {
  wpi_upb_MiniTableField field = {10, WPI_UPB_SIZE(44, 96), 0, kUpb_NoSub, 12, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsAlternate | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Array* arr = wpi_upb_Message_GetMutableArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (wpi_upb_StringView*)wpi_upb_Array_MutableDataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE wpi_upb_StringView* google_protobuf_DescriptorProto_resize_reserved_name(google_protobuf_DescriptorProto* msg, size_t size, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {10, WPI_UPB_SIZE(44, 96), 0, kUpb_NoSub, 12, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsAlternate | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return (wpi_upb_StringView*)wpi_upb_Message_ResizeArrayUninitialized(WPI_UPB_UPCAST(msg),
                                                   &field, size, arena);
}
WPI_UPB_INLINE bool google_protobuf_DescriptorProto_add_reserved_name(google_protobuf_DescriptorProto* msg, wpi_upb_StringView val, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {10, WPI_UPB_SIZE(44, 96), 0, kUpb_NoSub, 12, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsAlternate | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(
      WPI_UPB_UPCAST(msg), &field, arena);
  if (!arr || !WPI_UPB_PRIVATE(_wpi_upb_Array_ResizeUninitialized)(
                  arr, arr->WPI_UPB_PRIVATE(size) + 1, arena)) {
    return false;
  }
  WPI_UPB_PRIVATE(_wpi_upb_Array_Set)
  (arr, arr->WPI_UPB_PRIVATE(size) - 1, &val, sizeof(val));
  return true;
}

/* google.protobuf.DescriptorProto.ExtensionRange */

WPI_UPB_INLINE google_protobuf_DescriptorProto_ExtensionRange* google_protobuf_DescriptorProto_ExtensionRange_new(wpi_upb_Arena* arena) {
  return (google_protobuf_DescriptorProto_ExtensionRange*)_wpi_upb_Message_New(&google__protobuf__DescriptorProto__ExtensionRange_msg_init, arena);
}
WPI_UPB_INLINE google_protobuf_DescriptorProto_ExtensionRange* google_protobuf_DescriptorProto_ExtensionRange_parse(const char* buf, size_t size, wpi_upb_Arena* arena) {
  google_protobuf_DescriptorProto_ExtensionRange* ret = google_protobuf_DescriptorProto_ExtensionRange_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__DescriptorProto__ExtensionRange_msg_init, NULL, 0, arena) !=
      kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE google_protobuf_DescriptorProto_ExtensionRange* google_protobuf_DescriptorProto_ExtensionRange_parse_ex(const char* buf, size_t size,
                           const wpi_upb_ExtensionRegistry* extreg,
                           int options, wpi_upb_Arena* arena) {
  google_protobuf_DescriptorProto_ExtensionRange* ret = google_protobuf_DescriptorProto_ExtensionRange_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__DescriptorProto__ExtensionRange_msg_init, extreg, options,
                 arena) != kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE char* google_protobuf_DescriptorProto_ExtensionRange_serialize(const google_protobuf_DescriptorProto_ExtensionRange* msg, wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__DescriptorProto__ExtensionRange_msg_init, 0, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE char* google_protobuf_DescriptorProto_ExtensionRange_serialize_ex(const google_protobuf_DescriptorProto_ExtensionRange* msg, int options,
                                 wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__DescriptorProto__ExtensionRange_msg_init, options, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE void google_protobuf_DescriptorProto_ExtensionRange_clear_start(google_protobuf_DescriptorProto_ExtensionRange* msg) {
  const wpi_upb_MiniTableField field = {1, 12, 64, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE int32_t google_protobuf_DescriptorProto_ExtensionRange_start(const google_protobuf_DescriptorProto_ExtensionRange* msg) {
  int32_t default_val = (int32_t)0;
  int32_t ret;
  const wpi_upb_MiniTableField field = {1, 12, 64, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_DescriptorProto_ExtensionRange_has_start(const google_protobuf_DescriptorProto_ExtensionRange* msg) {
  const wpi_upb_MiniTableField field = {1, 12, 64, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_DescriptorProto_ExtensionRange_clear_end(google_protobuf_DescriptorProto_ExtensionRange* msg) {
  const wpi_upb_MiniTableField field = {2, 16, 65, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE int32_t google_protobuf_DescriptorProto_ExtensionRange_end(const google_protobuf_DescriptorProto_ExtensionRange* msg) {
  int32_t default_val = (int32_t)0;
  int32_t ret;
  const wpi_upb_MiniTableField field = {2, 16, 65, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_DescriptorProto_ExtensionRange_has_end(const google_protobuf_DescriptorProto_ExtensionRange* msg) {
  const wpi_upb_MiniTableField field = {2, 16, 65, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_DescriptorProto_ExtensionRange_clear_options(google_protobuf_DescriptorProto_ExtensionRange* msg) {
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(20, 24), 66, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_ExtensionRangeOptions* google_protobuf_DescriptorProto_ExtensionRange_options(const google_protobuf_DescriptorProto_ExtensionRange* msg) {
  const google_protobuf_ExtensionRangeOptions* default_val = NULL;
  const google_protobuf_ExtensionRangeOptions* ret;
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(20, 24), 66, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__ExtensionRangeOptions_msg_init);
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_DescriptorProto_ExtensionRange_has_options(const google_protobuf_DescriptorProto_ExtensionRange* msg) {
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(20, 24), 66, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}

WPI_UPB_INLINE void google_protobuf_DescriptorProto_ExtensionRange_set_start(google_protobuf_DescriptorProto_ExtensionRange *msg, int32_t value) {
  const wpi_upb_MiniTableField field = {1, 12, 64, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_DescriptorProto_ExtensionRange_set_end(google_protobuf_DescriptorProto_ExtensionRange *msg, int32_t value) {
  const wpi_upb_MiniTableField field = {2, 16, 65, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_DescriptorProto_ExtensionRange_set_options(google_protobuf_DescriptorProto_ExtensionRange *msg, google_protobuf_ExtensionRangeOptions* value) {
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(20, 24), 66, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__ExtensionRangeOptions_msg_init);
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE struct google_protobuf_ExtensionRangeOptions* google_protobuf_DescriptorProto_ExtensionRange_mutable_options(google_protobuf_DescriptorProto_ExtensionRange* msg, wpi_upb_Arena* arena) {
  struct google_protobuf_ExtensionRangeOptions* sub = (struct google_protobuf_ExtensionRangeOptions*)google_protobuf_DescriptorProto_ExtensionRange_options(msg);
  if (sub == NULL) {
    sub = (struct google_protobuf_ExtensionRangeOptions*)_wpi_upb_Message_New(&google__protobuf__ExtensionRangeOptions_msg_init, arena);
    if (sub) google_protobuf_DescriptorProto_ExtensionRange_set_options(msg, sub);
  }
  return sub;
}

/* google.protobuf.DescriptorProto.ReservedRange */

WPI_UPB_INLINE google_protobuf_DescriptorProto_ReservedRange* google_protobuf_DescriptorProto_ReservedRange_new(wpi_upb_Arena* arena) {
  return (google_protobuf_DescriptorProto_ReservedRange*)_wpi_upb_Message_New(&google__protobuf__DescriptorProto__ReservedRange_msg_init, arena);
}
WPI_UPB_INLINE google_protobuf_DescriptorProto_ReservedRange* google_protobuf_DescriptorProto_ReservedRange_parse(const char* buf, size_t size, wpi_upb_Arena* arena) {
  google_protobuf_DescriptorProto_ReservedRange* ret = google_protobuf_DescriptorProto_ReservedRange_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__DescriptorProto__ReservedRange_msg_init, NULL, 0, arena) !=
      kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE google_protobuf_DescriptorProto_ReservedRange* google_protobuf_DescriptorProto_ReservedRange_parse_ex(const char* buf, size_t size,
                           const wpi_upb_ExtensionRegistry* extreg,
                           int options, wpi_upb_Arena* arena) {
  google_protobuf_DescriptorProto_ReservedRange* ret = google_protobuf_DescriptorProto_ReservedRange_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__DescriptorProto__ReservedRange_msg_init, extreg, options,
                 arena) != kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE char* google_protobuf_DescriptorProto_ReservedRange_serialize(const google_protobuf_DescriptorProto_ReservedRange* msg, wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__DescriptorProto__ReservedRange_msg_init, 0, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE char* google_protobuf_DescriptorProto_ReservedRange_serialize_ex(const google_protobuf_DescriptorProto_ReservedRange* msg, int options,
                                 wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__DescriptorProto__ReservedRange_msg_init, options, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE void google_protobuf_DescriptorProto_ReservedRange_clear_start(google_protobuf_DescriptorProto_ReservedRange* msg) {
  const wpi_upb_MiniTableField field = {1, 12, 64, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE int32_t google_protobuf_DescriptorProto_ReservedRange_start(const google_protobuf_DescriptorProto_ReservedRange* msg) {
  int32_t default_val = (int32_t)0;
  int32_t ret;
  const wpi_upb_MiniTableField field = {1, 12, 64, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_DescriptorProto_ReservedRange_has_start(const google_protobuf_DescriptorProto_ReservedRange* msg) {
  const wpi_upb_MiniTableField field = {1, 12, 64, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_DescriptorProto_ReservedRange_clear_end(google_protobuf_DescriptorProto_ReservedRange* msg) {
  const wpi_upb_MiniTableField field = {2, 16, 65, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE int32_t google_protobuf_DescriptorProto_ReservedRange_end(const google_protobuf_DescriptorProto_ReservedRange* msg) {
  int32_t default_val = (int32_t)0;
  int32_t ret;
  const wpi_upb_MiniTableField field = {2, 16, 65, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_DescriptorProto_ReservedRange_has_end(const google_protobuf_DescriptorProto_ReservedRange* msg) {
  const wpi_upb_MiniTableField field = {2, 16, 65, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}

WPI_UPB_INLINE void google_protobuf_DescriptorProto_ReservedRange_set_start(google_protobuf_DescriptorProto_ReservedRange *msg, int32_t value) {
  const wpi_upb_MiniTableField field = {1, 12, 64, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_DescriptorProto_ReservedRange_set_end(google_protobuf_DescriptorProto_ReservedRange *msg, int32_t value) {
  const wpi_upb_MiniTableField field = {2, 16, 65, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}

/* google.protobuf.ExtensionRangeOptions */

WPI_UPB_INLINE google_protobuf_ExtensionRangeOptions* google_protobuf_ExtensionRangeOptions_new(wpi_upb_Arena* arena) {
  return (google_protobuf_ExtensionRangeOptions*)_wpi_upb_Message_New(&google__protobuf__ExtensionRangeOptions_msg_init, arena);
}
WPI_UPB_INLINE google_protobuf_ExtensionRangeOptions* google_protobuf_ExtensionRangeOptions_parse(const char* buf, size_t size, wpi_upb_Arena* arena) {
  google_protobuf_ExtensionRangeOptions* ret = google_protobuf_ExtensionRangeOptions_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__ExtensionRangeOptions_msg_init, NULL, 0, arena) !=
      kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE google_protobuf_ExtensionRangeOptions* google_protobuf_ExtensionRangeOptions_parse_ex(const char* buf, size_t size,
                           const wpi_upb_ExtensionRegistry* extreg,
                           int options, wpi_upb_Arena* arena) {
  google_protobuf_ExtensionRangeOptions* ret = google_protobuf_ExtensionRangeOptions_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__ExtensionRangeOptions_msg_init, extreg, options,
                 arena) != kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE char* google_protobuf_ExtensionRangeOptions_serialize(const google_protobuf_ExtensionRangeOptions* msg, wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__ExtensionRangeOptions_msg_init, 0, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE char* google_protobuf_ExtensionRangeOptions_serialize_ex(const google_protobuf_ExtensionRangeOptions* msg, int options,
                                 wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__ExtensionRangeOptions_msg_init, options, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE void google_protobuf_ExtensionRangeOptions_clear_declaration(google_protobuf_ExtensionRangeOptions* msg) {
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(12, 16), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_ExtensionRangeOptions_Declaration* const* google_protobuf_ExtensionRangeOptions_declaration(const google_protobuf_ExtensionRangeOptions* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(12, 16), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__ExtensionRangeOptions__Declaration_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (const google_protobuf_ExtensionRangeOptions_Declaration* const*)wpi_upb_Array_DataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE const wpi_upb_Array* _google_protobuf_ExtensionRangeOptions_declaration_wpi_upb_array(const google_protobuf_ExtensionRangeOptions* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(12, 16), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__ExtensionRangeOptions__Declaration_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE wpi_upb_Array* _google_protobuf_ExtensionRangeOptions_declaration_mutable_wpi_upb_array(google_protobuf_ExtensionRangeOptions* msg, size_t* size, wpi_upb_Arena* arena) {
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(12, 16), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__ExtensionRangeOptions__Declaration_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(WPI_UPB_UPCAST(msg),
                                                       &field, arena);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE void google_protobuf_ExtensionRangeOptions_clear_verification(google_protobuf_ExtensionRangeOptions* msg) {
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(16, 12), 64, 3, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE int32_t google_protobuf_ExtensionRangeOptions_verification(const google_protobuf_ExtensionRangeOptions* msg) {
  int32_t default_val = 1;
  int32_t ret;
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(16, 12), 64, 3, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_ExtensionRangeOptions_has_verification(const google_protobuf_ExtensionRangeOptions* msg) {
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(16, 12), 64, 3, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_ExtensionRangeOptions_clear_features(google_protobuf_ExtensionRangeOptions* msg) {
  const wpi_upb_MiniTableField field = {50, WPI_UPB_SIZE(20, 24), 65, 1, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_FeatureSet* google_protobuf_ExtensionRangeOptions_features(const google_protobuf_ExtensionRangeOptions* msg) {
  const google_protobuf_FeatureSet* default_val = NULL;
  const google_protobuf_FeatureSet* ret;
  const wpi_upb_MiniTableField field = {50, WPI_UPB_SIZE(20, 24), 65, 1, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FeatureSet_msg_init);
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_ExtensionRangeOptions_has_features(const google_protobuf_ExtensionRangeOptions* msg) {
  const wpi_upb_MiniTableField field = {50, WPI_UPB_SIZE(20, 24), 65, 1, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_ExtensionRangeOptions_clear_uninterpreted_option(google_protobuf_ExtensionRangeOptions* msg) {
  const wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(24, 32), 0, 2, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_UninterpretedOption* const* google_protobuf_ExtensionRangeOptions_uninterpreted_option(const google_protobuf_ExtensionRangeOptions* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(24, 32), 0, 2, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__UninterpretedOption_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (const google_protobuf_UninterpretedOption* const*)wpi_upb_Array_DataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE const wpi_upb_Array* _google_protobuf_ExtensionRangeOptions_uninterpreted_option_wpi_upb_array(const google_protobuf_ExtensionRangeOptions* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(24, 32), 0, 2, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__UninterpretedOption_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE wpi_upb_Array* _google_protobuf_ExtensionRangeOptions_uninterpreted_option_mutable_wpi_upb_array(google_protobuf_ExtensionRangeOptions* msg, size_t* size, wpi_upb_Arena* arena) {
  const wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(24, 32), 0, 2, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__UninterpretedOption_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(WPI_UPB_UPCAST(msg),
                                                       &field, arena);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}

WPI_UPB_INLINE google_protobuf_ExtensionRangeOptions_Declaration** google_protobuf_ExtensionRangeOptions_mutable_declaration(google_protobuf_ExtensionRangeOptions* msg, size_t* size) {
  wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(12, 16), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__ExtensionRangeOptions__Declaration_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetMutableArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (google_protobuf_ExtensionRangeOptions_Declaration**)wpi_upb_Array_MutableDataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE google_protobuf_ExtensionRangeOptions_Declaration** google_protobuf_ExtensionRangeOptions_resize_declaration(google_protobuf_ExtensionRangeOptions* msg, size_t size, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(12, 16), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return (google_protobuf_ExtensionRangeOptions_Declaration**)wpi_upb_Message_ResizeArrayUninitialized(WPI_UPB_UPCAST(msg),
                                                   &field, size, arena);
}
WPI_UPB_INLINE struct google_protobuf_ExtensionRangeOptions_Declaration* google_protobuf_ExtensionRangeOptions_add_declaration(google_protobuf_ExtensionRangeOptions* msg, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(12, 16), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__ExtensionRangeOptions__Declaration_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(
      WPI_UPB_UPCAST(msg), &field, arena);
  if (!arr || !WPI_UPB_PRIVATE(_wpi_upb_Array_ResizeUninitialized)(
                  arr, arr->WPI_UPB_PRIVATE(size) + 1, arena)) {
    return NULL;
  }
  struct google_protobuf_ExtensionRangeOptions_Declaration* sub = (struct google_protobuf_ExtensionRangeOptions_Declaration*)_wpi_upb_Message_New(&google__protobuf__ExtensionRangeOptions__Declaration_msg_init, arena);
  if (!arr || !sub) return NULL;
  WPI_UPB_PRIVATE(_wpi_upb_Array_Set)
  (arr, arr->WPI_UPB_PRIVATE(size) - 1, &sub, sizeof(sub));
  return sub;
}
WPI_UPB_INLINE void google_protobuf_ExtensionRangeOptions_set_verification(google_protobuf_ExtensionRangeOptions *msg, int32_t value) {
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(16, 12), 64, 3, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_ExtensionRangeOptions_set_features(google_protobuf_ExtensionRangeOptions *msg, google_protobuf_FeatureSet* value) {
  const wpi_upb_MiniTableField field = {50, WPI_UPB_SIZE(20, 24), 65, 1, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FeatureSet_msg_init);
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE struct google_protobuf_FeatureSet* google_protobuf_ExtensionRangeOptions_mutable_features(google_protobuf_ExtensionRangeOptions* msg, wpi_upb_Arena* arena) {
  struct google_protobuf_FeatureSet* sub = (struct google_protobuf_FeatureSet*)google_protobuf_ExtensionRangeOptions_features(msg);
  if (sub == NULL) {
    sub = (struct google_protobuf_FeatureSet*)_wpi_upb_Message_New(&google__protobuf__FeatureSet_msg_init, arena);
    if (sub) google_protobuf_ExtensionRangeOptions_set_features(msg, sub);
  }
  return sub;
}
WPI_UPB_INLINE google_protobuf_UninterpretedOption** google_protobuf_ExtensionRangeOptions_mutable_uninterpreted_option(google_protobuf_ExtensionRangeOptions* msg, size_t* size) {
  wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(24, 32), 0, 2, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__UninterpretedOption_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetMutableArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (google_protobuf_UninterpretedOption**)wpi_upb_Array_MutableDataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE google_protobuf_UninterpretedOption** google_protobuf_ExtensionRangeOptions_resize_uninterpreted_option(google_protobuf_ExtensionRangeOptions* msg, size_t size, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(24, 32), 0, 2, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return (google_protobuf_UninterpretedOption**)wpi_upb_Message_ResizeArrayUninitialized(WPI_UPB_UPCAST(msg),
                                                   &field, size, arena);
}
WPI_UPB_INLINE struct google_protobuf_UninterpretedOption* google_protobuf_ExtensionRangeOptions_add_uninterpreted_option(google_protobuf_ExtensionRangeOptions* msg, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(24, 32), 0, 2, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__UninterpretedOption_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(
      WPI_UPB_UPCAST(msg), &field, arena);
  if (!arr || !WPI_UPB_PRIVATE(_wpi_upb_Array_ResizeUninitialized)(
                  arr, arr->WPI_UPB_PRIVATE(size) + 1, arena)) {
    return NULL;
  }
  struct google_protobuf_UninterpretedOption* sub = (struct google_protobuf_UninterpretedOption*)_wpi_upb_Message_New(&google__protobuf__UninterpretedOption_msg_init, arena);
  if (!arr || !sub) return NULL;
  WPI_UPB_PRIVATE(_wpi_upb_Array_Set)
  (arr, arr->WPI_UPB_PRIVATE(size) - 1, &sub, sizeof(sub));
  return sub;
}

/* google.protobuf.ExtensionRangeOptions.Declaration */

WPI_UPB_INLINE google_protobuf_ExtensionRangeOptions_Declaration* google_protobuf_ExtensionRangeOptions_Declaration_new(wpi_upb_Arena* arena) {
  return (google_protobuf_ExtensionRangeOptions_Declaration*)_wpi_upb_Message_New(&google__protobuf__ExtensionRangeOptions__Declaration_msg_init, arena);
}
WPI_UPB_INLINE google_protobuf_ExtensionRangeOptions_Declaration* google_protobuf_ExtensionRangeOptions_Declaration_parse(const char* buf, size_t size, wpi_upb_Arena* arena) {
  google_protobuf_ExtensionRangeOptions_Declaration* ret = google_protobuf_ExtensionRangeOptions_Declaration_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__ExtensionRangeOptions__Declaration_msg_init, NULL, 0, arena) !=
      kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE google_protobuf_ExtensionRangeOptions_Declaration* google_protobuf_ExtensionRangeOptions_Declaration_parse_ex(const char* buf, size_t size,
                           const wpi_upb_ExtensionRegistry* extreg,
                           int options, wpi_upb_Arena* arena) {
  google_protobuf_ExtensionRangeOptions_Declaration* ret = google_protobuf_ExtensionRangeOptions_Declaration_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__ExtensionRangeOptions__Declaration_msg_init, extreg, options,
                 arena) != kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE char* google_protobuf_ExtensionRangeOptions_Declaration_serialize(const google_protobuf_ExtensionRangeOptions_Declaration* msg, wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__ExtensionRangeOptions__Declaration_msg_init, 0, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE char* google_protobuf_ExtensionRangeOptions_Declaration_serialize_ex(const google_protobuf_ExtensionRangeOptions_Declaration* msg, int options,
                                 wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__ExtensionRangeOptions__Declaration_msg_init, options, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE void google_protobuf_ExtensionRangeOptions_Declaration_clear_number(google_protobuf_ExtensionRangeOptions_Declaration* msg) {
  const wpi_upb_MiniTableField field = {1, 12, 64, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE int32_t google_protobuf_ExtensionRangeOptions_Declaration_number(const google_protobuf_ExtensionRangeOptions_Declaration* msg) {
  int32_t default_val = (int32_t)0;
  int32_t ret;
  const wpi_upb_MiniTableField field = {1, 12, 64, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_ExtensionRangeOptions_Declaration_has_number(const google_protobuf_ExtensionRangeOptions_Declaration* msg) {
  const wpi_upb_MiniTableField field = {1, 12, 64, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_ExtensionRangeOptions_Declaration_clear_full_name(google_protobuf_ExtensionRangeOptions_Declaration* msg) {
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(20, 24), 65, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE wpi_upb_StringView google_protobuf_ExtensionRangeOptions_Declaration_full_name(const google_protobuf_ExtensionRangeOptions_Declaration* msg) {
  wpi_upb_StringView default_val = wpi_upb_StringView_FromString("");
  wpi_upb_StringView ret;
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(20, 24), 65, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_ExtensionRangeOptions_Declaration_has_full_name(const google_protobuf_ExtensionRangeOptions_Declaration* msg) {
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(20, 24), 65, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_ExtensionRangeOptions_Declaration_clear_type(google_protobuf_ExtensionRangeOptions_Declaration* msg) {
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(28, 40), 66, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE wpi_upb_StringView google_protobuf_ExtensionRangeOptions_Declaration_type(const google_protobuf_ExtensionRangeOptions_Declaration* msg) {
  wpi_upb_StringView default_val = wpi_upb_StringView_FromString("");
  wpi_upb_StringView ret;
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(28, 40), 66, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_ExtensionRangeOptions_Declaration_has_type(const google_protobuf_ExtensionRangeOptions_Declaration* msg) {
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(28, 40), 66, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_ExtensionRangeOptions_Declaration_clear_reserved(google_protobuf_ExtensionRangeOptions_Declaration* msg) {
  const wpi_upb_MiniTableField field = {5, 16, 67, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE bool google_protobuf_ExtensionRangeOptions_Declaration_reserved(const google_protobuf_ExtensionRangeOptions_Declaration* msg) {
  bool default_val = false;
  bool ret;
  const wpi_upb_MiniTableField field = {5, 16, 67, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_ExtensionRangeOptions_Declaration_has_reserved(const google_protobuf_ExtensionRangeOptions_Declaration* msg) {
  const wpi_upb_MiniTableField field = {5, 16, 67, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_ExtensionRangeOptions_Declaration_clear_repeated(google_protobuf_ExtensionRangeOptions_Declaration* msg) {
  const wpi_upb_MiniTableField field = {6, 17, 68, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE bool google_protobuf_ExtensionRangeOptions_Declaration_repeated(const google_protobuf_ExtensionRangeOptions_Declaration* msg) {
  bool default_val = false;
  bool ret;
  const wpi_upb_MiniTableField field = {6, 17, 68, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_ExtensionRangeOptions_Declaration_has_repeated(const google_protobuf_ExtensionRangeOptions_Declaration* msg) {
  const wpi_upb_MiniTableField field = {6, 17, 68, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}

WPI_UPB_INLINE void google_protobuf_ExtensionRangeOptions_Declaration_set_number(google_protobuf_ExtensionRangeOptions_Declaration *msg, int32_t value) {
  const wpi_upb_MiniTableField field = {1, 12, 64, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_ExtensionRangeOptions_Declaration_set_full_name(google_protobuf_ExtensionRangeOptions_Declaration *msg, wpi_upb_StringView value) {
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(20, 24), 65, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_ExtensionRangeOptions_Declaration_set_type(google_protobuf_ExtensionRangeOptions_Declaration *msg, wpi_upb_StringView value) {
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(28, 40), 66, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_ExtensionRangeOptions_Declaration_set_reserved(google_protobuf_ExtensionRangeOptions_Declaration *msg, bool value) {
  const wpi_upb_MiniTableField field = {5, 16, 67, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_ExtensionRangeOptions_Declaration_set_repeated(google_protobuf_ExtensionRangeOptions_Declaration *msg, bool value) {
  const wpi_upb_MiniTableField field = {6, 17, 68, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}

/* google.protobuf.FieldDescriptorProto */

WPI_UPB_INLINE google_protobuf_FieldDescriptorProto* google_protobuf_FieldDescriptorProto_new(wpi_upb_Arena* arena) {
  return (google_protobuf_FieldDescriptorProto*)_wpi_upb_Message_New(&google__protobuf__FieldDescriptorProto_msg_init, arena);
}
WPI_UPB_INLINE google_protobuf_FieldDescriptorProto* google_protobuf_FieldDescriptorProto_parse(const char* buf, size_t size, wpi_upb_Arena* arena) {
  google_protobuf_FieldDescriptorProto* ret = google_protobuf_FieldDescriptorProto_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__FieldDescriptorProto_msg_init, NULL, 0, arena) !=
      kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE google_protobuf_FieldDescriptorProto* google_protobuf_FieldDescriptorProto_parse_ex(const char* buf, size_t size,
                           const wpi_upb_ExtensionRegistry* extreg,
                           int options, wpi_upb_Arena* arena) {
  google_protobuf_FieldDescriptorProto* ret = google_protobuf_FieldDescriptorProto_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__FieldDescriptorProto_msg_init, extreg, options,
                 arena) != kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE char* google_protobuf_FieldDescriptorProto_serialize(const google_protobuf_FieldDescriptorProto* msg, wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__FieldDescriptorProto_msg_init, 0, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE char* google_protobuf_FieldDescriptorProto_serialize_ex(const google_protobuf_FieldDescriptorProto* msg, int options,
                                 wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__FieldDescriptorProto_msg_init, options, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE void google_protobuf_FieldDescriptorProto_clear_name(google_protobuf_FieldDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(36, 32), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE wpi_upb_StringView google_protobuf_FieldDescriptorProto_name(const google_protobuf_FieldDescriptorProto* msg) {
  wpi_upb_StringView default_val = wpi_upb_StringView_FromString("");
  wpi_upb_StringView ret;
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(36, 32), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FieldDescriptorProto_has_name(const google_protobuf_FieldDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(36, 32), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FieldDescriptorProto_clear_extendee(google_protobuf_FieldDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(44, 48), 65, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE wpi_upb_StringView google_protobuf_FieldDescriptorProto_extendee(const google_protobuf_FieldDescriptorProto* msg) {
  wpi_upb_StringView default_val = wpi_upb_StringView_FromString("");
  wpi_upb_StringView ret;
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(44, 48), 65, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FieldDescriptorProto_has_extendee(const google_protobuf_FieldDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(44, 48), 65, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FieldDescriptorProto_clear_number(google_protobuf_FieldDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {3, 12, 66, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE int32_t google_protobuf_FieldDescriptorProto_number(const google_protobuf_FieldDescriptorProto* msg) {
  int32_t default_val = (int32_t)0;
  int32_t ret;
  const wpi_upb_MiniTableField field = {3, 12, 66, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FieldDescriptorProto_has_number(const google_protobuf_FieldDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {3, 12, 66, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FieldDescriptorProto_clear_label(google_protobuf_FieldDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {4, 16, 67, 1, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE int32_t google_protobuf_FieldDescriptorProto_label(const google_protobuf_FieldDescriptorProto* msg) {
  int32_t default_val = 1;
  int32_t ret;
  const wpi_upb_MiniTableField field = {4, 16, 67, 1, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FieldDescriptorProto_has_label(const google_protobuf_FieldDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {4, 16, 67, 1, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FieldDescriptorProto_clear_type(google_protobuf_FieldDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {5, 20, 68, 2, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE int32_t google_protobuf_FieldDescriptorProto_type(const google_protobuf_FieldDescriptorProto* msg) {
  int32_t default_val = 1;
  int32_t ret;
  const wpi_upb_MiniTableField field = {5, 20, 68, 2, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FieldDescriptorProto_has_type(const google_protobuf_FieldDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {5, 20, 68, 2, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FieldDescriptorProto_clear_type_name(google_protobuf_FieldDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {6, WPI_UPB_SIZE(52, 64), 69, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE wpi_upb_StringView google_protobuf_FieldDescriptorProto_type_name(const google_protobuf_FieldDescriptorProto* msg) {
  wpi_upb_StringView default_val = wpi_upb_StringView_FromString("");
  wpi_upb_StringView ret;
  const wpi_upb_MiniTableField field = {6, WPI_UPB_SIZE(52, 64), 69, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FieldDescriptorProto_has_type_name(const google_protobuf_FieldDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {6, WPI_UPB_SIZE(52, 64), 69, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FieldDescriptorProto_clear_default_value(google_protobuf_FieldDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {7, WPI_UPB_SIZE(60, 80), 70, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE wpi_upb_StringView google_protobuf_FieldDescriptorProto_default_value(const google_protobuf_FieldDescriptorProto* msg) {
  wpi_upb_StringView default_val = wpi_upb_StringView_FromString("");
  wpi_upb_StringView ret;
  const wpi_upb_MiniTableField field = {7, WPI_UPB_SIZE(60, 80), 70, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FieldDescriptorProto_has_default_value(const google_protobuf_FieldDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {7, WPI_UPB_SIZE(60, 80), 70, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FieldDescriptorProto_clear_options(google_protobuf_FieldDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {8, WPI_UPB_SIZE(24, 96), 71, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_FieldOptions* google_protobuf_FieldDescriptorProto_options(const google_protobuf_FieldDescriptorProto* msg) {
  const google_protobuf_FieldOptions* default_val = NULL;
  const google_protobuf_FieldOptions* ret;
  const wpi_upb_MiniTableField field = {8, WPI_UPB_SIZE(24, 96), 71, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FieldOptions_msg_init);
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FieldDescriptorProto_has_options(const google_protobuf_FieldDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {8, WPI_UPB_SIZE(24, 96), 71, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FieldDescriptorProto_clear_oneof_index(google_protobuf_FieldDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {9, WPI_UPB_SIZE(28, 24), 72, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE int32_t google_protobuf_FieldDescriptorProto_oneof_index(const google_protobuf_FieldDescriptorProto* msg) {
  int32_t default_val = (int32_t)0;
  int32_t ret;
  const wpi_upb_MiniTableField field = {9, WPI_UPB_SIZE(28, 24), 72, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FieldDescriptorProto_has_oneof_index(const google_protobuf_FieldDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {9, WPI_UPB_SIZE(28, 24), 72, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FieldDescriptorProto_clear_json_name(google_protobuf_FieldDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {10, WPI_UPB_SIZE(68, 104), 73, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE wpi_upb_StringView google_protobuf_FieldDescriptorProto_json_name(const google_protobuf_FieldDescriptorProto* msg) {
  wpi_upb_StringView default_val = wpi_upb_StringView_FromString("");
  wpi_upb_StringView ret;
  const wpi_upb_MiniTableField field = {10, WPI_UPB_SIZE(68, 104), 73, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FieldDescriptorProto_has_json_name(const google_protobuf_FieldDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {10, WPI_UPB_SIZE(68, 104), 73, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FieldDescriptorProto_clear_proto3_optional(google_protobuf_FieldDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {17, WPI_UPB_SIZE(32, 28), 74, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE bool google_protobuf_FieldDescriptorProto_proto3_optional(const google_protobuf_FieldDescriptorProto* msg) {
  bool default_val = false;
  bool ret;
  const wpi_upb_MiniTableField field = {17, WPI_UPB_SIZE(32, 28), 74, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FieldDescriptorProto_has_proto3_optional(const google_protobuf_FieldDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {17, WPI_UPB_SIZE(32, 28), 74, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}

WPI_UPB_INLINE void google_protobuf_FieldDescriptorProto_set_name(google_protobuf_FieldDescriptorProto *msg, wpi_upb_StringView value) {
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(36, 32), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_FieldDescriptorProto_set_extendee(google_protobuf_FieldDescriptorProto *msg, wpi_upb_StringView value) {
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(44, 48), 65, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_FieldDescriptorProto_set_number(google_protobuf_FieldDescriptorProto *msg, int32_t value) {
  const wpi_upb_MiniTableField field = {3, 12, 66, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_FieldDescriptorProto_set_label(google_protobuf_FieldDescriptorProto *msg, int32_t value) {
  const wpi_upb_MiniTableField field = {4, 16, 67, 1, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_FieldDescriptorProto_set_type(google_protobuf_FieldDescriptorProto *msg, int32_t value) {
  const wpi_upb_MiniTableField field = {5, 20, 68, 2, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_FieldDescriptorProto_set_type_name(google_protobuf_FieldDescriptorProto *msg, wpi_upb_StringView value) {
  const wpi_upb_MiniTableField field = {6, WPI_UPB_SIZE(52, 64), 69, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_FieldDescriptorProto_set_default_value(google_protobuf_FieldDescriptorProto *msg, wpi_upb_StringView value) {
  const wpi_upb_MiniTableField field = {7, WPI_UPB_SIZE(60, 80), 70, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_FieldDescriptorProto_set_options(google_protobuf_FieldDescriptorProto *msg, google_protobuf_FieldOptions* value) {
  const wpi_upb_MiniTableField field = {8, WPI_UPB_SIZE(24, 96), 71, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FieldOptions_msg_init);
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE struct google_protobuf_FieldOptions* google_protobuf_FieldDescriptorProto_mutable_options(google_protobuf_FieldDescriptorProto* msg, wpi_upb_Arena* arena) {
  struct google_protobuf_FieldOptions* sub = (struct google_protobuf_FieldOptions*)google_protobuf_FieldDescriptorProto_options(msg);
  if (sub == NULL) {
    sub = (struct google_protobuf_FieldOptions*)_wpi_upb_Message_New(&google__protobuf__FieldOptions_msg_init, arena);
    if (sub) google_protobuf_FieldDescriptorProto_set_options(msg, sub);
  }
  return sub;
}
WPI_UPB_INLINE void google_protobuf_FieldDescriptorProto_set_oneof_index(google_protobuf_FieldDescriptorProto *msg, int32_t value) {
  const wpi_upb_MiniTableField field = {9, WPI_UPB_SIZE(28, 24), 72, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_FieldDescriptorProto_set_json_name(google_protobuf_FieldDescriptorProto *msg, wpi_upb_StringView value) {
  const wpi_upb_MiniTableField field = {10, WPI_UPB_SIZE(68, 104), 73, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_FieldDescriptorProto_set_proto3_optional(google_protobuf_FieldDescriptorProto *msg, bool value) {
  const wpi_upb_MiniTableField field = {17, WPI_UPB_SIZE(32, 28), 74, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}

/* google.protobuf.OneofDescriptorProto */

WPI_UPB_INLINE google_protobuf_OneofDescriptorProto* google_protobuf_OneofDescriptorProto_new(wpi_upb_Arena* arena) {
  return (google_protobuf_OneofDescriptorProto*)_wpi_upb_Message_New(&google__protobuf__OneofDescriptorProto_msg_init, arena);
}
WPI_UPB_INLINE google_protobuf_OneofDescriptorProto* google_protobuf_OneofDescriptorProto_parse(const char* buf, size_t size, wpi_upb_Arena* arena) {
  google_protobuf_OneofDescriptorProto* ret = google_protobuf_OneofDescriptorProto_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__OneofDescriptorProto_msg_init, NULL, 0, arena) !=
      kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE google_protobuf_OneofDescriptorProto* google_protobuf_OneofDescriptorProto_parse_ex(const char* buf, size_t size,
                           const wpi_upb_ExtensionRegistry* extreg,
                           int options, wpi_upb_Arena* arena) {
  google_protobuf_OneofDescriptorProto* ret = google_protobuf_OneofDescriptorProto_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__OneofDescriptorProto_msg_init, extreg, options,
                 arena) != kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE char* google_protobuf_OneofDescriptorProto_serialize(const google_protobuf_OneofDescriptorProto* msg, wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__OneofDescriptorProto_msg_init, 0, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE char* google_protobuf_OneofDescriptorProto_serialize_ex(const google_protobuf_OneofDescriptorProto* msg, int options,
                                 wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__OneofDescriptorProto_msg_init, options, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE void google_protobuf_OneofDescriptorProto_clear_name(google_protobuf_OneofDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {1, 16, 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE wpi_upb_StringView google_protobuf_OneofDescriptorProto_name(const google_protobuf_OneofDescriptorProto* msg) {
  wpi_upb_StringView default_val = wpi_upb_StringView_FromString("");
  wpi_upb_StringView ret;
  const wpi_upb_MiniTableField field = {1, 16, 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_OneofDescriptorProto_has_name(const google_protobuf_OneofDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {1, 16, 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_OneofDescriptorProto_clear_options(google_protobuf_OneofDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(12, 32), 65, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_OneofOptions* google_protobuf_OneofDescriptorProto_options(const google_protobuf_OneofDescriptorProto* msg) {
  const google_protobuf_OneofOptions* default_val = NULL;
  const google_protobuf_OneofOptions* ret;
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(12, 32), 65, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__OneofOptions_msg_init);
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_OneofDescriptorProto_has_options(const google_protobuf_OneofDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(12, 32), 65, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}

WPI_UPB_INLINE void google_protobuf_OneofDescriptorProto_set_name(google_protobuf_OneofDescriptorProto *msg, wpi_upb_StringView value) {
  const wpi_upb_MiniTableField field = {1, 16, 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_OneofDescriptorProto_set_options(google_protobuf_OneofDescriptorProto *msg, google_protobuf_OneofOptions* value) {
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(12, 32), 65, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__OneofOptions_msg_init);
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE struct google_protobuf_OneofOptions* google_protobuf_OneofDescriptorProto_mutable_options(google_protobuf_OneofDescriptorProto* msg, wpi_upb_Arena* arena) {
  struct google_protobuf_OneofOptions* sub = (struct google_protobuf_OneofOptions*)google_protobuf_OneofDescriptorProto_options(msg);
  if (sub == NULL) {
    sub = (struct google_protobuf_OneofOptions*)_wpi_upb_Message_New(&google__protobuf__OneofOptions_msg_init, arena);
    if (sub) google_protobuf_OneofDescriptorProto_set_options(msg, sub);
  }
  return sub;
}

/* google.protobuf.EnumDescriptorProto */

WPI_UPB_INLINE google_protobuf_EnumDescriptorProto* google_protobuf_EnumDescriptorProto_new(wpi_upb_Arena* arena) {
  return (google_protobuf_EnumDescriptorProto*)_wpi_upb_Message_New(&google__protobuf__EnumDescriptorProto_msg_init, arena);
}
WPI_UPB_INLINE google_protobuf_EnumDescriptorProto* google_protobuf_EnumDescriptorProto_parse(const char* buf, size_t size, wpi_upb_Arena* arena) {
  google_protobuf_EnumDescriptorProto* ret = google_protobuf_EnumDescriptorProto_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__EnumDescriptorProto_msg_init, NULL, 0, arena) !=
      kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE google_protobuf_EnumDescriptorProto* google_protobuf_EnumDescriptorProto_parse_ex(const char* buf, size_t size,
                           const wpi_upb_ExtensionRegistry* extreg,
                           int options, wpi_upb_Arena* arena) {
  google_protobuf_EnumDescriptorProto* ret = google_protobuf_EnumDescriptorProto_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__EnumDescriptorProto_msg_init, extreg, options,
                 arena) != kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE char* google_protobuf_EnumDescriptorProto_serialize(const google_protobuf_EnumDescriptorProto* msg, wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__EnumDescriptorProto_msg_init, 0, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE char* google_protobuf_EnumDescriptorProto_serialize_ex(const google_protobuf_EnumDescriptorProto* msg, int options,
                                 wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__EnumDescriptorProto_msg_init, options, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE void google_protobuf_EnumDescriptorProto_clear_name(google_protobuf_EnumDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(28, 16), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE wpi_upb_StringView google_protobuf_EnumDescriptorProto_name(const google_protobuf_EnumDescriptorProto* msg) {
  wpi_upb_StringView default_val = wpi_upb_StringView_FromString("");
  wpi_upb_StringView ret;
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(28, 16), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_EnumDescriptorProto_has_name(const google_protobuf_EnumDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(28, 16), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_EnumDescriptorProto_clear_value(google_protobuf_EnumDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(12, 32), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_EnumValueDescriptorProto* const* google_protobuf_EnumDescriptorProto_value(const google_protobuf_EnumDescriptorProto* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(12, 32), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__EnumValueDescriptorProto_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (const google_protobuf_EnumValueDescriptorProto* const*)wpi_upb_Array_DataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE const wpi_upb_Array* _google_protobuf_EnumDescriptorProto_value_wpi_upb_array(const google_protobuf_EnumDescriptorProto* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(12, 32), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__EnumValueDescriptorProto_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE wpi_upb_Array* _google_protobuf_EnumDescriptorProto_value_mutable_wpi_upb_array(google_protobuf_EnumDescriptorProto* msg, size_t* size, wpi_upb_Arena* arena) {
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(12, 32), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__EnumValueDescriptorProto_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(WPI_UPB_UPCAST(msg),
                                                       &field, arena);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE void google_protobuf_EnumDescriptorProto_clear_options(google_protobuf_EnumDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(16, 40), 65, 1, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_EnumOptions* google_protobuf_EnumDescriptorProto_options(const google_protobuf_EnumDescriptorProto* msg) {
  const google_protobuf_EnumOptions* default_val = NULL;
  const google_protobuf_EnumOptions* ret;
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(16, 40), 65, 1, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__EnumOptions_msg_init);
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_EnumDescriptorProto_has_options(const google_protobuf_EnumDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(16, 40), 65, 1, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_EnumDescriptorProto_clear_reserved_range(google_protobuf_EnumDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {4, WPI_UPB_SIZE(20, 48), 0, 2, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_EnumDescriptorProto_EnumReservedRange* const* google_protobuf_EnumDescriptorProto_reserved_range(const google_protobuf_EnumDescriptorProto* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {4, WPI_UPB_SIZE(20, 48), 0, 2, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__EnumDescriptorProto__EnumReservedRange_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (const google_protobuf_EnumDescriptorProto_EnumReservedRange* const*)wpi_upb_Array_DataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE const wpi_upb_Array* _google_protobuf_EnumDescriptorProto_reserved_range_wpi_upb_array(const google_protobuf_EnumDescriptorProto* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {4, WPI_UPB_SIZE(20, 48), 0, 2, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__EnumDescriptorProto__EnumReservedRange_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE wpi_upb_Array* _google_protobuf_EnumDescriptorProto_reserved_range_mutable_wpi_upb_array(google_protobuf_EnumDescriptorProto* msg, size_t* size, wpi_upb_Arena* arena) {
  const wpi_upb_MiniTableField field = {4, WPI_UPB_SIZE(20, 48), 0, 2, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__EnumDescriptorProto__EnumReservedRange_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(WPI_UPB_UPCAST(msg),
                                                       &field, arena);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE void google_protobuf_EnumDescriptorProto_clear_reserved_name(google_protobuf_EnumDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {5, WPI_UPB_SIZE(24, 56), 0, kUpb_NoSub, 12, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsAlternate | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE wpi_upb_StringView const* google_protobuf_EnumDescriptorProto_reserved_name(const google_protobuf_EnumDescriptorProto* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {5, WPI_UPB_SIZE(24, 56), 0, kUpb_NoSub, 12, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsAlternate | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (wpi_upb_StringView const*)wpi_upb_Array_DataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE const wpi_upb_Array* _google_protobuf_EnumDescriptorProto_reserved_name_wpi_upb_array(const google_protobuf_EnumDescriptorProto* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {5, WPI_UPB_SIZE(24, 56), 0, kUpb_NoSub, 12, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsAlternate | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE wpi_upb_Array* _google_protobuf_EnumDescriptorProto_reserved_name_mutable_wpi_upb_array(google_protobuf_EnumDescriptorProto* msg, size_t* size, wpi_upb_Arena* arena) {
  const wpi_upb_MiniTableField field = {5, WPI_UPB_SIZE(24, 56), 0, kUpb_NoSub, 12, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsAlternate | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(WPI_UPB_UPCAST(msg),
                                                       &field, arena);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}

WPI_UPB_INLINE void google_protobuf_EnumDescriptorProto_set_name(google_protobuf_EnumDescriptorProto *msg, wpi_upb_StringView value) {
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(28, 16), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE google_protobuf_EnumValueDescriptorProto** google_protobuf_EnumDescriptorProto_mutable_value(google_protobuf_EnumDescriptorProto* msg, size_t* size) {
  wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(12, 32), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__EnumValueDescriptorProto_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetMutableArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (google_protobuf_EnumValueDescriptorProto**)wpi_upb_Array_MutableDataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE google_protobuf_EnumValueDescriptorProto** google_protobuf_EnumDescriptorProto_resize_value(google_protobuf_EnumDescriptorProto* msg, size_t size, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(12, 32), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return (google_protobuf_EnumValueDescriptorProto**)wpi_upb_Message_ResizeArrayUninitialized(WPI_UPB_UPCAST(msg),
                                                   &field, size, arena);
}
WPI_UPB_INLINE struct google_protobuf_EnumValueDescriptorProto* google_protobuf_EnumDescriptorProto_add_value(google_protobuf_EnumDescriptorProto* msg, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(12, 32), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__EnumValueDescriptorProto_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(
      WPI_UPB_UPCAST(msg), &field, arena);
  if (!arr || !WPI_UPB_PRIVATE(_wpi_upb_Array_ResizeUninitialized)(
                  arr, arr->WPI_UPB_PRIVATE(size) + 1, arena)) {
    return NULL;
  }
  struct google_protobuf_EnumValueDescriptorProto* sub = (struct google_protobuf_EnumValueDescriptorProto*)_wpi_upb_Message_New(&google__protobuf__EnumValueDescriptorProto_msg_init, arena);
  if (!arr || !sub) return NULL;
  WPI_UPB_PRIVATE(_wpi_upb_Array_Set)
  (arr, arr->WPI_UPB_PRIVATE(size) - 1, &sub, sizeof(sub));
  return sub;
}
WPI_UPB_INLINE void google_protobuf_EnumDescriptorProto_set_options(google_protobuf_EnumDescriptorProto *msg, google_protobuf_EnumOptions* value) {
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(16, 40), 65, 1, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__EnumOptions_msg_init);
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE struct google_protobuf_EnumOptions* google_protobuf_EnumDescriptorProto_mutable_options(google_protobuf_EnumDescriptorProto* msg, wpi_upb_Arena* arena) {
  struct google_protobuf_EnumOptions* sub = (struct google_protobuf_EnumOptions*)google_protobuf_EnumDescriptorProto_options(msg);
  if (sub == NULL) {
    sub = (struct google_protobuf_EnumOptions*)_wpi_upb_Message_New(&google__protobuf__EnumOptions_msg_init, arena);
    if (sub) google_protobuf_EnumDescriptorProto_set_options(msg, sub);
  }
  return sub;
}
WPI_UPB_INLINE google_protobuf_EnumDescriptorProto_EnumReservedRange** google_protobuf_EnumDescriptorProto_mutable_reserved_range(google_protobuf_EnumDescriptorProto* msg, size_t* size) {
  wpi_upb_MiniTableField field = {4, WPI_UPB_SIZE(20, 48), 0, 2, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__EnumDescriptorProto__EnumReservedRange_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetMutableArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (google_protobuf_EnumDescriptorProto_EnumReservedRange**)wpi_upb_Array_MutableDataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE google_protobuf_EnumDescriptorProto_EnumReservedRange** google_protobuf_EnumDescriptorProto_resize_reserved_range(google_protobuf_EnumDescriptorProto* msg, size_t size, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {4, WPI_UPB_SIZE(20, 48), 0, 2, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return (google_protobuf_EnumDescriptorProto_EnumReservedRange**)wpi_upb_Message_ResizeArrayUninitialized(WPI_UPB_UPCAST(msg),
                                                   &field, size, arena);
}
WPI_UPB_INLINE struct google_protobuf_EnumDescriptorProto_EnumReservedRange* google_protobuf_EnumDescriptorProto_add_reserved_range(google_protobuf_EnumDescriptorProto* msg, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {4, WPI_UPB_SIZE(20, 48), 0, 2, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__EnumDescriptorProto__EnumReservedRange_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(
      WPI_UPB_UPCAST(msg), &field, arena);
  if (!arr || !WPI_UPB_PRIVATE(_wpi_upb_Array_ResizeUninitialized)(
                  arr, arr->WPI_UPB_PRIVATE(size) + 1, arena)) {
    return NULL;
  }
  struct google_protobuf_EnumDescriptorProto_EnumReservedRange* sub = (struct google_protobuf_EnumDescriptorProto_EnumReservedRange*)_wpi_upb_Message_New(&google__protobuf__EnumDescriptorProto__EnumReservedRange_msg_init, arena);
  if (!arr || !sub) return NULL;
  WPI_UPB_PRIVATE(_wpi_upb_Array_Set)
  (arr, arr->WPI_UPB_PRIVATE(size) - 1, &sub, sizeof(sub));
  return sub;
}
WPI_UPB_INLINE wpi_upb_StringView* google_protobuf_EnumDescriptorProto_mutable_reserved_name(google_protobuf_EnumDescriptorProto* msg, size_t* size) {
  wpi_upb_MiniTableField field = {5, WPI_UPB_SIZE(24, 56), 0, kUpb_NoSub, 12, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsAlternate | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Array* arr = wpi_upb_Message_GetMutableArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (wpi_upb_StringView*)wpi_upb_Array_MutableDataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE wpi_upb_StringView* google_protobuf_EnumDescriptorProto_resize_reserved_name(google_protobuf_EnumDescriptorProto* msg, size_t size, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {5, WPI_UPB_SIZE(24, 56), 0, kUpb_NoSub, 12, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsAlternate | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return (wpi_upb_StringView*)wpi_upb_Message_ResizeArrayUninitialized(WPI_UPB_UPCAST(msg),
                                                   &field, size, arena);
}
WPI_UPB_INLINE bool google_protobuf_EnumDescriptorProto_add_reserved_name(google_protobuf_EnumDescriptorProto* msg, wpi_upb_StringView val, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {5, WPI_UPB_SIZE(24, 56), 0, kUpb_NoSub, 12, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsAlternate | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(
      WPI_UPB_UPCAST(msg), &field, arena);
  if (!arr || !WPI_UPB_PRIVATE(_wpi_upb_Array_ResizeUninitialized)(
                  arr, arr->WPI_UPB_PRIVATE(size) + 1, arena)) {
    return false;
  }
  WPI_UPB_PRIVATE(_wpi_upb_Array_Set)
  (arr, arr->WPI_UPB_PRIVATE(size) - 1, &val, sizeof(val));
  return true;
}

/* google.protobuf.EnumDescriptorProto.EnumReservedRange */

WPI_UPB_INLINE google_protobuf_EnumDescriptorProto_EnumReservedRange* google_protobuf_EnumDescriptorProto_EnumReservedRange_new(wpi_upb_Arena* arena) {
  return (google_protobuf_EnumDescriptorProto_EnumReservedRange*)_wpi_upb_Message_New(&google__protobuf__EnumDescriptorProto__EnumReservedRange_msg_init, arena);
}
WPI_UPB_INLINE google_protobuf_EnumDescriptorProto_EnumReservedRange* google_protobuf_EnumDescriptorProto_EnumReservedRange_parse(const char* buf, size_t size, wpi_upb_Arena* arena) {
  google_protobuf_EnumDescriptorProto_EnumReservedRange* ret = google_protobuf_EnumDescriptorProto_EnumReservedRange_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__EnumDescriptorProto__EnumReservedRange_msg_init, NULL, 0, arena) !=
      kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE google_protobuf_EnumDescriptorProto_EnumReservedRange* google_protobuf_EnumDescriptorProto_EnumReservedRange_parse_ex(const char* buf, size_t size,
                           const wpi_upb_ExtensionRegistry* extreg,
                           int options, wpi_upb_Arena* arena) {
  google_protobuf_EnumDescriptorProto_EnumReservedRange* ret = google_protobuf_EnumDescriptorProto_EnumReservedRange_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__EnumDescriptorProto__EnumReservedRange_msg_init, extreg, options,
                 arena) != kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE char* google_protobuf_EnumDescriptorProto_EnumReservedRange_serialize(const google_protobuf_EnumDescriptorProto_EnumReservedRange* msg, wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__EnumDescriptorProto__EnumReservedRange_msg_init, 0, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE char* google_protobuf_EnumDescriptorProto_EnumReservedRange_serialize_ex(const google_protobuf_EnumDescriptorProto_EnumReservedRange* msg, int options,
                                 wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__EnumDescriptorProto__EnumReservedRange_msg_init, options, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE void google_protobuf_EnumDescriptorProto_EnumReservedRange_clear_start(google_protobuf_EnumDescriptorProto_EnumReservedRange* msg) {
  const wpi_upb_MiniTableField field = {1, 12, 64, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE int32_t google_protobuf_EnumDescriptorProto_EnumReservedRange_start(const google_protobuf_EnumDescriptorProto_EnumReservedRange* msg) {
  int32_t default_val = (int32_t)0;
  int32_t ret;
  const wpi_upb_MiniTableField field = {1, 12, 64, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_EnumDescriptorProto_EnumReservedRange_has_start(const google_protobuf_EnumDescriptorProto_EnumReservedRange* msg) {
  const wpi_upb_MiniTableField field = {1, 12, 64, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_EnumDescriptorProto_EnumReservedRange_clear_end(google_protobuf_EnumDescriptorProto_EnumReservedRange* msg) {
  const wpi_upb_MiniTableField field = {2, 16, 65, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE int32_t google_protobuf_EnumDescriptorProto_EnumReservedRange_end(const google_protobuf_EnumDescriptorProto_EnumReservedRange* msg) {
  int32_t default_val = (int32_t)0;
  int32_t ret;
  const wpi_upb_MiniTableField field = {2, 16, 65, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_EnumDescriptorProto_EnumReservedRange_has_end(const google_protobuf_EnumDescriptorProto_EnumReservedRange* msg) {
  const wpi_upb_MiniTableField field = {2, 16, 65, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}

WPI_UPB_INLINE void google_protobuf_EnumDescriptorProto_EnumReservedRange_set_start(google_protobuf_EnumDescriptorProto_EnumReservedRange *msg, int32_t value) {
  const wpi_upb_MiniTableField field = {1, 12, 64, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_EnumDescriptorProto_EnumReservedRange_set_end(google_protobuf_EnumDescriptorProto_EnumReservedRange *msg, int32_t value) {
  const wpi_upb_MiniTableField field = {2, 16, 65, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}

/* google.protobuf.EnumValueDescriptorProto */

WPI_UPB_INLINE google_protobuf_EnumValueDescriptorProto* google_protobuf_EnumValueDescriptorProto_new(wpi_upb_Arena* arena) {
  return (google_protobuf_EnumValueDescriptorProto*)_wpi_upb_Message_New(&google__protobuf__EnumValueDescriptorProto_msg_init, arena);
}
WPI_UPB_INLINE google_protobuf_EnumValueDescriptorProto* google_protobuf_EnumValueDescriptorProto_parse(const char* buf, size_t size, wpi_upb_Arena* arena) {
  google_protobuf_EnumValueDescriptorProto* ret = google_protobuf_EnumValueDescriptorProto_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__EnumValueDescriptorProto_msg_init, NULL, 0, arena) !=
      kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE google_protobuf_EnumValueDescriptorProto* google_protobuf_EnumValueDescriptorProto_parse_ex(const char* buf, size_t size,
                           const wpi_upb_ExtensionRegistry* extreg,
                           int options, wpi_upb_Arena* arena) {
  google_protobuf_EnumValueDescriptorProto* ret = google_protobuf_EnumValueDescriptorProto_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__EnumValueDescriptorProto_msg_init, extreg, options,
                 arena) != kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE char* google_protobuf_EnumValueDescriptorProto_serialize(const google_protobuf_EnumValueDescriptorProto* msg, wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__EnumValueDescriptorProto_msg_init, 0, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE char* google_protobuf_EnumValueDescriptorProto_serialize_ex(const google_protobuf_EnumValueDescriptorProto* msg, int options,
                                 wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__EnumValueDescriptorProto_msg_init, options, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE void google_protobuf_EnumValueDescriptorProto_clear_name(google_protobuf_EnumValueDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(20, 16), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE wpi_upb_StringView google_protobuf_EnumValueDescriptorProto_name(const google_protobuf_EnumValueDescriptorProto* msg) {
  wpi_upb_StringView default_val = wpi_upb_StringView_FromString("");
  wpi_upb_StringView ret;
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(20, 16), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_EnumValueDescriptorProto_has_name(const google_protobuf_EnumValueDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(20, 16), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_EnumValueDescriptorProto_clear_number(google_protobuf_EnumValueDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {2, 12, 65, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE int32_t google_protobuf_EnumValueDescriptorProto_number(const google_protobuf_EnumValueDescriptorProto* msg) {
  int32_t default_val = (int32_t)0;
  int32_t ret;
  const wpi_upb_MiniTableField field = {2, 12, 65, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_EnumValueDescriptorProto_has_number(const google_protobuf_EnumValueDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {2, 12, 65, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_EnumValueDescriptorProto_clear_options(google_protobuf_EnumValueDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(16, 32), 66, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_EnumValueOptions* google_protobuf_EnumValueDescriptorProto_options(const google_protobuf_EnumValueDescriptorProto* msg) {
  const google_protobuf_EnumValueOptions* default_val = NULL;
  const google_protobuf_EnumValueOptions* ret;
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(16, 32), 66, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__EnumValueOptions_msg_init);
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_EnumValueDescriptorProto_has_options(const google_protobuf_EnumValueDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(16, 32), 66, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}

WPI_UPB_INLINE void google_protobuf_EnumValueDescriptorProto_set_name(google_protobuf_EnumValueDescriptorProto *msg, wpi_upb_StringView value) {
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(20, 16), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_EnumValueDescriptorProto_set_number(google_protobuf_EnumValueDescriptorProto *msg, int32_t value) {
  const wpi_upb_MiniTableField field = {2, 12, 65, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_EnumValueDescriptorProto_set_options(google_protobuf_EnumValueDescriptorProto *msg, google_protobuf_EnumValueOptions* value) {
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(16, 32), 66, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__EnumValueOptions_msg_init);
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE struct google_protobuf_EnumValueOptions* google_protobuf_EnumValueDescriptorProto_mutable_options(google_protobuf_EnumValueDescriptorProto* msg, wpi_upb_Arena* arena) {
  struct google_protobuf_EnumValueOptions* sub = (struct google_protobuf_EnumValueOptions*)google_protobuf_EnumValueDescriptorProto_options(msg);
  if (sub == NULL) {
    sub = (struct google_protobuf_EnumValueOptions*)_wpi_upb_Message_New(&google__protobuf__EnumValueOptions_msg_init, arena);
    if (sub) google_protobuf_EnumValueDescriptorProto_set_options(msg, sub);
  }
  return sub;
}

/* google.protobuf.ServiceDescriptorProto */

WPI_UPB_INLINE google_protobuf_ServiceDescriptorProto* google_protobuf_ServiceDescriptorProto_new(wpi_upb_Arena* arena) {
  return (google_protobuf_ServiceDescriptorProto*)_wpi_upb_Message_New(&google__protobuf__ServiceDescriptorProto_msg_init, arena);
}
WPI_UPB_INLINE google_protobuf_ServiceDescriptorProto* google_protobuf_ServiceDescriptorProto_parse(const char* buf, size_t size, wpi_upb_Arena* arena) {
  google_protobuf_ServiceDescriptorProto* ret = google_protobuf_ServiceDescriptorProto_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__ServiceDescriptorProto_msg_init, NULL, 0, arena) !=
      kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE google_protobuf_ServiceDescriptorProto* google_protobuf_ServiceDescriptorProto_parse_ex(const char* buf, size_t size,
                           const wpi_upb_ExtensionRegistry* extreg,
                           int options, wpi_upb_Arena* arena) {
  google_protobuf_ServiceDescriptorProto* ret = google_protobuf_ServiceDescriptorProto_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__ServiceDescriptorProto_msg_init, extreg, options,
                 arena) != kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE char* google_protobuf_ServiceDescriptorProto_serialize(const google_protobuf_ServiceDescriptorProto* msg, wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__ServiceDescriptorProto_msg_init, 0, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE char* google_protobuf_ServiceDescriptorProto_serialize_ex(const google_protobuf_ServiceDescriptorProto* msg, int options,
                                 wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__ServiceDescriptorProto_msg_init, options, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE void google_protobuf_ServiceDescriptorProto_clear_name(google_protobuf_ServiceDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(20, 16), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE wpi_upb_StringView google_protobuf_ServiceDescriptorProto_name(const google_protobuf_ServiceDescriptorProto* msg) {
  wpi_upb_StringView default_val = wpi_upb_StringView_FromString("");
  wpi_upb_StringView ret;
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(20, 16), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_ServiceDescriptorProto_has_name(const google_protobuf_ServiceDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(20, 16), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_ServiceDescriptorProto_clear_method(google_protobuf_ServiceDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(12, 32), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_MethodDescriptorProto* const* google_protobuf_ServiceDescriptorProto_method(const google_protobuf_ServiceDescriptorProto* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(12, 32), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__MethodDescriptorProto_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (const google_protobuf_MethodDescriptorProto* const*)wpi_upb_Array_DataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE const wpi_upb_Array* _google_protobuf_ServiceDescriptorProto_method_wpi_upb_array(const google_protobuf_ServiceDescriptorProto* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(12, 32), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__MethodDescriptorProto_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE wpi_upb_Array* _google_protobuf_ServiceDescriptorProto_method_mutable_wpi_upb_array(google_protobuf_ServiceDescriptorProto* msg, size_t* size, wpi_upb_Arena* arena) {
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(12, 32), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__MethodDescriptorProto_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(WPI_UPB_UPCAST(msg),
                                                       &field, arena);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE void google_protobuf_ServiceDescriptorProto_clear_options(google_protobuf_ServiceDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(16, 40), 65, 1, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_ServiceOptions* google_protobuf_ServiceDescriptorProto_options(const google_protobuf_ServiceDescriptorProto* msg) {
  const google_protobuf_ServiceOptions* default_val = NULL;
  const google_protobuf_ServiceOptions* ret;
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(16, 40), 65, 1, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__ServiceOptions_msg_init);
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_ServiceDescriptorProto_has_options(const google_protobuf_ServiceDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(16, 40), 65, 1, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}

WPI_UPB_INLINE void google_protobuf_ServiceDescriptorProto_set_name(google_protobuf_ServiceDescriptorProto *msg, wpi_upb_StringView value) {
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(20, 16), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE google_protobuf_MethodDescriptorProto** google_protobuf_ServiceDescriptorProto_mutable_method(google_protobuf_ServiceDescriptorProto* msg, size_t* size) {
  wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(12, 32), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__MethodDescriptorProto_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetMutableArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (google_protobuf_MethodDescriptorProto**)wpi_upb_Array_MutableDataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE google_protobuf_MethodDescriptorProto** google_protobuf_ServiceDescriptorProto_resize_method(google_protobuf_ServiceDescriptorProto* msg, size_t size, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(12, 32), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return (google_protobuf_MethodDescriptorProto**)wpi_upb_Message_ResizeArrayUninitialized(WPI_UPB_UPCAST(msg),
                                                   &field, size, arena);
}
WPI_UPB_INLINE struct google_protobuf_MethodDescriptorProto* google_protobuf_ServiceDescriptorProto_add_method(google_protobuf_ServiceDescriptorProto* msg, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(12, 32), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__MethodDescriptorProto_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(
      WPI_UPB_UPCAST(msg), &field, arena);
  if (!arr || !WPI_UPB_PRIVATE(_wpi_upb_Array_ResizeUninitialized)(
                  arr, arr->WPI_UPB_PRIVATE(size) + 1, arena)) {
    return NULL;
  }
  struct google_protobuf_MethodDescriptorProto* sub = (struct google_protobuf_MethodDescriptorProto*)_wpi_upb_Message_New(&google__protobuf__MethodDescriptorProto_msg_init, arena);
  if (!arr || !sub) return NULL;
  WPI_UPB_PRIVATE(_wpi_upb_Array_Set)
  (arr, arr->WPI_UPB_PRIVATE(size) - 1, &sub, sizeof(sub));
  return sub;
}
WPI_UPB_INLINE void google_protobuf_ServiceDescriptorProto_set_options(google_protobuf_ServiceDescriptorProto *msg, google_protobuf_ServiceOptions* value) {
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(16, 40), 65, 1, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__ServiceOptions_msg_init);
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE struct google_protobuf_ServiceOptions* google_protobuf_ServiceDescriptorProto_mutable_options(google_protobuf_ServiceDescriptorProto* msg, wpi_upb_Arena* arena) {
  struct google_protobuf_ServiceOptions* sub = (struct google_protobuf_ServiceOptions*)google_protobuf_ServiceDescriptorProto_options(msg);
  if (sub == NULL) {
    sub = (struct google_protobuf_ServiceOptions*)_wpi_upb_Message_New(&google__protobuf__ServiceOptions_msg_init, arena);
    if (sub) google_protobuf_ServiceDescriptorProto_set_options(msg, sub);
  }
  return sub;
}

/* google.protobuf.MethodDescriptorProto */

WPI_UPB_INLINE google_protobuf_MethodDescriptorProto* google_protobuf_MethodDescriptorProto_new(wpi_upb_Arena* arena) {
  return (google_protobuf_MethodDescriptorProto*)_wpi_upb_Message_New(&google__protobuf__MethodDescriptorProto_msg_init, arena);
}
WPI_UPB_INLINE google_protobuf_MethodDescriptorProto* google_protobuf_MethodDescriptorProto_parse(const char* buf, size_t size, wpi_upb_Arena* arena) {
  google_protobuf_MethodDescriptorProto* ret = google_protobuf_MethodDescriptorProto_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__MethodDescriptorProto_msg_init, NULL, 0, arena) !=
      kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE google_protobuf_MethodDescriptorProto* google_protobuf_MethodDescriptorProto_parse_ex(const char* buf, size_t size,
                           const wpi_upb_ExtensionRegistry* extreg,
                           int options, wpi_upb_Arena* arena) {
  google_protobuf_MethodDescriptorProto* ret = google_protobuf_MethodDescriptorProto_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__MethodDescriptorProto_msg_init, extreg, options,
                 arena) != kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE char* google_protobuf_MethodDescriptorProto_serialize(const google_protobuf_MethodDescriptorProto* msg, wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__MethodDescriptorProto_msg_init, 0, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE char* google_protobuf_MethodDescriptorProto_serialize_ex(const google_protobuf_MethodDescriptorProto* msg, int options,
                                 wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__MethodDescriptorProto_msg_init, options, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE void google_protobuf_MethodDescriptorProto_clear_name(google_protobuf_MethodDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(20, 16), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE wpi_upb_StringView google_protobuf_MethodDescriptorProto_name(const google_protobuf_MethodDescriptorProto* msg) {
  wpi_upb_StringView default_val = wpi_upb_StringView_FromString("");
  wpi_upb_StringView ret;
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(20, 16), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_MethodDescriptorProto_has_name(const google_protobuf_MethodDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(20, 16), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_MethodDescriptorProto_clear_input_type(google_protobuf_MethodDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(28, 32), 65, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE wpi_upb_StringView google_protobuf_MethodDescriptorProto_input_type(const google_protobuf_MethodDescriptorProto* msg) {
  wpi_upb_StringView default_val = wpi_upb_StringView_FromString("");
  wpi_upb_StringView ret;
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(28, 32), 65, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_MethodDescriptorProto_has_input_type(const google_protobuf_MethodDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(28, 32), 65, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_MethodDescriptorProto_clear_output_type(google_protobuf_MethodDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(36, 48), 66, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE wpi_upb_StringView google_protobuf_MethodDescriptorProto_output_type(const google_protobuf_MethodDescriptorProto* msg) {
  wpi_upb_StringView default_val = wpi_upb_StringView_FromString("");
  wpi_upb_StringView ret;
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(36, 48), 66, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_MethodDescriptorProto_has_output_type(const google_protobuf_MethodDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(36, 48), 66, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_MethodDescriptorProto_clear_options(google_protobuf_MethodDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {4, WPI_UPB_SIZE(12, 64), 67, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_MethodOptions* google_protobuf_MethodDescriptorProto_options(const google_protobuf_MethodDescriptorProto* msg) {
  const google_protobuf_MethodOptions* default_val = NULL;
  const google_protobuf_MethodOptions* ret;
  const wpi_upb_MiniTableField field = {4, WPI_UPB_SIZE(12, 64), 67, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__MethodOptions_msg_init);
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_MethodDescriptorProto_has_options(const google_protobuf_MethodDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {4, WPI_UPB_SIZE(12, 64), 67, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_MethodDescriptorProto_clear_client_streaming(google_protobuf_MethodDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {5, WPI_UPB_SIZE(16, 9), 68, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE bool google_protobuf_MethodDescriptorProto_client_streaming(const google_protobuf_MethodDescriptorProto* msg) {
  bool default_val = false;
  bool ret;
  const wpi_upb_MiniTableField field = {5, WPI_UPB_SIZE(16, 9), 68, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_MethodDescriptorProto_has_client_streaming(const google_protobuf_MethodDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {5, WPI_UPB_SIZE(16, 9), 68, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_MethodDescriptorProto_clear_server_streaming(google_protobuf_MethodDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {6, WPI_UPB_SIZE(17, 10), 69, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE bool google_protobuf_MethodDescriptorProto_server_streaming(const google_protobuf_MethodDescriptorProto* msg) {
  bool default_val = false;
  bool ret;
  const wpi_upb_MiniTableField field = {6, WPI_UPB_SIZE(17, 10), 69, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_MethodDescriptorProto_has_server_streaming(const google_protobuf_MethodDescriptorProto* msg) {
  const wpi_upb_MiniTableField field = {6, WPI_UPB_SIZE(17, 10), 69, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}

WPI_UPB_INLINE void google_protobuf_MethodDescriptorProto_set_name(google_protobuf_MethodDescriptorProto *msg, wpi_upb_StringView value) {
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(20, 16), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_MethodDescriptorProto_set_input_type(google_protobuf_MethodDescriptorProto *msg, wpi_upb_StringView value) {
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(28, 32), 65, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_MethodDescriptorProto_set_output_type(google_protobuf_MethodDescriptorProto *msg, wpi_upb_StringView value) {
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(36, 48), 66, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_MethodDescriptorProto_set_options(google_protobuf_MethodDescriptorProto *msg, google_protobuf_MethodOptions* value) {
  const wpi_upb_MiniTableField field = {4, WPI_UPB_SIZE(12, 64), 67, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__MethodOptions_msg_init);
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE struct google_protobuf_MethodOptions* google_protobuf_MethodDescriptorProto_mutable_options(google_protobuf_MethodDescriptorProto* msg, wpi_upb_Arena* arena) {
  struct google_protobuf_MethodOptions* sub = (struct google_protobuf_MethodOptions*)google_protobuf_MethodDescriptorProto_options(msg);
  if (sub == NULL) {
    sub = (struct google_protobuf_MethodOptions*)_wpi_upb_Message_New(&google__protobuf__MethodOptions_msg_init, arena);
    if (sub) google_protobuf_MethodDescriptorProto_set_options(msg, sub);
  }
  return sub;
}
WPI_UPB_INLINE void google_protobuf_MethodDescriptorProto_set_client_streaming(google_protobuf_MethodDescriptorProto *msg, bool value) {
  const wpi_upb_MiniTableField field = {5, WPI_UPB_SIZE(16, 9), 68, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_MethodDescriptorProto_set_server_streaming(google_protobuf_MethodDescriptorProto *msg, bool value) {
  const wpi_upb_MiniTableField field = {6, WPI_UPB_SIZE(17, 10), 69, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}

/* google.protobuf.FileOptions */

WPI_UPB_INLINE google_protobuf_FileOptions* google_protobuf_FileOptions_new(wpi_upb_Arena* arena) {
  return (google_protobuf_FileOptions*)_wpi_upb_Message_New(&google__protobuf__FileOptions_msg_init, arena);
}
WPI_UPB_INLINE google_protobuf_FileOptions* google_protobuf_FileOptions_parse(const char* buf, size_t size, wpi_upb_Arena* arena) {
  google_protobuf_FileOptions* ret = google_protobuf_FileOptions_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__FileOptions_msg_init, NULL, 0, arena) !=
      kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE google_protobuf_FileOptions* google_protobuf_FileOptions_parse_ex(const char* buf, size_t size,
                           const wpi_upb_ExtensionRegistry* extreg,
                           int options, wpi_upb_Arena* arena) {
  google_protobuf_FileOptions* ret = google_protobuf_FileOptions_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__FileOptions_msg_init, extreg, options,
                 arena) != kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE char* google_protobuf_FileOptions_serialize(const google_protobuf_FileOptions* msg, wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__FileOptions_msg_init, 0, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE char* google_protobuf_FileOptions_serialize_ex(const google_protobuf_FileOptions* msg, int options,
                                 wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__FileOptions_msg_init, options, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE void google_protobuf_FileOptions_clear_java_package(google_protobuf_FileOptions* msg) {
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(32, 24), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE wpi_upb_StringView google_protobuf_FileOptions_java_package(const google_protobuf_FileOptions* msg) {
  wpi_upb_StringView default_val = wpi_upb_StringView_FromString("");
  wpi_upb_StringView ret;
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(32, 24), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FileOptions_has_java_package(const google_protobuf_FileOptions* msg) {
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(32, 24), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FileOptions_clear_java_outer_classname(google_protobuf_FileOptions* msg) {
  const wpi_upb_MiniTableField field = {8, 40, 65, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE wpi_upb_StringView google_protobuf_FileOptions_java_outer_classname(const google_protobuf_FileOptions* msg) {
  wpi_upb_StringView default_val = wpi_upb_StringView_FromString("");
  wpi_upb_StringView ret;
  const wpi_upb_MiniTableField field = {8, 40, 65, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FileOptions_has_java_outer_classname(const google_protobuf_FileOptions* msg) {
  const wpi_upb_MiniTableField field = {8, 40, 65, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FileOptions_clear_optimize_for(google_protobuf_FileOptions* msg) {
  const wpi_upb_MiniTableField field = {9, 12, 66, 2, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE int32_t google_protobuf_FileOptions_optimize_for(const google_protobuf_FileOptions* msg) {
  int32_t default_val = 1;
  int32_t ret;
  const wpi_upb_MiniTableField field = {9, 12, 66, 2, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FileOptions_has_optimize_for(const google_protobuf_FileOptions* msg) {
  const wpi_upb_MiniTableField field = {9, 12, 66, 2, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FileOptions_clear_java_multiple_files(google_protobuf_FileOptions* msg) {
  const wpi_upb_MiniTableField field = {10, 16, 67, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE bool google_protobuf_FileOptions_java_multiple_files(const google_protobuf_FileOptions* msg) {
  bool default_val = false;
  bool ret;
  const wpi_upb_MiniTableField field = {10, 16, 67, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FileOptions_has_java_multiple_files(const google_protobuf_FileOptions* msg) {
  const wpi_upb_MiniTableField field = {10, 16, 67, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FileOptions_clear_go_package(google_protobuf_FileOptions* msg) {
  const wpi_upb_MiniTableField field = {11, WPI_UPB_SIZE(48, 56), 68, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE wpi_upb_StringView google_protobuf_FileOptions_go_package(const google_protobuf_FileOptions* msg) {
  wpi_upb_StringView default_val = wpi_upb_StringView_FromString("");
  wpi_upb_StringView ret;
  const wpi_upb_MiniTableField field = {11, WPI_UPB_SIZE(48, 56), 68, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FileOptions_has_go_package(const google_protobuf_FileOptions* msg) {
  const wpi_upb_MiniTableField field = {11, WPI_UPB_SIZE(48, 56), 68, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FileOptions_clear_cc_generic_services(google_protobuf_FileOptions* msg) {
  const wpi_upb_MiniTableField field = {16, 17, 69, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE bool google_protobuf_FileOptions_cc_generic_services(const google_protobuf_FileOptions* msg) {
  bool default_val = false;
  bool ret;
  const wpi_upb_MiniTableField field = {16, 17, 69, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FileOptions_has_cc_generic_services(const google_protobuf_FileOptions* msg) {
  const wpi_upb_MiniTableField field = {16, 17, 69, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FileOptions_clear_java_generic_services(google_protobuf_FileOptions* msg) {
  const wpi_upb_MiniTableField field = {17, 18, 70, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE bool google_protobuf_FileOptions_java_generic_services(const google_protobuf_FileOptions* msg) {
  bool default_val = false;
  bool ret;
  const wpi_upb_MiniTableField field = {17, 18, 70, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FileOptions_has_java_generic_services(const google_protobuf_FileOptions* msg) {
  const wpi_upb_MiniTableField field = {17, 18, 70, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FileOptions_clear_py_generic_services(google_protobuf_FileOptions* msg) {
  const wpi_upb_MiniTableField field = {18, 19, 71, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE bool google_protobuf_FileOptions_py_generic_services(const google_protobuf_FileOptions* msg) {
  bool default_val = false;
  bool ret;
  const wpi_upb_MiniTableField field = {18, 19, 71, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FileOptions_has_py_generic_services(const google_protobuf_FileOptions* msg) {
  const wpi_upb_MiniTableField field = {18, 19, 71, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FileOptions_clear_java_generate_equals_and_hash(google_protobuf_FileOptions* msg) {
  const wpi_upb_MiniTableField field = {20, 20, 72, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE bool google_protobuf_FileOptions_java_generate_equals_and_hash(const google_protobuf_FileOptions* msg) {
  bool default_val = false;
  bool ret;
  const wpi_upb_MiniTableField field = {20, 20, 72, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FileOptions_has_java_generate_equals_and_hash(const google_protobuf_FileOptions* msg) {
  const wpi_upb_MiniTableField field = {20, 20, 72, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FileOptions_clear_deprecated(google_protobuf_FileOptions* msg) {
  const wpi_upb_MiniTableField field = {23, 21, 73, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE bool google_protobuf_FileOptions_deprecated(const google_protobuf_FileOptions* msg) {
  bool default_val = false;
  bool ret;
  const wpi_upb_MiniTableField field = {23, 21, 73, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FileOptions_has_deprecated(const google_protobuf_FileOptions* msg) {
  const wpi_upb_MiniTableField field = {23, 21, 73, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FileOptions_clear_java_string_check_utf8(google_protobuf_FileOptions* msg) {
  const wpi_upb_MiniTableField field = {27, 22, 74, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE bool google_protobuf_FileOptions_java_string_check_utf8(const google_protobuf_FileOptions* msg) {
  bool default_val = false;
  bool ret;
  const wpi_upb_MiniTableField field = {27, 22, 74, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FileOptions_has_java_string_check_utf8(const google_protobuf_FileOptions* msg) {
  const wpi_upb_MiniTableField field = {27, 22, 74, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FileOptions_clear_cc_enable_arenas(google_protobuf_FileOptions* msg) {
  const wpi_upb_MiniTableField field = {31, 23, 75, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE bool google_protobuf_FileOptions_cc_enable_arenas(const google_protobuf_FileOptions* msg) {
  bool default_val = true;
  bool ret;
  const wpi_upb_MiniTableField field = {31, 23, 75, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FileOptions_has_cc_enable_arenas(const google_protobuf_FileOptions* msg) {
  const wpi_upb_MiniTableField field = {31, 23, 75, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FileOptions_clear_objc_class_prefix(google_protobuf_FileOptions* msg) {
  const wpi_upb_MiniTableField field = {36, WPI_UPB_SIZE(56, 72), 76, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE wpi_upb_StringView google_protobuf_FileOptions_objc_class_prefix(const google_protobuf_FileOptions* msg) {
  wpi_upb_StringView default_val = wpi_upb_StringView_FromString("");
  wpi_upb_StringView ret;
  const wpi_upb_MiniTableField field = {36, WPI_UPB_SIZE(56, 72), 76, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FileOptions_has_objc_class_prefix(const google_protobuf_FileOptions* msg) {
  const wpi_upb_MiniTableField field = {36, WPI_UPB_SIZE(56, 72), 76, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FileOptions_clear_csharp_namespace(google_protobuf_FileOptions* msg) {
  const wpi_upb_MiniTableField field = {37, WPI_UPB_SIZE(64, 88), 77, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE wpi_upb_StringView google_protobuf_FileOptions_csharp_namespace(const google_protobuf_FileOptions* msg) {
  wpi_upb_StringView default_val = wpi_upb_StringView_FromString("");
  wpi_upb_StringView ret;
  const wpi_upb_MiniTableField field = {37, WPI_UPB_SIZE(64, 88), 77, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FileOptions_has_csharp_namespace(const google_protobuf_FileOptions* msg) {
  const wpi_upb_MiniTableField field = {37, WPI_UPB_SIZE(64, 88), 77, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FileOptions_clear_swift_prefix(google_protobuf_FileOptions* msg) {
  const wpi_upb_MiniTableField field = {39, WPI_UPB_SIZE(72, 104), 78, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE wpi_upb_StringView google_protobuf_FileOptions_swift_prefix(const google_protobuf_FileOptions* msg) {
  wpi_upb_StringView default_val = wpi_upb_StringView_FromString("");
  wpi_upb_StringView ret;
  const wpi_upb_MiniTableField field = {39, WPI_UPB_SIZE(72, 104), 78, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FileOptions_has_swift_prefix(const google_protobuf_FileOptions* msg) {
  const wpi_upb_MiniTableField field = {39, WPI_UPB_SIZE(72, 104), 78, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FileOptions_clear_php_class_prefix(google_protobuf_FileOptions* msg) {
  const wpi_upb_MiniTableField field = {40, WPI_UPB_SIZE(80, 120), 79, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE wpi_upb_StringView google_protobuf_FileOptions_php_class_prefix(const google_protobuf_FileOptions* msg) {
  wpi_upb_StringView default_val = wpi_upb_StringView_FromString("");
  wpi_upb_StringView ret;
  const wpi_upb_MiniTableField field = {40, WPI_UPB_SIZE(80, 120), 79, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FileOptions_has_php_class_prefix(const google_protobuf_FileOptions* msg) {
  const wpi_upb_MiniTableField field = {40, WPI_UPB_SIZE(80, 120), 79, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FileOptions_clear_php_namespace(google_protobuf_FileOptions* msg) {
  const wpi_upb_MiniTableField field = {41, WPI_UPB_SIZE(88, 136), 80, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE wpi_upb_StringView google_protobuf_FileOptions_php_namespace(const google_protobuf_FileOptions* msg) {
  wpi_upb_StringView default_val = wpi_upb_StringView_FromString("");
  wpi_upb_StringView ret;
  const wpi_upb_MiniTableField field = {41, WPI_UPB_SIZE(88, 136), 80, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FileOptions_has_php_namespace(const google_protobuf_FileOptions* msg) {
  const wpi_upb_MiniTableField field = {41, WPI_UPB_SIZE(88, 136), 80, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FileOptions_clear_php_metadata_namespace(google_protobuf_FileOptions* msg) {
  const wpi_upb_MiniTableField field = {44, WPI_UPB_SIZE(96, 152), 81, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE wpi_upb_StringView google_protobuf_FileOptions_php_metadata_namespace(const google_protobuf_FileOptions* msg) {
  wpi_upb_StringView default_val = wpi_upb_StringView_FromString("");
  wpi_upb_StringView ret;
  const wpi_upb_MiniTableField field = {44, WPI_UPB_SIZE(96, 152), 81, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FileOptions_has_php_metadata_namespace(const google_protobuf_FileOptions* msg) {
  const wpi_upb_MiniTableField field = {44, WPI_UPB_SIZE(96, 152), 81, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FileOptions_clear_ruby_package(google_protobuf_FileOptions* msg) {
  const wpi_upb_MiniTableField field = {45, WPI_UPB_SIZE(104, 168), 82, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE wpi_upb_StringView google_protobuf_FileOptions_ruby_package(const google_protobuf_FileOptions* msg) {
  wpi_upb_StringView default_val = wpi_upb_StringView_FromString("");
  wpi_upb_StringView ret;
  const wpi_upb_MiniTableField field = {45, WPI_UPB_SIZE(104, 168), 82, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FileOptions_has_ruby_package(const google_protobuf_FileOptions* msg) {
  const wpi_upb_MiniTableField field = {45, WPI_UPB_SIZE(104, 168), 82, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FileOptions_clear_features(google_protobuf_FileOptions* msg) {
  const wpi_upb_MiniTableField field = {50, WPI_UPB_SIZE(24, 184), 83, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_FeatureSet* google_protobuf_FileOptions_features(const google_protobuf_FileOptions* msg) {
  const google_protobuf_FeatureSet* default_val = NULL;
  const google_protobuf_FeatureSet* ret;
  const wpi_upb_MiniTableField field = {50, WPI_UPB_SIZE(24, 184), 83, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FeatureSet_msg_init);
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FileOptions_has_features(const google_protobuf_FileOptions* msg) {
  const wpi_upb_MiniTableField field = {50, WPI_UPB_SIZE(24, 184), 83, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FileOptions_clear_uninterpreted_option(google_protobuf_FileOptions* msg) {
  const wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(28, 192), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_UninterpretedOption* const* google_protobuf_FileOptions_uninterpreted_option(const google_protobuf_FileOptions* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(28, 192), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__UninterpretedOption_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (const google_protobuf_UninterpretedOption* const*)wpi_upb_Array_DataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE const wpi_upb_Array* _google_protobuf_FileOptions_uninterpreted_option_wpi_upb_array(const google_protobuf_FileOptions* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(28, 192), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__UninterpretedOption_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE wpi_upb_Array* _google_protobuf_FileOptions_uninterpreted_option_mutable_wpi_upb_array(google_protobuf_FileOptions* msg, size_t* size, wpi_upb_Arena* arena) {
  const wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(28, 192), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__UninterpretedOption_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(WPI_UPB_UPCAST(msg),
                                                       &field, arena);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}

WPI_UPB_INLINE void google_protobuf_FileOptions_set_java_package(google_protobuf_FileOptions *msg, wpi_upb_StringView value) {
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(32, 24), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_FileOptions_set_java_outer_classname(google_protobuf_FileOptions *msg, wpi_upb_StringView value) {
  const wpi_upb_MiniTableField field = {8, 40, 65, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_FileOptions_set_optimize_for(google_protobuf_FileOptions *msg, int32_t value) {
  const wpi_upb_MiniTableField field = {9, 12, 66, 2, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_FileOptions_set_java_multiple_files(google_protobuf_FileOptions *msg, bool value) {
  const wpi_upb_MiniTableField field = {10, 16, 67, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_FileOptions_set_go_package(google_protobuf_FileOptions *msg, wpi_upb_StringView value) {
  const wpi_upb_MiniTableField field = {11, WPI_UPB_SIZE(48, 56), 68, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_FileOptions_set_cc_generic_services(google_protobuf_FileOptions *msg, bool value) {
  const wpi_upb_MiniTableField field = {16, 17, 69, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_FileOptions_set_java_generic_services(google_protobuf_FileOptions *msg, bool value) {
  const wpi_upb_MiniTableField field = {17, 18, 70, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_FileOptions_set_py_generic_services(google_protobuf_FileOptions *msg, bool value) {
  const wpi_upb_MiniTableField field = {18, 19, 71, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_FileOptions_set_java_generate_equals_and_hash(google_protobuf_FileOptions *msg, bool value) {
  const wpi_upb_MiniTableField field = {20, 20, 72, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_FileOptions_set_deprecated(google_protobuf_FileOptions *msg, bool value) {
  const wpi_upb_MiniTableField field = {23, 21, 73, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_FileOptions_set_java_string_check_utf8(google_protobuf_FileOptions *msg, bool value) {
  const wpi_upb_MiniTableField field = {27, 22, 74, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_FileOptions_set_cc_enable_arenas(google_protobuf_FileOptions *msg, bool value) {
  const wpi_upb_MiniTableField field = {31, 23, 75, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_FileOptions_set_objc_class_prefix(google_protobuf_FileOptions *msg, wpi_upb_StringView value) {
  const wpi_upb_MiniTableField field = {36, WPI_UPB_SIZE(56, 72), 76, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_FileOptions_set_csharp_namespace(google_protobuf_FileOptions *msg, wpi_upb_StringView value) {
  const wpi_upb_MiniTableField field = {37, WPI_UPB_SIZE(64, 88), 77, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_FileOptions_set_swift_prefix(google_protobuf_FileOptions *msg, wpi_upb_StringView value) {
  const wpi_upb_MiniTableField field = {39, WPI_UPB_SIZE(72, 104), 78, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_FileOptions_set_php_class_prefix(google_protobuf_FileOptions *msg, wpi_upb_StringView value) {
  const wpi_upb_MiniTableField field = {40, WPI_UPB_SIZE(80, 120), 79, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_FileOptions_set_php_namespace(google_protobuf_FileOptions *msg, wpi_upb_StringView value) {
  const wpi_upb_MiniTableField field = {41, WPI_UPB_SIZE(88, 136), 80, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_FileOptions_set_php_metadata_namespace(google_protobuf_FileOptions *msg, wpi_upb_StringView value) {
  const wpi_upb_MiniTableField field = {44, WPI_UPB_SIZE(96, 152), 81, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_FileOptions_set_ruby_package(google_protobuf_FileOptions *msg, wpi_upb_StringView value) {
  const wpi_upb_MiniTableField field = {45, WPI_UPB_SIZE(104, 168), 82, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_FileOptions_set_features(google_protobuf_FileOptions *msg, google_protobuf_FeatureSet* value) {
  const wpi_upb_MiniTableField field = {50, WPI_UPB_SIZE(24, 184), 83, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FeatureSet_msg_init);
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE struct google_protobuf_FeatureSet* google_protobuf_FileOptions_mutable_features(google_protobuf_FileOptions* msg, wpi_upb_Arena* arena) {
  struct google_protobuf_FeatureSet* sub = (struct google_protobuf_FeatureSet*)google_protobuf_FileOptions_features(msg);
  if (sub == NULL) {
    sub = (struct google_protobuf_FeatureSet*)_wpi_upb_Message_New(&google__protobuf__FeatureSet_msg_init, arena);
    if (sub) google_protobuf_FileOptions_set_features(msg, sub);
  }
  return sub;
}
WPI_UPB_INLINE google_protobuf_UninterpretedOption** google_protobuf_FileOptions_mutable_uninterpreted_option(google_protobuf_FileOptions* msg, size_t* size) {
  wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(28, 192), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__UninterpretedOption_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetMutableArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (google_protobuf_UninterpretedOption**)wpi_upb_Array_MutableDataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE google_protobuf_UninterpretedOption** google_protobuf_FileOptions_resize_uninterpreted_option(google_protobuf_FileOptions* msg, size_t size, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(28, 192), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return (google_protobuf_UninterpretedOption**)wpi_upb_Message_ResizeArrayUninitialized(WPI_UPB_UPCAST(msg),
                                                   &field, size, arena);
}
WPI_UPB_INLINE struct google_protobuf_UninterpretedOption* google_protobuf_FileOptions_add_uninterpreted_option(google_protobuf_FileOptions* msg, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(28, 192), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__UninterpretedOption_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(
      WPI_UPB_UPCAST(msg), &field, arena);
  if (!arr || !WPI_UPB_PRIVATE(_wpi_upb_Array_ResizeUninitialized)(
                  arr, arr->WPI_UPB_PRIVATE(size) + 1, arena)) {
    return NULL;
  }
  struct google_protobuf_UninterpretedOption* sub = (struct google_protobuf_UninterpretedOption*)_wpi_upb_Message_New(&google__protobuf__UninterpretedOption_msg_init, arena);
  if (!arr || !sub) return NULL;
  WPI_UPB_PRIVATE(_wpi_upb_Array_Set)
  (arr, arr->WPI_UPB_PRIVATE(size) - 1, &sub, sizeof(sub));
  return sub;
}

/* google.protobuf.MessageOptions */

WPI_UPB_INLINE google_protobuf_MessageOptions* google_protobuf_MessageOptions_new(wpi_upb_Arena* arena) {
  return (google_protobuf_MessageOptions*)_wpi_upb_Message_New(&google__protobuf__MessageOptions_msg_init, arena);
}
WPI_UPB_INLINE google_protobuf_MessageOptions* google_protobuf_MessageOptions_parse(const char* buf, size_t size, wpi_upb_Arena* arena) {
  google_protobuf_MessageOptions* ret = google_protobuf_MessageOptions_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__MessageOptions_msg_init, NULL, 0, arena) !=
      kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE google_protobuf_MessageOptions* google_protobuf_MessageOptions_parse_ex(const char* buf, size_t size,
                           const wpi_upb_ExtensionRegistry* extreg,
                           int options, wpi_upb_Arena* arena) {
  google_protobuf_MessageOptions* ret = google_protobuf_MessageOptions_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__MessageOptions_msg_init, extreg, options,
                 arena) != kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE char* google_protobuf_MessageOptions_serialize(const google_protobuf_MessageOptions* msg, wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__MessageOptions_msg_init, 0, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE char* google_protobuf_MessageOptions_serialize_ex(const google_protobuf_MessageOptions* msg, int options,
                                 wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__MessageOptions_msg_init, options, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE void google_protobuf_MessageOptions_clear_message_set_wire_format(google_protobuf_MessageOptions* msg) {
  const wpi_upb_MiniTableField field = {1, 9, 64, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE bool google_protobuf_MessageOptions_message_set_wire_format(const google_protobuf_MessageOptions* msg) {
  bool default_val = false;
  bool ret;
  const wpi_upb_MiniTableField field = {1, 9, 64, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_MessageOptions_has_message_set_wire_format(const google_protobuf_MessageOptions* msg) {
  const wpi_upb_MiniTableField field = {1, 9, 64, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_MessageOptions_clear_no_standard_descriptor_accessor(google_protobuf_MessageOptions* msg) {
  const wpi_upb_MiniTableField field = {2, 10, 65, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE bool google_protobuf_MessageOptions_no_standard_descriptor_accessor(const google_protobuf_MessageOptions* msg) {
  bool default_val = false;
  bool ret;
  const wpi_upb_MiniTableField field = {2, 10, 65, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_MessageOptions_has_no_standard_descriptor_accessor(const google_protobuf_MessageOptions* msg) {
  const wpi_upb_MiniTableField field = {2, 10, 65, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_MessageOptions_clear_deprecated(google_protobuf_MessageOptions* msg) {
  const wpi_upb_MiniTableField field = {3, 11, 66, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE bool google_protobuf_MessageOptions_deprecated(const google_protobuf_MessageOptions* msg) {
  bool default_val = false;
  bool ret;
  const wpi_upb_MiniTableField field = {3, 11, 66, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_MessageOptions_has_deprecated(const google_protobuf_MessageOptions* msg) {
  const wpi_upb_MiniTableField field = {3, 11, 66, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_MessageOptions_clear_map_entry(google_protobuf_MessageOptions* msg) {
  const wpi_upb_MiniTableField field = {7, 12, 67, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE bool google_protobuf_MessageOptions_map_entry(const google_protobuf_MessageOptions* msg) {
  bool default_val = false;
  bool ret;
  const wpi_upb_MiniTableField field = {7, 12, 67, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_MessageOptions_has_map_entry(const google_protobuf_MessageOptions* msg) {
  const wpi_upb_MiniTableField field = {7, 12, 67, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_MessageOptions_clear_deprecated_legacy_json_field_conflicts(google_protobuf_MessageOptions* msg) {
  const wpi_upb_MiniTableField field = {11, 13, 68, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE bool google_protobuf_MessageOptions_deprecated_legacy_json_field_conflicts(const google_protobuf_MessageOptions* msg) {
  bool default_val = false;
  bool ret;
  const wpi_upb_MiniTableField field = {11, 13, 68, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_MessageOptions_has_deprecated_legacy_json_field_conflicts(const google_protobuf_MessageOptions* msg) {
  const wpi_upb_MiniTableField field = {11, 13, 68, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_MessageOptions_clear_features(google_protobuf_MessageOptions* msg) {
  const wpi_upb_MiniTableField field = {12, 16, 69, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_FeatureSet* google_protobuf_MessageOptions_features(const google_protobuf_MessageOptions* msg) {
  const google_protobuf_FeatureSet* default_val = NULL;
  const google_protobuf_FeatureSet* ret;
  const wpi_upb_MiniTableField field = {12, 16, 69, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FeatureSet_msg_init);
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_MessageOptions_has_features(const google_protobuf_MessageOptions* msg) {
  const wpi_upb_MiniTableField field = {12, 16, 69, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_MessageOptions_clear_uninterpreted_option(google_protobuf_MessageOptions* msg) {
  const wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(20, 24), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_UninterpretedOption* const* google_protobuf_MessageOptions_uninterpreted_option(const google_protobuf_MessageOptions* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(20, 24), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__UninterpretedOption_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (const google_protobuf_UninterpretedOption* const*)wpi_upb_Array_DataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE const wpi_upb_Array* _google_protobuf_MessageOptions_uninterpreted_option_wpi_upb_array(const google_protobuf_MessageOptions* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(20, 24), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__UninterpretedOption_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE wpi_upb_Array* _google_protobuf_MessageOptions_uninterpreted_option_mutable_wpi_upb_array(google_protobuf_MessageOptions* msg, size_t* size, wpi_upb_Arena* arena) {
  const wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(20, 24), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__UninterpretedOption_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(WPI_UPB_UPCAST(msg),
                                                       &field, arena);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}

WPI_UPB_INLINE void google_protobuf_MessageOptions_set_message_set_wire_format(google_protobuf_MessageOptions *msg, bool value) {
  const wpi_upb_MiniTableField field = {1, 9, 64, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_MessageOptions_set_no_standard_descriptor_accessor(google_protobuf_MessageOptions *msg, bool value) {
  const wpi_upb_MiniTableField field = {2, 10, 65, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_MessageOptions_set_deprecated(google_protobuf_MessageOptions *msg, bool value) {
  const wpi_upb_MiniTableField field = {3, 11, 66, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_MessageOptions_set_map_entry(google_protobuf_MessageOptions *msg, bool value) {
  const wpi_upb_MiniTableField field = {7, 12, 67, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_MessageOptions_set_deprecated_legacy_json_field_conflicts(google_protobuf_MessageOptions *msg, bool value) {
  const wpi_upb_MiniTableField field = {11, 13, 68, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_MessageOptions_set_features(google_protobuf_MessageOptions *msg, google_protobuf_FeatureSet* value) {
  const wpi_upb_MiniTableField field = {12, 16, 69, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FeatureSet_msg_init);
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE struct google_protobuf_FeatureSet* google_protobuf_MessageOptions_mutable_features(google_protobuf_MessageOptions* msg, wpi_upb_Arena* arena) {
  struct google_protobuf_FeatureSet* sub = (struct google_protobuf_FeatureSet*)google_protobuf_MessageOptions_features(msg);
  if (sub == NULL) {
    sub = (struct google_protobuf_FeatureSet*)_wpi_upb_Message_New(&google__protobuf__FeatureSet_msg_init, arena);
    if (sub) google_protobuf_MessageOptions_set_features(msg, sub);
  }
  return sub;
}
WPI_UPB_INLINE google_protobuf_UninterpretedOption** google_protobuf_MessageOptions_mutable_uninterpreted_option(google_protobuf_MessageOptions* msg, size_t* size) {
  wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(20, 24), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__UninterpretedOption_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetMutableArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (google_protobuf_UninterpretedOption**)wpi_upb_Array_MutableDataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE google_protobuf_UninterpretedOption** google_protobuf_MessageOptions_resize_uninterpreted_option(google_protobuf_MessageOptions* msg, size_t size, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(20, 24), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return (google_protobuf_UninterpretedOption**)wpi_upb_Message_ResizeArrayUninitialized(WPI_UPB_UPCAST(msg),
                                                   &field, size, arena);
}
WPI_UPB_INLINE struct google_protobuf_UninterpretedOption* google_protobuf_MessageOptions_add_uninterpreted_option(google_protobuf_MessageOptions* msg, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(20, 24), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__UninterpretedOption_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(
      WPI_UPB_UPCAST(msg), &field, arena);
  if (!arr || !WPI_UPB_PRIVATE(_wpi_upb_Array_ResizeUninitialized)(
                  arr, arr->WPI_UPB_PRIVATE(size) + 1, arena)) {
    return NULL;
  }
  struct google_protobuf_UninterpretedOption* sub = (struct google_protobuf_UninterpretedOption*)_wpi_upb_Message_New(&google__protobuf__UninterpretedOption_msg_init, arena);
  if (!arr || !sub) return NULL;
  WPI_UPB_PRIVATE(_wpi_upb_Array_Set)
  (arr, arr->WPI_UPB_PRIVATE(size) - 1, &sub, sizeof(sub));
  return sub;
}

/* google.protobuf.FieldOptions */

WPI_UPB_INLINE google_protobuf_FieldOptions* google_protobuf_FieldOptions_new(wpi_upb_Arena* arena) {
  return (google_protobuf_FieldOptions*)_wpi_upb_Message_New(&google__protobuf__FieldOptions_msg_init, arena);
}
WPI_UPB_INLINE google_protobuf_FieldOptions* google_protobuf_FieldOptions_parse(const char* buf, size_t size, wpi_upb_Arena* arena) {
  google_protobuf_FieldOptions* ret = google_protobuf_FieldOptions_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__FieldOptions_msg_init, NULL, 0, arena) !=
      kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE google_protobuf_FieldOptions* google_protobuf_FieldOptions_parse_ex(const char* buf, size_t size,
                           const wpi_upb_ExtensionRegistry* extreg,
                           int options, wpi_upb_Arena* arena) {
  google_protobuf_FieldOptions* ret = google_protobuf_FieldOptions_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__FieldOptions_msg_init, extreg, options,
                 arena) != kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE char* google_protobuf_FieldOptions_serialize(const google_protobuf_FieldOptions* msg, wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__FieldOptions_msg_init, 0, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE char* google_protobuf_FieldOptions_serialize_ex(const google_protobuf_FieldOptions* msg, int options,
                                 wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__FieldOptions_msg_init, options, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE void google_protobuf_FieldOptions_clear_ctype(google_protobuf_FieldOptions* msg) {
  const wpi_upb_MiniTableField field = {1, 12, 64, 4, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE int32_t google_protobuf_FieldOptions_ctype(const google_protobuf_FieldOptions* msg) {
  int32_t default_val = 0;
  int32_t ret;
  const wpi_upb_MiniTableField field = {1, 12, 64, 4, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FieldOptions_has_ctype(const google_protobuf_FieldOptions* msg) {
  const wpi_upb_MiniTableField field = {1, 12, 64, 4, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FieldOptions_clear_packed(google_protobuf_FieldOptions* msg) {
  const wpi_upb_MiniTableField field = {2, 16, 65, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE bool google_protobuf_FieldOptions_packed(const google_protobuf_FieldOptions* msg) {
  bool default_val = false;
  bool ret;
  const wpi_upb_MiniTableField field = {2, 16, 65, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FieldOptions_has_packed(const google_protobuf_FieldOptions* msg) {
  const wpi_upb_MiniTableField field = {2, 16, 65, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FieldOptions_clear_deprecated(google_protobuf_FieldOptions* msg) {
  const wpi_upb_MiniTableField field = {3, 17, 66, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE bool google_protobuf_FieldOptions_deprecated(const google_protobuf_FieldOptions* msg) {
  bool default_val = false;
  bool ret;
  const wpi_upb_MiniTableField field = {3, 17, 66, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FieldOptions_has_deprecated(const google_protobuf_FieldOptions* msg) {
  const wpi_upb_MiniTableField field = {3, 17, 66, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FieldOptions_clear_lazy(google_protobuf_FieldOptions* msg) {
  const wpi_upb_MiniTableField field = {5, 18, 67, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE bool google_protobuf_FieldOptions_lazy(const google_protobuf_FieldOptions* msg) {
  bool default_val = false;
  bool ret;
  const wpi_upb_MiniTableField field = {5, 18, 67, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FieldOptions_has_lazy(const google_protobuf_FieldOptions* msg) {
  const wpi_upb_MiniTableField field = {5, 18, 67, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FieldOptions_clear_jstype(google_protobuf_FieldOptions* msg) {
  const wpi_upb_MiniTableField field = {6, 20, 68, 5, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE int32_t google_protobuf_FieldOptions_jstype(const google_protobuf_FieldOptions* msg) {
  int32_t default_val = 0;
  int32_t ret;
  const wpi_upb_MiniTableField field = {6, 20, 68, 5, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FieldOptions_has_jstype(const google_protobuf_FieldOptions* msg) {
  const wpi_upb_MiniTableField field = {6, 20, 68, 5, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FieldOptions_clear_weak(google_protobuf_FieldOptions* msg) {
  const wpi_upb_MiniTableField field = {10, 24, 69, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE bool google_protobuf_FieldOptions_weak(const google_protobuf_FieldOptions* msg) {
  bool default_val = false;
  bool ret;
  const wpi_upb_MiniTableField field = {10, 24, 69, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FieldOptions_has_weak(const google_protobuf_FieldOptions* msg) {
  const wpi_upb_MiniTableField field = {10, 24, 69, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FieldOptions_clear_unverified_lazy(google_protobuf_FieldOptions* msg) {
  const wpi_upb_MiniTableField field = {15, 25, 70, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE bool google_protobuf_FieldOptions_unverified_lazy(const google_protobuf_FieldOptions* msg) {
  bool default_val = false;
  bool ret;
  const wpi_upb_MiniTableField field = {15, 25, 70, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FieldOptions_has_unverified_lazy(const google_protobuf_FieldOptions* msg) {
  const wpi_upb_MiniTableField field = {15, 25, 70, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FieldOptions_clear_debug_redact(google_protobuf_FieldOptions* msg) {
  const wpi_upb_MiniTableField field = {16, 26, 71, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE bool google_protobuf_FieldOptions_debug_redact(const google_protobuf_FieldOptions* msg) {
  bool default_val = false;
  bool ret;
  const wpi_upb_MiniTableField field = {16, 26, 71, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FieldOptions_has_debug_redact(const google_protobuf_FieldOptions* msg) {
  const wpi_upb_MiniTableField field = {16, 26, 71, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FieldOptions_clear_retention(google_protobuf_FieldOptions* msg) {
  const wpi_upb_MiniTableField field = {17, 28, 72, 6, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE int32_t google_protobuf_FieldOptions_retention(const google_protobuf_FieldOptions* msg) {
  int32_t default_val = 0;
  int32_t ret;
  const wpi_upb_MiniTableField field = {17, 28, 72, 6, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FieldOptions_has_retention(const google_protobuf_FieldOptions* msg) {
  const wpi_upb_MiniTableField field = {17, 28, 72, 6, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FieldOptions_clear_targets(google_protobuf_FieldOptions* msg) {
  const wpi_upb_MiniTableField field = {19, 32, 0, 7, 14, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE int32_t const* google_protobuf_FieldOptions_targets(const google_protobuf_FieldOptions* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {19, 32, 0, 7, 14, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (int32_t const*)wpi_upb_Array_DataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE const wpi_upb_Array* _google_protobuf_FieldOptions_targets_wpi_upb_array(const google_protobuf_FieldOptions* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {19, 32, 0, 7, 14, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE wpi_upb_Array* _google_protobuf_FieldOptions_targets_mutable_wpi_upb_array(google_protobuf_FieldOptions* msg, size_t* size, wpi_upb_Arena* arena) {
  const wpi_upb_MiniTableField field = {19, 32, 0, 7, 14, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(WPI_UPB_UPCAST(msg),
                                                       &field, arena);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE void google_protobuf_FieldOptions_clear_edition_defaults(google_protobuf_FieldOptions* msg) {
  const wpi_upb_MiniTableField field = {20, WPI_UPB_SIZE(36, 40), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_FieldOptions_EditionDefault* const* google_protobuf_FieldOptions_edition_defaults(const google_protobuf_FieldOptions* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {20, WPI_UPB_SIZE(36, 40), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FieldOptions__EditionDefault_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (const google_protobuf_FieldOptions_EditionDefault* const*)wpi_upb_Array_DataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE const wpi_upb_Array* _google_protobuf_FieldOptions_edition_defaults_wpi_upb_array(const google_protobuf_FieldOptions* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {20, WPI_UPB_SIZE(36, 40), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FieldOptions__EditionDefault_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE wpi_upb_Array* _google_protobuf_FieldOptions_edition_defaults_mutable_wpi_upb_array(google_protobuf_FieldOptions* msg, size_t* size, wpi_upb_Arena* arena) {
  const wpi_upb_MiniTableField field = {20, WPI_UPB_SIZE(36, 40), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FieldOptions__EditionDefault_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(WPI_UPB_UPCAST(msg),
                                                       &field, arena);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE void google_protobuf_FieldOptions_clear_features(google_protobuf_FieldOptions* msg) {
  const wpi_upb_MiniTableField field = {21, WPI_UPB_SIZE(40, 48), 73, 1, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_FeatureSet* google_protobuf_FieldOptions_features(const google_protobuf_FieldOptions* msg) {
  const google_protobuf_FeatureSet* default_val = NULL;
  const google_protobuf_FeatureSet* ret;
  const wpi_upb_MiniTableField field = {21, WPI_UPB_SIZE(40, 48), 73, 1, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FeatureSet_msg_init);
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FieldOptions_has_features(const google_protobuf_FieldOptions* msg) {
  const wpi_upb_MiniTableField field = {21, WPI_UPB_SIZE(40, 48), 73, 1, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FieldOptions_clear_feature_support(google_protobuf_FieldOptions* msg) {
  const wpi_upb_MiniTableField field = {22, WPI_UPB_SIZE(44, 56), 74, 2, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_FieldOptions_FeatureSupport* google_protobuf_FieldOptions_feature_support(const google_protobuf_FieldOptions* msg) {
  const google_protobuf_FieldOptions_FeatureSupport* default_val = NULL;
  const google_protobuf_FieldOptions_FeatureSupport* ret;
  const wpi_upb_MiniTableField field = {22, WPI_UPB_SIZE(44, 56), 74, 2, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FieldOptions__FeatureSupport_msg_init);
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FieldOptions_has_feature_support(const google_protobuf_FieldOptions* msg) {
  const wpi_upb_MiniTableField field = {22, WPI_UPB_SIZE(44, 56), 74, 2, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FieldOptions_clear_uninterpreted_option(google_protobuf_FieldOptions* msg) {
  const wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(48, 64), 0, 3, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_UninterpretedOption* const* google_protobuf_FieldOptions_uninterpreted_option(const google_protobuf_FieldOptions* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(48, 64), 0, 3, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__UninterpretedOption_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (const google_protobuf_UninterpretedOption* const*)wpi_upb_Array_DataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE const wpi_upb_Array* _google_protobuf_FieldOptions_uninterpreted_option_wpi_upb_array(const google_protobuf_FieldOptions* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(48, 64), 0, 3, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__UninterpretedOption_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE wpi_upb_Array* _google_protobuf_FieldOptions_uninterpreted_option_mutable_wpi_upb_array(google_protobuf_FieldOptions* msg, size_t* size, wpi_upb_Arena* arena) {
  const wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(48, 64), 0, 3, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__UninterpretedOption_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(WPI_UPB_UPCAST(msg),
                                                       &field, arena);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}

WPI_UPB_INLINE void google_protobuf_FieldOptions_set_ctype(google_protobuf_FieldOptions *msg, int32_t value) {
  const wpi_upb_MiniTableField field = {1, 12, 64, 4, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_FieldOptions_set_packed(google_protobuf_FieldOptions *msg, bool value) {
  const wpi_upb_MiniTableField field = {2, 16, 65, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_FieldOptions_set_deprecated(google_protobuf_FieldOptions *msg, bool value) {
  const wpi_upb_MiniTableField field = {3, 17, 66, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_FieldOptions_set_lazy(google_protobuf_FieldOptions *msg, bool value) {
  const wpi_upb_MiniTableField field = {5, 18, 67, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_FieldOptions_set_jstype(google_protobuf_FieldOptions *msg, int32_t value) {
  const wpi_upb_MiniTableField field = {6, 20, 68, 5, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_FieldOptions_set_weak(google_protobuf_FieldOptions *msg, bool value) {
  const wpi_upb_MiniTableField field = {10, 24, 69, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_FieldOptions_set_unverified_lazy(google_protobuf_FieldOptions *msg, bool value) {
  const wpi_upb_MiniTableField field = {15, 25, 70, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_FieldOptions_set_debug_redact(google_protobuf_FieldOptions *msg, bool value) {
  const wpi_upb_MiniTableField field = {16, 26, 71, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_FieldOptions_set_retention(google_protobuf_FieldOptions *msg, int32_t value) {
  const wpi_upb_MiniTableField field = {17, 28, 72, 6, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE int32_t* google_protobuf_FieldOptions_mutable_targets(google_protobuf_FieldOptions* msg, size_t* size) {
  wpi_upb_MiniTableField field = {19, 32, 0, 7, 14, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Array* arr = wpi_upb_Message_GetMutableArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (int32_t*)wpi_upb_Array_MutableDataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE int32_t* google_protobuf_FieldOptions_resize_targets(google_protobuf_FieldOptions* msg, size_t size, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {19, 32, 0, 7, 14, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return (int32_t*)wpi_upb_Message_ResizeArrayUninitialized(WPI_UPB_UPCAST(msg),
                                                   &field, size, arena);
}
WPI_UPB_INLINE bool google_protobuf_FieldOptions_add_targets(google_protobuf_FieldOptions* msg, int32_t val, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {19, 32, 0, 7, 14, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(
      WPI_UPB_UPCAST(msg), &field, arena);
  if (!arr || !WPI_UPB_PRIVATE(_wpi_upb_Array_ResizeUninitialized)(
                  arr, arr->WPI_UPB_PRIVATE(size) + 1, arena)) {
    return false;
  }
  WPI_UPB_PRIVATE(_wpi_upb_Array_Set)
  (arr, arr->WPI_UPB_PRIVATE(size) - 1, &val, sizeof(val));
  return true;
}
WPI_UPB_INLINE google_protobuf_FieldOptions_EditionDefault** google_protobuf_FieldOptions_mutable_edition_defaults(google_protobuf_FieldOptions* msg, size_t* size) {
  wpi_upb_MiniTableField field = {20, WPI_UPB_SIZE(36, 40), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FieldOptions__EditionDefault_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetMutableArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (google_protobuf_FieldOptions_EditionDefault**)wpi_upb_Array_MutableDataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE google_protobuf_FieldOptions_EditionDefault** google_protobuf_FieldOptions_resize_edition_defaults(google_protobuf_FieldOptions* msg, size_t size, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {20, WPI_UPB_SIZE(36, 40), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return (google_protobuf_FieldOptions_EditionDefault**)wpi_upb_Message_ResizeArrayUninitialized(WPI_UPB_UPCAST(msg),
                                                   &field, size, arena);
}
WPI_UPB_INLINE struct google_protobuf_FieldOptions_EditionDefault* google_protobuf_FieldOptions_add_edition_defaults(google_protobuf_FieldOptions* msg, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {20, WPI_UPB_SIZE(36, 40), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FieldOptions__EditionDefault_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(
      WPI_UPB_UPCAST(msg), &field, arena);
  if (!arr || !WPI_UPB_PRIVATE(_wpi_upb_Array_ResizeUninitialized)(
                  arr, arr->WPI_UPB_PRIVATE(size) + 1, arena)) {
    return NULL;
  }
  struct google_protobuf_FieldOptions_EditionDefault* sub = (struct google_protobuf_FieldOptions_EditionDefault*)_wpi_upb_Message_New(&google__protobuf__FieldOptions__EditionDefault_msg_init, arena);
  if (!arr || !sub) return NULL;
  WPI_UPB_PRIVATE(_wpi_upb_Array_Set)
  (arr, arr->WPI_UPB_PRIVATE(size) - 1, &sub, sizeof(sub));
  return sub;
}
WPI_UPB_INLINE void google_protobuf_FieldOptions_set_features(google_protobuf_FieldOptions *msg, google_protobuf_FeatureSet* value) {
  const wpi_upb_MiniTableField field = {21, WPI_UPB_SIZE(40, 48), 73, 1, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FeatureSet_msg_init);
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE struct google_protobuf_FeatureSet* google_protobuf_FieldOptions_mutable_features(google_protobuf_FieldOptions* msg, wpi_upb_Arena* arena) {
  struct google_protobuf_FeatureSet* sub = (struct google_protobuf_FeatureSet*)google_protobuf_FieldOptions_features(msg);
  if (sub == NULL) {
    sub = (struct google_protobuf_FeatureSet*)_wpi_upb_Message_New(&google__protobuf__FeatureSet_msg_init, arena);
    if (sub) google_protobuf_FieldOptions_set_features(msg, sub);
  }
  return sub;
}
WPI_UPB_INLINE void google_protobuf_FieldOptions_set_feature_support(google_protobuf_FieldOptions *msg, google_protobuf_FieldOptions_FeatureSupport* value) {
  const wpi_upb_MiniTableField field = {22, WPI_UPB_SIZE(44, 56), 74, 2, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FieldOptions__FeatureSupport_msg_init);
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE struct google_protobuf_FieldOptions_FeatureSupport* google_protobuf_FieldOptions_mutable_feature_support(google_protobuf_FieldOptions* msg, wpi_upb_Arena* arena) {
  struct google_protobuf_FieldOptions_FeatureSupport* sub = (struct google_protobuf_FieldOptions_FeatureSupport*)google_protobuf_FieldOptions_feature_support(msg);
  if (sub == NULL) {
    sub = (struct google_protobuf_FieldOptions_FeatureSupport*)_wpi_upb_Message_New(&google__protobuf__FieldOptions__FeatureSupport_msg_init, arena);
    if (sub) google_protobuf_FieldOptions_set_feature_support(msg, sub);
  }
  return sub;
}
WPI_UPB_INLINE google_protobuf_UninterpretedOption** google_protobuf_FieldOptions_mutable_uninterpreted_option(google_protobuf_FieldOptions* msg, size_t* size) {
  wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(48, 64), 0, 3, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__UninterpretedOption_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetMutableArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (google_protobuf_UninterpretedOption**)wpi_upb_Array_MutableDataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE google_protobuf_UninterpretedOption** google_protobuf_FieldOptions_resize_uninterpreted_option(google_protobuf_FieldOptions* msg, size_t size, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(48, 64), 0, 3, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return (google_protobuf_UninterpretedOption**)wpi_upb_Message_ResizeArrayUninitialized(WPI_UPB_UPCAST(msg),
                                                   &field, size, arena);
}
WPI_UPB_INLINE struct google_protobuf_UninterpretedOption* google_protobuf_FieldOptions_add_uninterpreted_option(google_protobuf_FieldOptions* msg, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(48, 64), 0, 3, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__UninterpretedOption_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(
      WPI_UPB_UPCAST(msg), &field, arena);
  if (!arr || !WPI_UPB_PRIVATE(_wpi_upb_Array_ResizeUninitialized)(
                  arr, arr->WPI_UPB_PRIVATE(size) + 1, arena)) {
    return NULL;
  }
  struct google_protobuf_UninterpretedOption* sub = (struct google_protobuf_UninterpretedOption*)_wpi_upb_Message_New(&google__protobuf__UninterpretedOption_msg_init, arena);
  if (!arr || !sub) return NULL;
  WPI_UPB_PRIVATE(_wpi_upb_Array_Set)
  (arr, arr->WPI_UPB_PRIVATE(size) - 1, &sub, sizeof(sub));
  return sub;
}

/* google.protobuf.FieldOptions.EditionDefault */

WPI_UPB_INLINE google_protobuf_FieldOptions_EditionDefault* google_protobuf_FieldOptions_EditionDefault_new(wpi_upb_Arena* arena) {
  return (google_protobuf_FieldOptions_EditionDefault*)_wpi_upb_Message_New(&google__protobuf__FieldOptions__EditionDefault_msg_init, arena);
}
WPI_UPB_INLINE google_protobuf_FieldOptions_EditionDefault* google_protobuf_FieldOptions_EditionDefault_parse(const char* buf, size_t size, wpi_upb_Arena* arena) {
  google_protobuf_FieldOptions_EditionDefault* ret = google_protobuf_FieldOptions_EditionDefault_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__FieldOptions__EditionDefault_msg_init, NULL, 0, arena) !=
      kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE google_protobuf_FieldOptions_EditionDefault* google_protobuf_FieldOptions_EditionDefault_parse_ex(const char* buf, size_t size,
                           const wpi_upb_ExtensionRegistry* extreg,
                           int options, wpi_upb_Arena* arena) {
  google_protobuf_FieldOptions_EditionDefault* ret = google_protobuf_FieldOptions_EditionDefault_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__FieldOptions__EditionDefault_msg_init, extreg, options,
                 arena) != kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE char* google_protobuf_FieldOptions_EditionDefault_serialize(const google_protobuf_FieldOptions_EditionDefault* msg, wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__FieldOptions__EditionDefault_msg_init, 0, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE char* google_protobuf_FieldOptions_EditionDefault_serialize_ex(const google_protobuf_FieldOptions_EditionDefault* msg, int options,
                                 wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__FieldOptions__EditionDefault_msg_init, options, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE void google_protobuf_FieldOptions_EditionDefault_clear_value(google_protobuf_FieldOptions_EditionDefault* msg) {
  const wpi_upb_MiniTableField field = {2, 16, 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE wpi_upb_StringView google_protobuf_FieldOptions_EditionDefault_value(const google_protobuf_FieldOptions_EditionDefault* msg) {
  wpi_upb_StringView default_val = wpi_upb_StringView_FromString("");
  wpi_upb_StringView ret;
  const wpi_upb_MiniTableField field = {2, 16, 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FieldOptions_EditionDefault_has_value(const google_protobuf_FieldOptions_EditionDefault* msg) {
  const wpi_upb_MiniTableField field = {2, 16, 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FieldOptions_EditionDefault_clear_edition(google_protobuf_FieldOptions_EditionDefault* msg) {
  const wpi_upb_MiniTableField field = {3, 12, 65, 0, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE int32_t google_protobuf_FieldOptions_EditionDefault_edition(const google_protobuf_FieldOptions_EditionDefault* msg) {
  int32_t default_val = 0;
  int32_t ret;
  const wpi_upb_MiniTableField field = {3, 12, 65, 0, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FieldOptions_EditionDefault_has_edition(const google_protobuf_FieldOptions_EditionDefault* msg) {
  const wpi_upb_MiniTableField field = {3, 12, 65, 0, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}

WPI_UPB_INLINE void google_protobuf_FieldOptions_EditionDefault_set_value(google_protobuf_FieldOptions_EditionDefault *msg, wpi_upb_StringView value) {
  const wpi_upb_MiniTableField field = {2, 16, 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_FieldOptions_EditionDefault_set_edition(google_protobuf_FieldOptions_EditionDefault *msg, int32_t value) {
  const wpi_upb_MiniTableField field = {3, 12, 65, 0, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}

/* google.protobuf.FieldOptions.FeatureSupport */

WPI_UPB_INLINE google_protobuf_FieldOptions_FeatureSupport* google_protobuf_FieldOptions_FeatureSupport_new(wpi_upb_Arena* arena) {
  return (google_protobuf_FieldOptions_FeatureSupport*)_wpi_upb_Message_New(&google__protobuf__FieldOptions__FeatureSupport_msg_init, arena);
}
WPI_UPB_INLINE google_protobuf_FieldOptions_FeatureSupport* google_protobuf_FieldOptions_FeatureSupport_parse(const char* buf, size_t size, wpi_upb_Arena* arena) {
  google_protobuf_FieldOptions_FeatureSupport* ret = google_protobuf_FieldOptions_FeatureSupport_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__FieldOptions__FeatureSupport_msg_init, NULL, 0, arena) !=
      kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE google_protobuf_FieldOptions_FeatureSupport* google_protobuf_FieldOptions_FeatureSupport_parse_ex(const char* buf, size_t size,
                           const wpi_upb_ExtensionRegistry* extreg,
                           int options, wpi_upb_Arena* arena) {
  google_protobuf_FieldOptions_FeatureSupport* ret = google_protobuf_FieldOptions_FeatureSupport_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__FieldOptions__FeatureSupport_msg_init, extreg, options,
                 arena) != kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE char* google_protobuf_FieldOptions_FeatureSupport_serialize(const google_protobuf_FieldOptions_FeatureSupport* msg, wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__FieldOptions__FeatureSupport_msg_init, 0, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE char* google_protobuf_FieldOptions_FeatureSupport_serialize_ex(const google_protobuf_FieldOptions_FeatureSupport* msg, int options,
                                 wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__FieldOptions__FeatureSupport_msg_init, options, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE void google_protobuf_FieldOptions_FeatureSupport_clear_edition_introduced(google_protobuf_FieldOptions_FeatureSupport* msg) {
  const wpi_upb_MiniTableField field = {1, 12, 64, 0, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE int32_t google_protobuf_FieldOptions_FeatureSupport_edition_introduced(const google_protobuf_FieldOptions_FeatureSupport* msg) {
  int32_t default_val = 0;
  int32_t ret;
  const wpi_upb_MiniTableField field = {1, 12, 64, 0, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FieldOptions_FeatureSupport_has_edition_introduced(const google_protobuf_FieldOptions_FeatureSupport* msg) {
  const wpi_upb_MiniTableField field = {1, 12, 64, 0, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FieldOptions_FeatureSupport_clear_edition_deprecated(google_protobuf_FieldOptions_FeatureSupport* msg) {
  const wpi_upb_MiniTableField field = {2, 16, 65, 1, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE int32_t google_protobuf_FieldOptions_FeatureSupport_edition_deprecated(const google_protobuf_FieldOptions_FeatureSupport* msg) {
  int32_t default_val = 0;
  int32_t ret;
  const wpi_upb_MiniTableField field = {2, 16, 65, 1, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FieldOptions_FeatureSupport_has_edition_deprecated(const google_protobuf_FieldOptions_FeatureSupport* msg) {
  const wpi_upb_MiniTableField field = {2, 16, 65, 1, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FieldOptions_FeatureSupport_clear_deprecation_warning(google_protobuf_FieldOptions_FeatureSupport* msg) {
  const wpi_upb_MiniTableField field = {3, 24, 66, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE wpi_upb_StringView google_protobuf_FieldOptions_FeatureSupport_deprecation_warning(const google_protobuf_FieldOptions_FeatureSupport* msg) {
  wpi_upb_StringView default_val = wpi_upb_StringView_FromString("");
  wpi_upb_StringView ret;
  const wpi_upb_MiniTableField field = {3, 24, 66, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FieldOptions_FeatureSupport_has_deprecation_warning(const google_protobuf_FieldOptions_FeatureSupport* msg) {
  const wpi_upb_MiniTableField field = {3, 24, 66, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FieldOptions_FeatureSupport_clear_edition_removed(google_protobuf_FieldOptions_FeatureSupport* msg) {
  const wpi_upb_MiniTableField field = {4, 20, 67, 2, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE int32_t google_protobuf_FieldOptions_FeatureSupport_edition_removed(const google_protobuf_FieldOptions_FeatureSupport* msg) {
  int32_t default_val = 0;
  int32_t ret;
  const wpi_upb_MiniTableField field = {4, 20, 67, 2, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FieldOptions_FeatureSupport_has_edition_removed(const google_protobuf_FieldOptions_FeatureSupport* msg) {
  const wpi_upb_MiniTableField field = {4, 20, 67, 2, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}

WPI_UPB_INLINE void google_protobuf_FieldOptions_FeatureSupport_set_edition_introduced(google_protobuf_FieldOptions_FeatureSupport *msg, int32_t value) {
  const wpi_upb_MiniTableField field = {1, 12, 64, 0, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_FieldOptions_FeatureSupport_set_edition_deprecated(google_protobuf_FieldOptions_FeatureSupport *msg, int32_t value) {
  const wpi_upb_MiniTableField field = {2, 16, 65, 1, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_FieldOptions_FeatureSupport_set_deprecation_warning(google_protobuf_FieldOptions_FeatureSupport *msg, wpi_upb_StringView value) {
  const wpi_upb_MiniTableField field = {3, 24, 66, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_FieldOptions_FeatureSupport_set_edition_removed(google_protobuf_FieldOptions_FeatureSupport *msg, int32_t value) {
  const wpi_upb_MiniTableField field = {4, 20, 67, 2, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}

/* google.protobuf.OneofOptions */

WPI_UPB_INLINE google_protobuf_OneofOptions* google_protobuf_OneofOptions_new(wpi_upb_Arena* arena) {
  return (google_protobuf_OneofOptions*)_wpi_upb_Message_New(&google__protobuf__OneofOptions_msg_init, arena);
}
WPI_UPB_INLINE google_protobuf_OneofOptions* google_protobuf_OneofOptions_parse(const char* buf, size_t size, wpi_upb_Arena* arena) {
  google_protobuf_OneofOptions* ret = google_protobuf_OneofOptions_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__OneofOptions_msg_init, NULL, 0, arena) !=
      kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE google_protobuf_OneofOptions* google_protobuf_OneofOptions_parse_ex(const char* buf, size_t size,
                           const wpi_upb_ExtensionRegistry* extreg,
                           int options, wpi_upb_Arena* arena) {
  google_protobuf_OneofOptions* ret = google_protobuf_OneofOptions_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__OneofOptions_msg_init, extreg, options,
                 arena) != kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE char* google_protobuf_OneofOptions_serialize(const google_protobuf_OneofOptions* msg, wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__OneofOptions_msg_init, 0, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE char* google_protobuf_OneofOptions_serialize_ex(const google_protobuf_OneofOptions* msg, int options,
                                 wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__OneofOptions_msg_init, options, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE void google_protobuf_OneofOptions_clear_features(google_protobuf_OneofOptions* msg) {
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(12, 16), 64, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_FeatureSet* google_protobuf_OneofOptions_features(const google_protobuf_OneofOptions* msg) {
  const google_protobuf_FeatureSet* default_val = NULL;
  const google_protobuf_FeatureSet* ret;
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(12, 16), 64, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FeatureSet_msg_init);
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_OneofOptions_has_features(const google_protobuf_OneofOptions* msg) {
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(12, 16), 64, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_OneofOptions_clear_uninterpreted_option(google_protobuf_OneofOptions* msg) {
  const wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(16, 24), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_UninterpretedOption* const* google_protobuf_OneofOptions_uninterpreted_option(const google_protobuf_OneofOptions* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(16, 24), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__UninterpretedOption_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (const google_protobuf_UninterpretedOption* const*)wpi_upb_Array_DataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE const wpi_upb_Array* _google_protobuf_OneofOptions_uninterpreted_option_wpi_upb_array(const google_protobuf_OneofOptions* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(16, 24), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__UninterpretedOption_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE wpi_upb_Array* _google_protobuf_OneofOptions_uninterpreted_option_mutable_wpi_upb_array(google_protobuf_OneofOptions* msg, size_t* size, wpi_upb_Arena* arena) {
  const wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(16, 24), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__UninterpretedOption_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(WPI_UPB_UPCAST(msg),
                                                       &field, arena);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}

WPI_UPB_INLINE void google_protobuf_OneofOptions_set_features(google_protobuf_OneofOptions *msg, google_protobuf_FeatureSet* value) {
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(12, 16), 64, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FeatureSet_msg_init);
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE struct google_protobuf_FeatureSet* google_protobuf_OneofOptions_mutable_features(google_protobuf_OneofOptions* msg, wpi_upb_Arena* arena) {
  struct google_protobuf_FeatureSet* sub = (struct google_protobuf_FeatureSet*)google_protobuf_OneofOptions_features(msg);
  if (sub == NULL) {
    sub = (struct google_protobuf_FeatureSet*)_wpi_upb_Message_New(&google__protobuf__FeatureSet_msg_init, arena);
    if (sub) google_protobuf_OneofOptions_set_features(msg, sub);
  }
  return sub;
}
WPI_UPB_INLINE google_protobuf_UninterpretedOption** google_protobuf_OneofOptions_mutable_uninterpreted_option(google_protobuf_OneofOptions* msg, size_t* size) {
  wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(16, 24), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__UninterpretedOption_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetMutableArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (google_protobuf_UninterpretedOption**)wpi_upb_Array_MutableDataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE google_protobuf_UninterpretedOption** google_protobuf_OneofOptions_resize_uninterpreted_option(google_protobuf_OneofOptions* msg, size_t size, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(16, 24), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return (google_protobuf_UninterpretedOption**)wpi_upb_Message_ResizeArrayUninitialized(WPI_UPB_UPCAST(msg),
                                                   &field, size, arena);
}
WPI_UPB_INLINE struct google_protobuf_UninterpretedOption* google_protobuf_OneofOptions_add_uninterpreted_option(google_protobuf_OneofOptions* msg, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(16, 24), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__UninterpretedOption_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(
      WPI_UPB_UPCAST(msg), &field, arena);
  if (!arr || !WPI_UPB_PRIVATE(_wpi_upb_Array_ResizeUninitialized)(
                  arr, arr->WPI_UPB_PRIVATE(size) + 1, arena)) {
    return NULL;
  }
  struct google_protobuf_UninterpretedOption* sub = (struct google_protobuf_UninterpretedOption*)_wpi_upb_Message_New(&google__protobuf__UninterpretedOption_msg_init, arena);
  if (!arr || !sub) return NULL;
  WPI_UPB_PRIVATE(_wpi_upb_Array_Set)
  (arr, arr->WPI_UPB_PRIVATE(size) - 1, &sub, sizeof(sub));
  return sub;
}

/* google.protobuf.EnumOptions */

WPI_UPB_INLINE google_protobuf_EnumOptions* google_protobuf_EnumOptions_new(wpi_upb_Arena* arena) {
  return (google_protobuf_EnumOptions*)_wpi_upb_Message_New(&google__protobuf__EnumOptions_msg_init, arena);
}
WPI_UPB_INLINE google_protobuf_EnumOptions* google_protobuf_EnumOptions_parse(const char* buf, size_t size, wpi_upb_Arena* arena) {
  google_protobuf_EnumOptions* ret = google_protobuf_EnumOptions_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__EnumOptions_msg_init, NULL, 0, arena) !=
      kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE google_protobuf_EnumOptions* google_protobuf_EnumOptions_parse_ex(const char* buf, size_t size,
                           const wpi_upb_ExtensionRegistry* extreg,
                           int options, wpi_upb_Arena* arena) {
  google_protobuf_EnumOptions* ret = google_protobuf_EnumOptions_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__EnumOptions_msg_init, extreg, options,
                 arena) != kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE char* google_protobuf_EnumOptions_serialize(const google_protobuf_EnumOptions* msg, wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__EnumOptions_msg_init, 0, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE char* google_protobuf_EnumOptions_serialize_ex(const google_protobuf_EnumOptions* msg, int options,
                                 wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__EnumOptions_msg_init, options, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE void google_protobuf_EnumOptions_clear_allow_alias(google_protobuf_EnumOptions* msg) {
  const wpi_upb_MiniTableField field = {2, 9, 64, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE bool google_protobuf_EnumOptions_allow_alias(const google_protobuf_EnumOptions* msg) {
  bool default_val = false;
  bool ret;
  const wpi_upb_MiniTableField field = {2, 9, 64, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_EnumOptions_has_allow_alias(const google_protobuf_EnumOptions* msg) {
  const wpi_upb_MiniTableField field = {2, 9, 64, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_EnumOptions_clear_deprecated(google_protobuf_EnumOptions* msg) {
  const wpi_upb_MiniTableField field = {3, 10, 65, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE bool google_protobuf_EnumOptions_deprecated(const google_protobuf_EnumOptions* msg) {
  bool default_val = false;
  bool ret;
  const wpi_upb_MiniTableField field = {3, 10, 65, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_EnumOptions_has_deprecated(const google_protobuf_EnumOptions* msg) {
  const wpi_upb_MiniTableField field = {3, 10, 65, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_EnumOptions_clear_deprecated_legacy_json_field_conflicts(google_protobuf_EnumOptions* msg) {
  const wpi_upb_MiniTableField field = {6, 11, 66, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE bool google_protobuf_EnumOptions_deprecated_legacy_json_field_conflicts(const google_protobuf_EnumOptions* msg) {
  bool default_val = false;
  bool ret;
  const wpi_upb_MiniTableField field = {6, 11, 66, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_EnumOptions_has_deprecated_legacy_json_field_conflicts(const google_protobuf_EnumOptions* msg) {
  const wpi_upb_MiniTableField field = {6, 11, 66, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_EnumOptions_clear_features(google_protobuf_EnumOptions* msg) {
  const wpi_upb_MiniTableField field = {7, WPI_UPB_SIZE(12, 16), 67, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_FeatureSet* google_protobuf_EnumOptions_features(const google_protobuf_EnumOptions* msg) {
  const google_protobuf_FeatureSet* default_val = NULL;
  const google_protobuf_FeatureSet* ret;
  const wpi_upb_MiniTableField field = {7, WPI_UPB_SIZE(12, 16), 67, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FeatureSet_msg_init);
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_EnumOptions_has_features(const google_protobuf_EnumOptions* msg) {
  const wpi_upb_MiniTableField field = {7, WPI_UPB_SIZE(12, 16), 67, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_EnumOptions_clear_uninterpreted_option(google_protobuf_EnumOptions* msg) {
  const wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(16, 24), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_UninterpretedOption* const* google_protobuf_EnumOptions_uninterpreted_option(const google_protobuf_EnumOptions* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(16, 24), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__UninterpretedOption_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (const google_protobuf_UninterpretedOption* const*)wpi_upb_Array_DataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE const wpi_upb_Array* _google_protobuf_EnumOptions_uninterpreted_option_wpi_upb_array(const google_protobuf_EnumOptions* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(16, 24), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__UninterpretedOption_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE wpi_upb_Array* _google_protobuf_EnumOptions_uninterpreted_option_mutable_wpi_upb_array(google_protobuf_EnumOptions* msg, size_t* size, wpi_upb_Arena* arena) {
  const wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(16, 24), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__UninterpretedOption_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(WPI_UPB_UPCAST(msg),
                                                       &field, arena);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}

WPI_UPB_INLINE void google_protobuf_EnumOptions_set_allow_alias(google_protobuf_EnumOptions *msg, bool value) {
  const wpi_upb_MiniTableField field = {2, 9, 64, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_EnumOptions_set_deprecated(google_protobuf_EnumOptions *msg, bool value) {
  const wpi_upb_MiniTableField field = {3, 10, 65, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_EnumOptions_set_deprecated_legacy_json_field_conflicts(google_protobuf_EnumOptions *msg, bool value) {
  const wpi_upb_MiniTableField field = {6, 11, 66, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_EnumOptions_set_features(google_protobuf_EnumOptions *msg, google_protobuf_FeatureSet* value) {
  const wpi_upb_MiniTableField field = {7, WPI_UPB_SIZE(12, 16), 67, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FeatureSet_msg_init);
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE struct google_protobuf_FeatureSet* google_protobuf_EnumOptions_mutable_features(google_protobuf_EnumOptions* msg, wpi_upb_Arena* arena) {
  struct google_protobuf_FeatureSet* sub = (struct google_protobuf_FeatureSet*)google_protobuf_EnumOptions_features(msg);
  if (sub == NULL) {
    sub = (struct google_protobuf_FeatureSet*)_wpi_upb_Message_New(&google__protobuf__FeatureSet_msg_init, arena);
    if (sub) google_protobuf_EnumOptions_set_features(msg, sub);
  }
  return sub;
}
WPI_UPB_INLINE google_protobuf_UninterpretedOption** google_protobuf_EnumOptions_mutable_uninterpreted_option(google_protobuf_EnumOptions* msg, size_t* size) {
  wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(16, 24), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__UninterpretedOption_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetMutableArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (google_protobuf_UninterpretedOption**)wpi_upb_Array_MutableDataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE google_protobuf_UninterpretedOption** google_protobuf_EnumOptions_resize_uninterpreted_option(google_protobuf_EnumOptions* msg, size_t size, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(16, 24), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return (google_protobuf_UninterpretedOption**)wpi_upb_Message_ResizeArrayUninitialized(WPI_UPB_UPCAST(msg),
                                                   &field, size, arena);
}
WPI_UPB_INLINE struct google_protobuf_UninterpretedOption* google_protobuf_EnumOptions_add_uninterpreted_option(google_protobuf_EnumOptions* msg, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(16, 24), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__UninterpretedOption_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(
      WPI_UPB_UPCAST(msg), &field, arena);
  if (!arr || !WPI_UPB_PRIVATE(_wpi_upb_Array_ResizeUninitialized)(
                  arr, arr->WPI_UPB_PRIVATE(size) + 1, arena)) {
    return NULL;
  }
  struct google_protobuf_UninterpretedOption* sub = (struct google_protobuf_UninterpretedOption*)_wpi_upb_Message_New(&google__protobuf__UninterpretedOption_msg_init, arena);
  if (!arr || !sub) return NULL;
  WPI_UPB_PRIVATE(_wpi_upb_Array_Set)
  (arr, arr->WPI_UPB_PRIVATE(size) - 1, &sub, sizeof(sub));
  return sub;
}

/* google.protobuf.EnumValueOptions */

WPI_UPB_INLINE google_protobuf_EnumValueOptions* google_protobuf_EnumValueOptions_new(wpi_upb_Arena* arena) {
  return (google_protobuf_EnumValueOptions*)_wpi_upb_Message_New(&google__protobuf__EnumValueOptions_msg_init, arena);
}
WPI_UPB_INLINE google_protobuf_EnumValueOptions* google_protobuf_EnumValueOptions_parse(const char* buf, size_t size, wpi_upb_Arena* arena) {
  google_protobuf_EnumValueOptions* ret = google_protobuf_EnumValueOptions_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__EnumValueOptions_msg_init, NULL, 0, arena) !=
      kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE google_protobuf_EnumValueOptions* google_protobuf_EnumValueOptions_parse_ex(const char* buf, size_t size,
                           const wpi_upb_ExtensionRegistry* extreg,
                           int options, wpi_upb_Arena* arena) {
  google_protobuf_EnumValueOptions* ret = google_protobuf_EnumValueOptions_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__EnumValueOptions_msg_init, extreg, options,
                 arena) != kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE char* google_protobuf_EnumValueOptions_serialize(const google_protobuf_EnumValueOptions* msg, wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__EnumValueOptions_msg_init, 0, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE char* google_protobuf_EnumValueOptions_serialize_ex(const google_protobuf_EnumValueOptions* msg, int options,
                                 wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__EnumValueOptions_msg_init, options, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE void google_protobuf_EnumValueOptions_clear_deprecated(google_protobuf_EnumValueOptions* msg) {
  const wpi_upb_MiniTableField field = {1, 9, 64, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE bool google_protobuf_EnumValueOptions_deprecated(const google_protobuf_EnumValueOptions* msg) {
  bool default_val = false;
  bool ret;
  const wpi_upb_MiniTableField field = {1, 9, 64, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_EnumValueOptions_has_deprecated(const google_protobuf_EnumValueOptions* msg) {
  const wpi_upb_MiniTableField field = {1, 9, 64, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_EnumValueOptions_clear_features(google_protobuf_EnumValueOptions* msg) {
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(12, 16), 65, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_FeatureSet* google_protobuf_EnumValueOptions_features(const google_protobuf_EnumValueOptions* msg) {
  const google_protobuf_FeatureSet* default_val = NULL;
  const google_protobuf_FeatureSet* ret;
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(12, 16), 65, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FeatureSet_msg_init);
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_EnumValueOptions_has_features(const google_protobuf_EnumValueOptions* msg) {
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(12, 16), 65, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_EnumValueOptions_clear_debug_redact(google_protobuf_EnumValueOptions* msg) {
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(16, 10), 66, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE bool google_protobuf_EnumValueOptions_debug_redact(const google_protobuf_EnumValueOptions* msg) {
  bool default_val = false;
  bool ret;
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(16, 10), 66, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_EnumValueOptions_has_debug_redact(const google_protobuf_EnumValueOptions* msg) {
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(16, 10), 66, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_EnumValueOptions_clear_feature_support(google_protobuf_EnumValueOptions* msg) {
  const wpi_upb_MiniTableField field = {4, WPI_UPB_SIZE(20, 24), 67, 1, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_FieldOptions_FeatureSupport* google_protobuf_EnumValueOptions_feature_support(const google_protobuf_EnumValueOptions* msg) {
  const google_protobuf_FieldOptions_FeatureSupport* default_val = NULL;
  const google_protobuf_FieldOptions_FeatureSupport* ret;
  const wpi_upb_MiniTableField field = {4, WPI_UPB_SIZE(20, 24), 67, 1, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FieldOptions__FeatureSupport_msg_init);
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_EnumValueOptions_has_feature_support(const google_protobuf_EnumValueOptions* msg) {
  const wpi_upb_MiniTableField field = {4, WPI_UPB_SIZE(20, 24), 67, 1, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_EnumValueOptions_clear_uninterpreted_option(google_protobuf_EnumValueOptions* msg) {
  const wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(24, 32), 0, 2, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_UninterpretedOption* const* google_protobuf_EnumValueOptions_uninterpreted_option(const google_protobuf_EnumValueOptions* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(24, 32), 0, 2, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__UninterpretedOption_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (const google_protobuf_UninterpretedOption* const*)wpi_upb_Array_DataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE const wpi_upb_Array* _google_protobuf_EnumValueOptions_uninterpreted_option_wpi_upb_array(const google_protobuf_EnumValueOptions* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(24, 32), 0, 2, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__UninterpretedOption_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE wpi_upb_Array* _google_protobuf_EnumValueOptions_uninterpreted_option_mutable_wpi_upb_array(google_protobuf_EnumValueOptions* msg, size_t* size, wpi_upb_Arena* arena) {
  const wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(24, 32), 0, 2, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__UninterpretedOption_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(WPI_UPB_UPCAST(msg),
                                                       &field, arena);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}

WPI_UPB_INLINE void google_protobuf_EnumValueOptions_set_deprecated(google_protobuf_EnumValueOptions *msg, bool value) {
  const wpi_upb_MiniTableField field = {1, 9, 64, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_EnumValueOptions_set_features(google_protobuf_EnumValueOptions *msg, google_protobuf_FeatureSet* value) {
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(12, 16), 65, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FeatureSet_msg_init);
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE struct google_protobuf_FeatureSet* google_protobuf_EnumValueOptions_mutable_features(google_protobuf_EnumValueOptions* msg, wpi_upb_Arena* arena) {
  struct google_protobuf_FeatureSet* sub = (struct google_protobuf_FeatureSet*)google_protobuf_EnumValueOptions_features(msg);
  if (sub == NULL) {
    sub = (struct google_protobuf_FeatureSet*)_wpi_upb_Message_New(&google__protobuf__FeatureSet_msg_init, arena);
    if (sub) google_protobuf_EnumValueOptions_set_features(msg, sub);
  }
  return sub;
}
WPI_UPB_INLINE void google_protobuf_EnumValueOptions_set_debug_redact(google_protobuf_EnumValueOptions *msg, bool value) {
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(16, 10), 66, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_EnumValueOptions_set_feature_support(google_protobuf_EnumValueOptions *msg, google_protobuf_FieldOptions_FeatureSupport* value) {
  const wpi_upb_MiniTableField field = {4, WPI_UPB_SIZE(20, 24), 67, 1, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FieldOptions__FeatureSupport_msg_init);
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE struct google_protobuf_FieldOptions_FeatureSupport* google_protobuf_EnumValueOptions_mutable_feature_support(google_protobuf_EnumValueOptions* msg, wpi_upb_Arena* arena) {
  struct google_protobuf_FieldOptions_FeatureSupport* sub = (struct google_protobuf_FieldOptions_FeatureSupport*)google_protobuf_EnumValueOptions_feature_support(msg);
  if (sub == NULL) {
    sub = (struct google_protobuf_FieldOptions_FeatureSupport*)_wpi_upb_Message_New(&google__protobuf__FieldOptions__FeatureSupport_msg_init, arena);
    if (sub) google_protobuf_EnumValueOptions_set_feature_support(msg, sub);
  }
  return sub;
}
WPI_UPB_INLINE google_protobuf_UninterpretedOption** google_protobuf_EnumValueOptions_mutable_uninterpreted_option(google_protobuf_EnumValueOptions* msg, size_t* size) {
  wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(24, 32), 0, 2, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__UninterpretedOption_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetMutableArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (google_protobuf_UninterpretedOption**)wpi_upb_Array_MutableDataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE google_protobuf_UninterpretedOption** google_protobuf_EnumValueOptions_resize_uninterpreted_option(google_protobuf_EnumValueOptions* msg, size_t size, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(24, 32), 0, 2, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return (google_protobuf_UninterpretedOption**)wpi_upb_Message_ResizeArrayUninitialized(WPI_UPB_UPCAST(msg),
                                                   &field, size, arena);
}
WPI_UPB_INLINE struct google_protobuf_UninterpretedOption* google_protobuf_EnumValueOptions_add_uninterpreted_option(google_protobuf_EnumValueOptions* msg, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(24, 32), 0, 2, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__UninterpretedOption_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(
      WPI_UPB_UPCAST(msg), &field, arena);
  if (!arr || !WPI_UPB_PRIVATE(_wpi_upb_Array_ResizeUninitialized)(
                  arr, arr->WPI_UPB_PRIVATE(size) + 1, arena)) {
    return NULL;
  }
  struct google_protobuf_UninterpretedOption* sub = (struct google_protobuf_UninterpretedOption*)_wpi_upb_Message_New(&google__protobuf__UninterpretedOption_msg_init, arena);
  if (!arr || !sub) return NULL;
  WPI_UPB_PRIVATE(_wpi_upb_Array_Set)
  (arr, arr->WPI_UPB_PRIVATE(size) - 1, &sub, sizeof(sub));
  return sub;
}

/* google.protobuf.ServiceOptions */

WPI_UPB_INLINE google_protobuf_ServiceOptions* google_protobuf_ServiceOptions_new(wpi_upb_Arena* arena) {
  return (google_protobuf_ServiceOptions*)_wpi_upb_Message_New(&google__protobuf__ServiceOptions_msg_init, arena);
}
WPI_UPB_INLINE google_protobuf_ServiceOptions* google_protobuf_ServiceOptions_parse(const char* buf, size_t size, wpi_upb_Arena* arena) {
  google_protobuf_ServiceOptions* ret = google_protobuf_ServiceOptions_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__ServiceOptions_msg_init, NULL, 0, arena) !=
      kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE google_protobuf_ServiceOptions* google_protobuf_ServiceOptions_parse_ex(const char* buf, size_t size,
                           const wpi_upb_ExtensionRegistry* extreg,
                           int options, wpi_upb_Arena* arena) {
  google_protobuf_ServiceOptions* ret = google_protobuf_ServiceOptions_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__ServiceOptions_msg_init, extreg, options,
                 arena) != kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE char* google_protobuf_ServiceOptions_serialize(const google_protobuf_ServiceOptions* msg, wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__ServiceOptions_msg_init, 0, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE char* google_protobuf_ServiceOptions_serialize_ex(const google_protobuf_ServiceOptions* msg, int options,
                                 wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__ServiceOptions_msg_init, options, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE void google_protobuf_ServiceOptions_clear_deprecated(google_protobuf_ServiceOptions* msg) {
  const wpi_upb_MiniTableField field = {33, 9, 64, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE bool google_protobuf_ServiceOptions_deprecated(const google_protobuf_ServiceOptions* msg) {
  bool default_val = false;
  bool ret;
  const wpi_upb_MiniTableField field = {33, 9, 64, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_ServiceOptions_has_deprecated(const google_protobuf_ServiceOptions* msg) {
  const wpi_upb_MiniTableField field = {33, 9, 64, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_ServiceOptions_clear_features(google_protobuf_ServiceOptions* msg) {
  const wpi_upb_MiniTableField field = {34, WPI_UPB_SIZE(12, 16), 65, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_FeatureSet* google_protobuf_ServiceOptions_features(const google_protobuf_ServiceOptions* msg) {
  const google_protobuf_FeatureSet* default_val = NULL;
  const google_protobuf_FeatureSet* ret;
  const wpi_upb_MiniTableField field = {34, WPI_UPB_SIZE(12, 16), 65, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FeatureSet_msg_init);
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_ServiceOptions_has_features(const google_protobuf_ServiceOptions* msg) {
  const wpi_upb_MiniTableField field = {34, WPI_UPB_SIZE(12, 16), 65, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_ServiceOptions_clear_uninterpreted_option(google_protobuf_ServiceOptions* msg) {
  const wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(16, 24), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_UninterpretedOption* const* google_protobuf_ServiceOptions_uninterpreted_option(const google_protobuf_ServiceOptions* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(16, 24), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__UninterpretedOption_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (const google_protobuf_UninterpretedOption* const*)wpi_upb_Array_DataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE const wpi_upb_Array* _google_protobuf_ServiceOptions_uninterpreted_option_wpi_upb_array(const google_protobuf_ServiceOptions* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(16, 24), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__UninterpretedOption_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE wpi_upb_Array* _google_protobuf_ServiceOptions_uninterpreted_option_mutable_wpi_upb_array(google_protobuf_ServiceOptions* msg, size_t* size, wpi_upb_Arena* arena) {
  const wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(16, 24), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__UninterpretedOption_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(WPI_UPB_UPCAST(msg),
                                                       &field, arena);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}

WPI_UPB_INLINE void google_protobuf_ServiceOptions_set_deprecated(google_protobuf_ServiceOptions *msg, bool value) {
  const wpi_upb_MiniTableField field = {33, 9, 64, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_ServiceOptions_set_features(google_protobuf_ServiceOptions *msg, google_protobuf_FeatureSet* value) {
  const wpi_upb_MiniTableField field = {34, WPI_UPB_SIZE(12, 16), 65, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FeatureSet_msg_init);
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE struct google_protobuf_FeatureSet* google_protobuf_ServiceOptions_mutable_features(google_protobuf_ServiceOptions* msg, wpi_upb_Arena* arena) {
  struct google_protobuf_FeatureSet* sub = (struct google_protobuf_FeatureSet*)google_protobuf_ServiceOptions_features(msg);
  if (sub == NULL) {
    sub = (struct google_protobuf_FeatureSet*)_wpi_upb_Message_New(&google__protobuf__FeatureSet_msg_init, arena);
    if (sub) google_protobuf_ServiceOptions_set_features(msg, sub);
  }
  return sub;
}
WPI_UPB_INLINE google_protobuf_UninterpretedOption** google_protobuf_ServiceOptions_mutable_uninterpreted_option(google_protobuf_ServiceOptions* msg, size_t* size) {
  wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(16, 24), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__UninterpretedOption_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetMutableArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (google_protobuf_UninterpretedOption**)wpi_upb_Array_MutableDataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE google_protobuf_UninterpretedOption** google_protobuf_ServiceOptions_resize_uninterpreted_option(google_protobuf_ServiceOptions* msg, size_t size, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(16, 24), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return (google_protobuf_UninterpretedOption**)wpi_upb_Message_ResizeArrayUninitialized(WPI_UPB_UPCAST(msg),
                                                   &field, size, arena);
}
WPI_UPB_INLINE struct google_protobuf_UninterpretedOption* google_protobuf_ServiceOptions_add_uninterpreted_option(google_protobuf_ServiceOptions* msg, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(16, 24), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__UninterpretedOption_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(
      WPI_UPB_UPCAST(msg), &field, arena);
  if (!arr || !WPI_UPB_PRIVATE(_wpi_upb_Array_ResizeUninitialized)(
                  arr, arr->WPI_UPB_PRIVATE(size) + 1, arena)) {
    return NULL;
  }
  struct google_protobuf_UninterpretedOption* sub = (struct google_protobuf_UninterpretedOption*)_wpi_upb_Message_New(&google__protobuf__UninterpretedOption_msg_init, arena);
  if (!arr || !sub) return NULL;
  WPI_UPB_PRIVATE(_wpi_upb_Array_Set)
  (arr, arr->WPI_UPB_PRIVATE(size) - 1, &sub, sizeof(sub));
  return sub;
}

/* google.protobuf.MethodOptions */

WPI_UPB_INLINE google_protobuf_MethodOptions* google_protobuf_MethodOptions_new(wpi_upb_Arena* arena) {
  return (google_protobuf_MethodOptions*)_wpi_upb_Message_New(&google__protobuf__MethodOptions_msg_init, arena);
}
WPI_UPB_INLINE google_protobuf_MethodOptions* google_protobuf_MethodOptions_parse(const char* buf, size_t size, wpi_upb_Arena* arena) {
  google_protobuf_MethodOptions* ret = google_protobuf_MethodOptions_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__MethodOptions_msg_init, NULL, 0, arena) !=
      kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE google_protobuf_MethodOptions* google_protobuf_MethodOptions_parse_ex(const char* buf, size_t size,
                           const wpi_upb_ExtensionRegistry* extreg,
                           int options, wpi_upb_Arena* arena) {
  google_protobuf_MethodOptions* ret = google_protobuf_MethodOptions_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__MethodOptions_msg_init, extreg, options,
                 arena) != kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE char* google_protobuf_MethodOptions_serialize(const google_protobuf_MethodOptions* msg, wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__MethodOptions_msg_init, 0, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE char* google_protobuf_MethodOptions_serialize_ex(const google_protobuf_MethodOptions* msg, int options,
                                 wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__MethodOptions_msg_init, options, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE void google_protobuf_MethodOptions_clear_deprecated(google_protobuf_MethodOptions* msg) {
  const wpi_upb_MiniTableField field = {33, 9, 64, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE bool google_protobuf_MethodOptions_deprecated(const google_protobuf_MethodOptions* msg) {
  bool default_val = false;
  bool ret;
  const wpi_upb_MiniTableField field = {33, 9, 64, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_MethodOptions_has_deprecated(const google_protobuf_MethodOptions* msg) {
  const wpi_upb_MiniTableField field = {33, 9, 64, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_MethodOptions_clear_idempotency_level(google_protobuf_MethodOptions* msg) {
  const wpi_upb_MiniTableField field = {34, 12, 65, 2, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE int32_t google_protobuf_MethodOptions_idempotency_level(const google_protobuf_MethodOptions* msg) {
  int32_t default_val = 0;
  int32_t ret;
  const wpi_upb_MiniTableField field = {34, 12, 65, 2, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_MethodOptions_has_idempotency_level(const google_protobuf_MethodOptions* msg) {
  const wpi_upb_MiniTableField field = {34, 12, 65, 2, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_MethodOptions_clear_features(google_protobuf_MethodOptions* msg) {
  const wpi_upb_MiniTableField field = {35, 16, 66, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_FeatureSet* google_protobuf_MethodOptions_features(const google_protobuf_MethodOptions* msg) {
  const google_protobuf_FeatureSet* default_val = NULL;
  const google_protobuf_FeatureSet* ret;
  const wpi_upb_MiniTableField field = {35, 16, 66, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FeatureSet_msg_init);
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_MethodOptions_has_features(const google_protobuf_MethodOptions* msg) {
  const wpi_upb_MiniTableField field = {35, 16, 66, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_MethodOptions_clear_uninterpreted_option(google_protobuf_MethodOptions* msg) {
  const wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(20, 24), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_UninterpretedOption* const* google_protobuf_MethodOptions_uninterpreted_option(const google_protobuf_MethodOptions* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(20, 24), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__UninterpretedOption_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (const google_protobuf_UninterpretedOption* const*)wpi_upb_Array_DataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE const wpi_upb_Array* _google_protobuf_MethodOptions_uninterpreted_option_wpi_upb_array(const google_protobuf_MethodOptions* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(20, 24), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__UninterpretedOption_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE wpi_upb_Array* _google_protobuf_MethodOptions_uninterpreted_option_mutable_wpi_upb_array(google_protobuf_MethodOptions* msg, size_t* size, wpi_upb_Arena* arena) {
  const wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(20, 24), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__UninterpretedOption_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(WPI_UPB_UPCAST(msg),
                                                       &field, arena);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}

WPI_UPB_INLINE void google_protobuf_MethodOptions_set_deprecated(google_protobuf_MethodOptions *msg, bool value) {
  const wpi_upb_MiniTableField field = {33, 9, 64, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_MethodOptions_set_idempotency_level(google_protobuf_MethodOptions *msg, int32_t value) {
  const wpi_upb_MiniTableField field = {34, 12, 65, 2, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_MethodOptions_set_features(google_protobuf_MethodOptions *msg, google_protobuf_FeatureSet* value) {
  const wpi_upb_MiniTableField field = {35, 16, 66, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FeatureSet_msg_init);
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE struct google_protobuf_FeatureSet* google_protobuf_MethodOptions_mutable_features(google_protobuf_MethodOptions* msg, wpi_upb_Arena* arena) {
  struct google_protobuf_FeatureSet* sub = (struct google_protobuf_FeatureSet*)google_protobuf_MethodOptions_features(msg);
  if (sub == NULL) {
    sub = (struct google_protobuf_FeatureSet*)_wpi_upb_Message_New(&google__protobuf__FeatureSet_msg_init, arena);
    if (sub) google_protobuf_MethodOptions_set_features(msg, sub);
  }
  return sub;
}
WPI_UPB_INLINE google_protobuf_UninterpretedOption** google_protobuf_MethodOptions_mutable_uninterpreted_option(google_protobuf_MethodOptions* msg, size_t* size) {
  wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(20, 24), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__UninterpretedOption_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetMutableArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (google_protobuf_UninterpretedOption**)wpi_upb_Array_MutableDataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE google_protobuf_UninterpretedOption** google_protobuf_MethodOptions_resize_uninterpreted_option(google_protobuf_MethodOptions* msg, size_t size, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(20, 24), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return (google_protobuf_UninterpretedOption**)wpi_upb_Message_ResizeArrayUninitialized(WPI_UPB_UPCAST(msg),
                                                   &field, size, arena);
}
WPI_UPB_INLINE struct google_protobuf_UninterpretedOption* google_protobuf_MethodOptions_add_uninterpreted_option(google_protobuf_MethodOptions* msg, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {999, WPI_UPB_SIZE(20, 24), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__UninterpretedOption_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(
      WPI_UPB_UPCAST(msg), &field, arena);
  if (!arr || !WPI_UPB_PRIVATE(_wpi_upb_Array_ResizeUninitialized)(
                  arr, arr->WPI_UPB_PRIVATE(size) + 1, arena)) {
    return NULL;
  }
  struct google_protobuf_UninterpretedOption* sub = (struct google_protobuf_UninterpretedOption*)_wpi_upb_Message_New(&google__protobuf__UninterpretedOption_msg_init, arena);
  if (!arr || !sub) return NULL;
  WPI_UPB_PRIVATE(_wpi_upb_Array_Set)
  (arr, arr->WPI_UPB_PRIVATE(size) - 1, &sub, sizeof(sub));
  return sub;
}

/* google.protobuf.UninterpretedOption */

WPI_UPB_INLINE google_protobuf_UninterpretedOption* google_protobuf_UninterpretedOption_new(wpi_upb_Arena* arena) {
  return (google_protobuf_UninterpretedOption*)_wpi_upb_Message_New(&google__protobuf__UninterpretedOption_msg_init, arena);
}
WPI_UPB_INLINE google_protobuf_UninterpretedOption* google_protobuf_UninterpretedOption_parse(const char* buf, size_t size, wpi_upb_Arena* arena) {
  google_protobuf_UninterpretedOption* ret = google_protobuf_UninterpretedOption_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__UninterpretedOption_msg_init, NULL, 0, arena) !=
      kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE google_protobuf_UninterpretedOption* google_protobuf_UninterpretedOption_parse_ex(const char* buf, size_t size,
                           const wpi_upb_ExtensionRegistry* extreg,
                           int options, wpi_upb_Arena* arena) {
  google_protobuf_UninterpretedOption* ret = google_protobuf_UninterpretedOption_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__UninterpretedOption_msg_init, extreg, options,
                 arena) != kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE char* google_protobuf_UninterpretedOption_serialize(const google_protobuf_UninterpretedOption* msg, wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__UninterpretedOption_msg_init, 0, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE char* google_protobuf_UninterpretedOption_serialize_ex(const google_protobuf_UninterpretedOption* msg, int options,
                                 wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__UninterpretedOption_msg_init, options, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE void google_protobuf_UninterpretedOption_clear_name(google_protobuf_UninterpretedOption* msg) {
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(12, 16), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_UninterpretedOption_NamePart* const* google_protobuf_UninterpretedOption_name(const google_protobuf_UninterpretedOption* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(12, 16), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__UninterpretedOption__NamePart_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (const google_protobuf_UninterpretedOption_NamePart* const*)wpi_upb_Array_DataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE const wpi_upb_Array* _google_protobuf_UninterpretedOption_name_wpi_upb_array(const google_protobuf_UninterpretedOption* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(12, 16), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__UninterpretedOption__NamePart_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE wpi_upb_Array* _google_protobuf_UninterpretedOption_name_mutable_wpi_upb_array(google_protobuf_UninterpretedOption* msg, size_t* size, wpi_upb_Arena* arena) {
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(12, 16), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__UninterpretedOption__NamePart_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(WPI_UPB_UPCAST(msg),
                                                       &field, arena);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE void google_protobuf_UninterpretedOption_clear_identifier_value(google_protobuf_UninterpretedOption* msg) {
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(16, 24), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE wpi_upb_StringView google_protobuf_UninterpretedOption_identifier_value(const google_protobuf_UninterpretedOption* msg) {
  wpi_upb_StringView default_val = wpi_upb_StringView_FromString("");
  wpi_upb_StringView ret;
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(16, 24), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_UninterpretedOption_has_identifier_value(const google_protobuf_UninterpretedOption* msg) {
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(16, 24), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_UninterpretedOption_clear_positive_int_value(google_protobuf_UninterpretedOption* msg) {
  const wpi_upb_MiniTableField field = {4, WPI_UPB_SIZE(24, 40), 65, kUpb_NoSub, 4, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_8Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE uint64_t google_protobuf_UninterpretedOption_positive_int_value(const google_protobuf_UninterpretedOption* msg) {
  uint64_t default_val = (uint64_t)0ull;
  uint64_t ret;
  const wpi_upb_MiniTableField field = {4, WPI_UPB_SIZE(24, 40), 65, kUpb_NoSub, 4, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_8Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_UninterpretedOption_has_positive_int_value(const google_protobuf_UninterpretedOption* msg) {
  const wpi_upb_MiniTableField field = {4, WPI_UPB_SIZE(24, 40), 65, kUpb_NoSub, 4, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_8Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_UninterpretedOption_clear_negative_int_value(google_protobuf_UninterpretedOption* msg) {
  const wpi_upb_MiniTableField field = {5, WPI_UPB_SIZE(32, 48), 66, kUpb_NoSub, 3, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_8Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE int64_t google_protobuf_UninterpretedOption_negative_int_value(const google_protobuf_UninterpretedOption* msg) {
  int64_t default_val = (int64_t)0ll;
  int64_t ret;
  const wpi_upb_MiniTableField field = {5, WPI_UPB_SIZE(32, 48), 66, kUpb_NoSub, 3, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_8Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_UninterpretedOption_has_negative_int_value(const google_protobuf_UninterpretedOption* msg) {
  const wpi_upb_MiniTableField field = {5, WPI_UPB_SIZE(32, 48), 66, kUpb_NoSub, 3, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_8Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_UninterpretedOption_clear_double_value(google_protobuf_UninterpretedOption* msg) {
  const wpi_upb_MiniTableField field = {6, WPI_UPB_SIZE(40, 56), 67, kUpb_NoSub, 1, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_8Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE double google_protobuf_UninterpretedOption_double_value(const google_protobuf_UninterpretedOption* msg) {
  double default_val = 0;
  double ret;
  const wpi_upb_MiniTableField field = {6, WPI_UPB_SIZE(40, 56), 67, kUpb_NoSub, 1, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_8Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_UninterpretedOption_has_double_value(const google_protobuf_UninterpretedOption* msg) {
  const wpi_upb_MiniTableField field = {6, WPI_UPB_SIZE(40, 56), 67, kUpb_NoSub, 1, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_8Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_UninterpretedOption_clear_string_value(google_protobuf_UninterpretedOption* msg) {
  const wpi_upb_MiniTableField field = {7, WPI_UPB_SIZE(48, 64), 68, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE wpi_upb_StringView google_protobuf_UninterpretedOption_string_value(const google_protobuf_UninterpretedOption* msg) {
  wpi_upb_StringView default_val = wpi_upb_StringView_FromString("");
  wpi_upb_StringView ret;
  const wpi_upb_MiniTableField field = {7, WPI_UPB_SIZE(48, 64), 68, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_UninterpretedOption_has_string_value(const google_protobuf_UninterpretedOption* msg) {
  const wpi_upb_MiniTableField field = {7, WPI_UPB_SIZE(48, 64), 68, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_UninterpretedOption_clear_aggregate_value(google_protobuf_UninterpretedOption* msg) {
  const wpi_upb_MiniTableField field = {8, WPI_UPB_SIZE(56, 80), 69, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE wpi_upb_StringView google_protobuf_UninterpretedOption_aggregate_value(const google_protobuf_UninterpretedOption* msg) {
  wpi_upb_StringView default_val = wpi_upb_StringView_FromString("");
  wpi_upb_StringView ret;
  const wpi_upb_MiniTableField field = {8, WPI_UPB_SIZE(56, 80), 69, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_UninterpretedOption_has_aggregate_value(const google_protobuf_UninterpretedOption* msg) {
  const wpi_upb_MiniTableField field = {8, WPI_UPB_SIZE(56, 80), 69, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}

WPI_UPB_INLINE google_protobuf_UninterpretedOption_NamePart** google_protobuf_UninterpretedOption_mutable_name(google_protobuf_UninterpretedOption* msg, size_t* size) {
  wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(12, 16), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__UninterpretedOption__NamePart_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetMutableArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (google_protobuf_UninterpretedOption_NamePart**)wpi_upb_Array_MutableDataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE google_protobuf_UninterpretedOption_NamePart** google_protobuf_UninterpretedOption_resize_name(google_protobuf_UninterpretedOption* msg, size_t size, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(12, 16), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return (google_protobuf_UninterpretedOption_NamePart**)wpi_upb_Message_ResizeArrayUninitialized(WPI_UPB_UPCAST(msg),
                                                   &field, size, arena);
}
WPI_UPB_INLINE struct google_protobuf_UninterpretedOption_NamePart* google_protobuf_UninterpretedOption_add_name(google_protobuf_UninterpretedOption* msg, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(12, 16), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__UninterpretedOption__NamePart_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(
      WPI_UPB_UPCAST(msg), &field, arena);
  if (!arr || !WPI_UPB_PRIVATE(_wpi_upb_Array_ResizeUninitialized)(
                  arr, arr->WPI_UPB_PRIVATE(size) + 1, arena)) {
    return NULL;
  }
  struct google_protobuf_UninterpretedOption_NamePart* sub = (struct google_protobuf_UninterpretedOption_NamePart*)_wpi_upb_Message_New(&google__protobuf__UninterpretedOption__NamePart_msg_init, arena);
  if (!arr || !sub) return NULL;
  WPI_UPB_PRIVATE(_wpi_upb_Array_Set)
  (arr, arr->WPI_UPB_PRIVATE(size) - 1, &sub, sizeof(sub));
  return sub;
}
WPI_UPB_INLINE void google_protobuf_UninterpretedOption_set_identifier_value(google_protobuf_UninterpretedOption *msg, wpi_upb_StringView value) {
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(16, 24), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_UninterpretedOption_set_positive_int_value(google_protobuf_UninterpretedOption *msg, uint64_t value) {
  const wpi_upb_MiniTableField field = {4, WPI_UPB_SIZE(24, 40), 65, kUpb_NoSub, 4, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_8Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_UninterpretedOption_set_negative_int_value(google_protobuf_UninterpretedOption *msg, int64_t value) {
  const wpi_upb_MiniTableField field = {5, WPI_UPB_SIZE(32, 48), 66, kUpb_NoSub, 3, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_8Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_UninterpretedOption_set_double_value(google_protobuf_UninterpretedOption *msg, double value) {
  const wpi_upb_MiniTableField field = {6, WPI_UPB_SIZE(40, 56), 67, kUpb_NoSub, 1, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_8Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_UninterpretedOption_set_string_value(google_protobuf_UninterpretedOption *msg, wpi_upb_StringView value) {
  const wpi_upb_MiniTableField field = {7, WPI_UPB_SIZE(48, 64), 68, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_UninterpretedOption_set_aggregate_value(google_protobuf_UninterpretedOption *msg, wpi_upb_StringView value) {
  const wpi_upb_MiniTableField field = {8, WPI_UPB_SIZE(56, 80), 69, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}

/* google.protobuf.UninterpretedOption.NamePart */

WPI_UPB_INLINE google_protobuf_UninterpretedOption_NamePart* google_protobuf_UninterpretedOption_NamePart_new(wpi_upb_Arena* arena) {
  return (google_protobuf_UninterpretedOption_NamePart*)_wpi_upb_Message_New(&google__protobuf__UninterpretedOption__NamePart_msg_init, arena);
}
WPI_UPB_INLINE google_protobuf_UninterpretedOption_NamePart* google_protobuf_UninterpretedOption_NamePart_parse(const char* buf, size_t size, wpi_upb_Arena* arena) {
  google_protobuf_UninterpretedOption_NamePart* ret = google_protobuf_UninterpretedOption_NamePart_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__UninterpretedOption__NamePart_msg_init, NULL, 0, arena) !=
      kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE google_protobuf_UninterpretedOption_NamePart* google_protobuf_UninterpretedOption_NamePart_parse_ex(const char* buf, size_t size,
                           const wpi_upb_ExtensionRegistry* extreg,
                           int options, wpi_upb_Arena* arena) {
  google_protobuf_UninterpretedOption_NamePart* ret = google_protobuf_UninterpretedOption_NamePart_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__UninterpretedOption__NamePart_msg_init, extreg, options,
                 arena) != kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE char* google_protobuf_UninterpretedOption_NamePart_serialize(const google_protobuf_UninterpretedOption_NamePart* msg, wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__UninterpretedOption__NamePart_msg_init, 0, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE char* google_protobuf_UninterpretedOption_NamePart_serialize_ex(const google_protobuf_UninterpretedOption_NamePart* msg, int options,
                                 wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__UninterpretedOption__NamePart_msg_init, options, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE void google_protobuf_UninterpretedOption_NamePart_clear_name_part(google_protobuf_UninterpretedOption_NamePart* msg) {
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(12, 16), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE wpi_upb_StringView google_protobuf_UninterpretedOption_NamePart_name_part(const google_protobuf_UninterpretedOption_NamePart* msg) {
  wpi_upb_StringView default_val = wpi_upb_StringView_FromString("");
  wpi_upb_StringView ret;
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(12, 16), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_UninterpretedOption_NamePart_has_name_part(const google_protobuf_UninterpretedOption_NamePart* msg) {
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(12, 16), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_UninterpretedOption_NamePart_clear_is_extension(google_protobuf_UninterpretedOption_NamePart* msg) {
  const wpi_upb_MiniTableField field = {2, 9, 65, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE bool google_protobuf_UninterpretedOption_NamePart_is_extension(const google_protobuf_UninterpretedOption_NamePart* msg) {
  bool default_val = false;
  bool ret;
  const wpi_upb_MiniTableField field = {2, 9, 65, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_UninterpretedOption_NamePart_has_is_extension(const google_protobuf_UninterpretedOption_NamePart* msg) {
  const wpi_upb_MiniTableField field = {2, 9, 65, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}

WPI_UPB_INLINE void google_protobuf_UninterpretedOption_NamePart_set_name_part(google_protobuf_UninterpretedOption_NamePart *msg, wpi_upb_StringView value) {
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(12, 16), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_UninterpretedOption_NamePart_set_is_extension(google_protobuf_UninterpretedOption_NamePart *msg, bool value) {
  const wpi_upb_MiniTableField field = {2, 9, 65, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}

/* google.protobuf.FeatureSet */

WPI_UPB_INLINE google_protobuf_FeatureSet* google_protobuf_FeatureSet_new(wpi_upb_Arena* arena) {
  return (google_protobuf_FeatureSet*)_wpi_upb_Message_New(&google__protobuf__FeatureSet_msg_init, arena);
}
WPI_UPB_INLINE google_protobuf_FeatureSet* google_protobuf_FeatureSet_parse(const char* buf, size_t size, wpi_upb_Arena* arena) {
  google_protobuf_FeatureSet* ret = google_protobuf_FeatureSet_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__FeatureSet_msg_init, NULL, 0, arena) !=
      kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE google_protobuf_FeatureSet* google_protobuf_FeatureSet_parse_ex(const char* buf, size_t size,
                           const wpi_upb_ExtensionRegistry* extreg,
                           int options, wpi_upb_Arena* arena) {
  google_protobuf_FeatureSet* ret = google_protobuf_FeatureSet_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__FeatureSet_msg_init, extreg, options,
                 arena) != kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE char* google_protobuf_FeatureSet_serialize(const google_protobuf_FeatureSet* msg, wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__FeatureSet_msg_init, 0, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE char* google_protobuf_FeatureSet_serialize_ex(const google_protobuf_FeatureSet* msg, int options,
                                 wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__FeatureSet_msg_init, options, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE void google_protobuf_FeatureSet_clear_field_presence(google_protobuf_FeatureSet* msg) {
  const wpi_upb_MiniTableField field = {1, 12, 64, 0, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE int32_t google_protobuf_FeatureSet_field_presence(const google_protobuf_FeatureSet* msg) {
  int32_t default_val = 0;
  int32_t ret;
  const wpi_upb_MiniTableField field = {1, 12, 64, 0, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FeatureSet_has_field_presence(const google_protobuf_FeatureSet* msg) {
  const wpi_upb_MiniTableField field = {1, 12, 64, 0, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FeatureSet_clear_enum_type(google_protobuf_FeatureSet* msg) {
  const wpi_upb_MiniTableField field = {2, 16, 65, 1, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE int32_t google_protobuf_FeatureSet_enum_type(const google_protobuf_FeatureSet* msg) {
  int32_t default_val = 0;
  int32_t ret;
  const wpi_upb_MiniTableField field = {2, 16, 65, 1, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FeatureSet_has_enum_type(const google_protobuf_FeatureSet* msg) {
  const wpi_upb_MiniTableField field = {2, 16, 65, 1, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FeatureSet_clear_repeated_field_encoding(google_protobuf_FeatureSet* msg) {
  const wpi_upb_MiniTableField field = {3, 20, 66, 2, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE int32_t google_protobuf_FeatureSet_repeated_field_encoding(const google_protobuf_FeatureSet* msg) {
  int32_t default_val = 0;
  int32_t ret;
  const wpi_upb_MiniTableField field = {3, 20, 66, 2, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FeatureSet_has_repeated_field_encoding(const google_protobuf_FeatureSet* msg) {
  const wpi_upb_MiniTableField field = {3, 20, 66, 2, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FeatureSet_clear_utf8_validation(google_protobuf_FeatureSet* msg) {
  const wpi_upb_MiniTableField field = {4, 24, 67, 3, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE int32_t google_protobuf_FeatureSet_utf8_validation(const google_protobuf_FeatureSet* msg) {
  int32_t default_val = 0;
  int32_t ret;
  const wpi_upb_MiniTableField field = {4, 24, 67, 3, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FeatureSet_has_utf8_validation(const google_protobuf_FeatureSet* msg) {
  const wpi_upb_MiniTableField field = {4, 24, 67, 3, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FeatureSet_clear_message_encoding(google_protobuf_FeatureSet* msg) {
  const wpi_upb_MiniTableField field = {5, 28, 68, 4, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE int32_t google_protobuf_FeatureSet_message_encoding(const google_protobuf_FeatureSet* msg) {
  int32_t default_val = 0;
  int32_t ret;
  const wpi_upb_MiniTableField field = {5, 28, 68, 4, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FeatureSet_has_message_encoding(const google_protobuf_FeatureSet* msg) {
  const wpi_upb_MiniTableField field = {5, 28, 68, 4, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FeatureSet_clear_json_format(google_protobuf_FeatureSet* msg) {
  const wpi_upb_MiniTableField field = {6, 32, 69, 5, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE int32_t google_protobuf_FeatureSet_json_format(const google_protobuf_FeatureSet* msg) {
  int32_t default_val = 0;
  int32_t ret;
  const wpi_upb_MiniTableField field = {6, 32, 69, 5, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FeatureSet_has_json_format(const google_protobuf_FeatureSet* msg) {
  const wpi_upb_MiniTableField field = {6, 32, 69, 5, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}

WPI_UPB_INLINE void google_protobuf_FeatureSet_set_field_presence(google_protobuf_FeatureSet *msg, int32_t value) {
  const wpi_upb_MiniTableField field = {1, 12, 64, 0, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_FeatureSet_set_enum_type(google_protobuf_FeatureSet *msg, int32_t value) {
  const wpi_upb_MiniTableField field = {2, 16, 65, 1, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_FeatureSet_set_repeated_field_encoding(google_protobuf_FeatureSet *msg, int32_t value) {
  const wpi_upb_MiniTableField field = {3, 20, 66, 2, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_FeatureSet_set_utf8_validation(google_protobuf_FeatureSet *msg, int32_t value) {
  const wpi_upb_MiniTableField field = {4, 24, 67, 3, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_FeatureSet_set_message_encoding(google_protobuf_FeatureSet *msg, int32_t value) {
  const wpi_upb_MiniTableField field = {5, 28, 68, 4, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_FeatureSet_set_json_format(google_protobuf_FeatureSet *msg, int32_t value) {
  const wpi_upb_MiniTableField field = {6, 32, 69, 5, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}

/* google.protobuf.FeatureSetDefaults */

WPI_UPB_INLINE google_protobuf_FeatureSetDefaults* google_protobuf_FeatureSetDefaults_new(wpi_upb_Arena* arena) {
  return (google_protobuf_FeatureSetDefaults*)_wpi_upb_Message_New(&google__protobuf__FeatureSetDefaults_msg_init, arena);
}
WPI_UPB_INLINE google_protobuf_FeatureSetDefaults* google_protobuf_FeatureSetDefaults_parse(const char* buf, size_t size, wpi_upb_Arena* arena) {
  google_protobuf_FeatureSetDefaults* ret = google_protobuf_FeatureSetDefaults_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__FeatureSetDefaults_msg_init, NULL, 0, arena) !=
      kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE google_protobuf_FeatureSetDefaults* google_protobuf_FeatureSetDefaults_parse_ex(const char* buf, size_t size,
                           const wpi_upb_ExtensionRegistry* extreg,
                           int options, wpi_upb_Arena* arena) {
  google_protobuf_FeatureSetDefaults* ret = google_protobuf_FeatureSetDefaults_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__FeatureSetDefaults_msg_init, extreg, options,
                 arena) != kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE char* google_protobuf_FeatureSetDefaults_serialize(const google_protobuf_FeatureSetDefaults* msg, wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__FeatureSetDefaults_msg_init, 0, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE char* google_protobuf_FeatureSetDefaults_serialize_ex(const google_protobuf_FeatureSetDefaults* msg, int options,
                                 wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__FeatureSetDefaults_msg_init, options, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE void google_protobuf_FeatureSetDefaults_clear_defaults(google_protobuf_FeatureSetDefaults* msg) {
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(12, 24), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault* const* google_protobuf_FeatureSetDefaults_defaults(const google_protobuf_FeatureSetDefaults* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(12, 24), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FeatureSetDefaults__FeatureSetEditionDefault_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (const google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault* const*)wpi_upb_Array_DataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE const wpi_upb_Array* _google_protobuf_FeatureSetDefaults_defaults_wpi_upb_array(const google_protobuf_FeatureSetDefaults* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(12, 24), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FeatureSetDefaults__FeatureSetEditionDefault_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE wpi_upb_Array* _google_protobuf_FeatureSetDefaults_defaults_mutable_wpi_upb_array(google_protobuf_FeatureSetDefaults* msg, size_t* size, wpi_upb_Arena* arena) {
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(12, 24), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FeatureSetDefaults__FeatureSetEditionDefault_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(WPI_UPB_UPCAST(msg),
                                                       &field, arena);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE void google_protobuf_FeatureSetDefaults_clear_minimum_edition(google_protobuf_FeatureSetDefaults* msg) {
  const wpi_upb_MiniTableField field = {4, WPI_UPB_SIZE(16, 12), 64, 1, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE int32_t google_protobuf_FeatureSetDefaults_minimum_edition(const google_protobuf_FeatureSetDefaults* msg) {
  int32_t default_val = 0;
  int32_t ret;
  const wpi_upb_MiniTableField field = {4, WPI_UPB_SIZE(16, 12), 64, 1, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FeatureSetDefaults_has_minimum_edition(const google_protobuf_FeatureSetDefaults* msg) {
  const wpi_upb_MiniTableField field = {4, WPI_UPB_SIZE(16, 12), 64, 1, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FeatureSetDefaults_clear_maximum_edition(google_protobuf_FeatureSetDefaults* msg) {
  const wpi_upb_MiniTableField field = {5, WPI_UPB_SIZE(20, 16), 65, 2, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE int32_t google_protobuf_FeatureSetDefaults_maximum_edition(const google_protobuf_FeatureSetDefaults* msg) {
  int32_t default_val = 0;
  int32_t ret;
  const wpi_upb_MiniTableField field = {5, WPI_UPB_SIZE(20, 16), 65, 2, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FeatureSetDefaults_has_maximum_edition(const google_protobuf_FeatureSetDefaults* msg) {
  const wpi_upb_MiniTableField field = {5, WPI_UPB_SIZE(20, 16), 65, 2, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}

WPI_UPB_INLINE google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault** google_protobuf_FeatureSetDefaults_mutable_defaults(google_protobuf_FeatureSetDefaults* msg, size_t* size) {
  wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(12, 24), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FeatureSetDefaults__FeatureSetEditionDefault_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetMutableArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault**)wpi_upb_Array_MutableDataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault** google_protobuf_FeatureSetDefaults_resize_defaults(google_protobuf_FeatureSetDefaults* msg, size_t size, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(12, 24), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return (google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault**)wpi_upb_Message_ResizeArrayUninitialized(WPI_UPB_UPCAST(msg),
                                                   &field, size, arena);
}
WPI_UPB_INLINE struct google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault* google_protobuf_FeatureSetDefaults_add_defaults(google_protobuf_FeatureSetDefaults* msg, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(12, 24), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FeatureSetDefaults__FeatureSetEditionDefault_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(
      WPI_UPB_UPCAST(msg), &field, arena);
  if (!arr || !WPI_UPB_PRIVATE(_wpi_upb_Array_ResizeUninitialized)(
                  arr, arr->WPI_UPB_PRIVATE(size) + 1, arena)) {
    return NULL;
  }
  struct google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault* sub = (struct google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault*)_wpi_upb_Message_New(&google__protobuf__FeatureSetDefaults__FeatureSetEditionDefault_msg_init, arena);
  if (!arr || !sub) return NULL;
  WPI_UPB_PRIVATE(_wpi_upb_Array_Set)
  (arr, arr->WPI_UPB_PRIVATE(size) - 1, &sub, sizeof(sub));
  return sub;
}
WPI_UPB_INLINE void google_protobuf_FeatureSetDefaults_set_minimum_edition(google_protobuf_FeatureSetDefaults *msg, int32_t value) {
  const wpi_upb_MiniTableField field = {4, WPI_UPB_SIZE(16, 12), 64, 1, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_FeatureSetDefaults_set_maximum_edition(google_protobuf_FeatureSetDefaults *msg, int32_t value) {
  const wpi_upb_MiniTableField field = {5, WPI_UPB_SIZE(20, 16), 65, 2, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}

/* google.protobuf.FeatureSetDefaults.FeatureSetEditionDefault */

WPI_UPB_INLINE google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault* google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault_new(wpi_upb_Arena* arena) {
  return (google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault*)_wpi_upb_Message_New(&google__protobuf__FeatureSetDefaults__FeatureSetEditionDefault_msg_init, arena);
}
WPI_UPB_INLINE google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault* google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault_parse(const char* buf, size_t size, wpi_upb_Arena* arena) {
  google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault* ret = google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__FeatureSetDefaults__FeatureSetEditionDefault_msg_init, NULL, 0, arena) !=
      kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault* google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault_parse_ex(const char* buf, size_t size,
                           const wpi_upb_ExtensionRegistry* extreg,
                           int options, wpi_upb_Arena* arena) {
  google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault* ret = google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__FeatureSetDefaults__FeatureSetEditionDefault_msg_init, extreg, options,
                 arena) != kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE char* google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault_serialize(const google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault* msg, wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__FeatureSetDefaults__FeatureSetEditionDefault_msg_init, 0, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE char* google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault_serialize_ex(const google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault* msg, int options,
                                 wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__FeatureSetDefaults__FeatureSetEditionDefault_msg_init, options, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE void google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault_clear_edition(google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault* msg) {
  const wpi_upb_MiniTableField field = {3, 12, 64, 2, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE int32_t google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault_edition(const google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault* msg) {
  int32_t default_val = 0;
  int32_t ret;
  const wpi_upb_MiniTableField field = {3, 12, 64, 2, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault_has_edition(const google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault* msg) {
  const wpi_upb_MiniTableField field = {3, 12, 64, 2, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault_clear_overridable_features(google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault* msg) {
  const wpi_upb_MiniTableField field = {4, 16, 65, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_FeatureSet* google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault_overridable_features(const google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault* msg) {
  const google_protobuf_FeatureSet* default_val = NULL;
  const google_protobuf_FeatureSet* ret;
  const wpi_upb_MiniTableField field = {4, 16, 65, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FeatureSet_msg_init);
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault_has_overridable_features(const google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault* msg) {
  const wpi_upb_MiniTableField field = {4, 16, 65, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault_clear_fixed_features(google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault* msg) {
  const wpi_upb_MiniTableField field = {5, WPI_UPB_SIZE(20, 24), 66, 1, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_FeatureSet* google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault_fixed_features(const google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault* msg) {
  const google_protobuf_FeatureSet* default_val = NULL;
  const google_protobuf_FeatureSet* ret;
  const wpi_upb_MiniTableField field = {5, WPI_UPB_SIZE(20, 24), 66, 1, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FeatureSet_msg_init);
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault_has_fixed_features(const google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault* msg) {
  const wpi_upb_MiniTableField field = {5, WPI_UPB_SIZE(20, 24), 66, 1, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}

WPI_UPB_INLINE void google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault_set_edition(google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault *msg, int32_t value) {
  const wpi_upb_MiniTableField field = {3, 12, 64, 2, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault_set_overridable_features(google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault *msg, google_protobuf_FeatureSet* value) {
  const wpi_upb_MiniTableField field = {4, 16, 65, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FeatureSet_msg_init);
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE struct google_protobuf_FeatureSet* google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault_mutable_overridable_features(google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault* msg, wpi_upb_Arena* arena) {
  struct google_protobuf_FeatureSet* sub = (struct google_protobuf_FeatureSet*)google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault_overridable_features(msg);
  if (sub == NULL) {
    sub = (struct google_protobuf_FeatureSet*)_wpi_upb_Message_New(&google__protobuf__FeatureSet_msg_init, arena);
    if (sub) google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault_set_overridable_features(msg, sub);
  }
  return sub;
}
WPI_UPB_INLINE void google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault_set_fixed_features(google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault *msg, google_protobuf_FeatureSet* value) {
  const wpi_upb_MiniTableField field = {5, WPI_UPB_SIZE(20, 24), 66, 1, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__FeatureSet_msg_init);
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE struct google_protobuf_FeatureSet* google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault_mutable_fixed_features(google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault* msg, wpi_upb_Arena* arena) {
  struct google_protobuf_FeatureSet* sub = (struct google_protobuf_FeatureSet*)google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault_fixed_features(msg);
  if (sub == NULL) {
    sub = (struct google_protobuf_FeatureSet*)_wpi_upb_Message_New(&google__protobuf__FeatureSet_msg_init, arena);
    if (sub) google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault_set_fixed_features(msg, sub);
  }
  return sub;
}

/* google.protobuf.SourceCodeInfo */

WPI_UPB_INLINE google_protobuf_SourceCodeInfo* google_protobuf_SourceCodeInfo_new(wpi_upb_Arena* arena) {
  return (google_protobuf_SourceCodeInfo*)_wpi_upb_Message_New(&google__protobuf__SourceCodeInfo_msg_init, arena);
}
WPI_UPB_INLINE google_protobuf_SourceCodeInfo* google_protobuf_SourceCodeInfo_parse(const char* buf, size_t size, wpi_upb_Arena* arena) {
  google_protobuf_SourceCodeInfo* ret = google_protobuf_SourceCodeInfo_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__SourceCodeInfo_msg_init, NULL, 0, arena) !=
      kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE google_protobuf_SourceCodeInfo* google_protobuf_SourceCodeInfo_parse_ex(const char* buf, size_t size,
                           const wpi_upb_ExtensionRegistry* extreg,
                           int options, wpi_upb_Arena* arena) {
  google_protobuf_SourceCodeInfo* ret = google_protobuf_SourceCodeInfo_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__SourceCodeInfo_msg_init, extreg, options,
                 arena) != kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE char* google_protobuf_SourceCodeInfo_serialize(const google_protobuf_SourceCodeInfo* msg, wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__SourceCodeInfo_msg_init, 0, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE char* google_protobuf_SourceCodeInfo_serialize_ex(const google_protobuf_SourceCodeInfo* msg, int options,
                                 wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__SourceCodeInfo_msg_init, options, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE void google_protobuf_SourceCodeInfo_clear_location(google_protobuf_SourceCodeInfo* msg) {
  const wpi_upb_MiniTableField field = {1, 8, 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_SourceCodeInfo_Location* const* google_protobuf_SourceCodeInfo_location(const google_protobuf_SourceCodeInfo* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {1, 8, 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__SourceCodeInfo__Location_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (const google_protobuf_SourceCodeInfo_Location* const*)wpi_upb_Array_DataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE const wpi_upb_Array* _google_protobuf_SourceCodeInfo_location_wpi_upb_array(const google_protobuf_SourceCodeInfo* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {1, 8, 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__SourceCodeInfo__Location_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE wpi_upb_Array* _google_protobuf_SourceCodeInfo_location_mutable_wpi_upb_array(google_protobuf_SourceCodeInfo* msg, size_t* size, wpi_upb_Arena* arena) {
  const wpi_upb_MiniTableField field = {1, 8, 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__SourceCodeInfo__Location_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(WPI_UPB_UPCAST(msg),
                                                       &field, arena);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}

WPI_UPB_INLINE google_protobuf_SourceCodeInfo_Location** google_protobuf_SourceCodeInfo_mutable_location(google_protobuf_SourceCodeInfo* msg, size_t* size) {
  wpi_upb_MiniTableField field = {1, 8, 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__SourceCodeInfo__Location_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetMutableArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (google_protobuf_SourceCodeInfo_Location**)wpi_upb_Array_MutableDataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE google_protobuf_SourceCodeInfo_Location** google_protobuf_SourceCodeInfo_resize_location(google_protobuf_SourceCodeInfo* msg, size_t size, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {1, 8, 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return (google_protobuf_SourceCodeInfo_Location**)wpi_upb_Message_ResizeArrayUninitialized(WPI_UPB_UPCAST(msg),
                                                   &field, size, arena);
}
WPI_UPB_INLINE struct google_protobuf_SourceCodeInfo_Location* google_protobuf_SourceCodeInfo_add_location(google_protobuf_SourceCodeInfo* msg, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {1, 8, 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__SourceCodeInfo__Location_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(
      WPI_UPB_UPCAST(msg), &field, arena);
  if (!arr || !WPI_UPB_PRIVATE(_wpi_upb_Array_ResizeUninitialized)(
                  arr, arr->WPI_UPB_PRIVATE(size) + 1, arena)) {
    return NULL;
  }
  struct google_protobuf_SourceCodeInfo_Location* sub = (struct google_protobuf_SourceCodeInfo_Location*)_wpi_upb_Message_New(&google__protobuf__SourceCodeInfo__Location_msg_init, arena);
  if (!arr || !sub) return NULL;
  WPI_UPB_PRIVATE(_wpi_upb_Array_Set)
  (arr, arr->WPI_UPB_PRIVATE(size) - 1, &sub, sizeof(sub));
  return sub;
}

/* google.protobuf.SourceCodeInfo.Location */

WPI_UPB_INLINE google_protobuf_SourceCodeInfo_Location* google_protobuf_SourceCodeInfo_Location_new(wpi_upb_Arena* arena) {
  return (google_protobuf_SourceCodeInfo_Location*)_wpi_upb_Message_New(&google__protobuf__SourceCodeInfo__Location_msg_init, arena);
}
WPI_UPB_INLINE google_protobuf_SourceCodeInfo_Location* google_protobuf_SourceCodeInfo_Location_parse(const char* buf, size_t size, wpi_upb_Arena* arena) {
  google_protobuf_SourceCodeInfo_Location* ret = google_protobuf_SourceCodeInfo_Location_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__SourceCodeInfo__Location_msg_init, NULL, 0, arena) !=
      kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE google_protobuf_SourceCodeInfo_Location* google_protobuf_SourceCodeInfo_Location_parse_ex(const char* buf, size_t size,
                           const wpi_upb_ExtensionRegistry* extreg,
                           int options, wpi_upb_Arena* arena) {
  google_protobuf_SourceCodeInfo_Location* ret = google_protobuf_SourceCodeInfo_Location_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__SourceCodeInfo__Location_msg_init, extreg, options,
                 arena) != kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE char* google_protobuf_SourceCodeInfo_Location_serialize(const google_protobuf_SourceCodeInfo_Location* msg, wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__SourceCodeInfo__Location_msg_init, 0, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE char* google_protobuf_SourceCodeInfo_Location_serialize_ex(const google_protobuf_SourceCodeInfo_Location* msg, int options,
                                 wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__SourceCodeInfo__Location_msg_init, options, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE void google_protobuf_SourceCodeInfo_Location_clear_path(google_protobuf_SourceCodeInfo_Location* msg) {
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(12, 16), 0, kUpb_NoSub, 5, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsPacked | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE int32_t const* google_protobuf_SourceCodeInfo_Location_path(const google_protobuf_SourceCodeInfo_Location* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(12, 16), 0, kUpb_NoSub, 5, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsPacked | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (int32_t const*)wpi_upb_Array_DataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE const wpi_upb_Array* _google_protobuf_SourceCodeInfo_Location_path_wpi_upb_array(const google_protobuf_SourceCodeInfo_Location* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(12, 16), 0, kUpb_NoSub, 5, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsPacked | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE wpi_upb_Array* _google_protobuf_SourceCodeInfo_Location_path_mutable_wpi_upb_array(google_protobuf_SourceCodeInfo_Location* msg, size_t* size, wpi_upb_Arena* arena) {
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(12, 16), 0, kUpb_NoSub, 5, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsPacked | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(WPI_UPB_UPCAST(msg),
                                                       &field, arena);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE void google_protobuf_SourceCodeInfo_Location_clear_span(google_protobuf_SourceCodeInfo_Location* msg) {
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(16, 24), 0, kUpb_NoSub, 5, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsPacked | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE int32_t const* google_protobuf_SourceCodeInfo_Location_span(const google_protobuf_SourceCodeInfo_Location* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(16, 24), 0, kUpb_NoSub, 5, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsPacked | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (int32_t const*)wpi_upb_Array_DataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE const wpi_upb_Array* _google_protobuf_SourceCodeInfo_Location_span_wpi_upb_array(const google_protobuf_SourceCodeInfo_Location* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(16, 24), 0, kUpb_NoSub, 5, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsPacked | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE wpi_upb_Array* _google_protobuf_SourceCodeInfo_Location_span_mutable_wpi_upb_array(google_protobuf_SourceCodeInfo_Location* msg, size_t* size, wpi_upb_Arena* arena) {
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(16, 24), 0, kUpb_NoSub, 5, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsPacked | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(WPI_UPB_UPCAST(msg),
                                                       &field, arena);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE void google_protobuf_SourceCodeInfo_Location_clear_leading_comments(google_protobuf_SourceCodeInfo_Location* msg) {
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(24, 32), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE wpi_upb_StringView google_protobuf_SourceCodeInfo_Location_leading_comments(const google_protobuf_SourceCodeInfo_Location* msg) {
  wpi_upb_StringView default_val = wpi_upb_StringView_FromString("");
  wpi_upb_StringView ret;
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(24, 32), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_SourceCodeInfo_Location_has_leading_comments(const google_protobuf_SourceCodeInfo_Location* msg) {
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(24, 32), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_SourceCodeInfo_Location_clear_trailing_comments(google_protobuf_SourceCodeInfo_Location* msg) {
  const wpi_upb_MiniTableField field = {4, WPI_UPB_SIZE(32, 48), 65, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE wpi_upb_StringView google_protobuf_SourceCodeInfo_Location_trailing_comments(const google_protobuf_SourceCodeInfo_Location* msg) {
  wpi_upb_StringView default_val = wpi_upb_StringView_FromString("");
  wpi_upb_StringView ret;
  const wpi_upb_MiniTableField field = {4, WPI_UPB_SIZE(32, 48), 65, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_SourceCodeInfo_Location_has_trailing_comments(const google_protobuf_SourceCodeInfo_Location* msg) {
  const wpi_upb_MiniTableField field = {4, WPI_UPB_SIZE(32, 48), 65, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_SourceCodeInfo_Location_clear_leading_detached_comments(google_protobuf_SourceCodeInfo_Location* msg) {
  const wpi_upb_MiniTableField field = {6, WPI_UPB_SIZE(20, 64), 0, kUpb_NoSub, 12, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsAlternate | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE wpi_upb_StringView const* google_protobuf_SourceCodeInfo_Location_leading_detached_comments(const google_protobuf_SourceCodeInfo_Location* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {6, WPI_UPB_SIZE(20, 64), 0, kUpb_NoSub, 12, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsAlternate | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (wpi_upb_StringView const*)wpi_upb_Array_DataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE const wpi_upb_Array* _google_protobuf_SourceCodeInfo_Location_leading_detached_comments_wpi_upb_array(const google_protobuf_SourceCodeInfo_Location* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {6, WPI_UPB_SIZE(20, 64), 0, kUpb_NoSub, 12, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsAlternate | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE wpi_upb_Array* _google_protobuf_SourceCodeInfo_Location_leading_detached_comments_mutable_wpi_upb_array(google_protobuf_SourceCodeInfo_Location* msg, size_t* size, wpi_upb_Arena* arena) {
  const wpi_upb_MiniTableField field = {6, WPI_UPB_SIZE(20, 64), 0, kUpb_NoSub, 12, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsAlternate | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(WPI_UPB_UPCAST(msg),
                                                       &field, arena);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}

WPI_UPB_INLINE int32_t* google_protobuf_SourceCodeInfo_Location_mutable_path(google_protobuf_SourceCodeInfo_Location* msg, size_t* size) {
  wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(12, 16), 0, kUpb_NoSub, 5, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsPacked | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Array* arr = wpi_upb_Message_GetMutableArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (int32_t*)wpi_upb_Array_MutableDataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE int32_t* google_protobuf_SourceCodeInfo_Location_resize_path(google_protobuf_SourceCodeInfo_Location* msg, size_t size, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(12, 16), 0, kUpb_NoSub, 5, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsPacked | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return (int32_t*)wpi_upb_Message_ResizeArrayUninitialized(WPI_UPB_UPCAST(msg),
                                                   &field, size, arena);
}
WPI_UPB_INLINE bool google_protobuf_SourceCodeInfo_Location_add_path(google_protobuf_SourceCodeInfo_Location* msg, int32_t val, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(12, 16), 0, kUpb_NoSub, 5, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsPacked | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(
      WPI_UPB_UPCAST(msg), &field, arena);
  if (!arr || !WPI_UPB_PRIVATE(_wpi_upb_Array_ResizeUninitialized)(
                  arr, arr->WPI_UPB_PRIVATE(size) + 1, arena)) {
    return false;
  }
  WPI_UPB_PRIVATE(_wpi_upb_Array_Set)
  (arr, arr->WPI_UPB_PRIVATE(size) - 1, &val, sizeof(val));
  return true;
}
WPI_UPB_INLINE int32_t* google_protobuf_SourceCodeInfo_Location_mutable_span(google_protobuf_SourceCodeInfo_Location* msg, size_t* size) {
  wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(16, 24), 0, kUpb_NoSub, 5, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsPacked | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Array* arr = wpi_upb_Message_GetMutableArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (int32_t*)wpi_upb_Array_MutableDataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE int32_t* google_protobuf_SourceCodeInfo_Location_resize_span(google_protobuf_SourceCodeInfo_Location* msg, size_t size, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(16, 24), 0, kUpb_NoSub, 5, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsPacked | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return (int32_t*)wpi_upb_Message_ResizeArrayUninitialized(WPI_UPB_UPCAST(msg),
                                                   &field, size, arena);
}
WPI_UPB_INLINE bool google_protobuf_SourceCodeInfo_Location_add_span(google_protobuf_SourceCodeInfo_Location* msg, int32_t val, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(16, 24), 0, kUpb_NoSub, 5, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsPacked | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(
      WPI_UPB_UPCAST(msg), &field, arena);
  if (!arr || !WPI_UPB_PRIVATE(_wpi_upb_Array_ResizeUninitialized)(
                  arr, arr->WPI_UPB_PRIVATE(size) + 1, arena)) {
    return false;
  }
  WPI_UPB_PRIVATE(_wpi_upb_Array_Set)
  (arr, arr->WPI_UPB_PRIVATE(size) - 1, &val, sizeof(val));
  return true;
}
WPI_UPB_INLINE void google_protobuf_SourceCodeInfo_Location_set_leading_comments(google_protobuf_SourceCodeInfo_Location *msg, wpi_upb_StringView value) {
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(24, 32), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_SourceCodeInfo_Location_set_trailing_comments(google_protobuf_SourceCodeInfo_Location *msg, wpi_upb_StringView value) {
  const wpi_upb_MiniTableField field = {4, WPI_UPB_SIZE(32, 48), 65, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE wpi_upb_StringView* google_protobuf_SourceCodeInfo_Location_mutable_leading_detached_comments(google_protobuf_SourceCodeInfo_Location* msg, size_t* size) {
  wpi_upb_MiniTableField field = {6, WPI_UPB_SIZE(20, 64), 0, kUpb_NoSub, 12, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsAlternate | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Array* arr = wpi_upb_Message_GetMutableArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (wpi_upb_StringView*)wpi_upb_Array_MutableDataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE wpi_upb_StringView* google_protobuf_SourceCodeInfo_Location_resize_leading_detached_comments(google_protobuf_SourceCodeInfo_Location* msg, size_t size, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {6, WPI_UPB_SIZE(20, 64), 0, kUpb_NoSub, 12, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsAlternate | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return (wpi_upb_StringView*)wpi_upb_Message_ResizeArrayUninitialized(WPI_UPB_UPCAST(msg),
                                                   &field, size, arena);
}
WPI_UPB_INLINE bool google_protobuf_SourceCodeInfo_Location_add_leading_detached_comments(google_protobuf_SourceCodeInfo_Location* msg, wpi_upb_StringView val, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {6, WPI_UPB_SIZE(20, 64), 0, kUpb_NoSub, 12, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsAlternate | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(
      WPI_UPB_UPCAST(msg), &field, arena);
  if (!arr || !WPI_UPB_PRIVATE(_wpi_upb_Array_ResizeUninitialized)(
                  arr, arr->WPI_UPB_PRIVATE(size) + 1, arena)) {
    return false;
  }
  WPI_UPB_PRIVATE(_wpi_upb_Array_Set)
  (arr, arr->WPI_UPB_PRIVATE(size) - 1, &val, sizeof(val));
  return true;
}

/* google.protobuf.GeneratedCodeInfo */

WPI_UPB_INLINE google_protobuf_GeneratedCodeInfo* google_protobuf_GeneratedCodeInfo_new(wpi_upb_Arena* arena) {
  return (google_protobuf_GeneratedCodeInfo*)_wpi_upb_Message_New(&google__protobuf__GeneratedCodeInfo_msg_init, arena);
}
WPI_UPB_INLINE google_protobuf_GeneratedCodeInfo* google_protobuf_GeneratedCodeInfo_parse(const char* buf, size_t size, wpi_upb_Arena* arena) {
  google_protobuf_GeneratedCodeInfo* ret = google_protobuf_GeneratedCodeInfo_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__GeneratedCodeInfo_msg_init, NULL, 0, arena) !=
      kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE google_protobuf_GeneratedCodeInfo* google_protobuf_GeneratedCodeInfo_parse_ex(const char* buf, size_t size,
                           const wpi_upb_ExtensionRegistry* extreg,
                           int options, wpi_upb_Arena* arena) {
  google_protobuf_GeneratedCodeInfo* ret = google_protobuf_GeneratedCodeInfo_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__GeneratedCodeInfo_msg_init, extreg, options,
                 arena) != kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE char* google_protobuf_GeneratedCodeInfo_serialize(const google_protobuf_GeneratedCodeInfo* msg, wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__GeneratedCodeInfo_msg_init, 0, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE char* google_protobuf_GeneratedCodeInfo_serialize_ex(const google_protobuf_GeneratedCodeInfo* msg, int options,
                                 wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__GeneratedCodeInfo_msg_init, options, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE void google_protobuf_GeneratedCodeInfo_clear_annotation(google_protobuf_GeneratedCodeInfo* msg) {
  const wpi_upb_MiniTableField field = {1, 8, 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE const google_protobuf_GeneratedCodeInfo_Annotation* const* google_protobuf_GeneratedCodeInfo_annotation(const google_protobuf_GeneratedCodeInfo* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {1, 8, 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__GeneratedCodeInfo__Annotation_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (const google_protobuf_GeneratedCodeInfo_Annotation* const*)wpi_upb_Array_DataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE const wpi_upb_Array* _google_protobuf_GeneratedCodeInfo_annotation_wpi_upb_array(const google_protobuf_GeneratedCodeInfo* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {1, 8, 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__GeneratedCodeInfo__Annotation_msg_init);
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE wpi_upb_Array* _google_protobuf_GeneratedCodeInfo_annotation_mutable_wpi_upb_array(google_protobuf_GeneratedCodeInfo* msg, size_t* size, wpi_upb_Arena* arena) {
  const wpi_upb_MiniTableField field = {1, 8, 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__GeneratedCodeInfo__Annotation_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(WPI_UPB_UPCAST(msg),
                                                       &field, arena);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}

WPI_UPB_INLINE google_protobuf_GeneratedCodeInfo_Annotation** google_protobuf_GeneratedCodeInfo_mutable_annotation(google_protobuf_GeneratedCodeInfo* msg, size_t* size) {
  wpi_upb_MiniTableField field = {1, 8, 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__GeneratedCodeInfo__Annotation_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetMutableArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (google_protobuf_GeneratedCodeInfo_Annotation**)wpi_upb_Array_MutableDataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE google_protobuf_GeneratedCodeInfo_Annotation** google_protobuf_GeneratedCodeInfo_resize_annotation(google_protobuf_GeneratedCodeInfo* msg, size_t size, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {1, 8, 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return (google_protobuf_GeneratedCodeInfo_Annotation**)wpi_upb_Message_ResizeArrayUninitialized(WPI_UPB_UPCAST(msg),
                                                   &field, size, arena);
}
WPI_UPB_INLINE struct google_protobuf_GeneratedCodeInfo_Annotation* google_protobuf_GeneratedCodeInfo_add_annotation(google_protobuf_GeneratedCodeInfo* msg, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {1, 8, 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  WPI_UPB_PRIVATE(_wpi_upb_MiniTable_StrongReference)(&google__protobuf__GeneratedCodeInfo__Annotation_msg_init);
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(
      WPI_UPB_UPCAST(msg), &field, arena);
  if (!arr || !WPI_UPB_PRIVATE(_wpi_upb_Array_ResizeUninitialized)(
                  arr, arr->WPI_UPB_PRIVATE(size) + 1, arena)) {
    return NULL;
  }
  struct google_protobuf_GeneratedCodeInfo_Annotation* sub = (struct google_protobuf_GeneratedCodeInfo_Annotation*)_wpi_upb_Message_New(&google__protobuf__GeneratedCodeInfo__Annotation_msg_init, arena);
  if (!arr || !sub) return NULL;
  WPI_UPB_PRIVATE(_wpi_upb_Array_Set)
  (arr, arr->WPI_UPB_PRIVATE(size) - 1, &sub, sizeof(sub));
  return sub;
}

/* google.protobuf.GeneratedCodeInfo.Annotation */

WPI_UPB_INLINE google_protobuf_GeneratedCodeInfo_Annotation* google_protobuf_GeneratedCodeInfo_Annotation_new(wpi_upb_Arena* arena) {
  return (google_protobuf_GeneratedCodeInfo_Annotation*)_wpi_upb_Message_New(&google__protobuf__GeneratedCodeInfo__Annotation_msg_init, arena);
}
WPI_UPB_INLINE google_protobuf_GeneratedCodeInfo_Annotation* google_protobuf_GeneratedCodeInfo_Annotation_parse(const char* buf, size_t size, wpi_upb_Arena* arena) {
  google_protobuf_GeneratedCodeInfo_Annotation* ret = google_protobuf_GeneratedCodeInfo_Annotation_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__GeneratedCodeInfo__Annotation_msg_init, NULL, 0, arena) !=
      kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE google_protobuf_GeneratedCodeInfo_Annotation* google_protobuf_GeneratedCodeInfo_Annotation_parse_ex(const char* buf, size_t size,
                           const wpi_upb_ExtensionRegistry* extreg,
                           int options, wpi_upb_Arena* arena) {
  google_protobuf_GeneratedCodeInfo_Annotation* ret = google_protobuf_GeneratedCodeInfo_Annotation_new(arena);
  if (!ret) return NULL;
  if (wpi_upb_Decode(buf, size, WPI_UPB_UPCAST(ret), &google__protobuf__GeneratedCodeInfo__Annotation_msg_init, extreg, options,
                 arena) != kUpb_DecodeStatus_Ok) {
    return NULL;
  }
  return ret;
}
WPI_UPB_INLINE char* google_protobuf_GeneratedCodeInfo_Annotation_serialize(const google_protobuf_GeneratedCodeInfo_Annotation* msg, wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__GeneratedCodeInfo__Annotation_msg_init, 0, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE char* google_protobuf_GeneratedCodeInfo_Annotation_serialize_ex(const google_protobuf_GeneratedCodeInfo_Annotation* msg, int options,
                                 wpi_upb_Arena* arena, size_t* len) {
  char* ptr;
  (void)wpi_upb_Encode(WPI_UPB_UPCAST(msg), &google__protobuf__GeneratedCodeInfo__Annotation_msg_init, options, arena, &ptr, len);
  return ptr;
}
WPI_UPB_INLINE void google_protobuf_GeneratedCodeInfo_Annotation_clear_path(google_protobuf_GeneratedCodeInfo_Annotation* msg) {
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(12, 24), 0, kUpb_NoSub, 5, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsPacked | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE int32_t const* google_protobuf_GeneratedCodeInfo_Annotation_path(const google_protobuf_GeneratedCodeInfo_Annotation* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(12, 24), 0, kUpb_NoSub, 5, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsPacked | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (int32_t const*)wpi_upb_Array_DataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE const wpi_upb_Array* _google_protobuf_GeneratedCodeInfo_Annotation_path_wpi_upb_array(const google_protobuf_GeneratedCodeInfo_Annotation* msg, size_t* size) {
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(12, 24), 0, kUpb_NoSub, 5, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsPacked | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  const wpi_upb_Array* arr = wpi_upb_Message_GetArray(WPI_UPB_UPCAST(msg), &field);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE wpi_upb_Array* _google_protobuf_GeneratedCodeInfo_Annotation_path_mutable_wpi_upb_array(google_protobuf_GeneratedCodeInfo_Annotation* msg, size_t* size, wpi_upb_Arena* arena) {
  const wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(12, 24), 0, kUpb_NoSub, 5, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsPacked | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(WPI_UPB_UPCAST(msg),
                                                       &field, arena);
  if (size) {
    *size = arr ? arr->WPI_UPB_PRIVATE(size) : 0;
  }
  return arr;
}
WPI_UPB_INLINE void google_protobuf_GeneratedCodeInfo_Annotation_clear_source_file(google_protobuf_GeneratedCodeInfo_Annotation* msg) {
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(28, 32), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE wpi_upb_StringView google_protobuf_GeneratedCodeInfo_Annotation_source_file(const google_protobuf_GeneratedCodeInfo_Annotation* msg) {
  wpi_upb_StringView default_val = wpi_upb_StringView_FromString("");
  wpi_upb_StringView ret;
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(28, 32), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_GeneratedCodeInfo_Annotation_has_source_file(const google_protobuf_GeneratedCodeInfo_Annotation* msg) {
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(28, 32), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_GeneratedCodeInfo_Annotation_clear_begin(google_protobuf_GeneratedCodeInfo_Annotation* msg) {
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(16, 12), 65, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE int32_t google_protobuf_GeneratedCodeInfo_Annotation_begin(const google_protobuf_GeneratedCodeInfo_Annotation* msg) {
  int32_t default_val = (int32_t)0;
  int32_t ret;
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(16, 12), 65, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_GeneratedCodeInfo_Annotation_has_begin(const google_protobuf_GeneratedCodeInfo_Annotation* msg) {
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(16, 12), 65, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_GeneratedCodeInfo_Annotation_clear_end(google_protobuf_GeneratedCodeInfo_Annotation* msg) {
  const wpi_upb_MiniTableField field = {4, WPI_UPB_SIZE(20, 16), 66, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE int32_t google_protobuf_GeneratedCodeInfo_Annotation_end(const google_protobuf_GeneratedCodeInfo_Annotation* msg) {
  int32_t default_val = (int32_t)0;
  int32_t ret;
  const wpi_upb_MiniTableField field = {4, WPI_UPB_SIZE(20, 16), 66, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_GeneratedCodeInfo_Annotation_has_end(const google_protobuf_GeneratedCodeInfo_Annotation* msg) {
  const wpi_upb_MiniTableField field = {4, WPI_UPB_SIZE(20, 16), 66, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE void google_protobuf_GeneratedCodeInfo_Annotation_clear_semantic(google_protobuf_GeneratedCodeInfo_Annotation* msg) {
  const wpi_upb_MiniTableField field = {5, WPI_UPB_SIZE(24, 20), 67, 0, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_ClearBaseField(WPI_UPB_UPCAST(msg), &field);
}
WPI_UPB_INLINE int32_t google_protobuf_GeneratedCodeInfo_Annotation_semantic(const google_protobuf_GeneratedCodeInfo_Annotation* msg) {
  int32_t default_val = 0;
  int32_t ret;
  const wpi_upb_MiniTableField field = {5, WPI_UPB_SIZE(24, 20), 67, 0, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  _wpi_upb_Message_GetNonExtensionField(WPI_UPB_UPCAST(msg), &field,
                                    &default_val, &ret);
  return ret;
}
WPI_UPB_INLINE bool google_protobuf_GeneratedCodeInfo_Annotation_has_semantic(const google_protobuf_GeneratedCodeInfo_Annotation* msg) {
  const wpi_upb_MiniTableField field = {5, WPI_UPB_SIZE(24, 20), 67, 0, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  return wpi_upb_Message_HasBaseField(WPI_UPB_UPCAST(msg), &field);
}

WPI_UPB_INLINE int32_t* google_protobuf_GeneratedCodeInfo_Annotation_mutable_path(google_protobuf_GeneratedCodeInfo_Annotation* msg, size_t* size) {
  wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(12, 24), 0, kUpb_NoSub, 5, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsPacked | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Array* arr = wpi_upb_Message_GetMutableArray(WPI_UPB_UPCAST(msg), &field);
  if (arr) {
    if (size) *size = arr->WPI_UPB_PRIVATE(size);
    return (int32_t*)wpi_upb_Array_MutableDataPtr(arr);
  } else {
    if (size) *size = 0;
    return NULL;
  }
}
WPI_UPB_INLINE int32_t* google_protobuf_GeneratedCodeInfo_Annotation_resize_path(google_protobuf_GeneratedCodeInfo_Annotation* msg, size_t size, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(12, 24), 0, kUpb_NoSub, 5, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsPacked | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  return (int32_t*)wpi_upb_Message_ResizeArrayUninitialized(WPI_UPB_UPCAST(msg),
                                                   &field, size, arena);
}
WPI_UPB_INLINE bool google_protobuf_GeneratedCodeInfo_Annotation_add_path(google_protobuf_GeneratedCodeInfo_Annotation* msg, int32_t val, wpi_upb_Arena* arena) {
  wpi_upb_MiniTableField field = {1, WPI_UPB_SIZE(12, 24), 0, kUpb_NoSub, 5, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsPacked | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)};
  wpi_upb_Array* arr = wpi_upb_Message_GetOrCreateMutableArray(
      WPI_UPB_UPCAST(msg), &field, arena);
  if (!arr || !WPI_UPB_PRIVATE(_wpi_upb_Array_ResizeUninitialized)(
                  arr, arr->WPI_UPB_PRIVATE(size) + 1, arena)) {
    return false;
  }
  WPI_UPB_PRIVATE(_wpi_upb_Array_Set)
  (arr, arr->WPI_UPB_PRIVATE(size) - 1, &val, sizeof(val));
  return true;
}
WPI_UPB_INLINE void google_protobuf_GeneratedCodeInfo_Annotation_set_source_file(google_protobuf_GeneratedCodeInfo_Annotation *msg, wpi_upb_StringView value) {
  const wpi_upb_MiniTableField field = {2, WPI_UPB_SIZE(28, 32), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_GeneratedCodeInfo_Annotation_set_begin(google_protobuf_GeneratedCodeInfo_Annotation *msg, int32_t value) {
  const wpi_upb_MiniTableField field = {3, WPI_UPB_SIZE(16, 12), 65, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_GeneratedCodeInfo_Annotation_set_end(google_protobuf_GeneratedCodeInfo_Annotation *msg, int32_t value) {
  const wpi_upb_MiniTableField field = {4, WPI_UPB_SIZE(20, 16), 66, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}
WPI_UPB_INLINE void google_protobuf_GeneratedCodeInfo_Annotation_set_semantic(google_protobuf_GeneratedCodeInfo_Annotation *msg, int32_t value) {
  const wpi_upb_MiniTableField field = {5, WPI_UPB_SIZE(24, 20), 67, 0, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)};
  wpi_upb_Message_SetBaseField((wpi_upb_Message *)msg, &field, &value);
}

/* Max size 32 is google.protobuf.FileOptions */
/* Max size 64 is google.protobuf.FileOptions */
#define _WPI_UPB_MAXOPT_SIZE WPI_UPB_SIZE(112, 200)

#ifdef __cplusplus
}  /* extern "C" */
#endif


#endif  /* GOOGLE_PROTOBUF_DESCRIPTOR_PROTO_WPI_UPB_H_ */
// end:github_only

typedef enum {
  kUpb_Syntax_Proto2 = 2,
  kUpb_Syntax_Proto3 = 3,
  kUpb_Syntax_Editions = 99
} wpi_upb_Syntax;

// Forward declarations for circular references.
typedef struct wpi_upb_DefPool wpi_upb_DefPool;
typedef struct wpi_upb_EnumDef wpi_upb_EnumDef;
typedef struct wpi_upb_EnumReservedRange wpi_upb_EnumReservedRange;
typedef struct wpi_upb_EnumValueDef wpi_upb_EnumValueDef;
typedef struct wpi_upb_ExtensionRange wpi_upb_ExtensionRange;
typedef struct wpi_upb_FieldDef wpi_upb_FieldDef;
typedef struct wpi_upb_FileDef wpi_upb_FileDef;
typedef struct wpi_upb_MessageDef wpi_upb_MessageDef;
typedef struct wpi_upb_MessageReservedRange wpi_upb_MessageReservedRange;
typedef struct wpi_upb_MethodDef wpi_upb_MethodDef;
typedef struct wpi_upb_OneofDef wpi_upb_OneofDef;
typedef struct wpi_upb_ServiceDef wpi_upb_ServiceDef;

// EVERYTHING BELOW THIS LINE IS INTERNAL - DO NOT USE /////////////////////////

typedef struct wpi_upb_DefBuilder wpi_upb_DefBuilder;

#endif /* WPI_UPB_REFLECTION_COMMON_H_ */

#ifndef WPI_UPB_REFLECTION_DEF_TYPE_H_
#define WPI_UPB_REFLECTION_DEF_TYPE_H_


// Must be last.

// Inside a symtab we store tagged pointers to specific def types.
typedef enum {
  WPI_UPB_DEFTYPE_MASK = 7,

  // Only inside symtab table.
  WPI_UPB_DEFTYPE_EXT = 0,
  WPI_UPB_DEFTYPE_MSG = 1,
  WPI_UPB_DEFTYPE_ENUM = 2,
  WPI_UPB_DEFTYPE_ENUMVAL = 3,
  WPI_UPB_DEFTYPE_SERVICE = 4,

  // Only inside message table.
  WPI_UPB_DEFTYPE_FIELD = 0,
  WPI_UPB_DEFTYPE_ONEOF = 1,
} wpi_upb_deftype_t;

#ifdef __cplusplus
extern "C" {
#endif

// Our 3-bit pointer tagging requires all pointers to be multiples of 8.
// The arena will always yield 8-byte-aligned addresses, however we put
// the defs into arrays. For each element in the array to be 8-byte-aligned,
// the sizes of each def type must also be a multiple of 8.
//
// If any of these asserts fail, we need to add or remove padding on 32-bit
// machines (64-bit machines will have 8-byte alignment already due to
// pointers, which all of these structs have).
WPI_UPB_INLINE void _wpi_upb_DefType_CheckPadding(size_t size) {
  WPI_UPB_ASSERT((size & WPI_UPB_DEFTYPE_MASK) == 0);
}

wpi_upb_deftype_t _wpi_upb_DefType_Type(wpi_upb_value v);

wpi_upb_value _wpi_upb_DefType_Pack(const void* ptr, wpi_upb_deftype_t type);

const void* _wpi_upb_DefType_Unpack(wpi_upb_value v, wpi_upb_deftype_t type);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_REFLECTION_DEF_TYPE_H_ */

// Must be last.

#ifdef __cplusplus
extern "C" {
#endif

WPI_UPB_API void wpi_upb_DefPool_Free(wpi_upb_DefPool* s);

WPI_UPB_API wpi_upb_DefPool* wpi_upb_DefPool_New(void);

WPI_UPB_API const WPI_UPB_DESC(FeatureSetDefaults) *
    wpi_upb_DefPool_FeatureSetDefaults(const wpi_upb_DefPool* s);

WPI_UPB_API bool wpi_upb_DefPool_SetFeatureSetDefaults(wpi_upb_DefPool* s,
                                               const char* serialized_defaults,
                                               size_t serialized_len,
                                               wpi_upb_Status* status);

WPI_UPB_API const wpi_upb_MessageDef* wpi_upb_DefPool_FindMessageByName(
    const wpi_upb_DefPool* s, const char* sym);

const wpi_upb_MessageDef* wpi_upb_DefPool_FindMessageByNameWithSize(
    const wpi_upb_DefPool* s, const char* sym, size_t len);

WPI_UPB_API const wpi_upb_EnumDef* wpi_upb_DefPool_FindEnumByName(const wpi_upb_DefPool* s,
                                                      const char* sym);

const wpi_upb_EnumValueDef* wpi_upb_DefPool_FindEnumByNameval(const wpi_upb_DefPool* s,
                                                      const char* sym);

const wpi_upb_FileDef* wpi_upb_DefPool_FindFileByName(const wpi_upb_DefPool* s,
                                              const char* name);

const wpi_upb_FileDef* wpi_upb_DefPool_FindFileByNameWithSize(const wpi_upb_DefPool* s,
                                                      const char* name,
                                                      size_t len);

const wpi_upb_FieldDef* wpi_upb_DefPool_FindExtensionByMiniTable(
    const wpi_upb_DefPool* s, const wpi_upb_MiniTableExtension* ext);

WPI_UPB_API const wpi_upb_FieldDef* wpi_upb_DefPool_FindExtensionByName(const wpi_upb_DefPool* s,
                                                    const char* sym);

const wpi_upb_FieldDef* wpi_upb_DefPool_FindExtensionByNameWithSize(
    const wpi_upb_DefPool* s, const char* name, size_t size);

const wpi_upb_FieldDef* wpi_upb_DefPool_FindExtensionByNumber(const wpi_upb_DefPool* s,
                                                      const wpi_upb_MessageDef* m,
                                                      int32_t fieldnum);

WPI_UPB_API const wpi_upb_ServiceDef* wpi_upb_DefPool_FindServiceByName(
  const wpi_upb_DefPool* s, const char* name);

const wpi_upb_ServiceDef* wpi_upb_DefPool_FindServiceByNameWithSize(
    const wpi_upb_DefPool* s, const char* name, size_t size);

const wpi_upb_FileDef* wpi_upb_DefPool_FindFileContainingSymbol(const wpi_upb_DefPool* s,
                                                        const char* name);

WPI_UPB_API const wpi_upb_FileDef* wpi_upb_DefPool_AddFile(
    wpi_upb_DefPool* s, const WPI_UPB_DESC(FileDescriptorProto) * file_proto,
    wpi_upb_Status* status);

WPI_UPB_API const wpi_upb_ExtensionRegistry* wpi_upb_DefPool_ExtensionRegistry(
    const wpi_upb_DefPool* s);

const wpi_upb_FieldDef** wpi_upb_DefPool_GetAllExtensions(const wpi_upb_DefPool* s,
                                                  const wpi_upb_MessageDef* m,
                                                  size_t* count);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_REFLECTION_DEF_POOL_H_ */

// Must be last.

#ifdef __cplusplus
extern "C" {
#endif

wpi_upb_Arena* _wpi_upb_DefPool_Arena(const wpi_upb_DefPool* s);
size_t _wpi_upb_DefPool_BytesLoaded(const wpi_upb_DefPool* s);
wpi_upb_ExtensionRegistry* _wpi_upb_DefPool_ExtReg(const wpi_upb_DefPool* s);

bool _wpi_upb_DefPool_InsertExt(wpi_upb_DefPool* s, const wpi_upb_MiniTableExtension* ext,
                            const wpi_upb_FieldDef* f);
bool _wpi_upb_DefPool_InsertSym(wpi_upb_DefPool* s, wpi_upb_StringView sym, wpi_upb_value v,
                            wpi_upb_Status* status);
bool _wpi_upb_DefPool_LookupSym(const wpi_upb_DefPool* s, const char* sym, size_t size,
                            wpi_upb_value* v);

void** _wpi_upb_DefPool_ScratchData(const wpi_upb_DefPool* s);
size_t* _wpi_upb_DefPool_ScratchSize(const wpi_upb_DefPool* s);
void _wpi_upb_DefPool_SetPlatform(wpi_upb_DefPool* s, wpi_upb_MiniTablePlatform platform);

// For generated code only: loads a generated descriptor.
typedef struct _wpi_upb_DefPool_Init {
  struct _wpi_upb_DefPool_Init** deps;  // Dependencies of this file.
  const wpi_upb_MiniTableFile* layout;
  const char* filename;
  wpi_upb_StringView descriptor;  // Serialized descriptor.
} _wpi_upb_DefPool_Init;

bool _wpi_upb_DefPool_LoadDefInit(wpi_upb_DefPool* s, const _wpi_upb_DefPool_Init* init);

// Should only be directly called by tests. This variant lets us suppress
// the use of compiled-in tables, forcing a rebuild of the tables at runtime.
bool _wpi_upb_DefPool_LoadDefInitEx(wpi_upb_DefPool* s, const _wpi_upb_DefPool_Init* init,
                                bool rebuild_minitable);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_REFLECTION_DEF_POOL_INTERNAL_H_ */

// IWYU pragma: private, include "upb/reflection/def.h"

#ifndef WPI_UPB_REFLECTION_FILE_DEF_H_
#define WPI_UPB_REFLECTION_FILE_DEF_H_


// Must be last.

#ifdef __cplusplus
extern "C" {
#endif

WPI_UPB_API const char* wpi_upb_FileDef_EditionName(int edition);

const wpi_upb_FileDef* wpi_upb_FileDef_Dependency(const wpi_upb_FileDef* f, int i);
int wpi_upb_FileDef_DependencyCount(const wpi_upb_FileDef* f);
bool wpi_upb_FileDef_HasOptions(const wpi_upb_FileDef* f);
WPI_UPB_API const char* wpi_upb_FileDef_Name(const wpi_upb_FileDef* f);
const WPI_UPB_DESC(FileOptions) * wpi_upb_FileDef_Options(const wpi_upb_FileDef* f);
const WPI_UPB_DESC(FeatureSet) * wpi_upb_FileDef_ResolvedFeatures(const wpi_upb_FileDef* f);
const char* wpi_upb_FileDef_Package(const wpi_upb_FileDef* f);
WPI_UPB_DESC(Edition) wpi_upb_FileDef_Edition(const wpi_upb_FileDef* f);
WPI_UPB_API const wpi_upb_DefPool* wpi_upb_FileDef_Pool(const wpi_upb_FileDef* f);

const wpi_upb_FileDef* wpi_upb_FileDef_PublicDependency(const wpi_upb_FileDef* f, int i);
int wpi_upb_FileDef_PublicDependencyCount(const wpi_upb_FileDef* f);

const wpi_upb_ServiceDef* wpi_upb_FileDef_Service(const wpi_upb_FileDef* f, int i);
int wpi_upb_FileDef_ServiceCount(const wpi_upb_FileDef* f);

WPI_UPB_API wpi_upb_Syntax wpi_upb_FileDef_Syntax(const wpi_upb_FileDef* f);

const wpi_upb_EnumDef* wpi_upb_FileDef_TopLevelEnum(const wpi_upb_FileDef* f, int i);
int wpi_upb_FileDef_TopLevelEnumCount(const wpi_upb_FileDef* f);

const wpi_upb_FieldDef* wpi_upb_FileDef_TopLevelExtension(const wpi_upb_FileDef* f, int i);
int wpi_upb_FileDef_TopLevelExtensionCount(const wpi_upb_FileDef* f);

const wpi_upb_MessageDef* wpi_upb_FileDef_TopLevelMessage(const wpi_upb_FileDef* f, int i);
int wpi_upb_FileDef_TopLevelMessageCount(const wpi_upb_FileDef* f);

const wpi_upb_FileDef* wpi_upb_FileDef_WeakDependency(const wpi_upb_FileDef* f, int i);
int wpi_upb_FileDef_WeakDependencyCount(const wpi_upb_FileDef* f);

// Returns whether |symbol| is transitively included by |f|
bool wpi_upb_FileDef_Resolves(const wpi_upb_FileDef* f, const char* symbol);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_REFLECTION_FILE_DEF_H_ */

#ifndef WPI_UPB_REFLECTION_DEF_BUILDER_INTERNAL_H_
#define WPI_UPB_REFLECTION_DEF_BUILDER_INTERNAL_H_


// Must be last.

// We want to copy the options verbatim into the destination options proto.
// We use serialize+parse as our deep copy.
#define WPI_UPB_DEF_SET_OPTIONS(target, desc_type, options_type, proto)           \
  if (WPI_UPB_DESC(desc_type##_has_options)(proto)) {                             \
    size_t size;                                                              \
    char* pb = WPI_UPB_DESC(options_type##_serialize)(                            \
        WPI_UPB_DESC(desc_type##_options)(proto), ctx->tmp_arena, &size);         \
    if (!pb) _wpi_upb_DefBuilder_OomErr(ctx);                                     \
    target =                                                                  \
        WPI_UPB_DESC(options_type##_parse)(pb, size, _wpi_upb_DefBuilder_Arena(ctx)); \
    if (!target) _wpi_upb_DefBuilder_OomErr(ctx);                                 \
  } else {                                                                    \
    target = (const WPI_UPB_DESC(options_type)*)kUpbDefOptDefault;                \
  }

#ifdef __cplusplus
extern "C" {
#endif

struct wpi_upb_DefBuilder {
  wpi_upb_DefPool* symtab;
  wpi_upb_strtable feature_cache;             // Caches features by identity.
  WPI_UPB_DESC(FeatureSet*) legacy_features;  // For computing legacy features.
  char* tmp_buf;                          // Temporary buffer in tmp_arena.
  size_t tmp_buf_size;                    // Size of temporary buffer.
  wpi_upb_FileDef* file;                 // File we are building.
  wpi_upb_Arena* arena;                  // Allocate defs here.
  wpi_upb_Arena* tmp_arena;              // For temporary allocations.
  wpi_upb_Status* status;                // Record errors here.
  const wpi_upb_MiniTableFile* layout;   // NULL if we should build layouts.
  wpi_upb_MiniTablePlatform platform;    // Platform we are targeting.
  int enum_count;                    // Count of enums built so far.
  int msg_count;                     // Count of messages built so far.
  int ext_count;                     // Count of extensions built so far.
  jmp_buf err;                       // longjmp() on error.
};

extern const char* kUpbDefOptDefault;

// ctx->status has already been set elsewhere so just fail/longjmp()
WPI_UPB_NORETURN void _wpi_upb_DefBuilder_FailJmp(wpi_upb_DefBuilder* ctx);

WPI_UPB_NORETURN void _wpi_upb_DefBuilder_Errf(wpi_upb_DefBuilder* ctx, const char* fmt,
                                       ...) WPI_UPB_PRINTF(2, 3);
WPI_UPB_NORETURN void _wpi_upb_DefBuilder_OomErr(wpi_upb_DefBuilder* ctx);

const char* _wpi_upb_DefBuilder_MakeFullName(wpi_upb_DefBuilder* ctx,
                                         const char* prefix,
                                         wpi_upb_StringView name);

// Given a symbol and the base symbol inside which it is defined,
// find the symbol's definition.
const void* _wpi_upb_DefBuilder_ResolveAny(wpi_upb_DefBuilder* ctx,
                                       const char* from_name_dbg,
                                       const char* base, wpi_upb_StringView sym,
                                       wpi_upb_deftype_t* type);

const void* _wpi_upb_DefBuilder_Resolve(wpi_upb_DefBuilder* ctx,
                                    const char* from_name_dbg, const char* base,
                                    wpi_upb_StringView sym, wpi_upb_deftype_t type);

char _wpi_upb_DefBuilder_ParseEscape(wpi_upb_DefBuilder* ctx, const wpi_upb_FieldDef* f,
                                 const char** src, const char* end);

const char* _wpi_upb_DefBuilder_FullToShort(const char* fullname);

WPI_UPB_INLINE void* _wpi_upb_DefBuilder_Alloc(wpi_upb_DefBuilder* ctx, size_t bytes) {
  if (bytes == 0) return NULL;
  void* ret = wpi_upb_Arena_Malloc(ctx->arena, bytes);
  if (!ret) _wpi_upb_DefBuilder_OomErr(ctx);
  return ret;
}

// Adds a symbol |v| to the symtab, which must be a def pointer previously
// packed with pack_def(). The def's pointer to wpi_upb_FileDef* must be set before
// adding, so we know which entries to remove if building this file fails.
WPI_UPB_INLINE void _wpi_upb_DefBuilder_Add(wpi_upb_DefBuilder* ctx, const char* name,
                                    wpi_upb_value v) {
  wpi_upb_StringView sym = {.data = name, .size = strlen(name)};
  bool ok = _wpi_upb_DefPool_InsertSym(ctx->symtab, sym, v, ctx->status);
  if (!ok) _wpi_upb_DefBuilder_FailJmp(ctx);
}

WPI_UPB_INLINE wpi_upb_Arena* _wpi_upb_DefBuilder_Arena(const wpi_upb_DefBuilder* ctx) {
  return ctx->arena;
}

WPI_UPB_INLINE wpi_upb_FileDef* _wpi_upb_DefBuilder_File(const wpi_upb_DefBuilder* ctx) {
  return ctx->file;
}

// This version of CheckIdent() is only called by other, faster versions after
// they detect a parsing error.
void _wpi_upb_DefBuilder_CheckIdentSlow(wpi_upb_DefBuilder* ctx, wpi_upb_StringView name,
                                    bool full);

// Verify a full identifier string. This is slightly more complicated than
// verifying a relative identifier string because we must track '.' chars.
WPI_UPB_INLINE void _wpi_upb_DefBuilder_CheckIdentFull(wpi_upb_DefBuilder* ctx,
                                               wpi_upb_StringView name) {
  bool good = name.size > 0;
  bool start = true;

  for (size_t i = 0; i < name.size; i++) {
    const char c = name.data[i];
    const char d = c | 0x20;  // force lowercase
    const bool is_alpha = (('a' <= d) & (d <= 'z')) | (c == '_');
    const bool is_numer = ('0' <= c) & (c <= '9') & !start;
    const bool is_dot = (c == '.') & !start;

    good &= is_alpha | is_numer | is_dot;
    start = is_dot;
  }

  if (!good) _wpi_upb_DefBuilder_CheckIdentSlow(ctx, name, true);
}

// Returns true if the returned feature set is new and must be populated.
bool _wpi_upb_DefBuilder_GetOrCreateFeatureSet(wpi_upb_DefBuilder* ctx,
                                           const WPI_UPB_DESC(FeatureSet*) parent,
                                           wpi_upb_StringView key,
                                           WPI_UPB_DESC(FeatureSet**) set);

const WPI_UPB_DESC(FeatureSet*)
    _wpi_upb_DefBuilder_DoResolveFeatures(wpi_upb_DefBuilder* ctx,
                                      const WPI_UPB_DESC(FeatureSet*) parent,
                                      const WPI_UPB_DESC(FeatureSet*) child,
                                      bool is_implicit);

WPI_UPB_INLINE const WPI_UPB_DESC(FeatureSet*)
    _wpi_upb_DefBuilder_ResolveFeatures(wpi_upb_DefBuilder* ctx,
                                    const WPI_UPB_DESC(FeatureSet*) parent,
                                    const WPI_UPB_DESC(FeatureSet*) child) {
  return _wpi_upb_DefBuilder_DoResolveFeatures(ctx, parent, child, false);
}

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_REFLECTION_DEF_BUILDER_INTERNAL_H_ */

#ifndef WPI_UPB_REFLECTION_ENUM_DEF_INTERNAL_H_
#define WPI_UPB_REFLECTION_ENUM_DEF_INTERNAL_H_


// IWYU pragma: private, include "upb/reflection/def.h"

#ifndef WPI_UPB_REFLECTION_ENUM_DEF_H_
#define WPI_UPB_REFLECTION_ENUM_DEF_H_


// Must be last.

#ifdef __cplusplus
extern "C" {
#endif

bool wpi_upb_EnumDef_CheckNumber(const wpi_upb_EnumDef* e, int32_t num);
const wpi_upb_MessageDef* wpi_upb_EnumDef_ContainingType(const wpi_upb_EnumDef* e);
int32_t wpi_upb_EnumDef_Default(const wpi_upb_EnumDef* e);
WPI_UPB_API const wpi_upb_FileDef* wpi_upb_EnumDef_File(const wpi_upb_EnumDef* e);
const wpi_upb_EnumValueDef* wpi_upb_EnumDef_FindValueByName(const wpi_upb_EnumDef* e,
                                                    const char* name);
WPI_UPB_API const wpi_upb_EnumValueDef* wpi_upb_EnumDef_FindValueByNameWithSize(
    const wpi_upb_EnumDef* e, const char* name, size_t size);
WPI_UPB_API const wpi_upb_EnumValueDef* wpi_upb_EnumDef_FindValueByNumber(
    const wpi_upb_EnumDef* e, int32_t num);
WPI_UPB_API const char* wpi_upb_EnumDef_FullName(const wpi_upb_EnumDef* e);
bool wpi_upb_EnumDef_HasOptions(const wpi_upb_EnumDef* e);
bool wpi_upb_EnumDef_IsClosed(const wpi_upb_EnumDef* e);
bool wpi_upb_EnumDef_IsSpecifiedAsClosed(const wpi_upb_EnumDef* e);

// Creates a mini descriptor string for an enum, returns true on success.
bool wpi_upb_EnumDef_MiniDescriptorEncode(const wpi_upb_EnumDef* e, wpi_upb_Arena* a,
                                      wpi_upb_StringView* out);

const char* wpi_upb_EnumDef_Name(const wpi_upb_EnumDef* e);
const WPI_UPB_DESC(EnumOptions) * wpi_upb_EnumDef_Options(const wpi_upb_EnumDef* e);
const WPI_UPB_DESC(FeatureSet) * wpi_upb_EnumDef_ResolvedFeatures(const wpi_upb_EnumDef* e);

wpi_upb_StringView wpi_upb_EnumDef_ReservedName(const wpi_upb_EnumDef* e, int i);
int wpi_upb_EnumDef_ReservedNameCount(const wpi_upb_EnumDef* e);

const wpi_upb_EnumReservedRange* wpi_upb_EnumDef_ReservedRange(const wpi_upb_EnumDef* e,
                                                       int i);
int wpi_upb_EnumDef_ReservedRangeCount(const wpi_upb_EnumDef* e);

WPI_UPB_API const wpi_upb_EnumValueDef* wpi_upb_EnumDef_Value(const wpi_upb_EnumDef* e, int i);
WPI_UPB_API int wpi_upb_EnumDef_ValueCount(const wpi_upb_EnumDef* e);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_REFLECTION_ENUM_DEF_H_ */

// Must be last.

#ifdef __cplusplus
extern "C" {
#endif

wpi_upb_EnumDef* _wpi_upb_EnumDef_At(const wpi_upb_EnumDef* e, int i);
bool _wpi_upb_EnumDef_Insert(wpi_upb_EnumDef* e, wpi_upb_EnumValueDef* v, wpi_upb_Arena* a);
const wpi_upb_MiniTableEnum* _wpi_upb_EnumDef_MiniTable(const wpi_upb_EnumDef* e);

// Allocate and initialize an array of |n| enum defs.
wpi_upb_EnumDef* _wpi_upb_EnumDefs_New(wpi_upb_DefBuilder* ctx, int n,
                               const WPI_UPB_DESC(EnumDescriptorProto*)
                                   const* protos,
                               const WPI_UPB_DESC(FeatureSet*) parent_features,
                               const wpi_upb_MessageDef* containing_type);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_REFLECTION_ENUM_DEF_INTERNAL_H_ */

#ifndef WPI_UPB_REFLECTION_ENUM_VALUE_DEF_INTERNAL_H_
#define WPI_UPB_REFLECTION_ENUM_VALUE_DEF_INTERNAL_H_


// IWYU pragma: private, include "upb/reflection/def.h"

#ifndef WPI_UPB_REFLECTION_ENUM_VALUE_DEF_H_
#define WPI_UPB_REFLECTION_ENUM_VALUE_DEF_H_


// Must be last.

#ifdef __cplusplus
extern "C" {
#endif

const wpi_upb_EnumDef* wpi_upb_EnumValueDef_Enum(const wpi_upb_EnumValueDef* v);
const char* wpi_upb_EnumValueDef_FullName(const wpi_upb_EnumValueDef* v);
bool wpi_upb_EnumValueDef_HasOptions(const wpi_upb_EnumValueDef* v);
uint32_t wpi_upb_EnumValueDef_Index(const wpi_upb_EnumValueDef* v);
WPI_UPB_API const char* wpi_upb_EnumValueDef_Name(const wpi_upb_EnumValueDef* v);
WPI_UPB_API int32_t wpi_upb_EnumValueDef_Number(const wpi_upb_EnumValueDef* v);
const WPI_UPB_DESC(EnumValueOptions) *
    wpi_upb_EnumValueDef_Options(const wpi_upb_EnumValueDef* v);
const WPI_UPB_DESC(FeatureSet) *
    wpi_upb_EnumValueDef_ResolvedFeatures(const wpi_upb_EnumValueDef* e);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_REFLECTION_ENUM_VALUE_DEF_H_ */

// Must be last.

#ifdef __cplusplus
extern "C" {
#endif

wpi_upb_EnumValueDef* _wpi_upb_EnumValueDef_At(const wpi_upb_EnumValueDef* v, int i);

// Allocate and initialize an array of |n| enum value defs owned by |e|.
wpi_upb_EnumValueDef* _wpi_upb_EnumValueDefs_New(
    wpi_upb_DefBuilder* ctx, const char* prefix, int n,
    const WPI_UPB_DESC(EnumValueDescriptorProto*) const* protos,
    const WPI_UPB_DESC(FeatureSet*) parent_features, wpi_upb_EnumDef* e,
    bool* is_sorted);

const wpi_upb_EnumValueDef** _wpi_upb_EnumValueDefs_Sorted(const wpi_upb_EnumValueDef* v,
                                                   int n, wpi_upb_Arena* a);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_REFLECTION_ENUM_VALUE_DEF_INTERNAL_H_ */

#ifndef WPI_UPB_REFLECTION_FIELD_DEF_INTERNAL_H_
#define WPI_UPB_REFLECTION_FIELD_DEF_INTERNAL_H_


// IWYU pragma: private, include "upb/reflection/def.h"

#ifndef WPI_UPB_REFLECTION_FIELD_DEF_H_
#define WPI_UPB_REFLECTION_FIELD_DEF_H_

#include <stdint.h>


// Must be last.

// Maximum field number allowed for FieldDefs.
// This is an inherent limit of the protobuf wire format.
#define kUpb_MaxFieldNumber ((1 << 29) - 1)

#ifdef __cplusplus
extern "C" {
#endif

const wpi_upb_OneofDef* wpi_upb_FieldDef_ContainingOneof(const wpi_upb_FieldDef* f);
WPI_UPB_API const wpi_upb_MessageDef* wpi_upb_FieldDef_ContainingType(
    const wpi_upb_FieldDef* f);
WPI_UPB_API wpi_upb_CType wpi_upb_FieldDef_CType(const wpi_upb_FieldDef* f);
WPI_UPB_API wpi_upb_MessageValue wpi_upb_FieldDef_Default(const wpi_upb_FieldDef* f);
WPI_UPB_API const wpi_upb_EnumDef* wpi_upb_FieldDef_EnumSubDef(const wpi_upb_FieldDef* f);
const wpi_upb_MessageDef* wpi_upb_FieldDef_ExtensionScope(const wpi_upb_FieldDef* f);
WPI_UPB_API const wpi_upb_FileDef* wpi_upb_FieldDef_File(const wpi_upb_FieldDef* f);
const char* wpi_upb_FieldDef_FullName(const wpi_upb_FieldDef* f);
bool wpi_upb_FieldDef_HasDefault(const wpi_upb_FieldDef* f);
bool wpi_upb_FieldDef_HasJsonName(const wpi_upb_FieldDef* f);
bool wpi_upb_FieldDef_HasOptions(const wpi_upb_FieldDef* f);
WPI_UPB_API bool wpi_upb_FieldDef_HasPresence(const wpi_upb_FieldDef* f);
bool wpi_upb_FieldDef_HasSubDef(const wpi_upb_FieldDef* f);
uint32_t wpi_upb_FieldDef_Index(const wpi_upb_FieldDef* f);
WPI_UPB_API bool wpi_upb_FieldDef_IsEnum(const wpi_upb_FieldDef* f);
bool wpi_upb_FieldDef_IsExtension(const wpi_upb_FieldDef* f);
WPI_UPB_API bool wpi_upb_FieldDef_IsMap(const wpi_upb_FieldDef* f);
bool wpi_upb_FieldDef_IsOptional(const wpi_upb_FieldDef* f);
WPI_UPB_API bool wpi_upb_FieldDef_IsPacked(const wpi_upb_FieldDef* f);
bool wpi_upb_FieldDef_IsPrimitive(const wpi_upb_FieldDef* f);
WPI_UPB_API bool wpi_upb_FieldDef_IsRepeated(const wpi_upb_FieldDef* f);
bool wpi_upb_FieldDef_IsRequired(const wpi_upb_FieldDef* f);
bool wpi_upb_FieldDef_IsString(const wpi_upb_FieldDef* f);
WPI_UPB_API bool wpi_upb_FieldDef_IsSubMessage(const wpi_upb_FieldDef* f);
WPI_UPB_API const char* wpi_upb_FieldDef_JsonName(const wpi_upb_FieldDef* f);
WPI_UPB_API wpi_upb_Label wpi_upb_FieldDef_Label(const wpi_upb_FieldDef* f);
uint32_t wpi_upb_FieldDef_LayoutIndex(const wpi_upb_FieldDef* f);
WPI_UPB_API const wpi_upb_MessageDef* wpi_upb_FieldDef_MessageSubDef(const wpi_upb_FieldDef* f);
bool _wpi_upb_FieldDef_ValidateUtf8(const wpi_upb_FieldDef* f);
bool _wpi_upb_FieldDef_IsGroupLike(const wpi_upb_FieldDef* f);

// Creates a mini descriptor string for a field, returns true on success.
bool wpi_upb_FieldDef_MiniDescriptorEncode(const wpi_upb_FieldDef* f, wpi_upb_Arena* a,
                                       wpi_upb_StringView* out);

const wpi_upb_MiniTableField* wpi_upb_FieldDef_MiniTable(const wpi_upb_FieldDef* f);
const wpi_upb_MiniTableExtension* wpi_upb_FieldDef_MiniTableExtension(
    const wpi_upb_FieldDef* f);
WPI_UPB_API const char* wpi_upb_FieldDef_Name(const wpi_upb_FieldDef* f);
WPI_UPB_API uint32_t wpi_upb_FieldDef_Number(const wpi_upb_FieldDef* f);
const WPI_UPB_DESC(FieldOptions) * wpi_upb_FieldDef_Options(const wpi_upb_FieldDef* f);
const WPI_UPB_DESC(FeatureSet) *
    wpi_upb_FieldDef_ResolvedFeatures(const wpi_upb_FieldDef* f);
WPI_UPB_API const wpi_upb_OneofDef* wpi_upb_FieldDef_RealContainingOneof(
    const wpi_upb_FieldDef* f);
WPI_UPB_API wpi_upb_FieldType wpi_upb_FieldDef_Type(const wpi_upb_FieldDef* f);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_REFLECTION_FIELD_DEF_H_ */

// Must be last.

#ifdef __cplusplus
extern "C" {
#endif

wpi_upb_FieldDef* _wpi_upb_FieldDef_At(const wpi_upb_FieldDef* f, int i);

bool _wpi_upb_FieldDef_IsClosedEnum(const wpi_upb_FieldDef* f);
bool _wpi_upb_FieldDef_IsProto3Optional(const wpi_upb_FieldDef* f);
int _wpi_upb_FieldDef_LayoutIndex(const wpi_upb_FieldDef* f);
uint64_t _wpi_upb_FieldDef_Modifiers(const wpi_upb_FieldDef* f);
void _wpi_upb_FieldDef_Resolve(wpi_upb_DefBuilder* ctx, const char* prefix,
                           wpi_upb_FieldDef* f);
void _wpi_upb_FieldDef_BuildMiniTableExtension(wpi_upb_DefBuilder* ctx,
                                           const wpi_upb_FieldDef* f);

// Allocate and initialize an array of |n| extensions (field defs).
wpi_upb_FieldDef* _wpi_upb_Extensions_New(wpi_upb_DefBuilder* ctx, int n,
                                  const WPI_UPB_DESC(FieldDescriptorProto*)
                                      const* protos,
                                  const WPI_UPB_DESC(FeatureSet*) parent_features,
                                  const char* prefix, wpi_upb_MessageDef* m);

// Allocate and initialize an array of |n| field defs.
wpi_upb_FieldDef* _wpi_upb_FieldDefs_New(wpi_upb_DefBuilder* ctx, int n,
                                 const WPI_UPB_DESC(FieldDescriptorProto*)
                                     const* protos,
                                 const WPI_UPB_DESC(FeatureSet*) parent_features,
                                 const char* prefix, wpi_upb_MessageDef* m,
                                 bool* is_sorted);

// Allocate and return a list of pointers to the |n| field defs in |ff|,
// sorted by field number.
const wpi_upb_FieldDef** _wpi_upb_FieldDefs_Sorted(const wpi_upb_FieldDef* f, int n,
                                           wpi_upb_Arena* a);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_REFLECTION_FIELD_DEF_INTERNAL_H_ */

#ifndef WPI_UPB_REFLECTION_FILE_DEF_INTERNAL_H_
#define WPI_UPB_REFLECTION_FILE_DEF_INTERNAL_H_


// Must be last.

#ifdef __cplusplus
extern "C" {
#endif

const wpi_upb_MiniTableExtension* _wpi_upb_FileDef_ExtensionMiniTable(
    const wpi_upb_FileDef* f, int i);
const int32_t* _wpi_upb_FileDef_PublicDependencyIndexes(const wpi_upb_FileDef* f);
const int32_t* _wpi_upb_FileDef_WeakDependencyIndexes(const wpi_upb_FileDef* f);

// wpi_upb_FileDef_Package() returns "" if f->package is NULL, this does not.
const char* _wpi_upb_FileDef_RawPackage(const wpi_upb_FileDef* f);

void _wpi_upb_FileDef_Create(wpi_upb_DefBuilder* ctx,
                         const WPI_UPB_DESC(FileDescriptorProto) * file_proto);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_REFLECTION_FILE_DEF_INTERNAL_H_ */

#ifndef WPI_UPB_REFLECTION_MESSAGE_DEF_INTERNAL_H_
#define WPI_UPB_REFLECTION_MESSAGE_DEF_INTERNAL_H_


// IWYU pragma: private, include "upb/reflection/def.h"

#ifndef WPI_UPB_REFLECTION_MESSAGE_DEF_H_
#define WPI_UPB_REFLECTION_MESSAGE_DEF_H_


// Must be last.

// Well-known field tag numbers for map-entry messages.
#define kUpb_MapEntry_KeyFieldNumber 1
#define kUpb_MapEntry_ValueFieldNumber 2

// Well-known field tag numbers for Any messages.
#define kUpb_Any_TypeFieldNumber 1
#define kUpb_Any_ValueFieldNumber 2

// Well-known field tag numbers for duration messages.
#define kUpb_Duration_SecondsFieldNumber 1
#define kUpb_Duration_NanosFieldNumber 2

// Well-known field tag numbers for timestamp messages.
#define kUpb_Timestamp_SecondsFieldNumber 1
#define kUpb_Timestamp_NanosFieldNumber 2

// All the different kind of well known type messages. For simplicity of check,
// number wrappers and string wrappers are grouped together. Make sure the
// order and number of these groups are not changed.
typedef enum {
  kUpb_WellKnown_Unspecified,
  kUpb_WellKnown_Any,
  kUpb_WellKnown_FieldMask,
  kUpb_WellKnown_Duration,
  kUpb_WellKnown_Timestamp,

  // number wrappers
  kUpb_WellKnown_DoubleValue,
  kUpb_WellKnown_FloatValue,
  kUpb_WellKnown_Int64Value,
  kUpb_WellKnown_UInt64Value,
  kUpb_WellKnown_Int32Value,
  kUpb_WellKnown_UInt32Value,

  // string wrappers
  kUpb_WellKnown_StringValue,
  kUpb_WellKnown_BytesValue,
  kUpb_WellKnown_BoolValue,
  kUpb_WellKnown_Value,
  kUpb_WellKnown_ListValue,
  kUpb_WellKnown_Struct,
} wpi_upb_WellKnown;

#ifdef __cplusplus
extern "C" {
#endif

const wpi_upb_MessageDef* wpi_upb_MessageDef_ContainingType(const wpi_upb_MessageDef* m);

const wpi_upb_ExtensionRange* wpi_upb_MessageDef_ExtensionRange(const wpi_upb_MessageDef* m,
                                                        int i);
int wpi_upb_MessageDef_ExtensionRangeCount(const wpi_upb_MessageDef* m);

WPI_UPB_API const wpi_upb_FieldDef* wpi_upb_MessageDef_Field(const wpi_upb_MessageDef* m,
                                                 int i);
WPI_UPB_API int wpi_upb_MessageDef_FieldCount(const wpi_upb_MessageDef* m);

WPI_UPB_API const wpi_upb_FileDef* wpi_upb_MessageDef_File(const wpi_upb_MessageDef* m);

// Returns a field by either JSON name or regular proto name.
const wpi_upb_FieldDef* wpi_upb_MessageDef_FindByJsonNameWithSize(
    const wpi_upb_MessageDef* m, const char* name, size_t size);
WPI_UPB_INLINE const wpi_upb_FieldDef* wpi_upb_MessageDef_FindByJsonName(
    const wpi_upb_MessageDef* m, const char* name) {
  return wpi_upb_MessageDef_FindByJsonNameWithSize(m, name, strlen(name));
}

// Lookup of either field or oneof by name. Returns whether either was found.
// If the return is true, then the found def will be set, and the non-found
// one set to NULL.
WPI_UPB_API bool wpi_upb_MessageDef_FindByNameWithSize(const wpi_upb_MessageDef* m,
                                               const char* name, size_t size,
                                               const wpi_upb_FieldDef** f,
                                               const wpi_upb_OneofDef** o);
WPI_UPB_INLINE bool wpi_upb_MessageDef_FindByName(const wpi_upb_MessageDef* m,
                                          const char* name,
                                          const wpi_upb_FieldDef** f,
                                          const wpi_upb_OneofDef** o) {
  return wpi_upb_MessageDef_FindByNameWithSize(m, name, strlen(name), f, o);
}

const wpi_upb_FieldDef* wpi_upb_MessageDef_FindFieldByName(const wpi_upb_MessageDef* m,
                                                   const char* name);
WPI_UPB_API const wpi_upb_FieldDef* wpi_upb_MessageDef_FindFieldByNameWithSize(
    const wpi_upb_MessageDef* m, const char* name, size_t size);
WPI_UPB_API const wpi_upb_FieldDef* wpi_upb_MessageDef_FindFieldByNumber(
    const wpi_upb_MessageDef* m, uint32_t i);
const wpi_upb_OneofDef* wpi_upb_MessageDef_FindOneofByName(const wpi_upb_MessageDef* m,
                                                   const char* name);
WPI_UPB_API const wpi_upb_OneofDef* wpi_upb_MessageDef_FindOneofByNameWithSize(
    const wpi_upb_MessageDef* m, const char* name, size_t size);
WPI_UPB_API const char* wpi_upb_MessageDef_FullName(const wpi_upb_MessageDef* m);
bool wpi_upb_MessageDef_HasOptions(const wpi_upb_MessageDef* m);
bool wpi_upb_MessageDef_IsMapEntry(const wpi_upb_MessageDef* m);
bool wpi_upb_MessageDef_IsMessageSet(const wpi_upb_MessageDef* m);

// Creates a mini descriptor string for a message, returns true on success.
bool wpi_upb_MessageDef_MiniDescriptorEncode(const wpi_upb_MessageDef* m, wpi_upb_Arena* a,
                                         wpi_upb_StringView* out);

WPI_UPB_API const wpi_upb_MiniTable* wpi_upb_MessageDef_MiniTable(const wpi_upb_MessageDef* m);
const char* wpi_upb_MessageDef_Name(const wpi_upb_MessageDef* m);

const wpi_upb_EnumDef* wpi_upb_MessageDef_NestedEnum(const wpi_upb_MessageDef* m, int i);
const wpi_upb_FieldDef* wpi_upb_MessageDef_NestedExtension(const wpi_upb_MessageDef* m,
                                                   int i);
const wpi_upb_MessageDef* wpi_upb_MessageDef_NestedMessage(const wpi_upb_MessageDef* m,
                                                   int i);

int wpi_upb_MessageDef_NestedEnumCount(const wpi_upb_MessageDef* m);
int wpi_upb_MessageDef_NestedExtensionCount(const wpi_upb_MessageDef* m);
int wpi_upb_MessageDef_NestedMessageCount(const wpi_upb_MessageDef* m);

WPI_UPB_API const wpi_upb_OneofDef* wpi_upb_MessageDef_Oneof(const wpi_upb_MessageDef* m,
                                                 int i);
WPI_UPB_API int wpi_upb_MessageDef_OneofCount(const wpi_upb_MessageDef* m);
int wpi_upb_MessageDef_RealOneofCount(const wpi_upb_MessageDef* m);

const WPI_UPB_DESC(MessageOptions) *
    wpi_upb_MessageDef_Options(const wpi_upb_MessageDef* m);
const WPI_UPB_DESC(FeatureSet) *
    wpi_upb_MessageDef_ResolvedFeatures(const wpi_upb_MessageDef* m);

wpi_upb_StringView wpi_upb_MessageDef_ReservedName(const wpi_upb_MessageDef* m, int i);
int wpi_upb_MessageDef_ReservedNameCount(const wpi_upb_MessageDef* m);

const wpi_upb_MessageReservedRange* wpi_upb_MessageDef_ReservedRange(
    const wpi_upb_MessageDef* m, int i);
int wpi_upb_MessageDef_ReservedRangeCount(const wpi_upb_MessageDef* m);

WPI_UPB_API wpi_upb_Syntax wpi_upb_MessageDef_Syntax(const wpi_upb_MessageDef* m);
WPI_UPB_API wpi_upb_WellKnown wpi_upb_MessageDef_WellKnownType(const wpi_upb_MessageDef* m);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_REFLECTION_MESSAGE_DEF_H_ */

// Must be last.

#ifdef __cplusplus
extern "C" {
#endif

wpi_upb_MessageDef* _wpi_upb_MessageDef_At(const wpi_upb_MessageDef* m, int i);
bool _wpi_upb_MessageDef_InMessageSet(const wpi_upb_MessageDef* m);
bool _wpi_upb_MessageDef_Insert(wpi_upb_MessageDef* m, const char* name, size_t size,
                            wpi_upb_value v, wpi_upb_Arena* a);
void _wpi_upb_MessageDef_InsertField(wpi_upb_DefBuilder* ctx, wpi_upb_MessageDef* m,
                                 const wpi_upb_FieldDef* f);
bool _wpi_upb_MessageDef_IsValidExtensionNumber(const wpi_upb_MessageDef* m, int n);
void _wpi_upb_MessageDef_CreateMiniTable(wpi_upb_DefBuilder* ctx, wpi_upb_MessageDef* m);
void _wpi_upb_MessageDef_LinkMiniTable(wpi_upb_DefBuilder* ctx,
                                   const wpi_upb_MessageDef* m);
void _wpi_upb_MessageDef_Resolve(wpi_upb_DefBuilder* ctx, wpi_upb_MessageDef* m);

// Allocate and initialize an array of |n| message defs.
wpi_upb_MessageDef* _wpi_upb_MessageDefs_New(wpi_upb_DefBuilder* ctx, int n,
                                     const WPI_UPB_DESC(DescriptorProto*)
                                         const* protos,
                                     const WPI_UPB_DESC(FeatureSet*)
                                         parent_features,
                                     const wpi_upb_MessageDef* containing_type);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_REFLECTION_MESSAGE_DEF_INTERNAL_H_ */

#ifndef WPI_UPB_REFLECTION_SERVICE_DEF_INTERNAL_H_
#define WPI_UPB_REFLECTION_SERVICE_DEF_INTERNAL_H_


// IWYU pragma: private, include "upb/reflection/def.h"

#ifndef WPI_UPB_REFLECTION_SERVICE_DEF_H_
#define WPI_UPB_REFLECTION_SERVICE_DEF_H_


// Must be last.

#ifdef __cplusplus
extern "C" {
#endif

WPI_UPB_API const wpi_upb_FileDef* wpi_upb_ServiceDef_File(const wpi_upb_ServiceDef* s);
const wpi_upb_MethodDef* wpi_upb_ServiceDef_FindMethodByName(const wpi_upb_ServiceDef* s,
                                                     const char* name);
WPI_UPB_API const char* wpi_upb_ServiceDef_FullName(const wpi_upb_ServiceDef* s);
bool wpi_upb_ServiceDef_HasOptions(const wpi_upb_ServiceDef* s);
int wpi_upb_ServiceDef_Index(const wpi_upb_ServiceDef* s);
WPI_UPB_API const wpi_upb_MethodDef* wpi_upb_ServiceDef_Method(const wpi_upb_ServiceDef* s,
                                                   int i);
WPI_UPB_API int wpi_upb_ServiceDef_MethodCount(const wpi_upb_ServiceDef* s);
const char* wpi_upb_ServiceDef_Name(const wpi_upb_ServiceDef* s);
WPI_UPB_API const WPI_UPB_DESC(ServiceOptions) *
    wpi_upb_ServiceDef_Options(const wpi_upb_ServiceDef* s);
const WPI_UPB_DESC(FeatureSet) *
    wpi_upb_ServiceDef_ResolvedFeatures(const wpi_upb_ServiceDef* s);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_REFLECTION_SERVICE_DEF_H_ */

// Must be last.

#ifdef __cplusplus
extern "C" {
#endif

wpi_upb_ServiceDef* _wpi_upb_ServiceDef_At(const wpi_upb_ServiceDef* s, int i);

// Allocate and initialize an array of |n| service defs.
wpi_upb_ServiceDef* _wpi_upb_ServiceDefs_New(wpi_upb_DefBuilder* ctx, int n,
                                     const WPI_UPB_DESC(ServiceDescriptorProto*)
                                         const* protos,
                                     const WPI_UPB_DESC(FeatureSet*)
                                         parent_features);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_REFLECTION_SERVICE_DEF_INTERNAL_H_ */

#ifndef WPI_UPB_REFLECTION_WPI_UPB_EDITION_DEFAULTS_H_
#define WPI_UPB_REFLECTION_WPI_UPB_EDITION_DEFAULTS_H_

// This file contains the serialized FeatureSetDefaults object for
// language-independent features and (possibly at some point) for upb-specific
// features. This is used for feature resolution under Editions.
// NOLINTBEGIN
// clang-format off
#define WPI_UPB_INTERNAL_WPI_UPB_EDITION_DEFAULTS "\n\023\030\204\007\"\000*\014\010\001\020\002\030\002 \003(\0010\002\n\023\030\347\007\"\000*\014\010\002\020\001\030\001 \002(\0010\001\n\023\030\350\007\"\014\010\001\020\001\030\001 \002(\0010\001*\000 \346\007(\350\007"
// clang-format on
// NOLINTEND

#endif  // WPI_UPB_REFLECTION_WPI_UPB_EDITION_DEFAULTS_H_

#ifndef WPI_UPB_REFLECTION_DESC_STATE_INTERNAL_H_
#define WPI_UPB_REFLECTION_DESC_STATE_INTERNAL_H_


// Must be last.

// Manages the storage for mini descriptor strings as they are being encoded.
// TODO: Move some of this state directly into the encoder, maybe.
typedef struct {
  wpi_upb_MtDataEncoder e;
  size_t bufsize;
  char* buf;
  char* ptr;
} wpi_upb_DescState;

#ifdef __cplusplus
extern "C" {
#endif

WPI_UPB_INLINE void _wpi_upb_DescState_Init(wpi_upb_DescState* d) {
  d->bufsize = kUpb_MtDataEncoder_MinSize * 2;
  d->buf = NULL;
  d->ptr = NULL;
}

bool _wpi_upb_DescState_Grow(wpi_upb_DescState* d, wpi_upb_Arena* a);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_REFLECTION_DESC_STATE_INTERNAL_H_ */

#ifndef WPI_UPB_REFLECTION_DEF_H_
#define WPI_UPB_REFLECTION_DEF_H_

// IWYU pragma: begin_exports

// IWYU pragma: private, include "upb/reflection/def.h"

#ifndef WPI_UPB_REFLECTION_EXTENSION_RANGE_H_
#define WPI_UPB_REFLECTION_EXTENSION_RANGE_H_


// Must be last.

#ifdef __cplusplus
extern "C" {
#endif

int32_t wpi_upb_ExtensionRange_Start(const wpi_upb_ExtensionRange* r);
int32_t wpi_upb_ExtensionRange_End(const wpi_upb_ExtensionRange* r);

bool wpi_upb_ExtensionRange_HasOptions(const wpi_upb_ExtensionRange* r);
const WPI_UPB_DESC(ExtensionRangeOptions) *
    wpi_upb_ExtensionRange_Options(const wpi_upb_ExtensionRange* r);
const WPI_UPB_DESC(FeatureSet) *
    wpi_upb_ExtensionRange_ResolvedFeatures(const wpi_upb_ExtensionRange* e);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_REFLECTION_EXTENSION_RANGE_H_ */

// IWYU pragma: private, include "upb/reflection/def.h"

#ifndef WPI_UPB_REFLECTION_METHOD_DEF_H_
#define WPI_UPB_REFLECTION_METHOD_DEF_H_


// Must be last.

#ifdef __cplusplus
extern "C" {
#endif

WPI_UPB_API bool wpi_upb_MethodDef_ClientStreaming(const wpi_upb_MethodDef* m);
const char* wpi_upb_MethodDef_FullName(const wpi_upb_MethodDef* m);
bool wpi_upb_MethodDef_HasOptions(const wpi_upb_MethodDef* m);
int wpi_upb_MethodDef_Index(const wpi_upb_MethodDef* m);
WPI_UPB_API const wpi_upb_MessageDef* wpi_upb_MethodDef_InputType(const wpi_upb_MethodDef* m);
WPI_UPB_API const char* wpi_upb_MethodDef_Name(const wpi_upb_MethodDef* m);
WPI_UPB_API const WPI_UPB_DESC(MethodOptions) *
    wpi_upb_MethodDef_Options(const wpi_upb_MethodDef* m);
const WPI_UPB_DESC(FeatureSet) *
    wpi_upb_MethodDef_ResolvedFeatures(const wpi_upb_MethodDef* m);
WPI_UPB_API const wpi_upb_MessageDef* wpi_upb_MethodDef_OutputType(const wpi_upb_MethodDef* m);
WPI_UPB_API bool wpi_upb_MethodDef_ServerStreaming(const wpi_upb_MethodDef* m);
WPI_UPB_API const wpi_upb_ServiceDef* wpi_upb_MethodDef_Service(const wpi_upb_MethodDef* m);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_REFLECTION_METHOD_DEF_H_ */

// IWYU pragma: private, include "upb/reflection/def.h"

#ifndef WPI_UPB_REFLECTION_ONEOF_DEF_H_
#define WPI_UPB_REFLECTION_ONEOF_DEF_H_


// Must be last.

#ifdef __cplusplus
extern "C" {
#endif

WPI_UPB_API const wpi_upb_MessageDef* wpi_upb_OneofDef_ContainingType(
    const wpi_upb_OneofDef* o);
WPI_UPB_API const wpi_upb_FieldDef* wpi_upb_OneofDef_Field(const wpi_upb_OneofDef* o, int i);
WPI_UPB_API int wpi_upb_OneofDef_FieldCount(const wpi_upb_OneofDef* o);
const char* wpi_upb_OneofDef_FullName(const wpi_upb_OneofDef* o);
bool wpi_upb_OneofDef_HasOptions(const wpi_upb_OneofDef* o);
uint32_t wpi_upb_OneofDef_Index(const wpi_upb_OneofDef* o);
bool wpi_upb_OneofDef_IsSynthetic(const wpi_upb_OneofDef* o);
const wpi_upb_FieldDef* wpi_upb_OneofDef_LookupName(const wpi_upb_OneofDef* o,
                                            const char* name);
const wpi_upb_FieldDef* wpi_upb_OneofDef_LookupNameWithSize(const wpi_upb_OneofDef* o,
                                                    const char* name,
                                                    size_t size);
const wpi_upb_FieldDef* wpi_upb_OneofDef_LookupNumber(const wpi_upb_OneofDef* o,
                                              uint32_t num);
WPI_UPB_API const char* wpi_upb_OneofDef_Name(const wpi_upb_OneofDef* o);
int wpi_upb_OneofDef_numfields(const wpi_upb_OneofDef* o);
const WPI_UPB_DESC(OneofOptions*) wpi_upb_OneofDef_Options(const wpi_upb_OneofDef* o);
const WPI_UPB_DESC(FeatureSet*)
    wpi_upb_OneofDef_ResolvedFeatures(const wpi_upb_OneofDef* o);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_REFLECTION_ONEOF_DEF_H_ */
// IWYU pragma: end_exports

#endif /* WPI_UPB_REFLECTION_DEF_H_ */

#ifndef WPI_UPB_REFLECTION_ENUM_RESERVED_RANGE_INTERNAL_H_
#define WPI_UPB_REFLECTION_ENUM_RESERVED_RANGE_INTERNAL_H_


// IWYU pragma: private, include "upb/reflection/def.h"

#ifndef WPI_UPB_REFLECTION_ENUM_RESERVED_RANGE_H_
#define WPI_UPB_REFLECTION_ENUM_RESERVED_RANGE_H_


// Must be last.

#ifdef __cplusplus
extern "C" {
#endif

int32_t wpi_upb_EnumReservedRange_Start(const wpi_upb_EnumReservedRange* r);
int32_t wpi_upb_EnumReservedRange_End(const wpi_upb_EnumReservedRange* r);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_REFLECTION_ENUM_RESERVED_RANGE_H_ */

// Must be last.

#ifdef __cplusplus
extern "C" {
#endif

wpi_upb_EnumReservedRange* _wpi_upb_EnumReservedRange_At(const wpi_upb_EnumReservedRange* r,
                                                 int i);

// Allocate and initialize an array of |n| reserved ranges owned by |e|.
wpi_upb_EnumReservedRange* _wpi_upb_EnumReservedRanges_New(
    wpi_upb_DefBuilder* ctx, int n,
    const WPI_UPB_DESC(EnumDescriptorProto_EnumReservedRange*) const* protos,
    const wpi_upb_EnumDef* e);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_REFLECTION_ENUM_RESERVED_RANGE_INTERNAL_H_ */

#ifndef WPI_UPB_REFLECTION_INTERNAL_STRDUP2_H_
#define WPI_UPB_REFLECTION_INTERNAL_STRDUP2_H_

#include <stddef.h>


// Must be last.

#ifdef __cplusplus
extern "C" {
#endif

// Variant that works with a length-delimited rather than NULL-delimited string,
// as supported by strtable.
char* wpi_upb_strdup2(const char* s, size_t len, wpi_upb_Arena* a);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_REFLECTION_INTERNAL_STRDUP2_H_ */

#ifndef WPI_UPB_REFLECTION_EXTENSION_RANGE_INTERNAL_H_
#define WPI_UPB_REFLECTION_EXTENSION_RANGE_INTERNAL_H_


// Must be last.

#ifdef __cplusplus
extern "C" {
#endif

wpi_upb_ExtensionRange* _wpi_upb_ExtensionRange_At(const wpi_upb_ExtensionRange* r, int i);

// Allocate and initialize an array of |n| extension ranges owned by |m|.
wpi_upb_ExtensionRange* _wpi_upb_ExtensionRanges_New(
    wpi_upb_DefBuilder* ctx, int n,
    const WPI_UPB_DESC(DescriptorProto_ExtensionRange*) const* protos,
    const WPI_UPB_DESC(FeatureSet*) parent_features, const wpi_upb_MessageDef* m);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_REFLECTION_EXTENSION_RANGE_INTERNAL_H_ */

#ifndef WPI_UPB_REFLECTION_ONEOF_DEF_INTERNAL_H_
#define WPI_UPB_REFLECTION_ONEOF_DEF_INTERNAL_H_


// Must be last.

#ifdef __cplusplus
extern "C" {
#endif

wpi_upb_OneofDef* _wpi_upb_OneofDef_At(const wpi_upb_OneofDef* o, int i);
void _wpi_upb_OneofDef_Insert(wpi_upb_DefBuilder* ctx, wpi_upb_OneofDef* o,
                          const wpi_upb_FieldDef* f, const char* name, size_t size);

// Allocate and initialize an array of |n| oneof defs owned by |m|.
wpi_upb_OneofDef* _wpi_upb_OneofDefs_New(wpi_upb_DefBuilder* ctx, int n,
                                 const WPI_UPB_DESC(OneofDescriptorProto*)
                                     const* protos,
                                 const WPI_UPB_DESC(FeatureSet*) parent_features,
                                 wpi_upb_MessageDef* m);

size_t _wpi_upb_OneofDefs_Finalize(wpi_upb_DefBuilder* ctx, wpi_upb_MessageDef* m);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_REFLECTION_ONEOF_DEF_INTERNAL_H_ */

#ifndef WPI_UPB_REFLECTION_MESSAGE_H_
#define WPI_UPB_REFLECTION_MESSAGE_H_

#include <stddef.h>


// Must be last.

#ifdef __cplusplus
extern "C" {
#endif

// Returns a mutable pointer to a map, array, or submessage value. If the given
// arena is non-NULL this will construct a new object if it was not previously
// present. May not be called for primitive fields.
WPI_UPB_API wpi_upb_MutableMessageValue wpi_upb_Message_Mutable(wpi_upb_Message* msg,
                                                    const wpi_upb_FieldDef* f,
                                                    wpi_upb_Arena* a);

// Returns the field that is set in the oneof, or NULL if none are set.
WPI_UPB_API const wpi_upb_FieldDef* wpi_upb_Message_WhichOneofByDef(const wpi_upb_Message* msg,
                                                        const wpi_upb_OneofDef* o);

// Clear all data and unknown fields.
void wpi_upb_Message_ClearByDef(wpi_upb_Message* msg, const wpi_upb_MessageDef* m);

// Clears any field presence and sets the value back to its default.
WPI_UPB_API void wpi_upb_Message_ClearFieldByDef(wpi_upb_Message* msg,
                                         const wpi_upb_FieldDef* f);

// May only be called for fields where wpi_upb_FieldDef_HasPresence(f) == true.
WPI_UPB_API bool wpi_upb_Message_HasFieldByDef(const wpi_upb_Message* msg,
                                       const wpi_upb_FieldDef* f);

// Returns the value in the message associated with this field def.
WPI_UPB_API wpi_upb_MessageValue wpi_upb_Message_GetFieldByDef(const wpi_upb_Message* msg,
                                                   const wpi_upb_FieldDef* f);

// Sets the given field to the given value. For a msg/array/map/string, the
// caller must ensure that the target data outlives |msg| (by living either in
// the same arena or a different arena that outlives it).
//
// Returns false if allocation fails.
WPI_UPB_API bool wpi_upb_Message_SetFieldByDef(wpi_upb_Message* msg, const wpi_upb_FieldDef* f,
                                       wpi_upb_MessageValue val, wpi_upb_Arena* a);

// Iterate over present fields.
//
// size_t iter = kUpb_Message_Begin;
// const wpi_upb_FieldDef *f;
// wpi_upb_MessageValue val;
// while (wpi_upb_Message_Next(msg, m, ext_pool, &f, &val, &iter)) {
//   process_field(f, val);
// }
//
// If ext_pool is NULL, no extensions will be returned.  If the given symtab
// returns extensions that don't match what is in this message, those extensions
// will be skipped.

#define kUpb_Message_Begin -1

WPI_UPB_API bool wpi_upb_Message_Next(const wpi_upb_Message* msg, const wpi_upb_MessageDef* m,
                              const wpi_upb_DefPool* ext_pool,
                              const wpi_upb_FieldDef** f, wpi_upb_MessageValue* val,
                              size_t* iter);

// Clears all unknown field data from this message and all submessages.
WPI_UPB_API bool wpi_upb_Message_DiscardUnknown(wpi_upb_Message* msg,
                                        const wpi_upb_MessageDef* m, int maxdepth);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_REFLECTION_MESSAGE_H_ */

#ifndef WPI_UPB_REFLECTION_MESSAGE_RESERVED_RANGE_INTERNAL_H_
#define WPI_UPB_REFLECTION_MESSAGE_RESERVED_RANGE_INTERNAL_H_


// IWYU pragma: private, include "upb/reflection/def.h"

#ifndef WPI_UPB_REFLECTION_MESSAGE_RESERVED_RANGE_H_
#define WPI_UPB_REFLECTION_MESSAGE_RESERVED_RANGE_H_


// Must be last.

#ifdef __cplusplus
extern "C" {
#endif

int32_t wpi_upb_MessageReservedRange_Start(const wpi_upb_MessageReservedRange* r);
int32_t wpi_upb_MessageReservedRange_End(const wpi_upb_MessageReservedRange* r);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_REFLECTION_MESSAGE_RESERVED_RANGE_H_ */

// Must be last.

#ifdef __cplusplus
extern "C" {
#endif

wpi_upb_MessageReservedRange* _wpi_upb_MessageReservedRange_At(
    const wpi_upb_MessageReservedRange* r, int i);

// Allocate and initialize an array of |n| reserved ranges owned by |m|.
wpi_upb_MessageReservedRange* _wpi_upb_MessageReservedRanges_New(
    wpi_upb_DefBuilder* ctx, int n,
    const WPI_UPB_DESC(DescriptorProto_ReservedRange) * const* protos,
    const wpi_upb_MessageDef* m);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_REFLECTION_MESSAGE_RESERVED_RANGE_INTERNAL_H_ */

#ifndef WPI_UPB_REFLECTION_METHOD_DEF_INTERNAL_H_
#define WPI_UPB_REFLECTION_METHOD_DEF_INTERNAL_H_


// Must be last.

#ifdef __cplusplus
extern "C" {
#endif

wpi_upb_MethodDef* _wpi_upb_MethodDef_At(const wpi_upb_MethodDef* m, int i);

// Allocate and initialize an array of |n| method defs owned by |s|.
wpi_upb_MethodDef* _wpi_upb_MethodDefs_New(wpi_upb_DefBuilder* ctx, int n,
                                   const WPI_UPB_DESC(MethodDescriptorProto*)
                                       const* protos,
                                   const WPI_UPB_DESC(FeatureSet*) parent_features,
                                   wpi_upb_ServiceDef* s);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* WPI_UPB_REFLECTION_METHOD_DEF_INTERNAL_H_ */

// This should #undef all macros #defined in def.inc

#ifdef __GCC__
#pragma GCC diagnostic pop
#endif
#ifdef __clang__
#pragma clang diagnostic pop
#endif

#undef WPI_UPB_SIZE
#undef WPI_UPB_PTR_AT
#undef WPI_UPB_MAPTYPE_STRING
#undef WPI_UPB_EXPORT
#undef WPI_UPB_INLINE
#undef WPI_UPB_API
#undef WPI_UPB_API_INLINE
#undef WPI_UPB_ALIGN_UP
#undef WPI_UPB_ALIGN_DOWN
#undef WPI_UPB_ALIGN_MALLOC
#undef WPI_UPB_ALIGN_OF
#undef WPI_UPB_ALIGN_AS
#undef WPI_UPB_MALLOC_ALIGN
#undef WPI_UPB_LIKELY
#undef WPI_UPB_UNLIKELY
#undef WPI_UPB_FORCEINLINE
#undef WPI_UPB_NOINLINE
#undef WPI_UPB_NORETURN
#undef WPI_UPB_PRINTF
#undef WPI_UPB_MAX
#undef WPI_UPB_MIN
#undef WPI_UPB_UNUSED
#undef WPI_UPB_ASSUME
#undef WPI_UPB_ASSERT
#undef WPI_UPB_UNREACHABLE
#undef WPI_UPB_SETJMP
#undef WPI_UPB_LONGJMP
#undef WPI_UPB_PTRADD
#undef WPI_UPB_MUSTTAIL
#undef WPI_UPB_FASTTABLE_SUPPORTED
#undef WPI_UPB_FASTTABLE_MASK
#undef WPI_UPB_FASTTABLE
#undef WPI_UPB_FASTTABLE_INIT
#undef WPI_UPB_POISON_MEMORY_REGION
#undef WPI_UPB_UNPOISON_MEMORY_REGION
#undef WPI_UPB_ASAN
#undef WPI_UPB_ASAN_GUARD_SIZE
#undef WPI_UPB_CLANG_ASAN
#undef WPI_UPB_TREAT_CLOSED_ENUMS_LIKE_OPEN
#undef WPI_UPB_DEPRECATED
#undef WPI_UPB_GNUC_MIN
#undef WPI_UPB_DESCRIPTOR_WPI_UPB_H_FILENAME
#undef WPI_UPB_DESC
#undef WPI_UPB_DESC_MINITABLE
#undef WPI_UPB_IS_GOOGLE3
#undef WPI_UPB_ATOMIC
#undef WPI_UPB_USE_C11_ATOMICS
#undef WPI_UPB_PRIVATE
#undef WPI_UPB_ONLYBITS
#undef WPI_UPB_LINKARR_DECLARE
#undef WPI_UPB_LINKARR_APPEND
#undef WPI_UPB_LINKARR_START
#undef WPI_UPB_LINKARR_STOP
