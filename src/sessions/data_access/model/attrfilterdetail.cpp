/**************************************************************************
 *  This file is part of QXmlEdit                                         *
 *  Copyright (C) 2012 by Luca Bellonda and individual contributors       *
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


#include "attrfilterdetail.h"

AttrFilterDetail::AttrFilterDetail()
{
}

AttrFilterDetail::~AttrFilterDetail()
{
}


QStringList AttrFilterDetail::names()
{
    return _names;
}

void AttrFilterDetail::clear()
{
    _names.clear();
}


void AttrFilterDetail::addName(const QString &name)
{
    _names.append(name.trimmed());
}

bool AttrFilterDetail::compareTo(const AttrFilterDetail * other)
{
    bool isEquals = true ;
    if(NULL == other) {
        isEquals = false;
    } else {
        if(_names.size() != other->_names.size()) {
            isEquals = false;
        } else {
            foreach(QString nm, _names) {
                if(!other->_names.contains(nm)) {
                    isEquals = false;
                    break;
                }
            }
        }
    }
    return isEquals;
}
