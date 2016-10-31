# SA

# hw3

add harddisk

1.	cheack /var/run/dmesg.boot to get hardward information
	+	cheack if there is ada1 or other harddisk
2.	gpart create -s GPT "harddisk name"
3.	gpart add -t freebsd-ufs -a 1M "harddisk name"

