// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import com.nokia.meego 1.0

import "NJUiConstants_harmattan.js" as NJUiConstants;

Page {
    id: jobDetails;
    property QtObject jobInfo;

    tools: ToolBarLayout {
        id: tools
        visible: true

        ToolIcon {
            platformIconId: "toolbar-back"
            anchors.left: parent.left;
            onClicked: goBack();
        }

        ToolIcon {
            platformIconId: "toolbar-view-menu"
            anchors.right: (parent === undefined) ? undefined : parent.right
            onClicked: showMenu();
        }
    }

    NJHeader {
        id: header;
        anchors.top: parent.top;
        title: "Job details";
        subTitle: jobInfo.title();
    }

    ScrollDecorator {
        flickableItem: flickContainer;
    }

    Flickable {
        id: flickContainer;
        width: parent.width;
        height: parent.height;
        flickableDirection: Flickable.VerticalFlick;
        contentHeight: info.height;
        clip: true;
        anchors {
            top: header.bottom;
            topMargin: 5;
            bottom: parent.bottom;
            bottomMargin: 10;
        }

        Column {
            id: info
            width: parent.width;
            spacing: 7;

            anchors {
                left: parent.left;
                leftMargin: 15;
                right: parent.right;
                rightMargin: 15
            }

            Text {
                id: title;
                text: jobInfo.title();
                width: parent.width;
                wrapMode: Text.WordWrap;
                font.pixelSize: NJUiConstants.UI_DETAILS_TITLE_FONT_SIZE;
                font.bold: true;
            }

            Text {
                id: employerName;
                text: jobInfo.employer();
                width: parent.width;
                wrapMode: Text.WordWrap;
                font.pixelSize: NJUiConstants.UI_DETAILS_SUBTITLE_FONT_SIZE;
            }

            Text {
                id: location;
                text: jobInfo.location();
                width: parent.width;
                wrapMode: Text.WordWrap;
                font.pixelSize: NJUiConstants.UI_DETAILS_DESC_FONT_SIZE;
            }

            Text {
                id: date;
                text: jobInfo.date();
                width: parent.width;
                wrapMode: Text.WordWrap;
                font.pixelSize: NJUiConstants.UI_DETAILS_DESC_FONT_SIZE;
                color: "blue";
            }

            Text {
                id: desc;
                text: jobInfo.description();
                width: parent.width;
                wrapMode: Text.WordWrap;
                font.pixelSize: NJUiConstants.UI_DETAILS_DESC_FONT_SIZE;
                color: NJUiConstants.UI_CONTENT_TEXT_COLOR;
            }

            Button {
                id: showOriginalPosting;
                text: "See original posting";
                anchors.horizontalCenter: parent.horizontalCenter;
                onClicked: Qt.openUrlExternally(jobInfo.url());
            }

        } // column
    }
}

// eof
