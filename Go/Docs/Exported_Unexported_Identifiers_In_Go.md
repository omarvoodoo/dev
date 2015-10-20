
# Exported/Unexported Identifiers In Go

Go언어에서는 변수나 함수의 이름을 명명할때 첫번째 문자가 대문자일때와 소문자일때에 다른 특징을 보여준다.

첫번째 문자가 `소문자`이면 private 특성을 가지며 해당 변수가 선언된 패키지 내부에서만 사용 가능하다.

첫번째 문자가 `대문자`이면 public 특성을 가지며 패키지로부터 export된다. export된 식별자들은 외부 패키지에서 직접 접근하여 사용이 가능하다.

## Direct Identifier Access

```go
package counters

// AlertCounter is an exported type that
// contains an integer counter for alerts.
type AlertCounter int
```

```go
package main

import (
	"fmt"
	"test/counters"
)

func main() {
	// Create a variable of the exported type and
	// initialize the value to 10.
	counter := counters.AlertCounter(10)

	fmt.Printf("Counter: %d\n", counter)
}
```

Since the `AlertCounter` type has been exported, this code builds fine. When we run the program we get the value of 10.

Now let's change the exported `AlertCounter` type to be an unexported type by changing the name to `alertCounter` and see what happens:

```go
package counters

// alertCounter is an unexported type that
// contains an integer counter for alerts.
type alertCounter int
```

```go
package main

import (
	"fmt"
	"test/counters"
)

func main() {
	// Attempt to create a variable of the unexported type
	// and initialize the value to 10. This will NOT compile.
	counter := counters.alertCounter(10)

	fmt.Printf("Counter: %d\n", counter)
}
```

첫번째 문자를 소문자로 변경한 `alertCounter`를 main 패키지에서 사용하게되면 아래와 같이 컴파일 에러가 발생한다.

```bash
./main.go:11: cannot refer to unexported name counters.alertCounter
./main.go:11: undefined: counters.alertCounter
```

alertCounter type은 unexported 되어 있기 때문에 직접 접근이 안된다.

```go
package counters

// alertCounter is an unexported type that
// contains an integer counter for alerts.
type alertCounter int

// NewAlertCounter creates and returns objects of
// the unexported type alertCounter.
func NewAlertCounter(value int) alertCounter {
	return alertCounter(value)
}
```

```go
package main

import (
	"fmt"
	"test/counters"
)

func main() {
	// Create a variable of the unexported type using the
	// exported NewAlertCounter function from the package counters.
	counter := counters.NewAlertCounter(10)

	fmt.Printf("Counter: %d\n", counter)
}
```

In the counters package we add an exported function called NewAlertCounter. This function creates and returns values of the alertCounter type. In the main program we use this function and the programming logic stays the same.

What this example shows is that an identifier that is declared as unexported can still be accessed and used by other packages. It just can't be accessed directly.

## Using Structs

```go
package animals

// Dog represents information about dogs.
type Dog struct {
	Name         string
	BarkStrength int
	age          int
}
```

```go
package main

import (
	"fmt"
	"test/animals"
)

func main() {
	// Create an object of type Dog from the animals package.
	// This will NOT compile.
	dog := animals.Dog{
		Name:         "Chole",
		BarkStrength: 10,
		age:          5,
	}

	fmt.Printf("Counter: %#v\n", dog)
}
```

컴파일러는 다음과 같은 에러를 발생시킨다.

```sh
./main.go:14: unknown animal.Dog field 'age' in struct literal
```

예상대로 `age` 필드는 직접 접근이 안된다.

## References

 - [exportedunexported-identifiers-in-go](http://www.goinggo.net/2014/03/exportedunexported-identifiers-in-go.html)
 - [Go variable export unexport](http://jen6.tistory.com/92)
