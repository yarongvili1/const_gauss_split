# Notes

This is an experimental research code repository, provided under the terms of
the accompanying license, that includes enhancements by Yaron Gvili to
[original code](http://github.com/Angshumank/const_gauss_split) by Angshuman
Karmakar et al:

1. Performance improvements of the AES-CTR sampler as well as extending from a
   64-bit to a 128-bit cycle.
2. Performance improvements of the half-Gaussian sampler using AVX.
3. Automatic configuration of the half-Gaussian sampler.
4. High-performance conversion of a half-Gaussian sampler to a full-Gaussian
   sampler or a bimodal-Gaussian and for dynamic adjustment of a Gaussian
   sampler's parameters.
5. Additional makefiles for build automation.

The enhancements target Linux at run-time and GCC at build-time.

## Improved AES-CTR Sampler

The [AES-CTR sampler code](Standalone sampler/bliss_6/aes_rnd.c) relies on
compiler support for the `__int128` type to efficeintly provide a 128-bit cycle.
Correct support yields an ADC assembly instruction in the `aes\_random\_bytes`
function, as can be seen after running `make` in `Standalone sampler/bliss_6`:
```
$ objdump --disassemble=aes\_random\_bytes sample\_optx.o | head -n 15

sample\_optx.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000390 <aes\_random\_bytes>:
 390:   f3 0f 1e fa             endbr64 
 394:   48 8b 05 00 00 00 00    mov    0x0(%rip),%rax        # 39b <aes\_random\_bytes+0xb>
 39b:   48 8b 15 00 00 00 00    mov    0x0(%rip),%rdx        # 3a2 <aes\_random\_bytes+0x12>
 3a2:   48 83 c0 01             add    $0x1,%rax
 3a6:   c4 e1 f9 6e c0          vmovq  %rax,%xmm0
 3ab:   48 83 d2 00             adc    $0x0,%rdx
 3af:   c4 e3 f9 22 c2 01       vpinsrq $0x1,%rdx,%xmm0,%xmm0
 3b5:   c5 f8 29 05 00 00 00    vmovaps %xmm0,0x0(%rip)        # 3bd <aes\_random\_bytes+0x2d>
```

Further, the `aes\_random\_bytes` function uses the counter directly, without
copying it. These enhancements resulted in improved AES-CTR sampler performance
in the lab setting.

## Improved Half-Gaussian Sampler

The half-Gaussian sampler uses AVX, when available, to speed-up the sampling.
Using AVX, the sampler generates 256, rather than just 64, samples in parallel.
It is possible to generate 512 bits in parallel using simple adaptation of the
code and hardware supporting AVX512; however, this is currently not implemented.
The code for this is in `Standalone sampler/bliss_6/sample_optx.cpp` and relies
on compiler support for vector extensions, which is available in GCC. The code
includes Boolean logic, which is generated elsewhere (see next section), into
its implementation of `HalfGaussianSampler` in a way that leverages AVX, when
available.

## Automatic Configuration of Half-Gaussian Samplers

The automatic configuration is used to generate Boolean logic of a Half-Gaussian
sampler parameterized by `sigma` and number of output bits. After running `make`
at `Gaussian\_tool/Tool`, the automatic configuration can be invoked like so:
```
$ ./gen.sh combined.out 2 5
```
This relies on the system's ability to execute the 32-bit `espresso` executable.
A 64-bit system would normally require installation of 32-bit support-packages.

This will generate Boolean logic of a half-Gaussian sampler with `sigma=2` and 5
output bits into `combined.out`, which the half-Gaussian sampler (see previous
section) is hard-coded to pick up. This Boolean logic is valid for both AVX and
non-AVX sampling code. The automation involves these steps:

1. Generate the appropriate full Boolean logic description in espresso format.
2. Split the Boolean logic for efficeint processing of each part by `espresso`.
3. Invoke `espresso` on each part to obtain Boolean logic.
4. Combine the Boolean logic parts while adapting them for use in `C/C++` code.

## Improved Gaussian Samplers

The improved performance of the full-Gaussian sampler can be seen after running
`make` in `Standalone sampler/bliss_6`:
```
$ time ./gsampler 
Number of rejections: 0
Time for all samples: 442883399
Number of samples: 25600000
Time per sample: 17
 -32 : 0 -31 : 0 -30 : 0 -29 : 0 -28 : 0 -27 : 0 -26 : 0 -25 : 0 -24 : 0 -23 : 0 -22 : 0 -21 : 0 -20 : 0 -19 : 0 -18 : 0 -17 : 0 -16 : 0 -15 : 0 -14 : 0 -13 : 0 -12 : 0 -11 : 1 -10 : 20 -9 : 202 -8 : 1705 -7 : 11161 -6 : 56866 -5 : 224408 -4 : 690162 -3 : 1656743 -2 : 3096749 -1 : 4507707 0 : 5105980 1 : 4507445 2 : 3097939 3 : 1657359 4 : 690967 5 : 224953 6 : 56345 7 : 11281 8 : 1758 9 : 226 10 : 21 11 : 2 12 : 0 13 : 0 14 : 0 15 : 0 16 : 0 17 : 0 18 : 0 19 : 0 20 : 0 21 : 0 22 : 0 23 : 0 24 : 0 25 : 0 26 : 0 27 : 0 28 : 0 29 : 0 30 : 0 31 : 0

real	0m0.193s
user	0m0.193s
sys	0m0.000s
```

The above shows a single-threaded rate of over 130 million samples per second at
a 2.3 GHz CPU clock rate on an Intel(R) Core(TM) i7-10875H CPU in constructing a
histogram for a specific Gaussian sampler parameterized with `mu=0,sigma=2` and
5 output bits. The above time measurements are in cycles.

The improved full-Gaussian sampler also supports a sampler adjustment procedure,
in which the mu and sigma parameters can be moderately adjusted dynamically. The
performance is about an order of magnitude slower than without adjustment, as
can be seen in an invocation adjusting to `mu=0.5,sigmai=1.9`:
```
$ time ./gsampler 0.5 1.9
Number of rejections: 28308293
Time for all samples: 4983611143
Number of samples: 25600000
Time per sample: 194
 -32 : 0 -31 : 0 -30 : 0 -29 : 0 -28 : 0 -27 : 0 -26 : 0 -25 : 0 -24 : 0 -23 : 0 -22 : 0 -21 : 0 -20 : 0 -19 : 0 -18 : 0 -17 : 0 -16 : 0 -15 : 0 -14 : 0 -13 : 0 -12 : 0 -11 : 0 -10 : 3 -9 : 39 -8 : 242 -7 : 2329 -6 : 15217 -5 : 81496 -4 : 326525 -3 : 985879 -2 : 2260110 -1 : 3935355 0 : 5193219 1 : 5192161 2 : 3937596 3 : 2260733 4 : 984718 5 : 325497 6 : 81230 7 : 15171 8 : 2233 9 : 203 10 : 41 11 : 3 12 : 0 13 : 0 14 : 0 15 : 0 16 : 0 17 : 0 18 : 0 19 : 0 20 : 0 21 : 0 22 : 0 23 : 0 24 : 0 25 : 0 26 : 0 27 : 0 28 : 0 29 : 0 30 : 0 31 : 0

real	0m2.164s
user	0m2.164s
sys	0m0.000s

```

The above shows a single-threaded rate of about 12 million samples per second at
a 2.3 GHz CPU clock rate on an Intel(R) Core(TM) i7-10875H CPU in constructing a
histogram for a specific Gaussian sampler parameterized with `mu=0.5,sigma=1.9`
and 5 output bits. The above time measurements are in cycles.

The high-performance bimodal-Gaussian sampler runs at a slightly lower rate:
```
$ time ./sample\_optx 
Number of rejections: 38346419
Time for all samples: 5523391027
Number of samples: 25600000
Time per sample: 215
 -16 : 0 -15 : 0 -14 : 0 -13 : 0 -12 : 0 -11 : 0 -10 : 0 -9 : 0 -8 : 0 -7 : 0 -6 : 0 -5 : 56 -4 : 4267 -3 : 141314 -2 : 1725427 -1 : 7736888 0 : 6376737 1 : 7743082 2 : 1726404 3 : 141513 4 : 4265 5 : 47 6 : 0 7 : 0 8 : 0 9 : 0 10 : 0 11 : 0 12 : 0 13 : 0 14 : 0 15 : 0

real	0m2.398s
user	0m2.394s
sys	0m0.004s

```

The above shows a single-threaded rate of over 10 million samples per second at
a 2.3 GHz CPU clock rate on an Intel(R) Core(TM) i7-10875H CPU in constructing a
histogram for a specific Gaussian sampler parameterized with `mu=0,sigma=1.9`
and 5 output bits. The above time measurements are in cycles.

## Acknowledgements

Thanks to Angshuman Karmakar for kindly helping with discussions about the
original code, the associated paper, and related work.

Work on the enhancements was supported by ISF Grant No. 1399/17 and Project
PROMETHEUS (Grant 780701).
