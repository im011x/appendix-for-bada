/*
 * FormAxCurrencyNaver.cpp
 *
 *  Created on: 2012. 8. 3.
 *      Author: benny
 */

#include "FormAxCurrencyNaver.h"
#include "FactoryListItem.h"
#include "UtilCode.h"
#include "UtilApp.h"

FormAxCurrencyNaver::FormAxCurrencyNaver() : __pLabelCurrencyDate(null), __pListView(null), __pContextMenu(null), __pHttpCurrency(null), __pHttpCurrencyTime(null), __pArrCurrency(null)
{
}

FormAxCurrencyNaver::~FormAxCurrencyNaver()
{
}

bool FormAxCurrencyNaver::AtInitialize()
{
	Form::Construct(FORM_STYLE_INDICATOR | FORM_STYLE_HEADER);

	SetTitle("환율");

	SetPanelStype(BOTTOM_PANEL);

	SetRefreshButton(ID_UPDATE_CURRENCY);

	return true;

}

result FormAxCurrencyNaver::OnInitializing(void)
{
	result r = E_SUCCESS;

	// TODO: Add your initialization code here
	AppBaseForm::OnInitializing();

	Panel * bottomPanel = GetPanel();
	Osp::Graphics::Dimension dimBottomSize = bottomPanel->GetSize();
	int y = 0;
	int h = 90;

	Panel * panel = makePanel(Rectangle(0, 0, dimBottomSize.width, h), bottomPanel, "appendix/bg_white.png", true);
	//__pLabelCurrencyDate = makeLabelText(Rectangle(10, 0, dimBottomSize.width-20, 60), "환율 검색 시간 : " + UtilApp::GetNowString(), panel, 25, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_LEFT);
	__pLabelCurrencyDate = makeLabelText(Rectangle(10, 0, dimBottomSize.width-20, h),
			"가장 최근의 외환은행 고시 환율 정보입니다.       ",
			panel, 25, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_LEFT);
	__pLabelCurrencyDate->SetWrap();
	y += h;

	__pListView = makeListView(Rectangle(0, y, dimBottomSize.width, dimBottomSize.height-y), bottomPanel, (IListViewItemProvider *) this, (IListViewItemEventListener *) this, true);

	//
	init();
	invokeQuery();

	return r;
}
result FormAxCurrencyNaver::OnTerminating(void)
{
	if (__pHttpCurrency) {
		delete __pHttpCurrency;
	}

	if (__pHttpCurrencyTime) {
		delete __pHttpCurrencyTime;
	}

	if (__pArrCurrency) {
		__pArrCurrency->RemoveAll(true);
		delete __pArrCurrency;
	}

	if (__pContextMenu) {
		delete __pContextMenu;
	}

	return AppBaseForm::OnTerminating();
}

void FormAxCurrencyNaver::init()
{
	if (__pHttpCurrency == null) {
		__pHttpCurrency = new HttpCurrencyNaver(this);
	}

	if (__pHttpCurrencyTime == null) {
		__pHttpCurrencyTime = new HttpCurrencyNaver(this);
	}

	if (__pArrCurrency == null) {
		__pArrCurrency = new ArrayList();
		__pArrCurrency->Construct();
	}

/*
	//
	// 리스트 아이템을 옆으로 밀었을때 나타나는 context menu 생성한다.
	BcContextItem * p1 = new BcContextItem("지도보기", ID_CONTEXT_GET_POSITION);
	BcContextItem * p2 = new BcContextItem("주변정류소", ID_CONTEXT_GET_NEAR_STOP);

	ArrayList * pArrMenu = new ArrayList();
	pArrMenu->Construct();
	pArrMenu->Add(*p1);
	pArrMenu->Add(*p2);

	__pContextMenu = makeContextMenu(pArrMenu);

	pArrMenu->RemoveAll(true);
	delete pArrMenu;
	// 여기까지 context menu
*/

}

void FormAxCurrencyNaver::invokeQuery()
{
	__pLabelCurrencyDate->SetText(__pLabelCurrencyDate->GetText() + " 환율 검색 시간 : " + UtilApp::GetNowString());
	__pLabelCurrencyDate->RequestRedraw();

	if (__pHttpCurrency->GetCurrecny())
		commShow();
}

