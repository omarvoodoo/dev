
# ajax 요청시 CSRF token 포함하기

django App에 정의된 rest api를 호출할 때 아래와 같은 에러를 만나는 경우가 있다. django에서 csrf 체크를 하는데 요청 패킷이 유효하지 않다는 것이다.

```sh
CSRF Failed: CSRF token missing or incorrect
```

`@csrf_exempt` 데코레이션을 주어서 무시할 수 도 있겠지만 보안이 취약해지니 좋지 않은 방법이기 때문에 아래와 같은 방법으로 ajax 헤더에 cookie에 들어 있는 `csrftoken` 값을 가져와서 넣어주는 방식으로 처리하였다.

먼저, cookie 값을 가져오는 함수이다.

```javascript
// using jQuery
function getCookie(name) {
    var cookieValue = null;
    if (document.cookie && document.cookie != '') {
        var cookies = document.cookie.split(';');
        for (var i = 0; i < cookies.length; i++) {
            var cookie = jQuery.trim(cookies[i]);
            // Does this cookie string begin with the name we want?
            if (cookie.substring(0, name.length + 1) == (name + '=')) {
                cookieValue = decodeURIComponent(cookie.substring(name.length + 1));
                break;
            }
        }
    }
    return cookieValue;
}
```

위 함수를 이용하여 쿠키값을 가져와서 아래 예제와 같이 사용하면 된다.

> POST, PUT, DELETE 요청시에 사용한다.

```javascript
jQuery.ajax({
    type: 'PUT',
    url: host+'/api/users/'+id+'/',
    dataType: 'json',
    contentType: 'application/json; charset=utf-8',
    data: JSON.stringify(data),
    beforeSend: function (xhr) {
         xhr.setRequestHeader("X-CSRFToken", getCookie('csrftoken'));
    },
    success: function(data) {
        
    },
    error: function(e) {
        
    }
});
```

## References

* [http://stackoverflow.com/questions/22291919/how-to-make-a-django-rest-framework-api-that-takes-post-data](http://stackoverflow.com/questions/22291919/how-to-make-a-django-rest-framework-api-that-takes-post-data)
* [https://docs.djangoproject.com/en/1.7/ref/contrib/csrf/#csrf-ajax](https://docs.djangoproject.com/en/1.7/ref/contrib/csrf/#csrf-ajax)
* [http://www.insilicogen.com/blog/55](http://www.insilicogen.com/blog/55)
* [http://egloos.zum.com/killins/v/3092087](http://egloos.zum.com/killins/v/3092087)
