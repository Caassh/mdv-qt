// Main.qml
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform as Labs

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
        
        // Main Content Area - Tabs
        TabView {
            id: tabView
            SplitView.fillWidth: true
            tabBar: TabBar {
                id: tabbar
                background: Rectangle {
                    color: "#f0f0f0"
                    border.color: "#e0e0e0"
                    border.width: 1
                }
            }
        }
    }
    
    // File dialog
    Labs.FileDialog {
        id: fileDialog
        title: "Open Markdown File"
        nameFilters: ["Markdown files (*.md *.markdown)", "All files (*)"]
        onAccepted: documentManager.openDocument(fileUrl.toString().replace("file://", ""))
    }

    // Save as dialog
    Labs.FileDialog {
        id: saveAsDialog
        title: "Save Markdown File As"
        nameFilters: ["Markdown files (*.md *.markdown)", "All files (*)"]
        selectExisting: false
        onAccepted: documentManager.saveDocumentAs(fileUrl.toString().replace("file://", ""))
    }
    
    // Function to set editor mode
    function setEditorMode(mode) {
        // This will be connected to the active editor tab
        if (tabView.currentItem && tabView.currentItem.setEditorMode) {
            tabView.currentItem.setEditorMode(mode);
        }
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