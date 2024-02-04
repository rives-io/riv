#!/bin/sh
exec c2m jit.c -L/usr/lib -lriv -w -O3 -eg "$@"
