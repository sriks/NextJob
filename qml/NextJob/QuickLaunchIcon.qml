// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "NJUiConstants_harmattan.js" as NJUiConstants;

Column {
    id: launcherIcon;
    signal clicked(string msg);
    property alias text: quickLaunchText.text;
    property string msg;
    property string icon;
    width: NJUiConstants.UI_DEFAULT_QUICKLAUNCH_ICON_SIZE;
    spacing: 3;

    Image {
        id: iconBkg;
        source: "qrc:/images/defaulticon.svg";
        smooth: true;
        width: NJUiConstants.UI_QUICKLAUNCH_BKG_SIZE;
        height: NJUiConstants.UI_QUICKLAUNCH_BKG_SIZE;
        Image {
            id: innerIcon;
            source: launcherIcon.icon;
            anchors {
                horizontalCenter: parent.horizontalCenter;
                verticalCenter: parent.verticalCenter;
            }
            width: NJUiConstants.UI_QUICKLAUNCH_ICON_SIZE;
            height: NJUiConstants.UI_QUICKLAUNCH_ICON_SIZE;
            smooth: true;
        }
        MouseArea {
            anchors.fill: parent;
            onClicked: launcherIcon.clicked(msg);
        }
    }

    Text {
        id: quickLaunchText;
        width: parent.width;//NJUiConstants.UI_DEFAULT_QUICKLAUNCH_ICON_SIZE;
        smooth: true;
        wrapMode: Text.WordWrap;
        horizontalAlignment: Text.AlignHCenter;
        font.pixelSize: NJUiConstants.UI_QUICKLAUNCH_FONT_SIZE;
        color: "grey";
    }
}

// eof
