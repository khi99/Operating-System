/*
*   DKU Operating System Lab
*           Lab2 (Hash Queue Lock Problem)
*           Student id : 32181267
*           Student name : Kim hyung in
*
*   lab2_sync.c :
*       - lab2 main file.
*		- Thread-safe Hash Queue Lock code
*		- Coarse-grained, fine-grained lock code
*       - Must contains Hash Queue Problem function's declations.
*
*   Implement thread-safe Hash Queue for coarse-grained verison and fine-grained version.
*/

#include <aio.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <pthread.h>
#include <asm/unistd.h>

#include "lab2_sync_types.h"

/*
 * TODO
 *  Implement function which init queue nodes for front and rear
 *  ( refer to the ./include/lab2_sync_types.h for front and rear nodes)
 */
void init_queue() {
	front = (queue_node*)malloc(sizeof(queue_node));
    rear = (queue_node*)malloc(sizeof(queue_node));
	//rear->prev = front;
	//sizeof(struct hash_list*);

	/*
	(queue_node*)sizeof(queue_node);
	sizeof(struct q_node*);

	queue_node *q = (queue_node*)malloc(sizeof(queue_node));
	front = q;
	rear = q;
	*/
	queue_node *tmp = (queue_node*)malloc(sizeof(queue_node));
	//tmp->data = NULL;
	front->next = NULL;
	front->prev = tmp;
	rear->next = tmp;
	rear->prev = NULL;
	
}

/*
 * TODO
 *  Implement function which add new_node at next rear node
 *
 *  @param queue_node *new_node		: Node which you need to insert at queue.
 */
void enqueue(queue_node *new_node) {

	new_node->data = target;
	//new_node->next = NULL;
	//new_node->prev = NULL;

	if(rear->next->data == 0){ //처음시작해서 큐가 비어있는 상태라면, 초기화할때 동적할당해놓은 tmp노드 해제하고 타겟값을 갖는 노드 추가
		rear->next = new_node;
		free(front->prev);
		front->prev = new_node;
		new_node->prev = rear;
		new_node->next = front;
		}
	else{ //비어있지않다면 그냥 rear에서 추가
		rear->next->prev = new_node;
		new_node->next = rear->next;
		rear->next = new_node;
		new_node->prev = rear;
	}
	

}

/*
 * TODO
 *  Implement function which add new_node at next rear node
 *
 *  @param queue_node *new_node		: Node which you need to insert at queue in coarse-grained manner.
 */
pthread_mutex_t a,b;

void enqueue_cg(queue_node *new_node) {
	
	pthread_mutex_lock(&a);
	new_node->data = target;

	if(hashlist[hash(target)]->next == NULL){
		rear->next = new_node;
		new_node->prev = rear;
	}
	else{
		(rear->next)->prev = new_node;
		new_node->next = rear->next;
		rear->next = new_node;
		new_node->prev = rear;
	}
	pthread_mutex_unlock(&a);
}

/*
 * TODO
 *  Implement function which add new_node at next rear node
 *
 *  @param queue_node *new_node		: Node which you need to insert at queue in fine-grained manner.
 */
void enqueue_fg(queue_node *new_node) {
	
	
	new_node->data = target;

	if(hashlist[hash(target)]->next == NULL){
		pthread_mutex_lock(&b);
		rear->next = new_node;
		new_node->prev = rear;
		pthread_mutex_unlock(&b);
	}
	else{
		pthread_mutex_lock(&b);
		(rear->next)->prev = new_node;
		new_node->next = rear->next;
		rear->next = new_node;
		new_node->prev = rear;
		pthread_mutex_unlock(&b);
	}
	
}

/*
 * TODO
 *  Implement function which delete del_node at location that contains target key
 *
 *  @param queue_node *del_node		: Node which you need to delete at queue.
 */
void dequeue(queue_node *del_node) {

	(del_node->next)->prev = del_node->prev;
	(del_node->prev)->next = del_node->next;
	free(del_node);
}

