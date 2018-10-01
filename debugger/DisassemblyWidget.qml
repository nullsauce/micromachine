import QtQuick 2.0
import Fla 1.0
import QtQuick.Window 2.2


Window {
	id: disasmView
	flags: Qt.Tool
	Component.onCompleted: show()
	property alias breakpointRegistry: disassemblyView.breakpointRegistry
	property alias memoryRegion: disassemblyView.memoryRegion

	Rectangle {
		anchors.fill: parent
		color: Qt.darker("#1d1d1d")
	}

	Row {
		id: controlbar
		anchors.top: parent.top
		anchors.left: parent.left
		anchors.right: parent.right
		height: 20
		Repeater {
			anchors.fill: parent
			model: disassemblyView.addressTrackers
			delegate: Item {
				width: 40
				height: 20
				property bool isActiveTracker: modelData === disassemblyView.baseAddressRef
				Rectangle {
					anchors.fill: parent
					color: modelData.color
					border.width: isActiveTracker ? 1 : 0
					border.color: isActiveTracker ? "black" : "red"
				}

				Text {
					anchors.fill: parent
					font.family: "monospace"
					width: 40
					text: label
					horizontalAlignment: Text.AlignHCenter
					verticalAlignment: Text.AlignVCenter
					color: "#ddd"
					font.weight: isActiveTracker ? Font.Bold : Font.Normal
				}

				MouseArea {
					anchors.fill: parent
					cursorShape: Qt.PointingHandCursor
					onClicked: disassemblyView.baseAddressRef = modelData
				}
			}
		}
	}

	DisassemblyView {
		id: disassemblyView
		width: parent.width
		anchors.top: controlbar.bottom
		anchors.bottom: parent.bottom
		property AddressTracker baseAddressRef: pagedTracker
		//memoryRegion: CPU.memoryRegions[1]
		//breakpointRegistry: CPU.breakpointRegistry
		baseAddress: baseAddressRef.address
		Component.onCompleted: {
			console.log(memoryRegion)
		}

		addressTrackers: [
			AddressTracker {
				id: pagedTracker
				label: "Co"
				color: "#00000000"
				address: Math.floor(pcTracker.address / (2 * disassemblyView.maxVisibleInstructionCount))
						 * (2 * disassemblyView.maxVisibleInstructionCount)
			},
			AddressTracker {
				id: pcTracker
				label: "PC"
				color: "#123b56"
				address: CPU.regs[15].value
			},
			AddressTracker {
				label: "LR"
				color: "#446CB3"
				address: CPU.regs[14].value & 0xFFFFFFFE
			},
			AddressTracker {
				label: "SP"
				color: "#1E824C"
				address: CPU.regs[13].value
			}
		]

		MouseArea {
			id: mouseArea
			anchors.fill: parent
			onWheel: {
				var newPos = 0
				if (wheel.angleDelta.y > 0) {
					disassemblyView.scrollUp()
				} else {
					disassemblyView.scrollDown()
				}
			}
			onClicked: {
				var listPoint = disassemblyView.mapFromItem(mouseArea,
															mouse.x, mouse.y)
				var address = disassemblyView.addressAtYPos(listPoint.y)
				console.log("address", address)
				CPU.breakpointRegistry.toggleBreakpoint(address)
			}
		}
	}

	/*
	Rectangle {
		width:80
		height:20
		anchors.top:parent.top
		TextInput {
			id:disasmAddressInput
			anchors.fill:parent
			text:"00000000"
			horizontalAlignment: Text.AlignRight
			maximumLength: 8

			validator: RegExpValidator { regExp: /[0-9A-Fa-f]+/ }
			onTextChanged: {
				if(acceptableInput) {
					freeTrackerAddress = parseInt("0x"+text, 16);
				}
			}
		}
	}*/
}
