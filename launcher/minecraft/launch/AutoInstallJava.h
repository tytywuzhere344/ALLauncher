// SPDX-License-Identifier: GPL-3.0-only
/*
 *  ALLauncher - Minecraft Launcher
 *  Copyright (c) 2023-2024 Trial97 <alexandru.tripon97@gmail.com>
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

#include <launch/LaunchStep.h>
#include <launch/LaunchTask.h>
#include "meta/Version.h"
#include "minecraft/MinecraftInstance.h"
#include "tasks/Task.h"

class AutoInstallJava : public LaunchStep {
    Q_OBJECT

   public:
    explicit AutoInstallJava(LaunchTask* parent);
    ~AutoInstallJava() override = default;

    void executeTask() override;
    bool canAbort() const override { return m_current_task ? m_current_task->canAbort() : false; }
    bool abort() override;

   protected:
    void setJavaPath(QString path);
    void setJavaPathFromPartial();
    void downloadJava(Meta::Version::Ptr version, QString javaName);
    void tryNextMajorJava();

   private:
    MinecraftInstancePtr m_instance;
    Task::Ptr m_current_task;

    qsizetype m_majorJavaVersionIndex = 0;
    const QString m_supported_arch;
};
