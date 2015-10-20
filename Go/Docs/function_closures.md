# Function closures

함수 안에 함수를 만든다.

> 아래 내용은 [codingnuri.com](http://codingnuri.com)에서 퍼왔습니다.

```go
func main() {
    add := func(x, y int) int {
        return x + y
    }
    fmt.Println(add(1,1))
}
```

`add`는 타입이 `func(int, int)int`인(두 개의 int를 받아 int를 반환하는 함수) 지역 변수다. 이런 식으로 지역 함수를 생성할 경우 다른 지역 변수에도 접근할 수 있다.

```go
func main() {
    x := 0
    increment := func() int {
        x++
        return x
    }
    fmt.Println(increment())
    fmt.Println(increment())
}
```

increment는 main 함수의 유효범위 안에 정의돼 있는 변수 x에 1을 더한다. 이 x 변수는 increment 함수에서 접근하거나 수정할 수 있다. 이는 처음으로 increment를 호출했을 때 1이 출력되지만 두 번째로 호출했을 때는 2가 출력되기 때문이다.

이 같은 함수와 해당 함수가 참조하는 비지역 변수를 포함해서 클로저(closure)라 한다. 이 경우 increment와 변수 x가 클로저를 형성한다.

클로저를 사용하는 한 방법은 호출됐을 때 숫자의 나열을 생성할 수 있는 또 다른 함수를 반환하는 함수를 작성하는 것이다. 예를 들어, 다음 예제는 짝수를 생성하는 방법을 보여준다.

```go
func makeEvenGenerator() func() uint {
    i := uint(0)
    return func() (ret uint) {
        ret = i
        i += 2
        return
    }
}
func main() {
    nextEven := makeEvenGenerator()
    fmt.Println(nextEven()) // 0
    fmt.Println(nextEven()) // 2
    fmt.Println(nextEven()) // 4
}
```

`makeEvenGenerator`는 짝수를 만들어 내는 함수를 반환한다. 해당 함수를 호출할 때마다 지역 변수 i(평범한 지역 변수와 달리 호출되더라도 값이 지속되는)에 2를 더한다.

