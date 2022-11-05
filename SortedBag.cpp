#include <iostream>
#include "SortedBag.h"
#include "SortedBagIterator.h"

//Complexitate:
//best case: Θ(n)
//worst case: Θ(n)
//average case: Θ(n)
SortedBag::SortedBag(Relation r) {
    this->cap = 5;
    this->nrElem = 0;
    this->elements = new BSTNode[this->cap];
    this->root = -1;

    for (int index = 0; index < this->cap - 1; index++) {
        this->elements[index].info.first = NULL_TCOMP;
        this->elements[index].left = index + 1;
        this->elements[index].right = -1;
        this->elements[index].parent = -1;
    }

    this->elements[this->cap - 1].info.first = NULL_TCOMP;
    this->elements[this->cap - 1].left = -1;
    this->firstEmpty = 0;
    this->rel = r;
}


//Complexitate:
//best case: Θ(1)
//worst case: Θ(n)
//average case: Θ(logn)
void SortedBag::add(TComp e) {
    bool added = false;

    //resize
    if (this->nrElem == this->cap) {
        this->cap = this->cap * 2;
        BSTNode *newvec = new BSTNode[this->cap];

        for (int index = 0; index < this->nrElem; index++) {
            newvec[index] = this->elements[index];
        }

        for (int index = this->nrElem; index < this->cap - 1; index++) {
            newvec[index].info.first = NULL_TCOMP;
            newvec[index].left = index + 1;
        }

        newvec[this->cap - 1].info.first = NULL_TCOMP;
        newvec[this->cap - 1].left = -1;
        this->firstEmpty = this->nrElem;

        delete[] this->elements;
        this->elements = newvec;
    }

    //add root
    if (this->root == -1) {
        this->root = this->firstEmpty;
        this->elements[this->root].info.first = e;
        this->elements[this->root].info.second = 1;
        this->firstEmpty = this->elements[this->firstEmpty].left;
        this->elements[this->root].left = -1;
        this->elements[this->root].right = -1;
        this->elements[this->root].parent = -1;
        this->nrElem++;
        added = true;
    }

    if (!added) {
        int pos = this->root;
        int prev_pos;

        //go through the tree
        while (pos != -1 && !added) {
            //if element found
            if (this->elements[pos].info.first == e) {
                this->elements[pos].info.second++;
                this->nrElem++;
                added = true;
            }

            if (!this->rel(this->elements[pos].info.first, e)) {
                prev_pos = pos;
                pos = this->elements[pos].left;
            } else {
                prev_pos = pos;
                pos = this->elements[pos].right;
            }
        }

        //if element not found
        if (!added) {
            pos = this->firstEmpty;
            this->elements[pos].info.first = e;
            this->elements[pos].info.second = 1;
            this->firstEmpty = this->elements[this->firstEmpty].left;
            this->elements[pos].left = -1;
            this->elements[pos].right = -1;
            this->elements[pos].parent = prev_pos;

            if (!this->rel(this->elements[prev_pos].info.first, e)) {
                this->elements[prev_pos].left = pos;
            } else {
                this->elements[prev_pos].right = pos;;
            }

            this->nrElem++;
            added = true;
        }
    }
}

