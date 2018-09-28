import QtQuick 2.0
import Fla 1.0

Item {

	id: disasmView
	property int freeTrackerAddress: 0

	Rectangle {
		anchors.fill: parent
		color: Qt.darker("#1d1d1d")
	}

	AddressTrackerList {
		id: addressTrackers
	}

	function findTracker(address) {
		for (var i = 0; i < addressTrackers.trackers.length; i++) {
			if (addressTrackers.trackers[i].address === address) {
				return addressTrackers.trackers[i]
			}
		}
		return null
	}

	Row {
		id: controlbar
		anchors.top: parent.top
		anchors.left: parent.left
		anchors.right: parent.right
		height: 20

		Repeater {
			anchors.fill: parent
			model: addressTrackers.trackers
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
					onClicked: disassemblyView.centerTracker = modelData
				}
			}
		}
	}

	DisassemblyView {
		id: disassemblyView
		width: parent.width
		anchors.top: controlbar.bottom
		anchors.bottom: parent.bottom

		property AddressTracker baseAddressRef: pcTracker
		memoryRegion: CPU.memoryRegions[1]
		breakpointRegistry: CPU.breakpointRegistry
		baseAddress: Math.floor(
						 baseAddressRef.address / (2 * maxVisibleInstructionCount))
					 * (2 * maxVisibleInstructionCount)
		onBaseAddressChanged: console.log("base", baseAddress)
		addressTrackers: [
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

		Component.onCompleted: {

			var pcTracker = addressTrackers.addTracker("PC", "#123b56")
			pcTracker.address = Qt.binding(function () {
				return CPU.regs[15].value
			})

			var lrTracker = addressTrackers.addTracker("LR", "#446CB3")
			lrTracker.address = Qt.binding(function () {
				return CPU.regs[14].value & 0xFFFFFFFE
			})

			var spTracker = addressTrackers.addTracker("SP", "#1E824C")
			spTracker.address = Qt.binding(function () {
				return CPU.regs[13].value
			})

			var freeTracker = addressTrackers.addTracker("Ma", "#f0932b")
			freeTracker.address = Qt.binding(function () {
				return disasmView.freeTrackerAddress
			})

			disassemblyView.baseAddressRef = pcTracker
		}

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
