

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
