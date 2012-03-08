// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import com.nokia.meego 1.0
import "NJUiConstants_harmattan.js" as NJUiConstants;

Rectangle {
    id: searchBar
    property Item searchBarContent: searchContainer;
    property alias skill: generalSearchBox.text;
    property alias location: locationSearchBox.text;
    property alias country: countrySelection.text;

    signal searchEntered;
    function disappear() {
        if(appearTimer.running)
            appearTimer.stop();
        if(appearAnimation.running)
            appearAnimation.stop();
        disappearAnimation.start();
    }

    function appearWithDelay() {
        if(disappearAnimation.running)
            disappearAnimation.stop();
        appearTimer.start();
    }

    width: parent.width;
    height: searchBarContent.height;
    color: "transparent";

    Timer {
        id: appearTimer;
        interval: 2000;
        repeat: false;
        onTriggered: {
            if(appearAnimation.running)
                appearAnimation.stop();
            appearAnimation.start();
        }
    }

    ParallelAnimation {
        id: appearAnimation;
        PropertyAnimation {
            target: searchBar;
            property: "opacity"
            duration: 600;
            from: 0;
            to: 1;
        }
        PropertyAnimation {
            target: searchBar;
            property: "height"
            from: 0;
            to: searchBarContent.height;
            duration: 800;
        }
    }

    ParallelAnimation {
        id: disappearAnimation;
        PropertyAnimation {
            target: searchBar;
            property: "opacity"
            duration: 600;
            from: 1;
            to: 0;
        }
        PropertyAnimation {
            target: searchBar;
            property: "height"
            from: searchBarContent.height;
            to: 0;
            duration: 800;
        }
    }

    anchors {
        left: parent.left;
        right: parent.right;
        leftMargin: 7;
        rightMargin: 7;
    }

    Column {
        id: searchContainer;
        width: parent.width;
        spacing: 7;

        Row {
            id: generalSearchRow
            width: parent.width;
            spacing: 10;
            TextField {
                id: generalSearchBox
                placeholderText: "Add your skill";
                width: parent.width - (searchButton.width+parent.spacing);
            }

            // TODO: replace with tool button
            Button {
                id: searchButton;
                text: "Go"
                width: 70;
                onClicked: {
                    advancedSearchFields.hideAdvancedSearch();
                    searchEntered();
                }
            }
        }

        Row {
            id: advancedSearchFields;

            function showAdvancedSearch() {
                advancedSearchToogleText.text = "Hide advanced search";
                advancedSearchShown = true;
                advancedSearchContainer.show();
            }

            function hideAdvancedSearch() {
                advancedSearchToogleText.text = "Show advanced search";
                advancedSearchShown = false;
                advancedSearchContainer.hide();
            }

            function toogle() {
                if(advancedSearchShown)
                    hideAdvancedSearch();
                else
                    showAdvancedSearch();
            }

            property bool advancedSearchShown: false;
            width: parent.width;
            spacing: 10;

            Text {
                id: advancedSearchToogleText;
                text: "Show advanced search";
                font.pixelSize: NJUiConstants.UI_SEARCHBAR_ADVSEARCH_FONT_SIZE;
            }

            Rectangle {
                id: simpleLine
                width: parent.width - (advancedSearchToogleText.width + advancedSearchFields.spacing);
                height: NJUiConstants.UI_LINE_HEIGHT;
                anchors.verticalCenter: parent.verticalCenter;
            }

            MouseArea {
                anchors.fill: parent;
                onClicked: advancedSearchFields.toogle();
            }
        }

        Column {
            id: advancedSearchContainer;

            function show() {
                visible = true;
            }

            function hide() {
                visible = false;
            }

            width: parent.width;
            visible: false;
            spacing: 10;

            Row {
                id: locationSearchRow;
                width: parent.width;
                spacing: 10;

                Label {
                    id: locationLabel
                    text: "Location"
                    width: (NJUiConstants.UI_TEXTFIELD_LABEL_PERCENTAGE_WIDTH*parent.width)/100; // % of parent width
                    anchors.verticalCenter: parent.verticalCenter;
                }
                TextField {
                    id: locationSearchBox
                    placeholderText: "Add location";
                    width: parent.width - (locationLabel.width+locationSearchRow.spacing);
                }
            }

            Row {
                id: coutrySelectionRow;
                width: parent.width;
                spacing: 10;

                 Label {
                     id: countryLabel;
                     text: "Country"
                     width: (NJUiConstants.UI_TEXTFIELD_LABEL_PERCENTAGE_WIDTH*parent.width)/100; // % of parent width
                     anchors.verticalCenter: parent.verticalCenter;
                 }

                 Button {
                    id: countrySelection
                    // TODO: get default country from engine
                    function selectedValue() {
                        if(text !== placeHolderText)
                            return text;
                        else
                            return "";
                    }

                    property string placeHolderText: "Select Country";
                    text: (countrySelectionDialog.selectedIndex >=0)
                              ?(countrySelectionDialog.model.get(countrySelectionDialog.selectedIndex).name)
                              :(placeHolderText)
                    onClicked: countrySelectionDialog.open();
                 }
            }

            // TODO: This is occupying whole screen, even when not visible. Fix it.
            SelectionDialog {
                id: countrySelectionDialog
                titleText: "Select country"
                model: ListModel {
                    ListElement { name: "India" }
                    ListElement { name: "USA" }
                    ListElement { name: "Australia" }
                }
            }

        }
    }
}

// eof