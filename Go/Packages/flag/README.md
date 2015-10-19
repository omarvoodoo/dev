## flag

터미널에서 명령어를 실행할 때 전달된 명령줄 인자를 파싱한다. 기본 값 지정도 가능하다.

```go
package main

import (
  "flag"
  "fmt"
)

func main() {
  // param이라는 flag를 선언(기본값 1). param 포인터 변수에 *int형으로 저장된다.
  param := flag.Int("param", 1, "description")
  // paramB := flag.Bool("param_b", false, "desc param_b")
  flag.Parse()
  fmt.Printf("param = %d\n", *param)
}
```

결과

```shell
$ go run flag.go --param=100
param = 100
$ go run flag.go
param = 1
```

아래와 같이 Var() 함수들을 사용하여 변수를 flag에 결합할 수 있다.

```go
var flagvar int
func init() {
	flag.IntVar(&flagvar, "flagname", 1234, "help message for flagname")
}
```
