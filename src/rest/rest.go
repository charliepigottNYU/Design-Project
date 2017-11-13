package main

import (
    . "./rest_log"
    "fmt"
    "io"
    "log"
    "os/exec"
    "net"
    "net/http"
    "strconv"
    "mime/multipart"
    "time"
)

const SESSION_COOKIE = "session"
const BUFFER_SIZE = 1024

var LOGGER map[int]*log.Logger

func main() {
    //rest API built using golangs http library
    http.HandleFunc("/welcome", welcome)
    http.HandleFunc("/signup", signup)
    http.HandleFunc("/login", login)
    http.HandleFunc("/file_upload",upload)
    http.HandleFunc("/play", play)
    http.HandleFunc("/signup-submit", signupSubmit)
    http.HandleFunc("/login-submit", loginSubmit)

    LOGGER = InitLog("../../log/rest.log")
    http.ListenAndServe(":8080",nil)
}

//redirects to relivant http pages
func welcome(w http.ResponseWriter, r *http.Request) {
    http.ServeFile(w, r, "../../web/welcome.html")
}

func signup(w http.ResponseWriter, r *http.Request) {
    http.ServeFile(w, r, "../../web/signup.html")
}

func login(w http.ResponseWriter, r *http.Request) {
    http.ServeFile(w, r, "../../web/login.html")
}

func play(w http.ResponseWriter, r *http.Request) {
    clearCache(w)
    if r.Method == http.MethodGet {
        http.ServeFile(w, r, "../src/sound.mp3")
    }
}

//gets song information from user and sends it to filesystem
func upload(w http.ResponseWriter, r *http.Request) {
    clearCache(w)
    if r.Method == http.MethodGet {
        http.ServeFile(w, r, "../../web/file_upload.html")
    } else if r.Method == http.MethodPost {
        r.ParseForm()
        file, header, err := r.FormFile("song")
        defer file.Close()
        if err != nil {
            fmt.Println("error opening file", err)
            return
        }
        sendFile(file, header)
    }
}

//signup calls shell script to store info in databse, returns error if the username already exists
func signupSubmit(w http.ResponseWriter, r *http.Request) {
   clearCache(w)

   if r.Method == http.MethodPost {
        r.ParseForm()
        if r.PostFormValue("password") == r.PostFormValue("confirm") {
            args := []string{"../../shell/signup.sh", "-u", r.PostFormValue("username"),"-p", r.PostFormValue("password")}
            err := exec.Command("bash", args...).Run()
            if err != nil {
                LOGGER[ERROR].Println("error sending database info", err)
                return
            }
            fmt.Println(r.PostFormValue("username")," signup")
            http.SetCookie(w, genCookie(r.PostFormValue("username")))
        }
    }
}

//login submit functions similarly to signup with shell scripts as the means to interact with the database
func loginSubmit(w http.ResponseWriter, r *http.Request) {
    clearCache(w)

    if r.Method == http.MethodPost {
        r.ParseForm()

        args := []string{"../../shell/login.sh", "-u", r.PostFormValue("username")}
        output, err := exec.Command("bash", args...).Output()
        if err != nil || len(output) == 0 {
            LOGGER[INFO].Println("Username does not exist", r.PostFormValue("username"))
            return
        }

        password := string(output[:len(output)-1])          //remove newline character from output
        if password != r.PostFormValue("password") {
            LOGGER[INFO].Println("Incorrect Password",
                r.PostFormValue("username"), r.PostFormValue("password"))
            return
        }
        fmt.Println("login complete")
    }
}

func clearCache(w http.ResponseWriter) {
    w.Header().Set("Cache-Control", "no-cache, no-store, must-revalidate")
    w.Header().Set("Pragma", "no-cache")
    w.Header().Set("Expires", "0")
}

//sends file information over tcp to the filesystem, connects on port 5000
func sendFile(file multipart.File, header *multipart.FileHeader){
    LOGGER[INFO].Println("New Song Upload:", header.Filename, header.Size)
    conn, err := net.Dial("tcp","127.0.0.1:5000")
    defer conn.Close()
    if err != nil {
        LOGGER[ERROR].Println("error connecting to port 5000", err)
        return
    }
    fmt.Fprintf(conn, strconv.FormatInt(header.Size, 10))
    sendBuffer := make([]byte, BUFFER_SIZE)
    for {
        _, err = file.Read(sendBuffer)
        if err == io.EOF {
            break
        }
        conn.Write(sendBuffer)
    }
}

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
        Value: id,
        Expires: time.Now().Add(24 * time.Hour),
    }
}

