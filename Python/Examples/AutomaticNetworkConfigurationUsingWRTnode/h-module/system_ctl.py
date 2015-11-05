#!/usr/bin/python
# -*- coding: utf-8 -*-

import os
import sys
import StringIO

# 상위 디렉토리에 존재하는 사용자 라이브러리를 사용하기 위함
path = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
if not path in sys.path:
    sys.path.insert(1, path)
del path

CONFIG_FILE_PATH = '/etc/config/wireless'

# WRTnode 설정 파일 상에 ssid 값을 수정한다.
def modifySsidOnConf(ssid, targetSsid):
    f = open(CONFIG_FILE_PATH, 'r')
    output = StringIO.StringIO()
    while True:
        line = f.readline()
        if not line: break

        m = re.search('ApCliSsid', line)
        if m is None:
            mm = re.search('option ssid', line)
            if mm is None:
                output.write(line)
            else:
                output.write('\toption ssid \"' + ssid + '\"\n')
        else:
            output.write('\toption ApCliSsid \"' + targetSsid + '\"\n')
    f.close()
    f = open(CONFIG_FILE_PATH, 'w')
    f.write(output.getvalue())
    f.close()
    output.close()

def wifiRestart():
    f = os.popen('wifi reload')
    output = f.read()
    f.close()

def changeSsid(ssid):
    os.system("iwpriv ra0 set SSID=" + ssid)
    os.system("iwpriv ra0 set WPAPSK=12345678")

# dmesg를 확인하여 연결 성공 여부 체크
# wifi restart 후 3초간 체크하여 'RTMP_TimerListAdd: add timer obj' 메세지를
# 발견 못한다면 연결 실패로 간주한다
# + 0.2초 간격으로 3초간 체크함
def checkConnectionStatus():
    i = 1
    while True:
        f = os.popen('dmesg | tail -f')
        output = f.read()
        m = re.search('RTMP_TimerListAdd: add timer obj', output)
        if m != None:
            return 1
        if (i % 15) == 0:
            break
        i = i + 1
        time.sleep(0.2)
    return 0

# 시스템로그를 확인하여 인터페이스가 정상적으로 올라왔는지 확인하고 비정상일 경우
# 정상화 될때까지 네트워크 서비스를 재시작한다
def checkInterfaces():
    checkInterfaceApcli0()
    checkInterfaceRa0()

def checkInterfaceApcli0():
    while True:
        f = os.popen('dmesg | grep apcli0')
        result = f.read()
        f.close()

        if len(result) < 30:
            f = os.popen('/etc/init.d/network restart')
            result = f.read()
            f.close()
            time.sleep(5)
        else:
            break

def checkInterfaceRa0():
    while True:
        f = os.popen('dmesg | grep ra0')
        result = f.read()
        f.close()

        if len(result) < 30:
            f = os.popen('/etc/init.d/network restart')
            result = f.read()
            f.close()
            time.sleep(5)
        else:
            break
