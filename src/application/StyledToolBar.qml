// StyledToolBar.qml
import QtQuick
import QtQuick.Controls

ToolBar {
    id: control
    
    implicitWidth: 200
    implicitHeight: 40
    
    // iOS-7 inspired styling
    background: Rectangle {
        color: "#ffffff"  // White background
        border.color: "#e0e0e0"  // Light gray border
        border.width: 1
    }
}