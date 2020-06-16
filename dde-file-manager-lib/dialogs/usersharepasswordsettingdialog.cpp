/*
 * Copyright (C) 2016 ~ 2018 Deepin Technology Co., Ltd.
 *               2016 ~ 2018 dragondjf
 *
 * Author:     dragondjf<dingjiangfeng@deepin.com>
 *
 * Maintainer: dragondjf<dingjiangfeng@deepin.com>
 *             zccrs<zhangjide@deepin.com>
 *             Tangtong<tangtong@deepin.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "usersharepasswordsettingdialog.h"
#include "../dbusinterface/usershare_interface.h"
#include "../app/define.h"
#include "singleton.h"
#include "usershare/usersharemanager.h"
#include <QDebug>
#include <QProcess>
#include <QDBusReply>
#include <QVBoxLayout>
#include <QPushButton>

UserSharePasswordSettingDialog::UserSharePasswordSettingDialog(QWidget *parent) : DDialog(parent)
{
    setTitle(tr("Enter a password to protect shared folders"));
    setIcon(QIcon::fromTheme("dialog-password-publicshare"));
    initUI();
}

void UserSharePasswordSettingDialog::initUI()
{
    QStringList buttonTexts;
    buttonTexts << tr("Cancel") << tr("Confirm");
    addButton(buttonTexts[0], false);
    addButton(buttonTexts[1], false, DDialog::ButtonRecommend);
    setDefaultButton(1);
    m_passwordEdit = new DPasswordEdit(this);
    m_passwordEdit->setFocus();
    addContent(m_passwordEdit);
    setContentsMargins(0,0,0,0);
    getButton(1)->setEnabled(false);

    connect(m_passwordEdit,&DPasswordEdit::textChanged,this,[this]{
        getButton(1)->setEnabled(!m_passwordEdit->text().isEmpty());
    });

}

void UserSharePasswordSettingDialog::onButtonClicked(const int &index)
{
    if(index == 1) {
        // set usershare password
        QString password = m_passwordEdit->text();
        if (password.isEmpty()) {
            close();
            return;
        }
        userShareManager->setSambaPassword(UserShareManager::getCurrentUserName(), password);
    }
    close();
}
