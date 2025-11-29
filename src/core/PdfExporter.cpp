// PdfExporter.cpp
#include "PdfExporter.h"
#include <QtPrintSupport/QPrintDialog>
#include <QPdfWriter>
#include <QTextDocument>
#include <QPageSize>
#include <QPageLayout>
#include <QPainter>
#include <QFile>
#include <QTextStream>
#include <QDebug>

PdfExporter::PdfExporter(QObject *parent)
    : QObject(parent)
{
    // Create a dedicated profile for PDF export to avoid interfering with web views
    m_profile = new QWebEngineProfile(this);
    m_page = new QWebEnginePage(m_profile, this);
    
    connect(m_page, &QWebEnginePage::pdfPrintingFinished, 
            [this](const QString &filePath, bool success) {
        if (success) {
            emit exportCompleted(filePath);
        } else {
            emit exportError("PDF export failed for: " + filePath);
        }
    });
}

bool PdfExporter::exportToPdf(const QString &htmlContent, const QString &outputPath,
                              const ExportOptions &options)
{
    emit exportStarted();

    // Setup the HTML content for printing
    QString printHtml = injectPrintStyles(htmlContent, options);

    // For PDF export, we'll use QWebEnginePage's printToPdf method
    // But first we need to wait for the page to load
    bool success = false;
    QEventLoop loop;
    QTimer timeout;
    timeout.setSingleShot(true);
    timeout.setInterval(10000); // 10 second timeout

    // Connect to the finished signal
    connect(m_page, &QWebEnginePage::pdfPrintingFinished,
            [&](const QString &filePath, bool result) {
        success = result;
        loop.quit();
    });

    connect(&timeout, &QTimer::timeout, [&]() {
        loop.quit();
    });

    // Load the content into the web page
    m_page->setHtml(printHtml, QUrl());

    // Configure page layout based on options
    QPageLayout layout;

    // Set page size based on options
    QPageSize::PageSizeId pageSizeId = QPageSize::A4;
    switch (options.pageSize) {
        case Letter:
            pageSizeId = QPageSize::Letter;
            break;
        case Legal:
            pageSizeId = QPageSize::Legal;
            break;
        case Tabloid:
            pageSizeId = QPageSize::Tabloid;
            break;
        default:
            pageSizeId = QPageSize::A4;
            break;
    }

    layout.setPageSize(QPageSize(pageSizeId));
    layout.setUnits(QPageLayout::Millimeter);
    layout.setMargins(QMarginsF(options.marginLeft,
                               options.marginTop,
                               options.marginRight,
                               options.marginBottom));

    // Start PDF export
    m_page->printToPdf(outputPath, layout);
    timeout.start();
    loop.exec();

    if (success) {
        emit exportCompleted(outputPath);
        return true;
    } else {
        emit exportError("PDF export failed");
        return false;
    }
}

bool PdfExporter::exportDocumentToPdf(const QString &documentPath, const QString &outputPath,
                                     const ExportOptions &options)
{
    // Read the markdown document
    QFile file(documentPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        emit exportError("Could not read document: " + documentPath);
        return false;
    }
    
    QString markdownContent = file.readAll();
    file.close();
    
    // Convert markdown to HTML using the existing renderer
    // In a complete implementation, we would use the MarkdownRenderer
    // For now, we'll do a simple conversion
    
    // Simple markdown to HTML conversion for export
    QString html = markdownContent;
    html.replace("\n", "<br/>");
    html.replace("<h1>", "<h1 style=\"margin-top: 1em; margin-bottom: 0.5em;\">");
    html.replace("<h2>", "<h2 style=\"margin-top: 1em; margin-bottom: 0.5em;\">");
    html.replace("<h3>", "<h3 style=\"margin-top: 1em; margin-bottom: 0.5em;\">");
    html.replace("<p>", "<p style=\"margin-top: 0; margin-bottom: 1em;\">");
    html.replace("<pre>", "<pre style=\"background-color: #f8f9fa; padding: 1em; border-radius: 4px; overflow: auto;\">");
    
    html = "<!DOCTYPE html><html><head><meta charset=\"UTF-8\"><title>" + 
           options.title + "</title></head><body>" + html + "</body></html>";
    
    return exportToPdf(html, outputPath, options);
}

QStringList PdfExporter::supportedPageSizes() const
{
    return {"A4", "Letter", "Legal", "Tabloid"};
}

void PdfExporter::setupPrinter(QPrinter *printer, const ExportOptions &options)
{
    // Set page size
    QPageSize::PageSizeId pageSizeId = QPageSize::A4;
    switch (options.pageSize) {
        case Letter:
            pageSizeId = QPageSize::Letter;
            break;
        case Legal:
            pageSizeId = QPageSize::Legal;
            break;
        case Tabloid:
            pageSizeId = QPageSize::Tabloid;
            break;
        default:
            pageSizeId = QPageSize::A4;
            break;
    }
    printer->setPageSize(QPageSize(pageSizeId));

    // Set page margins
    printer->setPageMargins(QMarginsF(options.marginLeft, options.marginTop,
                                    options.marginRight, options.marginBottom),
                          QPageLayout::Millimeter);
}

QString PdfExporter::generatePrintCss(const ExportOptions &options) const
{
    QString css = R"(
        @media print {
            body {
                font-family: Arial, Helvetica, sans-serif;
                line-height: 1.6;
                color: #000;
                background-color: #fff;
            }
            h1, h2, h3, h4, h5, h6 {
                color: #000;
                page-break-after: avoid;
                page-break-inside: avoid;
            }
            img {
                max-width: 100% !important;
                height: auto;
                page-break-inside: avoid;
            }
            pre {
                white-space: pre-wrap;
                word-wrap: break-word;
                page-break-inside: avoid;
            }
            code {
                font-family: monospace;
            }
            blockquote {
                border-left: 3px solid #ccc;
                padding-left: 1em;
                margin-left: 0;
                page-break-inside: avoid;
            }
            table {
                width: 100%;
                border-collapse: collapse;
            }
            th, td {
                border: 1px solid #000;
                padding: 0.3em;
                page-break-inside: avoid;
            }
        }
    )";
    
    return css;
}

QString PdfExporter::injectPrintStyles(const QString &html, const ExportOptions &options) const
{
    QString styledHtml = html;
    
    // If it's already a complete HTML document, inject styles into the head
    if (html.contains("<head>") || html.contains("<!DOCTYPE")) {
        QString printCss = generatePrintCss(options);
        styledHtml.replace("<head>", "<head><style>" + printCss + "</style>");
    } else {
        // Otherwise, wrap in a complete HTML document
        QString printCss = generatePrintCss(options);
        styledHtml = QString(R"(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>%1</title>
    <style>
        body {
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, 'Helvetica Neue', Arial, sans-serif;
            line-height: 1.6;
            color: #2c3e50;
            margin: %2mm %3mm %4mm %5mm;
        }
        %6
    </style>
</head>
<body>
    %7
</body>
</html>
        )").arg(options.title.isEmpty() ? "Document" : options.title)
                .arg(options.marginTop).arg(options.marginRight)
                .arg(options.marginBottom).arg(options.marginLeft)
                .arg(printCss)
                .arg(html);
    }
    
    return styledHtml;
}