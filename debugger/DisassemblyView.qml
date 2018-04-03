import QtQuick 2.0
import Fla 1.0

Item {
    visible: true
    id:disasmView
    readonly property real disasmLineHeight:16
    readonly property color backgroundColor: "#22313F"
    readonly property color centerColor: Qt.darker("#22313F",1.3)
    readonly property real centerIntructionOrigin: disassembler.paddingInstructionCount * disasmLineHeight
    readonly property int linkRegister:CPU.regs[14].value & 0xFFFFFFFE
    property alias memoryRegion: disassembler.memoryRegion
    Disassembler {
        id: disassembler
        property AddressTracker centerTracker: null
        memoryRegion:CPU.memoryRegions[1]
        paddingInstructionCount: Math.floor((Math.max(0,instructionList.height) / disasmView.disasmLineHeight) / 2)
        centerAddress: centerTracker.address
    }

    Component.onCompleted: {

        var pcTracker = addressTrackers.addTracker("PC", "#123b56");
        pcTracker.address = Qt.binding(function() { return CPU.regs[15].value });

        var lrTracker = addressTrackers.addTracker("LR", "#446CB3");
        lrTracker.address = Qt.binding(function() { return CPU.regs[14].value & 0xFFFFFFFE });

        var spTracker = addressTrackers.addTracker("SP", "#1E824C");
        spTracker.address = Qt.binding(function() { return CPU.regs[13].value });

        disassembler.centerTracker = pcTracker;
    }

    Rectangle {
        anchors.fill: parent
        color:Qt.darker("#11181f")
        //border.color: disasmView.backgroundColor
    }


    AddressTrackerList {
        id: addressTrackers
    }

    Rectangle {
        anchors.fill: instructionList
        color:"#11181f"
        border.color: disasmView.backgroundColor
    }

    Row {
        id:trackerControl
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height:20
        Repeater {
            anchors.fill: parent
            model:addressTrackers.trackers
            delegate: Item {
                width:40
                height:20
                property bool isCenteredTracker: modelData === disassembler.centerTracker
                Rectangle {
                    anchors.fill: parent
                    color:modelData.color
                    border.width: isCenteredTracker ? 1 : 0
                    border.color: isCenteredTracker ? "black" : disasmView.backgroundColor
                }

                Text {
                    anchors.fill: parent
                    font.family: "monospace"
                    width:40
                    text:label
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    color:"#ddd"
                    font.weight: isCenteredTracker ? Font.Bold : Font.Normal
                }

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        disassembler.centerTracker = modelData
                    }
                }
            }
        }
    }

    function findTracker(address) {
        for(var i = 0; i < addressTrackers.trackers.length; i++) {
            if(addressTrackers.trackers[i].address === address) {
                return addressTrackers.trackers[i];
            }
        }
        return null;
    }

    Column {
        anchors.top: trackerControl.bottom
        anchors.bottom:parent.bottom
        width:parent.width
        id:instructionList

        Repeater {
            anchors.fill: parent
            model:disassembler.instructions
            delegate: Item {
                width:500
                height:disasmView.disasmLineHeight
                property AddressTracker tracker: disasmView.findTracker(address)
                property bool pointedByTracker: tracker !== null
                Rectangle {

                    anchors.fill: parent
                    color: pointedByTracker ? tracker.color : "black"
                    visible: pointedByTracker
                    width:parent.width

                    Text {
                        x:8
                        text:pointedByTracker ? tracker.label : "..."
                        font.family: "monospace"
                        horizontalAlignment: Text.AlignLeft
                        width:80
                        height:parent.height
                        color: "white"
                    }
                }
                Text {
                    x:32
                    text:hexAddress
                    font.family: "monospace"
                    horizontalAlignment: Text.AlignLeft
                    width:80
                    height:parent.height
                    color: pointedByTracker ? "#ddd" : "#555"
                }
                Text {
                    x:115
                    id:instructionText
                    text:code
                    font.family: "monospace"
                    horizontalAlignment: Text.AlignLeft
                    width:400
                    height:parent.height
                    color:"#ddd"
                }

                Text {
                    x:370
                    id:dataText
                    text:modelData.data
                    font.family: "monospace"
                    horizontalAlignment: Text.AlignLeft
                    width:400
                    height:parent.height
                    color:"#555"
                }
            }
        }
    }

    MouseArea {
        anchors.fill: instructionList
        onWheel: {
            var newPos = 0;
            if(wheel.angleDelta.y > 0) {
                disassembler.scrollUp();
            } else {
                disassembler.scrollDown();
            }
        }

        onClicked: {
            var listPoint = instructionList.mapFromItem(instructionList, mouse.x, mouse.y);
            var index = Math.floor(listPoint.y / disasmView.disasmLineHeight);
            CPU.toggleBreakpoint(disassembler.instructions[index].address);
        }
    }
}
