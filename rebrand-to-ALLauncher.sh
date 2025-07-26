#!/bin/bash

echo "🔁 Rebranding Prism Launcher to ALLauncher..."

# Set the base directory
BASE_DIR=$(pwd)

# 1. Replace text in source files
echo "🔍 Replacing names in code files..."
find "$BASE_DIR" -type f \( -name "*.cpp" -o -name "*.h" -o -name "*.ui" -o -name "*.ts" -o -name "*.desktop" -o -name "*.xml" -o -name "CMakeLists.txt" \) \
    -exec sed -i 's/Prism Launcher/ALLauncher/g' {} +
find "$BASE_DIR" -type f -exec sed -i 's/ALLauncher/ALLauncher/g' {} +

# 2. Rename desktop file (if it exists)
if [ -f "$BASE_DIR/launcher/launcher/prism-launcher.desktop" ]; then
    mv "$BASE_DIR/launcher/launcher/prism-launcher.desktop" "$BASE_DIR/launcher/launcher/allauncher.desktop"
fi

# 3. Rename AppData (if it exists)
if [ -f "$BASE_DIR/launcher/launcher/org.allauncher.ALLauncher.appdata.xml" ]; then
    mv "$BASE_DIR/launcher/launcher/org.allauncher.ALLauncher.appdata.xml" "$BASE_DIR/launcher/launcher/org.allauncher.ALLauncher.appdata.xml"
fi

# 4. Replace icon (optional)
ICON_SOURCE="$HOME/Downloads/allauncher-icon.png"
ICON_DEST="$BASE_DIR/resources/icons/allauncher-icon.png"

if [ -f "$ICON_SOURCE" ]; then
    echo "🎨 Copying new icon..."
    mkdir -p "$(dirname "$ICON_DEST")"
    cp "$ICON_SOURCE" "$ICON_DEST"
fi

echo "✅ Rebranding complete! You can now rebuild ALLauncher."
