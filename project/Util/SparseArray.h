
#ifndef ___SPARSE_ARRAY_
#define ___SPARSE_ARRAY_

/**
 * This class works like an array, possibly with
 * multiple empty (NULL) positions.
 */
template<class Tp>
class SparseArray
{
private:

	class Node {
    private:
		Tp value;
	    Node* next;
        int distanceNext;
    protected:

    public:
    	Node(Tp value);
        virtual ~Node();
    	void addValue(Tp v, int distance);
        Tp getValue(int distance);
		int getSumDistances();
        void eraseAllValues();
	};

	bool deleteValues;
	Node* root;

public:

	SparseArray(bool eraseValues = false);
    virtual ~SparseArray();

    Tp get(int pos);
    int getSize();

	void add(Tp node, int pos);

    //void remove(Tp* node, int pos);
    //void removeAll(int pos);

    void clear();

};


//...........................//
// CLASSE SparseArray::Node
//...........................//

template<class Tp>
SparseArray<Tp>::Node::Node(Tp v) {
	this->value = v;
    this->next = NULL;
    this->distanceNext = 0;
}

template<class Tp>
SparseArray<Tp>::Node::~Node() {
	if (next != NULL) {
		delete next;
	}
}

template<class Tp>
void SparseArray<Tp>::Node::addValue(Tp v, int distance) {

	if (distance == 0) {
		this->value = v;

	} else if (distance < this->distanceNext) {
		Node* newNode = new Node(v);
		newNode->next = this->next;
		newNode->distanceNext = this->distanceNext - distance;

		this->next = newNode;
		this->distanceNext = distance;

	} else {
		if (next != NULL) {
			next->addValue(v, distance - this->distanceNext);
		} else {
			this->next = new Node(v);
			this->distanceNext = distance;
		}
	}
}

template<class Tp>
Tp SparseArray<Tp>::Node::getValue(int distance) {
	Tp v = NULL;

	if (distance == 0) {
		v = this->value;

	} else if (distance >= this->distanceNext) {
		if (next != NULL) {
			v = next->getValue(distance - this->distanceNext);
		}
	}

	return v;
}

template<class Tp>
int SparseArray<Tp>::Node::getSumDistances() {
	if (next != NULL) {
		return next->getSumDistances() + this->distanceNext;
	} else {
		return this->distanceNext;
	}
}

template<class Tp>
void SparseArray<Tp>::Node::eraseAllValues() {
	if (this->value != NULL) {
		delete this->value;
	}
	this->value = NULL;
	if (next != NULL) {
		next->eraseAllValues();
	}
}


//.....................//
// CLASSE SparseArray
//.....................//

template<class Tp>
SparseArray<Tp>::SparseArray(bool eraseValues) {
	this->deleteValues = eraseValues;
	this->root = new Node(NULL);
}

template<class Tp>
SparseArray<Tp>::~SparseArray() {
	if (this->deleteValues) {
		root->eraseAllValues();
	}
	delete root;
}

template<class Tp>
Tp SparseArray<Tp>::get(int pos) {
	return root->getValue(pos);
}

template<class Tp>
int SparseArray<Tp>::getSize() {
	return root->getSumDistances();
}

template<class Tp>
void SparseArray<Tp>::add(Tp node, int pos) {
	this->root->addValue(node, pos);
}

template<class Tp>
void SparseArray<Tp>::clear() {
	if (this->deleteValues) {
		root->eraseAllValues();
	}
	delete root;
	this->root = new Node(NULL);
}


#endif
