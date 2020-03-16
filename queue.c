struct Queue
{
  int front, rear, size;
  unsigned capacity;
  int* array;
};
struct Queue* queue_create(unsigned capacity)
{
  struct Queue* queue = (struct Queue*) malloc(sizeof(struct Queue));
  queue->capacity = capacity;
  queue->front = queue->size = 0;
  queue->rear = capacity - 1;  // This is important, see the enqueue
  queue->array = (int*) malloc(queue->capacity * sizeof(int));
  return queue;
};

int queue_isFull(struct Queue* queue)
{
  return (queue->size == queue->capacity);
}

int queue_isEmpty(struct Queue* queue)
{
  return (queue->size == 0);
}

void queue_enqueue(struct Queue* queue, int item) 
{ 
    if (queue_isFull(queue)) 
        return; 
    queue->rear = (queue->rear + 1)%queue->capacity; 
    queue->array[queue->rear] = item; 
    queue->size = queue->size + 1; 
    printf("%d enqueued to queue\n", item); 
}

int queue_dequeue(struct Queue* queue) 
{ 
    if (queue_isEmpty(queue)) 
        return -1; 
    int item = queue->array[queue->front]; 
    queue->front = (queue->front + 1)%queue->capacity; 
    queue->size = queue->size - 1; 
    return item; 
} 

int queue_front(struct Queue* queue) 
{ 
    if (queue_isEmpty(queue)) 
        return -1; 
    return queue->array[queue->front]; 
} 
  
// Function to get rear of queue 
int queue_rear(struct Queue* queue) 
{ 
    if (queue_isEmpty(queue)) 
        return -1; 
    return queue->array[queue->rear]; 
} 
