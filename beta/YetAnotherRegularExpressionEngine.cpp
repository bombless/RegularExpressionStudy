#include "../alpha/YetAnotherRegularExpressionEngine.h"
#include "YetAnotherRegularExpressionEngine.h"
FA2::FA2(const char* re) :s0(ConstructDFA(FA1(re).s0)){}
FA2::~FA2(){
	if (s0)Free(s0);
}
void FA2::Free(FA2::DFAStatus* head){
	std::vector<DFAStatus*> list = GetStatus(head);
	for (size_t i = 0; i < list.size(); ++i){
		delete list[i];
	}
}
std::vector<FA2::DFAStatus*> FA2::GetStatus(DFAStatus* head){
	std::vector<DFAStatus*> ret, p, waitForAttach;
	p.push_back(head);
	while (p.size() > 0){
		for (size_t i = 0; i < p.size(); ++i){
			if (std::find(ret.begin(), ret.end(), p[i]) == ret.end()){
				p.erase(std::remove(p.begin(), p.end(), p[i]), p.end());
				i -= 1;
				continue;
			}
			ret.push_back(p[i]);
			for (DFAStatusMap::const_iterator it = p[i]->map.begin();
				it != p[i]->map.end(); ++it){
				/* 与Thompson构造法造出的NFA不同，这里的DFA是有到自己的转移的，因此需要多检查 */
				if (std::find(ret.begin(), ret.end(), it->second) == ret.end()){
					waitForAttach.push_back(it->second);
				}
			}
		}
		for (size_t i = 0; i < waitForAttach.size(); ++i){
			p.push_back(waitForAttach[i]);
		}
	}
	return ret;
}
std::vector<FA1::Status*> FA2::GetClosures(const std::vector<FA1::Status*>& list){
	std::vector<FA1::Status*> ret = list;
	for (size_t i = 0; i < list.size(); ++i){
		std::vector<FA1::Status*> innerList = list[i]->GetClosures();
		for (size_t i = 0; i < innerList.size(); ++i){
			if (std::find(ret.begin(), ret.end(), innerList[i]) == ret.end()){
				ret.push_back(innerList[i]);
			}
		}
	}
	return ret;
}
bool FA2::operator << (const char* p){
	if (!s0)throw FA1::SyntaxErrorException();
	DFAStatus* cur = s0;
	/* 在DFA中跟踪起来比NFA简单多了。有可行的转移就转移，没有就报失败。 */
	for (; *p != '\0'; ++p){
		if (cur->map.find(*p) != cur->map.end()){
			cur = cur->map[*p];
		}
		else{
			return false;
		}
	}
	return cur->accept;
}
FA2::Config FA2::Delta(const Config& config, char c){
	std::vector<FA1::Status*> ret;
	for (size_t i = 0; i < config.size(); ++i){
		if (config[i]->map.find(c) != config[i]->map.end()){
			FA1::Status* item = config[i]->map[c];
			if (std::find(ret.begin(), ret.end(), item) == ret.end()){
				ret.push_back(item);
			}
		}
	}
	return ret;
}
void FA2::SwapIfLessThan(FA1::Status*& lhs, FA1::Status*& rhs){
	if (lhs < rhs){
		FA1::Status* swap = lhs;
		lhs = rhs;
		rhs = swap;
	}
}
bool FA2::Equals(const Config& lhs, const Config& rhs){
	if (lhs.size() != rhs.size()){
		return false;
	}
	size_t size = lhs.size();
	FA1::Status** lhsArray = new FA1::Status*[size];
	FA1::Status** rhsArray = new FA1::Status*[size];
	for (size_t i = 0; i < size; ++i){
		lhsArray[i] = lhs[i];
		rhsArray[i] = rhs[i];
	}
	for (size_t i = 0; i < size; ++i){
		for (size_t j = 0; j < size; ++j){
			SwapIfLessThan(rhsArray[i], rhsArray[j]);
			SwapIfLessThan(lhsArray[i], lhsArray[j]);
		}
	}
	for (size_t i = 0; i < size; ++i){
		if (lhsArray[i] != rhsArray[i]){
			delete lhsArray;
			delete rhsArray;
			return false;
		}
	}
	delete lhsArray;
	delete rhsArray;
	return true;
}
bool FA2::Contains(const ConfigSet& set, const Config& config){
	for (size_t i = 0; i < set.size(); ++i){
		if (Equals(set[i], config))return true;
	}
	return false;
}
bool FA2::Contains(const std::vector<Config*>& set, const Config& item){
	for (size_t i = 0; i < set.size(); ++i){
		if (Equals(item, *set[i])){
			return true;
		}
	}
	return false;
}
FA2::Config* FA2::ConfigContainer::GetReference(const Config& config){
	for (std::vector<Config*>::iterator it = pool.begin(); it != pool.end(); ++it){
		if (Equals(config, **it)){
			return *it;
		}
	}
	Config* log = new Config(config);
	pool.push_back(log);
	return log;
}
FA2::ConfigContainer::~ConfigContainer(){
	for (size_t i = 0; i < pool.size(); ++i){
		delete pool[i];
	}
}
bool FA2::ContainsAcceptStatus(const Config& config){
	for (size_t i = 0; i < config.size(); ++i){
		if (config[i]->accept)
			return true;
	}
	return false;
}
FA2::DFAStatus* FA2::ConstructDFA(FA1::Status* nfa){
	if (!nfa)return 0;
	Config config0;
	config0.push_back(nfa);
	ConfigContainer configContainer;
	/* product这里我们只是需要构建映射，所以只要做对就可以了 */
	std::vector<Config*> product;
	product.push_back(configContainer.GetReference(config0));
	/* workingSet这里是要测试终止条件的。为了避免死循环还是小心为上。 */
	ConfigSet workingSet;
	workingSet.push_back(GetClosures(config0));
	typedef std::pair<Config*, char> TransitFrom;
	typedef std::map<TransitFrom, Config*> TransitTo;
	TransitTo map;
	while (workingSet.size() > 0){
		Config q = workingSet.back();
		workingSet.pop_back();
		Config* qRef = configContainer.GetReference(q);
		char c = '\0';
		do{
			Config t = GetClosures(Delta(q, c));
			if (t.size() == 0)continue;
			TransitFrom pair(qRef, c);
			Config* tRef = configContainer.GetReference(t);
			map[pair] = tRef;
			if (!Contains(product, t)){
				product.push_back(tRef);
				workingSet.push_back(t);
			}
		} while (++c != '\0');
	}
	size_t size = product.size();
	DFAStatus** arr = new DFAStatus*[size];
	std::map<Config*, int> mapToInteger;
	for (size_t i = 0; i < size; ++i){
		arr[i] = new DFAStatus;
		mapToInteger[product[i]] = i;
		/* DFAStatus的默认构造函数并没有初始化accpet成员，我们在这里一并赋值了 */
		arr[i]->accept = ContainsAcceptStatus(*product[i]);
	}
	for (TransitTo::iterator it = map.begin(); it != map.end(); ++it){
		const TransitFrom& pair(it->first);
		Config* transitFrom = pair.first;
		int indexFrom = mapToInteger[transitFrom];
		DFAStatus* lvalue = arr[indexFrom];
		Config* transitTo = it->second;
		int indexTo = mapToInteger[transitTo];
		DFAStatus* rvalue = arr[indexTo];
		lvalue->map[pair.second] = rvalue;
	}
	DFAStatus* head = *arr;
	delete arr;
	return head;
}