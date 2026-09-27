#ifndef RCC_MISC_H
#define RCC_MISC_H

#if __STDC_VERSION__ >= 202311L
#define RCC_NORETURN [[noreturn]]
#elif __STDC_VERSION__ >= 201112L
#define RCC_NORETURN _Noreturn
#else
#if defined(__GNUC__) || defined(__clang__)
#define RCC_NORETURN __attribute__((noreturn))
#elif defined(_MSC_VER)
#define RCC_NORETURN __declspec(noreturn)
#else
#define RCC_NORETURN /* Fallback, do nothing */
#endif
#endif

RCC_NORETURN void fail(const char *message);
RCC_NORETURN void unreachable();

#endif
