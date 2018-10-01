import QtQuick 2.0
import QtQuick.Window 2.2
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
			Text {
				color:"grey"
				text: address
			}
		}
	}
}
