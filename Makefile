GCC = /opt/arm-2010q1/bin/arm-none-linux-gnueabi-gcc

OPT = -march=armv5te 
#GCC = /usr/bin/gcc
#OPT = -march=i386
all: synergy-demo.c
	$(GCC) $(OPT) -I./include -I/usr/lib/jvm/java-6-openjdk-i386/include/linux -I/usr/lib/jvm/java-6-openjdk-i386/include -shared synergy-demo.c _fp.c _fphlp.c base64.c -o libsynergy2416.so 
	cp libsynergy2416.so /home/chaol/ftp/.

clean:
	rm -f *.so