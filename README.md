# Analyze core dump

ulimit -c unlimit

run demo

gdb demo core.dump

(gdb)bt

(gdb)frame 0...4 (swich frame)

(gdb)info locals

(gdb) core-file core.dump