//Complexitate:
//best case: Θ(1)
//worst case: Θ(n)
//average case: Θ(logn)
bool SortedBag::remove(TComp e) {

    //tree has only one element left
    if(this->nrElem == 1 && this->elements[this->root].info.first == e){
        //last element has only one apparition -> element must be removed
        if(this->elements[this->root].info.second == 1) {
            this->elements[this->root].info.first = NULL_TCOMP;
            this->elements[this->root].info.second = 0;

            //position is first Empty
            if (this->root < this->firstEmpty) {
                int save_pos = this->firstEmpty;
                this->firstEmpty = this->root;
                this->elements[this->firstEmpty].left = save_pos;
            }
            //removed elements will be linked
            else {
                int index_empty = this->firstEmpty;
                int prev_index_empty;
                while (index_empty < this->root && index_empty != -1) {
                    prev_index_empty = index_empty;
                    index_empty = this->elements[index_empty].left;
                }
                this->elements[prev_index_empty].left = this->root;
                this->elements[this->root].left = index_empty;
            }

            this->root = -1;
            this->nrElem--;
            return true;
        }

        //last element has more than one apparition
        if(this->elements[this->root].info.second > 1){
            this->elements[this->root].info.second--;
            this->nrElem--;
            return true;
        }
    }


    int pos = this->root;
    int prev_pos;
    while (pos != -1) {
        //element found
        if (this->elements[pos].info.first == e) {

            //if element has 1 apparition -> element must be removed
            if (this->elements[pos].info.second == 1) {

                //element has no child
                if(this->elements[pos].left == -1 && this->elements[pos].right == -1){
                    this->elements[pos].info.first = NULL_TCOMP;
                    this->elements[pos].info.second = 0;
                    this->elements[pos].parent = -1;

                    if (this->rel(this->elements[prev_pos].info.first, e)) {
                        this->elements[prev_pos].right = -1;
                    } else {
                        this->elements[prev_pos].left = -1;
                    }

                    //position is first Empty
                    if (pos < this->firstEmpty) {
                        int save_pos = this->firstEmpty;
                        this->firstEmpty = pos;
                        this->elements[this->firstEmpty].left = save_pos;
                    }
                    //removed elements will be linked
                    else {
                        int index_empty = this->firstEmpty;
                        int prev_index_empty;
                        while (index_empty < pos && index_empty != -1) {
                            prev_index_empty = index_empty;
                            index_empty = this->elements[index_empty].left;
                        }
                        this->elements[prev_index_empty].left = pos;
                        this->elements[pos].left = index_empty;
                    }

                    this->nrElem--;
                    return true;
                }

                //element to be removed has one child
                if(this->elements[pos].left == -1 || this->elements[pos].right == -1) {
                    this->elements[pos].info.first = NULL_TCOMP;
                    this->elements[pos].info.second = 0;

                    int next_pos;
                    if (this->elements[pos].left == -1) {
                        next_pos = this->elements[pos].right;
                    } else {
                        next_pos = this->elements[pos].left;
                    }

                    if(pos == this->root){
                        this->elements[pos].left = -1;
                        this->elements[pos].right = -1;
                        this->root = next_pos;
                    }
                    else {
                        this->elements[pos].left = -1;
                        this->elements[pos].right = -1;
                        this->elements[pos].parent = -1;
                        this->elements[next_pos].parent = prev_pos;

                        if (this->rel(this->elements[prev_pos].info.first, e)) {
                            this->elements[prev_pos].right = next_pos;
                        } else {
                            this->elements[prev_pos].left = next_pos;
                        }
                    }

                    //position is first Empty
                    if (pos < this->firstEmpty) {
                        int save_pos = this->firstEmpty;
                        this->firstEmpty = pos;
                        this->elements[firstEmpty].left = save_pos;
                    }
                    //removed elements will be linked
                    else {
                        int index_empty = this->firstEmpty;
                        int prev_index_empty;
                        while (index_empty < pos && index_empty != -1) {
                            prev_index_empty = index_empty;
                            index_empty = this->elements[index_empty].left;
                        }
                        this->elements[prev_index_empty].left = pos;
                        this->elements[pos].left = index_empty;
                    }

                    this->nrElem--;
                    return true;
                }

                //element to be removed has 2 children
                if(this->elements[pos].left != -1 && this->elements[pos].right != -1){

                    //we find the next elem
                    int successor = this->elements[pos].right;
                    int prev_successor= -1111;

                    if(this->elements[successor].left != -1){
                        while(this->elements[successor].left != -1) {
                            prev_successor = successor;
                            successor = this->elements[successor].left;
                        }
                    }

                    //if element to be removed is root
                    if(pos == root){
                        this->elements[pos].info.first = NULL_TCOMP;
                        this->elements[pos].info.second = 0;

                        this->root = successor;
                        this->elements[this->root].parent = -1;

                        //link left subtree
                        this->elements[this->root].left = this->elements[pos].left;
                        this->elements[this->elements[this->root].left].parent = successor;
                        this->elements[pos].left = -1;

                        //link what the successor had to the right with its former parent
                        if(prev_successor != -1111 && this->elements[successor].right != -1){
                            this->elements[prev_successor].left = this->elements[successor].right;
                            this->elements[this->elements[successor].right].parent = prev_successor;
                        }

                        //link right subtree
                        this->elements[this->root].right = this->elements[pos].right;
                        this->elements[this->elements[this->root].right].parent = successor;
                        this->elements[pos].right = -1;

                    }

                    //element to be removed is at a random position
                    else{
                        this->elements[pos].info.first = NULL_TCOMP;
                        this->elements[pos].info.second = 0;

                        this->elements[successor].parent = this->elements[pos].parent;
                        this->elements[pos].parent = -1;

                        //link left subtree
                        this->elements[successor].left = this->elements[pos].left;
                        this->elements[this->elements[successor].left].parent = successor;
                        this->elements[pos].left = -1;

                        //link what the successor had to the right with its former parent
                        if(prev_successor != -1111 && this->elements[successor].right != -1){
                            this->elements[prev_successor].left = this->elements[successor].right;
                            this->elements[this->elements[successor].right].parent = prev_successor;
                        }

                        //link right subtree
                        this->elements[successor].right = this->elements[pos].right;
                        this->elements[this->elements[successor].right].parent = successor;
                        this->elements[pos].right = -1;
                    }

                    //position is first Empty
                    if (pos < this->firstEmpty) {
                        int save_pos = this->firstEmpty;
                        this->firstEmpty = pos;
                        this->elements[firstEmpty].left = save_pos;
                    }
                    //removed elements will be linked
                    else {
                        int index_empty = this->firstEmpty;
                        int prev_index_empty;
                        while (index_empty < pos && index_empty != -1) {
                            prev_index_empty = index_empty;
                            index_empty = this->elements[index_empty].left;
                        }
                        this->elements[prev_index_empty].left = pos;
                        this->elements[pos].left = index_empty;
                    }

                    this->nrElem--;
                    return true;

                }

            }

            //if element has more than 1 apparition
            else{
                this->elements[pos].info.second--;
                this->nrElem--;
                return true;
            }
        }

        if (!this->rel(this->elements[pos].info.first, e)) {
            prev_pos = pos;
            pos = this->elements[pos].left;
        } else {
            prev_pos = pos;
            pos = this->elements[pos].right;
        }
    }

    return false;
}


