## Django 테스트 수행시 발생하는 `Access denied` 에러 대처하기

#### `Access denied` 에러 발생

아래와 같이Django testing을 수행 했을때 Access denied 에러가 발생함

```bash
$ python manage.py test 
Creating test database for alias 'default'...
Got an error creating the test database: (1044, "Access denied for user 'wisdome'@'localhost' to database 'test_webapp'")
Type 'yes' if you would like to try deleting the test database 'test_webapp', or 'no' to cancel: yes
Destroying old test database for alias 'default'...
Got an error recreating the test database: (1044, "Access denied for user 'wisdome'@'localhost' to database 'test_webapp'")
```

#### 해결방법

테스트시에 임시로 생성되는 `test_webapp`이라는 데이터베이스에 대한 접근 권한 문제가 발생하는 것이므로 Grant 명령어를 이용하여 접근 권한을 할당해주자.

```bash
mysql> use mysql;
Reading table information for completion of table and column names
You can turn off this feature to get a quicker startup with -A

Database changed
mysql> GRANT ALL PRIVILEGES ON test_webapp.* TO wisdome@localhost IDENTIFIED BY 'dkssudgktpdy';
Query OK, 0 rows affected, 1 warning (0.00 sec)

```

## References
* [stackoverflow.com](http://stackoverflow.com/questions/11170133/using-mysql-with-django-access-denied-for-user-localhost)
