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

#define FEROX_RAYLIB_IMPLEMENTATION
#include "ferox_raylib.h"

#include "ferox_benchmark.h"

#include "raylib.h"

/* Macros =================================================================> */

// clang-format off

#define BENCHMARK_REPEAT_LIMIT  10
#define BENCHMARK_TIME_LIMIT    3.0f

// clang-format on

/* Typedefs ===============================================================> */

typedef void (*InitBenchFunc)(int);
typedef void (*UpdateBenchFunc)(void);
typedef void (*DeinitBenchFunc)(void);

/* Public Function Prototypes =============================================> */

InitBenchExtern(Smash);
InitBenchExtern(Stacks);

UpdateBenchExtern(Smash);
UpdateBenchExtern(Stacks);

DeinitBenchExtern(Smash);
DeinitBenchExtern(Stacks);

/* Constants ==============================================================> */

static const InitBenchFunc initBenchFuncs[] = { InitBenchSmash,
                                                InitBenchStacks,
                                                NULL };

static const UpdateBenchFunc updateBenchFuncs[] = { UpdateBenchSmash,
                                                    UpdateBenchStacks,
                                                    NULL };

static const DeinitBenchFunc deinitBenchFuncs[] = { DeinitBenchSmash,
                                                    DeinitBenchStacks,
                                                    NULL };

/* Private Variables ======================================================> */

static float benchmarkCounter = 0.0f;

static int benchmarkBodyCount = (1 << 10);
static int benchmarkIndex = 0;
static int benchmarkRepeatCount = 0;

/* Public Functions =======================================================> */

int main(void) {
    InitWindow(SCREEN_WIDTH,
               SCREEN_HEIGHT,
               "c-krit/ferox-benchmark: " __FILE__);

    SetExitKey(KEY_NULL);

    initBenchFuncs[benchmarkIndex](benchmarkBodyCount);

    while (!WindowShouldClose()) {
        if (benchmarkCounter >= BENCHMARK_TIME_LIMIT) {
            deinitBenchFuncs[benchmarkIndex]();

            benchmarkCounter = 0.0f, benchmarkRepeatCount++;

            if (benchmarkRepeatCount >= BENCHMARK_REPEAT_LIMIT)
                benchmarkIndex++;

            initBenchFuncs[benchmarkIndex](benchmarkBodyCount);
        }

        BeginDrawing();

        ClearBackground(ColorBrightness(RAYWHITE, -0.05f));

        {
            updateBenchFuncs[benchmarkIndex]();

            benchmarkCounter += GetFrameTime();
        }

        DrawFPS(8, 8);

        DrawTextEx(GetFontDefault(),
                   TextFormat("[%d:%d] %.2f",
                              benchmarkIndex,
                              benchmarkRepeatCount,
                              benchmarkCounter),
                   (Vector2) { .x = 8.0f, .y = 32.0f },
                   GetFontDefault().baseSize,
                   1.0f,
                   DARKGREEN);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
