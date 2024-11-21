/*
    Copyright (c) 2024 Jaedeok Kim <jdeokkim@protonmail.com>

    Permission is hereby granted, free of charge, to any person obtaining a 
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation 
    the rights to use, copy, modify, merge, publish, distribute, sublicense, 
    and/or sell copies of the Software, and to permit persons to whom the 
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included 
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
    DEALINGS IN THE SOFTWARE.
*/

#ifndef FEROX_BENCHMARK_H
#define FEROX_BENCHMARK_H

/* Includes ===============================================================> */

#include <assert.h>
#include <stddef.h>

/* Macros =================================================================> */

#define SCREEN_WIDTH   1280
#define SCREEN_HEIGHT  800

#define TARGET_FPS     60

/* ========================================================================> */

#define InitBench(name) \
    void InitBench##name(int bodyCount); void InitBench##name(int bodyCount)

#define InitBenchExtern(name) \
    void InitBench##name(int bodyCount);

#define UpdateBench(name) \
    void UpdateBench##name(void); void UpdateBench##name(void)

#define UpdateBenchExtern(name) \
    void UpdateBench##name(void);

#define DeinitBench(name) \
    void DeinitBench##name(void); void DeinitBench##name(void)

#define DeinitBenchExtern(name) \
    void DeinitBench##name(void);

#endif // `FEROX_BENCHMARK_H`
