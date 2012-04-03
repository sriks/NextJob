// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import com.nokia.meego 1.0
import "NJUiConstants_harmattan.js" as NJUiConstants;

Page {
    id: alerts;

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
            onClicked: removeAllAlerts();
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
        title: "Alerts";
        subTitle: "View, edit or remove your alerts"
        counterText: (updates.count)?(updates.count):("");
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
