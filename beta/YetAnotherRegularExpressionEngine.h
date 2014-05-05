#ifndef YetAnotherRegularExpressionEngine_Beta
#define YetAnotherRegularExpressionEngine_Beta
#include "../alpha/YetAnotherRegularExpressionEngine.h"
class FA2{
	/*
	* 用子集构造法构造一个DFA
	* 子集构造法构造出的DFA的一个状态（称为配置）实际上对应于语言的一个“子语言”
	* 也就是对应另一个RE，这个RE总是能够匹配那些匹配到原RE的某一个串的前面部分
	*/
	struct DFAStatus;
	typedef std::map<char, DFAStatus*> DFAStatusMap;
	struct DFAStatus{
		DFAStatusMap map;
		bool accept;
	};
	/* 我们在这里主要处理配置（也称为格局，参考网址 [//zh.wikipedia.org/wiki/幂集构造] ） */
	;
	/* 配置是一个NFA状态集 */
	typedef std::vector<FA1::Status*> Config;
	/* 配置集 */
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