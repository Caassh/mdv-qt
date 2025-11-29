#include "FileExplorerModel.h"
#include <QMimeType>
#include <QFile>
#include <QTextStream>
#include <QtWidgets/QMessageBox>
#include <QDir>
#include <QIcon>

FileExplorerModel::FileExplorerModel(QObject *parent)
    : QFileSystemModel(parent)
    , m_markdownOnly(false)
{
    setIconProvider(&m_iconProvider);
}

Qt::ItemFlags FileExplorerModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QFileSystemModel::flags(index);
    
    if (index.isValid()) {
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
    }
    return defaultFlags;
}

QVariant FileExplorerModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DecorationRole && index.column() == 0) {
        QFileInfo info = fileInfo(index);
        return iconForFile(info);
    }
    
    return QFileSystemModel::data(index, role);
}

QIcon FileExplorerModel::iconForFile(const QFileInfo &info) const
{
    if (info.isDir()) {
        return m_iconProvider.icon(QFileIconProvider::Folder);
    }

    if (isMarkdownFile(info)) {
        // Use a custom markdown icon or a text file icon
        return m_iconProvider.icon(QFileInfo());
    }

    // Use system icon for other file types
    return m_iconProvider.icon(QFileInfo());
}

bool FileExplorerModel::createFile(const QString &path, const QString &fileName)
{
    QString fullPath = path + "/" + fileName;
    if (QFile::exists(fullPath)) {
        emit fileOperationError("File already exists: " + fullPath);
        return false;
    }
    
    QFile file(fullPath);
    if (file.open(QIODevice::WriteOnly)) {
        file.close();
        // Reset the model to refresh data
        beginResetModel();
        endResetModel();
        return true;
    }
    
    emit fileOperationError("Could not create file: " + fullPath);
    return false;
}

bool FileExplorerModel::createDirectory(const QString &path, const QString &dirName)
{
    QString fullPath = path + "/" + dirName;
    QDir dir(path);
    
    if (dir.mkdir(dirName)) {
        // Reset the model to refresh data
        beginResetModel();
        endResetModel();
        return true;
    }
    
    emit fileOperationError("Could not create directory: " + fullPath);
    return false;
}

bool FileExplorerModel::renameFile(const QString &oldPath, const QString &newPath)
{
    if (QFile::exists(newPath)) {
        emit fileOperationError("File already exists: " + newPath);
        return false;
    }
    
    if (QFile::rename(oldPath, newPath)) {
        // Reset the model to refresh data
        beginResetModel();
        endResetModel();
        return true;
    }
    
    emit fileOperationError("Could not rename file: " + oldPath);
    return false;
}

bool FileExplorerModel::deleteFile(const QString &path)
{
    QFileInfo fileInfo(path);
    bool success = false;
    
    if (fileInfo.isDir()) {
        QDir dir(path);
        success = dir.removeRecursively();
    } else {
        success = QFile::remove(path);
    }
    
    if (success) {
        // Reset the model to refresh data
        beginResetModel();
        endResetModel();
        return true;
    }
    
    emit fileOperationError("Could not delete: " + path);
    return false;
}

void FileExplorerModel::setMarkdownOnly(bool markdownOnly)
{
    m_markdownOnly = markdownOnly;
    if (m_markdownOnly) {
        setNameFilters(QStringList() << "*.md" << "*.markdown");
        setNameFilterDisables(false);
    } else {
        setNameFilterDisables(true);
    }
}

bool FileExplorerModel::isMarkdownFile(const QFileInfo &info) const
{
    if (!info.isFile()) return false;
    
    QString suffix = info.suffix().toLower();
    return (suffix == "md" || suffix == "markdown");
}