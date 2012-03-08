import QtQuick 1.1
import com.nokia.meego 1.0

// njengine is injected as context property
PageStackWindow {
    id: _root;

//    initialPage: mainPage
//    MainPage {
//        id: mainPage
//    }

    function showAddAlertPage() {
        pageStack.push("qrc:/qml/NextJob/AddAlert.qml")
    }

    function showJobSearchPage() {
        pageStack.push("qrc:/qml/NextJob/SearchView.qml")
    }

    function showDetails(jobInfo) {
        pageStack.push("qrc:/qml/NextJob/JobDetails.qml",{"jobInfo":jobInfo});
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

    Connections {
        target: njengine;
        onAlertUpdated: {
            console.debug("alert updated:"+key["skill"]+"  "+newItemsCount);
        }
    }

    Menu {
        id: myMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem { text: qsTr("Sample menu item") }
        }
    }

    Component.onCompleted:  {
        pageStack.push("qrc:/qml/NextJob/MainPage.qml")
    }
}
