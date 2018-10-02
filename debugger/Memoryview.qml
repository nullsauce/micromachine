import QtQuick 2.0
import Fla 1.0
import QtQuick.Controls 2.2
import "Helper.js" as Helper
Item {
	function clamp(val,min,max) {
		return Math.max(Math.min(val,max), min);
	}

	Component.onCompleted: {
		CPU.memView = hexview;
	}

	property alias memoryRegion: hexview.memoryRegion

	MemView {
		id:hexview
		anchors.top: parent.top
		anchors.bottom: parent.bottom
		anchors.left: parent.left
		anchors.right: scrollarea.left
		width:400
		height:300
		scroll:scrollarea.position
	}

	MouseArea {
		id:mouseArea
		anchors.fill: parent
		hoverEnabled: true
		property int addressUnderMouse: hexview.addressAtPos(mouseX, mouseY)
		property rect byteUnderMouse: hexview.addressBounds(addressUnderMouse)

		onWheel: {
			var newPos = 0;
			if(wheel.angleDelta.y > 0) {
				newPos = scrollarea.position - scrollarea.stepSize;
			} else {
				newPos = scrollarea.position + scrollarea.stepSize;
			}
			scrollarea.setScrollPosition(newPos);
		}


		Rectangle {
			visible:mouseArea.addressUnderMouse !== 0
			color: "#5fd7ff"
			opacity: 0.4
			width: mouseArea.byteUnderMouse.width
			height: mouseArea.byteUnderMouse.height
			x: mouseArea.byteUnderMouse.x
			y: mouseArea.byteUnderMouse.y
			CustomToolTip {
				delay: 500
				visible: mouseArea.containsMouse
				text: "address: %1"
				    .arg(Helper.hex32(mouseArea.addressUnderMouse))
			}
		}
	}

	ScrollBar {
		function setScrollPosition(newPos) {
			position = clamp(newPos, 0, 1);
		}

		id: scrollarea
		anchors.right: parent.right
		anchors.top: parent.top
		height:parent.height
		width:20
		hoverEnabled: true
		active: true
		interactive:true
		policy: ScrollBar.AlwaysOn
		orientation: Qt.Vertical
		size:hexview.visibleRatio
		anchors.bottom: parent.bottom
		stepSize: scale/hexview.rows
	}

}
