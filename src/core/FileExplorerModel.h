#ifndef FILEEXPLORERMODEL_H
#define FILEEXPLORERMODEL_H

#include <QFileSystemModel>
#include <QIcon>
#include <QDir>
#include <QMimeDatabase>
#include <QtWidgets/QFileIconProvider>

class FileExplorerModel : public QFileSystemModel
{
    Q_OBJECT

public:
    explicit FileExplorerModel(QObject *parent = nullptr);
    
    // Override flags to support drag and drop
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    
    // Custom data for file type icons
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    
    // Get file icon based on file type
    QIcon iconForFile(const QFileInfo &info) const;
    
    // Methods for file operations
    bool createFile(const QString &path, const QString &fileName);
    bool createDirectory(const QString &path, const QString &dirName);
    bool renameFile(const QString &oldPath, const QString &newPath);
    bool deleteFile(const QString &path);
    
    // Filter for markdown files
    void setMarkdownOnly(bool markdownOnly);
    bool isMarkdownOnly() const { return m_markdownOnly; }
    
signals:
    void fileOpened(const QString &filePath);
    void fileOperationError(const QString &error);

private:
    bool m_markdownOnly;
    QMimeDatabase m_mimeDatabase;
    QFileIconProvider m_iconProvider;
    
    bool isMarkdownFile(const QFileInfo &info) const;
};

#endif // FILEEXPLORERMODEL_H