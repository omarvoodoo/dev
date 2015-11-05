# Redigo

Redis DB를 위한 클라이언트 패키지

- [Redigo](https://github.com/garyburd/redigo)
- [Go Doc / Redigo](http://godoc.org/github.com/garyburd/redigo/redis)

## Redis의 기본 사용방법

Redigo는 아래와 같은 방법으로 Redis에 연결합니다.

```go
c, err := redis.Dial("tcp", ":6379")
if err != nil {
    // handle error
}
defer c.Close()
```

연결에 성공하면 `Do` 함수를 이용하여 Redis 명령(SET이나 GET 등)을 다음과 같은 형식으로 작성하여 Redis에 접근하여 데이터의 쓰기나 읽기 등을 처리 할 수 있다.

```go
Do(commandName string, args ...interface{}) (reply interface{}, err error)
```

> `commandName`에 SET이나 GET같은 Redis 명령어가 들어간다

### Example

```go
package main

import (
        "fmt"
        "os"
        "flag"
        "github.com/garyburd/redigo/redis"
)

func main() {
        var h = flag.String("hostname", "127.0.0.1", "Set Hostname")
        var p = flag.String("port", "6379", "Set Port")
        var key = flag.String("key", "foo", "Set Key")
        var val = flag.String("value", "bar", "Set Value")
        flag.Parse()

        c, err := redis.Dial("tcp", *h + ":" + *p)

        if err != nil {
                fmt.Println(err)
                os.Exit(1)
        }

        // Connection Close
        defer c.Close()

        c.Do("SET", *key, *val)
        s, err := redis.String(c.Do("GET", *key))
        if err != nil {
                fmt.Println(err)
                os.Exit(1)
        }
        fmt.Printf("%#v\n", s)
}
```
