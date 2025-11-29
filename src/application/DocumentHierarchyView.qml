// DocumentHierarchyView.qml
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: hierarchyView
    
    property string rootPath: ""
    property alias model: treeView.model
    
    ColumnLayout {
        anchors.fill: parent
        
        // Header
        Label {
            text: "Document Hierarchy"
            font.bold: true
            Layout.fillWidth: true
            color: "#2c3e50"
        }
        
        // Tree view for document hierarchy
        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            
            TreeView {
                id: treeView
                anchors.fill: parent
                
                model: ListModel {
                    // This would be populated by the DocumentLinker
                }
                
                delegate: Item {
                    id: item
                    height: 30
                    
                    Rectangle {
                        anchors.fill: parent
                        color: "transparent"
                        border.color: "#e0e0e0"
                        
                        Row {
                            anchors.verticalCenter: parent.verticalCenter
                            x: 10 + (styleData.depth * 20) // Indent based on depth
                            spacing: 8
                            
                            // Expand/collapse icon for folders
                            Item {
                                width: 16
                                height: 16
                                
                                Text {
                                    anchors.centerIn: parent
                                    text: styleData.hasChildren && !styleData.isExpanded ? "▶" : 
                                          styleData.hasChildren && styleData.isExpanded ? "▼" : "○"
                                    font.pixelSize: 10
                                    visible: styleData.hasChildren
                                }
                                
                                MouseArea {
                                    anchors.fill: parent
                                    visible: parent.visible
                                    onClicked: {
                                        if (styleData.hasChildren) {
                                            if (treeView.isExpanded(styleData.index)) {
                                                treeView.collapse(styleData.index)
                                            } else {
                                                treeView.expand(styleData.index)
                                            }
                                        }
                                    }
                                }
                            }
                            
                            Image {
                                source: model.isFolder ? 
                                       (styleData.isExpanded ? "qrc:/icons/folder-open.svg" : "qrc:/icons/folder.svg") : 
                                       "qrc:/icons/document.svg"
                                width: 16
                                height: 16
                                anchors.verticalCenter: parent.verticalCenter
                            }
                            
                            Text {
                                text: model.display
                                anchors.verticalCenter: parent.verticalCenter
                                color: model.isFolder ? "#3498db" : "#7f8c8d"
                                font.bold: model.isFolder
                            }
                        }
                    }
                    
                    // Selection indicator
                    Rectangle {
                        width: 4
                        height: parent.height - 4
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.margins: 2
                        color: "#724f97" // iOS-7 purple accent
                        visible: styleData.selected
                    }
                }
                
                itemDelegate: Rectangle {
                    color: styleData.selected ? "#f0f0f0" : "transparent"
                    border.color: "#e0e0e0"
                }
            }
        }
    }
    
    // Function to populate the hierarchy
    function populateHierarchy(rootPath) {
        // This would call the DocumentLinker to build the hierarchy
        // and populate the model
    }
}