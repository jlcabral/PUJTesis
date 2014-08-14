#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>
#define MQ_NAME "/mqlinuxpedia"
//#define MQ_MESSAGE_DATA "hello world"
#define MQ_MESSAGE_DATA "hola"
#define MQ_MESSAGE_LENGTH 12
#define MQ_MESSAGE_PRIORITY 0

int main()
{
	mqd_t mqd;
	int ret;

	/* Open the message queue. Message queue is already created from a different process */
	mqd = mq_open(MQ_NAME, O_WRONLY);
	if( mqd != (mqd_t)-1 )
	{
		printf(" Message Queue Opened\n");

		printf(" Sending message.... ");
		//ret = mq_send(mqd, MQ_MESSAGE_DATA, MQ_MESSAGE_LENGTH, MQ_MESSAGE_PRIORITY);
                ret = mq_send(mqd, MQ_MESSAGE_DATA, strlen(MQ_MESSAGE_DATA), MQ_MESSAGE_PRIORITY);

		if(ret)
			perror("Failed");
		else
			printf("Done\n");

		/* Close the message queue */
		ret = mq_close(mqd);
		if(ret)
			perror(" Message queue close failed");
		else
			printf(" Message Queue Closed\n");

	}
	else
	{
		perror(" Message queue open failed ");
	}

	return 0;
}
