package main

import (
    "net/http"
)

func main() {
    fs := http.FileServer(http.Dir("./"))
    http.Handle("/", fs)

    err := http.ListenAndServe(":3000", nil)
    if err != nil {
        panic("failed to start server")
    }
}
