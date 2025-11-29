# MDV-Qt - Markdown Viewer

A cross-platform markdown viewer application built with Qt6, featuring an iOS-7 inspired UI design and advanced editing capabilities.

## Features

- **Dual Mode Editing**: View formatted markdown documents or edit raw markdown
- **Split View**: Edit and preview simultaneously 
- **File System Explorer**: VS Code-style directory tree with file operations
- **Tabbed Interface**: Multiple document handling with tab persistence
- **Rich Text Editing**: Formatting toolbar with headings, lists, links, and code blocks
- **Advanced Markdown Support**: GitHub Flavored Markdown, math equations with KaTeX
- **Syntax Highlighting**: Color-coded code blocks for popular programming languages
- **File Management**: Save, auto-save, and file change detection
- **PDF Export**: Convert documents to PDF with customizable layouts
- **Document Linking**: Internal linking between documents and navigation
- **iOS-7 Inspired UI**: Clean, minimalist interface with purple accents (#724f97)
- **Cross Platform**: Works on Windows, macOS, and Linux

## Installation

### Prerequisites

- Qt6 with the following components:
  - Core, Gui, Widgets
  - Quick, QML, QuickControls2
  - WebEngine and WebEngineWidgets
  - PrintSupport
  - Core5Compat

### Arch Linux

```bash
# Install Qt6 dependencies
sudo pacman -S qt6-base qt6-tools qt6-webengine cmake gcc make

# Clone and build
git clone <repository-url>
cd mdv-qt
./build.sh
```

### Other Platforms

1. Install Qt6 with required components
2. Configure with CMake
3. Build with your preferred build system

## Building

```bash
# Create build directory
mkdir build && cd build

# Configure the project
cmake .. -DCMAKE_PREFIX_PATH=/path/to/qt6

# Build the application
cmake --build . --parallel

# Run the application
./mdviewer
```

Or use the provided build script:
```bash
./build.sh
```

## Usage

1. **Opening Files**: Use the "Open" button or navigate in the file explorer
2. **Editing**: Double-click on document to enter edit mode, or use "Edit" button
3. **Viewing**: Default mode shows formatted document appearance
4. **Split View**: Toggle "Split" to see both raw and formatted content
5. **Format Text**: Use the toolbar to apply headings, bold, italic, code, etc.
6. **Export PDF**: Use "Export PDF" button in the toolbar

## Keyboard Shortcuts

- `Ctrl+B` - Bold
- `Ctrl+I` - Italic
- `Ctrl+H` - Heading
- `Ctrl+L` - Link
- `Ctrl+K` - Code block
- `Ctrl+Q` - Blockquote

## Architecture

- **C++ Backend**: Core business logic and file operations
- **QML Frontend**: Modern UI components with native feel
- **Model-View-Controller**: Clean separation of concerns
- **Signal/Slot Communication**: Safe communication between components

## Project Structure

```
mdv-qt/
├── src/
│   ├── core/          # C++ business logic
│   └── application/   # QML UI components
├── assets/            # Images, icons, and CSS
├── build.sh          # Build script
├── CMakeLists.txt    # Build configuration
└── README.md         # This file
```

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add tests if applicable
5. Submit a pull request

## License

[Specify your license here]