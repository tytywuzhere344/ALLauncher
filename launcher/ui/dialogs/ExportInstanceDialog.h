// SPDX-License-Identifier: GPL-3.0-only
/*
 *  ALLauncher - Minecraft Launcher
 *  Copyright (C) 2023 TheKodeToad <TheKodeToad@proton.me>
 *  Copyright (c) 2023 Trial97 <alexandru.tripon97@gmail.com>
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

#include <QDialog>
#include <QModelIndex>
#include <memory>
#include "FastFileIconProvider.h"
#include "FileIgnoreProxy.h"

class BaseInstance;
using InstancePtr = std::shared_ptr<BaseInstance>;

namespace Ui {
class ExportInstanceDialog;
}

class ExportInstanceDialog : public QDialog {
    Q_OBJECT

   public:
    explicit ExportInstanceDialog(InstancePtr instance, QWidget* parent = 0);
    ~ExportInstanceDialog();

    virtual void done(int result);

   private:
    void doExport();
    QString ignoreFileName();

   private:
    Ui::ExportInstanceDialog* m_ui;
    InstancePtr m_instance;
    FileIgnoreProxy* m_proxyModel;
    FastFileIconProvider m_icons;

   private slots:
    void rowsInserted(QModelIndex parent, int top, int bottom);
};
