#pragma once
#ifndef _EASYSTATICCTRLCONNECT_H_
#define _EASYSTATICCTRLCONNECT_H_
#include "stdafx.h"
#include "resource.h"
#include "DynamicMenu.h"

#include <vector>
#include <iostream>

#define ES_ONLYINPUTNO		0x4000L
#define ES_NOINTABLE		0x8000L
#define ES_ONLYINPUTDECIMAL 0x0100L
#define ES_LIMIT16			0x0002L
#define ES_LIMIT64			0x0004L
#define ES_LIMIT256			0x0008L

typedef struct ManagerData
{
	int ctrlID;
	CString data;
	CString IDName;
	int ctrlName;
}manadata;

struct DataBaseTable
{
	CString tableIDName;
	std::vector<ManagerData> tableData;
};

namespace ControlType
{
	enum C
	{
		Edit = 1,
		ComboBox,
		DateTimePicker,
		MFCMenuButtonControl
	};
	enum D
	{
		NONE = -2,
		BASICINFO = 0,
		CTCHECK,
		PNR,
		OLD,
		EXCHECK,
		SURGERY,
		PATHOLOGY,
		IMMUNOHISTOCHEMISTRY,
		GENETIC,
		FOLLOW_UP_RECORDS
	};
}

struct tag_dataType
{
	CString TableID[10];
	CString TableName[10];
	tag_dataType()
	{
		TableName[0] = L"患者基本信息";
		TableID[0] = L"BASICINFO";
		TableName[1] = L"CT检查";
		TableID[1] = L"CTCHECK";
		TableName[2] = L"肺结节记录";
		TableID[2] = L"PNR";
		TableName[3] = L"肺部其他发现";
		TableID[3] = L"OLD";
		TableName[4] = L"胸部其他检查";
		TableID[4] = L"EXCHECK";
		TableName[5] = L"外科手术";
		TableID[5] = L"SURGERY";
		TableName[6] = L"病理结果";
		TableID[6] = L"PATHOLOGY";
		TableName[7] = L"免疫组化";
		TableID[7] = L"IMMUNOHISTOCHEMISTRY";
		TableName[8] = L"基因检测";
		TableID[8] = L"GENETIC";
		TableName[9] = L"随访记录";
		TableID[9] = L"FOLLOW_UP_RECORDS";
	}
	CString operator[](int _i)const
	{
		return TableID[_i];
	}
	int GetTableIndex(CString str)
	{
		int len = 0;
		CString temp;
		CString term;
		for (int i = 0; i < 10; ++i)
		{
			temp = (*this)[i];
			len = temp.GetLength();
			term = str.Mid(0, len);
			if (temp == term)
				return i;
		}
		return -1;
	}
};

extern struct tag_dataType g_DataType;

struct tag_ef_connect
{
	int staticID = 0;
	CString title;
	int ctrlID = 0;
	int ctrlName;
	CString IDName;
	int attr = 0;
	std::vector<CString> MenuInfo;
	int nTable = 0;
	HMENU hMenu = NULL;
	CDynamicMenu * MFCMenuInfo = nullptr;
	tag_ef_connect() { }
	tag_ef_connect(int _staticID, int _ctrlID, int _ctrlName) :staticID(_staticID), ctrlID(_ctrlID), ctrlName(_ctrlName) { }
	void SetData(int _staticID, int _ctrlID, int _ctrlName){ staticID = _staticID, ctrlID = _ctrlID, ctrlName = _ctrlName; }
	void SetTitle(CString _title){ title = _title; }
	void SetAttr(int _attr){ attr = _attr; }
};