/*
 * TODO
 *  Implement function which delete del_node at location that contains target key
 *
 *  @param queue_node *del_node		: Node which you need to delete at queue in coarse-grained manner.
 */
void dequeue_cg(queue_node *del_node) {

	pthread_mutex_lock(&b);
	(del_node->next)->prev = del_node->prev;
	(del_node->prev)->next = del_node->next;
	free(del_node);
	pthread_mutex_unlock(&b);

}

/*
 * TODO
 *  Implement function which delete del_node at location that contains target key
 *
 *  @param queue_node *del_node		: Node which you need to delete at queue in fine-grained manner.
 */
void dequeue_fg(queue_node *del_node) {

	pthread_mutex_lock(&b);
	(del_node->next)->prev = del_node->prev;
	(del_node->prev)->next = del_node->next;
	pthread_mutex_unlock(&b);

	free(del_node);
}









/*
 * TODO
 *  Implement function which init hashlist(same as hashtable) node.
 */
void init_hlist_node() {

	for(int i=0; i<sizeof(hashlist)/sizeof(struct hash_list*); i++){
		hashlist[i] = malloc(sizeof(struct hash_list*));
	}
	//sizeof(hlist_node);
	/*
	for(int i=0; i<HASH_SIZE; i++){
		hashlist[i]->next = NULL;
	}
	

	for(int i=0; i<sizeof(hashlist)/sizeof(struct hash_list*); i++){
		hashlist[i]->next = malloc(sizeof(hlist_node));
	}
	*/

	/*
	hlist_node *hashlist[0]= malloc(sizeof(hlist_node));
	hlist_node *hashlist[1]= malloc(sizeof(hlist_node));
	hlist_node *hashlist[2]= malloc(sizeof(hlist_node));
	hlist_node *hashlist[3]= malloc(sizeof(hlist_node));
	hlist_node *hashlist[4]= malloc(sizeof(hlist_node));
	hlist_node *hashlist[5]= malloc(sizeof(hlist_node));
	hlist_node *hashlist[6]= malloc(sizeof(hlist_node));
	hlist_node *hashlist[7]= malloc(sizeof(hlist_node));
	hlist_node *hashlist[8]= malloc(sizeof(hlist_node));
	hlist_node *hashlist[9]= malloc(sizeof(hlist_node));
	hlist_node *hashlist[10]= malloc(sizeof(hlist_node));
	hlist_node *hashlist[11]= malloc(sizeof(hlist_node));
	hlist_node *hashlist[12]= malloc(sizeof(hlist_node));
	hlist_node *hashlist[13]= malloc(sizeof(hlist_node));*/
	
}

/*
 * TODO
 *  Implement function which calculate hash value with modulo operation.
 */
// 해시값 얻어내는 함수 (나머지 연산으로)
int hash(int val) {
	return val%HASH_SIZE;
}

/*
 * TODO
 *  Implement function which 
 * 
 * insert the result of finding the location of the bucket 
 * tmp
 * using value to the entry and hashtable
 *
 *  @param hlist_node *hashtable		: A pointer variable containing the bucket
 *  @param int val						: Data to be stored in the queue node
 */
void hash_queue_add(hlist_node *hashtable, int val) {


}

/*
 * TODO
 *  Implement function which insert the resilt of finding the location 
 *  of the bucket using value to the entry and hashtable
 *
 *  @param hlist_node *hashtable		: A pointer variable containing the bucket
 *  @param int val						: Data to be stored in the queue node
 */
void hash_queue_add_cg(hlist_node *hashtable, int val) {
	// You need to implement hash_queue_add_cg function.
}

/*
 * TODO
 *  Implement function which insert the resilt of finding the location 
 *  of the bucket using value to the entry and hashtable
 *
 *  @param hlist_node *hashtable		: A pointer variable containing the bucket
 *  @param int val						: Data to be stored in the queue node
 */
