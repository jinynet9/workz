
#
# datagram
#

# client
$ echo "shell" | nc -uU "/tmp/process_a"





#
# stream
#

# server
$ ./sum_server /tmp/test_udp
5 + 6 = 11


# client
$ ls /tmp/test_udp -l
srwxr-xr-x 1 jinynet9 jinynet9 0 Nov 18 12:54 /tmp/test_udp=
$ ./sum_client /tmp/test_udp 5 6


