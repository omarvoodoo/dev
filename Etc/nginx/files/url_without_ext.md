# url에 html 확장자 없이 사용하기.

```bash
server {
    listen       80;
    server_name  mysite.com;

    index index.html;
    root /var/www/mysite/public;

    location / { 
        try_files $uri $uri/ @htmlext;
    }   

    location ~ \.html$ {
        try_files $uri =404;
    }   

    location @htmlext {
        rewrite ^(.*)$ $1.html last;
    }   
}
```
