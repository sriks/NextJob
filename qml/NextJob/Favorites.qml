// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import com.nokia.meego 1.0
import "NJUiConstants_harmattan.js" as NJUiConstants;

Page {
    id: alerts;
    property alias model: favList.model;
    tools: ToolBarLayout {
        id: tools
        visible: true

        ToolIcon {
            platformIconId: "toolbar-back"
            anchors.left: parent.left;
            onClicked: goBack();
        }

        ToolIcon {
            iconSource: "qrc:/images/delete.svg"

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
        title: "Favorite jobs";
        subTitle: "View or remove your favorite jobs";
    }

    ResultView {
        id: favList;
        model: njengine.favoriteJobs();
        onNoDataFetched: {
            console.debug("faorites.qml onNoDataFetched");
            favList.setMessage("No favorites yet");
        }
        width: parent.width;
        anchors {
            top: header.bottom;
            bottom: parent.bottom;
        }
    }
}

// eof
