/*
 *   SPDX-FileCopyrightText: 2017 Ivan Cukic <ivan.cukic (at) kde.org>
 *
 *   SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

#ifndef PLASMAVAULT_KDED_UI_VAULT_DELETION_WIDGET_H
#define PLASMAVAULT_KDED_UI_VAULT_DELETION_WIDGET_H

#include "dialogdsl.h"

class VaultDeletionWidget: public DialogDsl::DialogModule {
    Q_OBJECT

public:
    VaultDeletionWidget();

    ~VaultDeletionWidget();

    PlasmaVault::Vault::Payload fields() const override;

    void init(const PlasmaVault::Vault::Payload &payload) override;

private:
    class Private;
    QScopedPointer<Private> d;
};

inline DialogDsl::ModuleFactory vaultDeletion()
{
    return [=] {
        return new VaultDeletionWidget();
    };
}

#endif // include guard

