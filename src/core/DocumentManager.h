// DocumentManager.h
#ifndef DOCUMENTMANAGER_H
#define DOCUMENTMANAGER_H

#include <QObject>
#include <QHash>
#include <QTimer>
#include <QFileSystemWatcher>
#include <QSettings>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDir>

class DocumentManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString currentDocument READ currentDocument NOTIFY currentDocumentChanged)
    Q_PROPERTY(QStringList recentDocuments READ recentDocuments NOTIFY recentDocumentsChanged)

public:
    explicit DocumentManager(QObject *parent = nullptr);

    bool openDocument(const QString &filePath);
    bool saveDocument(const QString &filePath = "");
    bool saveDocumentAs(const QString &filePath);
    bool saveAllDocuments();
    void closeDocument(const QString &filePath);
    void newDocument();
    void saveCurrent();

    QString currentDocument() const;
    QStringList recentDocuments() const;

    bool isDocumentModified(const QString &filePath) const;
    QStringList getOpenDocuments() const;

    void updateDocumentContent(const QString &filePath, const QString &content);
    QString getDocumentContent(const QString &filePath) const;

    // Auto-save functionality
    void setAutoSaveEnabled(bool enabled);
    bool autoSaveEnabled() const;
    void setAutoSaveInterval(int seconds);

public slots:
    void autoSave();

signals:
    void documentOpened(const QString &filePath, const QString &content);
    void documentSaved(const QString &filePath);
    void documentClosed(const QString &filePath);
    void documentModified(const QString &filePath);
    void currentDocumentChanged();
    void recentDocumentsChanged();
    void errorOccurred(const QString &error);

private:
    QHash<QString, QString> m_documents;  // filePath -> content
    QHash<QString, bool> m_modifiedStatus;  // filePath -> isModified
    QHash<QString, QDateTime> m_lastSaved;  // filePath -> last save time
    QStringList m_recentDocuments;
    QString m_currentDocument;
    QTimer *m_autoSaveTimer;
    QFileSystemWatcher *m_fileWatcher;
    QSettings *m_settings;
    bool m_autoSaveEnabled;
    int m_autoSaveInterval;  // in seconds

    void updateRecentDocuments(const QString &filePath);
    void startWatchingFile(const QString &filePath);
    void stopWatchingFile(const QString &filePath);
    void createBackup(const QString &filePath);
    QString generateUntitledName() const;
    bool isUntitledDocument(const QString &filePath) const;
    QString getBackupPath(const QString &filePath) const;
    bool restoreFromBackup(const QString &filePath);
};

#endif // DOCUMENTMANAGER_H