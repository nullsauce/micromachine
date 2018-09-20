import QtQuick 2.0

Text {
	id:text
	text:hexValue
	font.family: "monospace"
	horizontalAlignment: Text.AlignLeft
	width:contentWidth * 1.1
	height:contentHeight
	color:"#ddd"
	renderType: Text.NativeRendering
}
