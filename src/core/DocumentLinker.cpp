// DocumentLinker.cpp
#include "DocumentLinker.h"
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QDir>
#include <QFileInfo>
#include <QDebug>

DocumentLinker::DocumentLinker(QObject *parent)
    : QObject(parent)
    , m_navigationIndex(0)
{
}

QList<DocumentLinker::LinkInfo> DocumentLinker::findInternalLinks(const QString &documentPath) const
{
    QList<LinkInfo> links;
    
    QFile file(documentPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return links;
    }
    
    QTextStream in(&file);
    QString content = in.readAll();
    file.close();
    
    // Regular expression to match markdown links [text](path)
    QRegularExpression linkRegex(R"(\[([^\]]+)\]\(([^)]+)\))");
    QRegularExpressionMatchIterator iter = linkRegex.globalMatch(content);
    
    int lineNum = 1;
    int pos = 0;
    
    // Calculate line numbers as we go
    QStringList lines = content.split('\n');
    int currentLineStart = 0;
    
    while (iter.hasNext()) {
        QRegularExpressionMatch match = iter.next();
        QString linkText = match.captured(1);
        QString linkPath = match.captured(2);
        
        // Find which line this match is on
        int matchPos = match.capturedStart();
        for (int i = 0; i < lines.size(); ++i) {
            int lineEnd = currentLineStart + lines[i].length() + 1; // +1 for newline
            if (matchPos >= currentLineStart && matchPos < lineEnd) {
                lineNum = i + 1;
                break;
            }
            currentLineStart = lineEnd;
        }
        
        // Only consider internal links (not external URLs)
        if (!linkPath.startsWith("http://") && !linkPath.startsWith("https://")) {
            LinkInfo linkInfo;
            linkInfo.sourcePath = documentPath;
            linkInfo.targetPath = resolveRelativeLink(documentPath, linkPath);
            linkInfo.linkText = linkText;
            linkInfo.line = lineNum;
            links.append(linkInfo);
        }
    }
    
    return links;
}

QList<DocumentLinker::LinkInfo> DocumentLinker::findBacklinks(const QString &documentPath) const
{
    QList<LinkInfo> backlinks;
    
    // In a full implementation, this would search all documents in the workspace
    // For now, we'll return an empty list
    // This would require maintaining an index of all documents and their links
    
    return backlinks;
}

DocumentLinker::DocumentHierarchy DocumentLinker::buildHierarchy(const QString &rootPath) const
{
    DocumentHierarchy root;
    root.path = rootPath;
    root.title = QFileInfo(rootPath).fileName();
    root.lastModified = QFileInfo(rootPath).lastModified();
    
    QDir dir(rootPath);
    QFileInfoList entries = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot, QDir::Name);
    
    for (const QFileInfo &entry : entries) {
        if (entry.isDir()) {
            DocumentHierarchy child = buildHierarchy(entry.filePath());
            root.children.append(child);
        } else if (entry.suffix().toLower() == "md") {
            DocumentHierarchy child;
            child.path = entry.filePath();
            child.title = entry.fileName();
            child.lastModified = entry.lastModified();
            // Don't add child.children since it's a file
            root.children.append(child);
        }
    }
    
    return root;
}

QString DocumentLinker::resolveRelativeLink(const QString &sourcePath, const QString &link) const
{
    QFileInfo sourceInfo(sourcePath);
    QDir sourceDir = sourceInfo.dir();
    
    // Handle absolute paths
    if (link.startsWith('/')) {
        // Absolute from root of file system or project
        return link;
    }
    
    // Handle relative paths
    return sourceDir.filePath(link);
}

bool DocumentLinker::createLink(const QString &sourcePath, const QString &targetPath)
{
    // In a real implementation, this would:
    // 1. Update the source document to include a link to the target
    // 2. Update link indices
    // 3. Emit signals for UI updates
    
    // For now, just return true
    return true;
}

void DocumentLinker::addToHistory(const QString &path)
{
    // Remove any forward history since we're adding a new path
    while (m_navigationStack.size() > m_navigationIndex) {
        m_navigationStack.pop();
    }
    
    m_navigationStack.push(path);
    m_navigationIndex = m_navigationStack.size();
    
    emit navigationHistoryChanged();
}

QString DocumentLinker::navigateBack()
{
    if (canNavigateBack()) {
        m_navigationIndex--;
        emit navigationHistoryChanged();
        return m_navigationStack[m_navigationIndex - 1]; // Return the new current page
    }
    return "";
}

QString DocumentLinker::navigateForward()
{
    if (canNavigateForward()) {
        m_navigationIndex++;
        emit navigationHistoryChanged();
        return m_navigationStack[m_navigationIndex - 1]; // Return the new current page
    }
    return "";
}

bool DocumentLinker::canNavigateBack() const
{
    return m_navigationIndex > 1; // Can go back if we're past the first item
}

bool DocumentLinker::canNavigateForward() const
{
    return m_navigationIndex < m_navigationStack.size(); // Can go forward if we're not at the last item
}

void DocumentLinker::updateLinkCache(const QString &documentPath)
{
    // In a full implementation, this would update the cached links for a document
    // and also update backlink references to this document
}