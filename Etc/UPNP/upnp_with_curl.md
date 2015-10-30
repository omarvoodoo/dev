# curl을 이용하여 UPNP 명령 처리하기

## Adventures in UPnP with cURL and netcat

### How to add NAT port forwarding with curl

```sh
curl '/Public_UPNP_C3' \
  -X 'POST' \
  -H 'Content-Type: text/xml; charset="utf-8"' \
  -H 'Connection: close' \
  -H 'SOAPAction: "urn:schemas-upnp-org:service:WANIPConnection:1#AddPortMapping"' \
  -d '<?xml version="1.0"?>
<s:Envelope
  xmlns:s="http://schemas.xmlsoap.org/soap/envelope/"
  s:encodingStyle="http://schemas.xmlsoap.org/soap/encoding/">
<s:Body>
<u:AddPortMapping
  xmlns:u="urn:schemas-upnp-org:service:WANIPConnection:1">
  <NewRemoteHost></NewRemoteHost>
  <NewExternalPort></NewExternalPort>
  <NewProtocol></NewProtocol>
  <NewInternalPort></NewInternalPort>
  <NewInternalClient></NewInternalClient>
  <NewEnabled>1</NewEnabled>
  <NewPortMappingDescription></NewPortMappingDescription>
  <NewLeaseDuration></NewLeaseDuration>
</u:AddPortMapping>
</s:Body>
</s:Envelope>'
```

### Example

```sh
# For forwarding traffic to my Raspberry PI through my Netgear Router
curl 'http://10.0.0.1:5000/Public_UPNP_C3' \
  -X 'POST' \
  -H 'Content-Type: text/xml; charset="utf-8"' \
  -H 'Connection: close' \
  -H 'SOAPAction: "urn:schemas-upnp-org:service:WANIPConnection:1#AddPortMapping"' \
  -d '<?xml version="1.0"?>
<s:Envelope xmlns:s="http://schemas.xmlsoap.org/soap/envelope/" s:encodingStyle="http://schemas.xmlsoap.org/soap/encoding/">
<s:Body>
<u:AddPortMapping xmlns:u="urn:schemas-upnp-org:service:WANIPConnection:1">
  <NewRemoteHost></NewRemoteHost>
  <NewExternalPort>443</NewExternalPort>
  <NewProtocol>TCP</NewProtocol>
  <NewInternalPort>443</NewInternalPort>
  <NewInternalClient>10.0.0.6</NewInternalClient>
  <NewEnabled>1</NewEnabled>
  <NewPortMappingDescription>node:nat:upnp</NewPortMappingDescription>
  <NewLeaseDuration>10</NewLeaseDuration>
</u:AddPortMapping>
</s:Body>
</s:Envelope>'
```

## References

- [Adventures in UPnP with cURL and netcat](https://coolaj86.com/articles/adventures-in-upnp-with-curl-and-netcat/)
