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
    http.HandleFunc("/login", login)

    http.HandleFunc("/signup-submit", signupSubmit)
    http.HandleFunc("/login-submit", loginSubmit)

    http.ListenAndServe(":8080",nil)
}

func signup(w http.ResponseWriter, r *http.Request){
    http.ServeFile(w, r, "../web/signup.html")
}

func login(w http.ResponseWriter, r *http.Request){
    http.ServeFile(w, r, "../web/login.html")
}

func signupSubmit(w http.ResponseWriter, r *http.Request){
   clearCache(w)

   if r.Method == http.MethodPost {
        r.ParseForm()
        if r.PostFormValue("password") == r.PostFormValue("confirm") {
            args := []string{"../shell/signup.sh", "-u", r.PostFormValue("username"),"-p", r.PostFormValue("password")}
            err := exec.Command("bash", args...).Run()
            if err != nil {
                fmt.Println("error sending database info", err)
                return
            }
            fmt.Println(r.PostFormValue("username")," signup")
        }
    }
}

func loginSubmit(w http.ResponseWriter, r *http.Request){
    clearCache(w)

    if r.Method == http.MethodPost {
        r.ParseForm()

        args := []string{"../shell/login.sh", "-u", r.PostFormValue("username")}
        output, err := exec.Command("bash", args...).Output()
        if err != nil || len(output) == 0 {
            fmt.Println("Incorrect username")
            return
        }

        password := string(output[:len(output)-1])          //remove newline character from output
        if password != r.PostFormValue("password") {
            fmt.Println(password, r.PostFormValue("password"))
            fmt.Println("Incorrect password")
            return
        }
        fmt.Println("login complete")
    }
}

func clearCache(w http.ResponseWriter){
    w.Header().Set("Cache-Control", "no-cache, no-store, must-revalidate")
    w.Header().Set("Pragma", "no-cache")
    w.Header().Set("Expires", "0")
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
