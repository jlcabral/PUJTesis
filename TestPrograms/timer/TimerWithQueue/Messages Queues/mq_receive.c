#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#define MQ_NAME "/mqlinuxpedia"
#define MQ_MESSAGE_MAX_LENGTH 100
#define MQ_MAX_NUM_OF_MESSAGES 10
#define MQ_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

int main()
{       
    int cont = 0;
	mqd_t mqd;
	struct mq_attr attr;
	int ret;
	char msg_buff[MQ_MESSAGE_MAX_LENGTH];
	ssize_t msg_len;

	/* Form the queue attributes */
	attr.mq_flags = 0; /* i.e mq_send will be block if message queue is full */
	attr.mq_maxmsg = MQ_MAX_NUM_OF_MESSAGES;
	attr.mq_msgsize = MQ_MESSAGE_MAX_LENGTH;
	attr.mq_curmsgs = 0; /* mq_curmsgs is dont care */

	/* Create message queue */
	mqd = mq_open(MQ_NAME, O_RDONLY | O_CREAT, MQ_MODE, &attr);
	if( mqd != (mqd_t)-1 )
	{
		printf(" Message Queue Opened\n");
            while(1){
		printf(" Receiving message .... \n");
		msg_len = mq_receive(mqd, msg_buff, MQ_MESSAGE_MAX_LENGTH, NULL);
		if(msg_len < 0){
			perror("   Failed");
		}
		else{
			msg_buff[MQ_MESSAGE_MAX_LENGTH-1] = '\0';
			printf(" Successfully received %d bytes\n", (int)msg_len);
			printf(" Message : %s, cont:[%d] \n", msg_buff,cont);
                        cont++;
		}
            }
		/* Close the message queue */
		ret = mq_close(mqd);
		if(ret)
			perror(" Message queue close failed");
		else
			printf(" Message Queue Closed\n");

		ret = mq_unlink(MQ_NAME);
		if(ret)
			perror(" Message queue unlink failed");
		else
			printf(" Message Queue unlinked\n");
	}
	else
	{
		perror(" Message queue open failed ");
	}

	return 0;
}
