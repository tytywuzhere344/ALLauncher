#!/bin/bash

# Revert name replacements in all files
echo "Reverting all instances of 'ALLauncher' to 'ALLauncher' in files..."
find . -type f -not -path "./.git/*" -exec sed -i 's/ALLauncher/ALLauncher/g' {} +

# Revert filenames
echo "Reverting filenames..."
find . -depth -name "*ALLauncher*" | while read fname; do
    newname=$(echo "$fname" | sed 's/ALLauncher/ALLauncher/g')
    mv "$fname" "$newname"
done

# Revert directory names
echo "Reverting directory names..."
find . -depth -type d -name "*ALLauncher*" | while read dname; do
    newd=$(echo "$dname" | sed 's/ALLauncher/ALLauncher/g')
    mv "$dname" "$newd"
done

echo "✅ Reversion complete."
