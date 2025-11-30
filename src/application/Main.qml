// Main.qml
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform as Labs  // For FileDialog

ApplicationWindow {
    id: mainWindow
    width: 1200
    height: 800
    title: "MDV-Qt - Markdown Viewer"
    visible: true
    
    // iOS-7 inspired styling
    color: "#ffffff"
    
    // Main content
    header: ToolBar {
        id: toolBar
        background: Rectangle {
            color: "#ffffff"
            border.color: "#e0e0e0"
            border.width: 1
        }
        
        RowLayout {
            anchors.fill: parent
            
            ToolButton {
                text: "New"
                onClicked: documentManager.newDocument()
            }
            
            ToolButton {
                text: "Open"
                onClicked: fileDialog.open()
            }
            
            ToolButton {
                text: "Save"
                onClicked: documentManager.saveCurrent()
            }
            
            ToolButton {
                text: "Save As"
                onClicked: saveAsDialog.open()
            }
            
            ToolButton {
                text: "Export PDF"
                onClicked: pdfExporter.exportCurrentToPdf()
            }
            
            Rectangle { 
                width: 1
                height: parent.height * 0.6
                color: "#d0d0d0"
                Layout.alignment: Qt.AlignVCenter
            }
            
            ToolButton {
                text: "View"
                checkable: true
                checked: true
                onClicked: setEditorMode(0)
            }
            
            ToolButton {
                text: "Edit"
                checkable: true
                onClicked: setEditorMode(1)
            }
            
            ToolButton {
                text: "Split"
                checkable: true
                onClicked: setEditorMode(2)
            }
        }
    }
    
    // Main content area with split view
    SplitView {
        anchors.fill: parent
        orientation: Qt.Horizontal
        
        // File Explorer Panel
        FileExplorer {
            id: fileExplorer
            SplitView.preferredWidth: 250
            SplitView.minimumWidth: 200
            
            onFileOpened: filePath => {
                documentManager.openDocument(filePath);
            }
        }
        
        // Main Content Area
        Item {
            id: tabAreaView
            SplitView.fillWidth: true

            // Simple tab implementation
            Row {
                id: tabBar
                width: parent.width
                height: 30
                spacing: 1

                // This would be populated dynamically in a real implementation
                Rectangle {
                    width: 100
                    height: 30
                    color: "#ffffff"
                    border.color: "#e0e0e0"
                    border.width: 1
                    Text {
                        anchors.centerIn: parent
                        text: "Document.md"
                        color: "#2c3e50"
                    }
                }
            }

            // Content area
            Item {
                anchors.top: tabBar.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom

                // Default content - simple text area for now
                TextArea {
                    id: defaultEditor
                    anchors.fill: parent
                    text: "# Welcome to MDV-Qt\n\nYour markdown viewer application.\n\nOpen a .md file from the file explorer to get started."
                    font.family: "Monospace"
                    font.pixelSize: 14
                    selectByMouse: true
                    wrapMode: TextArea.Wrap
                }
            }
        }
    }
    
    // File dialog
    Labs.FileDialog {
        id: fileDialog
        title: "Open Markdown File"
        folder: Labs.StandardPaths.writableLocation(Labs.StandardPaths.DocumentsLocation)
        nameFilters: ["Markdown files (*.md *.markdown)", "All files (*)"]
        onAccepted: documentManager.openDocument(fileDialog.file.toString().replace("file://", ""))
        onRejected: console.log("File open dialog closed")
    }

    // Save as dialog
    Labs.FileDialog {
        id: saveAsDialog
        title: "Save Markdown File As"
        folder: Labs.StandardPaths.writableLocation(Labs.StandardPaths.DocumentsLocation)
        nameFilters: ["Markdown files (*.md *.markdown)", "All files (*)"]
        onAccepted: documentManager.saveDocumentAs(saveAsDialog.file.toString().replace("file://", ""))
        onRejected: console.log("Save as dialog closed")
    }
    
    // Function to set editor mode
    function setEditorMode(mode) {
        // This will be connected to the active markdown editor
        // For now, we'll just log the mode change
        console.log("Setting editor mode to:", mode);
    }
    
    // Component for markdown document tabs
    Component {
        id: documentTab
        
        MarkdownEditor {
            id: editor
            filePath: tabFilePath
            content: tabContent
            
            onContentChanged: (filePath, content) => {
                documentManager.updateDocumentContent(filePath, content);
            }
        }
    }
}