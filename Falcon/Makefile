# Build script for the Falcon implementation.
#
# ==========================(LICENSE BEGIN)============================
#
# Copyright (c) 2017  Falcon Project
#
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to
# the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
# CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
# TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
# SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#
# ===========================(LICENSE END)=============================
#
# @author   Thomas Pornin <thomas.pornin@nccgroup.trust>

.POSIX:

CC = gcc
#CFLAGS = -W -Wall -O3 -std=c99 #-pg -fno-pie
#CFLAGS = -W -Wall -O3  #-pg -fno-pie
CFLAGS = -W -Wall -O3 -fomit-frame-pointer -march=native -std=c99 #-pg -fno-pie
LD = gcc
LDFLAGS = #-pg -no-pie
LDLIBS = -lm

OBJ = falcon-enc.o falcon-vrfy.o frng.o shake.o falcon-fft.o falcon-keygen.o falcon-sign.o c_sample.o aes_rng.o #cpucycles.o

all: test_falcon falcon

clean:
	-rm -f $(OBJ) test_falcon test_falcon.o falcon tool.o c_sample.o aes_rng.o #cpucycles.o

test_falcon: test_falcon.o $(OBJ)
	$(LD) $(LDFLAGS) -o test_falcon test_falcon.o $(OBJ) $(LDLIBS)

falcon: tool.o $(OBJ)
	$(LD) $(LDFLAGS) -o falcon tool.o $(OBJ) $(LDLIBS)

test_falcon.o: test_falcon.c falcon.h internal.h fpr-double.h shake.h aes_rng.h
	$(CC) $(CFLAGS) -c -o test_falcon.o test_falcon.c

tool.o: tool.c falcon.h
	$(CC) $(CFLAGS) -c -o tool.o tool.c

falcon-enc.o: falcon-enc.c falcon.h internal.h fpr-double.h shake.h
	$(CC) $(CFLAGS) -c -o falcon-enc.o falcon-enc.c

falcon-fft.o: falcon-fft.c falcon.h internal.h fpr-double.h shake.h
	$(CC) $(CFLAGS) -c -o falcon-fft.o falcon-fft.c

falcon-keygen.o: falcon-keygen.c falcon.h internal.h fpr-double.h shake.h
	$(CC) $(CFLAGS) -c -o falcon-keygen.o falcon-keygen.c

falcon-sign.o: falcon-sign.c falcon.h internal.h fpr-double.h shake.h c_sample.h aes_rng.h #cpucycles.h
	$(CC) $(CFLAGS) -c -o falcon-sign.o falcon-sign.c

falcon-vrfy.o: falcon-vrfy.c falcon.h internal.h fpr-double.h shake.h
	$(CC) $(CFLAGS) -c -o falcon-vrfy.o falcon-vrfy.c

frng.o: frng.c falcon.h internal.h fpr-double.h shake.h
	$(CC) $(CFLAGS) -c -o frng.o frng.c

shake.o: shake.c shake.h
	$(CC) $(CFLAGS) -c -o shake.o shake.c

c_sample.o: c_sample.c c_sample.h
	$(CC) $(CFLAGS) -c -o c_sample.o c_sample.c

aes_rng.o: aes_rng.c aes_rng.h
	$(CC) $(CFLAGS) -c -o aes_rng.o aes_rng.c

#cpucycles.o: cpucycles.c cpucycles.h
#	$(CC) $(CFLAGS) -c -o cpucycles.o cpucycles.c
	