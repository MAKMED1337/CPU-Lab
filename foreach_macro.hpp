// https://www.scs.stanford.edu/~dm/blog/va-opt.html

#define PARENS ()

#define EXPAND5(...) EXPAND4(EXPAND4(EXPAND4(EXPAND4(__VA_ARGS__))))
#define EXPAND4(...) EXPAND3(EXPAND3(EXPAND3(EXPAND3(__VA_ARGS__))))
#define EXPAND3(...) EXPAND2(EXPAND2(EXPAND2(EXPAND2(__VA_ARGS__))))
#define EXPAND2(...) EXPAND1(EXPAND1(EXPAND1(EXPAND1(__VA_ARGS__))))
#define EXPAND1(...) __VA_ARGS__

//64 is enough
#define EXPAND(...) EXPAND4(__VA_ARGS__)

#define FOR_EACH(macro, ...)                                    \
  __VA_OPT__(EXPAND(FOR_EACH_HELPER(macro, __VA_ARGS__)))
#define FOR_EACH_HELPER(macro, a1, ...)                         \
  macro(a1)                                                     \
  __VA_OPT__(FOR_EACH_AGAIN PARENS (macro, __VA_ARGS__))
#define FOR_EACH_AGAIN() FOR_EACH_HELPER



#define FOR_EACH2(macro, ...)                                   \
  __VA_OPT__(EXPAND(FOR_EACH_HELPER2(macro, __VA_ARGS__)))
#define FOR_EACH_HELPER2(macro, a1, a2, ...)                    \
  macro(a1, a2)                                                 \
  __VA_OPT__(FOR_EACH_AGAIN2 PARENS (macro, __VA_ARGS__))
#define FOR_EACH_AGAIN2() FOR_EACH_HELPER2
