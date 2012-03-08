import QtQuick 1.1
import com.nokia.meego 1.0
import "NJConstants.js" as NJConstants

Page {
    id: searchView;
    property string msg: NJConstants.ADDALERTPAGE;

    NJHeader {
        id: header;
        title: "Job alert";
    }

    Column {
        id: searchBox;
        spacing: 7;
        anchors {
            top: header.bottom;
            topMargin: 5;
            left: parent.left;
            leftMargin: 10;
            right: parent.right;
            rightMargin: 10;
        }

        TextField {
            id: skill;
            placeholderText: "Skill like C++,Qt,Java"
        }

        Button {
            id: add
            text: "Add";
            onClicked: {
                if(skill.text.length)
                    njengine.addAlert({"skill" : skill.text});
            }
        }
    }
}

// eof
