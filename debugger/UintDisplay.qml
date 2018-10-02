import QtQuick 2.0
import QtQuick.Controls 2.2
import "Helper.js" as Helper

RegularText {
	property int value: 0
	property int displayMode:1


	property string hexRepresentation: Helper.hex32(value)
	property string unsignedDecimalRepresentation: "%1".arg(value)

	function getTextRepresentation() {
		switch(displayMode) {
		    case 0: return unsignedDecimalRepresentation;
			case 1: return hexRepresentation;
			default: return unsignedDecimalRepresentation;
		}
	}

	function switchDisplayMode() {
		displayMode = (displayMode + 1) % 2;
	}

	height:contentHeight
	font.family: "Hack"
	font.styleName: "Regular"
	wrapMode: Text.NoWrap
	text: getTextRepresentation()

	MouseArea {
		id:mouseArea
		hoverEnabled: true
		anchors.fill: parent
		acceptedButtons: Qt.RightButton
		onClicked:switchDisplayMode()
		CustomToolTip {
			delay: 500
			visible: mouseArea.containsMouse
			text: "hex: %1\nuns: %2"
			    .arg(hexRepresentation)
			    .arg(unsignedDecimalRepresentation)
		}
	}


}
