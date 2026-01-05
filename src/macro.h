#ifndef A4B2713F_A7CE_48BE_9091_04110D6DEA70
#define A4B2713F_A7CE_48BE_9091_04110D6DEA70

/*
 * Concatenate preprocessor tokens A and B without expanding macro definitions
 * (however, if invoked from a macro, macro arguments are expanded).
 */
#define PPCAT_NX(A, B) A ## B

/*
 * Concatenate preprocessor tokens A and B after macro-expanding them.
 */
#define PPCAT(A, B) PPCAT_NX(A, B)

#endif /* A4B2713F_A7CE_48BE_9091_04110D6DEA70 */
