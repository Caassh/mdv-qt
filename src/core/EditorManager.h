// EditorManager.h
#ifndef EDITORMANAGER_H
#define EDITORMANAGER_H

#include <QObject>
#include <QHash>
#include <QTextDocument>

enum class EditorMode {
    VIEW_ONLY,
    EDIT_ONLY,
    SPLIT_VIEW
};

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

class EditorManager : public QObject
{
    Q_OBJECT
    
public:
    explicit EditorManager(QObject *parent = nullptr);
    
    void setEditMode(const QString &filePath);
    void setViewMode(const QString &filePath);
    void setSplitMode(const QString &filePath);
    void toggleMode(const QString &filePath);
    
    EditorMode currentMode(const QString &filePath) const;
    QString currentContent(const QString &filePath) const;
    
    void setFormatting(const QString &filePath, ElementType type);
    void insertElement(const QString &filePath, ElementType element);
    
    bool isDocumentModified(const QString &filePath) const;
    void setDocumentContent(const QString &filePath, const QString &content);
    
    QStringList openDocuments() const;
    void closeDocument(const QString &filePath);
    
signals:
    void modeChanged(const QString &filePath, EditorMode mode);
    void contentChanged(const QString &filePath, const QString &content);
    void documentClosed(const QString &filePath);
    
private:
    QHash<QString, EditorMode> m_editorModes;
    QHash<QString, QString> m_editorContents;
    QHash<QString, bool> m_modifiedStatus;
    QHash<QString, QTextDocument*> m_documents;
    
    void updateModifiedStatus(const QString &filePath);
};

#endif // EDITORMANAGER_H