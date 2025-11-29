// RichTextEditor.qml
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: richTextEditor
    
    property string content: ""
    property string filePath: ""
    property bool isEditing: false
    property alias textArea: markdownTextArea
    
    // Signals
    signal contentChanged(string content)
    signal formattingApplied(string formatType)
    signal elementInserted(string elementType)
    
    // Main layout
    ColumnLayout {
        anchors.fill: parent
        
        // Formatting toolbar
        ToolBar {
            id: formattingToolbar
            Layout.fillWidth: true
            height: 45
            visible: richTextEditor.isEditing
            
            background: Rectangle {
                color: "#ffffff"
                border.color: "#e0e0e0"
                border.width: 1
            }
            
            RowLayout {
                anchors.fill: parent
                spacing: 2
                
                // Basic formatting
                ToolButton {
                    text: "B"
                    font.bold: true
                    font.pixelSize: 16
                    width: 36
                    ToolTip.text: "Bold (Ctrl+B)"
                    ToolTip.visible: hovered
                    onClicked: applyFormatting("bold")
                    background: Rectangle {
                        border.width: 1
                        border.color: hovered ? "#a0a0a0" : "#c0c0c0"
                        radius: 3
                        color: "transparent"
                    }
                }
                
                ToolButton {
                    text: "I"
                    font.italic: true
                    font.pixelSize: 16
                    width: 36
                    ToolTip.text: "Italic (Ctrl+I)"
                    ToolTip.visible: hovered
                    onClicked: applyFormatting("italic")
                    background: Rectangle {
                        border.width: 1
                        border.color: hovered ? "#a0a0a0" : "#c0c0c0"
                        radius: 3
                        color: "transparent"
                    }
                }
                
                ToolButton {
                    text: "U"
                    font.underline: true
                    font.pixelSize: 16
                    width: 36
                    ToolTip.text: "Underline (Ctrl+U)"
                    ToolTip.visible: hovered
                    onClicked: applyFormatting("underline")
                    background: Rectangle {
                        border.width: 1
                        border.color: hovered ? "#a0a0a0" : "#c0c0c0"
                        radius: 3
                        color: "transparent"
                    }
                }
                
                Rectangle { width: 1; height: parent.height * 0.6; color: "#d0d0d0" }
                
                // Headings
                ComboBox {
                    model: ["Paragraph", "Heading 1", "Heading 2", "Heading 3"]
                    currentIndex: 0
                    width: 100
                    ToolTip.text: "Heading Level"
                    ToolTip.visible: hovered
                    onActivated: applyHeading(currentIndex)
                    
                    background: Rectangle {
                        border.width: 1
                        border.color: "#c0c0c0"
                        radius: 3
                        color: "#f8f8f8"
                    }
                }
                
                Rectangle { width: 1; height: parent.height * 0.6; color: "#d0d0d0" }
                
                // Lists
                ToolButton {
                    text: "•"
                    width: 36
                    ToolTip.text: "Bullet List"
                    ToolTip.visible: hovered
                    onClicked: applyFormatting("bulletList")
                    background: Rectangle {
                        border.width: 1
                        border.color: hovered ? "#a0a0a0" : "#c0c0c0"
                        radius: 3
                        color: "transparent"
                    }
                }
                
                ToolButton {
                    text: "1."
                    width: 36
                    ToolTip.text: "Numbered List"
                    ToolTip.visible: hovered
                    onClicked: applyFormatting("numberedList")
                    background: Rectangle {
                        border.width: 1
                        border.color: hovered ? "#a0a0a0" : "#c0c0c0"
                        radius: 3
                        color: "transparent"
                    }
                }
                
                Rectangle { width: 1; height: parent.height * 0.6; color: "#d0c0c0" }
                
                // Special blocks
                ToolButton {
                    text: "C"
                    width: 36
                    ToolTip.text: "Code Block"
                    ToolTip.visible: hovered
                    onClicked: insertCodeBlock()
                    background: Rectangle {
                        border.width: 1
                        border.color: hovered ? "#a0a0a0" : "#c0c0c0"
                        radius: 3
                        color: "transparent"
                    }
                }
                
                ToolButton {
                    text: ">"
                    width: 36
                    ToolTip.text: "Blockquote"
                    ToolTip.visible: hovered
                    onClicked: applyFormatting("blockquote")
                    background: Rectangle {
                        border.width: 1
                        border.color: hovered ? "#a0a0a0" : "#c0c0c0"
                        radius: 3
                        color: "transparent"
                    }
                }
                
                ToolButton {
                    text: "🔗"
                    width: 36
                    ToolTip.text: "Insert Link"
                    ToolTip.visible: hovered
                    onClicked: insertLink()
                    background: Rectangle {
                        border.width: 1
                        border.color: hovered ? "#a0a0a0" : "#c0c0c0"
                        radius: 3
                        color: "transparent"
                    }
                }
                
                ToolButton {
                    text: "🖼️"
                    width: 36
                    ToolTip.text: "Insert Image"
                    ToolTip.visible: hovered
                    onClicked: insertImage()
                    background: Rectangle {
                        border.width: 1
                        border.color: hovered ? "#a0a0a0" : "#c0c0c0"
                        radius: 3
                        color: "transparent"
                    }
                }
                
                Rectangle { 
                    width: 1
                    height: parent.height * 0.6
                    color: "#d0d0d0"
                    Layout.alignment: Qt.AlignVCenter
                }
                
                // Language selector for code blocks
                ComboBox {
                    id: languageSelector
                    model: ["Plain Text", "C++", "Python", "JavaScript", "HTML", "CSS", "Java", "TypeScript", "Go", "Rust", "C#", "PHP", "SQL", "YAML", "JSON"]
                    width: 120
                    ToolTip.text: "Code Block Language"
                    ToolTip.visible: hovered
                    enabled: false  // Only enabled when in a code block
                    
                    background: Rectangle {
                        border.width: 1
                        border.color: "#c0c0c0"
                        radius: 3
                        color: "#f8f8f8"
                    }
                }
            }
        }
        
        // Main text editing area
        ScrollView {
            id: textEditorScrollView
            Layout.fillWidth: true
            Layout.fillHeight: true
            
            TextArea {
                id: markdownTextArea
                text: richTextEditor.content
                font.family: "Consolas, Monaco, 'Courier New', monospace"
                font.pixelSize: 14
                wrapMode: TextArea.Wrap
                selectByMouse: true
                padding: 10
                
                // Handle content changes
                onTextChanged: {
                    if (richTextEditor.content !== text) {
                        richTextEditor.content = text
                        contentChanged(text)
                    }
                }
                
                // Keyboard shortcuts
                Keys.onPressed: {
                    if (event.ctrl && event.key === Qt.Key_B) {
                        applyFormatting("bold");
                        event.accepted = true;
                    } else if (event.ctrl && event.key === Qt.Key_I) {
                        applyFormatting("italic");
                        event.accepted = true;
                    } else if (event.ctrl && event.key === Qt.Key_U) {
                        applyFormatting("underline");
                        event.accepted = true;
                    } else if (event.ctrl && event.key === Qt.Key_L) {
                        if (event.modifiers & Qt.AltModifier) {
                            applyFormatting("numberedList");
                        } else {
                            applyFormatting("bulletList");
                        }
                        event.accepted = true;
                    }
                }
            }
        }
    }
    
    // Functions for formatting
    function applyFormatting(formatType) {
        var selectionStart = markdownTextArea.selectionStart;
        var selectionEnd = markdownTextArea.selectionEnd;
        
        if (selectionStart === selectionEnd) {
            // No selection, insert formatting around cursor
            applyFormattingAtCursor(formatType);
        } else {
            // Apply formatting to selection
            applyFormattingToSelection(formatType, selectionStart, selectionEnd);
        }
        formattingApplied(formatType);
    }
    
    function applyFormattingAtCursor(formatType) {
        var cursorPos = markdownTextArea.cursorPosition;
        var beforeCursor = markdownTextArea.text.substring(0, cursorPos);
        var afterCursor = markdownTextArea.text.substring(cursorPos);
        
        var formatStart, formatEnd;
        
        switch (formatType) {
            case "bold":
                formatStart = "**";
                formatEnd = "**";
                break;
            case "italic":
                formatStart = "*";
                formatEnd = "*";
                break;
            case "underline":
                formatStart = "__";
                formatEnd = "__";
                break;
            case "code":
                formatStart = "`";
                formatEnd = "`";
                break;
            case "blockquote":
                // Insert at beginning of line
                var lines = beforeCursor.split('\n');
                var lastLine = lines[lines.length - 1];
                lines[lines.length - 1] = "> " + lastLine;
                beforeCursor = lines.join('\n');
                break;
            case "bulletList":
                // Insert at beginning of line
                var lines = beforeCursor.split('\n');
                var lastLine = lines[lines.length - 1];
                lines[lines.length - 1] = "* " + lastLine;
                beforeCursor = lines.join('\n');
                break;
            case "numberedList":
                // Insert at beginning of line
                var lines = beforeCursor.split('\n');
                var lastLine = lines[lines.length - 1];
                lines[lines.length - 1] = "1. " + lastLine;
                beforeCursor = lines.join('\n');
                break;
            default:
                return;
        }
        
        if (formatStart && formatEnd) {
            markdownTextArea.text = beforeCursor + formatStart + formatEnd + afterCursor;
            markdownTextArea.cursorPosition = cursorPos + formatStart.length;
        } else {
            markdownTextArea.text = beforeCursor + afterCursor;
            markdownTextArea.cursorPosition = cursorPos;
        }
    }
    
    function applyFormattingToSelection(formatType, start, end) {
        var selectedText = markdownTextArea.text.substring(start, end);
        var beforeSelection = markdownTextArea.text.substring(0, start);
        var afterSelection = markdownTextArea.text.substring(end);
        
        var formatStart, formatEnd;
        
        switch (formatType) {
            case "bold":
                formatStart = "**";
                formatEnd = "**";
                break;
            case "italic":
                formatStart = "*";
                formatEnd = "*";
                break;
            case "underline":
                formatStart = "__";
                formatEnd = "__";
                break;
            case "code":
                formatStart = "`";
                formatEnd = "`";
                break;
            case "blockquote":
                var lines = selectedText.split('\n');
                for (var i = 0; i < lines.length; i++) {
                    if (lines[i].trim() !== "") {
                        lines[i] = "> " + lines[i];
                    }
                }
                markdownTextArea.text = beforeSelection + lines.join('\n') + afterSelection;
                markdownTextArea.cursorPosition = start;
                markdownTextArea.select(start, start + lines.join('\n').length);
                return;
            case "bulletList":
                var lines = selectedText.split('\n');
                for (var i = 0; i < lines.length; i++) {
                    if (lines[i].trim() !== "") {
                        lines[i] = "* " + lines[i];
                    }
                }
                markdownTextArea.text = beforeSelection + lines.join('\n') + afterSelection;
                markdownTextArea.cursorPosition = start;
                markdownTextArea.select(start, start + lines.join('\n').length);
                return;
            case "numberedList":
                var lines = selectedText.split('\n');
                for (var i = 0; i < lines.length; i++) {
                    if (lines[i].trim() !== "") {
                        lines[i] = (i + 1) + ". " + lines[i];
                    }
                }
                markdownTextArea.text = beforeSelection + lines.join('\n') + afterSelection;
                markdownTextArea.cursorPosition = start;
                markdownTextArea.select(start, start + lines.join('\n').length);
                return;
            default:
                return;
        }
        
        markdownTextArea.text = beforeSelection + formatStart + selectedText + formatEnd + afterSelection;
        markdownTextArea.cursorPosition = start + formatStart.length;
        markdownTextArea.select(start + formatStart.length, start + formatStart.length + selectedText.length);
    }
    
    function applyHeading(level) {
        var cursorPos = markdownTextArea.cursorPosition;
        var text = markdownTextArea.text;
        
        // Find the current line
        var beforeCursor = text.substring(0, cursorPos);
        var linesBefore = beforeCursor.split('\n');
        var currentLineIndex = linesBefore.length - 1;
        var lines = text.split('\n');
        
        // Remove existing heading markers
        lines[currentLineIndex] = lines[currentLineIndex].replace(/^[#]*\s*/, '');
        
        // Add new heading marker based on level
        if (level > 0) {
            var headingPrefix = '#'.repeat(level) + ' ';
            lines[currentLineIndex] = headingPrefix + lines[currentLineIndex];
        }
        
        markdownTextArea.text = lines.join('\n');
        markdownTextArea.cursorPosition = calculateCursorPosition(lines, currentLineIndex, cursorPos);
    }
    
    function insertCodeBlock() {
        var cursorPos = markdownTextArea.cursorPosition;
        var beforeCursor = markdownTextArea.text.substring(0, cursorPos);
        var afterCursor = markdownTextArea.text.substring(cursorPos);
        
        var codeBlock = "\n```\n\n```\n";
        markdownTextArea.text = beforeCursor + codeBlock + afterCursor;
        markdownTextArea.cursorPosition = cursorPos + 5; // Position inside the code block
        elementInserted("codeBlock");
    }
    
    function insertLink() {
        var cursorPos = markdownTextArea.cursorPosition;
        var beforeCursor = markdownTextArea.text.substring(0, cursorPos);
        var afterCursor = markdownTextArea.text.substring(cursorPos);
        
        var linkSyntax = "[link text](url)";
        markdownTextArea.text = beforeCursor + linkSyntax + afterCursor;
        markdownTextArea.cursorPosition = cursorPos + 1; // Position at "link text"
        markdownTextArea.select(cursorPos + 1, cursorPos + 9); // Select "link text"
        elementInserted("link");
    }
    
    function insertImage() {
        var cursorPos = markdownTextArea.cursorPosition;
        var beforeCursor = markdownTextArea.text.substring(0, cursorPos);
        var afterCursor = markdownTextArea.text.substring(cursorPos);
        
        var imageSyntax = "![alt text](image_url)";
        markdownTextArea.text = beforeCursor + imageSyntax + afterCursor;
        markdownTextArea.cursorPosition = cursorPos + 2; // Position at "alt text"
        markdownTextArea.select(cursorPos + 2, cursorPos + 10); // Select "alt text"
        elementInserted("image");
    }
    
    // Helper function to recalculate cursor position after text modification
    function calculateCursorPosition(lines, lineIndex, originalCursorPos) {
        var position = 0;
        for (var i = 0; i < lineIndex; i++) {
            position += lines[i].length + 1; // +1 for newline character
        }
        return position;
    }
    
    // Set content from external source
    function setContent(newContent) {
        markdownTextArea.text = newContent;
        richTextEditor.content = newContent;
    }
    
    // Toggle editing mode
    function toggleEditing() {
        richTextEditor.isEditing = !richTextEditor.isEditing;
    }
}