# 지연 / 패닉 / 복구

> 아래 내용은 [codingnuri.com](http://codingnuri.com/golang-book/7.html)에서 퍼왔습니다.

## defer(지연)

Go에서 `defer`는 해당 함수가 실행 완료될때 호출되는 구문을 지정한다.

```go
package main
 
import "fmt"
 
func first() {
    fmt.Println("1st")
}
func second() {
    fmt.Println("2nd")
}
func main() {
    defer second()
    first()
}
```

위 코드는 `1st`를 출력한 후 이어서 `2nd`를 출력한다. 기본적으로 `defer`는 `second`에 대한 호출을 해당 함수의 끝으로 옮긴다.

`defer`는 어떤 식으로든 자원을 해제해야 할 때 자주 사용된다. 예를 들어, 파일을 열 때 나중에 해당 파일을 반드시 닫아야 한다. defer를 이용하면 다음과 같이 하면 된다.

```go
f, _ := os.Open(filename)
defer f.Close()
```

이 방법에는 세 가지 장점이 있다. (1) Close 호출을 Open 호출 가까이에 둬서 이해하기가 쉽고, (2) 함수에 반환문이 여러 개 있더라도(if에 하나가 들어 있고, else에 하나가 들어 있는 것처럼) Close가 두 반환문 앞에서 모두 호출될 것이며, (3) 지연된 함수는 런타임 패닉이 일어나더라도 실행될 것이다.

## panic(패닉) / recover(복구)

Go에 내장된 recover 함수를 이용하면 런타임 패닉을 처리할 수 있다. recover는 패닉을 중단하고 panic 호출에 전달된 값을 반환한다. 
아래의 코드는 `panic`이라는 함수를 호출해 런타임 오류를 강제로 일으키고 다음 라인에서 `recover` 함수를 호출하여 복구 하도록 되어 있다.

```go
package main
 
import "fmt"
 
func main() {
    panic("PANIC")
    str := recover()
    fmt.Println(str)
}
```

하지만 이 경우 recover는 절대 호출되지 않을 텐데, panic에 대한 호출이 즉시 함수 호출을 중단할 것이기 때문이다. 대신 defer와 짝을 맞춰야 한다. 

```go
import "fmt"
 
func main() {
    defer func() {
        str := recover()
        fmt.Println(str)
    }()
    panic("PANIC")
}
```

일반적으로 패닉은 프로그래밍 오류를 나타내거나(가령 범위를 벗어난 배열 인덱스에 접근하려고 시도하거나 맵을 초기화하는 것을 잊어버리는 등) 손쉽게 복구할 수 없는 예외적인 상황을 나타낸다(그래서 "패닉"이라고 하는 것이다).
