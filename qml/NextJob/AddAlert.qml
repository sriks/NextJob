import QtQuick 1.1
import com.nokia.meego 1.0
import "NJConstants.js" as NJConstants

Page {
    id: searchView;
    property string msg: NJConstants.ADDALERTPAGE;

    function addAlert() {
        njengine.addAlert({"skill" : searchBar.skill,
                           "location" : searchBar.location,
                           "country" : searchBar.country});
    }

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
        title: "Add job alert";
    }

    SearchBar {
        id: searchBar;
        showDefaultSearchButton: false;
        anchors {
            top: header.bottom;
            topMargin: 15;
        }
    }

    Button {
        id: addAlertButton;
        text: "Add Alert";
        anchors {
            top: searchBar.bottom;
            topMargin: 15
            horizontalCenter: parent.horizontalCenter;
        }
        onClicked: addAlert();
    }
}

// eof
