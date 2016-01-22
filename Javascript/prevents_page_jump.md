
# 클릭 이벤트 발생시 페이지 최상단으로 이동되는 현상 막기

### case 1

다음처럼 사용하고 있다면
```javascript
<a href="#"></a>
```
아래와 같은 형식으로 사용한다.
```javascript
<a href="javascript:void(0);"></a>
```

### case 2

javascript 핸들러를 정의할때는 아래처럼 마지막에 'false'를 반환하면 된다.
```javascript
$('#id').on('click', function() {
  // your code
  return false;
}
```
