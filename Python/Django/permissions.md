# Permissions

Django는 Json 기반의 API 인증(Authentication) 인가(Authorization)을 프레임웍 자체에서 제공한다.

```python
# Example
REST_FRAMEWORK = {
    'DEFAULT_PERMISSION_CLASSES': ('rest_framework.permissions.IsAdminUser',),
}
```

[http://django-rest-framework.org/api-guide/permissions](http://django-rest-framework.org/api-guide/permissions)

아래와 같이 다양하게 권한을 설정 할 수있다.

* 모든 사용자에게 access를 허가하는 Allow Any
* 인증된 사용자에게만 access를 허가하는 isAuthenticated
* 관리자에게만 인증을 허가하는 isAdminUser
* 인증되지 않은 사용자에게는 Read,인증된 사용자에게는 Update를 제공하는 isAuthenticatedOrReadOnly
* 기타 DjangoModel (django.contrib.auth에 의해서 관리되는 권한)이나 Object 단위 권한등에 따라서, 조정할 수 있다.
* 그리고 TokenHasReadWriteScope의 경우에는 OAuth2Authentication등과 연계하여 Token 기반의 인증이 가능하게 된다.

※ 다양한 인증 메커니즘을 제공하기 때문에, 처음에는 어려울지 몰라도, 이해하고 나면 짜는 것 보다는 훨씬 쉬울듯.
위는 API접근 권한에 대한 것이었고, Authentication은 http://django-rest-framework.org/api-guide/authentication 에 정의되어 있다. 위에서 사용한 것과 같이 HTTP Basic Auth를 사용할 수 도 있고, Token이나 Http Session 기반의 Authentication 그리고, OAuth 1,2 기반의 인증을 모두 지원한다. (Token 방식의 경우에는 user가 authenticated된 다음, Authorization token을 서버에서 발급해준다. Token이 HTTP를 타고 내려가기 때문에, Token을 secure하게 전달하기 위해서 HTTPS를 필수로 사용해야 한다.)

출처 : [조대협의 블로그](http://bcho.tistory.com/845)
