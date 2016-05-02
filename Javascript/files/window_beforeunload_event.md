# 페이지 떠날 때 경고창 띄우기

아래와 같이 페이지를 벗어나기 전에 `수정사항이 있으니 다시 한번 확인해보세요`라는 메세지를 구현하는 방법이다.

![2016-05-02 11 08 54](https://cloud.githubusercontent.com/assets/2065457/14945988/5daf0018-1056-11e6-8132-eeccb5528bbc.png)

### 실행시점

window.onBeforeUnload Event는 Window개체가 Unload되기 전에 실행됩니다. 정확하게 다음과 같은 시점이 있을 수 있습니다. 다만 IE 7, 8에서는 버그로 인해 다르게 동작하는 경우도 있으므로 참고가 필요합니다.

1. 창이나 탭을 닫을 때
1. 새로고침, 앞, 뒤로 이동버튼 클릭 시
1. location.href 변경
1. Form Submit 발생 시
1. A Tag 클릭 시
 1. HREF에 URL을 지정한 경우: 발생
 1. HREF에 #을 지정한 경우: 발생 안함
 1. HREF에 javascript:void(0)을 지정한 경우: 발생 안함   
(IE7, 8에서는 onbefureunload이벤트가 발생하는 오류가 존재합니다.)
1. Iframe 내부에서 로드된 경우 Iframe의 Src가 변경될 경우

### 예제코드

```javascript
//onBeforeUnload 이벤트 지정
$(window).bind('beforeunload', function()
{
    //페이지 변경이 있었는지 체크..
    var isChanged = ....; 

    //출력할 내용을 Return 해주면 확인 창이 뜨게 됩니다.
    if(isChanged) 
        return '변경된 사항이 있습니다. 페이지에서 나가시겠습니까?';<br>
    //확인 창을 띄우지 않으려면 아무 내용도 Return 하지 마세요!! (Null조차도)
};

//Form Submit 등 onBeforeUnload Event가 발생하길 원하지 않는 경우, 이벤트 해제
$('form').submit(function() 
{
    $(window).unbind('beforeunload'); 
});
```

### References
* [http://nsinc.tistory.com/101](http://nsinc.tistory.com/101)
