// SPDX-FileCopyrightText: 2023 flowln <flowlnlnln@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "ui/pages/modplatform/ResourcePage.h"
#include "ui/pages/modplatform/ShaderPackModel.h"

namespace Ui {
class ResourcePage;
}

namespace ResourceDownload {

class ShaderPackDownloadDialog;

class ShaderPackResourcePage : public ResourcePage {
    Q_OBJECT

   public:
    template <typename T>
    static T* create(ShaderPackDownloadDialog* dialog, BaseInstance& instance)
    {
        auto page = new T(dialog, instance);
        auto model = static_cast<ShaderPackResourceModel*>(page->getModel());

        connect(model, &ResourceModel::versionListUpdated, page, &ResourcePage::versionListUpdated);
        connect(model, &ResourceModel::projectInfoUpdated, page, &ResourcePage::updateUi);
        connect(model, &QAbstractListModel::modelReset, page, &ResourcePage::modelReset);

        return page;
    }

    //: The plural version of 'shader pack'
    inline QString resourcesString() const override { return tr("shader packs"); }
    //: The singular version of 'shader packs'
    inline QString resourceString() const override { return tr("shader pack"); }

    bool supportsFiltering() const override { return false; };

    void addResourceToPage(ModPlatform::IndexedPack::Ptr, ModPlatform::IndexedVersion&, std::shared_ptr<ResourceFolderModel>) override;

    QMap<QString, QString> urlHandlers() const override;

    inline auto helpPage() const -> QString override { return "shaderpack-platform"; }

   protected:
    ShaderPackResourcePage(ShaderPackDownloadDialog* dialog, BaseInstance& instance);

   protected slots:
    void triggerSearch() override;
};

}  // namespace ResourceDownload
