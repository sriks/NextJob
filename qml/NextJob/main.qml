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

    function showDetails(jobInfo,type) {
        pageStack.push("qrc:/qml/NextJob/JobDetails.qml",{"jobInfo":jobInfo,"type":type});
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

    /*!
      Handles adding or removing of favorites.
      It adds to favorites if jobinfo is not a favorite.
      It removes from favorites if jobinfo is already a favorite.
      **/
    function handleFavorite(jobInfoObj) {
        var key = jobInfoObj.key();
        var infotext;
        if(!njengine.isFavorite(key)) {
            console.debug("not a fav");
            njengine.addToFavorites(key);
            jobInfoObj.favorite = true;
            showInfoBanner(NJConstants.NJ_STR_SAVED_TO_FAVS);
        } else {
            var type = pageStack.currentPage.type;
            if(type === NJConstants.QUICK_LAUNCH_FAVS) {
                // Since we cannot restore, this requires a confirmation from user.
                // Once confirmed, the favorite is deleated
                console.debug("it is a fav");
                removeFavoriteConfirmation.key = key;
                removeFavoriteConfirmation.open();
            } else {
                // User is removing fav from search results
                jobInfoObj.favorite = false;
                njengine.removeFromFavorites(key);
                showInfoBanner(NJConstants.NJ_STR_REMOVED_FROM_FAVS);
            }
        }

    }

    function removeAllFavorites() {
        removeAllFavoritesConfirmation.open();
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
        property variant key;
        width: parent.width;
        title: NJConstants.APPNAME;
        message: "Do you want to remove all alerts?"
        acceptButtonText: "Remove";
        rejectButtonText: "Cancel";
        onAccepted: {
            njengine.removeAllAlerts();
        }
    }

    QueryDialog {
        id: removeFavoriteConfirmation;
        property variant key;
        width: parent.width;
        title: NJConstants.APPNAME;
        message: NJConstants.NJ_STR_CONF_REMOVE_FAV;
        acceptButtonText: "Remove";
        rejectButtonText: "Cancel";
        onAccepted: {
            njengine.removeFromFavorites(key);
            pageStack.pop();
            showInfoBanner(NJConstants.NJ_STR_REMOVED_FROM_FAVS);
        }
    }

    QueryDialog {
        id: removeAllFavoritesConfirmation;
        width: parent.width;
        title: NJConstants.APPNAME;
        message: NJConstants.NJ_STR_CONF_REMOVE_ALL_FAVS;
        acceptButtonText: "Remove";
        rejectButtonText: "Cancel";
        onAccepted: {
            njengine.removeAllFavorites();
        }
    }


    NJInfoBanner {
        id: banner;
        anchors {
            top: parent.top;
            topMargin: 40;
        }
    }
}
