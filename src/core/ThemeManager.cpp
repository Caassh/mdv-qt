// ThemeManager.cpp
#include "ThemeManager.h"
#include <QGuiApplication>
#include <QtWidgets/QApplication>
#include <QtWidgets/QStyle>

ThemeManager::ThemeManager(QObject *parent)
    : QObject(parent)
    , m_currentTheme(ThemeType::Light)
{
    // Initialize color palettes
    // iOS-7 inspired colors
    QColor primaryColor(114, 79, 151);  // #724f97
    QColor backgroundColor(255, 255, 255);  // #ffffff
    QColor darkBackgroundColor(30, 30, 30);  // #1e1e1e
    QColor textColor(44, 62, 80);  // #2c3e50
    QColor darkTextColor(224, 224, 224);  // #e0e0e0
    QColor borderColor(224, 224, 224);  // #e0e0e0
    QColor darkBorderColor(64, 64, 64);  // #404040

    // Light theme
    m_lightPalette.setColor(QPalette::Window, backgroundColor);
    m_lightPalette.setColor(QPalette::WindowText, textColor);
    m_lightPalette.setColor(QPalette::Base, Qt::white);
    m_lightPalette.setColor(QPalette::AlternateBase, QColor(248, 249, 250));  // #f8f9fa
    m_lightPalette.setColor(QPalette::ToolTipBase, textColor);
    m_lightPalette.setColor(QPalette::ToolTipText, Qt::white);
    m_lightPalette.setColor(QPalette::Text, textColor);
    m_lightPalette.setColor(QPalette::Button, Qt::white);
    m_lightPalette.setColor(QPalette::ButtonText, textColor);
    m_lightPalette.setColor(QPalette::BrightText, Qt::red);
    m_lightPalette.setColor(QPalette::Highlight, primaryColor);
    m_lightPalette.setColor(QPalette::HighlightedText, Qt::white);
    m_lightPalette.setColor(QPalette::Light, borderColor);
    m_lightPalette.setColor(QPalette::Midlight, borderColor);
    m_lightPalette.setColor(QPalette::Dark, QColor(153, 153, 153));
    m_lightPalette.setColor(QPalette::Mid, QColor(192, 192, 192));
    m_lightPalette.setColor(QPalette::Shadow, QColor(218, 218, 218));

    // Dark theme
    m_darkPalette.setColor(QPalette::Window, darkBackgroundColor);
    m_darkPalette.setColor(QPalette::WindowText, darkTextColor);
    m_darkPalette.setColor(QPalette::Base, darkBackgroundColor);
    m_darkPalette.setColor(QPalette::AlternateBase, QColor(45, 45, 45));  // #2d2d2d
    m_darkPalette.setColor(QPalette::ToolTipBase, darkTextColor);
    m_darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    m_darkPalette.setColor(QPalette::Text, darkTextColor);
    m_darkPalette.setColor(QPalette::Button, darkBackgroundColor);
    m_darkPalette.setColor(QPalette::ButtonText, darkTextColor);
    m_darkPalette.setColor(QPalette::BrightText, Qt::red);
    m_darkPalette.setColor(QPalette::Highlight, primaryColor);
    m_darkPalette.setColor(QPalette::HighlightedText, Qt::white);
    m_darkPalette.setColor(QPalette::Light, darkBorderColor);
    m_darkPalette.setColor(QPalette::Midlight, darkBorderColor);
    m_darkPalette.setColor(QPalette::Dark, QColor(100, 100, 100));
    m_darkPalette.setColor(QPalette::Mid, QColor(80, 80, 80));
    m_darkPalette.setColor(QPalette::Shadow, QColor(50, 50, 50));

    // Apply the current theme
    applySystemTheme();
}

ThemeManager::ThemeType ThemeManager::currentTheme() const
{
    return m_currentTheme;
}

void ThemeManager::setCurrentTheme(ThemeType theme)
{
    if (m_currentTheme != theme) {
        m_currentTheme = theme;
        applySystemTheme();
        emit currentThemeChanged();
    }
}

QColor ThemeManager::primaryColor() const
{
    return QColor(114, 79, 151);  // #724f97
}

QColor ThemeManager::backgroundColor() const
{
    return m_currentTheme == ThemeType::Dark ?
           QColor(30, 30, 30) : QColor(255, 255, 255);
}

QColor ThemeManager::textColor() const
{
    return m_currentTheme == ThemeType::Dark ?
           QColor(224, 224, 224) : QColor(44, 62, 80);
}

QColor ThemeManager::colorForRole(ColorRole role) const
{
    switch (role) {
        case ColorRole::Primary:
            return QColor(114, 79, 151);  // #724f97
        case ColorRole::Secondary:
            return QColor(155, 89, 182);  // #9b59b6
        case ColorRole::Background:
            return m_currentTheme == ThemeType::Dark ?
                   QColor(30, 30, 30) : QColor(255, 255, 255);
        case ColorRole::Surface:
            return m_currentTheme == ThemeType::Dark ?
                   QColor(45, 45, 45) : QColor(248, 249, 250);
        case ColorRole::TextPrimary:
            return m_currentTheme == ThemeType::Dark ?
                   QColor(224, 224, 224) : QColor(44, 62, 80);
        case ColorRole::TextSecondary:
            return m_currentTheme == ThemeType::Dark ?
                   QColor(160, 160, 160) : QColor(127, 140, 141);
        case ColorRole::Border:
            return m_currentTheme == ThemeType::Dark ?
                   QColor(64, 64, 64) : QColor(224, 224, 224);
        case ColorRole::Error:
            return QColor(231, 76, 60);  // #e74c3c
        case ColorRole::Success:
            return QColor(46, 204, 113);  // #2ecc71
    }
    return QColor();  // Invalid color
}

