import QtQuick 1.1
import com.nokia.meego 1.0
import "NJUiConstants_harmattan.js" as NJUiConstants;

Rectangle {
    id: njHeader;
    property alias title: headerTitle.text;
    property alias subTitle: headerSubTitle.text;
    property bool busyIndicatorRunning: false;
    height: NJUiConstants.UI_HEADER_HEIGHT;
    width: parent.width;
    gradient: Gradient {
         GradientStop { position: 0.0; color: "#c5deea" }
         GradientStop { position: 0.15; color: "#8abbd7" }
         GradientStop { position: 1.0; color: "#066dab" }
    }

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
            source:  "qrc:/images/logo.svg";
            smooth: true;
            height: parent.height - 7;
            width: NJUiConstants.UI_LOGO_WIDTH;
            anchors.verticalCenter: parent.verticalCenter;
        }

        Column {
            id: headerTextColumn
            width: parent.width - logo.width;

            Label {
                id: headerTitle
                smooth: true;
                width: parent.width;
                color: "white";
                font.pixelSize: NJUiConstants.UI_HEADER_TITLE_FONT_SIZE;
                font.bold: true;
                maximumLineCount: 1
                elide: Text.ElideRight
            }
            Label {
                id: headerSubTitle;
                width: parent.width;
                smooth: true;
                color: "lightgrey";
                font.pixelSize: NJUiConstants.UI_HEADER_SUBTITLE_FONT_SIZE;
                maximumLineCount: 1
                elide: Text.ElideRight
            }
        }

        BusyIndicator {
            id:  headerBusyIndicator;
            width: 50;
            height: 50;
            running: njHeader.busyIndicatorRunning;
            visible: running;
            anchors {
                verticalCenter: parent.verticalCenter;
            }
        }
    }
}

// eof
