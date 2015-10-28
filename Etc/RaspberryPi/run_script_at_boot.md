# Run script at boot(라즈베리파이에서 부팅시 사용자 정의 스크립트 실행하기)

## 스크립트 파일 생성

아래와 같은 형식의 파일을 `/etc/init.d/`에 만든다.

> /etc/init.d/S99myscript

```sh
#! /bin/sh
### BEGIN INIT INFO
# Provides:          <your script name>
# Required-Start:    $all
# Required-Stop:
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: Manage my cool stuff
### END INIT INFO

PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/opt/bin

. /lib/init/vars.sh
. /lib/lsb/init-functions
# If you need to source some other scripts, do it here

case "$1" in
  start)
    log_begin_msg "Starting my super cool service"
# do something
# 실행 스크립트를 정의한다.
    sudo sh /home/pi/my_script.sh &
    log_end_msg $?
    exit 0
    ;;
  stop)
    log_begin_msg "Stopping the coolest service ever unfortunately"

    # do something to kill the service or cleanup or nothing

    log_end_msg $?
    exit 0
    ;;
  *)
    echo "Usage: /etc/init.d/<your script> {start|stop}"
    exit 1
    ;;
esac
```

## 시작 프로그램에 등록

### 위에서 생성한 파일을 실행 가능한 형식으로 변경한다

```sh
sudo chmod 755 /etc/init.d/S99myscript
```

### 스크립트가 부팅시에 시작될 수 있도록 등록한다

```sh
sudo update-rc.d /etc/init.d/S99myscript default
```
