# DDNS 서버 구축 및 활용

오픈소스를 활용하여 DDNS 서버를 구축하고 실제 활용해보자

## 사설 DDNS 서버 구축 및 활용

### 도메인 구입 및 DNS 정보 등록

1. 도메인 구입
 
 - [Gabia](http://www.gabia.com)에서 도메인을 구입했다. [www.yungyikim.com](http://yungyikim.com)

2. 호스트 등록

상단에서 설치한 사설 DDNS를 네임서버에 등록하기 위하여 호스트(이름)를 생성한다. 호스트 생성시 `IP 주소`에 사설 DDNS 서버의 `외부 IP`를 넣어주고 `호스트 명`은 `ns.yungyikim.com`으로 한다.

>호스트명에 서브 도메인을 `ns`로 사용하는건 암묵적 규약이다.

![enter image description here](https://lh3.googleusercontent.com/-FW_cMOILY8k/VVAv8PuhSJI/AAAAAAAAB5o/jzk4YwNvniw/s0/%E1%84%89%E1%85%B3%E1%84%8F%E1%85%B3%E1%84%85%E1%85%B5%E1%86%AB%E1%84%89%E1%85%A3%E1%86%BA+2015-05-11+13.03.23.png "스크린샷 2015-05-11 13.03.23.png")
 
3. 네임서버 등록

위에서 등록한 `호스트 명(ns.yungyikim.com)`을 네임서버로 등록한다.

![enter image description here](https://lh3.googleusercontent.com/-h-AfgxzYzRk/VVAyQayKNRI/AAAAAAAAB54/I62qtnyrb_I/s0/%E1%84%89%E1%85%B3%E1%84%8F%E1%85%B3%E1%84%85%E1%85%B5%E1%86%AB%E1%84%89%E1%85%A3%E1%86%BA+2015-05-11+13.37.53.png "스크린샷 2015-05-11 13.37.53.png")

4. 기다림

네임서버 변경 사항이 적용되는데 1일에서 7일까지 걸릴 수 있다고 하니 일단 기다림..

5. 테스트

적용이 확인되면 다음 명령어들을 이용하여 DNS 테스트가 가능하다.

```
nslookup yungyikim.com
dig yungyikim.com
```

### DDNS 서버 구축(node-dyndns)

node-dyndns는 Redis를 이용하여(기본은 아님) 도메인 정보 관리가 가능하다. Restful API를 이용하여 도메인 정보를 업데이트 한다.

node-dyndns는 아래와 같이 도메인 정보를 업데이트한다.

#### 설치

- node-dyndns는 nodejs가 설치되어 있어야 실행 가능하다. 

- [다운로드 링크](https://bitbucket.org/ntakimura/node-dyndns/overview)에서 소스를 다운로드한다.

- 외부에서 접근이 가능하도록 소스코드(lib/dnsserver.js)를 수정한다.

```javascript
if (!option.host) {
    option.host = '0.0.0.0';
}
```

#### DDNS 서버 실행

```
$ sudo ./bin/node-dyndns &
```

#### 도메인 정보 등록 및 업데이트

```
http://{DDNS 서버 외부 IP}:80/nic/update?hostname=test.yungyikim.com&myip={연결 대상 IP}
```

###테스트

#### 테스트 절차

 1. `test.yungyikim.com` 도메인에 대한 아이피 등록

	> 테스트를 진행하는 PC와 같은 로컬망에 존재하는 웹서버의 아이피(`192.168.11.19`)를 등록하여 테스트 하였다.
	
	```
http://104.xxx.xxx.xxx:80/nic/update?hostname=test.yungyikim.com&myip=192.168.11.19
	```

 2. 웹 브라우저를 통해 `test.yungyikim.com`에 연결

#### 테스트 결과

아래의 결과들은 웹 브라우저를 통해 `test.yungyikim.com`에 연결 요청을 했을때의 `DDNS 서버`의 로그와 `웹 브라우저`의 출력화면이다.

![enter image description here](https://lh3.googleusercontent.com/-GagmMuiiIH0/VVA9NbI_5lI/AAAAAAAAB7I/IFr99C2qDoE/s0/%E1%84%89%E1%85%B3%E1%84%8F%E1%85%B3%E1%84%85%E1%85%B5%E1%86%AB%E1%84%89%E1%85%A3%E1%86%BA+2015-05-11+14.17.01.png "스크린샷 2015-05-11 14.17.01.png")

![enter image description here](https://lh3.googleusercontent.com/-MVEW3dhJpaM/VVBDP7XlXiI/AAAAAAAAB7g/ke0nAmbS_ZM/s0/%E1%84%89%E1%85%B3%E1%84%8F%E1%85%B3%E1%84%85%E1%85%B5%E1%86%AB%E1%84%89%E1%85%A3%E1%86%BA+2015-05-11+14.08.15.png "스크린샷 2015-05-11 14.08.15.png")

  
