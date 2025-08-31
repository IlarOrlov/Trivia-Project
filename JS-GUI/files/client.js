
var sock; 
var connected;
var received;
var logged = false;

var roomName = "";
var isAdmin = false;

var username__cookie = "username_";
var password__cookie = "password_";

var triviaNight = document.querySelector(".triviaNight");


var menu__checkbox = document.getElementById("menu__checkbox");

var topMsg = document.querySelector(".topMsg");
var topMsg__img = document.querySelector(".topMsg__img");
var topMsg__text = document.querySelector(".topMsg__text");

var username, password;

var login__username = document.querySelector(".login__username");
var login__password = document.querySelector(".login__password");

var signup__username = document.querySelector(".signup__username");
var signup__password = document.querySelector(".signup__password");
var signup__email = document.querySelector(".signup__email");
var signup__phone = document.querySelector(".signup__phone");
var signup__date = document.querySelector(".signup__date");
var signup__city = document.querySelector(".signup__city");
var signup__street = document.querySelector(".signup__street");
var signup__apt = document.querySelector(".signup__apt");

var createRoom__checkbox = document.getElementById("createRoom__checkbox");
var showRooms__checkbox = document.getElementById("showRooms__checkbox");
var showStats__checkbox = document.getElementById("showStats__checkbox");

var menu = document.querySelector(".menu");
var createRoom = document.querySelector(".createRoom");
var createRoom__form = document.querySelector(".createRoom__form");
var room__name = document.querySelector(".room__name");
var room__tta = document.querySelector(".room__tta");
var room__maxplayers = document.querySelector(".room__maxplayers");
var room__questionCount = document.querySelector(".room__questionCount");

var roomLobby = document.querySelector(".roomLobby");
var roomLobby__subHeader = document.querySelector(".roomLobby__subHeader");
var roomLobby__admin = document.querySelector(".roomLobby__admin");
var roomLobby__players = document.querySelector(".roomLobby__players");
var roomLobby__startGame = null;

var showRooms = document.querySelector(".showRooms");
var showStats = document.querySelector(".showStats");

var stats__global__keys = document.querySelector(".showStats .global .keys");
var stats__global__values = document.querySelector(".showStats .global .values");
var stats__global__loader = document.querySelector(".global .loading");
var answerTime__val = document.querySelector(".answerTime__val");
var correctAnswers__val = document.querySelector(".correctAnswers__val");
var totalAnswers__val = document.querySelector(".totalAnswers__val");
var gamesPlayed__val = document.querySelector(".gamesPlayed__val");

connect();

    
signup.addEventListener('submit', (e) => {
    e.preventDefault();
    
    if (connected && !firstInputValidation()) return;

    log(false);
});


login.addEventListener('submit', (e) => {
    e.preventDefault();
    log(true);
});


menu__checkbox.addEventListener('change', () => {


    if (menu__checkbox.checked) {
        menu.classList.remove("hide");
        subHeader.classList.remove("hide");
        subHeader.innerHTML = "Menu";

        change(createRoom__checkbox, false);
        change(showStats__checkbox, false);
        change(showRooms__checkbox, false);
        
        roomLobby.classList.add("hide");
    }

    // logout
    else if (connected) {        
        
        menu__checkbox.checked = true;
        logout();
        
        if (logged) {
            delCookie(username__cookie);
            delCookie(password__cookie);
            username = null;
            password = null;
        }
        menu__checkbox.checked = false;
    }
});

createRoom__checkbox.addEventListener('change', () => {
    if (!createRoom__checkbox.checked || !username) {
        createRoom.classList.add("hide");
        return;
    } 
    
    menu.classList.add("hide");
    createRoom.classList.remove("hide"); 
    createRoom__form.classList.remove("hide"); 
    subHeader.innerHTML = "";

});

