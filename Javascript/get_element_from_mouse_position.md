# 마우스로 클릭한 위치의 엘레멘트 객체 얻기

```javascript
$(window).click(function(e) {
    var x = e.clientX, y = e.clientY,
        elementMouseIsOver = document.elementFromPoint(x, y);
    
    alert(elementMouseIsOver);
});
```
