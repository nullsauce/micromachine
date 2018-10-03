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
	color: "#1d1d1d"

	Component.onCompleted: disasmWindowCreator.createInstance()


	BreakpointsWindowCreator {
		id:breakpointsWindowCreator
		breakpointRegistry: CPU.breakpointRegistry
	}

	DisasmWindowCreator {
		id:disasmWindowCreator
		breakpointRegistry: CPU.breakpointRegistry
		memoryRegion: CPU.memoryRegions[1]
	}

	MemoryWindowCreator {
		id:memoryWindowCreator
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
		repeat: false
		interval: 16
		Component.onCompleted: {
			CPU.reset();
		}

		onTriggered: {
			if(false === CPU.step(iterationSlider.value)) {
				clock.stop();
			} else {
				clock.start();
			}
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
		Button {
			text:"memory"
			onClicked: memoryWindowCreator.createInstance()
		}

		Slider {
			id: iterationSlider
			from:1
			to:600000
			stepSize:10000
			width:200
		}
	}

	Icon {
		anchors.centerIn: parent
		source: "icons/memory.svg"
		color: "#333"
		width:300
		height:300
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

					UintDisplay {
						value: modelData.value
						id:regText
					}
					/*
					WordDisplay {
						id:regText
						text:hexValue
						color:"#ddd"
					}*/
				}
			}
		}
	}


}
