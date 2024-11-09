/* Amalgamated source file */
#include "wpi/upb.h"

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


#include <errno.h>
#include <float.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

// Must be last.

void wpi_upb_Status_Clear(wpi_upb_Status* status) {
  if (!status) return;
  status->ok = true;
  status->msg[0] = '\0';
}

bool wpi_upb_Status_IsOk(const wpi_upb_Status* status) { return status->ok; }

const char* wpi_upb_Status_ErrorMessage(const wpi_upb_Status* status) {
  return status->msg;
}

void wpi_upb_Status_SetErrorMessage(wpi_upb_Status* status, const char* msg) {
  if (!status) return;
  status->ok = false;
  strncpy(status->msg, msg, _kUpb_Status_MaxMessage - 1);
  status->msg[_kUpb_Status_MaxMessage - 1] = '\0';
}

void wpi_upb_Status_SetErrorFormat(wpi_upb_Status* status, const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  wpi_upb_Status_VSetErrorFormat(status, fmt, args);
  va_end(args);
}

void wpi_upb_Status_VSetErrorFormat(wpi_upb_Status* status, const char* fmt,
                                va_list args) {
  if (!status) return;
  status->ok = false;
  vsnprintf(status->msg, sizeof(status->msg), fmt, args);
  status->msg[_kUpb_Status_MaxMessage - 1] = '\0';
}

void wpi_upb_Status_VAppendErrorFormat(wpi_upb_Status* status, const char* fmt,
                                   va_list args) {
  size_t len;
  if (!status) return;
  status->ok = false;
  len = strlen(status->msg);
  vsnprintf(status->msg + len, sizeof(status->msg) - len, fmt, args);
  status->msg[_kUpb_Status_MaxMessage - 1] = '\0';
}
/* This file was generated by wpi_upb_generator from the input file:
 *
 *     google/protobuf/descriptor.proto
 *
 * Do not edit -- your changes will be discarded when the file is
 * regenerated.
 * NO CHECKED-IN PROTOBUF GENCODE */

#include <stddef.h>

// Must be last.

extern const struct wpi_upb_MiniTable WPI_UPB_PRIVATE(_kUpb_MiniTable_StaticallyTreeShaken);
static const wpi_upb_MiniTableSubInternal google_protobuf_FileDescriptorSet_submsgs[1] = {
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__FileDescriptorProto_msg_init_ptr},
};

static const wpi_upb_MiniTableField google_protobuf_FileDescriptorSet__fields[1] = {
  {1, 8, 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
};

const wpi_upb_MiniTable google__protobuf__FileDescriptorSet_msg_init = {
  &google_protobuf_FileDescriptorSet_submsgs[0],
  &google_protobuf_FileDescriptorSet__fields[0],
  16, 1, kUpb_ExtMode_NonExtendable, 1, WPI_UPB_FASTTABLE_MASK(8), 0,
#ifdef WPI_UPB_TRACING_ENABLED
  "google.protobuf.FileDescriptorSet",
#endif
  WPI_UPB_FASTTABLE_INIT({
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x000800003f00000a, &wpi_upb_prm_1bt_max192b},
  })
};

const wpi_upb_MiniTable* google__protobuf__FileDescriptorSet_msg_init_ptr = &google__protobuf__FileDescriptorSet_msg_init;
static const wpi_upb_MiniTableSubInternal google_protobuf_FileDescriptorProto_submsgs[7] = {
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__DescriptorProto_msg_init_ptr},
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__EnumDescriptorProto_msg_init_ptr},
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__ServiceDescriptorProto_msg_init_ptr},
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__FieldDescriptorProto_msg_init_ptr},
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__FileOptions_msg_init_ptr},
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__SourceCodeInfo_msg_init_ptr},
  {.WPI_UPB_PRIVATE(subenum) = &google_protobuf_Edition_enum_init},
};

static const wpi_upb_MiniTableField google_protobuf_FileDescriptorProto__fields[13] = {
  {1, WPI_UPB_SIZE(52, 16), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)},
  {2, WPI_UPB_SIZE(60, 32), 65, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)},
  {3, WPI_UPB_SIZE(12, 48), 0, kUpb_NoSub, 12, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsAlternate | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
  {4, WPI_UPB_SIZE(16, 56), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
  {5, WPI_UPB_SIZE(20, 64), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
  {6, WPI_UPB_SIZE(24, 72), 0, 2, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
  {7, WPI_UPB_SIZE(28, 80), 0, 3, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
  {8, WPI_UPB_SIZE(32, 88), 66, 4, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
  {9, WPI_UPB_SIZE(36, 96), 67, 5, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
  {10, WPI_UPB_SIZE(40, 104), 0, kUpb_NoSub, 5, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
  {11, WPI_UPB_SIZE(44, 112), 0, kUpb_NoSub, 5, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
  {12, WPI_UPB_SIZE(68, 120), 68, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)},
  {14, WPI_UPB_SIZE(48, 12), 69, 6, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)},
};

const wpi_upb_MiniTable google__protobuf__FileDescriptorProto_msg_init = {
  &google_protobuf_FileDescriptorProto_submsgs[0],
  &google_protobuf_FileDescriptorProto__fields[0],
  WPI_UPB_SIZE(80, 136), 13, kUpb_ExtMode_NonExtendable, 12, WPI_UPB_FASTTABLE_MASK(120), 0,
#ifdef WPI_UPB_TRACING_ENABLED
  "google.protobuf.FileDescriptorProto",
#endif
  WPI_UPB_FASTTABLE_INIT({
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x003000003f00001a, &wpi_upb_prs_1bt},
    {0x003800003f000022, &wpi_upb_prm_1bt_max128b},
    {0x004000003f01002a, &wpi_upb_prm_1bt_max128b},
    {0x004800003f020032, &wpi_upb_prm_1bt_max64b},
    {0x005000003f03003a, &wpi_upb_prm_1bt_max128b},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x006800003f000050, &wpi_upb_prv4_1bt},
    {0x007000003f000058, &wpi_upb_prv4_1bt},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
  })
};

const wpi_upb_MiniTable* google__protobuf__FileDescriptorProto_msg_init_ptr = &google__protobuf__FileDescriptorProto_msg_init;
static const wpi_upb_MiniTableSubInternal google_protobuf_DescriptorProto_submsgs[8] = {
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__FieldDescriptorProto_msg_init_ptr},
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__DescriptorProto_msg_init_ptr},
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__EnumDescriptorProto_msg_init_ptr},
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__DescriptorProto__ExtensionRange_msg_init_ptr},
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__FieldDescriptorProto_msg_init_ptr},
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__MessageOptions_msg_init_ptr},
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__OneofDescriptorProto_msg_init_ptr},
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__DescriptorProto__ReservedRange_msg_init_ptr},
};

static const wpi_upb_MiniTableField google_protobuf_DescriptorProto__fields[10] = {
  {1, WPI_UPB_SIZE(48, 16), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)},
  {2, WPI_UPB_SIZE(12, 32), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
  {3, WPI_UPB_SIZE(16, 40), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
  {4, WPI_UPB_SIZE(20, 48), 0, 2, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
  {5, WPI_UPB_SIZE(24, 56), 0, 3, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
  {6, WPI_UPB_SIZE(28, 64), 0, 4, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
  {7, WPI_UPB_SIZE(32, 72), 65, 5, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
  {8, WPI_UPB_SIZE(36, 80), 0, 6, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
  {9, WPI_UPB_SIZE(40, 88), 0, 7, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
  {10, WPI_UPB_SIZE(44, 96), 0, kUpb_NoSub, 12, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsAlternate | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
};

const wpi_upb_MiniTable google__protobuf__DescriptorProto_msg_init = {
  &google_protobuf_DescriptorProto_submsgs[0],
  &google_protobuf_DescriptorProto__fields[0],
  WPI_UPB_SIZE(56, 104), 10, kUpb_ExtMode_NonExtendable, 10, WPI_UPB_FASTTABLE_MASK(120), 0,
#ifdef WPI_UPB_TRACING_ENABLED
  "google.protobuf.DescriptorProto",
#endif
  WPI_UPB_FASTTABLE_INIT({
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x002000003f000012, &wpi_upb_prm_1bt_max128b},
    {0x002800003f01001a, &wpi_upb_prm_1bt_max128b},
    {0x003000003f020022, &wpi_upb_prm_1bt_max128b},
    {0x003800003f03002a, &wpi_upb_prm_1bt_max64b},
    {0x004000003f040032, &wpi_upb_prm_1bt_max128b},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x005000003f060042, &wpi_upb_prm_1bt_max64b},
    {0x005800003f07004a, &wpi_upb_prm_1bt_max64b},
    {0x006000003f000052, &wpi_upb_prs_1bt},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
  })
};

const wpi_upb_MiniTable* google__protobuf__DescriptorProto_msg_init_ptr = &google__protobuf__DescriptorProto_msg_init;
static const wpi_upb_MiniTableSubInternal google_protobuf_DescriptorProto_ExtensionRange_submsgs[1] = {
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__ExtensionRangeOptions_msg_init_ptr},
};

static const wpi_upb_MiniTableField google_protobuf_DescriptorProto_ExtensionRange__fields[3] = {
  {1, 12, 64, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)},
  {2, 16, 65, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)},
  {3, WPI_UPB_SIZE(20, 24), 66, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
};

const wpi_upb_MiniTable google__protobuf__DescriptorProto__ExtensionRange_msg_init = {
  &google_protobuf_DescriptorProto_ExtensionRange_submsgs[0],
  &google_protobuf_DescriptorProto_ExtensionRange__fields[0],
  WPI_UPB_SIZE(24, 32), 3, kUpb_ExtMode_NonExtendable, 3, WPI_UPB_FASTTABLE_MASK(255), 0,
#ifdef WPI_UPB_TRACING_ENABLED
  "google.protobuf.DescriptorProto.ExtensionRange",
#endif
};

const wpi_upb_MiniTable* google__protobuf__DescriptorProto__ExtensionRange_msg_init_ptr = &google__protobuf__DescriptorProto__ExtensionRange_msg_init;
static const wpi_upb_MiniTableField google_protobuf_DescriptorProto_ReservedRange__fields[2] = {
  {1, 12, 64, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)},
  {2, 16, 65, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)},
};

const wpi_upb_MiniTable google__protobuf__DescriptorProto__ReservedRange_msg_init = {
  NULL,
  &google_protobuf_DescriptorProto_ReservedRange__fields[0],
  24, 2, kUpb_ExtMode_NonExtendable, 2, WPI_UPB_FASTTABLE_MASK(255), 0,
#ifdef WPI_UPB_TRACING_ENABLED
  "google.protobuf.DescriptorProto.ReservedRange",
#endif
};

const wpi_upb_MiniTable* google__protobuf__DescriptorProto__ReservedRange_msg_init_ptr = &google__protobuf__DescriptorProto__ReservedRange_msg_init;
static const wpi_upb_MiniTableSubInternal google_protobuf_ExtensionRangeOptions_submsgs[4] = {
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__ExtensionRangeOptions__Declaration_msg_init_ptr},
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__FeatureSet_msg_init_ptr},
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__UninterpretedOption_msg_init_ptr},
  {.WPI_UPB_PRIVATE(subenum) = &google_protobuf_ExtensionRangeOptions_VerificationState_enum_init},
};

static const wpi_upb_MiniTableField google_protobuf_ExtensionRangeOptions__fields[4] = {
  {2, WPI_UPB_SIZE(12, 16), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
  {3, WPI_UPB_SIZE(16, 12), 64, 3, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)},
  {50, WPI_UPB_SIZE(20, 24), 65, 1, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
  {999, WPI_UPB_SIZE(24, 32), 0, 2, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
};

const wpi_upb_MiniTable google__protobuf__ExtensionRangeOptions_msg_init = {
  &google_protobuf_ExtensionRangeOptions_submsgs[0],
  &google_protobuf_ExtensionRangeOptions__fields[0],
  WPI_UPB_SIZE(32, 40), 4, kUpb_ExtMode_Extendable, 0, WPI_UPB_FASTTABLE_MASK(248), 0,
#ifdef WPI_UPB_TRACING_ENABLED
  "google.protobuf.ExtensionRangeOptions",
#endif
  WPI_UPB_FASTTABLE_INIT({
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x001000003f000012, &wpi_upb_prm_1bt_max64b},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x002000003f023eba, &wpi_upb_prm_2bt_max128b},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
  })
};

const wpi_upb_MiniTable* google__protobuf__ExtensionRangeOptions_msg_init_ptr = &google__protobuf__ExtensionRangeOptions_msg_init;
static const wpi_upb_MiniTableField google_protobuf_ExtensionRangeOptions_Declaration__fields[5] = {
  {1, 12, 64, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)},
  {2, WPI_UPB_SIZE(20, 24), 65, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)},
  {3, WPI_UPB_SIZE(28, 40), 66, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)},
  {5, 16, 67, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)},
  {6, 17, 68, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)},
};

const wpi_upb_MiniTable google__protobuf__ExtensionRangeOptions__Declaration_msg_init = {
  NULL,
  &google_protobuf_ExtensionRangeOptions_Declaration__fields[0],
  WPI_UPB_SIZE(40, 56), 5, kUpb_ExtMode_NonExtendable, 3, WPI_UPB_FASTTABLE_MASK(255), 0,
#ifdef WPI_UPB_TRACING_ENABLED
  "google.protobuf.ExtensionRangeOptions.Declaration",
#endif
};

const wpi_upb_MiniTable* google__protobuf__ExtensionRangeOptions__Declaration_msg_init_ptr = &google__protobuf__ExtensionRangeOptions__Declaration_msg_init;
static const wpi_upb_MiniTableSubInternal google_protobuf_FieldDescriptorProto_submsgs[3] = {
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__FieldOptions_msg_init_ptr},
  {.WPI_UPB_PRIVATE(subenum) = &google_protobuf_FieldDescriptorProto_Label_enum_init},
  {.WPI_UPB_PRIVATE(subenum) = &google_protobuf_FieldDescriptorProto_Type_enum_init},
};

static const wpi_upb_MiniTableField google_protobuf_FieldDescriptorProto__fields[11] = {
  {1, WPI_UPB_SIZE(36, 32), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)},
  {2, WPI_UPB_SIZE(44, 48), 65, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)},
  {3, 12, 66, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)},
  {4, 16, 67, 1, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)},
  {5, 20, 68, 2, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)},
  {6, WPI_UPB_SIZE(52, 64), 69, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)},
  {7, WPI_UPB_SIZE(60, 80), 70, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)},
  {8, WPI_UPB_SIZE(24, 96), 71, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
  {9, WPI_UPB_SIZE(28, 24), 72, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)},
  {10, WPI_UPB_SIZE(68, 104), 73, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)},
  {17, WPI_UPB_SIZE(32, 28), 74, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)},
};

const wpi_upb_MiniTable google__protobuf__FieldDescriptorProto_msg_init = {
  &google_protobuf_FieldDescriptorProto_submsgs[0],
  &google_protobuf_FieldDescriptorProto__fields[0],
  WPI_UPB_SIZE(80, 120), 11, kUpb_ExtMode_NonExtendable, 10, WPI_UPB_FASTTABLE_MASK(255), 0,
#ifdef WPI_UPB_TRACING_ENABLED
  "google.protobuf.FieldDescriptorProto",
#endif
};

const wpi_upb_MiniTable* google__protobuf__FieldDescriptorProto_msg_init_ptr = &google__protobuf__FieldDescriptorProto_msg_init;
static const wpi_upb_MiniTableSubInternal google_protobuf_OneofDescriptorProto_submsgs[1] = {
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__OneofOptions_msg_init_ptr},
};

static const wpi_upb_MiniTableField google_protobuf_OneofDescriptorProto__fields[2] = {
  {1, 16, 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)},
  {2, WPI_UPB_SIZE(12, 32), 65, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
};

const wpi_upb_MiniTable google__protobuf__OneofDescriptorProto_msg_init = {
  &google_protobuf_OneofDescriptorProto_submsgs[0],
  &google_protobuf_OneofDescriptorProto__fields[0],
  WPI_UPB_SIZE(24, 40), 2, kUpb_ExtMode_NonExtendable, 2, WPI_UPB_FASTTABLE_MASK(255), 0,
#ifdef WPI_UPB_TRACING_ENABLED
  "google.protobuf.OneofDescriptorProto",
#endif
};

const wpi_upb_MiniTable* google__protobuf__OneofDescriptorProto_msg_init_ptr = &google__protobuf__OneofDescriptorProto_msg_init;
static const wpi_upb_MiniTableSubInternal google_protobuf_EnumDescriptorProto_submsgs[3] = {
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__EnumValueDescriptorProto_msg_init_ptr},
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__EnumOptions_msg_init_ptr},
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__EnumDescriptorProto__EnumReservedRange_msg_init_ptr},
};

static const wpi_upb_MiniTableField google_protobuf_EnumDescriptorProto__fields[5] = {
  {1, WPI_UPB_SIZE(28, 16), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)},
  {2, WPI_UPB_SIZE(12, 32), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
  {3, WPI_UPB_SIZE(16, 40), 65, 1, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
  {4, WPI_UPB_SIZE(20, 48), 0, 2, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
  {5, WPI_UPB_SIZE(24, 56), 0, kUpb_NoSub, 12, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsAlternate | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
};

const wpi_upb_MiniTable google__protobuf__EnumDescriptorProto_msg_init = {
  &google_protobuf_EnumDescriptorProto_submsgs[0],
  &google_protobuf_EnumDescriptorProto__fields[0],
  WPI_UPB_SIZE(40, 64), 5, kUpb_ExtMode_NonExtendable, 5, WPI_UPB_FASTTABLE_MASK(56), 0,
#ifdef WPI_UPB_TRACING_ENABLED
  "google.protobuf.EnumDescriptorProto",
#endif
  WPI_UPB_FASTTABLE_INIT({
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x002000003f000012, &wpi_upb_prm_1bt_max64b},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x003000003f020022, &wpi_upb_prm_1bt_max64b},
    {0x003800003f00002a, &wpi_upb_prs_1bt},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
  })
};

const wpi_upb_MiniTable* google__protobuf__EnumDescriptorProto_msg_init_ptr = &google__protobuf__EnumDescriptorProto_msg_init;
static const wpi_upb_MiniTableField google_protobuf_EnumDescriptorProto_EnumReservedRange__fields[2] = {
  {1, 12, 64, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)},
  {2, 16, 65, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)},
};

const wpi_upb_MiniTable google__protobuf__EnumDescriptorProto__EnumReservedRange_msg_init = {
  NULL,
  &google_protobuf_EnumDescriptorProto_EnumReservedRange__fields[0],
  24, 2, kUpb_ExtMode_NonExtendable, 2, WPI_UPB_FASTTABLE_MASK(255), 0,
#ifdef WPI_UPB_TRACING_ENABLED
  "google.protobuf.EnumDescriptorProto.EnumReservedRange",
#endif
};

const wpi_upb_MiniTable* google__protobuf__EnumDescriptorProto__EnumReservedRange_msg_init_ptr = &google__protobuf__EnumDescriptorProto__EnumReservedRange_msg_init;
static const wpi_upb_MiniTableSubInternal google_protobuf_EnumValueDescriptorProto_submsgs[1] = {
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__EnumValueOptions_msg_init_ptr},
};

static const wpi_upb_MiniTableField google_protobuf_EnumValueDescriptorProto__fields[3] = {
  {1, WPI_UPB_SIZE(20, 16), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)},
  {2, 12, 65, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)},
  {3, WPI_UPB_SIZE(16, 32), 66, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
};

const wpi_upb_MiniTable google__protobuf__EnumValueDescriptorProto_msg_init = {
  &google_protobuf_EnumValueDescriptorProto_submsgs[0],
  &google_protobuf_EnumValueDescriptorProto__fields[0],
  WPI_UPB_SIZE(32, 40), 3, kUpb_ExtMode_NonExtendable, 3, WPI_UPB_FASTTABLE_MASK(255), 0,
#ifdef WPI_UPB_TRACING_ENABLED
  "google.protobuf.EnumValueDescriptorProto",
#endif
};

const wpi_upb_MiniTable* google__protobuf__EnumValueDescriptorProto_msg_init_ptr = &google__protobuf__EnumValueDescriptorProto_msg_init;
static const wpi_upb_MiniTableSubInternal google_protobuf_ServiceDescriptorProto_submsgs[2] = {
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__MethodDescriptorProto_msg_init_ptr},
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__ServiceOptions_msg_init_ptr},
};

static const wpi_upb_MiniTableField google_protobuf_ServiceDescriptorProto__fields[3] = {
  {1, WPI_UPB_SIZE(20, 16), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)},
  {2, WPI_UPB_SIZE(12, 32), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
  {3, WPI_UPB_SIZE(16, 40), 65, 1, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
};

const wpi_upb_MiniTable google__protobuf__ServiceDescriptorProto_msg_init = {
  &google_protobuf_ServiceDescriptorProto_submsgs[0],
  &google_protobuf_ServiceDescriptorProto__fields[0],
  WPI_UPB_SIZE(32, 48), 3, kUpb_ExtMode_NonExtendable, 3, WPI_UPB_FASTTABLE_MASK(24), 0,
#ifdef WPI_UPB_TRACING_ENABLED
  "google.protobuf.ServiceDescriptorProto",
#endif
  WPI_UPB_FASTTABLE_INIT({
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x002000003f000012, &wpi_upb_prm_1bt_max128b},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
  })
};

const wpi_upb_MiniTable* google__protobuf__ServiceDescriptorProto_msg_init_ptr = &google__protobuf__ServiceDescriptorProto_msg_init;
static const wpi_upb_MiniTableSubInternal google_protobuf_MethodDescriptorProto_submsgs[1] = {
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__MethodOptions_msg_init_ptr},
};

static const wpi_upb_MiniTableField google_protobuf_MethodDescriptorProto__fields[6] = {
  {1, WPI_UPB_SIZE(20, 16), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)},
  {2, WPI_UPB_SIZE(28, 32), 65, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)},
  {3, WPI_UPB_SIZE(36, 48), 66, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)},
  {4, WPI_UPB_SIZE(12, 64), 67, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
  {5, WPI_UPB_SIZE(16, 9), 68, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)},
  {6, WPI_UPB_SIZE(17, 10), 69, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)},
};

const wpi_upb_MiniTable google__protobuf__MethodDescriptorProto_msg_init = {
  &google_protobuf_MethodDescriptorProto_submsgs[0],
  &google_protobuf_MethodDescriptorProto__fields[0],
  WPI_UPB_SIZE(48, 72), 6, kUpb_ExtMode_NonExtendable, 6, WPI_UPB_FASTTABLE_MASK(255), 0,
#ifdef WPI_UPB_TRACING_ENABLED
  "google.protobuf.MethodDescriptorProto",
#endif
};

const wpi_upb_MiniTable* google__protobuf__MethodDescriptorProto_msg_init_ptr = &google__protobuf__MethodDescriptorProto_msg_init;
static const wpi_upb_MiniTableSubInternal google_protobuf_FileOptions_submsgs[3] = {
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__FeatureSet_msg_init_ptr},
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__UninterpretedOption_msg_init_ptr},
  {.WPI_UPB_PRIVATE(subenum) = &google_protobuf_FileOptions_OptimizeMode_enum_init},
};

static const wpi_upb_MiniTableField google_protobuf_FileOptions__fields[21] = {
  {1, WPI_UPB_SIZE(32, 24), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)},
  {8, 40, 65, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)},
  {9, 12, 66, 2, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)},
  {10, 16, 67, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)},
  {11, WPI_UPB_SIZE(48, 56), 68, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)},
  {16, 17, 69, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)},
  {17, 18, 70, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)},
  {18, 19, 71, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)},
  {20, 20, 72, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)},
  {23, 21, 73, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)},
  {27, 22, 74, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)},
  {31, 23, 75, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)},
  {36, WPI_UPB_SIZE(56, 72), 76, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)},
  {37, WPI_UPB_SIZE(64, 88), 77, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)},
  {39, WPI_UPB_SIZE(72, 104), 78, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)},
  {40, WPI_UPB_SIZE(80, 120), 79, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)},
  {41, WPI_UPB_SIZE(88, 136), 80, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)},
  {44, WPI_UPB_SIZE(96, 152), 81, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)},
  {45, WPI_UPB_SIZE(104, 168), 82, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)},
  {50, WPI_UPB_SIZE(24, 184), 83, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
  {999, WPI_UPB_SIZE(28, 192), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
};

const wpi_upb_MiniTable google__protobuf__FileOptions_msg_init = {
  &google_protobuf_FileOptions_submsgs[0],
  &google_protobuf_FileOptions__fields[0],
  WPI_UPB_SIZE(112, 200), 21, kUpb_ExtMode_Extendable, 1, WPI_UPB_FASTTABLE_MASK(248), 0,
#ifdef WPI_UPB_TRACING_ENABLED
  "google.protobuf.FileOptions",
#endif
  WPI_UPB_FASTTABLE_INIT({
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x00c000003f013eba, &wpi_upb_prm_2bt_max128b},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
  })
};

const wpi_upb_MiniTable* google__protobuf__FileOptions_msg_init_ptr = &google__protobuf__FileOptions_msg_init;
static const wpi_upb_MiniTableSubInternal google_protobuf_MessageOptions_submsgs[2] = {
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__FeatureSet_msg_init_ptr},
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__UninterpretedOption_msg_init_ptr},
};

static const wpi_upb_MiniTableField google_protobuf_MessageOptions__fields[7] = {
  {1, 9, 64, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)},
  {2, 10, 65, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)},
  {3, 11, 66, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)},
  {7, 12, 67, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)},
  {11, 13, 68, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)},
  {12, 16, 69, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
  {999, WPI_UPB_SIZE(20, 24), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
};

const wpi_upb_MiniTable google__protobuf__MessageOptions_msg_init = {
  &google_protobuf_MessageOptions_submsgs[0],
  &google_protobuf_MessageOptions__fields[0],
  WPI_UPB_SIZE(24, 32), 7, kUpb_ExtMode_Extendable, 3, WPI_UPB_FASTTABLE_MASK(248), 0,
#ifdef WPI_UPB_TRACING_ENABLED
  "google.protobuf.MessageOptions",
#endif
  WPI_UPB_FASTTABLE_INIT({
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x001800003f013eba, &wpi_upb_prm_2bt_max128b},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
  })
};

const wpi_upb_MiniTable* google__protobuf__MessageOptions_msg_init_ptr = &google__protobuf__MessageOptions_msg_init;
static const wpi_upb_MiniTableSubInternal google_protobuf_FieldOptions_submsgs[8] = {
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__FieldOptions__EditionDefault_msg_init_ptr},
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__FeatureSet_msg_init_ptr},
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__FieldOptions__FeatureSupport_msg_init_ptr},
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__UninterpretedOption_msg_init_ptr},
  {.WPI_UPB_PRIVATE(subenum) = &google_protobuf_FieldOptions_CType_enum_init},
  {.WPI_UPB_PRIVATE(subenum) = &google_protobuf_FieldOptions_JSType_enum_init},
  {.WPI_UPB_PRIVATE(subenum) = &google_protobuf_FieldOptions_OptionRetention_enum_init},
  {.WPI_UPB_PRIVATE(subenum) = &google_protobuf_FieldOptions_OptionTargetType_enum_init},
};

static const wpi_upb_MiniTableField google_protobuf_FieldOptions__fields[14] = {
  {1, 12, 64, 4, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)},
  {2, 16, 65, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)},
  {3, 17, 66, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)},
  {5, 18, 67, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)},
  {6, 20, 68, 5, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)},
  {10, 24, 69, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)},
  {15, 25, 70, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)},
  {16, 26, 71, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)},
  {17, 28, 72, 6, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)},
  {19, 32, 0, 7, 14, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
  {20, WPI_UPB_SIZE(36, 40), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
  {21, WPI_UPB_SIZE(40, 48), 73, 1, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
  {22, WPI_UPB_SIZE(44, 56), 74, 2, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
  {999, WPI_UPB_SIZE(48, 64), 0, 3, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
};

const wpi_upb_MiniTable google__protobuf__FieldOptions_msg_init = {
  &google_protobuf_FieldOptions_submsgs[0],
  &google_protobuf_FieldOptions__fields[0],
  WPI_UPB_SIZE(56, 72), 14, kUpb_ExtMode_Extendable, 3, WPI_UPB_FASTTABLE_MASK(248), 0,
#ifdef WPI_UPB_TRACING_ENABLED
  "google.protobuf.FieldOptions",
#endif
  WPI_UPB_FASTTABLE_INIT({
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x002800003f0001a2, &wpi_upb_prm_2bt_max64b},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x004000003f033eba, &wpi_upb_prm_2bt_max128b},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
  })
};

const wpi_upb_MiniTable* google__protobuf__FieldOptions_msg_init_ptr = &google__protobuf__FieldOptions_msg_init;
static const wpi_upb_MiniTableSubInternal google_protobuf_FieldOptions_EditionDefault_submsgs[1] = {
  {.WPI_UPB_PRIVATE(subenum) = &google_protobuf_Edition_enum_init},
};

static const wpi_upb_MiniTableField google_protobuf_FieldOptions_EditionDefault__fields[2] = {
  {2, 16, 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)},
  {3, 12, 65, 0, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)},
};

const wpi_upb_MiniTable google__protobuf__FieldOptions__EditionDefault_msg_init = {
  &google_protobuf_FieldOptions_EditionDefault_submsgs[0],
  &google_protobuf_FieldOptions_EditionDefault__fields[0],
  WPI_UPB_SIZE(24, 32), 2, kUpb_ExtMode_NonExtendable, 0, WPI_UPB_FASTTABLE_MASK(255), 0,
#ifdef WPI_UPB_TRACING_ENABLED
  "google.protobuf.FieldOptions.EditionDefault",
#endif
};

const wpi_upb_MiniTable* google__protobuf__FieldOptions__EditionDefault_msg_init_ptr = &google__protobuf__FieldOptions__EditionDefault_msg_init;
static const wpi_upb_MiniTableSubInternal google_protobuf_FieldOptions_FeatureSupport_submsgs[3] = {
  {.WPI_UPB_PRIVATE(subenum) = &google_protobuf_Edition_enum_init},
  {.WPI_UPB_PRIVATE(subenum) = &google_protobuf_Edition_enum_init},
  {.WPI_UPB_PRIVATE(subenum) = &google_protobuf_Edition_enum_init},
};

static const wpi_upb_MiniTableField google_protobuf_FieldOptions_FeatureSupport__fields[4] = {
  {1, 12, 64, 0, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)},
  {2, 16, 65, 1, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)},
  {3, 24, 66, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)},
  {4, 20, 67, 2, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)},
};

const wpi_upb_MiniTable google__protobuf__FieldOptions__FeatureSupport_msg_init = {
  &google_protobuf_FieldOptions_FeatureSupport_submsgs[0],
  &google_protobuf_FieldOptions_FeatureSupport__fields[0],
  WPI_UPB_SIZE(32, 40), 4, kUpb_ExtMode_NonExtendable, 4, WPI_UPB_FASTTABLE_MASK(255), 0,
#ifdef WPI_UPB_TRACING_ENABLED
  "google.protobuf.FieldOptions.FeatureSupport",
#endif
};

const wpi_upb_MiniTable* google__protobuf__FieldOptions__FeatureSupport_msg_init_ptr = &google__protobuf__FieldOptions__FeatureSupport_msg_init;
static const wpi_upb_MiniTableSubInternal google_protobuf_OneofOptions_submsgs[2] = {
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__FeatureSet_msg_init_ptr},
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__UninterpretedOption_msg_init_ptr},
};

static const wpi_upb_MiniTableField google_protobuf_OneofOptions__fields[2] = {
  {1, WPI_UPB_SIZE(12, 16), 64, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
  {999, WPI_UPB_SIZE(16, 24), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
};

const wpi_upb_MiniTable google__protobuf__OneofOptions_msg_init = {
  &google_protobuf_OneofOptions_submsgs[0],
  &google_protobuf_OneofOptions__fields[0],
  WPI_UPB_SIZE(24, 32), 2, kUpb_ExtMode_Extendable, 1, WPI_UPB_FASTTABLE_MASK(248), 0,
#ifdef WPI_UPB_TRACING_ENABLED
  "google.protobuf.OneofOptions",
#endif
  WPI_UPB_FASTTABLE_INIT({
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x001800003f013eba, &wpi_upb_prm_2bt_max128b},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
  })
};

const wpi_upb_MiniTable* google__protobuf__OneofOptions_msg_init_ptr = &google__protobuf__OneofOptions_msg_init;
static const wpi_upb_MiniTableSubInternal google_protobuf_EnumOptions_submsgs[2] = {
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__FeatureSet_msg_init_ptr},
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__UninterpretedOption_msg_init_ptr},
};

static const wpi_upb_MiniTableField google_protobuf_EnumOptions__fields[5] = {
  {2, 9, 64, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)},
  {3, 10, 65, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)},
  {6, 11, 66, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)},
  {7, WPI_UPB_SIZE(12, 16), 67, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
  {999, WPI_UPB_SIZE(16, 24), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
};

const wpi_upb_MiniTable google__protobuf__EnumOptions_msg_init = {
  &google_protobuf_EnumOptions_submsgs[0],
  &google_protobuf_EnumOptions__fields[0],
  WPI_UPB_SIZE(24, 32), 5, kUpb_ExtMode_Extendable, 0, WPI_UPB_FASTTABLE_MASK(248), 0,
#ifdef WPI_UPB_TRACING_ENABLED
  "google.protobuf.EnumOptions",
#endif
  WPI_UPB_FASTTABLE_INIT({
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x001800003f013eba, &wpi_upb_prm_2bt_max128b},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
  })
};

const wpi_upb_MiniTable* google__protobuf__EnumOptions_msg_init_ptr = &google__protobuf__EnumOptions_msg_init;
static const wpi_upb_MiniTableSubInternal google_protobuf_EnumValueOptions_submsgs[3] = {
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__FeatureSet_msg_init_ptr},
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__FieldOptions__FeatureSupport_msg_init_ptr},
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__UninterpretedOption_msg_init_ptr},
};

static const wpi_upb_MiniTableField google_protobuf_EnumValueOptions__fields[5] = {
  {1, 9, 64, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)},
  {2, WPI_UPB_SIZE(12, 16), 65, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
  {3, WPI_UPB_SIZE(16, 10), 66, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)},
  {4, WPI_UPB_SIZE(20, 24), 67, 1, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
  {999, WPI_UPB_SIZE(24, 32), 0, 2, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
};

const wpi_upb_MiniTable google__protobuf__EnumValueOptions_msg_init = {
  &google_protobuf_EnumValueOptions_submsgs[0],
  &google_protobuf_EnumValueOptions__fields[0],
  WPI_UPB_SIZE(32, 40), 5, kUpb_ExtMode_Extendable, 4, WPI_UPB_FASTTABLE_MASK(248), 0,
#ifdef WPI_UPB_TRACING_ENABLED
  "google.protobuf.EnumValueOptions",
#endif
  WPI_UPB_FASTTABLE_INIT({
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x002000003f023eba, &wpi_upb_prm_2bt_max128b},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
  })
};

const wpi_upb_MiniTable* google__protobuf__EnumValueOptions_msg_init_ptr = &google__protobuf__EnumValueOptions_msg_init;
static const wpi_upb_MiniTableSubInternal google_protobuf_ServiceOptions_submsgs[2] = {
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__FeatureSet_msg_init_ptr},
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__UninterpretedOption_msg_init_ptr},
};

static const wpi_upb_MiniTableField google_protobuf_ServiceOptions__fields[3] = {
  {33, 9, 64, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)},
  {34, WPI_UPB_SIZE(12, 16), 65, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
  {999, WPI_UPB_SIZE(16, 24), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
};

const wpi_upb_MiniTable google__protobuf__ServiceOptions_msg_init = {
  &google_protobuf_ServiceOptions_submsgs[0],
  &google_protobuf_ServiceOptions__fields[0],
  WPI_UPB_SIZE(24, 32), 3, kUpb_ExtMode_Extendable, 0, WPI_UPB_FASTTABLE_MASK(248), 0,
#ifdef WPI_UPB_TRACING_ENABLED
  "google.protobuf.ServiceOptions",
#endif
  WPI_UPB_FASTTABLE_INIT({
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x001800003f013eba, &wpi_upb_prm_2bt_max128b},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
  })
};

const wpi_upb_MiniTable* google__protobuf__ServiceOptions_msg_init_ptr = &google__protobuf__ServiceOptions_msg_init;
static const wpi_upb_MiniTableSubInternal google_protobuf_MethodOptions_submsgs[3] = {
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__FeatureSet_msg_init_ptr},
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__UninterpretedOption_msg_init_ptr},
  {.WPI_UPB_PRIVATE(subenum) = &google_protobuf_MethodOptions_IdempotencyLevel_enum_init},
};

static const wpi_upb_MiniTableField google_protobuf_MethodOptions__fields[4] = {
  {33, 9, 64, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)},
  {34, 12, 65, 2, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)},
  {35, 16, 66, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
  {999, WPI_UPB_SIZE(20, 24), 0, 1, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
};

const wpi_upb_MiniTable google__protobuf__MethodOptions_msg_init = {
  &google_protobuf_MethodOptions_submsgs[0],
  &google_protobuf_MethodOptions__fields[0],
  WPI_UPB_SIZE(24, 32), 4, kUpb_ExtMode_Extendable, 0, WPI_UPB_FASTTABLE_MASK(248), 0,
#ifdef WPI_UPB_TRACING_ENABLED
  "google.protobuf.MethodOptions",
#endif
  WPI_UPB_FASTTABLE_INIT({
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x001800003f013eba, &wpi_upb_prm_2bt_max128b},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
  })
};

const wpi_upb_MiniTable* google__protobuf__MethodOptions_msg_init_ptr = &google__protobuf__MethodOptions_msg_init;
static const wpi_upb_MiniTableSubInternal google_protobuf_UninterpretedOption_submsgs[1] = {
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__UninterpretedOption__NamePart_msg_init_ptr},
};

static const wpi_upb_MiniTableField google_protobuf_UninterpretedOption__fields[7] = {
  {2, WPI_UPB_SIZE(12, 16), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
  {3, WPI_UPB_SIZE(16, 24), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)},
  {4, WPI_UPB_SIZE(24, 40), 65, kUpb_NoSub, 4, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_8Byte << kUpb_FieldRep_Shift)},
  {5, WPI_UPB_SIZE(32, 48), 66, kUpb_NoSub, 3, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_8Byte << kUpb_FieldRep_Shift)},
  {6, WPI_UPB_SIZE(40, 56), 67, kUpb_NoSub, 1, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_8Byte << kUpb_FieldRep_Shift)},
  {7, WPI_UPB_SIZE(48, 64), 68, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)},
  {8, WPI_UPB_SIZE(56, 80), 69, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)},
};

const wpi_upb_MiniTable google__protobuf__UninterpretedOption_msg_init = {
  &google_protobuf_UninterpretedOption_submsgs[0],
  &google_protobuf_UninterpretedOption__fields[0],
  WPI_UPB_SIZE(64, 96), 7, kUpb_ExtMode_NonExtendable, 0, WPI_UPB_FASTTABLE_MASK(24), 0,
#ifdef WPI_UPB_TRACING_ENABLED
  "google.protobuf.UninterpretedOption",
#endif
  WPI_UPB_FASTTABLE_INIT({
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x001000003f000012, &wpi_upb_prm_1bt_max64b},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
  })
};

const wpi_upb_MiniTable* google__protobuf__UninterpretedOption_msg_init_ptr = &google__protobuf__UninterpretedOption_msg_init;
static const wpi_upb_MiniTableField google_protobuf_UninterpretedOption_NamePart__fields[2] = {
  {1, WPI_UPB_SIZE(12, 16), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)},
  {2, 9, 65, kUpb_NoSub, 8, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_1Byte << kUpb_FieldRep_Shift)},
};

const wpi_upb_MiniTable google__protobuf__UninterpretedOption__NamePart_msg_init = {
  NULL,
  &google_protobuf_UninterpretedOption_NamePart__fields[0],
  WPI_UPB_SIZE(24, 32), 2, kUpb_ExtMode_NonExtendable, 2, WPI_UPB_FASTTABLE_MASK(255), 2,
#ifdef WPI_UPB_TRACING_ENABLED
  "google.protobuf.UninterpretedOption.NamePart",
#endif
};

const wpi_upb_MiniTable* google__protobuf__UninterpretedOption__NamePart_msg_init_ptr = &google__protobuf__UninterpretedOption__NamePart_msg_init;
static const wpi_upb_MiniTableSubInternal google_protobuf_FeatureSet_submsgs[6] = {
  {.WPI_UPB_PRIVATE(subenum) = &google_protobuf_FeatureSet_FieldPresence_enum_init},
  {.WPI_UPB_PRIVATE(subenum) = &google_protobuf_FeatureSet_EnumType_enum_init},
  {.WPI_UPB_PRIVATE(subenum) = &google_protobuf_FeatureSet_RepeatedFieldEncoding_enum_init},
  {.WPI_UPB_PRIVATE(subenum) = &google_protobuf_FeatureSet_Utf8Validation_enum_init},
  {.WPI_UPB_PRIVATE(subenum) = &google_protobuf_FeatureSet_MessageEncoding_enum_init},
  {.WPI_UPB_PRIVATE(subenum) = &google_protobuf_FeatureSet_JsonFormat_enum_init},
};

static const wpi_upb_MiniTableField google_protobuf_FeatureSet__fields[6] = {
  {1, 12, 64, 0, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)},
  {2, 16, 65, 1, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)},
  {3, 20, 66, 2, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)},
  {4, 24, 67, 3, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)},
  {5, 28, 68, 4, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)},
  {6, 32, 69, 5, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)},
};

const wpi_upb_MiniTable google__protobuf__FeatureSet_msg_init = {
  &google_protobuf_FeatureSet_submsgs[0],
  &google_protobuf_FeatureSet__fields[0],
  40, 6, kUpb_ExtMode_Extendable, 6, WPI_UPB_FASTTABLE_MASK(255), 0,
#ifdef WPI_UPB_TRACING_ENABLED
  "google.protobuf.FeatureSet",
#endif
};

const wpi_upb_MiniTable* google__protobuf__FeatureSet_msg_init_ptr = &google__protobuf__FeatureSet_msg_init;
static const wpi_upb_MiniTableSubInternal google_protobuf_FeatureSetDefaults_submsgs[3] = {
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__FeatureSetDefaults__FeatureSetEditionDefault_msg_init_ptr},
  {.WPI_UPB_PRIVATE(subenum) = &google_protobuf_Edition_enum_init},
  {.WPI_UPB_PRIVATE(subenum) = &google_protobuf_Edition_enum_init},
};

static const wpi_upb_MiniTableField google_protobuf_FeatureSetDefaults__fields[3] = {
  {1, WPI_UPB_SIZE(12, 24), 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
  {4, WPI_UPB_SIZE(16, 12), 64, 1, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)},
  {5, WPI_UPB_SIZE(20, 16), 65, 2, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)},
};

const wpi_upb_MiniTable google__protobuf__FeatureSetDefaults_msg_init = {
  &google_protobuf_FeatureSetDefaults_submsgs[0],
  &google_protobuf_FeatureSetDefaults__fields[0],
  WPI_UPB_SIZE(24, 32), 3, kUpb_ExtMode_NonExtendable, 1, WPI_UPB_FASTTABLE_MASK(8), 0,
#ifdef WPI_UPB_TRACING_ENABLED
  "google.protobuf.FeatureSetDefaults",
#endif
  WPI_UPB_FASTTABLE_INIT({
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x001800003f00000a, &wpi_upb_prm_1bt_max64b},
  })
};

const wpi_upb_MiniTable* google__protobuf__FeatureSetDefaults_msg_init_ptr = &google__protobuf__FeatureSetDefaults_msg_init;
static const wpi_upb_MiniTableSubInternal google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault_submsgs[3] = {
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__FeatureSet_msg_init_ptr},
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__FeatureSet_msg_init_ptr},
  {.WPI_UPB_PRIVATE(subenum) = &google_protobuf_Edition_enum_init},
};

static const wpi_upb_MiniTableField google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault__fields[3] = {
  {3, 12, 64, 2, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)},
  {4, 16, 65, 0, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
  {5, WPI_UPB_SIZE(20, 24), 66, 1, 11, (int)kUpb_FieldMode_Scalar | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
};

const wpi_upb_MiniTable google__protobuf__FeatureSetDefaults__FeatureSetEditionDefault_msg_init = {
  &google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault_submsgs[0],
  &google_protobuf_FeatureSetDefaults_FeatureSetEditionDefault__fields[0],
  WPI_UPB_SIZE(24, 32), 3, kUpb_ExtMode_NonExtendable, 0, WPI_UPB_FASTTABLE_MASK(255), 0,
#ifdef WPI_UPB_TRACING_ENABLED
  "google.protobuf.FeatureSetDefaults.FeatureSetEditionDefault",
#endif
};

const wpi_upb_MiniTable* google__protobuf__FeatureSetDefaults__FeatureSetEditionDefault_msg_init_ptr = &google__protobuf__FeatureSetDefaults__FeatureSetEditionDefault_msg_init;
static const wpi_upb_MiniTableSubInternal google_protobuf_SourceCodeInfo_submsgs[1] = {
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__SourceCodeInfo__Location_msg_init_ptr},
};

static const wpi_upb_MiniTableField google_protobuf_SourceCodeInfo__fields[1] = {
  {1, 8, 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
};

const wpi_upb_MiniTable google__protobuf__SourceCodeInfo_msg_init = {
  &google_protobuf_SourceCodeInfo_submsgs[0],
  &google_protobuf_SourceCodeInfo__fields[0],
  16, 1, kUpb_ExtMode_NonExtendable, 1, WPI_UPB_FASTTABLE_MASK(8), 0,
#ifdef WPI_UPB_TRACING_ENABLED
  "google.protobuf.SourceCodeInfo",
#endif
  WPI_UPB_FASTTABLE_INIT({
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x000800003f00000a, &wpi_upb_prm_1bt_max128b},
  })
};

const wpi_upb_MiniTable* google__protobuf__SourceCodeInfo_msg_init_ptr = &google__protobuf__SourceCodeInfo_msg_init;
static const wpi_upb_MiniTableField google_protobuf_SourceCodeInfo_Location__fields[5] = {
  {1, WPI_UPB_SIZE(12, 16), 0, kUpb_NoSub, 5, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsPacked | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
  {2, WPI_UPB_SIZE(16, 24), 0, kUpb_NoSub, 5, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsPacked | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
  {3, WPI_UPB_SIZE(24, 32), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)},
  {4, WPI_UPB_SIZE(32, 48), 65, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)},
  {6, WPI_UPB_SIZE(20, 64), 0, kUpb_NoSub, 12, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsAlternate | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
};

const wpi_upb_MiniTable google__protobuf__SourceCodeInfo__Location_msg_init = {
  NULL,
  &google_protobuf_SourceCodeInfo_Location__fields[0],
  WPI_UPB_SIZE(40, 72), 5, kUpb_ExtMode_NonExtendable, 4, WPI_UPB_FASTTABLE_MASK(56), 0,
#ifdef WPI_UPB_TRACING_ENABLED
  "google.protobuf.SourceCodeInfo.Location",
#endif
  WPI_UPB_FASTTABLE_INIT({
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x001000003f00000a, &wpi_upb_ppv4_1bt},
    {0x001800003f000012, &wpi_upb_ppv4_1bt},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x004000003f000032, &wpi_upb_prs_1bt},
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
  })
};

const wpi_upb_MiniTable* google__protobuf__SourceCodeInfo__Location_msg_init_ptr = &google__protobuf__SourceCodeInfo__Location_msg_init;
static const wpi_upb_MiniTableSubInternal google_protobuf_GeneratedCodeInfo_submsgs[1] = {
  {.WPI_UPB_PRIVATE(submsg) = &google__protobuf__GeneratedCodeInfo__Annotation_msg_init_ptr},
};

static const wpi_upb_MiniTableField google_protobuf_GeneratedCodeInfo__fields[1] = {
  {1, 8, 0, 0, 11, (int)kUpb_FieldMode_Array | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
};

const wpi_upb_MiniTable google__protobuf__GeneratedCodeInfo_msg_init = {
  &google_protobuf_GeneratedCodeInfo_submsgs[0],
  &google_protobuf_GeneratedCodeInfo__fields[0],
  16, 1, kUpb_ExtMode_NonExtendable, 1, WPI_UPB_FASTTABLE_MASK(8), 0,
#ifdef WPI_UPB_TRACING_ENABLED
  "google.protobuf.GeneratedCodeInfo",
#endif
  WPI_UPB_FASTTABLE_INIT({
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x000800003f00000a, &wpi_upb_prm_1bt_max64b},
  })
};

const wpi_upb_MiniTable* google__protobuf__GeneratedCodeInfo_msg_init_ptr = &google__protobuf__GeneratedCodeInfo_msg_init;
static const wpi_upb_MiniTableSubInternal google_protobuf_GeneratedCodeInfo_Annotation_submsgs[1] = {
  {.WPI_UPB_PRIVATE(subenum) = &google_protobuf_GeneratedCodeInfo_Annotation_Semantic_enum_init},
};

static const wpi_upb_MiniTableField google_protobuf_GeneratedCodeInfo_Annotation__fields[5] = {
  {1, WPI_UPB_SIZE(12, 24), 0, kUpb_NoSub, 5, (int)kUpb_FieldMode_Array | (int)kUpb_LabelFlags_IsPacked | ((int)WPI_UPB_SIZE(kUpb_FieldRep_4Byte, kUpb_FieldRep_8Byte) << kUpb_FieldRep_Shift)},
  {2, WPI_UPB_SIZE(28, 32), 64, kUpb_NoSub, 12, (int)kUpb_FieldMode_Scalar | (int)kUpb_LabelFlags_IsAlternate | ((int)kUpb_FieldRep_StringView << kUpb_FieldRep_Shift)},
  {3, WPI_UPB_SIZE(16, 12), 65, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)},
  {4, WPI_UPB_SIZE(20, 16), 66, kUpb_NoSub, 5, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)},
  {5, WPI_UPB_SIZE(24, 20), 67, 0, 14, (int)kUpb_FieldMode_Scalar | ((int)kUpb_FieldRep_4Byte << kUpb_FieldRep_Shift)},
};

const wpi_upb_MiniTable google__protobuf__GeneratedCodeInfo__Annotation_msg_init = {
  &google_protobuf_GeneratedCodeInfo_Annotation_submsgs[0],
  &google_protobuf_GeneratedCodeInfo_Annotation__fields[0],
  WPI_UPB_SIZE(40, 48), 5, kUpb_ExtMode_NonExtendable, 5, WPI_UPB_FASTTABLE_MASK(8), 0,
#ifdef WPI_UPB_TRACING_ENABLED
  "google.protobuf.GeneratedCodeInfo.Annotation",
#endif
  WPI_UPB_FASTTABLE_INIT({
    {0x0000000000000000, &_wpi_upb_FastDecoder_DecodeGeneric},
    {0x001800003f00000a, &wpi_upb_ppv4_1bt},
  })
};

const wpi_upb_MiniTable* google__protobuf__GeneratedCodeInfo__Annotation_msg_init_ptr = &google__protobuf__GeneratedCodeInfo__Annotation_msg_init;
const wpi_upb_MiniTableEnum google_protobuf_Edition_enum_init = {
    64,
    9,
    {
        0x7,
        0x0,
        0x384,
        0x3e6,
        0x3e7,
        0x3e8,
        0x3e9,
        0x1869d,
        0x1869e,
        0x1869f,
        0x7fffffff,
    },
};

const wpi_upb_MiniTableEnum google_protobuf_ExtensionRangeOptions_VerificationState_enum_init = {
    64,
    0,
    {
        0x3,
        0x0,
    },
};

const wpi_upb_MiniTableEnum google_protobuf_FeatureSet_EnumType_enum_init = {
    64,
    0,
    {
        0x7,
        0x0,
    },
};

const wpi_upb_MiniTableEnum google_protobuf_FeatureSet_FieldPresence_enum_init = {
    64,
    0,
    {
        0xf,
        0x0,
    },
};

const wpi_upb_MiniTableEnum google_protobuf_FeatureSet_JsonFormat_enum_init = {
    64,
    0,
    {
        0x7,
        0x0,
    },
};

const wpi_upb_MiniTableEnum google_protobuf_FeatureSet_MessageEncoding_enum_init = {
    64,
    0,
    {
        0x7,
        0x0,
    },
};

const wpi_upb_MiniTableEnum google_protobuf_FeatureSet_RepeatedFieldEncoding_enum_init = {
    64,
    0,
    {
        0x7,
        0x0,
    },
};

const wpi_upb_MiniTableEnum google_protobuf_FeatureSet_Utf8Validation_enum_init = {
    64,
    0,
    {
        0xd,
        0x0,
    },
};

const wpi_upb_MiniTableEnum google_protobuf_FieldDescriptorProto_Label_enum_init = {
    64,
    0,
    {
        0xe,
        0x0,
    },
};

const wpi_upb_MiniTableEnum google_protobuf_FieldDescriptorProto_Type_enum_init = {
    64,
    0,
    {
        0x7fffe,
        0x0,
    },
};

const wpi_upb_MiniTableEnum google_protobuf_FieldOptions_CType_enum_init = {
    64,
    0,
    {
        0x7,
        0x0,
    },
};

const wpi_upb_MiniTableEnum google_protobuf_FieldOptions_JSType_enum_init = {
    64,
    0,
    {
        0x7,
        0x0,
    },
};

const wpi_upb_MiniTableEnum google_protobuf_FieldOptions_OptionRetention_enum_init = {
    64,
    0,
    {
        0x7,
        0x0,
    },
};

const wpi_upb_MiniTableEnum google_protobuf_FieldOptions_OptionTargetType_enum_init = {
    64,
    0,
    {
        0x3ff,
        0x0,
    },
};

const wpi_upb_MiniTableEnum google_protobuf_FileOptions_OptimizeMode_enum_init = {
    64,
    0,
    {
        0xe,
        0x0,
    },
};

const wpi_upb_MiniTableEnum google_protobuf_GeneratedCodeInfo_Annotation_Semantic_enum_init = {
    64,
    0,
    {
        0x7,
        0x0,
    },
};

const wpi_upb_MiniTableEnum google_protobuf_MethodOptions_IdempotencyLevel_enum_init = {
    64,
    0,
    {
        0x7,
        0x0,
    },
};

static const wpi_upb_MiniTable *messages_layout[33] = {
  &google__protobuf__FileDescriptorSet_msg_init,
  &google__protobuf__FileDescriptorProto_msg_init,
  &google__protobuf__DescriptorProto_msg_init,
  &google__protobuf__DescriptorProto__ExtensionRange_msg_init,
  &google__protobuf__DescriptorProto__ReservedRange_msg_init,
  &google__protobuf__ExtensionRangeOptions_msg_init,
  &google__protobuf__ExtensionRangeOptions__Declaration_msg_init,
  &google__protobuf__FieldDescriptorProto_msg_init,
  &google__protobuf__OneofDescriptorProto_msg_init,
  &google__protobuf__EnumDescriptorProto_msg_init,
  &google__protobuf__EnumDescriptorProto__EnumReservedRange_msg_init,
  &google__protobuf__EnumValueDescriptorProto_msg_init,
  &google__protobuf__ServiceDescriptorProto_msg_init,
  &google__protobuf__MethodDescriptorProto_msg_init,
  &google__protobuf__FileOptions_msg_init,
  &google__protobuf__MessageOptions_msg_init,
  &google__protobuf__FieldOptions_msg_init,
  &google__protobuf__FieldOptions__EditionDefault_msg_init,
  &google__protobuf__FieldOptions__FeatureSupport_msg_init,
  &google__protobuf__OneofOptions_msg_init,
  &google__protobuf__EnumOptions_msg_init,
  &google__protobuf__EnumValueOptions_msg_init,
  &google__protobuf__ServiceOptions_msg_init,
  &google__protobuf__MethodOptions_msg_init,
  &google__protobuf__UninterpretedOption_msg_init,
  &google__protobuf__UninterpretedOption__NamePart_msg_init,
  &google__protobuf__FeatureSet_msg_init,
  &google__protobuf__FeatureSetDefaults_msg_init,
  &google__protobuf__FeatureSetDefaults__FeatureSetEditionDefault_msg_init,
  &google__protobuf__SourceCodeInfo_msg_init,
  &google__protobuf__SourceCodeInfo__Location_msg_init,
  &google__protobuf__GeneratedCodeInfo_msg_init,
  &google__protobuf__GeneratedCodeInfo__Annotation_msg_init,
};

static const wpi_upb_MiniTableEnum *enums_layout[17] = {
  &google_protobuf_Edition_enum_init,
  &google_protobuf_ExtensionRangeOptions_VerificationState_enum_init,
  &google_protobuf_FeatureSet_EnumType_enum_init,
  &google_protobuf_FeatureSet_FieldPresence_enum_init,
  &google_protobuf_FeatureSet_JsonFormat_enum_init,
  &google_protobuf_FeatureSet_MessageEncoding_enum_init,
  &google_protobuf_FeatureSet_RepeatedFieldEncoding_enum_init,
  &google_protobuf_FeatureSet_Utf8Validation_enum_init,
  &google_protobuf_FieldDescriptorProto_Label_enum_init,
  &google_protobuf_FieldDescriptorProto_Type_enum_init,
  &google_protobuf_FieldOptions_CType_enum_init,
  &google_protobuf_FieldOptions_JSType_enum_init,
  &google_protobuf_FieldOptions_OptionRetention_enum_init,
  &google_protobuf_FieldOptions_OptionTargetType_enum_init,
  &google_protobuf_FileOptions_OptimizeMode_enum_init,
  &google_protobuf_GeneratedCodeInfo_Annotation_Semantic_enum_init,
  &google_protobuf_MethodOptions_IdempotencyLevel_enum_init,
};

const wpi_upb_MiniTableFile google_protobuf_descriptor_proto_wpi_upb_file_layout = {
  messages_layout,
  enums_layout,
  NULL,
  33,
  17,
  0,
};





static const char* _wpi_upb_EpsCopyInputStream_NoOpCallback(
    wpi_upb_EpsCopyInputStream* e, const char* old_end, const char* new_start) {
  return new_start;
}

const char* _wpi_upb_EpsCopyInputStream_IsDoneFallbackNoCallback(
    wpi_upb_EpsCopyInputStream* e, const char* ptr, int overrun) {
  return _wpi_upb_EpsCopyInputStream_IsDoneFallbackInline(
      e, ptr, overrun, _wpi_upb_EpsCopyInputStream_NoOpCallback);
}


#include <stdlib.h>

// Must be last.

static void* wpi_upb_global_allocfunc(wpi_upb_alloc* alloc, void* ptr, size_t oldsize,
                                  size_t size) {
  WPI_UPB_UNUSED(alloc);
  WPI_UPB_UNUSED(oldsize);
  if (size == 0) {
    free(ptr);
    return NULL;
  } else {
    return realloc(ptr, size);
  }
}

wpi_upb_alloc wpi_upb_alloc_global = {&wpi_upb_global_allocfunc};


#ifdef WPI_UPB_TRACING_ENABLED
#include <stdatomic.h>
#endif

#include <stddef.h>
#include <stdint.h>


// Must be last.

static WPI_UPB_ATOMIC(size_t) max_block_size = 32 << 10;

void wpi_upb_Arena_SetMaxBlockSize(size_t max) { max_block_size = max; }

typedef struct wpi_upb_MemBlock {
  // Atomic only for the benefit of SpaceAllocated().
  WPI_UPB_ATOMIC(struct wpi_upb_MemBlock*) next;
  uint32_t size;
  // Data follows.
} wpi_upb_MemBlock;

typedef struct wpi_upb_ArenaInternal {
  // wpi_upb_alloc* together with a low bit which signals if there is an initial
  // block.
  uintptr_t block_alloc;

  // When multiple arenas are fused together, each arena points to a parent
  // arena (root points to itself). The root tracks how many live arenas
  // reference it.

  // The low bit is tagged:
  //   0: pointer to parent
  //   1: count, left shifted by one
  WPI_UPB_ATOMIC(uintptr_t) parent_or_count;

  // All nodes that are fused together are in a singly-linked list.
  // == NULL at end of list.
  WPI_UPB_ATOMIC(struct wpi_upb_ArenaInternal*) next;

  // The last element of the linked list. This is present only as an
  // optimization, so that we do not have to iterate over all members for every
  // fuse.  Only significant for an arena root. In other cases it is ignored.
  // == self when no other list members.
  WPI_UPB_ATOMIC(struct wpi_upb_ArenaInternal*) tail;

  // Linked list of blocks to free/cleanup. Atomic only for the benefit of
  // wpi_upb_Arena_SpaceAllocated().
  WPI_UPB_ATOMIC(wpi_upb_MemBlock*) blocks;
} wpi_upb_ArenaInternal;

// All public + private state for an arena.
typedef struct {
  wpi_upb_Arena head;
  wpi_upb_ArenaInternal body;
} wpi_upb_ArenaState;

typedef struct {
  wpi_upb_ArenaInternal* root;
  uintptr_t tagged_count;
} wpi_upb_ArenaRoot;

static const size_t kUpb_MemblockReserve =
    WPI_UPB_ALIGN_UP(sizeof(wpi_upb_MemBlock), WPI_UPB_MALLOC_ALIGN);

// Extracts the (wpi_upb_ArenaInternal*) from a (wpi_upb_Arena*)
static wpi_upb_ArenaInternal* wpi_upb_Arena_Internal(const wpi_upb_Arena* a) {
  return &((wpi_upb_ArenaState*)a)->body;
}

static bool _wpi_upb_Arena_IsTaggedRefcount(uintptr_t parent_or_count) {
  return (parent_or_count & 1) == 1;
}

static bool _wpi_upb_Arena_IsTaggedPointer(uintptr_t parent_or_count) {
  return (parent_or_count & 1) == 0;
}

static uintptr_t _wpi_upb_Arena_RefCountFromTagged(uintptr_t parent_or_count) {
  WPI_UPB_ASSERT(_wpi_upb_Arena_IsTaggedRefcount(parent_or_count));
  return parent_or_count >> 1;
}

static uintptr_t _wpi_upb_Arena_TaggedFromRefcount(uintptr_t refcount) {
  uintptr_t parent_or_count = (refcount << 1) | 1;
  WPI_UPB_ASSERT(_wpi_upb_Arena_IsTaggedRefcount(parent_or_count));
  return parent_or_count;
}

static wpi_upb_ArenaInternal* _wpi_upb_Arena_PointerFromTagged(
    uintptr_t parent_or_count) {
  WPI_UPB_ASSERT(_wpi_upb_Arena_IsTaggedPointer(parent_or_count));
  return (wpi_upb_ArenaInternal*)parent_or_count;
}

static uintptr_t _wpi_upb_Arena_TaggedFromPointer(wpi_upb_ArenaInternal* ai) {
  uintptr_t parent_or_count = (uintptr_t)ai;
  WPI_UPB_ASSERT(_wpi_upb_Arena_IsTaggedPointer(parent_or_count));
  return parent_or_count;
}

static wpi_upb_alloc* _wpi_upb_ArenaInternal_BlockAlloc(wpi_upb_ArenaInternal* ai) {
  return (wpi_upb_alloc*)(ai->block_alloc & ~0x1);
}

static uintptr_t _wpi_upb_Arena_MakeBlockAlloc(wpi_upb_alloc* alloc, bool has_initial) {
  uintptr_t alloc_uint = (uintptr_t)alloc;
  WPI_UPB_ASSERT((alloc_uint & 1) == 0);
  return alloc_uint | (has_initial ? 1 : 0);
}

static bool _wpi_upb_ArenaInternal_HasInitialBlock(wpi_upb_ArenaInternal* ai) {
  return ai->block_alloc & 0x1;
}

#ifdef WPI_UPB_TRACING_ENABLED
static void (*_init_arena_trace_handler)(const wpi_upb_Arena*, size_t size) = NULL;
static void (*_fuse_arena_trace_handler)(const wpi_upb_Arena*,
                                         const wpi_upb_Arena*) = NULL;
static void (*_free_arena_trace_handler)(const wpi_upb_Arena*) = NULL;

void wpi_upb_Arena_SetTraceHandler(
    void (*initArenaTraceHandler)(const wpi_upb_Arena*, size_t size),
    void (*fuseArenaTraceHandler)(const wpi_upb_Arena*, const wpi_upb_Arena*),
    void (*freeArenaTraceHandler)(const wpi_upb_Arena*)) {
  _init_arena_trace_handler = initArenaTraceHandler;
  _fuse_arena_trace_handler = fuseArenaTraceHandler;
  _free_arena_trace_handler = freeArenaTraceHandler;
}

void wpi_upb_Arena_LogInit(const wpi_upb_Arena* arena, size_t size) {
  if (_init_arena_trace_handler) {
    _init_arena_trace_handler(arena, size);
  }
}
void wpi_upb_Arena_LogFuse(const wpi_upb_Arena* arena1, const wpi_upb_Arena* arena2) {
  if (_fuse_arena_trace_handler) {
    _fuse_arena_trace_handler(arena1, arena2);
  }
}
void wpi_upb_Arena_LogFree(const wpi_upb_Arena* arena) {
  if (_free_arena_trace_handler) {
    _free_arena_trace_handler(arena);
  }
}
#endif  // WPI_UPB_TRACING_ENABLED

static wpi_upb_ArenaRoot _wpi_upb_Arena_FindRoot(wpi_upb_Arena* a) {
  wpi_upb_ArenaInternal* ai = wpi_upb_Arena_Internal(a);
  uintptr_t poc = wpi_upb_Atomic_Load(&ai->parent_or_count, memory_order_acquire);
  while (_wpi_upb_Arena_IsTaggedPointer(poc)) {
    wpi_upb_ArenaInternal* next = _wpi_upb_Arena_PointerFromTagged(poc);
    WPI_UPB_ASSERT(ai != next);
    uintptr_t next_poc =
        wpi_upb_Atomic_Load(&next->parent_or_count, memory_order_acquire);

    if (_wpi_upb_Arena_IsTaggedPointer(next_poc)) {
      // To keep complexity down, we lazily collapse levels of the tree.  This
      // keeps it flat in the final case, but doesn't cost much incrementally.
      //
      // Path splitting keeps time complexity down, see:
      //   https://en.wikipedia.org/wiki/Disjoint-set_data_structure
      //
      // We can safely use a relaxed atomic here because all threads doing this
      // will converge on the same value and we don't need memory orderings to
      // be visible.
      //
      // This is true because:
      // - If no fuses occur, this will eventually become the root.
      // - If fuses are actively occurring, the root may change, but the
      //   invariant is that `parent_or_count` merely points to *a* parent.
      //
      // In other words, it is moving towards "the" root, and that root may move
      // further away over time, but the path towards that root will continue to
      // be valid and the creation of the path carries all the memory orderings
      // required.
      WPI_UPB_ASSERT(ai != _wpi_upb_Arena_PointerFromTagged(next_poc));
      wpi_upb_Atomic_Store(&ai->parent_or_count, next_poc, memory_order_relaxed);
    }
    ai = next;
    poc = next_poc;
  }
  return (wpi_upb_ArenaRoot){.root = ai, .tagged_count = poc};
}

size_t wpi_upb_Arena_SpaceAllocated(wpi_upb_Arena* arena, size_t* fused_count) {
  wpi_upb_ArenaInternal* ai = _wpi_upb_Arena_FindRoot(arena).root;
  size_t memsize = 0;
  size_t local_fused_count = 0;

  while (ai != NULL) {
    wpi_upb_MemBlock* block = wpi_upb_Atomic_Load(&ai->blocks, memory_order_relaxed);
    while (block != NULL) {
      memsize += sizeof(wpi_upb_MemBlock) + block->size;
      block = wpi_upb_Atomic_Load(&block->next, memory_order_relaxed);
    }
    ai = wpi_upb_Atomic_Load(&ai->next, memory_order_relaxed);
    local_fused_count++;
  }

  if (fused_count) *fused_count = local_fused_count;
  return memsize;
}

bool WPI_UPB_PRIVATE(_wpi_upb_Arena_Contains)(const wpi_upb_Arena* a, void* ptr) {
  wpi_upb_ArenaInternal* ai = wpi_upb_Arena_Internal(a);
  WPI_UPB_ASSERT(ai);

  wpi_upb_MemBlock* block = wpi_upb_Atomic_Load(&ai->blocks, memory_order_relaxed);
  while (block) {
    uintptr_t beg = (uintptr_t)block;
    uintptr_t end = beg + block->size;
    if ((uintptr_t)ptr >= beg && (uintptr_t)ptr < end) return true;
    block = wpi_upb_Atomic_Load(&block->next, memory_order_relaxed);
  }

  return false;
}

uint32_t wpi_upb_Arena_DebugRefCount(wpi_upb_Arena* a) {
  wpi_upb_ArenaInternal* ai = wpi_upb_Arena_Internal(a);
  // These loads could probably be relaxed, but given that this is debug-only,
  // it's not worth introducing a new variant for it.
  uintptr_t poc = wpi_upb_Atomic_Load(&ai->parent_or_count, memory_order_acquire);
  while (_wpi_upb_Arena_IsTaggedPointer(poc)) {
    ai = _wpi_upb_Arena_PointerFromTagged(poc);
    poc = wpi_upb_Atomic_Load(&ai->parent_or_count, memory_order_acquire);
  }
  return _wpi_upb_Arena_RefCountFromTagged(poc);
}

static void _wpi_upb_Arena_AddBlock(wpi_upb_Arena* a, void* ptr, size_t size) {
  wpi_upb_ArenaInternal* ai = wpi_upb_Arena_Internal(a);
  wpi_upb_MemBlock* block = ptr;

  // Insert into linked list.
  block->size = (uint32_t)size;
  wpi_upb_Atomic_Init(&block->next, ai->blocks);
  wpi_upb_Atomic_Store(&ai->blocks, block, memory_order_release);

  a->WPI_UPB_PRIVATE(ptr) = WPI_UPB_PTR_AT(block, kUpb_MemblockReserve, char);
  a->WPI_UPB_PRIVATE(end) = WPI_UPB_PTR_AT(block, size, char);

  WPI_UPB_POISON_MEMORY_REGION(a->WPI_UPB_PRIVATE(ptr),
                           a->WPI_UPB_PRIVATE(end) - a->WPI_UPB_PRIVATE(ptr));
}

static bool _wpi_upb_Arena_AllocBlock(wpi_upb_Arena* a, size_t size) {
  wpi_upb_ArenaInternal* ai = wpi_upb_Arena_Internal(a);
  if (!ai->block_alloc) return false;
  wpi_upb_MemBlock* last_block = wpi_upb_Atomic_Load(&ai->blocks, memory_order_acquire);
  size_t last_size = last_block != NULL ? last_block->size : 128;

  // Don't naturally grow beyond the max block size.
  size_t clamped_size = WPI_UPB_MIN(last_size * 2, max_block_size);

  // We may need to exceed the max block size if the user requested a large
  // allocation.
  size_t block_size = WPI_UPB_MAX(size, clamped_size) + kUpb_MemblockReserve;

  wpi_upb_MemBlock* block =
      wpi_upb_malloc(_wpi_upb_ArenaInternal_BlockAlloc(ai), block_size);

  if (!block) return false;
  _wpi_upb_Arena_AddBlock(a, block, block_size);
  WPI_UPB_ASSERT(WPI_UPB_PRIVATE(_wpi_upb_ArenaHas)(a) >= size);
  return true;
}

void* WPI_UPB_PRIVATE(_wpi_upb_Arena_SlowMalloc)(wpi_upb_Arena* a, size_t size) {
  if (!_wpi_upb_Arena_AllocBlock(a, size)) return NULL;  // OOM
  return wpi_upb_Arena_Malloc(a, size - WPI_UPB_ASAN_GUARD_SIZE);
}

static wpi_upb_Arena* _wpi_upb_Arena_InitSlow(wpi_upb_alloc* alloc) {
  const size_t first_block_overhead =
      sizeof(wpi_upb_ArenaState) + kUpb_MemblockReserve;
  wpi_upb_ArenaState* a;

  // We need to malloc the initial block.
  char* mem;
  size_t n = first_block_overhead + 256;
  if (!alloc || !(mem = wpi_upb_malloc(alloc, n))) {
    return NULL;
  }

  a = WPI_UPB_PTR_AT(mem, n - sizeof(wpi_upb_ArenaState), wpi_upb_ArenaState);
  n -= sizeof(wpi_upb_ArenaState);

  a->body.block_alloc = _wpi_upb_Arena_MakeBlockAlloc(alloc, 0);
  wpi_upb_Atomic_Init(&a->body.parent_or_count, _wpi_upb_Arena_TaggedFromRefcount(1));
  wpi_upb_Atomic_Init(&a->body.next, NULL);
  wpi_upb_Atomic_Init(&a->body.tail, &a->body);
  wpi_upb_Atomic_Init(&a->body.blocks, NULL);

  _wpi_upb_Arena_AddBlock(&a->head, mem, n);

  return &a->head;
}

wpi_upb_Arena* wpi_upb_Arena_Init(void* mem, size_t n, wpi_upb_alloc* alloc) {
  WPI_UPB_ASSERT(sizeof(void*) * WPI_UPB_ARENA_SIZE_HACK >= sizeof(wpi_upb_ArenaState));
  wpi_upb_ArenaState* a;

  if (n) {
    /* Align initial pointer up so that we return properly-aligned pointers. */
    void* aligned = (void*)WPI_UPB_ALIGN_UP((uintptr_t)mem, WPI_UPB_MALLOC_ALIGN);
    size_t delta = (uintptr_t)aligned - (uintptr_t)mem;
    n = delta <= n ? n - delta : 0;
    mem = aligned;
  }

  /* Round block size down to alignof(*a) since we will allocate the arena
   * itself at the end. */
  n = WPI_UPB_ALIGN_DOWN(n, WPI_UPB_ALIGN_OF(wpi_upb_ArenaState));

  if (WPI_UPB_UNLIKELY(n < sizeof(wpi_upb_ArenaState))) {
#ifdef WPI_UPB_TRACING_ENABLED
    wpi_upb_Arena* ret = _wpi_upb_Arena_InitSlow(alloc);
    wpi_upb_Arena_LogInit(ret, n);
    return ret;
#else
    return _wpi_upb_Arena_InitSlow(alloc);
#endif
  }

  a = WPI_UPB_PTR_AT(mem, n - sizeof(wpi_upb_ArenaState), wpi_upb_ArenaState);

  wpi_upb_Atomic_Init(&a->body.parent_or_count, _wpi_upb_Arena_TaggedFromRefcount(1));
  wpi_upb_Atomic_Init(&a->body.next, NULL);
  wpi_upb_Atomic_Init(&a->body.tail, &a->body);
  wpi_upb_Atomic_Init(&a->body.blocks, NULL);

  a->body.block_alloc = _wpi_upb_Arena_MakeBlockAlloc(alloc, 1);
  a->head.WPI_UPB_PRIVATE(ptr) = mem;
  a->head.WPI_UPB_PRIVATE(end) = WPI_UPB_PTR_AT(mem, n - sizeof(wpi_upb_ArenaState), char);
#ifdef WPI_UPB_TRACING_ENABLED
  wpi_upb_Arena_LogInit(&a->head, n);
#endif
  return &a->head;
}

static void _wpi_upb_Arena_DoFree(wpi_upb_ArenaInternal* ai) {
  WPI_UPB_ASSERT(_wpi_upb_Arena_RefCountFromTagged(ai->parent_or_count) == 1);
  while (ai != NULL) {
    // Load first since arena itself is likely from one of its blocks.
    wpi_upb_ArenaInternal* next_arena =
        (wpi_upb_ArenaInternal*)wpi_upb_Atomic_Load(&ai->next, memory_order_acquire);
    wpi_upb_alloc* block_alloc = _wpi_upb_ArenaInternal_BlockAlloc(ai);
    wpi_upb_MemBlock* block = wpi_upb_Atomic_Load(&ai->blocks, memory_order_acquire);
    while (block != NULL) {
      // Load first since we are deleting block.
      wpi_upb_MemBlock* next_block =
          wpi_upb_Atomic_Load(&block->next, memory_order_acquire);
      wpi_upb_free(block_alloc, block);
      block = next_block;
    }
    ai = next_arena;
  }
}

void wpi_upb_Arena_Free(wpi_upb_Arena* a) {
  wpi_upb_ArenaInternal* ai = wpi_upb_Arena_Internal(a);
  uintptr_t poc = wpi_upb_Atomic_Load(&ai->parent_or_count, memory_order_acquire);
retry:
  while (_wpi_upb_Arena_IsTaggedPointer(poc)) {
    ai = _wpi_upb_Arena_PointerFromTagged(poc);
    poc = wpi_upb_Atomic_Load(&ai->parent_or_count, memory_order_acquire);
  }

  // compare_exchange or fetch_sub are RMW operations, which are more
  // expensive then direct loads.  As an optimization, we only do RMW ops
  // when we need to update things for other threads to see.
  if (poc == _wpi_upb_Arena_TaggedFromRefcount(1)) {
#ifdef WPI_UPB_TRACING_ENABLED
    wpi_upb_Arena_LogFree(a);
#endif
    _wpi_upb_Arena_DoFree(ai);
    return;
  }

  if (wpi_upb_Atomic_CompareExchangeWeak(
          &ai->parent_or_count, &poc,
          _wpi_upb_Arena_TaggedFromRefcount(_wpi_upb_Arena_RefCountFromTagged(poc) - 1),
          memory_order_release, memory_order_acquire)) {
    // We were >1 and we decremented it successfully, so we are done.
    return;
  }

  // We failed our update, so someone has done something, retry the whole
  // process, but the failed exchange reloaded `poc` for us.
  goto retry;
}

static void _wpi_upb_Arena_DoFuseArenaLists(wpi_upb_ArenaInternal* const parent,
                                        wpi_upb_ArenaInternal* child) {
  wpi_upb_ArenaInternal* parent_tail =
      wpi_upb_Atomic_Load(&parent->tail, memory_order_relaxed);

  do {
    // Our tail might be stale, but it will always converge to the true tail.
    wpi_upb_ArenaInternal* parent_tail_next =
        wpi_upb_Atomic_Load(&parent_tail->next, memory_order_relaxed);
    while (parent_tail_next != NULL) {
      parent_tail = parent_tail_next;
      parent_tail_next =
          wpi_upb_Atomic_Load(&parent_tail->next, memory_order_relaxed);
    }

    wpi_upb_ArenaInternal* displaced =
        wpi_upb_Atomic_Exchange(&parent_tail->next, child, memory_order_relaxed);
    parent_tail = wpi_upb_Atomic_Load(&child->tail, memory_order_relaxed);

    // If we displaced something that got installed racily, we can simply
    // reinstall it on our new tail.
    child = displaced;
  } while (child != NULL);

  wpi_upb_Atomic_Store(&parent->tail, parent_tail, memory_order_relaxed);
}

static wpi_upb_ArenaInternal* _wpi_upb_Arena_DoFuse(wpi_upb_Arena* a1, wpi_upb_Arena* a2,
                                            uintptr_t* ref_delta) {
  // `parent_or_count` has two disctint modes
  // -  parent pointer mode
  // -  refcount mode
  //
  // In parent pointer mode, it may change what pointer it refers to in the
  // tree, but it will always approach a root.  Any operation that walks the
  // tree to the root may collapse levels of the tree concurrently.
  wpi_upb_ArenaRoot r1 = _wpi_upb_Arena_FindRoot(a1);
  wpi_upb_ArenaRoot r2 = _wpi_upb_Arena_FindRoot(a2);

  if (r1.root == r2.root) return r1.root;  // Already fused.

  // Avoid cycles by always fusing into the root with the lower address.
  if ((uintptr_t)r1.root > (uintptr_t)r2.root) {
    wpi_upb_ArenaRoot tmp = r1;
    r1 = r2;
    r2 = tmp;
  }

  // The moment we install `r1` as the parent for `r2` all racing frees may
  // immediately begin decrementing `r1`'s refcount (including pending
  // increments to that refcount and their frees!).  We need to add `r2`'s refs
  // now, so that `r1` can withstand any unrefs that come from r2.
  //
  // Note that while it is possible for `r2`'s refcount to increase
  // asynchronously, we will not actually do the reparenting operation below
  // unless `r2`'s refcount is unchanged from when we read it.
  //
  // Note that we may have done this previously, either to this node or a
  // different node, during a previous and failed DoFuse() attempt. But we will
  // not lose track of these refs because we always add them to our overall
  // delta.
  uintptr_t r2_untagged_count = r2.tagged_count & ~1;
  uintptr_t with_r2_refs = r1.tagged_count + r2_untagged_count;
  if (!wpi_upb_Atomic_CompareExchangeStrong(
          &r1.root->parent_or_count, &r1.tagged_count, with_r2_refs,
          memory_order_release, memory_order_acquire)) {
    return NULL;
  }

  // Perform the actual fuse by removing the refs from `r2` and swapping in the
  // parent pointer.
  if (!wpi_upb_Atomic_CompareExchangeStrong(
          &r2.root->parent_or_count, &r2.tagged_count,
          _wpi_upb_Arena_TaggedFromPointer(r1.root), memory_order_release,
          memory_order_acquire)) {
    // We'll need to remove the excess refs we added to r1 previously.
    *ref_delta += r2_untagged_count;
    return NULL;
  }

  // Now that the fuse has been performed (and can no longer fail) we need to
  // append `r2` to `r1`'s linked list.
  _wpi_upb_Arena_DoFuseArenaLists(r1.root, r2.root);
  return r1.root;
}

static bool _wpi_upb_Arena_FixupRefs(wpi_upb_ArenaInternal* new_root,
                                 uintptr_t ref_delta) {
  if (ref_delta == 0) return true;  // No fixup required.
  uintptr_t poc =
      wpi_upb_Atomic_Load(&new_root->parent_or_count, memory_order_relaxed);
  if (_wpi_upb_Arena_IsTaggedPointer(poc)) return false;
  uintptr_t with_refs = poc - ref_delta;
  WPI_UPB_ASSERT(!_wpi_upb_Arena_IsTaggedPointer(with_refs));
  return wpi_upb_Atomic_CompareExchangeStrong(&new_root->parent_or_count, &poc,
                                          with_refs, memory_order_relaxed,
                                          memory_order_relaxed);
}

bool wpi_upb_Arena_Fuse(wpi_upb_Arena* a1, wpi_upb_Arena* a2) {
  if (a1 == a2) return true;  // trivial fuse

#ifdef WPI_UPB_TRACING_ENABLED
  wpi_upb_Arena_LogFuse(a1, a2);
#endif

  wpi_upb_ArenaInternal* ai1 = wpi_upb_Arena_Internal(a1);
  wpi_upb_ArenaInternal* ai2 = wpi_upb_Arena_Internal(a2);

  // Do not fuse initial blocks since we cannot lifetime extend them.
  // Any other fuse scenario is allowed.
  if (_wpi_upb_ArenaInternal_HasInitialBlock(ai1) ||
      _wpi_upb_ArenaInternal_HasInitialBlock(ai2)) {
    return false;
  }

  // The number of refs we ultimately need to transfer to the new root.
  uintptr_t ref_delta = 0;
  while (true) {
    wpi_upb_ArenaInternal* new_root = _wpi_upb_Arena_DoFuse(a1, a2, &ref_delta);
    if (new_root != NULL && _wpi_upb_Arena_FixupRefs(new_root, ref_delta)) {
      return true;
    }
  }
}

bool wpi_upb_Arena_IncRefFor(wpi_upb_Arena* a, const void* owner) {
  wpi_upb_ArenaInternal* ai = wpi_upb_Arena_Internal(a);
  if (_wpi_upb_ArenaInternal_HasInitialBlock(ai)) return false;
  wpi_upb_ArenaRoot r;

retry:
  r = _wpi_upb_Arena_FindRoot(a);
  if (wpi_upb_Atomic_CompareExchangeWeak(
          &r.root->parent_or_count, &r.tagged_count,
          _wpi_upb_Arena_TaggedFromRefcount(
              _wpi_upb_Arena_RefCountFromTagged(r.tagged_count) + 1),
          memory_order_release, memory_order_acquire)) {
    // We incremented it successfully, so we are done.
    return true;
  }
  // We failed update due to parent switching on the arena.
  goto retry;
}

void wpi_upb_Arena_DecRefFor(wpi_upb_Arena* a, const void* owner) { wpi_upb_Arena_Free(a); }

void WPI_UPB_PRIVATE(_wpi_upb_Arena_SwapIn)(wpi_upb_Arena* des, const wpi_upb_Arena* src) {
  wpi_upb_ArenaInternal* desi = wpi_upb_Arena_Internal(des);
  wpi_upb_ArenaInternal* srci = wpi_upb_Arena_Internal(src);

  *des = *src;
  desi->block_alloc = srci->block_alloc;
  wpi_upb_MemBlock* blocks = wpi_upb_Atomic_Load(&srci->blocks, memory_order_relaxed);
  wpi_upb_Atomic_Init(&desi->blocks, blocks);
}

void WPI_UPB_PRIVATE(_wpi_upb_Arena_SwapOut)(wpi_upb_Arena* des, const wpi_upb_Arena* src) {
  wpi_upb_ArenaInternal* desi = wpi_upb_Arena_Internal(des);
  wpi_upb_ArenaInternal* srci = wpi_upb_Arena_Internal(src);

  *des = *src;
  wpi_upb_MemBlock* blocks = wpi_upb_Atomic_Load(&srci->blocks, memory_order_relaxed);
  wpi_upb_Atomic_Store(&desi->blocks, blocks, memory_order_relaxed);
}


#include <string.h>


// Must be last.

bool wpi_upb_Message_SetMapEntry(wpi_upb_Map* map, const wpi_upb_MiniTable* m,
                             const wpi_upb_MiniTableField* f,
                             wpi_upb_Message* map_entry_message, wpi_upb_Arena* arena) {
  WPI_UPB_ASSERT(!wpi_upb_Message_IsFrozen(map_entry_message));
  const wpi_upb_MiniTable* map_entry_mini_table =
      wpi_upb_MiniTable_MapEntrySubMessage(m, f);
  WPI_UPB_ASSERT(map_entry_mini_table);
  const wpi_upb_MiniTableField* map_entry_key_field =
      wpi_upb_MiniTable_MapKey(map_entry_mini_table);
  const wpi_upb_MiniTableField* map_entry_value_field =
      wpi_upb_MiniTable_MapValue(map_entry_mini_table);
  // Map key/value cannot have explicit defaults,
  // hence assuming a zero default is valid.
  wpi_upb_MessageValue default_val;
  memset(&default_val, 0, sizeof(wpi_upb_MessageValue));
  wpi_upb_MessageValue map_entry_key =
      wpi_upb_Message_GetField(map_entry_message, map_entry_key_field, default_val);
  wpi_upb_MessageValue map_entry_value = wpi_upb_Message_GetField(
      map_entry_message, map_entry_value_field, default_val);
  return wpi_upb_Map_Set(map, map_entry_key, map_entry_value, arena);
}


#include <stdint.h>
#include <string.h>


// Must be last.

wpi_upb_Array* wpi_upb_Array_New(wpi_upb_Arena* a, wpi_upb_CType type) {
  const int lg2 = WPI_UPB_PRIVATE(_wpi_upb_CType_SizeLg2)(type);
  return WPI_UPB_PRIVATE(_wpi_upb_Array_New)(a, 4, lg2);
}

wpi_upb_MessageValue wpi_upb_Array_Get(const wpi_upb_Array* arr, size_t i) {
  WPI_UPB_ASSERT(i < wpi_upb_Array_Size(arr));
  wpi_upb_MessageValue ret;
  const char* data = wpi_upb_Array_DataPtr(arr);
  const int lg2 = WPI_UPB_PRIVATE(_wpi_upb_Array_ElemSizeLg2)(arr);
  memcpy(&ret, data + (i << lg2), 1 << lg2);
  return ret;
}

wpi_upb_MutableMessageValue wpi_upb_Array_GetMutable(wpi_upb_Array* arr, size_t i) {
  WPI_UPB_ASSERT(i < wpi_upb_Array_Size(arr));
  wpi_upb_MutableMessageValue ret;
  char* data = wpi_upb_Array_MutableDataPtr(arr);
  const int lg2 = WPI_UPB_PRIVATE(_wpi_upb_Array_ElemSizeLg2)(arr);
  memcpy(&ret, data + (i << lg2), 1 << lg2);
  return ret;
}

void wpi_upb_Array_Set(wpi_upb_Array* arr, size_t i, wpi_upb_MessageValue val) {
  WPI_UPB_ASSERT(!wpi_upb_Array_IsFrozen(arr));
  WPI_UPB_ASSERT(i < wpi_upb_Array_Size(arr));
  char* data = wpi_upb_Array_MutableDataPtr(arr);
  const int lg2 = WPI_UPB_PRIVATE(_wpi_upb_Array_ElemSizeLg2)(arr);
  memcpy(data + (i << lg2), &val, 1 << lg2);
}

bool wpi_upb_Array_Append(wpi_upb_Array* arr, wpi_upb_MessageValue val, wpi_upb_Arena* arena) {
  WPI_UPB_ASSERT(!wpi_upb_Array_IsFrozen(arr));
  WPI_UPB_ASSERT(arena);
  if (!WPI_UPB_PRIVATE(_wpi_upb_Array_ResizeUninitialized)(
          arr, arr->WPI_UPB_PRIVATE(size) + 1, arena)) {
    return false;
  }
  wpi_upb_Array_Set(arr, arr->WPI_UPB_PRIVATE(size) - 1, val);
  return true;
}

void wpi_upb_Array_Move(wpi_upb_Array* arr, size_t dst_idx, size_t src_idx,
                    size_t count) {
  WPI_UPB_ASSERT(!wpi_upb_Array_IsFrozen(arr));
  const int lg2 = WPI_UPB_PRIVATE(_wpi_upb_Array_ElemSizeLg2)(arr);
  char* data = wpi_upb_Array_MutableDataPtr(arr);
  memmove(&data[dst_idx << lg2], &data[src_idx << lg2], count << lg2);
}

bool wpi_upb_Array_Insert(wpi_upb_Array* arr, size_t i, size_t count,
                      wpi_upb_Arena* arena) {
  WPI_UPB_ASSERT(!wpi_upb_Array_IsFrozen(arr));
  WPI_UPB_ASSERT(arena);
  WPI_UPB_ASSERT(i <= arr->WPI_UPB_PRIVATE(size));
  WPI_UPB_ASSERT(count + arr->WPI_UPB_PRIVATE(size) >= count);
  const size_t oldsize = arr->WPI_UPB_PRIVATE(size);
  if (!WPI_UPB_PRIVATE(_wpi_upb_Array_ResizeUninitialized)(
          arr, arr->WPI_UPB_PRIVATE(size) + count, arena)) {
    return false;
  }
  wpi_upb_Array_Move(arr, i + count, i, oldsize - i);
  return true;
}

/*
 *              i        end      arr->size
 * |------------|XXXXXXXX|--------|
 */
void wpi_upb_Array_Delete(wpi_upb_Array* arr, size_t i, size_t count) {
  WPI_UPB_ASSERT(!wpi_upb_Array_IsFrozen(arr));
  const size_t end = i + count;
  WPI_UPB_ASSERT(i <= end);
  WPI_UPB_ASSERT(end <= arr->WPI_UPB_PRIVATE(size));
  wpi_upb_Array_Move(arr, i, end, arr->WPI_UPB_PRIVATE(size) - end);
  arr->WPI_UPB_PRIVATE(size) -= count;
}

bool wpi_upb_Array_Resize(wpi_upb_Array* arr, size_t size, wpi_upb_Arena* arena) {
  WPI_UPB_ASSERT(!wpi_upb_Array_IsFrozen(arr));
  const size_t oldsize = arr->WPI_UPB_PRIVATE(size);
  if (WPI_UPB_UNLIKELY(
          !WPI_UPB_PRIVATE(_wpi_upb_Array_ResizeUninitialized)(arr, size, arena))) {
    return false;
  }
  const size_t newsize = arr->WPI_UPB_PRIVATE(size);
  if (newsize > oldsize) {
    const int lg2 = WPI_UPB_PRIVATE(_wpi_upb_Array_ElemSizeLg2)(arr);
    char* data = wpi_upb_Array_MutableDataPtr(arr);
    memset(data + (oldsize << lg2), 0, (newsize - oldsize) << lg2);
  }
  return true;
}

bool WPI_UPB_PRIVATE(_wpi_upb_Array_Realloc)(wpi_upb_Array* array, size_t min_capacity,
                                     wpi_upb_Arena* arena) {
  size_t new_capacity = WPI_UPB_MAX(array->WPI_UPB_PRIVATE(capacity), 4);
  const int lg2 = WPI_UPB_PRIVATE(_wpi_upb_Array_ElemSizeLg2)(array);
  size_t old_bytes = array->WPI_UPB_PRIVATE(capacity) << lg2;
  void* ptr = wpi_upb_Array_MutableDataPtr(array);

  // Log2 ceiling of size.
  while (new_capacity < min_capacity) new_capacity *= 2;

  const size_t new_bytes = new_capacity << lg2;
  ptr = wpi_upb_Arena_Realloc(arena, ptr, old_bytes, new_bytes);
  if (!ptr) return false;

  WPI_UPB_PRIVATE(_wpi_upb_Array_SetTaggedPtr)(array, ptr, lg2);
  array->WPI_UPB_PRIVATE(capacity) = new_capacity;
  return true;
}

void wpi_upb_Array_Freeze(wpi_upb_Array* arr, const wpi_upb_MiniTable* m) {
  if (wpi_upb_Array_IsFrozen(arr)) return;
  WPI_UPB_PRIVATE(_wpi_upb_Array_ShallowFreeze)(arr);

  if (m) {
    const size_t size = wpi_upb_Array_Size(arr);

    for (size_t i = 0; i < size; i++) {
      wpi_upb_MessageValue val = wpi_upb_Array_Get(arr, i);
      wpi_upb_Message_Freeze((wpi_upb_Message*)val.msg_val, m);
    }
  }
}


#include <stddef.h>
#include <stdint.h>


// Must be last.

const wpi_upb_MiniTableExtension* wpi_upb_Message_ExtensionByIndex(
    const wpi_upb_Message* msg, size_t index) {
  size_t count;
  const wpi_upb_Extension* ext = WPI_UPB_PRIVATE(_wpi_upb_Message_Getexts)(msg, &count);

  WPI_UPB_ASSERT(index < count);
  return ext[index].ext;
}

const wpi_upb_MiniTableExtension* wpi_upb_Message_FindExtensionByNumber(
    const wpi_upb_Message* msg, uint32_t field_number) {
  size_t count;
  const wpi_upb_Extension* ext = WPI_UPB_PRIVATE(_wpi_upb_Message_Getexts)(msg, &count);

  for (; count--; ext++) {
    const wpi_upb_MiniTableExtension* e = ext->ext;
    if (wpi_upb_MiniTableExtension_Number(e) == field_number) return e;
  }
  return NULL;
}


#include <stdint.h>
#include <string.h>


// Must be last.

// Strings/bytes are special-cased in maps.
char _wpi_upb_Map_CTypeSizeTable[12] = {
    [kUpb_CType_Bool] = 1,
    [kUpb_CType_Float] = 4,
    [kUpb_CType_Int32] = 4,
    [kUpb_CType_UInt32] = 4,
    [kUpb_CType_Enum] = 4,
    [kUpb_CType_Message] = sizeof(void*),
    [kUpb_CType_Double] = 8,
    [kUpb_CType_Int64] = 8,
    [kUpb_CType_UInt64] = 8,
    [kUpb_CType_String] = WPI_UPB_MAPTYPE_STRING,
    [kUpb_CType_Bytes] = WPI_UPB_MAPTYPE_STRING,
};

wpi_upb_Map* wpi_upb_Map_New(wpi_upb_Arena* a, wpi_upb_CType key_type, wpi_upb_CType value_type) {
  return _wpi_upb_Map_New(a, _wpi_upb_Map_CTypeSize(key_type),
                      _wpi_upb_Map_CTypeSize(value_type));
}

size_t wpi_upb_Map_Size(const wpi_upb_Map* map) { return _wpi_upb_Map_Size(map); }

bool wpi_upb_Map_Get(const wpi_upb_Map* map, wpi_upb_MessageValue key,
                 wpi_upb_MessageValue* val) {
  return _wpi_upb_Map_Get(map, &key, map->key_size, val, map->val_size);
}

void wpi_upb_Map_Clear(wpi_upb_Map* map) { _wpi_upb_Map_Clear(map); }

wpi_upb_MapInsertStatus wpi_upb_Map_Insert(wpi_upb_Map* map, wpi_upb_MessageValue key,
                                   wpi_upb_MessageValue val, wpi_upb_Arena* arena) {
  WPI_UPB_ASSERT(arena);
  return (wpi_upb_MapInsertStatus)_wpi_upb_Map_Insert(map, &key, map->key_size, &val,
                                              map->val_size, arena);
}

bool wpi_upb_Map_Delete(wpi_upb_Map* map, wpi_upb_MessageValue key, wpi_upb_MessageValue* val) {
  wpi_upb_value v;
  const bool removed = _wpi_upb_Map_Delete(map, &key, map->key_size, &v);
  if (val) _wpi_upb_map_fromvalue(v, val, map->val_size);
  return removed;
}

bool wpi_upb_Map_Next(const wpi_upb_Map* map, wpi_upb_MessageValue* key,
                  wpi_upb_MessageValue* val, size_t* iter) {
  wpi_upb_StringView k;
  wpi_upb_value v;
  const bool ok = wpi_upb_strtable_next2(&map->table, &k, &v, (intptr_t*)iter);
  if (ok) {
    _wpi_upb_map_fromkey(k, key, map->key_size);
    _wpi_upb_map_fromvalue(v, val, map->val_size);
  }
  return ok;
}

WPI_UPB_API void wpi_upb_Map_SetEntryValue(wpi_upb_Map* map, size_t iter,
                                   wpi_upb_MessageValue val) {
  wpi_upb_value v;
  _wpi_upb_map_tovalue(&val, map->val_size, &v, NULL);
  wpi_upb_strtable_setentryvalue(&map->table, iter, v);
}

bool wpi_upb_MapIterator_Next(const wpi_upb_Map* map, size_t* iter) {
  return _wpi_upb_map_next(map, iter);
}

bool wpi_upb_MapIterator_Done(const wpi_upb_Map* map, size_t iter) {
  wpi_upb_strtable_iter i;
  WPI_UPB_ASSERT(iter != kUpb_Map_Begin);
  i.t = &map->table;
  i.index = iter;
  return wpi_upb_strtable_done(&i);
}

// Returns the key and value for this entry of the map.
wpi_upb_MessageValue wpi_upb_MapIterator_Key(const wpi_upb_Map* map, size_t iter) {
  wpi_upb_strtable_iter i;
  wpi_upb_MessageValue ret;
  i.t = &map->table;
  i.index = iter;
  _wpi_upb_map_fromkey(wpi_upb_strtable_iter_key(&i), &ret, map->key_size);
  return ret;
}

wpi_upb_MessageValue wpi_upb_MapIterator_Value(const wpi_upb_Map* map, size_t iter) {
  wpi_upb_strtable_iter i;
  wpi_upb_MessageValue ret;
  i.t = &map->table;
  i.index = iter;
  _wpi_upb_map_fromvalue(wpi_upb_strtable_iter_value(&i), &ret, map->val_size);
  return ret;
}

void wpi_upb_Map_Freeze(wpi_upb_Map* map, const wpi_upb_MiniTable* m) {
  if (wpi_upb_Map_IsFrozen(map)) return;
  WPI_UPB_PRIVATE(_wpi_upb_Map_ShallowFreeze)(map);

  if (m) {
    size_t iter = kUpb_Map_Begin;
    wpi_upb_MessageValue key, val;

    while (wpi_upb_Map_Next(map, &key, &val, &iter)) {
      wpi_upb_Message_Freeze((wpi_upb_Message*)val.msg_val, m);
    }
  }
}

// EVERYTHING BELOW THIS LINE IS INTERNAL - DO NOT USE /////////////////////////

wpi_upb_Map* _wpi_upb_Map_New(wpi_upb_Arena* a, size_t key_size, size_t value_size) {
  wpi_upb_Map* map = wpi_upb_Arena_Malloc(a, sizeof(wpi_upb_Map));
  if (!map) return NULL;

  wpi_upb_strtable_init(&map->table, 4, a);
  map->key_size = key_size;
  map->val_size = value_size;
  map->WPI_UPB_PRIVATE(is_frozen) = false;

  return map;
}


#include <stdint.h>
#include <string.h>


// Must be last.

static void _wpi_upb_mapsorter_getkeys(const void* _a, const void* _b, void* a_key,
                                   void* b_key, size_t size) {
  const wpi_upb_tabent* const* a = _a;
  const wpi_upb_tabent* const* b = _b;
  wpi_upb_StringView a_tabkey = wpi_upb_tabstrview((*a)->key);
  wpi_upb_StringView b_tabkey = wpi_upb_tabstrview((*b)->key);
  _wpi_upb_map_fromkey(a_tabkey, a_key, size);
  _wpi_upb_map_fromkey(b_tabkey, b_key, size);
}

static int _wpi_upb_mapsorter_cmpi64(const void* _a, const void* _b) {
  int64_t a, b;
  _wpi_upb_mapsorter_getkeys(_a, _b, &a, &b, 8);
  return a < b ? -1 : a > b;
}

static int _wpi_upb_mapsorter_cmpu64(const void* _a, const void* _b) {
  uint64_t a, b;
  _wpi_upb_mapsorter_getkeys(_a, _b, &a, &b, 8);
  return a < b ? -1 : a > b;
}

static int _wpi_upb_mapsorter_cmpi32(const void* _a, const void* _b) {
  int32_t a, b;
  _wpi_upb_mapsorter_getkeys(_a, _b, &a, &b, 4);
  return a < b ? -1 : a > b;
}

static int _wpi_upb_mapsorter_cmpu32(const void* _a, const void* _b) {
  uint32_t a, b;
  _wpi_upb_mapsorter_getkeys(_a, _b, &a, &b, 4);
  return a < b ? -1 : a > b;
}

static int _wpi_upb_mapsorter_cmpbool(const void* _a, const void* _b) {
  bool a, b;
  _wpi_upb_mapsorter_getkeys(_a, _b, &a, &b, 1);
  return a < b ? -1 : a > b;
}

static int _wpi_upb_mapsorter_cmpstr(const void* _a, const void* _b) {
  wpi_upb_StringView a, b;
  _wpi_upb_mapsorter_getkeys(_a, _b, &a, &b, WPI_UPB_MAPTYPE_STRING);
  size_t common_size = WPI_UPB_MIN(a.size, b.size);
  int cmp = memcmp(a.data, b.data, common_size);
  if (cmp) return -cmp;
  return a.size < b.size ? -1 : a.size > b.size;
}

static int (*const compar[kUpb_FieldType_SizeOf])(const void*, const void*) = {
    [kUpb_FieldType_Int64] = _wpi_upb_mapsorter_cmpi64,
    [kUpb_FieldType_SFixed64] = _wpi_upb_mapsorter_cmpi64,
    [kUpb_FieldType_SInt64] = _wpi_upb_mapsorter_cmpi64,

    [kUpb_FieldType_UInt64] = _wpi_upb_mapsorter_cmpu64,
    [kUpb_FieldType_Fixed64] = _wpi_upb_mapsorter_cmpu64,

    [kUpb_FieldType_Int32] = _wpi_upb_mapsorter_cmpi32,
    [kUpb_FieldType_SInt32] = _wpi_upb_mapsorter_cmpi32,
    [kUpb_FieldType_SFixed32] = _wpi_upb_mapsorter_cmpi32,
    [kUpb_FieldType_Enum] = _wpi_upb_mapsorter_cmpi32,

    [kUpb_FieldType_UInt32] = _wpi_upb_mapsorter_cmpu32,
    [kUpb_FieldType_Fixed32] = _wpi_upb_mapsorter_cmpu32,

    [kUpb_FieldType_Bool] = _wpi_upb_mapsorter_cmpbool,

    [kUpb_FieldType_String] = _wpi_upb_mapsorter_cmpstr,
    [kUpb_FieldType_Bytes] = _wpi_upb_mapsorter_cmpstr,
};

static bool _wpi_upb_mapsorter_resize(_wpi_upb_mapsorter* s, _wpi_upb_sortedmap* sorted,
                                  int size) {
  sorted->start = s->size;
  sorted->pos = sorted->start;
  sorted->end = sorted->start + size;

  if (sorted->end > s->cap) {
    const int oldsize = s->cap * sizeof(*s->entries);
    s->cap = wpi_upb_Log2CeilingSize(sorted->end);
    const int newsize = s->cap * sizeof(*s->entries);
    s->entries = wpi_upb_grealloc(s->entries, oldsize, newsize);
    if (!s->entries) return false;
  }

  s->size = sorted->end;
  return true;
}

bool _wpi_upb_mapsorter_pushmap(_wpi_upb_mapsorter* s, wpi_upb_FieldType key_type,
                            const wpi_upb_Map* map, _wpi_upb_sortedmap* sorted) {
  int map_size = _wpi_upb_Map_Size(map);
  WPI_UPB_ASSERT(map_size);

  if (!_wpi_upb_mapsorter_resize(s, sorted, map_size)) return false;

  // Copy non-empty entries from the table to s->entries.
  const void** dst = &s->entries[sorted->start];
  const wpi_upb_tabent* src = map->table.t.entries;
  const wpi_upb_tabent* end = src + wpi_upb_table_size(&map->table.t);
  for (; src < end; src++) {
    if (!wpi_upb_tabent_isempty(src)) {
      *dst = src;
      dst++;
    }
  }
  WPI_UPB_ASSERT(dst == &s->entries[sorted->end]);

  // Sort entries according to the key type.
  qsort(&s->entries[sorted->start], map_size, sizeof(*s->entries),
        compar[key_type]);
  return true;
}

static int _wpi_upb_mapsorter_cmpext(const void* _a, const void* _b) {
  const wpi_upb_Extension* const* a = _a;
  const wpi_upb_Extension* const* b = _b;
  uint32_t a_num = wpi_upb_MiniTableExtension_Number((*a)->ext);
  uint32_t b_num = wpi_upb_MiniTableExtension_Number((*b)->ext);
  assert(a_num != b_num);
  return a_num < b_num ? -1 : 1;
}

bool _wpi_upb_mapsorter_pushexts(_wpi_upb_mapsorter* s, const wpi_upb_Extension* exts,
                             size_t count, _wpi_upb_sortedmap* sorted) {
  if (!_wpi_upb_mapsorter_resize(s, sorted, count)) return false;

  for (size_t i = 0; i < count; i++) {
    s->entries[sorted->start + i] = &exts[i];
  }

  qsort(&s->entries[sorted->start], count, sizeof(*s->entries),
        _wpi_upb_mapsorter_cmpext);
  return true;
}


#include <stddef.h>
#include <stdint.h>
#include <string.h>


// Must be last.

static const size_t message_overhead = sizeof(wpi_upb_Message_Internal);

wpi_upb_Message* wpi_upb_Message_New(const wpi_upb_MiniTable* m, wpi_upb_Arena* a) {
  return _wpi_upb_Message_New(m, a);
}

bool WPI_UPB_PRIVATE(_wpi_upb_Message_AddUnknown)(wpi_upb_Message* msg, const char* data,
                                          size_t len, wpi_upb_Arena* arena) {
  WPI_UPB_ASSERT(!wpi_upb_Message_IsFrozen(msg));
  if (!WPI_UPB_PRIVATE(_wpi_upb_Message_Realloc)(msg, len, arena)) return false;
  wpi_upb_Message_Internal* in = WPI_UPB_PRIVATE(_wpi_upb_Message_GetInternal)(msg);
  memcpy(WPI_UPB_PTR_AT(in, in->unknown_end, char), data, len);
  in->unknown_end += len;
  return true;
}

void _wpi_upb_Message_DiscardUnknown_shallow(wpi_upb_Message* msg) {
  WPI_UPB_ASSERT(!wpi_upb_Message_IsFrozen(msg));
  wpi_upb_Message_Internal* in = WPI_UPB_PRIVATE(_wpi_upb_Message_GetInternal)(msg);
  if (in) {
    in->unknown_end = message_overhead;
  }
}

const char* wpi_upb_Message_GetUnknown(const wpi_upb_Message* msg, size_t* len) {
  wpi_upb_Message_Internal* in = WPI_UPB_PRIVATE(_wpi_upb_Message_GetInternal)(msg);
  if (in) {
    *len = in->unknown_end - message_overhead;
    return (char*)(in + 1);
  } else {
    *len = 0;
    return NULL;
  }
}

void wpi_upb_Message_DeleteUnknown(wpi_upb_Message* msg, const char* data, size_t len) {
  WPI_UPB_ASSERT(!wpi_upb_Message_IsFrozen(msg));
  wpi_upb_Message_Internal* in = WPI_UPB_PRIVATE(_wpi_upb_Message_GetInternal)(msg);
  const char* internal_unknown_end = WPI_UPB_PTR_AT(in, in->unknown_end, char);

#ifndef NDEBUG
  size_t full_unknown_size;
  const char* full_unknown = wpi_upb_Message_GetUnknown(msg, &full_unknown_size);
  WPI_UPB_ASSERT((uintptr_t)data >= (uintptr_t)full_unknown);
  WPI_UPB_ASSERT((uintptr_t)data < (uintptr_t)(full_unknown + full_unknown_size));
  WPI_UPB_ASSERT((uintptr_t)(data + len) > (uintptr_t)data);
  WPI_UPB_ASSERT((uintptr_t)(data + len) <= (uintptr_t)internal_unknown_end);
#endif

  if ((data + len) != internal_unknown_end) {
    memmove((char*)data, data + len, internal_unknown_end - data - len);
  }
  in->unknown_end -= len;
}

size_t wpi_upb_Message_ExtensionCount(const wpi_upb_Message* msg) {
  size_t count;
  WPI_UPB_PRIVATE(_wpi_upb_Message_Getexts)(msg, &count);
  return count;
}

void wpi_upb_Message_Freeze(wpi_upb_Message* msg, const wpi_upb_MiniTable* m) {
  if (wpi_upb_Message_IsFrozen(msg)) return;
  WPI_UPB_PRIVATE(_wpi_upb_Message_ShallowFreeze)(msg);

  // Base Fields.
  const size_t field_count = wpi_upb_MiniTable_FieldCount(m);

  for (size_t i = 0; i < field_count; i++) {
    const wpi_upb_MiniTableField* f = wpi_upb_MiniTable_GetFieldByIndex(m, i);
    const wpi_upb_MiniTable* m2 = wpi_upb_MiniTable_SubMessage(m, f);

    switch (WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_Mode)(f)) {
      case kUpb_FieldMode_Array: {
        wpi_upb_Array* arr = wpi_upb_Message_GetMutableArray(msg, f);
        if (arr) wpi_upb_Array_Freeze(arr, m2);
        break;
      }
      case kUpb_FieldMode_Map: {
        wpi_upb_Map* map = wpi_upb_Message_GetMutableMap(msg, f);
        if (map) {
          const wpi_upb_MiniTableField* f2 = wpi_upb_MiniTable_MapValue(m2);
          const wpi_upb_MiniTable* m3 = wpi_upb_MiniTable_SubMessage(m2, f2);
          wpi_upb_Map_Freeze(map, m3);
        }
        break;
      }
      case kUpb_FieldMode_Scalar: {
        if (m2) {
          wpi_upb_Message* msg2 = wpi_upb_Message_GetMutableMessage(msg, f);
          if (msg2) wpi_upb_Message_Freeze(msg2, m2);
        }
        break;
      }
    }
  }

  // Extensions.
  size_t ext_count;
  const wpi_upb_Extension* ext = WPI_UPB_PRIVATE(_wpi_upb_Message_Getexts)(msg, &ext_count);

  for (size_t i = 0; i < ext_count; i++) {
    const wpi_upb_MiniTableExtension* e = ext[i].ext;
    const wpi_upb_MiniTableField* f = &e->WPI_UPB_PRIVATE(field);
    const wpi_upb_MiniTable* m2 = wpi_upb_MiniTableExtension_GetSubMessage(e);

    wpi_upb_MessageValue val;
    memcpy(&val, &ext[i].data, sizeof(wpi_upb_MessageValue));

    switch (WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_Mode)(f)) {
      case kUpb_FieldMode_Array: {
        wpi_upb_Array* arr = (wpi_upb_Array*)val.array_val;
        if (arr) wpi_upb_Array_Freeze(arr, m2);
        break;
      }
      case kUpb_FieldMode_Map:
        WPI_UPB_UNREACHABLE();  // Maps cannot be extensions.
        break;
      case kUpb_FieldMode_Scalar:
        if (wpi_upb_MiniTableField_IsSubMessage(f)) {
          wpi_upb_Message* msg2 = (wpi_upb_Message*)val.msg_val;
          if (msg2) wpi_upb_Message_Freeze(msg2, m2);
        }
        break;
    }
  }
}


#include <stddef.h>


// Must be last.


#ifdef __cplusplus
extern "C" {
#endif

bool wpi_upb_Message_IsEmpty(const wpi_upb_Message* msg, const wpi_upb_MiniTable* m) {
  if (wpi_upb_Message_ExtensionCount(msg)) return false;

  const wpi_upb_MiniTableField* f;
  wpi_upb_MessageValue v;
  size_t iter = kUpb_BaseField_Begin;
  return !WPI_UPB_PRIVATE(_wpi_upb_Message_NextBaseField)(msg, m, &f, &v, &iter);
}

static bool _wpi_upb_Array_IsEqual(const wpi_upb_Array* arr1, const wpi_upb_Array* arr2,
                               wpi_upb_CType ctype, const wpi_upb_MiniTable* m,
                               int options) {
  // Check for trivial equality.
  if (arr1 == arr2) return true;

  // Must have identical element counts.
  const size_t size1 = arr1 ? wpi_upb_Array_Size(arr1) : 0;
  const size_t size2 = arr2 ? wpi_upb_Array_Size(arr2) : 0;
  if (size1 != size2) return false;

  for (size_t i = 0; i < size1; i++) {
    const wpi_upb_MessageValue val1 = wpi_upb_Array_Get(arr1, i);
    const wpi_upb_MessageValue val2 = wpi_upb_Array_Get(arr2, i);

    if (!wpi_upb_MessageValue_IsEqual(val1, val2, ctype, m, options)) return false;
  }

  return true;
}

static bool _wpi_upb_Map_IsEqual(const wpi_upb_Map* map1, const wpi_upb_Map* map2,
                             const wpi_upb_MiniTable* m, int options) {
  // Check for trivial equality.
  if (map1 == map2) return true;

  // Must have identical element counts.
  size_t size1 = map1 ? wpi_upb_Map_Size(map1) : 0;
  size_t size2 = map2 ? wpi_upb_Map_Size(map2) : 0;
  if (size1 != size2) return false;

  const wpi_upb_MiniTableField* f = wpi_upb_MiniTable_MapValue(m);
  const wpi_upb_MiniTable* m2_value = wpi_upb_MiniTable_SubMessage(m, f);
  const wpi_upb_CType ctype = wpi_upb_MiniTableField_CType(f);

  wpi_upb_MessageValue key, val1, val2;
  size_t iter = kUpb_Map_Begin;
  while (wpi_upb_Map_Next(map1, &key, &val1, &iter)) {
    if (!wpi_upb_Map_Get(map2, key, &val2)) return false;
    if (!wpi_upb_MessageValue_IsEqual(val1, val2, ctype, m2_value, options))
      return false;
  }

  return true;
}

static bool _wpi_upb_Message_BaseFieldsAreEqual(const wpi_upb_Message* msg1,
                                            const wpi_upb_Message* msg2,
                                            const wpi_upb_MiniTable* m,
                                            int options) {
  // Iterate over all base fields for each message.
  // The order will always match if the messages are equal.
  size_t iter1 = kUpb_BaseField_Begin;
  size_t iter2 = kUpb_BaseField_Begin;

  for (;;) {
    const wpi_upb_MiniTableField *f1, *f2;
    wpi_upb_MessageValue val1, val2;

    const bool got1 =
        WPI_UPB_PRIVATE(_wpi_upb_Message_NextBaseField)(msg1, m, &f1, &val1, &iter1);
    const bool got2 =
        WPI_UPB_PRIVATE(_wpi_upb_Message_NextBaseField)(msg2, m, &f2, &val2, &iter2);

    if (got1 != got2) return false;  // Must have identical field counts.
    if (!got1) return true;          // Loop termination condition.
    if (f1 != f2) return false;      // Must have identical fields set.

    const wpi_upb_MiniTable* subm = wpi_upb_MiniTable_SubMessage(m, f1);
    const wpi_upb_CType ctype = wpi_upb_MiniTableField_CType(f1);

    bool eq;
    switch (WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_Mode)(f1)) {
      case kUpb_FieldMode_Array:
        eq = _wpi_upb_Array_IsEqual(val1.array_val, val2.array_val, ctype, subm,
                                options);
        break;
      case kUpb_FieldMode_Map:
        eq = _wpi_upb_Map_IsEqual(val1.map_val, val2.map_val, subm, options);
        break;
      case kUpb_FieldMode_Scalar:
        eq = wpi_upb_MessageValue_IsEqual(val1, val2, ctype, subm, options);
        break;
    }
    if (!eq) return false;
  }
}

static bool _wpi_upb_Message_ExtensionsAreEqual(const wpi_upb_Message* msg1,
                                            const wpi_upb_Message* msg2,
                                            const wpi_upb_MiniTable* m,
                                            int options) {
  // Must have identical extension counts.
  if (wpi_upb_Message_ExtensionCount(msg1) != wpi_upb_Message_ExtensionCount(msg2)) {
    return false;
  }

  const wpi_upb_MiniTableExtension* e;
  wpi_upb_MessageValue val1;

  // Iterate over all extensions for msg1, and search msg2 for each extension.
  size_t iter1 = kUpb_Extension_Begin;
  while (WPI_UPB_PRIVATE(_wpi_upb_Message_NextExtension)(msg1, m, &e, &val1, &iter1)) {
    const wpi_upb_Extension* ext2 = WPI_UPB_PRIVATE(_wpi_upb_Message_Getext)(msg2, e);
    if (!ext2) return false;

    const wpi_upb_MessageValue val2 = ext2->data;
    const wpi_upb_MiniTableField* f = &e->WPI_UPB_PRIVATE(field);
    const wpi_upb_MiniTable* subm = wpi_upb_MiniTableField_IsSubMessage(f)
                                    ? wpi_upb_MiniTableExtension_GetSubMessage(e)
                                    : NULL;
    const wpi_upb_CType ctype = wpi_upb_MiniTableField_CType(f);

    bool eq;
    switch (WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_Mode)(f)) {
      case kUpb_FieldMode_Array:
        eq = _wpi_upb_Array_IsEqual(val1.array_val, val2.array_val, ctype, subm,
                                options);
        break;
      case kUpb_FieldMode_Map:
        WPI_UPB_UNREACHABLE();  // Maps cannot be extensions.
        break;
      case kUpb_FieldMode_Scalar: {
        eq = wpi_upb_MessageValue_IsEqual(val1, val2, ctype, subm, options);
        break;
      }
    }
    if (!eq) return false;
  }
  return true;
}

bool wpi_upb_Message_IsEqual(const wpi_upb_Message* msg1, const wpi_upb_Message* msg2,
                         const wpi_upb_MiniTable* m, int options) {
  if (WPI_UPB_UNLIKELY(msg1 == msg2)) return true;

  if (!_wpi_upb_Message_BaseFieldsAreEqual(msg1, msg2, m, options)) return false;
  if (!_wpi_upb_Message_ExtensionsAreEqual(msg1, msg2, m, options)) return false;

  if (!(options & kUpb_CompareOption_IncludeUnknownFields)) return true;

  // Check the unknown fields.
  size_t usize1, usize2;
  const char* uf1 = wpi_upb_Message_GetUnknown(msg1, &usize1);
  const char* uf2 = wpi_upb_Message_GetUnknown(msg2, &usize2);

  // The wire encoder enforces a maximum depth of 100 so we match that here.
  return WPI_UPB_PRIVATE(_wpi_upb_Message_UnknownFieldsAreEqual)(
             uf1, usize1, uf2, usize2, 100) == kUpb_UnknownCompareResult_Equal;
}


#include <stdbool.h>
#include <string.h>


// Must be last.

static wpi_upb_StringView wpi_upb_Clone_StringView(wpi_upb_StringView str,
                                           wpi_upb_Arena* arena) {
  if (str.size == 0) {
    return wpi_upb_StringView_FromDataAndSize(NULL, 0);
  }
  void* cloned_data = wpi_upb_Arena_Malloc(arena, str.size);
  wpi_upb_StringView cloned_str =
      wpi_upb_StringView_FromDataAndSize(cloned_data, str.size);
  memcpy(cloned_data, str.data, str.size);
  return cloned_str;
}

static bool wpi_upb_Clone_MessageValue(void* value, wpi_upb_CType value_type,
                                   const wpi_upb_MiniTable* sub, wpi_upb_Arena* arena) {
  switch (value_type) {
    case kUpb_CType_Bool:
    case kUpb_CType_Float:
    case kUpb_CType_Int32:
    case kUpb_CType_UInt32:
    case kUpb_CType_Enum:
    case kUpb_CType_Double:
    case kUpb_CType_Int64:
    case kUpb_CType_UInt64:
      return true;
    case kUpb_CType_String:
    case kUpb_CType_Bytes: {
      wpi_upb_StringView source = *(wpi_upb_StringView*)value;
      int size = source.size;
      void* cloned_data = wpi_upb_Arena_Malloc(arena, size);
      if (cloned_data == NULL) {
        return false;
      }
      *(wpi_upb_StringView*)value =
          wpi_upb_StringView_FromDataAndSize(cloned_data, size);
      memcpy(cloned_data, source.data, size);
      return true;
    } break;
    case kUpb_CType_Message: {
      const wpi_upb_TaggedMessagePtr source = *(wpi_upb_TaggedMessagePtr*)value;
      bool is_empty = wpi_upb_TaggedMessagePtr_IsEmpty(source);
      if (is_empty) sub = WPI_UPB_PRIVATE(_wpi_upb_MiniTable_Empty)();
      WPI_UPB_ASSERT(source);
      wpi_upb_Message* clone = wpi_upb_Message_DeepClone(
          WPI_UPB_PRIVATE(_wpi_upb_TaggedMessagePtr_GetMessage)(source), sub, arena);
      *(wpi_upb_TaggedMessagePtr*)value =
          WPI_UPB_PRIVATE(_wpi_upb_TaggedMessagePtr_Pack)(clone, is_empty);
      return clone != NULL;
    } break;
  }
  WPI_UPB_UNREACHABLE();
}

wpi_upb_Map* wpi_upb_Map_DeepClone(const wpi_upb_Map* map, wpi_upb_CType key_type,
                           wpi_upb_CType value_type,
                           const wpi_upb_MiniTable* map_entry_table,
                           wpi_upb_Arena* arena) {
  wpi_upb_Map* cloned_map = _wpi_upb_Map_New(arena, map->key_size, map->val_size);
  if (cloned_map == NULL) {
    return NULL;
  }
  wpi_upb_MessageValue key, val;
  size_t iter = kUpb_Map_Begin;
  while (wpi_upb_Map_Next(map, &key, &val, &iter)) {
    const wpi_upb_MiniTableField* value_field =
        wpi_upb_MiniTable_MapValue(map_entry_table);
    const wpi_upb_MiniTable* value_sub =
        wpi_upb_MiniTableField_CType(value_field) == kUpb_CType_Message
            ? wpi_upb_MiniTable_GetSubMessageTable(map_entry_table, value_field)
            : NULL;
    wpi_upb_CType value_field_type = wpi_upb_MiniTableField_CType(value_field);
    if (!wpi_upb_Clone_MessageValue(&val, value_field_type, value_sub, arena)) {
      return NULL;
    }
    if (!wpi_upb_Map_Set(cloned_map, key, val, arena)) {
      return NULL;
    }
  }
  return cloned_map;
}

static wpi_upb_Map* wpi_upb_Message_Map_DeepClone(const wpi_upb_Map* map,
                                          const wpi_upb_MiniTable* mini_table,
                                          const wpi_upb_MiniTableField* f,
                                          wpi_upb_Message* clone,
                                          wpi_upb_Arena* arena) {
  WPI_UPB_ASSERT(!wpi_upb_Message_IsFrozen(clone));
  const wpi_upb_MiniTable* map_entry_table =
      wpi_upb_MiniTable_MapEntrySubMessage(mini_table, f);
  WPI_UPB_ASSERT(map_entry_table);

  const wpi_upb_MiniTableField* key_field = wpi_upb_MiniTable_MapKey(map_entry_table);
  const wpi_upb_MiniTableField* value_field =
      wpi_upb_MiniTable_MapValue(map_entry_table);

  wpi_upb_Map* cloned_map = wpi_upb_Map_DeepClone(
      map, wpi_upb_MiniTableField_CType(key_field),
      wpi_upb_MiniTableField_CType(value_field), map_entry_table, arena);
  if (!cloned_map) {
    return NULL;
  }
  wpi_upb_Message_SetBaseField(clone, f, &cloned_map);
  return cloned_map;
}

wpi_upb_Array* wpi_upb_Array_DeepClone(const wpi_upb_Array* array, wpi_upb_CType value_type,
                               const wpi_upb_MiniTable* sub, wpi_upb_Arena* arena) {
  const size_t size = wpi_upb_Array_Size(array);
  const int lg2 = WPI_UPB_PRIVATE(_wpi_upb_CType_SizeLg2)(value_type);
  wpi_upb_Array* cloned_array = WPI_UPB_PRIVATE(_wpi_upb_Array_New)(arena, size, lg2);
  if (!cloned_array) {
    return NULL;
  }
  if (!WPI_UPB_PRIVATE(_wpi_upb_Array_ResizeUninitialized)(cloned_array, size, arena)) {
    return NULL;
  }
  for (size_t i = 0; i < size; ++i) {
    wpi_upb_MessageValue val = wpi_upb_Array_Get(array, i);
    if (!wpi_upb_Clone_MessageValue(&val, value_type, sub, arena)) {
      return false;
    }
    wpi_upb_Array_Set(cloned_array, i, val);
  }
  return cloned_array;
}

static bool wpi_upb_Message_Array_DeepClone(const wpi_upb_Array* array,
                                        const wpi_upb_MiniTable* mini_table,
                                        const wpi_upb_MiniTableField* field,
                                        wpi_upb_Message* clone, wpi_upb_Arena* arena) {
  WPI_UPB_ASSERT(!wpi_upb_Message_IsFrozen(clone));
  WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_CheckIsArray)(field);
  wpi_upb_Array* cloned_array = wpi_upb_Array_DeepClone(
      array, wpi_upb_MiniTableField_CType(field),
      wpi_upb_MiniTableField_CType(field) == kUpb_CType_Message
          ? wpi_upb_MiniTable_GetSubMessageTable(mini_table, field)
          : NULL,
      arena);

  // Clear out wpi_upb_Array* due to parent memcpy.
  wpi_upb_Message_SetBaseField(clone, field, &cloned_array);
  return true;
}

static bool wpi_upb_Clone_ExtensionValue(
    const wpi_upb_MiniTableExtension* mini_table_ext, const wpi_upb_Extension* source,
    wpi_upb_Extension* dest, wpi_upb_Arena* arena) {
  dest->data = source->data;
  return wpi_upb_Clone_MessageValue(
      &dest->data, wpi_upb_MiniTableExtension_CType(mini_table_ext),
      wpi_upb_MiniTableExtension_GetSubMessage(mini_table_ext), arena);
}

wpi_upb_Message* _wpi_upb_Message_Copy(wpi_upb_Message* dst, const wpi_upb_Message* src,
                               const wpi_upb_MiniTable* mini_table,
                               wpi_upb_Arena* arena) {
  WPI_UPB_ASSERT(!wpi_upb_Message_IsFrozen(dst));
  wpi_upb_StringView empty_string = wpi_upb_StringView_FromDataAndSize(NULL, 0);
  // Only copy message area skipping wpi_upb_Message_Internal.
  memcpy(dst + 1, src + 1, mini_table->WPI_UPB_PRIVATE(size) - sizeof(wpi_upb_Message));
  for (int i = 0; i < wpi_upb_MiniTable_FieldCount(mini_table); ++i) {
    const wpi_upb_MiniTableField* field =
        wpi_upb_MiniTable_GetFieldByIndex(mini_table, i);
    if (wpi_upb_MiniTableField_IsScalar(field)) {
      switch (wpi_upb_MiniTableField_CType(field)) {
        case kUpb_CType_Message: {
          wpi_upb_TaggedMessagePtr tagged =
              wpi_upb_Message_GetTaggedMessagePtr(src, field, NULL);
          const wpi_upb_Message* sub_message =
              WPI_UPB_PRIVATE(_wpi_upb_TaggedMessagePtr_GetMessage)(tagged);
          if (sub_message != NULL) {
            // If the message is currently in an unlinked, "empty" state we keep
            // it that way, because we don't want to deal with decode options,
            // decode status, or possible parse failure here.
            bool is_empty = wpi_upb_TaggedMessagePtr_IsEmpty(tagged);
            const wpi_upb_MiniTable* sub_message_table =
                is_empty ? WPI_UPB_PRIVATE(_wpi_upb_MiniTable_Empty)()
                         : wpi_upb_MiniTable_GetSubMessageTable(mini_table, field);
            wpi_upb_Message* dst_sub_message =
                wpi_upb_Message_DeepClone(sub_message, sub_message_table, arena);
            if (dst_sub_message == NULL) {
              return NULL;
            }
            WPI_UPB_PRIVATE(_wpi_upb_Message_SetTaggedMessagePtr)
            (dst, field,
             WPI_UPB_PRIVATE(_wpi_upb_TaggedMessagePtr_Pack)(dst_sub_message,
                                                     is_empty));
          }
        } break;
        case kUpb_CType_String:
        case kUpb_CType_Bytes: {
          wpi_upb_StringView str = wpi_upb_Message_GetString(src, field, empty_string);
          if (str.size != 0) {
            if (!wpi_upb_Message_SetString(
                    dst, field, wpi_upb_Clone_StringView(str, arena), arena)) {
              return NULL;
            }
          }
        } break;
        default:
          // Scalar, already copied.
          break;
      }
    } else {
      if (wpi_upb_MiniTableField_IsMap(field)) {
        const wpi_upb_Map* map = wpi_upb_Message_GetMap(src, field);
        if (map != NULL) {
          if (!wpi_upb_Message_Map_DeepClone(map, mini_table, field, dst, arena)) {
            return NULL;
          }
        }
      } else {
        const wpi_upb_Array* array = wpi_upb_Message_GetArray(src, field);
        if (array != NULL) {
          if (!wpi_upb_Message_Array_DeepClone(array, mini_table, field, dst,
                                           arena)) {
            return NULL;
          }
        }
      }
    }
  }
  // Clone extensions.
  size_t ext_count;
  const wpi_upb_Extension* ext = WPI_UPB_PRIVATE(_wpi_upb_Message_Getexts)(src, &ext_count);
  for (size_t i = 0; i < ext_count; ++i) {
    const wpi_upb_Extension* msg_ext = &ext[i];
    const wpi_upb_MiniTableField* field = &msg_ext->ext->WPI_UPB_PRIVATE(field);
    wpi_upb_Extension* dst_ext = WPI_UPB_PRIVATE(_wpi_upb_Message_GetOrCreateExtension)(
        dst, msg_ext->ext, arena);
    if (!dst_ext) return NULL;
    if (wpi_upb_MiniTableField_IsScalar(field)) {
      if (!wpi_upb_Clone_ExtensionValue(msg_ext->ext, msg_ext, dst_ext, arena)) {
        return NULL;
      }
    } else {
      wpi_upb_Array* msg_array = (wpi_upb_Array*)msg_ext->data.array_val;
      WPI_UPB_ASSERT(msg_array);
      wpi_upb_Array* cloned_array = wpi_upb_Array_DeepClone(
          msg_array, wpi_upb_MiniTableField_CType(field),
          wpi_upb_MiniTableExtension_GetSubMessage(msg_ext->ext), arena);
      if (!cloned_array) {
        return NULL;
      }
      dst_ext->data.array_val = cloned_array;
    }
  }

  // Clone unknowns.
  size_t unknown_size = 0;
  const char* ptr = wpi_upb_Message_GetUnknown(src, &unknown_size);
  if (unknown_size != 0) {
    WPI_UPB_ASSERT(ptr);
    // Make a copy into destination arena.
    if (!WPI_UPB_PRIVATE(_wpi_upb_Message_AddUnknown)(dst, ptr, unknown_size, arena)) {
      return NULL;
    }
  }
  return dst;
}

bool wpi_upb_Message_DeepCopy(wpi_upb_Message* dst, const wpi_upb_Message* src,
                          const wpi_upb_MiniTable* mini_table, wpi_upb_Arena* arena) {
  WPI_UPB_ASSERT(!wpi_upb_Message_IsFrozen(dst));
  wpi_upb_Message_Clear(dst, mini_table);
  return _wpi_upb_Message_Copy(dst, src, mini_table, arena) != NULL;
}

// Deep clones a message using the provided target arena.
//
// Returns NULL on failure.
wpi_upb_Message* wpi_upb_Message_DeepClone(const wpi_upb_Message* msg,
                                   const wpi_upb_MiniTable* m, wpi_upb_Arena* arena) {
  wpi_upb_Message* clone = wpi_upb_Message_New(m, arena);
  return _wpi_upb_Message_Copy(clone, msg, m, arena);
}

// Performs a shallow copy. TODO: Extend to handle unknown fields.
void wpi_upb_Message_ShallowCopy(wpi_upb_Message* dst, const wpi_upb_Message* src,
                             const wpi_upb_MiniTable* m) {
  WPI_UPB_ASSERT(!wpi_upb_Message_IsFrozen(dst));
  memcpy(dst, src, m->WPI_UPB_PRIVATE(size));
}

// Performs a shallow clone. Ignores unknown fields.
wpi_upb_Message* wpi_upb_Message_ShallowClone(const wpi_upb_Message* msg,
                                      const wpi_upb_MiniTable* m,
                                      wpi_upb_Arena* arena) {
  wpi_upb_Message* clone = wpi_upb_Message_New(m, arena);
  wpi_upb_Message_ShallowCopy(clone, msg, m);
  return clone;
}

#include "stddef.h"

// Must be last.

bool wpi_upb_Message_MergeFrom(wpi_upb_Message* dst, const wpi_upb_Message* src,
                           const wpi_upb_MiniTable* mt,
                           const wpi_upb_ExtensionRegistry* extreg,
                           wpi_upb_Arena* arena) {
  char* buf = NULL;
  size_t size = 0;
  // This tmp arena is used to hold the bytes for `src` serialized. This bends
  // the typical "no hidden allocations" design of upb, but under a properly
  // optimized implementation this extra allocation would not be necessary and
  // so we don't want to unnecessarily have the bad API or bloat the passed-in
  // arena with this very-short-term allocation.
  wpi_upb_Arena* encode_arena = wpi_upb_Arena_New();
  wpi_upb_EncodeStatus e_status = wpi_upb_Encode(src, mt, 0, encode_arena, &buf, &size);
  if (e_status != kUpb_EncodeStatus_Ok) {
    wpi_upb_Arena_Free(encode_arena);
    return false;
  }
  wpi_upb_DecodeStatus d_status = wpi_upb_Decode(buf, size, dst, mt, extreg, 0, arena);
  if (d_status != kUpb_DecodeStatus_Ok) {
    wpi_upb_Arena_Free(encode_arena);
    return false;
  }
  wpi_upb_Arena_Free(encode_arena);
  return true;
}


#include <stddef.h>
#include <stdint.h>


// Must be last.

typedef struct {
  wpi_upb_MdDecoder base;
  wpi_upb_Arena* arena;
  wpi_upb_MiniTableEnum* enum_table;
  uint32_t enum_value_count;
  uint32_t enum_data_count;
  uint32_t enum_data_capacity;
} wpi_upb_MdEnumDecoder;

static size_t wpi_upb_MiniTableEnum_Size(size_t count) {
  return sizeof(wpi_upb_MiniTableEnum) + count * sizeof(uint32_t);
}

static wpi_upb_MiniTableEnum* _wpi_upb_MiniTable_AddEnumDataMember(wpi_upb_MdEnumDecoder* d,
                                                           uint32_t val) {
  if (d->enum_data_count == d->enum_data_capacity) {
    size_t old_sz = wpi_upb_MiniTableEnum_Size(d->enum_data_capacity);
    d->enum_data_capacity = WPI_UPB_MAX(2, d->enum_data_capacity * 2);
    size_t new_sz = wpi_upb_MiniTableEnum_Size(d->enum_data_capacity);
    d->enum_table = wpi_upb_Arena_Realloc(d->arena, d->enum_table, old_sz, new_sz);
    wpi_upb_MdDecoder_CheckOutOfMemory(&d->base, d->enum_table);
  }
  d->enum_table->WPI_UPB_PRIVATE(data)[d->enum_data_count++] = val;
  return d->enum_table;
}

static void wpi_upb_MiniTableEnum_BuildValue(wpi_upb_MdEnumDecoder* d, uint32_t val) {
  wpi_upb_MiniTableEnum* table = d->enum_table;
  d->enum_value_count++;
  if (table->WPI_UPB_PRIVATE(value_count) ||
      (val > 512 && d->enum_value_count < val / 32)) {
    if (table->WPI_UPB_PRIVATE(value_count) == 0) {
      WPI_UPB_ASSERT(d->enum_data_count == table->WPI_UPB_PRIVATE(mask_limit) / 32);
    }
    table = _wpi_upb_MiniTable_AddEnumDataMember(d, val);
    table->WPI_UPB_PRIVATE(value_count)++;
  } else {
    uint32_t new_mask_limit = ((val / 32) + 1) * 32;
    while (table->WPI_UPB_PRIVATE(mask_limit) < new_mask_limit) {
      table = _wpi_upb_MiniTable_AddEnumDataMember(d, 0);
      table->WPI_UPB_PRIVATE(mask_limit) += 32;
    }
    table->WPI_UPB_PRIVATE(data)[val / 32] |= 1ULL << (val % 32);
  }
}

static wpi_upb_MiniTableEnum* wpi_upb_MtDecoder_DoBuildMiniTableEnum(
    wpi_upb_MdEnumDecoder* d, const char* data, size_t len) {
  // If the string is non-empty then it must begin with a version tag.
  if (len) {
    if (*data != kUpb_EncodedVersion_EnumV1) {
      wpi_upb_MdDecoder_ErrorJmp(&d->base, "Invalid enum version: %c", *data);
    }
    data++;
    len--;
  }

  wpi_upb_MdDecoder_CheckOutOfMemory(&d->base, d->enum_table);

  // Guarantee at least 64 bits of mask without checking mask size.
  d->enum_table->WPI_UPB_PRIVATE(mask_limit) = 64;
  d->enum_table = _wpi_upb_MiniTable_AddEnumDataMember(d, 0);
  d->enum_table = _wpi_upb_MiniTable_AddEnumDataMember(d, 0);

  d->enum_table->WPI_UPB_PRIVATE(value_count) = 0;

  const char* ptr = data;
  uint32_t base = 0;

  while (ptr < d->base.end) {
    char ch = *ptr++;
    if (ch <= kUpb_EncodedValue_MaxEnumMask) {
      uint32_t mask = _wpi_upb_FromBase92(ch);
      for (int i = 0; i < 5; i++, base++, mask >>= 1) {
        if (mask & 1) wpi_upb_MiniTableEnum_BuildValue(d, base);
      }
    } else if (kUpb_EncodedValue_MinSkip <= ch &&
               ch <= kUpb_EncodedValue_MaxSkip) {
      uint32_t skip;
      ptr = wpi_upb_MdDecoder_DecodeBase92Varint(&d->base, ptr, ch,
                                             kUpb_EncodedValue_MinSkip,
                                             kUpb_EncodedValue_MaxSkip, &skip);
      base += skip;
    } else {
      wpi_upb_MdDecoder_ErrorJmp(&d->base, "Unexpected character: %c", ch);
    }
  }

  return d->enum_table;
}

static wpi_upb_MiniTableEnum* wpi_upb_MtDecoder_BuildMiniTableEnum(
    wpi_upb_MdEnumDecoder* const decoder, const char* const data,
    size_t const len) {
  if (WPI_UPB_SETJMP(decoder->base.err) != 0) return NULL;
  return wpi_upb_MtDecoder_DoBuildMiniTableEnum(decoder, data, len);
}

wpi_upb_MiniTableEnum* wpi_upb_MiniTableEnum_Build(const char* data, size_t len,
                                           wpi_upb_Arena* arena,
                                           wpi_upb_Status* status) {
  wpi_upb_MdEnumDecoder decoder = {
      .base =
          {
              .end = WPI_UPB_PTRADD(data, len),
              .status = status,
          },
      .arena = arena,
      .enum_table = wpi_upb_Arena_Malloc(arena, wpi_upb_MiniTableEnum_Size(2)),
      .enum_value_count = 0,
      .enum_data_count = 0,
      .enum_data_capacity = 1,
  };

  return wpi_upb_MtDecoder_BuildMiniTableEnum(&decoder, data, len);
}


#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>


// Must be last.

// We reserve unused hasbits to make room for wpi_upb_Message fields.
#define kUpb_Reserved_Hasbytes sizeof(struct wpi_upb_Message)

// 64 is the first hasbit that we currently use.
#define kUpb_Reserved_Hasbits (kUpb_Reserved_Hasbytes * 8)

// Note: we sort by this number when calculating layout order.
typedef enum {
  kUpb_LayoutItemType_OneofCase,   // Oneof case.
  kUpb_LayoutItemType_OneofField,  // Oneof field data.
  kUpb_LayoutItemType_Field,       // Non-oneof field data.

  kUpb_LayoutItemType_Max = kUpb_LayoutItemType_Field,
} wpi_upb_LayoutItemType;

#define kUpb_LayoutItem_IndexSentinel ((uint16_t) - 1)

typedef struct {
  // Index of the corresponding field.  When this is a oneof field, the field's
  // offset will be the index of the next field in a linked list.
  uint16_t field_index;
  uint16_t offset;
  wpi_upb_FieldRep rep;
  wpi_upb_LayoutItemType type;
} wpi_upb_LayoutItem;

typedef struct {
  wpi_upb_LayoutItem* data;
  size_t size;
  size_t capacity;
} wpi_upb_LayoutItemVector;

typedef struct {
  wpi_upb_MdDecoder base;
  wpi_upb_MiniTable* table;
  wpi_upb_MiniTableField* fields;
  wpi_upb_MiniTablePlatform platform;
  wpi_upb_LayoutItemVector vec;
  wpi_upb_Arena* arena;
} wpi_upb_MtDecoder;

// In each field's offset, we temporarily store a presence classifier:
enum PresenceClass {
  kNoPresence = 0,
  kHasbitPresence = 1,
  kRequiredPresence = 2,
  kOneofBase = 3,
  // Negative values refer to a specific oneof with that number.  Positive
  // values >= kOneofBase indicate that this field is in a oneof, and specify
  // the next field in this oneof's linked list.
};

static bool wpi_upb_MtDecoder_FieldIsPackable(wpi_upb_MiniTableField* field) {
  return (field->WPI_UPB_PRIVATE(mode) & kUpb_FieldMode_Array) &&
         wpi_upb_FieldType_IsPackable(field->WPI_UPB_PRIVATE(descriptortype));
}

typedef struct {
  uint16_t submsg_count;
  uint16_t subenum_count;
} wpi_upb_SubCounts;

static void wpi_upb_MiniTable_SetTypeAndSub(wpi_upb_MiniTableField* field,
                                        wpi_upb_FieldType type,
                                        wpi_upb_SubCounts* sub_counts,
                                        uint64_t msg_modifiers,
                                        bool is_proto3_enum) {
  if (is_proto3_enum) {
    WPI_UPB_ASSERT(type == kUpb_FieldType_Enum);
    type = kUpb_FieldType_Int32;
    field->WPI_UPB_PRIVATE(mode) |= kUpb_LabelFlags_IsAlternate;
  } else if (type == kUpb_FieldType_String &&
             !(msg_modifiers & kUpb_MessageModifier_ValidateUtf8)) {
    type = kUpb_FieldType_Bytes;
    field->WPI_UPB_PRIVATE(mode) |= kUpb_LabelFlags_IsAlternate;
  }

  field->WPI_UPB_PRIVATE(descriptortype) = type;

  if (wpi_upb_MtDecoder_FieldIsPackable(field) &&
      (msg_modifiers & kUpb_MessageModifier_DefaultIsPacked)) {
    field->WPI_UPB_PRIVATE(mode) |= kUpb_LabelFlags_IsPacked;
  }

  if (type == kUpb_FieldType_Message || type == kUpb_FieldType_Group) {
    field->WPI_UPB_PRIVATE(submsg_index) = sub_counts->submsg_count++;
  } else if (type == kUpb_FieldType_Enum) {
    // We will need to update this later once we know the total number of
    // submsg fields.
    field->WPI_UPB_PRIVATE(submsg_index) = sub_counts->subenum_count++;
  } else {
    field->WPI_UPB_PRIVATE(submsg_index) = kUpb_NoSub;
  }
}

static const char kUpb_EncodedToType[] = {
    [kUpb_EncodedType_Double] = kUpb_FieldType_Double,
    [kUpb_EncodedType_Float] = kUpb_FieldType_Float,
    [kUpb_EncodedType_Int64] = kUpb_FieldType_Int64,
    [kUpb_EncodedType_UInt64] = kUpb_FieldType_UInt64,
    [kUpb_EncodedType_Int32] = kUpb_FieldType_Int32,
    [kUpb_EncodedType_Fixed64] = kUpb_FieldType_Fixed64,
    [kUpb_EncodedType_Fixed32] = kUpb_FieldType_Fixed32,
    [kUpb_EncodedType_Bool] = kUpb_FieldType_Bool,
    [kUpb_EncodedType_String] = kUpb_FieldType_String,
    [kUpb_EncodedType_Group] = kUpb_FieldType_Group,
    [kUpb_EncodedType_Message] = kUpb_FieldType_Message,
    [kUpb_EncodedType_Bytes] = kUpb_FieldType_Bytes,
    [kUpb_EncodedType_UInt32] = kUpb_FieldType_UInt32,
    [kUpb_EncodedType_OpenEnum] = kUpb_FieldType_Enum,
    [kUpb_EncodedType_SFixed32] = kUpb_FieldType_SFixed32,
    [kUpb_EncodedType_SFixed64] = kUpb_FieldType_SFixed64,
    [kUpb_EncodedType_SInt32] = kUpb_FieldType_SInt32,
    [kUpb_EncodedType_SInt64] = kUpb_FieldType_SInt64,
    [kUpb_EncodedType_ClosedEnum] = kUpb_FieldType_Enum,
};

static void wpi_upb_MiniTable_SetField(wpi_upb_MtDecoder* d, uint8_t ch,
                                   wpi_upb_MiniTableField* field,
                                   uint64_t msg_modifiers,
                                   wpi_upb_SubCounts* sub_counts) {
  static const char kUpb_EncodedToFieldRep[] = {
      [kUpb_EncodedType_Double] = kUpb_FieldRep_8Byte,
      [kUpb_EncodedType_Float] = kUpb_FieldRep_4Byte,
      [kUpb_EncodedType_Int64] = kUpb_FieldRep_8Byte,
      [kUpb_EncodedType_UInt64] = kUpb_FieldRep_8Byte,
      [kUpb_EncodedType_Int32] = kUpb_FieldRep_4Byte,
      [kUpb_EncodedType_Fixed64] = kUpb_FieldRep_8Byte,
      [kUpb_EncodedType_Fixed32] = kUpb_FieldRep_4Byte,
      [kUpb_EncodedType_Bool] = kUpb_FieldRep_1Byte,
      [kUpb_EncodedType_String] = kUpb_FieldRep_StringView,
      [kUpb_EncodedType_Bytes] = kUpb_FieldRep_StringView,
      [kUpb_EncodedType_UInt32] = kUpb_FieldRep_4Byte,
      [kUpb_EncodedType_OpenEnum] = kUpb_FieldRep_4Byte,
      [kUpb_EncodedType_SFixed32] = kUpb_FieldRep_4Byte,
      [kUpb_EncodedType_SFixed64] = kUpb_FieldRep_8Byte,
      [kUpb_EncodedType_SInt32] = kUpb_FieldRep_4Byte,
      [kUpb_EncodedType_SInt64] = kUpb_FieldRep_8Byte,
      [kUpb_EncodedType_ClosedEnum] = kUpb_FieldRep_4Byte,
  };

  char pointer_rep = d->platform == kUpb_MiniTablePlatform_32Bit
                         ? kUpb_FieldRep_4Byte
                         : kUpb_FieldRep_8Byte;

  int8_t type = _wpi_upb_FromBase92(ch);
  if (ch >= _wpi_upb_ToBase92(kUpb_EncodedType_RepeatedBase)) {
    type -= kUpb_EncodedType_RepeatedBase;
    field->WPI_UPB_PRIVATE(mode) = kUpb_FieldMode_Array;
    field->WPI_UPB_PRIVATE(mode) |= pointer_rep << kUpb_FieldRep_Shift;
    field->WPI_UPB_PRIVATE(offset) = kNoPresence;
  } else {
    field->WPI_UPB_PRIVATE(mode) = kUpb_FieldMode_Scalar;
    field->WPI_UPB_PRIVATE(offset) = kHasbitPresence;
    if (type == kUpb_EncodedType_Group || type == kUpb_EncodedType_Message) {
      field->WPI_UPB_PRIVATE(mode) |= pointer_rep << kUpb_FieldRep_Shift;
    } else if ((unsigned long)type >= sizeof(kUpb_EncodedToFieldRep)) {
      wpi_upb_MdDecoder_ErrorJmp(&d->base, "Invalid field type: %d", (int)type);
    } else {
      field->WPI_UPB_PRIVATE(mode) |= kUpb_EncodedToFieldRep[type]
                                  << kUpb_FieldRep_Shift;
    }
  }
  if ((unsigned long)type >= sizeof(kUpb_EncodedToType)) {
    wpi_upb_MdDecoder_ErrorJmp(&d->base, "Invalid field type: %d", (int)type);
  }
  wpi_upb_MiniTable_SetTypeAndSub(field, kUpb_EncodedToType[type], sub_counts,
                              msg_modifiers, type == kUpb_EncodedType_OpenEnum);
}

static void wpi_upb_MtDecoder_ModifyField(wpi_upb_MtDecoder* d,
                                      uint32_t message_modifiers,
                                      uint32_t field_modifiers,
                                      wpi_upb_MiniTableField* field) {
  if (field_modifiers & kUpb_EncodedFieldModifier_FlipPacked) {
    if (!wpi_upb_MtDecoder_FieldIsPackable(field)) {
      wpi_upb_MdDecoder_ErrorJmp(&d->base,
                             "Cannot flip packed on unpackable field %" PRIu32,
                             wpi_upb_MiniTableField_Number(field));
    }
    field->WPI_UPB_PRIVATE(mode) ^= kUpb_LabelFlags_IsPacked;
  }

  if (field_modifiers & kUpb_EncodedFieldModifier_FlipValidateUtf8) {
    if (field->WPI_UPB_PRIVATE(descriptortype) != kUpb_FieldType_Bytes ||
        !(field->WPI_UPB_PRIVATE(mode) & kUpb_LabelFlags_IsAlternate)) {
      wpi_upb_MdDecoder_ErrorJmp(&d->base,
                             "Cannot flip ValidateUtf8 on field %" PRIu32
                             ", type=%d, mode=%d",
                             wpi_upb_MiniTableField_Number(field),
                             (int)field->WPI_UPB_PRIVATE(descriptortype),
                             (int)field->WPI_UPB_PRIVATE(mode));
    }
    field->WPI_UPB_PRIVATE(descriptortype) = kUpb_FieldType_String;
    field->WPI_UPB_PRIVATE(mode) &= ~kUpb_LabelFlags_IsAlternate;
  }

  bool singular = field_modifiers & kUpb_EncodedFieldModifier_IsProto3Singular;
  bool required = field_modifiers & kUpb_EncodedFieldModifier_IsRequired;

  // Validate.
  if ((singular || required) && field->WPI_UPB_PRIVATE(offset) != kHasbitPresence) {
    wpi_upb_MdDecoder_ErrorJmp(&d->base,
                           "Invalid modifier(s) for repeated field %" PRIu32,
                           wpi_upb_MiniTableField_Number(field));
  }
  if (singular && required) {
    wpi_upb_MdDecoder_ErrorJmp(
        &d->base, "Field %" PRIu32 " cannot be both singular and required",
        wpi_upb_MiniTableField_Number(field));
  }

  if (singular && wpi_upb_MiniTableField_IsSubMessage(field)) {
    wpi_upb_MdDecoder_ErrorJmp(&d->base,
                           "Field %" PRIu32 " cannot be a singular submessage",
                           wpi_upb_MiniTableField_Number(field));
  }

  if (singular) field->WPI_UPB_PRIVATE(offset) = kNoPresence;
  if (required) {
    field->WPI_UPB_PRIVATE(offset) = kRequiredPresence;
  }
}

static void wpi_upb_MtDecoder_PushItem(wpi_upb_MtDecoder* d, wpi_upb_LayoutItem item) {
  if (d->vec.size == d->vec.capacity) {
    size_t new_cap = WPI_UPB_MAX(8, d->vec.size * 2);
    d->vec.data = realloc(d->vec.data, new_cap * sizeof(*d->vec.data));
    wpi_upb_MdDecoder_CheckOutOfMemory(&d->base, d->vec.data);
    d->vec.capacity = new_cap;
  }
  d->vec.data[d->vec.size++] = item;
}

static void wpi_upb_MtDecoder_PushOneof(wpi_upb_MtDecoder* d, wpi_upb_LayoutItem item) {
  if (item.field_index == kUpb_LayoutItem_IndexSentinel) {
    wpi_upb_MdDecoder_ErrorJmp(&d->base, "Empty oneof");
  }
  item.field_index -= kOneofBase;

  // Push oneof data.
  item.type = kUpb_LayoutItemType_OneofField;
  wpi_upb_MtDecoder_PushItem(d, item);

  // Push oneof case.
  item.rep = kUpb_FieldRep_4Byte;  // Field Number.
  item.type = kUpb_LayoutItemType_OneofCase;
  wpi_upb_MtDecoder_PushItem(d, item);
}

static size_t wpi_upb_MtDecoder_SizeOfRep(wpi_upb_FieldRep rep,
                                      wpi_upb_MiniTablePlatform platform) {
  static const uint8_t kRepToSize32[] = {
      [kUpb_FieldRep_1Byte] = 1,
      [kUpb_FieldRep_4Byte] = 4,
      [kUpb_FieldRep_StringView] = 8,
      [kUpb_FieldRep_8Byte] = 8,
  };
  static const uint8_t kRepToSize64[] = {
      [kUpb_FieldRep_1Byte] = 1,
      [kUpb_FieldRep_4Byte] = 4,
      [kUpb_FieldRep_StringView] = 16,
      [kUpb_FieldRep_8Byte] = 8,
  };
  WPI_UPB_ASSERT(sizeof(wpi_upb_StringView) ==
             WPI_UPB_SIZE(kRepToSize32, kRepToSize64)[kUpb_FieldRep_StringView]);
  return platform == kUpb_MiniTablePlatform_32Bit ? kRepToSize32[rep]
                                                  : kRepToSize64[rep];
}

static size_t wpi_upb_MtDecoder_AlignOfRep(wpi_upb_FieldRep rep,
                                       wpi_upb_MiniTablePlatform platform) {
  static const uint8_t kRepToAlign32[] = {
      [kUpb_FieldRep_1Byte] = 1,
      [kUpb_FieldRep_4Byte] = 4,
      [kUpb_FieldRep_StringView] = 4,
      [kUpb_FieldRep_8Byte] = 8,
  };
  static const uint8_t kRepToAlign64[] = {
      [kUpb_FieldRep_1Byte] = 1,
      [kUpb_FieldRep_4Byte] = 4,
      [kUpb_FieldRep_StringView] = 8,
      [kUpb_FieldRep_8Byte] = 8,
  };
  WPI_UPB_ASSERT(WPI_UPB_ALIGN_OF(wpi_upb_StringView) ==
             WPI_UPB_SIZE(kRepToAlign32, kRepToAlign64)[kUpb_FieldRep_StringView]);
  return platform == kUpb_MiniTablePlatform_32Bit ? kRepToAlign32[rep]
                                                  : kRepToAlign64[rep];
}

static const char* wpi_upb_MtDecoder_DecodeOneofField(wpi_upb_MtDecoder* d,
                                                  const char* ptr,
                                                  char first_ch,
                                                  wpi_upb_LayoutItem* item) {
  uint32_t field_num;
  ptr = wpi_upb_MdDecoder_DecodeBase92Varint(
      &d->base, ptr, first_ch, kUpb_EncodedValue_MinOneofField,
      kUpb_EncodedValue_MaxOneofField, &field_num);
  wpi_upb_MiniTableField* f =
      (void*)wpi_upb_MiniTable_FindFieldByNumber(d->table, field_num);

  if (!f) {
    wpi_upb_MdDecoder_ErrorJmp(&d->base,
                           "Couldn't add field number %" PRIu32
                           " to oneof, no such field number.",
                           field_num);
  }
  if (f->WPI_UPB_PRIVATE(offset) != kHasbitPresence) {
    wpi_upb_MdDecoder_ErrorJmp(
        &d->base,
        "Cannot add repeated, required, or singular field %" PRIu32
        " to oneof.",
        field_num);
  }

  // Oneof storage must be large enough to accommodate the largest member.
  int rep = f->WPI_UPB_PRIVATE(mode) >> kUpb_FieldRep_Shift;
  if (wpi_upb_MtDecoder_SizeOfRep(rep, d->platform) >
      wpi_upb_MtDecoder_SizeOfRep(item->rep, d->platform)) {
    item->rep = rep;
  }
  // Prepend this field to the linked list.
  f->WPI_UPB_PRIVATE(offset) = item->field_index;
  item->field_index = (f - d->fields) + kOneofBase;
  return ptr;
}

static const char* wpi_upb_MtDecoder_DecodeOneofs(wpi_upb_MtDecoder* d,
                                              const char* ptr) {
  wpi_upb_LayoutItem item = {.rep = 0,
                         .field_index = kUpb_LayoutItem_IndexSentinel};
  while (ptr < d->base.end) {
    char ch = *ptr++;
    if (ch == kUpb_EncodedValue_FieldSeparator) {
      // Field separator, no action needed.
    } else if (ch == kUpb_EncodedValue_OneofSeparator) {
      // End of oneof.
      wpi_upb_MtDecoder_PushOneof(d, item);
      item.field_index = kUpb_LayoutItem_IndexSentinel;  // Move to next oneof.
    } else {
      ptr = wpi_upb_MtDecoder_DecodeOneofField(d, ptr, ch, &item);
    }
  }

  // Push final oneof.
  wpi_upb_MtDecoder_PushOneof(d, item);
  return ptr;
}

static const char* wpi_upb_MtDecoder_ParseModifier(wpi_upb_MtDecoder* d,
                                               const char* ptr, char first_ch,
                                               wpi_upb_MiniTableField* last_field,
                                               uint64_t* msg_modifiers) {
  uint32_t mod;
  ptr = wpi_upb_MdDecoder_DecodeBase92Varint(&d->base, ptr, first_ch,
                                         kUpb_EncodedValue_MinModifier,
                                         kUpb_EncodedValue_MaxModifier, &mod);
  if (last_field) {
    wpi_upb_MtDecoder_ModifyField(d, *msg_modifiers, mod, last_field);
  } else {
    if (!d->table) {
      wpi_upb_MdDecoder_ErrorJmp(&d->base,
                             "Extensions cannot have message modifiers");
    }
    *msg_modifiers = mod;
  }

  return ptr;
}

static void wpi_upb_MtDecoder_AllocateSubs(wpi_upb_MtDecoder* d,
                                       wpi_upb_SubCounts sub_counts) {
  uint32_t total_count = sub_counts.submsg_count + sub_counts.subenum_count;
  size_t subs_bytes = sizeof(*d->table->WPI_UPB_PRIVATE(subs)) * total_count;
  size_t ptrs_bytes = sizeof(wpi_upb_MiniTable*) * sub_counts.submsg_count;
  wpi_upb_MiniTableSubInternal* subs = wpi_upb_Arena_Malloc(d->arena, subs_bytes);
  const wpi_upb_MiniTable** subs_ptrs = wpi_upb_Arena_Malloc(d->arena, ptrs_bytes);
  wpi_upb_MdDecoder_CheckOutOfMemory(&d->base, subs);
  wpi_upb_MdDecoder_CheckOutOfMemory(&d->base, subs_ptrs);
  uint32_t i = 0;
  for (; i < sub_counts.submsg_count; i++) {
    subs_ptrs[i] = WPI_UPB_PRIVATE(_wpi_upb_MiniTable_Empty)();
    subs[i].WPI_UPB_PRIVATE(submsg) = &subs_ptrs[i];
  }
  if (sub_counts.subenum_count) {
    wpi_upb_MiniTableField* f = d->fields;
    wpi_upb_MiniTableField* end_f = f + d->table->WPI_UPB_PRIVATE(field_count);
    for (; f < end_f; f++) {
      if (f->WPI_UPB_PRIVATE(descriptortype) == kUpb_FieldType_Enum) {
        f->WPI_UPB_PRIVATE(submsg_index) += sub_counts.submsg_count;
      }
    }
    for (; i < sub_counts.submsg_count + sub_counts.subenum_count; i++) {
      subs[i].WPI_UPB_PRIVATE(subenum) = NULL;
    }
  }
  d->table->WPI_UPB_PRIVATE(subs) = subs;
}

static const char* wpi_upb_MtDecoder_Parse(wpi_upb_MtDecoder* d, const char* ptr,
                                       size_t len, void* fields,
                                       size_t field_size, uint16_t* field_count,
                                       wpi_upb_SubCounts* sub_counts) {
  uint64_t msg_modifiers = 0;
  uint32_t last_field_number = 0;
  wpi_upb_MiniTableField* last_field = NULL;
  bool need_dense_below = d->table != NULL;

  d->base.end = WPI_UPB_PTRADD(ptr, len);

  while (ptr < d->base.end) {
    char ch = *ptr++;
    if (ch <= kUpb_EncodedValue_MaxField) {
      if (!d->table && last_field) {
        // For extensions, consume only a single field and then return.
        return --ptr;
      }
      wpi_upb_MiniTableField* field = fields;
      *field_count += 1;
      fields = (char*)fields + field_size;
      field->WPI_UPB_PRIVATE(number) = ++last_field_number;
      last_field = field;
      wpi_upb_MiniTable_SetField(d, ch, field, msg_modifiers, sub_counts);
    } else if (kUpb_EncodedValue_MinModifier <= ch &&
               ch <= kUpb_EncodedValue_MaxModifier) {
      ptr = wpi_upb_MtDecoder_ParseModifier(d, ptr, ch, last_field, &msg_modifiers);
      if (msg_modifiers & kUpb_MessageModifier_IsExtendable) {
        d->table->WPI_UPB_PRIVATE(ext) |= kUpb_ExtMode_Extendable;
      }
    } else if (ch == kUpb_EncodedValue_End) {
      if (!d->table) {
        wpi_upb_MdDecoder_ErrorJmp(&d->base, "Extensions cannot have oneofs.");
      }
      ptr = wpi_upb_MtDecoder_DecodeOneofs(d, ptr);
    } else if (kUpb_EncodedValue_MinSkip <= ch &&
               ch <= kUpb_EncodedValue_MaxSkip) {
      if (need_dense_below) {
        d->table->WPI_UPB_PRIVATE(dense_below) = d->table->WPI_UPB_PRIVATE(field_count);
        need_dense_below = false;
      }
      uint32_t skip;
      ptr = wpi_upb_MdDecoder_DecodeBase92Varint(&d->base, ptr, ch,
                                             kUpb_EncodedValue_MinSkip,
                                             kUpb_EncodedValue_MaxSkip, &skip);
      last_field_number += skip;
      last_field_number--;  // Next field seen will increment.
    } else {
      wpi_upb_MdDecoder_ErrorJmp(&d->base, "Invalid char: %c", ch);
    }
  }

  if (need_dense_below) {
    d->table->WPI_UPB_PRIVATE(dense_below) = d->table->WPI_UPB_PRIVATE(field_count);
  }

  return ptr;
}

static void wpi_upb_MtDecoder_ParseMessage(wpi_upb_MtDecoder* d, const char* data,
                                       size_t len) {
  // Buffer length is an upper bound on the number of fields. We will return
  // what we don't use.
  d->fields = wpi_upb_Arena_Malloc(d->arena, sizeof(*d->fields) * len);
  wpi_upb_MdDecoder_CheckOutOfMemory(&d->base, d->fields);

  wpi_upb_SubCounts sub_counts = {0, 0};
  d->table->WPI_UPB_PRIVATE(field_count) = 0;
  d->table->WPI_UPB_PRIVATE(fields) = d->fields;
  wpi_upb_MtDecoder_Parse(d, data, len, d->fields, sizeof(*d->fields),
                      &d->table->WPI_UPB_PRIVATE(field_count), &sub_counts);

  wpi_upb_Arena_ShrinkLast(d->arena, d->fields, sizeof(*d->fields) * len,
                       sizeof(*d->fields) * d->table->WPI_UPB_PRIVATE(field_count));
  d->table->WPI_UPB_PRIVATE(fields) = d->fields;
  wpi_upb_MtDecoder_AllocateSubs(d, sub_counts);
}

static int wpi_upb_MtDecoder_CompareFields(const void* _a, const void* _b) {
  const wpi_upb_LayoutItem* a = _a;
  const wpi_upb_LayoutItem* b = _b;
  // Currently we just sort by:
  //  1. rep (smallest fields first)
  //  2. type (oneof cases first)
  //  2. field_index (smallest numbers first)
  // The main goal of this is to reduce space lost to padding.
  // Later we may have more subtle reasons to prefer a different ordering.
  const int rep_bits = wpi_upb_Log2Ceiling(kUpb_FieldRep_Max);
  const int type_bits = wpi_upb_Log2Ceiling(kUpb_LayoutItemType_Max);
  const int idx_bits = (sizeof(a->field_index) * 8);
  WPI_UPB_ASSERT(idx_bits + rep_bits + type_bits < 32);
#define WPI_UPB_COMBINE(rep, ty, idx) (((rep << type_bits) | ty) << idx_bits) | idx
  uint32_t a_packed = WPI_UPB_COMBINE(a->rep, a->type, a->field_index);
  uint32_t b_packed = WPI_UPB_COMBINE(b->rep, b->type, b->field_index);
  WPI_UPB_ASSERT(a_packed != b_packed);
#undef WPI_UPB_COMBINE
  return a_packed < b_packed ? -1 : 1;
}

static bool wpi_upb_MtDecoder_SortLayoutItems(wpi_upb_MtDecoder* d) {
  // Add items for all non-oneof fields (oneofs were already added).
  int n = d->table->WPI_UPB_PRIVATE(field_count);
  for (int i = 0; i < n; i++) {
    wpi_upb_MiniTableField* f = &d->fields[i];
    if (f->WPI_UPB_PRIVATE(offset) >= kOneofBase) continue;
    wpi_upb_LayoutItem item = {.field_index = i,
                           .rep = f->WPI_UPB_PRIVATE(mode) >> kUpb_FieldRep_Shift,
                           .type = kUpb_LayoutItemType_Field};
    wpi_upb_MtDecoder_PushItem(d, item);
  }

  if (d->vec.size) {
    qsort(d->vec.data, d->vec.size, sizeof(*d->vec.data),
          wpi_upb_MtDecoder_CompareFields);
  }

  return true;
}

static size_t wpi_upb_MiniTable_DivideRoundUp(size_t n, size_t d) {
  return (n + d - 1) / d;
}

static void wpi_upb_MtDecoder_AssignHasbits(wpi_upb_MtDecoder* d) {
  wpi_upb_MiniTable* ret = d->table;
  int n = ret->WPI_UPB_PRIVATE(field_count);
  size_t last_hasbit = kUpb_Reserved_Hasbits - 1;

  // First assign required fields, which must have the lowest hasbits.
  for (int i = 0; i < n; i++) {
    wpi_upb_MiniTableField* field =
        (wpi_upb_MiniTableField*)&ret->WPI_UPB_PRIVATE(fields)[i];
    if (field->WPI_UPB_PRIVATE(offset) == kRequiredPresence) {
      field->presence = ++last_hasbit;
    } else if (field->WPI_UPB_PRIVATE(offset) == kNoPresence) {
      field->presence = 0;
    }
  }
  if (last_hasbit > kUpb_Reserved_Hasbits + 63) {
    wpi_upb_MdDecoder_ErrorJmp(&d->base, "Too many required fields");
  }

  ret->WPI_UPB_PRIVATE(required_count) = last_hasbit - (kUpb_Reserved_Hasbits - 1);

  // Next assign non-required hasbit fields.
  for (int i = 0; i < n; i++) {
    wpi_upb_MiniTableField* field =
        (wpi_upb_MiniTableField*)&ret->WPI_UPB_PRIVATE(fields)[i];
    if (field->WPI_UPB_PRIVATE(offset) == kHasbitPresence) {
      field->presence = ++last_hasbit;
    }
  }

  ret->WPI_UPB_PRIVATE(size) =
      last_hasbit ? wpi_upb_MiniTable_DivideRoundUp(last_hasbit + 1, 8) : 0;
}

static size_t wpi_upb_MtDecoder_Place(wpi_upb_MtDecoder* d, wpi_upb_FieldRep rep) {
  size_t size = wpi_upb_MtDecoder_SizeOfRep(rep, d->platform);
  size_t align = wpi_upb_MtDecoder_AlignOfRep(rep, d->platform);
  size_t ret = WPI_UPB_ALIGN_UP(d->table->WPI_UPB_PRIVATE(size), align);
  static const size_t max = UINT16_MAX;
  size_t new_size = ret + size;
  if (new_size > max) {
    wpi_upb_MdDecoder_ErrorJmp(
        &d->base, "Message size exceeded maximum size of %zu bytes", max);
  }
  d->table->WPI_UPB_PRIVATE(size) = new_size;
  return ret;
}

static void wpi_upb_MtDecoder_AssignOffsets(wpi_upb_MtDecoder* d) {
  wpi_upb_LayoutItem* end = WPI_UPB_PTRADD(d->vec.data, d->vec.size);

  // Compute offsets.
  for (wpi_upb_LayoutItem* item = d->vec.data; item < end; item++) {
    item->offset = wpi_upb_MtDecoder_Place(d, item->rep);
  }

  // Assign oneof case offsets.  We must do these first, since assigning
  // actual offsets will overwrite the links of the linked list.
  for (wpi_upb_LayoutItem* item = d->vec.data; item < end; item++) {
    if (item->type != kUpb_LayoutItemType_OneofCase) continue;
    wpi_upb_MiniTableField* f = &d->fields[item->field_index];
    while (true) {
      f->presence = ~item->offset;
      if (f->WPI_UPB_PRIVATE(offset) == kUpb_LayoutItem_IndexSentinel) break;
      WPI_UPB_ASSERT(f->WPI_UPB_PRIVATE(offset) - kOneofBase <
                 d->table->WPI_UPB_PRIVATE(field_count));
      f = &d->fields[f->WPI_UPB_PRIVATE(offset) - kOneofBase];
    }
  }

  // Assign offsets.
  for (wpi_upb_LayoutItem* item = d->vec.data; item < end; item++) {
    wpi_upb_MiniTableField* f = &d->fields[item->field_index];
    switch (item->type) {
      case kUpb_LayoutItemType_OneofField:
        while (true) {
          uint16_t next_offset = f->WPI_UPB_PRIVATE(offset);
          f->WPI_UPB_PRIVATE(offset) = item->offset;
          if (next_offset == kUpb_LayoutItem_IndexSentinel) break;
          f = &d->fields[next_offset - kOneofBase];
        }
        break;
      case kUpb_LayoutItemType_Field:
        f->WPI_UPB_PRIVATE(offset) = item->offset;
        break;
      default:
        break;
    }
  }

  // The fasttable parser (supported on 64-bit only) depends on this being a
  // multiple of 8 in order to satisfy WPI_UPB_MALLOC_ALIGN, which is also 8.
  //
  // On 32-bit we could potentially make this smaller, but there is no
  // compelling reason to optimize this right now.
  d->table->WPI_UPB_PRIVATE(size) = WPI_UPB_ALIGN_UP(d->table->WPI_UPB_PRIVATE(size), 8);
}

static void wpi_upb_MtDecoder_ValidateEntryField(wpi_upb_MtDecoder* d,
                                             const wpi_upb_MiniTableField* f,
                                             uint32_t expected_num) {
  const char* name = expected_num == 1 ? "key" : "val";
  const uint32_t f_number = wpi_upb_MiniTableField_Number(f);
  if (f_number != expected_num) {
    wpi_upb_MdDecoder_ErrorJmp(&d->base,
                           "map %s did not have expected number (%d vs %d)",
                           name, expected_num, f_number);
  }

  if (!wpi_upb_MiniTableField_IsScalar(f)) {
    wpi_upb_MdDecoder_ErrorJmp(
        &d->base, "map %s cannot be repeated or map, or be in oneof", name);
  }

  uint32_t not_ok_types;
  if (expected_num == 1) {
    not_ok_types = (1 << kUpb_FieldType_Float) | (1 << kUpb_FieldType_Double) |
                   (1 << kUpb_FieldType_Message) | (1 << kUpb_FieldType_Group) |
                   (1 << kUpb_FieldType_Bytes) | (1 << kUpb_FieldType_Enum);
  } else {
    not_ok_types = 1 << kUpb_FieldType_Group;
  }

  if ((1 << wpi_upb_MiniTableField_Type(f)) & not_ok_types) {
    wpi_upb_MdDecoder_ErrorJmp(&d->base, "map %s cannot have type %d", name,
                           (int)f->WPI_UPB_PRIVATE(descriptortype));
  }
}

static void wpi_upb_MtDecoder_ParseMap(wpi_upb_MtDecoder* d, const char* data,
                                   size_t len) {
  wpi_upb_MtDecoder_ParseMessage(d, data, len);
  wpi_upb_MtDecoder_AssignHasbits(d);

  if (WPI_UPB_UNLIKELY(d->table->WPI_UPB_PRIVATE(field_count) != 2)) {
    wpi_upb_MdDecoder_ErrorJmp(&d->base, "%hu fields in map",
                           d->table->WPI_UPB_PRIVATE(field_count));
    WPI_UPB_UNREACHABLE();
  }

  wpi_upb_LayoutItem* end = WPI_UPB_PTRADD(d->vec.data, d->vec.size);
  for (wpi_upb_LayoutItem* item = d->vec.data; item < end; item++) {
    if (item->type == kUpb_LayoutItemType_OneofCase) {
      wpi_upb_MdDecoder_ErrorJmp(&d->base, "Map entry cannot have oneof");
    }
  }

  wpi_upb_MtDecoder_ValidateEntryField(d, &d->table->WPI_UPB_PRIVATE(fields)[0], 1);
  wpi_upb_MtDecoder_ValidateEntryField(d, &d->table->WPI_UPB_PRIVATE(fields)[1], 2);

  d->fields[0].WPI_UPB_PRIVATE(offset) = offsetof(wpi_upb_MapEntry, k);
  d->fields[1].WPI_UPB_PRIVATE(offset) = offsetof(wpi_upb_MapEntry, v);
  d->table->WPI_UPB_PRIVATE(size) = sizeof(wpi_upb_MapEntry);

  // Map entries have a special bit set to signal it's a map entry, used in
  // wpi_upb_MiniTable_SetSubMessage() below.
  d->table->WPI_UPB_PRIVATE(ext) |= kUpb_ExtMode_IsMapEntry;
}

static void wpi_upb_MtDecoder_ParseMessageSet(wpi_upb_MtDecoder* d, const char* data,
                                          size_t len) {
  if (len > 0) {
    wpi_upb_MdDecoder_ErrorJmp(&d->base, "Invalid message set encode length: %zu",
                           len);
  }

  wpi_upb_MiniTable* ret = d->table;
  ret->WPI_UPB_PRIVATE(size) = kUpb_Reserved_Hasbytes;
  ret->WPI_UPB_PRIVATE(field_count) = 0;
  ret->WPI_UPB_PRIVATE(ext) = kUpb_ExtMode_IsMessageSet;
  ret->WPI_UPB_PRIVATE(dense_below) = 0;
  ret->WPI_UPB_PRIVATE(table_mask) = -1;
  ret->WPI_UPB_PRIVATE(required_count) = 0;
}

static wpi_upb_MiniTable* wpi_upb_MtDecoder_DoBuildMiniTableWithBuf(
    wpi_upb_MtDecoder* decoder, const char* data, size_t len, void** buf,
    size_t* buf_size) {
  wpi_upb_MdDecoder_CheckOutOfMemory(&decoder->base, decoder->table);

  decoder->table->WPI_UPB_PRIVATE(size) = kUpb_Reserved_Hasbytes;
  decoder->table->WPI_UPB_PRIVATE(field_count) = 0;
  decoder->table->WPI_UPB_PRIVATE(ext) = kUpb_ExtMode_NonExtendable;
  decoder->table->WPI_UPB_PRIVATE(dense_below) = 0;
  decoder->table->WPI_UPB_PRIVATE(table_mask) = -1;
  decoder->table->WPI_UPB_PRIVATE(required_count) = 0;
#if WPI_UPB_TRACING_ENABLED
  // MiniTables built from MiniDescriptors will not be able to vend the message
  // name unless it is explicitly set with wpi_upb_MiniTable_SetFullName().
  decoder->table->WPI_UPB_PRIVATE(full_name) = 0;
#endif

  // Strip off and verify the version tag.
  if (!len--) goto done;
  const char vers = *data++;

  switch (vers) {
    case kUpb_EncodedVersion_MapV1:
      wpi_upb_MtDecoder_ParseMap(decoder, data, len);
      break;

    case kUpb_EncodedVersion_MessageV1:
      wpi_upb_MtDecoder_ParseMessage(decoder, data, len);
      wpi_upb_MtDecoder_AssignHasbits(decoder);
      wpi_upb_MtDecoder_SortLayoutItems(decoder);
      wpi_upb_MtDecoder_AssignOffsets(decoder);
      break;

    case kUpb_EncodedVersion_MessageSetV1:
      wpi_upb_MtDecoder_ParseMessageSet(decoder, data, len);
      break;

    default:
      wpi_upb_MdDecoder_ErrorJmp(&decoder->base, "Invalid message version: %c",
                             vers);
  }

done:
  *buf = decoder->vec.data;
  *buf_size = decoder->vec.capacity * sizeof(*decoder->vec.data);
  return decoder->table;
}

static wpi_upb_MiniTable* wpi_upb_MtDecoder_BuildMiniTableWithBuf(
    wpi_upb_MtDecoder* const decoder, const char* const data, const size_t len,
    void** const buf, size_t* const buf_size) {
  if (WPI_UPB_SETJMP(decoder->base.err) != 0) {
    *buf = decoder->vec.data;
    *buf_size = decoder->vec.capacity * sizeof(*decoder->vec.data);
    return NULL;
  }

  return wpi_upb_MtDecoder_DoBuildMiniTableWithBuf(decoder, data, len, buf,
                                               buf_size);
}

wpi_upb_MiniTable* wpi_upb_MiniTable_BuildWithBuf(const char* data, size_t len,
                                          wpi_upb_MiniTablePlatform platform,
                                          wpi_upb_Arena* arena, void** buf,
                                          size_t* buf_size,
                                          wpi_upb_Status* status) {
  wpi_upb_MtDecoder decoder = {
      .base = {.status = status},
      .platform = platform,
      .vec =
          {
              .data = *buf,
              .capacity = *buf_size / sizeof(*decoder.vec.data),
              .size = 0,
          },
      .arena = arena,
      .table = wpi_upb_Arena_Malloc(arena, sizeof(*decoder.table)),
  };

  return wpi_upb_MtDecoder_BuildMiniTableWithBuf(&decoder, data, len, buf,
                                             buf_size);
}

static const char* wpi_upb_MtDecoder_DoBuildMiniTableExtension(
    wpi_upb_MtDecoder* decoder, const char* data, size_t len,
    wpi_upb_MiniTableExtension* ext, const wpi_upb_MiniTable* extendee,
    wpi_upb_MiniTableSub sub) {
  // If the string is non-empty then it must begin with a version tag.
  if (len) {
    if (*data != kUpb_EncodedVersion_ExtensionV1) {
      wpi_upb_MdDecoder_ErrorJmp(&decoder->base, "Invalid ext version: %c", *data);
    }
    data++;
    len--;
  }

  uint16_t count = 0;
  wpi_upb_SubCounts sub_counts = {0, 0};
  const char* ret = wpi_upb_MtDecoder_Parse(decoder, data, len, ext, sizeof(*ext),
                                        &count, &sub_counts);
  if (!ret || count != 1) return NULL;

  wpi_upb_MiniTableField* f = &ext->WPI_UPB_PRIVATE(field);

  f->WPI_UPB_PRIVATE(mode) |= kUpb_LabelFlags_IsExtension;
  f->WPI_UPB_PRIVATE(offset) = 0;
  f->presence = 0;

  if (extendee->WPI_UPB_PRIVATE(ext) & kUpb_ExtMode_IsMessageSet) {
    // Extensions of MessageSet must be messages.
    if (!wpi_upb_MiniTableField_IsSubMessage(f)) return NULL;

    // Extensions of MessageSet must be non-repeating.
    if (wpi_upb_MiniTableField_IsArray(f)) return NULL;
  }

  ext->WPI_UPB_PRIVATE(extendee) = extendee;
  ext->WPI_UPB_PRIVATE(sub) = sub;

  return ret;
}

static const char* wpi_upb_MtDecoder_BuildMiniTableExtension(
    wpi_upb_MtDecoder* const decoder, const char* const data, const size_t len,
    wpi_upb_MiniTableExtension* const ext, const wpi_upb_MiniTable* const extendee,
    const wpi_upb_MiniTableSub sub) {
  if (WPI_UPB_SETJMP(decoder->base.err) != 0) return NULL;
  return wpi_upb_MtDecoder_DoBuildMiniTableExtension(decoder, data, len, ext,
                                                 extendee, sub);
}

const char* _wpi_upb_MiniTableExtension_Init(const char* data, size_t len,
                                         wpi_upb_MiniTableExtension* ext,
                                         const wpi_upb_MiniTable* extendee,
                                         wpi_upb_MiniTableSub sub,
                                         wpi_upb_MiniTablePlatform platform,
                                         wpi_upb_Status* status) {
  wpi_upb_MtDecoder decoder = {
      .base = {.status = status},
      .arena = NULL,
      .table = NULL,
      .platform = platform,
  };

  return wpi_upb_MtDecoder_BuildMiniTableExtension(&decoder, data, len, ext,
                                               extendee, sub);
}

wpi_upb_MiniTableExtension* _wpi_upb_MiniTableExtension_Build(
    const char* data, size_t len, const wpi_upb_MiniTable* extendee,
    wpi_upb_MiniTableSub sub, wpi_upb_MiniTablePlatform platform, wpi_upb_Arena* arena,
    wpi_upb_Status* status) {
  wpi_upb_MiniTableExtension* ext =
      wpi_upb_Arena_Malloc(arena, sizeof(wpi_upb_MiniTableExtension));
  if (WPI_UPB_UNLIKELY(!ext)) return NULL;

  const char* ptr = _wpi_upb_MiniTableExtension_Init(data, len, ext, extendee, sub,
                                                 platform, status);
  if (WPI_UPB_UNLIKELY(!ptr)) return NULL;

  return ext;
}

wpi_upb_MiniTable* _wpi_upb_MiniTable_Build(const char* data, size_t len,
                                    wpi_upb_MiniTablePlatform platform,
                                    wpi_upb_Arena* arena, wpi_upb_Status* status) {
  void* buf = NULL;
  size_t size = 0;
  wpi_upb_MiniTable* ret = wpi_upb_MiniTable_BuildWithBuf(data, len, platform, arena,
                                                  &buf, &size, status);
  free(buf);
  return ret;
}


#include <stddef.h>
#include <stdint.h>
#include <string.h>


// Must be last.

bool wpi_upb_MiniTable_SetSubMessage(wpi_upb_MiniTable* table,
                                 wpi_upb_MiniTableField* field,
                                 const wpi_upb_MiniTable* sub) {
  WPI_UPB_ASSERT((uintptr_t)table->WPI_UPB_PRIVATE(fields) <= (uintptr_t)field &&
             (uintptr_t)field < (uintptr_t)(table->WPI_UPB_PRIVATE(fields) +
                                            table->WPI_UPB_PRIVATE(field_count)));
  WPI_UPB_ASSERT(sub);

  const bool sub_is_map = sub->WPI_UPB_PRIVATE(ext) & kUpb_ExtMode_IsMapEntry;

  switch (field->WPI_UPB_PRIVATE(descriptortype)) {
    case kUpb_FieldType_Message:
      if (sub_is_map) {
        const bool table_is_map =
            table->WPI_UPB_PRIVATE(ext) & kUpb_ExtMode_IsMapEntry;
        if (WPI_UPB_UNLIKELY(table_is_map)) return false;

        field->WPI_UPB_PRIVATE(mode) =
            (field->WPI_UPB_PRIVATE(mode) & ~kUpb_FieldMode_Mask) |
            kUpb_FieldMode_Map;
      }
      break;

    case kUpb_FieldType_Group:
      if (WPI_UPB_UNLIKELY(sub_is_map)) return false;
      break;

    default:
      return false;
  }

  int idx = field->WPI_UPB_PRIVATE(submsg_index);
  wpi_upb_MiniTableSubInternal* table_subs = (void*)table->WPI_UPB_PRIVATE(subs);
  // TODO: Add this assert back once YouTube is updated to not call
  // this function repeatedly.
  // WPI_UPB_ASSERT(WPI_UPB_PRIVATE(_wpi_upb_MiniTable_IsEmpty)(table_sub->submsg));
  memcpy((void*)table_subs[idx].WPI_UPB_PRIVATE(submsg), &sub, sizeof(void*));
  return true;
}

bool wpi_upb_MiniTable_SetSubEnum(wpi_upb_MiniTable* table, wpi_upb_MiniTableField* field,
                              const wpi_upb_MiniTableEnum* sub) {
  WPI_UPB_ASSERT((uintptr_t)table->WPI_UPB_PRIVATE(fields) <= (uintptr_t)field &&
             (uintptr_t)field < (uintptr_t)(table->WPI_UPB_PRIVATE(fields) +
                                            table->WPI_UPB_PRIVATE(field_count)));
  WPI_UPB_ASSERT(sub);

  wpi_upb_MiniTableSub* table_sub =
      (void*)&table->WPI_UPB_PRIVATE(subs)[field->WPI_UPB_PRIVATE(submsg_index)];
  *table_sub = wpi_upb_MiniTableSub_FromEnum(sub);
  return true;
}

uint32_t wpi_upb_MiniTable_GetSubList(const wpi_upb_MiniTable* m,
                                  const wpi_upb_MiniTableField** subs) {
  uint32_t msg_count = 0;
  uint32_t enum_count = 0;

  for (int i = 0; i < wpi_upb_MiniTable_FieldCount(m); i++) {
    const wpi_upb_MiniTableField* f = wpi_upb_MiniTable_GetFieldByIndex(m, i);
    if (wpi_upb_MiniTableField_CType(f) == kUpb_CType_Message) {
      *subs = f;
      ++subs;
      msg_count++;
    }
  }

  for (int i = 0; i < wpi_upb_MiniTable_FieldCount(m); i++) {
    const wpi_upb_MiniTableField* f = wpi_upb_MiniTable_GetFieldByIndex(m, i);
    if (wpi_upb_MiniTableField_IsClosedEnum(f)) {
      *subs = f;
      ++subs;
      enum_count++;
    }
  }

  return (msg_count << 16) | enum_count;
}

// The list of sub_tables and sub_enums must exactly match the number and order
// of sub-message fields and sub-enum fields given by wpi_upb_MiniTable_GetSubList()
// above.
bool wpi_upb_MiniTable_Link(wpi_upb_MiniTable* m, const wpi_upb_MiniTable** sub_tables,
                        size_t sub_table_count,
                        const wpi_upb_MiniTableEnum** sub_enums,
                        size_t sub_enum_count) {
  uint32_t msg_count = 0;
  uint32_t enum_count = 0;

  for (int i = 0; i < wpi_upb_MiniTable_FieldCount(m); i++) {
    wpi_upb_MiniTableField* f =
        (wpi_upb_MiniTableField*)wpi_upb_MiniTable_GetFieldByIndex(m, i);
    if (wpi_upb_MiniTableField_CType(f) == kUpb_CType_Message) {
      const wpi_upb_MiniTable* sub = sub_tables[msg_count++];
      if (msg_count > sub_table_count) return false;
      if (sub && !wpi_upb_MiniTable_SetSubMessage(m, f, sub)) return false;
    }
  }

  for (int i = 0; i < wpi_upb_MiniTable_FieldCount(m); i++) {
    wpi_upb_MiniTableField* f =
        (wpi_upb_MiniTableField*)wpi_upb_MiniTable_GetFieldByIndex(m, i);
    if (wpi_upb_MiniTableField_IsClosedEnum(f)) {
      const wpi_upb_MiniTableEnum* sub = sub_enums[enum_count++];
      if (enum_count > sub_enum_count) return false;
      if (sub && !wpi_upb_MiniTable_SetSubEnum(m, f, sub)) return false;
    }
  }

  return (msg_count == sub_table_count) && (enum_count == sub_enum_count);
}


#include <stddef.h>
#include <stdint.h>
#include <string.h>


// Must be last.

#define EXTREG_KEY_SIZE (sizeof(wpi_upb_MiniTable*) + sizeof(uint32_t))

struct wpi_upb_ExtensionRegistry {
  wpi_upb_Arena* arena;
  wpi_upb_strtable exts;  // Key is wpi_upb_MiniTable* concatenated with fieldnum.
};

static void extreg_key(char* buf, const wpi_upb_MiniTable* l, uint32_t fieldnum) {
  memcpy(buf, &l, sizeof(l));
  memcpy(buf + sizeof(l), &fieldnum, sizeof(fieldnum));
}

wpi_upb_ExtensionRegistry* wpi_upb_ExtensionRegistry_New(wpi_upb_Arena* arena) {
  wpi_upb_ExtensionRegistry* r = wpi_upb_Arena_Malloc(arena, sizeof(*r));
  if (!r) return NULL;
  r->arena = arena;
  if (!wpi_upb_strtable_init(&r->exts, 8, arena)) return NULL;
  return r;
}

WPI_UPB_API bool wpi_upb_ExtensionRegistry_Add(wpi_upb_ExtensionRegistry* r,
                                       const wpi_upb_MiniTableExtension* e) {
  char buf[EXTREG_KEY_SIZE];
  extreg_key(buf, e->WPI_UPB_PRIVATE(extendee), wpi_upb_MiniTableExtension_Number(e));
  if (wpi_upb_strtable_lookup2(&r->exts, buf, EXTREG_KEY_SIZE, NULL)) return false;
  return wpi_upb_strtable_insert(&r->exts, buf, EXTREG_KEY_SIZE,
                             wpi_upb_value_constptr(e), r->arena);
}

bool wpi_upb_ExtensionRegistry_AddArray(wpi_upb_ExtensionRegistry* r,
                                    const wpi_upb_MiniTableExtension** e,
                                    size_t count) {
  const wpi_upb_MiniTableExtension** start = e;
  const wpi_upb_MiniTableExtension** end = WPI_UPB_PTRADD(e, count);
  for (; e < end; e++) {
    if (!wpi_upb_ExtensionRegistry_Add(r, *e)) goto failure;
  }
  return true;

failure:
  // Back out the entries previously added.
  for (end = e, e = start; e < end; e++) {
    const wpi_upb_MiniTableExtension* ext = *e;
    char buf[EXTREG_KEY_SIZE];
    extreg_key(buf, ext->WPI_UPB_PRIVATE(extendee),
               wpi_upb_MiniTableExtension_Number(ext));
    wpi_upb_strtable_remove2(&r->exts, buf, EXTREG_KEY_SIZE, NULL);
  }
  return false;
}

#ifdef WPI_UPB_LINKARR_DECLARE

WPI_UPB_LINKARR_DECLARE(wpi_upb_AllExts, wpi_upb_MiniTableExtension);

bool wpi_upb_ExtensionRegistry_AddAllLinkedExtensions(wpi_upb_ExtensionRegistry* r) {
  const wpi_upb_MiniTableExtension* start = WPI_UPB_LINKARR_START(wpi_upb_AllExts);
  const wpi_upb_MiniTableExtension* stop = WPI_UPB_LINKARR_STOP(wpi_upb_AllExts);
  for (const wpi_upb_MiniTableExtension* p = start; p < stop; p++) {
    // Windows can introduce zero padding, so we have to skip zeroes.
    if (wpi_upb_MiniTableExtension_Number(p) != 0) {
      if (!wpi_upb_ExtensionRegistry_Add(r, p)) return false;
    }
  }
  return true;
}

#endif  // WPI_UPB_LINKARR_DECLARE

const wpi_upb_MiniTableExtension* wpi_upb_ExtensionRegistry_Lookup(
    const wpi_upb_ExtensionRegistry* r, const wpi_upb_MiniTable* t, uint32_t num) {
  char buf[EXTREG_KEY_SIZE];
  wpi_upb_value v;
  extreg_key(buf, t, num);
  if (wpi_upb_strtable_lookup2(&r->exts, buf, EXTREG_KEY_SIZE, &v)) {
    return wpi_upb_value_getconstptr(v);
  } else {
    return NULL;
  }
}


#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>


// Must be last.

const wpi_upb_MiniTableField* wpi_upb_MiniTable_FindFieldByNumber(
    const wpi_upb_MiniTable* m, uint32_t number) {
  const size_t i = ((size_t)number) - 1;  // 0 wraps to SIZE_MAX

  // Ideal case: index into dense fields
  if (i < m->WPI_UPB_PRIVATE(dense_below)) {
    WPI_UPB_ASSERT(m->WPI_UPB_PRIVATE(fields)[i].WPI_UPB_PRIVATE(number) == number);
    return &m->WPI_UPB_PRIVATE(fields)[i];
  }

  // Slow case: binary search
  int lo = m->WPI_UPB_PRIVATE(dense_below);
  int hi = m->WPI_UPB_PRIVATE(field_count) - 1;
  while (lo <= hi) {
    int mid = (lo + hi) / 2;
    uint32_t num = m->WPI_UPB_PRIVATE(fields)[mid].WPI_UPB_PRIVATE(number);
    if (num < number) {
      lo = mid + 1;
      continue;
    }
    if (num > number) {
      hi = mid - 1;
      continue;
    }
    return &m->WPI_UPB_PRIVATE(fields)[mid];
  }
  return NULL;
}

const wpi_upb_MiniTableField* wpi_upb_MiniTable_GetOneof(const wpi_upb_MiniTable* m,
                                                 const wpi_upb_MiniTableField* f) {
  if (WPI_UPB_UNLIKELY(!wpi_upb_MiniTableField_IsInOneof(f))) {
    return NULL;
  }
  const wpi_upb_MiniTableField* ptr = &m->WPI_UPB_PRIVATE(fields)[0];
  const wpi_upb_MiniTableField* end =
      &m->WPI_UPB_PRIVATE(fields)[m->WPI_UPB_PRIVATE(field_count)];
  for (; ptr < end; ptr++) {
    if (ptr->presence == (*f).presence) {
      return ptr;
    }
  }
  return NULL;
}

bool wpi_upb_MiniTable_NextOneofField(const wpi_upb_MiniTable* m,
                                  const wpi_upb_MiniTableField** f) {
  const wpi_upb_MiniTableField* ptr = *f;
  const wpi_upb_MiniTableField* end =
      &m->WPI_UPB_PRIVATE(fields)[m->WPI_UPB_PRIVATE(field_count)];
  while (++ptr < end) {
    if (ptr->presence == (*f)->presence) {
      *f = ptr;
      return true;
    }
  }
  return false;
}


#include <stddef.h>
#include <stdint.h>


// Must be last.

// Checks if source and target mini table fields are identical.
//
// If the field is a sub message and sub messages are identical we record
// the association in table.
//
// Hashing the source sub message mini table and it's equivalent in the table
// stops recursing when a cycle is detected and instead just checks if the
// destination table is equal.
static wpi_upb_MiniTableEquals_Status wpi_upb_deep_check(const wpi_upb_MiniTable* src,
                                                 const wpi_upb_MiniTable* dst,
                                                 wpi_upb_inttable* table,
                                                 wpi_upb_Arena** arena) {
  if (src->WPI_UPB_PRIVATE(field_count) != dst->WPI_UPB_PRIVATE(field_count))
    return kUpb_MiniTableEquals_NotEqual;
  bool marked_src = false;
  for (int i = 0; i < wpi_upb_MiniTable_FieldCount(src); i++) {
    const wpi_upb_MiniTableField* src_field = wpi_upb_MiniTable_GetFieldByIndex(src, i);
    const wpi_upb_MiniTableField* dst_field = wpi_upb_MiniTable_FindFieldByNumber(
        dst, wpi_upb_MiniTableField_Number(src_field));

    if (wpi_upb_MiniTableField_CType(src_field) !=
        wpi_upb_MiniTableField_CType(dst_field))
      return false;
    if (src_field->WPI_UPB_PRIVATE(mode) != dst_field->WPI_UPB_PRIVATE(mode))
      return false;
    if (src_field->WPI_UPB_PRIVATE(offset) != dst_field->WPI_UPB_PRIVATE(offset))
      return false;
    if (src_field->presence != dst_field->presence) return false;
    if (src_field->WPI_UPB_PRIVATE(submsg_index) !=
        dst_field->WPI_UPB_PRIVATE(submsg_index))
      return kUpb_MiniTableEquals_NotEqual;

    // Go no further if we are only checking for compatibility.
    if (!table) continue;

    if (wpi_upb_MiniTableField_CType(src_field) == kUpb_CType_Message) {
      if (!*arena) {
        *arena = wpi_upb_Arena_New();
        if (!wpi_upb_inttable_init(table, *arena)) {
          return kUpb_MiniTableEquals_OutOfMemory;
        }
      }
      if (!marked_src) {
        marked_src = true;
        wpi_upb_value val;
        val.val = (uint64_t)dst;
        if (!wpi_upb_inttable_insert(table, (uintptr_t)src, val, *arena)) {
          return kUpb_MiniTableEquals_OutOfMemory;
        }
      }
      const wpi_upb_MiniTable* sub_src =
          wpi_upb_MiniTable_GetSubMessageTable(src, src_field);
      const wpi_upb_MiniTable* sub_dst =
          wpi_upb_MiniTable_GetSubMessageTable(dst, dst_field);
      if (sub_src != NULL) {
        wpi_upb_value cmp;
        if (wpi_upb_inttable_lookup(table, (uintptr_t)sub_src, &cmp)) {
          // We already compared this src before. Check if same dst.
          if (cmp.val != (uint64_t)sub_dst) {
            return kUpb_MiniTableEquals_NotEqual;
          }
        } else {
          // Recurse if not already visited.
          wpi_upb_MiniTableEquals_Status s =
              wpi_upb_deep_check(sub_src, sub_dst, table, arena);
          if (s != kUpb_MiniTableEquals_Equal) {
            return s;
          }
        }
      }
    }
  }
  return kUpb_MiniTableEquals_Equal;
}

bool wpi_upb_MiniTable_Compatible(const wpi_upb_MiniTable* src,
                              const wpi_upb_MiniTable* dst) {
  return wpi_upb_deep_check(src, dst, NULL, NULL);
}

wpi_upb_MiniTableEquals_Status wpi_upb_MiniTable_Equals(const wpi_upb_MiniTable* src,
                                                const wpi_upb_MiniTable* dst) {
  // Arena allocated on demand for hash table.
  wpi_upb_Arena* arena = NULL;
  // Table to keep track of visited mini tables to guard against cycles.
  wpi_upb_inttable table;
  wpi_upb_MiniTableEquals_Status status = wpi_upb_deep_check(src, dst, &table, &arena);
  if (arena) {
    wpi_upb_Arena_Free(arena);
  }
  return status;
}


#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>


// Must be last.

// A few fake field types for our tables.
enum {
  kUpb_FakeFieldType_FieldNotFound = 0,
  kUpb_FakeFieldType_MessageSetItem = 19,
};

// DecodeOp: an action to be performed for a wire-type/field-type combination.
enum {
  // Special ops: we don't write data to regular fields for these.
  kUpb_DecodeOp_UnknownField = -1,
  kUpb_DecodeOp_MessageSetItem = -2,

  // Scalar-only ops.
  kUpb_DecodeOp_Scalar1Byte = 0,
  kUpb_DecodeOp_Scalar4Byte = 2,
  kUpb_DecodeOp_Scalar8Byte = 3,
  kUpb_DecodeOp_Enum = 1,

  // Scalar/repeated ops.
  kUpb_DecodeOp_String = 4,
  kUpb_DecodeOp_Bytes = 5,
  kUpb_DecodeOp_SubMessage = 6,

  // Repeated-only ops (also see macros below).
  kUpb_DecodeOp_PackedEnum = 13,
};

// For packed fields it is helpful to be able to recover the lg2 of the data
// size from the op.
#define OP_FIXPCK_LG2(n) (n + 5) /* n in [2, 3] => op in [7, 8] */
#define OP_VARPCK_LG2(n) (n + 9) /* n in [0, 2, 3] => op in [9, 11, 12] */

typedef union {
  bool bool_val;
  uint32_t uint32_val;
  uint64_t uint64_val;
  uint32_t size;
} wireval;

// Ideally these two functions should take the owning MiniTable pointer as a
// first argument, then we could just put them in mini_table/message.h as nice
// clean getters. But we don't have that so instead we gotta write these
// Frankenfunctions that take an array of subtables.
// TODO: Move these to mini_table/ anyway since there are other places
// that could use them.

// Returns the MiniTable corresponding to a given MiniTableField
// from an array of MiniTableSubs.
static const wpi_upb_MiniTable* _wpi_upb_MiniTableSubs_MessageByField(
    const wpi_upb_MiniTableSubInternal* subs, const wpi_upb_MiniTableField* field) {
  return *subs[field->WPI_UPB_PRIVATE(submsg_index)].WPI_UPB_PRIVATE(submsg);
}

// Returns the MiniTableEnum corresponding to a given MiniTableField
// from an array of MiniTableSub.
static const wpi_upb_MiniTableEnum* _wpi_upb_MiniTableSubs_EnumByField(
    const wpi_upb_MiniTableSubInternal* subs, const wpi_upb_MiniTableField* field) {
  return subs[field->WPI_UPB_PRIVATE(submsg_index)].WPI_UPB_PRIVATE(subenum);
}

static const char* _wpi_upb_Decoder_DecodeMessage(wpi_upb_Decoder* d, const char* ptr,
                                              wpi_upb_Message* msg,
                                              const wpi_upb_MiniTable* layout);

WPI_UPB_NORETURN static void* _wpi_upb_Decoder_ErrorJmp(wpi_upb_Decoder* d,
                                                wpi_upb_DecodeStatus status) {
  WPI_UPB_ASSERT(status != kUpb_DecodeStatus_Ok);
  d->status = status;
  WPI_UPB_LONGJMP(d->err, 1);
}

const char* _wpi_upb_FastDecoder_ErrorJmp(wpi_upb_Decoder* d, int status) {
  WPI_UPB_ASSERT(status != kUpb_DecodeStatus_Ok);
  d->status = status;
  WPI_UPB_LONGJMP(d->err, 1);
  return NULL;
}

static void _wpi_upb_Decoder_VerifyUtf8(wpi_upb_Decoder* d, const char* buf, int len) {
  if (!_wpi_upb_Decoder_VerifyUtf8Inline(buf, len)) {
    _wpi_upb_Decoder_ErrorJmp(d, kUpb_DecodeStatus_BadUtf8);
  }
}

static bool _wpi_upb_Decoder_Reserve(wpi_upb_Decoder* d, wpi_upb_Array* arr, size_t elem) {
  bool need_realloc =
      arr->WPI_UPB_PRIVATE(capacity) - arr->WPI_UPB_PRIVATE(size) < elem;
  if (need_realloc && !WPI_UPB_PRIVATE(_wpi_upb_Array_Realloc)(
                          arr, arr->WPI_UPB_PRIVATE(size) + elem, &d->arena)) {
    _wpi_upb_Decoder_ErrorJmp(d, kUpb_DecodeStatus_OutOfMemory);
  }
  return need_realloc;
}

typedef struct {
  const char* ptr;
  uint64_t val;
} _wpi_upb_DecodeLongVarintReturn;

WPI_UPB_NOINLINE
static _wpi_upb_DecodeLongVarintReturn _wpi_upb_Decoder_DecodeLongVarint(
    const char* ptr, uint64_t val) {
  _wpi_upb_DecodeLongVarintReturn ret = {NULL, 0};
  uint64_t byte;
  for (int i = 1; i < 10; i++) {
    byte = (uint8_t)ptr[i];
    val += (byte - 1) << (i * 7);
    if (!(byte & 0x80)) {
      ret.ptr = ptr + i + 1;
      ret.val = val;
      return ret;
    }
  }
  return ret;
}

WPI_UPB_FORCEINLINE
const char* _wpi_upb_Decoder_DecodeVarint(wpi_upb_Decoder* d, const char* ptr,
                                      uint64_t* val) {
  uint64_t byte = (uint8_t)*ptr;
  if (WPI_UPB_LIKELY((byte & 0x80) == 0)) {
    *val = byte;
    return ptr + 1;
  } else {
    _wpi_upb_DecodeLongVarintReturn res = _wpi_upb_Decoder_DecodeLongVarint(ptr, byte);
    if (!res.ptr) _wpi_upb_Decoder_ErrorJmp(d, kUpb_DecodeStatus_Malformed);
    *val = res.val;
    return res.ptr;
  }
}

WPI_UPB_FORCEINLINE
const char* _wpi_upb_Decoder_DecodeTag(wpi_upb_Decoder* d, const char* ptr,
                                   uint32_t* val) {
  uint64_t byte = (uint8_t)*ptr;
  if (WPI_UPB_LIKELY((byte & 0x80) == 0)) {
    *val = byte;
    return ptr + 1;
  } else {
    const char* start = ptr;
    _wpi_upb_DecodeLongVarintReturn res = _wpi_upb_Decoder_DecodeLongVarint(ptr, byte);
    if (!res.ptr || res.ptr - start > 5 || res.val > UINT32_MAX) {
      _wpi_upb_Decoder_ErrorJmp(d, kUpb_DecodeStatus_Malformed);
    }
    *val = res.val;
    return res.ptr;
  }
}

WPI_UPB_FORCEINLINE
const char* wpi_upb_Decoder_DecodeSize(wpi_upb_Decoder* d, const char* ptr,
                                   uint32_t* size) {
  uint64_t size64;
  ptr = _wpi_upb_Decoder_DecodeVarint(d, ptr, &size64);
  if (size64 >= INT32_MAX ||
      !wpi_upb_EpsCopyInputStream_CheckSize(&d->input, ptr, (int)size64)) {
    _wpi_upb_Decoder_ErrorJmp(d, kUpb_DecodeStatus_Malformed);
  }
  *size = size64;
  return ptr;
}

static void _wpi_upb_Decoder_MungeInt32(wireval* val) {
  if (!wpi_upb_IsLittleEndian()) {
    /* The next stage will memcpy(dst, &val, 4) */
    val->uint32_val = val->uint64_val;
  }
}

static void _wpi_upb_Decoder_Munge(int type, wireval* val) {
  switch (type) {
    case kUpb_FieldType_Bool:
      val->bool_val = val->uint64_val != 0;
      break;
    case kUpb_FieldType_SInt32: {
      uint32_t n = val->uint64_val;
      val->uint32_val = (n >> 1) ^ -(int32_t)(n & 1);
      break;
    }
    case kUpb_FieldType_SInt64: {
      uint64_t n = val->uint64_val;
      val->uint64_val = (n >> 1) ^ -(int64_t)(n & 1);
      break;
    }
    case kUpb_FieldType_Int32:
    case kUpb_FieldType_UInt32:
    case kUpb_FieldType_Enum:
      _wpi_upb_Decoder_MungeInt32(val);
      break;
  }
}

static wpi_upb_Message* _wpi_upb_Decoder_NewSubMessage2(wpi_upb_Decoder* d,
                                                const wpi_upb_MiniTable* subl,
                                                const wpi_upb_MiniTableField* field,
                                                wpi_upb_TaggedMessagePtr* target) {
  WPI_UPB_ASSERT(subl);
  wpi_upb_Message* msg = _wpi_upb_Message_New(subl, &d->arena);
  if (!msg) _wpi_upb_Decoder_ErrorJmp(d, kUpb_DecodeStatus_OutOfMemory);

  // Extensions should not be unlinked. A message extension should not be
  // registered until its sub-message type is available to be linked.
  bool is_empty = WPI_UPB_PRIVATE(_wpi_upb_MiniTable_IsEmpty)(subl);
  bool is_extension = field->WPI_UPB_PRIVATE(mode) & kUpb_LabelFlags_IsExtension;
  WPI_UPB_ASSERT(!(is_empty && is_extension));

  if (is_empty && !(d->options & kUpb_DecodeOption_ExperimentalAllowUnlinked)) {
    _wpi_upb_Decoder_ErrorJmp(d, kUpb_DecodeStatus_UnlinkedSubMessage);
  }

  wpi_upb_TaggedMessagePtr tagged =
      WPI_UPB_PRIVATE(_wpi_upb_TaggedMessagePtr_Pack)(msg, is_empty);
  memcpy(target, &tagged, sizeof(tagged));
  return msg;
}

static wpi_upb_Message* _wpi_upb_Decoder_NewSubMessage(
    wpi_upb_Decoder* d, const wpi_upb_MiniTableSubInternal* subs,
    const wpi_upb_MiniTableField* field, wpi_upb_TaggedMessagePtr* target) {
  const wpi_upb_MiniTable* subl = _wpi_upb_MiniTableSubs_MessageByField(subs, field);
  return _wpi_upb_Decoder_NewSubMessage2(d, subl, field, target);
}

static wpi_upb_Message* _wpi_upb_Decoder_ReuseSubMessage(
    wpi_upb_Decoder* d, const wpi_upb_MiniTableSubInternal* subs,
    const wpi_upb_MiniTableField* field, wpi_upb_TaggedMessagePtr* target) {
  wpi_upb_TaggedMessagePtr tagged = *target;
  const wpi_upb_MiniTable* subl = _wpi_upb_MiniTableSubs_MessageByField(subs, field);
  WPI_UPB_ASSERT(subl);
  if (!wpi_upb_TaggedMessagePtr_IsEmpty(tagged) ||
      WPI_UPB_PRIVATE(_wpi_upb_MiniTable_IsEmpty)(subl)) {
    return WPI_UPB_PRIVATE(_wpi_upb_TaggedMessagePtr_GetMessage)(tagged);
  }

  // We found an empty message from a previous parse that was performed before
  // this field was linked.  But it is linked now, so we want to allocate a new
  // message of the correct type and promote data into it before continuing.
  wpi_upb_Message* existing =
      WPI_UPB_PRIVATE(_wpi_upb_TaggedMessagePtr_GetEmptyMessage)(tagged);
  wpi_upb_Message* promoted = _wpi_upb_Decoder_NewSubMessage(d, subs, field, target);
  size_t size;
  const char* unknown = wpi_upb_Message_GetUnknown(existing, &size);
  wpi_upb_DecodeStatus status = wpi_upb_Decode(unknown, size, promoted, subl, d->extreg,
                                       d->options, &d->arena);
  if (status != kUpb_DecodeStatus_Ok) _wpi_upb_Decoder_ErrorJmp(d, status);
  return promoted;
}

static const char* _wpi_upb_Decoder_ReadString(wpi_upb_Decoder* d, const char* ptr,
                                           int size, wpi_upb_StringView* str) {
  const char* str_ptr = ptr;
  ptr = wpi_upb_EpsCopyInputStream_ReadString(&d->input, &str_ptr, size, &d->arena);
  if (!ptr) _wpi_upb_Decoder_ErrorJmp(d, kUpb_DecodeStatus_OutOfMemory);
  str->data = str_ptr;
  str->size = size;
  return ptr;
}

WPI_UPB_FORCEINLINE
const char* _wpi_upb_Decoder_RecurseSubMessage(wpi_upb_Decoder* d, const char* ptr,
                                           wpi_upb_Message* submsg,
                                           const wpi_upb_MiniTable* subl,
                                           uint32_t expected_end_group) {
  if (--d->depth < 0) {
    _wpi_upb_Decoder_ErrorJmp(d, kUpb_DecodeStatus_MaxDepthExceeded);
  }
  ptr = _wpi_upb_Decoder_DecodeMessage(d, ptr, submsg, subl);
  d->depth++;
  if (d->end_group != expected_end_group) {
    _wpi_upb_Decoder_ErrorJmp(d, kUpb_DecodeStatus_Malformed);
  }
  return ptr;
}

WPI_UPB_FORCEINLINE
const char* _wpi_upb_Decoder_DecodeSubMessage(wpi_upb_Decoder* d, const char* ptr,
                                          wpi_upb_Message* submsg,
                                          const wpi_upb_MiniTableSubInternal* subs,
                                          const wpi_upb_MiniTableField* field,
                                          int size) {
  int saved_delta = wpi_upb_EpsCopyInputStream_PushLimit(&d->input, ptr, size);
  const wpi_upb_MiniTable* subl = _wpi_upb_MiniTableSubs_MessageByField(subs, field);
  WPI_UPB_ASSERT(subl);
  ptr = _wpi_upb_Decoder_RecurseSubMessage(d, ptr, submsg, subl, DECODE_NOGROUP);
  wpi_upb_EpsCopyInputStream_PopLimit(&d->input, ptr, saved_delta);
  return ptr;
}

WPI_UPB_FORCEINLINE
const char* _wpi_upb_Decoder_DecodeGroup(wpi_upb_Decoder* d, const char* ptr,
                                     wpi_upb_Message* submsg,
                                     const wpi_upb_MiniTable* subl,
                                     uint32_t number) {
  if (_wpi_upb_Decoder_IsDone(d, &ptr)) {
    _wpi_upb_Decoder_ErrorJmp(d, kUpb_DecodeStatus_Malformed);
  }
  ptr = _wpi_upb_Decoder_RecurseSubMessage(d, ptr, submsg, subl, number);
  d->end_group = DECODE_NOGROUP;
  return ptr;
}

WPI_UPB_FORCEINLINE
const char* _wpi_upb_Decoder_DecodeUnknownGroup(wpi_upb_Decoder* d, const char* ptr,
                                            uint32_t number) {
  return _wpi_upb_Decoder_DecodeGroup(d, ptr, NULL, NULL, number);
}

WPI_UPB_FORCEINLINE
const char* _wpi_upb_Decoder_DecodeKnownGroup(wpi_upb_Decoder* d, const char* ptr,
                                          wpi_upb_Message* submsg,
                                          const wpi_upb_MiniTableSubInternal* subs,
                                          const wpi_upb_MiniTableField* field) {
  const wpi_upb_MiniTable* subl = _wpi_upb_MiniTableSubs_MessageByField(subs, field);
  WPI_UPB_ASSERT(subl);
  return _wpi_upb_Decoder_DecodeGroup(d, ptr, submsg, subl,
                                  field->WPI_UPB_PRIVATE(number));
}

static char* wpi_upb_Decoder_EncodeVarint32(uint32_t val, char* ptr) {
  do {
    uint8_t byte = val & 0x7fU;
    val >>= 7;
    if (val) byte |= 0x80U;
    *(ptr++) = byte;
  } while (val);
  return ptr;
}

static void _wpi_upb_Decoder_AddUnknownVarints(wpi_upb_Decoder* d, wpi_upb_Message* msg,
                                           uint32_t val1, uint32_t val2) {
  char buf[20];
  char* end = buf;
  end = wpi_upb_Decoder_EncodeVarint32(val1, end);
  end = wpi_upb_Decoder_EncodeVarint32(val2, end);

  if (!WPI_UPB_PRIVATE(_wpi_upb_Message_AddUnknown)(msg, buf, end - buf, &d->arena)) {
    _wpi_upb_Decoder_ErrorJmp(d, kUpb_DecodeStatus_OutOfMemory);
  }
}

WPI_UPB_FORCEINLINE
bool _wpi_upb_Decoder_CheckEnum(wpi_upb_Decoder* d, const char* ptr, wpi_upb_Message* msg,
                            const wpi_upb_MiniTableEnum* e,
                            const wpi_upb_MiniTableField* field, wireval* val) {
  const uint32_t v = val->uint32_val;

  if (WPI_UPB_LIKELY(wpi_upb_MiniTableEnum_CheckValue(e, v))) return true;

  // Unrecognized enum goes into unknown fields.
  // For packed fields the tag could be arbitrarily far in the past,
  // so we just re-encode the tag and value here.
  const uint32_t tag =
      ((uint32_t)field->WPI_UPB_PRIVATE(number) << 3) | kUpb_WireType_Varint;
  wpi_upb_Message* unknown_msg =
      field->WPI_UPB_PRIVATE(mode) & kUpb_LabelFlags_IsExtension ? d->unknown_msg
                                                             : msg;
  _wpi_upb_Decoder_AddUnknownVarints(d, unknown_msg, tag, v);
  return false;
}

WPI_UPB_NOINLINE
static const char* _wpi_upb_Decoder_DecodeEnumArray(
    wpi_upb_Decoder* d, const char* ptr, wpi_upb_Message* msg, wpi_upb_Array* arr,
    const wpi_upb_MiniTableSubInternal* subs, const wpi_upb_MiniTableField* field,
    wireval* val) {
  const wpi_upb_MiniTableEnum* e = _wpi_upb_MiniTableSubs_EnumByField(subs, field);
  if (!_wpi_upb_Decoder_CheckEnum(d, ptr, msg, e, field, val)) return ptr;
  void* mem = WPI_UPB_PTR_AT(wpi_upb_Array_MutableDataPtr(arr),
                         arr->WPI_UPB_PRIVATE(size) * 4, void);
  arr->WPI_UPB_PRIVATE(size)++;
  memcpy(mem, val, 4);
  return ptr;
}

WPI_UPB_FORCEINLINE
const char* _wpi_upb_Decoder_DecodeFixedPacked(wpi_upb_Decoder* d, const char* ptr,
                                           wpi_upb_Array* arr, wireval* val,
                                           const wpi_upb_MiniTableField* field,
                                           int lg2) {
  int mask = (1 << lg2) - 1;
  size_t count = val->size >> lg2;
  if ((val->size & mask) != 0) {
    // Length isn't a round multiple of elem size.
    _wpi_upb_Decoder_ErrorJmp(d, kUpb_DecodeStatus_Malformed);
  }
  _wpi_upb_Decoder_Reserve(d, arr, count);
  void* mem = WPI_UPB_PTR_AT(wpi_upb_Array_MutableDataPtr(arr),
                         arr->WPI_UPB_PRIVATE(size) << lg2, void);
  arr->WPI_UPB_PRIVATE(size) += count;
  // Note: if/when the decoder supports multi-buffer input, we will need to
  // handle buffer seams here.
  if (wpi_upb_IsLittleEndian()) {
    ptr = wpi_upb_EpsCopyInputStream_Copy(&d->input, ptr, mem, val->size);
  } else {
    int delta = wpi_upb_EpsCopyInputStream_PushLimit(&d->input, ptr, val->size);
    char* dst = mem;
    while (!_wpi_upb_Decoder_IsDone(d, &ptr)) {
      if (lg2 == 2) {
        ptr = wpi_upb_WireReader_ReadFixed32(ptr, dst);
        dst += 4;
      } else {
        WPI_UPB_ASSERT(lg2 == 3);
        ptr = wpi_upb_WireReader_ReadFixed64(ptr, dst);
        dst += 8;
      }
    }
    wpi_upb_EpsCopyInputStream_PopLimit(&d->input, ptr, delta);
  }

  return ptr;
}

WPI_UPB_FORCEINLINE
const char* _wpi_upb_Decoder_DecodeVarintPacked(wpi_upb_Decoder* d, const char* ptr,
                                            wpi_upb_Array* arr, wireval* val,
                                            const wpi_upb_MiniTableField* field,
                                            int lg2) {
  int scale = 1 << lg2;
  int saved_limit = wpi_upb_EpsCopyInputStream_PushLimit(&d->input, ptr, val->size);
  char* out = WPI_UPB_PTR_AT(wpi_upb_Array_MutableDataPtr(arr),
                         arr->WPI_UPB_PRIVATE(size) << lg2, void);
  while (!_wpi_upb_Decoder_IsDone(d, &ptr)) {
    wireval elem;
    ptr = _wpi_upb_Decoder_DecodeVarint(d, ptr, &elem.uint64_val);
    _wpi_upb_Decoder_Munge(field->WPI_UPB_PRIVATE(descriptortype), &elem);
    if (_wpi_upb_Decoder_Reserve(d, arr, 1)) {
      out = WPI_UPB_PTR_AT(wpi_upb_Array_MutableDataPtr(arr),
                       arr->WPI_UPB_PRIVATE(size) << lg2, void);
    }
    arr->WPI_UPB_PRIVATE(size)++;
    memcpy(out, &elem, scale);
    out += scale;
  }
  wpi_upb_EpsCopyInputStream_PopLimit(&d->input, ptr, saved_limit);
  return ptr;
}

WPI_UPB_NOINLINE
static const char* _wpi_upb_Decoder_DecodeEnumPacked(
    wpi_upb_Decoder* d, const char* ptr, wpi_upb_Message* msg, wpi_upb_Array* arr,
    const wpi_upb_MiniTableSubInternal* subs, const wpi_upb_MiniTableField* field,
    wireval* val) {
  const wpi_upb_MiniTableEnum* e = _wpi_upb_MiniTableSubs_EnumByField(subs, field);
  int saved_limit = wpi_upb_EpsCopyInputStream_PushLimit(&d->input, ptr, val->size);
  char* out = WPI_UPB_PTR_AT(wpi_upb_Array_MutableDataPtr(arr),
                         arr->WPI_UPB_PRIVATE(size) * 4, void);
  while (!_wpi_upb_Decoder_IsDone(d, &ptr)) {
    wireval elem;
    ptr = _wpi_upb_Decoder_DecodeVarint(d, ptr, &elem.uint64_val);
    _wpi_upb_Decoder_MungeInt32(&elem);
    if (!_wpi_upb_Decoder_CheckEnum(d, ptr, msg, e, field, &elem)) {
      continue;
    }
    if (_wpi_upb_Decoder_Reserve(d, arr, 1)) {
      out = WPI_UPB_PTR_AT(wpi_upb_Array_MutableDataPtr(arr),
                       arr->WPI_UPB_PRIVATE(size) * 4, void);
    }
    arr->WPI_UPB_PRIVATE(size)++;
    memcpy(out, &elem, 4);
    out += 4;
  }
  wpi_upb_EpsCopyInputStream_PopLimit(&d->input, ptr, saved_limit);
  return ptr;
}

static wpi_upb_Array* _wpi_upb_Decoder_CreateArray(wpi_upb_Decoder* d,
                                           const wpi_upb_MiniTableField* field) {
  const wpi_upb_FieldType field_type = field->WPI_UPB_PRIVATE(descriptortype);
  const size_t lg2 = WPI_UPB_PRIVATE(_wpi_upb_FieldType_SizeLg2)(field_type);
  wpi_upb_Array* ret = WPI_UPB_PRIVATE(_wpi_upb_Array_New)(&d->arena, 4, lg2);
  if (!ret) _wpi_upb_Decoder_ErrorJmp(d, kUpb_DecodeStatus_OutOfMemory);
  return ret;
}

static const char* _wpi_upb_Decoder_DecodeToArray(
    wpi_upb_Decoder* d, const char* ptr, wpi_upb_Message* msg,
    const wpi_upb_MiniTableSubInternal* subs, const wpi_upb_MiniTableField* field,
    wireval* val, int op) {
  wpi_upb_Array** arrp = WPI_UPB_PTR_AT(msg, field->WPI_UPB_PRIVATE(offset), void);
  wpi_upb_Array* arr = *arrp;
  void* mem;

  if (arr) {
    _wpi_upb_Decoder_Reserve(d, arr, 1);
  } else {
    arr = _wpi_upb_Decoder_CreateArray(d, field);
    *arrp = arr;
  }

  switch (op) {
    case kUpb_DecodeOp_Scalar1Byte:
    case kUpb_DecodeOp_Scalar4Byte:
    case kUpb_DecodeOp_Scalar8Byte:
      /* Append scalar value. */
      mem = WPI_UPB_PTR_AT(wpi_upb_Array_MutableDataPtr(arr),
                       arr->WPI_UPB_PRIVATE(size) << op, void);
      arr->WPI_UPB_PRIVATE(size)++;
      memcpy(mem, val, 1 << op);
      return ptr;
    case kUpb_DecodeOp_String:
      _wpi_upb_Decoder_VerifyUtf8(d, ptr, val->size);
      /* Fallthrough. */
    case kUpb_DecodeOp_Bytes: {
      /* Append bytes. */
      wpi_upb_StringView* str = (wpi_upb_StringView*)wpi_upb_Array_MutableDataPtr(arr) +
                            arr->WPI_UPB_PRIVATE(size);
      arr->WPI_UPB_PRIVATE(size)++;
      return _wpi_upb_Decoder_ReadString(d, ptr, val->size, str);
    }
    case kUpb_DecodeOp_SubMessage: {
      /* Append submessage / group. */
      wpi_upb_TaggedMessagePtr* target = WPI_UPB_PTR_AT(
          wpi_upb_Array_MutableDataPtr(arr), arr->WPI_UPB_PRIVATE(size) * sizeof(void*),
          wpi_upb_TaggedMessagePtr);
      wpi_upb_Message* submsg = _wpi_upb_Decoder_NewSubMessage(d, subs, field, target);
      arr->WPI_UPB_PRIVATE(size)++;
      if (WPI_UPB_UNLIKELY(field->WPI_UPB_PRIVATE(descriptortype) ==
                       kUpb_FieldType_Group)) {
        return _wpi_upb_Decoder_DecodeKnownGroup(d, ptr, submsg, subs, field);
      } else {
        return _wpi_upb_Decoder_DecodeSubMessage(d, ptr, submsg, subs, field,
                                             val->size);
      }
    }
    case OP_FIXPCK_LG2(2):
    case OP_FIXPCK_LG2(3):
      return _wpi_upb_Decoder_DecodeFixedPacked(d, ptr, arr, val, field,
                                            op - OP_FIXPCK_LG2(0));
    case OP_VARPCK_LG2(0):
    case OP_VARPCK_LG2(2):
    case OP_VARPCK_LG2(3):
      return _wpi_upb_Decoder_DecodeVarintPacked(d, ptr, arr, val, field,
                                             op - OP_VARPCK_LG2(0));
    case kUpb_DecodeOp_Enum:
      return _wpi_upb_Decoder_DecodeEnumArray(d, ptr, msg, arr, subs, field, val);
    case kUpb_DecodeOp_PackedEnum:
      return _wpi_upb_Decoder_DecodeEnumPacked(d, ptr, msg, arr, subs, field, val);
    default:
      WPI_UPB_UNREACHABLE();
  }
}

static wpi_upb_Map* _wpi_upb_Decoder_CreateMap(wpi_upb_Decoder* d,
                                       const wpi_upb_MiniTable* entry) {
  // Maps descriptor type -> upb map size
  static const uint8_t kSizeInMap[] = {
      [0] = -1,  // invalid descriptor type
      [kUpb_FieldType_Double] = 8,
      [kUpb_FieldType_Float] = 4,
      [kUpb_FieldType_Int64] = 8,
      [kUpb_FieldType_UInt64] = 8,
      [kUpb_FieldType_Int32] = 4,
      [kUpb_FieldType_Fixed64] = 8,
      [kUpb_FieldType_Fixed32] = 4,
      [kUpb_FieldType_Bool] = 1,
      [kUpb_FieldType_String] = WPI_UPB_MAPTYPE_STRING,
      [kUpb_FieldType_Group] = sizeof(void*),
      [kUpb_FieldType_Message] = sizeof(void*),
      [kUpb_FieldType_Bytes] = WPI_UPB_MAPTYPE_STRING,
      [kUpb_FieldType_UInt32] = 4,
      [kUpb_FieldType_Enum] = 4,
      [kUpb_FieldType_SFixed32] = 4,
      [kUpb_FieldType_SFixed64] = 8,
      [kUpb_FieldType_SInt32] = 4,
      [kUpb_FieldType_SInt64] = 8,
  };

  const wpi_upb_MiniTableField* key_field = &entry->WPI_UPB_PRIVATE(fields)[0];
  const wpi_upb_MiniTableField* val_field = &entry->WPI_UPB_PRIVATE(fields)[1];
  char key_size = kSizeInMap[key_field->WPI_UPB_PRIVATE(descriptortype)];
  char val_size = kSizeInMap[val_field->WPI_UPB_PRIVATE(descriptortype)];
  WPI_UPB_ASSERT(key_field->WPI_UPB_PRIVATE(offset) == offsetof(wpi_upb_MapEntry, k));
  WPI_UPB_ASSERT(val_field->WPI_UPB_PRIVATE(offset) == offsetof(wpi_upb_MapEntry, v));
  wpi_upb_Map* ret = _wpi_upb_Map_New(&d->arena, key_size, val_size);
  if (!ret) _wpi_upb_Decoder_ErrorJmp(d, kUpb_DecodeStatus_OutOfMemory);
  return ret;
}

static const char* _wpi_upb_Decoder_DecodeToMap(
    wpi_upb_Decoder* d, const char* ptr, wpi_upb_Message* msg,
    const wpi_upb_MiniTableSubInternal* subs, const wpi_upb_MiniTableField* field,
    wireval* val) {
  wpi_upb_Map** map_p = WPI_UPB_PTR_AT(msg, field->WPI_UPB_PRIVATE(offset), wpi_upb_Map*);
  wpi_upb_Map* map = *map_p;
  wpi_upb_MapEntry ent;
  WPI_UPB_ASSERT(wpi_upb_MiniTableField_Type(field) == kUpb_FieldType_Message);
  const wpi_upb_MiniTable* entry = _wpi_upb_MiniTableSubs_MessageByField(subs, field);

  WPI_UPB_ASSERT(entry);
  WPI_UPB_ASSERT(entry->WPI_UPB_PRIVATE(field_count) == 2);
  WPI_UPB_ASSERT(wpi_upb_MiniTableField_IsScalar(&entry->WPI_UPB_PRIVATE(fields)[0]));
  WPI_UPB_ASSERT(wpi_upb_MiniTableField_IsScalar(&entry->WPI_UPB_PRIVATE(fields)[1]));

  if (!map) {
    map = _wpi_upb_Decoder_CreateMap(d, entry);
    *map_p = map;
  }

  // Parse map entry.
  memset(&ent, 0, sizeof(ent));

  if (entry->WPI_UPB_PRIVATE(fields)[1].WPI_UPB_PRIVATE(descriptortype) ==
          kUpb_FieldType_Message ||
      entry->WPI_UPB_PRIVATE(fields)[1].WPI_UPB_PRIVATE(descriptortype) ==
          kUpb_FieldType_Group) {
    // Create proactively to handle the case where it doesn't appear.
    wpi_upb_TaggedMessagePtr msg;
    _wpi_upb_Decoder_NewSubMessage(d, entry->WPI_UPB_PRIVATE(subs),
                               &entry->WPI_UPB_PRIVATE(fields)[1], &msg);
    ent.v.val = wpi_upb_value_uintptr(msg);
  }

  ptr = _wpi_upb_Decoder_DecodeSubMessage(d, ptr, &ent.message, subs, field,
                                      val->size);
  // check if ent had any unknown fields
  size_t size;
  wpi_upb_Message_GetUnknown(&ent.message, &size);
  if (size != 0) {
    char* buf;
    size_t size;
    uint32_t tag =
        ((uint32_t)field->WPI_UPB_PRIVATE(number) << 3) | kUpb_WireType_Delimited;
    wpi_upb_EncodeStatus status =
        wpi_upb_Encode(&ent.message, entry, 0, &d->arena, &buf, &size);
    if (status != kUpb_EncodeStatus_Ok) {
      _wpi_upb_Decoder_ErrorJmp(d, kUpb_DecodeStatus_OutOfMemory);
    }
    _wpi_upb_Decoder_AddUnknownVarints(d, msg, tag, size);
    if (!WPI_UPB_PRIVATE(_wpi_upb_Message_AddUnknown)(msg, buf, size, &d->arena)) {
      _wpi_upb_Decoder_ErrorJmp(d, kUpb_DecodeStatus_OutOfMemory);
    }
  } else {
    if (_wpi_upb_Map_Insert(map, &ent.k, map->key_size, &ent.v, map->val_size,
                        &d->arena) == kUpb_MapInsertStatus_OutOfMemory) {
      _wpi_upb_Decoder_ErrorJmp(d, kUpb_DecodeStatus_OutOfMemory);
    }
  }
  return ptr;
}

static const char* _wpi_upb_Decoder_DecodeToSubMessage(
    wpi_upb_Decoder* d, const char* ptr, wpi_upb_Message* msg,
    const wpi_upb_MiniTableSubInternal* subs, const wpi_upb_MiniTableField* field,
    wireval* val, int op) {
  void* mem = WPI_UPB_PTR_AT(msg, field->WPI_UPB_PRIVATE(offset), void);
  int type = field->WPI_UPB_PRIVATE(descriptortype);

  if (WPI_UPB_UNLIKELY(op == kUpb_DecodeOp_Enum) &&
      !_wpi_upb_Decoder_CheckEnum(d, ptr, msg,
                              _wpi_upb_MiniTableSubs_EnumByField(subs, field),
                              field, val)) {
    return ptr;
  }

  // Set presence if necessary.
  if (WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_HasHasbit)(field)) {
    WPI_UPB_PRIVATE(_wpi_upb_Message_SetHasbit)(msg, field);
  } else if (wpi_upb_MiniTableField_IsInOneof(field)) {
    // Oneof case
    uint32_t* oneof_case = WPI_UPB_PRIVATE(_wpi_upb_Message_OneofCasePtr)(msg, field);
    if (op == kUpb_DecodeOp_SubMessage &&
        *oneof_case != field->WPI_UPB_PRIVATE(number)) {
      memset(mem, 0, sizeof(void*));
    }
    *oneof_case = field->WPI_UPB_PRIVATE(number);
  }

  // Store into message.
  switch (op) {
    case kUpb_DecodeOp_SubMessage: {
      wpi_upb_TaggedMessagePtr* submsgp = mem;
      wpi_upb_Message* submsg;
      if (*submsgp) {
        submsg = _wpi_upb_Decoder_ReuseSubMessage(d, subs, field, submsgp);
      } else {
        submsg = _wpi_upb_Decoder_NewSubMessage(d, subs, field, submsgp);
      }
      if (WPI_UPB_UNLIKELY(type == kUpb_FieldType_Group)) {
        ptr = _wpi_upb_Decoder_DecodeKnownGroup(d, ptr, submsg, subs, field);
      } else {
        ptr = _wpi_upb_Decoder_DecodeSubMessage(d, ptr, submsg, subs, field,
                                            val->size);
      }
      break;
    }
    case kUpb_DecodeOp_String:
      _wpi_upb_Decoder_VerifyUtf8(d, ptr, val->size);
      /* Fallthrough. */
    case kUpb_DecodeOp_Bytes:
      return _wpi_upb_Decoder_ReadString(d, ptr, val->size, mem);
    case kUpb_DecodeOp_Scalar8Byte:
      memcpy(mem, val, 8);
      break;
    case kUpb_DecodeOp_Enum:
    case kUpb_DecodeOp_Scalar4Byte:
      memcpy(mem, val, 4);
      break;
    case kUpb_DecodeOp_Scalar1Byte:
      memcpy(mem, val, 1);
      break;
    default:
      WPI_UPB_UNREACHABLE();
  }

  return ptr;
}

WPI_UPB_NOINLINE
const char* _wpi_upb_Decoder_CheckRequired(wpi_upb_Decoder* d, const char* ptr,
                                       const wpi_upb_Message* msg,
                                       const wpi_upb_MiniTable* m) {
  WPI_UPB_ASSERT(m->WPI_UPB_PRIVATE(required_count));
  if (WPI_UPB_UNLIKELY(d->options & kUpb_DecodeOption_CheckRequired)) {
    d->missing_required =
        !WPI_UPB_PRIVATE(_wpi_upb_Message_IsInitializedShallow)(msg, m);
  }
  return ptr;
}

WPI_UPB_FORCEINLINE
bool _wpi_upb_Decoder_TryFastDispatch(wpi_upb_Decoder* d, const char** ptr,
                                  wpi_upb_Message* msg, const wpi_upb_MiniTable* m) {
#if WPI_UPB_FASTTABLE
  if (m && m->WPI_UPB_PRIVATE(table_mask) != (unsigned char)-1) {
    uint16_t tag = _wpi_upb_FastDecoder_LoadTag(*ptr);
    intptr_t table = decode_totable(m);
    *ptr = _wpi_upb_FastDecoder_TagDispatch(d, *ptr, msg, table, 0, tag);
    return true;
  }
#endif
  return false;
}

static const char* wpi_upb_Decoder_SkipField(wpi_upb_Decoder* d, const char* ptr,
                                         uint32_t tag) {
  int field_number = tag >> 3;
  int wire_type = tag & 7;
  switch (wire_type) {
    case kUpb_WireType_Varint: {
      uint64_t val;
      return _wpi_upb_Decoder_DecodeVarint(d, ptr, &val);
    }
    case kUpb_WireType_64Bit:
      return ptr + 8;
    case kUpb_WireType_32Bit:
      return ptr + 4;
    case kUpb_WireType_Delimited: {
      uint32_t size;
      ptr = wpi_upb_Decoder_DecodeSize(d, ptr, &size);
      return ptr + size;
    }
    case kUpb_WireType_StartGroup:
      return _wpi_upb_Decoder_DecodeUnknownGroup(d, ptr, field_number);
    default:
      _wpi_upb_Decoder_ErrorJmp(d, kUpb_DecodeStatus_Malformed);
  }
}

enum {
  kStartItemTag = ((kUpb_MsgSet_Item << 3) | kUpb_WireType_StartGroup),
  kEndItemTag = ((kUpb_MsgSet_Item << 3) | kUpb_WireType_EndGroup),
  kTypeIdTag = ((kUpb_MsgSet_TypeId << 3) | kUpb_WireType_Varint),
  kMessageTag = ((kUpb_MsgSet_Message << 3) | kUpb_WireType_Delimited),
};

static void wpi_upb_Decoder_AddKnownMessageSetItem(
    wpi_upb_Decoder* d, wpi_upb_Message* msg, const wpi_upb_MiniTableExtension* item_mt,
    const char* data, uint32_t size) {
  wpi_upb_Extension* ext =
      WPI_UPB_PRIVATE(_wpi_upb_Message_GetOrCreateExtension)(msg, item_mt, &d->arena);
  if (WPI_UPB_UNLIKELY(!ext)) {
    _wpi_upb_Decoder_ErrorJmp(d, kUpb_DecodeStatus_OutOfMemory);
  }
  wpi_upb_Message* submsg = _wpi_upb_Decoder_NewSubMessage2(
      d, ext->ext->WPI_UPB_PRIVATE(sub).WPI_UPB_PRIVATE(submsg),
      &ext->ext->WPI_UPB_PRIVATE(field), (wpi_upb_TaggedMessagePtr*)&ext->data);
  wpi_upb_DecodeStatus status = wpi_upb_Decode(
      data, size, submsg, wpi_upb_MiniTableExtension_GetSubMessage(item_mt),
      d->extreg, d->options, &d->arena);
  if (status != kUpb_DecodeStatus_Ok) _wpi_upb_Decoder_ErrorJmp(d, status);
}

static void wpi_upb_Decoder_AddUnknownMessageSetItem(wpi_upb_Decoder* d,
                                                 wpi_upb_Message* msg,
                                                 uint32_t type_id,
                                                 const char* message_data,
                                                 uint32_t message_size) {
  char buf[60];
  char* ptr = buf;
  ptr = wpi_upb_Decoder_EncodeVarint32(kStartItemTag, ptr);
  ptr = wpi_upb_Decoder_EncodeVarint32(kTypeIdTag, ptr);
  ptr = wpi_upb_Decoder_EncodeVarint32(type_id, ptr);
  ptr = wpi_upb_Decoder_EncodeVarint32(kMessageTag, ptr);
  ptr = wpi_upb_Decoder_EncodeVarint32(message_size, ptr);
  char* split = ptr;

  ptr = wpi_upb_Decoder_EncodeVarint32(kEndItemTag, ptr);
  char* end = ptr;

  if (!WPI_UPB_PRIVATE(_wpi_upb_Message_AddUnknown)(msg, buf, split - buf, &d->arena) ||
      !WPI_UPB_PRIVATE(_wpi_upb_Message_AddUnknown)(msg, message_data, message_size,
                                            &d->arena) ||
      !WPI_UPB_PRIVATE(_wpi_upb_Message_AddUnknown)(msg, split, end - split,
                                            &d->arena)) {
    _wpi_upb_Decoder_ErrorJmp(d, kUpb_DecodeStatus_OutOfMemory);
  }
}

static void wpi_upb_Decoder_AddMessageSetItem(wpi_upb_Decoder* d, wpi_upb_Message* msg,
                                          const wpi_upb_MiniTable* t,
                                          uint32_t type_id, const char* data,
                                          uint32_t size) {
  const wpi_upb_MiniTableExtension* item_mt =
      wpi_upb_ExtensionRegistry_Lookup(d->extreg, t, type_id);
  if (item_mt) {
    wpi_upb_Decoder_AddKnownMessageSetItem(d, msg, item_mt, data, size);
  } else {
    wpi_upb_Decoder_AddUnknownMessageSetItem(d, msg, type_id, data, size);
  }
}

static const char* wpi_upb_Decoder_DecodeMessageSetItem(
    wpi_upb_Decoder* d, const char* ptr, wpi_upb_Message* msg,
    const wpi_upb_MiniTable* layout) {
  uint32_t type_id = 0;
  wpi_upb_StringView preserved = {NULL, 0};
  typedef enum {
    kUpb_HaveId = 1 << 0,
    kUpb_HavePayload = 1 << 1,
  } StateMask;
  StateMask state_mask = 0;
  while (!_wpi_upb_Decoder_IsDone(d, &ptr)) {
    uint32_t tag;
    ptr = _wpi_upb_Decoder_DecodeTag(d, ptr, &tag);
    switch (tag) {
      case kEndItemTag:
        return ptr;
      case kTypeIdTag: {
        uint64_t tmp;
        ptr = _wpi_upb_Decoder_DecodeVarint(d, ptr, &tmp);
        if (state_mask & kUpb_HaveId) break;  // Ignore dup.
        state_mask |= kUpb_HaveId;
        type_id = tmp;
        if (state_mask & kUpb_HavePayload) {
          wpi_upb_Decoder_AddMessageSetItem(d, msg, layout, type_id, preserved.data,
                                        preserved.size);
        }
        break;
      }
      case kMessageTag: {
        uint32_t size;
        ptr = wpi_upb_Decoder_DecodeSize(d, ptr, &size);
        const char* data = ptr;
        ptr += size;
        if (state_mask & kUpb_HavePayload) break;  // Ignore dup.
        state_mask |= kUpb_HavePayload;
        if (state_mask & kUpb_HaveId) {
          wpi_upb_Decoder_AddMessageSetItem(d, msg, layout, type_id, data, size);
        } else {
          // Out of order, we must preserve the payload.
          preserved.data = data;
          preserved.size = size;
        }
        break;
      }
      default:
        // We do not preserve unexpected fields inside a message set item.
        ptr = wpi_upb_Decoder_SkipField(d, ptr, tag);
        break;
    }
  }
  _wpi_upb_Decoder_ErrorJmp(d, kUpb_DecodeStatus_Malformed);
}

static const wpi_upb_MiniTableField* _wpi_upb_Decoder_FindField(wpi_upb_Decoder* d,
                                                        const wpi_upb_MiniTable* t,
                                                        uint32_t field_number,
                                                        int* last_field_index) {
  static wpi_upb_MiniTableField none = {
      0, 0, 0, 0, kUpb_FakeFieldType_FieldNotFound, 0};
  if (t == NULL) return &none;

  size_t idx = ((size_t)field_number) - 1;  // 0 wraps to SIZE_MAX
  if (idx < t->WPI_UPB_PRIVATE(dense_below)) {
    // Fastest case: index into dense fields.
    goto found;
  }

  if (t->WPI_UPB_PRIVATE(dense_below) < t->WPI_UPB_PRIVATE(field_count)) {
    // Linear search non-dense fields. Resume scanning from last_field_index
    // since fields are usually in order.
    size_t last = *last_field_index;
    for (idx = last; idx < t->WPI_UPB_PRIVATE(field_count); idx++) {
      if (t->WPI_UPB_PRIVATE(fields)[idx].WPI_UPB_PRIVATE(number) == field_number) {
        goto found;
      }
    }

    for (idx = t->WPI_UPB_PRIVATE(dense_below); idx < last; idx++) {
      if (t->WPI_UPB_PRIVATE(fields)[idx].WPI_UPB_PRIVATE(number) == field_number) {
        goto found;
      }
    }
  }

  if (d->extreg) {
    switch (t->WPI_UPB_PRIVATE(ext)) {
      case kUpb_ExtMode_Extendable: {
        const wpi_upb_MiniTableExtension* ext =
            wpi_upb_ExtensionRegistry_Lookup(d->extreg, t, field_number);
        if (ext) return &ext->WPI_UPB_PRIVATE(field);
        break;
      }
      case kUpb_ExtMode_IsMessageSet:
        if (field_number == kUpb_MsgSet_Item) {
          static wpi_upb_MiniTableField item = {
              0, 0, 0, 0, kUpb_FakeFieldType_MessageSetItem, 0};
          return &item;
        }
        break;
    }
  }

  return &none;  // Unknown field.

found:
  WPI_UPB_ASSERT(t->WPI_UPB_PRIVATE(fields)[idx].WPI_UPB_PRIVATE(number) == field_number);
  *last_field_index = idx;
  return &t->WPI_UPB_PRIVATE(fields)[idx];
}

static int _wpi_upb_Decoder_GetVarintOp(const wpi_upb_MiniTableField* field) {
  static const int8_t kVarintOps[] = {
      [kUpb_FakeFieldType_FieldNotFound] = kUpb_DecodeOp_UnknownField,
      [kUpb_FieldType_Double] = kUpb_DecodeOp_UnknownField,
      [kUpb_FieldType_Float] = kUpb_DecodeOp_UnknownField,
      [kUpb_FieldType_Int64] = kUpb_DecodeOp_Scalar8Byte,
      [kUpb_FieldType_UInt64] = kUpb_DecodeOp_Scalar8Byte,
      [kUpb_FieldType_Int32] = kUpb_DecodeOp_Scalar4Byte,
      [kUpb_FieldType_Fixed64] = kUpb_DecodeOp_UnknownField,
      [kUpb_FieldType_Fixed32] = kUpb_DecodeOp_UnknownField,
      [kUpb_FieldType_Bool] = kUpb_DecodeOp_Scalar1Byte,
      [kUpb_FieldType_String] = kUpb_DecodeOp_UnknownField,
      [kUpb_FieldType_Group] = kUpb_DecodeOp_UnknownField,
      [kUpb_FieldType_Message] = kUpb_DecodeOp_UnknownField,
      [kUpb_FieldType_Bytes] = kUpb_DecodeOp_UnknownField,
      [kUpb_FieldType_UInt32] = kUpb_DecodeOp_Scalar4Byte,
      [kUpb_FieldType_Enum] = kUpb_DecodeOp_Enum,
      [kUpb_FieldType_SFixed32] = kUpb_DecodeOp_UnknownField,
      [kUpb_FieldType_SFixed64] = kUpb_DecodeOp_UnknownField,
      [kUpb_FieldType_SInt32] = kUpb_DecodeOp_Scalar4Byte,
      [kUpb_FieldType_SInt64] = kUpb_DecodeOp_Scalar8Byte,
      [kUpb_FakeFieldType_MessageSetItem] = kUpb_DecodeOp_UnknownField,
  };

  return kVarintOps[field->WPI_UPB_PRIVATE(descriptortype)];
}

WPI_UPB_FORCEINLINE
void _wpi_upb_Decoder_CheckUnlinked(wpi_upb_Decoder* d, const wpi_upb_MiniTable* mt,
                                const wpi_upb_MiniTableField* field, int* op) {
  // If sub-message is not linked, treat as unknown.
  if (field->WPI_UPB_PRIVATE(mode) & kUpb_LabelFlags_IsExtension) return;
  const wpi_upb_MiniTable* mt_sub =
      _wpi_upb_MiniTableSubs_MessageByField(mt->WPI_UPB_PRIVATE(subs), field);
  if ((d->options & kUpb_DecodeOption_ExperimentalAllowUnlinked) ||
      !WPI_UPB_PRIVATE(_wpi_upb_MiniTable_IsEmpty)(mt_sub)) {
    return;
  }
#ifndef NDEBUG
  const wpi_upb_MiniTableField* oneof = wpi_upb_MiniTable_GetOneof(mt, field);
  if (oneof) {
    // All other members of the oneof must be message fields that are also
    // unlinked.
    do {
      WPI_UPB_ASSERT(wpi_upb_MiniTableField_CType(oneof) == kUpb_CType_Message);
      const wpi_upb_MiniTable* oneof_sub =
          *mt->WPI_UPB_PRIVATE(subs)[oneof->WPI_UPB_PRIVATE(submsg_index)].WPI_UPB_PRIVATE(
              submsg);
      WPI_UPB_ASSERT(!oneof_sub);
    } while (wpi_upb_MiniTable_NextOneofField(mt, &oneof));
  }
#endif  // NDEBUG
  *op = kUpb_DecodeOp_UnknownField;
}

WPI_UPB_FORCEINLINE
void _wpi_upb_Decoder_MaybeVerifyUtf8(wpi_upb_Decoder* d,
                                  const wpi_upb_MiniTableField* field, int* op) {
  if ((field->WPI_UPB_ONLYBITS(mode) & kUpb_LabelFlags_IsAlternate) &&
      WPI_UPB_UNLIKELY(d->options & kUpb_DecodeOption_AlwaysValidateUtf8))
    *op = kUpb_DecodeOp_String;
}

static int _wpi_upb_Decoder_GetDelimitedOp(wpi_upb_Decoder* d, const wpi_upb_MiniTable* mt,
                                       const wpi_upb_MiniTableField* field) {
  enum { kRepeatedBase = 19 };

  static const int8_t kDelimitedOps[] = {
      // For non-repeated field type.
      [kUpb_FakeFieldType_FieldNotFound] =
          kUpb_DecodeOp_UnknownField,  // Field not found.
      [kUpb_FieldType_Double] = kUpb_DecodeOp_UnknownField,
      [kUpb_FieldType_Float] = kUpb_DecodeOp_UnknownField,
      [kUpb_FieldType_Int64] = kUpb_DecodeOp_UnknownField,
      [kUpb_FieldType_UInt64] = kUpb_DecodeOp_UnknownField,
      [kUpb_FieldType_Int32] = kUpb_DecodeOp_UnknownField,
      [kUpb_FieldType_Fixed64] = kUpb_DecodeOp_UnknownField,
      [kUpb_FieldType_Fixed32] = kUpb_DecodeOp_UnknownField,
      [kUpb_FieldType_Bool] = kUpb_DecodeOp_UnknownField,
      [kUpb_FieldType_String] = kUpb_DecodeOp_String,
      [kUpb_FieldType_Group] = kUpb_DecodeOp_UnknownField,
      [kUpb_FieldType_Message] = kUpb_DecodeOp_SubMessage,
      [kUpb_FieldType_Bytes] = kUpb_DecodeOp_Bytes,
      [kUpb_FieldType_UInt32] = kUpb_DecodeOp_UnknownField,
      [kUpb_FieldType_Enum] = kUpb_DecodeOp_UnknownField,
      [kUpb_FieldType_SFixed32] = kUpb_DecodeOp_UnknownField,
      [kUpb_FieldType_SFixed64] = kUpb_DecodeOp_UnknownField,
      [kUpb_FieldType_SInt32] = kUpb_DecodeOp_UnknownField,
      [kUpb_FieldType_SInt64] = kUpb_DecodeOp_UnknownField,
      [kUpb_FakeFieldType_MessageSetItem] = kUpb_DecodeOp_UnknownField,
      // For repeated field type.
      [kRepeatedBase + kUpb_FieldType_Double] = OP_FIXPCK_LG2(3),
      [kRepeatedBase + kUpb_FieldType_Float] = OP_FIXPCK_LG2(2),
      [kRepeatedBase + kUpb_FieldType_Int64] = OP_VARPCK_LG2(3),
      [kRepeatedBase + kUpb_FieldType_UInt64] = OP_VARPCK_LG2(3),
      [kRepeatedBase + kUpb_FieldType_Int32] = OP_VARPCK_LG2(2),
      [kRepeatedBase + kUpb_FieldType_Fixed64] = OP_FIXPCK_LG2(3),
      [kRepeatedBase + kUpb_FieldType_Fixed32] = OP_FIXPCK_LG2(2),
      [kRepeatedBase + kUpb_FieldType_Bool] = OP_VARPCK_LG2(0),
      [kRepeatedBase + kUpb_FieldType_String] = kUpb_DecodeOp_String,
      [kRepeatedBase + kUpb_FieldType_Group] = kUpb_DecodeOp_SubMessage,
      [kRepeatedBase + kUpb_FieldType_Message] = kUpb_DecodeOp_SubMessage,
      [kRepeatedBase + kUpb_FieldType_Bytes] = kUpb_DecodeOp_Bytes,
      [kRepeatedBase + kUpb_FieldType_UInt32] = OP_VARPCK_LG2(2),
      [kRepeatedBase + kUpb_FieldType_Enum] = kUpb_DecodeOp_PackedEnum,
      [kRepeatedBase + kUpb_FieldType_SFixed32] = OP_FIXPCK_LG2(2),
      [kRepeatedBase + kUpb_FieldType_SFixed64] = OP_FIXPCK_LG2(3),
      [kRepeatedBase + kUpb_FieldType_SInt32] = OP_VARPCK_LG2(2),
      [kRepeatedBase + kUpb_FieldType_SInt64] = OP_VARPCK_LG2(3),
      // Omitting kUpb_FakeFieldType_MessageSetItem, because we never emit a
      // repeated msgset type
  };

  int ndx = field->WPI_UPB_PRIVATE(descriptortype);
  if (wpi_upb_MiniTableField_IsArray(field)) ndx += kRepeatedBase;
  int op = kDelimitedOps[ndx];

  if (op == kUpb_DecodeOp_SubMessage) {
    _wpi_upb_Decoder_CheckUnlinked(d, mt, field, &op);
  } else if (op == kUpb_DecodeOp_Bytes) {
    _wpi_upb_Decoder_MaybeVerifyUtf8(d, field, &op);
  }

  return op;
}

WPI_UPB_FORCEINLINE
const char* _wpi_upb_Decoder_DecodeWireValue(wpi_upb_Decoder* d, const char* ptr,
                                         const wpi_upb_MiniTable* mt,
                                         const wpi_upb_MiniTableField* field,
                                         int wire_type, wireval* val, int* op) {
  static const unsigned kFixed32OkMask = (1 << kUpb_FieldType_Float) |
                                         (1 << kUpb_FieldType_Fixed32) |
                                         (1 << kUpb_FieldType_SFixed32);

  static const unsigned kFixed64OkMask = (1 << kUpb_FieldType_Double) |
                                         (1 << kUpb_FieldType_Fixed64) |
                                         (1 << kUpb_FieldType_SFixed64);

  switch (wire_type) {
    case kUpb_WireType_Varint:
      ptr = _wpi_upb_Decoder_DecodeVarint(d, ptr, &val->uint64_val);
      *op = _wpi_upb_Decoder_GetVarintOp(field);
      _wpi_upb_Decoder_Munge(field->WPI_UPB_PRIVATE(descriptortype), val);
      return ptr;
    case kUpb_WireType_32Bit:
      *op = kUpb_DecodeOp_Scalar4Byte;
      if (((1 << field->WPI_UPB_PRIVATE(descriptortype)) & kFixed32OkMask) == 0) {
        *op = kUpb_DecodeOp_UnknownField;
      }
      return wpi_upb_WireReader_ReadFixed32(ptr, &val->uint32_val);
    case kUpb_WireType_64Bit:
      *op = kUpb_DecodeOp_Scalar8Byte;
      if (((1 << field->WPI_UPB_PRIVATE(descriptortype)) & kFixed64OkMask) == 0) {
        *op = kUpb_DecodeOp_UnknownField;
      }
      return wpi_upb_WireReader_ReadFixed64(ptr, &val->uint64_val);
    case kUpb_WireType_Delimited:
      ptr = wpi_upb_Decoder_DecodeSize(d, ptr, &val->size);
      *op = _wpi_upb_Decoder_GetDelimitedOp(d, mt, field);
      return ptr;
    case kUpb_WireType_StartGroup:
      val->uint32_val = field->WPI_UPB_PRIVATE(number);
      if (field->WPI_UPB_PRIVATE(descriptortype) == kUpb_FieldType_Group) {
        *op = kUpb_DecodeOp_SubMessage;
        _wpi_upb_Decoder_CheckUnlinked(d, mt, field, op);
      } else if (field->WPI_UPB_PRIVATE(descriptortype) ==
                 kUpb_FakeFieldType_MessageSetItem) {
        *op = kUpb_DecodeOp_MessageSetItem;
      } else {
        *op = kUpb_DecodeOp_UnknownField;
      }
      return ptr;
    default:
      break;
  }
  _wpi_upb_Decoder_ErrorJmp(d, kUpb_DecodeStatus_Malformed);
}

WPI_UPB_FORCEINLINE
const char* _wpi_upb_Decoder_DecodeKnownField(wpi_upb_Decoder* d, const char* ptr,
                                          wpi_upb_Message* msg,
                                          const wpi_upb_MiniTable* layout,
                                          const wpi_upb_MiniTableField* field,
                                          int op, wireval* val) {
  const wpi_upb_MiniTableSubInternal* subs = layout->WPI_UPB_PRIVATE(subs);
  uint8_t mode = field->WPI_UPB_PRIVATE(mode);
  wpi_upb_MiniTableSubInternal ext_sub;

  if (WPI_UPB_UNLIKELY(mode & kUpb_LabelFlags_IsExtension)) {
    const wpi_upb_MiniTableExtension* ext_layout =
        (const wpi_upb_MiniTableExtension*)field;
    wpi_upb_Extension* ext = WPI_UPB_PRIVATE(_wpi_upb_Message_GetOrCreateExtension)(
        msg, ext_layout, &d->arena);
    if (WPI_UPB_UNLIKELY(!ext)) {
      _wpi_upb_Decoder_ErrorJmp(d, kUpb_DecodeStatus_OutOfMemory);
    }
    d->unknown_msg = msg;
    msg = (wpi_upb_Message*)&ext->data;
    if (wpi_upb_MiniTableField_IsSubMessage(&ext->ext->WPI_UPB_PRIVATE(field))) {
      ext_sub.WPI_UPB_PRIVATE(submsg) =
          &ext->ext->WPI_UPB_PRIVATE(sub).WPI_UPB_PRIVATE(submsg);
    } else {
      ext_sub.WPI_UPB_PRIVATE(subenum) =
          ext->ext->WPI_UPB_PRIVATE(sub).WPI_UPB_PRIVATE(subenum);
    }
    subs = &ext_sub;
  }

  switch (mode & kUpb_FieldMode_Mask) {
    case kUpb_FieldMode_Array:
      return _wpi_upb_Decoder_DecodeToArray(d, ptr, msg, subs, field, val, op);
    case kUpb_FieldMode_Map:
      return _wpi_upb_Decoder_DecodeToMap(d, ptr, msg, subs, field, val);
    case kUpb_FieldMode_Scalar:
      return _wpi_upb_Decoder_DecodeToSubMessage(d, ptr, msg, subs, field, val, op);
    default:
      WPI_UPB_UNREACHABLE();
  }
}

static const char* _wpi_upb_Decoder_ReverseSkipVarint(const char* ptr,
                                                  uint32_t val) {
  uint32_t seen = 0;
  do {
    ptr--;
    seen <<= 7;
    seen |= *ptr & 0x7f;
  } while (seen != val);
  return ptr;
}

static const char* _wpi_upb_Decoder_DecodeUnknownField(wpi_upb_Decoder* d,
                                                   const char* ptr,
                                                   wpi_upb_Message* msg,
                                                   int field_number,
                                                   int wire_type, wireval val) {
  if (field_number == 0) _wpi_upb_Decoder_ErrorJmp(d, kUpb_DecodeStatus_Malformed);

  // Since unknown fields are the uncommon case, we do a little extra work here
  // to walk backwards through the buffer to find the field start.  This frees
  // up a register in the fast paths (when the field is known), which leads to
  // significant speedups in benchmarks.
  const char* start = ptr;

  if (wire_type == kUpb_WireType_Delimited) ptr += val.size;
  if (msg) {
    switch (wire_type) {
      case kUpb_WireType_Varint:
      case kUpb_WireType_Delimited:
        start--;
        while (start[-1] & 0x80) start--;
        break;
      case kUpb_WireType_32Bit:
        start -= 4;
        break;
      case kUpb_WireType_64Bit:
        start -= 8;
        break;
      default:
        break;
    }

    assert(start == d->debug_valstart);
    uint32_t tag = ((uint32_t)field_number << 3) | wire_type;
    start = _wpi_upb_Decoder_ReverseSkipVarint(start, tag);
    assert(start == d->debug_tagstart);

    if (wire_type == kUpb_WireType_StartGroup) {
      d->unknown = start;
      d->unknown_msg = msg;
      ptr = _wpi_upb_Decoder_DecodeUnknownGroup(d, ptr, field_number);
      start = d->unknown;
      d->unknown = NULL;
    }
    if (!WPI_UPB_PRIVATE(_wpi_upb_Message_AddUnknown)(msg, start, ptr - start,
                                              &d->arena)) {
      _wpi_upb_Decoder_ErrorJmp(d, kUpb_DecodeStatus_OutOfMemory);
    }
  } else if (wire_type == kUpb_WireType_StartGroup) {
    ptr = _wpi_upb_Decoder_DecodeUnknownGroup(d, ptr, field_number);
  }
  return ptr;
}

WPI_UPB_NOINLINE
static const char* _wpi_upb_Decoder_DecodeMessage(wpi_upb_Decoder* d, const char* ptr,
                                              wpi_upb_Message* msg,
                                              const wpi_upb_MiniTable* layout) {
  int last_field_index = 0;

#if WPI_UPB_FASTTABLE
  // The first time we want to skip fast dispatch, because we may have just been
  // invoked by the fast parser to handle a case that it bailed on.
  if (!_wpi_upb_Decoder_IsDone(d, &ptr)) goto nofast;
#endif

  while (!_wpi_upb_Decoder_IsDone(d, &ptr)) {
    uint32_t tag;
    const wpi_upb_MiniTableField* field;
    int field_number;
    int wire_type;
    wireval val;
    int op;

    if (_wpi_upb_Decoder_TryFastDispatch(d, &ptr, msg, layout)) break;

#if WPI_UPB_FASTTABLE
  nofast:
#endif

#ifndef NDEBUG
    d->debug_tagstart = ptr;
#endif

    WPI_UPB_ASSERT(ptr < d->input.limit_ptr);
    ptr = _wpi_upb_Decoder_DecodeTag(d, ptr, &tag);
    field_number = tag >> 3;
    wire_type = tag & 7;

#ifndef NDEBUG
    d->debug_valstart = ptr;
#endif

    if (wire_type == kUpb_WireType_EndGroup) {
      d->end_group = field_number;
      return ptr;
    }

    field = _wpi_upb_Decoder_FindField(d, layout, field_number, &last_field_index);
    ptr = _wpi_upb_Decoder_DecodeWireValue(d, ptr, layout, field, wire_type, &val,
                                       &op);

    if (op >= 0) {
      ptr = _wpi_upb_Decoder_DecodeKnownField(d, ptr, msg, layout, field, op, &val);
    } else {
      switch (op) {
        case kUpb_DecodeOp_UnknownField:
          ptr = _wpi_upb_Decoder_DecodeUnknownField(d, ptr, msg, field_number,
                                                wire_type, val);
          break;
        case kUpb_DecodeOp_MessageSetItem:
          ptr = wpi_upb_Decoder_DecodeMessageSetItem(d, ptr, msg, layout);
          break;
      }
    }
  }

  return WPI_UPB_UNLIKELY(layout && layout->WPI_UPB_PRIVATE(required_count))
             ? _wpi_upb_Decoder_CheckRequired(d, ptr, msg, layout)
             : ptr;
}

const char* _wpi_upb_FastDecoder_DecodeGeneric(struct wpi_upb_Decoder* d,
                                           const char* ptr, wpi_upb_Message* msg,
                                           intptr_t table, uint64_t hasbits,
                                           uint64_t data) {
  (void)data;
  *(uint32_t*)msg |= hasbits;
  return _wpi_upb_Decoder_DecodeMessage(d, ptr, msg, decode_totablep(table));
}

static wpi_upb_DecodeStatus _wpi_upb_Decoder_DecodeTop(struct wpi_upb_Decoder* d,
                                               const char* buf,
                                               wpi_upb_Message* msg,
                                               const wpi_upb_MiniTable* m) {
  if (!_wpi_upb_Decoder_TryFastDispatch(d, &buf, msg, m)) {
    _wpi_upb_Decoder_DecodeMessage(d, buf, msg, m);
  }
  if (d->end_group != DECODE_NOGROUP) return kUpb_DecodeStatus_Malformed;
  if (d->missing_required) return kUpb_DecodeStatus_MissingRequired;
  return kUpb_DecodeStatus_Ok;
}

WPI_UPB_NOINLINE
const char* _wpi_upb_Decoder_IsDoneFallback(wpi_upb_EpsCopyInputStream* e,
                                        const char* ptr, int overrun) {
  return _wpi_upb_EpsCopyInputStream_IsDoneFallbackInline(
      e, ptr, overrun, _wpi_upb_Decoder_BufferFlipCallback);
}

static wpi_upb_DecodeStatus wpi_upb_Decoder_Decode(wpi_upb_Decoder* const decoder,
                                           const char* const buf,
                                           wpi_upb_Message* const msg,
                                           const wpi_upb_MiniTable* const m,
                                           wpi_upb_Arena* const arena) {
  if (WPI_UPB_SETJMP(decoder->err) == 0) {
    decoder->status = _wpi_upb_Decoder_DecodeTop(decoder, buf, msg, m);
  } else {
    WPI_UPB_ASSERT(decoder->status != kUpb_DecodeStatus_Ok);
  }

  WPI_UPB_PRIVATE(_wpi_upb_Arena_SwapOut)(arena, &decoder->arena);

  return decoder->status;
}

wpi_upb_DecodeStatus wpi_upb_Decode(const char* buf, size_t size, wpi_upb_Message* msg,
                            const wpi_upb_MiniTable* mt,
                            const wpi_upb_ExtensionRegistry* extreg, int options,
                            wpi_upb_Arena* arena) {
  WPI_UPB_ASSERT(!wpi_upb_Message_IsFrozen(msg));
  wpi_upb_Decoder decoder;
  unsigned depth = (unsigned)options >> 16;

  wpi_upb_EpsCopyInputStream_Init(&decoder.input, &buf, size,
                              options & kUpb_DecodeOption_AliasString);

  decoder.extreg = extreg;
  decoder.unknown = NULL;
  decoder.depth = depth ? depth : kUpb_WireFormat_DefaultDepthLimit;
  decoder.end_group = DECODE_NOGROUP;
  decoder.options = (uint16_t)options;
  decoder.missing_required = false;
  decoder.status = kUpb_DecodeStatus_Ok;

  // Violating the encapsulation of the arena for performance reasons.
  // This is a temporary arena that we swap into and swap out of when we are
  // done.  The temporary arena only needs to be able to handle allocation,
  // not fuse or free, so it does not need many of the members to be initialized
  // (particularly parent_or_count).
  WPI_UPB_PRIVATE(_wpi_upb_Arena_SwapIn)(&decoder.arena, arena);

  return wpi_upb_Decoder_Decode(&decoder, buf, msg, mt, arena);
}

wpi_upb_DecodeStatus wpi_upb_DecodeLengthPrefixed(const char* buf, size_t size,
                                          wpi_upb_Message* msg,
                                          size_t* num_bytes_read,
                                          const wpi_upb_MiniTable* mt,
                                          const wpi_upb_ExtensionRegistry* extreg,
                                          int options, wpi_upb_Arena* arena) {
  // To avoid needing to make a Decoder just to decode the initial length,
  // hand-decode the leading varint for the message length here.
  uint64_t msg_len = 0;
  for (size_t i = 0;; ++i) {
    if (i >= size || i > 9) {
      return kUpb_DecodeStatus_Malformed;
    }
    uint64_t b = *buf;
    buf++;
    msg_len += (b & 0x7f) << (i * 7);
    if ((b & 0x80) == 0) {
      *num_bytes_read = i + 1 + msg_len;
      break;
    }
  }

  // If the total number of bytes we would read (= the bytes from the varint
  // plus however many bytes that varint says we should read) is larger then the
  // input buffer then error as malformed.
  if (*num_bytes_read > size) {
    return kUpb_DecodeStatus_Malformed;
  }
  if (msg_len > INT32_MAX) {
    return kUpb_DecodeStatus_Malformed;
  }

  return wpi_upb_Decode(buf, msg_len, msg, mt, extreg, options, arena);
}

const char* wpi_upb_DecodeStatus_String(wpi_upb_DecodeStatus status) {
  switch (status) {
    case kUpb_DecodeStatus_Ok:
      return "Ok";
    case kUpb_DecodeStatus_Malformed:
      return "Wire format was corrupt";
    case kUpb_DecodeStatus_OutOfMemory:
      return "Arena alloc failed";
    case kUpb_DecodeStatus_BadUtf8:
      return "String field had bad UTF-8";
    case kUpb_DecodeStatus_MaxDepthExceeded:
      return "Exceeded wpi_upb_DecodeOptions_MaxDepth";
    case kUpb_DecodeStatus_MissingRequired:
      return "Missing required field";
    case kUpb_DecodeStatus_UnlinkedSubMessage:
      return "Unlinked sub-message field was present";
    default:
      return "Unknown decode status";
  }
}

#undef OP_FIXPCK_LG2
#undef OP_VARPCK_LG2

// We encode backwards, to avoid pre-computing lengths (one-pass encode).


#include <setjmp.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>


// Must be last.

// Returns the MiniTable corresponding to a given MiniTableField
// from an array of MiniTableSubs.
static const wpi_upb_MiniTable* _wpi_upb_Encoder_GetSubMiniTable(
    const wpi_upb_MiniTableSubInternal* subs, const wpi_upb_MiniTableField* field) {
  return *subs[field->WPI_UPB_PRIVATE(submsg_index)].WPI_UPB_PRIVATE(submsg);
}

#define WPI_UPB_PB_VARINT_MAX_LEN 10

WPI_UPB_NOINLINE
static size_t encode_varint64(uint64_t val, char* buf) {
  size_t i = 0;
  do {
    uint8_t byte = val & 0x7fU;
    val >>= 7;
    if (val) byte |= 0x80U;
    buf[i++] = byte;
  } while (val);
  return i;
}

static uint32_t encode_zz32(int32_t n) {
  return ((uint32_t)n << 1) ^ (n >> 31);
}
static uint64_t encode_zz64(int64_t n) {
  return ((uint64_t)n << 1) ^ (n >> 63);
}

typedef struct {
  wpi_upb_EncodeStatus status;
  jmp_buf err;
  wpi_upb_Arena* arena;
  char *buf, *ptr, *limit;
  int options;
  int depth;
  _wpi_upb_mapsorter sorter;
} wpi_upb_encstate;

static size_t wpi_upb_roundup_pow2(size_t bytes) {
  size_t ret = 128;
  while (ret < bytes) {
    ret *= 2;
  }
  return ret;
}

WPI_UPB_NORETURN static void encode_err(wpi_upb_encstate* e, wpi_upb_EncodeStatus s) {
  WPI_UPB_ASSERT(s != kUpb_EncodeStatus_Ok);
  e->status = s;
  WPI_UPB_LONGJMP(e->err, 1);
}

WPI_UPB_NOINLINE
static void encode_growbuffer(wpi_upb_encstate* e, size_t bytes) {
  size_t old_size = e->limit - e->buf;
  size_t new_size = wpi_upb_roundup_pow2(bytes + (e->limit - e->ptr));
  char* new_buf = wpi_upb_Arena_Realloc(e->arena, e->buf, old_size, new_size);

  if (!new_buf) encode_err(e, kUpb_EncodeStatus_OutOfMemory);

  // We want previous data at the end, realloc() put it at the beginning.
  // TODO: This is somewhat inefficient since we are copying twice.
  // Maybe create a realloc() that copies to the end of the new buffer?
  if (old_size > 0) {
    memmove(new_buf + new_size - old_size, e->buf, old_size);
  }

  e->ptr = new_buf + new_size - (e->limit - e->ptr);
  e->limit = new_buf + new_size;
  e->buf = new_buf;

  e->ptr -= bytes;
}

/* Call to ensure that at least "bytes" bytes are available for writing at
 * e->ptr.  Returns false if the bytes could not be allocated. */
WPI_UPB_FORCEINLINE
void encode_reserve(wpi_upb_encstate* e, size_t bytes) {
  if ((size_t)(e->ptr - e->buf) < bytes) {
    encode_growbuffer(e, bytes);
    return;
  }

  e->ptr -= bytes;
}

/* Writes the given bytes to the buffer, handling reserve/advance. */
static void encode_bytes(wpi_upb_encstate* e, const void* data, size_t len) {
  if (len == 0) return; /* memcpy() with zero size is UB */
  encode_reserve(e, len);
  memcpy(e->ptr, data, len);
}

static void encode_fixed64(wpi_upb_encstate* e, uint64_t val) {
  val = wpi_upb_BigEndian64(val);
  encode_bytes(e, &val, sizeof(uint64_t));
}

static void encode_fixed32(wpi_upb_encstate* e, uint32_t val) {
  val = wpi_upb_BigEndian32(val);
  encode_bytes(e, &val, sizeof(uint32_t));
}

WPI_UPB_NOINLINE
static void encode_longvarint(wpi_upb_encstate* e, uint64_t val) {
  size_t len;
  char* start;

  encode_reserve(e, WPI_UPB_PB_VARINT_MAX_LEN);
  len = encode_varint64(val, e->ptr);
  start = e->ptr + WPI_UPB_PB_VARINT_MAX_LEN - len;
  memmove(start, e->ptr, len);
  e->ptr = start;
}

WPI_UPB_FORCEINLINE
void encode_varint(wpi_upb_encstate* e, uint64_t val) {
  if (val < 128 && e->ptr != e->buf) {
    --e->ptr;
    *e->ptr = val;
  } else {
    encode_longvarint(e, val);
  }
}

static void encode_double(wpi_upb_encstate* e, double d) {
  uint64_t u64;
  WPI_UPB_ASSERT(sizeof(double) == sizeof(uint64_t));
  memcpy(&u64, &d, sizeof(uint64_t));
  encode_fixed64(e, u64);
}

static void encode_float(wpi_upb_encstate* e, float d) {
  uint32_t u32;
  WPI_UPB_ASSERT(sizeof(float) == sizeof(uint32_t));
  memcpy(&u32, &d, sizeof(uint32_t));
  encode_fixed32(e, u32);
}

static void encode_tag(wpi_upb_encstate* e, uint32_t field_number,
                       uint8_t wire_type) {
  encode_varint(e, (field_number << 3) | wire_type);
}

static void encode_fixedarray(wpi_upb_encstate* e, const wpi_upb_Array* arr,
                              size_t elem_size, uint32_t tag) {
  size_t bytes = wpi_upb_Array_Size(arr) * elem_size;
  const char* data = wpi_upb_Array_DataPtr(arr);
  const char* ptr = data + bytes - elem_size;

  if (tag || !wpi_upb_IsLittleEndian()) {
    while (true) {
      if (elem_size == 4) {
        uint32_t val;
        memcpy(&val, ptr, sizeof(val));
        val = wpi_upb_BigEndian32(val);
        encode_bytes(e, &val, elem_size);
      } else {
        WPI_UPB_ASSERT(elem_size == 8);
        uint64_t val;
        memcpy(&val, ptr, sizeof(val));
        val = wpi_upb_BigEndian64(val);
        encode_bytes(e, &val, elem_size);
      }

      if (tag) encode_varint(e, tag);
      if (ptr == data) break;
      ptr -= elem_size;
    }
  } else {
    encode_bytes(e, data, bytes);
  }
}

static void encode_message(wpi_upb_encstate* e, const wpi_upb_Message* msg,
                           const wpi_upb_MiniTable* m, size_t* size);

static void encode_TaggedMessagePtr(wpi_upb_encstate* e,
                                    wpi_upb_TaggedMessagePtr tagged,
                                    const wpi_upb_MiniTable* m, size_t* size) {
  if (wpi_upb_TaggedMessagePtr_IsEmpty(tagged)) {
    m = WPI_UPB_PRIVATE(_wpi_upb_MiniTable_Empty)();
  }
  encode_message(e, WPI_UPB_PRIVATE(_wpi_upb_TaggedMessagePtr_GetMessage)(tagged), m,
                 size);
}

static void encode_scalar(wpi_upb_encstate* e, const void* _field_mem,
                          const wpi_upb_MiniTableSubInternal* subs,
                          const wpi_upb_MiniTableField* f) {
  const char* field_mem = _field_mem;
  int wire_type;

#define CASE(ctype, type, wtype, encodeval) \
  {                                         \
    ctype val = *(ctype*)field_mem;         \
    encode_##type(e, encodeval);            \
    wire_type = wtype;                      \
    break;                                  \
  }

  switch (f->WPI_UPB_PRIVATE(descriptortype)) {
    case kUpb_FieldType_Double:
      CASE(double, double, kUpb_WireType_64Bit, val);
    case kUpb_FieldType_Float:
      CASE(float, float, kUpb_WireType_32Bit, val);
    case kUpb_FieldType_Int64:
    case kUpb_FieldType_UInt64:
      CASE(uint64_t, varint, kUpb_WireType_Varint, val);
    case kUpb_FieldType_UInt32:
      CASE(uint32_t, varint, kUpb_WireType_Varint, val);
    case kUpb_FieldType_Int32:
    case kUpb_FieldType_Enum:
      CASE(int32_t, varint, kUpb_WireType_Varint, (int64_t)val);
    case kUpb_FieldType_SFixed64:
    case kUpb_FieldType_Fixed64:
      CASE(uint64_t, fixed64, kUpb_WireType_64Bit, val);
    case kUpb_FieldType_Fixed32:
    case kUpb_FieldType_SFixed32:
      CASE(uint32_t, fixed32, kUpb_WireType_32Bit, val);
    case kUpb_FieldType_Bool:
      CASE(bool, varint, kUpb_WireType_Varint, val);
    case kUpb_FieldType_SInt32:
      CASE(int32_t, varint, kUpb_WireType_Varint, encode_zz32(val));
    case kUpb_FieldType_SInt64:
      CASE(int64_t, varint, kUpb_WireType_Varint, encode_zz64(val));
    case kUpb_FieldType_String:
    case kUpb_FieldType_Bytes: {
      wpi_upb_StringView view = *(wpi_upb_StringView*)field_mem;
      encode_bytes(e, view.data, view.size);
      encode_varint(e, view.size);
      wire_type = kUpb_WireType_Delimited;
      break;
    }
    case kUpb_FieldType_Group: {
      size_t size;
      wpi_upb_TaggedMessagePtr submsg = *(wpi_upb_TaggedMessagePtr*)field_mem;
      const wpi_upb_MiniTable* subm = _wpi_upb_Encoder_GetSubMiniTable(subs, f);
      if (submsg == 0) {
        return;
      }
      if (--e->depth == 0) encode_err(e, kUpb_EncodeStatus_MaxDepthExceeded);
      encode_tag(e, wpi_upb_MiniTableField_Number(f), kUpb_WireType_EndGroup);
      encode_TaggedMessagePtr(e, submsg, subm, &size);
      wire_type = kUpb_WireType_StartGroup;
      e->depth++;
      break;
    }
    case kUpb_FieldType_Message: {
      size_t size;
      wpi_upb_TaggedMessagePtr submsg = *(wpi_upb_TaggedMessagePtr*)field_mem;
      const wpi_upb_MiniTable* subm = _wpi_upb_Encoder_GetSubMiniTable(subs, f);
      if (submsg == 0) {
        return;
      }
      if (--e->depth == 0) encode_err(e, kUpb_EncodeStatus_MaxDepthExceeded);
      encode_TaggedMessagePtr(e, submsg, subm, &size);
      encode_varint(e, size);
      wire_type = kUpb_WireType_Delimited;
      e->depth++;
      break;
    }
    default:
      WPI_UPB_UNREACHABLE();
  }
#undef CASE

  encode_tag(e, wpi_upb_MiniTableField_Number(f), wire_type);
}

static void encode_array(wpi_upb_encstate* e, const wpi_upb_Message* msg,
                         const wpi_upb_MiniTableSubInternal* subs,
                         const wpi_upb_MiniTableField* f) {
  const wpi_upb_Array* arr = *WPI_UPB_PTR_AT(msg, f->WPI_UPB_PRIVATE(offset), wpi_upb_Array*);
  bool packed = wpi_upb_MiniTableField_IsPacked(f);
  size_t pre_len = e->limit - e->ptr;

  if (arr == NULL || wpi_upb_Array_Size(arr) == 0) {
    return;
  }

#define VARINT_CASE(ctype, encode)                                         \
  {                                                                        \
    const ctype* start = wpi_upb_Array_DataPtr(arr);                           \
    const ctype* ptr = start + wpi_upb_Array_Size(arr);                        \
    uint32_t tag =                                                         \
        packed ? 0 : (f->WPI_UPB_PRIVATE(number) << 3) | kUpb_WireType_Varint; \
    do {                                                                   \
      ptr--;                                                               \
      encode_varint(e, encode);                                            \
      if (tag) encode_varint(e, tag);                                      \
    } while (ptr != start);                                                \
  }                                                                        \
  break;

#define TAG(wire_type) (packed ? 0 : (f->WPI_UPB_PRIVATE(number) << 3 | wire_type))

  switch (f->WPI_UPB_PRIVATE(descriptortype)) {
    case kUpb_FieldType_Double:
      encode_fixedarray(e, arr, sizeof(double), TAG(kUpb_WireType_64Bit));
      break;
    case kUpb_FieldType_Float:
      encode_fixedarray(e, arr, sizeof(float), TAG(kUpb_WireType_32Bit));
      break;
    case kUpb_FieldType_SFixed64:
    case kUpb_FieldType_Fixed64:
      encode_fixedarray(e, arr, sizeof(uint64_t), TAG(kUpb_WireType_64Bit));
      break;
    case kUpb_FieldType_Fixed32:
    case kUpb_FieldType_SFixed32:
      encode_fixedarray(e, arr, sizeof(uint32_t), TAG(kUpb_WireType_32Bit));
      break;
    case kUpb_FieldType_Int64:
    case kUpb_FieldType_UInt64:
      VARINT_CASE(uint64_t, *ptr);
    case kUpb_FieldType_UInt32:
      VARINT_CASE(uint32_t, *ptr);
    case kUpb_FieldType_Int32:
    case kUpb_FieldType_Enum:
      VARINT_CASE(int32_t, (int64_t)*ptr);
    case kUpb_FieldType_Bool:
      VARINT_CASE(bool, *ptr);
    case kUpb_FieldType_SInt32:
      VARINT_CASE(int32_t, encode_zz32(*ptr));
    case kUpb_FieldType_SInt64:
      VARINT_CASE(int64_t, encode_zz64(*ptr));
    case kUpb_FieldType_String:
    case kUpb_FieldType_Bytes: {
      const wpi_upb_StringView* start = wpi_upb_Array_DataPtr(arr);
      const wpi_upb_StringView* ptr = start + wpi_upb_Array_Size(arr);
      do {
        ptr--;
        encode_bytes(e, ptr->data, ptr->size);
        encode_varint(e, ptr->size);
        encode_tag(e, wpi_upb_MiniTableField_Number(f), kUpb_WireType_Delimited);
      } while (ptr != start);
      return;
    }
    case kUpb_FieldType_Group: {
      const wpi_upb_TaggedMessagePtr* start = wpi_upb_Array_DataPtr(arr);
      const wpi_upb_TaggedMessagePtr* ptr = start + wpi_upb_Array_Size(arr);
      const wpi_upb_MiniTable* subm = _wpi_upb_Encoder_GetSubMiniTable(subs, f);
      if (--e->depth == 0) encode_err(e, kUpb_EncodeStatus_MaxDepthExceeded);
      do {
        size_t size;
        ptr--;
        encode_tag(e, wpi_upb_MiniTableField_Number(f), kUpb_WireType_EndGroup);
        encode_TaggedMessagePtr(e, *ptr, subm, &size);
        encode_tag(e, wpi_upb_MiniTableField_Number(f), kUpb_WireType_StartGroup);
      } while (ptr != start);
      e->depth++;
      return;
    }
    case kUpb_FieldType_Message: {
      const wpi_upb_TaggedMessagePtr* start = wpi_upb_Array_DataPtr(arr);
      const wpi_upb_TaggedMessagePtr* ptr = start + wpi_upb_Array_Size(arr);
      const wpi_upb_MiniTable* subm = _wpi_upb_Encoder_GetSubMiniTable(subs, f);
      if (--e->depth == 0) encode_err(e, kUpb_EncodeStatus_MaxDepthExceeded);
      do {
        size_t size;
        ptr--;
        encode_TaggedMessagePtr(e, *ptr, subm, &size);
        encode_varint(e, size);
        encode_tag(e, wpi_upb_MiniTableField_Number(f), kUpb_WireType_Delimited);
      } while (ptr != start);
      e->depth++;
      return;
    }
  }
#undef VARINT_CASE

  if (packed) {
    encode_varint(e, e->limit - e->ptr - pre_len);
    encode_tag(e, wpi_upb_MiniTableField_Number(f), kUpb_WireType_Delimited);
  }
}

static void encode_mapentry(wpi_upb_encstate* e, uint32_t number,
                            const wpi_upb_MiniTable* layout,
                            const wpi_upb_MapEntry* ent) {
  const wpi_upb_MiniTableField* key_field = wpi_upb_MiniTable_MapKey(layout);
  const wpi_upb_MiniTableField* val_field = wpi_upb_MiniTable_MapValue(layout);
  size_t pre_len = e->limit - e->ptr;
  size_t size;
  encode_scalar(e, &ent->v, layout->WPI_UPB_PRIVATE(subs), val_field);
  encode_scalar(e, &ent->k, layout->WPI_UPB_PRIVATE(subs), key_field);
  size = (e->limit - e->ptr) - pre_len;
  encode_varint(e, size);
  encode_tag(e, number, kUpb_WireType_Delimited);
}

static void encode_map(wpi_upb_encstate* e, const wpi_upb_Message* msg,
                       const wpi_upb_MiniTableSubInternal* subs,
                       const wpi_upb_MiniTableField* f) {
  const wpi_upb_Map* map = *WPI_UPB_PTR_AT(msg, f->WPI_UPB_PRIVATE(offset), const wpi_upb_Map*);
  const wpi_upb_MiniTable* layout = _wpi_upb_Encoder_GetSubMiniTable(subs, f);
  WPI_UPB_ASSERT(wpi_upb_MiniTable_FieldCount(layout) == 2);

  if (!map || !wpi_upb_Map_Size(map)) return;

  if (e->options & kUpb_EncodeOption_Deterministic) {
    _wpi_upb_sortedmap sorted;
    _wpi_upb_mapsorter_pushmap(
        &e->sorter, layout->WPI_UPB_PRIVATE(fields)[0].WPI_UPB_PRIVATE(descriptortype),
        map, &sorted);
    wpi_upb_MapEntry ent;
    while (_wpi_upb_sortedmap_next(&e->sorter, map, &sorted, &ent)) {
      encode_mapentry(e, wpi_upb_MiniTableField_Number(f), layout, &ent);
    }
    _wpi_upb_mapsorter_popmap(&e->sorter, &sorted);
  } else {
    intptr_t iter = WPI_UPB_STRTABLE_BEGIN;
    wpi_upb_StringView key;
    wpi_upb_value val;
    while (wpi_upb_strtable_next2(&map->table, &key, &val, &iter)) {
      wpi_upb_MapEntry ent;
      _wpi_upb_map_fromkey(key, &ent.k, map->key_size);
      _wpi_upb_map_fromvalue(val, &ent.v, map->val_size);
      encode_mapentry(e, wpi_upb_MiniTableField_Number(f), layout, &ent);
    }
  }
}

static bool encode_shouldencode(wpi_upb_encstate* e, const wpi_upb_Message* msg,
                                const wpi_upb_MiniTableField* f) {
  if (f->presence == 0) {
    // Proto3 presence or map/array.
    const void* mem = WPI_UPB_PTR_AT(msg, f->WPI_UPB_PRIVATE(offset), void);
    switch (WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_GetRep)(f)) {
      case kUpb_FieldRep_1Byte: {
        char ch;
        memcpy(&ch, mem, 1);
        return ch != 0;
      }
      case kUpb_FieldRep_4Byte: {
        uint32_t u32;
        memcpy(&u32, mem, 4);
        return u32 != 0;
      }
      case kUpb_FieldRep_8Byte: {
        uint64_t u64;
        memcpy(&u64, mem, 8);
        return u64 != 0;
      }
      case kUpb_FieldRep_StringView: {
        const wpi_upb_StringView* str = (const wpi_upb_StringView*)mem;
        return str->size != 0;
      }
      default:
        WPI_UPB_UNREACHABLE();
    }
  } else if (WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_HasHasbit)(f)) {
    // Proto2 presence: hasbit.
    return WPI_UPB_PRIVATE(_wpi_upb_Message_GetHasbit)(msg, f);
  } else {
    // Field is in a oneof.
    return WPI_UPB_PRIVATE(_wpi_upb_Message_GetOneofCase)(msg, f) ==
           wpi_upb_MiniTableField_Number(f);
  }
}

static void encode_field(wpi_upb_encstate* e, const wpi_upb_Message* msg,
                         const wpi_upb_MiniTableSubInternal* subs,
                         const wpi_upb_MiniTableField* field) {
  switch (WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_Mode)(field)) {
    case kUpb_FieldMode_Array:
      encode_array(e, msg, subs, field);
      break;
    case kUpb_FieldMode_Map:
      encode_map(e, msg, subs, field);
      break;
    case kUpb_FieldMode_Scalar:
      encode_scalar(e, WPI_UPB_PTR_AT(msg, field->WPI_UPB_PRIVATE(offset), void), subs,
                    field);
      break;
    default:
      WPI_UPB_UNREACHABLE();
  }
}

static void encode_msgset_item(wpi_upb_encstate* e, const wpi_upb_Extension* ext) {
  size_t size;
  encode_tag(e, kUpb_MsgSet_Item, kUpb_WireType_EndGroup);
  encode_message(e, ext->data.msg_val,
                 wpi_upb_MiniTableExtension_GetSubMessage(ext->ext), &size);
  encode_varint(e, size);
  encode_tag(e, kUpb_MsgSet_Message, kUpb_WireType_Delimited);
  encode_varint(e, wpi_upb_MiniTableExtension_Number(ext->ext));
  encode_tag(e, kUpb_MsgSet_TypeId, kUpb_WireType_Varint);
  encode_tag(e, kUpb_MsgSet_Item, kUpb_WireType_StartGroup);
}

static void encode_ext(wpi_upb_encstate* e, const wpi_upb_Extension* ext,
                       bool is_message_set) {
  if (WPI_UPB_UNLIKELY(is_message_set)) {
    encode_msgset_item(e, ext);
  } else {
    wpi_upb_MiniTableSubInternal sub;
    if (wpi_upb_MiniTableField_IsSubMessage(&ext->ext->WPI_UPB_PRIVATE(field))) {
      sub.WPI_UPB_PRIVATE(submsg) = &ext->ext->WPI_UPB_PRIVATE(sub).WPI_UPB_PRIVATE(submsg);
    } else {
      sub.WPI_UPB_PRIVATE(subenum) =
          ext->ext->WPI_UPB_PRIVATE(sub).WPI_UPB_PRIVATE(subenum);
    }
    encode_field(e, (wpi_upb_Message*)&ext->data, &sub,
                 &ext->ext->WPI_UPB_PRIVATE(field));
  }
}

static void encode_message(wpi_upb_encstate* e, const wpi_upb_Message* msg,
                           const wpi_upb_MiniTable* m, size_t* size) {
  size_t pre_len = e->limit - e->ptr;

  if (e->options & kUpb_EncodeOption_CheckRequired) {
    if (m->WPI_UPB_PRIVATE(required_count)) {
      if (!WPI_UPB_PRIVATE(_wpi_upb_Message_IsInitializedShallow)(msg, m)) {
        encode_err(e, kUpb_EncodeStatus_MissingRequired);
      }
    }
  }

  if ((e->options & kUpb_EncodeOption_SkipUnknown) == 0) {
    size_t unknown_size;
    const char* unknown = wpi_upb_Message_GetUnknown(msg, &unknown_size);

    if (unknown) {
      encode_bytes(e, unknown, unknown_size);
    }
  }

  if (m->WPI_UPB_PRIVATE(ext) != kUpb_ExtMode_NonExtendable) {
    /* Encode all extensions together. Unlike C++, we do not attempt to keep
     * these in field number order relative to normal fields or even to each
     * other. */
    size_t ext_count;
    const wpi_upb_Extension* ext =
        WPI_UPB_PRIVATE(_wpi_upb_Message_Getexts)(msg, &ext_count);
    if (ext_count) {
      if (e->options & kUpb_EncodeOption_Deterministic) {
        _wpi_upb_sortedmap sorted;
        _wpi_upb_mapsorter_pushexts(&e->sorter, ext, ext_count, &sorted);
        while (_wpi_upb_sortedmap_nextext(&e->sorter, &sorted, &ext)) {
          encode_ext(e, ext, m->WPI_UPB_PRIVATE(ext) == kUpb_ExtMode_IsMessageSet);
        }
        _wpi_upb_mapsorter_popmap(&e->sorter, &sorted);
      } else {
        const wpi_upb_Extension* end = ext + ext_count;
        for (; ext != end; ext++) {
          encode_ext(e, ext, m->WPI_UPB_PRIVATE(ext) == kUpb_ExtMode_IsMessageSet);
        }
      }
    }
  }

  if (wpi_upb_MiniTable_FieldCount(m)) {
    const wpi_upb_MiniTableField* f =
        &m->WPI_UPB_PRIVATE(fields)[m->WPI_UPB_PRIVATE(field_count)];
    const wpi_upb_MiniTableField* first = &m->WPI_UPB_PRIVATE(fields)[0];
    while (f != first) {
      f--;
      if (encode_shouldencode(e, msg, f)) {
        encode_field(e, msg, m->WPI_UPB_PRIVATE(subs), f);
      }
    }
  }

  *size = (e->limit - e->ptr) - pre_len;
}

static wpi_upb_EncodeStatus wpi_upb_Encoder_Encode(wpi_upb_encstate* const encoder,
                                           const wpi_upb_Message* const msg,
                                           const wpi_upb_MiniTable* const l,
                                           char** const buf, size_t* const size,
                                           bool prepend_len) {
  // Unfortunately we must continue to perform hackery here because there are
  // code paths which blindly copy the returned pointer without bothering to
  // check for errors until much later (b/235839510). So we still set *buf to
  // NULL on error and we still set it to non-NULL on a successful empty result.
  if (WPI_UPB_SETJMP(encoder->err) == 0) {
    size_t encoded_msg_size;
    encode_message(encoder, msg, l, &encoded_msg_size);
    if (prepend_len) {
      encode_varint(encoder, encoded_msg_size);
    }
    *size = encoder->limit - encoder->ptr;
    if (*size == 0) {
      static char ch;
      *buf = &ch;
    } else {
      WPI_UPB_ASSERT(encoder->ptr);
      *buf = encoder->ptr;
    }
  } else {
    WPI_UPB_ASSERT(encoder->status != kUpb_EncodeStatus_Ok);
    *buf = NULL;
    *size = 0;
  }

  _wpi_upb_mapsorter_destroy(&encoder->sorter);
  return encoder->status;
}

static wpi_upb_EncodeStatus _wpi_upb_Encode(const wpi_upb_Message* msg,
                                    const wpi_upb_MiniTable* l, int options,
                                    wpi_upb_Arena* arena, char** buf, size_t* size,
                                    bool prepend_len) {
  wpi_upb_encstate e;
  unsigned depth = (unsigned)options >> 16;

  e.status = kUpb_EncodeStatus_Ok;
  e.arena = arena;
  e.buf = NULL;
  e.limit = NULL;
  e.ptr = NULL;
  e.depth = depth ? depth : kUpb_WireFormat_DefaultDepthLimit;
  e.options = options;
  _wpi_upb_mapsorter_init(&e.sorter);

  return wpi_upb_Encoder_Encode(&e, msg, l, buf, size, prepend_len);
}

wpi_upb_EncodeStatus wpi_upb_Encode(const wpi_upb_Message* msg, const wpi_upb_MiniTable* l,
                            int options, wpi_upb_Arena* arena, char** buf,
                            size_t* size) {
  return _wpi_upb_Encode(msg, l, options, arena, buf, size, false);
}

wpi_upb_EncodeStatus wpi_upb_EncodeLengthPrefixed(const wpi_upb_Message* msg,
                                          const wpi_upb_MiniTable* l, int options,
                                          wpi_upb_Arena* arena, char** buf,
                                          size_t* size) {
  return _wpi_upb_Encode(msg, l, options, arena, buf, size, true);
}

const char* wpi_upb_EncodeStatus_String(wpi_upb_EncodeStatus status) {
  switch (status) {
    case kUpb_EncodeStatus_Ok:
      return "Ok";
    case kUpb_EncodeStatus_MissingRequired:
      return "Missing required field";
    case kUpb_EncodeStatus_MaxDepthExceeded:
      return "Max depth exceeded";
    case kUpb_EncodeStatus_OutOfMemory:
      return "Arena alloc failed";
    default:
      return "Unknown encode status";
  }
}

// Fast decoder: ~3x the speed of decode.c, but requires x86-64/ARM64.
// Also the table size grows by 2x.
//
// Could potentially be ported to other 64-bit archs that pass at least six
// arguments in registers and have 8 unused high bits in pointers.
//
// The overall design is to create specialized functions for every possible
// field type (eg. oneof boolean field with a 1 byte tag) and then dispatch
// to the specialized function as quickly as possible.



// Must be last.

#if WPI_UPB_FASTTABLE

// The standard set of arguments passed to each parsing function.
// Thanks to x86-64 calling conventions, these will stay in registers.
#define WPI_UPB_PARSE_PARAMS                                             \
  wpi_upb_Decoder *d, const char *ptr, wpi_upb_Message *msg, intptr_t table, \
      uint64_t hasbits, uint64_t data

#define WPI_UPB_PARSE_ARGS d, ptr, msg, table, hasbits, data

#define RETURN_GENERIC(m)                                 \
  /* Uncomment either of these for debugging purposes. */ \
  /* fprintf(stderr, m); */                               \
  /*__builtin_trap(); */                                  \
  return _wpi_upb_FastDecoder_DecodeGeneric(d, ptr, msg, table, hasbits, 0);

typedef enum {
  CARD_s = 0, /* Singular (optional, non-repeated) */
  CARD_o = 1, /* Oneof */
  CARD_r = 2, /* Repeated */
  CARD_p = 3  /* Packed Repeated */
} wpi_upb_card;

WPI_UPB_NOINLINE
static const char* fastdecode_isdonefallback(WPI_UPB_PARSE_PARAMS) {
  int overrun = data;
  ptr = _wpi_upb_EpsCopyInputStream_IsDoneFallbackInline(
      &d->input, ptr, overrun, _wpi_upb_Decoder_BufferFlipCallback);
  data = _wpi_upb_FastDecoder_LoadTag(ptr);
  WPI_UPB_MUSTTAIL return _wpi_upb_FastDecoder_TagDispatch(WPI_UPB_PARSE_ARGS);
}

WPI_UPB_FORCEINLINE
const char* fastdecode_dispatch(WPI_UPB_PARSE_PARAMS) {
  int overrun;
  switch (wpi_upb_EpsCopyInputStream_IsDoneStatus(&d->input, ptr, &overrun)) {
    case kUpb_IsDoneStatus_Done:
      ((uint32_t*)msg)[2] |= hasbits;  // Sync hasbits.
      const wpi_upb_MiniTable* m = decode_totablep(table);
      return WPI_UPB_UNLIKELY(m->WPI_UPB_PRIVATE(required_count))
                 ? _wpi_upb_Decoder_CheckRequired(d, ptr, msg, m)
                 : ptr;
    case kUpb_IsDoneStatus_NotDone:
      break;
    case kUpb_IsDoneStatus_NeedFallback:
      data = overrun;
      WPI_UPB_MUSTTAIL return fastdecode_isdonefallback(WPI_UPB_PARSE_ARGS);
  }

  // Read two bytes of tag data (for a one-byte tag, the high byte is junk).
  data = _wpi_upb_FastDecoder_LoadTag(ptr);
  WPI_UPB_MUSTTAIL return _wpi_upb_FastDecoder_TagDispatch(WPI_UPB_PARSE_ARGS);
}

WPI_UPB_FORCEINLINE
bool fastdecode_checktag(uint16_t data, int tagbytes) {
  if (tagbytes == 1) {
    return (data & 0xff) == 0;
  } else {
    return data == 0;
  }
}

WPI_UPB_FORCEINLINE
const char* fastdecode_longsize(const char* ptr, int* size) {
  int i;
  WPI_UPB_ASSERT(*size & 0x80);
  *size &= 0xff;
  for (i = 0; i < 3; i++) {
    ptr++;
    size_t byte = (uint8_t)ptr[-1];
    *size += (byte - 1) << (7 + 7 * i);
    if (WPI_UPB_LIKELY((byte & 0x80) == 0)) return ptr;
  }
  ptr++;
  size_t byte = (uint8_t)ptr[-1];
  // len is limited by 2gb not 4gb, hence 8 and not 16 as normally expected
  // for a 32 bit varint.
  if (WPI_UPB_UNLIKELY(byte >= 8)) return NULL;
  *size += (byte - 1) << 28;
  return ptr;
}

WPI_UPB_FORCEINLINE
const char* fastdecode_delimited(
    wpi_upb_Decoder* d, const char* ptr,
    wpi_upb_EpsCopyInputStream_ParseDelimitedFunc* func, void* ctx) {
  ptr++;

  // Sign-extend so varint greater than one byte becomes negative, causing
  // fast delimited parse to fail.
  int len = (int8_t)ptr[-1];

  if (!wpi_upb_EpsCopyInputStream_TryParseDelimitedFast(&d->input, &ptr, len, func,
                                                    ctx)) {
    // Slow case: Sub-message is >=128 bytes and/or exceeds the current buffer.
    // If it exceeds the buffer limit, limit/limit_ptr will change during
    // sub-message parsing, so we need to preserve delta, not limit.
    if (WPI_UPB_UNLIKELY(len & 0x80)) {
      // Size varint >1 byte (length >= 128).
      ptr = fastdecode_longsize(ptr, &len);
      if (!ptr) {
        // Corrupt wire format: size exceeded INT_MAX.
        return NULL;
      }
    }
    if (!wpi_upb_EpsCopyInputStream_CheckSize(&d->input, ptr, len)) {
      // Corrupt wire format: invalid limit.
      return NULL;
    }
    int delta = wpi_upb_EpsCopyInputStream_PushLimit(&d->input, ptr, len);
    ptr = func(&d->input, ptr, ctx);
    wpi_upb_EpsCopyInputStream_PopLimit(&d->input, ptr, delta);
  }
  return ptr;
}

/* singular, oneof, repeated field handling ***********************************/

typedef struct {
  wpi_upb_Array* arr;
  void* end;
} fastdecode_arr;

typedef enum {
  FD_NEXT_ATLIMIT,
  FD_NEXT_SAMEFIELD,
  FD_NEXT_OTHERFIELD
} fastdecode_next;

typedef struct {
  void* dst;
  fastdecode_next next;
  uint32_t tag;
} fastdecode_nextret;

WPI_UPB_FORCEINLINE
void* fastdecode_resizearr(wpi_upb_Decoder* d, void* dst, fastdecode_arr* farr,
                           int valbytes) {
  if (WPI_UPB_UNLIKELY(dst == farr->end)) {
    size_t old_capacity = farr->arr->WPI_UPB_PRIVATE(capacity);
    size_t old_bytes = old_capacity * valbytes;
    size_t new_capacity = old_capacity * 2;
    size_t new_bytes = new_capacity * valbytes;
    char* old_ptr = wpi_upb_Array_MutableDataPtr(farr->arr);
    char* new_ptr = wpi_upb_Arena_Realloc(&d->arena, old_ptr, old_bytes, new_bytes);
    uint8_t elem_size_lg2 = __builtin_ctz(valbytes);
    WPI_UPB_PRIVATE(_wpi_upb_Array_SetTaggedPtr)(farr->arr, new_ptr, elem_size_lg2);
    farr->arr->WPI_UPB_PRIVATE(capacity) = new_capacity;
    dst = (void*)(new_ptr + (old_capacity * valbytes));
    farr->end = (void*)(new_ptr + (new_capacity * valbytes));
  }
  return dst;
}

WPI_UPB_FORCEINLINE
bool fastdecode_tagmatch(uint32_t tag, uint64_t data, int tagbytes) {
  if (tagbytes == 1) {
    return (uint8_t)tag == (uint8_t)data;
  } else {
    return (uint16_t)tag == (uint16_t)data;
  }
}

WPI_UPB_FORCEINLINE
void fastdecode_commitarr(void* dst, fastdecode_arr* farr, int valbytes) {
  farr->arr->WPI_UPB_PRIVATE(size) =
      (size_t)((char*)dst - (char*)wpi_upb_Array_MutableDataPtr(farr->arr)) /
      valbytes;
}

WPI_UPB_FORCEINLINE
fastdecode_nextret fastdecode_nextrepeated(wpi_upb_Decoder* d, void* dst,
                                           const char** ptr,
                                           fastdecode_arr* farr, uint64_t data,
                                           int tagbytes, int valbytes) {
  fastdecode_nextret ret;
  dst = (char*)dst + valbytes;

  if (WPI_UPB_LIKELY(!_wpi_upb_Decoder_IsDone(d, ptr))) {
    ret.tag = _wpi_upb_FastDecoder_LoadTag(*ptr);
    if (fastdecode_tagmatch(ret.tag, data, tagbytes)) {
      ret.next = FD_NEXT_SAMEFIELD;
    } else {
      fastdecode_commitarr(dst, farr, valbytes);
      ret.next = FD_NEXT_OTHERFIELD;
    }
  } else {
    fastdecode_commitarr(dst, farr, valbytes);
    ret.next = FD_NEXT_ATLIMIT;
  }

  ret.dst = dst;
  return ret;
}

WPI_UPB_FORCEINLINE
void* fastdecode_fieldmem(wpi_upb_Message* msg, uint64_t data) {
  size_t ofs = data >> 48;
  return (char*)msg + ofs;
}

WPI_UPB_FORCEINLINE
void* fastdecode_getfield(wpi_upb_Decoder* d, const char* ptr, wpi_upb_Message* msg,
                          uint64_t* data, uint64_t* hasbits,
                          fastdecode_arr* farr, int valbytes, wpi_upb_card card) {
  WPI_UPB_ASSERT(!wpi_upb_Message_IsFrozen(msg));
  switch (card) {
    case CARD_s: {
      uint8_t hasbit_index = *data >> 24;
      // Set hasbit and return pointer to scalar field.
      *hasbits |= 1ull << hasbit_index;
      return fastdecode_fieldmem(msg, *data);
    }
    case CARD_o: {
      uint16_t case_ofs = *data >> 32;
      uint32_t* oneof_case = WPI_UPB_PTR_AT(msg, case_ofs, uint32_t);
      uint8_t field_number = *data >> 24;
      *oneof_case = field_number;
      return fastdecode_fieldmem(msg, *data);
    }
    case CARD_r: {
      // Get pointer to wpi_upb_Array and allocate/expand if necessary.
      uint8_t elem_size_lg2 = __builtin_ctz(valbytes);
      wpi_upb_Array** arr_p = fastdecode_fieldmem(msg, *data);
      char* begin;
      ((uint32_t*)msg)[2] |= *hasbits;
      *hasbits = 0;
      if (WPI_UPB_LIKELY(!*arr_p)) {
        farr->arr = WPI_UPB_PRIVATE(_wpi_upb_Array_New)(&d->arena, 8, elem_size_lg2);
        *arr_p = farr->arr;
      } else {
        farr->arr = *arr_p;
      }
      begin = wpi_upb_Array_MutableDataPtr(farr->arr);
      farr->end = begin + (farr->arr->WPI_UPB_PRIVATE(capacity) * valbytes);
      *data = _wpi_upb_FastDecoder_LoadTag(ptr);
      return begin + (farr->arr->WPI_UPB_PRIVATE(size) * valbytes);
    }
    default:
      WPI_UPB_UNREACHABLE();
  }
}

WPI_UPB_FORCEINLINE
bool fastdecode_flippacked(uint64_t* data, int tagbytes) {
  *data ^= (0x2 ^ 0x0);  // Patch data to match packed wiretype.
  return fastdecode_checktag(*data, tagbytes);
}

#define FASTDECODE_CHECKPACKED(tagbytes, card, func)                \
  if (WPI_UPB_UNLIKELY(!fastdecode_checktag(data, tagbytes))) {         \
    if (card == CARD_r && fastdecode_flippacked(&data, tagbytes)) { \
      WPI_UPB_MUSTTAIL return func(WPI_UPB_PARSE_ARGS);                     \
    }                                                               \
    RETURN_GENERIC("packed check tag mismatch\n");                  \
  }

/* varint fields **************************************************************/

WPI_UPB_FORCEINLINE
uint64_t fastdecode_munge(uint64_t val, int valbytes, bool zigzag) {
  if (valbytes == 1) {
    return val != 0;
  } else if (zigzag) {
    if (valbytes == 4) {
      uint32_t n = val;
      return (n >> 1) ^ -(int32_t)(n & 1);
    } else if (valbytes == 8) {
      return (val >> 1) ^ -(int64_t)(val & 1);
    }
    WPI_UPB_UNREACHABLE();
  }
  return val;
}

WPI_UPB_FORCEINLINE
const char* fastdecode_varint64(const char* ptr, uint64_t* val) {
  ptr++;
  *val = (uint8_t)ptr[-1];
  if (WPI_UPB_UNLIKELY(*val & 0x80)) {
    int i;
    for (i = 0; i < 8; i++) {
      ptr++;
      uint64_t byte = (uint8_t)ptr[-1];
      *val += (byte - 1) << (7 + 7 * i);
      if (WPI_UPB_LIKELY((byte & 0x80) == 0)) goto done;
    }
    ptr++;
    uint64_t byte = (uint8_t)ptr[-1];
    if (byte > 1) {
      return NULL;
    }
    *val += (byte - 1) << 63;
  }
done:
  WPI_UPB_ASSUME(ptr != NULL);
  return ptr;
}

#define FASTDECODE_UNPACKEDVARINT(d, ptr, msg, table, hasbits, data, tagbytes, \
                                  valbytes, card, zigzag, packed)              \
  uint64_t val;                                                                \
  void* dst;                                                                   \
  fastdecode_arr farr;                                                         \
                                                                               \
  FASTDECODE_CHECKPACKED(tagbytes, card, packed);                              \
                                                                               \
  dst = fastdecode_getfield(d, ptr, msg, &data, &hasbits, &farr, valbytes,     \
                            card);                                             \
  if (card == CARD_r) {                                                        \
    if (WPI_UPB_UNLIKELY(!dst)) {                                                  \
      RETURN_GENERIC("need array resize\n");                                   \
    }                                                                          \
  }                                                                            \
                                                                               \
  again:                                                                       \
  if (card == CARD_r) {                                                        \
    dst = fastdecode_resizearr(d, dst, &farr, valbytes);                       \
  }                                                                            \
                                                                               \
  ptr += tagbytes;                                                             \
  ptr = fastdecode_varint64(ptr, &val);                                        \
  if (ptr == NULL) _wpi_upb_FastDecoder_ErrorJmp(d, kUpb_DecodeStatus_Malformed);  \
  val = fastdecode_munge(val, valbytes, zigzag);                               \
  memcpy(dst, &val, valbytes);                                                 \
                                                                               \
  if (card == CARD_r) {                                                        \
    fastdecode_nextret ret = fastdecode_nextrepeated(                          \
        d, dst, &ptr, &farr, data, tagbytes, valbytes);                        \
    switch (ret.next) {                                                        \
      case FD_NEXT_SAMEFIELD:                                                  \
        dst = ret.dst;                                                         \
        goto again;                                                            \
      case FD_NEXT_OTHERFIELD:                                                 \
        data = ret.tag;                                                        \
        WPI_UPB_MUSTTAIL return _wpi_upb_FastDecoder_TagDispatch(WPI_UPB_PARSE_ARGS);      \
      case FD_NEXT_ATLIMIT:                                                    \
        return ptr;                                                            \
    }                                                                          \
  }                                                                            \
                                                                               \
  WPI_UPB_MUSTTAIL return fastdecode_dispatch(WPI_UPB_PARSE_ARGS);

typedef struct {
  uint8_t valbytes;
  bool zigzag;
  void* dst;
  fastdecode_arr farr;
} fastdecode_varintdata;

WPI_UPB_FORCEINLINE
const char* fastdecode_topackedvarint(wpi_upb_EpsCopyInputStream* e,
                                      const char* ptr, void* ctx) {
  wpi_upb_Decoder* d = (wpi_upb_Decoder*)e;
  fastdecode_varintdata* data = ctx;
  void* dst = data->dst;
  uint64_t val;

  while (!_wpi_upb_Decoder_IsDone(d, &ptr)) {
    dst = fastdecode_resizearr(d, dst, &data->farr, data->valbytes);
    ptr = fastdecode_varint64(ptr, &val);
    if (ptr == NULL) return NULL;
    val = fastdecode_munge(val, data->valbytes, data->zigzag);
    memcpy(dst, &val, data->valbytes);
    dst = (char*)dst + data->valbytes;
  }

  fastdecode_commitarr(dst, &data->farr, data->valbytes);
  return ptr;
}

#define FASTDECODE_PACKEDVARINT(d, ptr, msg, table, hasbits, data, tagbytes, \
                                valbytes, zigzag, unpacked)                  \
  fastdecode_varintdata ctx = {valbytes, zigzag};                            \
                                                                             \
  FASTDECODE_CHECKPACKED(tagbytes, CARD_r, unpacked);                        \
                                                                             \
  ctx.dst = fastdecode_getfield(d, ptr, msg, &data, &hasbits, &ctx.farr,     \
                                valbytes, CARD_r);                           \
  if (WPI_UPB_UNLIKELY(!ctx.dst)) {                                              \
    RETURN_GENERIC("need array resize\n");                                   \
  }                                                                          \
                                                                             \
  ptr += tagbytes;                                                           \
  ptr = fastdecode_delimited(d, ptr, &fastdecode_topackedvarint, &ctx);      \
                                                                             \
  if (WPI_UPB_UNLIKELY(ptr == NULL)) {                                           \
    _wpi_upb_FastDecoder_ErrorJmp(d, kUpb_DecodeStatus_Malformed);               \
  }                                                                          \
                                                                             \
  WPI_UPB_MUSTTAIL return fastdecode_dispatch(d, ptr, msg, table, hasbits, 0);

#define FASTDECODE_VARINT(d, ptr, msg, table, hasbits, data, tagbytes,     \
                          valbytes, card, zigzag, unpacked, packed)        \
  if (card == CARD_p) {                                                    \
    FASTDECODE_PACKEDVARINT(d, ptr, msg, table, hasbits, data, tagbytes,   \
                            valbytes, zigzag, unpacked);                   \
  } else {                                                                 \
    FASTDECODE_UNPACKEDVARINT(d, ptr, msg, table, hasbits, data, tagbytes, \
                              valbytes, card, zigzag, packed);             \
  }

#define z_ZZ true
#define b_ZZ false
#define v_ZZ false

/* Generate all combinations:
 * {s,o,r,p} x {b1,v4,z4,v8,z8} x {1bt,2bt} */

#define F(card, type, valbytes, tagbytes)                                      \
  WPI_UPB_NOINLINE                                                                 \
  const char* wpi_upb_p##card##type##valbytes##_##tagbytes##bt(WPI_UPB_PARSE_PARAMS) { \
    FASTDECODE_VARINT(d, ptr, msg, table, hasbits, data, tagbytes, valbytes,   \
                      CARD_##card, type##_ZZ,                                  \
                      wpi_upb_pr##type##valbytes##_##tagbytes##bt,                 \
                      wpi_upb_pp##type##valbytes##_##tagbytes##bt);                \
  }

#define TYPES(card, tagbytes) \
  F(card, b, 1, tagbytes)     \
  F(card, v, 4, tagbytes)     \
  F(card, v, 8, tagbytes)     \
  F(card, z, 4, tagbytes)     \
  F(card, z, 8, tagbytes)

#define TAGBYTES(card) \
  TYPES(card, 1)       \
  TYPES(card, 2)

TAGBYTES(s)
TAGBYTES(o)
TAGBYTES(r)
TAGBYTES(p)

#undef z_ZZ
#undef b_ZZ
#undef v_ZZ
#undef o_ONEOF
#undef s_ONEOF
#undef r_ONEOF
#undef F
#undef TYPES
#undef TAGBYTES
#undef FASTDECODE_UNPACKEDVARINT
#undef FASTDECODE_PACKEDVARINT
#undef FASTDECODE_VARINT

/* fixed fields ***************************************************************/

#define FASTDECODE_UNPACKEDFIXED(d, ptr, msg, table, hasbits, data, tagbytes, \
                                 valbytes, card, packed)                      \
  void* dst;                                                                  \
  fastdecode_arr farr;                                                        \
                                                                              \
  FASTDECODE_CHECKPACKED(tagbytes, card, packed)                              \
                                                                              \
  dst = fastdecode_getfield(d, ptr, msg, &data, &hasbits, &farr, valbytes,    \
                            card);                                            \
  if (card == CARD_r) {                                                       \
    if (WPI_UPB_UNLIKELY(!dst)) {                                                 \
      RETURN_GENERIC("couldn't allocate array in arena\n");                   \
    }                                                                         \
  }                                                                           \
                                                                              \
  again:                                                                      \
  if (card == CARD_r) {                                                       \
    dst = fastdecode_resizearr(d, dst, &farr, valbytes);                      \
  }                                                                           \
                                                                              \
  ptr += tagbytes;                                                            \
  memcpy(dst, ptr, valbytes);                                                 \
  ptr += valbytes;                                                            \
                                                                              \
  if (card == CARD_r) {                                                       \
    fastdecode_nextret ret = fastdecode_nextrepeated(                         \
        d, dst, &ptr, &farr, data, tagbytes, valbytes);                       \
    switch (ret.next) {                                                       \
      case FD_NEXT_SAMEFIELD:                                                 \
        dst = ret.dst;                                                        \
        goto again;                                                           \
      case FD_NEXT_OTHERFIELD:                                                \
        data = ret.tag;                                                       \
        WPI_UPB_MUSTTAIL return _wpi_upb_FastDecoder_TagDispatch(WPI_UPB_PARSE_ARGS);     \
      case FD_NEXT_ATLIMIT:                                                   \
        return ptr;                                                           \
    }                                                                         \
  }                                                                           \
                                                                              \
  WPI_UPB_MUSTTAIL return fastdecode_dispatch(WPI_UPB_PARSE_ARGS);

#define FASTDECODE_PACKEDFIXED(d, ptr, msg, table, hasbits, data, tagbytes, \
                               valbytes, unpacked)                          \
  FASTDECODE_CHECKPACKED(tagbytes, CARD_r, unpacked)                        \
                                                                            \
  ptr += tagbytes;                                                          \
  int size = (uint8_t)ptr[0];                                               \
  ptr++;                                                                    \
  if (size & 0x80) {                                                        \
    ptr = fastdecode_longsize(ptr, &size);                                  \
  }                                                                         \
                                                                            \
  if (WPI_UPB_UNLIKELY(!wpi_upb_EpsCopyInputStream_CheckDataSizeAvailable(          \
                       &d->input, ptr, size) ||                             \
                   (size % valbytes) != 0)) {                               \
    _wpi_upb_FastDecoder_ErrorJmp(d, kUpb_DecodeStatus_Malformed);              \
  }                                                                         \
                                                                            \
  wpi_upb_Array** arr_p = fastdecode_fieldmem(msg, data);                       \
  wpi_upb_Array* arr = *arr_p;                                                  \
  uint8_t elem_size_lg2 = __builtin_ctz(valbytes);                          \
  int elems = size / valbytes;                                              \
                                                                            \
  if (WPI_UPB_LIKELY(!arr)) {                                                   \
    *arr_p = arr =                                                          \
        WPI_UPB_PRIVATE(_wpi_upb_Array_New)(&d->arena, elems, elem_size_lg2);       \
    if (!arr) {                                                             \
      _wpi_upb_FastDecoder_ErrorJmp(d, kUpb_DecodeStatus_Malformed);            \
    }                                                                       \
  } else {                                                                  \
    WPI_UPB_PRIVATE(_wpi_upb_Array_ResizeUninitialized)(arr, elems, &d->arena);     \
  }                                                                         \
                                                                            \
  char* dst = wpi_upb_Array_MutableDataPtr(arr);                                \
  memcpy(dst, ptr, size);                                                   \
  arr->WPI_UPB_PRIVATE(size) = elems;                                           \
                                                                            \
  ptr += size;                                                              \
  WPI_UPB_MUSTTAIL return fastdecode_dispatch(WPI_UPB_PARSE_ARGS);

#define FASTDECODE_FIXED(d, ptr, msg, table, hasbits, data, tagbytes,     \
                         valbytes, card, unpacked, packed)                \
  if (card == CARD_p) {                                                   \
    FASTDECODE_PACKEDFIXED(d, ptr, msg, table, hasbits, data, tagbytes,   \
                           valbytes, unpacked);                           \
  } else {                                                                \
    FASTDECODE_UNPACKEDFIXED(d, ptr, msg, table, hasbits, data, tagbytes, \
                             valbytes, card, packed);                     \
  }

/* Generate all combinations:
 * {s,o,r,p} x {f4,f8} x {1bt,2bt} */

#define F(card, valbytes, tagbytes)                                         \
  WPI_UPB_NOINLINE                                                              \
  const char* wpi_upb_p##card##f##valbytes##_##tagbytes##bt(WPI_UPB_PARSE_PARAMS) { \
    FASTDECODE_FIXED(d, ptr, msg, table, hasbits, data, tagbytes, valbytes, \
                     CARD_##card, wpi_upb_ppf##valbytes##_##tagbytes##bt,       \
                     wpi_upb_prf##valbytes##_##tagbytes##bt);                   \
  }

#define TYPES(card, tagbytes) \
  F(card, 4, tagbytes)        \
  F(card, 8, tagbytes)

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
#undef FASTDECODE_UNPACKEDFIXED
#undef FASTDECODE_PACKEDFIXED

/* string fields **************************************************************/

typedef const char* fastdecode_copystr_func(struct wpi_upb_Decoder* d,
                                            const char* ptr, wpi_upb_Message* msg,
                                            const wpi_upb_MiniTable* table,
                                            uint64_t hasbits,
                                            wpi_upb_StringView* dst);

WPI_UPB_NOINLINE
static const char* fastdecode_verifyutf8(wpi_upb_Decoder* d, const char* ptr,
                                         wpi_upb_Message* msg, intptr_t table,
                                         uint64_t hasbits, uint64_t data) {
  WPI_UPB_ASSERT(!wpi_upb_Message_IsFrozen(msg));
  wpi_upb_StringView* dst = (wpi_upb_StringView*)data;
  if (!_wpi_upb_Decoder_VerifyUtf8Inline(dst->data, dst->size)) {
    _wpi_upb_FastDecoder_ErrorJmp(d, kUpb_DecodeStatus_BadUtf8);
  }
  WPI_UPB_MUSTTAIL return fastdecode_dispatch(WPI_UPB_PARSE_ARGS);
}

#define FASTDECODE_LONGSTRING(d, ptr, msg, table, hasbits, dst, validate_utf8) \
  int size = (uint8_t)ptr[0]; /* Could plumb through hasbits. */               \
  ptr++;                                                                       \
  if (size & 0x80) {                                                           \
    ptr = fastdecode_longsize(ptr, &size);                                     \
  }                                                                            \
                                                                               \
  if (WPI_UPB_UNLIKELY(!wpi_upb_EpsCopyInputStream_CheckSize(&d->input, ptr, size))) { \
    dst->size = 0;                                                             \
    _wpi_upb_FastDecoder_ErrorJmp(d, kUpb_DecodeStatus_Malformed);                 \
  }                                                                            \
                                                                               \
  const char* s_ptr = ptr;                                                     \
  ptr = wpi_upb_EpsCopyInputStream_ReadString(&d->input, &s_ptr, size, &d->arena); \
  if (!ptr) _wpi_upb_FastDecoder_ErrorJmp(d, kUpb_DecodeStatus_OutOfMemory);       \
  dst->data = s_ptr;                                                           \
  dst->size = size;                                                            \
                                                                               \
  if (validate_utf8) {                                                         \
    data = (uint64_t)dst;                                                      \
    WPI_UPB_MUSTTAIL return fastdecode_verifyutf8(WPI_UPB_PARSE_ARGS);                 \
  } else {                                                                     \
    WPI_UPB_MUSTTAIL return fastdecode_dispatch(WPI_UPB_PARSE_ARGS);                   \
  }

WPI_UPB_NOINLINE
static const char* fastdecode_longstring_utf8(struct wpi_upb_Decoder* d,
                                              const char* ptr, wpi_upb_Message* msg,
                                              intptr_t table, uint64_t hasbits,
                                              uint64_t data) {
  wpi_upb_StringView* dst = (wpi_upb_StringView*)data;
  FASTDECODE_LONGSTRING(d, ptr, msg, table, hasbits, dst, true);
}

WPI_UPB_NOINLINE
static const char* fastdecode_longstring_noutf8(
    struct wpi_upb_Decoder* d, const char* ptr, wpi_upb_Message* msg, intptr_t table,
    uint64_t hasbits, uint64_t data) {
  WPI_UPB_ASSERT(!wpi_upb_Message_IsFrozen(msg));
  wpi_upb_StringView* dst = (wpi_upb_StringView*)data;
  FASTDECODE_LONGSTRING(d, ptr, msg, table, hasbits, dst, false);
}

WPI_UPB_FORCEINLINE
void fastdecode_docopy(wpi_upb_Decoder* d, const char* ptr, uint32_t size, int copy,
                       char* data, size_t data_offset, wpi_upb_StringView* dst) {
  d->arena.WPI_UPB_PRIVATE(ptr) += copy;
  dst->data = data + data_offset;
  WPI_UPB_UNPOISON_MEMORY_REGION(data, copy);
  memcpy(data, ptr, copy);
  WPI_UPB_POISON_MEMORY_REGION(data + data_offset + size,
                           copy - data_offset - size);
}

#define FASTDECODE_COPYSTRING(d, ptr, msg, table, hasbits, data, tagbytes,     \
                              card, validate_utf8)                             \
  wpi_upb_StringView* dst;                                                         \
  fastdecode_arr farr;                                                         \
  int64_t size;                                                                \
  size_t arena_has;                                                            \
  size_t common_has;                                                           \
  char* buf;                                                                   \
                                                                               \
  WPI_UPB_ASSERT(!wpi_upb_EpsCopyInputStream_AliasingAvailable(&d->input, ptr, 0));    \
  WPI_UPB_ASSERT(fastdecode_checktag(data, tagbytes));                             \
                                                                               \
  dst = fastdecode_getfield(d, ptr, msg, &data, &hasbits, &farr,               \
                            sizeof(wpi_upb_StringView), card);                     \
                                                                               \
  again:                                                                       \
  if (card == CARD_r) {                                                        \
    dst = fastdecode_resizearr(d, dst, &farr, sizeof(wpi_upb_StringView));         \
  }                                                                            \
                                                                               \
  size = (uint8_t)ptr[tagbytes];                                               \
  ptr += tagbytes + 1;                                                         \
  dst->size = size;                                                            \
                                                                               \
  buf = d->arena.WPI_UPB_PRIVATE(ptr);                                             \
  arena_has = WPI_UPB_PRIVATE(_wpi_upb_ArenaHas)(&d->arena);                           \
  common_has = WPI_UPB_MIN(arena_has,                                              \
                       wpi_upb_EpsCopyInputStream_BytesAvailable(&d->input, ptr)); \
                                                                               \
  if (WPI_UPB_LIKELY(size <= 15 - tagbytes)) {                                     \
    if (arena_has < 16) goto longstr;                                          \
    fastdecode_docopy(d, ptr - tagbytes - 1, size, 16, buf, tagbytes + 1,      \
                      dst);                                                    \
  } else if (WPI_UPB_LIKELY(size <= 32)) {                                         \
    if (WPI_UPB_UNLIKELY(common_has < 32)) goto longstr;                           \
    fastdecode_docopy(d, ptr, size, 32, buf, 0, dst);                          \
  } else if (WPI_UPB_LIKELY(size <= 64)) {                                         \
    if (WPI_UPB_UNLIKELY(common_has < 64)) goto longstr;                           \
    fastdecode_docopy(d, ptr, size, 64, buf, 0, dst);                          \
  } else if (WPI_UPB_LIKELY(size < 128)) {                                         \
    if (WPI_UPB_UNLIKELY(common_has < 128)) goto longstr;                          \
    fastdecode_docopy(d, ptr, size, 128, buf, 0, dst);                         \
  } else {                                                                     \
    goto longstr;                                                              \
  }                                                                            \
                                                                               \
  ptr += size;                                                                 \
                                                                               \
  if (card == CARD_r) {                                                        \
    if (validate_utf8 &&                                                       \
        !_wpi_upb_Decoder_VerifyUtf8Inline(dst->data, dst->size)) {                \
      _wpi_upb_FastDecoder_ErrorJmp(d, kUpb_DecodeStatus_BadUtf8);                 \
    }                                                                          \
    fastdecode_nextret ret = fastdecode_nextrepeated(                          \
        d, dst, &ptr, &farr, data, tagbytes, sizeof(wpi_upb_StringView));          \
    switch (ret.next) {                                                        \
      case FD_NEXT_SAMEFIELD:                                                  \
        dst = ret.dst;                                                         \
        goto again;                                                            \
      case FD_NEXT_OTHERFIELD:                                                 \
        data = ret.tag;                                                        \
        WPI_UPB_MUSTTAIL return _wpi_upb_FastDecoder_TagDispatch(WPI_UPB_PARSE_ARGS);      \
      case FD_NEXT_ATLIMIT:                                                    \
        return ptr;                                                            \
    }                                                                          \
  }                                                                            \
                                                                               \
  if (card != CARD_r && validate_utf8) {                                       \
    data = (uint64_t)dst;                                                      \
    WPI_UPB_MUSTTAIL return fastdecode_verifyutf8(WPI_UPB_PARSE_ARGS);                 \
  }                                                                            \
                                                                               \
  WPI_UPB_MUSTTAIL return fastdecode_dispatch(WPI_UPB_PARSE_ARGS);                     \
                                                                               \
  longstr:                                                                     \
  if (card == CARD_r) {                                                        \
    fastdecode_commitarr(dst + 1, &farr, sizeof(wpi_upb_StringView));              \
  }                                                                            \
  ptr--;                                                                       \
  if (validate_utf8) {                                                         \
    WPI_UPB_MUSTTAIL return fastdecode_longstring_utf8(d, ptr, msg, table,         \
                                                   hasbits, (uint64_t)dst);    \
  } else {                                                                     \
    WPI_UPB_MUSTTAIL return fastdecode_longstring_noutf8(d, ptr, msg, table,       \
                                                     hasbits, (uint64_t)dst);  \
  }

#define FASTDECODE_STRING(d, ptr, msg, table, hasbits, data, tagbytes, card,  \
                          copyfunc, validate_utf8)                            \
  wpi_upb_StringView* dst;                                                        \
  fastdecode_arr farr;                                                        \
  int64_t size;                                                               \
                                                                              \
  if (WPI_UPB_UNLIKELY(!fastdecode_checktag(data, tagbytes))) {                   \
    RETURN_GENERIC("string field tag mismatch\n");                            \
  }                                                                           \
                                                                              \
  if (WPI_UPB_UNLIKELY(                                                           \
          !wpi_upb_EpsCopyInputStream_AliasingAvailable(&d->input, ptr, 0))) {    \
    WPI_UPB_MUSTTAIL return copyfunc(WPI_UPB_PARSE_ARGS);                             \
  }                                                                           \
                                                                              \
  dst = fastdecode_getfield(d, ptr, msg, &data, &hasbits, &farr,              \
                            sizeof(wpi_upb_StringView), card);                    \
                                                                              \
  again:                                                                      \
  if (card == CARD_r) {                                                       \
    dst = fastdecode_resizearr(d, dst, &farr, sizeof(wpi_upb_StringView));        \
  }                                                                           \
                                                                              \
  size = (int8_t)ptr[tagbytes];                                               \
  ptr += tagbytes + 1;                                                        \
                                                                              \
  if (WPI_UPB_UNLIKELY(                                                           \
          !wpi_upb_EpsCopyInputStream_AliasingAvailable(&d->input, ptr, size))) { \
    ptr--;                                                                    \
    if (validate_utf8) {                                                      \
      return fastdecode_longstring_utf8(d, ptr, msg, table, hasbits,          \
                                        (uint64_t)dst);                       \
    } else {                                                                  \
      return fastdecode_longstring_noutf8(d, ptr, msg, table, hasbits,        \
                                          (uint64_t)dst);                     \
    }                                                                         \
  }                                                                           \
                                                                              \
  dst->data = ptr;                                                            \
  dst->size = size;                                                           \
  ptr = wpi_upb_EpsCopyInputStream_ReadStringAliased(&d->input, &dst->data,       \
                                                 dst->size);                  \
                                                                              \
  if (card == CARD_r) {                                                       \
    if (validate_utf8 &&                                                      \
        !_wpi_upb_Decoder_VerifyUtf8Inline(dst->data, dst->size)) {               \
      _wpi_upb_FastDecoder_ErrorJmp(d, kUpb_DecodeStatus_BadUtf8);                \
    }                                                                         \
    fastdecode_nextret ret = fastdecode_nextrepeated(                         \
        d, dst, &ptr, &farr, data, tagbytes, sizeof(wpi_upb_StringView));         \
    switch (ret.next) {                                                       \
      case FD_NEXT_SAMEFIELD:                                                 \
        dst = ret.dst;                                                        \
        goto again;                                                           \
      case FD_NEXT_OTHERFIELD:                                                \
        data = ret.tag;                                                       \
        WPI_UPB_MUSTTAIL return _wpi_upb_FastDecoder_TagDispatch(WPI_UPB_PARSE_ARGS);     \
      case FD_NEXT_ATLIMIT:                                                   \
        return ptr;                                                           \
    }                                                                         \
  }                                                                           \
                                                                              \
  if (card != CARD_r && validate_utf8) {                                      \
    data = (uint64_t)dst;                                                     \
    WPI_UPB_MUSTTAIL return fastdecode_verifyutf8(WPI_UPB_PARSE_ARGS);                \
  }                                                                           \
                                                                              \
  WPI_UPB_MUSTTAIL return fastdecode_dispatch(WPI_UPB_PARSE_ARGS);

/* Generate all combinations:
 * {p,c} x {s,o,r} x {s, b} x {1bt,2bt} */

#define s_VALIDATE true
#define b_VALIDATE false

#define F(card, tagbytes, type)                                        \
  WPI_UPB_NOINLINE                                                         \
  const char* wpi_upb_c##card##type##_##tagbytes##bt(WPI_UPB_PARSE_PARAMS) {   \
    FASTDECODE_COPYSTRING(d, ptr, msg, table, hasbits, data, tagbytes, \
                          CARD_##card, type##_VALIDATE);               \
  }                                                                    \
  const char* wpi_upb_p##card##type##_##tagbytes##bt(WPI_UPB_PARSE_PARAMS) {   \
    FASTDECODE_STRING(d, ptr, msg, table, hasbits, data, tagbytes,     \
                      CARD_##card, wpi_upb_c##card##type##_##tagbytes##bt, \
                      type##_VALIDATE);                                \
  }

#define UTF8(card, tagbytes) \
  F(card, tagbytes, s)       \
  F(card, tagbytes, b)

#define TAGBYTES(card) \
  UTF8(card, 1)        \
  UTF8(card, 2)

TAGBYTES(s)
TAGBYTES(o)
TAGBYTES(r)

#undef s_VALIDATE
#undef b_VALIDATE
#undef F
#undef TAGBYTES
#undef FASTDECODE_LONGSTRING
#undef FASTDECODE_COPYSTRING
#undef FASTDECODE_STRING

/* message fields *************************************************************/

WPI_UPB_INLINE
wpi_upb_Message* decode_newmsg_ceil(wpi_upb_Decoder* d, const wpi_upb_MiniTable* m,
                                int msg_ceil_bytes) {
  size_t size = m->WPI_UPB_PRIVATE(size);
  char* msg_data;
  if (WPI_UPB_LIKELY(msg_ceil_bytes > 0 &&
                 WPI_UPB_PRIVATE(_wpi_upb_ArenaHas)(&d->arena) >= msg_ceil_bytes)) {
    WPI_UPB_ASSERT(size <= (size_t)msg_ceil_bytes);
    msg_data = d->arena.WPI_UPB_PRIVATE(ptr);
    d->arena.WPI_UPB_PRIVATE(ptr) += size;
    WPI_UPB_UNPOISON_MEMORY_REGION(msg_data, msg_ceil_bytes);
    memset(msg_data, 0, msg_ceil_bytes);
    WPI_UPB_POISON_MEMORY_REGION(msg_data + size, msg_ceil_bytes - size);
  } else {
    msg_data = (char*)wpi_upb_Arena_Malloc(&d->arena, size);
    memset(msg_data, 0, size);
  }
  return (wpi_upb_Message*)msg_data;
}

typedef struct {
  intptr_t table;
  wpi_upb_Message* msg;
} fastdecode_submsgdata;

WPI_UPB_FORCEINLINE
const char* fastdecode_tosubmsg(wpi_upb_EpsCopyInputStream* e, const char* ptr,
                                void* ctx) {
  wpi_upb_Decoder* d = (wpi_upb_Decoder*)e;
  fastdecode_submsgdata* submsg = ctx;
  ptr = fastdecode_dispatch(d, ptr, submsg->msg, submsg->table, 0, 0);
  WPI_UPB_ASSUME(ptr != NULL);
  return ptr;
}

#define FASTDECODE_SUBMSG(d, ptr, msg, table, hasbits, data, tagbytes,    \
                          msg_ceil_bytes, card)                           \
                                                                          \
  if (WPI_UPB_UNLIKELY(!fastdecode_checktag(data, tagbytes))) {               \
    RETURN_GENERIC("submessage field tag mismatch\n");                    \
  }                                                                       \
                                                                          \
  if (--d->depth == 0) {                                                  \
    _wpi_upb_FastDecoder_ErrorJmp(d, kUpb_DecodeStatus_MaxDepthExceeded);     \
  }                                                                       \
                                                                          \
  wpi_upb_Message** dst;                                                      \
  uint32_t submsg_idx = (data >> 16) & 0xff;                              \
  const wpi_upb_MiniTable* tablep = decode_totablep(table);                   \
  const wpi_upb_MiniTable* subtablep = wpi_upb_MiniTableSub_Message(              \
      *WPI_UPB_PRIVATE(_wpi_upb_MiniTable_GetSubByIndex)(tablep, submsg_idx));    \
  fastdecode_submsgdata submsg = {decode_totable(subtablep)};             \
  fastdecode_arr farr;                                                    \
                                                                          \
  if (subtablep->WPI_UPB_PRIVATE(table_mask) == (uint8_t)-1) {                \
    d->depth++;                                                           \
    RETURN_GENERIC("submessage doesn't have fast tables.");               \
  }                                                                       \
                                                                          \
  dst = fastdecode_getfield(d, ptr, msg, &data, &hasbits, &farr,          \
                            sizeof(wpi_upb_Message*), card);                  \
                                                                          \
  if (card == CARD_s) {                                                   \
    ((uint32_t*)msg)[2] |= hasbits;                                       \
    hasbits = 0;                                                          \
  }                                                                       \
                                                                          \
  again:                                                                  \
  if (card == CARD_r) {                                                   \
    dst = fastdecode_resizearr(d, dst, &farr, sizeof(wpi_upb_Message*));      \
  }                                                                       \
                                                                          \
  submsg.msg = *dst;                                                      \
                                                                          \
  if (card == CARD_r || WPI_UPB_LIKELY(!submsg.msg)) {                        \
    *dst = submsg.msg = decode_newmsg_ceil(d, subtablep, msg_ceil_bytes); \
  }                                                                       \
                                                                          \
  ptr += tagbytes;                                                        \
  ptr = fastdecode_delimited(d, ptr, fastdecode_tosubmsg, &submsg);       \
                                                                          \
  if (WPI_UPB_UNLIKELY(ptr == NULL || d->end_group != DECODE_NOGROUP)) {      \
    _wpi_upb_FastDecoder_ErrorJmp(d, kUpb_DecodeStatus_Malformed);            \
  }                                                                       \
                                                                          \
  if (card == CARD_r) {                                                   \
    fastdecode_nextret ret = fastdecode_nextrepeated(                     \
        d, dst, &ptr, &farr, data, tagbytes, sizeof(wpi_upb_Message*));       \
    switch (ret.next) {                                                   \
      case FD_NEXT_SAMEFIELD:                                             \
        dst = ret.dst;                                                    \
        goto again;                                                       \
      case FD_NEXT_OTHERFIELD:                                            \
        d->depth++;                                                       \
        data = ret.tag;                                                   \
        WPI_UPB_MUSTTAIL return _wpi_upb_FastDecoder_TagDispatch(WPI_UPB_PARSE_ARGS); \
      case FD_NEXT_ATLIMIT:                                               \
        d->depth++;                                                       \
        return ptr;                                                       \
    }                                                                     \
  }                                                                       \
                                                                          \
  d->depth++;                                                             \
  WPI_UPB_MUSTTAIL return fastdecode_dispatch(WPI_UPB_PARSE_ARGS);

#define F(card, tagbytes, size_ceil, ceil_arg)                               \
  const char* wpi_upb_p##card##m_##tagbytes##bt_max##size_ceil##b(               \
      WPI_UPB_PARSE_PARAMS) {                                                    \
    FASTDECODE_SUBMSG(d, ptr, msg, table, hasbits, data, tagbytes, ceil_arg, \
                      CARD_##card);                                          \
  }

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

#undef TAGBYTES
#undef SIZES
#undef F
#undef FASTDECODE_SUBMSG

#endif /* WPI_UPB_FASTTABLE */


#include <stddef.h>
#include <stdint.h>


// Must be last.

WPI_UPB_NOINLINE WPI_UPB_PRIVATE(_wpi_upb_WireReader_LongVarint)
    WPI_UPB_PRIVATE(_wpi_upb_WireReader_ReadLongVarint)(const char* ptr, uint64_t val) {
  WPI_UPB_PRIVATE(_wpi_upb_WireReader_LongVarint) ret = {NULL, 0};
  uint64_t byte;
  for (int i = 1; i < 10; i++) {
    byte = (uint8_t)ptr[i];
    val += (byte - 1) << (i * 7);
    if (!(byte & 0x80)) {
      ret.ptr = ptr + i + 1;
      ret.val = val;
      return ret;
    }
  }
  return ret;
}

const char* WPI_UPB_PRIVATE(_wpi_upb_WireReader_SkipGroup)(
    const char* ptr, uint32_t tag, int depth_limit,
    wpi_upb_EpsCopyInputStream* stream) {
  if (--depth_limit == 0) return NULL;
  uint32_t end_group_tag = (tag & ~7ULL) | kUpb_WireType_EndGroup;
  while (!wpi_upb_EpsCopyInputStream_IsDone(stream, &ptr)) {
    uint32_t tag;
    ptr = wpi_upb_WireReader_ReadTag(ptr, &tag);
    if (!ptr) return NULL;
    if (tag == end_group_tag) return ptr;
    ptr = _wpi_upb_WireReader_SkipValue(ptr, tag, depth_limit, stream);
    if (!ptr) return NULL;
  }
  return ptr;
}

/*
 * wpi_upb_table Implementation
 *
 * Implementation is heavily inspired by Lua's ltable.c.
 */

#include <string.h>


// Must be last.

#define WPI_UPB_MAXARRSIZE 16  // 2**16 = 64k.

// From Chromium.
#define ARRAY_SIZE(x) \
  ((sizeof(x) / sizeof(0 [x])) / ((size_t)(!(sizeof(x) % sizeof(0 [x])))))

static const double MAX_LOAD = 0.85;

/* The minimum utilization of the array part of a mixed hash/array table.  This
 * is a speed/memory-usage tradeoff (though it's not straightforward because of
 * cache effects).  The lower this is, the more memory we'll use. */
static const double MIN_DENSITY = 0.1;

static bool is_pow2(uint64_t v) { return v == 0 || (v & (v - 1)) == 0; }

static wpi_upb_value _wpi_upb_value_val(uint64_t val) {
  wpi_upb_value ret;
  _wpi_upb_value_setval(&ret, val);
  return ret;
}

static int log2ceil(uint64_t v) {
  int ret = 0;
  bool pow2 = is_pow2(v);
  while (v >>= 1) ret++;
  ret = pow2 ? ret : ret + 1;  // Ceiling.
  return WPI_UPB_MIN(WPI_UPB_MAXARRSIZE, ret);
}

/* A type to represent the lookup key of either a strtable or an inttable. */
typedef union {
  uintptr_t num;
  struct {
    const char* str;
    size_t len;
  } str;
} lookupkey_t;

static lookupkey_t strkey2(const char* str, size_t len) {
  lookupkey_t k;
  k.str.str = str;
  k.str.len = len;
  return k;
}

static lookupkey_t intkey(uintptr_t key) {
  lookupkey_t k;
  k.num = key;
  return k;
}

typedef uint32_t hashfunc_t(wpi_upb_tabkey key);
typedef bool eqlfunc_t(wpi_upb_tabkey k1, lookupkey_t k2);

/* Base table (shared code) ***************************************************/

static uint32_t wpi_upb_inthash(uintptr_t key) { return (uint32_t)key; }

static const wpi_upb_tabent* wpi_upb_getentry(const wpi_upb_table* t, uint32_t hash) {
  return t->entries + (hash & t->mask);
}

static bool wpi_upb_arrhas(wpi_upb_tabval key) { return key.val != (uint64_t)-1; }

static bool isfull(wpi_upb_table* t) { return t->count == t->max_count; }

static bool init(wpi_upb_table* t, uint8_t size_lg2, wpi_upb_Arena* a) {
  size_t bytes;

  t->count = 0;
  t->size_lg2 = size_lg2;
  t->mask = wpi_upb_table_size(t) ? wpi_upb_table_size(t) - 1 : 0;
  t->max_count = wpi_upb_table_size(t) * MAX_LOAD;
  bytes = wpi_upb_table_size(t) * sizeof(wpi_upb_tabent);
  if (bytes > 0) {
    t->entries = wpi_upb_Arena_Malloc(a, bytes);
    if (!t->entries) return false;
    memset(t->entries, 0, bytes);
  } else {
    t->entries = NULL;
  }
  return true;
}

static wpi_upb_tabent* emptyent(wpi_upb_table* t, wpi_upb_tabent* e) {
  wpi_upb_tabent* begin = t->entries;
  wpi_upb_tabent* end = begin + wpi_upb_table_size(t);
  for (e = e + 1; e < end; e++) {
    if (wpi_upb_tabent_isempty(e)) return e;
  }
  for (e = begin; e < end; e++) {
    if (wpi_upb_tabent_isempty(e)) return e;
  }
  WPI_UPB_ASSERT(false);
  return NULL;
}

static wpi_upb_tabent* getentry_mutable(wpi_upb_table* t, uint32_t hash) {
  return (wpi_upb_tabent*)wpi_upb_getentry(t, hash);
}

static const wpi_upb_tabent* findentry(const wpi_upb_table* t, lookupkey_t key,
                                   uint32_t hash, eqlfunc_t* eql) {
  const wpi_upb_tabent* e;

  if (t->size_lg2 == 0) return NULL;
  e = wpi_upb_getentry(t, hash);
  if (wpi_upb_tabent_isempty(e)) return NULL;
  while (1) {
    if (eql(e->key, key)) return e;
    if ((e = e->next) == NULL) return NULL;
  }
}

static wpi_upb_tabent* findentry_mutable(wpi_upb_table* t, lookupkey_t key,
                                     uint32_t hash, eqlfunc_t* eql) {
  return (wpi_upb_tabent*)findentry(t, key, hash, eql);
}

static bool lookup(const wpi_upb_table* t, lookupkey_t key, wpi_upb_value* v,
                   uint32_t hash, eqlfunc_t* eql) {
  const wpi_upb_tabent* e = findentry(t, key, hash, eql);
  if (e) {
    if (v) {
      _wpi_upb_value_setval(v, e->val.val);
    }
    return true;
  } else {
    return false;
  }
}

/* The given key must not already exist in the table. */
static void insert(wpi_upb_table* t, lookupkey_t key, wpi_upb_tabkey tabkey,
                   wpi_upb_value val, uint32_t hash, hashfunc_t* hashfunc,
                   eqlfunc_t* eql) {
  wpi_upb_tabent* mainpos_e;
  wpi_upb_tabent* our_e;

  WPI_UPB_ASSERT(findentry(t, key, hash, eql) == NULL);

  t->count++;
  mainpos_e = getentry_mutable(t, hash);
  our_e = mainpos_e;

  if (wpi_upb_tabent_isempty(mainpos_e)) {
    /* Our main position is empty; use it. */
    our_e->next = NULL;
  } else {
    /* Collision. */
    wpi_upb_tabent* new_e = emptyent(t, mainpos_e);
    /* Head of collider's chain. */
    wpi_upb_tabent* chain = getentry_mutable(t, hashfunc(mainpos_e->key));
    if (chain == mainpos_e) {
      /* Existing ent is in its main position (it has the same hash as us, and
       * is the head of our chain).  Insert to new ent and append to this chain.
       */
      new_e->next = mainpos_e->next;
      mainpos_e->next = new_e;
      our_e = new_e;
    } else {
      /* Existing ent is not in its main position (it is a node in some other
       * chain).  This implies that no existing ent in the table has our hash.
       * Evict it (updating its chain) and use its ent for head of our chain. */
      *new_e = *mainpos_e; /* copies next. */
      while (chain->next != mainpos_e) {
        chain = (wpi_upb_tabent*)chain->next;
        WPI_UPB_ASSERT(chain);
      }
      chain->next = new_e;
      our_e = mainpos_e;
      our_e->next = NULL;
    }
  }
  our_e->key = tabkey;
  our_e->val.val = val.val;
  WPI_UPB_ASSERT(findentry(t, key, hash, eql) == our_e);
}

static bool rm(wpi_upb_table* t, lookupkey_t key, wpi_upb_value* val,
               wpi_upb_tabkey* removed, uint32_t hash, eqlfunc_t* eql) {
  wpi_upb_tabent* chain = getentry_mutable(t, hash);
  if (wpi_upb_tabent_isempty(chain)) return false;
  if (eql(chain->key, key)) {
    /* Element to remove is at the head of its chain. */
    t->count--;
    if (val) _wpi_upb_value_setval(val, chain->val.val);
    if (removed) *removed = chain->key;
    if (chain->next) {
      wpi_upb_tabent* move = (wpi_upb_tabent*)chain->next;
      *chain = *move;
      move->key = 0; /* Make the slot empty. */
    } else {
      chain->key = 0; /* Make the slot empty. */
    }
    return true;
  } else {
    /* Element to remove is either in a non-head position or not in the
     * table. */
    while (chain->next && !eql(chain->next->key, key)) {
      chain = (wpi_upb_tabent*)chain->next;
    }
    if (chain->next) {
      /* Found element to remove. */
      wpi_upb_tabent* rm = (wpi_upb_tabent*)chain->next;
      t->count--;
      if (val) _wpi_upb_value_setval(val, chain->next->val.val);
      if (removed) *removed = rm->key;
      rm->key = 0; /* Make the slot empty. */
      chain->next = rm->next;
      return true;
    } else {
      /* Element to remove is not in the table. */
      return false;
    }
  }
}

static size_t next(const wpi_upb_table* t, size_t i) {
  do {
    if (++i >= wpi_upb_table_size(t)) return SIZE_MAX - 1; /* Distinct from -1. */
  } while (wpi_upb_tabent_isempty(&t->entries[i]));

  return i;
}

static size_t begin(const wpi_upb_table* t) { return next(t, -1); }

/* wpi_upb_strtable ***************************************************************/

/* A simple "subclass" of wpi_upb_table that only adds a hash function for strings.
 */

static wpi_upb_tabkey strcopy(lookupkey_t k2, wpi_upb_Arena* a) {
  uint32_t len = (uint32_t)k2.str.len;
  char* str = wpi_upb_Arena_Malloc(a, k2.str.len + sizeof(uint32_t) + 1);
  if (str == NULL) return 0;
  memcpy(str, &len, sizeof(uint32_t));
  if (k2.str.len) memcpy(str + sizeof(uint32_t), k2.str.str, k2.str.len);
  str[sizeof(uint32_t) + k2.str.len] = '\0';
  return (uintptr_t)str;
}

/* Adapted from ABSL's wyhash. */

static uint64_t UnalignedLoad64(const void* p) {
  uint64_t val;
  memcpy(&val, p, 8);
  return val;
}

static uint32_t UnalignedLoad32(const void* p) {
  uint32_t val;
  memcpy(&val, p, 4);
  return val;
}

#if defined(_MSC_VER) && defined(_M_X64)
#include <intrin.h>
#endif

/* Computes a * b, returning the low 64 bits of the result and storing the high
 * 64 bits in |*high|. */
static uint64_t wpi_upb_umul128(uint64_t v0, uint64_t v1, uint64_t* out_high) {
#ifdef __SIZEOF_INT128__
  __uint128_t p = v0;
  p *= v1;
  *out_high = (uint64_t)(p >> 64);
  return (uint64_t)p;
#elif defined(_MSC_VER) && defined(_M_X64)
  return _umul128(v0, v1, out_high);
#else
  uint64_t a32 = v0 >> 32;
  uint64_t a00 = v0 & 0xffffffff;
  uint64_t b32 = v1 >> 32;
  uint64_t b00 = v1 & 0xffffffff;
  uint64_t high = a32 * b32;
  uint64_t low = a00 * b00;
  uint64_t mid1 = a32 * b00;
  uint64_t mid2 = a00 * b32;
  low += (mid1 << 32) + (mid2 << 32);
  // Omit carry bit, for mixing we do not care about exact numerical precision.
  high += (mid1 >> 32) + (mid2 >> 32);
  *out_high = high;
  return low;
#endif
}

static uint64_t WyhashMix(uint64_t v0, uint64_t v1) {
  uint64_t high;
  uint64_t low = wpi_upb_umul128(v0, v1, &high);
  return low ^ high;
}

static uint64_t Wyhash(const void* data, size_t len, uint64_t seed,
                       const uint64_t salt[]) {
  const uint8_t* ptr = (const uint8_t*)data;
  uint64_t starting_length = (uint64_t)len;
  uint64_t current_state = seed ^ salt[0];

  if (len > 64) {
    // If we have more than 64 bytes, we're going to handle chunks of 64
    // bytes at a time. We're going to build up two separate hash states
    // which we will then hash together.
    uint64_t duplicated_state = current_state;

    do {
      uint64_t a = UnalignedLoad64(ptr);
      uint64_t b = UnalignedLoad64(ptr + 8);
      uint64_t c = UnalignedLoad64(ptr + 16);
      uint64_t d = UnalignedLoad64(ptr + 24);
      uint64_t e = UnalignedLoad64(ptr + 32);
      uint64_t f = UnalignedLoad64(ptr + 40);
      uint64_t g = UnalignedLoad64(ptr + 48);
      uint64_t h = UnalignedLoad64(ptr + 56);

      uint64_t cs0 = WyhashMix(a ^ salt[1], b ^ current_state);
      uint64_t cs1 = WyhashMix(c ^ salt[2], d ^ current_state);
      current_state = (cs0 ^ cs1);

      uint64_t ds0 = WyhashMix(e ^ salt[3], f ^ duplicated_state);
      uint64_t ds1 = WyhashMix(g ^ salt[4], h ^ duplicated_state);
      duplicated_state = (ds0 ^ ds1);

      ptr += 64;
      len -= 64;
    } while (len > 64);

    current_state = current_state ^ duplicated_state;
  }

  // We now have a data `ptr` with at most 64 bytes and the current state
  // of the hashing state machine stored in current_state.
  while (len > 16) {
    uint64_t a = UnalignedLoad64(ptr);
    uint64_t b = UnalignedLoad64(ptr + 8);

    current_state = WyhashMix(a ^ salt[1], b ^ current_state);

    ptr += 16;
    len -= 16;
  }

  // We now have a data `ptr` with at most 16 bytes.
  uint64_t a = 0;
  uint64_t b = 0;
  if (len > 8) {
    // When we have at least 9 and at most 16 bytes, set A to the first 64
    // bits of the input and B to the last 64 bits of the input. Yes, they will
    // overlap in the middle if we are working with less than the full 16
    // bytes.
    a = UnalignedLoad64(ptr);
    b = UnalignedLoad64(ptr + len - 8);
  } else if (len > 3) {
    // If we have at least 4 and at most 8 bytes, set A to the first 32
    // bits and B to the last 32 bits.
    a = UnalignedLoad32(ptr);
    b = UnalignedLoad32(ptr + len - 4);
  } else if (len > 0) {
    // If we have at least 1 and at most 3 bytes, read all of the provided
    // bits into A, with some adjustments.
    a = ((ptr[0] << 16) | (ptr[len >> 1] << 8) | ptr[len - 1]);
    b = 0;
  } else {
    a = 0;
    b = 0;
  }

  uint64_t w = WyhashMix(a ^ salt[1], b ^ current_state);
  uint64_t z = salt[1] ^ starting_length;
  return WyhashMix(w, z);
}

const uint64_t kWyhashSalt[5] = {
    0x243F6A8885A308D3ULL, 0x13198A2E03707344ULL, 0xA4093822299F31D0ULL,
    0x082EFA98EC4E6C89ULL, 0x452821E638D01377ULL,
};

uint32_t _wpi_upb_Hash(const void* p, size_t n, uint64_t seed) {
  return Wyhash(p, n, seed, kWyhashSalt);
}

static uint32_t _wpi_upb_Hash_NoSeed(const char* p, size_t n) {
  return _wpi_upb_Hash(p, n, 0);
}

static uint32_t strhash(wpi_upb_tabkey key) {
  uint32_t len;
  char* str = wpi_upb_tabstr(key, &len);
  return _wpi_upb_Hash_NoSeed(str, len);
}

static bool streql(wpi_upb_tabkey k1, lookupkey_t k2) {
  uint32_t len;
  char* str = wpi_upb_tabstr(k1, &len);
  return len == k2.str.len && (len == 0 || memcmp(str, k2.str.str, len) == 0);
}

bool wpi_upb_strtable_init(wpi_upb_strtable* t, size_t expected_size, wpi_upb_Arena* a) {
  // Multiply by approximate reciprocal of MAX_LOAD (0.85), with pow2
  // denominator.
  size_t need_entries = (expected_size + 1) * 1204 / 1024;
  WPI_UPB_ASSERT(need_entries >= expected_size * 0.85);
  int size_lg2 = wpi_upb_Log2Ceiling(need_entries);
  return init(&t->t, size_lg2, a);
}

void wpi_upb_strtable_clear(wpi_upb_strtable* t) {
  size_t bytes = wpi_upb_table_size(&t->t) * sizeof(wpi_upb_tabent);
  t->t.count = 0;
  memset((char*)t->t.entries, 0, bytes);
}

bool wpi_upb_strtable_resize(wpi_upb_strtable* t, size_t size_lg2, wpi_upb_Arena* a) {
  wpi_upb_strtable new_table;
  if (!init(&new_table.t, size_lg2, a)) return false;

  intptr_t iter = WPI_UPB_STRTABLE_BEGIN;
  wpi_upb_StringView key;
  wpi_upb_value val;
  while (wpi_upb_strtable_next2(t, &key, &val, &iter)) {
    wpi_upb_strtable_insert(&new_table, key.data, key.size, val, a);
  }
  *t = new_table;
  return true;
}

bool wpi_upb_strtable_insert(wpi_upb_strtable* t, const char* k, size_t len,
                         wpi_upb_value v, wpi_upb_Arena* a) {
  lookupkey_t key;
  wpi_upb_tabkey tabkey;
  uint32_t hash;

  if (isfull(&t->t)) {
    /* Need to resize.  New table of double the size, add old elements to it. */
    if (!wpi_upb_strtable_resize(t, t->t.size_lg2 + 1, a)) {
      return false;
    }
  }

  key = strkey2(k, len);
  tabkey = strcopy(key, a);
  if (tabkey == 0) return false;

  hash = _wpi_upb_Hash_NoSeed(key.str.str, key.str.len);
  insert(&t->t, key, tabkey, v, hash, &strhash, &streql);
  return true;
}

bool wpi_upb_strtable_lookup2(const wpi_upb_strtable* t, const char* key, size_t len,
                          wpi_upb_value* v) {
  uint32_t hash = _wpi_upb_Hash_NoSeed(key, len);
  return lookup(&t->t, strkey2(key, len), v, hash, &streql);
}

bool wpi_upb_strtable_remove2(wpi_upb_strtable* t, const char* key, size_t len,
                          wpi_upb_value* val) {
  uint32_t hash = _wpi_upb_Hash_NoSeed(key, len);
  wpi_upb_tabkey tabkey;
  return rm(&t->t, strkey2(key, len), val, &tabkey, hash, &streql);
}

/* Iteration */

void wpi_upb_strtable_begin(wpi_upb_strtable_iter* i, const wpi_upb_strtable* t) {
  i->t = t;
  i->index = begin(&t->t);
}

void wpi_upb_strtable_next(wpi_upb_strtable_iter* i) {
  i->index = next(&i->t->t, i->index);
}

bool wpi_upb_strtable_done(const wpi_upb_strtable_iter* i) {
  if (!i->t) return true;
  return i->index >= wpi_upb_table_size(&i->t->t) ||
         wpi_upb_tabent_isempty(str_tabent(i));
}

wpi_upb_StringView wpi_upb_strtable_iter_key(const wpi_upb_strtable_iter* i) {
  wpi_upb_StringView key;
  uint32_t len;
  WPI_UPB_ASSERT(!wpi_upb_strtable_done(i));
  key.data = wpi_upb_tabstr(str_tabent(i)->key, &len);
  key.size = len;
  return key;
}

wpi_upb_value wpi_upb_strtable_iter_value(const wpi_upb_strtable_iter* i) {
  WPI_UPB_ASSERT(!wpi_upb_strtable_done(i));
  return _wpi_upb_value_val(str_tabent(i)->val.val);
}

void wpi_upb_strtable_iter_setdone(wpi_upb_strtable_iter* i) {
  i->t = NULL;
  i->index = SIZE_MAX;
}

bool wpi_upb_strtable_iter_isequal(const wpi_upb_strtable_iter* i1,
                               const wpi_upb_strtable_iter* i2) {
  if (wpi_upb_strtable_done(i1) && wpi_upb_strtable_done(i2)) return true;
  return i1->t == i2->t && i1->index == i2->index;
}

/* wpi_upb_inttable ***************************************************************/

/* For inttables we use a hybrid structure where small keys are kept in an
 * array and large keys are put in the hash table. */

static uint32_t inthash(wpi_upb_tabkey key) { return wpi_upb_inthash(key); }

static bool inteql(wpi_upb_tabkey k1, lookupkey_t k2) { return k1 == k2.num; }

static wpi_upb_tabval* mutable_array(wpi_upb_inttable* t) {
  return (wpi_upb_tabval*)t->array;
}

static wpi_upb_tabval* inttable_val(wpi_upb_inttable* t, uintptr_t key) {
  if (key < t->array_size) {
    return wpi_upb_arrhas(t->array[key]) ? &(mutable_array(t)[key]) : NULL;
  } else {
    wpi_upb_tabent* e =
        findentry_mutable(&t->t, intkey(key), wpi_upb_inthash(key), &inteql);
    return e ? &e->val : NULL;
  }
}

static const wpi_upb_tabval* inttable_val_const(const wpi_upb_inttable* t,
                                            uintptr_t key) {
  return inttable_val((wpi_upb_inttable*)t, key);
}

size_t wpi_upb_inttable_count(const wpi_upb_inttable* t) {
  return t->t.count + t->array_count;
}

static void check(wpi_upb_inttable* t) {
  WPI_UPB_UNUSED(t);
#if defined(WPI_UPB_DEBUG_TABLE) && !defined(NDEBUG)
  {
    // This check is very expensive (makes inserts/deletes O(N)).
    size_t count = 0;
    intptr_t iter = WPI_UPB_INTTABLE_BEGIN;
    uintptr_t key;
    wpi_upb_value val;
    while (wpi_upb_inttable_next(t, &key, &val, &iter)) {
      WPI_UPB_ASSERT(wpi_upb_inttable_lookup(t, key, NULL));
    }
    WPI_UPB_ASSERT(count == wpi_upb_inttable_count(t));
  }
#endif
}

bool wpi_upb_inttable_sizedinit(wpi_upb_inttable* t, size_t asize, int hsize_lg2,
                            wpi_upb_Arena* a) {
  size_t array_bytes;

  if (!init(&t->t, hsize_lg2, a)) return false;
  /* Always make the array part at least 1 long, so that we know key 0
   * won't be in the hash part, which simplifies things. */
  t->array_size = WPI_UPB_MAX(1, asize);
  t->array_count = 0;
  array_bytes = t->array_size * sizeof(wpi_upb_value);
  t->array = wpi_upb_Arena_Malloc(a, array_bytes);
  if (!t->array) {
    return false;
  }
  memset(mutable_array(t), 0xff, array_bytes);
  check(t);
  return true;
}

bool wpi_upb_inttable_init(wpi_upb_inttable* t, wpi_upb_Arena* a) {
  return wpi_upb_inttable_sizedinit(t, 0, 4, a);
}

bool wpi_upb_inttable_insert(wpi_upb_inttable* t, uintptr_t key, wpi_upb_value val,
                         wpi_upb_Arena* a) {
  wpi_upb_tabval tabval;
  tabval.val = val.val;
  WPI_UPB_ASSERT(
      wpi_upb_arrhas(tabval)); /* This will reject (uint64_t)-1.  Fix this. */

  if (key < t->array_size) {
    WPI_UPB_ASSERT(!wpi_upb_arrhas(t->array[key]));
    t->array_count++;
    mutable_array(t)[key].val = val.val;
  } else {
    if (isfull(&t->t)) {
      /* Need to resize the hash part, but we re-use the array part. */
      size_t i;
      wpi_upb_table new_table;

      if (!init(&new_table, t->t.size_lg2 + 1, a)) {
        return false;
      }

      for (i = begin(&t->t); i < wpi_upb_table_size(&t->t); i = next(&t->t, i)) {
        const wpi_upb_tabent* e = &t->t.entries[i];
        uint32_t hash;
        wpi_upb_value v;

        _wpi_upb_value_setval(&v, e->val.val);
        hash = wpi_upb_inthash(e->key);
        insert(&new_table, intkey(e->key), e->key, v, hash, &inthash, &inteql);
      }

      WPI_UPB_ASSERT(t->t.count == new_table.count);

      t->t = new_table;
    }
    insert(&t->t, intkey(key), key, val, wpi_upb_inthash(key), &inthash, &inteql);
  }
  check(t);
  return true;
}

bool wpi_upb_inttable_lookup(const wpi_upb_inttable* t, uintptr_t key, wpi_upb_value* v) {
  const wpi_upb_tabval* table_v = inttable_val_const(t, key);
  if (!table_v) return false;
  if (v) _wpi_upb_value_setval(v, table_v->val);
  return true;
}

bool wpi_upb_inttable_replace(wpi_upb_inttable* t, uintptr_t key, wpi_upb_value val) {
  wpi_upb_tabval* table_v = inttable_val(t, key);
  if (!table_v) return false;
  table_v->val = val.val;
  return true;
}

bool wpi_upb_inttable_remove(wpi_upb_inttable* t, uintptr_t key, wpi_upb_value* val) {
  bool success;
  if (key < t->array_size) {
    if (wpi_upb_arrhas(t->array[key])) {
      wpi_upb_tabval empty = WPI_UPB_TABVALUE_EMPTY_INIT;
      t->array_count--;
      if (val) {
        _wpi_upb_value_setval(val, t->array[key].val);
      }
      mutable_array(t)[key] = empty;
      success = true;
    } else {
      success = false;
    }
  } else {
    success = rm(&t->t, intkey(key), val, NULL, wpi_upb_inthash(key), &inteql);
  }
  check(t);
  return success;
}

void wpi_upb_inttable_compact(wpi_upb_inttable* t, wpi_upb_Arena* a) {
  /* A power-of-two histogram of the table keys. */
  size_t counts[WPI_UPB_MAXARRSIZE + 1] = {0};

  /* The max key in each bucket. */
  uintptr_t max[WPI_UPB_MAXARRSIZE + 1] = {0};

  {
    intptr_t iter = WPI_UPB_INTTABLE_BEGIN;
    uintptr_t key;
    wpi_upb_value val;
    while (wpi_upb_inttable_next(t, &key, &val, &iter)) {
      int bucket = log2ceil(key);
      max[bucket] = WPI_UPB_MAX(max[bucket], key);
      counts[bucket]++;
    }
  }

  /* Find the largest power of two that satisfies the MIN_DENSITY
   * definition (while actually having some keys). */
  size_t arr_count = wpi_upb_inttable_count(t);
  int size_lg2;
  wpi_upb_inttable new_t;

  for (size_lg2 = ARRAY_SIZE(counts) - 1; size_lg2 > 0; size_lg2--) {
    if (counts[size_lg2] == 0) {
      /* We can halve again without losing any entries. */
      continue;
    } else if (arr_count >= (1 << size_lg2) * MIN_DENSITY) {
      break;
    }

    arr_count -= counts[size_lg2];
  }

  WPI_UPB_ASSERT(arr_count <= wpi_upb_inttable_count(t));

  {
    /* Insert all elements into new, perfectly-sized table. */
    size_t arr_size = max[size_lg2] + 1; /* +1 so arr[max] will fit. */
    size_t hash_count = wpi_upb_inttable_count(t) - arr_count;
    size_t hash_size = hash_count ? (hash_count / MAX_LOAD) + 1 : 0;
    int hashsize_lg2 = log2ceil(hash_size);

    wpi_upb_inttable_sizedinit(&new_t, arr_size, hashsize_lg2, a);

    {
      intptr_t iter = WPI_UPB_INTTABLE_BEGIN;
      uintptr_t key;
      wpi_upb_value val;
      while (wpi_upb_inttable_next(t, &key, &val, &iter)) {
        wpi_upb_inttable_insert(&new_t, key, val, a);
      }
    }

    WPI_UPB_ASSERT(new_t.array_size == arr_size);
    WPI_UPB_ASSERT(new_t.t.size_lg2 == hashsize_lg2);
  }
  *t = new_t;
}

// Iteration.

bool wpi_upb_inttable_next(const wpi_upb_inttable* t, uintptr_t* key, wpi_upb_value* val,
                       intptr_t* iter) {
  intptr_t i = *iter;
  if ((size_t)(i + 1) <= t->array_size) {
    while ((size_t)++i < t->array_size) {
      wpi_upb_tabval ent = t->array[i];
      if (wpi_upb_arrhas(ent)) {
        *key = i;
        *val = _wpi_upb_value_val(ent.val);
        *iter = i;
        return true;
      }
    }
    i--;  // Back up to exactly one position before the start of the table.
  }

  size_t tab_idx = next(&t->t, i - t->array_size);
  if (tab_idx < wpi_upb_table_size(&t->t)) {
    wpi_upb_tabent* ent = &t->t.entries[tab_idx];
    *key = ent->key;
    *val = _wpi_upb_value_val(ent->val.val);
    *iter = tab_idx + t->array_size;
    return true;
  }

  return false;
}

void wpi_upb_inttable_removeiter(wpi_upb_inttable* t, intptr_t* iter) {
  intptr_t i = *iter;
  if ((size_t)i < t->array_size) {
    t->array_count--;
    mutable_array(t)[i].val = -1;
  } else {
    wpi_upb_tabent* ent = &t->t.entries[i - t->array_size];
    wpi_upb_tabent* prev = NULL;

    // Linear search, not great.
    wpi_upb_tabent* end = &t->t.entries[wpi_upb_table_size(&t->t)];
    for (wpi_upb_tabent* e = t->t.entries; e != end; e++) {
      if (e->next == ent) {
        prev = e;
        break;
      }
    }

    if (prev) {
      prev->next = ent->next;
    }

    t->t.count--;
    ent->key = 0;
    ent->next = NULL;
  }
}

bool wpi_upb_strtable_next2(const wpi_upb_strtable* t, wpi_upb_StringView* key,
                        wpi_upb_value* val, intptr_t* iter) {
  size_t tab_idx = next(&t->t, *iter);
  if (tab_idx < wpi_upb_table_size(&t->t)) {
    wpi_upb_tabent* ent = &t->t.entries[tab_idx];
    uint32_t len;
    key->data = wpi_upb_tabstr(ent->key, &len);
    key->size = len;
    *val = _wpi_upb_value_val(ent->val.val);
    *iter = tab_idx;
    return true;
  }

  return false;
}

void wpi_upb_strtable_removeiter(wpi_upb_strtable* t, intptr_t* iter) {
  intptr_t i = *iter;
  wpi_upb_tabent* ent = &t->t.entries[i];
  wpi_upb_tabent* prev = NULL;

  // Linear search, not great.
  wpi_upb_tabent* end = &t->t.entries[wpi_upb_table_size(&t->t)];
  for (wpi_upb_tabent* e = t->t.entries; e != end; e++) {
    if (e->next == ent) {
      prev = e;
      break;
    }
  }

  if (prev) {
    prev->next = ent->next;
  }

  t->t.count--;
  ent->key = 0;
  ent->next = NULL;
}

void wpi_upb_strtable_setentryvalue(wpi_upb_strtable* t, intptr_t iter, wpi_upb_value v) {
  wpi_upb_tabent* ent = &t->t.entries[iter];
  ent->val.val = v.val;
}


// Must be last.

const char* wpi_upb_BufToUint64(const char* ptr, const char* end, uint64_t* val) {
  uint64_t u64 = 0;
  while (ptr < end) {
    unsigned ch = *ptr - '0';
    if (ch >= 10) break;
    if (u64 > UINT64_MAX / 10 || u64 * 10 > UINT64_MAX - ch) {
      return NULL;  // integer overflow
    }
    u64 *= 10;
    u64 += ch;
    ptr++;
  }

  *val = u64;
  return ptr;
}

const char* wpi_upb_BufToInt64(const char* ptr, const char* end, int64_t* val,
                           bool* is_neg) {
  bool neg = false;
  uint64_t u64;

  if (ptr != end && *ptr == '-') {
    ptr++;
    neg = true;
  }

  ptr = wpi_upb_BufToUint64(ptr, end, &u64);
  if (!ptr || u64 > (uint64_t)INT64_MAX + neg) {
    return NULL;  // integer overflow
  }

  *val = neg ? -u64 : u64;
  if (is_neg) *is_neg = neg;
  return ptr;
}


#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Must be last.

/* Miscellaneous utilities ****************************************************/

static void wpi_upb_FixLocale(char* p) {
  /* printf() is dependent on locales; sadly there is no easy and portable way
   * to avoid this. This little post-processing step will translate 1,2 -> 1.2
   * since JSON needs the latter. Arguably a hack, but it is simple and the
   * alternatives are far more complicated, platform-dependent, and/or larger
   * in code size. */
  for (; *p; p++) {
    if (*p == ',') *p = '.';
  }
}

void _wpi_upb_EncodeRoundTripDouble(double val, char* buf, size_t size) {
  assert(size >= kUpb_RoundTripBufferSize);
  if (isnan(val)) {
    snprintf(buf, size, "%s", "nan");
    return;
  }
  snprintf(buf, size, "%.*g", DBL_DIG, val);
  if (strtod(buf, NULL) != val) {
    snprintf(buf, size, "%.*g", DBL_DIG + 2, val);
    assert(strtod(buf, NULL) == val);
  }
  wpi_upb_FixLocale(buf);
}

void _wpi_upb_EncodeRoundTripFloat(float val, char* buf, size_t size) {
  assert(size >= kUpb_RoundTripBufferSize);
  if (isnan(val)) {
    snprintf(buf, size, "%s", "nan");
    return;
  }
  snprintf(buf, size, "%.*g", FLT_DIG, val);
  if (strtof(buf, NULL) != val) {
    snprintf(buf, size, "%.*g", FLT_DIG + 3, val);
    assert(strtof(buf, NULL) == val);
  }
  wpi_upb_FixLocale(buf);
}


#include <stdlib.h>
#include <string.h>

// Must be last.

// Determine the locale-specific radix character by calling sprintf() to print
// the number 1.5, then stripping off the digits.  As far as I can tell, this
// is the only portable, thread-safe way to get the C library to divulge the
// locale's radix character.  No, localeconv() is NOT thread-safe.

static int GetLocaleRadix(char *data, size_t capacity) {
  char temp[16];
  const int size = snprintf(temp, sizeof(temp), "%.1f", 1.5);
  WPI_UPB_ASSERT(temp[0] == '1');
  WPI_UPB_ASSERT(temp[size - 1] == '5');
  WPI_UPB_ASSERT(size < capacity);
  temp[size - 1] = '\0';
  strcpy(data, temp + 1);
  return size - 2;
}

// Populates a string identical to *input except that the character pointed to
// by pos (which should be '.') is replaced with the locale-specific radix.

static void LocalizeRadix(const char *input, const char *pos, char *output) {
  const int len1 = pos - input;

  char radix[8];
  const int len2 = GetLocaleRadix(radix, sizeof(radix));

  memcpy(output, input, len1);
  memcpy(output + len1, radix, len2);
  strcpy(output + len1 + len2, input + len1 + 1);
}

double _wpi_upb_NoLocaleStrtod(const char *str, char **endptr) {
  // We cannot simply set the locale to "C" temporarily with setlocale()
  // as this is not thread-safe.  Instead, we try to parse in the current
  // locale first.  If parsing stops at a '.' character, then this is a
  // pretty good hint that we're actually in some other locale in which
  // '.' is not the radix character.

  char *temp_endptr;
  double result = strtod(str, &temp_endptr);
  if (endptr != NULL) *endptr = temp_endptr;
  if (*temp_endptr != '.') return result;

  // Parsing halted on a '.'.  Perhaps we're in a different locale?  Let's
  // try to replace the '.' with a locale-specific radix character and
  // try again.

  char localized[80];
  LocalizeRadix(str, temp_endptr, localized);
  char *localized_endptr;
  result = strtod(localized, &localized_endptr);
  if ((localized_endptr - &localized[0]) > (temp_endptr - str)) {
    // This attempt got further, so replacing the decimal must have helped.
    // Update endptr to point at the right location.
    if (endptr != NULL) {
      // size_diff is non-zero if the localized radix has multiple bytes.
      int size_diff = strlen(localized) - strlen(str);
      *endptr = (char *)str + (localized_endptr - &localized[0] - size_diff);
    }
  }

  return result;
}


// Must be last.

int wpi_upb_Unicode_ToUTF8(uint32_t cp, char* out) {
  if (cp <= 0x7f) {
    out[0] = cp;
    return 1;
  }
  if (cp <= 0x07ff) {
    out[0] = (cp >> 6) | 0xc0;
    out[1] = (cp & 0x3f) | 0x80;
    return 2;
  }
  if (cp <= 0xffff) {
    out[0] = (cp >> 12) | 0xe0;
    out[1] = ((cp >> 6) & 0x3f) | 0x80;
    out[2] = (cp & 0x3f) | 0x80;
    return 3;
  }
  if (cp <= 0x10ffff) {
    out[0] = (cp >> 18) | 0xf0;
    out[1] = ((cp >> 12) & 0x3f) | 0x80;
    out[2] = ((cp >> 6) & 0x3f) | 0x80;
    out[3] = (cp & 0x3f) | 0x80;
    return 4;
  }
  return 0;
}


#include <stdlib.h>


// Must be last.

typedef struct wpi_upb_UnknownFields wpi_upb_UnknownFields;

typedef struct {
  uint32_t tag;
  union {
    uint64_t varint;
    uint64_t uint64;
    uint32_t uint32;
    wpi_upb_StringView delimited;
    wpi_upb_UnknownFields* group;
  } data;
} wpi_upb_UnknownField;

struct wpi_upb_UnknownFields {
  size_t size;
  size_t capacity;
  wpi_upb_UnknownField* fields;
};

typedef struct {
  wpi_upb_EpsCopyInputStream stream;
  wpi_upb_Arena* arena;
  wpi_upb_UnknownField* tmp;
  size_t tmp_size;
  int depth;
  wpi_upb_UnknownCompareResult status;
  jmp_buf err;
} wpi_upb_UnknownField_Context;

WPI_UPB_NORETURN static void wpi_upb_UnknownFields_OutOfMemory(
    wpi_upb_UnknownField_Context* ctx) {
  ctx->status = kUpb_UnknownCompareResult_OutOfMemory;
  WPI_UPB_LONGJMP(ctx->err, 1);
}

static void wpi_upb_UnknownFields_Grow(wpi_upb_UnknownField_Context* ctx,
                                   wpi_upb_UnknownField** base,
                                   wpi_upb_UnknownField** ptr,
                                   wpi_upb_UnknownField** end) {
  size_t old = (*ptr - *base);
  size_t new = WPI_UPB_MAX(4, old * 2);

  *base = wpi_upb_Arena_Realloc(ctx->arena, *base, old * sizeof(**base),
                            new * sizeof(**base));
  if (!*base) wpi_upb_UnknownFields_OutOfMemory(ctx);

  *ptr = *base + old;
  *end = *base + new;
}

// We have to implement our own sort here, since qsort() is not an in-order
// sort. Here we use merge sort, the simplest in-order sort.
static void wpi_upb_UnknownFields_Merge(wpi_upb_UnknownField* arr, size_t start,
                                    size_t mid, size_t end,
                                    wpi_upb_UnknownField* tmp) {
  memcpy(tmp, &arr[start], (end - start) * sizeof(*tmp));

  wpi_upb_UnknownField* ptr1 = tmp;
  wpi_upb_UnknownField* end1 = &tmp[mid - start];
  wpi_upb_UnknownField* ptr2 = &tmp[mid - start];
  wpi_upb_UnknownField* end2 = &tmp[end - start];
  wpi_upb_UnknownField* out = &arr[start];

  while (ptr1 < end1 && ptr2 < end2) {
    if (ptr1->tag <= ptr2->tag) {
      *out++ = *ptr1++;
    } else {
      *out++ = *ptr2++;
    }
  }

  if (ptr1 < end1) {
    memcpy(out, ptr1, (end1 - ptr1) * sizeof(*out));
  } else if (ptr2 < end2) {
    memcpy(out, ptr1, (end2 - ptr2) * sizeof(*out));
  }
}

static void wpi_upb_UnknownFields_SortRecursive(wpi_upb_UnknownField* arr, size_t start,
                                            size_t end, wpi_upb_UnknownField* tmp) {
  if (end - start > 1) {
    size_t mid = start + ((end - start) / 2);
    wpi_upb_UnknownFields_SortRecursive(arr, start, mid, tmp);
    wpi_upb_UnknownFields_SortRecursive(arr, mid, end, tmp);
    wpi_upb_UnknownFields_Merge(arr, start, mid, end, tmp);
  }
}

static void wpi_upb_UnknownFields_Sort(wpi_upb_UnknownField_Context* ctx,
                                   wpi_upb_UnknownFields* fields) {
  if (ctx->tmp_size < fields->size) {
    const int oldsize = ctx->tmp_size * sizeof(*ctx->tmp);
    ctx->tmp_size = WPI_UPB_MAX(8, ctx->tmp_size);
    while (ctx->tmp_size < fields->size) ctx->tmp_size *= 2;
    const int newsize = ctx->tmp_size * sizeof(*ctx->tmp);
    ctx->tmp = wpi_upb_grealloc(ctx->tmp, oldsize, newsize);
  }
  wpi_upb_UnknownFields_SortRecursive(fields->fields, 0, fields->size, ctx->tmp);
}

static wpi_upb_UnknownFields* wpi_upb_UnknownFields_DoBuild(
    wpi_upb_UnknownField_Context* ctx, const char** buf) {
  wpi_upb_UnknownField* arr_base = NULL;
  wpi_upb_UnknownField* arr_ptr = NULL;
  wpi_upb_UnknownField* arr_end = NULL;
  const char* ptr = *buf;
  uint32_t last_tag = 0;
  bool sorted = true;
  while (!wpi_upb_EpsCopyInputStream_IsDone(&ctx->stream, &ptr)) {
    uint32_t tag;
    ptr = wpi_upb_WireReader_ReadTag(ptr, &tag);
    WPI_UPB_ASSERT(tag <= UINT32_MAX);
    int wire_type = wpi_upb_WireReader_GetWireType(tag);
    if (wire_type == kUpb_WireType_EndGroup) break;
    if (tag < last_tag) sorted = false;
    last_tag = tag;

    if (arr_ptr == arr_end) {
      wpi_upb_UnknownFields_Grow(ctx, &arr_base, &arr_ptr, &arr_end);
    }
    wpi_upb_UnknownField* field = arr_ptr;
    field->tag = tag;
    arr_ptr++;

    switch (wire_type) {
      case kUpb_WireType_Varint:
        ptr = wpi_upb_WireReader_ReadVarint(ptr, &field->data.varint);
        break;
      case kUpb_WireType_64Bit:
        ptr = wpi_upb_WireReader_ReadFixed64(ptr, &field->data.uint64);
        break;
      case kUpb_WireType_32Bit:
        ptr = wpi_upb_WireReader_ReadFixed32(ptr, &field->data.uint32);
        break;
      case kUpb_WireType_Delimited: {
        int size;
        ptr = wpi_upb_WireReader_ReadSize(ptr, &size);
        const char* s_ptr = ptr;
        ptr = wpi_upb_EpsCopyInputStream_ReadStringAliased(&ctx->stream, &s_ptr,
                                                       size);
        field->data.delimited.data = s_ptr;
        field->data.delimited.size = size;
        break;
      }
      case kUpb_WireType_StartGroup:
        if (--ctx->depth == 0) {
          ctx->status = kUpb_UnknownCompareResult_MaxDepthExceeded;
          WPI_UPB_LONGJMP(ctx->err, 1);
        }
        field->data.group = wpi_upb_UnknownFields_DoBuild(ctx, &ptr);
        ctx->depth++;
        break;
      default:
        WPI_UPB_UNREACHABLE();
    }
  }

  *buf = ptr;
  wpi_upb_UnknownFields* ret = wpi_upb_Arena_Malloc(ctx->arena, sizeof(*ret));
  if (!ret) wpi_upb_UnknownFields_OutOfMemory(ctx);
  ret->fields = arr_base;
  ret->size = arr_ptr - arr_base;
  ret->capacity = arr_end - arr_base;
  if (!sorted) {
    wpi_upb_UnknownFields_Sort(ctx, ret);
  }
  return ret;
}

// Builds a wpi_upb_UnknownFields data structure from the binary data in buf.
static wpi_upb_UnknownFields* wpi_upb_UnknownFields_Build(wpi_upb_UnknownField_Context* ctx,
                                                  const char* ptr,
                                                  size_t size) {
  wpi_upb_EpsCopyInputStream_Init(&ctx->stream, &ptr, size, true);
  wpi_upb_UnknownFields* fields = wpi_upb_UnknownFields_DoBuild(ctx, &ptr);
  WPI_UPB_ASSERT(wpi_upb_EpsCopyInputStream_IsDone(&ctx->stream, &ptr) &&
             !wpi_upb_EpsCopyInputStream_IsError(&ctx->stream));
  return fields;
}

// Compares two sorted wpi_upb_UnknownFields structures for equality.
static bool wpi_upb_UnknownFields_IsEqual(const wpi_upb_UnknownFields* uf1,
                                      const wpi_upb_UnknownFields* uf2) {
  if (uf1->size != uf2->size) return false;
  for (size_t i = 0, n = uf1->size; i < n; i++) {
    wpi_upb_UnknownField* f1 = &uf1->fields[i];
    wpi_upb_UnknownField* f2 = &uf2->fields[i];
    if (f1->tag != f2->tag) return false;
    int wire_type = f1->tag & 7;
    switch (wire_type) {
      case kUpb_WireType_Varint:
        if (f1->data.varint != f2->data.varint) return false;
        break;
      case kUpb_WireType_64Bit:
        if (f1->data.uint64 != f2->data.uint64) return false;
        break;
      case kUpb_WireType_32Bit:
        if (f1->data.uint32 != f2->data.uint32) return false;
        break;
      case kUpb_WireType_Delimited:
        if (!wpi_upb_StringView_IsEqual(f1->data.delimited, f2->data.delimited)) {
          return false;
        }
        break;
      case kUpb_WireType_StartGroup:
        if (!wpi_upb_UnknownFields_IsEqual(f1->data.group, f2->data.group)) {
          return false;
        }
        break;
      default:
        WPI_UPB_UNREACHABLE();
    }
  }
  return true;
}

static wpi_upb_UnknownCompareResult wpi_upb_UnknownField_DoCompare(
    wpi_upb_UnknownField_Context* ctx, const char* buf1, size_t size1,
    const char* buf2, size_t size2) {
  wpi_upb_UnknownCompareResult ret;
  // First build both unknown fields into a sorted data structure (similar
  // to the UnknownFieldSet in C++).
  wpi_upb_UnknownFields* uf1 = wpi_upb_UnknownFields_Build(ctx, buf1, size1);
  wpi_upb_UnknownFields* uf2 = wpi_upb_UnknownFields_Build(ctx, buf2, size2);

  // Now perform the equality check on the sorted structures.
  if (wpi_upb_UnknownFields_IsEqual(uf1, uf2)) {
    ret = kUpb_UnknownCompareResult_Equal;
  } else {
    ret = kUpb_UnknownCompareResult_NotEqual;
  }
  return ret;
}

static wpi_upb_UnknownCompareResult wpi_upb_UnknownField_Compare(
    wpi_upb_UnknownField_Context* const ctx, const char* const buf1,
    const size_t size1, const char* const buf2, const size_t size2) {
  wpi_upb_UnknownCompareResult ret;
  if (WPI_UPB_SETJMP(ctx->err) == 0) {
    ret = wpi_upb_UnknownField_DoCompare(ctx, buf1, size1, buf2, size2);
  } else {
    ret = ctx->status;
    WPI_UPB_ASSERT(ret != kUpb_UnknownCompareResult_Equal);
  }

  wpi_upb_Arena_Free(ctx->arena);
  wpi_upb_gfree(ctx->tmp);
  return ret;
}

wpi_upb_UnknownCompareResult WPI_UPB_PRIVATE(_wpi_upb_Message_UnknownFieldsAreEqual)(
    const char* buf1, size_t size1, const char* buf2, size_t size2,
    int max_depth) {
  if (size1 == 0 && size2 == 0) return kUpb_UnknownCompareResult_Equal;
  if (size1 == 0 || size2 == 0) return kUpb_UnknownCompareResult_NotEqual;
  if (memcmp(buf1, buf2, size1) == 0) return kUpb_UnknownCompareResult_Equal;

  wpi_upb_UnknownField_Context ctx = {
      .arena = wpi_upb_Arena_New(),
      .depth = max_depth,
      .tmp = NULL,
      .tmp_size = 0,
      .status = kUpb_UnknownCompareResult_Equal,
  };

  if (!ctx.arena) return kUpb_UnknownCompareResult_OutOfMemory;

  return wpi_upb_UnknownField_Compare(&ctx, buf1, size1, buf2, size2);
}


#include <string.h>


// Must be last.

const wpi_upb_Extension* WPI_UPB_PRIVATE(_wpi_upb_Message_Getext)(
    const struct wpi_upb_Message* msg, const wpi_upb_MiniTableExtension* e) {
  size_t n;
  const wpi_upb_Extension* ext = WPI_UPB_PRIVATE(_wpi_upb_Message_Getexts)(msg, &n);

  // For now we use linear search exclusively to find extensions.
  // If this becomes an issue due to messages with lots of extensions,
  // we can introduce a table of some sort.
  for (size_t i = 0; i < n; i++) {
    if (ext[i].ext == e) {
      return &ext[i];
    }
  }

  return NULL;
}

const wpi_upb_Extension* WPI_UPB_PRIVATE(_wpi_upb_Message_Getexts)(
    const struct wpi_upb_Message* msg, size_t* count) {
  wpi_upb_Message_Internal* in = WPI_UPB_PRIVATE(_wpi_upb_Message_GetInternal)(msg);
  if (in) {
    *count = (in->size - in->ext_begin) / sizeof(wpi_upb_Extension);
    return WPI_UPB_PTR_AT(in, in->ext_begin, void);
  } else {
    *count = 0;
    return NULL;
  }
}

wpi_upb_Extension* WPI_UPB_PRIVATE(_wpi_upb_Message_GetOrCreateExtension)(
    struct wpi_upb_Message* msg, const wpi_upb_MiniTableExtension* e, wpi_upb_Arena* a) {
  WPI_UPB_ASSERT(!wpi_upb_Message_IsFrozen(msg));
  wpi_upb_Extension* ext = (wpi_upb_Extension*)WPI_UPB_PRIVATE(_wpi_upb_Message_Getext)(msg, e);
  if (ext) return ext;
  if (!WPI_UPB_PRIVATE(_wpi_upb_Message_Realloc)(msg, sizeof(wpi_upb_Extension), a))
    return NULL;
  wpi_upb_Message_Internal* in = WPI_UPB_PRIVATE(_wpi_upb_Message_GetInternal)(msg);
  in->ext_begin -= sizeof(wpi_upb_Extension);
  ext = WPI_UPB_PTR_AT(in, in->ext_begin, void);
  memset(ext, 0, sizeof(wpi_upb_Extension));
  ext->ext = e;
  return ext;
}


#include <math.h>
#include <string.h>


// Must be last.

const float kUpb_FltInfinity = INFINITY;
const double kUpb_Infinity = INFINITY;
const double kUpb_NaN = NAN;

bool WPI_UPB_PRIVATE(_wpi_upb_Message_Realloc)(struct wpi_upb_Message* msg, size_t need,
                                       wpi_upb_Arena* a) {
  WPI_UPB_ASSERT(!wpi_upb_Message_IsFrozen(msg));
  const size_t overhead = sizeof(wpi_upb_Message_Internal);

  wpi_upb_Message_Internal* in = WPI_UPB_PRIVATE(_wpi_upb_Message_GetInternal)(msg);
  if (!in) {
    // No internal data, allocate from scratch.
    size_t size = WPI_UPB_MAX(128, wpi_upb_Log2CeilingSize(need + overhead));
    in = wpi_upb_Arena_Malloc(a, size);
    if (!in) return false;

    in->size = size;
    in->unknown_end = overhead;
    in->ext_begin = size;
    WPI_UPB_PRIVATE(_wpi_upb_Message_SetInternal)(msg, in);
  } else if (in->ext_begin - in->unknown_end < need) {
    // Internal data is too small, reallocate.
    size_t new_size = wpi_upb_Log2CeilingSize(in->size + need);
    size_t ext_bytes = in->size - in->ext_begin;
    size_t new_ext_begin = new_size - ext_bytes;
    in = wpi_upb_Arena_Realloc(a, in, in->size, new_size);
    if (!in) return false;

    if (ext_bytes) {
      // Need to move extension data to the end.
      char* ptr = (char*)in;
      memmove(ptr + new_ext_begin, ptr + in->ext_begin, ext_bytes);
    }
    in->ext_begin = new_ext_begin;
    in->size = new_size;
    WPI_UPB_PRIVATE(_wpi_upb_Message_SetInternal)(msg, in);
  }

  WPI_UPB_ASSERT(in->ext_begin - in->unknown_end >= need);
  return true;
}

#if WPI_UPB_TRACING_ENABLED
static void (*_message_trace_handler)(const wpi_upb_MiniTable*, const wpi_upb_Arena*);

void wpi_upb_Message_LogNewMessage(const wpi_upb_MiniTable* m, const wpi_upb_Arena* arena) {
  if (_message_trace_handler) {
    _message_trace_handler(m, arena);
  }
}

void wpi_upb_Message_SetNewMessageTraceHandler(void (*handler)(const wpi_upb_MiniTable*,
                                                           const wpi_upb_Arena*)) {
  _message_trace_handler = handler;
}
#endif  // WPI_UPB_TRACING_ENABLED


#include <stddef.h>


// Must be last.

bool WPI_UPB_PRIVATE(_wpi_upb_Message_NextBaseField)(const wpi_upb_Message* msg,
                                             const wpi_upb_MiniTable* m,
                                             const wpi_upb_MiniTableField** out_f,
                                             wpi_upb_MessageValue* out_v,
                                             size_t* iter) {
  const size_t count = wpi_upb_MiniTable_FieldCount(m);
  size_t i = *iter;

  while (++i < count) {
    const wpi_upb_MiniTableField* f = wpi_upb_MiniTable_GetFieldByIndex(m, i);
    const void* src = WPI_UPB_PRIVATE(_wpi_upb_Message_DataPtr)(msg, f);

    wpi_upb_MessageValue val;
    WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_DataCopy)(f, &val, src);

    // Skip field if unset or empty.
    if (wpi_upb_MiniTableField_HasPresence(f)) {
      if (!wpi_upb_Message_HasBaseField(msg, f)) continue;
    } else {
      if (WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_DataIsZero)(f, src)) continue;

      if (wpi_upb_MiniTableField_IsArray(f)) {
        if (wpi_upb_Array_Size(val.array_val) == 0) continue;
      } else if (wpi_upb_MiniTableField_IsMap(f)) {
        if (wpi_upb_Map_Size(val.map_val) == 0) continue;
      }
    }

    *out_f = f;
    *out_v = val;
    *iter = i;
    return true;
  }

  return false;
}

bool WPI_UPB_PRIVATE(_wpi_upb_Message_NextExtension)(
    const wpi_upb_Message* msg, const wpi_upb_MiniTable* m,
    const wpi_upb_MiniTableExtension** out_e, wpi_upb_MessageValue* out_v,
    size_t* iter) {
  size_t count;
  const wpi_upb_Extension* exts = WPI_UPB_PRIVATE(_wpi_upb_Message_Getexts)(msg, &count);
  size_t i = *iter;

  if (++i < count) {
    *out_e = exts[i].ext;
    *out_v = exts[i].data;
    *iter = i;
    return true;
  }

  return false;
}

const char _kUpb_ToBase92[] = {
    ' ', '!', '#', '$', '%', '&', '(', ')', '*', '+', ',', '-', '.', '/',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=',
    '>', '?', '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K',
    'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y',
    'Z', '[', ']', '^', '_', '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
    'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '{', '|', '}', '~',
};

const int8_t _kUpb_FromBase92[] = {
    0,  1,  -1, 2,  3,  4,  5,  -1, 6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16,
    17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
    36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54,
    55, 56, 57, -1, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72,
    73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91,
};


#include <assert.h>
#include <stddef.h>
#include <stdint.h>


// Must be last.

typedef struct {
  uint64_t present_values_mask;
  uint32_t last_written_value;
} wpi_upb_MtDataEncoderInternal_EnumState;

typedef struct {
  uint64_t msg_modifiers;
  uint32_t last_field_num;
  enum {
    kUpb_OneofState_NotStarted,
    kUpb_OneofState_StartedOneof,
    kUpb_OneofState_EmittedOneofField,
  } oneof_state;
} wpi_upb_MtDataEncoderInternal_MsgState;

typedef struct {
  char* buf_start;  // Only for checking kUpb_MtDataEncoder_MinSize.
  union {
    wpi_upb_MtDataEncoderInternal_EnumState enum_state;
    wpi_upb_MtDataEncoderInternal_MsgState msg_state;
  } state;
} wpi_upb_MtDataEncoderInternal;

static wpi_upb_MtDataEncoderInternal* wpi_upb_MtDataEncoder_GetInternal(
    wpi_upb_MtDataEncoder* e, char* buf_start) {
  WPI_UPB_ASSERT(sizeof(wpi_upb_MtDataEncoderInternal) <= sizeof(e->internal));
  wpi_upb_MtDataEncoderInternal* ret = (wpi_upb_MtDataEncoderInternal*)e->internal;
  ret->buf_start = buf_start;
  return ret;
}

static char* wpi_upb_MtDataEncoder_PutRaw(wpi_upb_MtDataEncoder* e, char* ptr,
                                      char ch) {
  wpi_upb_MtDataEncoderInternal* in = (wpi_upb_MtDataEncoderInternal*)e->internal;
  WPI_UPB_ASSERT(ptr - in->buf_start < kUpb_MtDataEncoder_MinSize);
  if (ptr == e->end) return NULL;
  *ptr++ = ch;
  return ptr;
}

static char* wpi_upb_MtDataEncoder_Put(wpi_upb_MtDataEncoder* e, char* ptr, char ch) {
  return wpi_upb_MtDataEncoder_PutRaw(e, ptr, _wpi_upb_ToBase92(ch));
}

static char* wpi_upb_MtDataEncoder_PutBase92Varint(wpi_upb_MtDataEncoder* e, char* ptr,
                                               uint32_t val, int min, int max) {
  int shift = wpi_upb_Log2Ceiling(_wpi_upb_FromBase92(max) - _wpi_upb_FromBase92(min) + 1);
  WPI_UPB_ASSERT(shift <= 6);
  uint32_t mask = (1 << shift) - 1;
  do {
    uint32_t bits = val & mask;
    ptr = wpi_upb_MtDataEncoder_Put(e, ptr, bits + _wpi_upb_FromBase92(min));
    if (!ptr) return NULL;
    val >>= shift;
  } while (val);
  return ptr;
}

char* wpi_upb_MtDataEncoder_PutModifier(wpi_upb_MtDataEncoder* e, char* ptr,
                                    uint64_t mod) {
  if (mod) {
    ptr = wpi_upb_MtDataEncoder_PutBase92Varint(e, ptr, mod,
                                            kUpb_EncodedValue_MinModifier,
                                            kUpb_EncodedValue_MaxModifier);
  }
  return ptr;
}

char* wpi_upb_MtDataEncoder_EncodeExtension(wpi_upb_MtDataEncoder* e, char* ptr,
                                        wpi_upb_FieldType type, uint32_t field_num,
                                        uint64_t field_mod) {
  wpi_upb_MtDataEncoderInternal* in = wpi_upb_MtDataEncoder_GetInternal(e, ptr);
  in->state.msg_state.msg_modifiers = 0;
  in->state.msg_state.last_field_num = 0;
  in->state.msg_state.oneof_state = kUpb_OneofState_NotStarted;

  ptr = wpi_upb_MtDataEncoder_PutRaw(e, ptr, kUpb_EncodedVersion_ExtensionV1);
  if (!ptr) return NULL;

  return wpi_upb_MtDataEncoder_PutField(e, ptr, type, field_num, field_mod);
}

char* wpi_upb_MtDataEncoder_EncodeMap(wpi_upb_MtDataEncoder* e, char* ptr,
                                  wpi_upb_FieldType key_type,
                                  wpi_upb_FieldType value_type, uint64_t key_mod,
                                  uint64_t value_mod) {
  wpi_upb_MtDataEncoderInternal* in = wpi_upb_MtDataEncoder_GetInternal(e, ptr);
  in->state.msg_state.msg_modifiers = 0;
  in->state.msg_state.last_field_num = 0;
  in->state.msg_state.oneof_state = kUpb_OneofState_NotStarted;

  ptr = wpi_upb_MtDataEncoder_PutRaw(e, ptr, kUpb_EncodedVersion_MapV1);
  if (!ptr) return NULL;

  ptr = wpi_upb_MtDataEncoder_PutField(e, ptr, key_type, 1, key_mod);
  if (!ptr) return NULL;

  return wpi_upb_MtDataEncoder_PutField(e, ptr, value_type, 2, value_mod);
}

char* wpi_upb_MtDataEncoder_EncodeMessageSet(wpi_upb_MtDataEncoder* e, char* ptr) {
  (void)wpi_upb_MtDataEncoder_GetInternal(e, ptr);
  return wpi_upb_MtDataEncoder_PutRaw(e, ptr, kUpb_EncodedVersion_MessageSetV1);
}

char* wpi_upb_MtDataEncoder_StartMessage(wpi_upb_MtDataEncoder* e, char* ptr,
                                     uint64_t msg_mod) {
  wpi_upb_MtDataEncoderInternal* in = wpi_upb_MtDataEncoder_GetInternal(e, ptr);
  in->state.msg_state.msg_modifiers = msg_mod;
  in->state.msg_state.last_field_num = 0;
  in->state.msg_state.oneof_state = kUpb_OneofState_NotStarted;

  ptr = wpi_upb_MtDataEncoder_PutRaw(e, ptr, kUpb_EncodedVersion_MessageV1);
  if (!ptr) return NULL;

  return wpi_upb_MtDataEncoder_PutModifier(e, ptr, msg_mod);
}

static char* _wpi_upb_MtDataEncoder_MaybePutFieldSkip(wpi_upb_MtDataEncoder* e,
                                                  char* ptr,
                                                  uint32_t field_num) {
  wpi_upb_MtDataEncoderInternal* in = (wpi_upb_MtDataEncoderInternal*)e->internal;
  if (field_num <= in->state.msg_state.last_field_num) return NULL;
  if (in->state.msg_state.last_field_num + 1 != field_num) {
    // Put skip.
    WPI_UPB_ASSERT(field_num > in->state.msg_state.last_field_num);
    uint32_t skip = field_num - in->state.msg_state.last_field_num;
    ptr = wpi_upb_MtDataEncoder_PutBase92Varint(
        e, ptr, skip, kUpb_EncodedValue_MinSkip, kUpb_EncodedValue_MaxSkip);
    if (!ptr) return NULL;
  }
  in->state.msg_state.last_field_num = field_num;
  return ptr;
}

static char* _wpi_upb_MtDataEncoder_PutFieldType(wpi_upb_MtDataEncoder* e, char* ptr,
                                             wpi_upb_FieldType type,
                                             uint64_t field_mod) {
  static const char kUpb_TypeToEncoded[] = {
      [kUpb_FieldType_Double] = kUpb_EncodedType_Double,
      [kUpb_FieldType_Float] = kUpb_EncodedType_Float,
      [kUpb_FieldType_Int64] = kUpb_EncodedType_Int64,
      [kUpb_FieldType_UInt64] = kUpb_EncodedType_UInt64,
      [kUpb_FieldType_Int32] = kUpb_EncodedType_Int32,
      [kUpb_FieldType_Fixed64] = kUpb_EncodedType_Fixed64,
      [kUpb_FieldType_Fixed32] = kUpb_EncodedType_Fixed32,
      [kUpb_FieldType_Bool] = kUpb_EncodedType_Bool,
      [kUpb_FieldType_String] = kUpb_EncodedType_String,
      [kUpb_FieldType_Group] = kUpb_EncodedType_Group,
      [kUpb_FieldType_Message] = kUpb_EncodedType_Message,
      [kUpb_FieldType_Bytes] = kUpb_EncodedType_Bytes,
      [kUpb_FieldType_UInt32] = kUpb_EncodedType_UInt32,
      [kUpb_FieldType_Enum] = kUpb_EncodedType_OpenEnum,
      [kUpb_FieldType_SFixed32] = kUpb_EncodedType_SFixed32,
      [kUpb_FieldType_SFixed64] = kUpb_EncodedType_SFixed64,
      [kUpb_FieldType_SInt32] = kUpb_EncodedType_SInt32,
      [kUpb_FieldType_SInt64] = kUpb_EncodedType_SInt64,
  };

  int encoded_type = kUpb_TypeToEncoded[type];

  if (field_mod & kUpb_FieldModifier_IsClosedEnum) {
    WPI_UPB_ASSERT(type == kUpb_FieldType_Enum);
    encoded_type = kUpb_EncodedType_ClosedEnum;
  }

  if (field_mod & kUpb_FieldModifier_IsRepeated) {
    // Repeated fields shift the type number up (unlike other modifiers which
    // are bit flags).
    encoded_type += kUpb_EncodedType_RepeatedBase;
  }

  return wpi_upb_MtDataEncoder_Put(e, ptr, encoded_type);
}

static char* _wpi_upb_MtDataEncoder_MaybePutModifiers(wpi_upb_MtDataEncoder* e,
                                                  char* ptr, wpi_upb_FieldType type,
                                                  uint64_t field_mod) {
  wpi_upb_MtDataEncoderInternal* in = (wpi_upb_MtDataEncoderInternal*)e->internal;
  uint32_t encoded_modifiers = 0;
  if ((field_mod & kUpb_FieldModifier_IsRepeated) &&
      wpi_upb_FieldType_IsPackable(type)) {
    bool field_is_packed = field_mod & kUpb_FieldModifier_IsPacked;
    bool default_is_packed = in->state.msg_state.msg_modifiers &
                             kUpb_MessageModifier_DefaultIsPacked;
    if (field_is_packed != default_is_packed) {
      encoded_modifiers |= kUpb_EncodedFieldModifier_FlipPacked;
    }
  }

  if (type == kUpb_FieldType_String) {
    bool field_validates_utf8 = field_mod & kUpb_FieldModifier_ValidateUtf8;
    bool message_validates_utf8 =
        in->state.msg_state.msg_modifiers & kUpb_MessageModifier_ValidateUtf8;
    if (field_validates_utf8 != message_validates_utf8) {
      // Old binaries do not recognize the field modifier.  We need the failure
      // mode to be too lax rather than too strict.  Our caller should have
      // handled this (see _wpi_upb_MessageDef_ValidateUtf8()).
      assert(!message_validates_utf8);
      encoded_modifiers |= kUpb_EncodedFieldModifier_FlipValidateUtf8;
    }
  }

  if (field_mod & kUpb_FieldModifier_IsProto3Singular) {
    encoded_modifiers |= kUpb_EncodedFieldModifier_IsProto3Singular;
  }

  if (field_mod & kUpb_FieldModifier_IsRequired) {
    encoded_modifiers |= kUpb_EncodedFieldModifier_IsRequired;
  }

  return wpi_upb_MtDataEncoder_PutModifier(e, ptr, encoded_modifiers);
}

char* wpi_upb_MtDataEncoder_PutField(wpi_upb_MtDataEncoder* e, char* ptr,
                                 wpi_upb_FieldType type, uint32_t field_num,
                                 uint64_t field_mod) {
  wpi_upb_MtDataEncoder_GetInternal(e, ptr);

  ptr = _wpi_upb_MtDataEncoder_MaybePutFieldSkip(e, ptr, field_num);
  if (!ptr) return NULL;

  ptr = _wpi_upb_MtDataEncoder_PutFieldType(e, ptr, type, field_mod);
  if (!ptr) return NULL;

  return _wpi_upb_MtDataEncoder_MaybePutModifiers(e, ptr, type, field_mod);
}

char* wpi_upb_MtDataEncoder_StartOneof(wpi_upb_MtDataEncoder* e, char* ptr) {
  wpi_upb_MtDataEncoderInternal* in = wpi_upb_MtDataEncoder_GetInternal(e, ptr);
  if (in->state.msg_state.oneof_state == kUpb_OneofState_NotStarted) {
    ptr = wpi_upb_MtDataEncoder_Put(e, ptr, _wpi_upb_FromBase92(kUpb_EncodedValue_End));
  } else {
    ptr = wpi_upb_MtDataEncoder_Put(
        e, ptr, _wpi_upb_FromBase92(kUpb_EncodedValue_OneofSeparator));
  }
  in->state.msg_state.oneof_state = kUpb_OneofState_StartedOneof;
  return ptr;
}

char* wpi_upb_MtDataEncoder_PutOneofField(wpi_upb_MtDataEncoder* e, char* ptr,
                                      uint32_t field_num) {
  wpi_upb_MtDataEncoderInternal* in = wpi_upb_MtDataEncoder_GetInternal(e, ptr);
  if (in->state.msg_state.oneof_state == kUpb_OneofState_EmittedOneofField) {
    ptr = wpi_upb_MtDataEncoder_Put(
        e, ptr, _wpi_upb_FromBase92(kUpb_EncodedValue_FieldSeparator));
    if (!ptr) return NULL;
  }
  ptr = wpi_upb_MtDataEncoder_PutBase92Varint(e, ptr, field_num, _wpi_upb_ToBase92(0),
                                          _wpi_upb_ToBase92(63));
  in->state.msg_state.oneof_state = kUpb_OneofState_EmittedOneofField;
  return ptr;
}

char* wpi_upb_MtDataEncoder_StartEnum(wpi_upb_MtDataEncoder* e, char* ptr) {
  wpi_upb_MtDataEncoderInternal* in = wpi_upb_MtDataEncoder_GetInternal(e, ptr);
  in->state.enum_state.present_values_mask = 0;
  in->state.enum_state.last_written_value = 0;

  return wpi_upb_MtDataEncoder_PutRaw(e, ptr, kUpb_EncodedVersion_EnumV1);
}

static char* wpi_upb_MtDataEncoder_FlushDenseEnumMask(wpi_upb_MtDataEncoder* e,
                                                  char* ptr) {
  wpi_upb_MtDataEncoderInternal* in = (wpi_upb_MtDataEncoderInternal*)e->internal;
  ptr = wpi_upb_MtDataEncoder_Put(e, ptr, in->state.enum_state.present_values_mask);
  in->state.enum_state.present_values_mask = 0;
  in->state.enum_state.last_written_value += 5;
  return ptr;
}

char* wpi_upb_MtDataEncoder_PutEnumValue(wpi_upb_MtDataEncoder* e, char* ptr,
                                     uint32_t val) {
  // TODO: optimize this encoding.
  wpi_upb_MtDataEncoderInternal* in = wpi_upb_MtDataEncoder_GetInternal(e, ptr);
  WPI_UPB_ASSERT(val >= in->state.enum_state.last_written_value);
  uint32_t delta = val - in->state.enum_state.last_written_value;
  if (delta >= 5 && in->state.enum_state.present_values_mask) {
    ptr = wpi_upb_MtDataEncoder_FlushDenseEnumMask(e, ptr);
    if (!ptr) {
      return NULL;
    }
    delta -= 5;
  }

  if (delta >= 5) {
    ptr = wpi_upb_MtDataEncoder_PutBase92Varint(
        e, ptr, delta, kUpb_EncodedValue_MinSkip, kUpb_EncodedValue_MaxSkip);
    in->state.enum_state.last_written_value += delta;
    delta = 0;
  }

  WPI_UPB_ASSERT((in->state.enum_state.present_values_mask >> delta) == 0);
  in->state.enum_state.present_values_mask |= 1ULL << delta;
  return ptr;
}

char* wpi_upb_MtDataEncoder_EndEnum(wpi_upb_MtDataEncoder* e, char* ptr) {
  wpi_upb_MtDataEncoderInternal* in = wpi_upb_MtDataEncoder_GetInternal(e, ptr);
  if (!in->state.enum_state.present_values_mask) return ptr;
  return wpi_upb_MtDataEncoder_FlushDenseEnumMask(e, ptr);
}


#include <stddef.h>


// Must be last.

// A MiniTable for an empty message, used for unlinked sub-messages that are
// built via MiniDescriptors.  Messages that use this MiniTable may possibly
// be linked later, in which case this MiniTable will be replaced with a real
// one.  This pattern is known as "dynamic tree shaking", and it introduces
// complication because sub-messages may either be the "empty" type or the
// "real" type.  A tagged bit indicates the difference.
const struct wpi_upb_MiniTable WPI_UPB_PRIVATE(_kUpb_MiniTable_Empty) = {
    .WPI_UPB_PRIVATE(subs) = NULL,
    .WPI_UPB_PRIVATE(fields) = NULL,
    .WPI_UPB_PRIVATE(size) = sizeof(struct wpi_upb_Message),
    .WPI_UPB_PRIVATE(field_count) = 0,
    .WPI_UPB_PRIVATE(ext) = kUpb_ExtMode_NonExtendable,
    .WPI_UPB_PRIVATE(dense_below) = 0,
    .WPI_UPB_PRIVATE(table_mask) = -1,
    .WPI_UPB_PRIVATE(required_count) = 0,
};

// A MiniTable for a statically tree shaken message.  Messages that use this
// MiniTable are guaranteed to remain unlinked; unlike the empty message, this
// MiniTable is never replaced, which greatly simplifies everything, because the
// type of a sub-message is always known, without consulting a tagged bit.
const struct wpi_upb_MiniTable WPI_UPB_PRIVATE(_kUpb_MiniTable_StaticallyTreeShaken) = {
    .WPI_UPB_PRIVATE(subs) = NULL,
    .WPI_UPB_PRIVATE(fields) = NULL,
    .WPI_UPB_PRIVATE(size) = sizeof(struct wpi_upb_Message),
    .WPI_UPB_PRIVATE(field_count) = 0,
    .WPI_UPB_PRIVATE(ext) = kUpb_ExtMode_NonExtendable,
    .WPI_UPB_PRIVATE(dense_below) = 0,
    .WPI_UPB_PRIVATE(table_mask) = -1,
    .WPI_UPB_PRIVATE(required_count) = 0,
};



// Must be last.

struct wpi_upb_DefPool {
  wpi_upb_Arena* arena;
  wpi_upb_strtable syms;   // full_name -> packed def ptr
  wpi_upb_strtable files;  // file_name -> (wpi_upb_FileDef*)
  wpi_upb_inttable exts;   // (wpi_upb_MiniTableExtension*) -> (wpi_upb_FieldDef*)
  wpi_upb_ExtensionRegistry* extreg;
  const WPI_UPB_DESC(FeatureSetDefaults) * feature_set_defaults;
  wpi_upb_MiniTablePlatform platform;
  void* scratch_data;
  size_t scratch_size;
  size_t bytes_loaded;
};

void wpi_upb_DefPool_Free(wpi_upb_DefPool* s) {
  wpi_upb_Arena_Free(s->arena);
  wpi_upb_gfree(s->scratch_data);
  wpi_upb_gfree(s);
}

static const char serialized_defaults[] = WPI_UPB_INTERNAL_WPI_UPB_EDITION_DEFAULTS;

wpi_upb_DefPool* wpi_upb_DefPool_New(void) {
  wpi_upb_DefPool* s = wpi_upb_gmalloc(sizeof(*s));
  if (!s) return NULL;

  s->arena = wpi_upb_Arena_New();
  s->bytes_loaded = 0;

  s->scratch_size = 240;
  s->scratch_data = wpi_upb_gmalloc(s->scratch_size);
  if (!s->scratch_data) goto err;

  if (!wpi_upb_strtable_init(&s->syms, 32, s->arena)) goto err;
  if (!wpi_upb_strtable_init(&s->files, 4, s->arena)) goto err;
  if (!wpi_upb_inttable_init(&s->exts, s->arena)) goto err;

  s->extreg = wpi_upb_ExtensionRegistry_New(s->arena);
  if (!s->extreg) goto err;

  s->platform = kUpb_MiniTablePlatform_Native;

  wpi_upb_Status status;
  if (!wpi_upb_DefPool_SetFeatureSetDefaults(
          s, serialized_defaults, sizeof(serialized_defaults) - 1, &status)) {
    goto err;
  }

  if (!s->feature_set_defaults) goto err;

  return s;

err:
  wpi_upb_DefPool_Free(s);
  return NULL;
}

const WPI_UPB_DESC(FeatureSetDefaults) *
    wpi_upb_DefPool_FeatureSetDefaults(const wpi_upb_DefPool* s) {
  return s->feature_set_defaults;
}

bool wpi_upb_DefPool_SetFeatureSetDefaults(wpi_upb_DefPool* s,
                                       const char* serialized_defaults,
                                       size_t serialized_len,
                                       wpi_upb_Status* status) {
  const WPI_UPB_DESC(FeatureSetDefaults)* defaults = WPI_UPB_DESC(
      FeatureSetDefaults_parse)(serialized_defaults, serialized_len, s->arena);
  if (!defaults) {
    wpi_upb_Status_SetErrorFormat(status, "Failed to parse defaults");
    return false;
  }
  if (wpi_upb_strtable_count(&s->files) > 0) {
    wpi_upb_Status_SetErrorFormat(status,
                              "Feature set defaults can't be changed once the "
                              "pool has started building");
    return false;
  }
  int min_edition = WPI_UPB_DESC(FeatureSetDefaults_minimum_edition(defaults));
  int max_edition = WPI_UPB_DESC(FeatureSetDefaults_maximum_edition(defaults));
  if (min_edition > max_edition) {
    wpi_upb_Status_SetErrorFormat(status, "Invalid edition range %s to %s",
                              wpi_upb_FileDef_EditionName(min_edition),
                              wpi_upb_FileDef_EditionName(max_edition));
    return false;
  }
  size_t size;
  const WPI_UPB_DESC(
      FeatureSetDefaults_FeatureSetEditionDefault)* const* default_list =
      WPI_UPB_DESC(FeatureSetDefaults_defaults(defaults, &size));
  int prev_edition = WPI_UPB_DESC(EDITION_UNKNOWN);
  for (size_t i = 0; i < size; ++i) {
    int edition = WPI_UPB_DESC(
        FeatureSetDefaults_FeatureSetEditionDefault_edition(default_list[i]));
    if (edition == WPI_UPB_DESC(EDITION_UNKNOWN)) {
      wpi_upb_Status_SetErrorFormat(status, "Invalid edition UNKNOWN specified");
      return false;
    }
    if (edition <= prev_edition) {
      wpi_upb_Status_SetErrorFormat(status,
                                "Feature set defaults are not strictly "
                                "increasing, %s is greater than or equal to %s",
                                wpi_upb_FileDef_EditionName(prev_edition),
                                wpi_upb_FileDef_EditionName(edition));
      return false;
    }
    prev_edition = edition;
  }

  // Copy the defaults into the pool.
  s->feature_set_defaults = defaults;
  return true;
}

bool _wpi_upb_DefPool_InsertExt(wpi_upb_DefPool* s, const wpi_upb_MiniTableExtension* ext,
                            const wpi_upb_FieldDef* f) {
  return wpi_upb_inttable_insert(&s->exts, (uintptr_t)ext, wpi_upb_value_constptr(f),
                             s->arena);
}

bool _wpi_upb_DefPool_InsertSym(wpi_upb_DefPool* s, wpi_upb_StringView sym, wpi_upb_value v,
                            wpi_upb_Status* status) {
  // TODO: table should support an operation "tryinsert" to avoid the double
  // lookup.
  if (wpi_upb_strtable_lookup2(&s->syms, sym.data, sym.size, NULL)) {
    wpi_upb_Status_SetErrorFormat(status, "duplicate symbol '%s'", sym.data);
    return false;
  }
  if (!wpi_upb_strtable_insert(&s->syms, sym.data, sym.size, v, s->arena)) {
    wpi_upb_Status_SetErrorMessage(status, "out of memory");
    return false;
  }
  return true;
}

static const void* _wpi_upb_DefPool_Unpack(const wpi_upb_DefPool* s, const char* sym,
                                       size_t size, wpi_upb_deftype_t type) {
  wpi_upb_value v;
  return wpi_upb_strtable_lookup2(&s->syms, sym, size, &v)
             ? _wpi_upb_DefType_Unpack(v, type)
             : NULL;
}

bool _wpi_upb_DefPool_LookupSym(const wpi_upb_DefPool* s, const char* sym, size_t size,
                            wpi_upb_value* v) {
  return wpi_upb_strtable_lookup2(&s->syms, sym, size, v);
}

wpi_upb_ExtensionRegistry* _wpi_upb_DefPool_ExtReg(const wpi_upb_DefPool* s) {
  return s->extreg;
}

void** _wpi_upb_DefPool_ScratchData(const wpi_upb_DefPool* s) {
  return (void**)&s->scratch_data;
}

size_t* _wpi_upb_DefPool_ScratchSize(const wpi_upb_DefPool* s) {
  return (size_t*)&s->scratch_size;
}

void _wpi_upb_DefPool_SetPlatform(wpi_upb_DefPool* s, wpi_upb_MiniTablePlatform platform) {
  assert(wpi_upb_strtable_count(&s->files) == 0);
  s->platform = platform;
}

const wpi_upb_MessageDef* wpi_upb_DefPool_FindMessageByName(const wpi_upb_DefPool* s,
                                                    const char* sym) {
  return _wpi_upb_DefPool_Unpack(s, sym, strlen(sym), WPI_UPB_DEFTYPE_MSG);
}

const wpi_upb_MessageDef* wpi_upb_DefPool_FindMessageByNameWithSize(
    const wpi_upb_DefPool* s, const char* sym, size_t len) {
  return _wpi_upb_DefPool_Unpack(s, sym, len, WPI_UPB_DEFTYPE_MSG);
}

const wpi_upb_EnumDef* wpi_upb_DefPool_FindEnumByName(const wpi_upb_DefPool* s,
                                              const char* sym) {
  return _wpi_upb_DefPool_Unpack(s, sym, strlen(sym), WPI_UPB_DEFTYPE_ENUM);
}

const wpi_upb_EnumValueDef* wpi_upb_DefPool_FindEnumByNameval(const wpi_upb_DefPool* s,
                                                      const char* sym) {
  return _wpi_upb_DefPool_Unpack(s, sym, strlen(sym), WPI_UPB_DEFTYPE_ENUMVAL);
}

const wpi_upb_FileDef* wpi_upb_DefPool_FindFileByName(const wpi_upb_DefPool* s,
                                              const char* name) {
  wpi_upb_value v;
  return wpi_upb_strtable_lookup(&s->files, name, &v) ? wpi_upb_value_getconstptr(v)
                                                  : NULL;
}

const wpi_upb_FileDef* wpi_upb_DefPool_FindFileByNameWithSize(const wpi_upb_DefPool* s,
                                                      const char* name,
                                                      size_t len) {
  wpi_upb_value v;
  return wpi_upb_strtable_lookup2(&s->files, name, len, &v)
             ? wpi_upb_value_getconstptr(v)
             : NULL;
}

const wpi_upb_FieldDef* wpi_upb_DefPool_FindExtensionByNameWithSize(
    const wpi_upb_DefPool* s, const char* name, size_t size) {
  wpi_upb_value v;
  if (!wpi_upb_strtable_lookup2(&s->syms, name, size, &v)) return NULL;

  switch (_wpi_upb_DefType_Type(v)) {
    case WPI_UPB_DEFTYPE_FIELD:
      return _wpi_upb_DefType_Unpack(v, WPI_UPB_DEFTYPE_FIELD);
    case WPI_UPB_DEFTYPE_MSG: {
      const wpi_upb_MessageDef* m = _wpi_upb_DefType_Unpack(v, WPI_UPB_DEFTYPE_MSG);
      return _wpi_upb_MessageDef_InMessageSet(m)
                 ? wpi_upb_MessageDef_NestedExtension(m, 0)
                 : NULL;
    }
    default:
      break;
  }

  return NULL;
}

const wpi_upb_FieldDef* wpi_upb_DefPool_FindExtensionByName(const wpi_upb_DefPool* s,
                                                    const char* sym) {
  return wpi_upb_DefPool_FindExtensionByNameWithSize(s, sym, strlen(sym));
}

const wpi_upb_ServiceDef* wpi_upb_DefPool_FindServiceByName(const wpi_upb_DefPool* s,
                                                    const char* name) {
  return _wpi_upb_DefPool_Unpack(s, name, strlen(name), WPI_UPB_DEFTYPE_SERVICE);
}

const wpi_upb_ServiceDef* wpi_upb_DefPool_FindServiceByNameWithSize(
    const wpi_upb_DefPool* s, const char* name, size_t size) {
  return _wpi_upb_DefPool_Unpack(s, name, size, WPI_UPB_DEFTYPE_SERVICE);
}

const wpi_upb_FileDef* wpi_upb_DefPool_FindFileContainingSymbol(const wpi_upb_DefPool* s,
                                                        const char* name) {
  wpi_upb_value v;
  // TODO: non-extension fields and oneofs.
  if (wpi_upb_strtable_lookup(&s->syms, name, &v)) {
    switch (_wpi_upb_DefType_Type(v)) {
      case WPI_UPB_DEFTYPE_EXT: {
        const wpi_upb_FieldDef* f = _wpi_upb_DefType_Unpack(v, WPI_UPB_DEFTYPE_EXT);
        return wpi_upb_FieldDef_File(f);
      }
      case WPI_UPB_DEFTYPE_MSG: {
        const wpi_upb_MessageDef* m = _wpi_upb_DefType_Unpack(v, WPI_UPB_DEFTYPE_MSG);
        return wpi_upb_MessageDef_File(m);
      }
      case WPI_UPB_DEFTYPE_ENUM: {
        const wpi_upb_EnumDef* e = _wpi_upb_DefType_Unpack(v, WPI_UPB_DEFTYPE_ENUM);
        return wpi_upb_EnumDef_File(e);
      }
      case WPI_UPB_DEFTYPE_ENUMVAL: {
        const wpi_upb_EnumValueDef* ev =
            _wpi_upb_DefType_Unpack(v, WPI_UPB_DEFTYPE_ENUMVAL);
        return wpi_upb_EnumDef_File(wpi_upb_EnumValueDef_Enum(ev));
      }
      case WPI_UPB_DEFTYPE_SERVICE: {
        const wpi_upb_ServiceDef* service =
            _wpi_upb_DefType_Unpack(v, WPI_UPB_DEFTYPE_SERVICE);
        return wpi_upb_ServiceDef_File(service);
      }
      default:
        WPI_UPB_UNREACHABLE();
    }
  }

  const char* last_dot = strrchr(name, '.');
  if (last_dot) {
    const wpi_upb_MessageDef* parent =
        wpi_upb_DefPool_FindMessageByNameWithSize(s, name, last_dot - name);
    if (parent) {
      const char* shortname = last_dot + 1;
      if (wpi_upb_MessageDef_FindByNameWithSize(parent, shortname,
                                            strlen(shortname), NULL, NULL)) {
        return wpi_upb_MessageDef_File(parent);
      }
    }
  }

  return NULL;
}

static void remove_filedef(wpi_upb_DefPool* s, wpi_upb_FileDef* file) {
  intptr_t iter = WPI_UPB_INTTABLE_BEGIN;
  wpi_upb_StringView key;
  wpi_upb_value val;
  while (wpi_upb_strtable_next2(&s->syms, &key, &val, &iter)) {
    const wpi_upb_FileDef* f;
    switch (_wpi_upb_DefType_Type(val)) {
      case WPI_UPB_DEFTYPE_EXT:
        f = wpi_upb_FieldDef_File(_wpi_upb_DefType_Unpack(val, WPI_UPB_DEFTYPE_EXT));
        break;
      case WPI_UPB_DEFTYPE_MSG:
        f = wpi_upb_MessageDef_File(_wpi_upb_DefType_Unpack(val, WPI_UPB_DEFTYPE_MSG));
        break;
      case WPI_UPB_DEFTYPE_ENUM:
        f = wpi_upb_EnumDef_File(_wpi_upb_DefType_Unpack(val, WPI_UPB_DEFTYPE_ENUM));
        break;
      case WPI_UPB_DEFTYPE_ENUMVAL:
        f = wpi_upb_EnumDef_File(wpi_upb_EnumValueDef_Enum(
            _wpi_upb_DefType_Unpack(val, WPI_UPB_DEFTYPE_ENUMVAL)));
        break;
      case WPI_UPB_DEFTYPE_SERVICE:
        f = wpi_upb_ServiceDef_File(_wpi_upb_DefType_Unpack(val, WPI_UPB_DEFTYPE_SERVICE));
        break;
      default:
        WPI_UPB_UNREACHABLE();
    }

    if (f == file) wpi_upb_strtable_removeiter(&s->syms, &iter);
  }
}

static const wpi_upb_FileDef* wpi_upb_DefBuilder_AddFileToPool(
    wpi_upb_DefBuilder* const builder, wpi_upb_DefPool* const s,
    const WPI_UPB_DESC(FileDescriptorProto) * const file_proto,
    const wpi_upb_StringView name, wpi_upb_Status* const status) {
  if (WPI_UPB_SETJMP(builder->err) != 0) {
    WPI_UPB_ASSERT(!wpi_upb_Status_IsOk(status));
    if (builder->file) {
      remove_filedef(s, builder->file);
      builder->file = NULL;
    }
  } else if (!builder->arena || !builder->tmp_arena ||
             !wpi_upb_strtable_init(&builder->feature_cache, 16,
                                builder->tmp_arena) ||
             !(builder->legacy_features =
                   WPI_UPB_DESC(FeatureSet_new)(builder->tmp_arena))) {
    _wpi_upb_DefBuilder_OomErr(builder);
  } else {
    _wpi_upb_FileDef_Create(builder, file_proto);
    wpi_upb_strtable_insert(&s->files, name.data, name.size,
                        wpi_upb_value_constptr(builder->file), builder->arena);
    WPI_UPB_ASSERT(wpi_upb_Status_IsOk(status));
    wpi_upb_Arena_Fuse(s->arena, builder->arena);
  }

  if (builder->arena) wpi_upb_Arena_Free(builder->arena);
  if (builder->tmp_arena) wpi_upb_Arena_Free(builder->tmp_arena);
  return builder->file;
}

static const wpi_upb_FileDef* _wpi_upb_DefPool_AddFile(
    wpi_upb_DefPool* s, const WPI_UPB_DESC(FileDescriptorProto) * file_proto,
    const wpi_upb_MiniTableFile* layout, wpi_upb_Status* status) {
  const wpi_upb_StringView name = WPI_UPB_DESC(FileDescriptorProto_name)(file_proto);

  // Determine whether we already know about this file.
  {
    wpi_upb_value v;
    if (wpi_upb_strtable_lookup2(&s->files, name.data, name.size, &v)) {
      wpi_upb_Status_SetErrorFormat(status,
                                "duplicate file name " WPI_UPB_STRINGVIEW_FORMAT,
                                WPI_UPB_STRINGVIEW_ARGS(name));
      return NULL;
    }
  }

  wpi_upb_DefBuilder ctx = {
      .symtab = s,
      .tmp_buf = NULL,
      .tmp_buf_size = 0,
      .layout = layout,
      .platform = s->platform,
      .msg_count = 0,
      .enum_count = 0,
      .ext_count = 0,
      .status = status,
      .file = NULL,
      .arena = wpi_upb_Arena_New(),
      .tmp_arena = wpi_upb_Arena_New(),
  };

  return wpi_upb_DefBuilder_AddFileToPool(&ctx, s, file_proto, name, status);
}

const wpi_upb_FileDef* wpi_upb_DefPool_AddFile(wpi_upb_DefPool* s,
                                       const WPI_UPB_DESC(FileDescriptorProto) *
                                           file_proto,
                                       wpi_upb_Status* status) {
  return _wpi_upb_DefPool_AddFile(s, file_proto, NULL, status);
}

bool _wpi_upb_DefPool_LoadDefInitEx(wpi_upb_DefPool* s, const _wpi_upb_DefPool_Init* init,
                                bool rebuild_minitable) {
  /* Since this function should never fail (it would indicate a bug in upb) we
   * print errors to stderr instead of returning error status to the user. */
  _wpi_upb_DefPool_Init** deps = init->deps;
  WPI_UPB_DESC(FileDescriptorProto) * file;
  wpi_upb_Arena* arena;
  wpi_upb_Status status;

  wpi_upb_Status_Clear(&status);

  if (wpi_upb_DefPool_FindFileByName(s, init->filename)) {
    return true;
  }

  arena = wpi_upb_Arena_New();

  for (; *deps; deps++) {
    if (!_wpi_upb_DefPool_LoadDefInitEx(s, *deps, rebuild_minitable)) goto err;
  }

  file = WPI_UPB_DESC(FileDescriptorProto_parse_ex)(
      init->descriptor.data, init->descriptor.size, NULL,
      kUpb_DecodeOption_AliasString, arena);
  s->bytes_loaded += init->descriptor.size;

  if (!file) {
    wpi_upb_Status_SetErrorFormat(
        &status,
        "Failed to parse compiled-in descriptor for file '%s'. This should "
        "never happen.",
        init->filename);
    goto err;
  }

  const wpi_upb_MiniTableFile* mt = rebuild_minitable ? NULL : init->layout;
  if (!_wpi_upb_DefPool_AddFile(s, file, mt, &status)) {
    goto err;
  }

  wpi_upb_Arena_Free(arena);
  return true;

err:
  fprintf(stderr,
          "Error loading compiled-in descriptor for file '%s' (this should "
          "never happen): %s\n",
          init->filename, wpi_upb_Status_ErrorMessage(&status));
  wpi_upb_Arena_Free(arena);
  return false;
}

size_t _wpi_upb_DefPool_BytesLoaded(const wpi_upb_DefPool* s) {
  return s->bytes_loaded;
}

wpi_upb_Arena* _wpi_upb_DefPool_Arena(const wpi_upb_DefPool* s) { return s->arena; }

const wpi_upb_FieldDef* wpi_upb_DefPool_FindExtensionByMiniTable(
    const wpi_upb_DefPool* s, const wpi_upb_MiniTableExtension* ext) {
  wpi_upb_value v;
  bool ok = wpi_upb_inttable_lookup(&s->exts, (uintptr_t)ext, &v);
  WPI_UPB_ASSERT(ok);
  return wpi_upb_value_getconstptr(v);
}

const wpi_upb_FieldDef* wpi_upb_DefPool_FindExtensionByNumber(const wpi_upb_DefPool* s,
                                                      const wpi_upb_MessageDef* m,
                                                      int32_t fieldnum) {
  const wpi_upb_MiniTable* t = wpi_upb_MessageDef_MiniTable(m);
  const wpi_upb_MiniTableExtension* ext =
      wpi_upb_ExtensionRegistry_Lookup(s->extreg, t, fieldnum);
  return ext ? wpi_upb_DefPool_FindExtensionByMiniTable(s, ext) : NULL;
}

const wpi_upb_ExtensionRegistry* wpi_upb_DefPool_ExtensionRegistry(
    const wpi_upb_DefPool* s) {
  return s->extreg;
}

const wpi_upb_FieldDef** wpi_upb_DefPool_GetAllExtensions(const wpi_upb_DefPool* s,
                                                  const wpi_upb_MessageDef* m,
                                                  size_t* count) {
  size_t n = 0;
  intptr_t iter = WPI_UPB_INTTABLE_BEGIN;
  uintptr_t key;
  wpi_upb_value val;
  // This is O(all exts) instead of O(exts for m).  If we need this to be
  // efficient we may need to make extreg into a two-level table, or have a
  // second per-message index.
  while (wpi_upb_inttable_next(&s->exts, &key, &val, &iter)) {
    const wpi_upb_FieldDef* f = wpi_upb_value_getconstptr(val);
    if (wpi_upb_FieldDef_ContainingType(f) == m) n++;
  }
  const wpi_upb_FieldDef** exts = wpi_upb_gmalloc(n * sizeof(*exts));
  iter = WPI_UPB_INTTABLE_BEGIN;
  size_t i = 0;
  while (wpi_upb_inttable_next(&s->exts, &key, &val, &iter)) {
    const wpi_upb_FieldDef* f = wpi_upb_value_getconstptr(val);
    if (wpi_upb_FieldDef_ContainingType(f) == m) exts[i++] = f;
  }
  *count = n;
  return exts;
}

bool _wpi_upb_DefPool_LoadDefInit(wpi_upb_DefPool* s, const _wpi_upb_DefPool_Init* init) {
  return _wpi_upb_DefPool_LoadDefInitEx(s, init, false);
}


// Must be last.

wpi_upb_deftype_t _wpi_upb_DefType_Type(wpi_upb_value v) {
  const uintptr_t num = (uintptr_t)wpi_upb_value_getconstptr(v);
  return num & WPI_UPB_DEFTYPE_MASK;
}

wpi_upb_value _wpi_upb_DefType_Pack(const void* ptr, wpi_upb_deftype_t type) {
  uintptr_t num = (uintptr_t)ptr;
  WPI_UPB_ASSERT((num & WPI_UPB_DEFTYPE_MASK) == 0);
  num |= type;
  return wpi_upb_value_constptr((const void*)num);
}

const void* _wpi_upb_DefType_Unpack(wpi_upb_value v, wpi_upb_deftype_t type) {
  uintptr_t num = (uintptr_t)wpi_upb_value_getconstptr(v);
  return (num & WPI_UPB_DEFTYPE_MASK) == type
             ? (const void*)(num & ~WPI_UPB_DEFTYPE_MASK)
             : NULL;
}


// Must be last.

bool _wpi_upb_DescState_Grow(wpi_upb_DescState* d, wpi_upb_Arena* a) {
  const size_t oldbufsize = d->bufsize;
  const int used = d->ptr - d->buf;

  if (!d->buf) {
    d->buf = wpi_upb_Arena_Malloc(a, d->bufsize);
    if (!d->buf) return false;
    d->ptr = d->buf;
    d->e.end = d->buf + d->bufsize;
  }

  if (oldbufsize - used < kUpb_MtDataEncoder_MinSize) {
    d->bufsize *= 2;
    d->buf = wpi_upb_Arena_Realloc(a, d->buf, oldbufsize, d->bufsize);
    if (!d->buf) return false;
    d->ptr = d->buf + used;
    d->e.end = d->buf + d->bufsize;
  }

  return true;
}


#include <stddef.h>
#include <stdint.h>
#include <string.h>


// Must be last.

struct wpi_upb_EnumDef {
  const WPI_UPB_DESC(EnumOptions*) opts;
  const WPI_UPB_DESC(FeatureSet*) resolved_features;
  const wpi_upb_MiniTableEnum* layout;  // Only for proto2.
  const wpi_upb_FileDef* file;
  const wpi_upb_MessageDef* containing_type;  // Could be merged with "file".
  const char* full_name;
  wpi_upb_strtable ntoi;
  wpi_upb_inttable iton;
  const wpi_upb_EnumValueDef* values;
  const wpi_upb_EnumReservedRange* res_ranges;
  const wpi_upb_StringView* res_names;
  int value_count;
  int res_range_count;
  int res_name_count;
  int32_t defaultval;
  bool is_sorted;  // Whether all of the values are defined in ascending order.
#if UINTPTR_MAX == 0xffffffff
  uint32_t padding;  // Increase size to a multiple of 8.
#endif
};

wpi_upb_EnumDef* _wpi_upb_EnumDef_At(const wpi_upb_EnumDef* e, int i) {
  return (wpi_upb_EnumDef*)&e[i];
}

const wpi_upb_MiniTableEnum* _wpi_upb_EnumDef_MiniTable(const wpi_upb_EnumDef* e) {
  return e->layout;
}

bool _wpi_upb_EnumDef_Insert(wpi_upb_EnumDef* e, wpi_upb_EnumValueDef* v, wpi_upb_Arena* a) {
  const char* name = wpi_upb_EnumValueDef_Name(v);
  const wpi_upb_value val = wpi_upb_value_constptr(v);
  bool ok = wpi_upb_strtable_insert(&e->ntoi, name, strlen(name), val, a);
  if (!ok) return false;

  // Multiple enumerators can have the same number, first one wins.
  const int number = wpi_upb_EnumValueDef_Number(v);
  if (!wpi_upb_inttable_lookup(&e->iton, number, NULL)) {
    return wpi_upb_inttable_insert(&e->iton, number, val, a);
  }
  return true;
}

const WPI_UPB_DESC(EnumOptions) * wpi_upb_EnumDef_Options(const wpi_upb_EnumDef* e) {
  return e->opts;
}

bool wpi_upb_EnumDef_HasOptions(const wpi_upb_EnumDef* e) {
  return e->opts != (void*)kUpbDefOptDefault;
}

const WPI_UPB_DESC(FeatureSet) *
    wpi_upb_EnumDef_ResolvedFeatures(const wpi_upb_EnumDef* e) {
  return e->resolved_features;
}

const char* wpi_upb_EnumDef_FullName(const wpi_upb_EnumDef* e) { return e->full_name; }

const char* wpi_upb_EnumDef_Name(const wpi_upb_EnumDef* e) {
  return _wpi_upb_DefBuilder_FullToShort(e->full_name);
}

const wpi_upb_FileDef* wpi_upb_EnumDef_File(const wpi_upb_EnumDef* e) { return e->file; }

const wpi_upb_MessageDef* wpi_upb_EnumDef_ContainingType(const wpi_upb_EnumDef* e) {
  return e->containing_type;
}

int32_t wpi_upb_EnumDef_Default(const wpi_upb_EnumDef* e) {
  WPI_UPB_ASSERT(wpi_upb_EnumDef_FindValueByNumber(e, e->defaultval));
  return e->defaultval;
}

int wpi_upb_EnumDef_ReservedRangeCount(const wpi_upb_EnumDef* e) {
  return e->res_range_count;
}

const wpi_upb_EnumReservedRange* wpi_upb_EnumDef_ReservedRange(const wpi_upb_EnumDef* e,
                                                       int i) {
  WPI_UPB_ASSERT(0 <= i && i < e->res_range_count);
  return _wpi_upb_EnumReservedRange_At(e->res_ranges, i);
}

int wpi_upb_EnumDef_ReservedNameCount(const wpi_upb_EnumDef* e) {
  return e->res_name_count;
}

wpi_upb_StringView wpi_upb_EnumDef_ReservedName(const wpi_upb_EnumDef* e, int i) {
  WPI_UPB_ASSERT(0 <= i && i < e->res_name_count);
  return e->res_names[i];
}

int wpi_upb_EnumDef_ValueCount(const wpi_upb_EnumDef* e) { return e->value_count; }

const wpi_upb_EnumValueDef* wpi_upb_EnumDef_FindValueByName(const wpi_upb_EnumDef* e,
                                                    const char* name) {
  return wpi_upb_EnumDef_FindValueByNameWithSize(e, name, strlen(name));
}

const wpi_upb_EnumValueDef* wpi_upb_EnumDef_FindValueByNameWithSize(
    const wpi_upb_EnumDef* e, const char* name, size_t size) {
  wpi_upb_value v;
  return wpi_upb_strtable_lookup2(&e->ntoi, name, size, &v)
             ? wpi_upb_value_getconstptr(v)
             : NULL;
}

const wpi_upb_EnumValueDef* wpi_upb_EnumDef_FindValueByNumber(const wpi_upb_EnumDef* e,
                                                      int32_t num) {
  wpi_upb_value v;
  return wpi_upb_inttable_lookup(&e->iton, num, &v) ? wpi_upb_value_getconstptr(v)
                                                : NULL;
}

bool wpi_upb_EnumDef_CheckNumber(const wpi_upb_EnumDef* e, int32_t num) {
  // We could use wpi_upb_EnumDef_FindValueByNumber(e, num) != NULL, but we expect
  // this to be faster (especially for small numbers).
  return wpi_upb_MiniTableEnum_CheckValue(e->layout, num);
}

const wpi_upb_EnumValueDef* wpi_upb_EnumDef_Value(const wpi_upb_EnumDef* e, int i) {
  WPI_UPB_ASSERT(0 <= i && i < e->value_count);
  return _wpi_upb_EnumValueDef_At(e->values, i);
}

bool wpi_upb_EnumDef_IsClosed(const wpi_upb_EnumDef* e) {
  if (WPI_UPB_TREAT_CLOSED_ENUMS_LIKE_OPEN) return false;
  return wpi_upb_EnumDef_IsSpecifiedAsClosed(e);
}

bool wpi_upb_EnumDef_IsSpecifiedAsClosed(const wpi_upb_EnumDef* e) {
  return WPI_UPB_DESC(FeatureSet_enum_type)(e->resolved_features) ==
         WPI_UPB_DESC(FeatureSet_CLOSED);
}

bool wpi_upb_EnumDef_MiniDescriptorEncode(const wpi_upb_EnumDef* e, wpi_upb_Arena* a,
                                      wpi_upb_StringView* out) {
  wpi_upb_DescState s;
  _wpi_upb_DescState_Init(&s);

  const wpi_upb_EnumValueDef** sorted = NULL;
  if (!e->is_sorted) {
    sorted = _wpi_upb_EnumValueDefs_Sorted(e->values, e->value_count, a);
    if (!sorted) return false;
  }

  if (!_wpi_upb_DescState_Grow(&s, a)) return false;
  s.ptr = wpi_upb_MtDataEncoder_StartEnum(&s.e, s.ptr);

  // Duplicate values are allowed but we only encode each value once.
  uint32_t previous = 0;

  for (int i = 0; i < e->value_count; i++) {
    const uint32_t current =
        wpi_upb_EnumValueDef_Number(sorted ? sorted[i] : wpi_upb_EnumDef_Value(e, i));
    if (i != 0 && previous == current) continue;

    if (!_wpi_upb_DescState_Grow(&s, a)) return false;
    s.ptr = wpi_upb_MtDataEncoder_PutEnumValue(&s.e, s.ptr, current);
    previous = current;
  }

  if (!_wpi_upb_DescState_Grow(&s, a)) return false;
  s.ptr = wpi_upb_MtDataEncoder_EndEnum(&s.e, s.ptr);

  // There will always be room for this '\0' in the encoder buffer because
  // kUpb_MtDataEncoder_MinSize is overkill for wpi_upb_MtDataEncoder_EndEnum().
  WPI_UPB_ASSERT(s.ptr < s.buf + s.bufsize);
  *s.ptr = '\0';

  out->data = s.buf;
  out->size = s.ptr - s.buf;
  return true;
}

static wpi_upb_MiniTableEnum* create_enumlayout(wpi_upb_DefBuilder* ctx,
                                            const wpi_upb_EnumDef* e) {
  wpi_upb_StringView sv;
  bool ok = wpi_upb_EnumDef_MiniDescriptorEncode(e, ctx->tmp_arena, &sv);
  if (!ok) _wpi_upb_DefBuilder_Errf(ctx, "OOM while building enum MiniDescriptor");

  wpi_upb_Status status;
  wpi_upb_MiniTableEnum* layout =
      wpi_upb_MiniTableEnum_Build(sv.data, sv.size, ctx->arena, &status);
  if (!layout)
    _wpi_upb_DefBuilder_Errf(ctx, "Error building enum MiniTable: %s", status.msg);
  return layout;
}

static wpi_upb_StringView* _wpi_upb_EnumReservedNames_New(
    wpi_upb_DefBuilder* ctx, int n, const wpi_upb_StringView* protos) {
  wpi_upb_StringView* sv = _wpi_upb_DefBuilder_Alloc(ctx, sizeof(wpi_upb_StringView) * n);
  for (int i = 0; i < n; i++) {
    sv[i].data =
        wpi_upb_strdup2(protos[i].data, protos[i].size, _wpi_upb_DefBuilder_Arena(ctx));
    sv[i].size = protos[i].size;
  }
  return sv;
}

static void create_enumdef(wpi_upb_DefBuilder* ctx, const char* prefix,
                           const WPI_UPB_DESC(EnumDescriptorProto) * enum_proto,
                           const WPI_UPB_DESC(FeatureSet*) parent_features,
                           wpi_upb_EnumDef* e) {
  const WPI_UPB_DESC(EnumValueDescriptorProto)* const* values;
  const WPI_UPB_DESC(EnumDescriptorProto_EnumReservedRange)* const* res_ranges;
  const wpi_upb_StringView* res_names;
  wpi_upb_StringView name;
  size_t n_value, n_res_range, n_res_name;

  WPI_UPB_DEF_SET_OPTIONS(e->opts, EnumDescriptorProto, EnumOptions, enum_proto);
  e->resolved_features = _wpi_upb_DefBuilder_ResolveFeatures(
      ctx, parent_features, WPI_UPB_DESC(EnumOptions_features)(e->opts));

  // Must happen before _wpi_upb_DefBuilder_Add()
  e->file = _wpi_upb_DefBuilder_File(ctx);

  name = WPI_UPB_DESC(EnumDescriptorProto_name)(enum_proto);

  e->full_name = _wpi_upb_DefBuilder_MakeFullName(ctx, prefix, name);
  _wpi_upb_DefBuilder_Add(ctx, e->full_name,
                      _wpi_upb_DefType_Pack(e, WPI_UPB_DEFTYPE_ENUM));

  values = WPI_UPB_DESC(EnumDescriptorProto_value)(enum_proto, &n_value);

  bool ok = wpi_upb_strtable_init(&e->ntoi, n_value, ctx->arena);
  if (!ok) _wpi_upb_DefBuilder_OomErr(ctx);

  ok = wpi_upb_inttable_init(&e->iton, ctx->arena);
  if (!ok) _wpi_upb_DefBuilder_OomErr(ctx);

  e->defaultval = 0;
  e->value_count = n_value;
  e->values = _wpi_upb_EnumValueDefs_New(ctx, prefix, n_value, values,
                                     e->resolved_features, e, &e->is_sorted);

  if (n_value == 0) {
    _wpi_upb_DefBuilder_Errf(ctx, "enums must contain at least one value (%s)",
                         e->full_name);
  }

  res_ranges =
      WPI_UPB_DESC(EnumDescriptorProto_reserved_range)(enum_proto, &n_res_range);
  e->res_range_count = n_res_range;
  e->res_ranges = _wpi_upb_EnumReservedRanges_New(ctx, n_res_range, res_ranges, e);

  res_names =
      WPI_UPB_DESC(EnumDescriptorProto_reserved_name)(enum_proto, &n_res_name);
  e->res_name_count = n_res_name;
  e->res_names = _wpi_upb_EnumReservedNames_New(ctx, n_res_name, res_names);

  wpi_upb_inttable_compact(&e->iton, ctx->arena);

  if (wpi_upb_EnumDef_IsClosed(e)) {
    if (ctx->layout) {
      e->layout = wpi_upb_MiniTableFile_Enum(ctx->layout, ctx->enum_count++);
    } else {
      e->layout = create_enumlayout(ctx, e);
    }
  } else {
    e->layout = NULL;
  }
}

wpi_upb_EnumDef* _wpi_upb_EnumDefs_New(wpi_upb_DefBuilder* ctx, int n,
                               const WPI_UPB_DESC(EnumDescriptorProto*)
                                   const* protos,
                               const WPI_UPB_DESC(FeatureSet*) parent_features,
                               const wpi_upb_MessageDef* containing_type) {
  _wpi_upb_DefType_CheckPadding(sizeof(wpi_upb_EnumDef));

  // If a containing type is defined then get the full name from that.
  // Otherwise use the package name from the file def.
  const char* name = containing_type ? wpi_upb_MessageDef_FullName(containing_type)
                                     : _wpi_upb_FileDef_RawPackage(ctx->file);

  wpi_upb_EnumDef* e = _wpi_upb_DefBuilder_Alloc(ctx, sizeof(wpi_upb_EnumDef) * n);
  for (int i = 0; i < n; i++) {
    create_enumdef(ctx, name, protos[i], parent_features, &e[i]);
    e[i].containing_type = containing_type;
  }
  return e;
}



// Must be last.

struct wpi_upb_EnumReservedRange {
  int32_t start;
  int32_t end;
};

wpi_upb_EnumReservedRange* _wpi_upb_EnumReservedRange_At(const wpi_upb_EnumReservedRange* r,
                                                 int i) {
  return (wpi_upb_EnumReservedRange*)&r[i];
}

int32_t wpi_upb_EnumReservedRange_Start(const wpi_upb_EnumReservedRange* r) {
  return r->start;
}
int32_t wpi_upb_EnumReservedRange_End(const wpi_upb_EnumReservedRange* r) {
  return r->end;
}

wpi_upb_EnumReservedRange* _wpi_upb_EnumReservedRanges_New(
    wpi_upb_DefBuilder* ctx, int n,
    const WPI_UPB_DESC(EnumDescriptorProto_EnumReservedRange) * const* protos,
    const wpi_upb_EnumDef* e) {
  wpi_upb_EnumReservedRange* r =
      _wpi_upb_DefBuilder_Alloc(ctx, sizeof(wpi_upb_EnumReservedRange) * n);

  for (int i = 0; i < n; i++) {
    const int32_t start =
        WPI_UPB_DESC(EnumDescriptorProto_EnumReservedRange_start)(protos[i]);
    const int32_t end =
        WPI_UPB_DESC(EnumDescriptorProto_EnumReservedRange_end)(protos[i]);

    // A full validation would also check that each range is disjoint, and that
    // none of the fields overlap with the extension ranges, but we are just
    // sanity checking here.

    // Note: Not a typo! Unlike extension ranges and message reserved ranges,
    // the end value of an enum reserved range is *inclusive*!
    if (end < start) {
      _wpi_upb_DefBuilder_Errf(ctx, "Reserved range (%d, %d) is invalid, enum=%s\n",
                           (int)start, (int)end, wpi_upb_EnumDef_FullName(e));
    }

    r[i].start = start;
    r[i].end = end;
  }

  return r;
}


#include <stdint.h>


// Must be last.

struct wpi_upb_EnumValueDef {
  const WPI_UPB_DESC(EnumValueOptions*) opts;
  const WPI_UPB_DESC(FeatureSet*) resolved_features;
  const wpi_upb_EnumDef* parent;
  const char* full_name;
  int32_t number;
#if UINTPTR_MAX == 0xffffffff
  uint32_t padding;  // Increase size to a multiple of 8.
#endif
};

wpi_upb_EnumValueDef* _wpi_upb_EnumValueDef_At(const wpi_upb_EnumValueDef* v, int i) {
  return (wpi_upb_EnumValueDef*)&v[i];
}

static int _wpi_upb_EnumValueDef_Compare(const void* p1, const void* p2) {
  const uint32_t v1 = (*(const wpi_upb_EnumValueDef**)p1)->number;
  const uint32_t v2 = (*(const wpi_upb_EnumValueDef**)p2)->number;
  return (v1 < v2) ? -1 : (v1 > v2);
}

const wpi_upb_EnumValueDef** _wpi_upb_EnumValueDefs_Sorted(const wpi_upb_EnumValueDef* v,
                                                   int n, wpi_upb_Arena* a) {
  // TODO: Try to replace this arena alloc with a persistent scratch buffer.
  wpi_upb_EnumValueDef** out =
      (wpi_upb_EnumValueDef**)wpi_upb_Arena_Malloc(a, n * sizeof(void*));
  if (!out) return NULL;

  for (int i = 0; i < n; i++) {
    out[i] = (wpi_upb_EnumValueDef*)&v[i];
  }
  qsort(out, n, sizeof(void*), _wpi_upb_EnumValueDef_Compare);

  return (const wpi_upb_EnumValueDef**)out;
}

const WPI_UPB_DESC(EnumValueOptions) *
    wpi_upb_EnumValueDef_Options(const wpi_upb_EnumValueDef* v) {
  return v->opts;
}

bool wpi_upb_EnumValueDef_HasOptions(const wpi_upb_EnumValueDef* v) {
  return v->opts != (void*)kUpbDefOptDefault;
}

const WPI_UPB_DESC(FeatureSet) *
    wpi_upb_EnumValueDef_ResolvedFeatures(const wpi_upb_EnumValueDef* e) {
  return e->resolved_features;
}

const wpi_upb_EnumDef* wpi_upb_EnumValueDef_Enum(const wpi_upb_EnumValueDef* v) {
  return v->parent;
}

const char* wpi_upb_EnumValueDef_FullName(const wpi_upb_EnumValueDef* v) {
  return v->full_name;
}

const char* wpi_upb_EnumValueDef_Name(const wpi_upb_EnumValueDef* v) {
  return _wpi_upb_DefBuilder_FullToShort(v->full_name);
}

int32_t wpi_upb_EnumValueDef_Number(const wpi_upb_EnumValueDef* v) { return v->number; }

uint32_t wpi_upb_EnumValueDef_Index(const wpi_upb_EnumValueDef* v) {
  // Compute index in our parent's array.
  return v - wpi_upb_EnumDef_Value(v->parent, 0);
}

static void create_enumvaldef(wpi_upb_DefBuilder* ctx, const char* prefix,
                              const WPI_UPB_DESC(EnumValueDescriptorProto*)
                                  val_proto,
                              const WPI_UPB_DESC(FeatureSet*) parent_features,
                              wpi_upb_EnumDef* e, wpi_upb_EnumValueDef* v) {
  WPI_UPB_DEF_SET_OPTIONS(v->opts, EnumValueDescriptorProto, EnumValueOptions,
                      val_proto);
  v->resolved_features = _wpi_upb_DefBuilder_ResolveFeatures(
      ctx, parent_features, WPI_UPB_DESC(EnumValueOptions_features)(v->opts));

  wpi_upb_StringView name = WPI_UPB_DESC(EnumValueDescriptorProto_name)(val_proto);

  v->parent = e;  // Must happen prior to _wpi_upb_DefBuilder_Add()
  v->full_name = _wpi_upb_DefBuilder_MakeFullName(ctx, prefix, name);
  v->number = WPI_UPB_DESC(EnumValueDescriptorProto_number)(val_proto);
  _wpi_upb_DefBuilder_Add(ctx, v->full_name,
                      _wpi_upb_DefType_Pack(v, WPI_UPB_DEFTYPE_ENUMVAL));

  bool ok = _wpi_upb_EnumDef_Insert(e, v, ctx->arena);
  if (!ok) _wpi_upb_DefBuilder_OomErr(ctx);
}

static void _wpi_upb_EnumValueDef_CheckZeroValue(wpi_upb_DefBuilder* ctx,
                                             const wpi_upb_EnumDef* e,
                                             const wpi_upb_EnumValueDef* v, int n) {
  // When the special WPI_UPB_TREAT_CLOSED_ENUMS_LIKE_OPEN is enabled, we have to
  // exempt closed enums from this check, even when we are treating them as
  // open.
  if (wpi_upb_EnumDef_IsSpecifiedAsClosed(e) || n == 0 || v[0].number == 0) return;

  _wpi_upb_DefBuilder_Errf(ctx, "for open enums, the first value must be zero (%s)",
                       wpi_upb_EnumDef_FullName(e));
}

// Allocate and initialize an array of |n| enum value defs owned by |e|.
wpi_upb_EnumValueDef* _wpi_upb_EnumValueDefs_New(
    wpi_upb_DefBuilder* ctx, const char* prefix, int n,
    const WPI_UPB_DESC(EnumValueDescriptorProto*) const* protos,
    const WPI_UPB_DESC(FeatureSet*) parent_features, wpi_upb_EnumDef* e,
    bool* is_sorted) {
  _wpi_upb_DefType_CheckPadding(sizeof(wpi_upb_EnumValueDef));

  wpi_upb_EnumValueDef* v =
      _wpi_upb_DefBuilder_Alloc(ctx, sizeof(wpi_upb_EnumValueDef) * n);

  *is_sorted = true;
  uint32_t previous = 0;
  for (int i = 0; i < n; i++) {
    create_enumvaldef(ctx, prefix, protos[i], parent_features, e, &v[i]);

    const uint32_t current = v[i].number;
    if (previous > current) *is_sorted = false;
    previous = current;
  }

  _wpi_upb_EnumValueDef_CheckZeroValue(ctx, e, v, n);

  return v;
}


#include <stdint.h>


// Must be last.

struct wpi_upb_ExtensionRange {
  const WPI_UPB_DESC(ExtensionRangeOptions*) opts;
  const WPI_UPB_DESC(FeatureSet*) resolved_features;
  int32_t start;
  int32_t end;
};

wpi_upb_ExtensionRange* _wpi_upb_ExtensionRange_At(const wpi_upb_ExtensionRange* r, int i) {
  return (wpi_upb_ExtensionRange*)&r[i];
}

const WPI_UPB_DESC(ExtensionRangeOptions) *
    wpi_upb_ExtensionRange_Options(const wpi_upb_ExtensionRange* r) {
  return r->opts;
}

bool wpi_upb_ExtensionRange_HasOptions(const wpi_upb_ExtensionRange* r) {
  return r->opts != (void*)kUpbDefOptDefault;
}

int32_t wpi_upb_ExtensionRange_Start(const wpi_upb_ExtensionRange* r) {
  return r->start;
}

int32_t wpi_upb_ExtensionRange_End(const wpi_upb_ExtensionRange* r) { return r->end; }

wpi_upb_ExtensionRange* _wpi_upb_ExtensionRanges_New(
    wpi_upb_DefBuilder* ctx, int n,
    const WPI_UPB_DESC(DescriptorProto_ExtensionRange*) const* protos,
    const WPI_UPB_DESC(FeatureSet*) parent_features, const wpi_upb_MessageDef* m) {
  wpi_upb_ExtensionRange* r =
      _wpi_upb_DefBuilder_Alloc(ctx, sizeof(wpi_upb_ExtensionRange) * n);

  for (int i = 0; i < n; i++) {
    WPI_UPB_DEF_SET_OPTIONS(r[i].opts, DescriptorProto_ExtensionRange,
                        ExtensionRangeOptions, protos[i]);
    r[i].resolved_features = _wpi_upb_DefBuilder_ResolveFeatures(
        ctx, parent_features,
        WPI_UPB_DESC(ExtensionRangeOptions_features)(r[i].opts));

    const int32_t start =
        WPI_UPB_DESC(DescriptorProto_ExtensionRange_start)(protos[i]);
    const int32_t end = WPI_UPB_DESC(DescriptorProto_ExtensionRange_end)(protos[i]);
    const int32_t max = WPI_UPB_DESC(MessageOptions_message_set_wire_format)(
                            wpi_upb_MessageDef_Options(m))
                            ? INT32_MAX
                            : kUpb_MaxFieldNumber + 1;

    // A full validation would also check that each range is disjoint, and that
    // none of the fields overlap with the extension ranges, but we are just
    // sanity checking here.
    if (start < 1 || end <= start || end > max) {
      _wpi_upb_DefBuilder_Errf(ctx,
                           "Extension range (%d, %d) is invalid, message=%s\n",
                           (int)start, (int)end, wpi_upb_MessageDef_FullName(m));
    }

    r[i].start = start;
    r[i].end = end;
  }

  return r;
}


#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


// Must be last.

#define WPI_UPB_FIELD_TYPE_UNSPECIFIED 0

typedef struct {
  size_t len;
  char str[1];  // Null-terminated string data follows.
} str_t;

struct wpi_upb_FieldDef {
  const WPI_UPB_DESC(FieldOptions*) opts;
  const WPI_UPB_DESC(FeatureSet*) resolved_features;
  const wpi_upb_FileDef* file;
  const wpi_upb_MessageDef* msgdef;
  const char* full_name;
  const char* json_name;
  union {
    int64_t sint;
    uint64_t uint;
    double dbl;
    float flt;
    bool boolean;
    str_t* str;
    void* msg;  // Always NULL.
  } defaultval;
  union {
    const wpi_upb_OneofDef* oneof;
    const wpi_upb_MessageDef* extension_scope;
  } scope;
  union {
    const wpi_upb_MessageDef* msgdef;
    const wpi_upb_EnumDef* enumdef;
    const WPI_UPB_DESC(FieldDescriptorProto) * unresolved;
  } sub;
  uint32_t number_;
  uint16_t index_;
  uint16_t layout_index;  // Index into msgdef->layout->fields or file->exts
  bool has_default;
  bool has_json_name;
  bool has_presence;
  bool is_extension;
  bool is_proto3_optional;
  wpi_upb_FieldType type_;
  wpi_upb_Label label_;
};

wpi_upb_FieldDef* _wpi_upb_FieldDef_At(const wpi_upb_FieldDef* f, int i) {
  return (wpi_upb_FieldDef*)&f[i];
}

const WPI_UPB_DESC(FieldOptions) * wpi_upb_FieldDef_Options(const wpi_upb_FieldDef* f) {
  return f->opts;
}

bool wpi_upb_FieldDef_HasOptions(const wpi_upb_FieldDef* f) {
  return f->opts != (void*)kUpbDefOptDefault;
}

const WPI_UPB_DESC(FeatureSet) *
    wpi_upb_FieldDef_ResolvedFeatures(const wpi_upb_FieldDef* f) {
  return f->resolved_features;
}

const char* wpi_upb_FieldDef_FullName(const wpi_upb_FieldDef* f) {
  return f->full_name;
}

wpi_upb_CType wpi_upb_FieldDef_CType(const wpi_upb_FieldDef* f) {
  return wpi_upb_FieldType_CType(f->type_);
}

wpi_upb_FieldType wpi_upb_FieldDef_Type(const wpi_upb_FieldDef* f) { return f->type_; }

uint32_t wpi_upb_FieldDef_Index(const wpi_upb_FieldDef* f) { return f->index_; }

uint32_t wpi_upb_FieldDef_LayoutIndex(const wpi_upb_FieldDef* f) {
  return f->layout_index;
}

wpi_upb_Label wpi_upb_FieldDef_Label(const wpi_upb_FieldDef* f) { return f->label_; }

uint32_t wpi_upb_FieldDef_Number(const wpi_upb_FieldDef* f) { return f->number_; }

bool wpi_upb_FieldDef_IsExtension(const wpi_upb_FieldDef* f) { return f->is_extension; }

bool _wpi_upb_FieldDef_IsPackable(const wpi_upb_FieldDef* f) {
  return wpi_upb_FieldDef_IsRepeated(f) && wpi_upb_FieldDef_IsPrimitive(f);
}

bool wpi_upb_FieldDef_IsPacked(const wpi_upb_FieldDef* f) {
  return _wpi_upb_FieldDef_IsPackable(f) &&
         WPI_UPB_DESC(FeatureSet_repeated_field_encoding(f->resolved_features)) ==
             WPI_UPB_DESC(FeatureSet_PACKED);
}

const char* wpi_upb_FieldDef_Name(const wpi_upb_FieldDef* f) {
  return _wpi_upb_DefBuilder_FullToShort(f->full_name);
}

const char* wpi_upb_FieldDef_JsonName(const wpi_upb_FieldDef* f) {
  return f->json_name;
}

bool wpi_upb_FieldDef_HasJsonName(const wpi_upb_FieldDef* f) {
  return f->has_json_name;
}

const wpi_upb_FileDef* wpi_upb_FieldDef_File(const wpi_upb_FieldDef* f) { return f->file; }

const wpi_upb_MessageDef* wpi_upb_FieldDef_ContainingType(const wpi_upb_FieldDef* f) {
  return f->msgdef;
}

const wpi_upb_MessageDef* wpi_upb_FieldDef_ExtensionScope(const wpi_upb_FieldDef* f) {
  return f->is_extension ? f->scope.extension_scope : NULL;
}

const wpi_upb_OneofDef* wpi_upb_FieldDef_ContainingOneof(const wpi_upb_FieldDef* f) {
  return f->is_extension ? NULL : f->scope.oneof;
}

const wpi_upb_OneofDef* wpi_upb_FieldDef_RealContainingOneof(const wpi_upb_FieldDef* f) {
  const wpi_upb_OneofDef* oneof = wpi_upb_FieldDef_ContainingOneof(f);
  if (!oneof || wpi_upb_OneofDef_IsSynthetic(oneof)) return NULL;
  return oneof;
}

wpi_upb_MessageValue wpi_upb_FieldDef_Default(const wpi_upb_FieldDef* f) {
  wpi_upb_MessageValue ret;

  if (wpi_upb_FieldDef_IsRepeated(f) || wpi_upb_FieldDef_IsSubMessage(f)) {
    return (wpi_upb_MessageValue){.msg_val = NULL};
  }

  switch (wpi_upb_FieldDef_CType(f)) {
    case kUpb_CType_Bool:
      return (wpi_upb_MessageValue){.bool_val = f->defaultval.boolean};
    case kUpb_CType_Int64:
      return (wpi_upb_MessageValue){.int64_val = f->defaultval.sint};
    case kUpb_CType_UInt64:
      return (wpi_upb_MessageValue){.uint64_val = f->defaultval.uint};
    case kUpb_CType_Enum:
    case kUpb_CType_Int32:
      return (wpi_upb_MessageValue){.int32_val = (int32_t)f->defaultval.sint};
    case kUpb_CType_UInt32:
      return (wpi_upb_MessageValue){.uint32_val = (uint32_t)f->defaultval.uint};
    case kUpb_CType_Float:
      return (wpi_upb_MessageValue){.float_val = f->defaultval.flt};
    case kUpb_CType_Double:
      return (wpi_upb_MessageValue){.double_val = f->defaultval.dbl};
    case kUpb_CType_String:
    case kUpb_CType_Bytes: {
      str_t* str = f->defaultval.str;
      if (str) {
        return (wpi_upb_MessageValue){
            .str_val = (wpi_upb_StringView){.data = str->str, .size = str->len}};
      } else {
        return (wpi_upb_MessageValue){
            .str_val = (wpi_upb_StringView){.data = NULL, .size = 0}};
      }
    }
    default:
      WPI_UPB_UNREACHABLE();
  }

  return ret;
}

const wpi_upb_MessageDef* wpi_upb_FieldDef_MessageSubDef(const wpi_upb_FieldDef* f) {
  return wpi_upb_FieldDef_IsSubMessage(f) ? f->sub.msgdef : NULL;
}

const wpi_upb_EnumDef* wpi_upb_FieldDef_EnumSubDef(const wpi_upb_FieldDef* f) {
  return wpi_upb_FieldDef_IsEnum(f) ? f->sub.enumdef : NULL;
}

const wpi_upb_MiniTableField* wpi_upb_FieldDef_MiniTable(const wpi_upb_FieldDef* f) {
  if (wpi_upb_FieldDef_IsExtension(f)) {
    const wpi_upb_FileDef* file = wpi_upb_FieldDef_File(f);
    return (wpi_upb_MiniTableField*)_wpi_upb_FileDef_ExtensionMiniTable(
        file, f->layout_index);
  } else {
    const wpi_upb_MiniTable* layout = wpi_upb_MessageDef_MiniTable(f->msgdef);
    return &layout->WPI_UPB_PRIVATE(fields)[f->layout_index];
  }
}

const wpi_upb_MiniTableExtension* wpi_upb_FieldDef_MiniTableExtension(
    const wpi_upb_FieldDef* f) {
  WPI_UPB_ASSERT(wpi_upb_FieldDef_IsExtension(f));
  const wpi_upb_FileDef* file = wpi_upb_FieldDef_File(f);
  return _wpi_upb_FileDef_ExtensionMiniTable(file, f->layout_index);
}

bool _wpi_upb_FieldDef_IsClosedEnum(const wpi_upb_FieldDef* f) {
  if (f->type_ != kUpb_FieldType_Enum) return false;
  return wpi_upb_EnumDef_IsClosed(f->sub.enumdef);
}

bool _wpi_upb_FieldDef_IsProto3Optional(const wpi_upb_FieldDef* f) {
  return f->is_proto3_optional;
}

int _wpi_upb_FieldDef_LayoutIndex(const wpi_upb_FieldDef* f) { return f->layout_index; }

bool _wpi_upb_FieldDef_ValidateUtf8(const wpi_upb_FieldDef* f) {
  if (wpi_upb_FieldDef_Type(f) != kUpb_FieldType_String) return false;
  return WPI_UPB_DESC(FeatureSet_utf8_validation(f->resolved_features)) ==
         WPI_UPB_DESC(FeatureSet_VERIFY);
}

bool _wpi_upb_FieldDef_IsGroupLike(const wpi_upb_FieldDef* f) {
  // Groups are always tag-delimited.
  if (f->type_ != kUpb_FieldType_Group) {
    return false;
  }

  const wpi_upb_MessageDef* msg = wpi_upb_FieldDef_MessageSubDef(f);

  // Group fields always are always the lowercase type name.
  const char* mname = wpi_upb_MessageDef_Name(msg);
  const char* fname = wpi_upb_FieldDef_Name(f);
  size_t name_size = strlen(fname);
  if (name_size != strlen(mname)) return false;
  for (size_t i = 0; i < name_size; ++i) {
    if ((mname[i] | 0x20) != fname[i]) {
      // Case-insensitive ascii comparison.
      return false;
    }
  }

  if (wpi_upb_MessageDef_File(msg) != wpi_upb_FieldDef_File(f)) {
    return false;
  }

  // Group messages are always defined in the same scope as the field.  File
  // level extensions will compare NULL == NULL here, which is why the file
  // comparison above is necessary to ensure both come from the same file.
  return wpi_upb_FieldDef_IsExtension(f) ? wpi_upb_FieldDef_ExtensionScope(f) ==
                                           wpi_upb_MessageDef_ContainingType(msg)
                                     : wpi_upb_FieldDef_ContainingType(f) ==
                                           wpi_upb_MessageDef_ContainingType(msg);
}

uint64_t _wpi_upb_FieldDef_Modifiers(const wpi_upb_FieldDef* f) {
  uint64_t out = wpi_upb_FieldDef_IsPacked(f) ? kUpb_FieldModifier_IsPacked : 0;

  if (wpi_upb_FieldDef_IsRepeated(f)) {
    out |= kUpb_FieldModifier_IsRepeated;
  } else if (wpi_upb_FieldDef_IsRequired(f)) {
    out |= kUpb_FieldModifier_IsRequired;
  } else if (!wpi_upb_FieldDef_HasPresence(f)) {
    out |= kUpb_FieldModifier_IsProto3Singular;
  }

  if (_wpi_upb_FieldDef_IsClosedEnum(f)) {
    out |= kUpb_FieldModifier_IsClosedEnum;
  }

  if (_wpi_upb_FieldDef_ValidateUtf8(f)) {
    out |= kUpb_FieldModifier_ValidateUtf8;
  }

  return out;
}

bool wpi_upb_FieldDef_HasDefault(const wpi_upb_FieldDef* f) { return f->has_default; }
bool wpi_upb_FieldDef_HasPresence(const wpi_upb_FieldDef* f) { return f->has_presence; }

bool wpi_upb_FieldDef_HasSubDef(const wpi_upb_FieldDef* f) {
  return wpi_upb_FieldDef_IsSubMessage(f) || wpi_upb_FieldDef_IsEnum(f);
}

bool wpi_upb_FieldDef_IsEnum(const wpi_upb_FieldDef* f) {
  return wpi_upb_FieldDef_CType(f) == kUpb_CType_Enum;
}

bool wpi_upb_FieldDef_IsMap(const wpi_upb_FieldDef* f) {
  return wpi_upb_FieldDef_IsRepeated(f) && wpi_upb_FieldDef_IsSubMessage(f) &&
         wpi_upb_MessageDef_IsMapEntry(wpi_upb_FieldDef_MessageSubDef(f));
}

bool wpi_upb_FieldDef_IsOptional(const wpi_upb_FieldDef* f) {
  return wpi_upb_FieldDef_Label(f) == kUpb_Label_Optional;
}

bool wpi_upb_FieldDef_IsPrimitive(const wpi_upb_FieldDef* f) {
  return !wpi_upb_FieldDef_IsString(f) && !wpi_upb_FieldDef_IsSubMessage(f);
}

bool wpi_upb_FieldDef_IsRepeated(const wpi_upb_FieldDef* f) {
  return wpi_upb_FieldDef_Label(f) == kUpb_Label_Repeated;
}

bool wpi_upb_FieldDef_IsRequired(const wpi_upb_FieldDef* f) {
  return WPI_UPB_DESC(FeatureSet_field_presence)(f->resolved_features) ==
         WPI_UPB_DESC(FeatureSet_LEGACY_REQUIRED);
}

bool wpi_upb_FieldDef_IsString(const wpi_upb_FieldDef* f) {
  return wpi_upb_FieldDef_CType(f) == kUpb_CType_String ||
         wpi_upb_FieldDef_CType(f) == kUpb_CType_Bytes;
}

bool wpi_upb_FieldDef_IsSubMessage(const wpi_upb_FieldDef* f) {
  return wpi_upb_FieldDef_CType(f) == kUpb_CType_Message;
}

static bool between(int32_t x, int32_t low, int32_t high) {
  return x >= low && x <= high;
}

bool wpi_upb_FieldDef_checklabel(int32_t label) { return between(label, 1, 3); }
bool wpi_upb_FieldDef_checktype(int32_t type) { return between(type, 1, 11); }
bool wpi_upb_FieldDef_checkintfmt(int32_t fmt) { return between(fmt, 1, 3); }

bool wpi_upb_FieldDef_checkdescriptortype(int32_t type) {
  return between(type, 1, 18);
}

static bool streql2(const char* a, size_t n, const char* b) {
  return n == strlen(b) && memcmp(a, b, n) == 0;
}

// Implement the transformation as described in the spec:
//   1. upper case all letters after an underscore.
//   2. remove all underscores.
static char* make_json_name(const char* name, size_t size, wpi_upb_Arena* a) {
  char* out = wpi_upb_Arena_Malloc(a, size + 1);  // +1 is to add a trailing '\0'
  if (out == NULL) return NULL;

  bool ucase_next = false;
  char* des = out;
  for (size_t i = 0; i < size; i++) {
    if (name[i] == '_') {
      ucase_next = true;
    } else {
      *des++ = ucase_next ? toupper(name[i]) : name[i];
      ucase_next = false;
    }
  }
  *des++ = '\0';
  return out;
}

static str_t* newstr(wpi_upb_DefBuilder* ctx, const char* data, size_t len) {
  str_t* ret = _wpi_upb_DefBuilder_Alloc(ctx, sizeof(*ret) + len);
  if (!ret) _wpi_upb_DefBuilder_OomErr(ctx);
  ret->len = len;
  if (len) memcpy(ret->str, data, len);
  ret->str[len] = '\0';
  return ret;
}

static str_t* unescape(wpi_upb_DefBuilder* ctx, const wpi_upb_FieldDef* f,
                       const char* data, size_t len) {
  // Size here is an upper bound; escape sequences could ultimately shrink it.
  str_t* ret = _wpi_upb_DefBuilder_Alloc(ctx, sizeof(*ret) + len);
  char* dst = &ret->str[0];
  const char* src = data;
  const char* end = data + len;

  while (src < end) {
    if (*src == '\\') {
      src++;
      *dst++ = _wpi_upb_DefBuilder_ParseEscape(ctx, f, &src, end);
    } else {
      *dst++ = *src++;
    }
  }

  ret->len = dst - &ret->str[0];
  return ret;
}

static void parse_default(wpi_upb_DefBuilder* ctx, const char* str, size_t len,
                          wpi_upb_FieldDef* f) {
  char* end;
  char nullz[64];
  errno = 0;

  switch (wpi_upb_FieldDef_CType(f)) {
    case kUpb_CType_Int32:
    case kUpb_CType_Int64:
    case kUpb_CType_UInt32:
    case kUpb_CType_UInt64:
    case kUpb_CType_Double:
    case kUpb_CType_Float:
      // Standard C number parsing functions expect null-terminated strings.
      if (len >= sizeof(nullz) - 1) {
        _wpi_upb_DefBuilder_Errf(ctx, "Default too long: %.*s", (int)len, str);
      }
      memcpy(nullz, str, len);
      nullz[len] = '\0';
      str = nullz;
      break;
    default:
      break;
  }

  switch (wpi_upb_FieldDef_CType(f)) {
    case kUpb_CType_Int32: {
      long val = strtol(str, &end, 0);
      if (val > INT32_MAX || val < INT32_MIN || errno == ERANGE || *end) {
        goto invalid;
      }
      f->defaultval.sint = val;
      break;
    }
    case kUpb_CType_Enum: {
      const wpi_upb_EnumDef* e = f->sub.enumdef;
      const wpi_upb_EnumValueDef* ev =
          wpi_upb_EnumDef_FindValueByNameWithSize(e, str, len);
      if (!ev) {
        goto invalid;
      }
      f->defaultval.sint = wpi_upb_EnumValueDef_Number(ev);
      break;
    }
    case kUpb_CType_Int64: {
      long long val = strtoll(str, &end, 0);
      if (val > INT64_MAX || val < INT64_MIN || errno == ERANGE || *end) {
        goto invalid;
      }
      f->defaultval.sint = val;
      break;
    }
    case kUpb_CType_UInt32: {
      unsigned long val = strtoul(str, &end, 0);
      if (val > UINT32_MAX || errno == ERANGE || *end) {
        goto invalid;
      }
      f->defaultval.uint = val;
      break;
    }
    case kUpb_CType_UInt64: {
      unsigned long long val = strtoull(str, &end, 0);
      if (val > UINT64_MAX || errno == ERANGE || *end) {
        goto invalid;
      }
      f->defaultval.uint = val;
      break;
    }
    case kUpb_CType_Double: {
      double val = strtod(str, &end);
      if (errno == ERANGE || *end) {
        goto invalid;
      }
      f->defaultval.dbl = val;
      break;
    }
    case kUpb_CType_Float: {
      float val = strtof(str, &end);
      if (errno == ERANGE || *end) {
        goto invalid;
      }
      f->defaultval.flt = val;
      break;
    }
    case kUpb_CType_Bool: {
      if (streql2(str, len, "false")) {
        f->defaultval.boolean = false;
      } else if (streql2(str, len, "true")) {
        f->defaultval.boolean = true;
      } else {
        goto invalid;
      }
      break;
    }
    case kUpb_CType_String:
      f->defaultval.str = newstr(ctx, str, len);
      break;
    case kUpb_CType_Bytes:
      f->defaultval.str = unescape(ctx, f, str, len);
      break;
    case kUpb_CType_Message:
      /* Should not have a default value. */
      _wpi_upb_DefBuilder_Errf(ctx, "Message should not have a default (%s)",
                           wpi_upb_FieldDef_FullName(f));
  }

  return;

invalid:
  _wpi_upb_DefBuilder_Errf(ctx, "Invalid default '%.*s' for field %s of type %d",
                       (int)len, str, wpi_upb_FieldDef_FullName(f),
                       (int)wpi_upb_FieldDef_Type(f));
}

static void set_default_default(wpi_upb_DefBuilder* ctx, wpi_upb_FieldDef* f) {
  switch (wpi_upb_FieldDef_CType(f)) {
    case kUpb_CType_Int32:
    case kUpb_CType_Int64:
      f->defaultval.sint = 0;
      break;
    case kUpb_CType_UInt64:
    case kUpb_CType_UInt32:
      f->defaultval.uint = 0;
      break;
    case kUpb_CType_Double:
    case kUpb_CType_Float:
      f->defaultval.dbl = 0;
      break;
    case kUpb_CType_String:
    case kUpb_CType_Bytes:
      f->defaultval.str = newstr(ctx, NULL, 0);
      break;
    case kUpb_CType_Bool:
      f->defaultval.boolean = false;
      break;
    case kUpb_CType_Enum: {
      const wpi_upb_EnumValueDef* v = wpi_upb_EnumDef_Value(f->sub.enumdef, 0);
      f->defaultval.sint = wpi_upb_EnumValueDef_Number(v);
      break;
    }
    case kUpb_CType_Message:
      break;
  }
}

static bool _wpi_upb_FieldDef_InferLegacyFeatures(
    wpi_upb_DefBuilder* ctx, wpi_upb_FieldDef* f,
    const WPI_UPB_DESC(FieldDescriptorProto*) proto,
    const WPI_UPB_DESC(FieldOptions*) options, wpi_upb_Syntax syntax,
    WPI_UPB_DESC(FeatureSet*) features) {
  bool ret = false;

  if (WPI_UPB_DESC(FieldDescriptorProto_label)(proto) == kUpb_Label_Required) {
    if (syntax == kUpb_Syntax_Proto3) {
      _wpi_upb_DefBuilder_Errf(ctx, "proto3 fields cannot be required (%s)",
                           f->full_name);
    }
    int val = WPI_UPB_DESC(FeatureSet_LEGACY_REQUIRED);
    WPI_UPB_DESC(FeatureSet_set_field_presence(features, val));
    ret = true;
  }

  if (WPI_UPB_DESC(FieldDescriptorProto_type)(proto) == kUpb_FieldType_Group) {
    int val = WPI_UPB_DESC(FeatureSet_DELIMITED);
    WPI_UPB_DESC(FeatureSet_set_message_encoding(features, val));
    ret = true;
  }

  if (WPI_UPB_DESC(FieldOptions_has_packed)(options)) {
    int val = WPI_UPB_DESC(FieldOptions_packed)(options)
                  ? WPI_UPB_DESC(FeatureSet_PACKED)
                  : WPI_UPB_DESC(FeatureSet_EXPANDED);
    WPI_UPB_DESC(FeatureSet_set_repeated_field_encoding(features, val));
    ret = true;
  }

// begin:google_only
// #ifndef WPI_UPB_BOOTSTRAP_STAGE0
//   if (syntax == kUpb_Syntax_Proto3 &&
//       WPI_UPB_DESC(FieldOptions_has_enforce_utf8)(options) &&
//       !WPI_UPB_DESC(FieldOptions_enforce_utf8)(options)) {
//     int val = WPI_UPB_DESC(FeatureSet_UNVERIFIED);
//     WPI_UPB_DESC(FeatureSet_set_utf8_validation(features, val));
//     ret = true;
//   }
// #endif
//   // clang-format off
// end:google_only
  // clang-format on

  return ret;
}

static void _wpi_upb_FieldDef_Create(wpi_upb_DefBuilder* ctx, const char* prefix,
                                 const WPI_UPB_DESC(FeatureSet*) parent_features,
                                 const WPI_UPB_DESC(FieldDescriptorProto*)
                                     field_proto,
                                 wpi_upb_MessageDef* m, wpi_upb_FieldDef* f) {
  // Must happen before _wpi_upb_DefBuilder_Add()
  f->file = _wpi_upb_DefBuilder_File(ctx);

  const wpi_upb_StringView name = WPI_UPB_DESC(FieldDescriptorProto_name)(field_proto);
  f->full_name = _wpi_upb_DefBuilder_MakeFullName(ctx, prefix, name);
  f->number_ = WPI_UPB_DESC(FieldDescriptorProto_number)(field_proto);
  f->is_proto3_optional =
      WPI_UPB_DESC(FieldDescriptorProto_proto3_optional)(field_proto);
  f->msgdef = m;
  f->scope.oneof = NULL;

  WPI_UPB_DEF_SET_OPTIONS(f->opts, FieldDescriptorProto, FieldOptions, field_proto);

  wpi_upb_Syntax syntax = wpi_upb_FileDef_Syntax(f->file);
  const WPI_UPB_DESC(FeatureSet*) unresolved_features =
      WPI_UPB_DESC(FieldOptions_features)(f->opts);
  bool implicit = false;

  if (syntax != kUpb_Syntax_Editions) {
    wpi_upb_Message_Clear(WPI_UPB_UPCAST(ctx->legacy_features),
                      WPI_UPB_DESC_MINITABLE(FeatureSet));
    if (_wpi_upb_FieldDef_InferLegacyFeatures(ctx, f, field_proto, f->opts, syntax,
                                          ctx->legacy_features)) {
      implicit = true;
      unresolved_features = ctx->legacy_features;
    }
  }

  if (WPI_UPB_DESC(FieldDescriptorProto_has_oneof_index)(field_proto)) {
    int oneof_index = WPI_UPB_DESC(FieldDescriptorProto_oneof_index)(field_proto);

    if (!m) {
      _wpi_upb_DefBuilder_Errf(ctx, "oneof field (%s) has no containing msg",
                           f->full_name);
    }

    if (oneof_index < 0 || oneof_index >= wpi_upb_MessageDef_OneofCount(m)) {
      _wpi_upb_DefBuilder_Errf(ctx, "oneof_index out of range (%s)", f->full_name);
    }

    wpi_upb_OneofDef* oneof = (wpi_upb_OneofDef*)wpi_upb_MessageDef_Oneof(m, oneof_index);
    f->scope.oneof = oneof;
    parent_features = wpi_upb_OneofDef_ResolvedFeatures(oneof);

    _wpi_upb_OneofDef_Insert(ctx, oneof, f, name.data, name.size);
  }

  f->resolved_features = _wpi_upb_DefBuilder_DoResolveFeatures(
      ctx, parent_features, unresolved_features, implicit);

  f->label_ = (int)WPI_UPB_DESC(FieldDescriptorProto_label)(field_proto);
  if (f->label_ == kUpb_Label_Optional &&
      // TODO: remove once we can deprecate kUpb_Label_Required.
      WPI_UPB_DESC(FeatureSet_field_presence)(f->resolved_features) ==
          WPI_UPB_DESC(FeatureSet_LEGACY_REQUIRED)) {
    f->label_ = kUpb_Label_Required;
  }

  if (!WPI_UPB_DESC(FieldDescriptorProto_has_name)(field_proto)) {
    _wpi_upb_DefBuilder_Errf(ctx, "field has no name");
  }

  f->has_json_name = WPI_UPB_DESC(FieldDescriptorProto_has_json_name)(field_proto);
  if (f->has_json_name) {
    const wpi_upb_StringView sv =
        WPI_UPB_DESC(FieldDescriptorProto_json_name)(field_proto);
    f->json_name = wpi_upb_strdup2(sv.data, sv.size, ctx->arena);
  } else {
    f->json_name = make_json_name(name.data, name.size, ctx->arena);
  }
  if (!f->json_name) _wpi_upb_DefBuilder_OomErr(ctx);

  const bool has_type = WPI_UPB_DESC(FieldDescriptorProto_has_type)(field_proto);
  const bool has_type_name =
      WPI_UPB_DESC(FieldDescriptorProto_has_type_name)(field_proto);

  f->type_ = (int)WPI_UPB_DESC(FieldDescriptorProto_type)(field_proto);

  if (has_type) {
    switch (f->type_) {
      case kUpb_FieldType_Message:
      case kUpb_FieldType_Group:
      case kUpb_FieldType_Enum:
        if (!has_type_name) {
          _wpi_upb_DefBuilder_Errf(ctx, "field of type %d requires type name (%s)",
                               (int)f->type_, f->full_name);
        }
        break;
      default:
        if (has_type_name) {
          _wpi_upb_DefBuilder_Errf(
              ctx, "invalid type for field with type_name set (%s, %d)",
              f->full_name, (int)f->type_);
        }
    }
  }

  if ((!has_type && has_type_name) || f->type_ == kUpb_FieldType_Message) {
    f->type_ =
        WPI_UPB_FIELD_TYPE_UNSPECIFIED;  // We'll assign this in resolve_subdef()
  } else {
    if (f->type_ < kUpb_FieldType_Double || f->type_ > kUpb_FieldType_SInt64) {
      _wpi_upb_DefBuilder_Errf(ctx, "invalid type for field %s (%d)", f->full_name,
                           f->type_);
    }
  }

  if (f->label_ < kUpb_Label_Optional || f->label_ > kUpb_Label_Repeated) {
    _wpi_upb_DefBuilder_Errf(ctx, "invalid label for field %s (%d)", f->full_name,
                         f->label_);
  }

  /* We can't resolve the subdef or (in the case of extensions) the containing
   * message yet, because it may not have been defined yet.  We stash a pointer
   * to the field_proto until later when we can properly resolve it. */
  f->sub.unresolved = field_proto;

  if (WPI_UPB_DESC(FieldDescriptorProto_has_oneof_index)(field_proto)) {
    if (wpi_upb_FieldDef_Label(f) != kUpb_Label_Optional) {
      _wpi_upb_DefBuilder_Errf(ctx, "fields in oneof must have OPTIONAL label (%s)",
                           f->full_name);
    }
  }

  f->has_presence =
      (!wpi_upb_FieldDef_IsRepeated(f)) &&
      (f->is_extension ||
       (f->type_ == kUpb_FieldType_Message ||
        f->type_ == kUpb_FieldType_Group || wpi_upb_FieldDef_ContainingOneof(f) ||
        WPI_UPB_DESC(FeatureSet_field_presence)(f->resolved_features) !=
            WPI_UPB_DESC(FeatureSet_IMPLICIT)));
}

static void _wpi_upb_FieldDef_CreateExt(wpi_upb_DefBuilder* ctx, const char* prefix,
                                    const WPI_UPB_DESC(FeatureSet*) parent_features,
                                    const WPI_UPB_DESC(FieldDescriptorProto*)
                                        field_proto,
                                    wpi_upb_MessageDef* m, wpi_upb_FieldDef* f) {
  f->is_extension = true;
  _wpi_upb_FieldDef_Create(ctx, prefix, parent_features, field_proto, m, f);

  if (WPI_UPB_DESC(FieldDescriptorProto_has_oneof_index)(field_proto)) {
    _wpi_upb_DefBuilder_Errf(ctx, "oneof_index provided for extension field (%s)",
                         f->full_name);
  }

  f->scope.extension_scope = m;
  _wpi_upb_DefBuilder_Add(ctx, f->full_name, _wpi_upb_DefType_Pack(f, WPI_UPB_DEFTYPE_EXT));
  f->layout_index = ctx->ext_count++;

  if (ctx->layout) {
    WPI_UPB_ASSERT(wpi_upb_MiniTableExtension_Number(
                   wpi_upb_FieldDef_MiniTableExtension(f)) == f->number_);
  }
}

static void _wpi_upb_FieldDef_CreateNotExt(wpi_upb_DefBuilder* ctx, const char* prefix,
                                       const WPI_UPB_DESC(FeatureSet*)
                                           parent_features,
                                       const WPI_UPB_DESC(FieldDescriptorProto*)
                                           field_proto,
                                       wpi_upb_MessageDef* m, wpi_upb_FieldDef* f) {
  f->is_extension = false;
  _wpi_upb_FieldDef_Create(ctx, prefix, parent_features, field_proto, m, f);

  if (!WPI_UPB_DESC(FieldDescriptorProto_has_oneof_index)(field_proto)) {
    if (f->is_proto3_optional) {
      _wpi_upb_DefBuilder_Errf(
          ctx,
          "non-extension field (%s) with proto3_optional was not in a oneof",
          f->full_name);
    }
  }

  _wpi_upb_MessageDef_InsertField(ctx, m, f);
}

wpi_upb_FieldDef* _wpi_upb_Extensions_New(wpi_upb_DefBuilder* ctx, int n,
                                  const WPI_UPB_DESC(FieldDescriptorProto*)
                                      const* protos,
                                  const WPI_UPB_DESC(FeatureSet*) parent_features,
                                  const char* prefix, wpi_upb_MessageDef* m) {
  _wpi_upb_DefType_CheckPadding(sizeof(wpi_upb_FieldDef));
  wpi_upb_FieldDef* defs =
      (wpi_upb_FieldDef*)_wpi_upb_DefBuilder_Alloc(ctx, sizeof(wpi_upb_FieldDef) * n);

  for (int i = 0; i < n; i++) {
    wpi_upb_FieldDef* f = &defs[i];

    _wpi_upb_FieldDef_CreateExt(ctx, prefix, parent_features, protos[i], m, f);
    f->index_ = i;
  }

  return defs;
}

wpi_upb_FieldDef* _wpi_upb_FieldDefs_New(wpi_upb_DefBuilder* ctx, int n,
                                 const WPI_UPB_DESC(FieldDescriptorProto*)
                                     const* protos,
                                 const WPI_UPB_DESC(FeatureSet*) parent_features,
                                 const char* prefix, wpi_upb_MessageDef* m,
                                 bool* is_sorted) {
  _wpi_upb_DefType_CheckPadding(sizeof(wpi_upb_FieldDef));
  wpi_upb_FieldDef* defs =
      (wpi_upb_FieldDef*)_wpi_upb_DefBuilder_Alloc(ctx, sizeof(wpi_upb_FieldDef) * n);

  uint32_t previous = 0;
  for (int i = 0; i < n; i++) {
    wpi_upb_FieldDef* f = &defs[i];

    _wpi_upb_FieldDef_CreateNotExt(ctx, prefix, parent_features, protos[i], m, f);
    f->index_ = i;
    if (!ctx->layout) {
      // Speculate that the def fields are sorted.  We will always sort the
      // MiniTable fields, so if defs are sorted then indices will match.
      //
      // If this is incorrect, we will overwrite later.
      f->layout_index = i;
    }

    const uint32_t current = f->number_;
    if (previous > current) *is_sorted = false;
    previous = current;
  }

  return defs;
}

static void resolve_subdef(wpi_upb_DefBuilder* ctx, const char* prefix,
                           wpi_upb_FieldDef* f) {
  const WPI_UPB_DESC(FieldDescriptorProto)* field_proto = f->sub.unresolved;
  wpi_upb_StringView name = WPI_UPB_DESC(FieldDescriptorProto_type_name)(field_proto);
  bool has_name = WPI_UPB_DESC(FieldDescriptorProto_has_type_name)(field_proto);
  switch ((int)f->type_) {
    case WPI_UPB_FIELD_TYPE_UNSPECIFIED: {
      // Type was not specified and must be inferred.
      WPI_UPB_ASSERT(has_name);
      wpi_upb_deftype_t type;
      const void* def =
          _wpi_upb_DefBuilder_ResolveAny(ctx, f->full_name, prefix, name, &type);
      switch (type) {
        case WPI_UPB_DEFTYPE_ENUM:
          f->sub.enumdef = def;
          f->type_ = kUpb_FieldType_Enum;
          break;
        case WPI_UPB_DEFTYPE_MSG:
          f->sub.msgdef = def;
          f->type_ = kUpb_FieldType_Message;
          // TODO: remove once we can deprecate
          // kUpb_FieldType_Group.
          if (WPI_UPB_DESC(FeatureSet_message_encoding)(f->resolved_features) ==
                  WPI_UPB_DESC(FeatureSet_DELIMITED) &&
              !wpi_upb_MessageDef_IsMapEntry(def) &&
              !(f->msgdef && wpi_upb_MessageDef_IsMapEntry(f->msgdef))) {
            f->type_ = kUpb_FieldType_Group;
          }
          f->has_presence = !wpi_upb_FieldDef_IsRepeated(f);
          break;
        default:
          _wpi_upb_DefBuilder_Errf(ctx, "Couldn't resolve type name for field %s",
                               f->full_name);
      }
      break;
    }
    case kUpb_FieldType_Message:
    case kUpb_FieldType_Group:
      WPI_UPB_ASSERT(has_name);
      f->sub.msgdef = _wpi_upb_DefBuilder_Resolve(ctx, f->full_name, prefix, name,
                                              WPI_UPB_DEFTYPE_MSG);
      break;
    case kUpb_FieldType_Enum:
      WPI_UPB_ASSERT(has_name);
      f->sub.enumdef = _wpi_upb_DefBuilder_Resolve(ctx, f->full_name, prefix, name,
                                               WPI_UPB_DEFTYPE_ENUM);
      break;
    default:
      // No resolution necessary.
      break;
  }
}

static int _wpi_upb_FieldDef_Compare(const void* p1, const void* p2) {
  const uint32_t v1 = (*(wpi_upb_FieldDef**)p1)->number_;
  const uint32_t v2 = (*(wpi_upb_FieldDef**)p2)->number_;
  return (v1 < v2) ? -1 : (v1 > v2);
}

// _wpi_upb_FieldDefs_Sorted() is mostly a pure function of its inputs, but has one
// critical side effect that we depend on: it sets layout_index appropriately
// for non-sorted lists of fields.
const wpi_upb_FieldDef** _wpi_upb_FieldDefs_Sorted(const wpi_upb_FieldDef* f, int n,
                                           wpi_upb_Arena* a) {
  // TODO: Replace this arena alloc with a persistent scratch buffer.
  wpi_upb_FieldDef** out = (wpi_upb_FieldDef**)wpi_upb_Arena_Malloc(a, n * sizeof(void*));
  if (!out) return NULL;

  for (int i = 0; i < n; i++) {
    out[i] = (wpi_upb_FieldDef*)&f[i];
  }
  qsort(out, n, sizeof(void*), _wpi_upb_FieldDef_Compare);

  for (int i = 0; i < n; i++) {
    out[i]->layout_index = i;
  }
  return (const wpi_upb_FieldDef**)out;
}

bool wpi_upb_FieldDef_MiniDescriptorEncode(const wpi_upb_FieldDef* f, wpi_upb_Arena* a,
                                       wpi_upb_StringView* out) {
  WPI_UPB_ASSERT(f->is_extension);

  wpi_upb_DescState s;
  _wpi_upb_DescState_Init(&s);

  const int number = wpi_upb_FieldDef_Number(f);
  const uint64_t modifiers = _wpi_upb_FieldDef_Modifiers(f);

  if (!_wpi_upb_DescState_Grow(&s, a)) return false;
  s.ptr = wpi_upb_MtDataEncoder_EncodeExtension(&s.e, s.ptr, f->type_, number,
                                            modifiers);
  *s.ptr = '\0';

  out->data = s.buf;
  out->size = s.ptr - s.buf;
  return true;
}

static void resolve_extension(wpi_upb_DefBuilder* ctx, const char* prefix,
                              wpi_upb_FieldDef* f,
                              const WPI_UPB_DESC(FieldDescriptorProto) *
                                  field_proto) {
  if (!WPI_UPB_DESC(FieldDescriptorProto_has_extendee)(field_proto)) {
    _wpi_upb_DefBuilder_Errf(ctx, "extension for field '%s' had no extendee",
                         f->full_name);
  }

  wpi_upb_StringView name = WPI_UPB_DESC(FieldDescriptorProto_extendee)(field_proto);
  const wpi_upb_MessageDef* m =
      _wpi_upb_DefBuilder_Resolve(ctx, f->full_name, prefix, name, WPI_UPB_DEFTYPE_MSG);
  f->msgdef = m;

  if (!_wpi_upb_MessageDef_IsValidExtensionNumber(m, f->number_)) {
    _wpi_upb_DefBuilder_Errf(
        ctx,
        "field number %u in extension %s has no extension range in message %s",
        (unsigned)f->number_, f->full_name, wpi_upb_MessageDef_FullName(m));
  }
}

void _wpi_upb_FieldDef_BuildMiniTableExtension(wpi_upb_DefBuilder* ctx,
                                           const wpi_upb_FieldDef* f) {
  const wpi_upb_MiniTableExtension* ext = wpi_upb_FieldDef_MiniTableExtension(f);

  if (ctx->layout) {
    WPI_UPB_ASSERT(wpi_upb_FieldDef_Number(f) == wpi_upb_MiniTableExtension_Number(ext));
  } else {
    wpi_upb_StringView desc;
    if (!wpi_upb_FieldDef_MiniDescriptorEncode(f, ctx->tmp_arena, &desc)) {
      _wpi_upb_DefBuilder_OomErr(ctx);
    }

    wpi_upb_MiniTableExtension* mut_ext = (wpi_upb_MiniTableExtension*)ext;
    wpi_upb_MiniTableSub sub = {NULL};
    if (wpi_upb_FieldDef_IsSubMessage(f)) {
      const wpi_upb_MiniTable* submsg = wpi_upb_MessageDef_MiniTable(f->sub.msgdef);
      sub = wpi_upb_MiniTableSub_FromMessage(submsg);
    } else if (_wpi_upb_FieldDef_IsClosedEnum(f)) {
      const wpi_upb_MiniTableEnum* subenum = _wpi_upb_EnumDef_MiniTable(f->sub.enumdef);
      sub = wpi_upb_MiniTableSub_FromEnum(subenum);
    }
    bool ok2 = wpi_upb_MiniTableExtension_Init(desc.data, desc.size, mut_ext,
                                           wpi_upb_MessageDef_MiniTable(f->msgdef),
                                           sub, ctx->status);
    if (!ok2) _wpi_upb_DefBuilder_Errf(ctx, "Could not build extension mini table");
  }

  bool ok = _wpi_upb_DefPool_InsertExt(ctx->symtab, ext, f);
  if (!ok) _wpi_upb_DefBuilder_OomErr(ctx);
}

static void resolve_default(wpi_upb_DefBuilder* ctx, wpi_upb_FieldDef* f,
                            const WPI_UPB_DESC(FieldDescriptorProto) *
                                field_proto) {
  // Have to delay resolving of the default value until now because of the enum
  // case, since enum defaults are specified with a label.
  if (WPI_UPB_DESC(FieldDescriptorProto_has_default_value)(field_proto)) {
    wpi_upb_StringView defaultval =
        WPI_UPB_DESC(FieldDescriptorProto_default_value)(field_proto);

    if (wpi_upb_FileDef_Syntax(f->file) == kUpb_Syntax_Proto3) {
      _wpi_upb_DefBuilder_Errf(ctx,
                           "proto3 fields cannot have explicit defaults (%s)",
                           f->full_name);
    }

    if (wpi_upb_FieldDef_IsSubMessage(f)) {
      _wpi_upb_DefBuilder_Errf(ctx,
                           "message fields cannot have explicit defaults (%s)",
                           f->full_name);
    }

    parse_default(ctx, defaultval.data, defaultval.size, f);
    f->has_default = true;
  } else {
    set_default_default(ctx, f);
    f->has_default = false;
  }
}

void _wpi_upb_FieldDef_Resolve(wpi_upb_DefBuilder* ctx, const char* prefix,
                           wpi_upb_FieldDef* f) {
  // We have to stash this away since resolve_subdef() may overwrite it.
  const WPI_UPB_DESC(FieldDescriptorProto)* field_proto = f->sub.unresolved;

  resolve_subdef(ctx, prefix, f);
  resolve_default(ctx, f, field_proto);

  if (f->is_extension) {
    resolve_extension(ctx, prefix, f, field_proto);
  }
}


#include <stddef.h>
#include <stdint.h>
#include <string.h>


// Must be last.

struct wpi_upb_FileDef {
  const WPI_UPB_DESC(FileOptions*) opts;
  const WPI_UPB_DESC(FeatureSet*) resolved_features;
  const char* name;
  const char* package;
  WPI_UPB_DESC(Edition) edition;

  const wpi_upb_FileDef** deps;
  const int32_t* public_deps;
  const int32_t* weak_deps;
  const wpi_upb_MessageDef* top_lvl_msgs;
  const wpi_upb_EnumDef* top_lvl_enums;
  const wpi_upb_FieldDef* top_lvl_exts;
  const wpi_upb_ServiceDef* services;
  const wpi_upb_MiniTableExtension** ext_layouts;
  const wpi_upb_DefPool* symtab;

  int dep_count;
  int public_dep_count;
  int weak_dep_count;
  int top_lvl_msg_count;
  int top_lvl_enum_count;
  int top_lvl_ext_count;
  int service_count;
  int ext_count;  // All exts in the file.
  wpi_upb_Syntax syntax;
};

WPI_UPB_API const char* wpi_upb_FileDef_EditionName(int edition) {
  // TODO Synchronize this with descriptor.proto better.
  switch (edition) {
    case WPI_UPB_DESC(EDITION_PROTO2):
      return "PROTO2";
    case WPI_UPB_DESC(EDITION_PROTO3):
      return "PROTO3";
    case WPI_UPB_DESC(EDITION_2023):
      return "2023";
    default:
      return "UNKNOWN";
  }
}

const WPI_UPB_DESC(FileOptions) * wpi_upb_FileDef_Options(const wpi_upb_FileDef* f) {
  return f->opts;
}

const WPI_UPB_DESC(FeatureSet) *
    wpi_upb_FileDef_ResolvedFeatures(const wpi_upb_FileDef* f) {
  return f->resolved_features;
}

bool wpi_upb_FileDef_HasOptions(const wpi_upb_FileDef* f) {
  return f->opts != (void*)kUpbDefOptDefault;
}

const char* wpi_upb_FileDef_Name(const wpi_upb_FileDef* f) { return f->name; }

const char* wpi_upb_FileDef_Package(const wpi_upb_FileDef* f) {
  return f->package ? f->package : "";
}

WPI_UPB_DESC(Edition) wpi_upb_FileDef_Edition(const wpi_upb_FileDef* f) {
  return f->edition;
}

const char* _wpi_upb_FileDef_RawPackage(const wpi_upb_FileDef* f) { return f->package; }

wpi_upb_Syntax wpi_upb_FileDef_Syntax(const wpi_upb_FileDef* f) { return f->syntax; }

int wpi_upb_FileDef_TopLevelMessageCount(const wpi_upb_FileDef* f) {
  return f->top_lvl_msg_count;
}

int wpi_upb_FileDef_DependencyCount(const wpi_upb_FileDef* f) { return f->dep_count; }

int wpi_upb_FileDef_PublicDependencyCount(const wpi_upb_FileDef* f) {
  return f->public_dep_count;
}

int wpi_upb_FileDef_WeakDependencyCount(const wpi_upb_FileDef* f) {
  return f->weak_dep_count;
}

const int32_t* _wpi_upb_FileDef_PublicDependencyIndexes(const wpi_upb_FileDef* f) {
  return f->public_deps;
}

const int32_t* _wpi_upb_FileDef_WeakDependencyIndexes(const wpi_upb_FileDef* f) {
  return f->weak_deps;
}

int wpi_upb_FileDef_TopLevelEnumCount(const wpi_upb_FileDef* f) {
  return f->top_lvl_enum_count;
}

int wpi_upb_FileDef_TopLevelExtensionCount(const wpi_upb_FileDef* f) {
  return f->top_lvl_ext_count;
}

int wpi_upb_FileDef_ServiceCount(const wpi_upb_FileDef* f) { return f->service_count; }

const wpi_upb_FileDef* wpi_upb_FileDef_Dependency(const wpi_upb_FileDef* f, int i) {
  WPI_UPB_ASSERT(0 <= i && i < f->dep_count);
  return f->deps[i];
}

const wpi_upb_FileDef* wpi_upb_FileDef_PublicDependency(const wpi_upb_FileDef* f, int i) {
  WPI_UPB_ASSERT(0 <= i && i < f->public_dep_count);
  return f->deps[f->public_deps[i]];
}

const wpi_upb_FileDef* wpi_upb_FileDef_WeakDependency(const wpi_upb_FileDef* f, int i) {
  WPI_UPB_ASSERT(0 <= i && i < f->public_dep_count);
  return f->deps[f->weak_deps[i]];
}

const wpi_upb_MessageDef* wpi_upb_FileDef_TopLevelMessage(const wpi_upb_FileDef* f, int i) {
  WPI_UPB_ASSERT(0 <= i && i < f->top_lvl_msg_count);
  return _wpi_upb_MessageDef_At(f->top_lvl_msgs, i);
}

const wpi_upb_EnumDef* wpi_upb_FileDef_TopLevelEnum(const wpi_upb_FileDef* f, int i) {
  WPI_UPB_ASSERT(0 <= i && i < f->top_lvl_enum_count);
  return _wpi_upb_EnumDef_At(f->top_lvl_enums, i);
}

const wpi_upb_FieldDef* wpi_upb_FileDef_TopLevelExtension(const wpi_upb_FileDef* f, int i) {
  WPI_UPB_ASSERT(0 <= i && i < f->top_lvl_ext_count);
  return _wpi_upb_FieldDef_At(f->top_lvl_exts, i);
}

const wpi_upb_ServiceDef* wpi_upb_FileDef_Service(const wpi_upb_FileDef* f, int i) {
  WPI_UPB_ASSERT(0 <= i && i < f->service_count);
  return _wpi_upb_ServiceDef_At(f->services, i);
}

const wpi_upb_DefPool* wpi_upb_FileDef_Pool(const wpi_upb_FileDef* f) { return f->symtab; }

const wpi_upb_MiniTableExtension* _wpi_upb_FileDef_ExtensionMiniTable(
    const wpi_upb_FileDef* f, int i) {
  return f->ext_layouts[i];
}

// Note: Import cycles are not allowed so this will terminate.
bool wpi_upb_FileDef_Resolves(const wpi_upb_FileDef* f, const char* path) {
  if (!strcmp(f->name, path)) return true;

  for (int i = 0; i < wpi_upb_FileDef_PublicDependencyCount(f); i++) {
    const wpi_upb_FileDef* dep = wpi_upb_FileDef_PublicDependency(f, i);
    if (wpi_upb_FileDef_Resolves(dep, path)) return true;
  }
  return false;
}

static char* strviewdup(wpi_upb_DefBuilder* ctx, wpi_upb_StringView view) {
  char* ret = wpi_upb_strdup2(view.data, view.size, _wpi_upb_DefBuilder_Arena(ctx));
  if (!ret) _wpi_upb_DefBuilder_OomErr(ctx);
  return ret;
}

static bool streql_view(wpi_upb_StringView view, const char* b) {
  return view.size == strlen(b) && memcmp(view.data, b, view.size) == 0;
}

static int count_exts_in_msg(const WPI_UPB_DESC(DescriptorProto) * msg_proto) {
  size_t n;
  WPI_UPB_DESC(DescriptorProto_extension)(msg_proto, &n);
  int ext_count = n;

  const WPI_UPB_DESC(DescriptorProto)* const* nested_msgs =
      WPI_UPB_DESC(DescriptorProto_nested_type)(msg_proto, &n);
  for (size_t i = 0; i < n; i++) {
    ext_count += count_exts_in_msg(nested_msgs[i]);
  }

  return ext_count;
}

const WPI_UPB_DESC(FeatureSet*)
    _wpi_upb_FileDef_FindEdition(wpi_upb_DefBuilder* ctx, int edition) {
  const WPI_UPB_DESC(FeatureSetDefaults)* defaults =
      wpi_upb_DefPool_FeatureSetDefaults(ctx->symtab);

  int min = WPI_UPB_DESC(FeatureSetDefaults_minimum_edition)(defaults);
  int max = WPI_UPB_DESC(FeatureSetDefaults_maximum_edition)(defaults);
  if (edition < min) {
    _wpi_upb_DefBuilder_Errf(ctx,
                         "Edition %s is earlier than the minimum edition %s "
                         "given in the defaults",
                         wpi_upb_FileDef_EditionName(edition),
                         wpi_upb_FileDef_EditionName(min));
    return NULL;
  }
  if (edition > max) {
    _wpi_upb_DefBuilder_Errf(ctx,
                         "Edition %s is later than the maximum edition %s "
                         "given in the defaults",
                         wpi_upb_FileDef_EditionName(edition),
                         wpi_upb_FileDef_EditionName(max));
    return NULL;
  }

  size_t n;
  const WPI_UPB_DESC(FeatureSetDefaults_FeatureSetEditionDefault)* const* d =
      WPI_UPB_DESC(FeatureSetDefaults_defaults)(defaults, &n);
  const WPI_UPB_DESC(FeatureSetDefaults_FeatureSetEditionDefault)* result = NULL;
  for (size_t i = 0; i < n; i++) {
    if (WPI_UPB_DESC(FeatureSetDefaults_FeatureSetEditionDefault_edition)(d[i]) >
        edition) {
      break;
    }
    result = d[i];
  }
  if (result == NULL) {
    _wpi_upb_DefBuilder_Errf(ctx, "No valid default found for edition %s",
                         wpi_upb_FileDef_EditionName(edition));
    return NULL;
  }

  // Merge the fixed and overridable features to get the edition's default
  // feature set.
  const WPI_UPB_DESC(FeatureSet)* fixed = WPI_UPB_DESC(
      FeatureSetDefaults_FeatureSetEditionDefault_fixed_features)(result);
  const WPI_UPB_DESC(FeatureSet)* overridable = WPI_UPB_DESC(
      FeatureSetDefaults_FeatureSetEditionDefault_overridable_features)(result);
  if (!fixed && !overridable) {
    _wpi_upb_DefBuilder_Errf(ctx, "No valid default found for edition %s",
                         wpi_upb_FileDef_EditionName(edition));
    return NULL;
  } else if (!fixed) {
    return overridable;
  }
  return _wpi_upb_DefBuilder_DoResolveFeatures(ctx, fixed, overridable,
                                           /*is_implicit=*/true);
}

// Allocate and initialize one file def, and add it to the context object.
void _wpi_upb_FileDef_Create(wpi_upb_DefBuilder* ctx,
                         const WPI_UPB_DESC(FileDescriptorProto) * file_proto) {
  wpi_upb_FileDef* file = _wpi_upb_DefBuilder_Alloc(ctx, sizeof(wpi_upb_FileDef));
  ctx->file = file;

  const WPI_UPB_DESC(DescriptorProto)* const* msgs;
  const WPI_UPB_DESC(EnumDescriptorProto)* const* enums;
  const WPI_UPB_DESC(FieldDescriptorProto)* const* exts;
  const WPI_UPB_DESC(ServiceDescriptorProto)* const* services;
  const wpi_upb_StringView* strs;
  const int32_t* public_deps;
  const int32_t* weak_deps;
  size_t n;

  file->symtab = ctx->symtab;

  // Count all extensions in the file, to build a flat array of layouts.
  WPI_UPB_DESC(FileDescriptorProto_extension)(file_proto, &n);
  int ext_count = n;
  msgs = WPI_UPB_DESC(FileDescriptorProto_message_type)(file_proto, &n);
  for (size_t i = 0; i < n; i++) {
    ext_count += count_exts_in_msg(msgs[i]);
  }
  file->ext_count = ext_count;

  if (ctx->layout) {
    // We are using the ext layouts that were passed in.
    file->ext_layouts = ctx->layout->WPI_UPB_PRIVATE(exts);
    const int mt_ext_count = wpi_upb_MiniTableFile_ExtensionCount(ctx->layout);
    if (mt_ext_count != file->ext_count) {
      _wpi_upb_DefBuilder_Errf(ctx,
                           "Extension count did not match layout (%d vs %d)",
                           mt_ext_count, file->ext_count);
    }
  } else {
    // We are building ext layouts from scratch.
    file->ext_layouts = _wpi_upb_DefBuilder_Alloc(
        ctx, sizeof(*file->ext_layouts) * file->ext_count);
    wpi_upb_MiniTableExtension* ext =
        _wpi_upb_DefBuilder_Alloc(ctx, sizeof(*ext) * file->ext_count);
    for (int i = 0; i < file->ext_count; i++) {
      file->ext_layouts[i] = &ext[i];
    }
  }

  wpi_upb_StringView name = WPI_UPB_DESC(FileDescriptorProto_name)(file_proto);
  file->name = strviewdup(ctx, name);
  if (strlen(file->name) != name.size) {
    _wpi_upb_DefBuilder_Errf(ctx, "File name contained embedded NULL");
  }

  wpi_upb_StringView package = WPI_UPB_DESC(FileDescriptorProto_package)(file_proto);

  if (package.size) {
    _wpi_upb_DefBuilder_CheckIdentFull(ctx, package);
    file->package = strviewdup(ctx, package);
  } else {
    file->package = NULL;
  }

  // TODO: How should we validate this?
  file->edition = WPI_UPB_DESC(FileDescriptorProto_edition)(file_proto);

  if (WPI_UPB_DESC(FileDescriptorProto_has_syntax)(file_proto)) {
    wpi_upb_StringView syntax = WPI_UPB_DESC(FileDescriptorProto_syntax)(file_proto);

    if (streql_view(syntax, "proto2")) {
      file->syntax = kUpb_Syntax_Proto2;
      file->edition = WPI_UPB_DESC(EDITION_PROTO2);
    } else if (streql_view(syntax, "proto3")) {
      file->syntax = kUpb_Syntax_Proto3;
      file->edition = WPI_UPB_DESC(EDITION_PROTO3);
    } else if (streql_view(syntax, "editions")) {
      file->syntax = kUpb_Syntax_Editions;
      file->edition = WPI_UPB_DESC(FileDescriptorProto_edition)(file_proto);
    } else {
      _wpi_upb_DefBuilder_Errf(ctx, "Invalid syntax '" WPI_UPB_STRINGVIEW_FORMAT "'",
                           WPI_UPB_STRINGVIEW_ARGS(syntax));
    }
  } else {
    file->syntax = kUpb_Syntax_Proto2;
    file->edition = WPI_UPB_DESC(EDITION_PROTO2);
  }

  // Read options.
  WPI_UPB_DEF_SET_OPTIONS(file->opts, FileDescriptorProto, FileOptions, file_proto);

  // Resolve features.
  const WPI_UPB_DESC(FeatureSet*) edition_defaults =
      _wpi_upb_FileDef_FindEdition(ctx, file->edition);
  const WPI_UPB_DESC(FeatureSet*) unresolved =
      WPI_UPB_DESC(FileOptions_features)(file->opts);
  file->resolved_features =
      _wpi_upb_DefBuilder_ResolveFeatures(ctx, edition_defaults, unresolved);

  // Verify dependencies.
  strs = WPI_UPB_DESC(FileDescriptorProto_dependency)(file_proto, &n);
  file->dep_count = n;
  file->deps = _wpi_upb_DefBuilder_Alloc(ctx, sizeof(*file->deps) * n);

  for (size_t i = 0; i < n; i++) {
    wpi_upb_StringView str = strs[i];
    file->deps[i] =
        wpi_upb_DefPool_FindFileByNameWithSize(ctx->symtab, str.data, str.size);
    if (!file->deps[i]) {
      _wpi_upb_DefBuilder_Errf(ctx,
                           "Depends on file '" WPI_UPB_STRINGVIEW_FORMAT
                           "', but it has not been loaded",
                           WPI_UPB_STRINGVIEW_ARGS(str));
    }
  }

  public_deps = WPI_UPB_DESC(FileDescriptorProto_public_dependency)(file_proto, &n);
  file->public_dep_count = n;
  file->public_deps =
      _wpi_upb_DefBuilder_Alloc(ctx, sizeof(*file->public_deps) * n);
  int32_t* mutable_public_deps = (int32_t*)file->public_deps;
  for (size_t i = 0; i < n; i++) {
    if (public_deps[i] >= file->dep_count) {
      _wpi_upb_DefBuilder_Errf(ctx, "public_dep %d is out of range",
                           (int)public_deps[i]);
    }
    mutable_public_deps[i] = public_deps[i];
  }

  weak_deps = WPI_UPB_DESC(FileDescriptorProto_weak_dependency)(file_proto, &n);
  file->weak_dep_count = n;
  file->weak_deps = _wpi_upb_DefBuilder_Alloc(ctx, sizeof(*file->weak_deps) * n);
  int32_t* mutable_weak_deps = (int32_t*)file->weak_deps;
  for (size_t i = 0; i < n; i++) {
    if (weak_deps[i] >= file->dep_count) {
      _wpi_upb_DefBuilder_Errf(ctx, "weak_dep %d is out of range",
                           (int)weak_deps[i]);
    }
    mutable_weak_deps[i] = weak_deps[i];
  }

  // Create enums.
  enums = WPI_UPB_DESC(FileDescriptorProto_enum_type)(file_proto, &n);
  file->top_lvl_enum_count = n;
  file->top_lvl_enums =
      _wpi_upb_EnumDefs_New(ctx, n, enums, file->resolved_features, NULL);

  // Create extensions.
  exts = WPI_UPB_DESC(FileDescriptorProto_extension)(file_proto, &n);
  file->top_lvl_ext_count = n;
  file->top_lvl_exts = _wpi_upb_Extensions_New(
      ctx, n, exts, file->resolved_features, file->package, NULL);

  // Create messages.
  msgs = WPI_UPB_DESC(FileDescriptorProto_message_type)(file_proto, &n);
  file->top_lvl_msg_count = n;
  file->top_lvl_msgs =
      _wpi_upb_MessageDefs_New(ctx, n, msgs, file->resolved_features, NULL);

  // Create services.
  services = WPI_UPB_DESC(FileDescriptorProto_service)(file_proto, &n);
  file->service_count = n;
  file->services =
      _wpi_upb_ServiceDefs_New(ctx, n, services, file->resolved_features);

  // Now that all names are in the table, build layouts and resolve refs.

  for (int i = 0; i < file->top_lvl_msg_count; i++) {
    wpi_upb_MessageDef* m = (wpi_upb_MessageDef*)wpi_upb_FileDef_TopLevelMessage(file, i);
    _wpi_upb_MessageDef_Resolve(ctx, m);
  }

  for (int i = 0; i < file->top_lvl_ext_count; i++) {
    wpi_upb_FieldDef* f = (wpi_upb_FieldDef*)wpi_upb_FileDef_TopLevelExtension(file, i);
    _wpi_upb_FieldDef_Resolve(ctx, file->package, f);
  }

  for (int i = 0; i < file->top_lvl_msg_count; i++) {
    wpi_upb_MessageDef* m = (wpi_upb_MessageDef*)wpi_upb_FileDef_TopLevelMessage(file, i);
    _wpi_upb_MessageDef_CreateMiniTable(ctx, (wpi_upb_MessageDef*)m);
  }

  for (int i = 0; i < file->top_lvl_ext_count; i++) {
    wpi_upb_FieldDef* f = (wpi_upb_FieldDef*)wpi_upb_FileDef_TopLevelExtension(file, i);
    _wpi_upb_FieldDef_BuildMiniTableExtension(ctx, f);
  }

  for (int i = 0; i < file->top_lvl_msg_count; i++) {
    wpi_upb_MessageDef* m = (wpi_upb_MessageDef*)wpi_upb_FileDef_TopLevelMessage(file, i);
    _wpi_upb_MessageDef_LinkMiniTable(ctx, m);
  }

  if (file->ext_count) {
    bool ok = wpi_upb_ExtensionRegistry_AddArray(
        _wpi_upb_DefPool_ExtReg(ctx->symtab), file->ext_layouts, file->ext_count);
    if (!ok) _wpi_upb_DefBuilder_OomErr(ctx);
  }
}


#include <string.h>


// Must be last.

/* The upb core does not generally have a concept of default instances. However
 * for descriptor options we make an exception since the max size is known and
 * modest (<200 bytes). All types can share a default instance since it is
 * initialized to zeroes.
 *
 * We have to allocate an extra pointer for upb's internal metadata. */
static WPI_UPB_ALIGN_AS(8) const
    char opt_default_buf[_WPI_UPB_MAXOPT_SIZE + sizeof(void*)] = {0};
const char* kUpbDefOptDefault = &opt_default_buf[sizeof(void*)];

const char* _wpi_upb_DefBuilder_FullToShort(const char* fullname) {
  const char* p;

  if (fullname == NULL) {
    return NULL;
  } else if ((p = strrchr(fullname, '.')) == NULL) {
    /* No '.' in the name, return the full string. */
    return fullname;
  } else {
    /* Return one past the last '.'. */
    return p + 1;
  }
}

void _wpi_upb_DefBuilder_FailJmp(wpi_upb_DefBuilder* ctx) { WPI_UPB_LONGJMP(ctx->err, 1); }

void _wpi_upb_DefBuilder_Errf(wpi_upb_DefBuilder* ctx, const char* fmt, ...) {
  va_list argp;
  va_start(argp, fmt);
  wpi_upb_Status_VSetErrorFormat(ctx->status, fmt, argp);
  va_end(argp);
  _wpi_upb_DefBuilder_FailJmp(ctx);
}

void _wpi_upb_DefBuilder_OomErr(wpi_upb_DefBuilder* ctx) {
  wpi_upb_Status_SetErrorMessage(ctx->status, "out of memory");
  _wpi_upb_DefBuilder_FailJmp(ctx);
}

// Verify a relative identifier string. The loop is branchless for speed.
static void _wpi_upb_DefBuilder_CheckIdentNotFull(wpi_upb_DefBuilder* ctx,
                                              wpi_upb_StringView name) {
  bool good = name.size > 0;

  for (size_t i = 0; i < name.size; i++) {
    const char c = name.data[i];
    const char d = c | 0x20;  // force lowercase
    const bool is_alpha = (('a' <= d) & (d <= 'z')) | (c == '_');
    const bool is_numer = ('0' <= c) & (c <= '9') & (i != 0);

    good &= is_alpha | is_numer;
  }

  if (!good) _wpi_upb_DefBuilder_CheckIdentSlow(ctx, name, false);
}

const char* _wpi_upb_DefBuilder_MakeFullName(wpi_upb_DefBuilder* ctx,
                                         const char* prefix,
                                         wpi_upb_StringView name) {
  _wpi_upb_DefBuilder_CheckIdentNotFull(ctx, name);
  if (prefix) {
    // ret = prefix + '.' + name;
    size_t n = strlen(prefix);
    char* ret = _wpi_upb_DefBuilder_Alloc(ctx, n + name.size + 2);
    strcpy(ret, prefix);
    ret[n] = '.';
    memcpy(&ret[n + 1], name.data, name.size);
    ret[n + 1 + name.size] = '\0';
    return ret;
  } else {
    char* ret = wpi_upb_strdup2(name.data, name.size, ctx->arena);
    if (!ret) _wpi_upb_DefBuilder_OomErr(ctx);
    return ret;
  }
}

static bool remove_component(char* base, size_t* len) {
  if (*len == 0) return false;

  for (size_t i = *len - 1; i > 0; i--) {
    if (base[i] == '.') {
      *len = i;
      return true;
    }
  }

  *len = 0;
  return true;
}

const void* _wpi_upb_DefBuilder_ResolveAny(wpi_upb_DefBuilder* ctx,
                                       const char* from_name_dbg,
                                       const char* base, wpi_upb_StringView sym,
                                       wpi_upb_deftype_t* type) {
  if (sym.size == 0) goto notfound;
  wpi_upb_value v;
  if (sym.data[0] == '.') {
    // Symbols starting with '.' are absolute, so we do a single lookup.
    // Slice to omit the leading '.'
    if (!_wpi_upb_DefPool_LookupSym(ctx->symtab, sym.data + 1, sym.size - 1, &v)) {
      goto notfound;
    }
  } else {
    // Remove components from base until we find an entry or run out.
    size_t baselen = base ? strlen(base) : 0;
    char* tmp = wpi_upb_gmalloc(sym.size + baselen + 1);
    while (1) {
      char* p = tmp;
      if (baselen) {
        memcpy(p, base, baselen);
        p[baselen] = '.';
        p += baselen + 1;
      }
      memcpy(p, sym.data, sym.size);
      p += sym.size;
      if (_wpi_upb_DefPool_LookupSym(ctx->symtab, tmp, p - tmp, &v)) {
        break;
      }
      if (!remove_component(tmp, &baselen)) {
        wpi_upb_gfree(tmp);
        goto notfound;
      }
    }
    wpi_upb_gfree(tmp);
  }

  *type = _wpi_upb_DefType_Type(v);
  return _wpi_upb_DefType_Unpack(v, *type);

notfound:
  _wpi_upb_DefBuilder_Errf(ctx, "couldn't resolve name '" WPI_UPB_STRINGVIEW_FORMAT "'",
                       WPI_UPB_STRINGVIEW_ARGS(sym));
}

const void* _wpi_upb_DefBuilder_Resolve(wpi_upb_DefBuilder* ctx,
                                    const char* from_name_dbg, const char* base,
                                    wpi_upb_StringView sym, wpi_upb_deftype_t type) {
  wpi_upb_deftype_t found_type;
  const void* ret =
      _wpi_upb_DefBuilder_ResolveAny(ctx, from_name_dbg, base, sym, &found_type);
  if (ret && found_type != type) {
    _wpi_upb_DefBuilder_Errf(ctx,
                         "type mismatch when resolving %s: couldn't find "
                         "name " WPI_UPB_STRINGVIEW_FORMAT " with type=%d",
                         from_name_dbg, WPI_UPB_STRINGVIEW_ARGS(sym), (int)type);
  }
  return ret;
}

// Per ASCII this will lower-case a letter. If the result is a letter, the
// input was definitely a letter. If the output is not a letter, this may
// have transformed the character unpredictably.
static char wpi_upb_ascii_lower(char ch) { return ch | 0x20; }

// isalpha() etc. from <ctype.h> are locale-dependent, which we don't want.
static bool wpi_upb_isbetween(uint8_t c, uint8_t low, uint8_t high) {
  return low <= c && c <= high;
}

static bool wpi_upb_isletter(char c) {
  char lower = wpi_upb_ascii_lower(c);
  return wpi_upb_isbetween(lower, 'a', 'z') || c == '_';
}

static bool wpi_upb_isalphanum(char c) {
  return wpi_upb_isletter(c) || wpi_upb_isbetween(c, '0', '9');
}

static bool TryGetChar(const char** src, const char* end, char* ch) {
  if (*src == end) return false;
  *ch = **src;
  *src += 1;
  return true;
}

static int TryGetHexDigit(const char** src, const char* end) {
  char ch;
  if (!TryGetChar(src, end, &ch)) return -1;
  if ('0' <= ch && ch <= '9') {
    return ch - '0';
  }
  ch = wpi_upb_ascii_lower(ch);
  if ('a' <= ch && ch <= 'f') {
    return ch - 'a' + 0xa;
  }
  *src -= 1;  // Char wasn't actually a hex digit.
  return -1;
}

static char wpi_upb_DefBuilder_ParseHexEscape(wpi_upb_DefBuilder* ctx,
                                          const wpi_upb_FieldDef* f,
                                          const char** src, const char* end) {
  int hex_digit = TryGetHexDigit(src, end);
  if (hex_digit < 0) {
    _wpi_upb_DefBuilder_Errf(
        ctx, "\\x must be followed by at least one hex digit (field='%s')",
        wpi_upb_FieldDef_FullName(f));
    return 0;
  }
  unsigned int ret = hex_digit;
  while ((hex_digit = TryGetHexDigit(src, end)) >= 0) {
    ret = (ret << 4) | hex_digit;
  }
  if (ret > 0xff) {
    _wpi_upb_DefBuilder_Errf(ctx, "Value of hex escape in field %s exceeds 8 bits",
                         wpi_upb_FieldDef_FullName(f));
    return 0;
  }
  return ret;
}

static char TryGetOctalDigit(const char** src, const char* end) {
  char ch;
  if (!TryGetChar(src, end, &ch)) return -1;
  if ('0' <= ch && ch <= '7') {
    return ch - '0';
  }
  *src -= 1;  // Char wasn't actually an octal digit.
  return -1;
}

static char wpi_upb_DefBuilder_ParseOctalEscape(wpi_upb_DefBuilder* ctx,
                                            const wpi_upb_FieldDef* f,
                                            const char** src, const char* end) {
  char ch = 0;
  for (int i = 0; i < 3; i++) {
    char digit;
    if ((digit = TryGetOctalDigit(src, end)) >= 0) {
      ch = (ch << 3) | digit;
    }
  }
  return ch;
}

char _wpi_upb_DefBuilder_ParseEscape(wpi_upb_DefBuilder* ctx, const wpi_upb_FieldDef* f,
                                 const char** src, const char* end) {
  char ch;
  if (!TryGetChar(src, end, &ch)) {
    _wpi_upb_DefBuilder_Errf(ctx, "unterminated escape sequence in field %s",
                         wpi_upb_FieldDef_FullName(f));
    return 0;
  }
  switch (ch) {
    case 'a':
      return '\a';
    case 'b':
      return '\b';
    case 'f':
      return '\f';
    case 'n':
      return '\n';
    case 'r':
      return '\r';
    case 't':
      return '\t';
    case 'v':
      return '\v';
    case '\\':
      return '\\';
    case '\'':
      return '\'';
    case '\"':
      return '\"';
    case '?':
      return '\?';
    case 'x':
    case 'X':
      return wpi_upb_DefBuilder_ParseHexEscape(ctx, f, src, end);
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
      *src -= 1;
      return wpi_upb_DefBuilder_ParseOctalEscape(ctx, f, src, end);
  }
  _wpi_upb_DefBuilder_Errf(ctx, "Unknown escape sequence: \\%c", ch);
}

void _wpi_upb_DefBuilder_CheckIdentSlow(wpi_upb_DefBuilder* ctx, wpi_upb_StringView name,
                                    bool full) {
  const char* str = name.data;
  const size_t len = name.size;
  bool start = true;
  for (size_t i = 0; i < len; i++) {
    const char c = str[i];
    if (c == '.') {
      if (start || !full) {
        _wpi_upb_DefBuilder_Errf(
            ctx, "invalid name: unexpected '.' (" WPI_UPB_STRINGVIEW_FORMAT ")",
            WPI_UPB_STRINGVIEW_ARGS(name));
      }
      start = true;
    } else if (start) {
      if (!wpi_upb_isletter(c)) {
        _wpi_upb_DefBuilder_Errf(ctx,
                             "invalid name: path components must start with a "
                             "letter (" WPI_UPB_STRINGVIEW_FORMAT ")",
                             WPI_UPB_STRINGVIEW_ARGS(name));
      }
      start = false;
    } else if (!wpi_upb_isalphanum(c)) {
      _wpi_upb_DefBuilder_Errf(
          ctx,
          "invalid name: non-alphanumeric character (" WPI_UPB_STRINGVIEW_FORMAT
          ")",
          WPI_UPB_STRINGVIEW_ARGS(name));
    }
  }
  if (start) {
    _wpi_upb_DefBuilder_Errf(ctx,
                         "invalid name: empty part (" WPI_UPB_STRINGVIEW_FORMAT ")",
                         WPI_UPB_STRINGVIEW_ARGS(name));
  }

  // We should never reach this point.
  WPI_UPB_ASSERT(false);
}

wpi_upb_StringView _wpi_upb_DefBuilder_MakeKey(wpi_upb_DefBuilder* ctx,
                                       const WPI_UPB_DESC(FeatureSet*) parent,
                                       wpi_upb_StringView key) {
  size_t need = key.size + sizeof(void*);
  if (ctx->tmp_buf_size < need) {
    ctx->tmp_buf_size = WPI_UPB_MAX(64, wpi_upb_Log2Ceiling(need));
    ctx->tmp_buf = wpi_upb_Arena_Malloc(ctx->tmp_arena, ctx->tmp_buf_size);
    if (!ctx->tmp_buf) _wpi_upb_DefBuilder_OomErr(ctx);
  }

  memcpy(ctx->tmp_buf, &parent, sizeof(void*));
  memcpy(ctx->tmp_buf + sizeof(void*), key.data, key.size);
  return wpi_upb_StringView_FromDataAndSize(ctx->tmp_buf, need);
}

bool _wpi_upb_DefBuilder_GetOrCreateFeatureSet(wpi_upb_DefBuilder* ctx,
                                           const WPI_UPB_DESC(FeatureSet*) parent,
                                           wpi_upb_StringView key,
                                           WPI_UPB_DESC(FeatureSet**) set) {
  wpi_upb_StringView k = _wpi_upb_DefBuilder_MakeKey(ctx, parent, key);
  wpi_upb_value v;
  if (wpi_upb_strtable_lookup2(&ctx->feature_cache, k.data, k.size, &v)) {
    *set = wpi_upb_value_getptr(v);
    return false;
  }

  *set = (WPI_UPB_DESC(FeatureSet*))wpi_upb_Message_DeepClone(
      WPI_UPB_UPCAST(parent), WPI_UPB_DESC_MINITABLE(FeatureSet), ctx->arena);
  if (!*set) _wpi_upb_DefBuilder_OomErr(ctx);

  v = wpi_upb_value_ptr(*set);
  if (!wpi_upb_strtable_insert(&ctx->feature_cache, k.data, k.size, v,
                           ctx->tmp_arena)) {
    _wpi_upb_DefBuilder_OomErr(ctx);
  }

  return true;
}

const WPI_UPB_DESC(FeatureSet*)
    _wpi_upb_DefBuilder_DoResolveFeatures(wpi_upb_DefBuilder* ctx,
                                      const WPI_UPB_DESC(FeatureSet*) parent,
                                      const WPI_UPB_DESC(FeatureSet*) child,
                                      bool is_implicit) {
  assert(parent);
  if (!child) return parent;

  if (child && !is_implicit &&
      wpi_upb_FileDef_Syntax(ctx->file) != kUpb_Syntax_Editions) {
    _wpi_upb_DefBuilder_Errf(ctx, "Features can only be specified for editions");
  }

  WPI_UPB_DESC(FeatureSet*) resolved;
  size_t child_size;
  const char* child_bytes =
      WPI_UPB_DESC(FeatureSet_serialize)(child, ctx->tmp_arena, &child_size);
  if (!child_bytes) _wpi_upb_DefBuilder_OomErr(ctx);

  wpi_upb_StringView key = wpi_upb_StringView_FromDataAndSize(child_bytes, child_size);
  if (!_wpi_upb_DefBuilder_GetOrCreateFeatureSet(ctx, parent, key, &resolved)) {
    return resolved;
  }

  wpi_upb_DecodeStatus dec_status =
      wpi_upb_Decode(child_bytes, child_size, WPI_UPB_UPCAST(resolved),
                 WPI_UPB_DESC_MINITABLE(FeatureSet), NULL, 0, ctx->arena);
  if (dec_status != kUpb_DecodeStatus_Ok) _wpi_upb_DefBuilder_OomErr(ctx);

  return resolved;
}


#include <string.h>


// Must be last.

char* wpi_upb_strdup2(const char* s, size_t len, wpi_upb_Arena* a) {
  size_t n;
  char* p;

  // Prevent overflow errors.
  if (len == SIZE_MAX) return NULL;

  // Always null-terminate, even if binary data; but don't rely on the input to
  // have a null-terminating byte since it may be a raw binary buffer.
  n = len + 1;
  p = wpi_upb_Arena_Malloc(a, n);
  if (p) {
    if (len != 0) memcpy(p, s, len);
    p[len] = 0;
  }
  return p;
}


#include <stdint.h>
#include <string.h>


// Must be last.

bool wpi_upb_Message_HasFieldByDef(const wpi_upb_Message* msg, const wpi_upb_FieldDef* f) {
  const wpi_upb_MiniTableField* m_f = wpi_upb_FieldDef_MiniTable(f);
  WPI_UPB_ASSERT(wpi_upb_FieldDef_HasPresence(f));

  if (wpi_upb_MiniTableField_IsExtension(m_f)) {
    return wpi_upb_Message_HasExtension(msg, (const wpi_upb_MiniTableExtension*)m_f);
  } else {
    return wpi_upb_Message_HasBaseField(msg, m_f);
  }
}

const wpi_upb_FieldDef* wpi_upb_Message_WhichOneofByDef(const wpi_upb_Message* msg,
                                                const wpi_upb_OneofDef* o) {
  const wpi_upb_FieldDef* f = wpi_upb_OneofDef_Field(o, 0);
  if (wpi_upb_OneofDef_IsSynthetic(o)) {
    WPI_UPB_ASSERT(wpi_upb_OneofDef_FieldCount(o) == 1);
    return wpi_upb_Message_HasFieldByDef(msg, f) ? f : NULL;
  } else {
    const wpi_upb_MiniTableField* field = wpi_upb_FieldDef_MiniTable(f);
    uint32_t oneof_case = wpi_upb_Message_WhichOneofFieldNumber(msg, field);
    f = oneof_case ? wpi_upb_OneofDef_LookupNumber(o, oneof_case) : NULL;
    WPI_UPB_ASSERT((f != NULL) == (oneof_case != 0));
    return f;
  }
}

wpi_upb_MessageValue wpi_upb_Message_GetFieldByDef(const wpi_upb_Message* msg,
                                           const wpi_upb_FieldDef* f) {
  wpi_upb_MessageValue default_val = wpi_upb_FieldDef_Default(f);
  return wpi_upb_Message_GetField(msg, wpi_upb_FieldDef_MiniTable(f), default_val);
}

wpi_upb_MutableMessageValue wpi_upb_Message_Mutable(wpi_upb_Message* msg,
                                            const wpi_upb_FieldDef* f,
                                            wpi_upb_Arena* a) {
  WPI_UPB_ASSERT(!wpi_upb_Message_IsFrozen(msg));
  WPI_UPB_ASSERT(wpi_upb_FieldDef_IsSubMessage(f) || wpi_upb_FieldDef_IsRepeated(f));
  if (wpi_upb_FieldDef_HasPresence(f) && !wpi_upb_Message_HasFieldByDef(msg, f)) {
    // We need to skip the wpi_upb_Message_GetFieldByDef() call in this case.
    goto make;
  }

  wpi_upb_MessageValue val = wpi_upb_Message_GetFieldByDef(msg, f);
  if (val.array_val) {
    return (wpi_upb_MutableMessageValue){.array = (wpi_upb_Array*)val.array_val};
  }

  wpi_upb_MutableMessageValue ret;
make:
  if (!a) return (wpi_upb_MutableMessageValue){.array = NULL};
  if (wpi_upb_FieldDef_IsMap(f)) {
    const wpi_upb_MessageDef* entry = wpi_upb_FieldDef_MessageSubDef(f);
    const wpi_upb_FieldDef* key =
        wpi_upb_MessageDef_FindFieldByNumber(entry, kUpb_MapEntry_KeyFieldNumber);
    const wpi_upb_FieldDef* value =
        wpi_upb_MessageDef_FindFieldByNumber(entry, kUpb_MapEntry_ValueFieldNumber);
    ret.map =
        wpi_upb_Map_New(a, wpi_upb_FieldDef_CType(key), wpi_upb_FieldDef_CType(value));
  } else if (wpi_upb_FieldDef_IsRepeated(f)) {
    ret.array = wpi_upb_Array_New(a, wpi_upb_FieldDef_CType(f));
  } else {
    WPI_UPB_ASSERT(wpi_upb_FieldDef_IsSubMessage(f));
    const wpi_upb_MessageDef* m = wpi_upb_FieldDef_MessageSubDef(f);
    ret.msg = wpi_upb_Message_New(wpi_upb_MessageDef_MiniTable(m), a);
  }

  val.array_val = ret.array;
  wpi_upb_Message_SetFieldByDef(msg, f, val, a);

  return ret;
}

bool wpi_upb_Message_SetFieldByDef(wpi_upb_Message* msg, const wpi_upb_FieldDef* f,
                               wpi_upb_MessageValue val, wpi_upb_Arena* a) {
  WPI_UPB_ASSERT(!wpi_upb_Message_IsFrozen(msg));
  const wpi_upb_MiniTableField* m_f = wpi_upb_FieldDef_MiniTable(f);

  if (wpi_upb_MiniTableField_IsExtension(m_f)) {
    return wpi_upb_Message_SetExtension(msg, (const wpi_upb_MiniTableExtension*)m_f,
                                    &val, a);
  } else {
    wpi_upb_Message_SetBaseField(msg, m_f, &val);
    return true;
  }
}

void wpi_upb_Message_ClearFieldByDef(wpi_upb_Message* msg, const wpi_upb_FieldDef* f) {
  WPI_UPB_ASSERT(!wpi_upb_Message_IsFrozen(msg));
  const wpi_upb_MiniTableField* m_f = wpi_upb_FieldDef_MiniTable(f);

  if (wpi_upb_MiniTableField_IsExtension(m_f)) {
    wpi_upb_Message_ClearExtension(msg, (const wpi_upb_MiniTableExtension*)m_f);
  } else {
    wpi_upb_Message_ClearBaseField(msg, m_f);
  }
}

void wpi_upb_Message_ClearByDef(wpi_upb_Message* msg, const wpi_upb_MessageDef* m) {
  WPI_UPB_ASSERT(!wpi_upb_Message_IsFrozen(msg));
  wpi_upb_Message_Clear(msg, wpi_upb_MessageDef_MiniTable(m));
}

bool wpi_upb_Message_Next(const wpi_upb_Message* msg, const wpi_upb_MessageDef* m,
                      const wpi_upb_DefPool* ext_pool, const wpi_upb_FieldDef** out_f,
                      wpi_upb_MessageValue* out_val, size_t* iter) {
  const wpi_upb_MiniTable* mt = wpi_upb_MessageDef_MiniTable(m);
  size_t i = *iter;
  size_t n = wpi_upb_MiniTable_FieldCount(mt);
  wpi_upb_MessageValue zero = wpi_upb_MessageValue_Zero();
  WPI_UPB_UNUSED(ext_pool);

  // Iterate over normal fields, returning the first one that is set.
  while (++i < n) {
    const wpi_upb_MiniTableField* field = wpi_upb_MiniTable_GetFieldByIndex(mt, i);
    wpi_upb_MessageValue val = wpi_upb_Message_GetField(msg, field, zero);

    // Skip field if unset or empty.
    if (wpi_upb_MiniTableField_HasPresence(field)) {
      if (!wpi_upb_Message_HasBaseField(msg, field)) continue;
    } else {
      switch (WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_Mode)(field)) {
        case kUpb_FieldMode_Map:
          if (!val.map_val || wpi_upb_Map_Size(val.map_val) == 0) continue;
          break;
        case kUpb_FieldMode_Array:
          if (!val.array_val || wpi_upb_Array_Size(val.array_val) == 0) continue;
          break;
        case kUpb_FieldMode_Scalar:
          if (WPI_UPB_PRIVATE(_wpi_upb_MiniTableField_DataIsZero)(field, &val))
            continue;
          break;
      }
    }

    *out_val = val;
    *out_f =
        wpi_upb_MessageDef_FindFieldByNumber(m, wpi_upb_MiniTableField_Number(field));
    *iter = i;
    return true;
  }

  if (ext_pool) {
    // Return any extensions that are set.
    size_t count;
    const wpi_upb_Extension* ext = WPI_UPB_PRIVATE(_wpi_upb_Message_Getexts)(msg, &count);
    if (i - n < count) {
      ext += count - 1 - (i - n);
      memcpy(out_val, &ext->data, sizeof(*out_val));
      *out_f = wpi_upb_DefPool_FindExtensionByMiniTable(ext_pool, ext->ext);
      *iter = i;
      return true;
    }
  }

  *iter = i;
  return false;
}

bool _wpi_upb_Message_DiscardUnknown(wpi_upb_Message* msg, const wpi_upb_MessageDef* m,
                                 int depth) {
  WPI_UPB_ASSERT(!wpi_upb_Message_IsFrozen(msg));
  size_t iter = kUpb_Message_Begin;
  const wpi_upb_FieldDef* f;
  wpi_upb_MessageValue val;
  bool ret = true;

  if (--depth == 0) return false;

  _wpi_upb_Message_DiscardUnknown_shallow(msg);

  while (wpi_upb_Message_Next(msg, m, NULL /*ext_pool*/, &f, &val, &iter)) {
    const wpi_upb_MessageDef* subm = wpi_upb_FieldDef_MessageSubDef(f);
    if (!subm) continue;
    if (wpi_upb_FieldDef_IsMap(f)) {
      const wpi_upb_FieldDef* val_f = wpi_upb_MessageDef_FindFieldByNumber(subm, 2);
      const wpi_upb_MessageDef* val_m = wpi_upb_FieldDef_MessageSubDef(val_f);
      wpi_upb_Map* map = (wpi_upb_Map*)val.map_val;
      size_t iter = kUpb_Map_Begin;

      if (!val_m) continue;

      wpi_upb_MessageValue map_key, map_val;
      while (wpi_upb_Map_Next(map, &map_key, &map_val, &iter)) {
        if (!_wpi_upb_Message_DiscardUnknown((wpi_upb_Message*)map_val.msg_val, val_m,
                                         depth)) {
          ret = false;
        }
      }
    } else if (wpi_upb_FieldDef_IsRepeated(f)) {
      const wpi_upb_Array* arr = val.array_val;
      size_t i, n = wpi_upb_Array_Size(arr);
      for (i = 0; i < n; i++) {
        wpi_upb_MessageValue elem = wpi_upb_Array_Get(arr, i);
        if (!_wpi_upb_Message_DiscardUnknown((wpi_upb_Message*)elem.msg_val, subm,
                                         depth)) {
          ret = false;
        }
      }
    } else {
      if (!_wpi_upb_Message_DiscardUnknown((wpi_upb_Message*)val.msg_val, subm,
                                       depth)) {
        ret = false;
      }
    }
  }

  return ret;
}

bool wpi_upb_Message_DiscardUnknown(wpi_upb_Message* msg, const wpi_upb_MessageDef* m,
                                int maxdepth) {
  return _wpi_upb_Message_DiscardUnknown(msg, m, maxdepth);
}


#include <stddef.h>
#include <stdint.h>
#include <string.h>


// Must be last.

struct wpi_upb_MessageDef {
  const WPI_UPB_DESC(MessageOptions*) opts;
  const WPI_UPB_DESC(FeatureSet*) resolved_features;
  const wpi_upb_MiniTable* layout;
  const wpi_upb_FileDef* file;
  const wpi_upb_MessageDef* containing_type;
  const char* full_name;

  // Tables for looking up fields by number and name.
  wpi_upb_inttable itof;
  wpi_upb_strtable ntof;

  // Looking up fields by json name.
  wpi_upb_strtable jtof;

  /* All nested defs.
   * MEM: We could save some space here by putting nested defs in a contiguous
   * region and calculating counts from offsets or vice-versa. */
  const wpi_upb_FieldDef* fields;
  const wpi_upb_OneofDef* oneofs;
  const wpi_upb_ExtensionRange* ext_ranges;
  const wpi_upb_StringView* res_names;
  const wpi_upb_MessageDef* nested_msgs;
  const wpi_upb_MessageReservedRange* res_ranges;
  const wpi_upb_EnumDef* nested_enums;
  const wpi_upb_FieldDef* nested_exts;

  // TODO: These counters don't need anywhere near 32 bits.
  int field_count;
  int real_oneof_count;
  int oneof_count;
  int ext_range_count;
  int res_range_count;
  int res_name_count;
  int nested_msg_count;
  int nested_enum_count;
  int nested_ext_count;
  bool in_message_set;
  bool is_sorted;
  wpi_upb_WellKnown well_known_type;
#if UINTPTR_MAX == 0xffffffff
  uint32_t padding;  // Increase size to a multiple of 8.
#endif
};

static void assign_msg_wellknowntype(wpi_upb_MessageDef* m) {
  const char* name = m->full_name;
  if (name == NULL) {
    m->well_known_type = kUpb_WellKnown_Unspecified;
    return;
  }
  if (!strcmp(name, "google.protobuf.Any")) {
    m->well_known_type = kUpb_WellKnown_Any;
  } else if (!strcmp(name, "google.protobuf.FieldMask")) {
    m->well_known_type = kUpb_WellKnown_FieldMask;
  } else if (!strcmp(name, "google.protobuf.Duration")) {
    m->well_known_type = kUpb_WellKnown_Duration;
  } else if (!strcmp(name, "google.protobuf.Timestamp")) {
    m->well_known_type = kUpb_WellKnown_Timestamp;
  } else if (!strcmp(name, "google.protobuf.DoubleValue")) {
    m->well_known_type = kUpb_WellKnown_DoubleValue;
  } else if (!strcmp(name, "google.protobuf.FloatValue")) {
    m->well_known_type = kUpb_WellKnown_FloatValue;
  } else if (!strcmp(name, "google.protobuf.Int64Value")) {
    m->well_known_type = kUpb_WellKnown_Int64Value;
  } else if (!strcmp(name, "google.protobuf.UInt64Value")) {
    m->well_known_type = kUpb_WellKnown_UInt64Value;
  } else if (!strcmp(name, "google.protobuf.Int32Value")) {
    m->well_known_type = kUpb_WellKnown_Int32Value;
  } else if (!strcmp(name, "google.protobuf.UInt32Value")) {
    m->well_known_type = kUpb_WellKnown_UInt32Value;
  } else if (!strcmp(name, "google.protobuf.BoolValue")) {
    m->well_known_type = kUpb_WellKnown_BoolValue;
  } else if (!strcmp(name, "google.protobuf.StringValue")) {
    m->well_known_type = kUpb_WellKnown_StringValue;
  } else if (!strcmp(name, "google.protobuf.BytesValue")) {
    m->well_known_type = kUpb_WellKnown_BytesValue;
  } else if (!strcmp(name, "google.protobuf.Value")) {
    m->well_known_type = kUpb_WellKnown_Value;
  } else if (!strcmp(name, "google.protobuf.ListValue")) {
    m->well_known_type = kUpb_WellKnown_ListValue;
  } else if (!strcmp(name, "google.protobuf.Struct")) {
    m->well_known_type = kUpb_WellKnown_Struct;
  } else {
    m->well_known_type = kUpb_WellKnown_Unspecified;
  }
}

wpi_upb_MessageDef* _wpi_upb_MessageDef_At(const wpi_upb_MessageDef* m, int i) {
  return (wpi_upb_MessageDef*)&m[i];
}

bool _wpi_upb_MessageDef_IsValidExtensionNumber(const wpi_upb_MessageDef* m, int n) {
  for (int i = 0; i < m->ext_range_count; i++) {
    const wpi_upb_ExtensionRange* r = wpi_upb_MessageDef_ExtensionRange(m, i);
    if (wpi_upb_ExtensionRange_Start(r) <= n && n < wpi_upb_ExtensionRange_End(r)) {
      return true;
    }
  }
  return false;
}

const WPI_UPB_DESC(MessageOptions) *
    wpi_upb_MessageDef_Options(const wpi_upb_MessageDef* m) {
  return m->opts;
}

bool wpi_upb_MessageDef_HasOptions(const wpi_upb_MessageDef* m) {
  return m->opts != (void*)kUpbDefOptDefault;
}

const WPI_UPB_DESC(FeatureSet) *
    wpi_upb_MessageDef_ResolvedFeatures(const wpi_upb_MessageDef* m) {
  return m->resolved_features;
}

const char* wpi_upb_MessageDef_FullName(const wpi_upb_MessageDef* m) {
  return m->full_name;
}

const wpi_upb_FileDef* wpi_upb_MessageDef_File(const wpi_upb_MessageDef* m) {
  return m->file;
}

const wpi_upb_MessageDef* wpi_upb_MessageDef_ContainingType(const wpi_upb_MessageDef* m) {
  return m->containing_type;
}

const char* wpi_upb_MessageDef_Name(const wpi_upb_MessageDef* m) {
  return _wpi_upb_DefBuilder_FullToShort(m->full_name);
}

wpi_upb_Syntax wpi_upb_MessageDef_Syntax(const wpi_upb_MessageDef* m) {
  return wpi_upb_FileDef_Syntax(m->file);
}

const wpi_upb_FieldDef* wpi_upb_MessageDef_FindFieldByNumber(const wpi_upb_MessageDef* m,
                                                     uint32_t i) {
  wpi_upb_value val;
  return wpi_upb_inttable_lookup(&m->itof, i, &val) ? wpi_upb_value_getconstptr(val)
                                                : NULL;
}

const wpi_upb_FieldDef* wpi_upb_MessageDef_FindFieldByNameWithSize(
    const wpi_upb_MessageDef* m, const char* name, size_t size) {
  wpi_upb_value val;

  if (!wpi_upb_strtable_lookup2(&m->ntof, name, size, &val)) {
    return NULL;
  }

  return _wpi_upb_DefType_Unpack(val, WPI_UPB_DEFTYPE_FIELD);
}

const wpi_upb_OneofDef* wpi_upb_MessageDef_FindOneofByNameWithSize(
    const wpi_upb_MessageDef* m, const char* name, size_t size) {
  wpi_upb_value val;

  if (!wpi_upb_strtable_lookup2(&m->ntof, name, size, &val)) {
    return NULL;
  }

  return _wpi_upb_DefType_Unpack(val, WPI_UPB_DEFTYPE_ONEOF);
}

bool _wpi_upb_MessageDef_Insert(wpi_upb_MessageDef* m, const char* name, size_t len,
                            wpi_upb_value v, wpi_upb_Arena* a) {
  return wpi_upb_strtable_insert(&m->ntof, name, len, v, a);
}

bool wpi_upb_MessageDef_FindByNameWithSize(const wpi_upb_MessageDef* m,
                                       const char* name, size_t len,
                                       const wpi_upb_FieldDef** out_f,
                                       const wpi_upb_OneofDef** out_o) {
  wpi_upb_value val;

  if (!wpi_upb_strtable_lookup2(&m->ntof, name, len, &val)) {
    return false;
  }

  const wpi_upb_FieldDef* f = _wpi_upb_DefType_Unpack(val, WPI_UPB_DEFTYPE_FIELD);
  const wpi_upb_OneofDef* o = _wpi_upb_DefType_Unpack(val, WPI_UPB_DEFTYPE_ONEOF);
  if (out_f) *out_f = f;
  if (out_o) *out_o = o;
  return f || o; /* False if this was a JSON name. */
}

const wpi_upb_FieldDef* wpi_upb_MessageDef_FindByJsonNameWithSize(
    const wpi_upb_MessageDef* m, const char* name, size_t size) {
  wpi_upb_value val;

  if (wpi_upb_strtable_lookup2(&m->jtof, name, size, &val)) {
    return wpi_upb_value_getconstptr(val);
  }

  if (!wpi_upb_strtable_lookup2(&m->ntof, name, size, &val)) {
    return NULL;
  }

  return _wpi_upb_DefType_Unpack(val, WPI_UPB_DEFTYPE_FIELD);
}

int wpi_upb_MessageDef_ExtensionRangeCount(const wpi_upb_MessageDef* m) {
  return m->ext_range_count;
}

int wpi_upb_MessageDef_ReservedRangeCount(const wpi_upb_MessageDef* m) {
  return m->res_range_count;
}

int wpi_upb_MessageDef_ReservedNameCount(const wpi_upb_MessageDef* m) {
  return m->res_name_count;
}

int wpi_upb_MessageDef_FieldCount(const wpi_upb_MessageDef* m) {
  return m->field_count;
}

int wpi_upb_MessageDef_OneofCount(const wpi_upb_MessageDef* m) {
  return m->oneof_count;
}

int wpi_upb_MessageDef_RealOneofCount(const wpi_upb_MessageDef* m) {
  return m->real_oneof_count;
}

int wpi_upb_MessageDef_NestedMessageCount(const wpi_upb_MessageDef* m) {
  return m->nested_msg_count;
}

int wpi_upb_MessageDef_NestedEnumCount(const wpi_upb_MessageDef* m) {
  return m->nested_enum_count;
}

int wpi_upb_MessageDef_NestedExtensionCount(const wpi_upb_MessageDef* m) {
  return m->nested_ext_count;
}

const wpi_upb_MiniTable* wpi_upb_MessageDef_MiniTable(const wpi_upb_MessageDef* m) {
  return m->layout;
}

const wpi_upb_ExtensionRange* wpi_upb_MessageDef_ExtensionRange(const wpi_upb_MessageDef* m,
                                                        int i) {
  WPI_UPB_ASSERT(0 <= i && i < m->ext_range_count);
  return _wpi_upb_ExtensionRange_At(m->ext_ranges, i);
}

const wpi_upb_MessageReservedRange* wpi_upb_MessageDef_ReservedRange(
    const wpi_upb_MessageDef* m, int i) {
  WPI_UPB_ASSERT(0 <= i && i < m->res_range_count);
  return _wpi_upb_MessageReservedRange_At(m->res_ranges, i);
}

wpi_upb_StringView wpi_upb_MessageDef_ReservedName(const wpi_upb_MessageDef* m, int i) {
  WPI_UPB_ASSERT(0 <= i && i < m->res_name_count);
  return m->res_names[i];
}

const wpi_upb_FieldDef* wpi_upb_MessageDef_Field(const wpi_upb_MessageDef* m, int i) {
  WPI_UPB_ASSERT(0 <= i && i < m->field_count);
  return _wpi_upb_FieldDef_At(m->fields, i);
}

const wpi_upb_OneofDef* wpi_upb_MessageDef_Oneof(const wpi_upb_MessageDef* m, int i) {
  WPI_UPB_ASSERT(0 <= i && i < m->oneof_count);
  return _wpi_upb_OneofDef_At(m->oneofs, i);
}

const wpi_upb_MessageDef* wpi_upb_MessageDef_NestedMessage(const wpi_upb_MessageDef* m,
                                                   int i) {
  WPI_UPB_ASSERT(0 <= i && i < m->nested_msg_count);
  return &m->nested_msgs[i];
}

const wpi_upb_EnumDef* wpi_upb_MessageDef_NestedEnum(const wpi_upb_MessageDef* m, int i) {
  WPI_UPB_ASSERT(0 <= i && i < m->nested_enum_count);
  return _wpi_upb_EnumDef_At(m->nested_enums, i);
}

const wpi_upb_FieldDef* wpi_upb_MessageDef_NestedExtension(const wpi_upb_MessageDef* m,
                                                   int i) {
  WPI_UPB_ASSERT(0 <= i && i < m->nested_ext_count);
  return _wpi_upb_FieldDef_At(m->nested_exts, i);
}

wpi_upb_WellKnown wpi_upb_MessageDef_WellKnownType(const wpi_upb_MessageDef* m) {
  return m->well_known_type;
}

bool _wpi_upb_MessageDef_InMessageSet(const wpi_upb_MessageDef* m) {
  return m->in_message_set;
}

const wpi_upb_FieldDef* wpi_upb_MessageDef_FindFieldByName(const wpi_upb_MessageDef* m,
                                                   const char* name) {
  return wpi_upb_MessageDef_FindFieldByNameWithSize(m, name, strlen(name));
}

const wpi_upb_OneofDef* wpi_upb_MessageDef_FindOneofByName(const wpi_upb_MessageDef* m,
                                                   const char* name) {
  return wpi_upb_MessageDef_FindOneofByNameWithSize(m, name, strlen(name));
}

bool wpi_upb_MessageDef_IsMapEntry(const wpi_upb_MessageDef* m) {
  return WPI_UPB_DESC(MessageOptions_map_entry)(m->opts);
}

bool wpi_upb_MessageDef_IsMessageSet(const wpi_upb_MessageDef* m) {
  return WPI_UPB_DESC(MessageOptions_message_set_wire_format)(m->opts);
}

static wpi_upb_MiniTable* _wpi_upb_MessageDef_MakeMiniTable(wpi_upb_DefBuilder* ctx,
                                                    const wpi_upb_MessageDef* m) {
  wpi_upb_StringView desc;
  // Note: this will assign layout_index for fields, so wpi_upb_FieldDef_MiniTable()
  // is safe to call only after this call.
  bool ok = wpi_upb_MessageDef_MiniDescriptorEncode(m, ctx->tmp_arena, &desc);
  if (!ok) _wpi_upb_DefBuilder_OomErr(ctx);

  void** scratch_data = _wpi_upb_DefPool_ScratchData(ctx->symtab);
  size_t* scratch_size = _wpi_upb_DefPool_ScratchSize(ctx->symtab);
  wpi_upb_MiniTable* ret = wpi_upb_MiniTable_BuildWithBuf(
      desc.data, desc.size, ctx->platform, ctx->arena, scratch_data,
      scratch_size, ctx->status);
  if (!ret) _wpi_upb_DefBuilder_FailJmp(ctx);

  return ret;
}

void _wpi_upb_MessageDef_Resolve(wpi_upb_DefBuilder* ctx, wpi_upb_MessageDef* m) {
  for (int i = 0; i < m->field_count; i++) {
    wpi_upb_FieldDef* f = (wpi_upb_FieldDef*)wpi_upb_MessageDef_Field(m, i);
    _wpi_upb_FieldDef_Resolve(ctx, m->full_name, f);
  }

  m->in_message_set = false;
  for (int i = 0; i < wpi_upb_MessageDef_NestedExtensionCount(m); i++) {
    wpi_upb_FieldDef* ext = (wpi_upb_FieldDef*)wpi_upb_MessageDef_NestedExtension(m, i);
    _wpi_upb_FieldDef_Resolve(ctx, m->full_name, ext);
    if (wpi_upb_FieldDef_Type(ext) == kUpb_FieldType_Message &&
        wpi_upb_FieldDef_Label(ext) == kUpb_Label_Optional &&
        wpi_upb_FieldDef_MessageSubDef(ext) == m &&
        WPI_UPB_DESC(MessageOptions_message_set_wire_format)(
            wpi_upb_MessageDef_Options(wpi_upb_FieldDef_ContainingType(ext)))) {
      m->in_message_set = true;
    }
  }

  for (int i = 0; i < wpi_upb_MessageDef_NestedMessageCount(m); i++) {
    wpi_upb_MessageDef* n = (wpi_upb_MessageDef*)wpi_upb_MessageDef_NestedMessage(m, i);
    _wpi_upb_MessageDef_Resolve(ctx, n);
  }
}

void _wpi_upb_MessageDef_InsertField(wpi_upb_DefBuilder* ctx, wpi_upb_MessageDef* m,
                                 const wpi_upb_FieldDef* f) {
  const int32_t field_number = wpi_upb_FieldDef_Number(f);

  if (field_number <= 0 || field_number > kUpb_MaxFieldNumber) {
    _wpi_upb_DefBuilder_Errf(ctx, "invalid field number (%u)", field_number);
  }

  const char* json_name = wpi_upb_FieldDef_JsonName(f);
  const char* shortname = wpi_upb_FieldDef_Name(f);
  const size_t shortnamelen = strlen(shortname);

  wpi_upb_value v = wpi_upb_value_constptr(f);

  wpi_upb_value existing_v;
  if (wpi_upb_strtable_lookup(&m->ntof, shortname, &existing_v)) {
    _wpi_upb_DefBuilder_Errf(ctx, "duplicate field name (%s)", shortname);
  }

  const wpi_upb_value field_v = _wpi_upb_DefType_Pack(f, WPI_UPB_DEFTYPE_FIELD);
  bool ok =
      _wpi_upb_MessageDef_Insert(m, shortname, shortnamelen, field_v, ctx->arena);
  if (!ok) _wpi_upb_DefBuilder_OomErr(ctx);

  bool skip_json_conflicts =
      WPI_UPB_DESC(MessageOptions_deprecated_legacy_json_field_conflicts)(
          wpi_upb_MessageDef_Options(m));
  if (!skip_json_conflicts && strcmp(shortname, json_name) != 0 &&
      WPI_UPB_DESC(FeatureSet_json_format)(m->resolved_features) ==
          WPI_UPB_DESC(FeatureSet_ALLOW) &&
      wpi_upb_strtable_lookup(&m->ntof, json_name, &v)) {
    _wpi_upb_DefBuilder_Errf(
        ctx, "duplicate json_name for (%s) with original field name (%s)",
        shortname, json_name);
  }

  if (wpi_upb_strtable_lookup(&m->jtof, json_name, &v)) {
    if (!skip_json_conflicts) {
      _wpi_upb_DefBuilder_Errf(ctx, "duplicate json_name (%s)", json_name);
    }
  } else {
    const size_t json_size = strlen(json_name);
    ok = wpi_upb_strtable_insert(&m->jtof, json_name, json_size,
                             wpi_upb_value_constptr(f), ctx->arena);
    if (!ok) _wpi_upb_DefBuilder_OomErr(ctx);
  }

  if (wpi_upb_inttable_lookup(&m->itof, field_number, NULL)) {
    _wpi_upb_DefBuilder_Errf(ctx, "duplicate field number (%u)", field_number);
  }

  ok = wpi_upb_inttable_insert(&m->itof, field_number, v, ctx->arena);
  if (!ok) _wpi_upb_DefBuilder_OomErr(ctx);
}

void _wpi_upb_MessageDef_CreateMiniTable(wpi_upb_DefBuilder* ctx, wpi_upb_MessageDef* m) {
  if (ctx->layout == NULL) {
    m->layout = _wpi_upb_MessageDef_MakeMiniTable(ctx, m);
  } else {
    m->layout = wpi_upb_MiniTableFile_Message(ctx->layout, ctx->msg_count++);
    WPI_UPB_ASSERT(m->field_count == wpi_upb_MiniTable_FieldCount(m->layout));

    // We don't need the result of this call, but it will assign layout_index
    // for all the fields in O(n lg n) time.
    _wpi_upb_FieldDefs_Sorted(m->fields, m->field_count, ctx->tmp_arena);
  }

  for (int i = 0; i < m->nested_msg_count; i++) {
    wpi_upb_MessageDef* nested =
        (wpi_upb_MessageDef*)wpi_upb_MessageDef_NestedMessage(m, i);
    _wpi_upb_MessageDef_CreateMiniTable(ctx, nested);
  }
}

void _wpi_upb_MessageDef_LinkMiniTable(wpi_upb_DefBuilder* ctx,
                                   const wpi_upb_MessageDef* m) {
  for (int i = 0; i < wpi_upb_MessageDef_NestedExtensionCount(m); i++) {
    const wpi_upb_FieldDef* ext = wpi_upb_MessageDef_NestedExtension(m, i);
    _wpi_upb_FieldDef_BuildMiniTableExtension(ctx, ext);
  }

  for (int i = 0; i < m->nested_msg_count; i++) {
    _wpi_upb_MessageDef_LinkMiniTable(ctx, wpi_upb_MessageDef_NestedMessage(m, i));
  }

  if (ctx->layout) return;

  for (int i = 0; i < m->field_count; i++) {
    const wpi_upb_FieldDef* f = wpi_upb_MessageDef_Field(m, i);
    const wpi_upb_MessageDef* sub_m = wpi_upb_FieldDef_MessageSubDef(f);
    const wpi_upb_EnumDef* sub_e = wpi_upb_FieldDef_EnumSubDef(f);
    const int layout_index = _wpi_upb_FieldDef_LayoutIndex(f);
    wpi_upb_MiniTable* mt = (wpi_upb_MiniTable*)wpi_upb_MessageDef_MiniTable(m);

    WPI_UPB_ASSERT(layout_index < m->field_count);
    wpi_upb_MiniTableField* mt_f =
        (wpi_upb_MiniTableField*)&m->layout->WPI_UPB_PRIVATE(fields)[layout_index];
    if (sub_m) {
      if (!mt->WPI_UPB_PRIVATE(subs)) {
        _wpi_upb_DefBuilder_Errf(ctx, "unexpected submsg for (%s)", m->full_name);
      }
      WPI_UPB_ASSERT(mt_f);
      WPI_UPB_ASSERT(sub_m->layout);
      if (WPI_UPB_UNLIKELY(!wpi_upb_MiniTable_SetSubMessage(mt, mt_f, sub_m->layout))) {
        _wpi_upb_DefBuilder_Errf(ctx, "invalid submsg for (%s)", m->full_name);
      }
    } else if (_wpi_upb_FieldDef_IsClosedEnum(f)) {
      const wpi_upb_MiniTableEnum* mt_e = _wpi_upb_EnumDef_MiniTable(sub_e);
      if (WPI_UPB_UNLIKELY(!wpi_upb_MiniTable_SetSubEnum(mt, mt_f, mt_e))) {
        _wpi_upb_DefBuilder_Errf(ctx, "invalid subenum for (%s)", m->full_name);
      }
    }
  }

#ifndef NDEBUG
  for (int i = 0; i < m->field_count; i++) {
    const wpi_upb_FieldDef* f = wpi_upb_MessageDef_Field(m, i);
    const int layout_index = _wpi_upb_FieldDef_LayoutIndex(f);
    WPI_UPB_ASSERT(layout_index < wpi_upb_MiniTable_FieldCount(m->layout));
    const wpi_upb_MiniTableField* mt_f =
        &m->layout->WPI_UPB_PRIVATE(fields)[layout_index];
    WPI_UPB_ASSERT(wpi_upb_FieldDef_Type(f) == wpi_upb_MiniTableField_Type(mt_f));
    WPI_UPB_ASSERT(wpi_upb_FieldDef_CType(f) == wpi_upb_MiniTableField_CType(mt_f));
    WPI_UPB_ASSERT(wpi_upb_FieldDef_HasPresence(f) ==
               wpi_upb_MiniTableField_HasPresence(mt_f));
  }
#endif
}

static bool _wpi_upb_MessageDef_ValidateUtf8(const wpi_upb_MessageDef* m) {
  bool has_string = false;
  for (int i = 0; i < m->field_count; i++) {
    const wpi_upb_FieldDef* f = wpi_upb_MessageDef_Field(m, i);
    // Old binaries do not recognize the field-level "FlipValidateUtf8" wire
    // modifier, so we do not actually have field-level control for old
    // binaries.  Given this, we judge that the better failure mode is to be
    // more lax than intended, rather than more strict.  To achieve this, we
    // only mark the message with the ValidateUtf8 modifier if *all* fields
    // validate UTF-8.
    if (!_wpi_upb_FieldDef_ValidateUtf8(f)) return false;
    if (wpi_upb_FieldDef_Type(f) == kUpb_FieldType_String) has_string = true;
  }
  return has_string;
}

static uint64_t _wpi_upb_MessageDef_Modifiers(const wpi_upb_MessageDef* m) {
  uint64_t out = 0;

  if (WPI_UPB_DESC(FeatureSet_repeated_field_encoding(m->resolved_features)) ==
      WPI_UPB_DESC(FeatureSet_PACKED)) {
    out |= kUpb_MessageModifier_DefaultIsPacked;
  }

  if (_wpi_upb_MessageDef_ValidateUtf8(m)) {
    out |= kUpb_MessageModifier_ValidateUtf8;
  }

  if (m->ext_range_count) {
    out |= kUpb_MessageModifier_IsExtendable;
  }

  return out;
}

static bool _wpi_upb_MessageDef_EncodeMap(wpi_upb_DescState* s, const wpi_upb_MessageDef* m,
                                      wpi_upb_Arena* a) {
  if (m->field_count != 2) return false;

  const wpi_upb_FieldDef* key_field = wpi_upb_MessageDef_Field(m, 0);
  const wpi_upb_FieldDef* val_field = wpi_upb_MessageDef_Field(m, 1);
  if (key_field == NULL || val_field == NULL) return false;

  WPI_UPB_ASSERT(_wpi_upb_FieldDef_LayoutIndex(key_field) == 0);
  WPI_UPB_ASSERT(_wpi_upb_FieldDef_LayoutIndex(val_field) == 1);

  s->ptr = wpi_upb_MtDataEncoder_EncodeMap(
      &s->e, s->ptr, wpi_upb_FieldDef_Type(key_field), wpi_upb_FieldDef_Type(val_field),
      _wpi_upb_FieldDef_Modifiers(key_field), _wpi_upb_FieldDef_Modifiers(val_field));
  return true;
}

static bool _wpi_upb_MessageDef_EncodeMessage(wpi_upb_DescState* s,
                                          const wpi_upb_MessageDef* m,
                                          wpi_upb_Arena* a) {
  const wpi_upb_FieldDef** sorted = NULL;
  if (!m->is_sorted) {
    sorted = _wpi_upb_FieldDefs_Sorted(m->fields, m->field_count, a);
    if (!sorted) return false;
  }

  s->ptr = wpi_upb_MtDataEncoder_StartMessage(&s->e, s->ptr,
                                          _wpi_upb_MessageDef_Modifiers(m));

  for (int i = 0; i < m->field_count; i++) {
    const wpi_upb_FieldDef* f = sorted ? sorted[i] : wpi_upb_MessageDef_Field(m, i);
    const wpi_upb_FieldType type = wpi_upb_FieldDef_Type(f);
    const int number = wpi_upb_FieldDef_Number(f);
    const uint64_t modifiers = _wpi_upb_FieldDef_Modifiers(f);

    if (!_wpi_upb_DescState_Grow(s, a)) return false;
    s->ptr = wpi_upb_MtDataEncoder_PutField(&s->e, s->ptr, type, number, modifiers);
  }

  for (int i = 0; i < m->real_oneof_count; i++) {
    if (!_wpi_upb_DescState_Grow(s, a)) return false;
    s->ptr = wpi_upb_MtDataEncoder_StartOneof(&s->e, s->ptr);

    const wpi_upb_OneofDef* o = wpi_upb_MessageDef_Oneof(m, i);
    const int field_count = wpi_upb_OneofDef_FieldCount(o);
    for (int j = 0; j < field_count; j++) {
      const int number = wpi_upb_FieldDef_Number(wpi_upb_OneofDef_Field(o, j));

      if (!_wpi_upb_DescState_Grow(s, a)) return false;
      s->ptr = wpi_upb_MtDataEncoder_PutOneofField(&s->e, s->ptr, number);
    }
  }

  return true;
}

static bool _wpi_upb_MessageDef_EncodeMessageSet(wpi_upb_DescState* s,
                                             const wpi_upb_MessageDef* m,
                                             wpi_upb_Arena* a) {
  s->ptr = wpi_upb_MtDataEncoder_EncodeMessageSet(&s->e, s->ptr);

  return true;
}

bool wpi_upb_MessageDef_MiniDescriptorEncode(const wpi_upb_MessageDef* m, wpi_upb_Arena* a,
                                         wpi_upb_StringView* out) {
  wpi_upb_DescState s;
  _wpi_upb_DescState_Init(&s);

  if (!_wpi_upb_DescState_Grow(&s, a)) return false;

  if (wpi_upb_MessageDef_IsMapEntry(m)) {
    if (!_wpi_upb_MessageDef_EncodeMap(&s, m, a)) return false;
  } else if (WPI_UPB_DESC(MessageOptions_message_set_wire_format)(m->opts)) {
    if (!_wpi_upb_MessageDef_EncodeMessageSet(&s, m, a)) return false;
  } else {
    if (!_wpi_upb_MessageDef_EncodeMessage(&s, m, a)) return false;
  }

  if (!_wpi_upb_DescState_Grow(&s, a)) return false;
  *s.ptr = '\0';

  out->data = s.buf;
  out->size = s.ptr - s.buf;
  return true;
}

static wpi_upb_StringView* _wpi_upb_ReservedNames_New(wpi_upb_DefBuilder* ctx, int n,
                                              const wpi_upb_StringView* protos) {
  wpi_upb_StringView* sv = _wpi_upb_DefBuilder_Alloc(ctx, sizeof(wpi_upb_StringView) * n);
  for (int i = 0; i < n; i++) {
    sv[i].data =
        wpi_upb_strdup2(protos[i].data, protos[i].size, _wpi_upb_DefBuilder_Arena(ctx));
    sv[i].size = protos[i].size;
  }
  return sv;
}

static void create_msgdef(wpi_upb_DefBuilder* ctx, const char* prefix,
                          const WPI_UPB_DESC(DescriptorProto*) msg_proto,
                          const WPI_UPB_DESC(FeatureSet*) parent_features,
                          const wpi_upb_MessageDef* containing_type,
                          wpi_upb_MessageDef* m) {
  const WPI_UPB_DESC(OneofDescriptorProto)* const* oneofs;
  const WPI_UPB_DESC(FieldDescriptorProto)* const* fields;
  const WPI_UPB_DESC(DescriptorProto_ExtensionRange)* const* ext_ranges;
  const WPI_UPB_DESC(DescriptorProto_ReservedRange)* const* res_ranges;
  const wpi_upb_StringView* res_names;
  size_t n_oneof, n_field, n_enum, n_ext, n_msg;
  size_t n_ext_range, n_res_range, n_res_name;
  wpi_upb_StringView name;

  WPI_UPB_DEF_SET_OPTIONS(m->opts, DescriptorProto, MessageOptions, msg_proto);
  m->resolved_features = _wpi_upb_DefBuilder_ResolveFeatures(
      ctx, parent_features, WPI_UPB_DESC(MessageOptions_features)(m->opts));

  // Must happen before _wpi_upb_DefBuilder_Add()
  m->file = _wpi_upb_DefBuilder_File(ctx);

  m->containing_type = containing_type;
  m->is_sorted = true;

  name = WPI_UPB_DESC(DescriptorProto_name)(msg_proto);

  m->full_name = _wpi_upb_DefBuilder_MakeFullName(ctx, prefix, name);
  _wpi_upb_DefBuilder_Add(ctx, m->full_name, _wpi_upb_DefType_Pack(m, WPI_UPB_DEFTYPE_MSG));

  oneofs = WPI_UPB_DESC(DescriptorProto_oneof_decl)(msg_proto, &n_oneof);
  fields = WPI_UPB_DESC(DescriptorProto_field)(msg_proto, &n_field);
  ext_ranges =
      WPI_UPB_DESC(DescriptorProto_extension_range)(msg_proto, &n_ext_range);
  res_ranges =
      WPI_UPB_DESC(DescriptorProto_reserved_range)(msg_proto, &n_res_range);
  res_names = WPI_UPB_DESC(DescriptorProto_reserved_name)(msg_proto, &n_res_name);

  bool ok = wpi_upb_inttable_init(&m->itof, ctx->arena);
  if (!ok) _wpi_upb_DefBuilder_OomErr(ctx);

  ok = wpi_upb_strtable_init(&m->ntof, n_oneof + n_field, ctx->arena);
  if (!ok) _wpi_upb_DefBuilder_OomErr(ctx);

  ok = wpi_upb_strtable_init(&m->jtof, n_field, ctx->arena);
  if (!ok) _wpi_upb_DefBuilder_OomErr(ctx);

  m->oneof_count = n_oneof;
  m->oneofs = _wpi_upb_OneofDefs_New(ctx, n_oneof, oneofs, m->resolved_features, m);

  m->field_count = n_field;
  m->fields = _wpi_upb_FieldDefs_New(ctx, n_field, fields, m->resolved_features,
                                 m->full_name, m, &m->is_sorted);

  // Message Sets may not contain fields.
  if (WPI_UPB_UNLIKELY(WPI_UPB_DESC(MessageOptions_message_set_wire_format)(m->opts))) {
    if (WPI_UPB_UNLIKELY(n_field > 0)) {
      _wpi_upb_DefBuilder_Errf(ctx, "invalid message set (%s)", m->full_name);
    }
  }

  m->ext_range_count = n_ext_range;
  m->ext_ranges = _wpi_upb_ExtensionRanges_New(ctx, n_ext_range, ext_ranges,
                                           m->resolved_features, m);

  m->res_range_count = n_res_range;
  m->res_ranges =
      _wpi_upb_MessageReservedRanges_New(ctx, n_res_range, res_ranges, m);

  m->res_name_count = n_res_name;
  m->res_names = _wpi_upb_ReservedNames_New(ctx, n_res_name, res_names);

  const size_t synthetic_count = _wpi_upb_OneofDefs_Finalize(ctx, m);
  m->real_oneof_count = m->oneof_count - synthetic_count;

  assign_msg_wellknowntype(m);
  wpi_upb_inttable_compact(&m->itof, ctx->arena);

  const WPI_UPB_DESC(EnumDescriptorProto)* const* enums =
      WPI_UPB_DESC(DescriptorProto_enum_type)(msg_proto, &n_enum);
  m->nested_enum_count = n_enum;
  m->nested_enums =
      _wpi_upb_EnumDefs_New(ctx, n_enum, enums, m->resolved_features, m);

  const WPI_UPB_DESC(FieldDescriptorProto)* const* exts =
      WPI_UPB_DESC(DescriptorProto_extension)(msg_proto, &n_ext);
  m->nested_ext_count = n_ext;
  m->nested_exts = _wpi_upb_Extensions_New(ctx, n_ext, exts, m->resolved_features,
                                       m->full_name, m);

  const WPI_UPB_DESC(DescriptorProto)* const* msgs =
      WPI_UPB_DESC(DescriptorProto_nested_type)(msg_proto, &n_msg);
  m->nested_msg_count = n_msg;
  m->nested_msgs =
      _wpi_upb_MessageDefs_New(ctx, n_msg, msgs, m->resolved_features, m);
}

// Allocate and initialize an array of |n| message defs.
wpi_upb_MessageDef* _wpi_upb_MessageDefs_New(wpi_upb_DefBuilder* ctx, int n,
                                     const WPI_UPB_DESC(DescriptorProto*)
                                         const* protos,
                                     const WPI_UPB_DESC(FeatureSet*)
                                         parent_features,
                                     const wpi_upb_MessageDef* containing_type) {
  _wpi_upb_DefType_CheckPadding(sizeof(wpi_upb_MessageDef));

  const char* name = containing_type ? containing_type->full_name
                                     : _wpi_upb_FileDef_RawPackage(ctx->file);

  wpi_upb_MessageDef* m = _wpi_upb_DefBuilder_Alloc(ctx, sizeof(wpi_upb_MessageDef) * n);
  for (int i = 0; i < n; i++) {
    create_msgdef(ctx, name, protos[i], parent_features, containing_type,
                  &m[i]);
  }
  return m;
}


// Must be last.

struct wpi_upb_MessageReservedRange {
  int32_t start;
  int32_t end;
};

wpi_upb_MessageReservedRange* _wpi_upb_MessageReservedRange_At(
    const wpi_upb_MessageReservedRange* r, int i) {
  return (wpi_upb_MessageReservedRange*)&r[i];
}

int32_t wpi_upb_MessageReservedRange_Start(const wpi_upb_MessageReservedRange* r) {
  return r->start;
}
int32_t wpi_upb_MessageReservedRange_End(const wpi_upb_MessageReservedRange* r) {
  return r->end;
}

wpi_upb_MessageReservedRange* _wpi_upb_MessageReservedRanges_New(
    wpi_upb_DefBuilder* ctx, int n,
    const WPI_UPB_DESC(DescriptorProto_ReservedRange) * const* protos,
    const wpi_upb_MessageDef* m) {
  wpi_upb_MessageReservedRange* r =
      _wpi_upb_DefBuilder_Alloc(ctx, sizeof(wpi_upb_MessageReservedRange) * n);

  for (int i = 0; i < n; i++) {
    const int32_t start =
        WPI_UPB_DESC(DescriptorProto_ReservedRange_start)(protos[i]);
    const int32_t end = WPI_UPB_DESC(DescriptorProto_ReservedRange_end)(protos[i]);
    const int32_t max = kUpb_MaxFieldNumber + 1;

    // A full validation would also check that each range is disjoint, and that
    // none of the fields overlap with the extension ranges, but we are just
    // sanity checking here.
    if (start < 1 || end <= start || end > max) {
      _wpi_upb_DefBuilder_Errf(ctx,
                           "Reserved range (%d, %d) is invalid, message=%s\n",
                           (int)start, (int)end, wpi_upb_MessageDef_FullName(m));
    }

    r[i].start = start;
    r[i].end = end;
  }

  return r;
}



// Must be last.

struct wpi_upb_MethodDef {
  const WPI_UPB_DESC(MethodOptions*) opts;
  const WPI_UPB_DESC(FeatureSet*) resolved_features;
  wpi_upb_ServiceDef* service;
  const char* full_name;
  const wpi_upb_MessageDef* input_type;
  const wpi_upb_MessageDef* output_type;
  int index;
  bool client_streaming;
  bool server_streaming;
};

wpi_upb_MethodDef* _wpi_upb_MethodDef_At(const wpi_upb_MethodDef* m, int i) {
  return (wpi_upb_MethodDef*)&m[i];
}

const wpi_upb_ServiceDef* wpi_upb_MethodDef_Service(const wpi_upb_MethodDef* m) {
  return m->service;
}

const WPI_UPB_DESC(MethodOptions) * wpi_upb_MethodDef_Options(const wpi_upb_MethodDef* m) {
  return m->opts;
}

bool wpi_upb_MethodDef_HasOptions(const wpi_upb_MethodDef* m) {
  return m->opts != (void*)kUpbDefOptDefault;
}

const WPI_UPB_DESC(FeatureSet) *
    wpi_upb_MethodDef_ResolvedFeatures(const wpi_upb_MethodDef* m) {
  return m->resolved_features;
}

const char* wpi_upb_MethodDef_FullName(const wpi_upb_MethodDef* m) {
  return m->full_name;
}

const char* wpi_upb_MethodDef_Name(const wpi_upb_MethodDef* m) {
  return _wpi_upb_DefBuilder_FullToShort(m->full_name);
}

int wpi_upb_MethodDef_Index(const wpi_upb_MethodDef* m) { return m->index; }

const wpi_upb_MessageDef* wpi_upb_MethodDef_InputType(const wpi_upb_MethodDef* m) {
  return m->input_type;
}

const wpi_upb_MessageDef* wpi_upb_MethodDef_OutputType(const wpi_upb_MethodDef* m) {
  return m->output_type;
}

bool wpi_upb_MethodDef_ClientStreaming(const wpi_upb_MethodDef* m) {
  return m->client_streaming;
}

bool wpi_upb_MethodDef_ServerStreaming(const wpi_upb_MethodDef* m) {
  return m->server_streaming;
}

static void create_method(wpi_upb_DefBuilder* ctx,
                          const WPI_UPB_DESC(MethodDescriptorProto*) method_proto,
                          const WPI_UPB_DESC(FeatureSet*) parent_features,
                          wpi_upb_ServiceDef* s, wpi_upb_MethodDef* m) {
  WPI_UPB_DEF_SET_OPTIONS(m->opts, MethodDescriptorProto, MethodOptions,
                      method_proto);
  m->resolved_features = _wpi_upb_DefBuilder_ResolveFeatures(
      ctx, parent_features, WPI_UPB_DESC(MethodOptions_features)(m->opts));

  wpi_upb_StringView name = WPI_UPB_DESC(MethodDescriptorProto_name)(method_proto);

  m->service = s;
  m->full_name =
      _wpi_upb_DefBuilder_MakeFullName(ctx, wpi_upb_ServiceDef_FullName(s), name);
  m->client_streaming =
      WPI_UPB_DESC(MethodDescriptorProto_client_streaming)(method_proto);
  m->server_streaming =
      WPI_UPB_DESC(MethodDescriptorProto_server_streaming)(method_proto);
  m->input_type = _wpi_upb_DefBuilder_Resolve(
      ctx, m->full_name, m->full_name,
      WPI_UPB_DESC(MethodDescriptorProto_input_type)(method_proto),
      WPI_UPB_DEFTYPE_MSG);
  m->output_type = _wpi_upb_DefBuilder_Resolve(
      ctx, m->full_name, m->full_name,
      WPI_UPB_DESC(MethodDescriptorProto_output_type)(method_proto),
      WPI_UPB_DEFTYPE_MSG);
}

// Allocate and initialize an array of |n| method defs belonging to |s|.
wpi_upb_MethodDef* _wpi_upb_MethodDefs_New(wpi_upb_DefBuilder* ctx, int n,
                                   const WPI_UPB_DESC(MethodDescriptorProto*)
                                       const* protos,
                                   const WPI_UPB_DESC(FeatureSet*) parent_features,
                                   wpi_upb_ServiceDef* s) {
  wpi_upb_MethodDef* m = _wpi_upb_DefBuilder_Alloc(ctx, sizeof(wpi_upb_MethodDef) * n);
  for (int i = 0; i < n; i++) {
    create_method(ctx, protos[i], parent_features, s, &m[i]);
    m[i].index = i;
  }
  return m;
}


#include <ctype.h>
#include <stdlib.h>
#include <string.h>


// Must be last.

struct wpi_upb_OneofDef {
  const WPI_UPB_DESC(OneofOptions*) opts;
  const WPI_UPB_DESC(FeatureSet*) resolved_features;
  const wpi_upb_MessageDef* parent;
  const char* full_name;
  int field_count;
  bool synthetic;
  const wpi_upb_FieldDef** fields;
  wpi_upb_strtable ntof;  // lookup a field by name
  wpi_upb_inttable itof;  // lookup a field by number (index)
};

wpi_upb_OneofDef* _wpi_upb_OneofDef_At(const wpi_upb_OneofDef* o, int i) {
  return (wpi_upb_OneofDef*)&o[i];
}

const WPI_UPB_DESC(OneofOptions) * wpi_upb_OneofDef_Options(const wpi_upb_OneofDef* o) {
  return o->opts;
}

bool wpi_upb_OneofDef_HasOptions(const wpi_upb_OneofDef* o) {
  return o->opts != (void*)kUpbDefOptDefault;
}

const WPI_UPB_DESC(FeatureSet) *
    wpi_upb_OneofDef_ResolvedFeatures(const wpi_upb_OneofDef* o) {
  return o->resolved_features;
}

const char* wpi_upb_OneofDef_FullName(const wpi_upb_OneofDef* o) {
  return o->full_name;
}

const char* wpi_upb_OneofDef_Name(const wpi_upb_OneofDef* o) {
  return _wpi_upb_DefBuilder_FullToShort(o->full_name);
}

const wpi_upb_MessageDef* wpi_upb_OneofDef_ContainingType(const wpi_upb_OneofDef* o) {
  return o->parent;
}

int wpi_upb_OneofDef_FieldCount(const wpi_upb_OneofDef* o) { return o->field_count; }

const wpi_upb_FieldDef* wpi_upb_OneofDef_Field(const wpi_upb_OneofDef* o, int i) {
  WPI_UPB_ASSERT(i < o->field_count);
  return o->fields[i];
}

int wpi_upb_OneofDef_numfields(const wpi_upb_OneofDef* o) { return o->field_count; }

uint32_t wpi_upb_OneofDef_Index(const wpi_upb_OneofDef* o) {
  // Compute index in our parent's array.
  return o - wpi_upb_MessageDef_Oneof(o->parent, 0);
}

bool wpi_upb_OneofDef_IsSynthetic(const wpi_upb_OneofDef* o) { return o->synthetic; }

const wpi_upb_FieldDef* wpi_upb_OneofDef_LookupNameWithSize(const wpi_upb_OneofDef* o,
                                                    const char* name,
                                                    size_t size) {
  wpi_upb_value val;
  return wpi_upb_strtable_lookup2(&o->ntof, name, size, &val)
             ? wpi_upb_value_getptr(val)
             : NULL;
}

const wpi_upb_FieldDef* wpi_upb_OneofDef_LookupName(const wpi_upb_OneofDef* o,
                                            const char* name) {
  return wpi_upb_OneofDef_LookupNameWithSize(o, name, strlen(name));
}

const wpi_upb_FieldDef* wpi_upb_OneofDef_LookupNumber(const wpi_upb_OneofDef* o,
                                              uint32_t num) {
  wpi_upb_value val;
  return wpi_upb_inttable_lookup(&o->itof, num, &val) ? wpi_upb_value_getptr(val)
                                                  : NULL;
}

void _wpi_upb_OneofDef_Insert(wpi_upb_DefBuilder* ctx, wpi_upb_OneofDef* o,
                          const wpi_upb_FieldDef* f, const char* name,
                          size_t size) {
  o->field_count++;
  if (_wpi_upb_FieldDef_IsProto3Optional(f)) o->synthetic = true;

  const int number = wpi_upb_FieldDef_Number(f);
  const wpi_upb_value v = wpi_upb_value_constptr(f);

  // TODO: This lookup is unfortunate because we also perform it when
  // inserting into the message's table. Unfortunately that step occurs after
  // this one and moving things around could be tricky so let's leave it for
  // a future refactoring.
  const bool number_exists = wpi_upb_inttable_lookup(&o->itof, number, NULL);
  if (WPI_UPB_UNLIKELY(number_exists)) {
    _wpi_upb_DefBuilder_Errf(ctx, "oneof fields have the same number (%d)", number);
  }

  // TODO: More redundant work happening here.
  const bool name_exists = wpi_upb_strtable_lookup2(&o->ntof, name, size, NULL);
  if (WPI_UPB_UNLIKELY(name_exists)) {
    _wpi_upb_DefBuilder_Errf(ctx, "oneof fields have the same name (%.*s)",
                         (int)size, name);
  }

  const bool ok = wpi_upb_inttable_insert(&o->itof, number, v, ctx->arena) &&
                  wpi_upb_strtable_insert(&o->ntof, name, size, v, ctx->arena);
  if (WPI_UPB_UNLIKELY(!ok)) {
    _wpi_upb_DefBuilder_OomErr(ctx);
  }
}

// Returns the synthetic count.
size_t _wpi_upb_OneofDefs_Finalize(wpi_upb_DefBuilder* ctx, wpi_upb_MessageDef* m) {
  int synthetic_count = 0;

  for (int i = 0; i < wpi_upb_MessageDef_OneofCount(m); i++) {
    wpi_upb_OneofDef* o = (wpi_upb_OneofDef*)wpi_upb_MessageDef_Oneof(m, i);

    if (o->synthetic && o->field_count != 1) {
      _wpi_upb_DefBuilder_Errf(ctx,
                           "Synthetic oneofs must have one field, not %d: %s",
                           o->field_count, wpi_upb_OneofDef_Name(o));
    }

    if (o->synthetic) {
      synthetic_count++;
    } else if (synthetic_count != 0) {
      _wpi_upb_DefBuilder_Errf(
          ctx, "Synthetic oneofs must be after all other oneofs: %s",
          wpi_upb_OneofDef_Name(o));
    }

    o->fields =
        _wpi_upb_DefBuilder_Alloc(ctx, sizeof(wpi_upb_FieldDef*) * o->field_count);
    o->field_count = 0;
  }

  for (int i = 0; i < wpi_upb_MessageDef_FieldCount(m); i++) {
    const wpi_upb_FieldDef* f = wpi_upb_MessageDef_Field(m, i);
    wpi_upb_OneofDef* o = (wpi_upb_OneofDef*)wpi_upb_FieldDef_ContainingOneof(f);
    if (o) {
      o->fields[o->field_count++] = f;
    }
  }

  return synthetic_count;
}

static void create_oneofdef(wpi_upb_DefBuilder* ctx, wpi_upb_MessageDef* m,
                            const WPI_UPB_DESC(OneofDescriptorProto*) oneof_proto,
                            const WPI_UPB_DESC(FeatureSet*) parent_features,
                            const wpi_upb_OneofDef* _o) {
  wpi_upb_OneofDef* o = (wpi_upb_OneofDef*)_o;

  WPI_UPB_DEF_SET_OPTIONS(o->opts, OneofDescriptorProto, OneofOptions, oneof_proto);
  o->resolved_features = _wpi_upb_DefBuilder_ResolveFeatures(
      ctx, parent_features, WPI_UPB_DESC(OneofOptions_features)(o->opts));

  wpi_upb_StringView name = WPI_UPB_DESC(OneofDescriptorProto_name)(oneof_proto);

  o->parent = m;
  o->full_name =
      _wpi_upb_DefBuilder_MakeFullName(ctx, wpi_upb_MessageDef_FullName(m), name);
  o->field_count = 0;
  o->synthetic = false;

  if (wpi_upb_MessageDef_FindByNameWithSize(m, name.data, name.size, NULL, NULL)) {
    _wpi_upb_DefBuilder_Errf(ctx, "duplicate oneof name (%s)", o->full_name);
  }

  wpi_upb_value v = _wpi_upb_DefType_Pack(o, WPI_UPB_DEFTYPE_ONEOF);
  bool ok = _wpi_upb_MessageDef_Insert(m, name.data, name.size, v, ctx->arena);
  if (!ok) _wpi_upb_DefBuilder_OomErr(ctx);

  ok = wpi_upb_inttable_init(&o->itof, ctx->arena);
  if (!ok) _wpi_upb_DefBuilder_OomErr(ctx);

  ok = wpi_upb_strtable_init(&o->ntof, 4, ctx->arena);
  if (!ok) _wpi_upb_DefBuilder_OomErr(ctx);
}

// Allocate and initialize an array of |n| oneof defs.
wpi_upb_OneofDef* _wpi_upb_OneofDefs_New(wpi_upb_DefBuilder* ctx, int n,
                                 const WPI_UPB_DESC(OneofDescriptorProto*)
                                     const* protos,
                                 const WPI_UPB_DESC(FeatureSet*) parent_features,
                                 wpi_upb_MessageDef* m) {
  _wpi_upb_DefType_CheckPadding(sizeof(wpi_upb_OneofDef));

  wpi_upb_OneofDef* o = _wpi_upb_DefBuilder_Alloc(ctx, sizeof(wpi_upb_OneofDef) * n);
  for (int i = 0; i < n; i++) {
    create_oneofdef(ctx, m, protos[i], parent_features, &o[i]);
  }
  return o;
}



// Must be last.

struct wpi_upb_ServiceDef {
  const WPI_UPB_DESC(ServiceOptions*) opts;
  const WPI_UPB_DESC(FeatureSet*) resolved_features;
  const wpi_upb_FileDef* file;
  const char* full_name;
  wpi_upb_MethodDef* methods;
  int method_count;
  int index;
#if UINTPTR_MAX == 0xffffffff
  uint32_t padding;  // Increase size to a multiple of 8.
#endif
};

wpi_upb_ServiceDef* _wpi_upb_ServiceDef_At(const wpi_upb_ServiceDef* s, int index) {
  return (wpi_upb_ServiceDef*)&s[index];
}

const WPI_UPB_DESC(ServiceOptions) *
    wpi_upb_ServiceDef_Options(const wpi_upb_ServiceDef* s) {
  return s->opts;
}

bool wpi_upb_ServiceDef_HasOptions(const wpi_upb_ServiceDef* s) {
  return s->opts != (void*)kUpbDefOptDefault;
}

const WPI_UPB_DESC(FeatureSet) *
    wpi_upb_ServiceDef_ResolvedFeatures(const wpi_upb_ServiceDef* s) {
  return s->resolved_features;
}

const char* wpi_upb_ServiceDef_FullName(const wpi_upb_ServiceDef* s) {
  return s->full_name;
}

const char* wpi_upb_ServiceDef_Name(const wpi_upb_ServiceDef* s) {
  return _wpi_upb_DefBuilder_FullToShort(s->full_name);
}

int wpi_upb_ServiceDef_Index(const wpi_upb_ServiceDef* s) { return s->index; }

const wpi_upb_FileDef* wpi_upb_ServiceDef_File(const wpi_upb_ServiceDef* s) {
  return s->file;
}

int wpi_upb_ServiceDef_MethodCount(const wpi_upb_ServiceDef* s) {
  return s->method_count;
}

const wpi_upb_MethodDef* wpi_upb_ServiceDef_Method(const wpi_upb_ServiceDef* s, int i) {
  return (i < 0 || i >= s->method_count) ? NULL
                                         : _wpi_upb_MethodDef_At(s->methods, i);
}

const wpi_upb_MethodDef* wpi_upb_ServiceDef_FindMethodByName(const wpi_upb_ServiceDef* s,
                                                     const char* name) {
  for (int i = 0; i < s->method_count; i++) {
    const wpi_upb_MethodDef* m = _wpi_upb_MethodDef_At(s->methods, i);
    if (strcmp(name, wpi_upb_MethodDef_Name(m)) == 0) {
      return m;
    }
  }
  return NULL;
}

static void create_service(wpi_upb_DefBuilder* ctx,
                           const WPI_UPB_DESC(ServiceDescriptorProto*) svc_proto,
                           const WPI_UPB_DESC(FeatureSet*) parent_features,
                           wpi_upb_ServiceDef* s) {
  WPI_UPB_DEF_SET_OPTIONS(s->opts, ServiceDescriptorProto, ServiceOptions,
                      svc_proto);
  s->resolved_features = _wpi_upb_DefBuilder_ResolveFeatures(
      ctx, parent_features, WPI_UPB_DESC(ServiceOptions_features)(s->opts));

  // Must happen before _wpi_upb_DefBuilder_Add()
  s->file = _wpi_upb_DefBuilder_File(ctx);

  wpi_upb_StringView name = WPI_UPB_DESC(ServiceDescriptorProto_name)(svc_proto);
  const char* package = _wpi_upb_FileDef_RawPackage(s->file);
  s->full_name = _wpi_upb_DefBuilder_MakeFullName(ctx, package, name);
  _wpi_upb_DefBuilder_Add(ctx, s->full_name,
                      _wpi_upb_DefType_Pack(s, WPI_UPB_DEFTYPE_SERVICE));

  size_t n;
  const WPI_UPB_DESC(MethodDescriptorProto)* const* methods =
      WPI_UPB_DESC(ServiceDescriptorProto_method)(svc_proto, &n);
  s->method_count = n;
  s->methods = _wpi_upb_MethodDefs_New(ctx, n, methods, s->resolved_features, s);
}

wpi_upb_ServiceDef* _wpi_upb_ServiceDefs_New(wpi_upb_DefBuilder* ctx, int n,
                                     const WPI_UPB_DESC(ServiceDescriptorProto*)
                                         const* protos,
                                     const WPI_UPB_DESC(FeatureSet*)
                                         parent_features) {
  _wpi_upb_DefType_CheckPadding(sizeof(wpi_upb_ServiceDef));

  wpi_upb_ServiceDef* s = _wpi_upb_DefBuilder_Alloc(ctx, sizeof(wpi_upb_ServiceDef) * n);
  for (int i = 0; i < n; i++) {
    create_service(ctx, protos[i], parent_features, &s[i]);
    s[i].index = i;
  }
  return s;
}

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
