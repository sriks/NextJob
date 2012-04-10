import QtQuick 1.1
import com.nokia.meego 1.0
import "NJConstants.js" as NJConstants;

// njengine is injected as context property
PageStackWindow {
    id: _root;

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
        var m = njengine.favoriteJobs();
        pageStack.push("qrc:/qml/NextJob/Favorites.qml",{"model":m});
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

    function addAlert(key) {
        var infotext;
        if(!njengine.isAlertAdded(key)) {
            njengine.addAlert(key);
            infotext = qsTr("Alert added");
        } else {
            infotext = qsTr("You have already added this alert.");
        }
        showInfoBanner(infotext);
    }

    function handleFavorite(jobInfoObj) {
        var res = false;
        var key = jobInfoObj.key();
        var infotext;
        if(!njengine.isFavorite(key)) {
            njengine.addToFavorites(key);
            res = true;
            infotext = qsTr("Saved to favorites");
        } else {
            njengine.removeFromFavorites(key);
            res = false;
            infotext = qsTr("Removed from favorites");
        }
        jobInfoObj.setFavorite(res);
        showInfoBanner(infotext);
    }

    function showInfoBanner(text) {
        banner.text = text;
        banner.show();
    }

    LoadingScreen {
        id: loadingScreen;
        width: parent.width;
        height: parent.height;
        z: pageStack.z + 1;
    }

    Connections {
        target: njengine;
        onInitializationCompleted: {
            console.debug("init completed");
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

    NJInfoBanner {
        id: banner;
        anchors {
            top: parent.top;
            topMargin: 40;
        }
    }
}
