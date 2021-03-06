/**************************************************************************
 *  This file is part of QXmlEdit                                         *
 *  Copyright (C) 2016 by Luca Bellonda and individual contributors       *
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

#include "scxmlnavigatorwidget.h"
#include "ui_scxmlnavigatorwidget.h"
#include "modules/specialized/scxml/scxmlinfo.h"
#include "utils.h"

SCXMLNavigatorWidget::SCXMLNavigatorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SCXMLNavigatorWidget)
{
    _updatesEnabled = true ;
    _info = NULL ;
    _isInfoEnabled = false;
    ui->setupUi(this);
    ui->editCmd->setEnabled(false);
    ui->showCmd->setEnabled(false);
    ui->states->setHeaderLabel(tr("States"));
    _visIsFlat = false ;
}

SCXMLNavigatorWidget::~SCXMLNavigatorWidget()
{
    reset();
    delete ui;
}

void SCXMLNavigatorWidget::reset()
{
    ui->states->clear();
    if(NULL != _info) {
        delete _info ;
    }
}

void SCXMLNavigatorWidget::setEnabledInfo(const bool value)
{
    if(_isInfoEnabled != value) {
        _isInfoEnabled = value ;
    }
}

bool SCXMLNavigatorWidget::isEnabledInfo()
{
    return _isInfoEnabled ;
}

void SCXMLNavigatorWidget::applyNewInfo(SCXMLInfo *info)
{
    if(NULL == info) {
        reset();
        _info = info ;
    } else {
        if(!compareIfSameAndUpdateSchemas(_info, info)) {
            reset();
            _info = info ;
            redisplay(ui->flat->isChecked());
        } else {
            delete info;
        }
    }
}

void SCXMLNavigatorWidget::selectItem(Element *selection)
{
    if(_isInfoEnabled) {
        QTreeWidgetItem *item = _itemsByElement[selection];
        if(NULL != item) {
            _updatesEnabled = false;
            ui->states->setCurrentItem(item);
            ui->states->scrollToItem(item, QAbstractItemView::EnsureVisible);
            _updatesEnabled = true;
        }
    }
}

void SCXMLNavigatorWidget::redisplay(const bool isFlat)
{
    ui->states->setUpdatesEnabled(false);
    ui->states->clear();
    _itemsByElement.clear();
    _visIsFlat = isFlat ;
    foreach(SCXMLState *state, _info->children()) {
        loadState(state, NULL, isFlat);
    }
    if(_visIsFlat) {
        ui->states->sortByColumn(0, Qt::AscendingOrder);
        ui->states->setSortingEnabled(true);
    } else {
        ui->states->setSortingEnabled(false);
    }
    ui->states->setUpdatesEnabled(true);
}

void SCXMLNavigatorWidget::loadState(SCXMLState *state, QTreeWidgetItem *parentItem, const bool isFlat)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();
    QString stateName = state->id();
    if(state->isParallel()) {
        stateName += QString(" %1").arg(tr("parallel"));
    }
    item->setText(0, stateName);
    item->setData(0, Qt::UserRole, qVariantFromValue((void*)state));
    if(isFlat || (NULL == parentItem)) {
        ui->states->addTopLevelItem(item);
    } else {
        parentItem->addChild(item);
    }
    _itemsByElement[state->element()] = item ;
    item->setExpanded(true);
    foreach(SCXMLState *child, state->children()) {
        loadState(child, item, isFlat);
    }
}

bool SCXMLNavigatorWidget::compareIfSameAndUpdateSchemas(SCXMLInfo *oldOne, SCXMLInfo *newOne)
{
    if(NULL == oldOne) {
        return false;
    }
    if(!compareIfSameAndUpdateSchemas(oldOne->children(), newOne->children())) {
        return false;
    }
    return true ;
}

bool SCXMLNavigatorWidget::compareIfSameAndUpdateSchemas(QList<SCXMLState *> oldOne, QList<SCXMLState *> newOne)
{
    if(oldOne.size() != newOne.size()) {
        return false;
    }
    const int len = oldOne.size();
    for(int i = 0 ; i < len ; i ++) {
        SCXMLState *oldElm = oldOne.at(i);
        SCXMLState *newElm = newOne.at(i);
        if(oldElm->id() != newElm->id()) {
            return false;
        }
        if(oldElm->isParallel() != newElm->isParallel()) {
            return false;
        }
        oldElm->setElement(newElm->element());
        if(!compareIfSameAndUpdateSchemas(oldElm->children(), newElm->children())) {
            return false ;
        }

    }
    return true;
}

void SCXMLNavigatorWidget::on_showCmd_clicked()
{
    SCXMLState *state = getSelectedItem();
    if(NULL != state) {
        emit goToState(state->id(), state->element());
    }
}

void SCXMLNavigatorWidget::on_editCmd_clicked()
{
    SCXMLState *state = getSelectedItem();
    if(NULL != state) {
        emit editState(state->id(), state->element());
    }
}

void SCXMLNavigatorWidget::on_flat_clicked()
{
    const bool newFlat = ui->flat->isChecked();
    if(_visIsFlat != newFlat) {
        redisplay(newFlat);
    }
}

SCXMLState *SCXMLNavigatorWidget::getSelectedItem()
{
    if(ui->states->selectedItems().size() > 0) {
        QTreeWidgetItem *item = ui->states->selectedItems().at(0);
        QVariant data = item->data(0, Qt::UserRole);
        SCXMLState *state = (SCXMLState *)data.value<void*>();
        return state ;
    }
    return NULL;
}

void SCXMLNavigatorWidget::on_states_itemSelectionChanged()
{
    if(!_updatesEnabled) {
        return ;
    }
    bool isEnabled = false;
    SCXMLState *state = getSelectedItem();
    if(NULL != state) {
        isEnabled = true ;
        emit goToState(state->id(), state->element());
    }
    ui->editCmd->setEnabled(isEnabled);
    ui->showCmd->setEnabled(isEnabled);
}

void SCXMLNavigatorWidget::on_states_itemDoubleClicked(QTreeWidgetItem */*item*/, int /*column*/)
{
    SCXMLState *state = getSelectedItem();
    if(NULL != state) {
        emit editState(state->id(), state->element());
    }
}

void SCXMLNavigatorWidget::on_states_itemClicked(QTreeWidgetItem * /*item*/, int /*column*/)
{
    // nothing atm
}
