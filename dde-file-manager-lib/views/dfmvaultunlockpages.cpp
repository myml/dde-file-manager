/*
 * Copyright (C) 2019 Deepin Technology Co., Ltd.
 *
 * Author:     Gary Wang <wzc782970009@gmail.com>
 *
 * Maintainer: Gary Wang <wangzichong@deepin.com>
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
#include "dfmvaultunlockpages.h"
#include "vault/interfaceactivevault.h"
#include "controllers/vaultcontroller.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QFrame>
#include <QToolTip>
#include <QEvent>

#include <DPushButton>
#include <DPasswordEdit>
#include <DMessageBox>

DFMVaultUnlockPages::DFMVaultUnlockPages(QWidget *parent)
    : DDialog (parent)
{
    this->setTitle(tr("Unlock File Vault"));
    this->setIcon(QIcon::fromTheme("dfm_safebox"));
    this->setMessage(tr("Verify your fingerprint or password"));
    this->setFixedSize(438, 260);

    QStringList btnList({tr("Cancel"), tr("Unlock")});
    addButton(btnList[0], false);
    addButton(btnList[1], true, ButtonType::ButtonRecommend);
    getButton(1)->setEnabled(false);

    m_passwordEdit = new DPasswordEdit(this);
    m_passwordEdit->lineEdit()->setPlaceholderText(tr("Password"));
    m_passwordEdit->lineEdit()->setMaxLength(24);
    m_passwordEdit->lineEdit()->installEventFilter(this);
    m_helpButton = new QPushButton(this);
    m_helpButton->setIcon(QIcon(":/icons/images/icons/light_32px.svg"));

    QFrame *mainFrame = new QFrame(this);
    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addWidget(m_passwordEdit);
    mainLayout->addWidget(m_helpButton);
    mainLayout->setContentsMargins(0, 10, 0, 25);
    mainFrame->setLayout(mainLayout);

    addContent(mainFrame);
    setSpacing(0);
    // 防止点击按钮后界面隐藏
    setOnButtonClickedClose(false);

    connect(this, &DFMVaultUnlockPages::buttonClicked, this, &DFMVaultUnlockPages::onButtonClicked);
    connect(m_passwordEdit, &DPasswordEdit::textChanged, this, &DFMVaultUnlockPages::onPasswordChanged);
    connect(VaultController::getVaultController(), &VaultController::signalUnlockVault, this, &DFMVaultUnlockPages::onVaultUlocked);
    connect(m_helpButton, &QPushButton::clicked, [this]{
        QString strPwdHint("");
        if (InterfaceActiveVault::getPasswordHint(strPwdHint)){
            strPwdHint.insert(0, tr("Password hint:"));
            m_passwordEdit->showAlertMessage(strPwdHint);
        }
    });
}

void DFMVaultUnlockPages::showEvent(QShowEvent *event)
{
    // 重置所有控件状态
    m_passwordEdit->lineEdit()->clear();
    QLineEdit edit;
    QPalette palette = edit.palette();
    m_passwordEdit->lineEdit()->setPalette(palette);
    m_passwordEdit->setEchoMode(QLineEdit::Password);
}

DFMVaultUnlockPages *DFMVaultUnlockPages::instance()
{
    static DFMVaultUnlockPages s_instance;
    return &s_instance;
}

void DFMVaultUnlockPages::onButtonClicked(const int &index)
{
    if (index == 1){
        QString strPwd = m_passwordEdit->text();

        QString strClipher("");
        if (InterfaceActiveVault::checkPassword(strPwd, strClipher)){
            VaultController::getVaultController()->unlockVault(strClipher);
        }else {
            // 设置密码输入框颜色
            m_passwordEdit->lineEdit()->setStyleSheet("background-color:rgb(245, 218, 217)");

            m_passwordEdit->showAlertMessage(tr("Wrong password"));
        }
        return;
    }

    close();
}

void DFMVaultUnlockPages::onPasswordChanged(const QString &pwd)
{
    if (!pwd.isEmpty()){
        getButton(1)->setEnabled(true);
        QLineEdit edit;
        QPalette palette = edit.palette();
        m_passwordEdit->lineEdit()->setPalette(palette);
    }else {
        getButton(1)->setEnabled(false);
    }
}

void DFMVaultUnlockPages::onVaultUlocked(int state)
{
    if (state == 0){
        accept();
    }else {
        //others
        QString msg = tr("Unlock failed,the error code is ") + QString::number(state);
        DMessageBox::information(this, tr("tips"), msg);
    }
}

