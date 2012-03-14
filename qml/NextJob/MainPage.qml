import QtQuick 1.1
import com.nokia.meego 1.0
import "NJConstants.js" as NJConstants
import "NJUiConstants_harmattan.js" as NJUiConstants;

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

//    BorderImage {
//        id: bkg
//        source: "qrc:/images/clothtexture1.jpg";
//        smooth: true;
//        anchors.fill: parent;
//        border.left: 5; border.top: 5
//        border.right: 5; border.bottom: 5
//    }

    NJHeader {
        id: header;
        title: NJConstants.APPNAME;
    }

    Item {
        id: quickLanucher;
        height: quickLaunchContainer.height;
        function quickLaunchClicked(msg) {
            if(msg === NJConstants.QUICK_LAUNCH_SEARCH)
                showJobSearchPage();
            else if(msg === NJConstants.QUICK_LAUNCH_ADDALERT)
                showAddAlertPage();
            else if(msg === NJConstants.QUICK_LAUNCH_ALERTS)
                showAlerts();
            else if(msg === NJConstants.QUICK_LAUNCH_FAVS)
                showFavorites();
        }


        anchors {
            top: header.bottom;
            topMargin: 20;
            left: parent.left;
            leftMargin: 15;
            right: parent.right;
            rightMargin: 15;
        }

        Row {
            id: quickLaunchContainer;
            spacing: 30;
            anchors.horizontalCenter: parent.horizontalCenter;
            QuickLaunchIcon {
                msg: NJConstants.QUICK_LAUNCH_SEARCH;
                text: "Search";
                icon: "qrc:/images/search.svg";
                onClicked: quickLanucher.quickLaunchClicked(msg);
            }

            QuickLaunchIcon {
                msg: NJConstants.QUICK_LAUNCH_ADDALERT;
                text: "Add Alert";
                icon: "qrc:/images/add.svg";
                onClicked: quickLanucher.quickLaunchClicked(msg);
            }

            QuickLaunchIcon {
                msg: NJConstants.QUICK_LAUNCH_ALERTS;
                text: "All Alerts";
                icon: "qrc:/images/alerts.svg";
                onClicked: quickLanucher.quickLaunchClicked(msg);
            }

            QuickLaunchIcon {
                msg: NJConstants.QUICK_LAUNCH_FAVS;
                text: "Favorites";
                icon: "qrc:/images/favorite_quicklaunch.svg";
                onClicked: quickLanucher.quickLaunchClicked(msg);
            }
        }
    }

    Row {
        id: alertUpdatesTitle;
        spacing: 10;

        anchors {
            top: quickLanucher.bottom;
            topMargin: 10;
            left: parent.left;
            leftMargin: 10;
            right: parent.right;
            rightMargin: 10;
        }

        Text {
            id: alertupdatestitlestext;
            text: "New jobs";
            smooth: true;
            font.pixelSize: NJUiConstants.UI_ALERTUPDATES_FONT_SIZE;
            font.bold: true
        }

        SeparatorLine {
            id: seperator;
            width: parent.width - (alertupdatestitlestext.width) - 10;
            anchors.verticalCenter: parent.verticalCenter;
        }
    }

    AlertUpdates {
        id: alertUpdates;
        Component.onCompleted: {
            var m = njengine.newJobsAlertModel();
            m.loadData();
            model = m
        }

        onAlertClicked: {
            console.debug("alert clicked:"+alertObject);
            showNewJobsForAlert(alertObject);
        }

        anchors {
            top: alertUpdatesTitle.bottom;
            topMargin: 10;
            bottom: parent.bottom;
            left: parent.left;
            leftMargin: 15;
            right: parent.right;
            rightMargin: 15;
        }
    }
}

// eof
