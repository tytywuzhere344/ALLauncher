// SPDX-License-Identifier: GPL-3.0-only
/*
 *  ALLauncher - Minecraft Launcher
 *  Copyright (c) 2022 flowln <flowlnlnln@gmail.com>
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

#include <QFileInfo>
#include "Screenshot.h"
#include "net/NetRequest.h"

class ImgurUpload : public Net::NetRequest {
   public:
    class Sink : public Net::Sink {
       public:
        Sink(ScreenShot::Ptr shot) : m_shot(shot) {};
        virtual ~Sink() = default;

       public:
        auto init(QNetworkRequest& request) -> Task::State override;
        auto write(QByteArray& data) -> Task::State override;
        auto abort() -> Task::State override;
        auto finalize(QNetworkReply& reply) -> Task::State override;
        auto hasLocalData() -> bool override { return false; }

       private:
        ScreenShot::Ptr m_shot;
        QByteArray m_output;
    };
    ImgurUpload(QFileInfo info) : m_fileInfo(info) {}
    virtual ~ImgurUpload() = default;

    static NetRequest::Ptr make(ScreenShot::Ptr m_shot);

   private:
    virtual QNetworkReply* getReply(QNetworkRequest&) override;
    const QFileInfo m_fileInfo;
};
