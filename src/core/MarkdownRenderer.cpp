// MarkdownRenderer.cpp
#include "MarkdownRenderer.h"
#include <QRegularExpression>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QMimeDatabase>
#include <QMimeType>

// For now, we'll use a simple approach with basic markdown conversion
// In a real implementation, we would link with cmark or discount library

MarkdownRenderer::MarkdownRenderer(QObject *parent)
    : QObject(parent)
    , m_mathEnabled(true)
    , m_gfmEnabled(true)
    , m_codeBlockTheme("default")
{
}

QString MarkdownRenderer::renderMarkdown(const QString &markdown) const
{
    // This is a simplified markdown renderer
    // In a real implementation, we would use cmark or discount library
    QString html = markdown;
    
    // Convert headers
    html.replace(QRegularExpression("^# (.*$)", QRegularExpression::MultilineOption), "<h1>\\1</h1>");
    html.replace(QRegularExpression("^## (.*$)", QRegularExpression::MultilineOption), "<h2>\\1</h2>");
    html.replace(QRegularExpression("^### (.*$)", QRegularExpression::MultilineOption), "<h3>\\1</h3>");
    
    // Convert bold and italic
    html.replace(QRegularExpression("\\*\\*(.*?)\\*\\*"), "<strong>\\1</strong>");
    html.replace(QRegularExpression("\\*(.*?)\\*"), "<em>\\1</em>");
    html.replace(QRegularExpression("__(.*?)__"), "<strong>\\1</strong>");
    html.replace(QRegularExpression("_(.*?)_"), "<em>\\1</em>");
    
    // Convert links
    html.replace(QRegularExpression("\\[([^\\]]+)\\]\\(([^\\)]+)\\)"), "<a href=\"\\2\">\\1</a>");
    
    // Convert images
    html.replace(QRegularExpression("!\\[([^\\]]*)\\]\\(([^\\)]+)\\)"), "<img src=\"\\2\" alt=\"\\1\">");
    
    // Convert code blocks
    html.replace(QRegularExpression("```(\\w+)?\\s*\\n(.*?)```", QRegularExpression::DotMatchesEverythingOption | QRegularExpression::MultilineOption),
                 "<pre><code class=\"language-\\1\">\\2</code></pre>");

    // Convert inline code
    html.replace(QRegularExpression("`([^`]+)`"), "<code>\\1</code>");

    // Convert unordered lists
    html.replace(QRegularExpression("^\\* (.*)$", QRegularExpression::MultilineOption),
                 "<li>\\1</li>");
    html.replace(QRegularExpression("(<li>.*</li>)", QRegularExpression::DotMatchesEverythingOption | QRegularExpression::MultilineOption),
                 "<ul>\\1</ul>");
    
    // Convert paragraphs
    QStringList lines = html.split("\n");
    QString result;
    bool inParagraph = false;
    
    for (const QString &line : lines) {
        if (line.startsWith("<") && line.endsWith(">")) {  // HTML tag
            if (inParagraph) {
                result += "</p>\n";
                inParagraph = false;
            }
            result += line + "\n";
        } else if (!line.trimmed().isEmpty()) {
            if (!inParagraph) {
                result += "<p>";
                inParagraph = true;
            }
            result += line + " ";
        } else {
            if (inParagraph) {
                result += "</p>\n";
                inParagraph = false;
            }
            result += "\n";
        }
    }
    
    if (inParagraph) {
        result += "</p>\n";
    }
    
    // Apply iOS-7 inspired styling
    result = applyCustomStyling(result);
    
    // Process code blocks for syntax highlighting
    result = processCodeBlocks(result);
    
    // Inject KaTeX if math is enabled
    if (m_mathEnabled) {
        result = injectKaTeX(result);
    }
    
    // Resolve image paths relative to base URL
    result = resolveImagePaths(result);
    
    return result;
}

void MarkdownRenderer::processMarkdown(const QString &markdown)
{
    m_markdownContent = markdown;
    QString html = renderMarkdown(markdown);
    
    if (m_htmlContent != html) {
        m_htmlContent = html;
        emit htmlContentChanged();
    }
}

void MarkdownRenderer::setMathEnabled(bool enabled)
{
    if (m_mathEnabled != enabled) {
        m_mathEnabled = enabled;
        emit mathEnabledChanged();
        
        // Re-render if content exists
        if (!m_markdownContent.isEmpty()) {
            processMarkdown(m_markdownContent);
        }
    }
}

void MarkdownRenderer::setGfmEnabled(bool enabled)
{
    if (m_gfmEnabled != enabled) {
        m_gfmEnabled = enabled;
        emit gfmEnabledChanged();
        
        // Re-render if content exists
        if (!m_markdownContent.isEmpty()) {
            processMarkdown(m_markdownContent);
        }
    }
}

void MarkdownRenderer::setBaseUrl(const QUrl &baseUrl)
{
    m_baseUrl = baseUrl;
}

void MarkdownRenderer::setCodeBlockTheme(const QString &theme)
{
    if (m_codeBlockTheme != theme) {
        m_codeBlockTheme = theme;
        
        // Re-render if content exists
        if (!m_markdownContent.isEmpty()) {
            processMarkdown(m_markdownContent);
        }
    }
}

