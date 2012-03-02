#!/bin/sh
find . -name '*.c' -o -name '*.cpp' -o -name '*.cc' -o -name '*.h' -o -name '*.s' -o -name '*.S' > cscope.files
cscope -bi cscope.files
