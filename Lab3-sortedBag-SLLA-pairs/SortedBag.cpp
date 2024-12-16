#include "SortedBag.h"
#include "SortedBagIterator.h"

using namespace std;

SortedBag::SortedBag(Relation r) {
    maxCap = 31;
    firstEmpty = 0;
    head = -1;
    sizeBag = 0;
    nodes = new Node[maxCap];
    rel = r;
    //init nodes array
    for (int i=0; i<maxCap; i++) {
        nodes[i].pair = NULL_TPAIR;
        if (i<maxCap-1) {
            nodes[i].next = i+1;
        } else {
            nodes[i].next = -1;
        }
    }
}

void SortedBag::resize() {
    int oldCap = maxCap;
    maxCap *= 2;
    Node * newNodes = new Node[maxCap]();
    //copy up to oldCap
    for (int i=0; i<oldCap; i++) {
        newNodes[i] = nodes[i];
    }
    //init rest of allocated array-newNodes
    for (int i=oldCap; i<maxCap; i++) {
        newNodes[i].pair = NULL_TPAIR;
        if (i<maxCap-1) {
            newNodes[i].next = i+1;
        } else {
            newNodes[i].next = -1;
        }
    }
    firstEmpty = oldCap;
    delete [] nodes;
    nodes = newNodes;
}

void SortedBag::add(TComp e) {
    if (firstEmpty == -1) { //resize if no spots left
        resize();
    }
    
    //elements exist already sorted in the right spot just increment freq
    int nomad = head;
    while (nomad != -1) {
        if (nodes[nomad].pair.first == e) {
            nodes[nomad].pair.second += 1;
            sizeBag++;
            return;
        }
        nomad = nodes[nomad].next;
    }

    // Step 3: Insert the new element in the correct sorted position
      int newPos = firstEmpty;                    // Allocate a new node
      firstEmpty = nodes[firstEmpty].next;        // Update firstEmpty to the next free node

      nodes[newPos].pair = std::make_pair(e, 1);  // Set the element and its frequency
      nodes[newPos].next = -1;                    // Initialize next as -1 (will be updated)

      // Case 1: Empty list, insert as the head
      if (head == -1) {
          head = newPos;
      }
      // Case 2: Insert before the head
      else if (!rel(nodes[head].pair.first, e)) { // If e should come before head
          nodes[newPos].next = head;              // New node points to current head
          head = newPos;                          // Update head to new node
      }
      // Case 3 and 4: Insert in the middle or at the tail
      else {
          int prev = head;
          nomad = nodes[head].next;

          // Traverse to find the insertion point based on rel
          while (nomad != -1 && rel(nodes[nomad].pair.first, e)) {
              prev = nomad;
              nomad = nodes[nomad].next;
          }

          // Insert after prev
          nodes[newPos].next = nomad;            // New node points to current
          nodes[prev].next = newPos;               // Prev node points to new node
      }
    sizeBag++;
}



bool SortedBag::remove(TElem elem) {
    // Case 1: Empty bag
    if (head == -1) {
        return false;
    }

    int current = head;
    int prev = -1;

    // Find the node with the given element
    while (current != -1 && nodes[current].pair.first != elem) {
        prev = current;
        current = nodes[current].next;
    }

    // Case 2: Element not found
    if (current == -1) {
        return false;
    }

    // We found the element at 'current'
    // Check frequency
    if (nodes[current].pair.second > 1) {
        // Just decrement frequency if more than one occurrence
        nodes[current].pair.second--;
        sizeBag--;
        return true;
    } else {
        // Frequency is exactly 1, remove the node entirely
        sizeBag--;

        // Case 3: Removing the head node
        if (current == head) {
            head = nodes[current].next;
        } else {
            // Case 4: Removing a middle or last node
            nodes[prev].next = nodes[current].next;
        }

        // Add the removed node back to the free list
        nodes[current].pair = NULL_TPAIR;
        nodes[current].next = firstEmpty;
        firstEmpty = current;

        return true;
    }
}


bool SortedBag::search(TComp elem) const {
    int nomad = head;
    while (nomad != -1 && nodes[nomad].pair.first != elem) {
        nomad = nodes[nomad].next;
    }
    if (nomad != -1) {
        return true;
    } else {
        return false;
    }
}


int SortedBag::nrOccurrences(TComp elem) const {
    int nomad = head;
    while (nomad != -1) {
        if (nodes[nomad].pair.first == elem) {
            return nodes[nomad].pair.second;
        }
        nomad = nodes[nomad].next;
    }
	return 0;
}



int SortedBag::size() const {
    return sizeBag;
}


bool SortedBag::isEmpty() const {
    return sizeBag == 0;
}


SortedBagIterator SortedBag::iterator() const {
	return SortedBagIterator(*this);
}


SortedBag::~SortedBag() {
    delete [] nodes;
}
