import QtQuick 1.1
import com.nokia.meego 1.0
import "NJUiConstants_harmattan.js" as NJUiConstants;

Rectangle {
    id: njHeader;
    property alias title: headerTitle.text;
    property alias subTitle: headerSubTitle.text;
    property bool busyIndicatorRunning: false;
    width: parent.width;
    height: NJUiConstants.UI_HEADER_HEIGHT;
    color: NJUiConstants.UI_HEADER_COLOR;

//    gradient: Gradient {
//         GradientStop { position: 0.0; color: "#63b6db" }
//         // GradientStop { position: 0.15; color: "#8abbd7" }
//         GradientStop { position: 1.0; color: "#309dcf" }
//    }

    Row {
        id: headerContainer;
        spacing: 10;
        anchors {
            left: parent.left;
            leftMargin: 10;
            right: parent.right;
            rightMargin: 10;
            verticalCenter: parent.verticalCenter;
        }

        Image {
            id: logo;
            source: NJUiConstants.UI_LOGO;
            smooth: true;
            height: parent.height - 7;
            width: NJUiConstants.UI_LOGO_WIDTH;
            anchors.verticalCenter: parent.verticalCenter;
        }

        Column {
            id: headerTextColumn
            width: parent.width - (logo.width + headerBusyIndicator.width);

            Label {
                id: headerTitle
                smooth: true;
                width: parent.width;
                color: NJUiConstants.UI_HEADER_TITLE_COLOR;
                font.pixelSize: NJUiConstants.UI_HEADER_TITLE_FONT_SIZE;
                maximumLineCount: 1
                elide: Text.ElideRight
            }
            Label {
                id: headerSubTitle;
                width: parent.width;
                smooth: true;
                color: NJUiConstants.UI_HEADER_SUBTITLE_COLOR;
                font.pixelSize: NJUiConstants.UI_HEADER_SUBTITLE_FONT_SIZE;
                maximumLineCount: 1
                elide: Text.ElideRight
            }
        }

        BusyIndicator {
            id:  headerBusyIndicator;
            width: NJUiConstants.UI_HEADER_BUSYINDICATOR_SIZE;
            height: NJUiConstants.UI_HEADER_BUSYINDICATOR_SIZE;
            running: njHeader.busyIndicatorRunning;
            visible: running;
            anchors.verticalCenter: parent.verticalCenter;
        }
    }
}

// eof
