# TCP algorithms

## Nagle's algorithm (sender)

* If there is unacknowledged data, then the sending TCP buffers all user data (regardless of the PSH bit), until the outstanding data has been acknowledged or until the TCP can send a full-sized segment (Eff.snd.MSS bytes). (https://www.ietf.org/rfc/rfc1122.txt)

```shell script
./server 55555 1024 1024 1024 1024 5579 79 0 1
./client 55555 256 256 256 256 79 5579 0 1

12:35:12.204341 52746 > 55555: length 256
12:35:12.204346 55555 > 52746: ack 257, win 510, length 0
12:35:12.204368 52746 > 55555: length 256
12:35:12.204379 55555 > 52746: ack 513, win 508, length 0
12:35:12.204388 52746 > 55555: length 256
12:35:12.204416 55555 > 52746: ack 769, win 506, length 0
12:35:12.204421 52746 > 55555: length 768
12:35:12.246946 55555 > 52746: ack 1537, win 500, length 0
12:35:12.246964 52746 > 55555: length 4043
12:35:12.247017 55555 > 52746: ack 5580, win 469, length 79
```

## ACK delays (receiver)

* Receiver waits for a packet on which the ACK can be piggybacked, if not getting it, sends the ACK delayed (up to 500 ms).
 Timer is reset after every recv.

## TCP window (receiver)

```
RCV.NXT right edge
     |  |
   _______ RCV.BUFF
12|34|56|7|89 - not available
|  |  |  |
|  |  |  available but not yet advertised
|  |  advertised to sender (RCV.WND)
|  received but not yet consumed (RCV.USER)
received and consumed
```

* Receiver avoids advancing the right window edge in small increments (SWS avoidance).
It still ACK-s the received data, such that the sender knows not to retransmit it.

* Receiver keeps `RCV.NXT+RCV.WND` fixed until:
```
RCV.BUFF - RCV.USER - RCV.WND >= min( Fr * RCV.BUFF, Eff.snd.MSS )
```

* When the inequality is satisfied, `RCV.WND` is set to `RCV.BUFF-RCV.USER`.
 (https://www.ietf.org/rfc/rfc1122.txt)

# Issues

## SO_RCVBUF smaller than MSS

* If receiver does `connect()`, or `accept()`, before calling `setsockopt(SO_RCVBUF)` to set SO_RCVBUF smaller than stream MSS, and smaller than maximum advertised window, then sender waits 200ms after receiving ACK before transmitting more payload ***when receiver transmits small window***.

> The sender attempts to efficiently use network bandwidth through minimizing header overhead by waiting extra time (200ms) for the receiver to return an advertised window at least as large as the MSS or as large as the prior seen maximum advertised window.

> If a receiver application connects, or accepts, connections transmitting a large initial receive buffer, then the initial advertised window will be large. If the application later calls setsockopt(SO_RCVBUF) with a value smaller than the connection's MSS (and thus also smaller than the initial advertised window) then the sender assumes the client will be returning to an advertised window above MSS and should wait to avoid congesting the network with needless traffic when the receiver is experiencing memory pressure for some unknown reason. (https://access.redhat.com/solutions/6481061)

```shell script
./server 55555 2048 2048 2048 2048 5579 79 0 1
./client 55555 256 5579 256 5579 79 5579 0 100

18:09:27.946953 56346 > 55555: seq 61370:66234, ack 870, win 512, length 4864
18:09:27.946964 55555 > 56346: ack 66234, win 0, length 0
18:09:27.947012 55555 > 56346: ack 66234, win 16, length 0
18:09:27.947019 56346 > 55555: seq 66234:66949, ack 870, win 512, length 715
18:09:27.947033 55555 > 56346: ack 66949, win 16, length 0
18:09:27.947052 55555 > 56346: seq 870:949, ack 66949, win 16, length 79
18:09:27.947056 56346 > 55555: ack 949, win 512, length 0
-- 200 ms --
18:09:28.154944 56346 > 55555: seq 66949:68997, ack 949, win 512, length 2048
18:09:28.154952 55555 > 56346: ack 68997, win 0, length 0
18:09:28.154958 55555 > 56346: ack 68997, win 16, length 0
-- 200 ms --
18:09:28.366939 56346 > 55555: seq 68997:71045, ack 949, win 512, length 2048
18:09:28.366947 55555 > 56346: ack 71045, win 0, length 0
18:09:28.366961 55555 > 56346: ack 71045, win 16, length 0
-- 200 ms --
18:09:28.366966 56346 > 55555: seq 71045:72528, ack 949, win 512, length 1483
18:09:28.366998 55555 > 56346: ack 72528, win 16, length 0
18:09:28.367014 55555 > 56346: seq 949:1028, ack 72528, win 16, length 79
18:09:28.367018 56346 > 55555: ack 1028, win 512, length 0
```

* Receiver keeps descreasing the TCP window because it keeps the right edge fixed until the inequality is satisfied.
Since `RCV.BUFF < RCV.WND`, the inequality will only satisfy when `RCV.WND` reaches 0.

```shell script
./server 55555 2048 2048 2048 2048 5579 79 0 1
./client 55555 256 256 256 256 79 5579 0 100

17:34:57.667398 56248 > 55555: seq 61370:61626, ack 870, win 512, length 256
17:34:57.667423 55555 > 56248: ack 61626, win 37, length 0
17:34:57.667428 56248 > 55555: seq 61626:62394, ack 870, win 512, length 768
17:34:57.667441 55555 > 56248: ack 62394, win 31, length 0
17:34:57.667446 56248 > 55555: seq 62394:63162, length 768
17:34:57.710946 55555 > 56248: ack 63162, win 25, length 0

RCV.BUFF = 2048
RCV.USER = 0
RCV.WND = 25*(2^wscale 7) = 3200
RCV.BUFF - RCV.WND < 0

17:34:57.922953 56248 > 55555: seq 63162:66362, ack 870, win 512, length 3200
17:34:57.922962 55555 > 56248: ack 66362, win 0, length 0

RCV.BUFF = 2048
RCV.USER = 0
RCV.WND = 0
RCV.BUFF >= Fr * RCV.BUFF
RCV.WND -> RCV.BUFF - RCV.USER = 2048 = 16*(2^wscale 7)

17:34:57.922998 55555 > 56248: ack 66362, win 16, length 0
17:34:57.923004 56248 > 55555: seq 66362:66949, ack 870, win 512, length 587
17:34:57.923018 55555 > 56248: ack 66949, win 16, length 0
17:34:57.923035 55555 > 56248: seq 870:949, ack 66949, win 16, length 79
17:34:57.923039 56248 > 55555: ack 949, win 512, length 0
17:34:57.923059 56248 > 55555: seq 66949:67205, ack 949, win 512, length 256
17:34:57.966946 55555 > 56248: ack 67205, win 16, length 0
```
