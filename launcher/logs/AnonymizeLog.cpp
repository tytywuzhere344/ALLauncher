// SPDX-License-Identifier: GPL-3.0-only
/*
 *  ALLauncher - Minecraft Launcher
 *  Copyright (c) 2025 Trial97 <alexandru.tripon97@gmail.com>
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
#include "AnonymizeLog.h"

#include <QRegularExpression>

struct RegReplace {
    RegReplace(QRegularExpression r, QString w) : reg(r), with(w) { reg.optimize(); }
    QRegularExpression reg;
    QString with;
};

static const QVector<RegReplace> anonymizeRules = {
    RegReplace(QRegularExpression("C:\\\\Users\\\\([^\\\\]+)\\\\", QRegularExpression::CaseInsensitiveOption),
               "C:\\Users\\********\\"),  // windows
    RegReplace(QRegularExpression("C:\\/Users\\/([^\\/]+)\\/", QRegularExpression::CaseInsensitiveOption),
               "C:/Users/********/"),  // windows with forward slashes
    RegReplace(QRegularExpression("(?<!\\\\w)\\/home\\/[^\\/]+\\/", QRegularExpression::CaseInsensitiveOption),
               "/home/********/"),  // linux
    RegReplace(QRegularExpression("(?<!\\\\w)\\/Users\\/[^\\/]+\\/", QRegularExpression::CaseInsensitiveOption),
               "/Users/********/"),  // macos
    RegReplace(QRegularExpression("\\(Session ID is [^\\)]+\\)", QRegularExpression::CaseInsensitiveOption),
               "(Session ID is <SESSION_TOKEN>)"),  // SESSION_TOKEN
    RegReplace(QRegularExpression("new refresh token: \"[^\"]+\"", QRegularExpression::CaseInsensitiveOption),
               "new refresh token: \"<TOKEN>\""),  // refresh token
    RegReplace(QRegularExpression("\"device_code\" :  \"[^\"]+\"", QRegularExpression::CaseInsensitiveOption),
               "\"device_code\" :  \"<DEVICE_CODE>\""),  // device code
};

void anonymizeLog(QString& log)
{
    for (auto rule : anonymizeRules) {
        log.replace(rule.reg, rule.with);
    }
}