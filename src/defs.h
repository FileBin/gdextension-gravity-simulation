#ifndef B1FE9B28_1B07_4413_89CF_807CD159CD6A
#define B1FE9B28_1B07_4413_89CF_807CD159CD6A

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if !defined(GDE_EXPORT)
#if defined(_WIN32)
#define GDE_EXPORT __declspec(dllexport)
#elif defined(__GNUC__)
#define GDE_EXPORT __attribute__((visibility("default")))
#else
#define GDE_EXPORT
#endif
#endif // ! GDE_EXPORT

#ifdef BUILD_32
#define STRING_NAME_SIZE 4
#else
#define STRING_NAME_SIZE 8
#endif

#ifdef REAL_T_IS_DOUBLE
#define VARIANT_SIZE 40
#define VECTOR2_SIZE 16
#else
#define VARIANT_SIZE 24
#define VECTOR2_SIZE 8
#endif

// Types.

typedef struct
{
    uint8_t data[STRING_NAME_SIZE];
} StringName;

typedef struct
{
    uint8_t data[VARIANT_SIZE];
} Variant;

#endif /* B1FE9B28_1B07_4413_89CF_807CD159CD6A */
