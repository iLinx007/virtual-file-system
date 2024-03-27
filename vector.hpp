#ifndef VECTOR_H
#define VECTOR_H

#include<cstdlib>
#include <stdexcept>
#include<iostream>
#include<iomanip>
#include<sstream>
using namespace std;

template <typename T>
class Vector
{
	private:
		T *data;						//pointer to int(array) to store elements
		int v_size;						//current size of vector (number of elements in vector)
		int v_capacity;					//capacity of vector
	public:
		Vector(int cap=0);			//Constructor
		~Vector();					//Destructor
		int size() const;				//Return current size of vector
		int capacity() const;			//Return capacity of vector
		bool empty() const; 			//Rturn true if the vector is empty, False otherwise
		const T& front();				//Returns reference of the first element in the vector
		const T& back();				//Returns reference of the Last element in the vector
		void push_back(T element);		//Add an element at the end of vector
		void insert(int index, T element); //Add an element at the index 
		void erase(int index);			//Removes an element from the index
		T& operator[](int index);			//Returns the reference of an element at given index
		T& at(int index); 				//return reference of the element at given index
		void shrink_to_fit();			//Reduce vector capacity to fit its size
		void reserve(int N);
};


template <typename T>
Vector<T>::Vector(int cap){

	v_size = 0;
	v_capacity = cap;
	data = new T[v_capacity];
}

template <typename T>
Vector<T>::~Vector(){
	delete[] data;
}

template <typename T>
int Vector<T>::size() const{
	return v_size;
}

template <typename T>
int Vector<T>::capacity() const{
	return v_capacity;
}

template <typename T>
bool Vector<T>::empty() const{
	return v_size == 0;
}

template <typename T>
const T& Vector<T>::front(){
	if (empty()) throw runtime_error("Vector is empty");
	return data[0];
}


template <typename T>
const T& Vector<T>::back(){
	if (empty()) throw runtime_error("Vector is empty");
	return data[v_size-1];
}

template <typename T>
void Vector<T>::reserve(int N){

		T *temp_arr = new T[N];

		for (int i = 0; i < v_size; i++){
			temp_arr[i] = data[i];
		}

		delete[] data;
		data = temp_arr;
		v_capacity = N;
}

template <typename T>
void Vector<T>::push_back(T element){

	if (v_size >= v_capacity) reserve(max(1,2*v_capacity));

	data[v_size] = element;
	v_size++;
}

template <typename T>
void Vector<T>::insert(int index, T element){

	if (index < 0 || index >= v_size) throw runtime_error("Index out of range");

	if (v_size >= v_capacity) reserve(max(1,2*v_capacity));

	for (int i = v_size; i > index; i--){
		data[i] = data[i-1];
	}

	data[index] = element;
	v_size++;

}

template <typename T>
void Vector<T>::erase(int index){

	if (index < 0 || index >= v_size) throw runtime_error("Index out of range");

	for (int i = index; i < v_size-1; i++){
		data[i] = data[i+1];
	}
	v_size--;
}

template <typename T>
T& Vector<T>::operator[](int index){

	return data[index];
}

template <typename T>
T& Vector<T>::at(int index){

	if (index < 0 || index >= v_size) throw runtime_error("Index out of range");

	return data[index];
}

template <typename T>
void Vector<T>::shrink_to_fit(){

	if (empty()){
		throw runtime_error("Vector is empty");
		return;
	}

	reserve(v_size);
}

#endif