/*
    Copyright (C) 2014 Sebastian Ivarsson
    Contact: Sebastian Ivarsson <sebiva@student.chalmers.se>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


function removespecials(text) {
    var ans = text;
    var i;

    for(i=0;i<ans.length;i++) {
        if (ans.substring(i, i+1)=== "å" || ans.substring(i,i+1) === "Å") {
            ans = ans.substring(0, i) + "%C5" + ans.substring(i+1);
        } else if (ans.substring(i, i+1) === "ä" || ans.substring(i, i+1) === "Ä") {
            ans = ans.substring(0, i) + "%C4" + ans.substring(i+1);
        } else if (ans.substring(i, i+1) === "ö" || ans.substring(i, i+1) === "Ö") {
            ans = ans.substring(0, i) + "%D6" + ans.substring(i+1);
        }
    }
    return ans;
}

//Returns the attribute as a string
function getAttr(string, attr) {
    var start, end;


    start = string.indexOf(attr + '="') + attr.length + 2;
    if (start === -1 + attr.length + 2) {
        return null;
    }

    end = string.indexOf('"', start+1);
    if (end === -1) {
        return null;
    }

    //console.log(string.indexOf(attr + "=") + " sta " + start + ": "+ string.indexOf('"', start+1) + "end " + end);
    return string.substring(start, end)
}

//Returns the start and end positions
function getTag(string, stag, etag) {
    var res = [0,0];
    res[0] = string.indexOf(stag) + stag.length;
    res[1] = string.indexOf(etag, res[0]);
    if (res[0] ===-1 + stag.length|| res[1] ===-1) {
        return null;
    }
    //console.log("STAG: " + stag + ", ETAG: " + etag + ", Pos: "+ res[0]+":" +res[1]);
    return res;
}



