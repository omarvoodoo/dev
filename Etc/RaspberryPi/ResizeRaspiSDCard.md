# SDCard 이미지 사이즈를 줄여서 백업하기 on OSX

Raspi로 개발할때는 SDCard가 16GB였는데 실제 배포할때는 8GB로 줄여야 하는 상황이 발생하였다. 어차피 16GB 공간중에서 대부분이 비어 있으므로 이미지 사이즈를 줄이는 방법을 택했다.

#### 디스크 정보 확인

`diskutil`을 이용하여 sdcard 정보를 확인하자

```sh
$ diskutil list
```

![resultimage](https://cloud.githubusercontent.com/assets/2065457/11632742/9f2c7d50-9d4b-11e5-8ab7-cec8ababa9f2.png)

#### `e2fsprogs` 설치

```sh
$ brew install e2fsprogs
```

설치가 끝나면 저장 위치가 표시된다.

#### Resize SDCard

`e2fsprogs`가 설치된 경로로 이동하여 명령어를 실행 해보자. 아래와 같은 순서로 명령어를 실행하자

```sh
$ sudo ./e2fsck -f /dev/disk2s2
```

```sh
$ sudo ./resize2fs /dev/disk2s2 7G
```

> SDCard가 8GB 크기인데 8GB로 사이즈를 조절하면 실제 크기는 8GB가 초과하기 때문에 7GB로 사이즈를 조정하였다.

#### 이미지로 추출하기

이제 Resize 완료된 SDCard를 .img 파일로 백업 해보자.

> `Win32 Disk Imager`나 `ApplePi-Baker`를 이용하여 백업할 경우 원본 SDCard(16GB) 크기대로 백업이 되버리기 때문에 아래의 명령어를 이용하여 백업을 한다

```sh
$ sudo dd if=/dev/disk2 of=/Users/user1/raspi-backup.img bs=1m count=7000
```

> count 값은 SDCard용량에 비례하여 적어주면 된다. 7GB --> 7000, 4GB --> 4000
