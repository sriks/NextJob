// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import com.nokia.meego 1.0
import "NJUiConstants_harmattan.js" as NJUiConstants;

Page {
    id: alerts;

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
            onClicked: removeAllAlerts();
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
        title: "Alerts";
        subTitle: "View, edit or remove your alerts"
    }

    AlertUpdates {
        id: updates;
        model: njengine.allAlertsModel();

        Component.onCompleted: {
            var m = njengine.allAlertsModel();
            m.loadData();
            model = m;
        }

        onAlertClicked: {
            showNewJobsForAlert(alertObject);
        }

        anchors {
            top: header.bottom;
            bottom: parent.bottom;
        }
    }
}

// eof
