/*
 * Copyright 2019 The libgav1 Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef LIBGAV1_SRC_DSP_X86_INVERSE_TRANSFORM_SSE4_H_
#define LIBGAV1_SRC_DSP_X86_INVERSE_TRANSFORM_SSE4_H_

#include "src/dsp/dsp.h"
#include "src/utils/cpu.h"

namespace libgav1 {
namespace dsp {

// Initializes Dsp::inverse_transforms, see the defines below for specifics.
// This function is not thread-safe.
void InverseTransformInit_SSE4_1();

}  // namespace dsp
}  // namespace libgav1

// If sse4 is enabled and the baseline isn't set due to a higher level of
// optimization being enabled, signal the sse4 implementation should be used.
#if LIBGAV1_TARGETING_SSE4_1

#ifndef LIBGAV1_Dsp8bpp_Transform1dSize4_Transform1dDct
#define LIBGAV1_Dsp8bpp_Transform1dSize4_Transform1dDct LIBGAV1_CPU_SSE4_1
#endif

#ifndef LIBGAV1_Dsp8bpp_Transform1dSize8_Transform1dDct
#define LIBGAV1_Dsp8bpp_Transform1dSize8_Transform1dDct LIBGAV1_CPU_SSE4_1
#endif

#ifndef LIBGAV1_Dsp8bpp_Transform1dSize16_Transform1dDct
#define LIBGAV1_Dsp8bpp_Transform1dSize16_Transform1dDct LIBGAV1_CPU_SSE4_1
#endif

#ifndef LIBGAV1_Dsp8bpp_Transform1dSize32_Transform1dDct
#define LIBGAV1_Dsp8bpp_Transform1dSize32_Transform1dDct LIBGAV1_CPU_SSE4_1
#endif

#ifndef LIBGAV1_Dsp8bpp_Transform1dSize64_Transform1dDct
#define LIBGAV1_Dsp8bpp_Transform1dSize64_Transform1dDct LIBGAV1_CPU_SSE4_1
#endif

#ifndef LIBGAV1_Dsp8bpp_Transform1dSize4_Transform1dAdst
#define LIBGAV1_Dsp8bpp_Transform1dSize4_Transform1dAdst LIBGAV1_CPU_SSE4_1
#endif

#ifndef LIBGAV1_Dsp8bpp_Transform1dSize8_Transform1dAdst
#define LIBGAV1_Dsp8bpp_Transform1dSize8_Transform1dAdst LIBGAV1_CPU_SSE4_1
#endif

#ifndef LIBGAV1_Dsp8bpp_Transform1dSize16_Transform1dAdst
#define LIBGAV1_Dsp8bpp_Transform1dSize16_Transform1dAdst LIBGAV1_CPU_SSE4_1
#endif

#ifndef LIBGAV1_Dsp8bpp_Transform1dSize4_Transform1dIdentity
#define LIBGAV1_Dsp8bpp_Transform1dSize4_Transform1dIdentity LIBGAV1_CPU_SSE4_1
#endif

#ifndef LIBGAV1_Dsp8bpp_Transform1dSize8_Transform1dIdentity
#define LIBGAV1_Dsp8bpp_Transform1dSize8_Transform1dIdentity LIBGAV1_CPU_SSE4_1
#endif

#ifndef LIBGAV1_Dsp8bpp_Transform1dSize16_Transform1dIdentity
#define LIBGAV1_Dsp8bpp_Transform1dSize16_Transform1dIdentity LIBGAV1_CPU_SSE4_1
#endif

#ifndef LIBGAV1_Dsp8bpp_Transform1dSize32_Transform1dIdentity
#define LIBGAV1_Dsp8bpp_Transform1dSize32_Transform1dIdentity LIBGAV1_CPU_SSE4_1
#endif

#ifndef LIBGAV1_Dsp8bpp_Transform1dSize4_Transform1dWht
#define LIBGAV1_Dsp8bpp_Transform1dSize4_Transform1dWht LIBGAV1_CPU_SSE4_1
#endif
#endif  // LIBGAV1_TARGETING_SSE4_1
#endif  // LIBGAV1_SRC_DSP_X86_INVERSE_TRANSFORM_SSE4_H_
