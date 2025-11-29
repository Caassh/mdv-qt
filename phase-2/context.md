# Phase 2: Markdown Rendering Engine - Implementation Context

## Status
[TO BE COMPLETED IN THIS PHASE]

## Key Components to Implement

### MarkdownRenderer Class
- Responsible for: Markdown parsing, HTML generation, styling
- Integrates with: cmark or discount library, KaTeX, highlight.js
- Output: HTML content for QWebEngineView

**Header (MarkdownRenderer.h):**
```cpp
class MarkdownRenderer : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString htmlContent READ htmlContent NOTIFY htmlContentChanged)
    Q_PROPERTY(bool mathEnabled READ mathEnabled WRITE setMathEnabled NOTIFY mathEnabledChanged)
    Q_PROPERTY(bool gfmEnabled READ gfmEnabled WRITE setGfmEnabled NOTIFY gfmEnabledChanged)

public:
    explicit MarkdownRenderer(QObject *parent = nullptr);
    
    QString renderMarkdown(const QString &markdown) const;
    QString htmlContent() const { return m_htmlContent; }
    
    bool mathEnabled() const { return m_mathEnabled; }
    void setMathEnabled(bool enabled);
    
    bool gfmEnabled() const { return m_gfmEnabled; }
    void setGfmEnabled(bool enabled);
    
    void setBaseUrl(const QUrl &baseUrl);
    QUrl baseUrl() const { return m_baseUrl; }
    
public slots:
    void processMarkdown(const QString &markdown);
    void setCodeBlockTheme(const QString &theme);

signals:
    void htmlContentChanged();
    void mathEnabledChanged();
    void gfmEnabledChanged();
    void renderingError(const QString &error);

private:
    QString m_htmlContent;
    QString m_markdownContent;
    bool m_mathEnabled;
    bool m_gfmEnabled;
    QUrl m_baseUrl;
    QString m_codeBlockTheme;
    
    QString generateHtmlTemplate(const QString &bodyContent) const;
    QString applyCustomStyling(const QString &html) const;
    QString processCodeBlocks(const QString &html) const;
    QString resolveImagePaths(const QString &html) const;
    QString injectKaTeX(const QString &html) const;
    
    QString renderWithCmark(const QString &markdown) const;
    QString renderWithDiscount(const QString &markdown) const;
};
```

### HTML Template and Styling
- iOS-7 inspired design with white background and purple accents
- Responsive layout for different screen sizes
- Custom CSS for typography and spacing

### Web View Component
- QWebEngineView with custom CSS injection
- Scroll synchronization with editor
- Click-to-edit functionality

## Implementation Strategy

### Markdown Parsing Options
1. **cmark (reference implementation)**
   - Pros: Standard compliant, fast, secure
   - Cons: More complex to integrate

2. **discount (faster rendering)**
   - Pros: Fast, good GFM support
   - Cons: Less standard compliant

**Decision:** Use cmark for better standards compliance and security.

### GFM Support Implementation
- Tables with proper CSS styling
- Task lists with custom checkboxes
- Strikethrough formatting
- Fenced code blocks with language detection
- Autolinks and URL auto-detection

### Math Equation Support
- Integrate KaTeX for mathematical expressions
- Support both inline `$...$` and block `$$...$$` formats
- Proper font rendering for mathematical symbols

### Code Block Syntax Highlighting
- Use highlight.js for syntax highlighting
- Support all popular programming languages
- Theme support for different color schemes
- Line numbering options

### Image Handling
- Relative path resolution for local images
- Lazy loading for performance
- Responsive sizing for different screen dimensions
- Error handling for missing images

## iOS-7 Design Implementation

### Color Scheme
- Primary: #724f97 (purple accent)
- Background: #ffffff (white)
- Text: #2c3e50 (dark gray)
- Borders: #e0e0e0 (light gray)

### Typography
- System fonts with fallbacks
- Appropriate font weights and sizes
- Proper line height for readability
- Heading hierarchy with visual distinction

### Spacing and Layout
- Consistent padding and margins
- Responsive design principles
- Appropriate whitespace
- Grid-based alignment

## Performance Considerations
- Efficient markdown parsing with caching
- Asynchronous rendering to prevent UI blocking
- Memory management for large documents
- Image lazy loading implementation

## Security Considerations
- HTML sanitization to prevent XSS attacks
- Safe handling of external links
- Proper input validation

## Integration Points
- Connect to DocumentManager for document content
- Integrate with TabManager for display in tabs
- Work with Editor component for live preview
- Use with PDF export functionality

## Testing Approach
- Unit tests for markdown parsing
- Integration tests with web view
- Performance tests with large documents
- Cross-platform rendering consistency tests

## Files to Create
- MarkdownRenderer.cpp/h
- HTML template files with iOS-7 styling
- CSS files for custom styling
- JavaScript files for KaTeX and highlight.js integration
- Test files for various markdown scenarios