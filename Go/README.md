## Go의 특징

- 빠른 개발 속도
 -  Go의 개발 생산성은 무척 높습니다. 언어 내부에 공용 서버 컴포넌트가 있어 복잡한 서버를 빠른 시간안에 작성 할 수 있습니다. 적은 코드는 정확한 기능을 빠르게 만든다는 것을 의미합니다.
- 많은 기능
 - Go언어는 파이썬과 같이 많은 서버프로그램의 기능을 언어 자체에 내장하고 있습니다. 가령, 웹서버를 돌린다거나 소켓 서버를 작성하는 것, OS의 내부 기능을 엑세스하는 것등 파이썬과 거의 유사하게 이용할 수 있습니다.
- 멀티코어 지원, 높은 성능
 - 언어의 실행은 놀랍게도 빠르고 안정적입니다. 심지어 파이썬으로 개발하는 사람들에게는 경이적이다는 생각까지 들 정도입니다. 파이썬과 Node.js의 런타임과는 달리 Go의 런타임은 다중 OS 쓰레드를 지원하고, 동시적인 실행기능을 지원합니다.
- 크로스 컴파일
 - Go언어는 맥에서 작성되고 테스트 된 프로그램이 리눅스에서 완벽하게 컴파일 될 수 있음을 보장합니다. 그리고 대상 시스템을 맞는 옵션으로 컴파일을 하여 바이너리를 대상시스템으로 복사하면 바로 작동이 가능합니다.
- 가비즈 컬렉션
 - Go언어는 가비즈 컬렉션을 실행 파일안에 내장합니다. 가상 머신 위에서 실행되고 가상 머신이 메모리를 관리해 주는 Java나 C#과는 차이가 있습니다.

### References

- [Go 프로그래밍 언어의 가치](http://younghoonhong.blogspot.kr/2013/09/go.html)
- [Google이 만드는 꿈의 언어 Go, 10년 뒤 세상을 지배 할 수 있을까?](http://apps.iamday.net/apps/talk/662/view.iamday?con=iphone)

## Docs

### [Exported/Unexported Identifiers In Go](Docs/Exported_Unexported_Identifiers_In_Go.md)

### [Function closures(클로저)](Docs/function_closures.md)

## Tutorial Links

- [A Tour of Go](http://go-tour-kr.appspot.com/#1)

## 패키지 소개

### [http](Packages/http)
HTTP 요청을 핸들링하고 처리하는 서버를 구현한다.

### [runtime](Packages/runtime)
runtime 패키지는 goroutines을 제어하는 기능등의 런타임 시스템과 상호 작용하는 작업을 포함한다. 

### [flag](Packages/flag)
터미널에서 명령어를 실행할 때 전달된 명령줄 인자를 파싱한다.

### [GoRP(Go Relational Persistence)](Packages/gorp)
gorp 패키지는 간단하게 SQL databases를 Go 구조체로 마샬링 해준다.
