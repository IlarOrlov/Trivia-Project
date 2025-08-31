/*======================
    Form Authentication
========================*/


function removeNonNumeric(element) {
    element.value = element.value.replaceAll(/\D/g, "");
}
function removeNonAlphaNumeric(element) {
    element.value = element.value.replaceAll(/[\d\@\#\$\%\^\&\*\(\)\.\/\{\}\[\]\;\=]/g, "");
}

function updatePhone(phone) {
    
    let phoneLen = phone.value.length;
    
    if (phoneLen > 10) { 
        phone.value = phone.value.substring(0, 10);
        phoneLen = 10;    
    }
    let where = phoneLen == 10 ? 3 : phoneLen == 9 ? 2 : 0; 
    
    if (where) 
        phone.value = phone.value.substring(0, where) + "-" + phone.value.substring(where);   

}

function updateDate(dateElement) {
    
    let date = dateElement.value;
    let len = date.length;   
    
    if (len > 8)
        date = date.substring(0, 8);
    

    let helper = date;

    if (len >= 2) {

        let day = date.slice(0, 2);

        helper = (parseInt(day) > 31 ? "31" : parseInt(day) < 1 ? "01" : day) + "/"; 
        
        if (len >= 4) {
            let month = date.slice(2, 4);
            let year = date.slice(4);    
            let currYear = new Date().getFullYear();

            helper +=  (parseInt(month) > 12 ? "12" : parseInt(month) < 1 ? "01" : month ) + "/" + (year.length == 4 ? (parseInt(year) < 1903 ? "1903" : parseInt(year) > currYear ? currYear : year ) : year);
        }
        else 
            helper += date.slice(2);
    } 

    dateElement.value = helper;

}

function removeLeadingZero(element) {
    element.value = element.value.replace(/^0+/, '');
}

function checklimit(element, limit, infinite=true) {
    element.value = element.value > limit ? (infinite ? "infinite" : limit) : element.value < 0 ? 1 : element.value; 
}

function updatePassword(password) {
    let lower = document.querySelector(".lowercase");
    let upper = document.querySelector(".uppercase");
    let digit = document.querySelector(".digit");
    let special = document.querySelector(".special");
    let length = document.querySelector(".length");

    password.match(/[a-z]/) ? 
        lower.classList.add("valid") :
        lower.classList.remove("valid");
    password.match(/[A-Z]/) ? 
        upper.classList.add("valid") :
        upper.classList.remove("valid");
    password.match(/[\d]/i) ? 
        digit.classList.add("valid") :
        digit.classList.remove("valid");
    password.match(/[!|@|#|$|%|^|&|*]/i) ? 
        special.classList.add("valid") :
        special.classList.remove("valid");
    password.length >= 8 ? 
        length.classList.add("valid") :
        length.classList.remove("valid");
}
