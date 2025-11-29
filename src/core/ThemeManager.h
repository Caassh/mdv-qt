// ThemeManager.h
#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <QObject>
#include <QColor>
#include <QPalette>
#include <QGuiApplication>

class ThemeManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ThemeType currentTheme READ currentTheme WRITE setCurrentTheme NOTIFY currentThemeChanged)
    Q_PROPERTY(QColor primaryColor READ primaryColor CONSTANT)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor CONSTANT)
    Q_PROPERTY(QColor textColor READ textColor CONSTANT)

public:
    enum class ThemeType {
        Light,
        Dark,
        Auto
    };
    Q_ENUM(ThemeType)

    enum class ColorRole {
        Primary,
        Secondary,
        Background,
        Surface,
        TextPrimary,
        TextSecondary,
        Border,
        Error,
        Success
    };
    Q_ENUM(ColorRole)

    explicit ThemeManager(QObject *parent = nullptr);
    
    ThemeType currentTheme() const;
    void setCurrentTheme(ThemeType theme);
    
    QColor primaryColor() const;  // #724f97
    QColor backgroundColor() const;
    QColor textColor() const;
    
    QColor colorForRole(ColorRole role) const;
    
    static ThemeManager* instance();
    
signals:
    void currentThemeChanged();
    void colorsChanged();
    
private:
    ThemeType m_currentTheme;
    QPalette m_lightPalette;
    QPalette m_darkPalette;
    
    void applySystemTheme();
};

#endif // THEMEMANAGER_H