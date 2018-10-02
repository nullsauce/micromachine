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
			height: 20
			anchors.left: parent.left
			anchors.right: parent.right
			Row {
				anchors.fill: parent
				anchors.margins: 4
				spacing: 14
				UintDisplay {
					value:address
					color: "#6c6c6c";
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
