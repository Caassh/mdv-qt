// MarkdownEditor.qml
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: markdownEditor
    
    signal fileOpened(string filePath)
    signal contentChanged(string filePath, string content)
    
    // Properties
    property string filePath: ""
    property string content: ""
    property int currentMode: 0  // 0: View only, 1: Edit only, 2: Split view
    property alias showToolbar: toolbar.visible
    property alias toolbarPosition: toolbar.anchors.verticalCenter
    
    // State for editor modes
    readonly property bool isViewMode: currentMode === 0
    readonly property bool isEditMode: currentMode === 1
    readonly property bool isSplitMode: currentMode === 2
    
    // Main layout
    ColumnLayout {
        anchors.fill: parent
        
        // Toolbar with formatting options
        ToolBar {
            id: toolbar
            Layout.fillWidth: true
            height: 40
            visible: true
            
            background: Rectangle {
                color: "#f8f8f8"
                border.color: "#e0e0e0"
                border.width: 1
            }
            
            RowLayout {
                anchors.fill: parent
                spacing: 5
                
                // Mode toggle buttons
                ToolButton {
                    text: "View"
                    checkable: true
                    checked: markdownEditor.isViewMode
                    onClicked: markdownEditor.currentMode = 0
                }
                
                ToolButton {
                    text: "Edit"
                    checkable: true
                    checked: markdownEditor.isEditMode
                    onClicked: markdownEditor.currentMode = 1
                }
                
                ToolButton {
                    text: "Split"
                    checkable: true
                    checked: markdownEditor.isSplitMode
                    onClicked: markdownEditor.currentMode = 2
                }
                
                Rectangle { 
                    width: 1
                    height: parent.height * 0.6
                    color: "#d0d0d0"
                    Layout.alignment: Qt.AlignVCenter
                }
                
                // Formatting buttons
                ToolButton {
                    text: "B"
                    font.bold: true
                    ToolTip.text: "Bold (Ctrl+B)"
                    ToolTip.visible: hovered
                    onClicked: applyFormatting("bold")
                }
                
                ToolButton {
                    text: "I"
                    font.italic: true
                    ToolTip.text: "Italic (Ctrl+I)"
                    ToolTip.visible: hovered
                    onClicked: applyFormatting("italic")
                }
                
                ToolButton {
                    text: "H1"
                    font.bold: true
                    onClicked: applyFormatting("heading1")
                }
                
                ToolButton {
                    text: "H2"
                    font.bold: true
                    font.pixelSize: 14
                    onClicked: applyFormatting("heading2")
                }
                
                ToolButton {
                    text: "H3"
                    font.bold: true
                    font.pixelSize: 12
                    onClicked: applyFormatting("heading3")
                }
                
                ToolButton {
                    text: "C"
                    onClicked: applyFormatting("code")
                    ToolTip.text: "Code block"
                    ToolTip.visible: hovered
                }
                
                ToolButton {
                    text: "L"
                    onClicked: applyFormatting("link")
                    ToolTip.text: "Insert link"
                    ToolTip.visible: hovered
                }
                
                Rectangle { 
                    width: 1
                    height: parent.height * 0.6
                    color: "#d0d0d0"
                    Layout.alignment: Qt.AlignVCenter
                }
                
                Button {
                    text: "Toggle Mode"
                    onClicked: toggleEditorMode()
                }
            }
        }
        
        // Main content area
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
            
            // View-only mode (formatted display)
            ScrollView {
                id: viewOnlyScrollView
                anchors.fill: parent
                visible: markdownEditor.isViewMode

                Text {
                    id: viewOnlyText
                    text: contentAsHtml
                    textFormat: Text.RichText
                    wrapMode: Text.Wrap
                    font.family: "sans-serif"
                    font.pixelSize: 14
                    color: "#2c3e50"
                    // Double-click to enter edit mode
                    MouseArea {
                        anchors.fill: parent
                        onDoubleClicked: {
                            markdownEditor.currentMode = 1  // Switch to edit mode
                        }
                    }
                }
            }
            
            // Edit-only mode (raw markdown)
            ScrollView {
                id: editScrollView
                anchors.fill: parent
                visible: markdownEditor.isEditMode
                
                TextArea {
                    id: editTextArea
                    text: markdownEditor.content
                    font.family: "Consolas, Monaco, 'Courier New', monospace"
                    font.pixelSize: 14
                    wrapMode: TextArea.Wrap
                    selectByMouse: true
                    
                    // Update content when text changes
                    onTextChanged: {
                        if (markdownEditor.content !== text) {
                            markdownEditor.content = text
                            contentChanged(markdownEditor.filePath, text)
                        }
                    }
                }
            }
            
            // Split view mode
            SplitView {
                id: splitView
                anchors.fill: parent
                visible: markdownEditor.isSplitMode
                orientation: Qt.Horizontal
                
                // Left side: Raw markdown editor
                ScrollView {
                    id: splitEditScrollView
                    SplitView.preferredWidth: parent.width / 2
                    SplitView.minimumWidth: 200
                    
                    TextArea {
                        id: splitEditTextArea
                        text: markdownEditor.content
                        font.family: "Consolas, Monaco, 'Courier New', monospace"
                        font.pixelSize: 14
                        wrapMode: TextArea.Wrap
                        selectByMouse: true
                        
                        // Update content when text changes
                        onTextChanged: {
                            if (markdownEditor.content !== text) {
                                markdownEditor.content = text
                                contentChanged(markdownEditor.filePath, text)
                            }
                        }
                    }
                }
                
                // Right side: Rendered preview
                ScrollView {
                    id: splitPreviewScrollView
                    SplitView.fillWidth: true
                    SplitView.minimumWidth: 200
                    
                    Text {
                        id: splitPreviewText
                        text: contentAsHtml
                        textFormat: Text.RichText
                        wrapMode: Text.Wrap
                        font.family: "sans-serif"
                        font.pixelSize: 14
                        color: "#2c3e50"
                    }
                }
            }
        }
    }
    
    // Computed property for HTML content
    readonly property string contentAsHtml: {
        // In a real implementation, we'd call the MarkdownRenderer
        // For now, we'll create a simple HTML representation
        var html = markdownEditor.content
            .replace(/&/g, '&amp;')
            .replace(/</g, '&lt;')
            .replace(/>/g, '&gt;')
            .replace(/\n/g, '<br>');
            
        // Simple markdown to HTML conversion
        html = html.replace(/^# (.*$)/gm, '<h1>$1</h1>');
        html = html.replace(/^## (.*$)/gm, '<h2>$1</h2>');
        html = html.replace(/^### (.*$)/gm, '<h3>$1</h3>');
        html = html.replace(/\*\*(.*?)\*\*/g, '<strong>$1</strong>');
        html = html.replace(/\*(.*?)\*/g, '<em>$1</em>');
        html = html.replace(/__(.*?)__/g, '<strong>$1</strong>');
        html = html.replace(/_(.*?)_/g, '<em>$1</em>');
        html = html.replace(/\[(.+?)\]\((.+?)\)/g, '<a href="$2">$1</a>');
        
        return '<div style="font-family: -apple-system, BlinkMacSystemFont, \'Segoe UI\', Roboto, sans-serif; padding: 20px; color: #2c3e50;">' + html + '</div>';
    }
    
    // Function to toggle editor mode
    function toggleEditorMode() {
        if (currentMode === 0) {  // View mode
            currentMode = 1;  // Switch to edit
        } else if (currentMode === 1) {  // Edit mode
            currentMode = 2;  // Switch to split
        } else {  // Split mode
            currentMode = 0;  // Switch to view
        }
    }
    
    // Function to apply formatting
    function applyFormatting(type) {
        switch(type) {
            case "bold":
                insertAtCursor("**text**");
                break;
            case "italic":
                insertAtCursor("*text*");
                break;
            case "heading1":
                insertAtLineStart("# ");
                break;
            case "heading2":
                insertAtLineStart("## ");
                break;
            case "heading3":
                insertAtLineStart("### ");
                break;
            case "code":
                insertAtCursor("```\ncode\n```");
                break;
            case "link":
                insertAtCursor("[text](url)");
                break;
        }
    }
    
    // Helper function to insert text at cursor position
    function insertAtCursor(text) {
        if (isEditMode) {
            var cursorPos = editTextArea.cursorPosition;
            var before = editTextArea.text.substring(0, cursorPos);
            var after = editTextArea.text.substring(cursorPos);
            editTextArea.text = before + text + after;
            editTextArea.cursorPosition = cursorPos + text.length / 2;
        } else if (isSplitMode) {
            var cursorPos = splitEditTextArea.cursorPosition;
            var before = splitEditTextArea.text.substring(0, cursorPos);
            var after = splitEditTextArea.text.substring(cursorPos);
            splitEditTextArea.text = before + text + after;
            splitEditTextArea.cursorPosition = cursorPos + text.length / 2;
        }
    }
    
    // Helper function to insert text at beginning of current line
    function insertAtLineStart(text) {
        if (isEditMode) {
            var lines = editTextArea.text.split('\n');
            var lineIndex = 0;
            var pos = 0;
            
            // Find which line the cursor is on
            for (var i = 0; i < lines.length; i++) {
                if (pos + lines[i].length >= editTextArea.cursorPosition) {
                    lineIndex = i;
                    break;
                }
                pos += lines[i].length + 1; // +1 for newline character
            }
            
            lines[lineIndex] = text + lines[lineIndex];
            editTextArea.text = lines.join('\n');
        } else if (isSplitMode) {
            var lines = splitEditTextArea.text.split('\n');
            var lineIndex = 0;
            var pos = 0;
            
            // Find which line the cursor is on
            for (var i = 0; i < lines.length; i++) {
                if (pos + lines[i].length >= splitEditTextArea.cursorPosition) {
                    lineIndex = i;
                    break;
                }
                pos += lines[i].length + 1; // +1 for newline character
            }
            
            lines[lineIndex] = text + lines[lineIndex];
            splitEditTextArea.text = lines.join('\n');
        }
    }
    
    // Update content when file changes
    Component.onCompleted: {
        // Initialize with view mode
        currentMode = 0;
    }
}