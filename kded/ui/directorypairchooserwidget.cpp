/*
 *   Copyright 2017 by Ivan Cukic <ivan.cukic (at) kde.org>
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License as
 *   published by the Free Software Foundation; either version 2 of
 *   the License or (at your option) version 3 or any later version
 *   accepted by the membership of KDE e.V. (or its successor approved
 *   by the membership of KDE e.V.), which shall act as a proxy
 *   defined in Section 14 of version 3 of the license.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "directorypairchooserwidget.h"
#include "ui_directorypairchooserwidget.h"

#include "vault.h"
#include "backend_p.h"

#include <QStandardPaths>

class DirectoryPairChooserWidget::Private {
public:
    Ui::DirectoryPairChooserWidget ui;
    const DirectoryPairChooserWidget::Flags flags;

    DirectoryPairChooserWidget *const q;

    class DirectoryValidator {
    public:
        bool requireNew;
        bool requireExisting;
        bool valid = false;
        std::function<void()> update;

        DirectoryValidator(bool requireNew, bool requireExisting,
                           std::function<void()> update)
            : requireNew(requireNew)
            , requireExisting(requireExisting)
            , valid(!requireNew && !requireExisting)
            , update(update)
        {
        }

        bool isValid(const QUrl &url) const
        {
            if (url.isEmpty()) return false;

            const bool directoryExists
                = PlasmaVault::Backend::directoryExists(url.toLocalFile());

            if (requireNew && directoryExists) {
                return false;
            }

            if (requireExisting && !directoryExists) {
                return false;
            }

            return true;
        }

        void updateFor(const QUrl &url)
        {
            bool newValid = isValid(url);

            if (valid != newValid) {
                valid = newValid;
                update();
            }
        }
    };

    DirectoryValidator deviceValidator;
    DirectoryValidator mountPointValidator;
    bool allValid;

    void updateValidity()
    {
        bool newAllValid = deviceValidator.valid && mountPointValidator.valid;

        if (allValid != newAllValid) {
            allValid = newAllValid;
            q->setIsValid(allValid);
        }
    }

    Private(DirectoryPairChooserWidget *parent,
            DirectoryPairChooserWidget::Flags flags)
        : flags(flags)
        , q(parent)
        , deviceValidator(
                flags & RequireNewDevice,
                flags & RequireExistingDevice,
                [&] { updateValidity(); }
            )
        , mountPointValidator(
                flags & RequireNewMountPoint,
                flags & RequireExistingMountPoint,
                [&] { updateValidity(); }
            )
        , allValid(deviceValidator.valid && mountPointValidator.valid)
    {
    }
};

DirectoryPairChooserWidget::DirectoryPairChooserWidget(
    DirectoryPairChooserWidget::Flags flags)
    : DialogDsl::DialogModule(false), d(new Private(this, flags))
{
    d->ui.setupUi(this);

    if (!(flags & DirectoryPairChooserWidget::ShowDevicePicker)) {
        d->ui.editDevice->setVisible(false);
        d->ui.labelDevice->setVisible(false);
    }

    if (!(flags & DirectoryPairChooserWidget::ShowMountPointPicker)) {
        d->ui.editMountPoint->setVisible(false);
        d->ui.labelMountPoint->setVisible(false);
    }

    connect(d->ui.editDevice, &KUrlRequester::textEdited,
            this, [&] () {
                d->deviceValidator.updateFor(d->ui.editDevice->url());
            });

    connect(d->ui.editMountPoint, &KUrlRequester::textEdited,
            this, [&] () {
                d->mountPointValidator.updateFor(d->ui.editMountPoint->url());
            });
}


DirectoryPairChooserWidget::~DirectoryPairChooserWidget()
{
}


PlasmaVault::Vault::Payload DirectoryPairChooserWidget::fields() const
{
    return {
        { KEY_DEVICE,      d->ui.editDevice->url().toLocalFile() },
        { KEY_MOUNT_POINT, d->ui.editMountPoint->url().toLocalFile() }
    };
}



void DirectoryPairChooserWidget::init(
    const PlasmaVault::Vault::Payload &payload)
{
    if (d->flags & DirectoryPairChooserWidget::AutoFillPaths) {
        const QString basePath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation)
                + QStringLiteral("/plasma-vault");

        const auto name = payload[KEY_NAME].toString();

        Q_ASSERT(!name.isEmpty());

        QString path = QString("%1/%2.enc").arg(basePath).arg(name);
        int index = 1;
        while (QDir(path).exists()) {
            path = QString("%1/%2_%3.enc").arg(basePath).arg(name).arg(index++);
        }

        d->ui.editDevice->setText(path);
        d->ui.editMountPoint->setText(QDir::homePath() + QStringLiteral("/Vaults/") + name);
    }

    d->deviceValidator.updateFor(d->ui.editDevice->url());
    d->mountPointValidator.updateFor(d->ui.editMountPoint->url());
    setIsValid(d->allValid);
}

