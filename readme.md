This is a toy project to demonstrate some different implementations of
[this StackExchange question](https://codereview.stackexchange.com/questions/230430).

## Fun with Bash

Generate a bunch of codes, only looking at every millionth:

```
$ ./mbi 1A00A00AA00 1000000000 | awk 'NR % 1000000 == 0'
1A00A22TE99
1A00A45JJ99
1A00A68AP99
1A00A90TU99
1A00AC3JY99
1A00AE6CD99
1A00AG8UH99
1A00AK1KN99
1A00AN4CT99
1A00AQ6UX99
1A00AT9MC99
1A00AW2DG99
1A00AY4VM99
...
```

See how fast we can generate 200,000,000 codes (turns out to be one second):

```
$ time ./mbi 1A00A00AA00 200000000 > /dev/null

real	0m1.131s
user	0m1.077s
sys	0m0.054s
```
