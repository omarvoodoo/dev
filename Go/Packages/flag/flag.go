package main

import (
  "flag"
  "fmt"
)

func main() {
  // 기본 값
  // var paramB = flag.Bool("param_b", false, "desc param_b")
  param := flag.Int("param", 1, "description")
  flag.Parse()
  fmt.Printf("param = %d\n", *param)
}
