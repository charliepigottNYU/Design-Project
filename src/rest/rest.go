package main

import (
    . "../../lib"
    "encoding/binary"
    "fmt"
    "html/template"
    "io"
    "log"
    "os/exec"
    "net"
    "net/http"
    "mime/multipart"
    "strings"
    "time"
)

const SESSION_COOKIE = "session"
const BUFFER_SIZE = 1024

var LOG map[int]*log.Logger

func main() {
    //rest API built using golangs http library
    http.HandleFunc("/", welcomeRedirect)
    http.HandleFunc("/welcome", welcome)
    http.HandleFunc("/home", home)
    http.HandleFunc("/signup", signup)
    http.HandleFunc("/login", login)
    http.HandleFunc("/file_upload",upload)
    http.HandleFunc("/play", play)
    http.HandleFunc("/signup-submit", signupSubmit)
    http.HandleFunc("/login-submit", loginSubmit)
    http.HandleFunc("/get_songs", getSongs)
    http.HandleFunc("/search", search)
    http.HandleFunc("/song-page", populateSongPage)

    http.Handle("/song/", http.StripPrefix("/song/", http.FileServer(http.Dir("../../data"))))

    LOG = InitLog("../../log/rest.log")
    http.ListenAndServe(":8080",nil)
}

//redirects to relevant http pages
func welcomeRedirect(w http.ResponseWriter, r *http.Request) {
    http.Redirect(w, r, "/welcome", http.StatusSeeOther)
}

func welcome(w http.ResponseWriter, r *http.Request) {
    clearCache(w)
    _, ok := getCookie(r)
    if ok {
        http.Redirect(w, r, "/home", http.StatusSeeOther)
        return
    }
    http.ServeFile(w, r, "../../web/welcome.html")
}

func home(w http.ResponseWriter, r *http.Request) {
    clearCache(w)
    _, ok := getCookie(r)
    if !ok {
        http.Redirect(w, r, "/welcome", http.StatusSeeOther)
        return
    }
    http.ServeFile(w, r, "../../web/home.html")
}

func signup(w http.ResponseWriter, r *http.Request) {
    clearCache(w)
    _, ok := getCookie(r)
    if ok {
        http.Redirect(w, r, "/home", http.StatusSeeOther)
        return
    }
    http.ServeFile(w, r, "../../web/signup.html")
}

func login(w http.ResponseWriter, r *http.Request) {
    clearCache(w)
    _, ok := getCookie(r)
    if ok {
        http.Redirect(w, r, "/home", http.StatusSeeOther)
        return
    }
    http.ServeFile(w, r, "../../web/login.html")
}

func play(w http.ResponseWriter, r *http.Request) {
    clearCache(w)
    _, ok := getCookie(r)
    if !ok {
        http.Redirect(w, r, "/welcome", http.StatusSeeOther)
        return
    }
    if r.Method == http.MethodGet {
        http.ServeFile(w, r, "../src/sound.mp3")
    }
}

//gets song information from user and sends it to filesystem
func upload(w http.ResponseWriter, r *http.Request) {
    clearCache(w)
    _, ok := getCookie(r)
    if !ok {
        http.Redirect(w, r, "/welcome", http.StatusSeeOther)
        return
    }
    if r.Method == http.MethodGet {
        http.ServeFile(w, r, "../../web/file_upload.html")
    } else if r.Method == http.MethodPost {
        LOG[INFO].Println("new song upload")
        r.ParseForm()
        file, header, err := r.FormFile("song")
        defer file.Close()
        if err != nil {
            LOG[ERROR].Println("error opening file", err)
            return
        }
        sendFile(w, r, file, header)
    }
}

//signup calls shell script to store info in databse, returns error if the username already exists
func signupSubmit(w http.ResponseWriter, r *http.Request) {
   clearCache(w)
   _, ok := getCookie(r)
   if ok {
       http.Redirect(w, r, "/home", http.StatusSeeOther)
       return
   }
   if r.Method == http.MethodPost {
        r.ParseForm()
        if r.PostFormValue("password") == r.PostFormValue("confirm") {
            args := []string{"../../shell/signup.sh", "-u", r.PostFormValue("username"),"-p", r.PostFormValue("password")}
            err := exec.Command("bash", args...).Run()
            if err != nil {
                LOG[ERROR].Println("error sending database info", err)
                return
            }
            LOG[INFO].Println(r.PostFormValue("username")," signup")
            http.SetCookie(w, genCookie(r.PostFormValue("username")))
            http.Redirect(w, r, "/home", http.StatusSeeOther)
        }
    }
}

