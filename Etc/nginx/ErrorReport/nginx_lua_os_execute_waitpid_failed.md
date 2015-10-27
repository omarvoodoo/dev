
# nginx lua: `os.execute waitpid()` failed (10: No child processes)

Nginx에 [lua-nginx-module](https://github.com/openresty/lua-nginx-module)을 이용하여 시스템을 명령을 처리하는 작업을 하는 중 `os.execute()` 명령만 실행하면 Nginx Work Process가 내려갔다 올라오는 것을 확인했다. 원인을 모르겠고 다음과 같이 [lua-resty-shell](https://github.com/juce/lua-resty-shell)을 이용하는 방법으로 문제를 회피하였다.

## old

```lua
os.execute('uname -a')
```

## new

lua-resty-shell을 사용하려면 [sockproc](https://github.com/juce/sockproc)이 먼저 실행 되어야 한다.

```c
$ ./sockproc /tmp/shell.sock
```

```lua
local shell = require('resty.shell')
local status, out, err = shell.execute('uname -a')
```
