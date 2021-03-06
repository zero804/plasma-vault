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

#ifndef PLASMAVAULT_KDED_UI_NOTICE_WIDGET_H
#define PLASMAVAULT_KDED_UI_NOTICE_WIDGET_H

#include "dialogdsl.h"

class NoticeWidget: public DialogDsl::DialogModule {
    Q_OBJECT

public:
    enum Mode {
        ShowAlways,
        DoNotShowAgainOption
    };

    NoticeWidget(const QString &noticeId, const QString &message, Mode mode);

    ~NoticeWidget();

    PlasmaVault::Vault::Payload fields() const override;

    void aboutToBeShown() override;
    bool shouldBeShown() const override;
    void aboutToBeHidden() override;

private:
    class Private;
    QScopedPointer<Private> d;
};

inline DialogDsl::ModuleFactory notice(const QByteArray &noticeId,
                                       const QString &message,
                                       NoticeWidget::Mode mode = NoticeWidget::DoNotShowAgainOption)
{
    return [=] {
        return new NoticeWidget(noticeId, message, mode);
    };
}

#endif // include guard

