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
#include "vicr123filebugwidget.h"
#include "ui_vicr123filebugwidget.h"

#include "totpcoderequest.h"
#include <QNetworkAccessManager>
#include <ttoast.h>
#include <tpopover.h>

struct Vicr123FileBugWidgetPrivate {
    QString authToken;
    QNetworkAccessManager mgr;
};

Vicr123FileBugWidget::Vicr123FileBugWidget(QWidget *parent) :
    tStackedWidget(parent),
    ui(new Ui::Vicr123FileBugWidget)
{
    ui->setupUi(this);

    d = new Vicr123FileBugWidgetPrivate();

    this->setCurrentAnimation(SlideHorizontal);

    ui->loginSpinner->setVisible(false);
    ui->loginSpinner->setFixedWidth(SC_DPI(16));
}

Vicr123FileBugWidget::~Vicr123FileBugWidget()
{
    delete d;
    delete ui;
}


tPromise<QNetworkReply*>* Vicr123FileBugWidget::get(QString url) {

}

tPromise<QNetworkReply*>* Vicr123FileBugWidget::post(QString url, QByteArray data, QMap<QByteArray, QByteArray> extraHeaders) {
    QUrl requestUrl;
    requestUrl.setScheme("https");
    requestUrl.setHost("bugs.vicr123.com");
    requestUrl.setPath(url);

    QNetworkRequest req(requestUrl);
    if (!d->authToken.isEmpty()) {
        req.setRawHeader("Authorization", d->authToken.toUtf8());
    }

    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setHeader(QNetworkRequest::UserAgentHeader, "Bonkers/1.0");

    for (QByteArray key : extraHeaders.keys()) {
        req.setRawHeader(key, extraHeaders.value(key));
    }

    QNetworkReply* reply = d->mgr.post(req, data.append("\r\n\r\n"));

    return new tPromise<QNetworkReply*>([=](QString& error) -> QNetworkReply* {
        QEventLoop* loop = new QEventLoop();
        connect(reply, SIGNAL(finished()), loop, SLOT(quit()));
        loop->exec();
        loop->deleteLater();

        /*if (reply->error() != QNetworkReply::NoError) {
            error = reply->errorString();
        }*/

        return reply;
    });
}

void Vicr123FileBugWidget::on_backButton_clicked()
{
    emit Cancel();
}

void Vicr123FileBugWidget::on_nextPageButton_clicked()
{
    this->setCurrentIndex(1);
}

void Vicr123FileBugWidget::on_backButton_2_clicked()
{
    this->setCurrentIndex(0);
}

void Vicr123FileBugWidget::attemptLogin(QString totpCode) {
    //Get a token
    QJsonObject loginObject;
    loginObject.insert("username", ui->username->text());
    loginObject.insert("password", ui->password->text());
    loginObject.insert("totpCode", totpCode);

    this->setEnabled(false);
    ui->loginSpinner->setVisible(true);
    post("/api/users/getToken", QJsonDocument(loginObject).toJson())->then([=](QNetworkReply* reply) {
        this->setEnabled(true);
        ui->loginSpinner->setVisible(false);

        bool showToast = false;
        tToast* toast = new tToast();
        switch (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt()) {
            case 200:
                //We're logged in!
                toast->setTitle(tr("Logged in!"));
                toast->setText(tr("Correct your password and try again."));
                showToast = true;
                break;
            case 401:
                //Either incorrect password or need 2FA code
                if (reply->readAll() == "TOTP Token Required") {
                    //We need to ask for a 2FA code
                    TotpCodeRequest* req = new TotpCodeRequest();
                    tPopover* popover = new tPopover(req);
                    popover->setPopoverWidth(SC_DPI(400));
                    connect(req, &TotpCodeRequest::cancel, popover, &tPopover::dismiss);
                    connect(req, &TotpCodeRequest::finished, req, [=](QString mfaCode) {
                        popover->dismiss();
                        attemptLogin(mfaCode);
                    });
                    connect(popover, &tPopover::dismissed, popover, [=] {
                        popover->deleteLater();
                        req->deleteLater();
                    });
                    popover->show(this);
                } else {
                    //Incorrect password
                    toast->setTitle(tr("Incorrect Password"));
                    toast->setText(tr("Correct your password and try again."));
                    showToast = true;
                }
                break;
            default:
                //Error!
                toast->setTitle(tr("Something went wrong"));
                toast->setText(tr("Couldn't get to the bug reporting server."));
                showToast = true;
                break;
        }

        if (showToast) {
            toast->show(this);
            connect(toast, &tToast::dismissed, toast, &tToast::deleteLater);
        } else {
            toast->deleteLater();
        }

        reply->deleteLater();
    })->error([=](QString error) {
        tToast* toast = new tToast();
        toast->setTitle(tr("Something went wrong"));
        toast->setText(error);
        toast->show(this);
        connect(toast, &tToast::dismissed, toast, &tToast::deleteLater);
    });

}

void Vicr123FileBugWidget::on_loginButton_clicked()
{
    attemptLogin("");
}