class EasyStaticCtrlConnect
{
private:
	EasyStaticCtrlConnect()
	{
	}
protected:
	std::vector<tag_ef_connect> m_StaticCtrlList;
public:
	friend void __EF__SetStaticCtrlInfo(std::vector<tag_ef_connect> __EF__StaticCtrl);
	static EasyStaticCtrlConnect & GetInstance()
	{
		static EasyStaticCtrlConnect instance;
		return instance;
	}
	static tag_dataType GetTableData()
	{
		static tag_dataType m_TableInfo;
		return m_TableInfo;
	}
public:
	tag_ef_connect GetAt(int index) {
		return m_StaticCtrlList.at(index);
	}
	int GetLength() {
		return int(m_StaticCtrlList.size());
	}
	BOOL GetData(const CString szIDName, tag_ef_connect & outData)
	{
		std::vector<tag_ef_connect>::iterator it;
		for (it = m_StaticCtrlList.begin(); it != m_StaticCtrlList.end(); ++it)
		{
			if (szIDName == it->IDName)
			{
				outData = *it;
				return TRUE;
			}
		}
		return FALSE;
	}
	BOOL GetData(const int nCtrlID, tag_ef_connect & outData)
	{
		std::vector<tag_ef_connect>::iterator it;
		for (it = m_StaticCtrlList.begin(); it != m_StaticCtrlList.end(); ++it)
		{
			if (nCtrlID == it->ctrlID)
			{
				outData = *it;
				return TRUE;
			}
		}
		return FALSE;
	}
	std::vector<tag_ef_connect> GetData() {
		return m_StaticCtrlList;
	}
	std::vector<DataBaseTable> GetDataBaseDataTemplate()
	{
		tag_dataType tableInof;
		std::vector<DataBaseTable> DataBaseInfo;
		DataBaseTable term;
		int m_StaticCtrlList_len = m_StaticCtrlList.size();
		tag_ef_connect temp;
		ManagerData managerData;
		for (int i = 0; i < 10; ++i)
		{
			term.tableIDName = tableInof[i];
			term.tableData.clear();
			managerData.ctrlID = IDC_BASICINFO_MHID;
			managerData.data = L"";
			managerData.IDName = L"BASICINFO_MHID";
			managerData.ctrlName = ControlType::Edit;
			term.tableData.push_back(managerData);
			for (int j = 0; j < m_StaticCtrlList_len; ++j)
			{
				temp = m_StaticCtrlList.at(j);
				if (temp.nTable == i)
				{
					if (temp.IDName == L"BASICINFO_MHID")
						break;
					managerData.ctrlID = temp.ctrlID;
					managerData.data = L"";
					managerData.IDName = temp.IDName;
					managerData.ctrlName = temp.ctrlName;
					term.tableData.push_back(managerData);
				}
			}
			DataBaseInfo.push_back(term);
		}
		return DataBaseInfo;
	}
};

#define CREATE_EASYSTATICCTRLCONNECT struct tag_dataType g_DataType;

#define DECLARE_EASYSTATICCTRLCONNECT_MAP()			\
protected: 											\
	void __EF__ConnectStaticCtrl(); 				\
	void __EF__UpdateStaticPosition(bool bFlag); 	\
	void __EF__UpdateControlStatus();				\

//这一段会出现线程安全问题，不知道如何解决
#define EC___EF__SetStaticCtrlInfo																	\
	void __EF__SetStaticCtrlInfo(std::vector<tag_ef_connect> __EF__StaticCtrl) 						\
	{ 																								\
		static bool flag = true; 																	\
		if(flag == true) 																			\
		{ 																							\
			EasyStaticCtrlConnect &__EF__StaticCtrlInfo = EasyStaticCtrlConnect::GetInstance(); 	\
			__EF__StaticCtrlInfo.m_StaticCtrlList = __EF__StaticCtrl; 								\
			flag = false; 																			\
		} 																							\
	}

