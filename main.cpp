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
#include <QApplication>

#include <QCommandLineParser>
#include <QTextStream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCommandLineParser parser;
    parser.addOption({"appname", "Application Name", "appname"});
    parser.addOption({"apppath", "Application Path", "apppath"});
    parser.addOption({"bt", "Read a backtrace"});
    parser.addHelpOption();
    parser.process(a);

    CrashedArgs args;
    args.appName = parser.value("appname");
    args.appPath = parser.value("apppath");

    CrashedDialog w(args);

    //We want to read from stdin
    if (parser.isSet("bt")) {
        QTextStream s(stdin, QIODevice::ReadOnly);

        QStringList buf;
        QString currentLine;
        do {
            currentLine = s.readLine();
            buf.append(currentLine);
        } while (currentLine != "");

        w.setBacktrace(buf);
    }

    w.show();

    return a.exec();
}
