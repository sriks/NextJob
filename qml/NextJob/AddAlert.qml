import QtQuick 1.1
import com.nokia.meego 1.0
import "NJConstants.js" as NJConstants

Page {
    id: searchView;
    property string msg: NJConstants.ADDALERTPAGE;

    function addJobAlert() {
        var key = {"skill" : searchBar.skill,
         "location" : searchBar.location,
         "country" : searchBar.country};
        addAlert(key)
    }

    tools: ToolBarLayout {
        id: tools
        visible: true

        NJToolButton {
            njIconId: "toolbar-back"
            anchors.left: parent.left;
            onClicked: goBack();
        }

        NJToolButton {
            njIconId: "toolbar-view-menu"
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
        text: qsTr("Add Alert");
        enabled: (searchBar.skill.length)?(true):(false);
        anchors {
            top: searchBar.bottom;
            topMargin: 15
            horizontalCenter: parent.horizontalCenter;
        }
        onClicked: addJobAlert();
    }
}

// eof
