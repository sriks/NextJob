import QtQuick 1.1
import com.nokia.meego 1.0
import "NJUiConstants_harmattan.js" as NJUiConstants;

Rectangle {
    id: njHeader;
    property alias title: headerTitle.text;
    property alias subTitle: headerSubTitle.text;
    property bool busyIndicatorRunning: false;

    color: NJUiConstants.UI_HEADER_COLOR;
    height: NJUiConstants.UI_HEADER_HEIGHT;
    width: parent.width;

    Label {
        id: headerTitle
        smooth: true;
        color: "white";
        font.pixelSize: NJUiConstants.UI_HEADER_TITLE_FONT_SIZE;
        maximumLineCount: 1
        elide: Text.ElideRight
        anchors {
            left: parent.left;
            leftMargin: 15
            right: headerBusyIndicator.left;
            rightMargin: 15
            top: parent.top;
            topMargin: 3;
        }
    }

    Label {
        id: headerSubTitle;
        smooth: true;
        color: "lightgrey";
        font.pixelSize: NJUiConstants.UI_HEADER_SUBTITLE_FONT_SIZE;
        maximumLineCount: 1
        elide: Text.ElideRight
        anchors {
            left: parent.left;
            leftMargin: 15
            right: headerBusyIndicator.left;
            rightMargin: 15
            top: headerTitle.bottom;
            topMargin: 3;
        }
    }

    BusyIndicator {
        id:  headerBusyIndicator;
        width: 50;
        height: 50;
        running: njHeader.busyIndicatorRunning;
        visible: running;
        anchors {
            right: parent.right;
            rightMargin: 10;
            verticalCenter: parent.verticalCenter;
        }
    }
}

// eof
