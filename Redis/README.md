
## Data Types

### Strings

### Lists

### Sets

set은 string의 집합이다. 여러개의 값을 하나의 Value 내에 넣을 수 있다고 생각하면 되며 블로그 포스트의 태깅(Tag)등에 사용될 수 있다. 재미있는 점은 set간의 연산을 지원하는데, 집합인 만큼 교집합, 합집합, 차이(Differences)를 매우 빠른 시간내에 추출할 수 있다.

### Hashes

### Sorted sets

set 에 "score" 라는 필드가 추가된 데이타 형으로 score는 일종의 "가중치" 정도로 생각하면 된다. sorted set에서 데이타는 오름 차순으로 내부 정렬되며, 정렬이 되어 있는 만큼 score 값 범위에 따른 쿼리(range query), top rank에 따른 query 등이 가능하다.

### References

- [In memory dictionary Redis 소개](http://bcho.tistory.com/654)
- [Joinc](http://www.joinc.co.kr/modules/moniwiki/wiki.php/man/12/REDIS/IntroDataType)

## Benchmark

```bash
redis-benchmark -n 1000000 -r 1000000 -q -P 32 zadd zset __rand_int__ __rand_int__
```

### References

- [different-changes-to-redis-benchmark](http://grokbase.com/t/gg/redis-db/1388j6whz7/different-changes-to-redis-benchmark)
- [The Little Known Secret of Redis](http://nosql.mypopescu.com/post/16061147078/the-little-known-secret-of-redis)
- [NoSQL - Redis 통계정보, 성능 측정](http://develop.sunshiny.co.kr/1003)

## Tools

### [Redis Desktop Manager](http://redisdesktop.com/)

Redis Desktop Manager (aka RDM)— is a cross-platform open source Redis DB management tool
