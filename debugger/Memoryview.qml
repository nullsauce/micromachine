import QtQuick 2.0
import Fla 1.0
import QtQuick.Controls 2.2
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
		anchors.fill: parent
		onWheel: {
			var newPos = 0;
			if(wheel.angleDelta.y > 0) {
				newPos = scrollarea.position - scrollarea.stepSize;
			} else {
				newPos = scrollarea.position + scrollarea.stepSize;
			}
			scrollarea.setScrollPosition(newPos);
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
		size:0.5;//hexview.visibleRatio
		anchors.bottom: parent.bottom
		stepSize: scale/hexview.rows
	}

}
