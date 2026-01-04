#ifndef B1FE9B28_1B07_4413_89CF_807CD159CD6A
#define B1FE9B28_1B07_4413_89CF_807CD159CD6A

#include <math.h>
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
#define STRING_SIZE 4
#define STRING_NAME_SIZE 4
#else
#define STRING_SIZE 8
#define STRING_NAME_SIZE 8
#endif

#ifdef REAL_T_IS_DOUBLE
#define VARIANT_SIZE 40
#define VECTOR2_SIZE 16
typedef double real;
#else
#define VARIANT_SIZE 24
#define VECTOR2_SIZE 8

typedef float real;

#endif

// Types.

typedef struct {
    real x, y;
} Vector2;

typedef struct {
    uint8_t data[STRING_NAME_SIZE];
} StringName;

typedef struct {
    uint8_t data[VARIANT_SIZE];
} Variant;

typedef struct {
    uint8_t data[STRING_SIZE];
} String;

// Enums.

typedef enum {
    PROPERTY_HINT_NONE = 0,
} PropertyHint;

typedef enum {
    PROPERTY_USAGE_NONE = 0,
    PROPERTY_USAGE_STORAGE = 2,
    PROPERTY_USAGE_EDITOR = 4,
    PROPERTY_USAGE_DEFAULT = PROPERTY_USAGE_STORAGE | PROPERTY_USAGE_EDITOR,
} PropertyUsageFlags;

#endif /* B1FE9B28_1B07_4413_89CF_807CD159CD6A */
