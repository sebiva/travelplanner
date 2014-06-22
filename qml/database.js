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

.import QtQuick.LocalStorage 2.0 as LS

function getDatabase() {
    return LS.LocalStorage.openDatabaseSync("Travelplanner", "1.0", "StorageDatabase", 100);
}

/*
  Sets up the database, creating the necessary tables if needed.
  */
function setup() {
    var db = getDatabase();
    var res = 0
    db.transaction(function (trans) {
        // Check if there are already any tables
        var x = trans.executeSql('SELECT name FROM sqlite_master WHERE type="table" AND name="settings"')
        var tablesexist = x.rows.length > 0

        var transfer = false
        if (tablesexist) {
            // Get the version number to see if old favourites and lastsearch need to be imported
            x = trans.executeSql('SELECT valueattr FROM settings WHERE keyattr = "version"')
            var ver = x.rows.length === 0 ? 0 : x.rows.item(0)['valueattr'].valueOf()
            if (ver < 0.5) {
                trans.executeSql('ALTER TABLE favourites RENAME TO oldfavs')
                trans.executeSql('ALTER TABLE lastsearch RENAME TO oldlast')
                trans.executeSql('INSERT OR REPLACE INTO settings VALUES("version", "0.5")')
                transfer = true
            }
        }
        // Create the tables
        trans.executeSql('CREATE TABLE IF NOT EXISTS favourites(backend TEXT, fromid TEXT, toid TEXT, fromstop TEXT, tostop TEXT, time INTEGER, PRIMARY KEY(backend, fromid, toid))');
        trans.executeSql('CREATE TABLE IF NOT EXISTS lastsearch(backend TEXT, fromid TEXT, toid TEXT, fromstop TEXT, tostop TEXT, PRIMARY KEY(backend))')
        trans.executeSql('CREATE TABLE IF NOT EXISTS settings(keyattr TEXT PRIMARY KEY, valueattr TEXT)')
        if (!transfer) {
            // Set Västtrafik as default
            trans.executeSql('INSERT INTO settings VALUES("backend","Västtrafik")')
        }


        // Transfer old favourites and lastsearches if needed
        if (transfer) {
            trans.executeSql('INSERT INTO favourites(backend,fromid,toid,fromstop,tostop,time) SELECT "Västtrafik",fromid,toid,fromstop,tostop,time FROM oldfavs')
            trans.executeSql('INSERT INTO lastsearch(backend,fromid,toid,fromstop,tostop) SELECT "Västtrafik",fromid,toid,fromstop,tostop FROM oldlast')
            trans.executeSql('DROP TABLE oldfavs')
            trans.executeSql('DROP TABLE oldlast')
        }
        res = 1
    });
    return res
}
/*
  Sets a setting in the database to the given value. Returns 1 if successfull, otherwise 0.
  */
function setsetting(setting, value) {
    var db = getDatabase();
    var result;
    db.transaction(function (trans) {
        // Clear the old value
        trans.executeSql('DELETE FROM settings WHERE keyattr=?',[setting])
        //Set the new value
        var x = trans.executeSql('INSERT INTO settings VALUES(?, ?)', [setting, value]);
        if(x.rowsAffected > 0) {
            result= 1;
        } else {
            result = 0;
        }
    });
    return result;
}

/*
  Gets a setting from the database. Returns 0 on failure.
  */
function getsetting(setting) {
    var db = getDatabase();
    var result;
    db.transaction(function (trans) {
        var x = trans.executeSql('SELECT MAX(valueattr) AS val FROM settings WHERE keyattr = ?', setting);
        if(x.rows.length === 0) {
            result = 0;
        }
        result = x.rows.item(0)['val'];
    });
    return result;
}

/*
  Saves a trip as a favourite. Returns 1 on success, 0 on failure.
  */
function setfav(fromid, toid, from, to) {
    var db = getDatabase();
    var result;
    var backend = getsetting("backend")
    db.transaction(function (trans) {
        var x = trans.executeSql('INSERT OR REPLACE INTO favourites VALUES(?, ?, ?, ?, ?, datetime(?))', [backend, fromid, toid, from, to, 'now']);
        if(x.rowsAffected > 0) {
            result= 1;
        } else {
            result = 0;
        }
    });
    return result;
}


/*
  Removes a trip from being a favourite. Returns 1 when a row is deleted, 0 otherwise.
  */
function remfav(fromid, toid) {
    var db = getDatabase();
    var result;
    var backend = getsetting("backend")
    db.transaction(function (trans) {
        var x = trans.executeSql('DELETE FROM favourites WHERE backend=? AND fromid=? AND toid=?', [backend, fromid, toid]);
        if(x.rowsAffected > 0) {
            result= 1;
        } else {
            result = 0;
        }
    });
    return result;
}

/*
  Moves a favourite to the top of the "list"
  */
function movetotop(fromid, toid, from, to) {
    remfav(fromid, toid);
    setfav(fromid, toid, from, to);
}

/*
  Gets all the favourites, returning them as json objects in a list.
  If no favourites are found, 0 is returned.
  */
function getfaves() {
    var db = getDatabase();
    var result = [];
    console.log("Getting favourites")
    backend = getsetting("backend")
    db.transaction(function (trans) {
        var x = trans.executeSql('SELECT * FROM favourites WHERE backend=?ORDER BY datetime(time) DESC', [backend]);
        for(var i=0; i<x.rows.length; i++) {
            var row = x.rows.item(i);
            result[i] = {
                fromid: row['fromid'],
                toid: row['toid'],
                from: row['fromstop'],
                to: row['tostop']
            }
        }

        if(x.rows.length === 0) {
            result = 0;
        }
    });
    return result;
}

/*
  Gets the last trip searched, returning a json object. If there was
  no last search, 0 is returned.
  */
function getlastsearch() {
    var db = getDatabase();
    var result;
    var backend = getsetting("backend")
    console.log("backend is " + backend)
    db.transaction(function (trans) {
        var x = trans.executeSql('SELECT * FROM lastsearch WHERE backend = ?', [backend]);
        if (x.rows.length !== 1) {
            console.log("No last search found")
            result = 0;
            return;
        }

        var row = x.rows.item(0);

        result = {
            fromid: row['fromid'],
            toid: row['toid'],
            from: row['fromstop'],
            to: row['tostop']
        }
        console.log("Getting Last search: " + result.fromid + result.toid + result.from + result.to)
    });
    return result;
}

/*
  Sets the last trip searched, returning 1 on success, or 0 on failure.
  */
function setlastsearch(fromid, toid, from, to) {
    var db = getDatabase();
    var result;
    var backend = getsetting("backend")
    db.transaction(function (trans) {
        trans.executeSql('DELETE FROM lastsearch WHERE backend = ?', [backend])
        var x = trans.executeSql('INSERT INTO lastsearch VALUES(?, ?, ?, ?, ?)', [backend, fromid, toid, from, to]);
        if(x.rowsAffected > 0) {
            result= 1;
        } else {
            result = 0;
        }
    });
}
