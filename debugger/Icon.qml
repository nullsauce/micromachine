import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
	property alias source: image.source
	property alias color: colorOverlay.color

	Image {
		id: image
		anchors.fill: parent
		sourceSize.width: width
		sourceSize.height: height
	}

	ColorOverlay {
		id: colorOverlay
		anchors.fill: image
		source: image
	}
}
