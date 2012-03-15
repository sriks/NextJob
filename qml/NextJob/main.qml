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

    LoadingScreen {
        id: loadingScreen;
        width: parent.width;
        height: parent.height;
        z: pageStack.z + 1;
    }

    Connections {
        target: njengine;
        onInitializationCompleted: {
            showMainPage();
            loadingScreen.hide();
        }
    }

    Timer {
        id: loadTimer;
        interval: 400;
        repeat: false;
        onTriggered: njengine.initialize();
    }

    Component.onCompleted:  {
        loadTimer.running = true;
    }


    function showMainPage() {
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

    function showFavorites() {
        pageStack.push("qrc:/qml/NextJob/Favorites.qml",{"model":njengine.favoriteJobs()});
    }

    function removeAlert(key) {
        removeAlertConfirmation.key = key;
        removeAlertConfirmation.open();
    }

    function removeAllAlerts() {
        removeAllAlertsConfirmation.open();
    }

    /*!
      Returns true if added.
      Returns false if removed.
      **/
    function handleFavorite(key) {
        var res = false;
        if(!njengine.isFavorite(key)) {
            res = true;
            njengine.addToFavorites(key);
        } else {
            njengine.removeFromFavorites(key);
            res = false;
        }
        return res;
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
            MenuItem { text: qsTr("About") }
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
