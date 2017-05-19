/*********************************************************************************/
/*!
@file           timemanager

@brief         c++11 time tools class.

@author         StevenZhang

Copyright (c)   2014-2016, zhangliutong&zhengsutec, all rights reserved

This file is C++11 default tools class
 

this is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with PianoBooster.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef _TIMEMANAGER_timemanager_H_
#define _TIMEMANAGER_timemanager_H_
 
#include <ratio>
#include <chrono>
 
class CTimeManager 
{
public:
	static CTimeManager* getInstance() { static CTimeManager* p = new CTimeManager; return p; }
	/*
		1.��ʼ��¼ʱ��,���ص�ǰ΢��ʱ��
	*/
	double	time_begin(){
		using namespace std::chrono;
		time_point<system_clock, milliseconds> timeNow = time_point_cast<milliseconds>(system_clock::now());
		m_beginTime = (long long)timeNow.time_since_epoch().count();

		return m_beginTime;
	}

	/*
		2,��¼����ʱ�䷵�ص�ǰ΢��ʱ�� ����ʱ��-��ʼʱ�����, ����ʱ����
	*/
	double time_end(){
		using namespace std::chrono;
		time_point<system_clock, milliseconds> timeNow = time_point_cast<milliseconds>(system_clock::now());
		m_endTime = (long long)timeNow.time_since_epoch().count();

		return m_endTime;
	}
	/*
		����ʱ��,��ʼ������һ�ε���ʱ�����,������ʱ��update�Ĳ�ֵ
	*/
	void init_time_update(){
		using namespace std::chrono;
		time_point<system_clock, milliseconds> timeNow = time_point_cast<milliseconds>(system_clock::now());
		long long _timenow = (long long)timeNow.time_since_epoch().count();
		m_lastRecordTime = _timenow;
	}
	double time_update(){
		using namespace std::chrono;
		time_point<system_clock, milliseconds> timeNow = time_point_cast<milliseconds>(system_clock::now());
		long long _timenow = (long long)timeNow.time_since_epoch().count();
		m_intervalsTime = _timenow - m_lastRecordTime;
		m_lastRecordTime = _timenow;
		return m_intervalsTime;
	}


	/*
		���ʱ������������Ҫ����һ��timebegin,��ʼ���м����͵���update��������
	*/
	double getIntervalsTime() { return (double)m_intervalsTime; }

	/*
		��õ�ǰ��ʱ��
	*/
	double getCurrentTime(){
		using namespace std::chrono;
		time_point<system_clock, milliseconds> timeNow = time_point_cast<milliseconds>(system_clock::now());
		return (long long)timeNow.time_since_epoch().count();
	}
protected:
	/*��¼��ʼʱ��ϵͳ�Ժ�����м���*/
	long long m_beginTime;
	long long m_endTime;
	/*��һ�μ�¼ʱ�� �Ժ�����м���*/
	long long m_lastRecordTime;
	 
	/*���ʱ�������Ժ�����м��� elapse time*/
	int m_intervalsTime;
};
#endif /*_TIMEMANAGER_timemanager_H_*/