createRoom__form.addEventListener('submit', (e) => {
    e.preventDefault();

    if (!createRoom__checkbox.checked || !username) return;

    
    let json = '{"creator": "' + username + 
                '", "room_name": "' + room__name.value + 
                '", "max_users": ' + (room__maxplayers.value === "infinite" ? 0 : room__maxplayers.value) + 
                ', "answer_timeout": ' + (room__tta.value === "infinite" ? 0 : room__tta.value) + 
                ', "questions_count": ' + room__questionCount.value + '}';
    
    send(json, 3);

    waitFor(() => {
        if (received.error)
            raiseError(received.error);
        else {
            isAdmin = true;
            showRoomLobby(room__name.value);
        }
    });
});

showRooms__checkbox.addEventListener('change', () => {

    if (!showRooms__checkbox.checked || !username) {
        showRooms.classList.add("hide");
        return;
    }

    showRooms.classList.remove("hide");
    menu.classList.add("hide");

    subHeader.innerHTML = "Rooms";
    stopPrintRooms = false;
    printRooms();

});

showStats__checkbox.addEventListener('change', () => {
    
    if (!showStats__checkbox.checked || !username) {
        showStats.classList.add("hide");
        return;
    }
    
    menu.classList.add("hide");
    showStats.classList.remove("hide");
    stats__global__loader.classList.remove("hide");
    
    subHeader.innerHTML = "Statistics";

    
    while(stats__global__keys.firstChild) stats__global__keys.removeChild(stats__global__keys.firstChild);
    while(stats__global__values.firstChild) stats__global__values.removeChild(stats__global__values.firstChild);
    

    send('{"username":"' + username + '"}', 7);


    waitFor(() => {
        if (received.error) 
            raiseError(received.error);
        else {
            
            answerTime__val.innerHTML = received.personal_stats.time_for_answer;
            correctAnswers__val.innerHTML = received.personal_stats.right_answers;
            totalAnswers__val.innerHTML = parseFloat(received.personal_stats.wrong_answers) + parseFloat(received.personal_stats.right_answers);
            gamesPlayed__val.innerHTML = received.personal_stats.games_played;
            
            send('', 6);
            waitFor(() => {
                if (received.error) 
                    raiseError(received.error);
                else {
                    
                    let name, points;
                    let place = 1;
                    
                    stats__global__loader.classList.add("hide");

                    Object.entries(received.high_scores).reverse().forEach(([n, p]) => {
                        name = document.createElement('div');
                        points = document.createElement('div');
                        
                        name.innerHTML = n;
                        points.innerHTML = p;
                        

                        if (place <= 3) {
                            if (place == 1) {
                                name.classList.add("item", "firstPlace");
                                points.classList.add("item", "firstPlace");
                            } else if (place == 2) {
                                name.classList.add("item", "secPlace");
                                points.classList.add("item", "secPlace");
                            } else if (place == 3) { 
                                name.classList.add("item", "thirdPlace");
                                points.classList.add("item", "thirdPlace"); 
                            }
                        } else {
                            name.classList.add("item");
                            points.classList.add("item");
                        }
                        
                        stats__global__keys.appendChild(name);
                        stats__global__values.appendChild(points);
                        
                        ++place;
                    });
                }
            });
        }
    });
});



triviaNight.addEventListener('click', () => {
    if (menu__checkbox.checked) {
        
        leaveRoom();

        stopPrintRooms = true;
        stopPrintPlayers = true;
        roomLobby__startGame = null;
        roomLunched = false;
        isAdmin = false;
        roomClosed = false;
        roomName = "";

        menu.classList.remove("hide");
        change(menu__checkbox, true);
    }
});



if (getCookie("firstBoot") == "false")
    showConnectMsg(false);

handleCookies();

function connect() {

    setTimeout(() => {
        sock = new WebSocket("ws://localhost:4269");
        
        sock.onopen = () => {
            connected = true;
            setCookie("firstBoot", "false", 1);
            showConnectMsg(true);

            if (isLogged()) {
                username = getCookie(username__cookie);
                password = getCookie(password__cookie);
                log(true, true);
            }
        }
        
        sock.onclose = () => {
            connected = false;
            change(menu__checkbox, false);
            
            delCookie(username__cookie);
            delCookie(password__cookie);
            username = null;
            
            showConnectMsg(false);

            connect();
        }

        sock.onmessage = (e) => {
            let data = e.data.substring(40);
            received =  JSON.parse(data ? data.match(/.{1,8}/g).map(w => w.replace(/^0+/, '')).map(w => String.fromCharCode(parseInt(w, 2))).join('') : '{}');
        }
    }, 1000);
}

