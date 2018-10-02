import QtQuick 2.0
import Fla 1.0
Item {
	id:item
	width:600
	height:800

	property BreakpointRegistry breakpointRegistry
	property MemoryRegion memoryRegion

	function createInstance() {
		var posX = (appWindow.width / 2) -(item.width / 2);
		var posY = (appWindow.height / 2) -(item.height / 2);
		component.createObject(appWindow, {x:posX, y:posY, screen:appWindow.screen});
	}

	Component {
		id: component
		DisassemblyWindow {
			id:window
			width:item.width
			height:item.height
			breakpointRegistry: item.breakpointRegistry
			memoryRegion: item.memoryRegion
		}
	}
}
