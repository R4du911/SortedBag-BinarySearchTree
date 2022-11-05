#include "SortedBagIterator.h"
#include "SortedBag.h"
#include <exception>

using namespace std;

//Complexitate:
//best case: Θ(1)
//worst case: Θ(n^2), wegen Selektionsort
//average case: Θ(n^2), wegen Selektionsort
SortedBagIterator::SortedBagIterator(const SortedBag &b) : bag(b) {
    this->stack = new BSTNode[bag.nrElem];
    this->nr_keys = 0;

    //store elements from binary search tree
    for (int index = 0; index < bag.cap; index++) {
        if(bag.elements[index].info.first != NULL_TCOMP){
            stack[this->nr_keys] = bag.elements[index];
            this->nr_keys++;
        }
    }

    //sort element by selectionsort
    for (int index = 0; index < this->nr_keys - 1; index++) {
        int minim = index;
        for (int i = index + 1; i < this->nr_keys; i++)
            if (bag.rel(stack[i].info.first, stack[minim].info.first)) {
                minim = i;
            }
        BSTNode aux = stack[index];
        stack[index] = stack[minim];
        stack[minim] = aux;
    }

    this->currentnode = 0;
    this->currentfrecv = 0;
}


//Complexitate:
//best case: Θ(1)
//worst case: Θ(1)
//average case: Θ(1)
void SortedBagIterator::first() {
    this->currentnode = 0;
    this->currentfrecv = 0;
}


//Complexitate:
//best case: Θ(1)
//worst case: Θ(1)
//average case: Θ(1)
void SortedBagIterator::next() {
    if (valid()) {
        if (this->currentfrecv < stack[this->currentnode].info.second - 1) {
            this->currentfrecv++;
        } else {
            this->currentfrecv = 0;
            this->currentnode++;
        }
    } else {
        throw exception();
    }
}


//Complexitate:
//best case: Θ(1)
//worst case: Θ(1)
//average case: Θ(1)
TComp SortedBagIterator::getCurrent() {
    if (valid()) {
        return this->stack[currentnode].info.first;
    } else {
        throw exception();
    }
}

//Complexitate:
//best case: Θ(1)
//worst case: Θ(1)
//average case: Θ(1)
bool SortedBagIterator::valid() {
    if (this->currentnode < this->nr_keys)
        return true;
    return false;
}


