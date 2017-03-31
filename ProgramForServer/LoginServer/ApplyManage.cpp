#include "ApplyManage.h"
#include "common/commonfunction.h"
namespace WJ
{
	CApplayManage::CApplayManage()
	{
		clear();
	}

	CApplayManage::~CApplayManage()
	{
		clear();
	}

	//������м�¼
	void CApplayManage::clear()
	{
		m_datalist.clear();
	}

	//���
	bool CApplayManage::add(struct_applynode &data)
	{
		if (bExist(data.userid))
		{
			return false;
		}
		m_datalist.insert(std::map<long long, struct_applynode>::value_type(data.userid,data));
		return true;
	}

	//ɾ��
	bool CApplayManage::del(UINT userid)
	{
		auto iter = m_datalist.find(userid);
		if(iter != m_datalist.end())
		{
			m_datalist.erase(iter);
			return true;
		}

		return false;
	}

	//�Ƿ����
	bool CApplayManage::bExist(UINT userid)
	{
		auto iter = m_datalist.find(userid);
		if(iter != m_datalist.end())
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	//������ұ����䵽����
	void CApplayManage::AllotRoom(UINT userid, UINT uroomid)
	{
		auto iter = m_datalist.find(userid);
		if(iter != m_datalist.end())
		{
			(*iter).second.bAllot = true;
			(*iter).second.uroomid = uroomid;
		}
	}

	//�������δ�����䵽����
	void CApplayManage::unAllotRoom(UINT userid)
	{
		auto iter = m_datalist.find(userid);
		if (iter != m_datalist.end())
		{
			
			(*iter).second.bAllot = false;
			(*iter).second.uroomid = 0;
		}
	}

	//��øñ����ж����˱�����û�д���
	UINT CApplayManage::getusercount(UINT ucontestid)
	{
		UINT ucount = 0;
		auto iter = m_datalist.begin();
		while (iter != m_datalist.end())
		{
			if (ucontestid == (*iter).second.ucontestid
				&& !(*iter).second.bAllot)
			{
				++ucount;
			}
			++iter;
		}

		return ucount;
	}

	//��ø������ж����˱�����û�д���
	UINT CApplayManage::getusercount_unplaying()
	{
		UINT ucount = 0;
		auto iter = m_datalist.begin();
		while (iter != m_datalist.end())
		{
			if (!(*iter).second.bAllot)
			{
				++ucount;
			}
			++iter;
		}

		return ucount;
	}

	//��ø������ж������Ѿ��ڱ�����
	UINT CApplayManage::getusercount_playing()
	{
		UINT ucount = 0;
		auto iter = m_datalist.begin();
		while (iter != m_datalist.end())
		{
			if ((*iter).second.bAllot)
			{
				++ucount;
			}
			++iter;
		}

		return ucount;
	}

	//�����ұ�������
	bool CApplayManage::at(UINT userid, struct_applynode& data)
	{
		auto iter = m_datalist.find(userid);
		if(iter != m_datalist.end())
		{
			//QK_InfoLog("sdp_test", "hall CApplayManage::at(userid=%d,ucontestid=%d,bAllot=%d)", (*iter).userid, (*iter).ucontestid, (*iter).bAllot, (*iter).uroomid);
			data = (*iter).second;
			return true;
		}
		else
		{
			return false;
		}
	}

}