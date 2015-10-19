## runtime

runtime 패키지는 goroutines을 제어하는 기능등의 런타임 시스템과 상호 작용하는 작업을 포함한다. 

### GOMAXPROCS

GOMAXPROCS 변수는 운영체제가 사용자-레벨 Go 코드를 동시에 실행할 수 있는 최대 개수를 나타낸다. 기본 값은 1이고 CPU 개수로 지정하는 것을 추천한다.

```go
runtime.GOMAXPROCS(runtime.numCPU())
```
