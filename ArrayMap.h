/*
 * ArrayMap.h
 *
 */

#ifndef ARRAYMAP_H_
#define ARRAYMAP_H_

#include <Arduino.h>

template<typename K, typename V>
class ArrayMap {
public:
	ArrayMap(unsigned int capacity = 150, unsigned int capacityIncrease = 50) {
		keys = NULL;
		values = NULL;
		this->capacity = 0;
		this->capacityIncrease = capacityIncrease;
		position = 0;

		allocateCapacity(capacity);
	}

	~ArrayMap() {
	}

	void allocateCapacity(unsigned int capacity) {
		K *newKeys = new K[capacity];
		V *newValues = new V[capacity];

		if (keys) {
			for (int i = 0; i < position; i++) {
				newKeys[i] = keys[i];
				newValues[i] = values[i];
			}
//            delete keys;
//            delete values;
		}
		keys = newKeys;
		values = newValues;
		this->capacity = capacity;
	}

	unsigned int size() const {
		return position;
	}

	K keyAt(unsigned int idx) {
		return keys[idx];
	}

	V valueAt(unsigned int idx) {
		return values[idx];
	}

	unsigned int indexOf(K key) {
		for (int i = 0; i < position; i++) {
			if (key == keys[i]) {
				return i;
			}
		}
		return -1;
	}

	const V& operator[](const K key) const {
		return operator[](key);
	}

	V& operator[](const K key) {
		int index = indexOf(key);
		if (index != -1) {
			return values[index];
		}
		if (position == capacity) {
			allocateCapacity(capacity + capacityIncrease);
		}
		keys[position] = key;
		values[position] = dummyValue;
		return values[position++];
	}

	void remove(K key) {
		int index = indexOf(key);
		if (index != -1) {
			for (int i = index; i < capacity - 1; i++) {
				keys[i] = keys[i + 1];
				values[i] = values[i + 1];
			}
			position--;
		}
	}

protected:
	int capacity, capacityIncrease, position;
	K *keys;
	V *values;
	V dummyValue;
};
#endif /* ARRAYMAP_H_ */
