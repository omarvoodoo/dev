#!/usr/bin/python
# -*- coding: utf-8 -*-

import os
import sys
import re
import json
import time
import threading
import StringIO
import select, socket

# 상위 디렉토리에 존재하는 사용자 라이브러리를 사용하기 위함
path = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
if not path in sys.path:
    sys.path.insert(1, path)
del path

import httpWrapLib
import setting
import target
import system_ctrl

# 스레드 종료 표시자
stopper = 0

# 연결 상태 표시자
# 1 : 연결됨
# 0 : 연결 안됨
conn_status = 0

BROADCAST_IP = '172.16.255.255'
BROADCAST_PORT = 53005

def main():
    global stopper
    global conn_status

    #system_ctrl.checkInterfaces()
    #startReceiver()
    #startSender()

    # Ctrl+C Handling
    try:
        conn_status = connectUsingRaspiSsid()

        while True:
            if conn_status == 0:
                conn_status = connectUsingWrtnodeSsid()

            if conn_status == 1:
                a = 0
                # 연결 상태를 체크하는 로직을 추가해야함

            time.sleep(1)
    except KeyboardInterrupt:
        # 스레드 중지
        stopper = 1
        pass

def connectUsingWrtnodeSsid():
    apList = getApList()

    status = 0

    return status

def connectUsingRaspiSsid():
    apList = getApList()
    targetAp = target.selectTargetUsingRaspiSsid(
        setting,
        apList)

    status = 0
    if targetAp != None:
        setting.logger.info('targetAp[ssid]: %s' % (targetAp['ssid']))
        setting.logger.info('targetAp[node_id]: %s' % (targetAp['node_id']))
        status = connectNode(targetAp['node_id'])

    return status

def connectNode(targetNodeId):
    wrtnodeSsid = makeWrtnodeSsid(nodeId, targetNodeId)
    wrtnodeTargetSsid = makeWrtnodeTargetSsid(targetNodeId)

    setting.logger.info('WRTnodeTargetSsid:%s' % wrtnodeTargetSsid)

    system_ctrl.modifySsidOnConf(wrtnodeSsid, wrtnodeTargetSsid)
    system_ctrl.wifiRestart()
    status = system_ctrl.checkConnectionStatus()
    if status == 1:
        system_ctrl.changeSsid(wrtnodeSsid)

    return status

def makeWrtnodeTargetSsid(targetNodeId):
    return 'WRTnode_Tail_%s' % targetNodeId

def makeWrtnodeSsid(nodeId, targetNodeId):
    return 'WRTnode_%s_%s' % (nodeId, targetNodeId)

def getApList():
    jsonAps = getAps()
    return parseAps(jsonAps)

def parseAps(jsonAps):
    apList = []
    if jsonAps != None:
        aps = json.loads(jsonAps)
        for key in aps.keys():
            apList.append(aps[key])

    # dummy
    ap = {}
    ap['ssid'] = 'raspi'
    ap['mac'] = '90:9f:33:e8:c0:4e'
    ap['quality'] = '90'
    apList.append(ap)

    setting.logger.info('AP list ---------------------')
    for ap in apList:
        setting.logger.info(ap['ssid'])
    setting.logger.info('-----------------------------')

    return apList

def getAps():
    setting.logger.info('serverIP: %s, serverPort: %s' \
        % (setting.inst.getServerIp(), setting.inst.getServerPort()))
    uri = '/1.0/ap/info'
    data = ''
    jsonData = request(
        setting.inst.getServerIp(),
        setting.inst.getServerPort(),
        uri,
        data)
    return jsonData

def request(serverIp, serverPort, uri, data):
    response_data = None

    try:
        response_data = \
            httpWrapLib.send_request(
                serverIp,
                serverPort,
                "GET",
                uri,
                data)
    except Exception as e:
        setting.logger.info('error:%s request-info: %s %s %s %s' \
            % (type(e), serverIp, serverPort, uri, data))

    if response_data == None:
        setting.logger.info('response_data is None')
    else:
        setting.logger.info(response_data)

    return response_data

def startSender(nodeId):
    # 메세지 송신 스레드
    th = threading.Thread(target=runSender, args=(nodeId,))
    th.start()

def runSender(nodeId):
    i = 1
    while True:
        if stopper > 0:
            break

        if (i % 10) == 0:
            # 초당 1회 checkalive 메세지 송신
            sendMsg(nodeId)
            i = 0

        i = i + 1
        time.sleep(0.1)

    setting.logger.info('runSender() Stopped')

def sendMsg(nodeId):
    # 초당 1회 checkalive 메세지 송신
    conn_info = {}
    conn_info['node_id'] = nodeId

    msg = {}
    msg['type'] = 'conn_info'
    msg['msg'] = conn_info

    jsonMsg = json.dumps(msg)
    sendPacketWithBroadcast(jsonMsg)

def sendPacketWithBroadcast(msg):
    s = socket(AF_INET, SOCK_DGRAM)
    s.bind(('', 0))
    s.setsockopt(SOL_SOCKET, SO_BROADCAST, 1)
    s.sendto(msg, (BROADCAST_IP, BROADCAST_PORT))

def startConnectionStatusChecker(targetNodeId):
    th = threading.Thread(
        target=runConnectionStatusChecker,
        args=(targetNodeId,))
    th.start()
    th.join()

def runConnectionStatusChecker(targetNodeId):
    timeout = 60
    t = time.time()
    while True:
        if stopper > 0: break
        new_t = time.time()
        if (new_t - t) >= timeout:
            setting.logger.info('Timeout!')
            break

        time.sleep(0.5)

def startReceiver():
    # 메세지 수신 스레드
    th = threading.Thread(target=runReceiver)
    th.start()

def runReceiver():
    bufferSize = 1024

    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.bind((BROADCAST_IP, BROADCAST_PORT))
    s.setblocking(0)

    while True:
        if stopper > 0:
            break

        result = select.select([s],[],[])
        parseReceiveMsg(result[0][0].recv(bufferSize))

    setting.logger.info('runReceiver() Stopped')

def parseReceiveMsg(jsonMsg):
    setting.logger.info(jsonMsg)
    # msg 형식
    # {"type": "conn_info"}, {"msg": "message"}
    msg = json.loads(jsonMsg)
    if msg['type'] == 'conn_info':
        connInfo = msg['msg']

# 직접 파일을 실행 시켰을때만 수행된다.
if __name__ == "__main__":
    main()
