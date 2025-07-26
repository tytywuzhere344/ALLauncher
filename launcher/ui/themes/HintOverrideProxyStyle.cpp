// SPDX-License-Identifier: GPL-3.0-only
/*
 *  ALLauncher - Minecraft Launcher
 *  Copyright (C) 2024 TheKodeToad <TheKodeToad@proton.me>
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
 */

#include "HintOverrideProxyStyle.h"

HintOverrideProxyStyle::HintOverrideProxyStyle(QStyle* style) : QProxyStyle(style)
{
    setObjectName(baseStyle()->objectName());
}

int HintOverrideProxyStyle::styleHint(QStyle::StyleHint hint,
                                      const QStyleOption* option,
                                      const QWidget* widget,
                                      QStyleHintReturn* returnData) const
{
    if (hint == QStyle::SH_ItemView_ActivateItemOnSingleClick)
        return 0;

    if (hint == QStyle::SH_Slider_AbsoluteSetButtons)
        return Qt::LeftButton | Qt::MiddleButton;

    if (hint == QStyle::SH_Slider_PageSetButtons)
        return Qt::RightButton;

    return QProxyStyle::styleHint(hint, option, widget, returnData);
}
