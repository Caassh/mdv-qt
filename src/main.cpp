// main.cpp
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDir>
#include <QStandardPaths>
#include <QtQml>
#include <QDebug>

#include "core/DocumentManager.h"
#include "core/FileExplorerModel.h"
#include "core/MarkdownRenderer.h"
#include "core/EditorManager.h"
#include "core/ThemeManager.h"
#include "core/DocumentLinker.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // Set application properties
    app.setApplicationName("MDV-Qt");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("MDV-Qt");
    
    // Create the QQmlApplicationEngine
    QQmlApplicationEngine engine;
    
    // Create core components
    DocumentManager *documentManager = new DocumentManager(&app);
    FileExplorerModel *fileSystemModel = new FileExplorerModel(&app);
    MarkdownRenderer *markdownRenderer = new MarkdownRenderer(&app);
    EditorManager *editorManager = new EditorManager(&app);
    ThemeManager *themeManager = new ThemeManager(&app);
    DocumentLinker *documentLinker = new DocumentLinker(&app);

    // Set up file system model
    fileSystemModel->setRootPath(QDir::homePath());
    fileSystemModel->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::AllDirs);
    fileSystemModel->setMarkdownOnly(true);

    // Expose core components to QML
    engine.rootContext()->setContextProperty("documentManager", documentManager);
    engine.rootContext()->setContextProperty("fileSystemModel", fileSystemModel);
    engine.rootContext()->setContextProperty("markdownRenderer", markdownRenderer);
    engine.rootContext()->setContextProperty("editorManager", editorManager);
    engine.rootContext()->setContextProperty("themeManager", themeManager);
    engine.rootContext()->setContextProperty("documentLinker", documentLinker);
    
    // Load the main QML file
    const QUrl url(QStringLiteral("qrc:/src/application/Main.qml"));
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
        [](const QUrl &url) {
            qDebug() << "Failed to load QML file:" << url;
            QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    if (engine.rootObjects().isEmpty()) {
        qDebug() << "No root objects created";
        return -1;
    }

    return app.exec();
}