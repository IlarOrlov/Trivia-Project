
var sock; 
var connected;
var received;
var logged = false;

var username__cookie = "username_";
var password__cookie = "password_";

var trivia = document.querySelector(".trivia");


var isgame__checkbox = document.getElementById("isgame__checkbox");

var topMsg = document.querySelector(".topMsg");
var topMsg__img = document.querySelector(".topMsg__img");
var topMsg__text = document.querySelector(".topMsg__text");

var username;
var password;

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
var showRoom__checkbox = document.getElementById("showRoom__checkbox");
var showStats__checkbox = document.getElementById("showStats__checkbox");

var menu = document.querySelector(".menu");
var createRoom = document.querySelector(".createRoom");
var createRoom__form = document.querySelector(".createRoom__form");
var room__name = document.querySelector(".room__name");
var room__tta = document.querySelector(".room__tta");
var room__maxplayers = document.querySelector(".room__maxplayers");
var room__questionCount = document.querySelector(".room__questionCount");

var roomData = document.querySelector(".roomData");
var roomData__subHeader = document.querySelector(".roomData__subHeader");
var roomData__admin = document.querySelector(".roomData__admin");
var roomData__players = document.querySelector(".roomData__players");

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
    
    if (!firstInputValidation()) return;

    log(false);
});


login.addEventListener('submit', (e) => {
    e.preventDefault();
    log(true);
});


isgame__checkbox.addEventListener('change', () => {


    if (isgame__checkbox.checked) {
        subHeader.innerHTML = "Menu";

        change(createRoom__checkbox, false);
        change(showStats__checkbox, false);
        change(showRoom__checkbox, false);
        roomData.classList.add("hide");
    }

    // logout
    else if (connected) {        
        
        isgame__checkbox.checked = true;
        logout();

        if (logged) {
            delCookie(username__cookie);
            delCookie(password__cookie);
            username = null;
            password = null;
        }
        isgame__checkbox.checked = false;
    }
});

createRoom__checkbox.addEventListener('change', () => {
    if (!createRoom__checkbox.checked || !username) {
        createRoom.classList.add("hide");
        return;
    } 
        
    createRoom.classList.remove("hide"); 
    subHeader.innerHTML = "Create Room";

});

createRoom__form.addEventListener('submit', (e) => {
    e.preventDefault();

    if (!createRoom__checkbox.checked || !username) return;

    

    let json = '{"creator": "' + username + 
                '", "room_name": "' + room__name.value + 
                '", "max_users": ' + room__maxplayers.value + 
                ', "answer_timeout": ' + room__tta.value + 
                ', "questions_count": ' + room__questionCount.value + '}';
    

    send(json, 3);

    waitFor(() => {
        if (received.error) {
            raiseError(received.error);
        } else {

            send('{"room_name": "' + room__name.value + '"}', 5);

            waitFor(() => {
                if (received.error)
                    raiseError(received.error);
                else {
                    roomData.classList.remove("hide");

                    roomData__subHeader.innerHTML = room__name.value;
                    subHeader.classList.add("hide");
                    roomData__subHeader.classList.remove("hide");

                    for (var player of received.players) {
                        let p = document.createElement("div");

                        p.className = player == username ? "roomData__admin" : "roomData__player";
                        p.innerHTML = player;
                        roomData__players.appendChild(p);
                    }

                    createRoom__form.classList.add("hide");
                    roomData.classList.remove("hide");
                }
            });
        }
    });
});

showRoom__checkbox.addEventListener('change', () => {

    if (!showRoom__checkbox.checked || !username) {
        showRooms.classList.add("hide");
        return;
    }

                
    while(showRooms.firstChild)
        showRooms.removeChild(showRooms.firstChild);

    showRooms.classList.remove("hide");
        
    send('', 9);
    
    waitFor(() => {
        if (received.error) {
            raiseError(received.error);
    
        } else {

            subHeader.innerHTML = "Rooms";

            if (!received.rooms) return
        
            let room, admin, is_active, max_players, name;
            let sep1, sep2, sep3;
            let rooms = received.rooms;
            

            for (var i = 0; i < rooms.length; ++i) {
                
                room = document.createElement('div');
                name = document.createElement('div');
                sep1 = document.createElement('span');
                admin = document.createElement('div');
                sep2 = document.createElement('span');
                is_active = document.createElement('div');
                sep3 = document.createElement('span');
                max_players = document.createElement('div');
                
            
                room.className = "room";
                name.className = "room__name";
                admin.className = "room__admin";
                is_active.className = "room__active";
                max_players.className = "room__maxPlayers";

                sep1.innerHTML = '|';
                sep2.innerHTML = '|';
                sep3.innerHTML = '|';
                name.innerHTML = rooms[i].name;
                admin.innerHTML = rooms[i].admin;
                
                if (rooms[i].is_active) {
                    is_active.innerHTML = "online";
                    is_active.classList.add("room__onlineStatus");
                } else {
                    is_active.innerHTML = "offline";
                    is_active.classList.add("room__offlineStatus");
                }
                
                room.classList.add(rooms[i].players_count == rooms[i].max_players ? "room__full" : "nn");
                max_players.innerHTML = rooms[i].players_count + "/" + rooms[i].max_players;
                
                room.appendChild(name);
                room.appendChild(sep1);
                room.appendChild(admin);
                room.appendChild(sep2);
                room.appendChild(is_active);
                room.appendChild(sep3);
                room.appendChild(max_players);

                showRooms.appendChild(room);
                
                document.querySelectorAll(".room").forEach((name) => {
                    
                    if (!room.classList.contains("room__full")) {
                        console.log('here');

                        room.querySelector(".room__name").addEventListener("click", (name) => {
                            
                            send('{"room_name": "' + name.innerHTML + '", "username": "' + username + '"}', 4);
                            
                            waitFor(() => {
                                if (received.error) 
                                    raiseError(received.error);
                                else {

                                    roomData.classList.remove("hide");
                                    send('{"room_name": "' + name.innerHTML + '"}', 5);
                                    waitFor(() => {
                                        if (received.error)
                                            raiseError(received.error);
                                        else {

                                            subHeader.classList.add("hide");
                                            roomData__subHeader.innerHTML = name.innerHTML;
                                            roomData__subHeader.classList.remove("hide");
                                            console.log('');
                                            for (var player of received.players) {
                                                let p = document.createElement("div");

                                                p.className = player == admin.innerHTML ? "roomData__admin" : "roomData__player";
                                                p.innerHTML = player;
                                                roomData__players.appendChild(p);
                                            }

                                            showRooms.classList.add("hide");
                                        }
                                    });
                                }
                            });
                        })
                    }
                })
            }
        }
    })


});

