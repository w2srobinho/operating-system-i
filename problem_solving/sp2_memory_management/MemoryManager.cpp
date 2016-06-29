/*
 * File:   MemoryManager.cpp
 * Authors: Fábio Reina, Felipe Calistro, Willian Souza
 *
 * Created on June 14, 2016, 22h00m
 */


#include "Debug.h"
#include "MemoryManager.h"
#include "Simulator.h"
#include "Traits.h"

#include <iostream>


MemoryManager::MemoryManager(MemoryAllocationAlgorithm algorithm) {
    // INSERT YOUR CODE TO INITIALIZE MEMORY
    memoryAllocationAlgorithm = algorithm;

    partitions = new ListaEncaminhada<Partition *>();

    Partition *initialPartition = new Partition(0, 1e6 -1, true);
    partitions->adiciona(initialPartition);
    
    nextFitIndex = 0;
}

MemoryManager::MemoryManager(const MemoryManager& orig) {
}

MemoryManager::~MemoryManager() {
}

Partition* MemoryManager::allocateMemory(unsigned int size) {
    Debug::cout(Debug::Level::trace, "MemoryManager::allocateMemory(" + std::to_string(size) + ")");
    // INSERT YOUR CODE TO ALLOCATE MEMOTY FOR THE PROCESS
    Partition *allocatedPartition;
    switch(memoryAllocationAlgorithm){
        case FirstFit:  
                        allocatedPartition = firstFit(size);
                        break;
        case BestFit:
                        allocatedPartition = bestFit(size);
                        break;
        case NextFit:
                        allocatedPartition = nextFit(size);
                        break;
        case WorstFit:
                        allocatedPartition = worstFit(size);
                        break;
    }
    return allocatedPartition;
}

void MemoryManager::deallocateMemory(Partition* partition) {
    Debug::cout(Debug::Level::trace, "MemoryManager::deallocateMemory(" + std::to_string(reinterpret_cast<unsigned long> (partition)) + ")");
    // INSERT YOUR CODE TO DEALLOCATE MEMORY OF THE PROCESS
    
    int index = partitions->posicao(partition);
    
    
    if(index != -1){
        Partition *oldPartition = partitions->retiraDaPosicao(index);
        int beginAddress = oldPartition->getBeginAddress();
        int endAddress = oldPartition->getEndAddress();
        int partitionIndex = index;
        Debug::cout(Debug::Level::trace, "BeginAddress: " + std::to_string(beginAddress) + " - EndAddress: " + std::to_string(endAddress) + " - Size: " + std::to_string(oldPartition->getLength()));
        //Junta todas as partições livres somente a partir do elemento removido (era a cabeça da lista)
        if(index == 0){
            bool continuar = true;
            int i = index + 1;
            while(continuar && i < partitions->getSize()){
                Partition *tmp = partitions->consultaInfoPosicao(i);
                if(tmp->isFree()){
                    Partition *old = partitions->retiraDaPosicao(i);
                    endAddress = old->getEndAddress();
                    i++;
                }
                else{
                    continuar = false;
                }
            }
        }
        
        //Junta todas as partições livres anteriores ao elemento removido (era o final da lista)
        else if(index == partitions->getSize() - 1){
            bool continuar = true;
            int i = index - 1;
            while(continuar && i >= 0){
                Partition *tmp = partitions->consultaInfoPosicao(i);
                if(tmp->isFree()){
                    Partition *old = partitions->retiraDaPosicao(i);
                    beginAddress = old->getBeginAddress();
                    partitionIndex = i;
                    i--;
                }
                else{
                    continuar = false;
                }
            }
        }
        
        //Junta todas as partições livres anteriores e posteriores ao elemento removido (era no meio da lista)
        else{
            bool continuar = true;
            int i = index - 1;
            while(continuar && i >= 0){
                Partition *tmp = partitions->consultaInfoPosicao(i);
                if(tmp->isFree()){
                    Partition *old = partitions->retiraDaPosicao(i);
                    beginAddress = old->getBeginAddress();
                    partitionIndex = i;
                    index--;
                    i--;
                }
                else{
                    continuar = false;
                }
            }

            continuar = true;
            i = index;
            while(continuar && i < partitions->getSize()){
                Partition *tmp = partitions->consultaInfoPosicao(i);
                if(tmp->isFree()){
                    Partition *old = partitions->retiraDaPosicao(i);
                    endAddress = old->getEndAddress();
                    i++;
                }
                else{
                    continuar = false;
                }
            }
            
        }
        
        Partition *newPartition = new Partition(beginAddress, endAddress, true);
        partitions->adicionaNaPosicao(newPartition, partitionIndex);
        allocatedPartitions--;
    }
    else{
    }
    
}

