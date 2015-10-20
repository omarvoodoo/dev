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

등록되어 있는 구조체들과 매칭되는 테이블들을 생성하거나 제거 할 수 있다. 유닛 테스트시에 유용할 것이다.

```go
// create all registered tables
dbmap.CreateTables()

// drop
dbmap.DropTables()
```

Optionally you can pass in a log.Logger to trace all SQL statements:

```go
// Will log all SQL statements + args as they are run
// The first arg is a string prefix to prepend to all log messages
dbmap.TraceOn("[gorp]", log.New(os.Stdout, "myapp:", log.Lmicroseconds)) 

// Turn off tracing
dbmap.TraceOff()
```

다음은 데이터를 저장하는 코드이다.

```go
// Must declare as pointers so optional callback hooks
// can operate on your data, not copies
inv1 := &Invoice{0, 100, 200, "first order", 0}
inv2 := &Invoice{0, 100, 200, "second order", 0}

// Insert your rows
err := dbmap.Insert(inv1, inv2)

// Because we called SetKeys(true) on Invoice, the Id field
// will be populated after the Insert() automatically
fmt.Printf("inv1.Id=%d  inv2.Id=%d\n", inv1.Id, inv2.Id)
```

다음과 같이 raw SQL을 실행 할 수 있다.

```go
res, err := dbmap.Exec("delete from invoice_test where PersonId=?", 10)
```

다음과 같이 Join도 가능하다.

```go
// Define a type for your join
// It *must* contain all the columns in your SELECT statement
//
// The names here should match the aliased column names you specify
// in your SQL - no additional binding work required.  simple.
//
type InvoicePersonView struct {
    InvoiceId   int64
    PersonId    int64
    Memo        string
    FName       string
}

// Create some rows
p1 := &Person{0, 0, 0, "bob", "smith"}
dbmap.Insert(p1)

// notice how we can wire up p1.Id to the invoice easily
inv1 := &Invoice{0, 0, 0, "xmas order", p1.Id}
dbmap.Insert(inv1)

// Run your query
query := "select i.Id InvoiceId, p.Id PersonId, i.Memo, p.FName " +
    "from invoice_test i, person_test p " +
    "where i.PersonId = p.Id"
list, err := dbmap.Select(InvoicePersonView{}, query)

// this should test true
expected := &InvoicePersonView{inv1.Id, p1.Id, inv1.Memo, p1.FName}
if reflect.DeepEqual(list[0], expected) {
    fmt.Println("Woot! My join worked!")
}
```

트랜젝션을 이용한 일괄 처리도 가능하다..

```go
func InsertInv(dbmap *DbMap, inv *Invoice, per *Person) error {
    // Start a new transaction
    trans := dbmap.Begin()

    trans.Insert(per)
    inv.PersonId = per.Id
    trans.Insert(inv)

    // if the commit is successful, a nil error is returned
    return trans.Commit()
}
```

Use hooks to update data before/after saving to the db. Good for timestamps:

```go
// implement the PreInsert and PreUpdate hooks
func (i *Invoice) PreInsert(s gorp.SqlExecutor) error {
    i.Created = time.Now().UnixNano()
    i.Updated = i.Created
    return nil
}

func (i *Invoice) PreUpdate(s gorp.SqlExecutor) error {
    i.Updated = time.Now().UnixNano()
    return nil
}

// You can use the SqlExecutor to cascade additional SQL
// Take care to avoid cycles. gorp won't prevent them.
//
// Here's an example of a cascading delete
//
func (p *Person) PreDelete(s gorp.SqlExecutor) error {
    query := "delete from invoice_test where PersonId=?"
    err := s.Exec(query, p.Id); if err != nil {
        return err
    }
    return nil
}
```

Full list of hooks that you can implement:

```go
PostGet
PreInsert
PostInsert
PreUpdate
PostUpdate
PreDelete
PostDelete

All have the same signature.  for example:

func (p *MyStruct) PostUpdate(s gorp.SqlExecutor) error
```

Optimistic locking (similar to JPA)

```go
// Version is an auto-incremented number, managed by gorp
// If this property is present on your struct, update
// operations will be constrained
//
// For example, say we defined Person as:

type Person struct {
    Id       int64
    Created  int64
    Updated  int64
    FName    string
    LName    string

    // automatically used as the Version col
    // use dbmap.SetVersionCol() to map a different
    // struct field as the version field
    Version  int64
}

p1 := &Person{0, 0, 0, "Bob", "Smith", 0}
dbmap.Insert(p1)  // Version is now 1

obj, err := dbmap.Get(Person{}, p1.Id)
p2 := obj.(*Person)
p2.LName = "Edwards"
dbmap.Update(p2)  // Version is now 2

p1.LName = "Howard"

// Raises error because p1.Version == 1, which is out of date
count, err := dbmap.Update(p1)
if _, ok := err.(OptimisticLockError); !ok {
    // should reach this statement

    // in a real app you might reload the row and retry, or
    // you might propegate this to the user, depending on the desired
    // semantics
    fmt.Printf("Tried to update row with stale data: %v\n", err)
}
```

### References

- [Go Relational Persistence](https://gowalker.org/github.com/dspiteself/gorp)
