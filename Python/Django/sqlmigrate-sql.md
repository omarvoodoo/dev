# migration 실행 후 수행된 SQL 스크립트 보기

```sh
$ python manage.py makemigrations {App 이름}
```

Question과 Choice 모델이 정의되어 있고 위와 같이 실행하게 되면 아래와 같은 형태의 결과를 얻게 된다.

```sh
Migrations for '{App 이름}':
  0001_initial.py:
    - Create model Question
    - Create model Choice
    - Add field question to choice
```

0001_initial.py라는 파일이 생성 되었음을 확인할 수 있고 아래와 같이 실행하게 되면 수행된 SQL 스크립트가 출력된다.

```sh
$ python manage.py sqlmigrate {App이름} 0001
```

```sql
BEGIN;
CREATE TABLE {App이름}_question (
    "id" serial NOT NULL PRIMARY KEY,
    "question_text" varchar(200) NOT NULL,
    "pub_date" timestamp with time zone NOT NULL
);

CREATE TABLE {App이름}_choice (
    "id" serial NOT NULL PRIMARY KEY,
    "question_id" integer NOT NULL,
    "choice_text" varchar(200) NOT NULL,
    "votes" integer NOT NULL
);

CREATE INDEX {App이름}_choice_7aa0f6ee ON "{App이름}_choice" ("question_id");

ALTER TABLE "{App이름}_choice"
  ADD CONSTRAINT polls_choice_question_id_246c99a640fbbd72_fk_{App이름}_question_id
    FOREIGN KEY ("question_id")
    REFERENCES "{App이름}_question" ("id")
    DEFERRABLE INITIALLY DEFERRED;
COMMIT;
```