void FormAxCurrencyNaver::OnActionPerformed(const Osp::Ui::Control& source, int actionId)
{
	switch(actionId)
	{
	case ID_MESSAGEBOX_CONFIRM:
		break;
	case ID_MESSAGEBOX_NO_DATA:
	case ID_MESSAGEBOX_BAD_DATA:
		GotoBack();
		break;
	case ID_UPDATE_CURRENCY:
		invokeQuery();
		break;
	default:
		AppBaseForm::OnActionPerformed(source, actionId);
		break;
	}

}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

void FormAxCurrencyNaver::OnGoodResponse(HandleHttp * pHttp, ByteBuffer* pBuffer)
{
	if (pHttp == __pHttpCurrency->GetHandler())  {
		ByteBuffer * pBufferUtf8 = UtilCode::Euckr_to_Utf8N(pBuffer);
		__pHttpCurrency->ParseCurrecny(pBufferUtf8, __pArrCurrency);
		delete pBufferUtf8;

		// 포기 !!!
		//__pHttpCurrencyTime->GetCurrecnyTime();

		__pListView->UpdateList();
	}

	if (pHttp == __pHttpCurrencyTime->GetHandler())  {
		// 포기한 이유는 그 결과 HTML 파싱이 안된다...
		__pHttpCurrencyTime->ParseCurrecnyTime(pBuffer);
		//__pLabelCurrencyDate->SetText("환율 검색 시간 : " + UtilApp::GetNowString());  // __pHttpCurrencyTime-> 환율검색시간
		//__pLabelCurrencyDate->RequestRedraw();
	}

	commHide();
}


void FormAxCurrencyNaver::OnBadResponse(HandleHttp * pHttp, const String & message)
{
	commHide();
	UtilMessageBox::ConfirmWithCallback("", message, this, ID_MESSAGEBOX_BAD_DATA);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

int FormAxCurrencyNaver::GetItemCount(void)
{
	if (__pArrCurrency == null)
		return 0;

	return __pArrCurrency->GetCount();
}

Osp::Ui::Controls::ListItemBase* FormAxCurrencyNaver::CreateItem(int index, int itemWidth)
{
#if 0
	String a;
	a.Format(100, L"item %d", index + 1);

	SimpleItem * pItem = new SimpleItem();
	pItem->Construct(Dimension(itemWidth, 100), LIST_ANNEX_STYLE_DETAILED);
	pItem->SetElement(a);

	return pItem;
#else
	return FactoryListItem::makeListItem_Currency(index, itemWidth,  static_cast<DataCurrency*> (__pArrCurrency->GetAt(index)));
	//CustomItem * pItem = FactoryListItem::makeListItem_Currency(index, itemWidth,  static_cast<DataCurrency*> (__pArrCurrency->GetAt(index)));
	//pItem->SetContextItem(__pContextMenu);
	//return pItem;
#endif
}

bool FormAxCurrencyNaver::DeleteItem(int index, Osp::Ui::Controls::ListItemBase* pItem, int itemWidth)
{
	delete pItem;
	pItem = null;
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

void FormAxCurrencyNaver::OnListViewContextItemStateChanged(Osp::Ui::Controls::ListView &listView, int index, int elementId, Osp::Ui::Controls::ListContextItemStatus state)
{
	AppLogDebug("context menu - element = %d and item = %d", elementId, index);

}

void FormAxCurrencyNaver::OnListViewItemStateChanged(Osp::Ui::Controls::ListView &listView, int index, int elementId, Osp::Ui::Controls::ListItemStatus status)
{
	AppLogDebug("index = %d and status = %d", index, status);

	DataCurrency* pCurrCurrency = static_cast<DataCurrency*> (__pArrCurrency->GetAt(index));
	SetKeyObject("환율상세_오브젝트", pCurrCurrency);
	GotoForm(FORM_ID_CURRENCY_DETAIL);
}

void FormAxCurrencyNaver::OnListViewItemSwept(Osp::Ui::Controls::ListView &listView, int index, Osp::Ui::Controls::SweepDirection direction)
{
	AppLogDebug("");
}

void FormAxCurrencyNaver::OnListViewItemLongPressed(Osp::Ui::Controls::ListView &listView, int index, int elementId, bool& invokeListViewItemCallback)
{
	DataCurrency* pCurrCurrency = static_cast<DataCurrency*> (__pArrCurrency->GetAt(index));

	String ttsContents = pCurrCurrency->__currencyName;
	ttsContents.Append(" 의 환율 정보 입니다. 매매 기준율은 ");
	ttsContents.Append(pCurrCurrency->__currencyRate[0]);
	ttsContents.Append(" 원 입니다.");

	TTS(ttsContents);

}
