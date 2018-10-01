import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import Fla 1.0

Window {
	flags: Qt.Tool
	Component.onCompleted: show()
	property BreakpointRegistry breakpointRegistry

	Rectangle {
		anchors.fill: parent
		color: Qt.darker("#1d1d1d")
	}

	ListView {
		anchors.fill: parent
		model: breakpointRegistry.breakpoints
		delegate: Item {
			height: 20
			width: parent.width
			Row {
				spacing: 10
				Text {
					width:contentWidth
					color:"white"
					text:addressHex
				}
				CheckBox {
					id:enabledCheckbox
					enabled:false
					checked: modelData.enabled
				}
			}
			MouseArea {
				anchors.fill: parent
				onClicked: modelData.enabled = !modelData.enabled
			}
		}
	}
}
