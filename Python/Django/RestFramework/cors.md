#CORS(Cross-Origin Resource Sharing)

> “No 'Access-Control-Allow-Origin' header is present on the requested resource” in django

최근 브라우저들은 보안상의 이유로 ajax로 요청하는 도메인과 요청받는 도메인이 다를 경우 브라우저 개발 콘솔창에서 아래와 같은 에러메세지를 확인 할 수있다.

```
"No 'Access-Control-Allow-Origin' header is present on the requested resource.
Origin 'null' is therefore not allowed access. "
```

Django에서 위와 같은 문제를 해결하기 위해서는 몇가지 설정을 추가해야 한다.

1. `manage.py` 파일이 있는 경로에서 다음 스크립트를 실행하여 `django-cors-headers` 패키지를 설치한다.

  ```
  pip install django-cors-headers
  ```
  
2. 아래 코드를 참고하여 `settings.py` 파일을 편집한다.

  ```
  INSTALLED_APPS = (
      ...
      'corsheaders',
      ...
  )
  
  MIDDLEWARE_CLASSES = (
      ...
      'corsheaders.middleware.CorsMiddleware',
      'django.middleware.common.CommonMiddleware',
      ...
  )
  
  CORS_ORIGIN_ALLOW_ALL = True 
  ```
  
##References
* [http://stackoverflow.com/questions/22476273/no-access-control-allow-origin-header-is-present-on-the-requested-resource-i](http://stackoverflow.com/questions/22476273/no-access-control-allow-origin-header-is-present-on-the-requested-resource-i)
* [https://github.com/ottoyiu/django-cors-headers](https://github.com/ottoyiu/django-cors-headers)
