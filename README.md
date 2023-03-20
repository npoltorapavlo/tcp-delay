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
https://access.redhat.com/solutions/6481061