unsigned int MemoryManager::getNumPartitions() {
    // INSERT YOUR CODE TO RETURN THE QUANTITY OF ALLOCATED PARTITIONS
    return allocatedPartitions;
}

Partition* MemoryManager::getPartition(unsigned int index) {
    // INSERT YOUR CODE TO RETURN THE PARTITION AT INDEX index
    return partitions->consultaInfoPosicao(index);
}

void MemoryManager::showMemory() {
    // INSERT YOUR CODE TO SHOW THE MEMORY MAP, IN THE FOLLOWING FORMAT
    // <beginAddress>-<endAddress>: <FREE|ALLOCATED> <size>
    
    // Exemplo:
    /*
      0-1499:FREE 1500
      1500-1999:ALLOCATED 500
      2000-2999:ALLOCATED 1000
      3000-9999:FREE 7000
      10000-19999:ALLOCATED 10000
      20000-1000000:FREE 800000
    */

    // INSERT YOUR CODE TO SHOW THE MEMORY MAP
    int sizePartitions = partitions->getSize();
    for(int i = 0; i < sizePartitions; i++){
        Partition *partition = partitions->consultaInfoPosicao(i);
        std::string allocated = (partition->isFree() ? "FREE" : "ALLOCATED");
        std::cout << partition->getBeginAddress() << "-" << partition->getEndAddress() << ": " << allocated << " " << partition->getLength() << std::endl;
    }
}

Partition* MemoryManager::firstFit(unsigned int size){
    int sizePartitions = partitions->getSize();
    for(int i = 0; i < sizePartitions; i++){
        Partition *partition = partitions->consultaInfoPosicao(i);
        if(partition->getLength() >= size && partition->isFree()){
            return allocPartition(size, i);
        }
    }
    
    return nullptr;
}


Partition* MemoryManager::bestFit(unsigned int size){
    int index = -1;
    unsigned int diffSize = 1e6 - 1;
    
    int sizePartitions = partitions->getSize();
    for(int i = 0; i < sizePartitions; i++){
        Partition *partition = partitions->consultaInfoPosicao(i);
        if(partition->getLength() >= size && partition->isFree()){
            unsigned int diff = partition->getLength() - size;
            if(diff <= diffSize){
                diffSize = diff;
                index = i;
            }
        }
    }
    
    if(index != -1){
        return allocPartition(size, index);
    }
    
    return nullptr;
}

Partition* MemoryManager::nextFit(unsigned int size){
    int sizePartitions = partitions->getSize();
    int i = nextFitIndex;
    for(; i < sizePartitions; i++){
        Partition *partition = partitions->consultaInfoPosicao(i);
        if(partition->getLength() >= size && partition->isFree()){
            nextFitIndex = i + 2;
            return allocPartition(size, i);
        }
    }
    i = 0;
    for(; i < nextFitIndex; i++){
        Partition *partition = partitions->consultaInfoPosicao(i);
        if(partition->getLength() >= size && partition->isFree()){
            nextFitIndex = i + 2;
            return allocPartition(size, i);
        }
    }
    
    return nullptr;
}

Partition* MemoryManager::worstFit(unsigned int size){
    int index = -1;
    int diffSize = -1;
    
    int sizePartitions = partitions->getSize();
    for(int i = 0; i < sizePartitions; i++){
        Partition *partition = partitions->consultaInfoPosicao(i);
        if(partition->getLength() >= size && partition->isFree()){
            int diff = partition->getLength() - size;
            if(diff >= diffSize){
                diffSize = diff;
                index = i;
            }
        }
    }
    
    if(index != -1){
        return allocPartition(size, index);
    }
    
    return nullptr;
}

Partition* MemoryManager::allocPartition(unsigned int size, int index){
    unsigned int endAddress = partitions->consultaInfoPosicao(index)->getBeginAddress() + size;
    Partition *oldPartition = partitions->retiraDaPosicao(index);
    Partition *newPartition = new Partition(oldPartition->getBeginAddress(), endAddress - 1, false);
    partitions->adicionaNaPosicao(newPartition, index);
    if(endAddress <= 1e6 - 1){
        partitions->adicionaNaPosicao(new Partition(endAddress, oldPartition->getEndAddress(), true), index+1);
    }
    
    allocatedPartitions++;
    return newPartition;
}

