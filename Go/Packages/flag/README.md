## flag

터미널에서 명령어를 실행할 때 전달된 명령줄 인자를 파싱한다. 기본 값 지정도 가능하다.

```go
package main

import (
  "flag"
  "fmt"
)

func main() {
  // 기본 값
  // paramB := flag.Bool("param_b", false, "desc param_b")
  param := flag.Int("param", 1, "description")
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
