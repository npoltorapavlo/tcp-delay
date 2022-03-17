# TCP algorithms

## Nagle's

> If there is unacknowledged data,
> then the sending TCP buffers all user data (regardless of the PSH bit),
> until the outstanding data has been acknowledged
> or until the TCP can send a full-sized segment (Eff.snd.MSS bytes).

https://www.ietf.org/rfc/rfc1122.txt

```shell script
sudo tcpdump -i lo port 55555 > Logs/nagle.log
./server 55555 0 0 1024 1024 1024 1024 5579 79 0 1 &
sleep 5 &&
./client 55555 0 0 256 256 256 256 79 5579 0 1
```

[nagle.log](Logs/nagle.log)

## ACK delays

> Receiver waits for a packet on which the ACK can be piggybacked,
> if not getting it, sends the ACK delayed (up to 500 ms).
> Timer is reset after every recv.

## TCP window

```
RCV.NXT right edge
     |  |
   _______ RCV.BUFF
**|**|**|*|** - not available
|  |  |  |
|  |  |  available but not yet advertised
|  |  advertised to sender (RCV.WND)
|  received but not yet consumed (RCV.USER)
received and consumed
```

> Receiver avoids advancing the right window edge in small increments (SWS avoidance).
> It still ACK-s the received data, such that the sender knows not to retransmit it.

> Receiver keeps `RCV.NXT+RCV.WND` fixed until:

```
RCV.BUFF - RCV.USER - RCV.WND >= min( Fr * RCV.BUFF, Eff.snd.MSS )
```

> When the inequality is satisfied, `RCV.WND` is set to `RCV.BUFF-RCV.USER`.

https://www.ietf.org/rfc/rfc1122.txt

# Issues

## SO_RCVBUF smaller than MSS

> The sender attempts to efficiently use network bandwidth through
> minimizing header overhead by waiting extra time (200ms) for the receiver
> to return an advertised window at least as large as the MSS
> or as large as the prior seen maximum advertised window.

> If a receiver application connects, or accepts,
> connections transmitting a large initial receive buffer,
> then the initial advertised window will be large.
> If the application later calls setsockopt(SO_RCVBUF) with a value
> smaller than the connection's MSS
> (and thus also smaller than the initial advertised window)
> then the sender assumes the client will be returning
> to an advertised window above MSS and should wait
> to avoid congesting the network with needless traffic
> when the receiver is experiencing memory pressure for some unknown reason.

https://access.redhat.com/solutions/6481061

> Since RCV.BUFF < RCV.WND, the [inequality](#tcp-window)
> is satisfied when RCV.WND reaches one-half of RCV.BUFF.
> Then RCV.WND is set to RCV.BUFF.

```shell script
sudo tcpdump -i lo port 55555 > Logs/delays200ms.log
./server 55555 0 0 2048 2048 2048 2048 5579 79 0 1 &
sleep 5 &&
./client 55555 0 0 256 5579 256 5579 79 5579 0 100
```

[delays200ms.log](Logs/delays200ms.log)

## TCP deadlock

> If retry `send / recv` on `EAGAIN`,
> the deadlock occurs when both the client and server
> are trying to send an amount of data that is larger
> than the combined input and output buffer size.
> A read cannot be finished if a write transaction is in progress, and vice versa.

```shell script
sudo tcpdump -i lo port 55555 > Logs/deadlock.log
./server 55555 0 0 1024 1024 1024 1024 4096 200000 0 1 &
sleep 5 &&
./client 55555 0 0 256 256 256 256 200000 200000 0 1
```

[deadlock.log](Logs/deadlock.log)
