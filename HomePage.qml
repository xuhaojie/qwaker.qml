import QtQuick 2.15

Rectangle{
    id: home
    anchors.fill: parent
    property Item currentPage: homePage
    property int animationDuration: 250
    property string targetMac: ""
    property string targetName: ""
    ListView {
        id: listview
        focus: true
        clip: true
        width: parent.width
        height: parent.height - setupButton.height
        model: hostModel
        delegate: Rectangle {
            id: delegate
            width: listview.width
            height: home.height / 10
            Image {
                id: hostIcon
                anchors.left: parent.left
                height: delegate.height
                width: height
                source: (icon != undefined && icon.length > 0) ? icon: "images/computer.png"
            }

            Item{
                anchors.left: hostIcon.right
                anchors.right: removeBtn.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                Text {
                    id: hostName
                    anchors.left: parent.left
                    anchors.top: parent.top
                    height: delegate.height / 2
                    width: parent.width * 0.5
                    text: name
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: height *0.5
                    color: "black"
                }

                Text {
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                    height: delegate.height / 2
                    text: mac
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: height *0.5
                    color: "gray"
                }
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    var host = {
                        "icon": icon,
                        "name" : name,
                        "mac" : mac
                    };
                    powerConfirmBox.prompt = "Power on " + name + " ?"
                    powerConfirmBox.doModal();
                    powerConfirmBox.userParam = host
                }
            }
            Image {
                id: removeBtn
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                height: delegate.height * 0.5
                width: height
                source: "images/remove.jpg"

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        removeConfirmBox.prompt = "Delete item " + name + " ?"
                        removeConfirmBox.doModal();
                        removeConfirmBox.userParam = index
                    }
                }
            }
        }
    }

    Connections {
        target: removeConfirmBox
        function onItemClicked(index) {
            if(index === 0) {
                hostModel.remove(removeConfirmBox.userParam);
            }
        }
    }

    Connections {
        target: powerConfirmBox
        function onItemClicked(index) {
            var host = powerConfirmBox.userParam;
            if(index === 0) {
                targetName = host.name;
                targetMac = host.mac;

                waker.login();
            }
        }
    }

    Connections {
        target: waker
        function onSignalLoginResult (result) {
            if(result !== true){
                errorMessageBox.title = "ERROR";
                errorMessageBox.prompt= "Login failed!";
                errorMessageBox.doModal();
            } else {
                console.debug("login succeed.");
                waker.wakeUp(targetMac);
            }

        }
    }

    Connections {
        target: waker
        function onSignalExecuteCommandResult (result) {
            if(result !== true){
                errorMessageBox.title = "ERROR";
                errorMessageBox.prompt= "Execute command failed!";
                errorMessageBox.doModal();
            } else {
                console.debug("Execute command succeed.");
                waker.logout()
            }
        }
    }

    Connections {
        target: waker
        function onSignalLogoutResult (result) {
            if(result !== true){
                errorMessageBox.title = "ERROR";
                errorMessageBox.prompt= "Logout failed!";
                errorMessageBox.doModal();
            } else {
                var msg = "Wake " + targetMac + " succeed.";
                errorMessageBox.title = "INFO";
                errorMessageBox.prompt= "Command sent to " + targetName;
                errorMessageBox.doModal();
                console.debug("Logout succeed.");
                console.debug(msg);
            }
        }
    }


    Image {
        id: addButton
        height : home.height / 10
        width : height
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        source: "images/add.jpg"
        MouseArea {
            anchors.fill: parent
            onClicked: {
                switchTo(hostPage);
            }
            Connections {
                target: hostPage
                function onButtonClicked(button) {
                    switchTo(homePage)
                    if("Ok" === button) {
                        var host = {
                            "icon" : hostPage.hostIcon,
                            "name" : hostPage.hostName,
                            "mac" : hostPage.hostMac
                        };
                        hostModel.append(host);
                        listview.positionViewAtEnd()
                        hostModel.save();
                    }
                }
            }
        }
    }

    Image  {
        id: setupButton
        height : home.height / 10
        width : height
        anchors.bottom: parent.bottom
        anchors.left: parent.left

        source: "images/setup.png"
        MouseArea {
            anchors.fill: parent
            onClicked: {
                switchTo(setupPage);
            }
            Connections {
                target: setupPage
                function onButtonClicked() {
                    switchTo(homePage)
                }
            }
        }
    }
}

