# SA

# hw3

add harddisk

1.	cheack /var/run/dmesg.boot to get hardward information
	+	check if there is ada1 or other harddisk
2.	gpart create -s GPT "harddisk name"
3.	gpart add -t freebsd-ufs -a 1M "harddisk name"

compress zfs snapshot (use xz)

1.	zfs send "snapshot name" | xz > "compress file name"
	-	want to use another compress tool, just change "xz" in the command to the tool you like to use

aes256

1.	encryption
	-	openssl aes-256-cbc -in "filename" -out "filename.enc"
2.	decryption
	-	openssl aes-256-cbc -d -in "filename.enc" -out "filename"

upload or download snapshot (google drive)

1.	upload
	-	gdrive upload "filename"
2.	download
	-	gdrive download "filename"
+	gdrive can download from:
	-	https://github.com/prasmussen/gdrive#downloads
+	install gdrive 
	1.	download the gdrive
	2.	you wiil get a uc....... file, just rename it by mv uc....... gdrive
	3.	chmod +x gdrive
	4.	sudo install gdrive /usr/local/bin/gdrive
+	for the first time using gdrive, enter the command below:
	-	gdrive about

receive snapshot

1.	zfs receive "snapshot name" < "file path"

roll back
`zfs rollback "snapshot name"`

