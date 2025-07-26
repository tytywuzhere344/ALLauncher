#!/bin/bash

echo "🔍 Searching and restoring metadata URL to Prism Launcher..."

# Define the domain substitution
OLD_DOMAIN="meta.prismlauncher.org"
NEW_DOMAIN="meta.prismlauncher.org"

# Find and replace in all relevant text/code files (exclude build dirs and binaries)
find . \
  -type f \
  \( -name "*.cpp" -o -name "*.h" -o -name "*.json" -o -name "*.txt" -o -name "*.xml" -o -name "*.in" \) \
  ! -path "./build/*" \
  -exec grep -Il "$OLD_DOMAIN" {} \; \
  -exec sed -i "s|$OLD_DOMAIN|$NEW_DOMAIN|g" {} \;

echo "✅ All references to '$OLD_DOMAIN' have been replaced with '$NEW_DOMAIN'."
