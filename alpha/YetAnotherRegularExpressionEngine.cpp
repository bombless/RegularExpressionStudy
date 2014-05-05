#include "YetAnotherRegularExpressionEngine.h"
FA1::Status::Status()
/* �򵥵�����һ��û���κ�ת�ƵĿ�״̬ */
:closure(), accept(false), map(){}
FA1::Status* FA1::Status::FindAcceptStatus(){
	std::vector<Status*> list = GetStatus();
	for (size_t i = 0; i < list.size(); ++i){
		if (list[i]->accept)return list[i];
	}
	throw("bad!");
}
void FA1::Status::SetClosure(Status* rhs){
	/* ����״̬���һ�����ɿմ���Ŀ���ת�� */
	closure.push_back(rhs);
}
std::vector<FA1::Status*> FA1::Status::GetClosures(std::vector<FA1::Status*> acc){
	/* ��û�пմ�ת����ֱ�ӷ��� */
	if (closure.size() == 0){
		return closure;
	}
	bool add = false;
	std::vector<Status*> ret;
	/* ����״̬�Ŀմ�ת��״̬���Ƶ�ret���� */
	for (size_t i = 0; i < closure.size(); ++i){
		if (std::find(acc.begin(), acc.end(), closure[i]) == acc.end()){
			ret.push_back(closure[i]);
			acc.push_back(closure[i]);
			add = true;
		}
	}
	if (!add)return ret;
	for (size_t i = 0; i < closure.size(); ++i){
		/* ���մ�ת��״̬�µĿմ�ת��״̬Ҳ���Ƶ�ret���� */
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
	/* ��״̬��װ�����������һ�����غ������� */
	std::vector<Status*> v;
	v.push_back(s);
	return ConnectTo(v);
}
FA1::Status* FA1::Status::ConnectTo(std::vector<Status*> v){
	/* ����״̬���ɿմ�ת�Ƶ�Ŀ�� */
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
	/* ������״̬���Ľ���״̬���ɿմ�ת�Ƶ�����״̬������ʼ״̬ */
	ret->ConnectTo(rhsCopy);
	/* ����Ľ���״̬�����ǽ���״̬ */
	retAcceptStatus->accept = false;
	/* rhsCopy����ӵ��������Щ�ڵ� */
	rhsCopy = 0;
	rhsCopyAcceptStatus = 0;
	return ret;
}
FA1::Status* FA1::Status::Pipe(Status* rhs){
	Status* lhsCopy = this->DeepCopy();
	Status* rhsCopy = rhs->DeepCopy();
	/* �����Լ�����һ����ʼ״̬ */
	Status* ret(new Status);
	/* �������ʼ״̬����һ���մ�ת�Ƶ�����������״̬������ʼ״̬ */
	ret->SetClosure(lhsCopy);
	ret->SetClosure(rhsCopy);
	/* �������ֹ״̬��������������״̬������һ���մ�ת�ƹ�ȥ�� */
	Status* f(new Status);
	f->accept = true;
	/* Ϊ������2��״̬���Ŀ�������ӿմ�ת�ƣ���ȡ��ԭ������ֹ״̬��� */
	lhsCopy->ConnectTo(f);
	/* ע�⣬�˴�rhsCopy�Ľڵ��Ѿ���ret������ */
	rhsCopy->ConnectTo(f);
	/* ����rhsCopy��lhsCopy�����нڵ��Ѿ���ret������ */
	lhsCopy = rhsCopy = 0;
	return ret;
}
FA1::Status* FA1::Status::Kleene(){
	Status* s(new Status);
	Status* e(new Status);
	std::vector<Status*> t;
	/* Ϊ״̬������ֹ״̬��Ӿ��ɿմ����µ���ֹ״̬��ԭ�е���ʼ״̬��ת�� */
	Status* lhsCopy = this->DeepCopy();
	t.push_back(lhsCopy);
	t.push_back(e);
	lhsCopy->ConnectTo(t);
	/* eΪ�µ���ֹ״̬ */
	e->accept = true;
	/* Ϊ�µ���ʼ״̬��Ӿ��ɿմ���ԭ��ʼ״̬�Լ�����ֹ״̬��ת�� */
	s->SetClosure(lhsCopy);
	s->SetClosure(e);
	/* sΪ�µ���ʼ״̬ */
	return s;
}
FA1::Status* FA1::Status::DeepCopy(){
	Status* ret = 0;
	const std::vector<Status*> list = this->GetStatus();
	size_t size = list.size();
	std::map<const Status*const, int> mapToInteger;
	/* ������Ҫ��NFAͼ��һ��Ӱ���Ա��ӳ���ϵ�̻����� */
	std::vector<StatusShadow> listShadow;
	/* ���ȸ����еĽڵ��һ���� */
	for (size_t i = 0; i < size; ++i){
		mapToInteger[list[i]] = i;
	}
	for (size_t i = 0; i < size; ++i){
		/* ���Ǹ��մ�ת����Ӱ */
		std::vector<Status*> iClosures = list[i]->closure;
		std::vector<int> closureShadow;
		for (size_t j = 0; j < iClosures.size(); ++j){
			int index = mapToInteger[iClosures[j]];
			closureShadow.push_back(index);
		}
		/* ���ǿ��ַ�ӳ����Ӱ */
		StatusMap iMap = list[i]->map;
		std::map<char, int> mapShadow;
		for (StatusMap::const_iterator it = iMap.begin(); it != iMap.end(); ++it){
			int index = mapToInteger[it->second];
			mapShadow[it->first] = index;
		}
		/* ��������ڵ��Ӱ�Ӿ������ */
		listShadow.push_back(StatusShadow(closureShadow, mapShadow));
	}
	/* ��һ�����ӣ�������Ķ����ȷų����� */
	Status **pointerPool = new Status*[size];
	for (size_t i = 0; i < size; ++i){
		pointerPool[i] = new Status;
	}
	/* ��Ҫ����������NFAͼ�����ֹ״̬����������Ҫ�õ�ԭ��ֹ״̬�ĵ�ַ */
	const Status*const acceptStatus = FindAcceptStatus();
	for (size_t i = 0; i < size; ++i){
		/* װ���i��״̬��ӳ���ϵ */
		Status& node = *pointerPool[i];
		/* ����Ӱ�ӹ�����ַ�ӳ�� */
		std::vector<Status*>& nodeClosure = node.closure;
		const std::vector<int>& iClosure = listShadow[i].closure;
		for (size_t j = 0; j < iClosure.size(); ++j){
			int index = iClosure[j];
			nodeClosure.push_back(pointerPool[index]);
		}
		/* ����Ӱ�ӹ���ǿ��ַ�ӳ�� */
		StatusMap& nodeMap = node.map;
		const std::map<char, int>& iMap = listShadow[i].map;
		for (std::map<char, int>::const_iterator it = iMap.begin();
			it != iMap.end(); ++it){
			nodeMap[it->first] = pointerPool[it->second];
		}
		/* ��¼��ʼ״̬��Ϊ����ֵ��ͨ�����б��е�һ���ڵ� */
		if (mapToInteger[this] == i){
			ret = &node;
		}
		/* ����ֹ״̬��� */
		if (mapToInteger[acceptStatus] == i){
			node.accept = true;
		}
	}
	delete pointerPool;
	return ret;
}
FA1::Status* FA1::ConstructUnitAutomachine(){
	/* ��Ϊ������ǽ���״̬����������Ͳ�����ɿմ��������ת���� */
	Status* ret(new Status);
	ret->accept = true;
	return ret;
}
FA1::Status* FA1::ConstructSingleTransitionAutomachine(char c){
	/* ���ַ�RE��ֻ�����һ��ת�� */
	if (c == '*' || c == '|')return 0;
	Status* s0(new Status);
	Status* sf(new Status);
	sf->accept = true;
	s0->map[c] = sf;
	return s0;
}
FA1::Status* FA1::Constructor(const char* p){
	/* ��Ϊ��"a*", "ab*"������RE���ڣ�����������Ҫһ����״̬����Ϊ�ݹ����ֹ�� */
	if (*p == '\0'){
		return ConstructUnitAutomachine();
	}
	/*
	*  ���ڴ���"a*|b"������������Բ���ֻ��ǰ��һλ����2λҲ���У�����"a|b*"��
	*  ���Ǳ�����ǰ�ҹܵ����ţ�ͨ���ܵ��������и�RE
	*/
	int f = FindPipe(p);
	if (f < 0){
		/* �����������ֱհ���Kleene closure�� */
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
		/* ��ͨ�����ֻ������2��״̬�� */
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
		/* �йܵ����ţ���Ҫ�������ܵ�ǰ�����ݣ��ֱ���״̬�� */
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
	/* �����ַ����鲻���ͷ���-1���鵽����ƫ�� */
	for (int i = 0; p[i] != '\0'; ++i){
		if (p[i] == '|')return i;
	}
	return -1;
}
bool FA1::operator << (const char* pc){
	if (!s0)throw SyntaxErrorException();
	std::vector<Status*> p, waitForAttach;
	/* ����Ҫ���ٶ��ִ�������е㸴�� */
	p.push_back(s0);
	/* �ҵ����п��Ծ��ɿմ�ת�Ƶ���״̬���Ժ����١����൱����һ���ȴ����� */
	waitForAttach = s0->GetClosures();
	/* ����ַ���� */
	for (; *pc != '\0'; pc += 1){
		/* ����һ�׶��ҵ��Ŀմ�ת��״̬��ӵ�׷������ */
		for (size_t i = 0; i < waitForAttach.size(); ++i){
			if (std::find(p.begin(), p.end(), waitForAttach[i]) == p.end()){
				p.push_back(waitForAttach[i]);
			}
		}
		/* ��յȴ����У�����ظ���� */
		waitForAttach = std::vector < Status* >();
		for (size_t i = 0; i < p.size(); ++i){
			/* ����ҵ�һ�����е�ת�ƣ���ô�������Ҫת�Ƶ���һ��״̬ */
			if (p[i]->map.find(*pc) != p[i]->map.end()){
				p[i] = p[i]->map[*pc];
				/* ����һ��״̬ת�ƺ�������Ҫ������״̬�Ŀմ�ת��״̬ */
				/* ������һ��ѭ���л��пմ�ת�ƣ�������Ҫ���Ƚ�����Щ״̬����ӵ����� */
				std::vector < Status* > t = p[i]->GetClosures();
				for (size_t j = 0; j < t.size(); ++j){
					waitForAttach.push_back(t[j]);
				}
				/* �Ѵ������������Ӧ��ǰ�ַ���״̬ת�ƣ���˸�����һ��ѭ������ */
				continue;
			}
			else{
				/* û�п��е�ת�ƣ�����������һ��������ôRE����ʧ�� */
				if (p.size() == 1)return false;
				p.erase(std::remove(p.begin(), p.end(), p[i]), p.end());
				i -= 1;
				continue;
			}
		}
	}
	/* �ǿ��ַ����������ֹ��˵��RE����ʧ�� */
	if (*pc != '\0')return false;

	/* ����������������ڽ���״̬��˵��RE���гɹ� */
	for (size_t i = 0; i < p.size(); ++i){
		if (p[i]->accept)return true;
	}
	for (size_t i = 0; i < waitForAttach.size(); ++i){
		if (waitForAttach[i]->accept)return true;
	}
	/* ��������Ľ���Ҳ��ζ��RE����ʧ�� */
	return false;
}
std::vector<FA1::Status*> FA1::Status::GetStatus(){
	std::vector<Status*> list, p, waitForAttach;
	p.push_back(this);
	while (p.size() > 0){
		for (size_t i = 0; i < p.size(); ++i){
			/* ����ڵ��Ѿ��ҵ����ˣ��ͷ�����������һ����ʼ */
			if (std::find(list.begin(), list.end(), p[i]) != list.end()){
				p.erase(std::remove(p.begin(), p.end(), p[i]), p.end());
				i -= 1;
				continue;
			}
			list.push_back(p[i]);
/* �����и�����Ҫ�ļ�������˻ᵼ��ջ�������ѭ����Thompson���취�������NFA��û�е��Լ���ת�Ƶ� */
			for (size_t j = 0; j < p[i]->closure.size(); ++j){
				waitForAttach.push_back(p[i]->closure[j]);
			}
			for (StatusMap::const_iterator it = p[i]->map.begin();
				it != p[i]->map.end(); ++it){
				waitForAttach.push_back(it->second);
			}
		}
		/* ��������ҵ���״̬׼����ʼ��һ������ */
		for (size_t i = 0; i < waitForAttach.size(); ++i){
			if (std::find(list.begin(), list.end(), waitForAttach[i]) == list.end()){
				p.push_back(waitForAttach[i]);
			}
		}
	}
	return list;
}
