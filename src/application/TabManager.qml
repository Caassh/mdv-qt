import QtQuick
import QtQuick.Controls

TabView {
    id: tabView
    
    property alias tabModel: tabManager.model
    
    // Tab bar with custom styling
    tabBar: TabBar {
        id: tabBar
        background: Rectangle {
            color: "#f8f9fa"
            border.color: "#e0e0e0"
            border.width: 1
        }
        
        contentItem: ListView {
            model: tabManager.model
            orientation: ListView.Horizontal
            boundsBehavior: Flickable.DragOverBounds
            leftMargin: 5
            spacing: 1
            
            // Custom delegate for tabs
            delegate: TabButton {
                width: 150
                height: 30
                text: model.title
                icon: model.icon
                checked: tabView.currentIndex === index
                onCloseRequested: {
                    // Request to close the tab
                    tabManager.closeTab(index)
                }
            }
        }
    }
    
    // Content area for tab content
    contentItem: StackView {
        id: stackView
        currentIndex: tabView.currentIndex
        
        model: tabManager.model
        
        delegate: StackViewDelegate {
            pushTransition: StackViewTransition {
                PropertyAnimation {
                    property: "x"
                    from: stackView.currentItem.width
                    to: 0
                    duration: 200
                }
            }
            popTransition: StackViewTransition {
                PropertyAnimation {
                    property: "x"
                    from: 0
                    to: stackView.currentItem.width
                    duration: 200
                }
            }
        }
    }
    
    // Function to add a new tab
    function addTab(title, component, data) {
        return tabManager.addTab(title, component, data);
    }
    
    // Function to close a tab
    function closeTab(index) {
        tabManager.closeTab(index);
    }
}

// Custom tab button with close functionality
Item {
    id: tabButton
    implicitWidth: 150
    implicitHeight: 30
    
    signal closeRequested()
    
    Rectangle {
        id: background
        anchors.fill: parent
        color: tabButton.checked ? "#ffffff" : "#e9ecef"
        border.color: "#c8c8c8"
        border.width: 1
        radius: 3
        
        // Add top and bottom border only (not left/right to make tabs appear connected)
        Rectangle {
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: 1
            color: "#c8c8c8"
        }
    }
    
    Row {
        anchors.fill: parent
        anchors.leftMargin: 8
        anchors.rightMargin: 8
        
        Label {
            id: titleLabel
            text: tabButton.text
            elide: Label.ElideRight
            verticalAlignment: Qt.AlignVCenter
            anchors.verticalCenter: parent.verticalCenter
        }
        
        Item {
            width: 20
            height: 20
            anchors.verticalCenter: parent.verticalCenter
            
            Rectangle {
                anchors.centerIn: parent
                width: 16
                height: 16
                radius: 8
                color: "transparent"
                border.color: "#999999"
                border.width: 1
                visible: tabButton.hovered
                
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: tabButton.closeRequested()
                    visible: parent.visible
                    
                    Text {
                        anchors.centerIn: parent
                        text: "×"
                        font.pixelSize: 12
                        color: "#999999"
                    }
                }
            }
        }
    }
    
    states: State {
        name: "checked"
        when: tabButton.checked
        PropertyChanges {
            target: background
            color: "#ffffff"
        }
    }
    
    transitions: Transition {
        ColorAnimation { duration: 100 }
    }
}