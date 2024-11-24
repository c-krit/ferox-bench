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

/* Includes ===============================================================> */

#include <stdio.h>

#define FEROX_RAYLIB_IMPLEMENTATION
#include "ferox_raylib.h"

#include "ferox_benchmark.h"

#include "raylib.h"

/* Macros =================================================================> */

// clang-format off

#define BENCHMARK_COUNT         2
#define BENCHMARK_REPEAT_LIMIT  10
#define BENCHMARK_SAMPLE_RATE   1.0f
#define BENCHMARK_TIME_LIMIT    10.0f

#define MAX_FILE_NAME_LENGTH    64

// clang-format on

/* Typedefs ===============================================================> */

typedef void (*InitBenchFunc)(int);
typedef void (*DrawBenchFunc)(void);
typedef void (*UpdateBenchFunc)(void);
typedef void (*DeinitBenchFunc)(void);

/* Public Function Prototypes =============================================> */

InitBenchExtern(Smash);
InitBenchExtern(Stacks);

DrawBenchExtern(Smash);
DrawBenchExtern(Stacks);

UpdateBenchExtern(Smash);
UpdateBenchExtern(Stacks);

DeinitBenchExtern(Smash);
DeinitBenchExtern(Stacks);

/* Constants ==============================================================> */

static const InitBenchFunc initBenchFuncs[] = { InitBenchSmash,
                                                InitBenchStacks,
                                                NULL };

static const DrawBenchFunc drawBenchFuncs[] = { DrawBenchSmash,
                                                DrawBenchStacks,
                                                NULL };

static const UpdateBenchFunc updateBenchFuncs[] = { UpdateBenchSmash,
                                                    UpdateBenchStacks,
                                                    NULL };

static const DeinitBenchFunc deinitBenchFuncs[] = { DeinitBenchSmash,
                                                    DeinitBenchStacks,
                                                    NULL };

static const int benchmarkSampleCount = ((BENCHMARK_TIME_LIMIT
                                          / BENCHMARK_SAMPLE_RATE)
                                         * (BENCHMARK_REPEAT_LIMIT));

/* Private Variables ======================================================> */

static char benchmarkResultFileName[MAX_FILE_NAME_LENGTH];

static float benchmarkCounter = 0.0f;
static float benchmarkSampleCounter = 0.0f;

static long long int fpsSum;

static int benchmarkBodyCount = (1 << 11);
static int benchmarkIndex = 0;
static int benchmarkRepeatCount = 0;

static int averageFPS[BENCHMARK_COUNT];

/* Public Functions =======================================================> */

int main(void) {
    InitWindow(SCREEN_WIDTH,
               SCREEN_HEIGHT,
               "c-krit/ferox-benchmark: " __FILE__);

    SetExitKey(KEY_NULL);

    initBenchFuncs[benchmarkIndex](benchmarkBodyCount);

    while (!WindowShouldClose()) {
        if (benchmarkSampleCounter >= BENCHMARK_SAMPLE_RATE) {
            fpsSum += GetFPS();

            benchmarkSampleCounter = 0.0f;
        }

        if (benchmarkCounter >= BENCHMARK_TIME_LIMIT) {
            deinitBenchFuncs[benchmarkIndex]();

            benchmarkCounter = 0.0f, benchmarkRepeatCount++;

            if (benchmarkRepeatCount >= BENCHMARK_REPEAT_LIMIT) {
                averageFPS[benchmarkIndex] = fpsSum / benchmarkSampleCount;

                benchmarkIndex++, benchmarkRepeatCount = 0;

                if (initBenchFuncs[benchmarkIndex] == NULL) break;
            }

            initBenchFuncs[benchmarkIndex](benchmarkBodyCount);
        }

        {
            updateBenchFuncs[benchmarkIndex]();

            benchmarkCounter += GetFrameTime();
            benchmarkSampleCounter += GetFrameTime();
        }

        BeginDrawing();

        ClearBackground(ColorBrightness(RAYWHITE, -0.05f));

        drawBenchFuncs[benchmarkIndex]();

        DrawFPS(8, 8);

        DrawTextEx(GetFontDefault(),
                   TextFormat("[#%d:%d] %.2f",
                              benchmarkIndex,
                              benchmarkRepeatCount,
                              benchmarkCounter),
                   (Vector2) { .x = 8.0f, .y = 32.0f },
                   (GetFontDefault().baseSize << 1),
                   1.0f,
                   DARKGREEN);

        EndDrawing();
    }

    {
        for (int i = 0; i < BENCHMARK_COUNT; i++) {
            TextCopy(benchmarkResultFileName,
                     TextFormat("ferox_bench-%d.txt", i));

            FILE *file = fopen(benchmarkResultFileName, "a");

            fputs(TextFormat("%d %d\n", benchmarkBodyCount, averageFPS[i]),
                  file);

            fclose(file);
        }
    }

    CloseWindow();

    return 0;
}
