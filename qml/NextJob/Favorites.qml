// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import com.nokia.meego 1.0
import "NJUiConstants_harmattan.js" as NJUiConstants;

Page {
    id: alerts;
    property QtObject model;
    tools: ToolBarLayout {
        id: tools
        visible: true

        NJToolButton {
            njIconId: NJUiConstants.UI_TOOLICON_BACK;
            anchors.left: parent.left;
            onClicked: goBack();
        }

        NJToolButton {
            njIconId: NJUiConstants.UI_TOOLICON_DELETE;
            visible: (favList.count)?(true):(false);
        }

        NJToolButton {
            njIconId: NJUiConstants.UI_TOOLICON_MENU;
            anchors.right: (parent === undefined) ? undefined : parent.right
            onClicked: showMenu();
        }
    }

    NJHeader {
        id: header;
        anchors.top: parent.top;
        title: "Favorite jobs";
        subTitle: "View or remove your favorite jobs";
        counterText: (favList.count)?(favList.count):("");
    }

    ResultView {
        id: favList;
        showDefaultErrorMessages: false;
        loadMoreDataWhenRequired: false;
        model: alerts.model;
        onCountChanged: if(count === 0) favList.setMessage("No favorites yet");
        onNoDataFetched: favList.setMessage("No favorites yet");
        width: parent.width;
        anchors {
            top: header.bottom;
            bottom: parent.bottom;
        }
    }
}

// eof
