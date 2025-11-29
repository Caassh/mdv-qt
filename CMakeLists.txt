cmake_minimum_required(VERSION 3.16)
project(mdviewer VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    Quick
    WebEngineCore
    WebEngineWidgets
    Qml
    QuickControls2
    PrintSupport
    Core5Compat
)

qt_standard_project_setup()

# Define source files
set(SOURCES
    src/main.cpp
    src/core/DocumentManager.cpp
    src/core/FileExplorerModel.cpp
    src/core/MarkdownRenderer.cpp
    src/core/EditorManager.cpp
    src/core/PdfExporter.cpp
    src/core/ThemeManager.cpp
    src/core/DocumentLinker.cpp
    src/core/TestFramework.cpp
)

set(HEADERS
    src/core/DocumentManager.h
    src/core/FileExplorerModel.h
    src/core/MarkdownRenderer.h
    src/core/EditorManager.h
    src/core/PdfExporter.h
    src/core/ThemeManager.h
    src/core/DocumentLinker.h
    src/core/TestFramework.h
)

# Create the executable
add_executable(mdviewer
    ${SOURCES}
    ${HEADERS}
)

# Link libraries
target_link_libraries(mdviewer
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Quick
    Qt6::WebEngineCore
    Qt6::Qml
    Qt6::QuickControls2
    Qt6::PrintSupport
)

# Add resources for QML files
qt_add_resources(mdviewer "qml_files" PREFIX "/" FILES
    src/application/Main.qml
    src/application/FileExplorer.qml
    src/application/TabManager.qml
    src/application/MarkdownEditor.qml
    src/application/TreeItemDelegate.qml
    src/application/RichTextEditor.qml
    src/application/StyledButton.qml
    src/application/StyledToolBar.qml
    src/application/DocumentHierarchyView.qml
)

# Install rules (optional)
install(TARGETS mdviewer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)