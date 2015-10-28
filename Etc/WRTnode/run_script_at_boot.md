# Run script at boot(WRTnode에서 부팅시 사용자 정의 스크립트 실행하기)

## 스크립트 파일 생성

아래와 같은 형식의 파일을 `/etc/rc.d/`에 만든다.

> /etc/rc.d/S99myscript

```sh
#!/bin/sh /etc/rc.common
# Copyright (C) 2013 OpenWrt.org

sh my_script.sh &
```

### 위에서 생성한 파일을 실행 가능한 형식으로 변경한다

```sh
chmod 755 /etc/rc.d/S99myscript
```
