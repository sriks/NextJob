import QtQuick 1.1
import com.nokia.meego 1.0
import "NJConstants.js" as NJConstants;

// njengine is injected as context property
PageStackWindow {
    id: _root;

//    initialPage: mainPage
//    MainPage {
//        id: mainPage
//    }

    Component.onCompleted:  {
        pageStack.push("qrc:/qml/NextJob/MainPage.qml")
    }

    function showAddAlertPage() {
        pageStack.push("qrc:/qml/NextJob/AddAlert.qml")
    }

    function showJobSearchPage() {
        pageStack.push("qrc:/qml/NextJob/SearchView.qml")
    }

    function showDetails(jobInfo) {
        pageStack.push("qrc:/qml/NextJob/JobDetails.qml",{"jobInfo":jobInfo});
    }

    function showNewJobsForAlert(alert) {
        pageStack.push("qrc:/qml/NextJob/NewJobsForAlert.qml",{"alert":alert});
    }

    function showAlerts() {
        pageStack.push("qrc:/qml/NextJob/Alerts.qml");
    }

    function removeAlert(key) {
        removeAlertConfirmation.key = key;
        removeAlertConfirmation.open();
    }

    function removeAllAlerts() {
        removeAllAlertsConfirmation.open();
    }

    function goBack() {
        pageStack.pop();
    }

    function showMenu() {
        if(myMenu.status === DialogStatus.Closed)
            myMenu.open();
        else
            myMenu.close();
    }

    Menu {
        id: myMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem { text: qsTr("Sample menu item") }
        }
    }

    QueryDialog {
        id: removeAlertConfirmation;
        property variant key;
        width: parent.width;
        title: NJConstants.APPNAME;
        message: "Do you want to remove this alert?"
        acceptButtonText: "Remove";
        rejectButtonText: "Cancel";
        onAccepted: njengine.removeAlert(key);
    }

    QueryDialog {
        id: removeAllAlertsConfirmation;
        width: parent.width;
        title: NJConstants.APPNAME;
        message: "Do you want to remove all alerts?"
        acceptButtonText: "Remove";
        rejectButtonText: "Cancel";
        onAccepted: njengine.removeAllAlerts();
    }

}
