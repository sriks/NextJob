// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import "NJUiConstants_harmattan.js" as NJUiConstants;

// Default line seperator
Rectangle {
    id: seperatorLine
    height: NJUiConstants.UI_LINE_HEIGHT;
    radius: height/2;
    color: NJUiConstants.UI_LINE_COLOR;
    smooth: true;
//    anchors {
//        left: parent.left;
//        leftMargin: NJUiConstants.UI_LINE_MARGIN;
//        right: parent.right;
//        rightMargin: NJUiConstants.UI_LINE_MARGIN;
//    }
}
