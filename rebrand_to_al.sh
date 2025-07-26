#!/bin/bash

echo "🔄 Replacing 'ALLauncher' with 'ALLauncher'..."

# Replace inside files
find . -type f -not -path "./.git/*" -exec sed -i 's/ALLauncher/ALLauncher/g' {} +
find . -type f -not -path "./.git/*" -exec sed -i 's/allauncher/allauncher/g' {} +

# Rename files and directories
echo "📁 Renaming files and directories..."
find . -depth -name '*ALLauncher*' | while read name; do
  newname="$(echo "$name" | sed 's/ALLauncher/ALLauncher/g')"
  mv "$name" "$newname"
done

find . -depth -name '*allauncher*' | while read name; do
  newname="$(echo "$name" | sed 's/allauncher/allauncher/g')"
  mv "$name" "$newname"
done

echo "✅ Done: All instances of 'ALLauncher' renamed to 'ALLauncher'"
