#!/bin/bash

set -xe

cc -O3 -o annotate annotate.c -Iraylib/include raylib/lib/libraylib.a -lm