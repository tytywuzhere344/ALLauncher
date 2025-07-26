cmake_minimum_required(VERSION 3.16)
project(ALLauncher)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable necessary Qt modules
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui Network)

# Subdirectories
add_subdirectory(launcher)

# Install launcher icons (added patch)
install(DIRECTORY launcher/resources/icons/ DESTINATION share/icons/hicolor)