#include <typedef.h>
typedef struct {
    UW r_[8];

    // these are automatically saved by the hardware
    UW r[4];
    UW ip;
    UW lr;
    UW pc;
    UW xpsr;
} StackFrame;

void *make_context(UW *sp, UINT ssize, void (*fp)()) {
    StackFrame *sfp;
    sfp = (StackFrame *)((UB *)sp + ssize);
    sfp--;

    sfp->xpsr = 0x01000000;
    sfp->pc = (UW)fp & ~1UL;
    return (void *)sfp;
}
