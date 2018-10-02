import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Controls 1.4

Window {
	id:window
	/*
	flags: Qt.Tool | Qt.CustomizeWindowHint | Qt.WindowCloseButtonHint
		& ~Qt.WindowTitleHint*/
	flags: Qt.Tool
	property alias content: contentHolder.children
	property alias header: headerHolder.children

	Rectangle {
		anchors.fill: parent
		color: "#1d1d1d"
		border.color: "#333"
	}

	Rectangle {
		id:header
		height:20
		width:parent.width
		color:"#2d2d2d"
		MouseArea {
			cursorShape: Qt.OpenHandCursor
			id:mouseArea
			anchors.fill: parent
			hoverEnabled: true
			onPressed: {
				pressPoint = Qt.vector2d(mouse.x, mouse.y);
			}
			property vector2d pressPoint: Qt.vector2d(0,0);
			onMouseXChanged: {
				if(mouseArea.pressed) {
					var mousePos = Qt.vector2d(mouse.x, mouse.y);
					var delta = mousePos.minus(pressPoint);
					window.x += delta.x;
					window.y += delta.y;
				}
			}
		}
		Item {
			id:headerHolder
			anchors.fill: parent
		}
	}

	Item {
		id:contentHolder
		anchors.top: header.bottom
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.bottom: parent.bottom
	}

}
