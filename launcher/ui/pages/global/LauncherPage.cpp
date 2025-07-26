// SPDX-License-Identifier: GPL-3.0-only
/*
 *  ALLauncher - Minecraft Launcher
 *  Copyright (c) 2022 Jamie Mansfield <jmansfield@cadixdev.org>
 *  Copyright (c) 2022 dada513 <dada513@protonmail.com>
 *  Copyright (C) 2022 Tayou <git@tayou.org>
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

#include "LauncherPage.h"
#include "ui_LauncherPage.h"

#include <QDir>
#include <QFileDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <QTextCharFormat>

#include <FileSystem.h>
#include "Application.h"
#include "BuildConfig.h"
#include "DesktopServices.h"
#include "settings/SettingsObject.h"
#include "ui/themes/ITheme.h"
#include "ui/themes/ThemeManager.h"
#include "updater/ExternalUpdater.h"

#include <QApplication>
#include <QProcess>

// FIXME: possibly move elsewhere
enum InstSortMode {
    // Sort alphabetically by name.
    Sort_Name,
    // Sort by which instance was launched most recently.
    Sort_LastLaunch
};

enum InstRenamingMode {
    // Rename metadata only.
    Rename_Always,
    // Ask everytime.
    Rename_Ask,
    // Rename physical directory too.
    Rename_Never
};

LauncherPage::LauncherPage(QWidget* parent) : QWidget(parent), ui(new Ui::LauncherPage)
{
    ui->setupUi(this);

    ui->sortingModeGroup->setId(ui->sortByNameBtn, Sort_Name);
    ui->sortingModeGroup->setId(ui->sortLastLaunchedBtn, Sort_LastLaunch);

    loadSettings();

    ui->updateSettingsBox->setHidden(!APPLICATION->updater());
}

LauncherPage::~LauncherPage()
{
    delete ui;
}

bool LauncherPage::apply()
{
    applySettings();
    return true;
}

void LauncherPage::on_instDirBrowseBtn_clicked()
{
    QString raw_dir = QFileDialog::getExistingDirectory(this, tr("Instance Folder"), ui->instDirTextBox->text());

    // do not allow current dir - it's dirty. Do not allow dirs that don't exist
    if (!raw_dir.isEmpty() && QDir(raw_dir).exists()) {
        QString cooked_dir = FS::NormalizePath(raw_dir);
        if (FS::checkProblemticPathJava(QDir(cooked_dir))) {
            QMessageBox warning;
            warning.setText(
                tr("You're trying to specify an instance folder which\'s path "
                   "contains at least one \'!\'. "
                   "Java is known to cause problems if that is the case, your "
                   "instances (probably) won't start!"));
            warning.setInformativeText(
                tr("Do you really want to use this path? "
                   "Selecting \"No\" will close this and not alter your instance path."));
            warning.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            int result = warning.exec();
            if (result == QMessageBox::Ok) {
                ui->instDirTextBox->setText(cooked_dir);
            }
        } else if (DesktopServices::isFlatpak() && raw_dir.startsWith("/run/user")) {
            QMessageBox warning;
            warning.setText(tr("You're trying to specify an instance folder "
                               "which was granted temporarily via Flatpak.\n"
                               "This is known to cause problems. "
                               "After a restart the launcher might break, "
                               "because it will no longer have access to that directory.\n\n"
                               "Granting %1 access to it via Flatseal is recommended.")
                                .arg(BuildConfig.LAUNCHER_DISPLAYNAME));
            warning.setInformativeText(tr("Do you want to proceed anyway?"));
            warning.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            int result = warning.exec();
            if (result == QMessageBox::Ok) {
                ui->instDirTextBox->setText(cooked_dir);
            }
        } else {
            ui->instDirTextBox->setText(cooked_dir);
        }
    }
}

void LauncherPage::on_iconsDirBrowseBtn_clicked()
{
    QString raw_dir = QFileDialog::getExistingDirectory(this, tr("Icons Folder"), ui->iconsDirTextBox->text());

    // do not allow current dir - it's dirty. Do not allow dirs that don't exist
    if (!raw_dir.isEmpty() && QDir(raw_dir).exists()) {
        QString cooked_dir = FS::NormalizePath(raw_dir);
        ui->iconsDirTextBox->setText(cooked_dir);
    }
}

void LauncherPage::on_modsDirBrowseBtn_clicked()
{
    QString raw_dir = QFileDialog::getExistingDirectory(this, tr("Mods Folder"), ui->modsDirTextBox->text());

    // do not allow current dir - it's dirty. Do not allow dirs that don't exist
    if (!raw_dir.isEmpty() && QDir(raw_dir).exists()) {
        QString cooked_dir = FS::NormalizePath(raw_dir);
        ui->modsDirTextBox->setText(cooked_dir);
    }
}

void LauncherPage::on_downloadsDirBrowseBtn_clicked()
{
    QString raw_dir = QFileDialog::getExistingDirectory(this, tr("Downloads Folder"), ui->downloadsDirTextBox->text());

    if (!raw_dir.isEmpty() && QDir(raw_dir).exists()) {
        QString cooked_dir = FS::NormalizePath(raw_dir);
        ui->downloadsDirTextBox->setText(cooked_dir);
    }
}

void LauncherPage::on_javaDirBrowseBtn_clicked()
{
    QString raw_dir = QFileDialog::getExistingDirectory(this, tr("Java Folder"), ui->javaDirTextBox->text());

    if (!raw_dir.isEmpty() && QDir(raw_dir).exists()) {
        QString cooked_dir = FS::NormalizePath(raw_dir);
        ui->javaDirTextBox->setText(cooked_dir);
    }
}

void LauncherPage::on_skinsDirBrowseBtn_clicked()
{
    QString raw_dir = QFileDialog::getExistingDirectory(this, tr("Skins Folder"), ui->skinsDirTextBox->text());

    // do not allow current dir - it's dirty. Do not allow dirs that don't exist
    if (!raw_dir.isEmpty() && QDir(raw_dir).exists()) {
        QString cooked_dir = FS::NormalizePath(raw_dir);
        ui->skinsDirTextBox->setText(cooked_dir);
    }
}

void LauncherPage::on_metadataEnableBtn_clicked()
{
    ui->metadataWarningLabel->setHidden(ui->metadataEnableBtn->isChecked());
}

void LauncherPage::applySettings()
{
    auto s = APPLICATION->settings();

    // Updates
    if (APPLICATION->updater()) {
        APPLICATION->updater()->setAutomaticallyChecksForUpdates(ui->autoUpdateCheckBox->isChecked());
        APPLICATION->updater()->setUpdateCheckInterval(ui->updateIntervalSpinBox->value() * 3600);
    }

    s->set("MenuBarInsteadOfToolBar", ui->preferMenuBarCheckBox->isChecked());

    s->set("NumberOfConcurrentTasks", ui->numberOfConcurrentTasksSpinBox->value());
    s->set("NumberOfConcurrentDownloads", ui->numberOfConcurrentDownloadsSpinBox->value());
    s->set("NumberOfManualRetries", ui->numberOfManualRetriesSpinBox->value());
    s->set("RequestTimeout", ui->timeoutSecondsSpinBox->value());

    // Console settings
    s->set("ConsoleMaxLines", ui->lineLimitSpinBox->value());
    s->set("ConsoleOverflowStop", ui->checkStopLogging->checkState() != Qt::Unchecked);

    // Folders
    // TODO: Offer to move instances to new instance folder.
    s->set("InstanceDir", ui->instDirTextBox->text());
    s->set("CentralModsDir", ui->modsDirTextBox->text());
    s->set("IconsDir", ui->iconsDirTextBox->text());
    s->set("DownloadsDir", ui->downloadsDirTextBox->text());
    s->set("SkinsDir", ui->skinsDirTextBox->text());
    s->set("JavaDir", ui->javaDirTextBox->text());
    s->set("DownloadsDirWatchRecursive", ui->downloadsDirWatchRecursiveCheckBox->isChecked());
    s->set("MoveModsFromDownloadsDir", ui->downloadsDirMoveCheckBox->isChecked());

    // Instance
    auto sortMode = (InstSortMode)ui->sortingModeGroup->checkedId();
    switch (sortMode) {
        case Sort_LastLaunch:
            s->set("InstSortMode", "LastLaunch");
            break;
        case Sort_Name:
        default:
            s->set("InstSortMode", "Name");
            break;
    }

    auto renamingMode = (InstRenamingMode)ui->renamingBehaviorComboBox->currentIndex();
    switch (renamingMode) {
        case Rename_Always:
            s->set("InstRenamingMode", "MetadataOnly");
            break;
        case Rename_Never:
            s->set("InstRenamingMode", "PhysicalDir");
            break;
        case Rename_Ask:
        default:
            s->set("InstRenamingMode", "AskEverytime");
            break;
    }

    // Mods
    s->set("ModMetadataDisabled", !ui->metadataEnableBtn->isChecked());
    s->set("ModDependenciesDisabled", !ui->dependenciesEnableBtn->isChecked());
    s->set("SkipModpackUpdatePrompt", !ui->modpackUpdatePromptBtn->isChecked());
}
void LauncherPage::loadSettings()
{
    auto s = APPLICATION->settings();
    // Updates
    if (APPLICATION->updater()) {
        ui->autoUpdateCheckBox->setChecked(APPLICATION->updater()->getAutomaticallyChecksForUpdates());
        ui->updateIntervalSpinBox->setValue(APPLICATION->updater()->getUpdateCheckInterval() / 3600);
    }

    ui->preferMenuBarCheckBox->setChecked(s->get("MenuBarInsteadOfToolBar").toBool());

    ui->numberOfConcurrentTasksSpinBox->setValue(s->get("NumberOfConcurrentTasks").toInt());
    ui->numberOfConcurrentDownloadsSpinBox->setValue(s->get("NumberOfConcurrentDownloads").toInt());
    ui->numberOfManualRetriesSpinBox->setValue(s->get("NumberOfManualRetries").toInt());
    ui->timeoutSecondsSpinBox->setValue(s->get("RequestTimeout").toInt());

    // Console settings
    ui->lineLimitSpinBox->setValue(s->get("ConsoleMaxLines").toInt());
    ui->checkStopLogging->setChecked(s->get("ConsoleOverflowStop").toBool());

    // Folders
    ui->instDirTextBox->setText(s->get("InstanceDir").toString());
    ui->modsDirTextBox->setText(s->get("CentralModsDir").toString());
    ui->iconsDirTextBox->setText(s->get("IconsDir").toString());
    ui->downloadsDirTextBox->setText(s->get("DownloadsDir").toString());
    ui->skinsDirTextBox->setText(s->get("SkinsDir").toString());
    ui->javaDirTextBox->setText(s->get("JavaDir").toString());
    ui->downloadsDirWatchRecursiveCheckBox->setChecked(s->get("DownloadsDirWatchRecursive").toBool());
    ui->downloadsDirMoveCheckBox->setChecked(s->get("MoveModsFromDownloadsDir").toBool());

    // Instance
    QString sortMode = s->get("InstSortMode").toString();
    if (sortMode == "LastLaunch") {
        ui->sortLastLaunchedBtn->setChecked(true);
    } else {
        ui->sortByNameBtn->setChecked(true);
    }

    QString renamingMode = s->get("InstRenamingMode").toString();
    InstRenamingMode renamingModeEnum;
    if (renamingMode == "MetadataOnly") {
        renamingModeEnum = Rename_Always;
    } else if (renamingMode == "PhysicalDir") {
        renamingModeEnum = Rename_Never;
    } else {
        renamingModeEnum = Rename_Ask;
    }
    ui->renamingBehaviorComboBox->setCurrentIndex(renamingModeEnum);

    // Mods
    ui->metadataEnableBtn->setChecked(!s->get("ModMetadataDisabled").toBool());
    ui->metadataWarningLabel->setHidden(ui->metadataEnableBtn->isChecked());
    ui->dependenciesEnableBtn->setChecked(!s->get("ModDependenciesDisabled").toBool());
    ui->modpackUpdatePromptBtn->setChecked(!s->get("SkipModpackUpdatePrompt").toBool());
}

void LauncherPage::retranslate()
{
    ui->retranslateUi(this);
}
