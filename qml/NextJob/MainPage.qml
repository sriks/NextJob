import QtQuick 1.1
import com.nokia.meego 1.0
import "NJConstants.js" as NJConstants

Page {
    property string msg: NJConstants.MAINPAGE;

    tools: ToolBarLayout {
        id: tools
        visible: true
        ToolIcon {
            platformIconId: "toolbar-view-menu"
            anchors.right: (parent === undefined) ? undefined : parent.right
            onClicked: showMenu();
        }
    }

    NJHeader {
        id: header;
        title: NJConstants.APPNAME;
    }

    Column {
        id: tmp;
        anchors {
            top: header.bottom;
        }

        Button {
            id: addAlert
            text: "Add Alert";
            onClicked: _root.showAddAlertPage();
        }

        Button {
            id: search
            text: "Search";
            onClicked: _root.showJobSearchPage();
        }

    }
}
