import QtQuick 2.0

Item {
	id: control
	property alias enabled: mouseArea.enabled
	property alias source: icon.source
	readonly property color idleColor: "#123b56"
	readonly property color activeColor: Qt.lighter("#123b56")
	readonly property color disabledColor: Qt.darker("#123b56")
	readonly property int padding: 4
	signal clicked

	Rectangle {
		id: background
		color: "#1d1d1d"
		anchors.fill: parent
		border.color: "#333"
		border.width: 1
	}

	Icon {
		id: icon
		anchors.fill: parent
		anchors.margins: padding
		source: "icons/memory.svg"
		color: idleColor
	}

	MouseArea {
		id: mouseArea
		cursorShape: Qt.PointingHandCursor
		hoverEnabled: true
		anchors.fill: parent
		onClicked: control.clicked()
	}

	states: [
		State {
			when: false === enabled
			PropertyChanges {
				target: icon
				color: disabledColor
			}
		},
		State {
			when: mouseArea.containsMouse
			PropertyChanges {
				target: icon
				color: activeColor
			}
		}
	]
}