QString MarkdownRenderer::generateHtmlTemplate(const QString &bodyContent) const
{
    QString templateStr = R"(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>Markdown Document</title>
    <style>
        body {
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Oxygen, Ubuntu, Cantarell, 'Open Sans', 'Helvetica Neue', sans-serif;
            line-height: 1.6;
            color: #2c3e50;
            max-width: 800px;
            margin: 0 auto;
            padding: 20px;
            background-color: #ffffff;
        }
        h1, h2, h3, h4, h5, h6 {
            color: #2c3e50;
            border-bottom: 1px solid #e0e0e0;
            padding-bottom: 0.3em;
        }
        h1 {
            font-size: 2em;
            margin-top: 0.8em;
        }
        h2 {
            font-size: 1.5em;
            margin-top: 1em;
        }
        h3 {
            font-size: 1.25em;
        }
        p {
            margin-top: 0;
            margin-bottom: 1em;
        }
        a {
            color: #724f97;
            text-decoration: none;
        }
        a:hover {
            text-decoration: underline;
        }
        code {
            background-color: #f8f9fa;
            padding: 0.2em 0.4em;
            border-radius: 3px;
            font-family: 'SFMono-Regular', Consolas, 'Liberation Mono', Menlo, monospace;
        }
        pre {
            background-color: #f8f9fa;
            padding: 1em;
            border-radius: 4px;
            overflow: auto;
            border: 1px solid #e0e0e0;
        }
        pre code {
            background: none;
            padding: 0;
        }
        blockquote {
            border-left: 4px solid #724f97;
            padding-left: 1em;
            margin-left: 0;
            color: #6c757d;
        }
        img {
            max-width: 100%;
            height: auto;
            border-radius: 4px;
        }
        table {
            border-collapse: collapse;
            width: 100%;
            margin: 1em 0;
        }
        th, td {
            border: 1px solid #e0e0e0;
            padding: 0.5em 1em;
        }
        th {
            background-color: #f8f9fa;
        }
        ul, ol {
            padding-left: 2em;
        }
        li {
            margin: 0.25em 0;
        }
        hr {
            border: none;
            height: 1px;
            background-color: #e0e0e0;
            margin: 1.5em 0;
        }
    </style>
)";

    if (m_mathEnabled) {
        templateStr += R"(
    <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/katex@0.16.0/dist/katex.min.css">
)";
    }

    templateStr += R"(
</head>
<body>
)";

    templateStr += bodyContent;

    if (m_mathEnabled) {
        templateStr += R"(
    <script src="https://cdn.jsdelivr.net/npm/katex@0.16.0/dist/katex.min.js"></script>
    <script>
        document.addEventListener("DOMContentLoaded", function() {
            var mathElements = document.querySelectorAll('.math');
            for (var i = 0; i < mathElements.length; i++) {
                var element = mathElements[i];
                var displayMode = element.classList.contains('display');
                katex.render(element.textContent, element, {
                    displayMode: displayMode,
                    throwOnError: false
                });
            }
        });
    </script>
)";
    }

    templateStr += R"(
</body>
</html>
)";

    return templateStr;
}

QString MarkdownRenderer::applyCustomStyling(const QString &html) const
{
    QString styledHtml = html;
    
    // Apply iOS-7 inspired styling with purple accents
    styledHtml = generateHtmlTemplate(styledHtml);
    
    return styledHtml;
}

QString MarkdownRenderer::processCodeBlocks(const QString &html) const
{
    QString processedHtml = html;
    
    // For now, we just add a class for syntax highlighting
    // In a real implementation, we would use highlight.js
    processedHtml.replace("<code class=\"language-", "<code class=\"language-");
    
    return processedHtml;
}

QString MarkdownRenderer::resolveImagePaths(const QString &html) const
{
    if (m_baseUrl.isEmpty()) {
        return html;
    }
    
    QString resolvedHtml = html;
    
    // Replace relative image paths with absolute paths
    QRegularExpression imgRegex(R"(<img\s+[^>]*src\s*=\s*["']([^"']+)["'][^>]*>)");
    QRegularExpressionMatchIterator iter = imgRegex.globalMatch(resolvedHtml);
    
    int offset = 0;
    while (iter.hasNext()) {
        QRegularExpressionMatch match = iter.next();
        QString imgTag = match.captured(0);
        QString src = match.captured(1);
        
        // Only resolve relative paths
        if (!src.startsWith("http") && !src.startsWith("/")) {
            QUrl resolvedUrl = m_baseUrl.resolved(QUrl(src));
            QString newSrc = resolvedUrl.toString();
            
            QString newImgTag = imgTag;
            newImgTag.replace(src, newSrc);
            
            resolvedHtml.replace(offset + match.capturedStart(), 
                                match.capturedLength(), newImgTag);
            
            // Update offset for next replacement
            offset += newImgTag.length() - imgTag.length();
        }
    }
    
    return resolvedHtml;
}

QString MarkdownRenderer::injectKaTeX(const QString &html) const
{
    QString katexHtml = html;
    
    // Convert $...$ to inline math and $$...$$ to display math
    // This is a simplified approach - a full implementation would be more complex
    katexHtml.replace(QRegularExpression(R"(\$\$(.*?)\$\$)"), 
                      R"(<span class="math display">\1</span>)");
    katexHtml.replace(QRegularExpression(R"(\$(.*?)\$)"), 
                      R"(<span class="math">\1</span>)");
    
    return katexHtml;
}

QString MarkdownRenderer::renderWithCmark(const QString &markdown) const
{
    // In a real implementation, we would call the cmark library here
    // For now, we use the simplified regex-based approach
    return renderMarkdown(markdown);
}

QString MarkdownRenderer::renderWithDiscount(const QString &markdown) const
{
    // In a real implementation, we would call the discount library here
    // For now, we use the simplified regex-based approach
    return renderMarkdown(markdown);
}