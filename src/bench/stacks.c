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

#include "ferox_benchmark.h"
#include "ferox_raylib.h"

/* Macros ================================================================== */

// clang-format off

#define BOX_SIZE             1.0f

#define BOX_COUNT_PER_TOWER  8

// clang-format on

/* Constants =============================================================== */

static const float CELL_SIZE = 1.0f, DELTA_TIME = 1.0f / TARGET_FPS;

/* Private Variables ======================================================> */

static frWorld *world;

static frShape *boxShape, *groundShape;

static Camera2D camera = {
    .offset = { .x = 0.5f * SCREEN_WIDTH, .y = 0.65f * SCREEN_HEIGHT },
    .target = { .x = 0.5f * SCREEN_WIDTH, .y = 0.5f * SCREEN_HEIGHT },
    .zoom = 0.5f
};

static Color boxColor = GRAY, groundColor = BROWN;

static Rectangle boxRectangle;

static RenderTexture boxRenderTexture;

/* Public Functions =======================================================> */

InitBench(Stacks) {
    world = frCreateWorld(frVector2ScalarMultiply(FR_WORLD_DEFAULT_GRAVITY,
                                                  0.75f),
                          CELL_SIZE);

    {
        boxRenderTexture = LoadRenderTexture(frUnitsToPixels(BOX_SIZE),
                                             frUnitsToPixels(BOX_SIZE));

        {
            BeginTextureMode(boxRenderTexture);

            ClearBackground(BLANK);

            boxRectangle = (Rectangle) { .width = frUnitsToPixels(BOX_SIZE),
                                         .height = frUnitsToPixels(BOX_SIZE) };

            DrawRectangleLinesEx(boxRectangle, 2.0f, boxColor);

            DrawCircleV((Vector2) { .x = 0.5f * frUnitsToPixels(BOX_SIZE),
                                    .y = 0.5f * frUnitsToPixels(BOX_SIZE) },
                        2.0f,
                        boxColor);

            EndTextureMode();
        }
    }

    float groundWidth = frPixelsToUnits(16.0f * SCREEN_WIDTH);
    float groundHeight = frPixelsToUnits(0.4f * SCREEN_HEIGHT);

    frVector2 groundPosition = frVector2PixelsToUnits(
        (frVector2) { .x = 0.5f * SCREEN_WIDTH, .y = 1.0f * SCREEN_HEIGHT });

    {
        groundShape = frCreateRectangle((frMaterial) { .density = 1.0f,
                                                       .friction = 0.5f },
                                        groundWidth,
                                        groundHeight);

        frBody *ground = frCreateBodyFromShape(FR_BODY_STATIC,
                                               groundPosition,
                                               groundShape);

        frSetBodyUserData(ground, &groundColor);

        frAddBodyToWorld(world, ground);
    }

    {
        boxShape = frCreateRectangle((frMaterial) { .density = 1.0f,
                                                    .friction = 0.5f },
                                     BOX_SIZE,
                                     BOX_SIZE);

        int towerCount = bodyCount / BOX_COUNT_PER_TOWER;

        frVector2 towerOrigin = {
            .x = groundPosition.x - (towerCount * BOX_SIZE),
            .y = (groundPosition.y - (0.5f * groundHeight)) - (0.5f * BOX_SIZE)
        };

        for (int i = 0; i < bodyCount; i++) {
            frVector2 boxPosition = {
                .x = towerOrigin.x
                     + ((i / BOX_COUNT_PER_TOWER) * (2.0f * BOX_SIZE)),
                .y = towerOrigin.y
                     - ((i % BOX_COUNT_PER_TOWER) * (BOX_SIZE + 0.1f))
            };

            frBody *box = frCreateBodyFromShape(FR_BODY_DYNAMIC,
                                                boxPosition,
                                                boxShape);

            frAddBodyToWorld(world, box);
        }
    }
}

DrawBench(Stacks) {
    BeginMode2D(camera);

    for (int i = 0, n = frGetBodyCountInWorld(world); i < n; i++) {
        frBody *body = frGetBodyInWorld(world, i);

        frVector2 bodyPosition = frGetBodyPosition(body);
        Color *bodyColor = frGetBodyUserData(body);

        if (bodyColor != NULL) frDrawBodyLines(body, 4.0f, *bodyColor);
        else {
            DrawTexturePro(boxRenderTexture.texture,
                           boxRectangle,
                           (Rectangle) { .x = frUnitsToPixels(bodyPosition.x),
                                         .y = frUnitsToPixels(bodyPosition.y),
                                         .width = frUnitsToPixels(BOX_SIZE),
                                         .height = frUnitsToPixels(BOX_SIZE) },
                           (Vector2) { .x = 0.5f * frUnitsToPixels(BOX_SIZE),
                                       .y = 0.5f * frUnitsToPixels(BOX_SIZE) },
                           RAD2DEG * frGetBodyAngle(body),
                           WHITE);
        }
    }

    EndMode2D();
}

UpdateBench(Stacks) {
    frUpdateWorld(world, DELTA_TIME);
}

DeinitBench(Stacks) {
    frReleaseShape(boxShape);
    frReleaseShape(groundShape);

    frReleaseWorld(world);
}