//login submit functions similarly to signup with shell scripts as the means to interact with the database
func loginSubmit(w http.ResponseWriter, r *http.Request) {
    clearCache(w)
    _, ok := getCookie(r)
    if ok {
        http.Redirect(w, r, "/home", http.StatusSeeOther)
        return
    }
    if r.Method == http.MethodPost {
        r.ParseForm()

        args := []string{"../../shell/login.sh", "-u", r.PostFormValue("username")}
        output, err := exec.Command("bash", args...).Output()
        if err != nil || len(output) == 0 {
            LOG[INFO].Println("Username does not exist", r.PostFormValue("username"))
            return
        }

        password := string(output[:len(output)-1])          //remove newline character from output
        if password != r.PostFormValue("password") {
            LOG[INFO].Println("Incorrect Password",
                r.PostFormValue("username"), r.PostFormValue("password"))
            return
        }
        http.SetCookie(w, genCookie(r.PostFormValue("username")))
        http.Redirect(w, r, "/home", http.StatusSeeOther)
    }
}

func getSongs(w http.ResponseWriter, r *http.Request) {
    clearCache(w)
    cookie, ok := getCookie(r)
    if !ok {
        LOG[ERROR].Println("session cookie not found")
        http.Redirect(w, r, "/welcome", http.StatusSeeOther)
        return
    }
    var result []string
    args := []string{"../../shell/get_contributer_songs.sh", "-u", cookie.Value}
    output, err := exec.Command("bash", args...).Output()
    if err != nil || len(output) <= 1 {
        LOG[INFO].Println("User", cookie.Value, "has no contributions")
    } else {
        result = strings.Split(string(output[:len(output)-1]), " ")
    }
    var songs []struct {Title, Path string}
    for _, elem := range(result) {
        nameAndPath := strings.Split(elem, ",")
        fmt.Println(elem)
        songs = append(songs, struct{Title, Path string}{nameAndPath[0], nameAndPath[1]})
    }
    t, err := template.ParseFiles("../../web/view_songs.html")
    if err != nil {
        LOG[ERROR].Println("HTML Template Error", err)
        http.Redirect(w, r, "/welcome", http.StatusSeeOther)
        return
    }
    err = t.Execute(w, songs)
    if err != nil {
        LOG[ERROR].Println("Unable to execute template", err)
        return
    }
}

func search(w http.ResponseWriter, r *http.Request) {
    clearCache(w)
    _, ok := getCookie(r)
    if !ok {
        http.Redirect(w, r, "/welcome", http.StatusSeeOther)
        return
    }
    if r.Method == http.MethodGet {
        r.ParseForm()
        var result []string
        args := []string{"../../shell/get_songs_by_name.sh", "-s", r.FormValue("song")}
        output, err := exec.Command("bash", args...).Output()
        if err != nil || len(output) <= 1 {
            LOG[INFO].Println("No songs found for search", r.PostFormValue("song"), err)
        } else {
            result = strings.Split(string(output[:len(output)-1]), " ")
        }
        var songs []struct {Title, Creator string}
        for _, elem := range(result) {
            titleAndCreator := strings.Split(elem, ",")
            fmt.Println(elem)
            songs = append(songs, struct{Title, Creator string}{titleAndCreator[0], titleAndCreator[1]})
        }
        t, err := template.ParseFiles("../../web/search.html")
        if err != nil {
            LOG[ERROR].Println("HTML Template Error", err)
            http.Redirect(w, r, "/", http.StatusSeeOther)
            return
        }
        err = t.Execute(w, songs)
        if err != nil {
            LOG[ERROR].Println("Unable to execute template", err)
            return
        }
    }
}

