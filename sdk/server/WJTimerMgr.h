// �򵥼�ʱ�������ʺϳ�ǧ����ļ�ʱ������. 100 ����ʱ��������٣��ĳ������ʺ�

#pragma once

#include <functional>
#include <thread>
#include <list>
#include <unordered_map>
#include "common/WJSpinlock.h"


namespace WJ
{
    typedef unsigned int TimerID;

	class CallBack_Time
	{
	public:
		///��ʱ����Ϣ ���������أ�
		virtual bool OnTimerMessage(TimerID timerId) = 0;
	};

    struct CWJTimer
    {
        TimerID m_timerID;  // input   ǧ��֤tiemerIDΨһ, ���ظ�
        int     m_interval; // input	ʱ����
		CallBack_Time* m_pCallBack_Time;  //input �ص�����
        time_t  m_deadLine; // interval use
    };
    
    class CWJTimerMgr
    {
    public:
        CWJTimerMgr()
        {
            m_timerIdSeed = 0;
            m_bRunning = true;
            m_timerThread = std::thread(&CWJTimerMgr::ThreadTimerProc, this);
        }
        ~CWJTimerMgr()
        {
            m_bRunning = false;
            m_timerThread.join();
        }
        bool AddTimer(CWJTimer* pTimer);
        bool DelTimer(CWJTimer * pTimer);
		bool DelTimer(TimerID m_timerID);

        int  ProcessTimerEvents();  //���ش���ʱ�¼�����
    private:
        void ThreadTimerProc();

	private:
        std::thread m_timerThread;
        bool m_bRunning;

        TimerID               m_timerIdSeed;

		std::unordered_map<TimerID, CWJTimer>  m_timerid2timers;  //��������ӵļ�ʱ��
        std::list<CWJTimer>   m_timeoutTimers;  //�ѵ��ڵļ�ʱ��
        CSpinlock             m_timersLock;
        
    };
}