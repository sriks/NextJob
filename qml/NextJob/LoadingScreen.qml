// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import com.nokia.meego 1.0
import "NJUiConstants_harmattan.js" as NJUiConstants;

Rectangle {
    id: loadingScreen;
    z: parent.z + 5;

    function show() {
        loadingScreen.visible = true;
    }

    function hide() {
        hideAnimation.start();
    }

    gradient: Gradient {
         GradientStop { position: 0.0; color: "#63b6db" }
         GradientStop { position: 0.15; color: "#8abbd7" }
         GradientStop { position: 1.0; color: "#309dcf" }
    }

    transform: Rotation {
          id: rotation
          origin.x: loadingScreen.width/2
          origin.y: loadingScreen.height/2
          axis.x: 0; axis.y: 1; axis.z: 0     // set axis.y to 1 to rotate around y-axis
          angle: 0    // the default angle
    }

    ParallelAnimation {
        id: hideAnimation;
        PropertyAnimation {
            target: rotation;
            property: "angle";
            to: 45;
            duration: 600;
        }
        PropertyAnimation {
            target: loadingScreen;
            property: "opacity";
            to: 0;
            duration: 600;
        }
    }

    Column {
        width: parent.width;
        anchors.verticalCenter: parent.verticalCenter;

        Image {
            id: logoNJUiConstants
            smooth: true;
            anchors.horizontalCenter: parent.horizontalCenter;
            source: NJUiConstants.UI_LOGO;
            width: NJUiConstants.UI_LOADINGSCREEN_LOGO_WIDTH;
            height: NJUiConstants.UI_LOADINGSCREEN_LOGO_HEIGHT;
        }

        Row {
            id: appnameContainer
            width: 200;
            anchors.horizontalCenter: parent.horizontalCenter;
            spacing: 2;

            Text {
                id: appname1;
                smooth: true;
                text: "Next J";
                font.pixelSize: NJUiConstants.UI_LOADINGSCREEN_FONT_SIZE;
            }

            BusyIndicator {
                id: busy;
                running: true;
                anchors.verticalCenter: parent.verticalCenter;
                width: NJUiConstants.UI_LOADINGSCREEN_BUSYINDICATOR_WIDTH;
            }

            Text {
                id: appname2;
                smooth: true;
                text: "b";
                font.pointSize: NJUiConstants.UI_LOADINGSCREEN_FONT_SIZE;
            }
        }
    }
}

// eof
