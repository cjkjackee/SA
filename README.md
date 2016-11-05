# SA

# hw3

add harddisk

1.	cheack /var/run/dmesg.boot to get hardward information
	+	check if there is ada1 or other harddisk
2.	gpart create -s GPT "harddisk name"
3.	gpart add -t freebsd-ufs -a 1M "harddisk name"

compress zfs snapshot

1.	zfs send "snapshot name" | xz > "compress file name"

aes256
1.	encryption
	-	openssl aes-256-cbc -in "filename" -out "filename.enc"
2.	decryption
	-	openssl aes-256-cbc -d -in "filename.enc" -out "filename"
