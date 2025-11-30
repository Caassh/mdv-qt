// TreeItemDelegate.qml
import QtQuick
import QtQuick.Controls

Item {
    id: treeItemDelegate
    width: parent ? parent.width : 0
    height: 32

    property var modelData: null
    property var fileSystemModel: null
    property bool isCurrentItem: false
    property int itemIndex: -1
    property var listView: null  // Reference to the parent ListView

    signal clicked(var mouse)
    signal pressAndHold(var mouse)

    Rectangle {
        id: background
        anchors.fill: parent
        color: treeItemDelegate.isCurrentItem ? "#f0f0f0" : "transparent"
        border.color: "#e0e0e0"

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            propagateComposedEvents: true
            acceptedButtons: Qt.LeftButton | Qt.RightButton

            onClicked: {
                treeItemDelegate.clicked(mouse);
                mouse.accepted = true;
            }

            onPressAndHold: {
                treeItemDelegate.pressAndHold(mouse);
                mouse.accepted = true;
            }
        }
    }
    
    Row {
        anchors.verticalCenter: parent.verticalCenter
        x: 10 + (modelData.depth * 20) // Indentation based on folder depth
        spacing: 8
        
        // Expand/collapse indicator for folders
        Rectangle {
            width: 16
            height: 16
            color: "transparent"
            visible: modelData.isFolder
            
            Text {
                anchors.centerIn: parent
                text: treeItemDelegate.listView && treeItemDelegate.listView.isExpanded(treeItemDelegate.itemIndex) ? "▼" : "▶"
                font.pixelSize: 10
                color: "#666666"
            }
            
            MouseArea {
                anchors.fill: parent
                visible: parent.visible
                onClicked: {
                    if (treeItemDelegate.listView && treeItemDelegate.listView.isExpanded(treeItemDelegate.itemIndex)) {
                        treeItemDelegate.listView.collapse(treeItemDelegate.itemIndex)
                    } else {
                        treeItemDelegate.listView.expand(treeItemDelegate.itemIndex)
                    }
                }
            }
        }
        
        Image {
            id: icon
            source: modelData.isFolder ?
                   (treeItemDelegate.listView && treeItemDelegate.listView.isExpanded(treeItemDelegate.itemIndex) ?
                       "qrc:/icons/folder-open.svg" : "qrc:/icons/folder.svg") :
                   (fileSystemModel.isMarkdownFile(modelData.filePath) ?
                       "qrc:/icons/markdown.svg" : "qrc:/icons/document.svg")
            width: 16
            height: 16
            anchors.verticalCenter: parent.verticalCenter
        }
        
        Text {
            id: fileName
            text: modelData.display
            anchors.verticalCenter: parent.verticalCenter
            color: modelData.isFolder ? "#3498db" : "#2c3e50"
            elide: Text.ElideRight
            maximumLineCount: 1
            font.bold: modelData.isFolder
        }
    }
    
    // Selection indicator
    Rectangle {
        id: selectionIndicator
        width: 4
        height: parent.height - 4
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.margins: 2
        color: "#724f97" // iOS-7 purple accent
        visible: treeItemDelegate.isCurrentItem
    }
}