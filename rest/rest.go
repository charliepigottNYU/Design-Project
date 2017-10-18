package main

import (
    "os/exec"
    "fmt"
    "net/http"
    //"time"
)

const SESSION_COOKIE = "session"

func main(){
    http.HandleFunc("/signup", signup)

    http.HandleFunc("/signup-submit", signupSubmit)

    http.ListenAndServe(":80",nil)
}

func signup(w http.ResponseWriter, r *http.Request){
    http.ServeFile(w, r, "../web/signup.html")
}

func signupSubmit(w http.ResponseWriter, r *http.Request){
    //Do not cache the signup
    w.Header().Set("Cache-Control", "no-cache, no-store, must-revalidate")
    w.Header().Set("Pragma", "no-cache")
    w.Header().Set("Expires", "0")
    fmt.Println("here")
    if r.Method == http.MethodPost {
        r.ParseForm()
        if r.PostFormValue("password") == r.PostFormValue("confirm") {
            args := []string{"../shell/insert.sh", "-u",r.PostFormValue("username"),"-p",r.PostFormValue("password")}
            if err := exec.Command("bash", args...).Run(); err != nil{
                fmt.Println("error sending database info",err)
                return
            }
            fmt.Println(r.PostFormValue("username")," signup")
        }
    }
}


//cookie stuff to do later
/*
func getCookie(w http.ResponseWriter, r *http.Request) (bool, *http.Cookie){
    cookie, _ := r.Cookie(SESSION_COOKIE)
    if cookie == nil {
        return false, nil
    }
    return true, cookie
}

func genCookie(id string) *http.Cookie {
    return &http.Cookie{
        Name: SESSION_COOKIE,
        Value: id
        Expires: time.Now().add(24 * time.Hour)
    }
}
*/
