/*
 *   SPDX-FileCopyrightText: 2017, 2018, 2019 Ivan Cukic <ivan.cukic (at) kde.org>
 *   SPDX-FileCopyrightText: 2020 Nate Graham <nate@kde.org>
 *
 *   SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

import QtQuick 2.1
import QtQuick.Controls 2.12

import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.extras 2.0 as PlasmaExtras

PlasmaExtras.ExpandableListItem {
    id: expandableListItem
    property var vaultsModelActions: plasmoid.nativeInterface.vaultsModel.actionsModel()

    icon: model.icon
    iconEmblem: model.message.length !== 0 ? "emblem-error" :
                            model.isOpened ? "emblem-mounted" :
                        model.isOfflineOnly ? "network-disconnect" :
                                                ""
    title: model.name
    subtitle: model.message
    defaultActionButtonAction: Action {
        icon.name: model.isOpened ? "lock" : "unlock"
        text: model.isOpened ? i18nd("plasmavault-kde", "Lock Vault") : i18nd("plasmavault-kde", "Unlock Vault")
        onTriggered: {
            vaultsModelActions.toggle(model.device);
        }
    }
    isBusy: Plasmoid.busy
    isEnabled: model.isEnabled
    contextualActionsModel: [
        Action {
            icon.name: "system-file-manager"
            text: model.isOpened ? i18nd("plasmavault-kde", "Show in File Manager") : i18nd("plasmavault-kde", "Unlock and Show in File Manager")
            onTriggered: vaultsModelActions.openInFileManager(model.device);
        },
        Action {
            icon.name: "window-close"
            text: i18nd("plasmavault-kde", "Forcefully Lock Vault")
            onTriggered: vaultsModelActions.forceClose(model.device);
            enabled: model.isOpened
        },
        Action {
            icon.name: "configure"
            text: i18nd("plasmavault-kde", "Configure Vault...")
            onTriggered: vaultsModelActions.configure(model.device);
        }
    ]
}
