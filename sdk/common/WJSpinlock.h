#pragma once

#include <atomic>

/*
C++11ʵ��������
��������һ�����ڱ������̹߳�����Դ������
��һ��Ļ�����(mutex)��֮ͬ�����ڵ����������Ի�ȡ��������Ȩʱ����æ�ȴ�(busy waiting)����ʽ���ϵ�ѭ��������Ƿ���á�
�ڶദ���������жԳ�����ʱ��϶̵ĳ�����˵ʹ������������һ��Ļ�������������߳�������ܡ�
*/


namespace WJ
{
	class CSpinlock
	{
	public:
		CSpinlock() 
		{
			m_busy.store(0);
		}
		void Lock();
		void UnLock();
	private:
		std::atomic<int>  m_busy;
	};
}