// DocumentLinker.h
#ifndef DOCUMENTLINKER_H
#define DOCUMENTLINKER_H

#include <QObject>
#include <QStack>
#include <QHash>
#include <QList>
#include <QDateTime>

class DocumentLinker : public QObject
{
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

#endif // DOCUMENTLINKER_H