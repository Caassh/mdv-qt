import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

Item {
    id: fileExplorer
    
    signal fileOpened(string filePath)
    signal fileOperationError(string error)
    
    // Properties to control the file explorer
    property bool showOnlyMarkdown: true
    property string currentPath: ""
    
    // Toggle for showing only markdown files
    property alias markdownOnly: filterToggle.checked
    
    ColumnLayout {
        anchors.fill: parent
        
        // Toolbar with file operations
        ToolBar {
            id: explorerToolbar
            Layout.fillWidth: true
            height: 40
            
            background: Rectangle {
                color: "#f8f8f8"
                border.color: "#e0e0e0"
                border.width: 1
            }
            
            RowLayout {
                anchors.fill: parent
                spacing: 5
                
                ToolButton {
                    id: newFileButton
                    text: "New File"
                    icon.source: "qrc:/icons/document-new.svg"
                    icon.width: 16
                    icon.height: 16
                    onClicked: createNewFile()
                    ToolTip.text: "Create new markdown file"
                    ToolTip.visible: hovered
                }
                
                ToolButton {
                    id: newFolderButton
                    text: "New Folder"
                    icon.source: "qrc:/icons/folder-new.svg"
                    icon.width: 16
                    icon.height: 16
                    onClicked: createNewFolder()
                    ToolTip.text: "Create new folder"
                    ToolTip.visible: hovered
                }
                
                ToolButton {
                    id: refreshButton
                    text: "Refresh"
                    icon.source: "qrc:/icons/view-refresh.svg"
                    icon.width: 16
                    icon.height: 16
                    onClicked: fileSystemModel.refresh()
                    ToolTip.text: "Refresh file list"
                    ToolTip.visible: hovered
                }
                
                Rectangle {
                    Layout.fillWidth: true
                    color: "transparent"
                }
                
                ToolButton {
                    id: filterToggle
                    text: "Markdown Only"
                    checkable: true
                    checked: true
                    onClicked: fileSystemModel.markdownOnly = checked
                    ToolTip.text: "Show only markdown files"
                    ToolTip.visible: hovered
                }
            }
        }
        
        // Search field for filtering files
        TextField {
            id: searchField
            Layout.fillWidth: true
            placeholderText: "Search files..."
            background: Rectangle {
                border.color: "#cccccc"
                border.width: 1
                radius: 3
                color: "#ffffff"
            }
            
            onTextChanged: {
                // Implement filtering based on search text
                fileSystemModel.filterRegExp = new RegExp(text, "i");
            }
        }
        
        // Directory Tree View
        ScrollView {
            id: scrollView
            Layout.fillWidth: true
            Layout.fillHeight: true
            
            ListView {
                id: directoryTree
                flickableDirection: Flickable.VerticalFlick
                boundsBehavior: Flickable.DragOverBounds
                
                // Custom delegate for tree items
                delegate: TreeItemDelegate {
                    width: ListView.view.width
                    isCurrentItem: ListView.isCurrentItem
                    itemIndex: index
                    listView: ListView.view
                    modelData: model
                    fileSystemModel: fileSystemModel

                    onClicked: function(mouse) {
                        ListView.view.currentIndex = index;
                        if (model.isFolder) {
                            // Expand/collapse folder
                            if (directoryTree.isExpanded(index)) {
                                directoryTree.collapse(index)
                            } else {
                                directoryTree.expand(index)
                            }
                        } else {
                            // Open file
                            if (fileSystemModel.isMarkdownFile(model.filePath)) {
                                fileExplorer.fileOpened(model.filePath)
                            }
                        }
                    }

                    onPressAndHold: function(mouse) {
                        ListView.view.currentIndex = index;
                        // Show context menu
                        contextMenu.currentItem = model
                        contextMenu.popup()
                    }
                }
            }
        }
    }
    
    // Context menu for file operations
    Menu {
        id: contextMenu
        property var currentItem: null
        
        MenuItem {
            text: "Open"
            onTriggered: {
                if (contextMenu.currentItem && !contextMenu.currentItem.isFolder) {
                    fileExplorer.fileOpened(contextMenu.currentItem.filePath)
                }
            }
            visible: contextMenu.currentItem && !contextMenu.currentItem.isFolder
        }
        
        MenuItem {
            text: "Rename"
            onTriggered: {
                // Implement rename functionality
                renameDialog.currentPath = contextMenu.currentItem.filePath
                renameDialog.open()
            }
        }
        
        MenuItem {
            text: "Delete"
            onTriggered: {
                if (contextMenu.currentItem) {
                    deleteDialog.currentPath = contextMenu.currentItem.filePath
                    deleteDialog.open()
                }
            }
        }
        
        MenuSeparator { }
        
        MenuItem {
            text: "New File"
            onTriggered: {
                var parentPath = contextMenu.currentItem ? 
                                (contextMenu.currentItem.isFolder ? 
                                    contextMenu.currentItem.filePath : 
                                    Qt.QDir.fromNativeSeparators(contextMenu.currentItem.filePath).split('/').slice(0, -1).join('/')) :
                                fileSystemModel.rootPath
                createNewFile(parentPath)
            }
        }
        
        MenuItem {
            text: "New Folder"
            onTriggered: {
                var parentPath = contextMenu.currentItem ? 
                                (contextMenu.currentItem.isFolder ? 
                                    contextMenu.currentItem.filePath : 
                                    Qt.QDir.fromNativeSeparators(contextMenu.currentItem.filePath).split('/').slice(0, -1).join('/')) :
                                fileSystemModel.rootPath
                createNewFolder(parentPath)
            }
        }
    }
    
    // Dialog for creating new files
    Dialog {
        id: newFileDialog
        title: "New File"
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        width: 300
        height: 150

        standardButtons: Dialog.Ok | Dialog.Cancel
        
        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 10
            
            TextField {
                id: newFileName
                placeholderText: "Enter filename (e.g., document.md)"
                Layout.fillWidth: true
                focus: true
            }
            
            Label {
                text: "File will be created in: " + (contextMenu.currentItem?.filePath || fileSystemModel.rootPath)
                Layout.fillWidth: true
                wrapMode: Text.Wrap
                font.pixelSize: 10
                color: "#666666"
            }
        }
        
        onAccepted: {
            var parentPath = contextMenu.currentItem ? 
                           (contextMenu.currentItem.isFolder ? 
                               contextMenu.currentItem.filePath : 
                               Qt.QDir.fromNativeSeparators(contextMenu.currentItem.filePath).split('/').slice(0, -1).join('/')) :
                           fileSystemModel.rootPath
            
            if (newFileName.text.trim() !== "") {
                var newFilePath = parentPath + "/" + newFileName.text.trim()
                fileSystemModel.createFile(parentPath, newFileName.text.trim())
                newFileName.text = ""
            }
        }
    }
    
    // Dialog for creating new folders
    Dialog {
        id: newFolderDialog
        title: "New Folder"
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        width: 300
        height: 150

        standardButtons: Dialog.Ok | Dialog.Cancel
        
        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 10
            
            TextField {
                id: newFolderName
                placeholderText: "Enter folder name"
                Layout.fillWidth: true
                focus: true
            }
            
            Label {
                text: "Folder will be created in: " + (contextMenu.currentItem?.filePath || fileSystemModel.rootPath)
                Layout.fillWidth: true
                wrapMode: Text.Wrap
                font.pixelSize: 10
                color: "#666666"
            }
        }
        
        onAccepted: {
            var parentPath = contextMenu.currentItem ? 
                           (contextMenu.currentItem.isFolder ? 
                               contextMenu.currentItem.filePath : 
                               Qt.QDir.fromNativeSeparators(contextMenu.currentItem.filePath).split('/').slice(0, -1).join('/')) :
                           fileSystemModel.rootPath
            
            if (newFolderName.text.trim() !== "") {
                fileSystemModel.createDirectory(parentPath, newFolderName.text.trim())
                newFolderName.text = ""
            }
        }
    }
    
    // Dialog for renaming files
    Dialog {
        id: renameDialog
        title: "Rename"
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        width: 300
        height: 150

        standardButtons: Dialog.Ok | Dialog.Cancel
        
        property string currentPath: ""
        
        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 10
            
            TextField {
                id: renameField
                text: currentPath ? Qt.QDir.fromNativeSeparators(currentPath).split('/').pop() : ""
                placeholderText: "Enter new name"
                Layout.fillWidth: true
                focus: true
            }
            
            Label {
                text: "Current path: " + (renameDialog.currentPath || "")
                Layout.fillWidth: true
                wrapMode: Text.Wrap
                font.pixelSize: 10
                color: "#666666"
            }
        }
        
        onAccepted: {
            if (renameField.text.trim() !== "" && renameDialog.currentPath !== "") {
                var currentDir = Qt.QDir.fromNativeSeparators(renameDialog.currentPath).split('/').slice(0, -1).join('/')
                var newPath = currentDir + "/" + renameField.text.trim()
                fileSystemModel.renameFile(renameDialog.currentPath, newPath)
            }
        }
    }
    
    // Confirmation dialog for deletion
    Dialog {
        id: deleteDialog
        title: "Confirm Delete"
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        width: 400
        height: 150

        standardButtons: Dialog.Ok | Dialog.Cancel
        
        property string currentPath: ""
        
        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 10
            
            Label {
                text: "Are you sure you want to delete:\n" + (deleteDialog.currentPath || "")
                Layout.fillWidth: true
                wrapMode: Text.Wrap
            }
            
            Label {
                text: "This action cannot be undone."
                font.bold: true
                color: "#e74c3c"
            }
        }
        
        onAccepted: {
            if (deleteDialog.currentPath !== "") {
                fileSystemModel.deleteFile(deleteDialog.currentPath)
            }
        }
    }
    
    // Functions for creating new files and folders
    function createNewFile(parentPath) {
        var path = parentPath || (contextMenu.currentItem?.filePath || fileSystemModel.rootPath)
        if (contextMenu.currentItem && !contextMenu.currentItem.isFolder) {
            path = Qt.QDir.fromNativeSeparators(contextMenu.currentItem.filePath).split('/').slice(0, -1).join('/')
        }
        newFileDialog.open()
    }
    
    function createNewFolder(parentPath) {
        var path = parentPath || (contextMenu.currentItem?.filePath || fileSystemModel.rootPath)
        if (contextMenu.currentItem && !contextMenu.currentItem.isFolder) {
            path = Qt.QDir.fromNativeSeparators(contextMenu.currentItem.filePath).split('/').slice(0, -1).join('/')
        }
        newFolderDialog.open()
    }
}