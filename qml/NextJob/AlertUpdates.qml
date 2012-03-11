// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import com.nokia.meego 1.0
import "NJUiConstants_harmattan.js" as NJUiConstants;

Item {
    id: alertsViewContainer;
    signal alertClicked(variant alertObject);
    property alias model: alertsListView.model;
    anchors {
        left: parent.left;
        leftMargin: NJUiConstants.UI_DEFAULT_LEFTRIGHT_MARGIN;
        right: parent.right;
        rightMargin: NJUiConstants.UI_DEFAULT_LEFTRIGHT_MARGIN;
    }

    ListView {
        id: alertsListView
        delegate: alertComponent;
        clip: true;
        cacheBuffer: 500;
        spacing: NJUiConstants.UI_DEFAULT_LIST_SPACING;
        anchors.fill: parent;
    } // listview

    ScrollDecorator {
        flickableItem: alertsListView;
    }

    ContextMenu {
         id: alertsContextMenu;
         property variant selectedKey;
         MenuLayout {
             MenuItem {
                 text: "Remove this alert";
                 onClicked: removeAlert(alertsContextMenu.selectedKey);
             }
         }
    }

    Component {
        id: alertComponent;
        Rectangle {
            id: alertInfoView;
            property QtObject alert: alertinfo; // rolename for jobalert
            width: parent.width;
            height: alertInfoContainer.height;
            color: "transparent";
            MouseArea {
                id: marea;
                anchors.fill: parent;
                onClicked: alertClicked(alert);
                onPressAndHold: {
                    alertsContextMenu.selectedKey = alert.key();
                    alertsContextMenu.open();
                }
            }

            Row {
                id: alertInfoContainer;
                spacing: 10;
                width: parent.width;

                Item {
                    id: iconAndText;
                    width: icon.height
                    height: icon.height;
                    anchors {
                        verticalCenter: parent.verticalCenter;
                    }

                    Image {
                       id: icon;
                       source: "qrc:/images/defaulticon.svg";
                       smooth: true;
                       width: NJUiConstants.UI_DEFAULT_LISTICON_SIZE;
                       height: NJUiConstants.UI_DEFAULT_LISTICON_SIZE;
                    }
                    Text {
                        id: iconText;
                        width: NJUiConstants.UI_DEFAULT_LISTICON_SIZE;
                        horizontalAlignment: Text.AlignHCenter;
                        verticalAlignment: Text.AlignVCenter;
                        text: alert.newJobsCount;
                        font.pixelSize: NJUiConstants.UI_ALERTUPDATES_NEWJOBCOUNT_FONT_SIZE;
                        color: NJUiConstants.UI_ALERTUPDATES_NEWJOBCOUNT_COLOR;
                        font.bold: true;
                        smooth: true;
                        z: icon.z + 1;
                        anchors.centerIn: parent;
                    }
                }

                Column {
                    id: alertInfoTextContainer
                    width: parent.width;
                    spacing: NJUiConstants.UI_DEFAULT_LISTITEM_SPACING;

                    Label {
                        id: lineone;
                        text: alert.skill();
                        width: parent.width;
                        wrapMode: Text.WordWrap;
                        font.bold: true;
                    }
                    Label {
                        id: linetwo;
                        text: alert.location();
                        width: parent.width;
                        wrapMode: Text.WordWrap;
                    }
                    Label {
                        id: linethree;
                        text: alert.country();
                        width: parent.width;
                        wrapMode: Text.WordWrap;
                    }

                    Rectangle {
                        id: lineSeperator;
                        width: parent.width;
                        height: NJUiConstants.UI_LINE_HEIGHT;
                        color: NJUiConstants.UI_LINE_COLOR;
                    }
                }
            }
        }
    } // delegate
}

// eof
