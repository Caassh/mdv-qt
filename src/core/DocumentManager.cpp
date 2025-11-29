// DocumentManager.cpp
#include "DocumentManager.h"
#include <QDir>
#include <QStandardPaths>
#include <QFileInfo>
#include <QDebug>

DocumentManager::DocumentManager(QObject *parent)
    : QObject(parent)
    , m_autoSaveEnabled(true)
    , m_autoSaveInterval(3) // 3 seconds
{
    m_autoSaveTimer = new QTimer(this);
    m_autoSaveTimer->setInterval(m_autoSaveInterval * 1000);
    connect(m_autoSaveTimer, &QTimer::timeout, this, &DocumentManager::autoSave);
    
    m_fileWatcher = new QFileSystemWatcher(this);
    connect(m_fileWatcher, &QFileSystemWatcher::fileChanged, [this](const QString &path) {
        // Handle external file changes
        // For now, just emit a signal
        emit errorOccurred("File changed externally: " + path);
    });
    
    m_settings = new QSettings(this);
    
    // Load recent documents
    m_recentDocuments = m_settings->value("recentDocuments", QStringList()).toStringList();
    
    // Start auto-save timer if enabled
    if (m_autoSaveEnabled) {
        m_autoSaveTimer->start();
    }
}

bool DocumentManager::openDocument(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        emit errorOccurred("Could not open file: " + file.errorString());
        return false;
    }

    QTextStream in(&file);
    QString content = in.readAll();
    file.close();

    m_documents[filePath] = content;
    m_modifiedStatus[filePath] = false;
    m_currentDocument = filePath;
    
    // Update recent documents
    updateRecentDocuments(filePath);
    
    // Start watching this file for external changes
    startWatchingFile(filePath);
    
    emit documentOpened(filePath, content);
    emit currentDocumentChanged();
    
    return true;
}

bool DocumentManager::saveDocument(const QString &filePath)
{
    QString path = filePath.isEmpty() ? m_currentDocument : filePath;
    
    if (path.isEmpty()) {
        emit errorOccurred("No file path specified for save");
        return false;
    }
    
    if (!m_documents.contains(path)) {
        emit errorOccurred("Document not loaded: " + path);
        return false;
    }
    
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        emit errorOccurred("Could not save file: " + file.errorString());
        return false;
    }
    
    QTextStream out(&file);
    out << m_documents[path];
    file.close();
    
    m_modifiedStatus[path] = false;
    m_lastSaved[path] = QDateTime::currentDateTime();
    
    // Create backup after successful save
    createBackup(path);
    
    emit documentSaved(path);
    return true;
}

bool DocumentManager::saveDocumentAs(const QString &filePath)
{
    if (m_currentDocument.isEmpty()) {
        emit errorOccurred("No document currently open");
        return false;
    }
    
    // Save content to new path
    m_documents[filePath] = m_documents[m_currentDocument];
    m_modifiedStatus[filePath] = false;
    m_lastSaved[filePath] = QDateTime::currentDateTime();
    
    // Move document from old path to new path
    if (m_currentDocument != filePath) {
        m_documents.remove(m_currentDocument);
        m_modifiedStatus.remove(m_currentDocument);
        m_lastSaved.remove(m_currentDocument);
        stopWatchingFile(m_currentDocument);
        
        m_currentDocument = filePath;
        
        // Start watching new file path
        startWatchingFile(filePath);
    }
    
    return saveDocument(filePath);
}

bool DocumentManager::saveAllDocuments()
{
    bool allSaved = true;
    for (const QString &path : m_documents.keys()) {
        if (m_modifiedStatus[path]) {
            if (!saveDocument(path)) {
                allSaved = false;
            }
        }
    }
    return allSaved;
}

void DocumentManager::closeDocument(const QString &filePath)
{
    QString path = filePath.isEmpty() ? m_currentDocument : filePath;
    
    if (m_documents.contains(path)) {
        // Stop watching file
        stopWatchingFile(path);
        
        m_documents.remove(path);
        m_modifiedStatus.remove(path);
        m_lastSaved.remove(path);
        
        if (m_currentDocument == path) {
            // Set current document to another open document, or empty if none
            if (!m_documents.isEmpty()) {
                m_currentDocument = m_documents.keys().first();
            } else {
                m_currentDocument = "";
            }
            emit currentDocumentChanged();
        }
        
        emit documentClosed(path);
    }
}

