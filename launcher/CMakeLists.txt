# ... (existing content)
qt_add_resources(LauncherResources launcher/resources/icons.qrc)

# Install desktop icons
install(DIRECTORY resources/icons/ DESTINATION share/icons/hicolor)