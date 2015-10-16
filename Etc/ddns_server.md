# DDNS 서버 구축하기 및 활용

오픈소스를 활용하여 DDNS 서버를 구축하고 실제 활용해보자

## 사설 DDNS 서버 구축

### node-dyndns

node-dyndns는 Redis를 이용하여(기본은 아님) 도메인 정보 관리가 가능하다. Restful API를 이용하여 도메인 정보를 업데이트 한다.

node-dyndns는 아래와 같이 도메인 정보를 업데이트한다.

```
http://{사설 DDNS 서버 IP}:80/nic/update?hostname=test.yungyikim.com&myip=192.168.11.20
```

[다운로드 링크](https://bitbucket.org/ntakimura/node-dyndns/overview)

## DDNS 서버 활용하기

아래 내용은 DDNS 서버를 활용하기 위한 작업 내용에 대한 기록이다. 사설 DDNS 서버의 IP 주소는 211.11.11.11라고 임의로

1. 도메인 구입
 
 - [Gabia](http://www.gabia.com)에서 도메인을 구입했다. [www.yungyikim.com](http://yungyikim.com)

2. 호스트 등록

 - Gabia 메뉴를 통해 호스트를 등록한다.
 - ns.yungyikim.com - {사설 DDNS 서버 IP}
 
3. yungyikim.com의 네임서버에 해당 호스트(ns.yungyikim.com)을 등록

4. 네임서버 변경 사항이 적용되는데 1일에서 7일까지 걸릴 수 있다고 하니 일단 기다림..

5. 적용이 확인되면 DNS 서비스 테스트

 - nslookup yungyikim.com
 - dig yungyikim.com

![ddns_server_00](ddns_server_00.png)
  
