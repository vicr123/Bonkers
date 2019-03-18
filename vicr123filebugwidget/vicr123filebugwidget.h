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
#ifndef VICR123FILEBUGWIDGET_H
#define VICR123FILEBUGWIDGET_H

#include <tstackedwidget.h>
#include <tpromise.h>
#include <QNetworkReply>

namespace Ui {
    class Vicr123FileBugWidget;
}

struct Vicr123FileBugWidgetPrivate;
class Vicr123FileBugWidget : public tStackedWidget
{
        Q_OBJECT

    public:
        explicit Vicr123FileBugWidget(QWidget *parent = nullptr);
        ~Vicr123FileBugWidget();

    private slots:
        void on_backButton_clicked();

        void on_nextPageButton_clicked();

        void on_backButton_2_clicked();

        void on_loginButton_clicked();

        void attemptLogin(QString totpCode);

    signals:
        void Cancel();

    private:
        Ui::Vicr123FileBugWidget *ui;

        Vicr123FileBugWidgetPrivate* d;

        tPromise<QNetworkReply*>* get(QString url);
        tPromise<QNetworkReply*>* post(QString url, QByteArray data, QMap<QByteArray, QByteArray> extraHeaders = QMap<QByteArray, QByteArray>());
};

#endif // VICR123FILEBUGWIDGET_H
