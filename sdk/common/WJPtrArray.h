//Copyright (c) 2012��2015 Beijing TianRuiDiAn Network Technology Co,Ltd. All rights reserved
#pragma once

#ifndef __WJ_PtrArray_H__
#define __WJ_PtrArray_H__
#include <mutex>
#include "common/WJCommonMarco.h"

namespace WJ
{
	///ָ�������ࣨ��ɢ��ʽ��
	template <class ClassType> class CWJPtrArray
	{
		///��������
	private:
		UINT						m_uActiveCount;						///��ǰ��Ч��Ŀ
		UINT						m_uPermitCount;						///���������Ŀ
		ClassType					* * m_pActivePtr;					///��Ч����
		ClassType					* * m_pSleepPtr;					///��Ч����
		ClassType					* m_pClassArray;					///��������
		std::recursive_mutex					m_dataMutex;						//�������������̶߳Ի���������
		///
		///Kylin 20090116 Vip��½����
		UINT						m_uVipReseverCount;						///Vip������½��Ŀ
		///
		///��������
	public:
		///���캯��
		CWJPtrArray();
		///��������
		~CWJPtrArray();

		///���ܺ���
	public:
		///��ʼ��
		bool Init(UINT uPermitCount,UINT uVipReseverCount);
		///ж��
		bool UnInit();
		///��ȡ���ɢ��ʽ��
		ClassType * NewItem();
		///�ͷ����ɢ��ʽ��
		bool FreeItem(ClassType * pItem);
		///��ȡ�˳��ʽ��
		ClassType * NewItem_Order(UINT uIndex);

		///��������
	public:
		///��ȡ��Ч��Ŀ
		UINT GetActiveCount() { return m_uActiveCount; };
		///��ȡ������Ŀ
		UINT GetPermitCount() { return m_uPermitCount; };
		///
		///Kylin 20090116 Vip��½����	
		UINT GetVipReseverCount() { return m_uVipReseverCount; };
		///
		///ͨ���������ȡ������
		ClassType * GetActiveItem(UINT uIndex) { return *(m_pActivePtr+uIndex); };
		///ͨ��������ȡ��ڴ��
		ClassType * GetArrayItem(UINT uIndex) { return (m_pClassArray+uIndex); };
		///ͨ�����ȡ�������ڴ��
		UINT GetItemIndex(ClassType * pFindItem) { return (UINT)(pFindItem-m_pClassArray); };
		///��ȡ�ڴ�����
		ClassType * GetClassArrayPtr() { return m_pClassArray; };
		///��ȡ��
		std::recursive_mutex *GetArrayLock() { return &m_dataMutex; }
	};

	///���캯��
	template <class ClassType> CWJPtrArray<ClassType>::CWJPtrArray()
	{
		m_uPermitCount=0;
		m_uActiveCount=0;
		m_pActivePtr = nullptr;
		m_pSleepPtr = nullptr;
		m_pClassArray = nullptr;
		return;
	}

	///��������
	template <class ClassType> CWJPtrArray<ClassType>::~CWJPtrArray()
	{
		UnInit();
		return;
	}

	///��ʼ������
	///
	///Kylin 20090116 Vip��½����
	template<class ClassType> bool CWJPtrArray<ClassType>::Init(UINT uPermitCount, UINT uVipReseverCount)
	{
		try
		{
			///�����ڴ�
			m_pClassArray=new ClassType [uPermitCount];
			m_pSleepPtr=new ClassType * [uPermitCount];
			m_pActivePtr=new ClassType * [uPermitCount];

			///Ч�����
			if ((m_pClassArray == nullptr) || (m_pSleepPtr == nullptr) || (m_pActivePtr == nullptr))
			{
				UnInit();
				return false;
			}

			///���ò���
			memset(m_pActivePtr,0,sizeof(ClassType *)*uPermitCount);
			for (UINT i=0;i<uPermitCount;i++) *(m_pSleepPtr+i)=m_pClassArray+i;
			m_uActiveCount=0;
			m_uPermitCount=uPermitCount;
			m_uVipReseverCount=uVipReseverCount;
		
	///	
			return true;
		}
		catch (...) 
		{
			//TRACE("CATCH:%s with %s\n",__FILE__,__FUNCTION__);
			UnInit(); 
		}
	
		return false;
	}

	///ж�غ���
	template<class ClassType> bool CWJPtrArray<ClassType>::UnInit()
	{
		WJ_SAFE_DELETE_ARRAY(m_pActivePtr);
		WJ_SAFE_DELETE_ARRAY(m_pSleepPtr);
		WJ_SAFE_DELETE_ARRAY(m_pClassArray);
		m_uPermitCount=0;
		m_uActiveCount=0;
		m_pActivePtr=nullptr;
		m_pSleepPtr=nullptr;

		return true;
	}

	///��ȡ�����ɢ��ʽ��
	template<class ClassType> ClassType * CWJPtrArray<ClassType>::NewItem()
	{
		if (m_uActiveCount<m_uPermitCount)
		{
			*(m_pActivePtr+m_uActiveCount)=*m_pSleepPtr;
			MoveMemory(m_pSleepPtr,m_pSleepPtr+1,(m_uPermitCount-m_uActiveCount-1)*sizeof(ClassType *));
			*(m_pSleepPtr + m_uPermitCount - m_uActiveCount - 1) = nullptr;
			return *(m_pActivePtr+m_uActiveCount++);
		}
		return nullptr;
	}

	///�ͷ����ɢ��ʽ��
	template<class ClassType> bool CWJPtrArray<ClassType>::FreeItem(ClassType * pItem)
	{
		if (pItem != nullptr)
		{
			for (UINT i=0;i<m_uActiveCount;i++)
			{
				if (*(m_pActivePtr+i)==pItem)
				{
					*(m_pSleepPtr+m_uPermitCount-m_uActiveCount)=*(m_pActivePtr+i);
					*(m_pActivePtr + i) = nullptr;
					*(m_pActivePtr+i)=*(m_pActivePtr+(--m_uActiveCount));
					return true;
				}
			}
		}
		return false;
	}
}

#endif//__WJ_PtrArray_H__