// SPDX-License-Identifier: GPL-3.0-only
/*
 *  ALLauncher - Minecraft Launcher
 *  Copyright (C) 2023 Rachel Powers <508861+Ryex@users.noreply.github.com>
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
 */

#include "net/Logging.h"

Q_LOGGING_CATEGORY(taskNetLogC, "launcher.task.net")
Q_LOGGING_CATEGORY(taskDownloadLogC, "launcher.task.net.download")
Q_LOGGING_CATEGORY(taskUploadLogC, "launcher.task.net.upload")
Q_LOGGING_CATEGORY(taskMCSkinsLogC, "launcher.task.minecraft.skins")
Q_LOGGING_CATEGORY(taskMetaCacheLogC, "launcher.task.net.metacache")
Q_LOGGING_CATEGORY(taskHttpMetaCacheLogC, "launcher.task.net.metacache.http")
