#include <stdio.h>
#include <string.h>
#include <gtest/gtest.h>
#include "fifo.h"

const char* strings[] =
{
	"one",
	"two",
	"three",
	"four",
	"five",
	"six",
	"seven",
	"eight",
	"nine"
};

TEST(FIFO, EnqueueDequeue) {
	int i = 0;

	fifo_t* fifo = fifo_init();
	
	for (i = 0; i < 10; i++)
	{
 		ASSERT_EQ(FIFO_ST_OK, fifo->push(fifo, &i, sizeof(int)));
		printf("enqueue(%d), size(%d)\n", i, fifo->count(fifo));
		ASSERT_EQ(i + 1, fifo->count(fifo));
	}
 		
	for (i = 0; i < 10; i++)
	{
		fifo_elm_t* elm = fifo->pop(fifo);
		ASSERT_NE(elm, (void*)NULL) << "Element is NULL.";
		if (elm)
		{	
			printf("dequeue(%d), size(%d)\n", *(int*)elm->data, fifo->count(fifo));
			ASSERT_EQ(i, *(int*)elm->data);
			ASSERT_EQ(sizeof(int), elm->size);
			ASSERT_EQ(FIFO_ST_OK, elm->free(&elm));
		}
	}

	fifo_destroy(&fifo);
}

TEST(FIFO, EnqueueDequeueStringsCopy) {
	fifo_t* fifo = fifo_init();
	
	for (int i = 0; i < sizeof(strings) / sizeof(char*); i++)
	{
 		ASSERT_EQ(FIFO_ST_OK, fifo->push(fifo, (void*)strings[i], strlen(strings[i]) +1 ));
		printf("enqueue(%s), size(%d)\n", strings[i], fifo->count(fifo) );
		ASSERT_EQ(i + 1, fifo->count(fifo));
	}
 		
	for (int i = 0; i < sizeof(strings) / sizeof(char*); i++)
	{
		fifo_elm_t* elm = fifo->pop(fifo);
		ASSERT_NE(elm, (void*)NULL) << "Element is NULL.";
		if (elm)
		{	
			printf("dequeue(%s), size(%d)\n", (char*)elm->data, fifo->count(fifo));
			ASSERT_STREQ(strings[i], (char*)elm->data);
			ASSERT_EQ(strlen(strings[i]) + 1, elm->size);
			ASSERT_EQ(FIFO_ST_OK, elm->free(&elm));
		}
	}

	fifo_destroy(&fifo);
}

TEST(FIFO, EnqueueDequeueStringsReference) {
	fifo_t* fifo = fifo_init();
	
	for (int i = 0; i < sizeof(strings) / sizeof(char*); i++)
	{
 		ASSERT_EQ(FIFO_ST_OK, fifo->push(fifo, (void*)strings[i], 0));
		printf("enqueue(%s), size(%d)\n", strings[i], fifo->count(fifo) );
		ASSERT_EQ(i + 1, fifo->count(fifo));
	}
 		
	for (int i = 0; i < sizeof(strings) / sizeof(char*); i++)
	{
		fifo_elm_t* elm = fifo->pop(fifo);
		ASSERT_NE(elm, (void*)NULL) << "Element is NULL.";
		if (elm)
		{	
			printf("dequeue(%s), size(%d)\n", (char*)elm->data, fifo->count(fifo));
			ASSERT_STREQ(strings[i], (char*)elm->data);
			ASSERT_EQ(0, elm->size);
			ASSERT_EQ(FIFO_ST_OK, elm->free(&elm));
		}
	}

	fifo_destroy(&fifo);
}

TEST(FIFO, Destroy) {
	fifo_t* fifo = fifo_init();
	
	fifo_destroy(&fifo);
}
 
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}