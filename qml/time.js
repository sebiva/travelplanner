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

function convertdate(date) {
    if (date.length === 10 && date.indexOf("-") !== 0) {
        return date;
    } else if (date.indexOf("/") !== -1 && date.split("/").length === 3) {
        return date.split("/")[2] + "-" + date.split("/")[1] + "-" + date.split("/")[0];
    }
    return "";
}


function duration(deptime, arivtime, depdate, arivdate ) {
    //console.log("dur" + deptime + arivtime + depdate + arivdate);
    depdate = convertdate(depdate)
    arivdate = convertdate(arivdate)

    var dephr = deptime.split(":")[0];
    var depmin = deptime.split(":")[1];
    var arivhr = arivtime.split(":")[0];
    var arivmin = arivtime.split(":")[1];
    var hours = 0;
    var minutes = 0;
    var extradays = 0;
    if (depdate !== arivdate) {
        console.log("Different days!, " + depdate + " " + arivdate)
        var dep = depdate.split("-");
        var dyear = parseInt(dep[0]); var dmonth = parseInt(dep[1]) - 1; var dday = parseInt(dep[2]);
        var ariv = arivdate.split("-");
        var ayear = parseInt(ariv[0]); var amonth = parseInt(ariv[1]) - 1; var aday = parseInt(ariv[2]);
        var ddate = new Date(dyear, dmonth, dday, 0, 0, 0, 0);
        var adate = new Date(ayear, amonth, aday, 0, 0, 0, 0);
        console.log("Different days!, " + ddate + " " + adate)
        extradays = dateDiffInDays(ddate, adate);
    }
    hours = parseInt(arivhr) - parseInt(dephr);
    minutes = parseInt(arivmin) - parseInt(depmin);
    if(hours * 60 + minutes < 0 && extradays <=0) {
        console.log(hours, "hr", minutes, "min", extradays, "d");
        return "error";
    }

    var answermin = 0;
    var answerhr = 0;
    var answer = "";
    if (hours == 0 && minutes != 0) {
        answermin = minutes;
    }else if (hours < 0) {
        extradays--;
        answerhr = (hours+24);
        if (minutes < 0) {
            answerhr--;
            answermin = minutes+60;
        } else {
           answermin = minutes;
        }
    }else if (minutes<0)  {
        answerhr = hours-1;
        answermin = minutes+60;
    } else {
        answerhr = hours;
        answermin = minutes;
    }

    //console.log("\n\n\n");
    if (answerhr > 0) {
        answer = answerhr.toString() + "h ";
        //console.log("Hour:" + answer);
    }
    if (answermin >0) {
        answer = answer + answermin.toString() + "min";
        //console.log("Min:" + answermin.toString() + "min");
    }

    if (extradays > 0) {
        console.log("Extra day" + extradays)
        answer = answer + " +" + extradays + "d";
    } else if (extradays < 0) {
        console.log("Extar day" + extradays)
        answer = answer + " " + extradays + "d";
    }

    //console.log("DURATION: \nfrom " + deptime + " , " + depdate + "\nto " + arivtime + " , " + arivdate + " : " +  answer);
    if (answer === "") {
        answer = "0min";
    }

    return answer;
}

function dateDiffInDays(a, b) {
    var utc1 = Date.UTC(a.getFullYear(), a.getMonth(), a.getDate());
    var utc2 = Date.UTC(b.getFullYear(), b.getMonth(), b.getDate());

    return Math.floor((utc2 - utc1) / (1000*60*60*24));
}

function delay(time, rttime, date, rtdate) {
    date = convertdate(date)
    rtdate = convertdate(date)
    //console.log("DELAY" + time + "  " + rttime + "  " + date + "  " + rtdate + " " + (date === rtdate))
    if ((time === rttime) && (date === rtdate)) {
        //console.log("no delay")
        return ""
    }
    var minus = false;
    var dur = duration(time, rttime, date, rtdate);
    if (dur === "error") {
        dur = duration(rttime, time, rtdate, date);
        minus = true;
    }


    if (dur === null || dur === undefined) {
        return "";
    }

    //console.log("DELAYDURATION: \nfrom " + time + " , " + date + "\nto " + rttime + " , " + rtdate + " : " +  dur);
    var min, hr;
    if (dur.indexOf("h") === -1) {
        //console.log("no hour")
        min = dur.split("min")[0];
        if (minus) {
            return "-" + min;
        }
        return "+" + min;
    } else {
        hr = dur.split("h")[0];
        min = dur.split(" ")[0].split("min")[0];
        if (minus) {
            return "-" + hr + ":" + min
        }

        return "+" + hr + ":" + min;
    }
}

function getcurrentdate() {
    return Qt.formatDateTime(new Date(), "yyyy-MM-dd")
}
function getcurrenttime() {
    return Qt.formatDateTime(new Date(), "hh:mm")
}
