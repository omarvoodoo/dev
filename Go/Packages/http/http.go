package main

import (
	"encoding/json"
	"log"
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
	http.HandleFunc("/json", jsonHandler)
	http.ListenAndServe(":8080", nil)
}

func textHandler(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "text/plain")
	w.Write(helloWorldBytes)
}

type joinData struct {
	Test string
}

// Test it with: curl -X POST -d "{\"test\": \"that\"}" http://localhost:8080/json
func jsonHandler(w http.ResponseWriter, r *http.Request) {
	// Body 구문을 분석
	decoder := json.NewDecoder(r.Body)
	t := new(joinData)
	err := decoder.Decode(&t)
	checkErr(err, "Decode failed")
	log.Println(t.Test)
}

func checkErr(err error, msg string) {
	if err != nil {
		log.Fatalln(msg, err)
	}
}
