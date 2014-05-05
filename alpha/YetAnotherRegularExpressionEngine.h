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
	friend FA2;
public:
	/* �����Ĺ��캯��������һ���ַ������õ�һ��״̬�� */
	FA1(const char* RE);
	/* ����������Ҫȷ�����е�״̬���ٱ����ã��ر��ǹ�����̻�����ܶ���ʱ״̬��ҪС�� */
	~FA1();
	/* ����һ���ַ��� */
	bool operator << (const char* tested);
	class SyntaxErrorException :std::exception{};
private:
	/* �˷�ǰ���������⡣���ÿһ��״̬��Ҫָ��������״̬ */
	class Status;
	/* StatusMap�Ǵ洢��״̬�еģ��ɷǿ��ַ�ָ������״̬��ӳ�� */
	typedef std::map<const char, Status*> StatusMap;
	class Status{
		/* ��Ԫ��״̬����Ҫ������Щ״̬���ڲ� */
		friend FA1;
		friend FA2;		
		/* ��״̬�Ŀ��ַ�ӳ�� */
		std::vector<Status*> closure;
		/* �ɷǿ��ַ�ӳ�䵽����״̬ */
		StatusMap map;
		/* �Ƿ���һ������״̬ */
		bool accept;
		/* ����һ����״̬���������κ�ת�� */
		Status();
		/* ���һ�����ɿմ���Ŀ��״̬��ת�� */
		void SetClosure(Status*);
		/* ��ȡһ��״̬�¿��Ծ��ɿմ�ת�Ƶ���״̬�����ӵ������ϲ����� */
		std::vector<Status*> GetClosures(std::vector<FA1::Status*> = std::vector<FA1::Status*>());
		/* ����2��RE */
		Status* Concat(Status*);
		/* ���ֱհ�������"a*"�� */
		Status* Kleene();
		/* ������ʽ�еĻ�(�ֳƹܵ�����) */
		Status* Pipe(Status*);
		/* ��this��ΪNFAͼ����ʼ״̬��������ֹ״̬���һ���ɿմ�ת�Ƶ�Ŀ��״̬��ת�Ʋ�����ԭ��ֹ״̬ */
		Status* ConnectTo(Status* target_status);
		/* ��this��ΪNFAͼ����ʼ״̬��������ֹ״̬���һ���ɿմ�ת�Ƶ�����ָ��״̬��ת�Ʋ�����ԭ��ֹ״̬ */
		Status* ConnectTo(std::vector<Status*> list_of_target_status);
		/* ��this��ΪNFAͼ����ʼ״̬��������ֹ״̬ */
		Status* FindAcceptStatus();
		/* ��this��ΪNFAͼ����ʼ״̬����ȡͼ�ϵ�����״̬ */
		std::vector<Status*> GetStatus();
		Status* DeepCopy();
	};
	struct StatusShadow{
		std::vector<int> closure;
		std::map<char, int> map;
		StatusShadow(std::vector<int> c, std::map<char, int> m)
			:closure(c), map(m){}
	};
	/* ��ʼ״̬��ָ�롣 */
	Status* s0;
	/* ����һ����λ״̬������ֻ��һ��ֻ���ɿմ�ת�Ƶ������״̬ */
	static Status* ConstructUnitAutomachine();
	/* ��Ӧ���ַ�RE��״̬�� */
	static Status* ConstructSingleTransitionAutomachine(char c);
	/* һ�㻯��״̬�� */
	static Status* Constructor(const char*);
	/* �����Լ����ͷź���������һ��ͷ�ڵ����ͷ�����״̬���Ŀռ� */
	static int Free(Status*);
	/* ���ҹܵ����� */
	static int FindPipe(const char* to_search);
};
#endif
