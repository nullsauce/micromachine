import QtQuick 2.0
import QtQuick.Controls 2.2
import "Helper.js" as Helper
import Fla 1.0

IntegerText {
	width:contentWidth
	height:contentHeight

	function switchDisplayMode() {
		if(IntegerText.Hexadecimal === displayMode) {
			displayMode = IntegerText.SignedDecimal;
		} else if(IntegerText.SignedDecimal === displayMode) {
			displayMode = IntegerText.UnsignedDecimal;
		} else if(IntegerText.UnsignedDecimal === displayMode) {
			displayMode = IntegerText.Hexadecimal;
		}
	}

	MouseArea {
		id:mouseArea
		hoverEnabled: true
		anchors.fill: parent
		acceptedButtons: Qt.RightButton
		onClicked:switchDisplayMode()

		CustomToolTip {
			delay: 500
			visible: mouseArea.containsMouse
			text: "hex: %1\nuns: %2\nsig: %3"
				.arg(asHexString)
				.arg(asUnsignedDecimalString)
				.arg(asSignedDecimalString)
		}
	}


}
