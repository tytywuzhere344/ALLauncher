// SPDX-License-Identifier: GPL-3.0-only
/*
 *  ALLauncher - Minecraft Launcher
 *  Copyright (C) 2022 Sefa Eyeoglu <contact@scrumplex.net>
 *  Copyright (C) 2023 TheKodeToad <TheKodeToad@proton.me>
 *  Copyright (C) 2025 Yihe Li <winmikedows@hotmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, version 3.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * This file incorporates work covered by the following copyright and
 * permission notice:
 *
 *      Copyright 2013-2021 MultiMC Contributors
 *
 *      Licensed under the Apache License, Version 2.0 (the "License");
 *      you may not use this file except in compliance with the License.
 *      You may obtain a copy of the License at
 *
 *          http://www.apache.org/licenses/LICENSE-2.0
 *
 *      Unless required by applicable law or agreed to in writing, software
 *      distributed under the License is distributed on an "AS IS" BASIS,
 *      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *      See the License for the specific language governing permissions and
 *      limitations under the License.
 */

#pragma once
#include "Application.h"

#include <QList>
#include <QMessageBox>

namespace ShortcutUtils {
/// A struct to hold parameters for creating a shortcut
struct Shortcut {
    BaseInstance* instance;
    QString name;
    QString targetString;
    QWidget* parent = nullptr;
    QStringList extraArgs = {};
    QString iconKey = "";
    ShortcutTarget target;
};

/// Create an instance shortcut on the specified file path
bool createInstanceShortcut(const Shortcut& shortcut, const QString& filePath);

/// Create an instance shortcut on the desktop
bool createInstanceShortcutOnDesktop(const Shortcut& shortcut);

/// Create an instance shortcut in the Applications directory
bool createInstanceShortcutInApplications(const Shortcut& shortcut);

/// Create an instance shortcut in other directories
bool createInstanceShortcutInOther(const Shortcut& shortcut);

}  // namespace ShortcutUtils