ThemeManager* ThemeManager::instance()
{
    static ThemeManager* s_instance = nullptr;
    if (!s_instance) {
        s_instance = new ThemeManager(qApp);
    }
    return s_instance;
}

void ThemeManager::applySystemTheme()
{
    if (m_currentTheme == ThemeType::Auto) {
        // For now, default to light theme
        // In a complete implementation, we would detect system theme
        qApp->setPalette(m_lightPalette);
    } else if (m_currentTheme == ThemeType::Dark) {
        qApp->setPalette(m_darkPalette);
    } else {
        qApp->setPalette(m_lightPalette);
    }

    // Set application styling properties
    if (qobject_cast<QApplication*>(qApp)) {
        qobject_cast<QApplication*>(qApp)->setStyleSheet(QString(
        "QMainWindow { background-color: %1; }"
        "QMenuBar { background-color: %1; spacing: 6px; }"
        "QMenuBar::item { padding: 4px 8px; }"
        "QMenuBar::item:selected { background: %2; }"
        "QToolBar { background-color: %1; border: 1px solid %3; spacing: 2px; }"
        "QStatusBar { background-color: %1; }"
        "QLabel { color: %4; }"
        "QPushButton { "
            "background-color: %1; "
            "border: 1px solid %3; "
            "border-radius: 3px; "
            "padding: 4px 8px; "
            "color: %4; "
        "}"
        "QPushButton:hover { "
            "border: 1px solid %5; "
        "}"
        "QPushButton:pressed { "
            "background-color: %6; "
        "}"
        "QToolButton { "
            "background-color: transparent; "
            "border: 1px solid transparent; "
            "border-radius: 3px; "
            "padding: 4px 8px; "
            "color: %4; "
        "}"
        "QToolButton:hover { "
            "background-color: %7; "
            "border: 1px solid %3; "
        "}"
        "QToolButton:checked { "
            "background-color: %8; "
            "border: 1px solid %8; "
        "}"
        "QLineEdit { "
            "background-color: %1; "
            "border: 1px solid %3; "
            "border-radius: 3px; "
            "padding: 4px; "
            "color: %4; "
        "}"
        "QTextEdit { "
            "background-color: %1; "
            "border: 1px solid %3; "
            "color: %4; "
        "}"
        "QTabWidget::pane { "
            "border: 1px solid %3; "
            "border-top: none; "
        "}"
        "QTabBar::tab { "
            "background: %1; "
            "border: 1px solid %3; "
            "border-top-left-radius: 4px; "
            "border-top-right-radius: 4px; "
            "padding: 4px 8px; "
            "margin: 0px 1px 0 0; "
            "color: %4; "
        "}"
        "QTabBar::tab:selected, QTabBar::tab:hover { "
            "background: %7; "
        "}"
        "QTabBar::tab:selected { "
            "border-bottom-color: %1; "
        "}"
        "QTabBar::tab:!selected { "
            "margin-top: 2px; "
        "}"
        "QScrollBar:vertical { "
            "background: %1; "
            "width: 15px; "
            "margin: 15px 0 15px 0; "
        "}"
        "QScrollBar::handle:vertical { "
            "background: %3; "
            "min-height: 20px; "
            "border-radius: 3px; "
        "}"
        "QScrollBar::handle:vertical:hover { "
            "background: %5; "
        "}"
        "QScrollBar:horizontal { "
            "background: %1; "
            "height: 15px; "
            "margin: 0 15px 0 15px; "
        "}"
        "QScrollBar::handle:horizontal { "
            "background: %3; "
            "min-width: 20px; "
            "border-radius: 3px; "
        "}"
        "QScrollBar::handle:horizontal:hover { "
            "background: %5; "
        "}"
        ).arg(
            m_currentTheme == ThemeType::Dark ? "#1e1e1e" : "#ffffff",  // background
            m_currentTheme == ThemeType::Dark ? "#3d3d3d" : "#e9ecef",  // menu selection
            m_currentTheme == ThemeType::Dark ? "#404040" : "#e0e0e0",  // border
            m_currentTheme == ThemeType::Dark ? "#e0e0e0" : "#2c3e50",  // text
            m_currentTheme == ThemeType::Dark ? "#606060" : "#c0c0c0",  // hover border
            m_currentTheme == ThemeType::Dark ? "#303030" : "#f8f9fa",  // pressed
            m_currentTheme == ThemeType::Dark ? "#2d2d2d" : "#f8f9fa",  // tab hover
            m_currentTheme == ThemeType::Dark ? "#2a2a2a" : "#ffffff"   // selected tab
        ));
    }

    emit colorsChanged();
}