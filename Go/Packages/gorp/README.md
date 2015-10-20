## Installation

```go
// install the library
go get gopkg.in/gorp.v1

// use in your .go code
import (
	"gopkg.in/gorp.v1"
)
```

## Examples

```go
type Invoice struct {
    Id       int64
    Created  int64
    Updated  int64
    Memo     string
    PersonId int64
}

type Person struct {
    Id      int64
    Created int64
    Updated int64
    FName   string
    LName   string
}
```

```go
// Go의 표준 SQL API를 이용하여 DB에 연결
db, err := sql.Open("mysql", "myuser:mypassword@localhost:3306/dbname")

// gorp DbMap 객체 생성
dbmap := &gorp.DbMap{Db: db, Dialect: gorp.MySQLDialect{"InnoDB", "UTF8"}}

// gorp에서 사용할 구조체를 등록한다.
// SetKeys(true)는 자동으로 증가되는 기본키를 지정한다.
t1 := dbmap.AddTableWithName(Invoice{}, "invoice_test").SetKeys(true, "Id")
t2 := dbmap.AddTableWithName(Person{}, "person_test").SetKeys(true, "Id")
```
