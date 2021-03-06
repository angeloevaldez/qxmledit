/**************************************************************************
 *  This file is part of QXmlEdit                                         *
 *  Copyright (C) 2011 by Luca Bellonda and individual contributors       *
 *    as indicated in the AUTHORS file                                    *
 *  lbellonda _at_ gmail.com                                              *
 *                                                                        *
 * This library is free software; you can redistribute it and/or          *
 * modify it under the terms of the GNU Library General Public            *
 * License as published by the Free Software Foundation; either           *
 * version 2 of the License, or (at your option) any later version.       *
 *                                                                        *
 * This library is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU      *
 * Library General Public License for more details.                       *
 *                                                                        *
 * You should have received a copy of the GNU Library General Public      *
 * License along with this library; if not, write to the                  *
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,       *
 * Boston, MA  02110-1301  USA                                            *
 **************************************************************************/

#ifndef WELCOMEDIALOG_H
#define WELCOMEDIALOG_H

#include <QDialog>
#include <QStringListModel>

namespace Ui
{
class WelcomeDialog;
}

class WelcomeDialog : public QDialog
{
    Q_OBJECT
public:

    enum EResult {
        E_CLOSE,
        E_OPENSCHEMA,
        E_SCANFILE,
        E_RECENTFILECHOOSE,
        E_MANAGESESSIONS,
        E_VIEWDATA,
        E_COMPARE
    };

private:
    EResult _result ;
    QString _fileChoosen;
    bool _showNextTime;
    QStringListModel _recentModel;

public:

    explicit WelcomeDialog(QStringList &recentFiles, QWidget *parent = 0);
    ~WelcomeDialog();

    EResult result();
    QString fileChoosen();
    bool isShowNextTime();

private:
    Ui::WelcomeDialog *ui;

public slots:
    void reject();
    void accept();

private slots:
    void on_examineSchema_clicked();
    void on_splitFile_clicked();
    void on_chooseRecent_clicked();
    void on_recentFiles_doubleClicked(const QModelIndex & index);
    void on_cmdManageSessions_clicked();
    void on_cmdViewData_clicked();
    void on_compareFiles_clicked();
};

#endif // WELCOMEDIALOG_H
