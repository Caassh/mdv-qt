# Phase 5: Advanced Features - Implementation Context

## Status
[TO BE COMPLETED IN THIS PHASE]

## Key Components to Implement

### PDF Export Functionality
- Convert rendered markdown view to PDF
- Page break support
- Print-style CSS for proper formatting
- Export options (page size, margins, etc.)
- Header and footer support
- Table of contents generation

**PDF Export Manager:**
```cpp
// PdfExporter (C++ backend)
class PdfExporter : public QObject {
    Q_OBJECT

public:
    explicit PdfExporter(QObject *parent = nullptr);
    
    enum PageSize {
        A4,
        Letter,
        Legal,
        Tabloid
    };
    Q_ENUM(PageSize)
    
    struct ExportOptions {
        PageSize pageSize = A4;
        qreal marginLeft = 20.0;  // mm
        qreal marginRight = 20.0; // mm
        qreal marginTop = 20.0;   // mm
        qreal marginBottom = 20.0; // mm
        bool includeHeaders = true;
        bool includeToc = false;
        QString headerText = "";
        QString footerText = "Page ${page} of ${pages}";
        QString title = "";
    };
    
    bool exportToPdf(const QString &htmlContent, const QString &outputPath, 
                     const ExportOptions &options = ExportOptions());
    
    bool exportDocumentToPdf(const QString &documentPath, const QString &outputPath,
                           const ExportOptions &options = ExportOptions());
    
    QStringList supportedPageSizes() const;
    
signals:
    void exportStarted();
    void exportProgress(int percentage);
    void exportCompleted(const QString &filePath);
    void exportError(const QString &error);

private:
    void setupPrinter(QPrinter *printer, const ExportOptions &options);
    QString generatePrintCss(const ExportOptions &options) const;
    QString injectPrintStyles(const QString &html, const ExportOptions &options) const;
};
```

### Subpages Implementation
- Nested pages/files support
- Internal linking between documents
- Navigation between related documents
- Hierarchical organization in explorer view

**Document Linking System:**
```cpp
// DocumentLinker (C++ backend)
class DocumentLinker : public QObject {
    Q_OBJECT

public:
    explicit DocumentLinker(QObject *parent = nullptr);
    
    struct LinkInfo {
        QString sourcePath;
        QString targetPath;
        QString linkText;
        int line; // Line number in source file
    };
    
    struct DocumentHierarchy {
        QString path;
        QString title;
        QList<DocumentHierarchy> children;
        QDateTime lastModified;
    };
    
    QList<LinkInfo> findInternalLinks(const QString &documentPath) const;
    QList<LinkInfo> findBacklinks(const QString &documentPath) const;
    DocumentHierarchy buildHierarchy(const QString &rootPath) const;
    
    QString resolveRelativeLink(const QString &sourcePath, const QString &link) const;
    bool createLink(const QString &sourcePath, const QString &targetPath);
    
    // Navigation history
    void addToHistory(const QString &path);
    QString navigateBack();
    QString navigateForward();
    bool canNavigateBack() const;
    bool canNavigateForward() const;
    
signals:
    void linksFound(const QList<LinkInfo> &links);
    void hierarchyChanged();
    void navigationHistoryChanged();
    
private:
    QStack<QString> m_navigationStack;
    int m_navigationIndex;
    QHash<QString, QList<LinkInfo>> m_forwardLinks;
    QHash<QString, QList<LinkInfo>> m_backLinks;
    
    void updateLinkCache(const QString &documentPath);
};
```

### Search and Navigation Features
- Full-text search within documents
- Find and replace functionality
- Document outline/TOC generation
- Quick navigation panel

## Implementation Strategy

### PDF Export Implementation

#### Technical Approach
1. **QWebEngineView to PDF Conversion**
   - Use QWebEnginePage::printToPdf() for direct conversion
   - Prepare HTML content with print-specific CSS
   - Handle page breaks and layout adjustments

2. **Print CSS Styling**
   - @media print rules for optimal printing
   - Page margins and orientation handling
   - Header/footer injection
   - Page numbering and TOC generation

3. **Export Options Implementation**
   - Page size selection (A4, Letter, etc.)
   - Customizable margins
   - Header/footer templates
   - Include/exclude options

#### Key Features
- High-resolution output
- Font embedding for consistent appearance
- Image embedding and scaling
- Table of contents generation
- Page numbering system

### Subpages Implementation

#### Hierarchical Organization
1. **Directory-Based Structure**
   - Organize documents in folders
   - Automatic hierarchy detection
   - Visual representation in file explorer

2. **Internal Linking System**
   - Detect and resolve relative links
   - Support for both file and anchor links
   - Broken link detection and highlighting

3. **Navigation Features**
   - Breadcrumb trail for hierarchy
   - Quick navigation panel
   - Back/forward navigation
   - Link preview on hover

#### Link Resolution
- Relative path calculation between documents
- Support for both absolute and relative links
- Error handling for broken links
- Validation of link integrity

### Search Implementation

#### Full-Text Search
1. **Indexing System**
   - Index all documents in workspace
   - Incremental updates when files change
   - Search result ranking

2. **Search Interface**
   - Global search across all documents
   - Search within current document
   - Advanced search options (regex, case-sensitive, etc.)

3. **Find and Replace**
   - In-document find/replace
   - Find all occurrences highlighting
   - Replace across multiple documents

### Document Outline Generation
- Extract headings from markdown
- Create interactive table of contents
- Synchronize with document scrolling
- Navigate by outline clicks

## Integration Points
- Connect PDF export to main menu
- Integrate subpages with file explorer
- Link search with document manager
- Connect navigation history with tab manager
- Integrate with editor for context-aware search

## Performance Considerations
- Efficient PDF generation without UI blocking
- Optimized link resolution algorithms
- Asynchronous search indexing
- Memory management for large document sets

## Cross-Platform Compatibility
- Platform-specific PDF handling
- Consistent output across platforms
- Proper file path handling
- Character encoding considerations

## Security Considerations
- Safe HTML rendering for PDF generation
- Sanitized user inputs for links
- Proper file access controls

## Testing Approach
- PDF export quality verification
- Link validation testing
- Search performance testing
- Cross-platform PDF compatibility
- Large document handling tests

## Files to Create
- PdfExporter.cpp/h
- DocumentLinker.cpp/h
- SearchManager.cpp/h
- QML components:
  - PdfExportDialog.qml
  - DocumentOutline.qml
  - LinkPreview.qml
  - SearchPanel.qml
  - NavigationHistory.qml
- Print CSS files
- Test files for PDF generation
- Test files for link validation