func populateSongPage(w http.ResponseWriter, r *http.Request) {
    clearCache(w)
    if r.Method == http.MethodPost {
        r.ParseForm()
        creator := r.PostFormValue("creator")
        title := r.PostFormValue("title")

        var contributers []string
        args := []string{"../../shell/get_contributer_by_song.sh", "-s", title, "-u", creator}
        output, err := exec.Command("bash", args...).Output()
        if err != nil || len(output) <= 1 {
            LOG[WARNING].Println("No contributers to song:", title, "for creator", creator)
        } else {
            contributers = strings.Split(string(output[:len(output)-1]), " ")
        }

        var modifications []string
        args = []string{"../../shell/get_modification_by_song.sh", "-s", title, "-u", creator}
        output, err = exec.Command("bash", args...).Output()
        if err != nil || len(output) <= 1 {
            LOG[INFO].Println("No modifications for song:", title, "created by", creator)
        } else {
            modifications = strings.Split(string(output[:len(output)-1]), " ")
        }

        var modInfo []struct{Title, Path, Votes string}
        for _, elem := range(modifications) {
            info := strings.Split(elem, ",")
            fmt.Println(elem)
            modInfo = append(modInfo, struct{Title, Path, Votes string}{info[0], info[1], info[2]})
        }

        t, err := template.ParseFiles("../../web/song_page.html")
        err = t.Execute(w, struct{
            Title string
            Creator string
            Path string
            Contributers []string
            Modifications []struct{Title, Path, Votes string}
        }{
            Title: title,
            Creator: creator,
            Path: /* Path goes here */
            Contributers: contributers,
            Modifications: modInfo,
        })
        if err != nil {
            LOG[ERROR].Println("Unable to execute template", err)
            return
        }
    }
}

func clearCache(w http.ResponseWriter) {
    w.Header().Set("Cache-Control", "no-cache, no-store, must-revalidate")
    w.Header().Set("Pragma", "no-cache")
    w.Header().Set("Expires", "0")
}

//sends file information over tcp to the filesystem, connects on port 5000
func sendFile(w http.ResponseWriter, r *http.Request, file multipart.File,
        header *multipart.FileHeader){

    LOG[INFO].Println("New Song Upload:", header.Filename, header.Size)
    conn, err := net.Dial("tcp","127.0.0.1:5000")
    if err != nil {
        LOG[ERROR].Println("error connecting to port 5000", err)
        return
    }
    defer conn.Close()
    cookie, ok := getCookie(r)
    if !ok {
        LOG[INFO].Println("session cookie not found")
        return
    }

    err = binary.Write(conn, binary.LittleEndian, CommandCreateSong)

    //send the size of the username over the network
    var userSize uint8
    userSize = uint8(len(cookie.Value))
    err = binary.Write(conn, binary.LittleEndian, userSize)
    if err != nil {
        LOG[ERROR].Println("binary.Write failed:", err)
        return
    }

    //send username
    fmt.Fprintf(conn, cookie.Value)

    //send length of file name over network then file name
    var fileNameSize int64
    fileNameSize = int64(len(header.Filename))
    err = binary.Write(conn, binary.LittleEndian, fileNameSize)
    if err != nil {
        LOG[ERROR].Println("binary.Write failed:", err)
        return
    }

    fmt.Fprintf(conn, header.Filename)

    var result uint8 = 0
    binary.Read(conn, binary.LittleEndian, &result)
    if result == 0 {
        LOG[WARNING].Println("Invalid song name:", header.Filename)
        return
    }

    //send the size of the song file, then the size
    err = binary.Write(conn, binary.LittleEndian, header.Size)
    if err != nil {
        LOG[ERROR].Println("binary.Write failed:", err)
        return
    }
    sendBuffer := make([]byte, BUFFER_SIZE)
    for {
        _, err = file.Read(sendBuffer)
        if err == io.EOF {
            break
        }
        conn.Write(sendBuffer)
    }
}

func getCookie(r *http.Request) (*http.Cookie, bool){
    cookie, _ := r.Cookie(SESSION_COOKIE)
    if cookie == nil {
        return nil, false
    }
    return cookie, true
}

func genCookie(id string) *http.Cookie {
    return &http.Cookie{
        Name: SESSION_COOKIE,
        Value: id,
        Expires: time.Now().Add(24 * time.Hour),
    }
}

