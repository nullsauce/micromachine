import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import Fla 1.0

ToolWindow {

	Component.onCompleted: show()
	property BreakpointRegistry breakpointRegistry

	content: ListView {
		anchors.fill: parent
		model: breakpointRegistry.breakpoints
		delegate: Item {
			height: 30
			anchors.left: parent.left
			anchors.right: parent.right
			Row {
				id: infos
				anchors.right: controls.left
				anchors.left: parent.left
				anchors.top: parent.top
				anchors.bottom: parent.bottom
				anchors.margins: 4
				spacing: 14
				Rectangle {
					anchors.fill: parent
					color: "#123b56"
					visible: reached
				}
				UintDisplay {
					value:address
				}
				CheckBox {
					id:enabledCheckbox
					enabled:false
					checked: modelData.enabled
				}
			}

			MouseArea {
				id: lineMouseArea
				anchors.fill: parent
				hoverEnabled: true
				onClicked: modelData.enabled = !modelData.enabled
			}

			Item {
				id:controls
				visible: lineMouseArea.containsMouse
				width:60
				anchors.right: parent.right
				height: parent.height
				IconButton {
					source: "icons/cancel-circle.svg"
					anchors.top: parent.top
					anchors.bottom: parent.bottom
					anchors.right: parent.right
					anchors.margins: 2
					width:height
					onClicked: breakpointRegistry.destroyBreakpoint(modelData.address)
				}
			}
		}
	}
}
