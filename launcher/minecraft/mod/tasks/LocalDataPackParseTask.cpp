// SPDX-FileCopyrightText: 2022 Rachel Powers <508861+Ryex@users.noreply.github.com>
//
// SPDX-License-Identifier: GPL-3.0-only

/*
 *  ALLauncher - Minecraft Launcher
 *  Copyright (C) 2022 Rachel Powers <508861+Ryex@users.noreply.github.com>
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

#include "LocalDataPackParseTask.h"

#include "FileSystem.h"
#include "Json.h"
#include "minecraft/mod/ResourcePack.h"

#include <quazip/quazip.h>
#include <quazip/quazipdir.h>
#include <quazip/quazipfile.h>

#include <QCryptographicHash>

namespace DataPackUtils {

bool process(DataPack* pack, ProcessingLevel level)
{
    switch (pack->type()) {
        case ResourceType::FOLDER:
            return DataPackUtils::processFolder(pack, level);
        case ResourceType::ZIPFILE:
            return DataPackUtils::processZIP(pack, level);
        default:
            qWarning() << "Invalid type for data pack parse task!";
            return false;
    }
}

bool processFolder(DataPack* pack, ProcessingLevel level)
{
    Q_ASSERT(pack->type() == ResourceType::FOLDER);

    auto mcmeta_invalid = [&pack]() {
        qWarning() << "Data pack at" << pack->fileinfo().filePath() << "does not have a valid pack.mcmeta";
        return false;  // the mcmeta is not optional
    };

    QFileInfo mcmeta_file_info(FS::PathCombine(pack->fileinfo().filePath(), "pack.mcmeta"));
    if (mcmeta_file_info.exists() && mcmeta_file_info.isFile()) {
        QFile mcmeta_file(mcmeta_file_info.filePath());
        if (!mcmeta_file.open(QIODevice::ReadOnly))
            return mcmeta_invalid();  // can't open mcmeta file

        auto data = mcmeta_file.readAll();

        bool mcmeta_result = DataPackUtils::processMCMeta(pack, std::move(data));

        mcmeta_file.close();
        if (!mcmeta_result) {
            return mcmeta_invalid();  // mcmeta invalid
        }
    } else {
        return mcmeta_invalid();  // mcmeta file isn't a valid file
    }

    QFileInfo data_dir_info(FS::PathCombine(pack->fileinfo().filePath(), pack->directory()));
    if (!data_dir_info.exists() || !data_dir_info.isDir()) {
        return false;  // data dir does not exists or isn't valid
    }

    if (level == ProcessingLevel::BasicInfoOnly) {
        return true;  // only need basic info already checked
    }
    auto png_invalid = [&pack]() {
        qWarning() << "Data pack at" << pack->fileinfo().filePath() << "does not have a valid pack.png";
        return true;  // the png is optional
    };

    QFileInfo image_file_info(FS::PathCombine(pack->fileinfo().filePath(), "pack.png"));
    if (image_file_info.exists() && image_file_info.isFile()) {
        QFile pack_png_file(image_file_info.filePath());
        if (!pack_png_file.open(QIODevice::ReadOnly))
            return png_invalid();  // can't open pack.png file

        auto data = pack_png_file.readAll();

        bool pack_png_result = DataPackUtils::processPackPNG(pack, std::move(data));

        pack_png_file.close();
        if (!pack_png_result) {
            return png_invalid();  // pack.png invalid
        }
    } else {
        return png_invalid();  // pack.png does not exists or is not a valid file.
    }

    return true;  // all tests passed
}

bool processZIP(DataPack* pack, ProcessingLevel level)
{
    Q_ASSERT(pack->type() == ResourceType::ZIPFILE);

    QuaZip zip(pack->fileinfo().filePath());
    if (!zip.open(QuaZip::mdUnzip))
        return false;  // can't open zip file

    QuaZipFile file(&zip);

    auto mcmeta_invalid = [&pack]() {
        qWarning() << "Data pack at" << pack->fileinfo().filePath() << "does not have a valid pack.mcmeta";
        return false;  // the mcmeta is not optional
    };

    if (zip.setCurrentFile("pack.mcmeta")) {
        if (!file.open(QIODevice::ReadOnly)) {
            qCritical() << "Failed to open file in zip.";
            zip.close();
            return mcmeta_invalid();
        }

        auto data = file.readAll();

        bool mcmeta_result = DataPackUtils::processMCMeta(pack, std::move(data));

        file.close();
        if (!mcmeta_result) {
            return mcmeta_invalid();  // mcmeta invalid
        }
    } else {
        return mcmeta_invalid();  // could not set pack.mcmeta as current file.
    }

    QuaZipDir zipDir(&zip);
    if (!zipDir.exists(pack->directory())) {
        return false;  // data dir does not exists at zip root
    }

    if (level == ProcessingLevel::BasicInfoOnly) {
        zip.close();
        return true;  // only need basic info already checked
    }

    auto png_invalid = [&pack]() {
        qWarning() << "Data pack at" << pack->fileinfo().filePath() << "does not have a valid pack.png";
        return true;  // the png is optional
    };

    if (zip.setCurrentFile("pack.png")) {
        if (!file.open(QIODevice::ReadOnly)) {
            qCritical() << "Failed to open file in zip.";
            zip.close();
            return png_invalid();
        }

        auto data = file.readAll();

        bool pack_png_result = DataPackUtils::processPackPNG(pack, std::move(data));

        file.close();
        zip.close();
        if (!pack_png_result) {
            return png_invalid();  // pack.png invalid
        }
    } else {
        zip.close();
        return png_invalid();  // could not set pack.mcmeta as current file.
    }
    zip.close();

    return true;
}

// https://minecraft.wiki/w/Data_pack#pack.mcmeta
// https://minecraft.wiki/w/Raw_JSON_text_format
// https://minecraft.wiki/w/Tutorials/Creating_a_resource_pack#Formatting_pack.mcmeta
bool processMCMeta(DataPack* pack, QByteArray&& raw_data)
{
    try {
        auto json_doc = QJsonDocument::fromJson(raw_data);
        auto pack_obj = Json::requireObject(json_doc.object(), "pack", {});

        pack->setPackFormat(Json::ensureInteger(pack_obj, "pack_format", 0));
        pack->setDescription(DataPackUtils::processComponent(pack_obj.value("description")));
    } catch (Json::JsonException& e) {
        qWarning() << "JsonException: " << e.what() << e.cause();
        return false;
    }
    return true;
}

QString buildStyle(const QJsonObject& obj)
{
    QStringList styles;
    if (auto color = Json::ensureString(obj, "color"); !color.isEmpty()) {
        styles << QString("color: %1;").arg(color);
    }
    if (obj.contains("bold")) {
        QString weight = "normal";
        if (Json::ensureBoolean(obj, "bold", false)) {
            weight = "bold";
        }
        styles << QString("font-weight: %1;").arg(weight);
    }
    if (obj.contains("italic")) {
        QString style = "normal";
        if (Json::ensureBoolean(obj, "italic", false)) {
            style = "italic";
        }
        styles << QString("font-style: %1;").arg(style);
    }

    return styles.isEmpty() ? "" : QString("style=\"%1\"").arg(styles.join(" "));
}

QString processComponent(const QJsonArray& value, bool strikethrough, bool underline)
{
    QString result;
    for (auto current : value)
        result += processComponent(current, strikethrough, underline);
    return result;
}

QString processComponent(const QJsonObject& obj, bool strikethrough, bool underline)
{
    underline = Json::ensureBoolean(obj, "underlined", underline);
    strikethrough = Json::ensureBoolean(obj, "strikethrough", strikethrough);

    QString result = Json::ensureString(obj, "text");
    if (underline) {
        result = QString("<u>%1</u>").arg(result);
    }
    if (strikethrough) {
        result = QString("<s>%1</s>").arg(result);
    }
    // the extra needs to be a array
    result += processComponent(Json::ensureArray(obj, "extra"), strikethrough, underline);
    if (auto style = buildStyle(obj); !style.isEmpty()) {
        result = QString("<span %1>%2</span>").arg(style, result);
    }
    if (obj.contains("clickEvent")) {
        auto click_event = Json::ensureObject(obj, "clickEvent");
        auto action = Json::ensureString(click_event, "action");
        auto value = Json::ensureString(click_event, "value");
        if (action == "open_url" && !value.isEmpty()) {
            result = QString("<a href=\"%1\">%2</a>").arg(value, result);
        }
    }
    return result;
}

QString processComponent(const QJsonValue& value, bool strikethrough, bool underline)
{
    if (value.isString()) {
        return value.toString();
    }
    if (value.isBool()) {
        return value.toBool() ? "true" : "false";
    }
    if (value.isDouble()) {
        return QString::number(value.toDouble());
    }
    if (value.isArray()) {
        return processComponent(value.toArray(), strikethrough, underline);
    }
    if (value.isObject()) {
        return processComponent(value.toObject(), strikethrough, underline);
    }
    qWarning() << "Invalid component type!";
    return {};
}

bool processPackPNG(const DataPack* pack, QByteArray&& raw_data)
{
    auto img = QImage::fromData(raw_data);
    if (!img.isNull()) {
        pack->setImage(img);
    } else {
        qWarning() << "Failed to parse pack.png.";
        return false;
    }
    return true;
}

bool processPackPNG(const DataPack* pack)
{
    auto png_invalid = [&pack]() {
        qWarning() << "Data pack at" << pack->fileinfo().filePath() << "does not have a valid pack.png";
        return false;
    };

    switch (pack->type()) {
        case ResourceType::FOLDER: {
            QFileInfo image_file_info(FS::PathCombine(pack->fileinfo().filePath(), "pack.png"));
            if (image_file_info.exists() && image_file_info.isFile()) {
                QFile pack_png_file(image_file_info.filePath());
                if (!pack_png_file.open(QIODevice::ReadOnly))
                    return png_invalid();  // can't open pack.png file

                auto data = pack_png_file.readAll();

                bool pack_png_result = DataPackUtils::processPackPNG(pack, std::move(data));

                pack_png_file.close();
                if (!pack_png_result) {
                    return png_invalid();  // pack.png invalid
                }
            } else {
                return png_invalid();  // pack.png does not exists or is not a valid file.
            }
            return false;  // not processed correctly; https://github.com/ALLauncher/ALLauncher/issues/1740
        }
        case ResourceType::ZIPFILE: {
            QuaZip zip(pack->fileinfo().filePath());
            if (!zip.open(QuaZip::mdUnzip))
                return false;  // can't open zip file

            QuaZipFile file(&zip);
            if (zip.setCurrentFile("pack.png")) {
                if (!file.open(QIODevice::ReadOnly)) {
                    qCritical() << "Failed to open file in zip.";
                    zip.close();
                    return png_invalid();
                }

                auto data = file.readAll();

                bool pack_png_result = DataPackUtils::processPackPNG(pack, std::move(data));

                file.close();
                if (!pack_png_result) {
                    return png_invalid();  // pack.png invalid
                }
            } else {
                return png_invalid();  // could not set pack.mcmeta as current file.
            }
            return false;  // not processed correctly; https://github.com/ALLauncher/ALLauncher/issues/1740
        }
        default:
            qWarning() << "Invalid type for data pack parse task!";
            return false;
    }
}

bool validate(QFileInfo file)
{
    DataPack dp{ file };
    return DataPackUtils::process(&dp, ProcessingLevel::BasicInfoOnly) && dp.valid();
}

bool validateResourcePack(QFileInfo file)
{
    ResourcePack rp{ file };
    return DataPackUtils::process(&rp, ProcessingLevel::BasicInfoOnly) && rp.valid();
}

}  // namespace DataPackUtils

LocalDataPackParseTask::LocalDataPackParseTask(int token, DataPack* dp) : Task(false), m_token(token), m_data_pack(dp) {}

void LocalDataPackParseTask::executeTask()
{
    if (!DataPackUtils::process(m_data_pack)) {
        emitFailed("process failed");
        return;
    }

    emitSucceeded();
}