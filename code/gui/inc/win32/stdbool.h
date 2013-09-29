#ifndef __STDBOOL_H
#define __STDBOOL_H

#if defined(_MSC_VER)
typedef int _Bool;
#endif

/* Don't define bool, true, and false in C++, except as a GNU extension. */
#ifndef __cplusplus
#define bool _Bool
#define true 1
#define false 0
#else
/* Define _Bool, bool, false, true as a GNU extension. */
#define _Bool bool
#define bool  bool
#define false false
#define true  true
#endif

#define __bool_true_false_are_defined 1

#endif /* __STDBOOL_H */