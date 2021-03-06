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

import QtQuick 2.7

Rectangle {
    property int keyWidth: (parent.width - 80)/52
    property int keyHeight: 3.4*keyWidth

    function noteOn(chan, pitch, vel) {
        if (vel > 0)
            highlightKey(pitch, "#778692")
        else
            noteOff(chan, pitch, vel)
    }
    function noteOff(chan, pitch, vel) {
        highlightKey(pitch, ([1,3,6,8,10].indexOf(pitch % 12) > -1) ? "black":"white")
    }
    function noteMark(chan, pitch, vel, color) {
        noteMark.createObject(itemForPitch(pitch), { color: color })
    }
    function noteUnmark(chan, pitch, vel, color) {
        var item = itemForPitch(pitch).children[1]
        if (item != undefined)
            item.destroy()
    }
    function clearAllMarks() {
        for (var index = 21; index <= 108; ++index) {
            noteOff(0, index, 0)
            var markItem = itemForPitch(index).children[1]
            if (markItem != undefined)
                markItem.destroy()
        }
    }
    function highlightKey(pitch, color) {
        itemForPitch(pitch).color = color
    }
    function itemForPitch(pitch) {
        var noteItem
        if (pitch < 24) {
            noteItem = keyboard.children[pitch-21]
        } else if (pitch == 108) {
            noteItem = whiteKeyC
        } else {
            var note = (pitch - 24) % 12
            var octave = (pitch - 24 - note) / 12
            noteItem = keyboard.children[3+octave].children[note]
        }
        return noteItem
    }

    width: 3*keyWidth+7*(7*keyWidth) + 20; height: keyHeight + 30
    radius: 5
    color: "#141414"

    Item {
        id: keyboard

        anchors { horizontalCenter: parent.horizontalCenter; bottom: parent.bottom; bottomMargin: 5 }
        width: 3*keyWidth+7*(7*keyWidth); height: keyHeight

        WhiteKey { id: whiteKeyA }
        BlackKey { anchor: whiteKeyA }
        WhiteKey { id: whiteKeyB; anchor: whiteKeyA }
        Octave { id: octave1; initialAnchor: whiteKeyB }
        Octave { id: octave2; initialAnchor: octave1 }
        Octave { id: octave3; initialAnchor: octave2 }
        Octave { id: octave4; initialAnchor: octave3 }
        Octave { id: octave5; initialAnchor: octave4 }
        Octave { id: octave6; initialAnchor: octave5 }
        Octave { id: octave7; initialAnchor: octave6 }
        WhiteKey { id: whiteKeyC; anchor: octave7 }
        Rectangle {
            width: 3*keyWidth+7*(7*keyWidth); height: 2
            anchors { left: whiteKeyA.left; bottom: whiteKeyA.top }
            color: "#A40E09"
        }
    }
    Component {
        id: noteMark

        Rectangle {
            width: keyWidth - 4; height: keyWidth - 4
            radius: (keyWidth - 4)/2
            border.color: "black"
            anchors { horizontalCenter: parent.horizontalCenter; bottom: parent.bottom; bottomMargin: 2 }
        }
    }
}
