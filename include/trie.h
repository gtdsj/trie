#ifndef __TRIE_H_PROTECTED__
#define __TRIE_H_PROTECTED__

#include <stdint.h>
#include <map>

typedef uint16_t trie_char_t;

template <typename T>
class trie
{
public:
	typedef std::map<trie_char_t, trie> states_t;
public:
	trie() :_end(0), _count(0) {  }
	~trie(){}

	void insert(std::string& str, T& data);
	bool find(std::string& str, T& data);

	static uint32_t state_count;
protected:
	uint32_t _end : 1;
	uint32_t _count : 31;
	T _data;
	states_t _states;

public:
	void end(uint8_t e = 0) {
		_end = e;
	}
	bool is_end() {
		return (_end == 1);
	}
	uint32_t inc() {
		if (_count < 0x7fffffff) {
			_count++;
		}
		else {
			throw "count out of range";
		}
		return _count;
	}
	void set_data(T& data) {
		_data = data;
	}
};

template <typename T>
uint32_t trie<T>::state_count = 0;

template <typename T>
void trie<T>::insert(std::string& str, T& data)
{
	trie* cur = this;
	if (str.empty()) {
		return;
	}
	for (std::string::size_type i = 0; i < str.size(); ++i) {
		trie_char_t word = 0;
		if (str[i] & 0x80) {
			word = (*(trie_char_t*)&str[i]);
			i++;
		}
		else {
			word = str[i];
		}
		trie::states_t::iterator it = cur->_states.find(word);
		if (it == cur->_states.end()) {
			state_count++;
			cur->_states.insert(std::pair<trie_char_t, trie>(word, trie()));
			cur = &cur->_states.find(word)->second;
		}
		else {
			cur = &it->second;
		}
	}
	if (cur != this) {
		cur->end(1);
		cur->inc();
		cur->set_data(data);
	}

}

template <typename T>
bool trie<T>::find(std::string& str, T& data)
{
	trie* cur = this;
	if (str.empty()) {
		return false;
	}
	for (std::string::size_type i = 0; i < str.size(); ++i) {
		trie_char_t word = 0;
		if (str[i] & 0x80) {
			word = (*(trie_char_t*)&str[i]);
			i++;
		}
		else {
			word = str[i];
		}
		trie<T>::states_t::iterator it = cur->_states.find(word);
		if (it == cur->_states.end()) {
			return false;
		}
		cur = &it->second;
	}
	if (cur->is_end()) {
		data = cur->_data;
	}
	return true;
}
#endif // !__TRIE_H_PROTECTED__



