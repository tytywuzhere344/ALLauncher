// SPDX-License-Identifier: GPL-3.0-only
/*
 *  ALLauncher - Minecraft Launcher
 *  Copyright (c) 2022 flowln <flowlnlnln@gmail.com>
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
 */

#pragma once

#include <QList>
#include <QMetaType>
#include <QString>
#include <QVariant>
#include <memory>

class QIODevice;

namespace ModPlatform {

enum ModLoaderType {
    NeoForge = 1 << 0,
    Forge = 1 << 1,
    Cauldron = 1 << 2,
    LiteLoader = 1 << 3,
    Fabric = 1 << 4,
    Quilt = 1 << 5,
    DataPack = 1 << 6,
    Babric = 1 << 7,
    BTA = 1 << 8
};
Q_DECLARE_FLAGS(ModLoaderTypes, ModLoaderType)
QList<ModLoaderType> modLoaderTypesToList(ModLoaderTypes flags);

enum class ResourceProvider { MODRINTH, FLAME };

enum class DependencyType { REQUIRED, OPTIONAL, INCOMPATIBLE, EMBEDDED, TOOL, INCLUDE, UNKNOWN };

enum class Side { NoSide = 0, ClientSide = 1 << 0, ServerSide = 1 << 1, UniversalSide = ClientSide | ServerSide };

namespace SideUtils {
QString toString(Side side);
Side fromString(QString side);
}  // namespace SideUtils

namespace ProviderCapabilities {
const char* name(ResourceProvider);
QString readableName(ResourceProvider);
QStringList hashType(ResourceProvider);
}  // namespace ProviderCapabilities

struct ModpackAuthor {
    QString name;
    QString url;
};

struct DonationData {
    QString id;
    QString platform;
    QString url;
};

struct IndexedVersionType {
    enum class VersionType { Release = 1, Beta, Alpha, Unknown };
    IndexedVersionType(const QString& type);
    IndexedVersionType(const IndexedVersionType::VersionType& type);
    IndexedVersionType(const IndexedVersionType& type);
    IndexedVersionType() : IndexedVersionType(IndexedVersionType::VersionType::Unknown) {}
    static const QString toString(const IndexedVersionType::VersionType& type);
    static IndexedVersionType::VersionType enumFromString(const QString& type);
    bool isValid() const { return m_type != IndexedVersionType::VersionType::Unknown; }
    IndexedVersionType& operator=(const IndexedVersionType& other);
    bool operator==(const IndexedVersionType& other) const { return m_type == other.m_type; }
    bool operator==(const IndexedVersionType::VersionType& type) const { return m_type == type; }
    bool operator!=(const IndexedVersionType& other) const { return m_type != other.m_type; }
    bool operator!=(const IndexedVersionType::VersionType& type) const { return m_type != type; }
    bool operator<(const IndexedVersionType& other) const { return m_type < other.m_type; }
    bool operator<(const IndexedVersionType::VersionType& type) const { return m_type < type; }
    bool operator<=(const IndexedVersionType& other) const { return m_type <= other.m_type; }
    bool operator<=(const IndexedVersionType::VersionType& type) const { return m_type <= type; }
    bool operator>(const IndexedVersionType& other) const { return m_type > other.m_type; }
    bool operator>(const IndexedVersionType::VersionType& type) const { return m_type > type; }
    bool operator>=(const IndexedVersionType& other) const { return m_type >= other.m_type; }
    bool operator>=(const IndexedVersionType::VersionType& type) const { return m_type >= type; }

    QString toString() const { return toString(m_type); }

    IndexedVersionType::VersionType m_type;
};

struct Dependency {
    QVariant addonId;
    DependencyType type;
    QString version;
};

struct IndexedVersion {
    QVariant addonId;
    QVariant fileId;
    QString version;
    QString version_number = {};
    IndexedVersionType version_type;
    QStringList mcVersion;
    QString downloadUrl;
    QString date;
    QString fileName;
    ModLoaderTypes loaders = {};
    QString hash_type;
    QString hash;
    bool is_preferred = true;
    QString changelog;
    QList<Dependency> dependencies;
    Side side;  // this is for flame API

    // For internal use, not provided by APIs
    bool is_currently_selected = false;
};

struct ExtraPackData {
    QList<DonationData> donate;

    QString issuesUrl;
    QString sourceUrl;
    QString wikiUrl;
    QString discordUrl;

    QString status;

    QString body;
};

struct IndexedPack {
    using Ptr = std::shared_ptr<IndexedPack>;

    QVariant addonId;
    ResourceProvider provider;
    QString name;
    QString slug;
    QString description;
    QList<ModpackAuthor> authors;
    QString logoName;
    QString logoUrl;
    QString websiteUrl;
    Side side;

    bool versionsLoaded = false;
    QList<IndexedVersion> versions;

    // Don't load by default, since some modplatform don't have that info
    bool extraDataLoaded = true;
    ExtraPackData extraData;

    // For internal use, not provided by APIs
    bool isVersionSelected(int index) const
    {
        if (!versionsLoaded)
            return false;

        return versions.at(index).is_currently_selected;
    }
    bool isAnyVersionSelected() const
    {
        if (!versionsLoaded)
            return false;

        return std::any_of(versions.constBegin(), versions.constEnd(), [](auto const& v) { return v.is_currently_selected; });
    }
};

struct OverrideDep {
    QString quilt;
    QString fabric;
    QString slug;
    ModPlatform::ResourceProvider provider;
};

inline auto getOverrideDeps() -> QList<OverrideDep>
{
    return { { "634179", "306612", "API", ModPlatform::ResourceProvider::FLAME },
             { "720410", "308769", "KotlinLibraries", ModPlatform::ResourceProvider::FLAME },

             { "qvIfYCYJ", "P7dR8mSH", "API", ModPlatform::ResourceProvider::MODRINTH },
             { "lwVhp9o5", "Ha28R6CL", "KotlinLibraries", ModPlatform::ResourceProvider::MODRINTH } };
}

QString getMetaURL(ResourceProvider provider, QVariant projectID);

auto getModLoaderAsString(ModLoaderType type) -> const QString;
auto getModLoaderFromString(QString type) -> ModLoaderType;

constexpr bool hasSingleModLoaderSelected(ModLoaderTypes l) noexcept
{
    auto x = static_cast<int>(l);
    return x && !(x & (x - 1));
}

struct Category {
    QString name;
    QString id;
};

}  // namespace ModPlatform

Q_DECLARE_METATYPE(ModPlatform::IndexedPack)
Q_DECLARE_METATYPE(ModPlatform::IndexedPack::Ptr)
Q_DECLARE_METATYPE(ModPlatform::ResourceProvider)
