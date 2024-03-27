#ifndef QUEUE_H
#define QUEUE_H
#include<cstdlib>
#include<stdexcept>
#define QUEUE_SIZE 10 

using namespace std;
template <typename T>

class Queue
{
	private:
		T *array;
		int capacity;	//Max Capacity of the Queue
		int size;		//current number of elements in the Queue
		int front;		//front of the Queue
		int rear;		//index where a new element will be added
	public:
		Queue(int capacity=10);
		~Queue();
		void enqueue(T); 
		T dequeue();
		bool isEmpty();
		bool isFull();
		T front_element();
		friend void display();
};
//================================================

template<typename T> Queue<T>::Queue(int capacity)
{	
	this->front = 0;                                               // Initialize front, back and size of the queue to 0
	this->rear = 0;
	this->size = 0;
	this->capacity = QUEUE_SIZE;
	array = new T[capacity];                                       // Allocate memory for the queue using dynamic array
}
template<typename T> Queue<T>::~Queue()
{
	delete [] array;                                               // Delete array to deallocate memory and prevent memory leaks
}
//===========================================
template<typename T> void Queue<T>::enqueue(T element)
{
	if (isFull()) throw runtime_error("Queue is full");            // Check if the queue is full

	array[rear] = element;                                         // Add the element to the rear of the queue
	rear = (rear+1) % capacity;                                    // Update the rear pointer to the next position in a circular manner
	size++;                                                        // Increment the size of the queue
}
//===========================================
template<typename T> T Queue<T>::dequeue()
{
	if (isEmpty()) throw runtime_error("Queue is empty");          // Check if the queue is empty

	T element = array[front];                                      // Get the element at the front of the queue
	front = (front+1) % capacity;                                  // Update the front pointer to the next position in a circular manner
	size--;                                                        // Decrement the size of the queue
	return element;                                                // Return the dequeued element
}
//===========================================
template<typename T> bool Queue<T>::isEmpty()                      // Check if the queue is empty
{
	return (this->size <= 0);
}

template<typename T> T Queue<T>::front_element()                      // Returns the front element
{
	return array[0];
}
//===========================================
template<typename T> bool Queue<T>::isFull()                       // Check if the queue is full
{
	return (this->size == this->capacity);
}

#endif

