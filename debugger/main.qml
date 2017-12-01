import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    color:"#111"
    ListModel {
        ListElement {
            address:0
            text:"add r1, r2"
        }
    }

    function zpad(n, width) {
      n = n + '';
      return n.length >= width ? n : new Array(width - n.length + 1).join('0') + n;
    }


    Column {

        Repeater {
            model:CPU.instructions

            Item {
                height:instructionText.height
                width:300
                property bool isCurrent: index == 0
                Rectangle {
                    anchors.fill: instrRow
                    color:"#123b56"
                    visible: isCurrent
                }

                Row {
                    id:instrRow
                    spacing: 10
                    Text {
                        text:hexAddress
                        font.family: "monospace"
                        horizontalAlignment: Text.AlignRight
                        width:80
                        height:contentHeight
                        color:"#555"
                    }
                    Text {
                        id:instructionText
                        text:code
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

    Column {
        anchors.right: parent.right
        Repeater {
            model:CPU.regs

            Row {
                height:regText.height
                spacing: 10
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

    Row {
        anchors.right: parent.right
        anchors.bottom: parent.bottom
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
            onClicked: CPU.step()
        }
        Button {
            text:"reset"
            onClicked: CPU.reset()
        }
    }

    Timer {
        id:clock
        running: false
        repeat: true
        interval: 16
        onTriggered: CPU.step()
        onRunningChanged: {
            if(running) {
                CPU.reset()
            }
        }
    }
}
