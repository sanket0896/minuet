/****************************************************************************
**
** Copyright (C) 2016 by Sandro S. Andrade <sandroandrade@kde.org>
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License as
** published by the Free Software Foundation; either version 2 of
** the License or (at your option) version 3 or any later version
** accepted by the membership of KDE e.V. (or its successor approved
** by the membership of KDE e.V.), which shall act as a proxy
** defined in Section 14 of version 3 of the license.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#include "core.h"

#include "uicontroller.h"
#include "plugincontroller.h"
#include "exercisecontroller.h"

#include <interfaces/isoundbackend.h>

namespace Minuet
{

Core::~Core()
{
}

bool Core::initialize()
{
    if (m_self)
        return true;

    m_self = new Core;

    return true;
}

IPluginController *Core::pluginController()
{
    return m_pluginController;
}

ISoundBackend *Core::soundBackend()
{
    return m_soundBackend;
}

IExerciseController *Core::exerciseController()
{
    return m_exerciseController;
}

IUiController *Core::uiController()
{
    return m_uiController;
}

void Core::setSoundBackend(ISoundBackend *soundBackend)
{
    if (m_soundBackend != soundBackend) {
        m_soundBackend = soundBackend;
        emit soundBackendChanged(m_soundBackend);
    }
}

Core::Core(QObject *parent)
    : ICore(parent),
      m_soundBackend(0)
{
    m_pluginController = new PluginController(this);
    ((PluginController *)m_pluginController)->initialize(this);

    m_exerciseController = new ExerciseController(this);
    ((ExerciseController *)m_exerciseController)->initialize(this);

    m_uiController = new UiController(this);
    ((UiController *)m_uiController)->initialize(this);
}

}

