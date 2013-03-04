liblockdep-kernel
=================

Linux kernel's lockdep utility as a library by a patchset of Sasha Levin. 

Directories
-----------

* ./patches: The patchsets that was used in order to add the lockdep library. No longer needed, they were added as a historic reference. They can be found in https://lkml.org, search for liblockdep.
* ./tools/lib/lockdep: The files for the liblockdep
* ./tools/perf: The perf tool from the kernel. liblockdep add some instrumentation in this tool.

Building
--------

Not tested yet.

How to use it
-------------

<pre>LD_PRELOAD=/path/to/liblockdep.so /path/to/my/program</pre>
