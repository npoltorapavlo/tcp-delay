```shell script
tcpdump -i lo port 55555 > tcp.log
./tcp-delay 55555 1 1 1024 256 5579 79 256
	55555 port
	1 SO_REUSEADDR | SO_REUSEPORT
	1 backlog
	1024 SO_RCVBUF
	256 SO_SNDBUF
	5579 message size
	79 response size
	256 packet size
```

1. Data is buffered by the [Nagle Algorithm](https://www.ietf.org/rfc/rfc1122.txt). See `length` in [tcp.log](./tcp.log)
2. Receiver keeps `RCV.NXT+RCV.WND` fixed until [`RCV.BUFF-RCV.USER-RCV.WND >= min(Fr*RCV.BUFF, Eff.snd.MSS)`](https://www.ietf.org/rfc/rfc1122.txt). See `win` in [tcp.log](./tcp.log)
3. After some time, performance degrades rapidly from 40ms to 1s. See https://access.redhat.com/solutions/6481061
