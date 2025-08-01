// SPDX-FileCopyrightText: 2023 flowln <flowlnlnln@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <memory>
#include "modplatform/ResourceAPI.h"

class NetworkResourceAPI : public ResourceAPI {
   public:
    Task::Ptr searchProjects(SearchArgs&&, SearchCallbacks&&) const override;

    Task::Ptr getProject(QString addonId, std::shared_ptr<QByteArray> response) const override;

    Task::Ptr getProjectInfo(ProjectInfoArgs&&, ProjectInfoCallbacks&&) const override;
    Task::Ptr getProjectVersions(VersionSearchArgs&&, VersionSearchCallbacks&&) const override;
    Task::Ptr getDependencyVersion(DependencySearchArgs&&, DependencySearchCallbacks&&) const override;

   protected:
    virtual auto getSearchURL(SearchArgs const& args) const -> std::optional<QString> = 0;
    virtual auto getInfoURL(QString const& id) const -> std::optional<QString> = 0;
    virtual auto getVersionsURL(VersionSearchArgs const& args) const -> std::optional<QString> = 0;
    virtual auto getDependencyURL(DependencySearchArgs const& args) const -> std::optional<QString> = 0;
};
