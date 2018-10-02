import QtQuick 2.6
import QtQuick.Controls 2.2

ToolTip {
	id: control
	contentItem: RegularText {
		text: control.text
		font: control.font
	}

	background: Rectangle {
		color: "#1d1d1d"
		border.color: "#333"
	}
}
