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
#include "crasheddialog.h"
#include "ui_crasheddialog.h"

#include <tvariantanimation.h>
#include <QProcess>
#include <QFileDialog>
#include <QFontDatabase>

#include "vicr123filebugwidget/vicr123filebugwidget.h"

struct CrashedDialogPrivate {
    CrashedArgs args;
};

CrashedDialog::CrashedDialog(CrashedArgs args, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CrashedDialog)
{
    ui->setupUi(this);

    d = new CrashedDialogPrivate();
    d->args = args;

    ui->titleLabel->setText(args.appName);
    ui->messageLabel->setText(tr("Sorry, %1 had a problem and has stopped working.").arg(args.appName));
    ui->relaunchButton->setText(tr("Relaunch %1").arg(args.appName));
    ui->closeButton->setText(tr("Exit %1").arg(args.appName));

    QSize buttonIconSize = QSize(16, 16) * theLibsGlobal::getDPIScaling();
    ui->relaunchButton->setIconSize(buttonIconSize);
    ui->closeButton->setIconSize(buttonIconSize);
    ui->moreInfoButton->setIconSize(buttonIconSize);
    ui->fileBugButton->setIconSize(buttonIconSize);
    ui->fileBugButton->setVisible(false);

    ui->backtraceBrowser->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));

    ui->stackedWidget->setCurrentAnimation(tStackedWidget::SlideHorizontal);
    this->setFixedSize(QSize(SC_DPI(500), ui->initialPage->sizeHint().height()));

    if (args.appPath.isEmpty()) {
        //Can't relaunch the app because no path was provided
        ui->relaunchButton->setEnabled(false);
    }

    Vicr123FileBugWidget* fileBug = new Vicr123FileBugWidget();
    connect(fileBug, &Vicr123FileBugWidget::Cancel, [=] {
        ui->stackedWidget->setCurrentIndex(0);
    });
    ui->stackedWidget->addWidget(fileBug);
}

CrashedDialog::~CrashedDialog()
{
    delete d;
    delete ui;
}

void CrashedDialog::on_relaunchButton_clicked()
{
    //Relaunch the crashed app
    QProcess::startDetached(d->args.appPath);
    this->close();
}

void CrashedDialog::setBacktrace(QStringList backtrace) {
    ui->backtraceBrowser->setPlainText(backtrace.join("\n"));
}

void CrashedDialog::on_backButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void CrashedDialog::on_moreInfoButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void CrashedDialog::on_stackedWidget_switchingFrame(int arg1)
{
    tVariantAnimation* anim = new tVariantAnimation();
    anim->setStartValue(this->geometry());

    QRect newGeometry;
    switch (arg1) {
        case 0:
            newGeometry.setSize(QSize(SC_DPI(500), ui->initialPage->sizeHint().height()));
            break;
        case 1:
        case 2: //File Bug
            newGeometry.setSize(QSize(800, 600) * theLibsGlobal::getDPIScaling());
            break;
    }
    newGeometry.moveCenter(this->geometry().center());

    anim->setEndValue(newGeometry);
    anim->setDuration(250);
    anim->setEasingCurve(QEasingCurve::OutCubic);
    connect(anim, &tVariantAnimation::valueChanged, [=](QVariant value) {
        this->setFixedSize(value.toRect().size());
        this->setGeometry(value.toRect());
    });
    connect(anim, &tVariantAnimation::finished, anim, &tVariantAnimation::deleteLater);
    anim->start();
}

void CrashedDialog::on_saveInfoButton_clicked()
{
    QEventLoop* loop = new QEventLoop();
    QFileDialog* saveDialog = new QFileDialog(this, Qt::Sheet);
    saveDialog->setWindowModality(Qt::WindowModal);
    saveDialog->setAcceptMode(QFileDialog::AcceptSave);
    saveDialog->setDirectory(QDir::home());
    saveDialog->setNameFilters(QStringList() << tr("Text File") + " (*.txt)");
    connect(saveDialog, SIGNAL(finished(int)), saveDialog, SLOT(deleteLater()));
    connect(saveDialog, SIGNAL(finished(int)), loop, SLOT(quit()));
    saveDialog->show();

    //Block until dialog is finished
    loop->exec();
    loop->deleteLater();

    if (saveDialog->result() == QDialog::Accepted) {
        QFile file(saveDialog->selectedFiles().first());
        file.open(QFile::WriteOnly);
        file.write(ui->backtraceBrowser->toPlainText().toUtf8());
        file.close();
    }
}

void CrashedDialog::on_closeButton_clicked()
{
    this->close();
}

void CrashedDialog::on_fileBugButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}