#define EC___EF__UpdateStaticPosition(class)																														\
	void class::__EF__UpdateStaticPosition(bool bFlag)																												\
	{ 																																								\
		static std::vector<int> StaticCtrlDistance; 																												\
		CRect StaticRect, ControlRect; 																																\
		std::vector<tag_ef_connect> __EF__StaticCtrl = EasyStaticCtrlConnect::GetInstance().GetData(); 																\
		CWnd * pStatic = nullptr; 																																	\
		CWnd * pCtrl = nullptr; 																																	\
		int nCountor = __EF__StaticCtrl.size(); 																													\
		if (bFlag) 																																					\
		{ 																																							\
			StaticCtrlDistance.clear(); 																															\
			for (int i = 0; i < nCountor; ++i) 																														\
			{ 																																						\
				pStatic = GetDlgItem(__EF__StaticCtrl.at(i).staticID); 																								\
				if (IsWindow(pStatic->m_hWnd)) 																														\
				{ 																																					\
					pStatic->GetWindowRect(StaticRect); 																											\
				} 																																					\
				pCtrl = GetDlgItem(__EF__StaticCtrl.at(i).ctrlID); 																									\
				if (IsWindow(pCtrl->m_hWnd)) 																														\
				{ 																																					\
					pCtrl->GetWindowRect(ControlRect); 																												\
				} 																																					\
				StaticCtrlDistance.push_back(ControlRect.left - StaticRect.left); 																					\
			} 																																						\
		} 																																							\
		else 																																						\
		{ 																																							\
			int wide = 0; 																																			\
			for (int i = 0; i < nCountor; ++i) 																														\
			{ 																																						\
				pStatic = GetDlgItem(__EF__StaticCtrl.at(i).staticID); 																								\
				if (pStatic != nullptr && IsWindow(pStatic->m_hWnd)) 																								\
					pStatic->GetWindowRect(StaticRect); 																											\
				pCtrl = GetDlgItem(__EF__StaticCtrl.at(i).ctrlID); 																									\
				if (pCtrl != nullptr && IsWindow(pCtrl->m_hWnd)) 																									\
					pCtrl->GetWindowRect(ControlRect); 																												\
				ScreenToClient(StaticRect); 																														\
				ScreenToClient(ControlRect); 																														\
				wide = StaticRect.right - StaticRect.left; 																											\
				StaticRect.left = ControlRect.left - StaticCtrlDistance.at(i); 																						\
				StaticRect.right = StaticRect.left + wide; 																											\
				if (pStatic != nullptr && IsWindow(pStatic->m_hWnd)) 																								\
					pStatic->MoveWindow(StaticRect.left, StaticRect.top, StaticRect.right - StaticRect.left, StaticRect.bottom - StaticRect.top, FALSE); 			\
			} 																																						\
		} 																																							\
		Invalidate(); 																																				\
		UpdateWindow(); 																																			\
	}																																								\

#define EC___EF__UpdateControlStatus(class)												\
	void class::__EF__UpdateControlStatus() 											\
	{ 																					\
		EasyStaticCtrlConnect & ControlInfo = EasyStaticCtrlConnect::GetInstance(); 	\
		tag_ef_connect term; 															\
		int i = 0; 																		\
		int len = ControlInfo.GetLength(); 												\
		CEdit * pEdit = nullptr; 														\
		for (i = 0; i < len; ++i) 														\
		{ 																				\
			term = ControlInfo.GetAt(i); 												\
			if (term.ctrlName == ControlType::Edit) 									\
			{ 																			\
				pEdit = (CEdit *)GetDlgItem(term.ctrlID); 								\
				if (term.attr & ES_LIMIT16) { pEdit->LimitText(16); } 					\
				else if (term.attr & ES_LIMIT64) { pEdit->LimitText(64); } 				\
				else if (term.attr & ES_LIMIT256) { pEdit->LimitText(256); } 			\
			} 																			\
		} 																				\
	}																					\

#define EC___EF__FUNCTION(class)			\
	EC___EF__SetStaticCtrlInfo 				\
	EC___EF__UpdateStaticPosition(class) 	\
	EC___EF__UpdateControlStatus(class)		\

