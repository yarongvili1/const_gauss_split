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
CXX = g++
#CFLAGS = -W -Wall -O3 -std=c99 #-pg -fno-pie
#CFLAGS = -W -Wall -O3  #-pg -fno-pie
CFLAGS = -W -Wall -O3 -fomit-frame-pointer -march=native -std=gnu99 #-pg -fno-pie
CFLAGS += -msse -msse2 -mavx -mavx2 -maes -iquote ../../Falcon -iquote ../../Gaussian_tool/Tool
CXXFLAGS = $(CFLAGS) -std=c++11
LD = gcc
LDFLAGS = #-pg -no-pie
LDLIBS = -lm

OBJ = #cpucycles.o c_sample1.o c_sample2.o c_sample.o fips202.o randombytes.o

all: master-128 sample_opt sample_optx gsampler

clean:
	-rm -f $(OBJ) master-128 master-128.o sample_opt sample_opt.o sample_optx sample_optx.o gsampler gsampler.o

master-128: master-128.o $(OBJ)
	$(LD) $(LDFLAGS) -o master-128 master-128.o $(OBJ) $(LDLIBS)

sample_opt: sample_opt.o $(OBJ)
	$(LD) $(LDFLAGS) -o sample_opt sample_opt.o $(OBJ) $(LDLIBS)

sample_optx: sample_optx.o $(OBJ)
	$(LD) $(LDFLAGS) -o sample_optx sample_optx.o $(OBJ) $(LDLIBS)

gsampler: gsampler.o $(OBJ)
	$(LD) $(LDFLAGS) -o gsampler gsampler.o $(OBJ) $(LDLIBS)
