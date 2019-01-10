# use normal make for this Makefile
#
# Makefile for building user programs to run on top of Nachos
#
# Several things to be aware of:
#
#    Nachos assumes that the location of the program startup routine (the
# 	location the kernel jumps to when the program initially starts up)
#       is at location 0.  This means: start.o must be the first .o passed 
# 	to ld, in order for the routine "Start" to be loaded at location 0
#

# if you are cross-compiling, you need to point to the right executables
# and change the flags to ld and the build procedure for as

GCCDIR = ../../../gnu-decstation-ultrix/decstation-ultrix/2.95.3/

LDFLAGS = -T script -N
ASFLAGS = -mips2
CPPFLAGS = $(INCDIR)

# if you aren't cross-compiling:
# GCCDIR =
# LDFLAGS = -N -T 0
# ASFLAGS =
# CPPFLAGS = -P $(INCDIR)

PATH = $(GCCDIR):/lib:/usr/bin:/bin

CC = $(GCCDIR)gcc -B../../../gnu-decstation-ultrix/
AS = $(GCCDIR)as
LD = $(GCCDIR)ld

CPP = gcc -E
INCDIR =-I../userprog -I../threads
CFLAGS = -G 0 -c $(INCDIR)

all: halt shell matmult sort openfile printstring echo createfile cat copy write ping pong scheduler Nhap_Bot Nhap_Nguyen_Lieu Nhap_Gia_Vi Lam_Banh San_Xuat_Banh

start.o: start.s ../userprog/syscall.h
	$(CPP) $(CPPFLAGS) start.c > strt.s
	$(AS) $(ASFLAGS) -o start.o strt.s
	rm strt.s

halt.o: halt.c
	$(CC) $(CFLAGS) -c halt.c
halt: halt.o start.o
	$(LD) $(LDFLAGS) start.o halt.o -o halt.coff
	../bin/coff2noff halt.coff halt

shell.o: shell.c
	$(CC) $(CFLAGS) -c shell.c
shell: shell.o start.o
	$(LD) $(LDFLAGS) start.o shell.o -o shell.coff
	../bin/coff2noff shell.coff shell

sort.o: sort.c
	$(CC) $(CFLAGS) -c sort.c
sort: sort.o start.o
	$(LD) $(LDFLAGS) start.o sort.o -o sort.coff
	../bin/coff2noff sort.coff sort

matmult.o: matmult.c
	$(CC) $(CFLAGS) -c matmult.c
matmult: matmult.o start.o
	$(LD) $(LDFLAGS) start.o matmult.o -o matmult.coff
	../bin/coff2noff matmult.coff matmult

createfile.o: createfile.c
	$(CC) $(CFLAGS) -c createfile.c
createfile: createfile.o start.o
	$(LD) $(LDFLAGS) start.o createfile.o -o createfile.coff
	../bin/coff2noff createfile.coff createfile

openfile.o: openfile.c
	$(CC) $(CFLAGS) -c openfile.c
openfile: openfile.o start.o
	$(LD) $(LDFLAGS) start.o openfile.o -o openfile.coff
	../bin/coff2noff openfile.coff openfile
	
printstring.o: printstring.c
	$(CC) $(CFLAGS) -c printstring.c
printstring: printstring.o start.o
	$(LD) $(LDFLAGS) start.o printstring.o -o printstring.coff
	../bin/coff2noff printstring.coff printstring

echo.o: echo.c
	$(CC) $(CFLAGS) -c echo.c
echo: echo.o start.o
	$(LD) $(LDFLAGS) start.o echo.o -o echo.coff
	../bin/coff2noff echo.coff echo	

read.o: read.c
	$(CC) $(CFLAGS) -c read.c
read: read.o start.o
	$(LD) $(LDFLAGS) start.o read.o -o read.coff
	../bin/coff2noff read.coff read

write.o: write.c
	$(CC) $(CFLAGS) -c write.c
write: write.o start.o
	$(LD) $(LDFLAGS) start.o write.o -o write.coff
	../bin/coff2noff write.coff write

cat.o: cat.c
	$(CC) $(CFLAGS) -c cat.c
cat: cat.o start.o
	$(LD) $(LDFLAGS) start.o cat.o -o cat.coff
	../bin/coff2noff cat.coff cat

copy.o: copy.c
	$(CC) $(CFLAGS) -c copy.c
copy: copy.o start.o
	$(LD) $(LDFLAGS) start.o copy.o -o copy.coff
	../bin/coff2noff copy.coff copy

ping.o: ping.c
	$(CC) $(CFLAGS) -c ping.c
ping: ping.o start.o
	$(LD) $(LDFLAGS) start.o ping.o -o ping.coff
	../bin/coff2noff ping.coff ping

pong.o: pong.c
	$(CC) $(CFLAGS) -c pong.c
pong: pong.o start.o
	$(LD) $(LDFLAGS) start.o pong.o -o pong.coff
	../bin/coff2noff pong.coff pong

scheduler.o: scheduler.c
	$(CC) $(CFLAGS) -c scheduler.c
scheduler: scheduler.o start.o
	$(LD) $(LDFLAGS) start.o scheduler.o -o scheduler.coff
	../bin/coff2noff scheduler.coff scheduler
Nhap_Bot.o: Nhap_Bot.c
	$(CC) $(CFLAGS) -c Nhap_Bot.c
Nhap_Bot: Nhap_Bot.o start.o
	$(LD) $(LDFLAGS) start.o Nhap_Bot.o -o Nhap_Bot.coff
	../bin/coff2noff Nhap_Bot.coff Nhap_Bot

Nhap_Nguyen_Lieu.o: Nhap_Nguyen_Lieu.c
	$(CC) $(CFLAGS) -c Nhap_Nguyen_Lieu.c
Nhap_Nguyen_Lieu: Nhap_Nguyen_Lieu.o start.o
	$(LD) $(LDFLAGS) start.o Nhap_Nguyen_Lieu.o -o Nhap_Nguyen_Lieu.coff
	../bin/coff2noff Nhap_Nguyen_Lieu.coff Nhap_Nguyen_Lieu

Nhap_Gia_Vi.o: Nhap_Gia_Vi.c
	$(CC) $(CFLAGS) -c Nhap_Gia_Vi.c
Nhap_Gia_Vi: Nhap_Gia_Vi.o start.o
	$(LD) $(LDFLAGS) start.o Nhap_Gia_Vi.o -o Nhap_Gia_Vi.coff
	../bin/coff2noff Nhap_Gia_Vi.coff Nhap_Gia_Vi

Lam_Banh.o: Lam_Banh.c
	$(CC) $(CFLAGS) -c Lam_Banh.c
Lam_Banh: Lam_Banh.o start.o
	$(LD) $(LDFLAGS) start.o Lam_Banh.o -o Lam_Banh.coff
	../bin/coff2noff Lam_Banh.coff Lam_Banh

San_Xuat_Banh.o: San_Xuat_Banh.c
	$(CC) $(CFLAGS) -c San_Xuat_Banh.c
San_Xuat_Banh: San_Xuat_Banh.o start.o
	$(LD) $(LDFLAGS) start.o San_Xuat_Banh.o -o San_Xuat_Banh.coff
	../bin/coff2noff San_Xuat_Banh.coff San_Xuat_Banh