function log(isLogin, showResponse = true) {
    let json, code;

    menu.classList.remove("hide");

    if (isLogin) {
        json = '{"username": "' + (login__username.value ? login__username.value : username) + 
             '", "password": "' + (login__password.value ? login__password.value : password) + '"}';
        code = 1;

    } else {
        json = '{"username": "' + signup__username.value + 
                 '", "password": "' + signup__password.value + 
                 '", "email": "' + signup__email.value +
                 '", "phone": "' + signup__phone.value +
                 '", "date": "' + signup__date.value +
                 '", "city": "' + signup__city.value +
                 '", "street": "' + signup__street.value +
                 '", "apartment": "' + signup__apt.value + '"}';
        code = 2;
    }

    if (connected) {
        send(json, code);
        
        waitFor(() => {
            if (received.error) {
                if (showResponse)
                    raiseError(received.error);
            } else {
                
                logged = true;

                if (login__username.value || signup__username.value) 
                {
                    username = isLogin ? login__username.value :
                                        signup__username.value ;
                    password = isLogin ? login__password.value :
                                        signup__password.value ;
                    setCookie(username__cookie, username);
                    setCookie(password__cookie, password);
                }
                
                login__checkbox.checked = false;
                signup__checkbox.checked = false;

                change(menu__checkbox, true);
            }
        });
    
    } else if (showResponse) {
        topMsg.classList.add("topMsg__bold");
        setTimeout(() => {topMsg.classList.remove("topMsg__bold");}, 1000);
    }

}

function logout() {
    
    leaveRoom();

    send( '{"username":"' + username + '"}' , 8);

    waitFor(() => {
        if (received.error)
            raiseError(received.error);
        else 
            logged = false;
    });
}


