// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.1
import "NJUiConstants_harmattan.js" as NJUiConstants;

Item {
    id: resultViewContainer
    property alias model: resultListView.model;
    property bool loading: false;
    property bool loadMoreDataWhenRequired: true;
    signal scrolling;
    signal scrollingStopped;
    width: parent.width;

    function handleDataFinished() {
        loading = false;
    }

    function handleError(errorMessage) {
        console.debug("ResultView.qml handleError() "+errorMessage);
        resultListView.model = 0;
        msgLabel.setMessage("Cannot fetch update.")
    }

    Connections {
        target: model;
        onDataFinished: handleDataFinished();
        onError: handleError(errorMessage);
    }

    Label {
        id: msgLabel
        function setMessage(msg) {
            clear();
            text = msg;
            visible = true;
        }
        function clear() {
            text = "";
            visible = false;
        }

        font.pixelSize: NJUiConstants.UI_ERROR_TEXT_FONTSIZE;
        visible: false;
        anchors.fill: parent;
        horizontalAlignment: Text.AlignHCenter;
        verticalAlignment: Text.AlignVCenter;
        width: parent.width;
        wrapMode: Text.WordWrap;
        z: resultListView.z -1;
    }

    ListView {
        id: resultListView
        property real yPos: resultListView.visibleArea.yPosition;
        property int _prevCount: 0; // internal
        signal requestingMore;
        clip: true;
        delegate: jobInfoDelegate;
        spacing: 7;
        anchors.fill: parent;
        interactive: !moreData.visible;
        flickableDirection: Flickable.VerticalFlick;
        cacheBuffer: (100*30); // Each item occupies 100 in height.

        onAtYEndChanged:{
            // request for new data set
            if(atYEnd && loadMoreDataWhenRequired && count) {
                _prevCount = count;
                loading = true;
                model.next();
            }
        }
        onCountChanged: {
            loading = false;
        }
        onMovementStarted: scrolling();
        onMovementEnded: scrollingStopped();

        Component {
            id: jobInfoDelegate;
            Rectangle {
                // Each delegate recieves an instance of JobInfo class
                id: jobInfoView;
                property QtObject jobInfo: jobinfo; // role name for jobinfo
                width: (jobInfo.isValid())?(parent.width):(0);
                height:(jobInfo.isValid())?(jobInfoContainer.height):(0);
                visible: jobInfo.isValid();
                radius: 7;
                color: "transparent";
                anchors {
                    left: parent.left;
                    right: parent.right;
                    leftMargin: 10;
                    rightMargin: 10;
                }

                MouseArea {
                    anchors.fill: parent;
                    onClicked: {
                        viewedFlag.visible = false;
                        if(jobInfoView.jobInfo.isValid()) showDetails(jobInfoView.jobInfo);
                    }
                }

                Row {
                    id: jobInfoContainer
                    width: parent.width;
                    spacing: 12
                    Rectangle {
                        id: viewedFlag
                        height: parent.height - 10;
                        width: NJUiConstants.UI_VIEWED_FLAG_WIDTH;
                        color: NJUiConstants.UI_VIEWED_FLAG_COLOR;
                        smooth: true;
                    }

                    Column {
                        id: jobInfoColumn
                        width: parent.width - viewedFlag.width;
                        spacing: 4;

                        Label {
                            id: title;
                            text: jobInfo.title();
                            width: parent.width;
                            wrapMode: Text.WordWrap;
                            elide: Text.ElideRight
                            maximumLineCount: 1;
                            font.bold: true;
                            font.pixelSize: NJUiConstants.UI_RESULTVIEW_TITLE_FONT_SIZE;
                        }

                        Label {
                            id: employer
                            text: jobInfo.employer();
                            width: parent.width;
                            wrapMode: Text.WrapAnywhere;
                            elide: Text.ElideRight
                            maximumLineCount: 1;
                            font.pixelSize: NJUiConstants.UI_RESULTVIEW_SUBTITLE_FONT_SIZE;
                        }

                        Label {
                            id: location;
                            text: jobInfo.location();
                            width: parent.width;
                            wrapMode: Text.WordWrap
                            elide: Text.ElideRight
                            maximumLineCount: 1;
                            font.pixelSize: NJUiConstants.UI_RESULTVIEW_EXTRATEXT_FONT_SIZE;
                            color: "grey";
                        }

                        Rectangle {
                            id: seperator
                            height: NJUiConstants.UI_LINE_HEIGHT;
                            color: NJUiConstants.UI_LINE_COLOR;
                            anchors {
                                left: parent.left;
                                right: parent.right;
                            }
                        }
                    }
                }
            }
        } // jobInfoDelegate
    }

    Rectangle {
        id: moreData;
        visible: loading;
        width: parent.width;
        height: 50;
        color: "grey"
        z: resultListView.z + 1;
        opacity: 0.75;
        anchors.bottom: parent.bottom;
        BusyIndicator {
            width: 40;
            height: 40;
            anchors.centerIn: parent;
            running: true;
        }
    } // moredata

    ScrollDecorator { flickableItem: resultListView }
}// item
// eof
