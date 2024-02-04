int main(int argc, char** argv);
extern _Noreturn void __libc_start_main(int (*main)(int, char**), int argc, char** argv, void (*init)(), void (*fini)(), void (*rtld_fini)(), void* stack);
__attribute__((externally_visible,naked)) void _start(void) {
  asm volatile(
    ".weak __global_pointer$;\n"
    ".hidden __global_pointer$;\n" // don't export symbol
    ".option push;\n"
    ".option norelax;\n"
    "la gp, __global_pointer$;\n"
    "mv a0, sp;\n" // stack pointer
    "lw a1, 0(sp);\n" // argc
    "addi a2, sp, 8;\n" // argv
    "andi sp, sp, -16;\n" // make stack 16-byte aligned
    ".option pop;\n"
  );
  register int argc asm("a1");
  register char** argv asm("a2");
  register void* sp asm("a0");
  __libc_start_main(main, argc, argv, (void*)0, (void*)0, (void*)0, sp);
}
