// SPDX-FileCopyrightText: 2023 flowln <flowlnlnln@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-only AND Apache-2.0
/*
 *  ALLauncher - Minecraft Launcher
 *  Copyright (C) 2022 Sefa Eyeoglu <contact@scrumplex.net>
 *  Copyright (C) 2022 TheKodeToad <TheKodeToad@proton.me>
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

#include <ui/pages/modplatform/DataPackPage.h>
#include "Application.h"

#include "modplatform/ResourceAPI.h"

#include "ui/pages/modplatform/ModPage.h"
#include "ui/pages/modplatform/ResourcePackPage.h"
#include "ui/pages/modplatform/ShaderPackPage.h"
#include "ui/pages/modplatform/TexturePackPage.h"

namespace ResourceDownload {

namespace Flame {
static inline QString displayName()
{
    return "CurseForge";
}
static inline QIcon icon()
{
    return APPLICATION->getThemedIcon("flame");
}
static inline QString id()
{
    return "curseforge";
}
static inline QString debugName()
{
    return "Flame";
}
static inline QString metaEntryBase()
{
    return "FlameMods";
}
}  // namespace Flame

class FlameModPage : public ModPage {
    Q_OBJECT

   public:
    static FlameModPage* create(ModDownloadDialog* dialog, BaseInstance& instance)
    {
        return ModPage::create<FlameModPage>(dialog, instance);
    }

    FlameModPage(ModDownloadDialog* dialog, BaseInstance& instance);
    ~FlameModPage() override = default;

    bool shouldDisplay() const override;

    inline auto displayName() const -> QString override { return Flame::displayName(); }
    inline auto icon() const -> QIcon override { return Flame::icon(); }
    inline auto id() const -> QString override { return Flame::id(); }
    inline auto debugName() const -> QString override { return Flame::debugName(); }
    inline auto metaEntryBase() const -> QString override { return Flame::metaEntryBase(); }

    inline auto helpPage() const -> QString override { return "Mod-platform"; }

    void openUrl(const QUrl& url) override;
    std::unique_ptr<ModFilterWidget> createFilterWidget() override;

   protected:
    virtual void prepareProviderCategories() override;

   private:
    Task::Ptr m_categoriesTask;
};

class FlameResourcePackPage : public ResourcePackResourcePage {
    Q_OBJECT

   public:
    static FlameResourcePackPage* create(ResourcePackDownloadDialog* dialog, BaseInstance& instance)
    {
        return ResourcePackResourcePage::create<FlameResourcePackPage>(dialog, instance);
    }

    FlameResourcePackPage(ResourcePackDownloadDialog* dialog, BaseInstance& instance);
    ~FlameResourcePackPage() override = default;

    bool shouldDisplay() const override;

    inline auto displayName() const -> QString override { return Flame::displayName(); }
    inline auto icon() const -> QIcon override { return Flame::icon(); }
    inline auto id() const -> QString override { return Flame::id(); }
    inline auto debugName() const -> QString override { return Flame::debugName(); }
    inline auto metaEntryBase() const -> QString override { return Flame::metaEntryBase(); }

    inline auto helpPage() const -> QString override { return ""; }

    void openUrl(const QUrl& url) override;
};

class FlameTexturePackPage : public TexturePackResourcePage {
    Q_OBJECT

   public:
    static FlameTexturePackPage* create(TexturePackDownloadDialog* dialog, BaseInstance& instance)
    {
        return TexturePackResourcePage::create<FlameTexturePackPage>(dialog, instance);
    }

    FlameTexturePackPage(TexturePackDownloadDialog* dialog, BaseInstance& instance);
    ~FlameTexturePackPage() override = default;

    bool shouldDisplay() const override;

    inline auto displayName() const -> QString override { return Flame::displayName(); }
    inline auto icon() const -> QIcon override { return Flame::icon(); }
    inline auto id() const -> QString override { return Flame::id(); }
    inline auto debugName() const -> QString override { return Flame::debugName(); }
    inline auto metaEntryBase() const -> QString override { return Flame::metaEntryBase(); }

    inline auto helpPage() const -> QString override { return ""; }

    void openUrl(const QUrl& url) override;
};

class FlameShaderPackPage : public ShaderPackResourcePage {
    Q_OBJECT

   public:
    static FlameShaderPackPage* create(ShaderPackDownloadDialog* dialog, BaseInstance& instance)
    {
        return ShaderPackResourcePage::create<FlameShaderPackPage>(dialog, instance);
    }

    FlameShaderPackPage(ShaderPackDownloadDialog* dialog, BaseInstance& instance);
    ~FlameShaderPackPage() override = default;

    bool shouldDisplay() const override;

    inline auto displayName() const -> QString override { return Flame::displayName(); }
    inline auto icon() const -> QIcon override { return Flame::icon(); }
    inline auto id() const -> QString override { return Flame::id(); }
    inline auto debugName() const -> QString override { return Flame::debugName(); }
    inline auto metaEntryBase() const -> QString override { return Flame::metaEntryBase(); }

    inline auto helpPage() const -> QString override { return ""; }

    void openUrl(const QUrl& url) override;
};



class FlameDataPackPage : public DataPackResourcePage {
    Q_OBJECT

   public:
    static FlameDataPackPage* create(DataPackDownloadDialog* dialog, BaseInstance& instance)
    {
        return DataPackResourcePage::create<FlameDataPackPage>(dialog, instance);
    }

    FlameDataPackPage(DataPackDownloadDialog* dialog, BaseInstance& instance);
    ~FlameDataPackPage() override = default;

    bool shouldDisplay() const override;

    inline auto displayName() const -> QString override { return Flame::displayName(); }
    inline auto icon() const -> QIcon override { return Flame::icon(); }
    inline auto id() const -> QString override { return Flame::id(); }
    inline auto debugName() const -> QString override { return Flame::debugName(); }
    inline auto metaEntryBase() const -> QString override { return Flame::metaEntryBase(); }

    inline auto helpPage() const -> QString override { return ""; }

    void openUrl(const QUrl& url) override;
};

}  // namespace ResourceDownload
