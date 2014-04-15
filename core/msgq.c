#include "msgq.h"

#ifdef msgq_test
	#include <assert.h>
	#define TEST_FILE __FILE__
	#define TEST_VAL 128  // consider +1.

	void test_nothing_happened();
	void test_one_happened();
	void test_one_happened_anyway();
	void test_two_happened_parallel();
	void test_two_happened_anyway();
	void test_two_happened_distribution();

	int main(){
		test_nothing_happened();
		test_one_happened();
		test_one_happened_anyway();
		test_two_happened_parallel();
		test_two_happened_anyway();
		test_two_happened_distribution();
		// TODO : Add more test.
		return 0;
	}
#endif

key_t msgq_open(char * const keyfile){
	return msgget(ftok(keyfile, 1), 0666 | IPC_CREAT);
}

int msgq_close(key_t key){
	return msgctl(key, IPC_RMID, NULL);
}

int msgq_send(key_t key, long mtype, int mdata){
	msg_pack new;
	new.mtype = mtype;
	new.mdata = mdata;
	return msgsnd(key, &new, 4, IPC_NOWAIT);
}

size_t msgq_recv(key_t key, long mtype, int *mdata){
	msg_pack new;
	int retval = msgrcv(key, &new, 4, mtype, IPC_NOWAIT);
	*mdata = new.mdata;
	return retval;
}

#ifdef msgq_test
	key_t mykey;
	int result;

	void test_nothing_happened(){
		assert((mykey = msgq_open(TEST_FILE)) > 0);
		assert(!msgq_close(mykey));
	}
	void test_one_happened(){
		assert((mykey = msgq_open(TEST_FILE)) > 0);
		assert(!msgq_send(mykey, 1, TEST_VAL));
		assert((msgq_recv(mykey, 1, &result) != -1) && (result == TEST_VAL));
		assert(!msgq_close(mykey));
	}
	void test_one_happened_anyway(){
		assert((mykey = msgq_open(TEST_FILE)) > 0);
		assert(!msgq_send(mykey, 1, TEST_VAL));
		assert((msgq_recv(mykey, 0, &result) != -1) && (result == TEST_VAL));
		assert(!msgq_close(mykey));
	}
	void test_two_happened_parallel(){
		assert((mykey = msgq_open(TEST_FILE)) > 0);
		assert(!msgq_send(mykey, 1, TEST_VAL));
		assert(!msgq_send(mykey, 1, TEST_VAL + 1));
		assert((msgq_recv(mykey, 0, &result) != -1) && (result == TEST_VAL));
		assert((msgq_recv(mykey, 0, &result) != -1) && (result == TEST_VAL + 1));
		assert(!msgq_close(mykey));
	}
	void test_two_happened_anyway(){
		assert((mykey = msgq_open(TEST_FILE)) > 0);
		assert(!msgq_send(mykey, 1, TEST_VAL));
		assert(!msgq_send(mykey, 2, TEST_VAL + 1));
		assert((msgq_recv(mykey, 0, &result) != -1) && (result == TEST_VAL));
		assert((msgq_recv(mykey, 0, &result) != -1) && (result == TEST_VAL + 1));
		assert(!msgq_close(mykey));
	}
	void test_two_happened_distribution(){
		assert((mykey = msgq_open(TEST_FILE)) > 0);
		assert(!msgq_send(mykey, 1, TEST_VAL));
		assert(!msgq_send(mykey, 2, TEST_VAL + 1));
		assert((msgq_recv(mykey, 2, &result) != -1) && (result == TEST_VAL + 1));
		assert((msgq_recv(mykey, 1, &result) != -1) && (result == TEST_VAL));
		assert(!msgq_close(mykey));
	}
#endif