showStats__checkbox.addEventListener('change', () => {
    
    if (!showStats__checkbox.checked || !username) {
        showStats.classList.add("hide");
        return;
    }
    
    showStats.classList.remove("hide");
    stats__global__loader.classList.remove("hide");
    
    subHeader.innerHTML = "Statistics";
    

    send('{"username":"' + username + '"}', 7);


    waitFor(() => {
        if (received.error) 
            raiseError(received.error);
        else {
            
            while(stats__global__keys.firstChild) stats__global__keys.removeChild(stats__global__keys.firstChild);
            while(stats__global__values.firstChild) stats__global__values.removeChild(stats__global__values.firstChild);
            

            answerTime__val.innerHTML = received.personal_stats.time_for_answer;
            correctAnswers__val.innerHTML = received.personal_stats.right_answers;
            totalAnswers__val.innerHTML = received.personal_stats.wrong_answers;
            gamesPlayed__val.innerHTML = received.personal_stats.games_played;
            
            send('', 6);
            waitFor(() => {
                if (received.error) 
                    raiseError(received.error);
                else {
                    
                    let name, points;
                    let place = 1;
                    
                    stats__global__loader.classList.add("hide");

                    Object.entries(received.high_scores).forEach(([n, p]) => {
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

trivia.addEventListener('click', () => {
    if (isgame__checkbox.checked) {
        
        if (showRoom__checkbox.checked)
            quitRoom();
        
        change(isgame__checkbox, true);
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
                console.log(username);
                console.log(password);
                log(true, true);
            }
        }
        
        sock.onclose = () => {
            connected = false;
            change(isgame__checkbox, false);
            
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

                change(isgame__checkbox, true);
            }
        });
    
    } else if (showResponse) {
        topMsg.classList.add("topMsg__bold");
        setTimeout(() => {topMsg.classList.remove("topMsg__bold");}, 1000);
    }

}


function logout() {

    if (showRoom__checkbox.checked) quitRoom();

    let json = '{"username":"' + username + '"}';
    send(json, 8);

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
    }, 1);
}

function raiseError(err) {
    topMsg__text.innerHTML = err;
    topMsg__text.classList.remove("topMsg__text__connected");
    topMsg.classList.remove("topMsg__hide");
    topMsg__img.classList.add("hide");
    setTimeout(() => {topMsg.classList.add("topMsg__hide")}, 2500);
}

function showConnectMsg(connected) {

    topMsg__img.classList.remove("hide");
    if (connected) {
        topMsg__text.innerHTML = "connected!";
        topMsg__text.classList.add("topMsg__text__connected");
        topMsg__img.classList.add("topMsg__img__connected");
        topMsg.classList.remove("topMsg__hide");
        
        setTimeout(() => { topMsg.classList.add("topMsg__hide"); }, 50);

    } else {
        topMsg__text.innerHTML = "not connected!";
        topMsg__text.classList.remove("topMsg__text__connected");
        topMsg__img.classList.remove("topMsg__img__connected");
        topMsg.classList.remove("topMsg__hide");
    }
}


function change(checkbox, val) {
    checkbox.checked = val;

    checkbox.dispatchEvent(new Event('change'));
}

function makePlayersList() {

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
        window.location.href = "http://localhost:5500/home.html?u=" + newId;
    }
}

function quitRoom() {
    // quit room
}

window.onbeforeunload = () => {
    
    if (logged)
        logout();
    
    console.log('refresh: username: ' + username);
}