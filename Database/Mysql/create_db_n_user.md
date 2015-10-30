## 데이터베이스 생성하기

```sql
Welcome to the MariaDB monitor.  Commands end with ; or \g.
Your MariaDB connection id is 3
Server version: 5.5.30-MariaDB-log Source distribution
 
Copyright (c) 2000, 2013, Oracle, Monty Program Ab and others.
 
Type 'help;' or '\h' for help. Type '\c' to clear the current input statement.
 
MariaDB [(none)]> use mysql
Database changed
 
MariaDB [mysql]> create database orademo;
Query OK, 1 row affected (0.00 sec)
```

## 사용자 추가 / 권한부여

```sql
-- User를 생성하고 권한을 부여합니다.
MariaDB [orademo]> create user 'ecsees'@'localhost' identified by 'xxxxx';
Query OK, 0 rows affected (0.00 sec)
 
-- 혹시라도 외부에서 접속하려면 localhost가 아닌 '%'로 추가 생성하여야 합니다.
MariaDB [orademo]> grant all privileges on orademo.* to ecsees@localhost;
Query OK, 0 rows affected (0.00 sec)
 
-- 확인합니다.
MariaDB [orademo]> flush privileges;
Query OK, 0 rows affected (0.00 sec)
```

## References

- [Ora-SYSDBA](http://ora-sysdba.tistory.com/43)
