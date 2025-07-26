cmake_minimum_required(VERSION 3.15)
project(launcher Java)
find_package(Java 1.7 REQUIRED COMPONENTS Development)

include(UseJava)
set(CMAKE_JAVA_JAR_ENTRY_POINT org.allauncher.EntryPoint)
set(CMAKE_JAVA_COMPILE_FLAGS -target 7 -source 7)

set(SRC
    org/allauncher/EntryPoint.java
    org/allauncher/launcher/Launcher.java
    org/allauncher/launcher/impl/AbstractLauncher.java
    org/allauncher/launcher/impl/StandardLauncher.java
    org/allauncher/exception/ParameterNotFoundException.java
    org/allauncher/exception/ParseException.java
    org/allauncher/utils/Parameters.java
    org/allauncher/utils/ReflectionUtils.java
    org/allauncher/utils/logging/Level.java
    org/allauncher/utils/logging/Log.java
    org/allauncher/legacy/LegacyProxy.java
)

set(LEGACY_SRC
    legacy/org/allauncher/legacy/LegacyFrame.java
    legacy/org/allauncher/legacy/LegacyLauncher.java
    legacy/org/allauncher/legacy/fix/online/Handler.java
    legacy/org/allauncher/legacy/fix/online/OnlineFixes.java
    legacy/org/allauncher/legacy/fix/online/OnlineModeFix.java
    legacy/org/allauncher/legacy/fix/online/SkinFix.java
    legacy/org/allauncher/legacy/utils/Base64.java
    legacy/org/allauncher/legacy/utils/api/MojangApi.java
    legacy/org/allauncher/legacy/utils/api/Texture.java
    legacy/org/allauncher/legacy/utils/json/JsonParseException.java
    legacy/org/allauncher/legacy/utils/json/JsonParser.java
    legacy/org/allauncher/legacy/utils/url/ByteArrayUrlConnection.java
    legacy/org/allauncher/legacy/utils/url/UrlUtils.java
    legacy/net/minecraft/Launcher.java
    legacy/org/allauncher/legacy/LegacyProxy.java
)

add_jar(NewLaunch ${SRC})
add_jar(NewLaunchLegacy ${LEGACY_SRC} INCLUDE_JARS NewLaunch)
install_jar(NewLaunch "${JARS_DEST_DIR}")
install_jar(NewLaunchLegacy "${JARS_DEST_DIR}")