function firstInputValidation() {

    // password 
    if (!signup__password.value.match(/(?=.*\d.*)(?=.*[a-zA-Z].*)(?=.*[\!\@\#\$\%\^\&\*].*).{8,}/)) {
        raiseError("password is invalid");
        return false;
    } 
    // email
    else if (!signup__email.value.match(/[a-zA-Z0-9]+@[a-zA-Z0-9]+[.][a-zA-Z0-9]+([.][a-zA-Z0-9]+)?/)) {
        raiseError("email is invalid");
        return false;
    }
    // phone 
    else if (!signup__phone.value.match(/^0[\d]{1,2}[-][\d]{7,9}$/)) {
        raiseError("phone is invalid");
        return false;
    }
    // date
    else if (!signup__date.value.match(/^[\d]{2}[.|/][\d]{2}[.|/][\d]{4}$/)) {
        raiseError("date is invalid");
        return false;
    }
    // city
    else if (!signup__city.value.match(/[a-zA-Z]+/)) {
        raiseError("city should include letters only");
        return false;
    }
    // street
    else if (!signup__street.value.match(/[a-zA-Z]+/)) {
        raiseError("street should include letters only");
        return false;
    }
    // apt    
    else if (!signup__apt.value.match(/[0-9]+/)) {
        raiseError("apt should include numeric only");
        return false;
    }
    return true;
}


// cookies related methods
function isLogged() {
    return getCookie(username__cookie);
}

function getCookie(key) {
    var name = key + "=";
    var ca = decodeURIComponent(document.cookie).split(';');

    for (var i = 0; i < ca.length; i++) {
      var c = ca[i];
      while (c.charAt(0) == ' ')
        c = c.substring(1);
      
      if (c.indexOf(name) == 0)
        return c.substring(name.length, c.length);
    }
    return "";
}
function setCookie(key, val, date) {
    var d = new Date();
    
    if (date) d.setTime(d.getTime() + (date * 60 * 1000));

    document.cookie = encodeURIComponent(key) + "=" + encodeURIComponent(val) + (date ? "expires=" +  d.toUTCString() : "") + ';';
}
function delCookie(key) {
    document.cookie = key + "=;";
}


function send(data, id) {

    var dataInBits = "";

    for (var i = 0; i < data.length; ++i) 
        dataInBits += data[i].charCodeAt(0).toString(2).padStart(8, "0");

    sock.send ( id.toString(2).padStart(8, "0") + 
                dataInBits.length.toString(2).padStart(32, "0") + 
                dataInBits);
}

function waitFor(callback) {
    setTimeout(() => {
        if (received != null) {
            callback();
            received = null;
        } else { 
            waitFor(callback);
        }
    }, 5);
}

function raiseError(err) {

    raiseMsg(err, () => {
        topMsg__text.classList.add("topMsg__text__notConnected")
    });
}

function raiseMsg(msg, callback = null) {
    topMsg__text.innerHTML = msg;
    
    topMsg__text.classList.remove("topMsg__text__notConnected");
    topMsg__text.classList.remove("topMsg__text__connected");
    topMsg.classList.remove("topMsg__hide");
    topMsg__img.classList.add("hide");

    if (callback) callback();

    setTimeout(() => {topMsg.classList.add("topMsg__hide")}, 2500);
}

function showConnectMsg(connected) {

    topMsg__img.classList.remove("hide");
    if (connected) {
        topMsg__text.innerHTML = "connected!";
        topMsg__text.classList.remove("topMsg__text__notConnected");
        topMsg__text.classList.add("topMsg__text__connected");
        topMsg__img.classList.add("topMsg__img__connected");
        topMsg.classList.remove("topMsg__hide");
        
        setTimeout(() => { topMsg.classList.add("topMsg__hide"); }, 50);

    } else {

        topMsg__text.innerHTML = "not connected!";
        topMsg__text.classList.add("topMsg__text__notConnected");
        topMsg__text.classList.remove("topMsg__text__connected");
        topMsg__img.classList.remove("topMsg__img__connected");
        topMsg.classList.remove("topMsg__hide");
    }
}


function change(checkbox, val) {
    checkbox.checked = val;

    checkbox.dispatchEvent(new Event('change'));
}

function handleCookies() {
    
    let id = parseInt(window.location.href.slice(-1));
    
    if (id) {
        username__cookie += id;
        password__cookie += id;
    } else {
        setCookie("count", getCookie("count") ? parseInt(getCookie("count")) + 1 : 1);

        let newId = getCookie("count");
        username__cookie += newId;
        password__cookie += newId;
        window.location.href = "http://localhost:5500/GUI.html?u=" + newId;
    }
}

function leaveRoom() {
    if (!roomName) return; 

    if (isAdmin) {
        send('{"room_name": "' + roomName + '"}', 10);
        raiseMsg("Room Closed!");
    } else {
        send('{"username": "' + username +  '", "room_name": "' + roomName + '"}', 13);
        if (!roomClosed) raiseMsg("Left Room!");
    }

    waitFor(() => {
        if (received.error) { 
            raiseError(received.error);
            triviaNight.click();
        }
    })
}

function showRoomLobby(r) {

    showRooms.classList.add("hide");
    createRoom__form.classList.add("hide");


    subHeader.classList.add("hide");

    roomLobby__subHeader.innerHTML = r;
    roomName = r;

    roomLobby.classList.remove("hide");
    roomLobby__subHeader.classList.remove("hide");
    
    stopPrintPlayers = false;
    firstLobbyBoot = true;
    printPlayers(r);
}

var stopPrintRooms = false;

function printRooms() {
    
    if (stopPrintRooms) return;

    while(showRooms.firstChild)
        showRooms.removeChild(showRooms.firstChild);



    showRooms.classList.remove("hide");

    send('', 9);

    waitFor(() => {
        if (received.error) {
            raiseError(received.error);
        } else {
            if (received.rooms) {
            
                var name;
                let room, admin, state, max_players;
                let sep1, sep2, sep3;
                let rooms = received.rooms;
                

                for (var i = 0; i < rooms.length; ++i) {
                    
                    room = document.createElement('div');
                    name = document.createElement('div');
                    sep1 = document.createElement('span');
                    admin = document.createElement('div');
                    sep2 = document.createElement('span');
                    state = document.createElement('div');
                    sep3 = document.createElement('span');
                    max_players = document.createElement('div');
                    
                
                    room.className = "room";
                    name.className = "room__name";
                    admin.className = "room__admin";
                    state.className = "room_state";
                    max_players.className = "room__maxPlayers";

                    sep1.innerHTML = '|';
                    sep2.innerHTML = '|';
                    sep3.innerHTML = '|';
                    name.innerHTML = rooms[i].name;
                    admin.innerHTML = rooms[i].admin;
                    
                    state.innerHTML = rooms[i].state;

                    state.innerHTML == "waiting" ?
                        state.classList.add("room__onlineStatus") :
                        state.classList.add("room__offlineStatus");
                    
                    room.classList.add(rooms[i].players_count == rooms[i].max_players || state.innerHTML != "waiting" ? "room__full" : "nn");

                    max_players.innerHTML = rooms[i].players_count + "/" + (rooms[i].max_players != "0" ? rooms[i].max_players : "∞"); 

                    room.appendChild(name);
                    room.appendChild(sep1);
                    room.appendChild(admin);
                    room.appendChild(sep2);
                    room.appendChild(state);
                    room.appendChild(sep3);
                    room.appendChild(max_players);

                    showRooms.appendChild(room);
                    
                    if (!room.classList.contains("room__full")) {

                        room.addEventListener("click", () => {
                            stopPrintRooms = true;

                            send('{"room_name": "' + name.innerHTML + '", "username": "' + username + '"}', 4);

                            waitFor(() => {
                                if (received.error) 
                                    raiseError(received.error);
                                else {
                                    
                                    showRoomLobby(name.innerHTML);
                                }
                            });
                        })
                    }
                }
            }
            if (!stopPrintRooms)
                setTimeout(printRooms, 4000);
        }
    })
}

var stopPrintPlayers = false;
var firstLobbyBoot = true;
var roomLunched = false;
var roomClosed = false;

function printPlayers(r) {


    if (roomLobby__startGame && (stopPrintPlayers || roomLunched)) {
        roomLobby.removeChild(roomLobby__startGame);
        roomLobby__startGame = null;
    }
    if (stopPrintPlayers)
        return;

    if (isAdmin && firstLobbyBoot) {
        roomLobby__startGame = document.createElement("button");
        roomLobby__startGame.classList.add("roomLobby__startGame");
        roomLobby__startGame.innerHTML = "LAUNCH";
        roomLobby__startGame.onclick = () => { lunchRoom(roomLobby__startGame) };
        roomLobby.appendChild(roomLobby__startGame);
        firstLobbyBoot = false;
    }
    

    send('{"room_name": "' + r + '"}', 12);
    
    waitFor(() => {
        
        if (received.error) {
            stopPrintPlayers = true;
            
            if (received.error == "Room doesn't exist") {
                raiseMsg("Room Closed!");
                roomClosed = true;
            } else 
                raiseError(received.error);

            
            triviaNight.click();
        }    
        else {

                if (received.state == "playing" && !roomLunched) {
                    
                    roomLunched = true;
                    raiseMsg("Room Launched!");
                }

                // clear players list
                while (roomLobby__players.firstChild)
                    roomLobby__players.removeChild(roomLobby__players.firstChild);
    
                // add all players
                
                let adm = document.createElement("div");
    
                adm.className = "roomLobby__admin";
                adm.innerHTML = received.admin;
                roomLobby__players.appendChild(adm);

                for (var player of received.players) {
                    if (player == received.admin) continue 
                    
                    let p = document.createElement("div");
    
                    p.className = "roomLobby__player";
                    p.innerHTML = player;
                    roomLobby__players.appendChild(p);
                }
            setTimeout(printPlayers, 2000, r);
        }
    });
}

function lunchRoom(b) {

    send('{"room_name" : "' + roomName + '"}', 11);

    waitFor(() => {
        if (received.error) {
            raiseError(received.error)
        } else {
            roomLunched = true;
            raiseMsg("Room Lunched!");
        }
    });

}

window.onbeforeunload = () => {
    
    if (logged)
        logout();

    console.log('refresh: username: ' + username);
}