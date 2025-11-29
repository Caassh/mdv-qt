// StyledButton.qml
import QtQuick
import QtQuick.Controls

Button {
    id: control
    
    // iOS-7 inspired styling with purple accents
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                           implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                            implicitContentHeight + topPadding + bottomPadding)
    
    // Background
    background: Rectangle {
        id: buttonBackground
        implicitWidth: 80
        implicitHeight: 24
        radius: 4
        border.width: 1
        border.color: control.flat ? "transparent" : 
                     control.down ? "#5a3a7d" : 
                     control.hovered ? "#8a6fbf" : "#c0c0c0"
        color: control.flat ? "transparent" :
               control.down ? "#5a3a7d" :  // Darker purple when pressed
               control.hovered ? "#e9d8f0" :  // Light purple when hovered
               "#ffffff"  // White background
        
        Behavior on color {
            ColorAnimation { duration: 100 }
        }
        Behavior on border.color {
            ColorAnimation { duration: 100 }
        }
    }
    
    // Content (text)
    contentItem: Text {
        text: control.text
        font: control.font
        color: control.flat ? 
               (control.down ? "#5a3a7d" : 
                control.hovered ? "#8a6fbf" : "#724f97") :  // Purple accents
               control.down ? "#ffffff" : 
               control.hovered ? "#5a3a7d" : "#2c3e50"  // Dark text for default buttons
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }
}