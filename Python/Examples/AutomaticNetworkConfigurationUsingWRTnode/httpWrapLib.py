#!/usr/bin/python
# -*- coding: utf-8 -*-

import httplib
import socket

# 서버 API 호출
#   method: GET 또는 POST
#   params: POST일때만 유효함
def send_request(host, port, method, url, params):
    socket.setdefaulttimeout(10)

    conn = httplib.HTTPConnection(host+":"+str(port))

    try:
        if method == "POST":
            headers = {"Content-type": "application/json", "Accept": "*/*"}
            conn.request(method, url, params, headers)
        else:
            conn.request(method, url)
    except Exception as e:
        print (type(e)), host, port, method, url, params
        pass

    response = conn.getresponse()
    data = response.read()
    conn.close()
    return data
