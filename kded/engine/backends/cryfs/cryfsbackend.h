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

#ifndef PLASMAVAULT_KDED_ENGINE_BACKENDS_CRYFS_BACKEND_H
#define PLASMAVAULT_KDED_ENGINE_BACKENDS_CRYFS_BACKEND_H

#include "../../fusebackend_p.h"

namespace PlasmaVault {

class CryFsBackend: public FuseBackend {
public:
    CryFsBackend();
    ~CryFsBackend() override;

    static Backend::Ptr instance();

    bool isInitialized(const Device &device) const override;

    FutureResult<> validateBackend() override;

    QString name() const override { return "cryfs"; }

protected:
    FutureResult<> mount(const Device &device,
                         const MountPoint &mountPoint,
                         const Vault::Payload &payload) override;

private:
    QProcess *cryfs(const QStringList &arguments) const;
};

} // namespace PlasmaVault

#endif // include guard

