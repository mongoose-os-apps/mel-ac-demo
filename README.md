# MEL-AC Demo App

Mitsubishi Electic AirCo and HeapPump control using Mongoose OS [MEL-AC](https://github.com/mongoose-os-libs/mel-ac) library

## Install

* Set your wifi credentials and debug config (if required) and `"mel_ac.uart_no"` in file
```
mos.yml 
```
* Build with 
```
$ mos build
```
* Flash with 
```
$ mos flash
```

## Read current params from HVAC

Reading is performed by ***Mongoose OS*** RPC. Currently enabled HTTP and WS, but MQTT or any other RPC channel could be added

### WebSockets

```javascript
$ mos call --port ws://192.168.1.216/rpc MEL-AC.GetParams
{
"connected": true,
"power": 0,
"mode": 3,
"setpoint": 21.0,
"fan": 3,
"vane_vert": 0,
"vane_horiz": 3,
"isee": true,
"operating": false,
"room": 25.0
}
```
### HTTP

```javascript
http://192.168.1.216/rpc/MEL-AC.GetParams
{"connected": true, "power": 0, "mode": 1, "setpoint": 20.0, "fan": 2, "vane_vert": 0, "vane_horiz": 3, "isee": true, "operating": false, "room": 22.0}
```

## Set new params to HVAC

### WebSockets

```javascript
$ mos call --port ws://192.168.1.216/rpc MEL-AC.SetParams '{"mode":1,"fan":2,"setpoint":20}'
{
  "success": true,
  "power": 0,
  "mode": 1,
  "setpoint": 20.0,
  "fan": 2,
  "vane_vert": 0,
  "vane_horiz": 3
}
```
### HTTP

```javascript
$ curl -d '{"mode":1,"fan":2,"setpoint":20}' 192.168.1.216/rpc/MEL-AC.SetParams
{"success": true, "power": 0, "mode": 1, "setpoint": 20.0, "fan": 2, "vane_vert": 0, "vane_horiz": 3} 
```
