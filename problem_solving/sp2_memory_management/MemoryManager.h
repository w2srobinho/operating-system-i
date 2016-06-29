/*
 * File:   MemoryManager.h
 * Authors: Fábio Reina, Felipe Calistro, Willian Souza
 *
 * Created on June 14, 2016, 22h00m
 */

#ifndef MEMORYMANAGER_H
#define	MEMORYMANAGER_H

#include "Debug.h"


template<typename T>
class Elemento {
    private:
        T *info;
        Elemento<T> *_next;
    
    public:
        Elemento(const T& info, Elemento<T> *next) : info(new T(info)), _next(next) {}
        
        ~Elemento() {
            delete info;
        }
        
        T getInfo() const {
            return *info;
        }
        
        Elemento<T>* getNext() const {
            return _next;
        }
        
        void setNext(Elemento<T> *next){
            _next = next;
        }
};    

template<typename T>
class ListaEncaminhada{
    public:
        ListaEncaminhada(){
            _size = 0;
            head = nullptr;
        }
        
        ~ListaEncaminhada(){
            destroyList();
        }
        
        int getSize(){
            return _size;
        }
        
        void adicionaNoInicio(const T& dado){
            head = new Elemento<T>(dado, head);
            _size++;
        }
        
        T retiraDoInicio(){
            Elemento<T> *oldHead = head;
            head = head->getNext();
            T info = oldHead->getInfo();
            _size--;
            return info;
        }
        
        void eliminaDoInicio(){
            Elemento<T> *oldHead = head;
            head = head->getNext();
            delete oldHead;
            _size--;
        }
        
        void adicionaNaPosicao(const T& dado, int pos){
            if(_size == 0 || pos == 0){
                adicionaNoInicio(dado);
                return;
            }
            
            Elemento<T> *tmp = head;
            
            int i = 0;
            while(i < pos - 1){
                tmp = tmp->getNext();
                i++;
            }
            
            Elemento<T> *newElemento = new Elemento<T>(dado, tmp->getNext());
            tmp->setNext(newElemento);
            _size++;
        }
        
        int posicao(const T& dado) const{
            int i = 0;
            Elemento<T> *tmp = head;
            while(i < _size){
                if(tmp->getInfo() == dado) {
                    return i;
                }
                tmp = tmp->getNext();
                i++;
            }
            return -1;
        }
        
        bool contem(const T& dado){
            if(posicao(dado) != -1){
                return true;
            }
            else{
                return false;
            }
        }
        
        T retiraDaPosicao(int pos){
            if(pos < 0 || pos >= _size){
                return nullptr;
            }
            if(pos == 0) {
                return retiraDoInicio();
            }
            
            Elemento<T> *tmp = head;
            int i = 0;
            while(i < pos - 1){
                tmp = tmp->getNext();
                i++;
            }
            
            Elemento<T> *removed = tmp->getNext();
            tmp->setNext(removed->getNext());
            T info = removed->getInfo();
            _size--;
            return info;
        }
        
        T consultaInfoPosicao(int pos) const {
            if(pos < 0 || pos >= _size){
                return nullptr;
            }
            if(pos == 0){
                return head->getInfo();
            }
            
            Elemento<T> *tmp = head;
            int i = 0;
            while(i < pos - 1){
                tmp = tmp->getNext();
                i++;
            }
            
            return tmp->getNext()->getInfo();
        }
        
        void adiciona(const T& dado){
            adicionaNaPosicao(dado, _size);
        }
        
        T retira(){
            return retiraDaPosicao(_size - 1);
        }
        
        bool listaVazia() const{
            if(_size < 1){
                return true;
            }
            else{
                return false;
            }
        }
        
        void destroyList(){
            while(_size > 0){
                eliminaDoInicio();
            }
        }
        
        void destroyList(int pos){
            for(int i = pos; i < _size; i++){
                retiraDaPosicao(i);
            }
        }
        
    private:
        Elemento<T> *head;
        int _size = 0;
};

class Partition {
public:

    Partition(unsigned int beginAddress, unsigned int endAddress, bool isFree) {
        _beginAddress = beginAddress;
        _endAddress = endAddress;
        _isFree = isFree;
    }
    
    bool isFree(){
        return _isFree;
    }
    
public: // do not change

    unsigned int getBeginAddress() const {
        return _beginAddress;
    }

    unsigned int getEndAddress() const {
        return _endAddress;
    }

    unsigned int getLength() const {
        return _endAddress - _beginAddress + 1;
    }
private: // do not change
    unsigned int _beginAddress;
    unsigned int _endAddress;
    bool _isFree;
private:
    // INSERT YOUR CODE HERE
    
};

enum MemoryAllocationAlgorithm {FirstFit, NextFit, BestFit, WorstFit};

class MemoryManager {
public: // do not change
    MemoryManager(MemoryAllocationAlgorithm algorithm);
    MemoryManager(const MemoryManager& orig);
    virtual ~MemoryManager();
public: // do not change
    Partition* allocateMemory(unsigned int size);
    void deallocateMemory(Partition* partition);
    void showMemory();
    unsigned int getNumPartitions();
    Partition* getPartition(unsigned int index);
private: // private attributes and methods
    
    ListaEncaminhada<Partition *> *partitions;
    int nextFitIndex;
    int allocatedPartitions;

    Partition* firstFit(unsigned int size);
    Partition* bestFit(unsigned int size);
    Partition* nextFit(unsigned int size);
    Partition* worstFit(unsigned int size);
    
    Partition* allocPartition(unsigned int size, int index);

    MemoryAllocationAlgorithm memoryAllocationAlgorithm;
};

#endif	/* MEMORYMANAGER_H */

