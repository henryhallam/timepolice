/* Time Police
 * Copyright April 2014 Henry Hallam <henry@pericynthion.org>
 * Released under MIT License
 *
 * When installed, this will print a notification to the kernel log any time
 * something adjusts the system clock, identifying the process concerned.
 *
 */


#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kprobes.h>
#include <linux/sched.h>
#include <uapi/linux/time.h>

/* Syscalls of interest:
   kernel/time.c
   81:SYSCALL_DEFINE1(stime, time_t __user *, tptr)
   188:SYSCALL_DEFINE2(settimeofday, struct timeval __user *, tv,
   209:SYSCALL_DEFINE1(adjtimex, struct timex __user *, txc_p)

   kernel/posix-timers.c
   1002:SYSCALL_DEFINE2(clock_settime, const clockid_t, which_clock,
   1035:SYSCALL_DEFINE2(clock_adjtime, const clockid_t, which_clock,

   ultimately these all call either do_settimeofday or do_adjtimex, both in
   kernel/time/timekeeping.c
*/

/* Proxy routine having the same arguments as actual do_settimeofday() routine */
static long jdo_settimeofday(const struct timespec *tv)
{
	printk(KERN_INFO
	       "TP: pid %d (%s) caused do_settimeofday(*%p = {%ld, %ld})\n",
	       task_pid_nr(current), current->comm, tv, (long)tv->tv_sec,
	       tv->tv_nsec);

	jprobe_return();
	return 0;
}

static struct jprobe my_jprobe = {
	.entry = jdo_settimeofday,
	.kp = {
	       .symbol_name = "do_settimeofday",
	       },
};

static int __init tp_init(void)
{
	int ret;

	ret = register_jprobe(&my_jprobe);
	if (ret < 0) {
		printk(KERN_INFO "TP: register_jprobe failed, returned %d\n", ret);
		return -1;
	}
	pr_info("TP: Locked and loaded.\n");
	return 0;
}

static void __exit tp_exit(void)
{
	unregister_jprobe(&my_jprobe);
	pr_info
	    ("TP: Never run away from the Time Police!  You will not survive.\n");
}

module_init(tp_init);
module_exit(tp_exit);

MODULE_LICENSE("Dual MIT/GPL");
MODULE_DESCRIPTION("Time police");
MODULE_AUTHOR("Henry Hallam <henry@pericynthion.org>");
