#!/bin/bash

cd $(dirname -- "$0")

NAME="$1"
if [ -z "$NAME" ]; then echo "Usage: $0 <name> [sigma] [n_out]" 1>&2; exit 1; fi
shift
SIGMA="$1"
if [ -z "$SIGMA" ]; then SIGMA=2; fi
shift
N_OUT="$1"
if [ -z "$N_OUT" ]; then N_OUT=5; fi

if [ -d inp ]; then rm -rf inp; fi
mkdir inp
if [ -d out ]; then rm -rf out; fi
mkdir out

(./sampler $SIGMA $N_OUT > inp/L) && ./split.sh && ./run.sh && ./merge.sh "$NAME"
