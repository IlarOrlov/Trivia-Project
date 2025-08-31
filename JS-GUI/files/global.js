

/*===============================
    Sign up Email Completer
=================================*/


var showEmailCompleter = true;
var options = document.querySelectorAll(".emailCompleter .option");
var emailCompleter__checkbox = document.getElementById("emailCompleter__checkbox");
var signup__email = document.querySelector(".signup__email");
var signup__phone = document.querySelectorAll(".signup__phone");


function updateEmailCompleter(email) {

    if (email.value.includes('@')) {
        if (showEmailCompleter) {
            emailCompleter__checkbox.checked = true; 
            var n = 0;
            options.forEach((o) => {o.tabIndex = toString(4 + n); n++});
        }
    } else {
        emailCompleter__checkbox.checked = false;
        showEmailCompleter = true;

        options.forEach((o) => {o.tabIndex = '-1'});
    }
}

signup__email.addEventListener('blur', () => {
    emailCompleter__checkbox.checked = false;
});

signup__email.addEventListener('focus', () => {
    updateEmailCompleter(signup__email);
});


options.forEach((option) => {
    option.addEventListener("click", () => {
        
        signup__email.value = signup__email.value.split('@')[0] + '@' + option.innerHTML;
        emailCompleter__checkbox.checked = false;
        showEmailCompleter = false;
        signup__phone.focus();
    });

    option.addEventListener('focus', () => {
        emailCompleter__checkbox.checked = true;    
    });

    option.addEventListener('blur', () => {
        emailCompleter__checkbox.checked = false;    
    });
});



/*===============================
    log method switch transitions
=================================*/

var signup__checkbox = document.getElementById("signup__checkbox");
var login__checkbox = document.getElementById("login__checkbox");
var login = document.querySelector(".login");
var signup = document.querySelector(".signup");
var subHeader = document.querySelector(".subHeader");


signup__checkbox.addEventListener("change", () => {

    let that = signup__checkbox.checked;
    let other = login__checkbox.checked;
    
    signup.style.transition = "all 0s";
    login.style.transition = "all 0s";
    
    if (that) {
        if (other) 
            login__checkbox.checked = false;    

        subHeader.innerHTML = "Signup";
    
    } else if (!other)
        signup.style.transition = "all 0s .8s";
     
    subHeader.style.transition = signup.style.transition;

});


login__checkbox.addEventListener("change", () => {

    let that = login__checkbox.checked;
    let other = signup__checkbox.checked;

    login.style.transition = "all 0s";
    signup.style.transition = "all 0s";

    if (that) {
        if (other)
            signup__checkbox.checked = false;

        subHeader.innerHTML = "Login";
    
    } else if (!other) 
        login.style.transition = "all 0s .8s";
    
    subHeader.style.transition = login.style.transition;

});


/*========================
    Validate Room Name
==========================*/

function validateRoomName(room) {
    if (room.value.length > 10)
        room.value = room.value.substr(0, 10);
}