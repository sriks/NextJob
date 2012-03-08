// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import com.nokia.meego 1.0
import "NJConstants.js" as NJConstants

Page {
    id: searchView;
    property string msg: NJConstants.SEARCHPAGE;
    property QtObject _currentJobModel;
    property Item _resultView;
    function handleSearch(skill,location,country) {
        // key should be same as defined in Constants.h
        njengine.search({"skill" : skill,
                        "location" : location,
                        "country" : country});
        header.subTitle = skill;
        header.busyIndicatorRunning = true;
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

    Connections {
        target: njengine;
        onSearchResultAvailable: {
            // jobModel is passed as argument in this signal.
            _currentJobModel = jobModel;
            if(_resultView)
                _resultView.destroy();
            _resultView = resultViewComponent.createObject(searchView,{"model":jobModel});
        }
    }

    Connections {
        target: _currentJobModel;
        onNetworkRequestStarted: header.busyIndicatorRunning = true;
        onNetworkRequestFinished: header.busyIndicatorRunning = false;
    }

    NJHeader {
        id: header;
        title: "Search";
    }

    SearchBar {
        id: searchBar;
        anchors {
            top: header.bottom;
            topMargin: 10;
        }
        onSearchEntered: {
            handleSearch(skill,location,country);
        }
    }

    Component {
        id: resultViewComponent;
        ResultView {
            id: lv
            width: parent.width;
            anchors {
                top: searchBar.bottom;
                topMargin: 10;
                bottom: parent.bottom;
                bottomMargin: 10;
            }
// Fix it.
//            onScrolling: searchBar.disappear();
//            onScrollingStopped: searchBar.appearWithDelay()
        }
    }
}

// eof
