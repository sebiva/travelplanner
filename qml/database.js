.import QtQuick.LocalStorage 2.0 as LS

function test() {
    console.log("IN DATABASE TEST")


    var db = getDatabase();
//    db.transaction(function (trans) {
//        trans.executeSql('DROP TABLE IF EXISTS favourites');
//    });
    setup();
    console.log("SET1: " + setfav("42", "52", "hej", "då"));
    console.log("SET2: " + setfav("42", "53", "hej", "då"));
    console.log("SET3: " + setfav("43", "52", "hej", "då"));
    console.log("SET4: " + setfav("42", "52", "hej", "då"));
    console.log("SET5: " + setfav("2", "5", "hej", "då"));


    getfaves();
}



function getDatabase() {
    return LS.LocalStorage.openDatabaseSync("Travelplanner", "1.0", "StorageDatabase", 100);
}

function setup() {
    var db = getDatabase(); //db.transaction(function (trans) {
        //trans.executeSql('DROP TABLE IF EXISTS favourites');
        //console.log("TABLE CLEARED" + setuped)
    //});
    db.transaction(function (trans) {
        //trans.executeSql('DELETE FROM favourites')
        trans.executeSql('CREATE TABLE IF NOT EXISTS favourites(fromid TEXT, toid TEXT, fromstop TEXT, tostop TEXT, time INTEGER, PRIMARY KEY(fromid, toid))');
        trans.executeSql('CREATE TABLE IF NOT EXISTS lastsearch(fromid TEXT, toid TEXT, fromstop TEXT, tostop TEXT)')
    });
}

function setfav(fromid, toid, from, to) {
    var db = getDatabase();
    var result;
    db.transaction(function (trans) {
        var x = trans.executeSql('INSERT OR REPLACE INTO favourites VALUES(?, ?, ?, ?, datetime(?))', [fromid, toid, from, to, 'now']);
        console.log("ADDED: " + from + to)
        if(x.rowsAffected > 0) {
            result= 1;
        } else {
            result = 0;
        }
    });
    return result;
}

function remfav(fromid, toid) {
    var db = getDatabase();
    var result;
    db.transaction(function (trans) {
        var x = trans.executeSql('DELETE FROM favourites WHERE fromid=? AND toid=?', [fromid, toid]);
        if(x.rowsAffected > 0) {
            result= 1;
        } else {
            result = 0;
        }
    });
    return result;
}

function movetotop(fromid, toid, from, to) {
    remfav(fromid, toid);
    setfav(fromid, toid, from, to);
}

function getfaves() {
    var db = getDatabase();
    var result = [];
    db.transaction(function (trans) {
        var x = trans.executeSql('SELECT * FROM favourites ORDER BY datetime(time) DESC');
        //console.log("SIZE of fav: " + x.rows.length);
        for(var i=0; i<x.rows.length; i++) {
            var row = x.rows.item(i);
            result[i] = {
                fromid: row['fromid'],
                toid: row['toid'],
                from: row['fromstop'],
                to: row['tostop']
            }
            //console.log(row['fromid'], row['toid'], row['fromstop'], row['tostop'], row['time']);
        }

        if(x.rows.length === 0) {
            result = 0;
        }
    });
    return result;
}

function getlastsearch() {
    var db = getDatabase();
    var result;
    db.transaction(function (trans) {
        var x = trans.executeSql('SELECT * FROM lastsearch');
        //console.log("Getting : " + x.rows.length)
        if (x.rows.length !== 1) {
            result = 0;
        }

        var row = x.rows.item(0);

        result = {
            fromid: row['fromid'],
            toid: row['toid'],
            from: row['fromstop'],
            to: row['tostop']
        }
        console.log("Getting: " + result.fromid + result.toid + result.from + result.to)
    });
    return result;
}

function setlastsearch(fromid, toid, from, to) {
    var db = getDatabase();
    var result;
    db.transaction(function (trans) {
        //console.log("Setting: " + fromid + toid + from + to)
        trans.executeSql('DELETE FROM lastsearch')
        var x = trans.executeSql('INSERT INTO lastsearch VALUES(?, ?, ?, ?)', [fromid, toid, from, to]);
        if(x.rowsAffected > 0) {
            result= 1;
        } else {
            result = 0;
        }
    });
}
