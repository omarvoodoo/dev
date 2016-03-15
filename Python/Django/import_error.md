django와 기타 모듈들을 설치 했지만 아래처럼 ImportError가 발생한다면 

```bash
kimui-MacBook-Pro:app kimyungyi$ python manage.py runserver
Traceback (most recent call last):
  File "manage.py", line 8, in <module>
    from django.core.management import execute_from_command_line
ImportError: No module named django.core.management
```

PYTHONPATH에 실제 파이썬 패키지들이 설치된 경로를 지정해주자. 아래의 예제는 맥상에서 파이썬 패키지 경로를 export하는 코드이다.

```bash
export PYTHONPATH="/usr/local/lib/python2.7/site-packages/:$PYTHONPATH"
```
