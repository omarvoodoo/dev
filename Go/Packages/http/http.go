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

type joinReqData struct {
	Test string
}

type joinRspData struct {
	Result string
}

// Test it with: curl -X POST -d "{\"test\": \"that\"}" http://localhost:8080/json
func jsonHandler(w http.ResponseWriter, r *http.Request) {
	// Body 구문을 분석
	reqBody := new(joinReqData)
	err := json.NewDecoder(r.Body).Decode(&reqBody)
	checkErr(err, "Decode failed")
	log.Println(reqBody.Test)

	// json형식의 응답 메세지 구성
	rspBody := joinRspData{
		Result: "result hahahaha",
	}
	json.NewEncoder(w).Encode(rspBody)
}

func checkErr(err error, msg string) {
	if err != nil {
		log.Fatalln(msg, err)
	}
}
