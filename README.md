fastalen
========

Every bioinformatician has its own method to get the size of sequences from
FASTA files. Here is mine.

Compilation
-----------

`fastalen` is written in C99, and uses the POSIX C library. In addition,
it uses:

* `kseq.h` from [klib][1] to parse FASTA sequences,
* [argtable3][2] to parse command-line arguments.

This files are included.

To compile, just run

    $ c99 -o fastalen argtable3.c fastalen.c

Usage
-----

    Usage: fastalen  <in.fasta> [<in.fasta>]...  [-t|--tsv] [-c|--csv] [-C|--count] [-O|--only-count]  [--help] [--version]
    Read file(s) containing FASTA sequence(s) and return their ID and length.

    Mandatory argument:         
      <in.fasta>                input fasta file(s)
                            
    Formatting options:
      -t, --tsv                 output as TSV
      -c, --csv                 output as CSV
      -C, --count               count the number of sequences
      -O, --only-count          ONLY count the number of sequences
                            
      --help                    display this help and exit
      --version                 display version info and exit

Licence
-------

Copyright © 2018 Sylvain PULICANI <picani@laposte.net>

This work is free. You can redistribute it and/or modify it under the
terms of the Do What The Fuck You Want To Public License, Version 2,
as published by Sam Hocevar. See the COPYING file for more details.


[1]: https://attractivechaos.github.io/klib/#About
[2]: https://www.argtable.org
