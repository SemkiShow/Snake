// SPDX-FileCopyrightText: 2024 SemkiShow
//
// SPDX-License-Identifier: MIT

#pragma once

#include <raylib.h>
#include <string>

extern bool funMode;
extern int funModeLevel;
extern int scale;
extern Color appleColor;
extern int applesNumber;
extern bool noSpeedLimit;
extern bool autoMode;
extern bool vsync;
extern float audioVolume;

void Save(std::string fileName);
void Load(std::string fileName);