#define BEGIN_EASYSTATICCTRLCONNECT_MAP(class)																\
	EC___EF__FUNCTION(class) 																				\
	void class::__EF__ConnectStaticCtrl() 																	\
	{ 																										\
		CEdit * pEdit = nullptr; 																			\
		tag_ef_connect temp; 																				\
		CString str; 																						\
		int nCount; 																						\
		std::vector<tag_ef_connect> __EF__StaticCtrl = EasyStaticCtrlConnect::GetInstance().GetData(); 		\

#define ESCC_CONNECT(staticID, ctrlID, ctrlName, attribute)						\
		temp.SetData(staticID, ctrlID, ctrlName); 								\
		temp.IDName.Format(L""##L#ctrlID); 										\
		temp.IDName = temp.IDName.Mid(4, temp.IDName.GetLength()); 				\
		GetDlgItem(staticID)->GetWindowText(str); 								\
		str.TrimRight(_T("：")); 												\
		temp.SetTitle(str); 													\
		temp.MenuInfo.clear(); 													\
		temp.hMenu = NULL; 														\
		if (ctrlName == ControlType::ComboBox) 									\
		{ 																		\
			nCount = ((CComboBox *)GetDlgItem(ctrlID))->GetCount(); 			\
			for (int i = 0; i < nCount; ++i) 									\
			{ 																	\
				((CComboBox *)GetDlgItem(ctrlID))->GetLBText(i, str); 			\
				temp.MenuInfo.push_back(str); 									\
			} 																	\
		} 																		\
		if (ctrlName == ControlType::MFCMenuButtonControl) 						\
		{ 																		\
			temp.hMenu = HMENU(attribute); 										\
			temp.SetAttr(0); 													\
		} 																		\
		else 																	\
		{ 																		\
			temp.SetAttr(int(attribute)); 										\
		} 																		\
		if (temp.attr & ES_NOINTABLE) 											\
			temp.nTable = ControlType::NONE; 									\
		else 																	\
			temp.nTable = g_DataType.GetTableIndex(temp.IDName); 				\
		ASSERT(temp.nTable != -1); 												\
		__EF__StaticCtrl.push_back(temp);										\

#define END_EASYSTATICCTRLCONNECT_MAP					\
		__EF__SetStaticCtrlInfo(__EF__StaticCtrl); 		\
	}													\

#define ESCC_CONNECT_MFCMENU(staticID, ctrlID, ctrlName, DMenuInfo, o)		\
		temp.SetData(staticID, ctrlID, ctrlName); 							\
		temp.IDName.Format(L""##L#ctrlID); 									\
		temp.IDName = temp.IDName.Mid(4, temp.IDName.GetLength()); 			\
		GetDlgItem(staticID)->GetWindowText(str); 							\
		str.TrimRight(_T("：")); 											\
		temp.SetTitle(str); 												\
		temp.MenuInfo.clear(); 												\
		temp.hMenu = NULL; 													\
		temp.MFCMenuInfo = new CDynamicMenu; 								\
		if (temp.MFCMenuInfo == nullptr) exit(1); 							\
		*(temp.MFCMenuInfo) = DMenuInfo; 									\
		temp.hMenu = temp.MFCMenuInfo->m_hDMenu; 							\
		if ((o) & ES_NOINTABLE) 											\
			temp.nTable = ControlType::NONE; 								\
		else 																\
			temp.nTable = g_DataType.GetTableIndex(temp.IDName); 			\
		ASSERT(temp.nTable != -1); 											\
		__EF__StaticCtrl.push_back(temp);									\
	 
#define CONNECTSTATICCTRL									\
		static bool __EF__hasConnectStaticCtrl = false; 	\
		if (!__EF__hasConnectStaticCtrl) 					\
		{ 													\
			__EF__ConnectStaticCtrl(); 						\
			__EF__hasConnectStaticCtrl = true; 				\
		} 													\
		__EF__UpdateControlStatus(); 						\
		__EF__UpdateStaticPosition(TRUE);					\

#define UPDATECONTROLPOSITION __EF__UpdateStaticPosition(FALSE);	\

#endif // !_EASYSTATICCTRLCONNECT_H_