/****************************************
 *
 *   INSERT-PROJECT-NAME-HERE - INSERT-GENERIC-NAME-HERE
 *   Copyright (C) 2019 Victor Tran
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * *************************************/
#include "totpcoderequest.h"
#include "ui_totpcoderequest.h"

TotpCodeRequest::TotpCodeRequest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TotpCodeRequest)
{
    ui->setupUi(this);
}

TotpCodeRequest::~TotpCodeRequest()
{
    delete ui;
}

void TotpCodeRequest::on_cancelButton_clicked()
{
    emit cancel();
}

void TotpCodeRequest::on_mfaCode_textEdited(const QString &arg1)
{
    if (arg1.size() == 6) {
        emit finished(arg1);
    }
}
