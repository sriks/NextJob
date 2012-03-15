// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import com.nokia.meego 1.0

Page {
    id: newJobsForAlert;
    property QtObject alert;
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

    NJHeader {
        id: header;
        title: "Job alert";
        subTitle: alert.displayName();
        busyIndicatorRunning: lv.loading;
    }

    ButtonRow {
        id: jobTypeButtonRow;
        exclusive: true;
        checkedButton: newJobs;
        width: parent.width/2;
        anchors {
            top: header.bottom;
            topMargin: 15;
            horizontalCenter: parent.horizontalCenter;
        }
        Button {
            id: newJobs;
            text: "New"
            onClicked: {
                //lv.flipped = !lv.flipped;
                lv.loadMoreDataWhenRequired = false;
                lv.model = alert.model();
            }
        }
        Button {
            id: allJobs;
            text: "All";
            onClicked: {
                //lv.flipped = !lv.flipped;
                var m = njengine.search(alert.key());
                lv.loadMoreDataWhenRequired = true;
                lv.model = m;
            }
        }
    }

    ResultView {
        id: lv
        property bool flipped: false;
        width: parent.width;
        model: alert.model();
        onModelChanged: model.reset();
        loadMoreDataWhenRequired: false; // show only new items
        anchors {
            top: jobTypeButtonRow.bottom;
            topMargin: 5;
            bottom: parent.bottom;
        }

        transform: Rotation {
              id: rotation
              origin.x: lv.width/2
              origin.y: lv.height/2
              axis.x: 0; axis.y: 1; axis.z: 0     // set axis.y to 1 to rotate around y-axis
              angle: 0    // the default angle
          }

          states: State {
              name: "back"
              PropertyChanges { target: rotation; angle: 360 }
              when: lv.flipped
          }

          transitions: Transition {
              NumberAnimation { target: rotation; property: "angle"; duration: 800 }
          }
     }
}

// eof
