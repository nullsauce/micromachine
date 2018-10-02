import QtQuick 2.0
import Fla 1.0
import QtQuick.Window 2.3
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

ToolWindow {
	Component.onCompleted: show()

	content: Item {
		anchors.fill: parent

		ComboBox {
			id:regionSelectBox
			anchors.right: parent.right
			model: CPU.memoryRegions
			textRole: "name"
			height:30
			width:150
		}

		StackLayout {
			width:parent.width
			anchors.top: regionSelectBox.bottom
			anchors.bottom: parent.bottom
			currentIndex: regionSelectBox.currentIndex
			Repeater {
				model:CPU.memoryRegions
				Item {
					anchors.fill: parent
					Memoryview {
						anchors.fill: parent
						memoryRegion:modelData
					}
				}
			}
		}
	}
}
