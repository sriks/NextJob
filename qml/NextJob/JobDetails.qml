// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.1

import "NJUiConstants_harmattan.js" as NJUiConstants;

Page {
    id: jobDetails;
    property QtObject jobInfo;

    tools: ToolBarLayout {
        id: tools
        visible: true

        NJToolButton {
            njIconId: NJUiConstants.UI_TOOLICON_BACK;
            anchors.left: parent.left;
            onClicked: goBack();
        }

        NJToolButton {
            njIconId: NJUiConstants.UI_TOOLICON_FAV;
            onClicked: handleFavorite(jobInfo);
        }

        NJToolButton {
            njIconId: NJUiConstants.UI_TOOLICON_MENU
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

        Image {
            id: favFlag;
            smooth: true;
            anchors {
                right: parent.right;
                rightMargin: 10;
                top: parent.top;
                topMargin: 10
            }
            source: NJUiConstants.UI_FAVJOB_ICON;
            width: NJUiConstants.UI_FAVJOB_SMALL_SIZE;
            height: NJUiConstants.UI_FAVJOB_SMALL_SIZE;
            visible: (jobInfo.favorite || njengine.isFavorite(jobInfo.key()));
        }

        Column {
            id: info
            width: parent.width - favFlag.width;
            spacing: 7;

            anchors {
                left: parent.left;
                leftMargin: 15;
                right: parent.right;
                rightMargin: 15
            }

            Text {
                id: title;
                smooth: true;
                text: jobInfo.title();
                width: parent.width;
                wrapMode: Text.WordWrap;
                font.pixelSize: NJUiConstants.UI_RESULTVIEW_TITLE_FONT_SIZE;
                font.bold: true;
            }

            Row {
                id: empBox;
                width: parent.width;
                spacing: 10;

                Text {
                    id: empNameText;
                    smooth: true;
                    text: "Company:";
                    font.pixelSize: NJUiConstants.UI_RESULTVIEW_SUBTITLE_FONT_SIZE;
                    font.bold: true;
                }

                Text {
                    id: employerName;
                    smooth: true;
                    text: jobInfo.employer();
                    width: parent.width - (empNameText.width+empBox.spacing);
                    wrapMode: Text.WordWrap;
                    font.pixelSize: NJUiConstants.UI_RESULTVIEW_SUBTITLE_FONT_SIZE;
                }
            }

            Row {
                id: locBox;
                width: parent.width;
                spacing: 10;

                Text {
                    id: locNameText;
                    smooth: true;
                    text: "Location:";
                    font.pixelSize: NJUiConstants.UI_RESULTVIEW_SUBTITLE_FONT_SIZE;
                    font.bold: true;
                }

                Text {
                    id: location;
                    smooth: true;
                    text: jobInfo.location();
                    width: parent.width - (locNameText.width+locBox.spacing);
                    wrapMode: Text.WordWrap;
                    font.pixelSize: NJUiConstants.UI_RESULTVIEW_SUBTITLE_FONT_SIZE;
                }
            }

            Text {
                id: date;
                text: jobInfo.date();
                width: parent.width;
                wrapMode: Text.WordWrap;
                smooth: true;
                font.pixelSize: NJUiConstants.UI_RESULTVIEW_EXTRATEXT_FONT_SIZE;
                color: NJUiConstants.UI_RESULTVIEW_EXTRATEXT_COLOR;
            }

            SeparatorLine {
                id: separator;
                width: parent.width;
            }

            Item {
                id: descItem;
                width: parent.width;
                height: desc.height;
                Text{
                    id: desc;
                    text: jobInfo.description;
                    width: parent.width;
                    wrapMode: Text.WordWrap;
                    smooth: true;
                    font.pixelSize: NJUiConstants.UI_RESULTVIEW_CONTENT_FONT_SIZE;
                    Component.onCompleted: jobInfo.fetchDetailedDescription();
                }

                BusyIndicator {
                    id: descLoading;
                    visible: true;
                    running: visible;
                    width: NJUiConstants.UI_HEADER_BUSYINDICATOR_SIZE;
                    anchors.centerIn: desc;
                }

                Connections {
                    target: jobInfo;
                    onDescriptionChanged: descLoading.visible = false;
                }
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
