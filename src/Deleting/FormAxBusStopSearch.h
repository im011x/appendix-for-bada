/*
 * FormAxBusStopSearch.h
 *
 *  Created on: 2012. 7. 1.
 *      Author: benny
 */

#ifndef FORMAXBUSSTOPSEARCH_H_
#define FORMAXBUSSTOPSEARCH_H_

#include "FormAxBus.h"

class FormAxBusStopSearch
	: public FormAxBus
	, public Osp::Ui::Controls::ISearchBarEventListener
	, public Osp::Ui::IKeypadEventListener
{
public:
	FormAxBusStopSearch();
	virtual ~FormAxBusStopSearch();
	virtual bool AtInitialize();

// Implementation
protected:
	const 	static 	int 		ID_SEARCH_OPTION_STOPNAME = 101;
	const 	static 	int 		ID_SEARCH_OPTION_STOPCODE = 102;
	const 	static 	int 		ID_SEARCH_OPTION_DONGNAME = 103;
	const 	static 	int 		ID_SEARCH_OPTION_ZIPCODE = 104;

	const 	static 	int 		ID_SEARCH_WITH_STT = 201;
	const 	static 	int 		ID_SEARCH_DO = 301;

	//
	MyButton * 		__optionBtnStopName;
	MyButton * 		__optionBtnStopCode;
	MyButton * 		__optionBtnDongName;
	MyButton * 		__optionBtnZipCode;

	Osp::Ui::Controls::SearchBar * 		__pSearchBar;

	long 			__howSearchBusStop;

public:
	virtual result OnInitializing(void);
	virtual result OnTerminating(void);
	virtual void OnActionPerformed(const Osp::Ui::Control& source, int actionId);

	// ISearchBarEventListener
	virtual void OnSearchBarModeChanged(Osp::Ui::Controls::SearchBar& source, Osp::Ui::Controls::SearchBarMode mode);

	// IKeypadEventListener
	virtual void OnKeypadActionPerformed(Osp::Ui::Control &source, Osp::Ui::KeypadAction keypadAction);
	virtual void OnKeypadClosed(Osp::Ui::Control &source) {};
	virtual void OnKeypadOpened(Osp::Ui::Control &source) {};
	virtual void OnKeypadWillOpen(Osp::Ui::Control &source) {};

};

#endif /* FORMAXBUSSTOPSEARCH_H_ */