void hash_queue_add_fg(hlist_node *hashtable, int val) {
	// You need to implement hash_queue_add_fg function.
}

/*
 * TODO
 *  Implement function which check if the data(value) to be stored is in the hashtable
 *
 *  @param int val						: variable needed to check if data exists
 *  @return								: status (success or fail)
 */


int value_exist(int val) {

	int bucket_index=hash(val);
	hlist_node *tmp = hashlist[bucket_index];
	if(tmp->next == NULL){ //엔트리가 비어있으면 값이 값이 없음을 바로 알림
		return 0;
	}
	else{
		while(tmp->next != NULL){//엔트리를 하나씩 보고 값이 확인되면 있음을 알림, 끝까지 보고 없으면 없음을 알림
			if(tmp->next->q_loc->data == val)
				return 1;
			else
				tmp = tmp->next;	
		}
		return 0;
	}
	
	
}



/*
 * TODO
 *  Implement function which 
 * 
 *   find the bucket location using target
 */


void hash_queue_insert_by_target() {
	
	int bucket_index = hash(target);
	queue_node *new_q_node = malloc(sizeof(queue_node));

	if(value_exist(target) == 0){ //넣으려는값이 해시테이블에 없으면
		//hash_queue_add(hashlist[bucket_index], target);
		
		hlist_node *p = hashlist[bucket_index];
		while(p->next != NULL){p = p->next;} //해시의 해당 인덱스 체인의 끝부분으로 이동
		enqueue(new_q_node); //큐에 새 노드 삽입
		//p->next = new_q_node; //해시체인의 next멤버에 새노드위치 할당
		p->next = malloc(sizeof(hlist_node));
		p->next->q_loc = new_q_node; //해시체인의 다음 노드의 큐노드를 가리키는 멤버에 새노드위치 할당
		p->next->next = NULL; // 해시체인의 마지막의 next는 NULL
		
	}

}

/*
 * TODO
 *  Implement function which find the bucket location using target
 */

pthread_mutex_t c,d;


//pthread_mutex_lock(&c);
//pthread_mutex_unlock(&c);
void hash_queue_insert_by_target_cg() {
	
	pthread_mutex_lock(&c);
	int bucket_index = hash(target);
	queue_node *new_q_node = malloc(sizeof(queue_node));

	if(value_exist(target) == 0){ //넣으려는값이 해시테이블에 없으면
		//hash_queue_add(hashlist[bucket_index], target);
		hlist_node *p = hashlist[bucket_index];
		while(p->next != NULL){p = p->next;} //해시의 해당 인덱스 체인의 끝부분으로 이동
		enqueue(new_q_node); //큐에 새 노드 삽입
		//p->next = new_q_node; //해시체인의 next멤버에 새노드위치 할당
		p->next = malloc(sizeof(hlist_node));
		p->next->q_loc = new_q_node; //해시체인의 다음 노드의 큐노드를 가리키는 멤버에 새노드위치 할당
		p->next->next = NULL; // 해시체인의 마지막의 next는 NULL
	}
	pthread_mutex_unlock(&c);
}
/*
 * TODO
 *  Implement function which find the bucket location using target
 */


void hash_queue_insert_by_target_fg() {
	
	int bucket_index = hash(target);
	queue_node *new_q_node = malloc(sizeof(queue_node));

	if(value_exist(target) == 0){ //넣으려는값이 해시테이블에 없으면
		//hash_queue_add(hashlist[bucket_index], target);
		hlist_node *p = hashlist[bucket_index];

		pthread_mutex_lock(&d);
		while(p->next != NULL){p = p->next;} //해시의 해당 인덱스 체인의 끝부분으로 이동
		enqueue(new_q_node); //큐에 새 노드 삽입
		//p->next = new_q_node; //해시체인의 next멤버에 새노드위치 할당
		p->next = malloc(sizeof(hlist_node));
		p->next->q_loc = new_q_node; //해시체인의 다음 노드의 큐노드를 가리키는 멤버에 새노드위치 할당
		p->next->next = NULL; // 해시체인의 마지막의 next는 NULL
		pthread_mutex_unlock(&d);
	}
	
}

