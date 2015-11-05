#!/usr/bin/python
# -*- coding: utf-8 -*-

NODE_INFO_LIST_FILE = '../../node_info_list.txt'

nodeInfoList = []
myNodeInfo = {}
mySetting = None

def loadNodeInfoList():
    nodeInfoList = []
    f = open(NODE_INFO_LIST_FILE, 'r')

    # 첫번째 라인은 설명 값이므로 버린다
    line = f.readline()
    while True:
        line = f.readline()
        if not line: break
        # 마지막 문자(개행문자)는 제거
        line = line[:-1]
        s = line.split()
        nodeInfo = {}
        nodeInfo['node_id'] = s[0]
        nodeInfo['mac'] = s[1]
        nodeInfoList.append(nodeInfo)
    f.close()
    return nodeInfoList

nodeInfoList = loadNodeInfoList()

def selectTargetUsingRaspiSsid(setting, apList):
    global mySetting
    mySetting = setting

    setNodeInfo(mySetting.inst.node_id)
    raspiApList = separateRaspiAp(apList)
    targetAp = selectTargetBaseQuality(raspiApList)

    if targetAp != None:
        for nodeInfo in nodeInfoList:
            if nodeInfo['mac'] == targetAp['mac']:
                targetAp['node_id'] = nodeInfo['node_id']
                break

    return targetAp

def selectTargetBaseQuality(apList):
    if len(apList) == 0:
        return None

    sortedApList = sorted(
        apList,
        key=lambda ap: ap['quality'],
        reverse=True)

    curAp = sortedApList[0]
    for ap in sortedApList:
        # 신호 세기가 80이상이면 동일한 기준을 적용한다
        if int(ap['quality']) > curAp['quality']:
            curAp = ap

    return curAp

def separateRaspiAp(apList):
    raspiApList = []
    for ap in apList:
        tmp = ap['mac']
        mySetting.logger.info('tmp: %s' % (tmp))
        if validateAp(ap) != True:
            continue
        if ap['ssid'] == 'raspi':
            raspiApList.append(ap)
    return raspiApList

def validateAp(ap):
    if ap['mac'] == myNodeInfo['mac']:
        return False
    return True

def setNodeInfo(nodeId):
    global myNodeInfo
    myNodeInfo['node_id'] = nodeId

    mySetting.logger.info('nodeId: %s' % (nodeId))

    for nodeInfo in nodeInfoList:
        if nodeId == nodeInfo['node_id']:
            myNodeInfo['mac'] = nodeInfo['mac']
            break

def selectTargetUsingWrtnodeSsid(apList):
    return None
