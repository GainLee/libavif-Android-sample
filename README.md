# libavif-Android-sample [![AppVeyor Build Status](https://ci.appveyor.com/api/projects/status/github/louquillio/libavif?branch=master&svg=true)](https://ci.appveyor.com/project/louquillio/libavif) [![Travis Build Status](https://travis-ci.com/AOMediaCodec/libavif.svg?branch=master)](https://travis-ci.com/AOMediaCodec/libavif)
An Android sample of AVIF decoding and encoding

This is a simplified repository of 
[libavif](https://github.com/AOMediaCodec/libavif) on android. It shows how to decode avif files on android and how to encode image data into avif files on android.

It contains three prebuilt libraries, libavif, a decoder library, and an encoder library. You can try out
them without going through the build process, they are all static library builds. The decoder is libgav1 and the encoder is AOM.
If you want to replace the codec,  you will replace not only  the corresponding codec static library,  but alslo the libavif static library, which means you must recompile the libavif static library, and modify the corresponding include file. You can refer to the libavif repository for compilation.


## License

Released under the BSD License.

```markdown
Copyright (c) 2022, GainLee, perfecter.gen@gmail.com
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
