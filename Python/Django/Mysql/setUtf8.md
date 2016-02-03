#Django에서 한글 데이터를 MySQL에 저장하기

Django를 통해 전달되는 한글 형태의 데이터를 MySQL에 저장하기 위한 설정을 해보자. 만약, 해당 설정이 안된 상태에서 한글 형태의 데이터를 저장하게 되면 아래와 같은 메세지를 보게 될 것이고 MySQL을 확인해 보면 한글은 전부 깨져있을 것이다.

```
Warning: Incorrect string value: '\xED\x95\xB4\xEB\xA5\x9C' for column 'firstname' at row 1
```

##MySQL 설정

###`my.cnf`를 아래와 같이 편집한다.

> 이미 사용할 테이블이 생성된 상태라면 삭제 후 진행하는 것을 추천한다. 설정 전에 생성된 테이블은 적용 안될 가능성이 있다.

####[client] 부분에 추가

```
default-character-set = utf8
```

####[mysqld] 부분에 추가

```
init_connect = SET collation_connection = utf_general_ci   
init_connect = SET NAMES utf8   
character-set-server = utf8   
collation-server = utf8_general_ci   
```

####[mysqldump] 부분에 추가

```
default-character-set = utf8
```

###MySQL 재시작

```
sudo service mysql restart
```

##Django 설정

### `settings.py`를 다음처럼 편집한다.

```python
DATABASES = {
    'default': {
        'ENGINE': 'django.db.backends.mysql', # Add 'postgresql_psycopg2', 'postgresql', 'mysql', 'sqlite3' or 'oracle'.
        'NAME': '프로젝트명',                      # Or path to database file if using sqlite3.
        'USER': 'DB사용자',                      # Not used with sqlite3.
        'PASSWORD': 'DB패스워드',                  # Not used with sqlite3.
        'HOST': 'localhost',                      # Set to empty string for localhost. Not used with sqlite3.
        'PORT': '3306',                      # Set to empty string for default. Not used with sqlite3.
    }
}
DATABASE_OPTIONS = {'charset': 'utf8'}
TIME_ZONE = 'Asia/Seoul'
LANGUAGE_CODE = 'ko-kr'
```

## References

* [egloos.zum.com/mcchae/](http://egloos.zum.com/mcchae/v/10625376)
* [nicesea.tistory.com/](http://nicesea.tistory.com/entry/MySQL-%ED%95%9C%EA%B8%80-%EC%A0%80%EC%9E%A5-%EC%84%A4%EC%A0%95)
