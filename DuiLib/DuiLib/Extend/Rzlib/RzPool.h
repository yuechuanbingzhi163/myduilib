#ifndef __RZBUF_H__
#define __RZBUF_H__
// #include <iostream>
// using namespace std;

/*****************************************************************************************************
  本类为资源池类，以链表形式管理，实则栈式，从链表头处插入与移除，
  同时类本身也为一个链表的节点;  

******************************************************************************************************/
namespace RzStd
{
	template <class T>
	struct _Node
	{
		_Node<T> *pNext;
		T data;	
	};

	template <class T,unsigned int nInitCount = 10>
	class RZ_DLL_API CRzPool
	{
	private:
		_Node<T>*   m_pHead;	                 //头结点
		DWORD       m_nOutCount;            //抛出个数
		DWORD       m_nInCount;             //存有个数	
		CRITICAL_SECTION m_lock;
	public:
		CRzPool();
		~CRzPool();
	public:
		T* GetBuf();
		void FreeBuf(T* p,bool bDelete = false);
		void PrintInfo();
	private:
		void DeleteAll_Node();
	};

	/***************************************************************************/
	template <class T,unsigned int nInitCount>
	CRzPool<T,nInitCount>::CRzPool():m_pHead(NULL),m_nOutCount(0),m_nInCount(0)
	{	
		::InitializeCriticalSection(&m_lock);
		for (unsigned int i=0;i<nInitCount;++i)
		{
			_Node<T>* p_Node = new _Node<T>;
			if (p_Node)
			{
				p_Node->pNext = NULL;
				if (m_pHead==NULL)
				{
					m_pHead = p_Node;
				}
				else
				{
					p_Node->pNext = m_pHead;
					m_pHead = p_Node;
				}
				++m_nInCount;
			}
		}
	}

	template <class T,unsigned int nInitCount>
	CRzPool<T,nInitCount>::~CRzPool()
	{
		::EnterCriticalSection(&m_lock);
		if (m_pHead)
		{
			DeleteAll_Node();
		}
		::LeaveCriticalSection(&m_lock);
		//cout<<"对像池, 存个数:"<<m_nInCount<<"抛出个数："<<m_nOutCount<<endl;
		::DeleteCriticalSection(&m_lock);
	}

	template <class T,unsigned int nInitCount>
	T* CRzPool<T,nInitCount>::GetBuf()
	{
		T* pResult =NULL;
		_Node<T>* p_Node = NULL;

		::EnterCriticalSection(&m_lock);
		if (m_pHead)                           //注意不考虑头节点为空的情况
		{                                  //为空时检查申请条件，满足则申请，否刚直接返回NULL
			p_Node = m_pHead;                   //内存释放时直接添加到头节点处,见FreeBuf 函数
			m_pHead = m_pHead->pNext;			
			pResult = &(p_Node->data);
			--m_nInCount;
			++m_nOutCount;
		}
		else 
		{	
			p_Node = new _Node<T>;
			if (p_Node)
			{	
				p_Node->pNext = NULL;
				pResult = &(p_Node->data);	
				++m_nOutCount;
			}
		}
		::LeaveCriticalSection(&m_lock);

		return pResult;
	}

	template <class T,unsigned int nInitCount>
	void CRzPool<T,nInitCount>::FreeBuf(T* p,bool bDelete /* = false */)
	{	
		::EnterCriticalSection(&m_lock);
		{
			_Node<T>* p_Node = (_Node<T>*)((char*)p - sizeof(_Node<T>*));
			if (bDelete)
			{
				delete p_Node;
				p_Node = NULL;
			}
			else
			{	
				p_Node->pNext = m_pHead;
				m_pHead = p_Node;
				++m_nInCount;
			}
			--m_nOutCount;
		}	
		::LeaveCriticalSection(&m_lock);
	}

	template <class T,unsigned int nInitCount>
	void CRzPool<T,nInitCount>::DeleteAll_Node()
	{                                       //本函数类私有，类中其它函数调用（加锁），所以此处不用加锁
		_Node<T>* p_Node = NULL;
		while(1)
		{
			if (!m_pHead) break;
			p_Node = m_pHead->pNext;
			delete m_pHead;
			m_pHead = p_Node;
			--m_nInCount;
		}
	}
};
#endif