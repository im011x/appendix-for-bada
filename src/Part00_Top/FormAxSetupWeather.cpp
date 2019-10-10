/*
 * FormAxSetupWeather.cpp
 *
 *  Created on: 2012. 9. 27.
 *      Author: benny
 */

#include "FormAxSetupWeather.h"
#include "FactoryListItem.h"
#include "HttpKmaWeather.h"

result CompareString::Compare(const Osp::Base::Object& obj1, const Osp::Base::Object& obj2, int& cmp) const
{
	const String * p1 = static_cast <const String*>(&obj1);
	const String * p2 = static_cast <const String*>(&obj2);

/*
	int nCompareType = String::Compare(p1->__routeType, p2->__routeType);
	//int nCompareType = Integer::Compare(reorderType(p1->__routeType), reorderType(p2->__routeType));
	// 노선검색 fast scroll 까지 바꾸어야 한다 ...

	if (nCompareType < 0) {
		cmp = -1;
	}
	else if (nCompareType > 0) {
		cmp = 1;
	}
	else {
		cmp = String::Compare(p1->__busRouteNm, p2->__busRouteNm);
	}
*/

	cmp = p1->CompareTo(*p2);

	return E_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

FormAxSetupWeather::FormAxSetupWeather() : __pList(null)
{
}

FormAxSetupWeather::~FormAxSetupWeather()
{
}

bool FormAxSetupWeather::AtInitialize()
{
	Form::Construct(FORM_STYLE_INDICATOR | FORM_STYLE_HEADER | FORM_STYLE_FOOTER );

	SetTitle("설정");
	SetPanelStype(BOTTOM_PANEL);

	return true;
}

result FormAxSetupWeather::OnInitializing(void)
{
	result r = E_SUCCESS;

	init();

	// TODO: Add your initialization code here
	FormAxSetup::OnInitializing();

	Panel * bottomPanel = GetPanel();
	Osp::Graphics::Dimension dimBottomSize = bottomPanel->GetSize();
	int w = dimBottomSize.width - 20;
	int y = 0;

	Panel * pPanelBG1 = makePanel(Rectangle(0,  y, dimBottomSize.width, 80), bottomPanel, "appendix/bg_white.png");
	y += 80;
	makeLabelText(Rectangle(10, 0, w-10, 40), "날씨 기준 지역", pPanelBG1, 30, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_LEFT);
	makeLabelText(Rectangle(10, 40, w-10, 40), "변경된 내용은 다음번 실행에 반영됩니다.", pPanelBG1, 25, Color::COLOR_GREY,  Color(0,0,0,0), ALIGNMENT_LEFT);

	//
	CompareString * pComparer = new CompareString();
	HttpKmaWeather::__pARRweathers->Sort(*pComparer);
	delete pComparer;

	// create list view
	__pList = makeListView(Rectangle(0, y, dimBottomSize.width, dimBottomSize.height-y), bottomPanel, (IListViewItemProvider *) this, (IListViewItemEventListener *) this, true);
	__pList->SetTextOfEmptyList("날씨 정보를 받아온 이후에 이용하세요");

	return r;
}

result FormAxSetupWeather::OnTerminating(void)
{
	save();
	return FormAxSetup::OnTerminating();
}

void FormAxSetupWeather::init()
{
	// read setting information form AppRegistry and keep them ...
	RegistryGet("WeatherCity", HttpKmaWeather::__sCITYweather);
}

void FormAxSetupWeather::save()
{
	// save the setting information to AppRegistry
	RegistrySet("WeatherCity", HttpKmaWeather::__sCITYweather);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

int FormAxSetupWeather::GetItemCount(void)
{
	if (HttpKmaWeather::__pARRweathers == null) {
		return 0;
	}

	return HttpKmaWeather::__pARRweathers->GetCount();
}

Osp::Ui::Controls::ListItemBase* FormAxSetupWeather::CreateItem(int index, int itemWidth)
{
	/*
	SimpleItem * pItem = new SimpleItem();
	pItem->Construct(Dimension(itemWidth, 100), LIST_ANNEX_STYLE_MARK);
	pItem->SetElement(a);
	//pItem->SetBackgroundColor(LIST_ITEM_DRAWING_STATUS_NORMAL, Color(0,0,0,0));
	//pItem->SetDescriptionTextColor(Color(0,0,0));
	 */

	String * pCityName = static_cast<String*> (HttpKmaWeather::__pARRweathers->GetAt(index));

	CustomItem * pItem = FactoryListItem::makeListItem_String(index, itemWidth, *pCityName, LIST_ANNEX_STYLE_MARK);

	if (*pCityName == HttpKmaWeather::__sCITYweather) {
		__pList->SetItemChecked(index, true);
	}

	return pItem;
}

bool FormAxSetupWeather::DeleteItem(int index, Osp::Ui::Controls::ListItemBase* pItem, int itemWidth)
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

void FormAxSetupWeather::OnListViewContextItemStateChanged(Osp::Ui::Controls::ListView &listView, int index, int elementId, Osp::Ui::Controls::ListContextItemStatus state)
{
	AppLogDebug("context menu - element = %d and item = %d", elementId, index);
}

void FormAxSetupWeather::OnListViewItemStateChanged(Osp::Ui::Controls::ListView &listView, int index, int elementId, Osp::Ui::Controls::ListItemStatus status)
{
	AppLogDebug("index = %d and status = %d", index, status);

	for (int i=0; i<__pList->GetItemCount(); i++) {
		__pList->SetItemChecked(i, false);
	}

	__pList->SetItemChecked(index, true);

	//__shortcutFormID = index;
	String * pCityName = static_cast<String*> (HttpKmaWeather::__pARRweathers->GetAt(index));
	HttpKmaWeather::__sCITYweather = *pCityName;

}

void FormAxSetupWeather::OnListViewItemSwept(Osp::Ui::Controls::ListView &listView, int index, Osp::Ui::Controls::SweepDirection direction)
{
	AppLogDebug("");
}

void FormAxSetupWeather::OnListViewItemLongPressed(Osp::Ui::Controls::ListView &listView, int index, int elementId, bool& invokeListViewItemCallback)
{
//	String ttsContents = "바로가기로 지정할 화면은 ";
//	ttsContents.Append(ArrShortCut[index]);
//	ttsContents.Append(" 입니다.");
//
//	TTS(ttsContents);
}

