# CSRF verification failed. Request aborted.

> Forbidden (403)
>
> CSRF verification failed. Request aborted.

django 1.2부터 POST로 요청을 하면 위와 같은 에러가 나오는 경우가 있는데, CSRF 보안 때문이다. 이 문제를 해결하기 위한 두가지 방법이 있다.

1. POST form뒤에 `{% csrf_token %}`을 넣어준다.   
```html
<form action="" method="post">{% csrf_token %}
```
2. CSRF 보안 처리가 안된 Python 코드 안에 `@csrf_exempt`로 데코레이션하면 된다. 

 > `@csrf_exempt`는 CSRF를 무시한다.
 
```python
form django.views.decorators.csrf import csrf_exempt
...
@csrf_exempt
def foo:
```

## References
* [http://givenjazz.com/39](http://givenjazz.com/39)
* [Cross Site Request Forgery protection](https://docs.djangoproject.com/en/dev/ref/csrf/)
