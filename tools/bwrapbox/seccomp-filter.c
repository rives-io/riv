#define _GNU_SOURCE
#include <seccomp.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sched.h>
#include <stdio.h>
#include <sys/ioctl.h>

#define ALLOW_RULE(call) { if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(call), 0) < 0) goto out; }
#define DISALOW_RULE(call, errnum) { if (seccomp_rule_add(ctx, SCMP_ACT_ERRNO(errnum), SCMP_SYS(call), 0) < 0) goto out; }
#define DISALOW_RULE_CMP(call, errnum, a0) { if (seccomp_rule_add(ctx, SCMP_ACT_ERRNO(errnum), SCMP_SYS(call), 1, a0) < 0) goto out; }

int main(int argc, char *argv[])
{
    int fd = -1;
    int res = -1;
    scmp_filter_ctx ctx = {0};

    // init seccomp for blacklisting
    ctx = seccomp_init(SCMP_ACT_ERRNO(EPERM));
    if (ctx == NULL)
        goto out;

    // remove current architecture
    if (seccomp_arch_remove(ctx, SCMP_ARCH_NATIVE) < 0)
        goto out;

    // set RISC-V 64 architecture
    if (seccomp_arch_add(ctx, SCMP_ARCH_RISCV64) < 0)
        goto out;

    // disallow clone() with CLONE_NEWUSER
    DISALOW_RULE_CMP(clone, ENOSYS, SCMP_A0(SCMP_CMP_MASKED_EQ, CLONE_NEWUSER, CLONE_NEWUSER))
    // disallow faking input to the controlling tty (CVE-2017-5226)
    DISALOW_RULE_CMP(ioctl, ENOSYS, SCMP_A1(SCMP_CMP_MASKED_EQ, 0xFFFFFFFFu, (int)TIOCSTI))
    // disallow clone3(), by returning ENOSYS user-space will fall back to clone()
    DISALOW_RULE(clone3, ENOSYS)

    #include "seccomp-filter-rules.h"

    // create filter file
    if (argc < 0)
        goto out;
    fd = open(argv[1], O_CREAT | O_WRONLY, 0644);
    if (fd < 0)
        goto out;

    // export bpf
    if (seccomp_export_bpf(ctx, fd) < 0)
        goto out;

    // success
    res = 0;

 out:
    if (fd >= 0)
        close(fd);
    seccomp_release(ctx);
    return res;
}
