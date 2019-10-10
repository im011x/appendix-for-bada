/*
 * FormAxSetupShortcut.cpp
 *
 *  Created on: 2012. 8. 25.
 *      Author: benny
 */

#include "FormAxSetupShortcut.h"
#include "FactoryListItem.h"

FormAxSetupShortcut::FormAxSetupShortcut() : __pList(null), __shortcutOnStartup(0), __shortcutFormID(0)
{
	__pOptionsBtn[0] = null;
	__pOptionsBtn[1] = null;
}

FormAxSetupShortcut::~FormAxSetupShortcut()
{
}

bool FormAxSetupShortcut::AtInitialize()
{
	Form::Construct(FORM_STYLE_INDICATOR | FORM_STYLE_HEADER | FORM_STYLE_FOOTER );

	SetTitle("설정");
	SetPanelStype(BOTTOM_PANEL);

	return true;
}

result FormAxSetupShortcut::OnInitializing(void)
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
	y += 100;
	Panel * pPanelBG2 = makePanel(Rectangle(0,  y, dimBottomSize.width, 200), bottomPanel);
	y += 200;

	//
	makeLabelText(Rectangle(10, 0, w-10, 80), "바로가기", pPanelBG1, 40, Color::COLOR_BLACK, Color(0,0,0,0), ALIGNMENT_LEFT);

	y = 0;
	makeLabelText(Rectangle(10, 0, w-10, 50), "시작하자마자 바로가기를 수행합니다 ", pPanelBG2, 30, Color::COLOR_RED, Color(0,0,0,0), ALIGNMENT_LEFT);
	y += 50+10;
	__pOptionsBtn[0] = makeStateButtonX(Rectangle(20,  y, w-20, 50), "아니오", pPanelBG2, ID_SHORTCUT_DONOT, this, (__shortcutOnStartup == 0));
	y += 50;
	__pOptionsBtn[1] = makeStateButtonX(Rectangle(20, y, w-20, 50), "예", pPanelBG2, ID_SHORTCUT_DO, this, (__shortcutOnStartup == 1));
	y += 50;

	y += 100;
	y += 20;

	makeLabelText(Rectangle(10, y, w-10, 30), "바로가기할 화면을 선택하세요", bottomPanel, 30, Color::COLOR_RED, Color(0,0,0,0), ALIGNMENT_LEFT);
	y += 50;

	// create list view
	__pList = makeListView(Rectangle(0, y, dimBottomSize.width, dimBottomSize.height-y), bottomPanel, (IListViewItemProvider *) this, (IListViewItemEventListener *) this, true);
	__pList->SetTextOfEmptyList("");

	return r;
}

result FormAxSetupShortcut::OnTerminating(void)
{
	save();
	return FormAxSetup::OnTerminating();
}

void FormAxSetupShortcut::OnActionPerformed(const Osp::Ui::Control& source, int actionId)
{
	switch(actionId)
	{
	case ID_SHORTCUT_DO:
		__pOptionsBtn[0]->SetSelection(false);
		__pOptionsBtn[1]->SetSelection(true);
		__shortcutOnStartup = 1;
		break;
	case ID_SHORTCUT_DONOT:
		__pOptionsBtn[0]->SetSelection(true);
		__pOptionsBtn[1]->SetSelection(false);
		__shortcutOnStartup = 0;
		break;
	default:
		FormAxSetup::OnActionPerformed(source, actionId);
		return;
	}

	this->RequestRedraw();
}

void FormAxSetupShortcut::init()
{
	// read setting information form AppRegistry and keep them ...
	RegistryGet("ShortcutOnStartup", __shortcutOnStartup);
	RegistryGet("ShortcutIndex", __shortcutFormID);

	AppLogDebug("load registry value (shortcut start) = %d", __shortcutOnStartup);
	AppLogDebug("load registry value (shortcut index) = %d", __shortcutFormID);
}

void FormAxSetupShortcut::save()
{
	// save the setting information to AppRegistry
	AppLogDebug("save registry value (shortcut start) = %d", __shortcutOnStartup);
	AppLogDebug("save registry value (shortcut index) = %d", __shortcutFormID);

	RegistrySet("ShortcutOnStartup", __shortcutOnStartup);
	RegistrySet("ShortcutIndex", __shortcutFormID);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

int FormAxSetupShortcut::GetItemCount(void)
{
	return 6;
}

static String ArrShortCut[] = {
	"버스 > 내 버스"
	, "버스 > 자주 쓰는"
	, "주소 > 북 마크"
	, "주요 전화"
	, "m 서울"
	, "환율"
};

Osp::Ui::Controls::ListItemBase* FormAxSetupShortcut::CreateItem(int index, int itemWidth)
{
	/*
	String a;

	switch (index) {
	case 0:
		a.Format(100, L"버스 > 내 버스");
		break;
	case 1:
		a.Format(100, L"버스 > 자주 쓰는");
		break;
	case 2:
		a.Format(100, L"주소 > 북 마크");
		break;
	case 3:
		a.Format(100, L"주요전화");
		break;
	case 4:
		a.Format(100, L"m서울");
		break;
	case 5:
		a.Format(100, L"환율");
		break;
	default:
		a.Format(100, L"미지정");
		break;
	}
	 */

	/*
	SimpleItem * pItem = new SimpleItem();
	pItem->Construct(Dimension(itemWidth, 100), LIST_ANNEX_STYLE_MARK);
	pItem->SetElement(a);
	//pItem->SetBackgroundColor(LIST_ITEM_DRAWING_STATUS_NORMAL, Color(0,0,0,0));
	//pItem->SetDescriptionTextColor(Color(0,0,0));
	 */

	CustomItem * pItem = FactoryListItem::makeListItem_String(index, itemWidth, ArrShortCut[index], LIST_ANNEX_STYLE_MARK);

	if (index == __shortcutFormID) {
		__pList->SetItemChecked(index, true);
	}

	return pItem;
}

bool FormAxSetupShortcut::DeleteItem(int index, Osp::Ui::Controls::ListItemBase* pItem, int itemWidth)
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

void FormAxSetupShortcut::OnListViewContextItemStateChanged(Osp::Ui::Controls::ListView &listView, int index, int elementId, Osp::Ui::Controls::ListContextItemStatus state)
{
	AppLogDebug("context menu - element = %d and item = %d", elementId, index);
}

void FormAxSetupShortcut::OnListViewItemStateChanged(Osp::Ui::Controls::ListView &listView, int index, int elementId, Osp::Ui::Controls::ListItemStatus status)
{
	AppLogDebug("index = %d and status = %d", index, status);

	for (int i=0; i<__pList->GetItemCount(); i++) {
		__pList->SetItemChecked(i, false);
	}

	__shortcutFormID = index;
	__pList->SetItemChecked(index, true);
}

void FormAxSetupShortcut::OnListViewItemSwept(Osp::Ui::Controls::ListView &listView, int index, Osp::Ui::Controls::SweepDirection direction)
{
	AppLogDebug("");
}

void FormAxSetupShortcut::OnListViewItemLongPressed(Osp::Ui::Controls::ListView &listView, int index, int elementId, bool& invokeListViewItemCallback)
{
	AppLogDebug("");

	String ttsContents = "바로가기로 지정할 화면은 ";
	ttsContents.Append(ArrShortCut[index]);
	ttsContents.Append(" 입니다.");

	TTS(ttsContents);
}

