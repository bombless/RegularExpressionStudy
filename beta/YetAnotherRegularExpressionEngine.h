#ifndef YetAnotherRegularExpressionEngine_Beta
#define YetAnotherRegularExpressionEngine_Beta
#include "../alpha/YetAnotherRegularExpressionEngine.h"
class FA2{
	/*
	* ���Ӽ����취����һ��DFA
	* �Ӽ����취�������DFA��һ��״̬����Ϊ���ã�ʵ���϶�Ӧ�����Ե�һ���������ԡ�
	* Ҳ���Ƕ�Ӧ��һ��RE�����RE�����ܹ�ƥ����Щƥ�䵽ԭRE��ĳһ������ǰ�沿��
	*/
	struct DFAStatus;
	typedef std::map<char, DFAStatus*> DFAStatusMap;
	struct DFAStatus{
		DFAStatusMap map;
		bool accept;
	};
	/* ������������Ҫ�������ã�Ҳ��Ϊ��֣��ο���ַ [//zh.wikipedia.org/wiki/�ݼ�����] �� */
	;
	/* ������һ��NFA״̬�� */
	typedef std::vector<FA1::Status*> Config;
	/* ���ü� */
	typedef std::vector<Config> ConfigSet;
	class ConfigContainer{
		std::vector<Config*> pool;
	public:
		Config* GetReference(const Config&);
		~ConfigContainer();
	};
	DFAStatus* s0;
	static std::vector<DFAStatus*> GetStatus(DFAStatus*);
	static std::vector<FA1::Status*> GetClosures(const std::vector<FA1::Status*>&);
	static Config Delta(const Config&, char);
	static inline void SwapIfLessThan(FA1::Status*&, FA1::Status*&);
	static inline bool Equals(const Config&, const Config&);
	static bool Contains(const ConfigSet&, const Config&);
	static bool Contains(const std::vector<Config*>&, const Config&);
	static ConfigSet::iterator GetReferrence(ConfigSet&);
	static DFAStatus* ConstructDFA(FA1::Status*);
	static void Free(DFAStatus*);
	static bool ContainsAcceptStatus(const Config&);
public:
	bool operator << (const char*);
	FA2(const char*);
	~FA2();
};
#endif