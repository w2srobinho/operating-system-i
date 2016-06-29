/*
 * BOOOS.h
 *
 */

#include <iostream>

#include "Queue.h"

namespace BOOOS {


// Queue constructor: must initialize queue's attributes
Queue::Queue() :
        _length(0)
{ }

// Queue destructor: must finalize queue's attributes
Queue::~Queue()
{
    while(_length)
        remove();
}

// insert method: must insert the Element at the end of the queue
// Error messages:
//   01: invalid Element (if elem == 0)
//   02: duplicated Element (if elem already is on the queue)
void Queue::insert(Queue::Element *elem)
{
    if (!elem) {
        std::cerr << "Error 01: invalid Element \n";
        return;
    }

    if (!_length) {
        _head.next(elem);
        _head.prev(elem);
        elem->next(_head.next());
        elem->prev(_head.prev());
        ++_length;
    }

    auto currentElement = &_head;
    for (int i = 0; i < _length; ++i) {
        if (currentElement == elem) {
            std::cerr << "Error 02: duplicated Element \n";
            return;
        }
        currentElement = currentElement->prev();
    }

    _head.prev()->next(elem);
    elem->prev(_head.prev());
    _head.prev(elem);
    _head.prev()->next(_head.next());
    _head.next()->prev(_head.prev());
    ++_length;
}

void Queue::insert_ordered(Element *elem)
{
    if (!elem) {
        std::cerr << "Error 01: invalid Element \n";
        return;
    }
    /*!
     * Insert if hasn't element in queue
     */
    if (!_length) {
        insert(elem);
        return;
    }
    /*!
     * check duplicated
     */
    auto currentElement = &_head;
    for (int i = 0; i < _length; ++i) {
        if (currentElement == elem) {
            std::cerr << "Error 02: duplicated Element \n";
            return;
        }
        currentElement = currentElement->prev();
    }

    if (_length == 1) {
        if (elem->rank() < head()->next()->rank()) {
            elem->next(head()->next());
            elem->prev(head()->next());
            head()->next()->next(elem);
            head()->next()->prev(elem);
            head()->next(elem);
            head()->prev(head()->next()->prev());
            ++_length;
            return;

        } else {
            insert(elem);
            return;
        }
    }
    /**
     * push front
     */
    if (elem->rank() < head()->next()->rank()) {
        elem->next(head()->next());

        elem->prev(head()->prev());
        head()->next()->prev(elem);
        head()->next(elem);
        ++_length;
        return;
    }
    /**
     * push back
     */
    if (elem->rank() >= head()->prev()->rank()) {
        insert(elem);
        return;
    }

    Element *current;
    for (current = head()->next(); current->rank() <= elem->rank(); current = current->next());
    elem->next(current);
    elem->prev(current->prev());
    current->prev()->next(elem);
    current->prev(elem);
    ++_length;
}

// remove: must search and remove the element from the queue
// Error messages:
//   01: queue is empty
Queue::Element* Queue::remove()
{
    if (!_length) {
        std::cerr << "Error 01: queue is empty! \n";
        return nullptr;
    }

    auto currentElement = _head.next();

    if (_length == 1){
        _head.next(nullptr);
        _head.prev(nullptr);
    } else {
        _head.next(_head.next()->next());
        _head.next()->prev(_head.prev());
        _head.prev()->next(_head.next());
    }

    --_length;
    currentElement->next(nullptr);
    currentElement->prev(nullptr);
    return currentElement;
}

void Queue::remove(Element* e)
{
    auto elementFound = search(e);

    if (!elementFound)
        return;

    if(elementFound == _head.next()) {
        remove();
        return;
    }

    if(elementFound == _head.prev()) {
        _head.prev(_head.prev()->prev());
        _head.next()->prev(_head.prev());
        _head.prev()->next(_head.next());
    } else {
        elementFound->prev()->next(elementFound->next());
        elementFound->next()->prev(elementFound->prev());
    }
    --_length;
    elementFound->next(nullptr);
    elementFound->prev(nullptr);
}

Queue::Element* Queue::search(Element* elem)
{
    auto currentElement = &_head;
    for (int i = 0; i < _length; ++i) {
        if (currentElement == elem)
            return elem;

        currentElement = currentElement->prev();
    }
    return nullptr;
}

}
