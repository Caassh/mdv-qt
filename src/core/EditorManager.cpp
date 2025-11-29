// EditorManager.cpp
#include "EditorManager.h"
#include <QTextCursor>
#include <QTextBlock>

EditorManager::EditorManager(QObject *parent)
    : QObject(parent)
{
}

void EditorManager::setEditMode(const QString &filePath)
{
    m_editorModes[filePath] = EditorMode::EDIT_ONLY;
    emit modeChanged(filePath, EditorMode::EDIT_ONLY);
}

void EditorManager::setViewMode(const QString &filePath)
{
    m_editorModes[filePath] = EditorMode::VIEW_ONLY;
    emit modeChanged(filePath, EditorMode::VIEW_ONLY);
}

void EditorManager::setSplitMode(const QString &filePath)
{
    m_editorModes[filePath] = EditorMode::SPLIT_VIEW;
    emit modeChanged(filePath, EditorMode::SPLIT_VIEW);
}

void EditorManager::toggleMode(const QString &filePath)
{
    EditorMode current = m_editorModes.value(filePath, EditorMode::VIEW_ONLY);
    EditorMode newMode;
    
    switch (current) {
        case EditorMode::VIEW_ONLY:
            newMode = EditorMode::EDIT_ONLY;
            break;
        case EditorMode::EDIT_ONLY:
            newMode = EditorMode::SPLIT_VIEW;
            break;
        case EditorMode::SPLIT_VIEW:
            newMode = EditorMode::VIEW_ONLY;
            break;
    }
    
    m_editorModes[filePath] = newMode;
    emit modeChanged(filePath, newMode);
}

EditorMode EditorManager::currentMode(const QString &filePath) const
{
    return m_editorModes.value(filePath, EditorMode::VIEW_ONLY);
}

QString EditorManager::currentContent(const QString &filePath) const
{
    return m_editorContents.value(filePath);
}

void EditorManager::setFormatting(const QString &filePath, ElementType type)
{
    // In a real implementation, this would interact with the QTextDocument
    // to apply formatting to the selected text
    QString content = m_editorContents[filePath];
    
    switch (type) {
        case ElementType::BOLD:
            // Would apply bold formatting in real implementation
            break;
        case ElementType::ITALIC:
            // Would apply italic formatting in real implementation
            break;
        case ElementType::HEADING_1:
            content = "# " + content; // Simplified - would work on selection in real implementation
            break;
        case ElementType::HEADING_2:
            content = "## " + content; // Simplified - would work on selection in real implementation
            break;
        case ElementType::HEADING_3:
            content = "### " + content; // Simplified - would work on selection in real implementation
            break;
        case ElementType::CODE_BLOCK:
            content = "```\n" + content + "\n```"; // Simplified - would work on selection in real implementation
            break;
        case ElementType::LINK:
            content = "[link text](" + content + ")"; // Simplified - would work on selection in real implementation
            break;
        case ElementType::IMAGE:
            content = "![alt text](" + content + ")"; // Simplified - would work on selection in real implementation
            break;
        case ElementType::LIST_BULLET:
            content = "* " + content; // Simplified - would work on selection in real implementation
            break;
        case ElementType::LIST_NUMBERED:
            content = "1. " + content; // Simplified - would work on selection in real implementation
            break;
        case ElementType::BLOCKQUOTE:
            content = "> " + content; // Simplified - would work on selection in real implementation
            break;
        case ElementType::CODE_SPAN:
            content = "`" + content + "`"; // Simplified - would work on selection in real implementation
            break;
    }
    
    setDocumentContent(filePath, content);
}

void EditorManager::insertElement(const QString &filePath, ElementType element)
{
    setFormatting(filePath, element);
}

void EditorManager::setDocumentContent(const QString &filePath, const QString &content)
{
    if (m_editorContents[filePath] != content) {
        m_editorContents[filePath] = content;
        updateModifiedStatus(filePath);
        emit contentChanged(filePath, content);
    }
}

bool EditorManager::isDocumentModified(const QString &filePath) const
{
    return m_modifiedStatus.value(filePath, false);
}

QStringList EditorManager::openDocuments() const
{
    return m_editorContents.keys();
}

void EditorManager::closeDocument(const QString &filePath)
{
    m_editorModes.remove(filePath);
    m_editorContents.remove(filePath);
    m_modifiedStatus.remove(filePath);
    
    if (m_documents.contains(filePath)) {
        delete m_documents[filePath];
        m_documents.remove(filePath);
    }
    
    emit documentClosed(filePath);
}

void EditorManager::updateModifiedStatus(const QString &filePath)
{
    // In a real implementation, we would compare the current content with the saved version
    bool isModified = true; // Simplified logic
    if (m_modifiedStatus[filePath] != isModified) {
        m_modifiedStatus[filePath] = isModified;
    }
}