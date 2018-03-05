import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
import Fla 1.0

ApplicationWindow {
    id:window
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    color:"#111"

    function zpad(n, width) {
        n = n + '';
        return n.length >= width ? n : new Array(width - n.length + 1).join('0') + n;
    }

    function hex32(address) {
        return zpad(address.toString(16),8);
    }

    Binding {
        target:CPU
        property: "desiredInstructionCount"
        value:(window.height/16)-2
    }



    Timer {
        id:clock
        running: false
        repeat: true
        interval: 16
        Component.onCompleted: {
            CPU.reset();
        }

        onTriggered: {
            if(false === CPU.step(1000)) {
                // breakpoint reached
                clock.stop();
            }
        }
        onRunningChanged: {
            //if(running) {

            //}
        }
    }

    Row {
        id:controls
        anchors.top: parent.top
        Button {
            text:clock.running ? "stop" : "play"
            onClicked: {
                if(clock.running) {
                    clock.stop();
                } else {
                    clock.start();
                }
            }
        }
        Button {
            text:"step"
            onClicked: {
                CPU.step(1);
            }
        }
        Button {
            text:"reset"
            onClicked: CPU.reset();
        }
    }


    Window {
        visible: true
        width:800
        height:600
        id:disasmView
        readonly property real disasmLineHeight:16
        property int test:CPU.regs[15].value
        readonly property color backgroundColor: "#22313F"
        readonly property color centerColor: Qt.darker("#22313F",1.3)
        readonly property real centerIntructionOrigin: disassembler.paddingInstructionCount * disasmLineHeight

        Rectangle {
            anchors.fill: parent
            color:"#11181f"
            border.color: disasmView.backgroundColor
        }

        Disassembler {
            id:disassembler
            memoryRegion:CPU.memoryRegions[1]
            paddingInstructionCount: Math.floor((instructionList.height / disasmView.disasmLineHeight) / 2)
            centerAddress:CPU.regs[15].value

        }

        Item {
            y:disasmView.centerIntructionOrigin
            height:disasmView.disasmLineHeight
            width:50

            Text {
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignRight
                text:"->"
                font.family: "monospace"
                color:"#ddd"
            }
        }

        Column {
            anchors.fill: parent
            anchors.leftMargin: 80
            id:instructionList

            Repeater {
                anchors.fill: parent
                model:disassembler.instructions
                delegate: Item {
                    width:500
                    height:disasmView.disasmLineHeight

                    Rectangle {
                        anchors.fill: parent
                        color:disasmView.centerColor
                        visible: address === disassembler.centerAddress
                        width:parent.width
                    }
                    Rectangle {
                        anchors.fill: parent
                        color:"#123b56"
                        visible: address === disasmView.test
                        width:parent.width
                    }
                    Text {
                        text:hexAddress
                        font.family: "monospace"
                        horizontalAlignment: Text.AlignLeft
                        width:80
                        height:parent.height
                        color: address === disassembler.centerAddress ? "#ddd" : "#555"
                    }
                    Text {
                        x:90
                        id:instructionText
                        text:code
                        font.family: "monospace"
                        horizontalAlignment: Text.AlignLeft
                        width:400
                        height:parent.height
                        color:"#ddd"
                    }
                }
            }
        }
        /* optim for center marker
        Rectangle {
            width:500
            height:disasmView.disasmLineHeight
            color:"red"
            y:disassembler.paddingInstructionCount * height
        }*/
        MouseArea {
            anchors.fill: parent
            onWheel: {
                var newPos = 0;
                if(wheel.angleDelta.y > 0) {
                    disassembler.scrollUp();
                } else {
                    disassembler.scrollDown();
                }

            }
        }
    }

    Rectangle {
        anchors.fill: disasm
        color:"#11181f"
        border.color: "#22313F"
    }

    Column {
        id:disasm
        anchors.left: parent.left
        anchors.top: controls.bottom
        anchors.bottom: parent.bottom

        Repeater {
            model:CPU.instructions

            Item {
                id:instructionItem
                height:16
                width:420
                property bool isCurrent: address === CPU.currentPC
                Rectangle {
                    height:instrRow.height
                    color:"#123b56"
                    visible: isCurrent
                    width:parent.width
                }

                Row {
                    id:instrRow
                    spacing: 0
                    height:parent.height

                    Item {
                        width:15
                        height:parent.height
                        Rectangle {
                            anchors.top: parent.top
                            anchors.bottom: parent.bottom
                            anchors.left: parent.left
                            anchors.margins: 3
                            width:height
                            radius:height*0.5
                            color:isBreakPoint ? "yellow" : Qt.lighter("#11181f")
                        }
                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                CPU.setBreakPoint(address, !isBreakPoint);
                            }
                        }
                    }

                    Text {
                        text:hexAddress
                        font.family: "monospace"
                        horizontalAlignment: Text.AlignLeft
                        width:80
                        height:instructionItem.height
                        color:"#555"
                    }
                    Text {
                        id:instructionText
                        text:code
                        font.family: "monospace"
                        horizontalAlignment: Text.AlignLeft
                        width:contentWidth * 1.1
                        height:instructionItem.height
                        color:"#ddd"
                    }
                }
            }
        }
    }

    RowLayout {
        anchors.left: disasm.right
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: registers.top

        Item {
            height:parent.height
            Layout.maximumWidth : 900
            Layout.alignment: Qt.AlignRight
            Layout.fillWidth: true
            Layout.fillHeight: true
            id:memoryViewPanel
            TabBar {
                id:memoryRegionTab
                anchors.top: parent.top
                Repeater {
                    model:CPU.memoryRegions
                    TabButton {
                        width: Math.max(100, memoryViewPanel.width / CPU.memoryRegions.length)
                        text: name
                    }
                }
            }

            StackLayout {
                width:parent.width
                anchors.top: memoryRegionTab.bottom
                anchors.bottom: parent.bottom
                currentIndex: memoryRegionTab.currentIndex
                Repeater {
                    model:CPU.memoryRegions
                    Item {
                        anchors.fill: parent
                        Memoryview {
                            anchors.fill: parent
                            memoryRegion:modelData
                        }
                    }
                }
            }
        }
    }

    Item {
        id:registers
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 8
        width:500
        height:100
        Rectangle {
            anchors.fill: parent
            color:"#11181f"
            border.color: "#22313F"
        }


        GridLayout {
            id:registerGrid
            anchors.fill: parent
            rows:4
            columns:4
            flow: GridLayout.TopToBottom
            Repeater {
                model:CPU.regs
                Row {
                    height:regText.height
                    spacing: 8
                    Text {
                        text:name
                        font.family: "monospace"
                        horizontalAlignment: Text.AlignRight
                        width:30
                        height:contentHeight
                        color:"#999"
                    }
                    Text {
                        id:regText
                        text:hexValue
                        font.family: "monospace"
                        horizontalAlignment: Text.AlignLeft
                        width:contentWidth * 1.1
                        height:contentHeight
                        color:"#ddd"
                    }
                }
            }
        }
    }


}
