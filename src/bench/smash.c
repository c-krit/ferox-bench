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

#include "ferox.h"
#include "ferox_raylib.h"
#include "ferox_benchmark.h"

#include "raylib.h"

/* Macros ================================================================== */

// clang-format off

#define BOX_SIZE        1.0f

#define SQRT_BOX_COUNT  32

// clang-format on

/* Constants =============================================================== */

static const float CELL_SIZE = BOX_SIZE, DELTA_TIME = 1.0f / TARGET_FPS;

/* Private Variables ======================================================> */

static frWorld *world;

static frShape *ballShape, *boxShape;

static Camera2D camera = {
    .offset = { .x = 0.5f * SCREEN_WIDTH, .y = 0.5f * SCREEN_HEIGHT },
    .target = { .x = 0.5f * SCREEN_WIDTH, .y = 0.5f * SCREEN_HEIGHT },
    .zoom = 0.5f
};

static Color ballColor = RED, boxColor = GRAY;

/* Public Functions =======================================================> */

InitBench(Smash) {
    world = frCreateWorld(frStructZero(frVector2), CELL_SIZE);

    {
        boxShape = frCreateRectangle((frMaterial) { .density = 1.0f,
                                                    .friction = 0.5f },
                                     BOX_SIZE,
                                     BOX_SIZE);

        frVector2 boxOrigin = { .x = 0.5f
                                     * (frPixelsToUnits(SCREEN_WIDTH)
                                        - (BOX_SIZE * SQRT_BOX_COUNT)),
                                .y = 0.5f
                                     * (frPixelsToUnits(SCREEN_HEIGHT)
                                        - (BOX_SIZE * SQRT_BOX_COUNT)) };

        for (int y = 0; y < SQRT_BOX_COUNT; y++)
            for (int x = 0; x < SQRT_BOX_COUNT; x++) {
                frVector2 boxPosition = {
                    .x = (boxOrigin.x + (0.5f * BOX_SIZE)) + (x * BOX_SIZE),
                    .y = (boxOrigin.y + (0.5f * BOX_SIZE)) + (y * BOX_SIZE)
                };

                frBody *box = frCreateBodyFromShape(FR_BODY_DYNAMIC,
                                                    boxPosition,
                                                    boxShape);

                frAddBodyToWorld(world, box);

                frSetBodyUserData(box, &boxColor);
            }
    }

    {
        ballShape = frCreateCircle((frMaterial) { .density = 1.0f,
                                                  .friction = 0.5f },
                                   frPixelsToUnits(84.0f));

        frBody *ball = frCreateBodyFromShape(
            FR_BODY_DYNAMIC,
            frVector2PixelsToUnits((frVector2) { .x = -0.5f * SCREEN_WIDTH,
                                                 .y = 0.5f * SCREEN_HEIGHT }),
            ballShape);

        frSetBodyUserData(ball, &ballColor);

        frApplyImpulseToBody(ball,
                             frGetBodyPosition(ball),
                             (frVector2) { .x = (1 << 11) });

        frAddBodyToWorld(world, ball);
    }
}

UpdateBench(Smash) {
    { frUpdateWorld(world, DELTA_TIME); }

    BeginMode2D(camera);

    for (int i = 0, n = frGetBodyCountInWorld(world); i < n; i++) {
        frBody *body = frGetBodyInWorld(world, i);

        Color *bodyColor = frGetBodyUserData(body);

        if (bodyColor != NULL) frDrawBodyLines(body, 2.0f, *bodyColor);
    }

    EndMode2D();
}

DeinitBench(Smash) {
    frReleaseShape(ballShape), frReleaseShape(boxShape);

    frReleaseWorld(world);
}