//Complexitate:
//best case: Θ(1)
//worst case: Θ(n^2), wegen Selektionsort
//average case: Θ(n^2), wegen Selektionsort
void SortedBagIterator::remove_current() {

    //search pos of element
    int pos = bag.root;
    int prev_pos;

    while (pos != -1) {
        if (bag.elements[pos].info.first == stack[currentnode].info.first) {
            break;
        }

        if (bag.rel(bag.elements[pos].info.first, stack[currentnode].info.first)) {
            prev_pos = pos;
            pos = bag.elements[pos].right;
        } else {
            prev_pos = pos;
            pos = bag.elements[pos].left;
        }
    }


    //if element has 1 apparition -> element must be removed
    if (bag.elements[pos].info.second == 1) {

        //element has no child
        if (bag.elements[pos].left == -1 && bag.elements[pos].right == -1) {
            bag.elements[pos].info.first = NULL_TCOMP;
            bag.elements[pos].info.second = 0;
            bag.elements[pos].parent = -1;

            if (bag.rel(bag.elements[prev_pos].info.first, stack[currentnode].info.first)) {
                bag.elements[prev_pos].right = -1;
            } else {
                bag.elements[prev_pos].left = -1;
            }


            bag.elements[pos].left = bag.firstEmpty;
            const_cast<SortedBag &>(bag).firstEmpty = pos;
            const_cast<SortedBag &>(bag).nrElem--;
            stack[currentnode].info.first = NULL_TCOMP;
            nr_keys--;
        }

        //element to be removed has one child
        if (bag.elements[pos].left == -1 || bag.elements[pos].right == -1) {
            bag.elements[pos].info.first = NULL_TCOMP;
            bag.elements[pos].info.second = 0;

            int next_pos;
            if (bag.elements[pos].left == -1) {
                next_pos = bag.elements[pos].right;
            } else {
                next_pos = bag.elements[pos].left;
            }

            if (pos == bag.root) {
                bag.elements[pos].left = -1;
                bag.elements[pos].right = -1;
                const_cast<SortedBag &>(bag).root = next_pos;
            } else {
                bag.elements[pos].left = -1;
                bag.elements[pos].right = -1;
                bag.elements[pos].parent = -1;
                bag.elements[next_pos].parent = prev_pos;

                if (bag.rel(bag.elements[prev_pos].info.first, stack[currentnode].info.first)) {
                    bag.elements[prev_pos].right = next_pos;
                } else {
                    bag.elements[prev_pos].left = next_pos;
                }
            }

            bag.elements[pos].left = bag.firstEmpty;
            const_cast<SortedBag &>(bag).firstEmpty = pos;
            const_cast<SortedBag &>(bag).nrElem--;
            stack[currentnode].info.first = NULL_TCOMP;
            nr_keys--;
        }

        //element to be removed has 2 children
        if (bag.elements[pos].left != -1 && bag.elements[pos].right != -1) {

            //we find the next elem
            int successor = bag.elements[pos].right;
            int prev_successor = -1111;

            if (bag.elements[successor].left != -1) {
                while (bag.elements[successor].left != -1) {
                    prev_successor = successor;
                    successor = bag.elements[successor].left;
                }
            }

            //if element to be removed is root
            if (pos == bag.root) {
                bag.elements[pos].info.first = NULL_TCOMP;
                bag.elements[pos].info.second = 0;

                const_cast<SortedBag &>(bag).root = successor;
                bag.elements[bag.root].parent = -1;

                //link left subtree
                bag.elements[bag.root].left = bag.elements[pos].left;
                bag.elements[bag.elements[bag.root].left].parent = successor;
                bag.elements[pos].left = -1;

                //link what the successor had to the right with its former parent
                if (prev_successor != -1111 && bag.elements[successor].right != -1) {
                    bag.elements[prev_successor].left = bag.elements[successor].right;
                    bag.elements[bag.elements[successor].right].parent = prev_successor;
                }

                //link right subtree
                bag.elements[bag.root].right = bag.elements[pos].right;
                bag.elements[bag.elements[bag.root].right].parent = successor;
                bag.elements[pos].right = -1;

            }

            //element to be removed is at a random position
            else {
                bag.elements[pos].info.first = NULL_TCOMP;
                bag.elements[pos].info.second = 0;

                bag.elements[successor].parent = bag.elements[pos].parent;
                bag.elements[pos].parent = -1;

                //link left subtree
                bag.elements[successor].left = bag.elements[pos].left;
                bag.elements[bag.elements[successor].left].parent = successor;
                bag.elements[pos].left = -1;

                //link what the successor had to the right with its former parent
                if (prev_successor != -1111 && bag.elements[successor].right != -1) {
                    bag.elements[prev_successor].left = bag.elements[successor].right;
                    bag.elements[bag.elements[successor].right].parent = prev_successor;
                }

                //link right subtree
                bag.elements[successor].right = bag.elements[pos].right;
                bag.elements[bag.elements[successor].right].parent = successor;
                bag.elements[pos].right = -1;
            }

            bag.elements[pos].left = bag.firstEmpty;
            const_cast<SortedBag &>(bag).firstEmpty = pos;
            const_cast<SortedBag &>(bag).nrElem--;
            stack[currentnode].info.first = NULL_TCOMP;
            nr_keys--;

        }

    }

    //if element has more than 1 apparition
    else {
        bag.elements[pos].info.second--;
        const_cast<SortedBag &>(bag).nrElem--;
    }


    //store elements from binary search tree
    int pos_stack = 0;
    for (int index = 0; index < bag.cap; index++) {
        if(bag.elements[index].info.first != NULL_TCOMP){
            stack[pos_stack] = bag.elements[index];
            pos_stack++;
        }
    }

    //sort element by selectionsort
    for (int index = 0; index < this->nr_keys - 1; index++) {
        int minim = index;
        for (int i = index + 1; i < this->nr_keys; i++)
            if (bag.rel(stack[i].info.first, stack[minim].info.first)) {
                minim = i;
            }
        BSTNode aux = stack[index];
        stack[index] = stack[minim];
        stack[minim] = aux;
    }

}





