import QtQuick 2.0
import Fla 1.0
Item {
	id:item
	width:250
	height:400
	property BreakpointRegistry breakpointRegistry

	function createInstance(posX, posY) {
		var posX = (appWindow.width / 2) -(item.width / 2);
		var posY = (appWindow.height / 2) -(item.height / 2);
		component.createObject(appWindow, {x:posX, y:posY});
	}

	Component {
		id: component
		BreakpointsWindow {
			id:window
			width:item.width
			height:item.height
			breakpointRegistry: item.breakpointRegistry
		}
	}
}
