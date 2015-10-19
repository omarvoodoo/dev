package main

import (
	"net/http"
	"runtime"
)

const helloWorldString = "Hello, World!"

var (
	helloWorldBytes = []byte(helloWorldString)
)

func main() {
	runtime.GOMAXPROCS(runtime.NumCPU())

	http.HandleFunc("/text", textHandler)
	http.ListenAndServe(":8080", nil)
}

func textHandler(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "text/plain")
	w.Write(helloWorldBytes)
}
