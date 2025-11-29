// PdfExporter.h
#ifndef PDFEXPORTER_H
#define PDFEXPORTER_H

#include <QObject>
#include <QtPrintSupport/QPrinter>
#include <QWebEnginePage>
#include <QWebEngineProfile>
#include <QEventLoop>
#include <QTimer>

class PdfExporter : public QObject
{
    Q_OBJECT

public:
    explicit PdfExporter(QObject *parent = nullptr);
    
    enum PageSize {
        A4,
        Letter,
        Legal,
        Tabloid
    };
    Q_ENUM(PageSize)
    
    struct ExportOptions {
        PageSize pageSize;
        qreal marginLeft;  // mm
        qreal marginRight; // mm
        qreal marginTop;   // mm
        qreal marginBottom; // mm
        bool includeHeaders;
        bool includeToc;
        QString headerText;
        QString footerText;
        QString title;

        ExportOptions()
            : pageSize(A4)
            , marginLeft(20.0)
            , marginRight(20.0)
            , marginTop(20.0)
            , marginBottom(20.0)
            , includeHeaders(true)
            , includeToc(false)
            , headerText("")
            , footerText("Page ${page} of ${pages}")
            , title("")
        {}
    };
    
    bool exportToPdf(const QString &htmlContent, const QString &outputPath, 
                     const ExportOptions &options = ExportOptions());
    
    bool exportDocumentToPdf(const QString &documentPath, const QString &outputPath,
                           const ExportOptions &options = ExportOptions());
    
    QStringList supportedPageSizes() const;
    
signals:
    void exportStarted();
    void exportProgress(int percentage);
    void exportCompleted(const QString &filePath);
    void exportError(const QString &error);

private:
    QWebEngineProfile *m_profile;
    QWebEnginePage *m_page;
    
    void setupPrinter(QPrinter *printer, const ExportOptions &options);
    QString generatePrintCss(const ExportOptions &options) const;
    QString injectPrintStyles(const QString &html, const ExportOptions &options) const;
};

#endif // PDFEXPORTER_H