/*
 * TODO
 *  Implement function which find the bucket location and stored data
 *  using target and delete node that contains target
 */


void hash_queue_delete_by_target() {
	
	int bucket_index = hash(target);
	hlist_node *p = hashlist[bucket_index];
	if(value_exist(target) == 1) { //삭제하려는값이 해시테이블에 있으면
		
		while(p->next->q_loc->data != target){p = p->next;} 
		hlist_node *del_next = p->next->next; //삭제할 노드(p->next)의 다음 엔트리 노드를 가리키는 포인터(p->next->next) 저장
		dequeue(p->next->q_loc); //삭제할 노드 큐에서 삭제
		//dequeue(p->next->q_loc);
		free(p->next); //해시엔트리에서 매모리 해제
		p->next=del_next; //엔트리에서 (삭제한노드의 전 노드)의 다음을, 삭제한노드의 다음노드를 가리키게

	}


	/*
	int bucket_index = hash(target);
	queue_node *del_prev = malloc(sizeof(queue_node));
	queue_node *del_next = malloc(sizeof(queue_node));
	hlist_node *tmp = malloc(sizeof(hlist_node));
	if(value_exist(target) == 1){ //삭제하려는값이 해시테이블에 있으면
		
		while(tmp->next->q_loc->data != target){tmp = tmp->next;} //tmp의 다음노드의 값이 삭제할 값일때까지 tmp를 다음노드로 변경
		del_prev = tmp;
		del_next = tmp->next->next;
		dequeue(del_prev->next); //해당 노드 큐에서 삭제
		del_prev->next=tmp->next->next; //체인에서 삭제한노드 전 노드의 다음을 가리키는 포인터를 삭제한노드의 다음노드를 가리키게

	} 
	*/
}


pthread_mutex_t e,f;
/*
 * TODO
 *  Implement function which find the bucket location and stored data
 *  using target and delete node that contains target
 */
void hash_queue_delete_by_target_cg() {

	pthread_mutex_lock(&e);
	int bucket_index = hash(target);
	hlist_node *p = hashlist[bucket_index];
	if(value_exist(target) == 1) { //삭제하려는값이 해시테이블에 있으면

		while(p->next->q_loc->data != target){p = p->next;} 
		hlist_node *del_next = p->next->next; //삭제할 노드(p->next)의 다음 엔트리 노드를 가리키는 포인터(p->next->next) 저장
		dequeue(p->next->q_loc); //삭제할 노드 큐에서 삭제
		free(p->next); //해시엔트리에서 매모리 해제
		p->next=del_next; //엔트리에서 (삭제한노드의 전 노드)의 다음을, 삭제한노드의 다음노드를 가리키게
	}
	pthread_mutex_unlock(&e);
}

/*
 * TODO
 *  Implement function which find the bucket location and stored data
 *  using target and delete node that contains target
 */
void hash_queue_delete_by_target_fg() {

	int bucket_index = hash(target);
	hlist_node *p = hashlist[bucket_index];
	if(value_exist(target) == 1) { //삭제하려는값이 해시테이블에 있으면

		pthread_mutex_lock(&f);
		while(p->next->q_loc->data != target){p = p->next;} 
		hlist_node *del_next = p->next->next; //삭제할 노드(p->next)의 다음 엔트리 노드를 가리키는 포인터(p->next->next) 저장
		dequeue(p->next->q_loc); //삭제할 노드 큐에서 삭제
		free(p->next); //해시엔트리에서 매모리 해제
		p->next=del_next; //엔트리에서 (삭제한노드의 전 노드)의 다음을, 삭제한노드의 다음노드를 가리키게
		pthread_mutex_unlock(&f);
	}
}
/*
int main(){

	init_hlist_node();
	init_queue();

	target = 10;
	hash_queue_insert_by_target();

	

	return 0;
}*/
