#include "YetAnotherRegularExpressionEngine.h"
FA1::Status::Status()
/* 简单的制造一个没有任何转移的空状态 */
:closure(), accept(false), map(){}
FA1::Status* FA1::Status::FindAcceptStatus(){
	std::vector<Status*> list = GetStatus();
	for (size_t i = 0; i < list.size(); ++i){
		if (list[i]->accept)return list[i];
	}
	throw("bad!");
}
void FA1::Status::SetClosure(Status* rhs){
	/* 给本状态添加一个经由空串到目标的转移 */
	closure.push_back(rhs);
}
std::vector<FA1::Status*> FA1::Status::GetClosures(std::vector<FA1::Status*> acc){
	/* 若没有空串转移则直接返回 */
	if (closure.size() == 0){
		return closure;
	}
	bool add = false;
	std::vector<Status*> ret;
	/* 将本状态的空串转移状态复制到ret变量 */
	for (size_t i = 0; i < closure.size(); ++i){
		if (std::find(acc.begin(), acc.end(), closure[i]) == acc.end()){
			ret.push_back(closure[i]);
			acc.push_back(closure[i]);
			add = true;
		}
	}
	if (!add)return ret;
	for (size_t i = 0; i < closure.size(); ++i){
		/* 将空串转移状态下的空串转移状态也复制到ret变量 */
		std::vector<Status*> r = closure[i]->GetClosures(acc);
		for (size_t j = 0; j < r.size(); ++j){
			if (r[j] != this){
				ret.push_back(r[j]);
			}
		}
	}
	return ret;
}
FA1::Status* FA1::Status::ConnectTo(Status* s){
	/* 将状态包装到向量里，给另一个重载函数处理 */
	std::vector<Status*> v;
	v.push_back(s);
	return ConnectTo(v);
}
FA1::Status* FA1::Status::ConnectTo(std::vector<Status*> v){
	/* 接受状态经由空串转移到目标 */
	Status* acceptStatus = FindAcceptStatus();
	acceptStatus->accept = false;
	for (size_t i = 0; i < v.size(); ++i){
		acceptStatus->SetClosure(v[i]);
	}
	return acceptStatus;
}
FA1::Status* FA1::Status::Concat(Status* rhs){
	Status* ret = this->DeepCopy();
	Status* retAcceptStatus = ret->FindAcceptStatus();
	Status* rhsCopy = rhs->DeepCopy();
	Status* rhsCopyAcceptStatus = rhsCopy->FindAcceptStatus();
	/* 将左手状态机的接受状态经由空串转移到右手状态机的起始状态 */
	ret->ConnectTo(rhsCopy);
	/* 产物的接受状态不再是接受状态 */
	retAcceptStatus->accept = false;
	/* rhsCopy不再拥有它的那些节点 */
	rhsCopy = 0;
	rhsCopyAcceptStatus = 0;
	return ret;
}
FA1::Status* FA1::Status::Pipe(Status* rhs){
	Status* lhsCopy = this->DeepCopy();
	Status* rhsCopy = rhs->DeepCopy();
	/* 产物自己会有一个起始状态 */
	Status* ret(new Status);
	/* 产物的起始状态经由一个空串转移到左右手两个状态机的起始状态 */
	ret->SetClosure(lhsCopy);
	ret->SetClosure(rhsCopy);
	/* 产物的终止状态是由左右手两个状态机经由一个空串转移过去的 */
	Status* f(new Status);
	f->accept = true;
	/* 为左右手2个状态机的拷贝件添加空串转移，并取消原来的终止状态标记 */
	lhsCopy->ConnectTo(f);
	/* 注意，此处rhsCopy的节点已经被ret共享了 */
	rhsCopy->ConnectTo(f);
	/* 至此rhsCopy与lhsCopy的所有节点已经被ret所持有 */
	lhsCopy = rhsCopy = 0;
	return ret;
}
FA1::Status* FA1::Status::Kleene(){
	Status* s(new Status);
	Status* e(new Status);
	std::vector<Status*> t;
	/* 为状态机的终止状态添加经由空串到新的终止状态和原有的起始状态的转移 */
	Status* lhsCopy = this->DeepCopy();
	t.push_back(lhsCopy);
	t.push_back(e);
	lhsCopy->ConnectTo(t);
	/* e为新的终止状态 */
	e->accept = true;
	/* 为新的起始状态添加经由空串到原起始状态以及新终止状态的转移 */
	s->SetClosure(lhsCopy);
	s->SetClosure(e);
	/* s为新的起始状态 */
	return s;
}
FA1::Status* FA1::Status::DeepCopy(){
	Status* ret = 0;
	const std::vector<Status*> list = this->GetStatus();
	size_t size = list.size();
	std::map<const Status*const, int> mapToInteger;
	/* 我们需要给NFA图造一个影子以便把映射关系刻画出来 */
	std::vector<StatusShadow> listShadow;
	/* 首先给所有的节点编一个号 */
	for (size_t i = 0; i < size; ++i){
		mapToInteger[list[i]] = i;
	}
	for (size_t i = 0; i < size; ++i){
		/* 我们给空串转移造影 */
		std::vector<Status*> iClosures = list[i]->closure;
		std::vector<int> closureShadow;
		for (size_t j = 0; j < iClosures.size(); ++j){
			int index = mapToInteger[iClosures[j]];
			closureShadow.push_back(index);
		}
		/* 给非空字符映射造影 */
		StatusMap iMap = list[i]->map;
		std::map<char, int> mapShadow;
		for (StatusMap::const_iterator it = iMap.begin(); it != iMap.end(); ++it){
			int index = mapToInteger[it->second];
			mapShadow[it->first] = index;
		}
		/* 至此这个节点的影子就造好了 */
		listShadow.push_back(StatusShadow(closureShadow, mapShadow));
	}
	/* 造一个池子，深拷贝出的对象先放池子里 */
	Status **pointerPool = new Status*[size];
	for (size_t i = 0; i < size; ++i){
		pointerPool[i] = new Status;
	}
	/* 需要给拷贝出的NFA图标记终止状态，所以我们要拿到原终止状态的地址 */
	const Status*const acceptStatus = FindAcceptStatus();
	for (size_t i = 0; i < size; ++i){
		/* 装配第i个状态的映射关系 */
		Status& node = *pointerPool[i];
		/* 根据影子构造空字符映射 */
		std::vector<Status*>& nodeClosure = node.closure;
		const std::vector<int>& iClosure = listShadow[i].closure;
		for (size_t j = 0; j < iClosure.size(); ++j){
			int index = iClosure[j];
			nodeClosure.push_back(pointerPool[index]);
		}
		/* 根据影子构造非空字符映射 */
		StatusMap& nodeMap = node.map;
		const std::map<char, int>& iMap = listShadow[i].map;
		for (std::map<char, int>::const_iterator it = iMap.begin();
			it != iMap.end(); ++it){
			nodeMap[it->first] = pointerPool[it->second];
		}
		/* 记录起始状态作为返回值，通常是列表中第一个节点 */
		if (mapToInteger[this] == i){
			ret = &node;
		}
		/* 打终止状态标记 */
		if (mapToInteger[acceptStatus] == i){
			node.accept = true;
		}
	}
	delete pointerPool;
	return ret;
}
FA1::Status* FA1::ConstructUnitAutomachine(){
	/* 因为自身就是接受状态，所以这里就不添加由空串到自身的转移了 */
	Status* ret(new Status);
	ret->accept = true;
	return ret;
}
FA1::Status* FA1::ConstructSingleTransitionAutomachine(char c){
	/* 单字符RE，只需添加一个转移 */
	if (c == '*' || c == '|')return 0;
	Status* s0(new Status);
	Status* sf(new Status);
	sf->accept = true;
	s0->map[c] = sf;
	return s0;
}
FA1::Status* FA1::Constructor(const char* p){
	/* 因为有"a*", "ab*"这样的RE存在，所以这里需要一个空状态机作为递归的终止点 */
	if (*p == '\0'){
		return ConstructUnitAutomachine();
	}
	/*
	*  由于存在"a*|b"这种情况，所以不能只向前看一位（看2位也不行，比如"a|b*"）
	*  我们必须往前找管道符号，通过管道符号来切割RE
	*/
	int f = FindPipe(p);
	if (f < 0){
		/* 假如遇到科林闭包（Kleene closure） */
		if (p[1] == '*'){
			Status* left = ConstructSingleTransitionAutomachine(*p);
			if (!left)return 0;
			Status* right = Constructor(p + 2);
			if (!right){
				Free(left);
				return 0;
			}
			Status* leftKleene = left->Kleene();
			Free(left);
			Status* ret = leftKleene->Concat(right);
			Free(leftKleene);
			Free(right);
			return ret;
		}
		/* 普通情况下只需连接2个状态机 */
		else{
			Status* left = ConstructSingleTransitionAutomachine(*p);
			if (!left)return 0;
			Status* right = Constructor(p + 1);
			if (!right){
				Free(left);
				return 0;
			}
			Status* ret = left->Concat(right);
			Free(left);
			Free(right);
			return ret;
		}
	}
	else{
		/* 有管道符号，需要拷贝出管道前的内容，分别构造状态机 */
		Status* left;
		if (f == 0){
			left = ConstructUnitAutomachine();
		}
		else{
			char *buf = new char[f + 1];
			memcpy(buf, p, f);
			buf[f] = '\0';
			left = Constructor(buf);
			delete buf;
			if (!left){
				return 0;
			}
		}
		Status* right = Constructor(&p[f + 1]);
		if (!right){
			Free(left);
			return 0;
		}
		Status* ret = left->Pipe(right);
		Free(left);
		Free(right);
		return ret;
	}
}
FA1::FA1(const char *p) :s0(Constructor(p)){}
int FA1::Free(Status* s0){
	const std::vector<Status*> p = s0->GetStatus();
	for (size_t i = 0; i < p.size(); ++i){
		delete p[i];
	}
	return p.size();
}
FA1::~FA1(){
	if (s0)Free(s0);
}
int FA1::FindPipe(const char* p){
	/* 查找字符，查不到就返回-1，查到返回偏移 */
	for (int i = 0; p[i] != '\0'; ++i){
		if (p[i] == '|')return i;
	}
	return -1;
}
bool FA1::operator << (const char* pc){
	if (!s0)throw SyntaxErrorException();
	std::vector<Status*> p, waitForAttach;
	/* 我们要跟踪多个执行流，有点复杂 */
	p.push_back(s0);
	/* 找到所有可以经由空串转移到的状态，稍后会跟踪。这相当于是一个等待队列 */
	waitForAttach = s0->GetClosures();
	/* 逐个字符检测 */
	for (; *pc != '\0'; pc += 1){
		/* 将上一阶段找到的空串转移状态添加到追踪流中 */
		for (size_t i = 0; i < waitForAttach.size(); ++i){
			if (std::find(p.begin(), p.end(), waitForAttach[i]) == p.end()){
				p.push_back(waitForAttach[i]);
			}
		}
		/* 清空等待队列，免得重复添加 */
		waitForAttach = std::vector < Status* >();
		for (size_t i = 0; i < p.size(); ++i){
			/* 如果找到一个可行的转移，那么这个流需要转移到下一个状态 */
			if (p[i]->map.find(*pc) != p[i]->map.end()){
				p[i] = p[i]->map[*pc];
				/* 发生一个状态转移后我们需要查找新状态的空串转移状态 */
				/* 避免下一次循环中还有空串转移，我们需要将先接收这些状态再添加到队列 */
				std::vector < Status* > t = p[i]->GetClosures();
				for (size_t j = 0; j < t.size(); ++j){
					waitForAttach.push_back(t[j]);
				}
				/* 已处理完这个流对应当前字符的状态转移，因此该由下一次循环上了 */
				continue;
			}
			else{
				/* 没有可行的转移，如果这是最后一个流，那么RE命中失败 */
				if (p.size() == 1)return false;
				p.erase(std::remove(p.begin(), p.end(), p[i]), p.end());
				i -= 1;
				continue;
			}
		}
	}
	/* 非空字符串意外的终止，说明RE命中失败 */
	if (*pc != '\0')return false;

	/* 输入结束后有流处于接受状态，说明RE命中成功 */
	for (size_t i = 0; i < p.size(); ++i){
		if (p[i]->accept)return true;
	}
	for (size_t i = 0; i < waitForAttach.size(); ++i){
		if (waitForAttach[i]->accept)return true;
	}
	/* 输入意外的结束也意味着RE命中失败 */
	return false;
}
std::vector<FA1::Status*> FA1::Status::GetStatus(){
	std::vector<Status*> list, p, waitForAttach;
	p.push_back(this);
	while (p.size() > 0){
		for (size_t i = 0; i < p.size(); ++i){
			/* 如果节点已经找到过了，就放弃它，从下一个开始 */
			if (std::find(list.begin(), list.end(), p[i]) != list.end()){
				p.erase(std::remove(p.begin(), p.end(), p[i]), p.end());
				i -= 1;
				continue;
			}
			list.push_back(p[i]);
/* 这里有个很重要的假设避免了会导致栈溢出的死循环：Thompson构造法构造出的NFA是没有到自己的转移的 */
			for (size_t j = 0; j < p[i]->closure.size(); ++j){
				waitForAttach.push_back(p[i]->closure[j]);
			}
			for (StatusMap::const_iterator it = p[i]->map.begin();
				it != p[i]->map.end(); ++it){
				waitForAttach.push_back(it->second);
			}
		}
		/* 附上这次找到的状态准备开始下一次搜索 */
		for (size_t i = 0; i < waitForAttach.size(); ++i){
			if (std::find(list.begin(), list.end(), waitForAttach[i]) == list.end()){
				p.push_back(waitForAttach[i]);
			}
		}
	}
	return list;
}
