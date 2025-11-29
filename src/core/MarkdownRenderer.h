// MarkdownRenderer.h
#ifndef MARKDOWNRENDERER_H
#define MARKDOWNRENDERER_H

#include <QObject>
#include <QString>
#include <QUrl>
#include <QTimer>

class MarkdownRenderer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString htmlContent READ htmlContent NOTIFY htmlContentChanged)
    Q_PROPERTY(bool mathEnabled READ mathEnabled WRITE setMathEnabled NOTIFY mathEnabledChanged)
    Q_PROPERTY(bool gfmEnabled READ gfmEnabled WRITE setGfmEnabled NOTIFY gfmEnabledChanged)

public:
    explicit MarkdownRenderer(QObject *parent = nullptr);
    
    QString renderMarkdown(const QString &markdown) const;
    QString htmlContent() const { return m_htmlContent; }
    
    bool mathEnabled() const { return m_mathEnabled; }
    void setMathEnabled(bool enabled);
    
    bool gfmEnabled() const { return m_gfmEnabled; }
    void setGfmEnabled(bool enabled);
    
    void setBaseUrl(const QUrl &baseUrl);
    QUrl baseUrl() const { return m_baseUrl; }
    
public slots:
    void processMarkdown(const QString &markdown);
    void setCodeBlockTheme(const QString &theme);

signals:
    void htmlContentChanged();
    void mathEnabledChanged();
    void gfmEnabledChanged();
    void renderingError(const QString &error);

private:
    QString m_htmlContent;
    QString m_markdownContent;
    bool m_mathEnabled;
    bool m_gfmEnabled;
    QUrl m_baseUrl;
    QString m_codeBlockTheme;
    
    QString generateHtmlTemplate(const QString &bodyContent) const;
    QString applyCustomStyling(const QString &html) const;
    QString processCodeBlocks(const QString &html) const;
    QString resolveImagePaths(const QString &html) const;
    QString injectKaTeX(const QString &html) const;
    
    QString renderWithCmark(const QString &markdown) const;
    QString renderWithDiscount(const QString &markdown) const;
};

#endif // MARKDOWNRENDERER_H