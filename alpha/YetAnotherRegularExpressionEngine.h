#ifndef YetAnotherRegularExpressionEngine_Alpha
#define YetAnotherRegularExpressionEngine_Alpha
#include <map>
#include <vector>
#include <algorithm>
#include <memory>
#include <exception>
#include <memory.h>
class FA2;
class FA1{
	friend class FA2;
public:
	/* 公开的构造函数，接受一个字符串，得到一个状态机 */
	FA1(const char* RE);
	/* 析构，这里要确保持有的状态不再被引用，特别是构造过程会产生很多临时状态机要小心 */
	~FA1();
	/* 测试一个字符串 */
	bool operator << (const char* tested);
	class SyntaxErrorException :std::exception{};
private:
	/* 克服前向引用问题。这里，每一个状态需要指向其他的状态 */
	class Status;
	/* StatusMap是存储在状态中的，由非空字符指向其他状态的映射 */
	typedef std::map<const char, Status*> StatusMap;
	class Status{
		/* 友元，状态机需要访问这些状态的内部 */
		friend class FA1;
		friend class FA2;		
		/* 对状态的空字符映射 */
		std::vector<Status*> closure;
		/* 由非空字符映射到其他状态 */
		StatusMap map;
		/* 是否是一个接受状态 */
		bool accept;
		/* 制造一个空状态，它不含任何转移 */
		Status();
		/* 添加一个经由空串到目标状态的转移 */
		void SetClosure(Status*);
		/* 获取一个状态下可以经由空串转移到的状态，附加到参数上并返回 */
		std::vector<Status*>
		GetClosures(std::vector<Status*> = std::vector<Status*>());
		/* 连接2个RE */
		Status* Concat(Status*);
		/* 科林闭包（例如"a*"） */
		Status* Kleene();
		/* 正则表达式中的或(又称管道符号) */
		Status* Pipe(Status*);
		/* 以this作为NFA图的起始状态，
			给其终止状态添加一个由空串转移到目标状态的转移并返回原终止状态 */
		Status* ConnectTo(Status* target_status);
		/* 以this作为NFA图的起始状态，
			给其终止状态添加一个由空串转移到数个指定状态的转移并返回原终止状态 */
		Status* ConnectTo(std::vector<Status*> list_of_target_status);
		/* 以this作为NFA图的起始状态，查找终止状态 */
		Status* FindAcceptStatus();
		/* 以this作为NFA图的起始状态，获取图上的所有状态 */
		std::vector<Status*> GetStatus();
		Status* DeepCopy();
	};
	/* 起始状态的指针。 */
	Status* s0;
	/* 制造一个单位状态机，它只含一个只能由空串转移到自身的状态 */
	static Status* ConstructUnitAutomachine();
	/* 对应单字符RE的状态机 */
	static Status* ConstructSingleTransitionAutomachine(char c);
	/* 一般化的状态机 */
	static Status* Constructor(const char*);
	/* 我们自己的释放函数，传递一个头节点以释放整个状态机的空间 */
	static int Free(Status*);
	/* 查找管道符号 */
	static int FindPipe(const char* to_search);
};
#endif
