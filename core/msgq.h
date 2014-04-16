#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#ifndef msgq_h
	#define msgq_h
	typedef struct _msg_pack{
		long mtype;
		int mdata;  // TODO : is it enough?
	}msg_pack;
	#define MSG_TO_INPUT 10
	#define MSG_TO_PROCESS 20
	#define MSG_TO_OUTPUT 30

	key_t msgq_open(char * const keyfile);
	int msgq_close(key_t key);
	int msgq_send(key_t key, long mtype, int mdata);
	size_t msgq_recv(key_t key, long mtype, int *mdata);
#endif
