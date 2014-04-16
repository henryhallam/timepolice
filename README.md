timepolice
==========

Linux kernel module to catch sneaky attempts to adjust the system clock

Authors
-------
henry@pericynthion.org

Dependencies
------------
sudo apt-get install build-essential linux-headers-$(uname -r)

Installation
------------
make && sudo make install

Example
-------

    henry@commsbox:~/work/kernel/timepolice$ sudo date -s "Jan 16 00:23:42 UTC 1977"
    Sat Jan 15 16:23:42 PST 1977
    henry@commsbox:~/work/kernel/timepolice$ sudo ntpd -qg
    ntpd: time set +1175413488.468819s
    henry@commsbox:~/work/kernel/timepolice$ dmesg | tail -3
    [274489.694540] TP: Locked and loaded.
    [274610.647076] TP: pid 2281 (date) caused do_settimeofday(*e658bf98 = {222222222, 0})
    [274631.144557] TP: pid 2300 (ntpd) caused do_settimeofday(*c4cc3f98 = {1397635730, 957701000})
