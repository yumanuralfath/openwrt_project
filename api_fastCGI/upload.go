package main

import (
    "fmt"
    "io"
    "net/http"
    "os"
)

func main() {
    // FastCGI hanya perlu listen TCP/Unix socket
    http.HandleFunc("/upload", func(w http.ResponseWriter, r *http.Request) {
        if r.Method != "POST" {
            http.Error(w, "Only POST allowed", http.StatusMethodNotAllowed)
            return
        }
        file, header, err := r.FormFile("file")
        if err != nil {
            http.Error(w, err.Error(), http.StatusBadRequest)
            return
        }
        defer file.Close()

        out, err := os.Create("/mnt/storage/files/" + header.Filename)
        if err != nil {
            http.Error(w, err.Error(), http.StatusInternalServerError)
            return
        }
        defer out.Close()

        io.Copy(out, file)
        fmt.Fprintf(w, "Upload complete: %s", header.Filename)
    })

    http.ListenAndServe("0.0.0.0:9000", nil) // FastCGI bisa listen di TCP atau Unix socket
}

