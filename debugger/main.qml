import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
import Fla 1.0

ApplicationWindow {
	id:appWindow
	visible: true
	width: 1280
	height: 720
	title: qsTr("Hello World")
	color:"#111"

	Rectangle {
		anchors.fill: parent
		color: Qt.darker("#1d1d1d")
	}


	BreakpointsWindowCreator {
		id:breakpointsWindowCreator
		breakpointRegistry: CPU.breakpointRegistry
	}

	DisasmWindowCreator {
		id:disasmWindowCreator
		breakpointRegistry: CPU.breakpointRegistry
		memoryRegion: CPU.memoryRegions[1]
	}

	function zpad(n, width) {
		n = n + '';
		return n.length >= width ? n : new Array(width - n.length + 1).join('0') + n;
	}

	function hex32(address) {
		return zpad(address.toString(16),8);
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
		Button {
			text:"breakpoints"
			onClicked: breakpointsWindowCreator.createInstance()
		}
		Button {
			text:"code"
			onClicked: disasmWindowCreator.createInstance()
		}
	}

	/*
	Rectangle {
		anchors.fill: disasm
		color:"#11181f"
		border.color: "#22313F"
	}*/
	/*
	DisassemblyWidget {
		id:disasm
		anchors.left: parent.left
		width:500
		anchors.top: controls.bottom
		anchors.bottom: parent.bottom
	}*/

	RowLayout {

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

					WordDisplay {
						id:regText
						text:hexValue
						color:"#ddd"
					}
				}
			}
		}
	}


}
