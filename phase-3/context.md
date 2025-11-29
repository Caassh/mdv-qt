# Phase 3: Markdown Editor Implementation - Implementation Context

## Status
[TO BE COMPLETED IN THIS PHASE]

## Key Components to Implement

### Dual Mode Editor Component
- View Mode: Formatted document appearance (default)
- Edit Mode: Raw markdown editing
- Split View: Editor + Preview simultaneously
- Seamless transitions between modes

### Core Editor Classes
```cpp
// EditorManager (C++ backend)
class EditorManager : public QObject {
    Q_OBJECT
    
public:
    explicit EditorManager(QObject *parent = nullptr);
    
    void setEditMode(const QString &filePath);
    void setViewMode(const QString &filePath);
    void setSplitMode(const QString &filePath);
    void toggleMode(const QString &filePath);
    
    EditorMode currentMode(const QString &filePath) const;
    QString currentContent(const QString &filePath) const;
    
    void setFormatting(const QString &filePath, FormattingType type);
    void insertElement(const QString &filePath, ElementType element);
    
signals:
    void modeChanged(const QString &filePath, EditorMode mode);
    void contentChanged(const QString &filePath, const QString &content);
    
private:
    QHash<QString, EditorMode> m_editorModes;
    QHash<QString, QString> m_editorContents;
    QHash<QString, QTextDocument*> m_documents;
};

// ElementType enum
enum class ElementType {
    HEADING_1,
    HEADING_2,
    HEADING_3,
    BOLD,
    ITALIC,
    CODE_BLOCK,
    LINK,
    IMAGE,
    LIST_BULLET,
    LIST_NUMBERED,
    BLOCKQUOTE,
    CODE_SPAN
};

// EditorMode enum
enum class EditorMode {
    VIEW_ONLY,
    EDIT_ONLY,
    SPLIT_VIEW
};
```

### QML Editor Component
- Custom text editor with syntax highlighting
- Rich text formatting toolbar
- Split view layout management
- Keyboard shortcut integration

## Implementation Strategy

### View Mode Implementation
- Default mode showing formatted markdown
- Clean document-like appearance
- Double-click anywhere to enter edit mode
- Smooth animations during transitions
- Read-only display with proper styling

### Edit Mode Implementation
- Raw markdown text editing
- Syntax highlighting for markdown syntax
- Line numbers and indentation guides
- Auto-completion for markdown elements
- Real-time parsing feedback

### Split View Mode
- Side-by-side editor and preview
- Synchronized scrolling between views
- Real-time content updates
- Adjustable split ratio
- Consistent styling between views

### Rich Text Features
1. **Formatting Toolbar**
   - Bold, italic, underline
   - Headings (H1, H2, H3)
   - Lists (bulleted, numbered)
   - Code blocks with language selection
   - Links and images

2. **Keyboard Shortcuts**
   - Ctrl+B: Bold
   - Ctrl+I: Italic
   - Ctrl+H: Heading
   - Ctrl+L: Link
   - Ctrl+K: Code block
   - Ctrl+Q: Blockquote

3. **Advanced Editing Features**
   - Code block language selection dropdown
   - Live syntax highlighting
   - Auto-indentation
   - Bracket matching
   - Block-level editing (reordering, duplication, deletion)

### Code Block Implementation
- Custom language selection dropdown (all popular languages)
- Syntax highlighting using highlight.js
- Line numbering option
- Copy button for code blocks
- Collapsible code blocks

### Block-Level Editing
- Drag and drop repositioning of elements
- Duplicate/remove blocks
- Convert between block types
- Indent/outdent for nested structures

## Architecture Pattern

### Communication Flow
1. QML Editor component receives user input
2. Input is processed by EditorManager
3. Changes are sent to MarkdownRenderer for preview
4. Content is updated in DocumentManager
5. Changes are synchronized across views

### State Management
- Editor mode per document
- Undo/redo stack per document
- Selection and cursor position tracking
- View synchronization state

## Technical Implementation Details

### Text Editing Components
1. **QTextEdit** for raw markdown editing
   - Custom QSyntaxHighlighter for markdown
   - Proper undo/redo functionality
   - Line numbering support

2. **QWebEngineView** for formatted preview
   - Real-time content updates
   - Custom CSS injection
   - JavaScript interaction support

3. **Custom Widgets** for specialized elements
   - Code block editor with language selector
   - Image uploader component
   - Link insertion dialog

### Synchronization Mechanism
- Scroll synchronization between views
- Cursor position tracking in both views
- Selection highlighting coordination
- Real-time content updates

### Performance Optimization
- Efficient content update algorithms
- Asynchronous rendering
- Caching for unchanged content
- Virtual scrolling for large documents

## iOS-7 Design Integration
- Toolbar with subtle iOS-7 styling
- Clean, minimalist editing interface
- Purple accent color (#724f97) for active elements
- Proper spacing and typography
- Smooth transition animations

## Integration Points
- Connect to DocumentManager for document handling
- Integrate with MarkdownRenderer for live preview
- Work with TabManager for multi-document support
- Use with FileExplorer for file operations
- Integrate with SettingsManager for preferences

## Keyboard Navigation Support
- Arrow keys for navigation
- Ctrl+Tab for document switching
- Ctrl+Plus/Minus for zoom
- Custom shortcuts for formatting

## Testing Approach
- Unit tests for editing operations
- Integration tests for mode switching
- Performance tests with large documents
- Cross-platform UI consistency tests

## Files to Create
- EditorManager.cpp/h
- MarkdownEditor.qml (main QML component)
- FormattingToolbar.qml
- CodeBlockEditor.qml
- SplitViewManager.qml
- KeyboardShortcuts.qml
- Test files for editing functionality