void DocumentManager::newDocument()
{
    QString untitledName = generateUntitledName();
    m_documents[untitledName] = "";
    m_modifiedStatus[untitledName] = true;
    m_currentDocument = untitledName;
    
    emit documentOpened(untitledName, "");
    emit currentDocumentChanged();
}

void DocumentManager::saveCurrent()
{
    saveDocument();
}

QString DocumentManager::currentDocument() const
{
    return m_currentDocument;
}

QStringList DocumentManager::recentDocuments() const
{
    return m_recentDocuments;
}

bool DocumentManager::isDocumentModified(const QString &filePath) const
{
    return m_modifiedStatus.value(filePath, false);
}

QStringList DocumentManager::getOpenDocuments() const
{
    return m_documents.keys();
}

void DocumentManager::updateDocumentContent(const QString &filePath, const QString &content)
{
    if (m_documents[filePath] != content) {
        m_documents[filePath] = content;
        m_modifiedStatus[filePath] = true;
        
        emit documentModified(filePath);
    }
}

QString DocumentManager::getDocumentContent(const QString &filePath) const
{
    return m_documents.value(filePath, "");
}

void DocumentManager::setAutoSaveEnabled(bool enabled)
{
    if (m_autoSaveEnabled != enabled) {
        m_autoSaveEnabled = enabled;
        if (enabled) {
            m_autoSaveTimer->start();
        } else {
            m_autoSaveTimer->stop();
        }
    }
}

bool DocumentManager::autoSaveEnabled() const
{
    return m_autoSaveEnabled;
}

void DocumentManager::setAutoSaveInterval(int seconds)
{
    if (m_autoSaveInterval != seconds && seconds > 0) {
        m_autoSaveInterval = seconds;
        if (m_autoSaveEnabled) {
            m_autoSaveTimer->setInterval(seconds * 1000);
        }
    }
}

void DocumentManager::autoSave()
{
    if (!m_autoSaveEnabled) return;
    
    for (const QString &path : m_documents.keys()) {
        if (m_modifiedStatus[path]) {
            // Save with auto-save flag to distinguish from user-initiated save
            if (QFileInfo(path).isFile()) {  // Only save if it's an actual file (not untitled document)
                QFile file(path);
                if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                    QTextStream out(&file);
                    out << m_documents[path];
                    file.close();
                    
                    m_modifiedStatus[path] = false;
                    m_lastSaved[path] = QDateTime::currentDateTime();
                    
                    // Create backup after auto-save
                    createBackup(path);
                    
                    qDebug() << "Auto-saved:" << path;
                }
            }
        }
    }
}

void DocumentManager::updateRecentDocuments(const QString &filePath)
{
    if (m_recentDocuments.contains(filePath)) {
        m_recentDocuments.removeAll(filePath);
    }
    
    m_recentDocuments.prepend(filePath);
    
    // Limit to 10 recent documents
    if (m_recentDocuments.size() > 10) {
        m_recentDocuments = m_recentDocuments.mid(0, 10);
    }
    
    m_settings->setValue("recentDocuments", m_recentDocuments);
    emit recentDocumentsChanged();
}

void DocumentManager::startWatchingFile(const QString &filePath)
{
    // Stop watching if already watching
    stopWatchingFile(filePath);
    
    // Add file to watcher
    m_fileWatcher->addPath(filePath);
}

void DocumentManager::stopWatchingFile(const QString &filePath)
{
    m_fileWatcher->removePath(filePath);
}

void DocumentManager::createBackup(const QString &filePath)
{
    if (!QFileInfo::exists(filePath)) return;
    
    QString backupPath = getBackupPath(filePath);
    QFile(filePath).copy(backupPath);
}

QString DocumentManager::generateUntitledName() const
{
    int counter = 1;
    QString name;
    do {
        name = "untitled_" + QString::number(counter) + ".md";
        counter++;
    } while (m_documents.contains(name));
    
    return name;
}

bool DocumentManager::isUntitledDocument(const QString &filePath) const
{
    return filePath.startsWith("untitled_");
}

QString DocumentManager::getBackupPath(const QString &filePath) const
{
    QFileInfo fileInfo(filePath);
    QString backupDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/backups";
    QDir().mkpath(backupDir);
    
    QString backupPath = backupDir + "/" + fileInfo.fileName() + ".backup";
    return backupPath;
}

bool DocumentManager::restoreFromBackup(const QString &filePath)
{
    QString backupPath = getBackupPath(filePath);
    if (QFile::exists(backupPath)) {
        QFile::copy(backupPath, filePath);
        return true;
    }
    return false;
}