//Complexitate:
//best case: Θ(1)
//worst case: Θ(n)
//average case: Θ(logn)
bool SortedBag::search(TComp elem) const {
    int pos = this->root;

    while (pos != -1) {
        if (this->elements[pos].info.first == elem) {
            return true;
        }

        if (!this->rel(this->elements[pos].info.first, elem)) {
            pos = this->elements[pos].left;
        } else {
            pos = this->elements[pos].right;
        }
    }

    return false;
}


//Complexitate:
//best case: Θ(1)
//worst case: Θ(n)
//average case: Θ(logn)
int SortedBag::nrOccurrences(TComp elem) const {
    int pos = this->root;

    while (pos != -1) {
        if (this->elements[pos].info.first == elem) {
            return this->elements[pos].info.second;
        }

        if (!this->rel(this->elements[pos].info.first, elem)) {
            pos = this->elements[pos].left;
        } else {
            pos = this->elements[pos].right;
        }
    }

    return 0;
}


//Complexitate:
//best case: Θ(1)
//worst case: Θ(1)
//average case: Θ(1)
int SortedBag::size() const {
    return this->nrElem;
}


//Complexitate:
//best case: Θ(1)
//worst case: Θ(1)
//average case: Θ(1)
bool SortedBag::isEmpty() const {
    if (this->nrElem == 0)
        return true;
    return false;
}


SortedBagIterator SortedBag::iterator() const {
    return SortedBagIterator(*this);
}


//Complexitate:
//best case: Θ(1)
//worst case: Θ(1)
//average case: Θ(1)
SortedBag::~SortedBag() {
    delete[] this->elements;
}
