#Makefile for TWMailerServer

CC=g++
CFLAGS= -g -Wall -std=c++0x -pthread
VERSION=1.0

all: TWMailerServer

TWMailerServer: TWMailerServer.o MessageService.o MessageDao.o Message.o LDAPService.o Helper.o File.o BlockedUserService.o ConnectionHandler.o
	${CC} ${CFLAGS} -o TWMailerServer TWMailerServer.o MessageService.o MessageDao.o Message.o LDAPService.o Helper.o File.o BlockedUserService.o ConnectionHandler.o -lldap

TWMailerServer.o: TWMailerServer.cpp MessageService.o ConnectionHandler.o
	${CC} ${CFLAGS} -c TWMailerServer.cpp

MessageService.o: MessageService.cpp MessageService.h MessageDao.o
	${CC} ${CFLAGS} -c MessageService.cpp

MessageDao.o: MessageDao.cpp MessageDao.h Message.o
	${CC} ${CFLAGS} -c MessageDao.cpp

Message.o: Message.cpp Message.h File.o
	${CC} ${CFLAGS} -c Message.cpp

File.o: File.cpp File.h
	${CC} ${CFLAGS} -c File.cpp

LDAPService.o: LDAPService.cpp LDAPService.h
	${CC} ${CFLAGS} -c -DLDAP_DEPRECATED LDAPService.cpp

Helper.o: Helper.cpp Helper.h
	${CC} ${CFLAGS} -c Helper.cpp

BlockedUserService.o: BlockedUserService.cpp BlockedUserService.h
	${CC} ${CFLAGS} -c BlockedUserService.cpp
	
ConnectionHandler.o: ConnectionHandler.cpp ConnectionHandler.h
	${CC} ${CFLAGS} -c ConnectionHandler.cpp

clean: 
	rm -f *.o
	rm -f TWMailerServer

dist:
	mkdir TWMailerServer-${VERSION}
	cp *.h TWMailerServer-${VERSION}/
	cp *.cpp TWMailerServer-${VERSION}/
	cp Makefile TWMailerServer-${VERSION}/Makefile
	tar -cvzf TWMailerServer-${VERSION}.tgz TWMailerServer-${VERSION}/
	rm -R TWMailerServer-${VERSION}

distclean:
	rm -f TWMailerServer-${VERSION}.tgz
